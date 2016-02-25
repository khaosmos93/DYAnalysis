void plotYieldRatio() {

  TFile* f = new TFile("rootfiles/unfoldData.root");
  f->cd();
  TH1D* preunfolded = (TH1D*)hmeas->Clone();
  TH1D* unfolded = (TH1D*)hunfolded->Clone();

  TH1D* ratio = (TH1D*)unfolded->Clone();
  ratio->Divide(preunfolded,unfolded);

  TCanvas* c1 = new TCanvas("ratio_unf1D","ratio_unf1D");
  c1->cd();
  ratio->GetYaxis()->SetRangeUser(-0.2,1.5);
  ratio->SetMarkerStyle(22);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
  ratio->GetYaxis()->SetTitle("Pre-unfolded/unfolded");
  ratio->Draw("P");
  c1->SaveAs("ratio_unf1D.png"); 

  TFile* f2 = new TFile("rootfiles/unfoldData_HLT_Double_2D.root");
  f2->cd();
  TH1D* preunfolded2 = (TH1D*)hmeas->Clone();
  TH1D* unfolded2 = (TH1D*)hunfolded->Clone();

  TH1D* ratio2 = (TH1D*)unfolded2->Clone();
  ratio2->Divide(preunfolded2,unfolded2);

  TCanvas* c2 = new TCanvas("ratio_unf2D","ratio_unf2D");
  c2->cd();  
  ratio2->GetYaxis()->SetRangeUser(0.35,1.3);
  ratio2->SetMarkerStyle(22);
  ratio2->GetXaxis()->SetMoreLogLabels();
  ratio2->GetXaxis()->SetTitle("dimuon rapidity for each mass");
  ratio2->GetYaxis()->SetTitle("Pre-unfolded/unfolded");
  ratio2->Draw("P");
  c2->SaveAs("ratio_unf2D.png");
}
