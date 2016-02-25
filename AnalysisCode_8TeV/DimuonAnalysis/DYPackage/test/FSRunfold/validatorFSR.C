void validatorFSR(void) {

   TString REGIME = ""; //_corr

    TFile* f = new TFile("SizeOfCorrection"+REGIME+".root");
    f->cd();
    TH1D* preunfolded = (TH1D*)hpreunfolded->Clone();
    TH1D* unfolded = (TH1D*)hunfolded->Clone();
    if (REGIME == "_corr") REGIME = "corr";
    TH1D* corr1 = (TH1D*)gDirectory->Get("heff1D_1"+REGIME);
    TH1D* corr2 = (TH1D*)gDirectory->Get("heff1D_2"+REGIME);
    unfolded->Divide(unfolded,corr1);
    unfolded->Divide(unfolded,corr2);
    unfolded->Divide(hpreunfolded,unfolded);

    for (int i = 0; i < 40; i++) {
      unfolded->SetBinError(i+1, 0.000001);
    } 

    TFile* g = new TFile("BinByBin.root");
    g->cd();
    if (REGIME == "corr") REGIME = "_corr";
    TH1D* binBybin = (TH1D*)gDirectory->Get("hpreFSR"+REGIME);
    TH1D* postFSR = (TH1D*)gDirectory->Get("hpostFSR"+REGIME); 
    binBybin->Divide(postFSR,binBybin);

    unfolded->SetMarkerStyle(22);
    unfolded->GetXaxis()->SetMoreLogLabels();
    unfolded->Draw();
    binBybin->SetMarkerStyle(22);
    binBybin->SetMarkerColor(kRed);
    binBybin->Draw("same"); 

}
