#include "ZprimeAnalysis_80X/Include/PlotTools.h"

// -- AN2016_391_v5, Table 11 -- //
Double_t SF_Zpeak( TString region )
{
	Double_t SF = -1;
	if( region == "All" )
		SF = 0.9387;
	else if( region == "BB" )
		SF = 0.9527;
	else if( region == "BEEE" )
		SF = 0.9286;

	if( SF == -1 )
		printf("No corresponding scale factor is available! ... it is set as -1\n");

	return SF;
}

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

class Tool_Systematic_Eff
{
public:
	Int_t i_canvas;
	TFile *f_output;

	TString DENStr;
	TString NUMStr;
	TString Region;

	TH1D* h_data_DEN_BkgSub;
	TH1D* h_data_NUM_BkgSub;

	TH1D* h_DY_DEN;
	TH1D* h_DY_NUM;

	TGraphAsymmErrors* g_data;
	TGraphAsymmErrors* g_MC;

	Tool_Systematic_Eff()
	{
		i_canvas = 0;
	}

	void Set_Output( TFile *_f_output )
	{
		this->f_output = _f_output;
	}

	void Set_DEN_NUM( TString _DENStr, TString _NUMStr )
	{
		this->DENStr = _DENStr;
		this->NUMStr = _NUMStr;
	}

	void Set_Region( TString _Region )
	{
		this->Region = _Region;
	}

	void DrawCanvas_Mass()
	{
		this->DrawCanvas_EachTypeAndRegion( this->DENStr, this->Region );
		this->DrawCanvas_EachTypeAndRegion( this->NUMStr, this->Region );
	}

	void DrawCanvas_Eff_Data_vs_MC()
	{
		this->g_data = this->EfficiencyGraph( "Data" );
		this->g_MC = this->EfficiencyGraph( "MC" );
		
		GraphInfo *Graph_MC = new GraphInfo( kRed, "MC (DY)" );
		Graph_MC->Set_Graph( this->g_MC );

		GraphInfo *Graph_data = new GraphInfo( kBlack, "Data, RunG-H (Bkg.Sub.)" );
		Graph_data->Set_Graph( this->g_data );
		Graph_data->Calc_RatioGraph_Denominator( Graph_MC->g );

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = TString::Format("c_Eff_%s_%s_%s", this->DENStr.Data(), this->NUMStr.Data(), this->Region.Data());
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad );

		c->cd();
		TopPad->cd();

		Graph_MC->DrawGraph( "APSAME" );
		Graph_data->DrawGraph( "PSAME" );

		SetGraphFormat_TopPad( Graph_MC->g, "Efficiency" );
		Graph_MC->g->GetYaxis()->SetRangeUser( 0.83, 1.05 );

		Graph_MC->g->SetMarkerSize(1.5);
		Graph_data->g->SetMarkerSize(1.5);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.30, 0.45, 0.45);

		legend->AddEntry( Graph_data->g, Graph_data->LegendName );
		legend->AddEntry( Graph_MC->g, Graph_MC->LegendName );

		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 16.1, 13 );
		Latex_Info( latex, "", this->Region );

		c->cd();
		BottomPad->cd();

		Graph_data->g_ratio->Draw("APSAME");
		SetGraphFormat_BottomPad( Graph_data->g_ratio, "m [GeV]", "Data/MC", 0.8, 1.05 );

		Graph_data->g_ratio->SetMarkerSize(1.5);

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");

		// Print_Graph( Graph_data->g_ratio );
	}

protected:
	void DrawCanvas_EachTypeAndRegion( TString Type, TString region )
	{
		// if( !(Type == "DEN" || Type == "NUM") )
		// {
		// 	printf("[%s is not correct Type]\n", Type.Data() );
		// 	return;
		// }

		if( !(region == "All" || region == "BB" || region == "BEEE" ) )
		{
			printf("[%s is not correct region]\n", region.Data() );
			return;
		}

		TString HistName = TString::Format("h_mass_%s_%s", region.Data(), Type.Data());
		if( region == "All" )
			HistName = TString::Format("h_mass_%s", Type.Data());

		TH1D* h_data = Get_Hist( "ROOTFile_DataRunGtoH.root", HistName );
		HistInfo *Hist_data = new HistInfo( kBlack, "Data" );
		Hist_data->Set_Histogram( h_data );
		Hist_data->Set();

		TH1D* h_DY = Get_Hist( "ROOTFile_DYPowheg.root", HistName );
		HistInfo *Hist_DY = new HistInfo( kPink, "Z/#gamma*#rightarrow#mu#mu" );
		Hist_DY->Set_Histogram( h_DY );
		Hist_DY->Set();
		Hist_DY->h->SetFillColorAlpha( Hist_DY->Color, 1 );

		// -- ttbar + tW + tbarW -- //
		TH1D* h_ttbar = Get_Hist( "ROOTFile_ttbarTo2L2Nu.root", HistName);
		TH1D* h_tW = Get_Hist( "ROOTFile_tW.root", HistName );
		TH1D* h_tbarW = Get_Hist( "ROOTFile_tbarW.root", HistName );

		TH1D* h_top = (TH1D*)h_ttbar->Clone(); h_top->Sumw2();
		h_top->Add( h_tW );
		h_top->Add( h_tbarW );

		HistInfo *Hist_top = new HistInfo( kYellow, "t#bar{t}+tW+#bar{t}W" );
		Hist_top->Set_Histogram( h_top );
		Hist_top->Set();
		Hist_top->h->SetFillColorAlpha( Hist_top->Color, 1 );

		// -- dibosons -- //
		TH1D* h_WW = Get_Hist("ROOTFile_WWTo2L2Nu.root", HistName );
		TH1D* h_WZ = Get_Hist("ROOTFile_WZ.root", HistName );
		TH1D* h_ZZ = Get_Hist("ROOTFile_ZZ.root", HistName );

		TH1D* h_diboson = (TH1D*)h_WW->Clone(); h_diboson->Sumw2();
		h_diboson->Add( h_WZ );
		h_diboson->Add( h_ZZ );

		HistInfo *Hist_diboson = new HistInfo( kBlue, "Diboson" );
		Hist_diboson->Set_Histogram( h_diboson );
		Hist_diboson->Set();
		Hist_diboson->h->SetFillColorAlpha( Hist_diboson->Color, 1 );

		// -- rebin -- //
		Int_t RebinSize = 50;
		TString YTitle = TString::Format("Events / %.0d GeV", RebinSize);
		Hist_data->h->Rebin( RebinSize );
		Hist_DY->h->Rebin( RebinSize );
		Hist_top->h->Rebin( RebinSize );
		Hist_diboson->h->Rebin( RebinSize );
		Double_t xMin = 50;

		// TString YTitle = "Entries per bin";
		// Hist_data->h = Rebin_Mass( Hist_data->h );
		// Hist_DY->h = Rebin_Mass( Hist_DY->h );
		// Hist_top->h = Rebin_Mass( Hist_top->h );
		// Hist_diboson->h = Rebin_Mass( Hist_diboson->h );
		// Double_t xMin = 60;

		// -- apply scale factor (data/MC @ Z-peak) -- //
		// Double_t SF = SF_Zpeak( region );
		Double_t SF = this->SF_Zpeak_DirectlyExtracted( Type, Hist_data->h, Hist_DY->h, Hist_top->h, Hist_diboson->h );
		Hist_DY->h->Scale( SF );
		Hist_top->h->Scale( SF );
		Hist_diboson->h->Scale( SF );

		// -- MC stack -- //
		THStack *hs = new THStack("hs", "");
		hs->Add( Hist_diboson->h );
		hs->Add( Hist_top->h );
		hs->Add( Hist_DY->h );

		// -- total MC (for ratio) -- //
		TH1D* h_totMC = (TH1D*)Hist_DY->h->Clone("h_totMC"); h_totMC->Sumw2();
		h_totMC->Add( Hist_top->h );
		h_totMC->Add( Hist_diboson->h );

		// -- draw canvas -- //
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = TString::Format("c%02d_Mass_%s_%s", i_canvas, Type.Data(), region.Data() ); i_canvas++;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 0, 1 );

		c->cd();
		TopPad->cd();

		TH1D* h_format = (TH1D*)Hist_data->h->Clone();
		h_format->Reset("ICES");
		h_format->Draw("");
		hs->Draw("histsame");
		Hist_data->h->Draw("EPsame");
		h_format->Draw("axissame");

		SetHistFormat_TopPad( h_format,  YTitle);
		h_format->GetXaxis()->SetRangeUser(xMin, 2500);
		h_format->GetYaxis()->SetRangeUser(1e-3, 1e7);

		TLegend *legend;
		SetLegend( legend, 0.75, 0.75, 0.95, 0.95 );
		legend->SetTextFont(62);
		legend->AddEntry( Hist_data->h, Hist_data->LegendName );
		legend->AddEntry( Hist_DY->h, Hist_DY->LegendName );
		legend->AddEntry( Hist_top->h, Hist_top->LegendName );
		legend->AddEntry( Hist_diboson->h, Hist_diboson->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 16.1, 13 );
		Latex_Info( latex, Type, region );

		c->cd();
		BottomPad->cd();
		TH1D* h_ratio = (TH1D*)Hist_data->h->Clone();
		h_ratio->Divide( Hist_data->h, h_totMC );
		h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( h_ratio, "m [GeV]", "Data/MC", 0, 2.5);
		h_ratio->GetXaxis()->SetRangeUser(xMin, 2500);

		// Print_Histogram( h_ratio );

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");

		if( Type == this->DENStr )
		{
			this->h_DY_DEN = (TH1D*)Hist_DY->h->Clone(); // -- after applying SF -- //
			this->h_data_DEN_BkgSub = this->BackgroundSubtraction( Hist_data->h, Hist_top->h, Hist_diboson->h );
		}
		else if( Type == this->NUMStr )
		{
			this->h_DY_NUM = (TH1D*)Hist_DY->h->Clone(); // -- after applying SF -- //
			this->h_data_NUM_BkgSub = this->BackgroundSubtraction( Hist_data->h, Hist_top->h, Hist_diboson->h );
		}
	}

	TH1D* BackgroundSubtraction( TH1D* h_data, TH1D* h_top, TH1D* h_diboson )
	{
		TString HistName = h_data->GetName();
		HistName = HistName  + "_BkgSub";
		TH1D* h_BkgSub = (TH1D*)h_data->Clone(HistName);

		h_BkgSub->Sumw2();
		h_top->Sumw2();
		h_diboson->Sumw2();

		h_BkgSub->Add( h_top, -1 );
		h_BkgSub->Add( h_diboson, -1);

		return h_BkgSub;
	}

	TGraphAsymmErrors* EfficiencyGraph( TString Type )
	{
		TH1D* h_DEN;
		TH1D* h_NUM;

		if( Type == "Data" )
		{
			h_DEN = (TH1D*)this->h_data_DEN_BkgSub->Clone();
			h_NUM = (TH1D*)this->h_data_NUM_BkgSub->Clone();
		}
		else if( Type == "MC" )
		{
			h_DEN = (TH1D*)this->h_DY_DEN->Clone();
			h_NUM = (TH1D*)this->h_DY_NUM->Clone();
		}

		h_DEN = this->Rebin_Mass( h_DEN );
		h_NUM = this->Rebin_Mass( h_NUM );

		DrawCanvas_DEN_vs_NUM( Type, this->Region, h_DEN, h_NUM );

		this->RemoveUnderOverFlow( h_DEN );
		this->RemoveUnderOverFlow( h_NUM );

		// -- TEfficiency -- //
		TEfficiency *TEff = new TEfficiency(*h_NUM, *h_DEN);

		TString GraphName = TString::Format("g_%s_%s_%s_%s", Type.Data(), this->DENStr.Data(), this->NUMStr.Data(), this->Region.Data());
		TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)TEff->CreateGraph()->Clone(GraphName);

		return g_Eff;
	}

	void RemoveUnderOverFlow( TH1D* h )
	{
		h->SetBinContent(0, 1);
		h->SetBinError(0, 1);
		h->SetBinContent( h->GetNbinsX()+1, 1);
		h->SetBinError( h->GetNbinsX()+1, 1);
	}

	TH1D* Rebin_Mass( TH1D* h_before )
	{
		const Int_t _nMassBin = 7;
		Double_t _MassBinEdges[_nMassBin+1] = {60, 120, 200, 400, 600, 800, 1000, 2500};

		return (TH1D*)h_before->Rebin(_nMassBin, h_before->GetName(), _MassBinEdges);
	}

	Double_t SF_Zpeak_DirectlyExtracted( TString Type, TH1D* h_data, TH1D* h_DY, TH1D* h_top, TH1D* h_diboson )
	{
		// -- total MC (for ratio) -- //
		TH1D* h_totMC = (TH1D*)h_DY->Clone("h_totMC"); h_totMC->Sumw2();
		h_totMC->Add( h_top );
		h_totMC->Add( h_diboson );

		Double_t nEvent_Data = this->nEvent_Zpeak( h_data );
		Double_t nEvent_MC = this->nEvent_Zpeak( h_totMC );
		Double_t SF = nEvent_Data / nEvent_MC;

		printf( "[%s, %s]\n", Type.Data(), this->Region.Data() );
		printf( "[# events in Zpeak: (data, MC) = (%.0lf, %.0lf)] -> SF = %lf\n", nEvent_Data, nEvent_MC, SF );

		return SF;
	}

	Double_t nEvent_Zpeak(TH1D* h )
	{
		Double_t nEv_Zpeak = 0;
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinCenter = h->GetBinCenter(i_bin);
			if( BinCenter > 60 && BinCenter < 120 )
				nEv_Zpeak = nEv_Zpeak + h->GetBinContent(i_bin);
		}

		return nEv_Zpeak;
	}

	void DrawCanvas_DEN_vs_NUM( TString Type, TString region, TH1D* h_DEN, TH1D* h_NUM )
	{
		HistInfo *Hist_DEN = new HistInfo( kBlue, "Denomiantor" );
		Hist_DEN->Set_Histogram( h_DEN );
		Hist_DEN->Set();

		HistInfo *Hist_NUM = new HistInfo( kGreen+2, "Numerator" );
		Hist_NUM->Set_Histogram( h_NUM );
		Hist_NUM->Set();
		Hist_NUM->Calc_RatioHist_Denominator( Hist_DEN->h );

		TString CanvasName = TString::Format("c_DEN_vs_NUM_%s_%s_%s", Type.Data(), region.Data(), this->NUMStr.Data());
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 0, 1 );

		c->cd();
		TopPad->cd();

		Hist_DEN->h->Draw("EPSAME");
		Hist_NUM->h->Draw("EPSAME");

		SetHistFormat_TopPad( Hist_DEN->h, "Entries per bin" );

		TLegend *legend;
		SetLegend( legend );
		legend->AddEntry( Hist_DEN->h, Hist_DEN->LegendName );
		legend->AddEntry( Hist_NUM->h, Hist_NUM->LegendName );
		legend->Draw();

		TLatex latex;
		if( Type == "Data" )
			Latex_Preliminary( latex, 16.1, 13 );
		else if( Type == "MC" )
			Latex_Simulation( latex );
		Latex_Info( latex, Type, region );

		c->cd();
		BottomPad->cd();

		Hist_NUM->h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( Hist_NUM->h_ratio, "m [GeV]", "NUM/DEN", 0.91, 1.09);

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");

		Int_t nBin = Hist_NUM->h_ratio->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t value = Hist_NUM->h_ratio->GetBinContent(i_bin);

			if( value > 1 )
			{
				printf("======================================================================================\n");
				printf("[DEN/NUM = %lf > 1 ... correct it to force NUM to have same value with DEN]\n", value);
				printf("======================================================================================\n");

				Double_t DEN = h_DEN->GetBinContent(i_bin);
				Double_t Err_DEN = h_DEN->GetBinError(i_bin);

				h_NUM->SetBinContent(i_bin, DEN);
				h_NUM->SetBinError(i_bin, Err_DEN);
			}
		}
	}
};


void DrawCanvas_Eff_VariousNUM( TFile* f_input, TString DataType, TString Region, vector<TString> vec_NUMStr )
{
	vector< GraphInfo* > vec_GraphInfo;

	Int_t nNUM = (Int_t)vec_NUMStr.size();
	for(Int_t i_num=0; i_num<nNUM; i_num++)
	{
		TString GraphName = TString::Format("g_%s_%s_%s_%s", DataType.Data(), "DEN", vec_NUMStr[i_num].Data(), Region.Data());
		TGraphAsymmErrors* g_temp = (TGraphAsymmErrors*)f_input->Get(GraphName)->Clone();

		GraphInfo *GraphInfo_temp = new GraphInfo( i_num+1, vec_NUMStr[i_num] );
		GraphInfo_temp->Set_Graph( g_temp );

		vec_GraphInfo.push_back( GraphInfo_temp );
	}

	TCanvas *c;
	TString CanvasName = TString::Format( "c_VariousNUM_%s_%s", DataType.Data(), Region.Data() );
	SetCanvas_Square( c, CanvasName );

	c->cd();

	TLegend *legend;
	SetLegend( legend, 0.15, 0.14, 0.95, 0.35);
	legend->SetNColumns(3);
	for(Int_t i_num=0; i_num<nNUM; i_num++)
	{
		if( i_num == 0 )
			vec_GraphInfo[i_num]->DrawGraph("APSAME");
		else
			vec_GraphInfo[i_num]->DrawGraph("PSAME");

		vec_GraphInfo[i_num]->g->SetMarkerSize(1.5);

		legend->AddEntry( vec_GraphInfo[i_num]->g, vec_GraphInfo[i_num]->LegendName );
	}
	SetGraphFormat_SinglePad( vec_GraphInfo[0]->g, "m [GeV]", "Efficiency" );
	vec_GraphInfo[0]->g->GetYaxis()->SetRangeUser( 0.85, 1.05 );
	
	legend->Draw();

	TLatex latex;
	if( DataType == "Data" )
		Latex_Preliminary( latex, 16.1, 13 );
	else
		Latex_Simulation( latex );

	Latex_Info( latex, "", Region );

	c->SaveAs(".pdf");
}












