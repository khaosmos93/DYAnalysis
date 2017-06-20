#include "FullChain.h"

void FullChain()
{
	DXSecTool *tool = new DXSecTool();
	tool->Run_FullChain();

	// // -- x-check -- //
	// TH1D* h_New = (TH1D*)tool->h_DiffXSec->Clone();
	// TH1D* h_Old = Get_Hist( tool->ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_DiffXsec_FSRCorr");

	// HistInfo *Hist_New = new HistInfo( kRed, "New", h_New );
	// HistInfo *Hist_Old = new HistInfo( kBlack, "Old", h_Old );

	// TString CanvasName = "Local/XCheck_Old_vs_New";
	// DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_New, Hist_Old );
	// canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "New/Old");
	// canvas->SetLegendPosition( 0.70, 0.83, 0.95, 0.95 );
	// canvas->SetRatioRange( 0.9, 1.1 );
	// canvas->SetLatex( "Preliminary", 2.8, 13 );
	// canvas->Draw(1, 1);
}