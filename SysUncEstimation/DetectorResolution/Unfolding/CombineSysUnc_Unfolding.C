void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D *h_SysUnc_Tot );

void CombineSysUnc_Unfolding()
{
	TFile *f_MCModel = new TFile("ROOTFile_SysUnc_Unfolding_MCModel.root"); f_MCModel->cd();
	TH1D* h_SysUnc_MCModel = (TH1D*)f_MCModel->Get("h_RelSysUnc_Unfolding_DetRes_AltMC_Percent")->Clone("h_RelSysUnc_AltMC_Percent");

	TFile *f_MCSize = new TFile("ROOTFile_SysUnc_Unfolding_FiniteSizeMC.root"); f_MCSize->cd();
	TH1D* h_SysUnc_MCSize = (TH1D*)f_MCSize->Get("h_RelSysUnc_Percent")->Clone("h_SysUnc_MCSize_Percent");

	TH1D *h_SysUnc_Tot = (TH1D*)h_SysUnc_MCModel->Clone("h_SysUnc_Tot_Percent");

	TotRelSysUnc( h_SysUnc_MCModel, h_SysUnc_MCSize, h_SysUnc_Tot);

	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_Unfolding", "", 800, 800);
	c_RelUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	// h_SysUnc_MCModel->Draw("HISTLP");
	// h_SysUnc_MCSize->Draw("HISTLPSAME");
	// h_SysUnc_Tot->Draw("HISTLPSAME");

	h_SysUnc_MCModel->Draw("LP");
	h_SysUnc_MCSize->Draw("LPSAME");
	h_SysUnc_Tot->Draw("LPSAME");

	h_SysUnc_MCModel->SetStats(kFALSE);
	h_SysUnc_MCModel->GetXaxis()->SetLabelSize(0.04);
	h_SysUnc_MCModel->GetXaxis()->SetTitleSize(0.04);
	h_SysUnc_MCModel->GetXaxis()->SetNoExponent();
	h_SysUnc_MCModel->GetXaxis()->SetMoreLogLabels();
	h_SysUnc_MCModel->GetYaxis()->SetTitleOffset(1.2);
	h_SysUnc_MCModel->GetYaxis()->SetRangeUser(0, 120);

	h_SysUnc_MCModel->SetXTitle( "Dimuon Mass [GeV]");
	h_SysUnc_MCModel->SetYTitle( "Relative Uncertainty (%)");

	h_SysUnc_MCModel->SetMarkerStyle(20);
	h_SysUnc_MCModel->SetMarkerColor(kBlue);
	h_SysUnc_MCModel->SetLineColor(kBlue);
	h_SysUnc_MCModel->SetFillColorAlpha(kWhite, 0);

	h_SysUnc_MCSize->SetStats(kFALSE);
	h_SysUnc_MCSize->SetMarkerStyle(20);
	h_SysUnc_MCSize->SetMarkerColor(kGreen+1);
	h_SysUnc_MCSize->SetLineColor(kGreen+1);
	h_SysUnc_MCSize->SetFillColorAlpha(kWhite, 0);

	h_SysUnc_Tot->SetStats(kFALSE);
	h_SysUnc_Tot->SetMarkerStyle(20);
	h_SysUnc_Tot->SetMarkerColor(kRed);
	h_SysUnc_Tot->SetLineColor(kRed);
	h_SysUnc_Tot->SetFillColorAlpha(kWhite, 0);

	TLegend *legend = new TLegend(0.45, 0.75, 0.85, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry( h_SysUnc_MCModel, "Syst. from MC model");
	legend->AddEntry( h_SysUnc_MCSize, "Syst. from finite MC size");
	legend->AddEntry( h_SysUnc_Tot, "Quadrature Sum" );
	legend->Draw();

	c_RelUnc->SaveAs( "c_RelSysUnc_Unfolding.pdf" );

	h_SysUnc_MCModel->GetYaxis()->SetRangeUser(0, 16);
	legend->SetX1NDC(0.20);
	legend->SetX2NDC(0.55);

	c_RelUnc->SaveAs( "c_RelSysUnc_Unfolding_Upto16.pdf" );

	h_SysUnc_MCModel->GetYaxis()->SetRangeUser(0, 120);
	legend->SetX1NDC(0.45);
	legend->SetX2NDC(0.75);

	TFile *f_output = new TFile("ROOTFile_SysUnc_Unfolding.root", "RECREATE"); f_output->cd();
	h_SysUnc_MCModel->Write();
	h_SysUnc_MCSize->Write();
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