void ABCD2vari_init(const char* WHICHHIST, const char* ISO, const char* REGION) { //Only iso is external, all the regions go in a loop!!! to be fed in the macro at once


  TString PREFIX = "./";

  //repeat for region C
  TFile* f = new TFile(PREFIX+"QCDFrac_"+TString(ISO)+"_"+REGION+".root", "open");
  f->cd();
  TH1D* sig = (TH1D*)gDirectory->Get("sig"+TString(WHICHHIST));
  TH1D* ewk = (TH1D*)gDirectory->Get("ewk"+TString(WHICHHIST));
  TH1D* tau = (TH1D*)gDirectory->Get("tau"+TString(WHICHHIST));
  TH1D* diboson = (TH1D*)gDirectory->Get("diboson"+TString(WHICHHIST));
  TH1D* ttbar = (TH1D*)gDirectory->Get("ttbar"+TString(WHICHHIST));
  TH1D* data = (TH1D*)gDirectory->Get("data"+TString(WHICHHIST));

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

  TH1D* hRatio = (TH1D*)hNum->Clone();
  hRatio->Divide(hNum, hData);
  hRatio->Fit("pol0");

  //we will need ratio C as a function of invariant mass
  TFile* fout = new TFile("./factor_qcd_"+TString(ISO)+"_"+REGION+"_tmp_"+TString(WHICHHIST)+".root", "recreate");
  fout->cd();
  hRatio->SetName("hfactor"+TString(WHICHHIST));
  hRatio->Write();
  fout->Close();
  //FIXME all the _tmp_ histos are to be hadded afterwards!
}
