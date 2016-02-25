void validator1D_2D() {

  //target validation binning
  double binsx[7] = {20,30,45,60,120,200,1500};
  TH1D* h2 = new TH1D("rebin2d","rebin2d",6,binsx);

  //get 1D
  TFile* f1 = new TFile("missingQCD.root");
  f1->cd();
  //get histo
  TH1D* datahistv = (TH1D*)gDirectory->Get("datahistv");
  //rebin
  datahistv->Rebin(6,"rebin1d",binsx);
  TH1D* h1 = (TH1D*)gDirectory->Get("rebin1d");

  TFile* f2 = new TFile("missingQCD_2D.root");
  f2->cd();
  //get histo
  TH1D* data_dmdyHistv20 = (TH1D*)gDirectory->Get("data_dmdyHistv20");
  TH1D* data_dmdyHistv30 = (TH1D*)gDirectory->Get("data_dmdyHistv30");
  TH1D* data_dmdyHistv45 = (TH1D*)gDirectory->Get("data_dmdyHistv45");
  TH1D* data_dmdyHistv60 = (TH1D*)gDirectory->Get("data_dmdyHistv60");
  TH1D* data_dmdyHistv120 = (TH1D*)gDirectory->Get("data_dmdyHistv120");
  TH1D* data_dmdyHistv200 = (TH1D*)gDirectory->Get("data_dmdyHistv200");
  TH1D* data_dmdyHistv1500 = (TH1D*)gDirectory->Get("data_dmdyHistv1500");
  
  h2->SetBinContent(1,data_dmdyHistv30->Integral());
  h2->SetBinError(1,0);
  h2->SetBinContent(2,data_dmdyHistv45->Integral());
  h2->SetBinError(2,0);
  h2->SetBinContent(3,data_dmdyHistv60->Integral());
  h2->SetBinError(3,0);
  h2->SetBinContent(4,data_dmdyHistv120->Integral());
  h2->SetBinError(4,0);
  h2->SetBinContent(5,data_dmdyHistv200->Integral());
  h2->SetBinError(5,0);
  h2->SetBinContent(6,data_dmdyHistv1500->Integral());
  h2->SetBinError(6,0);


  TCanvas* c = new TCanvas();
  c->cd();
  h1->GetXaxis()->SetTitle("m [GeV]");
  h1->GetYaxis()->SetTitle("Events");
  h1->SetMarkerStyle(22);
  h1->Draw("P"); 
  h2->SetMarkerStyle(21);
  h2->SetMarkerColor(kRed);
  h2->Draw("Psame");
  cout << h1->Integral() << " " << h2->Integral() << endl;

}
