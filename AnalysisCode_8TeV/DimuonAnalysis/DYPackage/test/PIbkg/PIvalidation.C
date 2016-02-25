void PIvalidation(void) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

    TFile *f = new TFile("pi_background2D_CMS_NNPDF23.root");
    f->cd();
    TH1D* rap = (TH1D*)gDirectory->Get("pi_back_2d");
    TH1D* rap_DY = (TH1D*)gDirectory->Get("dy_2d");

    gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/histotools.C");
    //FIXME get slices first
    TH1D* h_dz_slice1 = get2DSlice(rap, "absrap2030",false,true);
    TH1D* h_dz_slice2 = get2DSlice(rap, "absrap3045",false,true);
    TH1D* h_dz_slice3 = get2DSlice(rap, "absrap4560",false,true);
    TH1D* h_dz_slice4 = get2DSlice(rap, "absrap60120",false,true);
    TH1D* h_dz_slice5 = get2DSlice(rap, "absrap120200",false,true);
    TH1D* h_dz_slice6 = get2DSlice(rap, "absrap200",false,true);
    h_dz_slice2->Smooth(300,"R");
    h_dz_slice3->Smooth(300,"R");
    h_dz_slice4->Smooth(300,"R");
    rap = sliceCombiner(h_dz_slice1,h_dz_slice2,h_dz_slice3,h_dz_slice4,h_dz_slice5,h_dz_slice6,true);


    //FIXME get slices first
    TH1D* h_dz_slic1 = get2DSlice(rap_DY, "absrap2030",false,true);
    TH1D* h_dz_slic2 = get2DSlice(rap_DY, "absrap3045",false,true);
    TH1D* h_dz_slic3 = get2DSlice(rap_DY, "absrap4560",false,true);
    TH1D* h_dz_slic4 = get2DSlice(rap_DY, "absrap60120",false,true);
    TH1D* h_dz_slic5 = get2DSlice(rap_DY, "absrap120200",false,true);
    TH1D* h_dz_slic6 = get2DSlice(rap_DY, "absrap200",false,true);
    h_dz_slic2->Smooth(300,"R");
    h_dz_slic3->Smooth(300,"R");
    h_dz_slic4->Smooth(300,"R");
    rap_DY = sliceCombiner(h_dz_slic1,h_dz_slic2,h_dz_slic3,h_dz_slic4,h_dz_slic5,h_dz_slic6,true);


    TH1D* gamma_contribution = (TH1D*)rap->Clone();
    gamma_contribution->Divide(rap,rap_DY);

    TH1D* h_dz1 = get2DSlice(gamma_contribution, "absrap2030",false,true);
    TH1D* h_dz2 = get2DSlice(gamma_contribution, "absrap3045",false,true);
    TH1D* h_dz3 = get2DSlice(gamma_contribution, "absrap4560",false,true);
    TH1D* h_dz4 = get2DSlice(gamma_contribution, "absrap60120",false,true);
    TH1D* h_dz5 = get2DSlice(gamma_contribution, "absrap120200",false,true);
    TH1D* h_dz6 = get2DSlice(gamma_contribution, "absrap200",false,true);

    //output the smoothed result
    TFile fout("pi_background2D_CMS_NNPDF23_smoothed.root","recreate");
    rap->Write("pi_back_2d");
    rap_DY->Write("dy_2d");
    fout.Close();

     plot2D(h_dz1,0);
     plot2D(h_dz2,1);
     plot2D(h_dz3,2);
     plot2D(h_dz4,3);
     plot2D(h_dz5,4);
     plot2D(h_dz6,5); 

    //1D case

    TFile *f2 = new TFile("pi_background1D_CMS_NNPDF23.root");
    f2->cd();
    TH1D* invm = (TH1D*)gDirectory->Get("pi_back_1d");
    TH1D* invm_DY = (TH1D*)gDirectory->Get("dy_1d");

    //invm->SetBinContent(16,0.92*invm->GetBinContent(15));
    //invm->SetBinContent(17,0.85*invm->GetBinContent(16));
    //invm->SetBinContent(18,0.75*invm->GetBinContent(15));
    //invm->Smooth();
    //invm->Draw("P");

    TH1D* gamma_contribution1D = (TH1D*)invm->Clone();
    gamma_contribution1D->Divide(invm,invm_DY);
    TCanvas* cgamma1D = new TCanvas("gamma_fraction_1D","gamma_fraction_1D");
    cgamma1D->cd();
    cgamma1D->SetLeftMargin(0.15);
    //gamma_contribution1D->GetYaxis()->SetRangeUser(0,0.008);
    //gamma_contribution1D->SetBinContent(40,(gamma_contribution1D->GetBinContent(39)+gamma_contribution1D->GetBinContent(41))/2.);
    //gamma_contribution1D->SetMarkerStyle(22);
    gamma_contribution1D->GetXaxis()->SetTitle("m [GeV]");
    gamma_contribution1D->GetYaxis()->SetTitle("#sigma(#gamma#gamma)/#sigma(DY)");
    gamma_contribution1D->GetXaxis()->SetMoreLogLabels();
    //gamma_contribution1D->SetLineColor(kViolet);
    //gamma_contribution1D->SetLineWidth(3);
    //gamma_contribution1D->SetMarkerSize(0);
    //gamma_contribution1D->SetFillColor(kViolet);

    for (int i = 0; i < gamma_contribution1D->GetNbinsX(); i++) {
      gamma_contribution1D->SetBinContent(i+1,gamma_contribution1D->GetBinContent(i+1)/2.1);
      gamma_contribution1D->SetBinError(i+1,gamma_contribution1D->GetBinError(i+1)/2.1);
    }

    gamma_contribution1D->Draw("P");  

   TLegend *leg= new TLegend(0.20,0.70,0.35,0.90);
   leg->SetTextFont(72);
   leg->SetTextSize(0.060);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->AddEntry(gamma_contribution1D,"FEWZ+NNPDF2.3 NNLO","lp");
   leg->Draw("same");

}


void plot2D(TH1D* h, int index) {

    TString beauty_whiches[6] = {"20-30 GeV","30-45 GeV","45-60 GeV","60-120 GeV","120-200 GeV","200-1500 GeV"};


    TCanvas* cgamma = new TCanvas("gamma_fraction"+TString(h->GetName()),"gamma_fraction"+TString(h->GetName()));
    cgamma->cd();
    cgamma->SetLeftMargin(0.15);
    //h->GetYaxis()->SetRangeUser(0,0.08);
    h->SetMarkerStyle(22);
    h->GetXaxis()->SetTitle("Mass-rapidity index");
    h->GetYaxis()->SetTitle("#sigma(#gamma#gamma)/#sigma(DY)");
    //h->SetLineColor(kViolet);
    //h->SetLineWidth(3);
    //h->SetMarkerSize(0);
    //h->SetFillColor(kViolet);

    for (int i = 0; i < h->GetNbinsX(); i++) {
      h->SetBinContent(i+1,h->GetBinContent(i+1)/2.1);
      h->SetBinError(i+1,h->GetBinError(i+1)/2.1);
    }

    h->Draw("P");

   TLegend *leg2= new TLegend(0.20,0.70,0.35,0.90);
   leg2->SetTextFont(72);
   leg2->SetTextSize(0.060);
   leg2->SetBorderSize(0);
   leg2->SetFillColor(0);
   leg2->AddEntry(h,"FEWZ+NNPDF2.3 NNLO","lp");
   leg2->Draw("same");

   TPaveText *pt = new TPaveText(0.65,0.70,0.85,0.90,"brNDC");
   pt->SetFillColor(0);
   pt->SetTextFont(72);
   pt->SetTextSize(0.060);
   pt->SetBorderSize(0);
   pt->SetTextAlign(12);
  pt->AddText(beauty_whiches[index]);
  pt->Draw("same");

}
