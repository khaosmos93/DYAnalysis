#include <Include/PlotTools.h>

void DrawPlot_Acc_VariousPt()
{
	const Int_t nPtCut = 3;
	Double_t Arr_PtCut[nPtCut] = {22, 29, 20};
	Int_t Arr_Color[nPtCut] = { kRed, kBlue, kGreen+1 };
	TString Arr_Legend[nPtCut] = {"(P_{T}^{lead}, P_{T}^{sub})=(22, 10) (2015)", "(P_{T}^{lead}, P_{T}^{sub})=(29, 10) (2017, single #mu trigger)", "(P_{T}^{lead}, P_{T}^{sub})=(20, 10) (2017, double #mu trigger)"};

	TString FileName = "ROOTFile_Histogram_Acc_VariousPtCut_aMCNLO_MM.root";

	TGraphAsymmErrors* g_Acc[nPtCut];
	GraphInfo *Graph_Acc[nPtCut];
	for(Int_t i=0; i<nPtCut; i++)
	{
		TString GraphName = TString::Format("g_Acc_%.0lf_10", Arr_PtCut[i]);
		g_Acc[i] = Get_Graph( FileName, GraphName );

		Graph_Acc[i] = new GraphInfo( Arr_Color[i], Arr_Legend[i] );
		Graph_Acc[i]->Set_Graph( g_Acc[i] );
		if( i != 0 )
			Graph_Acc[i]->CalcRatio_DEN( g_Acc[0] ); // -- ratio w.r.t 2015 acceptance -- //
	}

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "c_Acc_VariousPt", TopPad, BottomPad, 1, 1 );
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

	SetGraphFormat_BottomPad( Graph_Acc[1]->g_ratio, "m [GeV]", "Ratio to 2015", 0.3, 1.7 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}