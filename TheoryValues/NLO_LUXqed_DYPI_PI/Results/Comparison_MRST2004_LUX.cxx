#include <Include/PlotTools.h>

void Comparison_MRST2004_LUX()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_MRST2004 = AnalyzerPath+"/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";
	TString FileName_LUX = "ROOTFile_DYPI_DY_FEWZ.root";

	TH1D* h_MRST2004 = Get_Hist( FileName_MRST2004, "h_ratio" );
	TH1D* h_LUX = Get_Hist( FileName_LUX, "h_ratio" );

	HistInfo *Hist_MRST2004 = new HistInfo( kBlack, "MRST2004qed", h_MRST2004 );
	HistInfo *Hist_LUX = new HistInfo( kRed, "LUXqed (with PDF4LHC15)", h_LUX );
	Hist_LUX->CalcRatio_DEN( Hist_MRST2004->h );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	TString CanvasName = "Local/DYPI_PI_MRST2004_LUX";
	SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 0 );
	c->cd();
	TopPad->cd();

	Hist_MRST2004->Draw("EPSAME");
	Hist_LUX->Draw("EPSAME");
	SetHistFormat_TopPad( Hist_MRST2004->h, "(DY+PI)/DY");

	TLegend *legend;
	SetLegend( legend, 0.50, 0.32, 0.97, 0.45 );
	Hist_MRST2004->AddToLegend( legend );
	Hist_LUX->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	Latex_Simulation( latex );
	latex.DrawLatexNDC( 0.16, 0.915, "#scale[0.8]{#font[42]{#mu channel}}");
	latex.DrawLatexNDC( 0.16, 0.87, "#scale[0.6]{#font[42]{QCD@NLO, EWK@NLO by FEWZ}}");
	latex.DrawLatexNDC( 0.16, 0.83, "#scale[0.6]{#font[42]{P_{T}^{lead} > 22 GeV, P_{T}^{sub} > 10 GeV, |#eta| < 2.4}}");

	c->cd();
	BottomPad->cd();
	Hist_LUX->DrawRatio("EPSAME");
	SetHistFormat_BottomPad( Hist_LUX->h_ratio, "m [GeV]", "LUX/MRST", 0.94, 1.06 );

	TF1 *line;
	DrawLine( line );

	c->SaveAs(".pdf");
}