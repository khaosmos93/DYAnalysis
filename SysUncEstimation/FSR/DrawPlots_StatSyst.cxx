TH1D* QuadratureSum_Unc( TH1D* h1, TH1D* h2, TString HistName );

void DrawPlots_StatSyst()
{
	TString BaseLocation = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20160731_1st_ApplyEffSF_MCBkg";

	TFile *f_stat = TFile::Open(BaseLocation+"/ROOTFile_SysUnc_FSR.root"); f_stat->cd();
	TH1D* h_stat = (TH1D*)f_stat->Get("h_RelSysUnc_FSRCorr_FiniteMC_Percent")->Clone();

	TFile *f_syst = TFile::Open("./ROOTFile_SysUnc_FSR_Modeling.root"); f_syst->cd();
	TH1D* h_syst = (TH1D*)f_syst->Get("h_RelSysUnc_FSRCorr_AltMC_Percent")->Clone();


	// -- total -- //
	TH1D* h_tot = QuadratureSum_Unc( h_stat, h_syst, "h_RelSysUnc_Tot" );

	// -- draw plot -- //
	TCanvas *c1 = new TCanvas("c_tot", "", 800, 800);
	c1->cd();
	gPad->SetBottomMargin(0.12);
	gPad->SetLeftMargin(0.12);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.04);

	gPad->SetLogx();

	h_tot->Draw("AXIS");
	h_stat->Draw("HISTLPSAME");
	h_syst->Draw("HISTLPSAME");
	h_tot->Draw("HISTLPSAME");

	h_tot->SetStats(kFALSE);
	h_tot->SetLineColor(kRed);
	h_tot->SetMarkerStyle(20);
	h_tot->SetMarkerColor(kRed);
	h_tot->SetFillColorAlpha( kWhite, 0 );

	h_stat->SetStats(kFALSE);
	h_stat->SetLineColor(kBlack);
	h_stat->SetMarkerStyle(20);
	h_stat->SetMarkerColor(kBlack);
	h_stat->SetFillColorAlpha( kWhite, 0 );

	h_syst->SetStats(kFALSE);
	h_syst->SetLineColor(kBlue);
	h_syst->SetMarkerStyle(20);
	h_syst->SetMarkerColor(kBlue);
	h_syst->SetFillColorAlpha( kWhite, 0 );

	// -- axis settings -- //
	h_tot->SetXTitle("m (#mu#mu) [GeV]");
	h_tot->SetYTitle("Rel. Unc. (%)");

	h_tot->GetXaxis()->SetNoExponent();
	h_tot->GetXaxis()->SetMoreLogLabels();
	h_tot->GetXaxis()->SetTitleOffset(1.4);
	h_tot->GetYaxis()->SetTitleOffset(1.4);
	h_tot->SetMinimum(0);

	TLegend *legend1 = new TLegend(0.15, 0.75, 0.55, 0.95);
	legend1->SetBorderSize(0);
	legend1->SetFillStyle(0);
	legend1->SetTextFont(62);
	legend1->AddEntry( h_tot, "Total Unc. from FSR" );
	legend1->AddEntry( h_stat, "Stat.");
	legend1->AddEntry( h_syst, "Syst.");
	legend1->Draw();

	TLatex latex;
	Double_t Lumi = 2832.673;
	latex.DrawLatexNDC(0.68, 0.96, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi / 1000.0) );
	latex.DrawLatexNDC(0.125, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.235, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

	c1->SaveAs(".pdf");


	// // -- draw plot: stat, syst, and total uncertainty -- //
	// TCanvas *c2 = new TCanvas("c_tot", "", 800, 800);
	// c2->cd();
	// gPad->SetBottomMargin(0.12);
	// gPad->SetLeftMargin(0.12);
	// gPad->SetTopMargin(0.05);
	// gPad->SetRightMargin(0.04);

	// gPad->SetLogx();

	// h_tot->Draw("AXIS");
	// h_stat->Draw("HISTLPSAME");
	// h_syst_tot->Draw("HISTLPSAME");
	// h_tot->Draw("HISTLPSAME");

	// h_tot->SetStats(kFALSE);
	// h_tot->SetLineColor(kRed);
	// h_tot->SetMarkerStyle(20);
	// h_tot->SetMarkerColor(kRed);
	// h_tot->SetFillColorAlpha( kWhite, 0 );

	// h_stat->SetStats(kFALSE);
	// h_stat->SetLineColor(kBlack);
	// h_stat->SetMarkerStyle(20);
	// h_stat->SetMarkerColor(kBlack);
	// h_stat->SetFillColorAlpha( kWhite, 0 );

	// // h_syst_tot->SetStats(kFALSE);
	// // h_syst_tot->SetLineColor(kBlue+1);
	// // h_syst_tot->SetMarkerStyle(20);
	// // h_syst_tot->SetMarkerColor(kBlue+1);
	// // h_syst_tot->SetFillColorAlpha( kWhite, 0 );

	// // -- axis settings -- //
	// h_tot->SetXTitle("m (#mu#mu) [GeV]");
	// h_tot->SetYTitle("Rel. Unc. (%)");

	// h_tot->GetXaxis()->SetNoExponent();
	// h_tot->GetXaxis()->SetMoreLogLabels();
	// h_tot->GetXaxis()->SetTitleOffset(1.4);
	// h_tot->GetYaxis()->SetTitleOffset(1.4);
	// h_tot->SetMinimum(0);

	// TLegend *legend2 = new TLegend(0.55, 0.80, 0.95, 0.95);
	// legend2->SetBorderSize(0);
	// legend2->SetFillStyle(0);
	// legend2->SetTextFont(62);
	// legend2->AddEntry( h_tot, "Total Unc. from Eff.SF" );
	// legend2->AddEntry( h_stat, "Stat.");
	// legend2->AddEntry( h_syst_tot, "Total Syst.");
	// legend2->Draw();

	// latex.DrawLatexNDC(0.68, 0.96, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi / 1000.0) );
	// latex.DrawLatexNDC(0.125, 0.96, "#font[62]{CMS}");
	// latex.DrawLatexNDC(0.235, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

	// c2->SaveAs(".pdf");

	TFile *f_output = TFile::Open("ROOTFile_RelUnc_Stat_Syst_Tot.root", "RECREATE");
	f_output->cd();

	h_tot->SetName("h_RelUnc_Tot");
	h_tot->Write();

	h_stat->SetName("h_RelUnc_Stat");
	h_stat->Write();

	h_syst->SetName("h_RelUnc_Syst");
	h_syst->Write();
}

TH1D* QuadratureSum_Unc( TH1D* h1, TH1D* h2, TString HistName )
{
	TH1D* h_totUnc = (TH1D*)h1->Clone(HistName);

	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t unc1 = h1->GetBinContent(i_bin);
		Double_t unc2 = h2->GetBinContent(i_bin);

		Double_t unc_tot = sqrt( unc1*unc1 + unc2*unc2 );

		h_totUnc->SetBinContent( i_bin, unc_tot );
		h_totUnc->SetBinError(i_bin, 0);
	}

	return h_totUnc;
}