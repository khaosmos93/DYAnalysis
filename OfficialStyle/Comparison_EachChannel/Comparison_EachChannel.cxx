#include </Users/KyeongPil_Lee/Physics/Include/PlotTools.h>

TString FileName = "./ROOTFile_Input_ForComp_EachChannel.root";

TString HistName_DXSec_mm = "h_DXSec_mm";
TString HistName_RelStatUnc_mm = "h_RelStatUnc_mm";

TString HistName_DXSec_ee = "h_DXSec_ee";
TString HistName_RelStatUnc_ee = "h_RelStatUnc_ee";

TString HistName_DXSec_ll = "h_DXSec_ll";
TString HistName_RelStatUnc_ll = "h_RelStatUnc_ll";
TString HistName_RelSystUnc_ll = "h_RelSystUnc_ll";
TString HistName_RelTotUnc_ll = "h_RelTotUnc_ll";

// -- included in PlotTools.h -- //
// void AssignErrors( TH1D* h_DXSec, TH1D* h_RelUnc, Bool_t isPercent = kFALSE )
// {
// 	Int_t nBin = h_DXSec->GetNbinsX();
// 	for(Int_t i=0; i<nBin; i++)
// 	{
// 		Int_t i_bin = i+1;

// 		Double_t value = h_DXSec->GetBinContent(i_bin);
// 		Double_t RelUnc = h_RelUnc->GetBinContent(i_bin);
// 		if( isPercent ) RelUnc = RelUnc / 100.0;
// 		Double_t AbsUnc = value * RelUnc;

// 		h_DXSec->SetBinError(i_bin, AbsUnc);
// 	}
// }

// -- included in PlotTools.h -- //
// void AssignErrors_Graph( TGraphAsymmErrors* g, TH1D* h_RelUnc, Bool_t isPercent = kFALSE )
// {
// 	Int_t nPoint = g->GetN();
// 	Int_t nBin = h_RelUnc->GetNbinsX();
// 	if( nPoint != nBin )
// 	{
// 		printf("[nPoint != nBin: %d, %d]\n", nPoint, nBin);
// 		return;
// 	}

// 	for(Int_t i=0; i<nPoint; i++)
// 	{
// 		Double_t x = 0;
// 		Double_t y = 0;
// 		g->GetPoint( i, x, y );

// 		Int_t i_bin = i+1;
// 		Double_t RelUnc = h_RelUnc->GetBinContent(i_bin);
// 		if( isPercent ) RelUnc = RelUnc / 100.0;
		
// 		Double_t AbsUnc = y * RelUnc;

// 		g->SetPointEYlow(i, AbsUnc );
// 		g->SetPointEYhigh(i, AbsUnc );
// 	}
// }

TGraphAsymmErrors* ConvertToGraph_Shift_PositionX( TH1D* h, Double_t ShiftDir)
{
	TGraphAsymmErrors *g = new TGraphAsymmErrors();
	TString GraphName = h->GetName();
	GraphName.ReplaceAll("h_", "g_");
	g->SetName(GraphName);

	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t BinCenter = h->GetBinCenter(i_bin);
		Double_t BinWidth = h->GetBinWidth(i_bin);
		Double_t Shift = BinWidth * 0.25;
		Double_t x = BinCenter + ShiftDir * Shift;

		Double_t value = h->GetBinContent(i_bin);
		Double_t error = h->GetBinError(i_bin);

		g->SetPoint(i, x, value);
		g->SetPointError(i, 0, 0, error, error);
	}

	return g;
}

void Comparison_EachChannel_XRange( Double_t xMin, Double_t xMax, Double_t yMin, Double_t yMax )
{
	////////////////////////////
	// -- combined channel -- //
	////////////////////////////
	TH1D* h_DXSec_ll = Get_Hist( FileName, HistName_DXSec_ll );
	TH1D* h_DXSec_ll_withSystUnc = (TH1D*)h_DXSec_ll->Clone();
	TH1D* h_DXSec_ll_withTotUnc = (TH1D*)h_DXSec_ll->Clone();

	TH1D* h_RelStatUnc_ll = Get_Hist( FileName, HistName_RelStatUnc_ll );
	TH1D* h_RelSystUnc_ll = Get_Hist( FileName, HistName_RelSystUnc_ll );
	TH1D* h_RelTotUnc_ll = Get_Hist( FileName, HistName_RelTotUnc_ll );
	
	AssignErrors( h_DXSec_ll, h_RelStatUnc_ll, kFALSE);
	AssignErrors( h_DXSec_ll_withSystUnc, h_RelSystUnc_ll, kFALSE);
	AssignErrors( h_DXSec_ll_withTotUnc, h_RelTotUnc_ll, kFALSE);

	// -- convert from histogram to remove x-axis error -- //
	TGraphAsymmErrors *g_DXSec_ll = ConvertToGraph_Shift_PositionX( h_DXSec_ll, 0 );
	GraphInfo *Graph_DXSec_ll = new GraphInfo( kBlack, "Combined Channel" );
	Graph_DXSec_ll->Set_Graph( g_DXSec_ll );
	Graph_DXSec_ll->Calc_RatioGraph_Denominator( Graph_DXSec_ll->g );
	AssignErrors_Graph( Graph_DXSec_ll->g_ratio, h_RelStatUnc_ll );

	// -- for systematic uncertainty band -- //
	Int_t color_syst = kSpring+2;
	HistInfo *Hist_ll_withSystUnc = new HistInfo( color_syst, "Systematic Uncertainty");
	Hist_ll_withSystUnc->Set_Histogram( h_DXSec_ll_withSystUnc );
	Hist_ll_withSystUnc->Set();
	Hist_ll_withSystUnc->h->SetMarkerColorAlpha( color_syst, 0 );
	Hist_ll_withSystUnc->h->SetFillColorAlpha( color_syst, 1 );
	Hist_ll_withSystUnc->Calc_RatioHist_Denominator( Hist_ll_withSystUnc->h ); // -- all central value: 1 -- //
	AssignErrors( Hist_ll_withSystUnc->h_ratio, h_RelSystUnc_ll, kFALSE);

	// -- for total uncertainty band -- //
	Int_t color_tot = kSpring+1;
	HistInfo *Hist_ll_withTotUnc = new HistInfo( color_tot, "Total Uncertainty (w/o lumi.)");
	Hist_ll_withTotUnc->Set_Histogram( h_DXSec_ll_withTotUnc );
	Hist_ll_withTotUnc->Set();
	Hist_ll_withTotUnc->h->SetMarkerColorAlpha( color_tot, 0 );
	Hist_ll_withTotUnc->h->SetFillColorAlpha( color_tot, 1 );
	Hist_ll_withTotUnc->Calc_RatioHist_Denominator( Hist_ll_withTotUnc->h ); // -- all central value: 1 -- //
	AssignErrors( Hist_ll_withTotUnc->h_ratio, h_RelTotUnc_ll, kFALSE);

	////////////////////////
	// -- muon channel -- //
	////////////////////////
	TH1D* h_DXSec_mm = Get_Hist( FileName, HistName_DXSec_mm );
	TH1D* h_RelStatUnc_mm = Get_Hist( FileName, HistName_RelStatUnc_mm );
	AssignErrors( h_DXSec_mm, h_RelStatUnc_mm, kFALSE);

	// -- convert from histogram to graph in order to shift x position -- //
	TGraphAsymmErrors *g_DXSec_mm = ConvertToGraph_Shift_PositionX( h_DXSec_mm, 1.0 );
	GraphInfo *Graph_DXSec_mm = new GraphInfo( kBlue, "Muon Channel" );
	Graph_DXSec_mm->Set_Graph( g_DXSec_mm );
	Graph_DXSec_mm->Calc_RatioGraph_Denominator( Graph_DXSec_ll->g );
	AssignErrors_Graph( Graph_DXSec_mm->g_ratio, h_RelStatUnc_mm );

	////////////////////////////
	// -- electron channel -- //
	////////////////////////////
	TH1D* h_DXSec_ee = Get_Hist( FileName, HistName_DXSec_ee );
	TH1D* h_RelStatUnc_ee = Get_Hist( FileName, HistName_RelStatUnc_ee );
	AssignErrors( h_DXSec_ee, h_RelStatUnc_ee, kFALSE);

	// -- convert from histogram to graph in order to shift x position -- //
	TGraphAsymmErrors *g_DXSec_ee = ConvertToGraph_Shift_PositionX( h_DXSec_ee, -1.0 );
	GraphInfo *Graph_DXSec_ee = new GraphInfo( kRed, "Electron Channel" );
	Graph_DXSec_ee->Set_Graph( g_DXSec_ee );
	Graph_DXSec_ee->Calc_RatioGraph_Denominator( Graph_DXSec_ll->g );
	AssignErrors_Graph( Graph_DXSec_ee->g_ratio, h_RelStatUnc_ee );

	///////////////////////
	// -- draw canvas -- //
	///////////////////////
	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	Bool_t isLogX = kFALSE;
	if( xMax == 440 || xMax == 3000 )
		isLogX = kTRUE;
	SetCanvas_Ratio( c, "c_Comparison_EachChannel_"+TString::Format("%.0lf_to_%.0lf", xMin, xMax), TopPad, BottomPad, isLogX, 1 );

	c->cd();
	TopPad->cd();

	Hist_ll_withTotUnc->h->Draw("E2PSAME");	
	Hist_ll_withSystUnc->h->Draw("E2PSAME");
	Graph_DXSec_ll->DrawGraph("E1PSAME");
	Graph_DXSec_mm->DrawGraph("E1PSAME");
	Graph_DXSec_ee->DrawGraph("E1PSAME");

	SetHistFormat_TopPad(Hist_ll_withTotUnc->h, "d#sigma/dm [pb/GeV]" );
	Hist_ll_withTotUnc->h->GetXaxis()->SetRangeUser(xMin, xMax);
	Hist_ll_withTotUnc->h->GetYaxis()->SetRangeUser(yMin, yMax);
	Hist_ll_withSystUnc->h->GetXaxis()->SetRangeUser(xMin, xMax);




	Graph_DXSec_ll->g->SetMarkerStyle(20);
	Graph_DXSec_mm->g->SetMarkerStyle(kFullSquare);
	Graph_DXSec_ee->g->SetMarkerStyle(kFullTriangleUp);

	TLegend *legend_Unc;
	SetLegend( legend_Unc, 0.15, 0.37, 0.45, 0.47 );
	legend_Unc->SetTextFont(62);
	legend_Unc->AddEntry( Hist_ll_withSystUnc->h, Hist_ll_withSystUnc->LegendName );
	legend_Unc->AddEntry( Hist_ll_withTotUnc->h, Hist_ll_withTotUnc->LegendName );
	legend_Unc->Draw();

	TLegend *legend_DXSec;
	SetLegend( legend_DXSec, 0.45, 0.32, 0.75, 0.47 );
	legend_DXSec->SetTextFont(62);
	legend_DXSec->AddEntry( Graph_DXSec_ll->g, Graph_DXSec_ll->LegendName );
	legend_DXSec->AddEntry( Graph_DXSec_mm->g, Graph_DXSec_mm->LegendName );
	legend_DXSec->AddEntry( Graph_DXSec_ee->g, Graph_DXSec_ee->LegendName );
	legend_DXSec->Draw();

	TLatex latex;
	Latex_Preliminary_NoDataInfo( latex );
	latex.DrawLatexNDC( 0.55, 0.96, "#font[42]{#scale[0.8]{2.3 fb^{-1} (ee)}}");
	latex.DrawLatexNDC( 0.75, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (#mu#mu)}}");

	c->cd();
	BottomPad->cd();

	Hist_ll_withTotUnc->h_ratio->Draw("E2PSAME");
	Hist_ll_withSystUnc->h_ratio->Draw("E2PSAME");
	Graph_DXSec_ll->g_ratio->Draw("E1PSAME");
	Graph_DXSec_mm->g_ratio->Draw("E1PSAME");
	Graph_DXSec_ee->g_ratio->Draw("E1PSAME");

	SetHistFormat_BottomPad( Hist_ll_withTotUnc->h_ratio, "m [GeV]", "#frac{each channel}{combined}", 0.4, 1.6 );
	Hist_ll_withTotUnc->h_ratio->SetNdivisions(506);
	Hist_ll_withTotUnc->h_ratio->GetXaxis()->SetRangeUser(xMin, xMax);
	Hist_ll_withTotUnc->h_ratio->GetYaxis()->SetTitleOffset( 0.5 );
	Hist_ll_withSystUnc->h_ratio->GetXaxis()->SetRangeUser(xMin, xMax);

	// TF1 *f_line;
	// DrawLine( f_line );

	c->SaveAs(".pdf");
}

void Comparison_EachChannel()
{
	Comparison_EachChannel_XRange( 15, 3000, 1e-8, 1e+3 );
	Comparison_EachChannel_XRange( 15, 60, 1, 1e+3);
	Comparison_EachChannel_XRange( 60, 120, 1e-1, 1e+3);
	Comparison_EachChannel_XRange( 120, 440, 1e-3, 1);
	Comparison_EachChannel_XRange( 440, 3000, 1e-8, 1e-2);
}