#include <Include/PlotTools.h>

void Comp_Small_vs_LargeStatistics()
{
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_Small = AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/Test_v01_XCheck_Electron/ROOTFile_Acc_Dressed_EE.root";
	TString GraphName_Small = "g_Acc";
	TGraphAsymmErrors *g_Small = Get_Graph( FileName_Small, GraphName_Small );
	TH1D* h_Small = Convert_GraphToHist( g_Small );

	TString FileName_Large = AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/Test_v01_XCheck_Electron/Test_v03_LargerStatistics/ROOTFile_Acc_Dressed_EE.root";
	TString GraphName_Large = "g_Acc";
	TGraphAsymmErrors *g_Large = Get_Graph( FileName_Large, GraphName_Large );
	TH1D* h_Large = Convert_GraphToHist( g_Large );

	HistInfo* Hist_Small = new HistInfo( kBlack, "Nominal one", h_Small );
	HistInfo* Hist_Large = new HistInfo( kRed, "Large statistics for M10-50 & M50-inf", h_Large );

	TString CanvasName = "Local/Comp_Small_vs_LargeStatistics";
	DrawCanvas_TwoHistRatio* canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_Large, Hist_Small );
	canvas->SetTitle("m [GeV]", "Acceptance", "Large/Small");
	canvas->SetLegendPosition( 0.50, 0.32, 0.97, 0.45 );
	canvas->SetRatioRange( 0.94, 1.06 );
	canvas->SetLatex("Simulation");
	canvas->Draw(1, 0);
	canvas->c->cd();
	canvas->TopPad->cd();
	canvas->latex.DrawLatexNDC(0.16, 0.915, "#scale[0.8]{#font[42]{aMC@NLO, e channel}}");
	canvas->c->SaveAs(".pdf");

}