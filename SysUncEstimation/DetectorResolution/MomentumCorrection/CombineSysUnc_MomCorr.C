void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D *h_SysUnc_Tot );

void CombineSysUnc_MomCorr()
{
	TFile *f_DataMomCorr = new TFile("ROOTFile_SysUnc_DataMomCorr_UnbinnedFit.root"); f_DataMomCorr->cd();
	TH1D* h_SysUnc_DataMomCorr = (TH1D*)f_DataMomCorr->Get("h_RMS")->Clone();
	h_SysUnc_DataMomCorr->SetName("h_SysUnc_DataMomCorr_Percent");
	h_SysUnc_DataMomCorr->Scale(100); // -- convert to % -- //

	TFile *f_MCMomCorr = new TFile("ROOTFile_SysUnc_MCMomCorr_UnbinnedFit.root"); f_MCMomCorr->cd();
	TH1D* h_SysUnc_MCMomCorr = (TH1D*)f_MCMomCorr->Get("h_RMS")->Clone();
	h_SysUnc_MCMomCorr->SetName("h_SysUnc_MCMomCorr_Percent");
	h_SysUnc_MCMomCorr->Scale(100); // -- convert to % -- //

	TH1D *h_SysUnc_Tot = (TH1D*)h_SysUnc_DataMomCorr->Clone();
	h_SysUnc_Tot->SetName("h_SysUnc_Tot_Percent");

	TotRelSysUnc( h_SysUnc_DataMomCorr, h_SysUnc_MCMomCorr, h_SysUnc_Tot);

	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_MomCorr", "", 800, 800);
	c_RelUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	// h_SysUnc_DataMomCorr->Draw("HISTLP");
	// h_SysUnc_MCMomCorr->Draw("HISTLPSAME");
	// h_SysUnc_Tot->Draw("HISTLPSAME");

	h_SysUnc_DataMomCorr->Draw("LP");
	h_SysUnc_MCMomCorr->Draw("LPSAME");
	h_SysUnc_Tot->Draw("LPSAME");

	h_SysUnc_DataMomCorr->SetStats(kFALSE);
	h_SysUnc_DataMomCorr->GetXaxis()->SetLabelSize(0.04);
	h_SysUnc_DataMomCorr->GetXaxis()->SetTitleSize(0.04);
	h_SysUnc_DataMomCorr->GetXaxis()->SetNoExponent();
	h_SysUnc_DataMomCorr->GetXaxis()->SetMoreLogLabels();
	h_SysUnc_DataMomCorr->GetYaxis()->SetTitleOffset(1.2);
	h_SysUnc_DataMomCorr->GetYaxis()->SetRangeUser(0, 150);

	h_SysUnc_DataMomCorr->SetXTitle( "Dimuon Mass [GeV]");
	h_SysUnc_DataMomCorr->SetYTitle( "Relative Uncertainty (%)");

	h_SysUnc_DataMomCorr->SetMarkerStyle(20);
	h_SysUnc_DataMomCorr->SetMarkerColor(kBlue);
	h_SysUnc_DataMomCorr->SetLineColor(kBlue);
	h_SysUnc_DataMomCorr->SetFillColorAlpha(kWhite, 0);

	h_SysUnc_MCMomCorr->SetStats(kFALSE);
	h_SysUnc_MCMomCorr->SetMarkerStyle(20);
	h_SysUnc_MCMomCorr->SetMarkerColor(kGreen+1);
	h_SysUnc_MCMomCorr->SetLineColor(kGreen+1);
	h_SysUnc_MCMomCorr->SetFillColorAlpha(kWhite, 0);

	h_SysUnc_Tot->SetStats(kFALSE);
	h_SysUnc_Tot->SetMarkerStyle(20);
	h_SysUnc_Tot->SetMarkerColor(kRed);
	h_SysUnc_Tot->SetLineColor(kRed);
	h_SysUnc_Tot->SetFillColorAlpha(kWhite, 0);

	TLegend *legend = new TLegend(0.45, 0.75, 0.85, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry( h_SysUnc_DataMomCorr, "Syst. from Data Mom.Corr.");
	legend->AddEntry( h_SysUnc_MCMomCorr, "Syst. from MC Mom.Corr.");
	legend->AddEntry( h_SysUnc_Tot, "Quadrature Sum" );
	legend->Draw();

	c_RelUnc->SaveAs( "c_RelSysUnc_MomCorr.pdf" );

	TFile *f_output = new TFile("ROOTFile_SysUnc_MomCorr.root", "RECREATE"); f_output->cd();
	h_SysUnc_DataMomCorr->Write();
	h_SysUnc_MCMomCorr->Write();
	h_SysUnc_Tot->Write();
	c_RelUnc->Write();
}

void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D *h_SysUnc_Tot )
{
	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t unc1 = h1->GetBinContent(i_bin);
		Double_t unc2 = h2->GetBinContent(i_bin);

		Double_t totunc = sqrt( unc1 * unc1 + unc2 * unc2 );

		h_SysUnc_Tot->SetBinContent(i_bin, totunc);
		h_SysUnc_Tot->SetBinError(i_bin, 0);
	}
}