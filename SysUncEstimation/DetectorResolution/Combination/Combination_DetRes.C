void MakeHistogram_TotalUnc(TH1D *h1, TH1D *h2, TH1D* h_tot);
void MakeCanvas( TH1D* h_MomCorr, TH1D* h_unfolding, TH1D* h_tot );

void Combination_DetRes(TString version)
{
	TString FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/" + version;

	TFile *f_MomCorr = TFile::Open(FileLocation+ "/ROOTFile_SysUnc_MomCorr.root");
	TH1D* h_MomCorr = (TH1D*)f_MomCorr->Get("h_SysUnc_Tot_Percent")->Clone();

	TFile *f_unfolding = TFile::Open(FileLocation+ "/ROOTFile_SysUnc_Unfolding.root");
	TH1D* h_unfolding = (TH1D*)f_unfolding->Get("h_SysUnc_Tot_Percent")->Clone();

	TH1D* h_tot = (TH1D*)h_MomCorr->Clone();

	MakeHistogram_TotalUnc(h_MomCorr, h_unfolding, h_tot);

	TFile *f_output = new TFile("ROOTFile_SysUnc_DetRes.root", "RECREATE"); f_output->cd();
	h_MomCorr->SetName("h_RelSysUnc_MomCorr_Percent");
	h_MomCorr->Write();

	h_unfolding->SetName("h_RelSysUnc_Unfolding_Percent");
	h_unfolding->Write();

	h_tot->SetName("h_RelSysUnc_DetRes_Percent");
	h_tot->Write();

	// -- printout -- //
	printf("                     ");
	printf("%10s", "MomCorr" );
	printf("%10s", "Unfold");
	printf("%10s", "Tot." );
	printf("\n");

	for(Int_t i=0; i<h_tot->GetNbinsX(); i++)
	{
		Int_t i_bin = i+1;
		Double_t lowerEdge = h_tot->GetBinLowEdge(i_bin);
		Double_t upperEdge = h_tot->GetBinLowEdge(i_bin+1);

		printf("[%2.0d bin] (%4.0lf, %4.0lf)", i_bin, lowerEdge, upperEdge );

		printf("%10.2lf", h_MomCorr->GetBinContent(i_bin) );
		printf("%10.2lf", h_unfolding->GetBinContent(i_bin) );
		printf("%10.2lf", h_tot->GetBinContent(i_bin) );
		printf("\n");
	}

	MakeCanvas( h_MomCorr, h_unfolding, h_tot );
}

void MakeCanvas( TH1D* h_MomCorr, TH1D* h_unfolding, TH1D* h_tot )
{
	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_DetRes", "", 800, 800);
	c_RelUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	h_MomCorr->Draw("HISTLP");
	h_unfolding->Draw("HISTLPSAME");
	h_tot->Draw("HISTLPSAME");

	h_MomCorr->SetStats(kFALSE);
	h_MomCorr->GetXaxis()->SetLabelSize(0.04);
	h_MomCorr->GetXaxis()->SetTitleSize(0.04);
	h_MomCorr->GetXaxis()->SetNoExponent();
	h_MomCorr->GetXaxis()->SetMoreLogLabels();
	h_MomCorr->GetYaxis()->SetTitleOffset(1.2);
	h_MomCorr->GetYaxis()->SetRangeUser(0, 160);

	h_MomCorr->SetXTitle( "Dimuon Mass [GeV]");
	h_MomCorr->SetYTitle( "Relative Uncertainty (%)");

	h_MomCorr->SetMarkerStyle(20);
	h_MomCorr->SetMarkerColor(kBlue);
	h_MomCorr->SetLineColor(kBlue);
	h_MomCorr->SetFillColorAlpha( kWhite, 0);

	h_unfolding->SetStats(kFALSE);
	h_unfolding->SetMarkerStyle(20);
	h_unfolding->SetMarkerColor(kGreen+1);
	h_unfolding->SetLineColor(kGreen+1);
	h_unfolding->SetFillColorAlpha( kWhite, 0);

	h_tot->SetStats(kFALSE);
	h_tot->SetMarkerStyle(20);
	h_tot->SetMarkerColor(kRed);
	h_tot->SetLineColor(kRed);
	h_tot->SetFillColorAlpha( kWhite, 0);

	TLegend *legend = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry( h_MomCorr, "Syst. from Mom. Scale");
	legend->AddEntry( h_unfolding, "Syst. from unfolding ");
	legend->AddEntry( h_tot, "Total Syst. from Det. Res." );
	legend->Draw();

	c_RelUnc->SaveAs( "c_RelSysUnc_DetRes.pdf" );
	h_MomCorr->GetYaxis()->SetRangeUser(0, 20);
	c_RelUnc->SaveAs( "c_RelSysUnc_DetRes_Upto20.pdf" );
}

void MakeHistogram_TotalUnc(TH1D *h1, TH1D *h2, TH1D* h_tot)
{
	for(Int_t i=0; i<h1->GetNbinsX(); i++)
	{
		Int_t i_bin = i+1;

		Double_t TotUnc = 0;

		Double_t unc1 = h1->GetBinContent(i_bin);
		Double_t unc2 = h2->GetBinContent(i_bin);

		TotUnc = sqrt( unc1*unc1 + unc2*unc2 );
		h_tot->SetBinContent(i_bin, TotUnc);
		h_tot->SetBinError(i_bin, 0);
	}
}