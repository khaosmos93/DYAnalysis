TH1D* TotRelSysUnc( TH1D *h1, TH1D *h2 );
void DrawCanvas_OldType(TCanvas *c_RelUnc, TH1D* h_RelUnc_Coarse, TH1D* h_RelUnc_Fine, TH1D* h_RelUnc_Tot);
void DrawCanvas(TH1D* h_RelUnc_Coarse, TH1D* h_RelUnc_Fine, TH1D* h_RelUnc_Tot);

void Calc_TotUnc_BinningChoice(TString version)
{
	TString BasePath = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/";
	TFile *f_input = TFile::Open(BasePath + version + "/ROOTFile_SysUnc_EffSF_BinningChoice.root");

	TH1D* h_RelSysUnc_Percent_CB = (TH1D*)f_input->Get("h_RelUnc_Coarse_Percent")->Clone();
	TH1D* h_RelSysUnc_Percent_FB = (TH1D*)f_input->Get("h_RelUnc_Fine_Percent")->Clone();

	TH1D* h_RelSysUnc_Percent_Total = TotRelSysUnc( h_RelSysUnc_Percent_CB, h_RelSysUnc_Percent_FB );
	h_RelSysUnc_Percent_Total->SetName("h_RelUnc_BinningChoice_Percent");

	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_BininngChoice_OldType", "", 800, 800);
	DrawCanvas_OldType( c_RelUnc, h_RelSysUnc_Percent_CB, h_RelSysUnc_Percent_FB, h_RelSysUnc_Percent_Total);
	DrawCanvas( h_RelSysUnc_Percent_CB, h_RelSysUnc_Percent_FB, h_RelSysUnc_Percent_Total);

	TFile *f_output = new TFile("ROOTFile_SysUnc_EffSF_BinningChoice_Updated.root", "RECREATE");
	h_RelSysUnc_Percent_CB->SetName("h_RelUnc_Coarse_Percent");
	h_RelSysUnc_Percent_CB->Write();

	h_RelSysUnc_Percent_FB->SetName("h_RelUnc_Fine_Percent");
	h_RelSysUnc_Percent_FB->Write();

	h_RelSysUnc_Percent_Total->SetName("h_RelUnc_BinningChoice_Percent");
	h_RelSysUnc_Percent_Total->Write();

	c_RelUnc->Write();
}

TH1D* TotRelSysUnc( TH1D *h1, TH1D *h2 )
{
	TH1D* h_tot = (TH1D*)h1->Clone();

	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t unc1 = h1->GetBinContent(i_bin);
		Double_t unc2 = h2->GetBinContent(i_bin);

		// -- take larger uncertainty -- //
		Double_t totunc = 0;
		if( unc1 > unc2 ) totunc = unc1;
		else totunc = unc2;

		h_tot->SetBinContent(i_bin, totunc);
		h_tot->SetBinError(i_bin, 0);
	}

	return h_tot;
}

void DrawCanvas(TH1D* h_RelUnc_Coarse, TH1D* h_RelUnc_Fine, TH1D* h_RelUnc_Tot)
{
	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_BininngChoice", "", 800, 800);
	c_RelUnc->cd();

	gPad->SetBottomMargin(0.12);
	gPad->SetLeftMargin(0.12);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.04);
	gPad->SetLogx();

	h_RelUnc_Coarse->Draw("HISTLP");
	h_RelUnc_Fine->Draw("HISTLPSAME");
	h_RelUnc_Tot->Draw("HISTLPSAME");

	h_RelUnc_Coarse->SetStats(kFALSE);
	h_RelUnc_Coarse->GetXaxis()->SetTitleOffset(1.4);
	h_RelUnc_Coarse->GetXaxis()->SetNoExponent();
	h_RelUnc_Coarse->GetXaxis()->SetMoreLogLabels();
	h_RelUnc_Coarse->GetYaxis()->SetTitleOffset(1.4);
	h_RelUnc_Coarse->SetMinimum(0);

	h_RelUnc_Coarse->SetXTitle( "m (#mu#mu) [GeV]");
	h_RelUnc_Coarse->SetYTitle( "Rel. Unc. (%)");

	h_RelUnc_Coarse->SetMarkerStyle(20);
	h_RelUnc_Coarse->SetMarkerColor(kBlue);
	h_RelUnc_Coarse->SetLineColor(kBlue);

	h_RelUnc_Fine->SetStats(kFALSE);
	h_RelUnc_Fine->SetMarkerStyle(20);
	h_RelUnc_Fine->SetMarkerColor(kGreen+1);
	h_RelUnc_Fine->SetLineColor(kGreen+1);

	h_RelUnc_Tot->SetStats(kFALSE);
	h_RelUnc_Tot->SetMarkerStyle(20);
	h_RelUnc_Tot->SetMarkerColor(kRed);
	h_RelUnc_Tot->SetLineColor(kRed);

	TLegend *legend = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->SetTextFont(62);
	legend->AddEntry( h_RelUnc_Coarse, "Coarse Binning");
	legend->AddEntry( h_RelUnc_Fine, "Fine Binning");
	legend->AddEntry( h_RelUnc_Tot, "Total" );
	legend->Draw();

	TLatex latex;
	Double_t Lumi = 2832.673;
	latex.DrawLatexNDC(0.68, 0.96, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi / 1000.0) );
	latex.DrawLatexNDC(0.125, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.235, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

	c_RelUnc->SaveAs( ".pdf" );
}

void DrawCanvas_OldType(TCanvas *c_RelUnc, TH1D* h_RelUnc_Coarse, TH1D* h_RelUnc_Fine, TH1D* h_RelUnc_Tot)
{
	c_RelUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	h_RelUnc_Coarse->Draw("HISTLP");
	h_RelUnc_Fine->Draw("HISTLPSAME");
	h_RelUnc_Tot->Draw("HISTLPSAME");

	h_RelUnc_Coarse->SetStats(kFALSE);
	h_RelUnc_Coarse->GetXaxis()->SetLabelSize(0.04);
	h_RelUnc_Coarse->GetXaxis()->SetTitleSize(0.04);
	h_RelUnc_Coarse->GetXaxis()->SetNoExponent();
	h_RelUnc_Coarse->GetXaxis()->SetMoreLogLabels();
	h_RelUnc_Coarse->GetYaxis()->SetTitleOffset(1.2);
	h_RelUnc_Coarse->GetYaxis()->SetRangeUser(0, 3);


	h_RelUnc_Coarse->SetXTitle( "Dimuon Mass [GeV]");
	h_RelUnc_Coarse->SetYTitle( "Relative Uncertainty (%)");

	h_RelUnc_Coarse->SetMarkerStyle(20);
	h_RelUnc_Coarse->SetMarkerColor(kBlue);
	h_RelUnc_Coarse->SetLineColor(kBlue);

	h_RelUnc_Fine->SetStats(kFALSE);
	h_RelUnc_Fine->SetMarkerStyle(20);
	h_RelUnc_Fine->SetMarkerColor(kGreen+1);
	h_RelUnc_Fine->SetLineColor(kGreen+1);

	h_RelUnc_Tot->SetStats(kFALSE);
	h_RelUnc_Tot->SetMarkerStyle(20);
	h_RelUnc_Tot->SetMarkerColor(kRed);
	h_RelUnc_Tot->SetLineColor(kRed);

	TLegend *legend = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry( h_RelUnc_Coarse, "Coarse Binning");
	legend->AddEntry( h_RelUnc_Fine, "Fine Binning");
	legend->AddEntry( h_RelUnc_Tot, "Total" );
	legend->Draw();

	c_RelUnc->SaveAs( ".pdf" );
}