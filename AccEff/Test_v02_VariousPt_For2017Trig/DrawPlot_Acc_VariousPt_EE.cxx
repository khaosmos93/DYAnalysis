#include <Include/PlotTools.h>

void DrawPlot_Acc_VariousPt_EE()
{
	const Int_t nPtCut = 3;
	Double_t Arr_PtCut[nPtCut] = {30, 38+5, 23+5 };
	Double_t Arr_SubPtCut[nPtCut] = {10, 10, 12+5 };
	Int_t Arr_Color[nPtCut] = { kRed, kBlue, kGreen+1 };
	TString Arr_Legend[nPtCut] = {"(P_{T}^{lead}, P_{T}^{sub})=(30, 10) (2015)", "(P_{T}^{lead}, P_{T}^{sub})=(43, 10) (2017, single e trigger)", "(P_{T}^{lead}, P_{T}^{sub})=(28, 17) (2017, double e trigger)"};

	TString FileName = "ROOTFile_Histogram_Acc_VariousPtCut_aMCNLO_EE.root";

	TGraphAsymmErrors* g_Acc[nPtCut];
	GraphInfo *Graph_Acc[nPtCut];
	for(Int_t i=0; i<nPtCut; i++)
	{
		TString GraphName = TString::Format("g_Acc_%.0lf_%.0lf", Arr_PtCut[i], Arr_SubPtCut[i]);
		g_Acc[i] = Get_Graph( FileName, GraphName );

		Graph_Acc[i] = new GraphInfo( Arr_Color[i], Arr_Legend[i] );
		Graph_Acc[i]->Set_Graph( g_Acc[i] );
		if( i != 0 )
			Graph_Acc[i]->CalcRatio_DEN( g_Acc[0] ); // -- ratio w.r.t 2015 acceptance -- //
	}

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "c_Acc_VariousPt_EE", TopPad, BottomPad, 1, 1 );
	c->cd();
	TopPad->cd();

	for(Int_t i=0; i<nPtCut; i++)
	{
		if( i == 0 )
			Graph_Acc[i]->Draw("APSAME");
		else
			Graph_Acc[i]->Draw("PSAME");
	}

	SetGraphFormat_TopPad( Graph_Acc[0]->g, "Acceptance" );
	Graph_Acc[0]->g->GetYaxis()->SetRangeUser( 5e-4, 1.1 );

	TLegend *legend;
	SetLegend( legend, 0.25, 0.33, 0.95, 0.55 );
	for(Int_t i=0; i<nPtCut; i++)
		Graph_Acc[i]->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	Latex_Simulation( latex );

	c->cd();
	BottomPad->cd();

	for(Int_t i=1; i<nPtCut; i++)
	{
		if( i == 1 )
			Graph_Acc[i]->DrawRatio("APSAME");
		else
			Graph_Acc[i]->DrawRatio("PSAME");
	}

	SetGraphFormat_BottomPad( Graph_Acc[1]->g_ratio, "m [GeV]", "Ratio to 2015", 0.2, 1.05 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}