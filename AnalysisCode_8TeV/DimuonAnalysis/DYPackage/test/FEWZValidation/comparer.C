void comparer(void) {

   TString bins[7] = {"20","30","45","60","120","200","1500"};

   //FULL SIGMAS
   //double sigmasNLO[7] = {553.268136238061,195.259028308450};
   //double sigmasNNLO[7] = {549.539,195.259028308450,};

   for (int i = 0; i < 6; i++) {

    TFile* f = new TFile("outmptmp"+bins[i]+"to"+bins[i+1]+".dat_NLO.root");
    f->cd();
    TH1D* NLO = (TH1D*)xsec->Clone();

    TFile* g = new TFile("outmptmp"+bins[i]+"to"+bins[i+1]+".dat_NNLO.root");
    g->cd();
    TH1D* NNLO = (TH1D*)xsec->Clone();

    TCanvas* c = new TCanvas(bins[i]+"to"+bins[i+1],bins[i]+"to"+bins[i+1]);
    NLO->SetMarkerStyle(20);
    NLO->GetXaxis()->SetMoreLogLabels();
    NLO->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
    NNLO->SetMarkerStyle(20);
    NNLO->SetMarkerColor(kRed);
    NLO->Draw("P");
    NNLO->Draw("Psame");
    cout << NLO->Integral() << " " << NNLO->Integral() << endl; 

   
    //SAVE NEW ROOT FILES FOR NNLO
    //TFile* fout_new = new TFile("2Dabsxsec"+bins[i]+"to"+bins[i+1]+"_NNLO_plain_CTEQ12NNLO.root","recreate");
    //fout_new->cd();
    //NLO->Scale(sigmasNNLO[i]/sigmasNLO[i]);    


   }


}
