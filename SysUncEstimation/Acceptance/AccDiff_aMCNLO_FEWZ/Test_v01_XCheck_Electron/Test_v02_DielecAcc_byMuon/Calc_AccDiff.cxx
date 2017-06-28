#include <Include/PlotTools.h>

TH1D* Calc_RelDiff( TH1D* h_aMCNLO, TH1D* h_FEWZ );
void DrawCanvas_XCheck_Acc_aMCNLO_MM();
void DrawCanvas_DifferentFlavor_SameAccCut( TH1D* h_Muon );

void Calc_AccDiff()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString FileName_aMCNLO = "ROOTFile_Acc_Dressed_MM_ElecAcc.root";
	TGraphAsymmErrors *g_Acc = Get_Graph( FileName_aMCNLO, "g_Acc" );
	TH1D* h_aMCNLO = Convert_GraphToHist( g_Acc );

	TString FileName_FEWZ = "../ROOTFile_Acc_Electron_FEWZ_PDF4LHC15.root";
	TH1D* h_FEWZ = Get_Hist( FileName_FEWZ, "h_Acc" );

	TH1D* h_RelDiff = Calc_RelDiff( h_aMCNLO, h_FEWZ );

	// -- Comparison between aMC@NLO and FEWZ -- //
	HistInfo *Hist_aMCNLO = new HistInfo( kRed, "aMC@NLO (l=#mu, electron acc.)", h_aMCNLO );
	HistInfo *Hist_FEWZ = new HistInfo( kBlue, "FEWZ (NNLO)", h_FEWZ );

	TString CanvasName = "Local/Acc_aMCNLO_FEWZ_EE";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_FEWZ, Hist_aMCNLO );
	canvas->SetTitle( "m [GeV]", "Acceptance", "FEWZ/aMC@NLO");
	canvas->SetRatioRange( 0.5, 1.5 );
	canvas->SetLegendPosition( 0.45, 0.32, 0.97, 0.45 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw( 1, 0 );
	////////////////////////////////////////////////

	// -- Comparison of Acc.Diff between muon and electron channel -- //
	TString FileName_MM = "../../ROOTFile_SysUnc_DiffWithFEWZ.root";
	TH1D* h_MM = Get_Hist( FileName_MM, "h_RelUnc");
	HistInfo* Hist_MM = new HistInfo( kGreen+2, "#mu channel", h_MM );

	HistInfo* Hist_EE = new HistInfo( kOrange+2, "e channel (aMC@NLO: l=#mu, electron acc.)", h_RelDiff );

	CanvasName = "Local/AccDiff_EE_MM";
	DrawCanvas_TwoHistRatio *canvas2 = new DrawCanvas_TwoHistRatio( CanvasName, Hist_EE, Hist_MM );
	canvas2->SetTitle( "m [GeV]", "Rel. Unc.", "e ch./#mu ch.");
	canvas2->SetRatioRange( 0, 2.5 );
	canvas2->SetLegendPosition( 0.35, 0.78, 0.97, 0.92 );
	canvas2->SetYRange(0, 0.17);
	canvas2->SetLatex( "Simulation" );
	canvas2->SetDrawOption( "HISTLPSAME" );
	canvas2->Draw( 1, 0 );
	canvas2->c->cd();
	canvas2->TopPad->cd();
	canvas2->latex.DrawLatexNDC( 0.50, 0.92, "#scale[0.6]{#font[42]{Uncertainty from Acc. diff. with FEWZ}}");
	canvas2->c->SaveAs(".pdf");

	DrawCanvas_DifferentFlavor_SameAccCut( h_aMCNLO );
}

TH1D* Calc_RelDiff( TH1D* h_aMCNLO, TH1D* h_FEWZ )
{
	TH1D* h_RelDiff = (TH1D*)h_aMCNLO->Clone();
	Int_t nBin = h_aMCNLO->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t aMCNLO = h_aMCNLO->GetBinContent(i_bin);
		Double_t FEWZ = h_FEWZ->GetBinContent(i_bin);

		Double_t RelDiff = fabs( aMCNLO - FEWZ ) / aMCNLO;

		h_RelDiff->SetBinContent(i_bin, RelDiff);
		h_RelDiff->SetBinError(i_bin, 0);
	}

	return h_RelDiff;
}

void DrawCanvas_DifferentFlavor_SameAccCut( TH1D* h_Muon )
{
	TString FileName = "../ROOTFile_Acc_Dressed_EE.root";
	TGraphAsymmErrors *g_Acc = Get_Graph( FileName, "g_Acc" );
	TH1D* h_Electron = Convert_GraphToHist( g_Acc );

	HistInfo* Hist_MM = new HistInfo( kGreen+2, "Muon", h_Muon );
	HistInfo* Hist_EE = new HistInfo( kOrange+2, "Electron", h_Electron );

	TString CanvasName = "Local/Acc_aMCNLO_Muon_vs_Electron_ElecAccCut";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_EE, Hist_MM );
	canvas->SetTitle( "m [GeV]", "Acceptance (aMC@NLO)", "e/#mu");
	canvas->SetRatioRange( 0.94, 1.06 );
	canvas->SetLegendPosition( 0.60, 0.32, 0.99, 0.45 );
	canvas->SetYRange(1e-4, 1.1);
	canvas->SetLatex( "Simulation" );
	canvas->SetDrawOption( "HISTLPSAME" );
	canvas->Draw( 1, 0 );
	canvas->c->cd();
	canvas->TopPad->cd();
	canvas->latex.DrawLatexNDC( 0.15, 0.915, "#scale[0.6]{#font[42]{P_{T}^{lead} > 30 GeV, P_{T}^{sub} > 10 GeV, |#eta| < 2.5}}");
	canvas->latex.DrawLatexNDC( 0.15, 0.88, "#scale[0.6]{#font[42]{& ECAL gap (1.4442 < |#eta| < 1.566) is excluded}}");
	canvas->c->SaveAs(".pdf");




}