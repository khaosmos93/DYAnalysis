#include <TMath.h>
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

//get the histograms with efficiency correction factors per bin 
//both 1D and 2D all efficineyc types  - can make a free parameter
void effCorrelEstimator_step1(string REPLICA, TString filePostfix) {

  TString SYST = "_"; 
  if (atoi(REPLICA.c_str()) > 0) {
     SYST = "_syst";
  } 

  const int theNBins = 40;
  Double_t mBins[theNBins+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};
  const int the2D_Mbins = 6;
  const int the2D_Ybins = 24;
  int the2D_MYbins = the2D_Mbins * the2D_Ybins;

  Double_t mBins_2D[the2D_Mbins+1] = {20,30, 45, 60, 120, 200, 1500};
  Double_t yBins_2D[the2D_Ybins+1] = {0.,  0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
                                      1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                                      2.0, 2.1, 2.2, 2.3, 2.4};

  float effLimit_low = 0.8;
  float effLimit_high = 1.2;
  const int theEffBins = 80;

  //Structs to hold histos for 1D and 2D
  //This is per each analysis bin
  struct CorrelationHistos{
    TH2D* corrPlot;
    TH2D* NORM_corrPlot;
  } corrHist[theNBins][theNBins], corrHist_2D[the2D_Mbins][the2D_Ybins][the2D_Mbins][the2D_Ybins];


  //This is for 1D
  string specTitle;
  string specName;
  for (int i=0;i<theNBins;++i){
    for (int j=0;j<theNBins;++j){
      specTitle = Form("Efficiency correction correlations : mass bin (%d-%d) GeV vs (%d-%d) GeV; #epsilon;#epsilon",
                       int(mBins[i]+0.1), int(mBins[i+1]+0.1), int(mBins[j]+0.1),int(mBins[j+1]+0.1));
      specName = Form("massBins_%d_%d",i+1,j+1);
      corrHist[i][j].corrPlot =
        new TH2D(specName.c_str(),specTitle.c_str(),theEffBins,effLimit_low,effLimit_high,theEffBins,effLimit_low,effLimit_high);
      //
      specName = Form("massBins_NORM_%d_%d",i+1,j+1);
      corrHist[i][j].NORM_corrPlot =
        new TH2D(specName.c_str(),specTitle.c_str(),theEffBins,effLimit_low,effLimit_high,theEffBins,effLimit_low,effLimit_high);
    }
  }

  //This is for 2D
  //int m,n;
  for (int i=0;i<the2D_Mbins;++i){
    for (int j=0;j<the2D_Ybins;++j){
      for (int k=0;k<the2D_Mbins;++k){
        for (int l=0;l<the2D_Ybins;++l){
          specTitle = Form("Efficiency correction correlations : mass bin (%d-%d) and |Y| bin (%3.1f-%3.1f) vs mass bin (%d-%d) GeV and |Y| bin (%3.1f-%3.1f); #epsilon;#epsilon",
                           int(mBins_2D[i]+0.1),int(mBins[i+1]+0.1), yBins_2D[j], yBins_2D[j+1], int(mBins[k]+0.1),int(mBins[k+1]+0.1),yBins_2D[l], yBins_2D[l+1]);
          specName = Form("massYBins_%d_%d__%d_%d",i+1,j+1,k+1, l+1);
          //std::cout<<"specName = "<< specName<<std::endl;
          corrHist_2D[i][j][k][l].corrPlot =
            new TH2D(specName.c_str(),specTitle.c_str(),theEffBins,effLimit_low,effLimit_high,theEffBins,effLimit_low,effLimit_high);
          //
          //m = i * the2D_Mbins + j;
          //n = k * the2D_Mbins + l;
          specName = Form("massYBins_NORM_%d_%d__%d_%d",i+1,j+1,k+1, l+1);
          //std::cout<<"specName_Norm = "<< specName<<std::endl;
          corrHist_2D[i][j][k][l].NORM_corrPlot =            new TH2D(specName.c_str(),specTitle.c_str(),theEffBins,effLimit_low,effLimit_high,theEffBins,effLimit_low,effLimit_high);
        }
      }
    }  }
  
  //Get the histograms with weighted and plain mass distributions, or even the ready ratio
  //Denom
  gROOT->ProcessLine(".L ../tools/histotools.C");
  //Denom
  TFile *fden = new TFile("DYMBase_1andall_TRM.root");
  fden->cd();
  TH1D* h_rebinMass = (TH1D*)gDirectory->Get("invm");
  //2D not weighted
  TH1D* hden_pre = (TH1D*)gDirectory->Get("rap");
  TH1D* h_Y_M20_30 = get2DSlice(hden_pre, "absrap2030",true);
  TH1D* h_Y_M30_45 = get2DSlice(hden_pre, "absrap3045",true);
  TH1D* h_Y_M45_60 = get2DSlice(hden_pre, "absrap4560",true);
  TH1D* h_Y_M60_120 = get2DSlice(hden_pre, "absrap60120",true);
  TH1D* h_Y_M120_200 = get2DSlice(hden_pre, "absrap120200",true);
  TH1D* h_Y_M200_1500 = get2DSlice(hden_pre, "absrap200",true);

  cout << "Got all the histograms " << h_rebinMass->Integral() << " " << h_Y_M60_120->Integral() << endl;


  TFile* f_isocorr_dd = new TFile("outputs/iso/correction_isoinvm"+filePostfix+"1D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd->cd();
  TH1D* isocorr_dd = (TH1D*)gDirectory->Get("invm");
  TFile* f_recoidcorr_dd = new TFile("outputs/recoid/correction_recoidinvm"+filePostfix+"1D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd->cd();
  TH1D* recoidcorr_dd = (TH1D*)gDirectory->Get("invm");
  TFile* f_trigcorr_dd = new TFile("outputs/trig/correction_triginvm"+filePostfix+"1D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd->cd();
  TH1D* trigcorr_dd = (TH1D*)gDirectory->Get("invm");

  TH1D* h_ratio_rebin = histoMultiply3(recoidcorr_dd,isocorr_dd,trigcorr_dd, false);
  cout << "Got correction histograms " << isocorr_dd->Integral() << " " << trigcorr_dd->Integral() << " " << recoidcorr_dd->Integral() << " " << h_ratio_rebin ->Integral() << endl;


  //I change paths to the current layout
  TFile* f_isocorr_dd_2030 = new TFile("outputs/iso/correction_isoabsrap2030"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd_2030->cd();
  TH1D* isocorr_dd_2030 = (TH1D*)gDirectory->Get("absrap2030");
  TFile* f_recoidcorr_dd_2030 = new TFile("outputs/recoid/correction_recoidabsrap2030"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd_2030->cd();
  TH1D* recoidcorr_dd_2030 = (TH1D*)gDirectory->Get("absrap2030");
  TFile* f_trigcorr_dd_2030 = new TFile("outputs/trig/correction_trigabsrap2030"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd_2030->cd();
  TH1D* trigcorr_dd_2030 = (TH1D*)gDirectory->Get("absrap2030");

  TFile* f_isocorr_dd_3045 = new TFile("outputs/iso/correction_isoabsrap3045"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd_3045->cd();
  TH1D* isocorr_dd_3045 = (TH1D*)gDirectory->Get("absrap3045");
  TFile* f_recoidcorr_dd_3045 = new TFile("outputs/recoid/correction_recoidabsrap3045"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd_3045->cd();
  TH1D* recoidcorr_dd_3045 = (TH1D*)gDirectory->Get("absrap3045");
  TFile* f_trigcorr_dd_3045 = new TFile("outputs/trig/correction_trigabsrap3045"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd_3045->cd();
  TH1D* trigcorr_dd_3045 = (TH1D*)gDirectory->Get("absrap3045");

  TFile* f_isocorr_dd_4560 = new TFile("outputs/iso/correction_isoabsrap4560"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd_4560->cd();
  TH1D* isocorr_dd_4560 = (TH1D*)gDirectory->Get("absrap4560");
  TFile* f_recoidcorr_dd_4560 = new TFile("outputs/recoid/correction_recoidabsrap4560"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd_4560->cd();
  TH1D* recoidcorr_dd_4560 = (TH1D*)gDirectory->Get("absrap4560");
  TFile* f_trigcorr_dd_4560 = new TFile("outputs/trig/correction_trigabsrap4560"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd_4560->cd();
  TH1D* trigcorr_dd_4560 = (TH1D*)gDirectory->Get("absrap4560");

  TFile* f_isocorr_dd_60120 = new TFile("outputs/iso/correction_isoabsrap60120"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd_60120->cd();
  TH1D* isocorr_dd_60120 = (TH1D*)gDirectory->Get("absrap60120");
  TFile* f_recoidcorr_dd_60120 = new TFile("outputs/recoid/correction_recoidabsrap60120"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd_60120->cd();
  TH1D* recoidcorr_dd_60120 = (TH1D*)gDirectory->Get("absrap60120");
  TFile* f_trigcorr_dd_60120 = new TFile("outputs/trig/correction_trigabsrap60120"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd_60120->cd();
  TH1D* trigcorr_dd_60120 = (TH1D*)gDirectory->Get("absrap60120");

  TFile* f_isocorr_dd_120200 = new TFile("outputs/iso/correction_isoabsrap120200"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd_120200->cd();
  TH1D* isocorr_dd_120200 = (TH1D*)gDirectory->Get("absrap120200");
  TFile* f_recoidcorr_dd_120200 = new TFile("outputs/recoid/correction_recoidabsrap120200"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd_120200->cd();
  TH1D* recoidcorr_dd_120200 = (TH1D*)gDirectory->Get("absrap120200");
  TFile* f_trigcorr_dd_120200 = new TFile("outputs/trig/correction_trigabsrap120200"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd_120200->cd();
  TH1D* trigcorr_dd_120200 = (TH1D*)gDirectory->Get("absrap120200");

  TFile* f_isocorr_dd_200 = new TFile("outputs/iso/correction_isoabsrap200"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_isocorr_dd_200->cd();
  TH1D* isocorr_dd_200 = (TH1D*)gDirectory->Get("absrap200");
  TFile* f_recoidcorr_dd_200 = new TFile("outputs/recoid/correction_recoidabsrap200"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_recoidcorr_dd_200->cd();
  TH1D* recoidcorr_dd_200 = (TH1D*)gDirectory->Get("absrap200");
  TFile* f_trigcorr_dd_200 = new TFile("outputs/trig/correction_trigabsrap200"+filePostfix+"2D"+SYST+TString(REPLICA)+".root");
  f_trigcorr_dd_200->cd();
  TH1D* trigcorr_dd_200 = (TH1D*)gDirectory->Get("absrap200");

  TH1D* h_ratio_Y_M20_30   = histoMultiply3(recoidcorr_dd_2030,isocorr_dd_2030,trigcorr_dd_2030,false);
  TH1D* h_ratio_Y_M30_45   = histoMultiply3(recoidcorr_dd_3045,isocorr_dd_3045,trigcorr_dd_3045,false);
  TH1D* h_ratio_Y_M45_60   = histoMultiply3(recoidcorr_dd_4560,isocorr_dd_4560,trigcorr_dd_4560,false);
  TH1D* h_ratio_Y_M60_120  = histoMultiply3(recoidcorr_dd_60120,isocorr_dd_60120,trigcorr_dd_60120,false);
  TH1D* h_ratio_Y_M120_200 = histoMultiply3(recoidcorr_dd_120200,isocorr_dd_120200,trigcorr_dd_120200,false);
  TH1D* h_ratio_Y_M200_1500     = histoMultiply3(recoidcorr_dd_200,isocorr_dd_200,trigcorr_dd_200,false);


  //EXTRAS for 1D
  //Make h_rebinMassW
  TH1D* h_rebinMassW = (TH1D*)h_rebinMass->Clone();
  for (int i = 0; i < h_rebinMassW->GetXaxis()->GetNbins(); i++) {
    h_rebinMassW->SetBinContent(i+1,h_rebinMassW->GetBinContent(i+1)*h_ratio_rebin->GetBinContent(i+1));
    h_rebinMassW->SetBinError(i+1,0.);//h_rebinMassW->GetBinContent(i+1)*sqrt(pow((h_rebinMassW->GetBinError(i+1)*h_ratio_rebin->GetBinContent(i+1))/h_rebinMassW->GetBinContent(i+1),2)+pow(h_ratio_rebin->GetBinError(i+1)/h_ratio_rebin->GetBinContent(i+1),2)));
  } 
 
  double normM = float(h_rebinMass->Integral(10,22))/float(h_rebinMassW->Integral(10,22));

  TH1D* h_ratio_rebin_NORM=(TH1D*)h_rebinMassW->Clone();
  h_ratio_rebin_NORM->Divide(h_rebinMassW,h_rebinMass,normM,1.);

  //FILL the 1D correlation histograms
  for (int i=0;i<theNBins;++i){
      std::cout<<" i = "<<i<<" ratio = "<<h_ratio_rebin->GetBinContent(i+1)<<std::endl;
      for (int j=0;j<theNBins;++j){
        std::cout<<"Filling "<<h_ratio_rebin->GetBinContent(i+1) << " " << h_ratio_rebin->GetBinContent(j+1) <<std::endl;
        corrHist[i][j].corrPlot->Fill(h_ratio_rebin->GetBinContent(i+1),h_ratio_rebin->GetBinContent(j+1));
        corrHist[i][j].NORM_corrPlot->Fill(h_ratio_rebin_NORM->GetBinContent(i+1),h_ratio_rebin_NORM->GetBinContent(j+1));
    }
  }

/*
  for (int i=0;i<theNBins;++i){
      for (int j=0;j<theNBins;++j){
         for (int ix=0;ix<theEffBins;++ix){
            for (int jy=0;jy<theEffBins;++jy){
                cout << "Ij " << corrHist[i][j].corrPlot->GetBinContent(ix+1,jy+1) << endl;
            }
         }
      }
  }
*/

  //EXTRAS for 2D
  TH1D* h_YW_M20_30_pred   = histoMultiply3(recoidcorr_dd_2030,isocorr_dd_2030,trigcorr_dd_2030,false);
  TH1D* h_YW_M30_45_pred   = histoMultiply3(recoidcorr_dd_3045,isocorr_dd_3045,trigcorr_dd_3045,false);
  TH1D* h_YW_M45_60_pred   = histoMultiply3(recoidcorr_dd_4560,isocorr_dd_4560,trigcorr_dd_4560,false);
  TH1D* h_YW_M60_120_pred  = histoMultiply3(recoidcorr_dd_60120,isocorr_dd_60120,trigcorr_dd_60120,false);
  TH1D* h_YW_M120_200_pred = histoMultiply3(recoidcorr_dd_120200,isocorr_dd_120200,trigcorr_dd_120200,false);
  TH1D* h_YW_M200_1500_pred     = histoMultiply3(recoidcorr_dd_200,isocorr_dd_200,trigcorr_dd_200,false);
  
  TH1D* h_YW_M20_30 = (TH1D*)h_YW_M20_30_pred->Clone();
  for (int i = 0; i < h_YW_M20_30_pred->GetXaxis()->GetNbins(); i++) {
    h_YW_M20_30->SetBinContent(i+1,h_Y_M20_30->GetBinContent(i+1)*h_ratio_Y_M20_30->GetBinContent(i+1));
    h_YW_M20_30->SetBinError(i+1,0.);//h_YW_M20_30->GetBinContent(i+1)*sqrt(pow((h_YW_M20_30->GetBinError(i+1)*h_ratio_Y_M20_30->GetBinContent(i+1))/h_YW_M20_30->GetBinContent(i+1),2)+pow(h_ratio_Y_M20_30->GetBinError(i+1)/h_ratio_Y_M20_30->GetBinContent(i+1),2)));
  } 
  
  TH1D* h_YW_M30_45 = (TH1D*)h_YW_M30_45_pred->Clone();
  for (int i = 0; i < h_YW_M30_45_pred->GetXaxis()->GetNbins(); i++) {
    h_YW_M30_45->SetBinContent(i+1,h_Y_M30_45->GetBinContent(i+1)*h_ratio_Y_M30_45->GetBinContent(i+1));
    h_YW_M30_45->SetBinError(i+1,0.);//h_YW_M30_45->GetBinContent(i+1)*sqrt(pow((h_YW_M30_45->GetBinError(i+1)*h_ratio_Y_M30_45->GetBinContent(i+1))/h_YW_M30_45->GetBinContent(i+1),2)+pow(h_ratio_Y_M30_45->GetBinError(i+1)/h_ratio_Y_M30_45->GetBinContent(i+1),2)));
  }

  TH1D* h_YW_M45_60 = (TH1D*)h_YW_M45_60_pred->Clone();
  for (int i = 0; i < h_YW_M45_60_pred->GetXaxis()->GetNbins(); i++) {
    h_YW_M45_60->SetBinContent(i+1,h_Y_M45_60->GetBinContent(i+1)*h_ratio_Y_M45_60->GetBinContent(i+1));
    h_YW_M45_60->SetBinError(i+1,0.);//h_YW_M45_60->GetBinContent(i+1)*sqrt(pow((h_YW_M45_60->GetBinError(i+1)*h_ratio_Y_M45_60->GetBinContent(i+1))/h_YW_M45_60->GetBinContent(i+1),2)+pow(h_ratio_Y_M45_60->GetBinError(i+1)/h_ratio_Y_M45_60->GetBinContent(i+1),2)));
  }

  TH1D* h_YW_M60_120 = (TH1D*)h_YW_M60_120_pred->Clone();
  for (int i = 0; i < h_YW_M60_120_pred->GetXaxis()->GetNbins(); i++) {
    h_YW_M60_120->SetBinContent(i+1,h_Y_M60_120->GetBinContent(i+1)*h_ratio_Y_M60_120->GetBinContent(i+1));
    h_YW_M60_120->SetBinError(i+1,0.);//h_YW_M60_120->GetBinContent(i+1)*sqrt(pow((h_YW_M60_120->GetBinError(i+1)*h_ratio_Y_M60_120->GetBinContent(i+1))/h_YW_M60_120->GetBinContent(i+1),2)+pow(h_ratio_Y_M60_120->GetBinError(i+1)/h_ratio_Y_M60_120->GetBinContent(i+1),2)));
  }

  TH1D* h_YW_M120_200 = (TH1D*)h_YW_M120_200_pred->Clone();
  for (int i = 0; i < h_YW_M120_200_pred->GetXaxis()->GetNbins(); i++) {
    h_YW_M120_200->SetBinContent(i+1,h_Y_M120_200->GetBinContent(i+1)*h_ratio_Y_M120_200->GetBinContent(i+1));
    h_YW_M120_200->SetBinError(i+1,0.);//h_YW_M120_200->GetBinContent(i+1)*sqrt(pow((h_YW_M120_200->GetBinError(i+1)*h_ratio_Y_M120_200->GetBinContent(i+1))/h_YW_M120_200->GetBinContent(i+1),2)+pow(h_ratio_Y_M120_200->GetBinError(i+1)/h_ratio_Y_M120_200->GetBinContent(i+1),2)));
  }

  TH1D* h_YW_M200_1500 = (TH1D*)h_YW_M200_1500_pred->Clone();
  for (int i = 0; i < h_YW_M200_1500_pred->GetXaxis()->GetNbins(); i++) {
    h_YW_M200_1500->SetBinContent(i+1,h_Y_M200_1500->GetBinContent(i+1)*h_ratio_Y_M200_1500->GetBinContent(i+1));
    h_YW_M200_1500->SetBinError(i+1,0.);//h_YW_M200_1500->GetBinContent(i+1)*sqrt(pow((h_YW_M200_1500->GetBinError(i+1)*h_ratio_Y_M200_1500->GetBinContent(i+1))/h_YW_M200_1500->GetBinContent(i+1),2)+pow(h_ratio_Y_M200_1500->GetBinError(i+1)/h_ratio_Y_M200_1500->GetBinContent(i+1),2)));
  }

  TH1D* h_ratio_Y_M20_30_NORM=(TH1D*)h_YW_M20_30->Clone();
  h_ratio_Y_M20_30_NORM->Divide(h_YW_M20_30,h_Y_M20_30,normM,1.);
  TH1D* h_ratio_Y_M30_45_NORM=(TH1D*)h_YW_M30_45->Clone();
  h_ratio_Y_M30_45_NORM->Divide(h_YW_M30_45,h_Y_M30_45,normM,1.);
  TH1D* h_ratio_Y_M45_60_NORM=(TH1D*)h_YW_M45_60->Clone();
  h_ratio_Y_M45_60_NORM->Divide(h_YW_M45_60,h_Y_M45_60,normM,1.);
  TH1D* h_ratio_Y_M60_120_NORM=(TH1D*)h_YW_M60_120->Clone();
  h_ratio_Y_M60_120_NORM->Divide(h_YW_M60_120,h_Y_M60_120,normM,1.);
  TH1D* h_ratio_Y_M120_200_NORM=(TH1D*)h_YW_M120_200->Clone();
  h_ratio_Y_M120_200_NORM->Divide(h_YW_M120_200,h_Y_M120_200,normM,1.);
  TH1D* h_ratio_Y_M200_1500_NORM=(TH1D*)h_YW_M200_1500->Clone();
  h_ratio_Y_M200_1500_NORM->Divide(h_YW_M200_1500,h_Y_M200_1500,normM,1.);

   //FILL the 2D correlation histograms
  TH1D* h_ratio_YM_1;
  TH1D* h_ratio_YM_2;
  TH1D* h_ratio_YM_NORM_1;
  TH1D* h_ratio_YM_NORM_2;

  for (int i=0;i<the2D_Mbins;++i){
      switch (i){
      case 0:
        h_ratio_YM_1 = h_ratio_Y_M20_30;
        h_ratio_YM_NORM_1 = h_ratio_Y_M20_30_NORM;
        break;
      case 1:
        h_ratio_YM_1 = h_ratio_Y_M30_45;
        h_ratio_YM_NORM_1 = h_ratio_Y_M30_45_NORM;
        break;
      case 2:
        h_ratio_YM_1 = h_ratio_Y_M45_60;
        h_ratio_YM_NORM_1 = h_ratio_Y_M45_60_NORM;
        break;
      case 3:
        h_ratio_YM_1 = h_ratio_Y_M60_120;
        h_ratio_YM_NORM_1 = h_ratio_Y_M60_120_NORM;
        break;
      case 4:
        h_ratio_YM_1 = h_ratio_Y_M120_200;
        h_ratio_YM_NORM_1 = h_ratio_Y_M120_200_NORM;
        break;
      case 5:
        h_ratio_YM_1 = h_ratio_Y_M200_1500;
        h_ratio_YM_NORM_1 = h_ratio_Y_M200_1500_NORM;
        break;
      default:
        break;
      }
      for (int j=0;j<the2D_Ybins;++j){
        for (int k=0;k<the2D_Mbins;++k){
          switch (k){
          case 0:
            h_ratio_YM_2 = h_ratio_Y_M20_30;
            h_ratio_YM_NORM_2 = h_ratio_Y_M20_30_NORM;
            break;
          case 1:
            h_ratio_YM_2 = h_ratio_Y_M30_45;
            h_ratio_YM_NORM_2 = h_ratio_Y_M30_45_NORM;
            break;
          case 2:
            h_ratio_YM_2 = h_ratio_Y_M45_60;
            h_ratio_YM_NORM_2 = h_ratio_Y_M45_60_NORM;
            break;
          case 3:
            h_ratio_YM_2 = h_ratio_Y_M60_120;
            h_ratio_YM_NORM_2 = h_ratio_Y_M60_120_NORM;
            break;
          case 4:
            h_ratio_YM_2 = h_ratio_Y_M120_200;
            h_ratio_YM_NORM_2 = h_ratio_Y_M120_200_NORM;
            break;
          case 5:
            h_ratio_YM_2 = h_ratio_Y_M200_1500;
            h_ratio_YM_NORM_2 = h_ratio_Y_M200_1500_NORM;
            break;
          default:
            break;
          }
          for (int l=0;l<the2D_Ybins;++l){
            corrHist_2D[i][j][k][l].corrPlot->Fill(h_ratio_YM_1->GetBinContent(j+1),h_ratio_YM_2->GetBinContent(l+1));
            corrHist_2D[i][j][k][l].NORM_corrPlot->Fill(h_ratio_YM_NORM_1->GetBinContent(j+1),h_ratio_YM_NORM_2->GetBinContent(l+1));
          }
        }
      }//loop over 2D bins
    }


  //FINISH and SAVE
  std::cout<<" Finishing the job..."<<std::endl;
  const char* ftitle = "effCorrelations_step1"+filePostfix+"_"+TString(REPLICA)+".root";
  TFile* effFile = TFile::Open("/scratch/scratch95/a/asvyatko/correlReps/"+TString(ftitle), "RECREATE"); // my output file
  effFile->mkdir("1Dcorrel_histos");
  effFile->cd("1Dcorrel_histos");
  for (int i=0;i<theNBins;++i){
    for (int j=0;j<theNBins;++j){
      //cout << "Correlation factor " << i << " " << j << " " << corrHist[i][j].corrPlot->GetCorrelationFactor(1,2) << endl;
      corrHist[i][j].corrPlot->Write();
      //
      corrHist[i][j].NORM_corrPlot->Write();
    }
  }   
  effFile->cd();

  //    effFile->mkdir("2Dcorrel_histos");
  //    effFile->cd("2Dcorrel_histos");
  //
  std::string current_MY_bin;
  int mloc,nloc;
  for (int i=0;i<the2D_Mbins;++i){
    for (int j=0;j<the2D_Ybins;++j){
      mloc = i * the2D_Ybins + j;
      current_MY_bin = Form("2Dcorrel_histos_Mbin%d_Ybin%d",i+1,j+1);
      effFile->mkdir(current_MY_bin.c_str());
      effFile->cd(current_MY_bin.c_str());
      for (int k=0;k<the2D_Mbins;++k){
        for (int l=0;l<the2D_Ybins;++l){
          nloc = k * the2D_Ybins + l;
          corrHist_2D[i][j][k][l].corrPlot->Write();
          corrHist_2D[i][j][k][l].NORM_corrPlot->Write();
        }
      }
      //        effFile->cd("2Dcorrel_histos");
    }
  }
  effFile->cd();
  //
  effFile->Close();
  cout << "ROOT file with efficiency corrections for both 1D and 2D with Stoyan format are created" << endl;

}
