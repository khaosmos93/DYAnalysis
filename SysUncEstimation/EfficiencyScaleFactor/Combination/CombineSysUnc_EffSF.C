#include <TColor.h>
#include <TFile.h>
#include <TH1.h>
#include <TPad.h>
#include <TLegend.h>
#include <TCanvas.h>

void TotRelSysUnc( TH1D *h1, TH1D *h2, TH1D *h_tot );

void CombineSysUnc_EffSF(TString version)
{
	TString FileLocation = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/" + version;

	TFile *f_tp = new TFile(FileLocation + "/ROOTFile_SysUnc_EffSF_TagProbe_UnbinnedFit.root"); f_tp->cd();
	TH1D *h_tp = (TH1D*)f_tp->Get("h_RMS")->Clone();
	h_tp->Scale( 100 ); // -- Convert to % -- //

	TFile *f_binning = new TFile(FileLocation + "/ROOTFile_SysUnc_EffSF_BinningChoice.root"); f_binning->cd();
	TH1D *h_binning = (TH1D*)f_binning->Get("h_RelUnc_BinningChoice_Percent")->Clone();

	TH1D* h_tot = (TH1D*)h_tp->Clone();
	TotRelSysUnc( h_tp, h_binning, h_tot );

	TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_EffSF", "", 800, 800);
	c_RelUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	h_tp->Draw("HISTLP");
	h_binning->Draw("HISTLPSAME");
	h_tot->Draw("HISTLPSAME");

	h_tp->SetStats(kFALSE);
	h_tp->GetXaxis()->SetLabelSize(0.04);
	h_tp->GetXaxis()->SetTitleSize(0.04);
	h_tp->GetXaxis()->SetNoExponent();
	h_tp->GetXaxis()->SetMoreLogLabels();
	h_tp->GetYaxis()->SetTitleOffset(1.2);
	h_tp->GetYaxis()->SetRangeUser(0, 4);

	h_tp->SetXTitle( "Dimuon Mass [GeV]");
	h_tp->SetYTitle( "Relative Uncertainty (%)");

	h_tp->SetMarkerStyle(20);
	h_tp->SetMarkerColor(kBlue);
	h_tp->SetLineColor(kBlue);
	h_tp->SetFillColorAlpha( kWhite, 0);

	h_binning->SetStats(kFALSE);
	h_binning->SetMarkerStyle(20);
	h_binning->SetMarkerColor(kGreen+1);
	h_binning->SetLineColor(kGreen+1);
	h_binning->SetFillColorAlpha( kWhite, 0);

	h_tot->SetStats(kFALSE);
	h_tot->SetMarkerStyle(20);
	h_tot->SetMarkerColor(kRed);
	h_tot->SetLineColor(kRed);
	h_tot->SetFillColorAlpha( kWhite, 0);

	TLegend *legend = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->AddEntry( h_tp, "Sys. from Tag&Probe");
	legend->AddEntry( h_binning, "Sys. from Binning Choice");
	legend->AddEntry( h_tot, "Total Sys. from Eff. SF" );
	legend->Draw();

	c_RelUnc->SaveAs( "c_RelSysUnc_EffSF.pdf" );

	//////////////////////////////////////////////
	// -- Comparison with 7-8TeV uncertainty -- //
	//////////////////////////////////////////////

	const Int_t nMassBin_8TeV = 41;
	Double_t MassBinEdges_8TeV[nMassBin_8TeV+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
												 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
												 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
												 200, 220, 243, 273, 320, 380, 440, 510, 600,
												 1000, 1500, 2000};


	Double_t RMS_8TeV[nMassBin_8TeV] = {1.35, 0.99, 0.81, 0.69, 0.56, 0.56, 0.51, 0.42, 0.38, 0.35, 
									 	0.32, 0.31, 0.30, 0.30, 0.32, 0.38, 0.40, 0.40, 0.39, 0.38, 
									 	0.39, 0.39, 0.42, 0.45, 0.43, 0.42, 0.46, 0.49, 0.52, 0.55, 
									 	1.10, 1.25, 1.34, 1.53, 1.87, 1.23, 1.36, 1.56, 1.77, 2.39, 3.39};
	
	const Int_t nMassBin_7TeV = 40;
	Double_t MassBinEdges_7TeV[nMassBin_7TeV+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
													 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
													 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
													 200, 220, 243, 273, 320, 380, 440, 510, 600,
													 1000, 1500};

	Double_t RMS_7TeV[nMassBin_7TeV] = {1.90, 2.31, 2.26, 1.48, 1.19, 1.12, 1.10, 1.07, 1.07, 1.06, 
										1.06, 1.06, 1.05, 1.06, 1.11, 1.07, 1.08, 1.29, 1.31, 1.32, 
										1.34, 1.33, 1.36, 1.35, 1.31, 1.29, 1.36, 1.42, 1.53, 1.60, 
										1.71, 1.75, 1.86, 1.90, 1.90, 1.93, 1.97, 2.02, 2.01, 2.14};

	TH1D* h_SysUnc_8TeV = new TH1D("h_SysUnc_8TeV", "", nMassBin_8TeV, MassBinEdges_8TeV);
	for(Int_t i=0; i<nMassBin_8TeV; i++)
	{
		Int_t i_bin = i+1;
		h_SysUnc_8TeV->SetBinContent( i_bin, RMS_8TeV[i] );
	}

	TH1D* h_SysUnc_7TeV = new TH1D("h_SysUnc_7TeV", "", nMassBin_7TeV, MassBinEdges_7TeV);
	for(Int_t i=0; i<nMassBin_7TeV; i++)
	{
		Int_t i_bin = i+1;
		h_SysUnc_7TeV->SetBinContent( i_bin, RMS_7TeV[i] );
	}

	TH1D *h_SysUnc_13TeV = (TH1D*)h_tot->Clone();

	TCanvas *c_Compare_previousSysUnc = new TCanvas("c_Compare_previousSysUnc", "", 800, 800);
	c_Compare_previousSysUnc->cd();

	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);
	gPad->SetLeftMargin(0.15);
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	h_SysUnc_13TeV->Draw("HISTLP");
	h_SysUnc_7TeV->Draw("HISTLPSAME");
	h_SysUnc_8TeV->Draw("HISTLPSAME");
	h_SysUnc_13TeV->Draw("HISTLPSAME");

	
	h_SysUnc_13TeV->GetXaxis()->SetLabelSize(0.04);
	h_SysUnc_13TeV->GetXaxis()->SetTitleSize(0.04);
	h_SysUnc_13TeV->GetXaxis()->SetNoExponent();
	h_SysUnc_13TeV->GetXaxis()->SetMoreLogLabels();
	h_SysUnc_13TeV->GetYaxis()->SetTitleOffset(1.2);
	h_SysUnc_13TeV->GetYaxis()->SetRangeUser(0, 4);

	h_SysUnc_13TeV->SetXTitle( "Dimuon Mass [GeV]");
	h_SysUnc_13TeV->SetYTitle( "Relative Uncertainty (%)");

	h_SysUnc_7TeV->SetStats(kFALSE);
	h_SysUnc_7TeV->SetMarkerStyle(20);
	h_SysUnc_7TeV->SetMarkerColor(kBlack);
	h_SysUnc_7TeV->SetLineColor(kBlack);
	h_SysUnc_7TeV->SetFillColorAlpha( kWhite, 0);

	h_SysUnc_8TeV->SetStats(kFALSE);
	h_SysUnc_8TeV->SetMarkerStyle(20);
	h_SysUnc_8TeV->SetMarkerColor(kViolet+1);
	h_SysUnc_8TeV->SetLineColor(kViolet+1);
	h_SysUnc_8TeV->SetFillColorAlpha( kWhite, 0);

	h_SysUnc_13TeV->SetStats(kFALSE);
	h_SysUnc_13TeV->SetMarkerStyle(20);
	h_SysUnc_13TeV->SetMarkerColor(kRed);
	h_SysUnc_13TeV->SetLineColor(kRed);
	h_SysUnc_13TeV->SetFillColorAlpha( kWhite, 0);

	TLegend *legend2 = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend2->SetFillStyle(0);
	legend2->SetBorderSize(0);
	legend2->AddEntry( h_SysUnc_7TeV, "Sys. from Eff. SF (7 TeV)");
	legend2->AddEntry( h_SysUnc_8TeV, "Sys. from Eff. SF (8 TeV)");
	legend2->AddEntry( h_SysUnc_13TeV, "Sys. from Eff. SF (13 TeV)" );
	legend2->Draw();

	c_Compare_previousSysUnc->SaveAs( "c_Compare_previousSysUnc.pdf" );


	TFile *f_output = new TFile("ROOTFile_SysUnc_EffSF.root", "RECREATE");
	h_tp->SetName("h_RelSysUnc_TagProbe");
	h_tp->Write();

	h_binning->SetName("h_RelSysUnc_Binning");
	h_binning->Write();

	h_tot->SetName("h_RelSysUnc_Tot");
	h_tot->Write();

	h_SysUnc_7TeV->SetName("h_RelSysUnc_Tot_7TeV");
	h_SysUnc_7TeV->Write();

	h_SysUnc_8TeV->SetName("h_RelSysUnc_Tot_8TeV");
	h_SysUnc_8TeV->Write();
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