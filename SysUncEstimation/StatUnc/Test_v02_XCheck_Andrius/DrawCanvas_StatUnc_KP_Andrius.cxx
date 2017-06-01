#include <Include/PlotTools.h>

void DrawCanvas_StatUnc_KP_Andrius()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_KP = ROOTFilePath + "/ROOTFile_RelStatUnc.root";
	TH1D* h_KP = Get_Hist( FileName_KP, "h_RelStatUnc_Percent" ); h_KP->Scale( 0.01 );

	TString FileName_Andrius = "cov_mumu_varYieldPoisson_1000_veryslim.root";
	TH1D* h_Andrius = Get_Hist( FileName_Andrius, "h1relUncFromCov");

	HistInfo *Hist_KP = new HistInfo( kBlue, "KP" );
	Hist_KP->Set_Histogram( h_KP );
	Hist_KP->Set();

	HistInfo *Hist_Andrius = new HistInfo( kGreen+2, "Andrius" );
	Hist_Andrius->Set_Histogram( h_Andrius );
	Hist_Andrius->Set();

	TString CanvasName = "StatUnc_KP_Andrius";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_KP, Hist_Andrius );
	canvas->SetTitle( "m [GeV]", "Stat. Unc (%)", "KP/Andrius");
	canvas->SetLegendPosition( 0.18, 0.75, 0.50, 0.95 );
	canvas->SetLatex( "Preliminary", 2.8, 13 );
	canvas->SetDrawOption( "HISTLPSAME" );
	canvas->SetRatioRange( 0.7, 1.3 );
	canvas->Draw( 1, 1 );
}