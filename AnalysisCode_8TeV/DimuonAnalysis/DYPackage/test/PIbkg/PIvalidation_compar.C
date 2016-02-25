void PIvalidation_compar(void) {

    TFile *f = new TFile("pi_background2D_CMS.root");
    f->cd();
    TH1D* rap = (TH1D*)gDirectory->Get("pi_back_2d");

    gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/histotools.C");
    //FIXME get slices first
    TH1D* h_dz_slice1 = get2DSlice(rap, "absrap2030",false,true);
    TH1D* h_dz_slice2 = get2DSlice(rap, "absrap3045",false,true);
    TH1D* h_dz_slice3 = get2DSlice(rap, "absrap4560",false,true);
    TH1D* h_dz_slice4 = get2DSlice(rap, "absrap60120",false,true);
    TH1D* h_dz_slice5 = get2DSlice(rap, "absrap120200",false,true);
    TH1D* h_dz_slice6 = get2DSlice(rap, "absrap200",false,true);

    h_dz_slice4->Smooth(300,"R");

    rap = sliceCombiner(h_dz_slice1,h_dz_slice2,h_dz_slice3,h_dz_slice4,h_dz_slice5,h_dz_slice6,true);

    //convert to shape
    for (int i = 0; i < 132; i++) {
       rap->SetBinContent(i+1,rap->GetBinContent(i+1)/rap->GetBinWidth(i+1));
       rap->SetBinError(i+1,rap->GetBinError(i+1)/rap->GetBinWidth(i+1));
    }

    TCanvas* cg = new TCanvas("gamma_absolute","gamma_absolute");
    cg->cd();
    rap->SetMarkerStyle(22);
    rap->GetXaxis()->SetTitle("Mass-rapidity index");
    rap->GetYaxis()->SetTitle("#sigma(#gamma#gamma) [pb]"); 
    rap->GetYaxis()->SetRangeUser(0,0.05);
    rap->Draw("P");

    TFile fout("pi_background2D_CMS_smoothed.root","recreate");
    rap->Write("pi_back_2d");
    fout.Close();

    cout << "The below number needs to have shpeR2D.C rerun if the above changed!" << endl;

    TFile* f2 = new TFile("pi_fraction.root");
    f2->cd();
    TH1D* gamma_contribution = (TH1D*)gDirectory->Get("pi_fraction");
    gamma_contribution->SetBinContent(119,gamma_contribution->GetBinContent(119)/2.);
    gamma_contribution->SetBinContent(120,gamma_contribution->GetBinContent(120)/2.);

    TCanvas* cgamma = new TCanvas("gamma_fraction","gamma_fraction");
    cgamma->cd();
    cgamma->SetLeftMargin(0.15);
    gamma_contribution->GetYaxis()->SetRangeUser(0,0.008);
    gamma_contribution->SetMarkerStyle(22);
    gamma_contribution->GetXaxis()->SetTitle("Mass-rapidity index");
    gamma_contribution->GetYaxis()->SetTitle("#sigma(#gamma#gamma)/#sigma(DY)");
    gamma_contribution->Draw("P");

/*
    //1D case

    TFile *f2 = new TFile("pi_background1D_CMS.root");
    f2->cd();
    TH1D* invm = (TH1D*)gDirectory->Get("pi_back_1d");

    //get the other one
    TFile *f2b = new TFile("pi_background1D_CMS_NNPDF23.root");
    f2b->cd();
    TH1D* invm2 = (TH1D*)gDirectory->Get("pi_back_1d");

    TCanvas* cg2 = new TCanvas("gamma_absolute_1D","gamma_absolute_1D");
    cg2->cd();
    invm->SetMarkerStyle(22);
    invm->GetXaxis()->SetMoreLogLabels();
    invm->GetXaxis()->SetTitle("m [GeV]");
    invm->GetYaxis()->SetTitle("#sigma(#gamma#gamma) [pb]");
    invm2->SetMarkerStyle(22);
    invm2->SetMarkerColor(kBlue);
    invm2->SetLineColor(kBlue);
    //invm->GetYaxis()->SetRangeUser(0,0.05);

    //convert to shape
    for (int i = 0; i < 41; i++) {
       invm->SetBinContent(i+1,invm->GetBinContent(i+1)/invm->GetBinWidth(i+1));
       invm->SetBinError(i+1,invm->GetBinError(i+1)/invm->GetBinWidth(i+1));
       invm2->SetBinContent(i+1,invm2->GetBinContent(i+1)/invm2->GetBinWidth(i+1));
       invm2->SetBinError(i+1,invm2->GetBinError(i+1)/invm2->GetBinWidth(i+1));
    }
    invm->SetBinContent(16,0.92*invm->GetBinContent(15));
    invm->SetBinContent(17,0.85*invm->GetBinContent(16));
    invm->SetBinContent(18,0.75*invm->GetBinContent(15));
    invm->Smooth();
    invm->Draw("P");
    invm2->Draw("Psame");

    TFile* f3 = new TFile("../../Outputs/absex_full.root");
    f3->cd();
    TH1D* hxsec = (TH1D*)gDirectory->Get("hxsec");
    TH1D* gamma_contribution1D = (TH1D*)hxsec->Clone();
    gamma_contribution1D->Divide(invm,hxsec);

    TH1D* gamma_contribution1D_2 = (TH1D*)hxsec->Clone();
    gamma_contribution1D_2->Divide(invm2,hxsec);

    TCanvas* cgamma1D = new TCanvas("gamma_fraction_1D","gamma_fraction_1D");
    cgamma1D->cd();
    cgamma1D->SetLeftMargin(0.15);
    gamma_contribution1D->GetYaxis()->SetRangeUser(0,0.8);
    gamma_contribution1D->SetBinContent(40,(gamma_contribution1D->GetBinContent(39)+gamma_contribution1D->GetBinContent(41))/2.);
    gamma_contribution1D->SetMarkerStyle(22);
    gamma_contribution1D->GetXaxis()->SetTitle("m [GeV]");
    gamma_contribution1D->GetYaxis()->SetTitle("#sigma(#gamma#gamma)/#sigma(DY)");
    gamma_contribution1D->GetXaxis()->SetMoreLogLabels();
    gamma_contribution1D->Draw("P");  
    gamma_contribution1D_2->SetMarkerStyle(22);
    gamma_contribution1D_2->SetMarkerColor(kBlue);
    gamma_contribution1D_2->SetLineColor(kBlue);
    gamma_contribution1D_2->Draw("Psame");

   TLegend *leg= new TLegend(0.20,0.70,0.35,0.90);
   leg->SetTextFont(72);
   leg->SetTextSize(0.060);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->AddEntry(gamma_contribution1D,"MRST2004QED","lp");
   leg->AddEntry(gamma_contribution1D_2,"NNPDF2.3QED","lp");
   leg->Draw("same");
*/

}
