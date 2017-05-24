#include <Include/PlotTools.h>

void DrawCanvas_StatUnc()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TH1D* h_WithCorr = Get_Hist(AnalyzerPath + "/SysUncEstimation/StatUnc/ROOTFile_RelStatUnc.root", "h_RelStatUnc_Percent");
	TH1D* h_WithCorr_FpoF = Get_Hist(AnalyzerPath + "/SysUncEstimation/StatUnc/ROOTFile_RelStatUnc.root", "h_FpoF_RelStatUnc_Percent");

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TH1D* h_NoCorr = Get_Hist(ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_RelUnc_Stat_7TeVMethod");
	h_NoCorr->Scale( 100 ); // -- convert to % -- //

	HistInfo *Hist_NoCorr = new HistInfo( kRed, "7 TeV method" );
	Hist_NoCorr->Set_Histogram( h_NoCorr );
	Hist_NoCorr->Set();

	HistInfo *Hist_WithCorr = new HistInfo( kBlue, "Randomized yield" );
	Hist_WithCorr->Set_Histogram( h_WithCorr );
	Hist_WithCorr->Set();
	Hist_WithCorr->CalcRatio_DEN( Hist_NoCorr->h );

	HistInfo *Hist_WithCorr_FpoF = new HistInfo( kGreen+2, "Randomized yield, post-FSR" );
	Hist_WithCorr_FpoF->Set_Histogram( h_WithCorr_FpoF );
	Hist_WithCorr_FpoF->Set();
	Hist_WithCorr_FpoF->CalcRatio_DEN( Hist_NoCorr->h );

	TString CanvasName = "c_Comp_RelStatUnc";
	TCanvas *c; TPad *TopPad; TPad *BottomPad;

	SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 1 );
	c->cd();
	TopPad->cd();

	Hist_NoCorr->Draw("HISTLPSAME");
	Hist_WithCorr_FpoF->Draw("HISTLPSAME");
	Hist_WithCorr->Draw("HISTLPSAME");

	SetHistFormat_TopPad( Hist_NoCorr->h, "Stat. Unc. (%)");

	TLegend *legend;
	SetLegend( legend, 0.17, 0.72, 0.60, 0.95);
	Hist_NoCorr->AddToLegend( legend );
	Hist_WithCorr_FpoF->AddToLegend( legend );
	Hist_WithCorr->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	Latex_Preliminary( latex, 2.8, 13 );

	c->cd();
	BottomPad->cd();

	Hist_WithCorr->DrawRatio( "HISTLPSAME" );
	Hist_WithCorr_FpoF->DrawRatio( "HISTLPSAME" );

	SetHistFormat_BottomPad( Hist_WithCorr->h_ratio, "m [GeV]", "rndm.yield/7TeV", 0.6, 2.0 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}