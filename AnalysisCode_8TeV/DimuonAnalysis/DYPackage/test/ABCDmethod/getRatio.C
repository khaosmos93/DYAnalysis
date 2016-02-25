void getRatio() { //const char* ISO, const char* REGION)

//2. run getRatio.C
//- Need to produce
//mu1iso B, C, D
//mu2iso B, C, D
//totally 6 correction factors


  TString REGION = "C";
  TString ISO = "mu1iso";

  //FIXME
  //1iso B, D is empty
  //2iso B, D,

  //gStyle->SetOptStat(0);
  TFile* f = new TFile("QCDFrac_"+ISO+"_"+REGION+".root", "open");
  f->cd();
  //Stack up all the MC 
  TH1D* htot = (TH1D*)sig->Clone();
  htot->Add(ewk);
  htot->Add(tau);
  htot->Add(diboson);
  htot->Add(ttbar);
  htot->Sumw2();
  
  TH1D* hData = (TH1D*) data->Clone();
  hData->Sumw2();
  
  TH1D* hNum = (TH1D*) data->Clone();
  hNum->Add(htot, -1);
  
  TFile* fout = new TFile("test_factor_qcd_"+ISO+"_"+REGION+".root", "recreate");
  fout->cd();
  TH1D* hRatio = (TH1D*)hNum->Clone();
  hRatio->Divide(hNum, hData);
  hRatio->Fit("pol0");
  hRatio->SetName("hfactor");
  fout->Write();

  TF1 *myfit = (TF1*)hRatio->GetFunction("pol0");
  Double_t factor_B = myfit->GetParameter(0);
  Double_t factor_B_err = myfit->GetParError(0);

  cout << factor_B << " " << factor_B_err<< endl;

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  c1->SetLogx();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  hRatio->GetXaxis()->SetTitle("Dimuon Invariant Mass");
  hRatio->Draw("ep");
  //c1->Print("./eps/factor_qcd_"+ISO+"_"+REGION+".png");
}
