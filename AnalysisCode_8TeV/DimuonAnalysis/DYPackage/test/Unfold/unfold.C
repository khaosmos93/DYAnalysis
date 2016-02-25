//USE source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.30.00/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh version of ROOT or even 5.28
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TMatrixT.h"
#include "TMatrixD.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "TCanvas.h"

#include "TStopwatch.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TF1.h"
#include "TSystem.h"
//set up Roo
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "RooUnfold-1.1.1/src/RooUnfoldSvd.h"
#include "RooUnfold-1.1.1/src/RooUnfoldResponse.h"
#include "TSVDUnfold.h"
#endif

//defaults
//#define USE_OVERFLOWS false
//#define DO_ROO false
//#define DO_BAYES false
//#define FEWZ_CORRECTED true

#define USE_OVERFLOWS false
#define DATA_DRIVEN_BKG false
#define DO_ROO false
#define DO_BAYES true
#define FEWZ_CORRECTED true

using namespace std;

TFile* _inFile;
TFile* _dataFile;
TFile* _outFile;

#include "include/Unfolder.h"
Unfolder* _unfolder;

TH1F* _inTrue = 0;
TH1F* _inMeas= 0;
TH1F* _inMeasMC= 0;
TH1F* _inMeasData= 0;
TCanvas* can = 0;

TH2F* _inMigMat = 0;
TH2F* _inResMat = 0;
TH2F* _inEvMat = 0;

void colNormalize(TH2F* hist);
void rebinWithOU1(TH1F*);
void rebinWithOU2(TH2F*);

void unfold(TString RR){

        bool CLOSURE_TEST = false;

        TString POSTFIX_corr = "";
        if (FEWZ_CORRECTED) POSTFIX_corr = "_corr";

        //SET UP ROO
        gSystem->Load("RooUnfold-1.1.1/libRooUnfold.so");

        if (RR == "POWHEG") {
          CLOSURE_TEST = true;
        }
        _inFile = new TFile("../Inputs/unfold/resMatrixProd_41.root");
        _inTrue = (TH1F*)_inFile->Get("htrue"+POSTFIX_corr);
        _inMeasMC = (TH1F*)_inFile->Get("hmeas"+POSTFIX_corr);
        _inEvMat = (TH2F*)_inFile->Get("hden"+POSTFIX_corr);

        TFile* _inData;
        _inData = new TFile("../Inputs/rawYield/DYspectrum_mass_41.root");
        _inData->cd();
        TH1F* sigNorm = (TH1F*)hsig_fewz->Clone();
        //FIXME commented out for the test 
        _inTrue->Scale(sigNorm->Integral()/_inTrue->Integral()); //10,22));
        _inMeasMC->Scale(sigNorm->Integral()/_inMeasMC->Integral());
        _inEvMat->Scale(sigNorm->Integral()/_inEvMat->Integral());
        //normalizeToPeak(_inTrue,sigNorm);
        //normalizeToPeak(_inMeasMC,sigNorm);
        //normalizeToPeak2D(_inEvMat,sigNorm);
        //_inData->Close();

        _inMigMat = getMigMatrix(_inTrue,_inEvMat); //(TH2F*)_inFile->Get("migMatrix");
        TMatrixD* mat = new TMatrixD(41,41);
        for (int i = 0; i < 41; i++) {
           for (int j = 0; j < 41; j++) {
              mat(i,j) = _inMigMat->GetBinContent(i+1,j+1);
           }
        }
        TMatrixDEigen ME(*mat);
        TMatrixD eigVec= ME.GetEigenVectors();  
        TVectorD eig= ME.GetEigenValues();
        TH1D* h = new TH1D(eig);
        TCanvas* cc = new TCanvas("1","1");
        cc->cd();
        h->GetXaxis()->SetTitle("Eigenvalue number");
        h->GetYaxis()->SetTitle("Eigenvalue");
        h->Draw("hist");
        cc->SaveAs("1.png");

        if (USE_OVERFLOWS && !DO_ROO) _inTrue = rebinWithOU1(_inTrue);
        if (USE_OVERFLOWS && !DO_ROO) _inMeasMC = rebinWithOU1(_inMeasMC);
        if (USE_OVERFLOWS && !DO_ROO) _inMigMat =  rebinWithOU2(_inMigMat);

        _inMeas = (TH1F*)_inMeasMC->Clone("inMeas");
        _inMeasData = (TH1F*)_inMeasMC->Clone("inMeas");

	_inResMat = (TH2F*)_inMigMat->Clone("resMatrix");
	//colNormalize(_inResMat);
	_inMeas->Reset();
      
        //STEP1: subtract the bacgrkounds
        if (!CLOSURE_TEST) { 

          TFile* _inData = new TFile("../Inputs/rawYield/DYspectrum_mass_41.root");
          _inData->cd();
          TH1F* hdata = (TH1F*)gDirectory->Get("hdata");
          TH1F* hqcd = (TH1F*)gDirectory->Get("hqcd");
          TH1F* httbar_mc = (TH1F*)gDirectory->Get("httbar_mc");
          TH1F* hdiboson = (TH1F*)gDirectory->Get("hdiboson");
          TH1F* hDYtautau = (TH1F*)gDirectory->Get("hDYtautau");
          TH1F* hWlepton = (TH1F*)gDirectory->Get("hWlepton");
          TFile* _inAux = new TFile("../Inputs/nonQCD_emu/1D/emu_prediction.root");
          _inAux->cd();          
          TH1F* hnonQCD = (TH1F*)gDirectory->Get("emu_full");
          double background = 0; 

          for( int i=0; i < hdata->GetXaxis()->GetNbins()+1; i++ ) { 
             if (i==0) hqcd->SetBinContent(0,hqcd->GetBinContent(1)); 
             if (!DATA_DRIVEN_BKG) { background = hqcd->GetBinContent(i) + httbar_mc->GetBinContent(i) + hDYtautau->GetBinContent(i) + hWlepton->GetBinContent(i) + hdiboson->GetBinContent(i);
             } else { 
                background = hqcd->GetBinContent(i) + hnonQCD->GetBinContent(i); 
             } 

             _inMeasData->SetBinContent(i, hdata->GetBinContent(i) - background);
             _inMeasData->SetBinError(i, sqrt(pow(hdata->GetBinError(i),2) + pow(hqcd->GetBinError(i),2) + pow(httbar_mc->GetBinError(i),2) + pow(hDYtautau->GetBinError(i),2) + pow(hWlepton->GetBinError(i),2) + pow(hdiboson->GetBinError(i),2)));
           }
           if (USE_OVERFLOWS && !DO_ROO) { 
             _inMeasData = rebinWithOU1(_inMeasData);
           }
        }//not closure test


        cout << "Set up canvas to draw" << endl;
        //
        //SET UP CANVAS TO DRAW
        //
        can = new TCanvas("can","can",800,600);
        gStyle->SetPalette(1);
        can->SetLogy();
        can->SetLogx();

	//_inMeas->Draw();
	//_inTrue->SetLineColor(2);
	//_inTrue->Draw("samehist");
	//_inMeas->Draw();


        //
        //STEP2: SET UP THE UNFOLDER CLASS
        //   
	_unfolder = new Unfolder("badger");
	_unfolder->setTrue(_inTrue);
        if (!CLOSURE_TEST) {  
          _inMeas->Add(_inMeasData);
	  _unfolder->setMeas(_inMeas);
        } else {
          _unfolder->setMeas(_inMeasMC);
        } 
	_unfolder->setMigrationMatrix(_inResMat);
	_unfolder->unfold();
	_unfolder->getUnfolded()->SetLineColor(kRed);//Draw();	
        //get true to draw
        _unfolder->getTrue()->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
        //_unfolder->getTrue()->GetXaxis()->SetTitle("Mass-rapidity index");
        _unfolder->getTrue()->GetYaxis()->SetTitle("Events");
        _unfolder->getTrue()->GetXaxis()->SetMoreLogLabels();
        //_unfolder->getTrue()->SetMarkerStyle(20);
        //_unfolder->getTrue()->SetMarkerSize(2.5);
        //_unfolder->getTrue()->GetXaxis()->SetRangeUser(15,1499);
	_unfolder->getTrue()->Draw("hist");	
        //_unfolder->getMeas()->Draw("hist");
	//_inMeas->Print("all");	

        //
        //STEP3: DO UNFOLDING
        //
        TH1F* crap;
        if (!DO_ROO) {        
	  crap = (TH1F*)_unfolder->getUnfolded();//->Clone("hunfolded");//Draw();	
	  _htrue = (TH1F*)_unfolder->getTrue();
         
	  //crap->Print("all");	
        } else {
          if (!DO_BAYES) {
             cout << "Starting Roo unfold" << endl;
             cout << _inMeasMC->Integral() << " " << _inTrue->Integral() << " " << _inEvMat->Integral() << endl;
             //RooUnfoldResponse response(_inMeasMC,_inTrue,_inEvMat); //_h2_tmp); //_inResMat);
             TSVDUnfold *tsvdunf = new TSVDUnfold((TH1D*)_inMeasMC,(TH1D*)_inMeasMC, (TH1D*)_inTrue, (TH2D*)_inEvMat);
          } else {
             RooUnfoldResponse response(_inMeasMC,_inTrue,_inResMat);
          }
          if (USE_OVERFLOWS) response.UseOverflow();
          if (CLOSURE_TEST) {
             if (DO_BAYES) { 
                RooUnfoldBayes unfoldBayes(&response, _inMeasMC, 1);
             } else {
                cout << "Do unfolding SVD" << endl;
                //RooUnfoldSvd unfoldSVD(&response, _inMeasMC, 2);
             }
          } else  {
             if (DO_BAYES) { 
                RooUnfoldBayes unfoldBayes(&response, _inMeasData, 1);
             } else {
                //RooUnfoldSvd unfoldSVD(&response, _inMeasData, 2);
             }
          }
          if (DO_BAYES) { 
              crap = (TH1F*) unfoldBayes.Hreco();
          } else { 
              //crap = (TH1F*) unfoldSVD.Hreco(); //analog of getUnfolded() in unfolder class
              crap = (TH1F*)tsvdunf->Unfold(13);
          }
        }
        cout << "Normalization check, unfolded: " << crap->Integral() << endl;

        crap->SetMarkerStyle(20);
        crap->SetMarkerColor(kRed);
        crap->SetLineColor(kRed);
        crap->Draw("Psame"); 

        TH1D* ratio = (TH1D*)crap->Clone();
        ratio->Divide(crap,_unfolder->getTrue());
        //ratio->GetXaxis()->SetRangeUser(15,1499);
        ratio->GetYaxis()->SetRangeUser(0.95,1.15);
        ////ratio->SetBinContent(39,1.);
        ////ratio->SetBinContent(40,1.);
        ////ratio->SetBinContent(41,1.);
        //ratio->GetXaxis()->SetTitle("m [GeV]");
        ratio->GetXaxis()->SetTitle("Mass-rapidity index");
        ratio->GetYaxis()->SetTitle("N_{unf, MC}/N_{true, MC}");  
        //ratio->SetBinContent(1,ratio->GetBinContent(1)-(ratio->GetBinContent(1)-1)/1.5);
        //ratio->Draw();

        TLegend* leg = MakeLegend();
        leg->AddEntry(_unfolder->getTrue(),"True","l");
        if (CLOSURE_TEST) {
          leg->AddEntry(crap,"Unfolded MC","lp");
        } else {
          leg->AddEntry(crap,"Unfolded","lp");
        }
        leg->Draw("same");
        can->SaveAs("plot.png");

        //
        //STEP4: ALL DONE SAVING OUTPUT
        //
        TH2F* invrseResMat = (TH2F*)_unfolder->matrixToHist(_unfolder->getMigMatInverse(),_inResMat);
        if (USE_OVERFLOWS && !DO_ROO) {
          _inMeas = rebinWithNoOU1(_inMeas);
          _inTrue = rebinWithNoOU1(_inTrue);
        }
        if (USE_OVERFLOWS && !DO_ROO) {
          crap = rebinWithNoOU1(crap);
        } 

        _outFile = new TFile("rootfiles/unfoldData.root","recreate");
        _outFile->cd();
        invrseResMat->Write("inverse_Response");
        _inMeas->Write("hmeas");
	crap->Write("hunfolded");
	_inTrue->Write("htrue");
	_outFile->Write();

        //check dims before and after
        //_unfolder->getMigMatInverse()->Print();


	return;		
	//_unfolder->getMigMat()->Print();
	//_unfolder->getMigMatInverse()->Print();

		
	//_inTrue->Print("all");	

}

TH1F* rebinWithOU1(TH1F* h_initial) {
        //
        //switch to under-overlow inclusion will be provided by changing the matrix
        // 
        //FIXME right now the final binning is hard coded, and depends on the initial binning
        const int nbins = 43;
        double mass_xbin[nbins+1] = {0,15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000, 10000};

        //Take the over and under flows into account
        TH1F* h_ou = new TH1F(h_initial->GetName(),h_initial->GetName(),nbins,mass_xbin);
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
             h_ou->SetBinContent(i+2,h_initial->GetBinContent(i+1));
             h_ou->SetBinError(i+2,h_initial->GetBinError(i+1));
        }
        h_ou->SetBinContent(1,h_initial->GetBinContent(0));
        h_ou->SetBinContent(h_initial->GetXaxis()->GetNbins()+2,h_initial->GetBinContent(h_initial->GetXaxis()->GetNbins()+1)); //41
        h_ou->SetBinError(1,h_initial->GetBinError(0));
        h_ou->SetBinError(h_initial->GetXaxis()->GetNbins()+2,h_initial->GetBinError(h_initial->GetXaxis()->GetNbins()+1));

        //now set the poibter to it
        return h_ou;

}

TH1F* rebinWithNoOU1(TH1F* h_initial) {
        //
        //This is meant to rebin back a histogram that was rebinned with OU once! Keep this in mind
        // 
        //FIXME right now the final binning is hard coded, and depends on the initial binning
        const int nbins = 41;
        double mass_xbin[nbins+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

        //Take the over and under flows into account
        TH1F* h = new TH1F(h_initial->GetName(),h_initial->GetName(),nbins,mass_xbin);
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
             h->SetBinContent(i,h_initial->GetBinContent(i+1));
             h->SetBinError(i,h_initial->GetBinError(i+1));
        }

        //now set the poibter to it
        return h;

}



TH2F* rebinWithOU2(TH2F* h_initial) {
        //
        //switch to under-overlow inclusion will be provided by changing the matrix
        // 
        //FIXME right now the final binning is hard coded, and depends on the initial binning
        const int nbins = 43;
        double mass_xbin[nbins+1] = {0,15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000,10000};


        //Take the over and under flows into account
        TH2F* h_ou = new TH2F(h_initial->GetName(),h_initial->GetName(),nbins,mass_xbin,nbins,mass_xbin);
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
          for (int j = 0; j < h_initial->GetYaxis()->GetNbins(); j++) {
             h_ou->SetBinContent(i+2,j+2,h_initial->GetBinContent(i+1,j+1));
             h_ou->SetBinError(i+2,j+2,h_initial->GetBinError(i+1,j+1));
          }
        }
        //organize vertical overflows
        //make left
        for (int j = 0; j < h_initial->GetXaxis()->GetNbins()+2; j++) {
          h_ou->SetBinContent(1,j+1,h_initial->GetBinContent(0,j));
          h_ou->SetBinError(  1,j+1,h_initial->GetBinError(0,j));
        }
        //make right
        for (int j = 0; j < h_initial->GetXaxis()->GetNbins()+2; j++) {
          h_ou->SetBinContent(h_initial->GetYaxis()->GetNbins()+2,j+1,h_initial->GetBinContent(h_initial->GetYaxis()->GetNbins()+1,j));
          h_ou->SetBinError(  h_initial->GetYaxis()->GetNbins()+2,j+1,h_initial->GetBinError(  h_initial->GetYaxis()->GetNbins()+1,j));
        }
        //make up
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins()+2; i++) {
          //cout << i+1 << " make up " << 1 << endl;
          h_ou->SetBinContent(i+1,1,h_initial->GetBinContent(i,0));
          h_ou->SetBinError(  i+1,1,h_initial->GetBinError(i,0));
        }
        //make down
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins()+2; i++) {
          h_ou->SetBinContent(i+1,h_initial->GetYaxis()->GetNbins()+2,h_initial->GetBinContent(i,h_initial->GetYaxis()->GetNbins()+1));
          h_ou->SetBinError(  i+1,h_initial->GetYaxis()->GetNbins()+2,h_initial->GetBinError(  i,h_initial->GetYaxis()->GetNbins()+1));
        }
        //now set the poibter to it
        return h_ou;

}


//
// column normalize
//
void colNormalize(TH2F* hist){
	
	int nbinsx = hist->GetNbinsX();
	int nbinsy = hist->GetNbinsY();

	for (int icol = 1; icol <= nbinsx; ++icol){

		double sum = 0.;
		for (int irow = 1; irow <= nbinsy; ++irow){
			sum += hist->GetBinContent(icol,irow);
		}
		
		for (int irow = 1; irow <= nbinsy; ++irow){
			double val = hist->GetBinContent(icol,irow);
			val /= sum;
			hist->SetBinContent(icol,irow,val);
		}

	}

}

double err_calc( float v1, float v2, float e1, float e2 )
{
  // if v = v1/v2, type is 0

  float v = v1/v2;
  float dv1 = 1./v2;
  float dv2 = - v1/(v2*v2);
  float dv_2 = dv1*dv1*e1*e1 + dv2*dv2*e2*e2;

  if( dv_2 == 0) dv_2 = 0.0000001;
  return TMath::Sqrt(dv_2);
}

TLegend*  MakeLegend() {
  
   TLegend *_leg= new TLegend(0.20,0.25,0.35,0.42);
   _leg->SetTextFont(72);
   _leg->SetTextSize(0.060);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);
   return _leg;
  
} 


void printHisto1D(TH1F* h_initial) {

        //Take the over and under flows into account
        cout << "Size " << h_initial->GetXaxis()->GetNbins() << endl;
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
             //cout << "Bin " << i+1 << " " << h_initial->GetBinCenter(i+1)-h_initial->GetBinWidth(i+1)/2 << " " << h_initial->GetBinCenter(i+1)+h_initial->GetBinWidth(i+1)/2<< " content/error " << h_initial->GetBinContent(i+1)  << " " << h_initial->GetBinError(i+1)<< endl;
               cout << "Bin " << i <<" content/error " << h_initial->GetBinContent(i+1)  << " " << h_initial->GetBinError(i+1)<< endl;
        }
}

void printHisto2D(TH2F* h_initial) {

        //Take the over and under flows into account
        cout << "Size " << h_initial->GetXaxis()->GetNbins() <<"x" << h_initial->GetYaxis()->GetNbins()<< endl;
        for (int i = -1; i < h_initial->GetXaxis()->GetNbins()+1; i++) {
           for (int j = -1; j < h_initial->GetYaxis()->GetNbins()+1; j++) { //41 is overflow 42 is new last bin
             //if (i != j) continue;
             cout << "Bin " << i+1 << " x " << j+1 << " content/error " << h_initial->GetBinContent(i+1,j+1)  << " " << h_initial->GetBinError(i+1,j+1)<< endl;
          }
       }
}     


TH2F* getMigMatrix(TH1F* htrue, TH2F* hden) {

    TH2F* migMatrix = (TH2F*)hden->Clone();
    const int nbin = hden->GetXaxis()->GetNbins();

    for( int i = 0; i < nbin+1; i++ ) {
        for( int j = 0; j < nbin+1; j++ ) {
          double den = htrue->GetBinContent(i);
          migMatrix->SetBinContent(i, j, hden->GetBinContent(i,j)/den);
        }
    }
   //printHisto2D(migMatrix); 

   return migMatrix;
}

TH2F* getFromMigMatrix(TH1F* htrue, TH2F* hmig) {

    TH2F* evMatrix = (TH2F*)hmig->Clone();
    const int nbin = hmig->GetXaxis()->GetNbins();

    for( int i = 0; i < nbin+1; i++ ) {
        for( int j = 0; j < nbin+1; j++ ) {
          double den = htrue->GetBinContent(i);
          evMatrix->SetBinContent(i, j, hmig->GetBinContent(i,j)*den);
        }
    }
   //printHisto2D(migMatrix); 

   return evMatrix;
}

void normalizeToPeak(TH1F* hmc_init, TH1F* hdata) {
   double this_mc = hmc_init->Integral(10,22);
   double this_data = hdata->Integral(10,22);
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc_init->Scale(this_data/this_mc);
      this_mc = hmc_init->Integral(10,22);
   }
}

void normalizeToPeak2D(TH2F* hmc_init, TH1F* hdata) {
   double this_mc = hmc_init->Integral(10,22,10,22);
   double this_data = hdata->Integral(10,22);
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      hmc_init->Scale(this_data/this_mc);
      this_mc = hmc_init->Integral(10,22);
   }
}
