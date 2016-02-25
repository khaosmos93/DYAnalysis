void quickPlotter() {

    TString histos[8] = {"unfoldCov_TM","effCov_TM","fsrCov_TM","totalCov_TM","unfoldCorr_TM","effCorr_TM","fsrCorr_TM","totalCorr_TM"};

    TFile* f = new TFile("covariance_finalResults_1D_mu_preFSR_fullAcc_normalized.root");
    f->cd();
    gStyle->SetPalette(1);

    for (int i = 0; i < 8; i++) {
      TCanvas* c = new TCanvas(); 
      c->cd(); 
      TMatrixT <double> *h  = (TMatrixT <double> *)gDirectory->Get(histos[i]);
      TH2D *h2 = new TH2D (*h);

      h2->SetStats(kFALSE);
      h2->GetXaxis()->SetMoreLogLabels();
      h2->GetXaxis()->SetTitle("m [GeV]");
      h2->GetYaxis()->SetTitle("m [GeV]");
      h2->Draw("COLZ");
      c->SaveAs(histos[i]+".png");
    }

}
