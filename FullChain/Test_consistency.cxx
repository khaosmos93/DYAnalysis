#include <Include/PlotTools.h>
void Test_consistency()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_Ref = ROOTFilePath + "/ROOTFile_Results_DYAnalysis_76X.root";
	TH1D* h_Ref = Get_Hist( FileName_Ref, "h_DiffXsec_FSRCorr");

	TString FileName_New = "./Local/ROOTFile_Results_DYAnalysis_76X.root";
	TH1D* h_New = Get_Hist( FileName_New, "h_DiffXsec_FSRCorr" );

	HistInfo *Hist_Ref = new HistInfo( kBlack, "Reference" );
	Hist_Ref->Set_Histogram( h_Ref );
	Hist_Ref->Set();

	HistInfo *Hist_New = new HistInfo( kRed, "New one" );
	Hist_New->Set_Histogram( h_New );
	Hist_New->Set();

	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( "c_DiffXSec_New_Ref", Hist_New, Hist_Ref );
	canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]", "New/Ref");
	canvas->SetLatex("Preliminary", 2.8, 13);
	canvas->SetRatioRange(0.99, 1.01);
	canvas->Draw(1, 1);
}