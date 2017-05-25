#include <Include/PlotTools.h>

void DrawRatio_MuAcc_ElecAcc()
{
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_MM = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";
	TString FileName_EE = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ElectronAcc_30_10/ROOTFile_DYPI_DY_FEWZ.root";

	TH1D *h_MM =  Get_Hist( FileName_MM, "h_ratio" );
	TH1D *h_EE =  Get_Hist( FileName_EE, "h_ratio" );

	HistInfo *Hist_MM = new HistInfo( kBlue, "Dimuon acceptance" );
	Hist_MM->Set_Histogram( h_MM );
	Hist_MM->Set();

	HistInfo *Hist_EE = new HistInfo( kGreen+2, "Dielectron acceptance" );
	Hist_EE->Set_Histogram( h_EE );
	Hist_EE->Set();

	TString CanvasName = "c_Ratio_DYPI_PI_MuAcc_ElecAcc";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_EE, Hist_MM);
	canvas->SetTitle("m [GeV]", "#sigma_{DY+PI}/#sigma_{DY}", "Acc(e)/Acc(#mu)");
	canvas->SetLegendPosition(0.17, 0.75, 0.60, 0.95);
	canvas->SetLatex( "Simulation" );
	canvas->Draw(1, 0);
}