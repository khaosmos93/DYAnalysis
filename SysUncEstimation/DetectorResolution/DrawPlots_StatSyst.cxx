TH1D* QuadratureSum_Unc( TH1D* h1, TH1D* h2, TString HistName );

void DrawPlots_StatSyst()
{
	TString BaseLocation = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20160731_1st_ApplyEffSF_MCBkg";

	TFile *f_unfold = TFile::Open(BaseLocation+"/ROOTFile_SysUnc_Unfolding.root"); f_unfold->cd();
	TH1D* h_stat_unfold = (TH1D*)f_unfold->Get("h_SysUnc_MCSize_Percent")->Clone();
	TH1D* h_syst = (TH1D*)f_unfold->Get("h_RelSysUnc_AltMC_Percent")->Clone();


	TFile *f_stat_momCorr = TFile::Open(BaseLocation+"/ROOTFile_SysUnc_DetRes.root"); f_stat_momCorr->cd();
	TH1D* h_stat_momCorr = (TH1D*)f_stat_momCorr->Get("h_RelSysUnc_MomCorr_Percent")->Clone();
	TH1D* h_tot = (TH1D*)f_stat_momCorr->Get("h_RelSysUnc_DetRes_Percent")->Clone();


	// -- total statistical uncertainty -- //
	TH1D* h_stat_tot = QuadratureSum_Unc( h_stat_unfold, h_stat_momCorr, "h_RelUnc_TotStat" );

	// -- draw plot: systematic source only -- //
	TCanvas *c1 = new TCanvas("c_stat", "", 800, 800);
	c1->cd();
	gPad->SetBottomMargin(0.12);
	gPad->SetLeftMargin(0.12);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.04);

	gPad->SetLogx();
	gPad->SetLogy();

	h_stat_tot->Draw("AXIS");
	h_stat_unfold->Draw("HISTLPSAME");
	h_stat_momCorr->Draw("HISTLPSAME");
	h_stat_tot->Draw("HISTLPSAME");

	h_stat_tot->SetStats(kFALSE);
	h_stat_tot->SetLineColor(kBlack);
	h_stat_tot->SetMarkerStyle(20);
	h_stat_tot->SetMarkerColor(kBlack);
	h_stat_tot->SetFillColorAlpha( kWhite, 0 );

	h_stat_unfold->SetStats(kFALSE);
	h_stat_unfold->SetLineColor(kGreen+1);
	h_stat_unfold->SetMarkerStyle(20);
	h_stat_unfold->SetMarkerColor(kGreen+1);
	h_stat_unfold->SetFillColorAlpha( kWhite, 0 );

	h_stat_momCorr->SetStats(kFALSE);
	h_stat_momCorr->SetLineColor(kOrange+1);
	h_stat_momCorr->SetMarkerStyle(20);
	h_stat_momCorr->SetMarkerColor(kOrange+1);
	h_stat_momCorr->SetFillColorAlpha( kWhite, 0 );

	// -- axis settings -- //
	h_stat_tot->SetXTitle("m (#mu#mu) [GeV]");
	h_stat_tot->SetYTitle("Rel. Unc. (%)");

	h_stat_tot->GetXaxis()->SetNoExponent();
	h_stat_tot->GetXaxis()->SetMoreLogLabels();
	h_stat_tot->GetXaxis()->SetTitleOffset(1.4);
	h_stat_tot->GetYaxis()->SetTitleOffset(1.4);
	h_stat_tot->SetMinimum(1e-2);
	h_stat_unfold->SetMinimum(1e-2);
	h_stat_momCorr->SetMinimum(1e-2);

	TLegend *legend1 = new TLegend(0.15, 0.75, 0.55, 0.95);
	legend1->SetBorderSize(0);
	legend1->SetFillStyle(0);
	legend1->SetTextFont(62);
	legend1->AddEntry( h_stat_tot, "Total Stat." );
	legend1->AddEntry( h_stat_unfold, "Stat. (Resp.M)");
	legend1->AddEntry( h_stat_momCorr, "Stat. (Mom. Corr.)");
	legend1->Draw();

	TLatex latex;
	Double_t Lumi = 2832.673;
	latex.DrawLatexNDC(0.68, 0.96, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi / 1000.0) );
	latex.DrawLatexNDC(0.125, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.235, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

	c1->SaveAs(".pdf");


	// -- draw plot: stat, syst, and total uncertainty -- //
	TCanvas *c2 = new TCanvas("c_tot", "", 800, 800);
	c2->cd();
	gPad->SetBottomMargin(0.12);
	gPad->SetLeftMargin(0.12);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.04);

	gPad->SetLogx();
	gPad->SetLogy();

	h_tot->Draw("AXIS");
	h_stat_tot->Draw("HISTLPSAME");
	h_syst->Draw("HISTLPSAME");
	h_tot->Draw("HISTLPSAME");

	h_tot->SetStats(kFALSE);
	h_tot->SetLineColor(kRed);
	h_tot->SetMarkerStyle(20);
	h_tot->SetMarkerColor(kRed);
	h_tot->SetFillColorAlpha( kWhite, 0 );

	// h_stat_tot->SetStats(kFALSE);
	// h_stat_tot->SetLineColor(kBlack);
	// h_stat_tot->SetMarkerStyle(20);
	// h_stat_tot->SetMarkerColor(kBlack);
	// h_stat_tot->SetFillColorAlpha( kWhite, 0 );

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
	// h_tot->SetMinimum(0);
	h_tot->SetMinimum(1e-2);
	h_stat_tot->SetMinimum(1e-2);
	h_syst->SetMinimum(1e-2);

	TLegend *legend2 = new TLegend(0.15, 0.75, 0.55, 0.95);
	legend2->SetBorderSize(0);
	legend2->SetFillStyle(0);
	legend2->SetTextFont(62);
	legend2->AddEntry( h_tot, "Total Unc. from Det.Res" );
	legend2->AddEntry( h_stat_tot, "Total Stat.");
	legend2->AddEntry( h_syst, "Syst.");
	legend2->Draw();

	latex.DrawLatexNDC(0.68, 0.96, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi / 1000.0) );
	latex.DrawLatexNDC(0.125, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.235, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

	c2->SaveAs(".pdf");

	TFile *f_output = TFile::Open("ROOTFile_RelUnc_Stat_Syst_Tot.root", "RECREATE");
	f_output->cd();

	h_tot->SetName("h_RelUnc_Tot");
	h_tot->Write();

	h_stat_tot->SetName("h_RelUnc_Stat");
	h_stat_tot->Write();

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