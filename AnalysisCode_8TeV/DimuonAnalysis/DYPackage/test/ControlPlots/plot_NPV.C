void plot_NPV() {

  TFile* f = new TFile("pileup.root");
  f->cd();
  TH1D* npv_data = (TH1D*)gDirectory->Get("nvtx_data");
  TH1D* npv_mc = (TH1D*)gDirectory->Get("nvtx_mc");
  
  TCanvas* c = new TCanvas();
  c->cd();
  npv_data->SetMarkerStyle(22);
  npv_data->GetXaxis()->SetTitle("N_{PV}");
  npv_data->GetYaxis()->SetTitle("Events");
  npv_mc->SetMarkerStyle(22);
  npv_mc->SetMarkerColor(kRed);
  npv_data->Draw("P");
  npv_mc->Draw("Psame");

}
