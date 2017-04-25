#include <Include/PlotTools.h>

void DrawCanvas_CompSyst()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TH1D* h_before = Get_Hist( ROOTFilePath+"/ROOTFile_SysUnc_EffSF_TagProbe_UnbinnedFit.root", "h_RMS");
	h_before->Scale( 100 );
	HistInfo *Hist_before = new HistInfo( kBlue, "Alt.Sig = CB+Voig" );
	Hist_before->Set_Histogram( h_before );
	Hist_before->Set();

	TH1D* h_after = Get_Hist( "./ROOTFile_SysUnc_EffSF_TagProbe_UnbinnedFit.root", "h_RMS" );
	h_after->Scale( 100 );
	HistInfo *Hist_after = new HistInfo( kGreen+2, "Alt.Sig = MC Template");
	Hist_after->Set_Histogram( h_after );
	Hist_after->Set();


	DrawCanvas_TwoHistRatio *canvas
	= new DrawCanvas_TwoHistRatio( "c_Comp_before_after", Hist_after, Hist_before );
	canvas->SetTitle( "m [GeV]", "Rel. Unc. (%)", "MCTemp./CB+Voig");
	canvas->SetLatex( "Preliminary", 2.8, 13 );
	canvas->SetDrawOption( "HISTLPSAME" );
	canvas->SetLegendXRange( 0.15, 0.55 );
	canvas->SetLegendYRange( 0.80, 0.95 );
	canvas->SetYRange( 0, 5 );
	canvas->SetRatioRange( 0.9, 2.0 );
	canvas->Draw( 1, 0 );
	canvas->TopPad->cd();
	canvas->latex.DrawLatexNDC( 0.59, 0.91, "#font[42]{#scale[0.6]{Syst. from Tag&Probe method}}");
	canvas->c->SaveAs(".pdf");


}