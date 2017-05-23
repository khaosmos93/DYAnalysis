#include <Include/PlotTools.h>

void DrawCanvas_StatUnc()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TH1D* h_WithCorr = Get_Hist("../ROOTFile_RelStatUnc.root", "h_RelStatUnc_Percent");
	TH1D* h_WithCorr_FpoF = Get_Hist("../ROOTFile_RelStatUnc.root", "h_FpoF_RelStatUnc_Percent");

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TH1D* h_NoCorr = Get_Hist(ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_RelUnc_Stat_7TeVMethod");
	h_NoCorr->Scale( 100 ); // -- convert to % -- //

	HistInfo *Hist_WithCorr = new HistInfo( kBlue, "Randomized yield" );
	Hist_WithCorr->Set_Histogram( h_WithCorr );
	Hist_WithCorr->Set();

	HistInfo *Hist_WithCorr_FpoF = new HistInfo( kGreen+2, "Randomized yield, post-FSR" );
	Hist_WithCorr_FpoF->Set_Histogram( h_WithCorr_FpoF );
	Hist_WithCorr_FpoF->Set();

	HistInfo *Hist_NoCorr = new HistInfo( kRed, "7 TeV method" );
	Hist_NoCorr->Set_Histogram( h_NoCorr );
	Hist_NoCorr->Set();

	TString CanvasName = "c_Comp_RelStatUnc";
	TCanvas *c; TPad *TopPad; TPad *BottomPad;

	SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 0 );
	c->cd();
	TopPad->cd();

	Hist_NoCorr->Draw("HISTLPSAME");
	Hist_WithCorr_FpoF->Draw("HISTLPSAME");
	Hist_WithCorr->Draw("HISTLPSAME");

	SetHistFormat_TopPad( Hist_NoCorr->h, "Stat. Unc. (%)" );


}