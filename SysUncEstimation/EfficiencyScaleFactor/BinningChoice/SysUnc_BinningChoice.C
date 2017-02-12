#include "SysUncTool_BinningChoice.h"

void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D *h_tot );
void DrawCanvas(TCanvas *c_RelUnc, TH1D* h_RelUnc_Coarse, TH1D* h_RelUnc_Fine, TH1D* h_RelUnc_Tot);

void SysUnc_BinningChoice(TString version, TString Ver_CMSSW = "76X")
{
	// -- Coarse binning -- //
	SysUncTool_BinningChoice* tool1 = new SysUncTool_BinningChoice("CoarseBinning", version, Ver_CMSSW);

	const Int_t nPtBin1 = 4;
	Double_t PtBinEdges1[nPtBin1+1] = {10, 22, 40, 70, 250};

	const Int_t nEtaBin1 = 3;
	Double_t EtaBinEdges1[nEtaBin1+1] = {-2.4, -0.9, 0.9, 2.4};

	tool1->SetupBinning(nPtBin1, PtBinEdges1, nEtaBin1, EtaBinEdges1);
	tool1->SetupEfficiencyScaleFactor("ROOTFile_TagProbeEff_SysUnc_BinningChoice_CoarseBinning.root");
	tool1->CalcEfficiencies();
	tool1->Calc_EffCorr();
	tool1->SetIsDataDriven( kTRUE );
	tool1->CalcDiffXsec_GivenEffSF();
	tool1->ComparisonPlots_WithCentralValue();
	tool1->Calc_SysUnc();
	tool1->SaveResults();


	SysUncTool_BinningChoice* tool2 = new SysUncTool_BinningChoice("FineBinning", version, Ver_CMSSW);

	const Int_t nPtBin2 = 8;
	Double_t PtBinEdges2[nPtBin2+1] = {10, 16, 22, 31, 40, 55, 70, 85, 250};

	const Int_t nEtaBin2 = 10;
	Double_t EtaBinEdges2[nEtaBin2+1] = {-2.4, -2.1, -1.2, -0.9, -0.3, 0, 0.3, 0.9, 1.2, 2.1, 2.4};

	tool2->SetupBinning(nPtBin2, PtBinEdges2, nEtaBin2, EtaBinEdges2);
	tool2->SetupEfficiencyScaleFactor("ROOTFile_TagProbeEff_SysUnc_BinningChoice_FineBinning.root");
	tool2->CalcEfficiencies();
	tool2->Calc_EffCorr();
	tool2->SetIsDataDriven( kTRUE );
	tool2->CalcDiffXsec_GivenEffSF();
	tool2->ComparisonPlots_WithCentralValue();
	tool2->Calc_SysUnc();
	tool2->SaveResults();

	// -- Canvas of relative systematic uncertainties from the binning choice -- //
	TH1D *h_RelSysUnc_Percent_CB = (TH1D*)tool1->h_RelSysUnc_Percent->Clone();
	TH1D *h_RelSysUnc_Percent_FB = (TH1D*)tool2->h_RelSysUnc_Percent->Clone();
	TH1D *h_RelSysUnc_Percent_Total = (TH1D*)h_RelSysUnc_Percent_CB->Clone();
	TotRelSysUnc( h_RelSysUnc_Percent_CB, h_RelSysUnc_Percent_FB, h_RelSysUnc_Percent_Total );

	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_BininngChoice", "", 800, 800);
	DrawCanvas( c_RelUnc, h_RelSysUnc_Percent_CB, h_RelSysUnc_Percent_FB, h_RelSysUnc_Percent_Total);

	TFile *f_output = new TFile("ROOTFile_SysUnc_EffSF_BinningChoice.root", "RECREATE");
	h_RelSysUnc_Percent_CB->SetName("h_RelUnc_Coarse_Percent");
	h_RelSysUnc_Percent_CB->Write();

	h_RelSysUnc_Percent_FB->SetName("h_RelUnc_Fine_Percent");
	h_RelSysUnc_Percent_FB->Write();

	h_RelSysUnc_Percent_Total->SetName("h_RelUnc_BinningChoice_Percent");
	h_RelSysUnc_Percent_Total->Write();

	c_RelUnc->Write();
}

void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D *h_tot )
{
	Int_t nBin = h1->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t unc1 = h1->GetBinContent(i_bin);
		Double_t unc2 = h2->GetBinContent(i_bin);

		Double_t totunc = sqrt( unc1 * unc1 + unc2 * unc2 );

		h_tot->SetBinContent(i_bin, totunc);
	}
}

void DrawCanvas(TCanvas *c_RelUnc, TH1D* h_RelUnc_Coarse, TH1D* h_RelUnc_Fine, TH1D* h_RelUnc_Tot)
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
	legend->AddEntry( h_RelUnc_Tot, "Quadrature Sum" );
	legend->Draw();

	c_RelUnc->SaveAs( "c_RelSysUnc_BininngChoice.pdf" );
}
