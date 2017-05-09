#include <Include/PlotTools.h>
void Comp_PDFUnc_aMCNLO_FEWZ()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_FEWZ = ROOTFilePath + "/ROOTFile_SysUnc_Acceptance.root";
	TH1D* h_FEWZ_PDF = Get_Hist( FileName_FEWZ, "h_RelSysUnc_PDF_Percent");
	TH1D* h_FEWZ_AlphaS = Get_Hist( FileName_FEWZ, "h_RelSysUnc_Alpha_Percent");
	TH1D* h_FEWZ = QuadSum_NoError( h_FEWZ_PDF, h_FEWZ_AlphaS );

	TString FileName_aMCNLO = "ROOTFile_PDFUnc_aMCNLO.root";
	TH1D* h_aMCNLO = Get_Hist( FileName_aMCNLO, "h_RMS" );
	h_aMCNLO->Scale( 100 ); // -- convert to % -- //

	HistInfo *Hist_FEWZ = new HistInfo( kBlue, "FEWZ (NNLO)" );
	Hist_FEWZ->Set_Histogram( h_FEWZ );
	Hist_FEWZ->Set();

	HistInfo *Hist_aMCNLO = new HistInfo( kRed, "aMC@NLO" );
	Hist_aMCNLO->Set_Histogram( h_aMCNLO );
	Hist_aMCNLO->Set();

	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( "c_PDFUnc_aMCNLO_FEWZ", Hist_aMCNLO, Hist_FEWZ );
	canvas->SetTitle("m [GeV]", "PDF+#alpha_{s} Uncertainty (%)", "aMC@NLO/FEWZ");
	canvas->SetYRange( 0, 6 );
	canvas->SetRatioRange( 0, 1.5 );
	canvas->SetLegendPosition( 0.55, 0.85, 0.95, 0.95 );
	canvas->SetLatex( "Simulation" );
	canvas->SetDrawOption( "HISTLPSAME" );
	canvas->Draw( 1, 0 );

	// Print_Histogram( h_aMCNLO );
}