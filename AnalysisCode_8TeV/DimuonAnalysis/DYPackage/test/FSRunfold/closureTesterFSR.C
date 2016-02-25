void closureTesterFSR(void) {

   TString REGIME = ""; //_corr

    //2D case
    TFile* g = new TFile("../Inputs/unfold/unfoldData_HLT_Double_2D_NoTRM.root");
    g->cd();
    TH1F* unfolded = (TH1F*)gDirectory->Get("hunfolded");
    //FSR correction
    TFile* gg = new TFile("../Inputs/FSR/BinByBin.root"); 
    gg->cd();
    TH1F* FSRcorr_num = (TH1F*)gDirectory->Get("hpostFSR2");
    TH1F* FSRcorr_den = (TH1F*)gDirectory->Get("hpreFSR2");
    TH1F* FSRcorr = (TH1F*)FSRcorr_num->Clone();
    FSRcorr->Divide(FSRcorr_num,FSRcorr_den);
    for (int i = 0; i < 132; i++) {
       unfolded->SetBinContent(i+1,unfolded->GetBinContent(i+1)*FSRcorr->GetBinContent(i+1));
    }  
    unfolded->SetMarkerStyle(22);
    unfolded->GetXaxis()->SetTitle("Bin number");
    unfolded->GetYaxis()->SetTitle("Events");
    unfolded->Draw("hist");
    TH1D* other = (TH1D*)unfolded->Clone();
    other->SetMarkerStyle(22);
    other->SetMarkerColor(kRed);
    other->SetLineColor(kRed);
    other->Draw("Psame"); 

    TCanvas* c = new TCanvas();
    c->cd();
    TH1D* ratio = (TH1D*)unfolded->Clone();
    ratio->Divide(other,unfolded);
    ratio->SetMarkerStyle(22);
    ratio->GetXaxis()->SetTitle("N_{unfolded}/N_{preunfolded}");
    ratio->GetXaxis()->SetTitle("Bin number");
    ratio->Draw("P");  


    
}
