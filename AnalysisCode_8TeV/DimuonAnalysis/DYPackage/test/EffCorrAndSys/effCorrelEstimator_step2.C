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
void effCorrelEstimator_step2() {

  TString corrMode = "_split"; //"FlatHalfPercent"; //"40pertest"; //"Split";

  float effLimit_low = 0.8;
  float effLimit_high = 1.2;
  const int theEffBins = 80;

  const int theNBins = 40;
  Double_t mBins[theNBins+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};
  const int the2D_Mbins = 6;
  const int the2D_Ybins = 24;
  int the2D_MYbins = the2D_Mbins * the2D_Ybins;

  Double_t mBins_2D[the2D_Mbins+1] = {20,30, 45, 60, 120, 200, 1500};
  Double_t yBins_2D[the2D_Ybins+1] = {0.,  0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
                                      1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                                      2.0, 2.1, 2.2, 2.3, 2.4};

  //
  TH2D *effCorrelations = new TH2D("effCorrelations", "effCorrelations; mass, GeV; mass, GeV", theNBins, mBins, theNBins, mBins);
  TH2D *effCorrelations_bins = new TH2D("effCorrelations_bins", "effCorrelations_bins; mass bin; mass bin", theNBins, 0.5, theNBins+0.5, theNBins, 0.5, theNBins+0.5 );
  TH2D *effCorrelations_NORM = new TH2D("effCorrelations_NORM", "effCorrelations_NORM; mass, GeV; mass, GeV", theNBins, mBins, theNBins, mBins);
  TH2D *effCorrelations_bins_NORM = new TH2D("effCorrelations_bins_NORM", "effCorrelations_bins_NORM; mass bin; mass bin", theNBins, 0.5, theNBins+0.5, theNBins, 0.5, theNBins+0.5 );

  //TH2D *effCorrelations_2D = new TH2D("effCorrelations", "effCorrelations; mass, GeV; mass, GeV", theNBins, mBins, theNBins, mBins);
  TH2D *effCorrelations_2D_bins = new TH2D("effCorrelations_2D_bins", "effCorrelations_2D_bins; mass+Y bin index; mass+Y bin index", the2D_MYbins, 0.5, the2D_MYbins+0.5, the2D_MYbins, 0.5, the2D_MYbins+0.5 );
  //TH2D *effCorrelations_NORM = new TH2D("effCorrelations_NORM", "effCorrelations_NORM; mass, GeV; mass, GeV", theNBins, mBins, theNBins, mBins);
  TH2D *effCorrelations_2D_bins_NORM = new TH2D("effCorrelations_2D_bins_NORM", "effCorrelations_2D_bins_NORM; mass+Y bin index; mass+Y bin index", the2D_MYbins, 0.5, the2D_MYbins
+0.5, the2D_MYbins, 0.5, the2D_MYbins+0.5 );
  //


  //FINISH and SAVE
  std::cout<<" Finishing the job..."<<std::endl;
  //Open file with a replica i
  TFile* f = new TFile("/scratch/scratch95/a/asvyatko/correlReps/1.root");
  f->cd();
  string specName = "";
  string specName_NORM = "";  
  for (int i=0;i<theNBins;++i){
    for (int j=0;j<theNBins;++j){
      specName = Form("massBins_%d_%d",i+1,j+1);
      specName_NORM = Form("massBins_NORM_%d_%d",i+1,j+1); 
      TH2D* this_corrPlot = (TH2D*)gDirectory->Get("1Dcorrel_histos/"+TString(specName));
      TH2D* this_corrPlot_NORM = (TH2D*)gDirectory->Get("1Dcorrel_histos/"+TString(specName_NORM));

      effCorrelations->Fill(mBins[i],mBins[j],this_corrPlot->GetCorrelationFactor(1,2));
      effCorrelations_bins->Fill(i+1,j+1,this_corrPlot->GetCorrelationFactor(1,2));
      //
      effCorrelations_NORM->Fill(mBins[i],mBins[j],this_corrPlot_NORM->GetCorrelationFactor(1,2));
      effCorrelations_bins_NORM->Fill(i+1,j+1,this_corrPlot_NORM->GetCorrelationFactor(1,2));
    }
  }   

  //    effFile->mkdir("2Dcorrel_histos");
  //    effFile->cd("2Dcorrel_histos");
  //
  std::string current_MY_bin;
  int mloc,nloc;
  for (int i=0;i<the2D_Mbins;++i){
    for (int j=0;j<the2D_Ybins;++j){
      mloc = i * the2D_Ybins + j;
      current_MY_bin = Form("2Dcorrel_histos_Mbin%d_Ybin%d",i+1,j+1);
      for (int k=0;k<the2D_Mbins;++k){
        for (int l=0;l<the2D_Ybins;++l){
          nloc = k * the2D_Ybins + l;
          specName = Form("massYBins_%d_%d__%d_%d",i+1,j+1,k+1, l+1);
          specName_NORM = Form("massYBins_NORM_%d_%d__%d_%d",i+1,j+1,k+1, l+1);
          cout << TString(current_MY_bin)+"/"+TString(specName) << endl;
          cout << TString(current_MY_bin)+"/"+TString(specName_NORM) << endl;
          cout << i << " " << j << " " << k << " " << l<< endl;
          TH2D* this_corrPlot2D = (TH2D*)gDirectory->Get(TString(current_MY_bin)+"/"+TString(specName));
          TH2D* this_corrPlot2D_NORM = (TH2D*)gDirectory->Get(TString(current_MY_bin)+"/"+TString(specName_NORM));

          effCorrelations_2D_bins->Fill(mloc+1,nloc+1,this_corrPlot2D->GetCorrelationFactor(1,2));
          effCorrelations_2D_bins_NORM->Fill(mloc+1,nloc+1,this_corrPlot2D_NORM->GetCorrelationFactor(1,2));
        }
      }
      //        effFile->cd("2Dcorrel_histos");
    }
  }

  f->Close();
  TFile* effFile = new TFile("2"+corrMode+".root","RECREATE");
  effFile->cd();
  //
  effCorrelations->Write();
  effCorrelations_bins->Write();
  effCorrelations_NORM->Write();
  effCorrelations_bins_NORM->Write();
  effCorrelations_2D_bins->Write();
  effCorrelations_2D_bins_NORM->Write();
  effFile->Close();
  cout << "ROOT file with efficiency corrections for both 1D and 2D with Stoyan format are created" << endl;

}
