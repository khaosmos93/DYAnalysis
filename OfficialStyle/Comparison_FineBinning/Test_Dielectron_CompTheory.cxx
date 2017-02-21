#include "DYAnalysis_76X/Include/DYAnalyzer.h"
#include "DYAnalysis_76X/Include/PlotTools.h"
void Test_Dielectron_CompTheory()
{
	TString FileName_Elec = GetBasePath() + "Include/Results_ROOTFiles_76X/DiffXsec_Electron.root";

	TH1D* h_CenV = Get_Hist( FileName_Elec, "h_DiffXSec" );
	TH1D* h_RelStatUnc = Get_Hist( FileName_Elec, "h_RelUnc_Stat" ); h_RelStatUnc->Scale(0.01);

	TH1D* h_RelSystUnc = Get_Hist( FileName_Elec, "h_RelUnc_Syst" ); h_RelSystUnc->Scale(0.01);
	TH1D* h_RelTotUnc = QuadSum_NoError( h_RelStatUnc, h_RelSystUnc );

	AssignErrors( h_CenV, h_RelTotUnc );

	TString FileName_Data = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_DiffXSec_FullUnc.root";
	TH1D* h_NNLO_DYBin = Get_Hist( FileName_Data, "h_DiffXsec_FEWZ_NNPDF_NNLO" );

	HistInfo *Hist_Theory = new HistInfo( kRed, "FEWZ (NNLO, NNPDF 3.0)" );
	Hist_Theory->Set_Histogram( h_NNLO_DYBin );
	Hist_Theory->Set();

	HistInfo *Hist_data = new HistInfo( kBlack, "Data (ee)" );
	Hist_data->Set_Histogram( h_CenV );
	Hist_data->Set();
	Hist_data->Calc_RatioHist_Denominator( Hist_Theory->h );

	TCanvas *c; TPad *TopPad; TPad* BottomPad;
	SetCanvas_Ratio( c, "Test_Dielectron_CompTheory", TopPad, BottomPad, 1, 1 );

	c->cd();
	TopPad->cd();
	Hist_Theory->h->Draw("E1PSAME");
	Hist_data->h->Draw("E1PSAME");

	SetHistFormat_TopPad( Hist_Theory->h, "d#sigma/dm [pb/GeV]");

	TLegend *legend;
	SetLegend( legend );
	legend->AddEntry( Hist_data->h, Hist_data->LegendName );
	legend->AddEntry( Hist_Theory->h, Hist_Theory->LegendName );
	legend->Draw();

	c->cd();
	BottomPad->cd();

	Hist_data->h_ratio->Draw("EPSAME");
	SetHistFormat_BottomPad( Hist_data->h_ratio, "m [GeV]", "Data/Theory", 0.31, 1.69 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");

}