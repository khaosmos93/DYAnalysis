#include <Include/PlotTools.h>

void Compare_MyXCheck_vs_Andrius()
{
	TString BasePath = gSystem->Getenv("KP_ROOTFILE_PATH");

	TH1D* h_sqrtN = Get_Hist( "./Local/ROOTFile_Output_StatUnc_StepByStep.root", "h_1OverSqrtN"); h_sqrtN->Scale( 100 ); // -- convert to % -- //
	TH1D* h_KP = Get_Hist( BasePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent" );
	TH1D* h_MyXCheck = Get_Hist( "./Local/ROOTFile_Output_StatUnc_StepByStep.root", "h_RelStatUnc_DiffXSec"); h_MyXCheck->Scale( 100 );

	
	TH1D* h_DiffXSec_Andrius = Get_Hist( BasePath + "/dymm_study_yieldErrPropagation_5000.root", "h1__cs" );
	TH1D* h_Andrius = Extract_RelUnc( h_DiffXSec_Andrius ); h_Andrius->Scale( 100 );

	HistInfo *Hist_sqrtN = new HistInfo( kBlack, "1 / #sqrt{N^{obs}}");
	Hist_sqrtN->Set_Histogram( h_sqrtN );
	Hist_sqrtN->Set();

	HistInfo *Hist_KP = new HistInfo( kBlue, "Stat. by KP" );
	Hist_KP->Set_Histogram( h_KP );
	Hist_KP->Set();
	Hist_KP->Calc_RatioHist_Denominator( Hist_sqrtN->h );

	HistInfo *Hist_MyXCheck = new HistInfo( kRed, "Andrius' method done by KP" );
	Hist_MyXCheck->Set_Histogram( h_MyXCheck );
	Hist_MyXCheck->Set();
	Hist_MyXCheck->Calc_RatioHist_Denominator( Hist_sqrtN->h );

	HistInfo *Hist_Andrius = new HistInfo( kGreen+2, "Stat. by Andrius" );
	Hist_Andrius->Set_Histogram( h_Andrius );
	Hist_Andrius->Set();
	Hist_Andrius->Calc_RatioHist_Denominator( Hist_sqrtN->h );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "Compare_RelStatUnc_IncludeXCheck", TopPad, BottomPad, 1, 1 );

	c->cd();
	TopPad->cd();

	Hist_sqrtN->h->Draw("HISTLPSAME");
	Hist_KP->h->Draw("HISTLPSAME");
	Hist_MyXCheck->h->Draw("HISTLPSAME");
	Hist_Andrius->h->Draw("HISTLPSAME");

	SetHistFormat_TopPad( Hist_sqrtN->h, "Stat. uncertainty (%)" );

	TLegend *legend;
	SetLegend( legend, 0.15, 0.70, 0.60, 0.95 );
	legend->SetTextFont(62);
	legend->AddEntry( Hist_sqrtN->h, Hist_sqrtN->LegendName );
	legend->AddEntry( Hist_KP->h, Hist_KP->LegendName );
	legend->AddEntry( Hist_MyXCheck->h, Hist_MyXCheck->LegendName );
	legend->AddEntry( Hist_Andrius->h, Hist_Andrius->LegendName );
	legend->Draw();

	TLatex latex;
	Latex_Preliminary( latex, 2.8, 13 );

	c->cd();
	BottomPad->cd();

	Hist_KP->h_ratio->Draw("HISTLPSAME");
	Hist_MyXCheck->h_ratio->Draw("HISTLPSAME");
	Hist_Andrius->h_ratio->Draw("HISTLPSAME");

	SetHistFormat_BottomPad( Hist_KP->h_ratio, "m (#mu#mu) [GeV]", "Ratio to 1/#sqrt{N}", 0.9, 3.0 );
	Hist_KP->h_ratio->GetYaxis()->CenterTitle();
	Hist_KP->h_ratio->GetYaxis()->SetTitleOffset(0.4);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}