#include <DYAnalysis_76X/Include/PlotTools.h>
#include <DYAnalysis_76X/Include/DYAnalyzer.h>

// -- nMassBin: defined in DYAnalyzer -- //
Double_t MassBinEdges[nMassBin+1] = {
	15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
	64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
	110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
	200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
	830, 1000, 1500, 3000};

void Compare_sqrtN()
{
	TString FileName_Data = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root";
	TH1D* h_ObservedYield = Get_Hist( FileName_Data, "h_mass_OS_Data" );
	h_ObservedYield = (TH1D*)h_ObservedYield->Rebin(nMassBin, h_ObservedYield->GetName(), MassBinEdges);

	TString FileName_KP = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_DiffXSec_FullUnc.root";
	TH1D* h_DiffXSec_KP = Get_Hist( FileName_KP, "h_DiffXsec_FSRCorr" );
	TH1D* h_RelStatUnc_KP = Get_Hist( FileName_KP, "h_RelStatUnc_Percent" ); h_RelStatUnc_KP->Scale( 0.01 );

	TString FileName_Andrius = GetBasePath() + "Include/Results_ROOTFiles_76X/dymm_study_yieldErrPropagation_5000.root";
	TH1D* h_DiffXSec_Andrius = Get_Hist( FileName_Andrius, "h1__cs" );
	TH1D* h_RelStatUnc_Andrius = Extract_RelUnc( h_DiffXSec_Andrius );

	TH1D* h_1OverSqrtN = (TH1D*)h_RelStatUnc_KP->Clone("h_1OverSqrtN");
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t nYield = h_ObservedYield->GetBinContent(i_bin);
		Double_t OneOverSqrtN = 1 / sqrt(nYield);

		h_1OverSqrtN->SetBinContent(i_bin, OneOverSqrtN );
		h_1OverSqrtN->SetBinError(i_bin, 0);

		Double_t DiffXSec_KP = h_DiffXSec_KP->GetBinContent(i_bin);
		Double_t RelStatUnc_KP = h_RelStatUnc_KP->GetBinContent(i_bin);
		Double_t RelStatUnc_Andrius = h_RelStatUnc_Andrius->GetBinContent(i_bin);
		// Double_t AbsStatUnc_KP = DiffXSec_KP * RelStatUnc_KP;

		// h_AbsStatUnc_KP->SetBinContent(i_bin, AbsStatUnc_KP );
		// h_AbsStatUnc_KP->SetBinError(i_bin, 0);

		printf("[%02d bin: %5.1lf < M < %5.1lf] (N, 1/SqrtN, RelStatUnc (KP)) = (%10.0lf, %9.3lf, %9.3lf, %9.3lf)\n",
			i_bin, MassBinEdges[i], MassBinEdges[i+1], nYield, OneOverSqrtN*100, RelStatUnc_KP*100, RelStatUnc_Andrius*100 );
	}

	HistInfo *Hist_1OverSqrtN = new HistInfo( kBlack, "1 / #sqrt{N^{obs}}");
	Hist_1OverSqrtN->Set_Histogram( h_1OverSqrtN );
	Hist_1OverSqrtN->Set();

	HistInfo *Hist_KP = new HistInfo( kBlue, "Stat. by KP" );
	Hist_KP->Set_Histogram( h_RelStatUnc_KP );
	Hist_KP->Set();
	Hist_KP->Calc_RatioHist_Denominator( Hist_1OverSqrtN->h );

	HistInfo *Hist_Andrius = new HistInfo( kGreen+2, "Stat. by Andrius" );
	Hist_Andrius->Set_Histogram( h_RelStatUnc_Andrius );
	Hist_Andrius->Set();
	Hist_Andrius->Calc_RatioHist_Denominator( Hist_1OverSqrtN->h );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "Compare_RelStatUnc", TopPad, BottomPad, 1, 1 );

	c->cd();
	TopPad->cd();

	Hist_1OverSqrtN->h->Draw("LPSAME");
	Hist_KP->h->Draw("LPSAME");
	Hist_Andrius->h->Draw("LPSAME");

	SetHistFormat_TopPad( Hist_1OverSqrtN->h, "Relative uncertainty (Stat.)");

	TLegend *legend;
	SetLegend( legend, 0.20, 0.75, 0.50, 0.95 );
	legend->AddEntry( Hist_1OverSqrtN->h, Hist_1OverSqrtN->LegendName );
	legend->AddEntry( Hist_KP->h, Hist_KP->LegendName );
	legend->AddEntry( Hist_Andrius->h, Hist_Andrius->LegendName );
	legend->Draw();

	TLatex latex;
	Latex_Preliminary( latex, 2.8, 13 );

	c->cd();
	BottomPad->cd();

	Hist_KP->h_ratio->Draw("LPSAME");
	Hist_Andrius->h_ratio->Draw("LPSAME");

	SetHistFormat_BottomPad( Hist_KP->h_ratio, "m (#mu#mu) [GeV]", "Unc. / black", 0.9, 2.0 );
	Hist_KP->h_ratio->GetYaxis()->CenterTitle();
	Hist_KP->h_ratio->GetYaxis()->SetTitleOffset(0.4);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}