#include <Include/PlotTools.h>

void DrawCanvas( TGraphAsymmErrors* g_MC, TGraphAsymmErrors* g_Data, TString BaseGraphName, TString Region );

void Comparison_Benjamin()
{
	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	vector< TString > vec_BaseGraphName;
	vec_BaseGraphName.push_back("g_Data");
	vec_BaseGraphName.push_back("g_MC");

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_Benjamin = AnalyzerPath + "/Systematic_Eff/test_v19_noBadMuonFlag/WithFlag/Local/ROOTFile_Output_Systematic_Eff.root";
	TString FileName_KP = "./Local/ROOTFile_Output_Systematic_Eff.root";

	for( const auto &Region : vec_Region )
	{
		for( const auto &BaseGraphName : vec_BaseGraphName )
		{
			TString GraphName = TString::Format("%s_DEN_NUM_%s", BaseGraphName.Data(), Region.Data() );
			TGraphAsymmErrors* g_Benjamin = Get_Graph( FileName_Benjamin, GraphName );
			TGraphAsymmErrors* g_KP = Get_Graph( FileName_KP, GraphName );

			DrawCanvas( g_Benjamin, g_KP, BaseGraphName, Region );
		}
	}
}

void DrawCanvas( TGraphAsymmErrors* g_Benjamin, TGraphAsymmErrors* g_KP, TString BaseGraphName, TString Region )
{
	GraphInfo *Graph_Benjamin = new GraphInfo( kRed, "Benjamin" );
	Graph_Benjamin->Set_Graph( g_Benjamin );

	GraphInfo *Graph_KP = new GraphInfo( kBlue, "KP" );
	Graph_KP->Set_Graph( g_KP );
	Graph_KP->Calc_RatioGraph_Denominator( Graph_Benjamin->g );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	TString CanvasName = TString::Format("c_Eff_%s_%s", BaseGraphName.Data(), Region.Data());
	SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad );

	c->cd();
	TopPad->cd();

	Graph_Benjamin->DrawGraph( "APSAME" );
	Graph_KP->DrawGraph( "PSAME" );

	SetGraphFormat_TopPad( Graph_Benjamin->g, "Efficiency" );
	Graph_Benjamin->g->GetYaxis()->SetRangeUser( 0.5, 1.05 );

	Graph_Benjamin->g->SetMarkerSize(1.5);
	Graph_KP->g->SetMarkerSize(1.5);

	TLegend *legend;
	SetLegend( legend, 0.15, 0.30, 0.60, 0.50);

	legend->AddEntry( Graph_Benjamin->g, Graph_Benjamin->LegendName );
	legend->AddEntry( Graph_KP->g, Graph_KP->LegendName );

	legend->Draw();

	// TLatex latex;
	// Latex_Preliminary( latex, 35.9, 13 );

	c->cd();
	BottomPad->cd();

	Graph_KP->g_ratio->Draw("APSAME");
	SetGraphFormat_BottomPad( Graph_KP->g_ratio, "m [GeV]", "KP/Benjamin", 0.98, 1.02 );
	Graph_KP->g_ratio->GetYaxis()->SetNdivisions(505);

	Graph_KP->g_ratio->SetMarkerSize(1.5);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}