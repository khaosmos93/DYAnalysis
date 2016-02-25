#include "include/Unfolder.h"
Unfolder* _unfolder;

void Matrix( void )
{

  gStyle->SetOptStat(0);

  TFile* f  = new TFile("../Inputs/unfold/resMatrixProd_41.root");
  f->cd();
  //uncomment for 1D case
  //TH2F* hmig = (TH2F*)hden->Clone();
  //TH1F* _inTrue = (TH1F*)htrue->Clone();
  //TH1F* _inMeas = (TH1F*)hmeas->Clone();
  //uncomment for 2D case
  TH2F* hmig = (TH2F*)hden2->Clone();
  TH1F* _inTrue = (TH1F*)htrue2->Clone();
  TH1F* _inMeas = (TH1F*)hmeas2->Clone(); 
  TH2F* _inResMat = getMigMatrix(_inTrue, hmig);
  //_inResMat = rebinWithOU2(_inResMat); 

  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  //gStyle->SetPaintTextFormat("4.2g");
/*
  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  c1->cd();
  c1->SetLogy();
  c1->SetLogx();
  c1->SetLeftMargin(0.14);
  c1->SetRightMargin(0.13);
  c1->SetBottomMargin(0.13); 
  hmig->SetTitle("");
  hmig->SetStats(kFALSE); 
  //hmig->SetTitle("CMS at 7TeV, Lumi = 36.1 pb^{-1}");
  hmig->GetXaxis()->SetTitle("generated M(#mu#mu) post-FSR [GeV]");
  hmig->GetYaxis()->SetTitle("reconstructed M(#mu#mu) [GeV]");
  hmig->GetXaxis()->SetMoreLogLabels();
  hmig->GetYaxis()->SetMoreLogLabels();
  hmig->GetYaxis()->SetNoExponent();
  hmig->GetXaxis()->SetNoExponent();
  //hmig->SetMinimum(0.001);
  hmig->Draw("COLZ");
*/
  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->cd();
  //c2->SetLogy();
  //c2->SetLogx();
  c2->SetLeftMargin(0.14);
  c2->SetRightMargin(0.13);
  c2->SetBottomMargin(0.13);
  _inResMat->SetTitle("");
  _inResMat->SetStats(kFALSE);
  _inResMat->GetXaxis()->SetTitle("generated mass/rapidity bins");  //generated M(#mu#mu) post-FSR [GeV]");
  _inResMat->GetYaxis()->SetTitle("reconstructed mass/rapidity bins"); //reconstructed M(#mu#mu) [GeV]");
  _inResMat->GetXaxis()->SetMoreLogLabels();
  _inResMat->GetYaxis()->SetNoExponent();
  _inResMat->GetXaxis()->SetNoExponent();
  _inResMat->SetMinimum(0.00025);
  _inResMat->Draw("COLZ");
  c2->Print("eps/resMatrix.png");
/*

  //
  //Inverted rsponse matrix
  //
  _unfolder = new Unfolder("badger");
  _unfolder->setTrue(_inTrue);
  _unfolder->setMeas(_inMeas);
  _unfolder->setMigrationMatrix(_inResMat);
  _unfolder->unfold();

  TH2F* invrseResMat = (TH2F*)_unfolder->matrixToHist(_unfolder->getMigMatInverse(),_inResMat);

  TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
  c3->cd();
  c3->SetLogy();
  c3->SetLogx();
  c3->SetLeftMargin(0.14);
  c3->SetRightMargin(0.13);
  c3->SetBottomMargin(0.13);
  invrseResMat->SetTitle("");
  invrseResMat->SetStats(kFALSE);
  //invrseResMat->SetTitle("CMS at 7TeV, Lumi = 36.1 pb^{-1}");
  invrseResMat->GetXaxis()->SetTitle("generated M(#mu#mu) post-FSR [GeV]");
  invrseResMat->GetYaxis()->SetTitle("reconstructed M(#mu#mu) [GeV]");
  invrseResMat->GetXaxis()->SetMoreLogLabels();
  //invrseResMat->GetYaxis()->SetMoreLogLabels();
  invrseResMat->GetYaxis()->SetNoExponent();
  invrseResMat->GetXaxis()->SetNoExponent();
  //invrseResMat->SetMinimum(0.001);
  invrseResMat->Draw("COLZ");
*/
}

TH2F* getMigMatrix(TH1F* htrue, TH2F* hden) {

    TH2F* migMatrix = (TH2F*)hden->Clone();
    const int nbin = htrue->GetXaxis()->GetNbins();

    for( int i = 0; i < nbin+2; i++ ) {
        for( int j = 0; j < nbin+2; j++ ) {
          double den = htrue->GetBinContent(i);
          migMatrix->SetBinContent(i, j, hden->GetBinContent(i,j)/den);
        }
     }
   //printHisto2D(migMatrix); 

   return migMatrix;
}

TH2F* rebinWithOU2(TH2F* h_initial) {
        //
        //switch to under-overlow inclusion will be provided by changing the matrix
        // 
        const int nbins = 43;
        double mass_xbin[nbins+1] = {0,15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500,2000,10000};
        
        
        //Take the over and under flows into account
        TH2F* h_ou = new TH2F(h_initial->GetName(),h_initial->GetName(),nbins,mass_xbin,nbins,mass_xbin);
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
          for (int j = 0; j < h_initial->GetYaxis()->GetNbins(); j++) {             h_ou->SetBinContent(i+2,j+2,h_initial->GetBinContent(i+1,j+1));
             h_ou->SetBinError(i+2,j+2,h_initial->GetBinError(i+1,j+1));          }
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
          h_ou->SetBinContent(i+1,1,h_initial->GetBinContent(i,0));
          h_ou->SetBinError(  i+1,1,h_initial->GetBinError(i,0));
        }
        //make down
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins()+2; i++) {
          h_ou->SetBinContent(i+1,h_initial->GetYaxis()->GetNbins()+2,h_initial->GetBinContent(i,h_initial->GetYaxis()->GetNbins()+1));
          h_ou->SetBinError(  i+1,h_initial->GetYaxis()->GetNbins()+2,h_initial->GetBinError(  i,h_initial->GetYaxis()->GetNbins()+1));
        }        //now set the poibter to it
        return h_ou;
        
}

