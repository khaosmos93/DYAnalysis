#include <Include/PlotTools.h>

void Comparison_Ratio_fiducial_4pi()
{
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_fiducial = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";
	TH1D* h_fiducial = Get_Hist( FileName_fiducial, "h_ratio" );

	TString FileName_4pi = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/FullPhaseSpace/ROOTFile_DYPI_DY_FEWZ.root";
	TH1D* h_4pi = Get_Hist( FileName_4pi, "h_ratio" );

	HistInfo *Hist_fiducial = new HistInfo( kBlue, "Fiducial" );
	Hist_fiducial->Set_Histogram( h_fiducial );
	Hist_fiducial->Set();

	HistInfo *Hist_4pi = new HistInfo( kRed, "Full phase space (4#pi)" );
	Hist_4pi->Set_Histogram( h_4pi );
	Hist_4pi->Set();

	DrawCanvas_TwoHistRatio* canvas = new DrawCanvas_TwoHistRatio( "c_Ratio_fiducial_4pi", Hist_4pi, Hist_fiducial );
	canvas->SetTitle("m [GeV]", "#sigma_{DY+PI}/#sigma_{DY}", "4#pi/fiducial");
	canvas->SetLegendPosition(0.17, 0.76, 0.60, 0.90);
	canvas->SetLatex( "Simulation" );
	canvas->SetRatioRange(0.9, 1.5);
	canvas->Draw( 1, 0 );
	canvas->c->cd();
	canvas->TopPad->cd();
	canvas->latex.DrawLatexNDC(0.18, 0.91, "#font[42]{#scale[0.7]{FEWZ, NLO (PDF: MRST2004qed)}}");
	canvas->c->SaveAs(".pdf");
}