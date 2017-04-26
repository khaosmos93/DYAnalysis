#include <Include/PlotTools.h>

void Latex_Info( TLatex &latex, TString Type, TString region );
void DrawCanvas( TGraphAsymmErrors* g_MC, TGraphAsymmErrors* g_Data, TGraphAsymmErrors* g_Data_noFlag, TString Region, TFile *f_output );
void DrawComparisonPlot()
{
	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	TString FileName_WithFlag = "./WithFlag/Local/ROOTFile_Output_Systematic_Eff.root";
	TString FileName_WithoutFlag = "./WithoutFlag/Local/ROOTFile_Output_Systematic_Eff.root";

	TFile *f_output = TFile::Open("ROOTFile_DataMC_Ratio.root", "RECREATE");
	for( const auto &Region : vec_Region )
	{
		TString BaseGraphName = TString::Format( "DEN_NUM_%s", Region.Data() );
		TString GraphName_Data = "g_Data_"+BaseGraphName;
		TString GraphName_MC = "g_MC_"+BaseGraphName;

		TGraphAsymmErrors* g_MC = Get_Graph(FileName_WithFlag, GraphName_MC);
		TGraphAsymmErrors* g_Data = Get_Graph( FileName_WithFlag, GraphName_Data );
		TGraphAsymmErrors* g_Data_noFlag = Get_Graph( FileName_WithoutFlag, GraphName_Data );

		DrawCanvas( g_MC, g_Data, g_Data_noFlag, Region, f_output );
	}

	f_output->Close();
}

void DrawCanvas( TGraphAsymmErrors* g_MC, TGraphAsymmErrors* g_Data, TGraphAsymmErrors* g_Data_noFlag, TString Region, TFile *f_output )
{
	GraphInfo *Graph_MC = new GraphInfo( kRed, "MC (DY)" );
	Graph_MC->Set_Graph( g_MC );

	GraphInfo *Graph_data = new GraphInfo( kBlue, " Data (Bkg.Sub., noBadMuon flag)" );
	Graph_data->Set_Graph( g_Data );
	Graph_data->Calc_RatioGraph_Denominator( Graph_MC->g );

	GraphInfo *Graph_data_noFlag = new GraphInfo( kBlack, "Data (Bkg.Sub.)" );
	Graph_data_noFlag->Set_Graph( g_Data_noFlag );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	TString CanvasName = TString::Format("c_Eff_%s", Region.Data());
	SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad );

	c->cd();
	TopPad->cd();

	Graph_MC->DrawGraph( "APSAME" );
	Graph_data_noFlag->DrawGraph( "PSAME" );
	Graph_data->DrawGraph( "PSAME" );

	SetGraphFormat_TopPad( Graph_MC->g, "Efficiency" );
	Graph_MC->g->GetYaxis()->SetRangeUser( 0.5, 1.05 );

	Graph_MC->g->SetMarkerSize(1.5);
	Graph_data->g->SetMarkerSize(1.5);
	Graph_data_noFlag->g->SetMarkerSize(1.5);

	TLegend *legend;
	SetLegend( legend, 0.15, 0.30, 0.60, 0.50);

	legend->AddEntry( Graph_MC->g, Graph_MC->LegendName );
	legend->AddEntry( Graph_data->g, Graph_data->LegendName );
	legend->AddEntry( Graph_data_noFlag->g, Graph_data_noFlag->LegendName );

	legend->Draw();

	TLatex latex;
	Latex_Preliminary( latex, 35.9, 13 );
	Latex_Info( latex, "", Region );

	c->cd();
	BottomPad->cd();

	Graph_data->g_ratio->Draw("APSAME");
	SetGraphFormat_BottomPad( Graph_data->g_ratio, "m [GeV]", "Data/MC", 0.6, 1.1 );
	Graph_data->g_ratio->GetYaxis()->SetNdivisions(505);

	Graph_data->g_ratio->SetMarkerSize(1.5);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");

	f_output->cd();
	Graph_data->g_ratio->Write("g_ratio_"+Region);
}

void Latex_Info( TLatex &latex, TString Type, TString region )
{
	TString TStr_Type = "";
	if( Type.Contains("DEN") ) TStr_Type = "Denominator";
	else if( Type == "" ) TStr_Type = "";
	else TStr_Type = "Numerator";

	TString TStr_Region = "";
	if( region == "BB" ) TStr_Region = "BB";
	if( region == "BEEE" ) TStr_Region = "BE+EE";
	if( region == "All" ) TStr_Region = "All";

	latex.DrawLatexNDC(0.15, 0.91, "#font[42]{#scale[0.8]{"+TStr_Region+"}}");
	latex.DrawLatexNDC(0.15, 0.88, "#font[42]{#scale[0.6]{"+TStr_Type+"}}");
}