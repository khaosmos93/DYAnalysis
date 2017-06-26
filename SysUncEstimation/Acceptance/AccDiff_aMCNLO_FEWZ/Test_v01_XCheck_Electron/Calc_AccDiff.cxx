#include <Include/PlotTools.h>

TH1D* Calc_RelDiff( TH1D* h_aMCNLO, TH1D* h_FEWZ );
void DrawCanvas_XCheck_Acc_aMCNLO_MM();

void Calc_AccDiff()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString FileName_aMCNLO = "ROOTFIle_Acc_Dressed_EE.root";
	TGraphAsymmErrors *g_Acc = Get_Graph( FileName_aMCNLO, "g_Acc" );
	TH1D* h_aMCNLO = Convert_GraphToHist( g_Acc );

	TString FileName_FEWZ = "ROOTFile_Acc_Electron_FEWZ_PDF4LHC15.root";
	TH1D* h_FEWZ = Get_Hist( FileName_FEWZ, "h_Acc" );

	TH1D* h_RelDiff = Calc_RelDiff( h_aMCNLO, h_FEWZ );

	// -- Comparison between aMC@NLO and FEWZ -- //
	HistInfo *Hist_aMCNLO = new HistInfo( kRed, "aMC@NLO", h_aMCNLO );
	HistInfo *Hist_FEWZ = new HistInfo( kBlue, "FEWZ (NNLO)", h_FEWZ );

	TString CanvasName = "Local/Acc_aMCNLO_FEWZ_EE";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_FEWZ, Hist_aMCNLO );
	canvas->SetTitle( "m [GeV]", "Acceptance", "FEWZ/aMC@NLO");
	canvas->SetRatioRange( 0.5, 1.5 );
	canvas->SetLegendPosition( 0.55, 0.32, 0.95, 0.45 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw( 1, 0 );
	////////////////////////////////////////////////

	// -- Comparison of Acc.Diff between muon and electron channel -- //
	TString FileName_MM = "../ROOTFile_SysUnc_DiffWithFEWZ.root";
	TH1D* h_MM = Get_Hist( FileName_MM, "h_RelUnc");
	HistInfo* Hist_MM = new HistInfo( kGreen+2, "#mu channel", h_MM );

	HistInfo* Hist_EE = new HistInfo( kOrange+2, "e channel", h_RelDiff );

	CanvasName = "Local/AccDiff_EE_MM";
	DrawCanvas_TwoHistRatio *canvas2 = new DrawCanvas_TwoHistRatio( CanvasName, Hist_EE, Hist_MM );
	canvas2->SetTitle( "m [GeV]", "Rel. Unc.", "e ch./#mu ch.");
	canvas2->SetRatioRange( 0, 2.5 );
	canvas2->SetLegendPosition( 0.60, 0.82, 0.95, 0.95 );
	canvas2->SetYRange(0, 0.17);
	canvas2->SetLatex( "Simulation" );
	canvas2->SetDrawOption( "HISTLPSAME" );
	canvas2->Draw( 1, 0 );
	canvas2->c->cd();
	canvas2->TopPad->cd();
	canvas2->latex.DrawLatexNDC( 0.15, 0.92, "#scale[0.6]{#font[42]{Uncertainty from Acc. diff. with FEWZ}}");
	canvas2->c->SaveAs(".pdf");

	DrawCanvas_XCheck_Acc_aMCNLO_MM();
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

void DrawCanvas_XCheck_Acc_aMCNLO_MM()
{
	TString FileName_Old = "../ROOTFile_SysUnc_DiffWithFEWZ.root";
	TH1D* h_Old = Get_Hist( FileName_Old, "h_Acc_aMCNLO" );

	TString FileName_New = "ROOTFile_Acc_Dressed_MM.root";
	TGraphAsymmErrors *g_Acc = Get_Graph( FileName_New, "g_Acc" );  
	TH1D* h_New = Convert_GraphToHist( g_Acc );

	HistInfo *Hist_Old = new HistInfo( kBlack, "aMC@NLO (old)", h_Old );
	HistInfo *Hist_New = new HistInfo( kViolet, "aMC@NLO (new)", h_New );

	TString CanvasName = "Local/Acc_Old_New_MM";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_New, Hist_Old );
	canvas->SetTitle( "m [GeV]", "Acceptance", "New/Old");
	canvas->SetRatioRange( 0.99, 1.01 );
	canvas->SetLegendPosition( 0.55, 0.32, 0.95, 0.45 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw( 1, 0 );
}
