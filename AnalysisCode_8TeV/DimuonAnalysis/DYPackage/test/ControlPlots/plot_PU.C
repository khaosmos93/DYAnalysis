void plot_PU(void) {

  TFile* f = new TFile("TT_CP.root");
  f->cd();
  TH1D* ratio = (TH1D*)invm_PU->Clone();
  invm_PU->Sumw2();
  invm_Roch->Sumw2();
  ratio->Divide(invm_PU,invm_Roch);
  ratio->SetMarkerStyle(22);
  ratio->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
  ratio->GetYaxis()->SetTitle("N_{PU corr}/N");
  ratio->GetXaxis()->SetMoreLogLabels();
  //ratio->Smooth(10,"R");
  ratio->Draw();

}
