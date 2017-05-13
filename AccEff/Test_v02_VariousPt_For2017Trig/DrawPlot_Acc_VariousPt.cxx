#include <Include/PlotTools.h>

void DrawPlot_Acc_VariousPt()
{
	const Int_t nPtCut = 4;
	Double_t Arr_PtCut[nPtCut] = {22, 26, 32, 35};
	Int_t Arr_Color[nPtCut] = { kRed, kBlue, kGreen+1, kBlack };
	TString Arr_Legend[nPtCut] = {"(22, 10) (2015)" , "(26, 10) (2016)", "(32, 10) (2017, scenario1)", "(35, 10) (2017, scenario2)"};

	TString FileName = "ROOTFile_Histogram_Acc_VariousPtCut_aMCNLO.root";

	TGraphAsymmErrors* g_Acc[nPtCut];
	GraphInfo *Graph_Acc[nPtCut];
	for(Int_t i=0; i<nPtCut; i++)
	{
		TString GraphName = TString::Format("g_Acc_%0.lf_10", Arr_PtCut[i_pt]);
		g_Acc[i] = Get_Graph( FileName, GraphName );

		Graph_Acc[i] = new GraphInfo( Arr_Color[i], Arr_Legend[i] );
		Graph_Acc[i]->Set_Graph( g_Acc[i] );
		if( i != 0 )
			Graph_Acc[i]->CalcRatio_DEN( g_Acc[0] ); // -- ratio w.r.t 2015 acceptance -- //
	}

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "c_Acc_VariousPt", TopPad, BottomPad );
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

	TLegend *legend;
	SetLegend( legend );
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

	SetGraphFormat_BottomPad( Graph_Acc[1]->g_ratio, "m [GeV]", "Ratio to 2015", 0, 1 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}