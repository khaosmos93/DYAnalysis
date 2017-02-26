#include "ZprimeAnalysis_80X/Include/PlotTools.h"

void Latex_Info( TLatex &latex, TString Type, TString region )
{
	TString TStr_Type = "";
	if( Type == "DEN" ) TStr_Type = "Denominator";
	else if( Type == "" ) TStr_Type = "";
	else TStr_Type = "Numerator";

	TString TStr_Region = "";
	if( region == "BB" ) TStr_Region = "BB";
	if( region == "BEEE" ) TStr_Region = "BE+EE";
	if( region == "All" ) TStr_Region = "All";

	latex.DrawLatexNDC(0.15, 0.91, "#font[42]{#scale[0.8]{"+TStr_Region+"}}");
	latex.DrawLatexNDC(0.15, 0.88, "#font[42]{#scale[0.6]{"+TStr_Type+"}}");
}


class Tool_DrawCompPlots
{
public:
	TString DataType; // -- data or MC -- //
	TString TStr_DataInfo;
	TString DENStr;
	TString NUMStr;
	TString Region;

	Bool_t isRunBtoF;
	Bool_t isRunGtoH;

	Double_t Lumi;
	Double_t Lumi_Total;
	Double_t Lumi_RunBtoF;
	Double_t Lumi_RunGtoH;

	TString BasePath;

	Tool_DrawCompPlots()
	{
		this->BasePath = "/Users/KyeongPil_Lee/Physics/ZprimeAnalysis_80X/Systematic_Eff/";

		this->Lumi = 0;
		this->Lumi_Total = 35867.060;
		this->Lumi_RunBtoF = 19720.882;
		this->Lumi_RunGtoH = 16146.178;
	}

	void Set_DataType(TString _DataType)
	{
		this->DataType = _DataType;
	}

	void Set_Region( TString _Region )
	{
		this->Region = _Region;
	}

	void Set_DEN_NUM( TString _DEN, TString _NUM )
	{
		this->DENStr = _DEN;
		this->NUMStr = _NUM;

		this->TStr_DataInfo = "";
		this->isRunBtoF = kFALSE;
		this->isRunGtoH = kFALSE;

		if( this->DENStr.Contains("RunBtoF") )
		{
			this->isRunBtoF = kTRUE;
			this->Lumi = this->Lumi_RunBtoF;
			this->TStr_DataInfo = "Data, RunB-F";
		}
		else if( this->DENStr.Contains("RunGtoH") )
		{
			this->isRunGtoH = kTRUE;
			this->Lumi = this->Lumi_RunGtoH;
			this->TStr_DataInfo = "Data, RunG-H";
		}
		else
		{
			this->Lumi = this->Lumi_Total;
			if( this->DataType == "Data" )
				this->TStr_DataInfo = "Data";
			else
				this->TStr_DataInfo = "MC";
		}
	}

	void DrawCanvas()
	{
		TString GraphName = TString::Format("g_%s_%s_%s_%s", this->DataType.Data(), this->DENStr.Data(), this->NUMStr.Data(), this->Region.Data());

		TString InputName = "ROOTFile_Output_Systematic_Eff.root";

		GraphInfo *Graph_Old = this->GetGraphInfo_Old(InputName);

		GraphInfo *Graph_PFIso = new GraphInfo(kViolet, "PF isolation");
		Graph_PFIso->Set_Graph( this->BasePath+"test_v05_ChangeIso_PF/Local/"+InputName, GraphName );

		GraphInfo *Graph_Inclusive = new GraphInfo( kBlue, "t#bar{t} inclusive");
		Graph_Inclusive->Set_Graph( this->BasePath+"test_v06_ttbarInclusive/Local/"+InputName, GraphName );

		GraphInfo *Graph_Tagging = new GraphInfo( kGreen+2, "Tagging");
		Graph_Tagging->Set_Graph( this->BasePath+"test_v07_Tagging/Local/"+InputName, GraphName );

		GraphInfo *Graph_All = new GraphInfo( kRed, "PFIso & Inclusive & Tagging");
		Graph_All->Set_Graph( this->BasePath+"test_v08_ApplyAllv050607/Local/"+InputName, GraphName );

		TCanvas *c;
		TString CanvasName = GraphName;
		GraphName.ReplaceAll("g_", "c_");
		SetCanvas_Square( c, CanvasName );

		Graph_Old->DrawGraph( "APSAME" );
		Graph_PFIso->DrawGraph( "PSAME" );
		Graph_Inclusive->DrawGraph( "PSAME" );
		Graph_Tagging->DrawGraph( "PSAME" );
		Graph_All->DrawGraph( "PSAME" );

		SetGraphFormat_SinglePad( Graph_Old->g, "m [GeV]", "Efficiency" );
		Graph_Old->g->GetYaxis()->SetRangeUser(0.5, 1.05);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.15, 0.60, 0.30);
		legend->AddEntry( Graph_Old->g, Graph_Old->LegendName );
		legend->AddEntry( Graph_PFIso->g, Graph_PFIso->LegendName );
		legend->AddEntry( Graph_Inclusive->g, Graph_Inclusive->LegendName );
		legend->AddEntry( Graph_Tagging->g, Graph_Tagging->LegendName );
		legend->AddEntry( Graph_All->g, Graph_All->LegendName );
		legend->Draw();

		TLatex latex;
		if( this->DataType == "Data" )
			Latex_Preliminary( latex, this->Lumi / 1000.0, 13 );
		else
			Latex_Simulation( latex );

		Latex_Info( latex, "", this->Region );
		Latex_DataType( latex );

		c->SaveAs(".pdf");
	}

protected:
	void Latex_DataType( TLatex &latex )
	{
		latex.DrawLatexNDC(0.70, 0.91, "#font[42]{#scale[0.8]{"+this->TStr_DataInfo+"}}");
	}

	GraphInfo* GetGraphInfo_Old( TString InputName )
	{
		GraphInfo *Graph = new GraphInfo(kBlack, "No new condition");

		TString FileName;
		TString GraphName;

		if( this->DataType == "Data" )
		{
			GraphName = TString::Format("g_Data_%s", this->Region.Data() );

			if( this->isRunBtoF )
				FileName = this->BasePath + "test_v03_RunBtoF_RunGtoH/Local/v01_RunBtoF/"+ InputName;

			else if( this->isRunGtoH )
				FileName = this->BasePath + "test_v03_RunBtoF_RunGtoH/Local/v02_RunGtoH/"+ InputName;

			else
				FileName = BasePath + InputName;
		}
		else
		{
			FileName = BasePath + InputName;
			GraphName = TString::Format("g_MC_%s", this->Region.Data() );
		}

		Graph->Set_Graph( FileName, GraphName );

		return Graph;
	}

};
