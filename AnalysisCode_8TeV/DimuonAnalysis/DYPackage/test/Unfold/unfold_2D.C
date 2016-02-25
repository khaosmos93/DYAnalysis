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
#endif

#define USE_OVERFLOWS false
#define DO_ROO false
#define DO_BAYES false
#define FEWZ_CORRECTED true
#define DATA_DRIVEN_BKG true

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
TH1F* _inTrue24 = 0;
TH1F* _inMeas24= 0;
TH1F* _inMeasMC24= 0;
TH1F* _inMeasData24= 0;
//TH1F* _inTrue12 = 0;
//TH1F* _inMeas12= 0;
//TH1F* _inMeasMC12= 0;
//TH1F* _inMeasData12= 0;
TH1F* sigNorm = 0;
TH1F* sigNorm24 = 0;

TCanvas* can = 0;

TH2F* _inMigMat = 0;
TH2F* _inResMat = 0;
TH2F* _inEvMat = 0;
int nmass_bins = 6;

void colNormalize(TH2F* hist);
//void rebinWithOU1(TH1F*);
//void rebinWithOU2(TH2F*);
TH2F* Smooth(TH2F*, Int_t, Option_t*);

void unfold_2D(TString RR){

        bool CLOSURE_TEST = false;

        TString POSTFIX_corr = "";
        if (FEWZ_CORRECTED) POSTFIX_corr = "_corr";

        gROOT->ProcessLine(".L ../tools/plottools.C");
        gROOT->ProcessLine(".L ../tools/histotools.C");

        //SET UP ROO
        gSystem->Load("RooUnfold-1.1.1/libRooUnfold.so");

        //_inFile = new TFile("resMatrixProd.root");
        TString resRR = "";
        if (RR == "POWHEG") {
           CLOSURE_TEST = true;
           resRR = "_POWHEG";
        }
        _inFile = new TFile("../Inputs/unfold/resMatrixProd"+resRR+"_41.root");
        //
        //STEP0: GET HISTOGRAMS FROM THE PREVIOUS FILE
        //
        _inTrue = (TH1F*)_inFile->Get("htrue2"+POSTFIX_corr);
        _inMeasMC= (TH1F*)_inFile->Get("hmeas2"+POSTFIX_corr);
        if (USE_OVERFLOWS) {
          _inTrue24 = (TH1F*)_inFile->Get("htrue2_24"+POSTFIX_corr);
          _inMeasMC24= (TH1F*)_inFile->Get("hmeas2_24"+POSTFIX_corr);
          //_inTrue12 = (TH1F*)_inFile->Get("htrue2_12"+POSTFIX_corr);
          //_inMeasMC12= (TH1F*)_inFile->Get("hmeas2_12"+POSTFIX_corr);
        }
        if (USE_OVERFLOWS) _inEvMat = (TH2F*)_inFile->Get("hden2_156"+POSTFIX_corr);
        else _inEvMat = (TH2F*)_inFile->Get("hden2"+POSTFIX_corr); 
      
        TFile* _inData = new TFile("../Inputs/rawYield/DYspectrum_Rap.root");
        _inData->cd();
        if (USE_OVERFLOWS) sigNorm = (TH1F*)gDirectory->Get("hsig_fewz_r");
        else sigNorm = (TH1F*)gDirectory->Get("hsig_fewz");

        if (USE_OVERFLOWS) { 
          _inTrue = rebinWithOU1_2D(_inTrue,_inTrue24);
          _inMeasMC = rebinWithOU1_2D(_inMeasMC,_inMeasMC24);
        }  

        //SCALING PERFORMED ON A FINAL OU HISTO
        _inTrue->Scale(sigNorm->Integral()/_inTrue->Integral()); //73,96));
        _inMeasMC->Scale(sigNorm->Integral()/_inMeasMC->Integral());
        _inEvMat->Scale(sigNorm->Integral()/_inEvMat->Integral());

        normalizeToPeak(_inTrue,sigNorm);
        normalizeToPeak(_inMeasMC,sigNorm);
        normalizeToPeak2D(_inEvMat,sigNorm);

        _inMigMat = getMigMatrix(_inTrue,_inEvMat); //(TH2F*)_inFile->Get("migMatrix");
        //cout << "_inMigMat " << _inMigMat->GetBinContent(40,40) << " " << _inMigMat->GetBinContent(41,41) << endl;

        _inMeas = (TH1F*)_inMeasMC->Clone("inMeas");
        _inMeasData = (TH1F*)_inMeasMC->Clone("inMeas");

        _inResMat = (TH2F*)_inMigMat->Clone("resMatrix");

        _inMeas->Reset();

        //STEP1: subtract the bacgrkounds
        if (!CLOSURE_TEST) {
          //fill the raw yields to appropriate arrays 
	  std::vector<double> input_data;
	  std::vector<double> input_err;
          if (USE_OVERFLOWS) { 
             for (int i = 0; i < 156; i++) {
                 input_data.push_back(0.);
                 input_err.push_back(0.);
             }
          } else {
             for (int i = 0; i < 132; i++) {
                 input_data.push_back(0.);
                 input_err.push_back(0.);
             }
          }

          TFile* f = new TFile("../Inputs/rawYield/DYspectrum_Rap.root");
          f->cd(); 
          //main 132 bin histos
          TH1F* data, *qcd, *ttbar, *DYtautau, *ttbar_mc, *Wlepton, *diboson;
          TH1F* hdata, *hqcd, *httbar, *hDYtautau, *httbar_mc, *hWlepton, *hdiboson;
          //underflow
          TH1F* udata, *uqcd, *uttbar, *uDYtautau, *uttbar_mc, *uWlepton, *udiboson;
          //overflow
          //TH1F* odata, *oqcd, *ottbar, *oDYtautau, *ottbar_mc, *oWlepton, *odiboson;
          if (USE_OVERFLOWS && !DO_ROO) {
             data = (TH1F*)gDirectory->Get("hdata");
             udata = (TH1F*)hdata24->Clone();
             //odata = (TH1F*)hdata12->Clone();  
             hdata = rebinWithOU1_2D(data,udata);//,odata);
          } else {
             hdata = (TH1F*)gDirectory->Get("hdata");
          } 
          if (DATA_DRIVEN_BKG)  {
           qcd = (TH1F*)gDirectory->Get("hqcd");
           ttbar = (TH1F*)gDirectory->Get("httbar_mc");
           //case of OU 
           if (USE_OVERFLOWS && !DO_ROO) {
             uqcd = (TH1F*)hqcd24->Clone();
             uttbar = (TH1F*)httbar24->Clone();
             //oqcd = (TH1F*)hqcd12->Clone();
             //ottbar = (TH1F*)httbar12->Clone();
             hqcd = rebinWithOU1_2D(qcd,uqcd);//,oqcd);
             httbar = rebinWithOU1_2D(ttbar,uttbar);//,ottbar);
           } else {
             hqcd = (TH1F*)gDirectory->Get("hqcd");
             httbar = (TH1F*)gDirectory->Get("httbar_mc");
           }
          } else {
           DYtautau = (TH1F*)gDirectory->Get("hDYtautau");
           ttbar_mc = (TH1F*)gDirectory->Get("httbar_mc");
           Wlepton = (TH1F*)gDirectory->Get("hWlepton");
           diboson = (TH1F*)gDirectory->Get("hdiboson");
           //case of OU 
           if (USE_OVERFLOWS && !DO_ROO) {
             uDYtautau = (TH1F*)DYtautau24->Clone();
             uttbar_mc = (TH1F*)ttbar_mc24->Clone();
             uWlepton = (TH1F*)Wlepton24->Clone();
             udiboson = (TH1F*)diboson24->Clone();
             //oDYtautau =(TH1F*)DYtautau12->Clone();
             //ottbar_mc =(TH1F*)ttbar_mc12->Clone();
             //oWlepton =(TH1F*)Wlepton12->Clone();
             //odiboson =(TH1F*)diboson12->Clone();
             hDYtautau = rebinWithOU1_2D(DYtautau,DYtautau24);//,DYtautau12);
             httbar_mc= rebinWithOU1_2D(ttbar_mc,ttbar_mc24);//,ttbar_mc12);
             hWlepton= rebinWithOU1_2D(Wlepton,Wlepton24);//,Wlepton12);
             hdiboson=rebinWithOU1_2D(diboson,diboson24);//,diboson12);
           } else {
             hDYtautau = (TH1F*)gDirectory->Get("hDYtautau");
             httbar_mc = (TH1F*)gDirectory->Get("httbar_mc");
             hWlepton = (TH1F*)gDirectory->Get("hWlepton");
             hdiboson = (TH1F*)gDirectory->Get("hdiboson");
           }
          }

          if (USE_OVERFLOWS) nmass_bins = 7;
          for(int i = 0; i < nmass_bins; i++ ) {
            int nRapbins = 24;
            if(i == 5 && !USE_OVERFLOWS) nRapbins = 12;
            if(i == 6 && USE_OVERFLOWS) nRapbins = 12;
            for( int j = 0; j < nRapbins; j++ ) {
                //take data and subtract backgrounds
                double background = 0;
                double error = 0;
                //FIXME this assumes the data-driven error is proper in the file acessed
                if (DATA_DRIVEN_BKG) { 
                   background = (hqcd->GetBinContent(i*24+j+1)+httbar->GetBinContent(i*24+j+1));
                   error = sqrt(pow(hdata->GetBinError(i*24+j+1),2)+ pow(hqcd->GetBinError(i*24+j+1),2)+pow(httbar->GetBinError(i*24+j+1),2)); 
                } else { 
                   background = (hqcd->GetBinContent(i*24+j+1)+httbar_mc->GetBinContent(i*24+j+1)+hDYtautau->GetBinContent(i*24+j+1)+hWlepton->GetBinContent(i*24+j+1)+hdiboson->GetBinContent(i*24+j+1));
                   error = sqrt(pow(hdata->GetBinError(i*24+j+1),2)+ pow(hqcd->GetBinError(i*24+j+1),2)+pow(httbar_mc->GetBinError(i*24+j+1),2)+pow(hDYtautau->GetBinError(i*24+j+1),2)+pow(hWlepton->GetBinError(i*24+j+1),2)+pow(hdiboson->GetBinError(i*24+j+1),2));
                }
                input_data.at(i*24+j) = hdata->GetBinContent(i*24+j+1) - background;
                input_err.at(i*24+j) = error;
            }
          }

  	  int numbins = _inMeasData->GetNbinsX();
	  for( int ibin = 0; ibin < numbins; ibin++ ) {
	    //cout << "input(" << ibin << ") = " << input_data.at(ibin) << " " << input_err.at(ibin) << endl;
	    _inMeasData->SetBinContent(ibin+1, input_data.at(ibin));
	    _inMeasData->SetBinError(ibin+1, input_err.at(ibin));
	  }
	  _inMeasData->SetBinContent(0, 0);
	  _inMeasData->SetBinError(0, 0);
        } ////not closure test

        //
        //SET UP CANVAS TO DRAW
        //
        can = new TCanvas("can","can",800,600);
        gStyle->SetPalette(1);
        //can->SetLogy();
        //can->SetLogx();

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
             RooUnfoldResponse response(_inMeasMC,_inTrue,_inEvMat); //_h2_tmp); //_inResMat);
          } else {
             RooUnfoldResponse response(_inMeasMC,_inTrue,_inResMat);
          }
          //if (USE_OVERFLOWS) response.UseOverflow();
          if (CLOSURE_TEST) {
             if (DO_BAYES) {
                RooUnfoldBayes unfoldBayes(&response, _inMeasMC, 1);
             } else {
                RooUnfoldSvd unfoldSVD(&response, _inMeasMC, 2);
             }
          } else  {
             if (DO_BAYES) {
                RooUnfoldBayes unfoldBayes(&response, _inMeasData, 1);
             } else {
                RooUnfoldSvd unfoldSVD(&response, _inMeasData, 2);
             }
          }
          if (DO_BAYES) {
              crap = (TH1F*) unfoldBayes.Hreco();
          } else {
              crap = (TH1F*) unfoldSVD.Hreco(); //analog of getUnfolded() in unfolder class
          }
        }

	for( int i = 0; i < nmass_bins; i++ ) {
          int nRapbins;
          if( i == nmass_bins-1 ) nRapbins = 12;
          else nRapbins = 24;
          for( int j = 0; j < nRapbins; j++ ) {
            double val = crap->GetBinContent(i*24+j+1);
            double err = sqrt(pow(crap->GetBinError(i*24+j+1),2));
	    crap->SetBinContent(24*i+j+1, val);
	    crap->SetBinError(24*i+j+1, err);
	  }
	}

        crap->SetMarkerStyle(20);
        crap->SetMarkerColor(kRed);
        crap->Draw("same");
        can->SaveAs("2D.png");

        TLegend* leg = MakeLegend();
        leg->AddEntry(_unfolder->getTrue(),"True","l");
        if (CLOSURE_TEST) {
          leg->AddEntry(crap,"Unfolded MC","lp");
        } else {
          leg->AddEntry(crap,"Unfolded","lp");
        }

        //
        //STEP4: ALL DONE SAVING OUTPUT
        //
        TH2F* invrseResMat = (TH2F*)_unfolder->matrixToHist(_unfolder->getMigMatInverse(),_inResMat);

	_outFile = new TFile("./rootfiles/unfoldData_HLT_Double_2D.root","recreate");
        _outFile->cd();
        invrseResMat->Write("inverse_Response");
        _inMeas->Write("hmeas");
        _inTrue->Write("htrue");
        crap->Write("hunfolded");
        _outFile->Write();


	return;		
	_unfolder->getMigMat()->Print();
	_unfolder->getMigMatInverse()->Print();

		
	_inTrue->Print("all");	
	

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

// This replaces the 132 bin histogram with 168, adding 24 rapidity bins at low mass and 12 at high mass.
// It sets the initial 132 bin contents to the ones starting bin number #24
// Here underflow and overflow histograms simply 24 and 12 bin histos correspondingly which are filled for 0-20 GeV mass and 1500-10000 GeV mass
TH1F* rebinWithOU1_2D(TH1F* h_initial, TH1F* underflow) { //, TH1F* overflow) {
        //
        //switch to under-overlow inclusion will be provided by changing the matrix
        // 
        //FIXME right now the final binning is hard coded, and depends on the initial binning
        const int nbins = 156;
        TH1F* h_ou = new TH1F(h_initial->GetName(),h_initial->GetName(),nbins,0,nbins);
        //fill underflow
        for (int i = 0; i < underflow->GetXaxis()->GetNbins(); i++) {
             h_ou->SetBinContent(i+1,underflow->GetBinContent(i+1));
             h_ou->SetBinError(i+1,underflow->GetBinError(i+1));
        }       
        //fill main histogram
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
             h_ou->SetBinContent(i+1+24,h_initial->GetBinContent(i+1));
             h_ou->SetBinError(i+1+24,h_initial->GetBinError(i+1));
        }
        //fill underflow
        //for (int i = 0; i < overflow->GetXaxis()->GetNbins(); i++) {
        //     h_ou->SetBinContent(i+1+132+24,overflow->GetBinContent(i+1));
        //     h_ou->SetBinError(i+1+132+24,overflow->GetBinError(i+1));
        //}

        return h_ou;

}

TH2F* getMigMatrix(TH1F* htrue, TH2F* hden) {

    TH2F* migMatrix = (TH2F*)hden->Clone();
    const int nbin = hden->GetXaxis()->GetNbins(); //this return 168 in the over/under case

    for( int i = 0; i < nbin+1; i++ ) {
        for( int j = 0; j < nbin+1; j++ ) {
          double den = htrue->GetBinContent(i+1);
          migMatrix->SetBinContent(i+1, j+1, hden->GetBinContent(i+1,j+1)/den);
        }
    }
   //printHisto2D(migMatrix); 

   return migMatrix;
}

TH2F* Smooth(TH2F* thish, Int_t ntimes, Option_t *option, Int_t offset) {

   // Smooth bin contents of this 2-d histogram using kernel algorithms
   // similar to the ones used in the raster graphics community.
   // Bin contents in the active range are replaced by their smooth values.
   // If Errors are defined via Sumw2, they are scaled.
   // 3 kernels are proposed k5a, k5b and k3a.
   // k5a and k5b act on 5x5 cells (i-2,i-1,i,i+1,i+2, and same for j)
   // k5b is a bit more stronger in smoothing
   // k3a acts only on 3x3 cells (i-1,i,i+1, and same for j).
   // By default the kernel "k5a" is used. You can select the kernels "k5b" or "k3a"
   // via the option argument.
   // If TAxis::SetRange has been called on the x or/and y axis, only the bins
   // in the specified range are smoothed.
   // In the current implementation if the first argument is not used (default value=1).
   //
   // implementation by David McKee (dmckee@bama.ua.edu). Extended by Rene Brun

   Double_t k5a[5][5] =  { { 0, 0, 1, 0, 0 },
                           { 0, 2, 2, 2, 0 },
                           { 1, 2, 5, 2, 1 },
                           { 0, 2, 2, 2, 0 },
                           { 0, 0, 1, 0, 0 } };
   Double_t k5b[5][5] =  { { 0, 1, 2, 1, 0 },
                           { 1, 2, 4, 2, 1 },
                           { 2, 4, 8, 4, 2 },
                           { 1, 2, 4, 2, 1 },
                           { 0, 1, 2, 1, 0 } };
   Double_t k3a[3][3] =  { { 0, 1, 0 },
                           { 1, 2, 1 },
                           { 0, 1, 0 } };

   if (ntimes > 1) {
      Warning("Smooth","Currently only ntimes=1 is supported");
   }
   TString opt = option;
   opt.ToLower();
   Int_t ksize_x=5;
   Int_t ksize_y=5;
   Double_t *kernel = &k5a[0][0];
   if (opt.Contains("k5b")) kernel = &k5b[0][0];
   if (opt.Contains("k3a")) {
      kernel = &k3a[0][0];
      ksize_x=3;
      ksize_y=3;
   }

   TH2F* hworking = (TH2F*)thish->Clone();

   // find i,j ranges
   Int_t ifirst = thish->GetXaxis()->GetFirst();
   Int_t ilast  = thish->GetXaxis()->GetLast();
   Int_t jfirst = thish->GetYaxis()->GetFirst();
   Int_t jlast  = thish->GetYaxis()->GetLast();

   // Determine the size of the bin buffer(s) needed
   Int_t nx = thish->GetNbinsX();
   Int_t ny = thish->GetNbinsY();
  cout << nx << " " << ny << endl; 
   Int_t bufSize  = (nx+2)*(ny+2);
   Double_t *buf  = new Double_t[bufSize];
   Double_t *ebuf = new Double_t[bufSize];

   // Copy all the data to the temporary buffers
   Int_t i,j,bin;
   for (i=ifirst; i<=ilast; i++){
      for (j=jfirst; j<=jlast; j++){
         Int_t bin = thish->GetBin(i,j);
         buf[bin] =thish->GetBinContent(bin);
         ebuf[bin]=thish->GetBinError(bin);
      }
   }

   // Kernel tail sizes (kernel sizes must be odd for this to work!)
   Int_t x_push = (ksize_x-1)/2;
   Int_t y_push = (ksize_y-1)/2;

   // main work loop
   for (int i=ifirst; i<=ilast; i++){
      for (int j=jfirst; j<=jlast; j++) {
         if (i-j<offset) continue;

         Double_t content = 0.0;
         Double_t error = 0.0;
         Double_t norm = 0.0;

         for (Int_t n=0; n<ksize_x; n++) {
            for (Int_t m=0; m<ksize_y; m++) {
               Int_t xb = i+(n-x_push);
               Int_t yb = j+(m-y_push);
               if ( (xb >= 1) && (xb <= nx) && (yb >= 1) && (yb <= ny) ) {
                  Int_t bin = thish->GetBin(xb,yb);
                  Double_t k = kernel[n*ksize_y +m];
                  //if ( (k != 0.0 ) && (buf[bin] != 0.0) ) { // General version probably does not want the second condition
                  if ( k != 0.0 ) {
                     norm    += k;
                     content += k*buf[bin];
                     error   += k*k*buf[bin]*buf[bin];
                  }
               }
            }
         }

         if ( norm != 0.0 ) {
            hworking->SetBinContent(i,j,content/norm);
            error /= (norm*norm);
            hworking->SetBinError(i,j,sqrt(error));
         }
      }
   }

   delete [] buf;
   delete [] ebuf;
   return hworking;
}
