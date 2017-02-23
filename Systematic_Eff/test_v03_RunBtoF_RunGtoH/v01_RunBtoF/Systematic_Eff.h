#include <DYAnalysis_76X/Include/PlotTools.h>

TH1D* Rebin_Mass( TH1D* h_before )
{
	const Int_t _nMassBin = 7;
	Double_t _MassBinEdges[_nMassBin+1] = {60, 120, 200, 400, 600, 800, 1000, 2500};

	return (TH1D*)h_before->Rebin(_nMassBin, h_before->GetName(), _MassBinEdges);
}

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

Double_t SF_Zpeak_DirectlyExtracted( TString Type, TString region )
{
	TString HistName = TString::Format( "h_mass_%s_%s", region.Data(), Type.Data() );
	if( region == "All" )
		HistName = TString::Format( "h_mass_%s", Type.Data() );

	TH1D* h_data = Get_Hist( "ROOTFile_DataRunBtoF.root", HistName );	

	// -- DY -- //
	TH1D* h_DY = Get_Hist( "ROOTFile_DYPowheg.root", HistName );

	// -- ttbar + tW + tbarW -- //
	TH1D* h_ttbar = Get_Hist( "ROOTFile_ttbarTo2L2Nu.root", HistName);
	TH1D* h_tW = Get_Hist( "ROOTFile_tW.root", HistName );
	TH1D* h_tbarW = Get_Hist( "ROOTFile_tbarW.root", HistName );

	TH1D* h_top = (TH1D*)h_ttbar->Clone(); h_top->Sumw2();
	h_top->Add( h_tW );
	h_top->Add( h_tbarW );

	// -- dibosons -- //
	TH1D* h_WW = Get_Hist("ROOTFile_WWTo2L2Nu.root", HistName );
	TH1D* h_WZ = Get_Hist("ROOTFile_WZ.root", HistName );
	TH1D* h_ZZ = Get_Hist("ROOTFile_ZZ.root", HistName );

	TH1D* h_diboson = (TH1D*)h_WW->Clone(); h_diboson->Sumw2();
	h_diboson->Add( h_WZ );
	h_diboson->Add( h_ZZ );

	// -- total MC (for ratio) -- //
	TH1D* h_totMC = (TH1D*)h_DY->Clone("h_totMC"); h_totMC->Sumw2();
	h_totMC->Add( h_top );
	h_totMC->Add( h_diboson );

	Double_t nEvent_Data = nEvent_Zpeak( h_data );
	Double_t nEvent_MC = nEvent_Zpeak( h_totMC );
	Double_t SF = nEvent_Data / nEvent_MC;

	printf( "[%s, %s]\n", Type.Data(), region.Data() );
	printf( "[# events in Zpeak: (data, MC) = (%.0lf, %.0lf)] -> SF = %lf\n", nEvent_Data, nEvent_MC, SF );

	return SF;
}


TH1D* BackgroundSubtraction( TH1D* h_data, TString DENNUM, TString region )
{
	// -- dibosons -- //
	TString HistName = h_data->GetName();

	TH1D* h_WW = Get_Hist("ROOTFile_WWTo2L2Nu.root", HistName );
	TH1D* h_WZ = Get_Hist("ROOTFile_WZ.root", HistName );
	TH1D* h_ZZ = Get_Hist("ROOTFile_ZZ.root", HistName );

	TH1D* h_diboson = (TH1D*)h_WW->Clone(); h_diboson->Sumw2();
	h_diboson->Add( h_WZ );
	h_diboson->Add( h_ZZ );


	// -- ttbar + tW + tbarW -- //
	TH1D* h_ttbar = Get_Hist( "ROOTFile_ttbarTo2L2Nu.root", HistName);
	TH1D* h_tW = Get_Hist( "ROOTFile_tW.root", HistName );
	TH1D* h_tbarW = Get_Hist( "ROOTFile_tbarW.root", HistName );

	TH1D* h_top = (TH1D*)h_ttbar->Clone(); h_top->Sumw2();
	h_top->Add( h_tW );
	h_top->Add( h_tbarW );

	// -- rebin -- //
	h_diboson = Rebin_Mass( h_diboson );
	h_top = Rebin_Mass( h_top );

	// -- apply SF -- //
	// Double_t SF = SF_Zpeak( region );
	Double_t SF = SF_Zpeak_DirectlyExtracted( DENNUM, region );
	printf("SF = %lf is applied\n", SF);
	h_diboson->Scale( SF );
	h_top->Scale( SF );

	// -- background subtraction -- //
	TH1D* h_data_BkgSub = (TH1D*)h_data->Clone(); h_data_BkgSub->Sumw2();

	h_data_BkgSub->Add( h_diboson, -1 );
	h_data_BkgSub->Add( h_top, -1 );

	return h_data_BkgSub;
}

void Latex_Info( TLatex &latex, TString Type, TString region )
{
	TString TStr_Type = "";
	if( Type == "DEN" ) TStr_Type = "Denominator";
	if( Type == "NUM" ) TStr_Type = "Numerator";

	TString TStr_Region = "";
	if( region == "BB" ) TStr_Region = "BB";
	if( region == "BEEE" ) TStr_Region = "BE+EE";
	if( region == "All" ) TStr_Region = "All";

	latex.DrawLatexNDC(0.15, 0.91, "#font[42]{#scale[0.8]{"+TStr_Region+"}}");
	latex.DrawLatexNDC(0.15, 0.88, "#font[42]{#scale[0.6]{"+TStr_Type+"}}");
}

class PlotTools_Mass
{
public:
	Int_t i_canvas;
	TFile *f_output;

	PlotTools_Mass()
	{
		i_canvas = 0;
	}

	void Set_Output( TFile *_f_output )
	{
		this->f_output = _f_output;
	}

	void DrawCanvas_EachTypeAndRegion( TString Type, TString region )
	{
		if( !(Type == "DEN" || Type == "NUM") )
		{
			printf("[%s is not correct Type]\n", Type.Data() );
			return;
		}

		if( !(region == "All" || region == "BB" || region == "BEEE" ) )
		{
			printf("[%s is not correct region]\n", region.Data() );
			return;
		}

		TString HistName = TString::Format("h_mass_%s_%s", region.Data(), Type.Data());
		if( region == "All" )
			HistName = TString::Format("h_mass_%s", Type.Data());

		TH1D* h_data = Get_Hist( "ROOTFile_DataRunBtoF.root", HistName );
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
		Double_t SF = SF_Zpeak_DirectlyExtracted( Type, region );
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
		Latex_Preliminary( latex, 19.7, 13 );
		Latex_Info( latex, Type, region );

		c->cd();
		BottomPad->cd();
		TH1D* h_ratio = (TH1D*)Hist_data->h->Clone();
		h_ratio->Divide( Hist_data->h, h_totMC );
		h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( h_ratio, "m [GeV]", "Data/MC", 0, 2.5);
		h_ratio->GetXaxis()->SetRangeUser(xMin, 2500);

		Print_Histogram( h_ratio );

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}

protected:
};

void DrawCanvas_DEN_vs_NUM( TString Type, TString region, TH1D* h_DEN, TH1D* h_NUM )
{
	HistInfo *Hist_DEN = new HistInfo( kBlue, "Denomiantor" );
	Hist_DEN->Set_Histogram( h_DEN );
	Hist_DEN->Set();

	HistInfo *Hist_NUM = new HistInfo( kGreen+2, "Numerator" );
	Hist_NUM->Set_Histogram( h_NUM );
	Hist_NUM->Set();
	Hist_NUM->Calc_RatioHist_Denominator( Hist_DEN->h );

	TString CanvasName = TString::Format("c_DEN_vs_NUM_%s_%s", Type.Data(), region.Data());
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
		Latex_Preliminary( latex, 19.7, 13 );
	else if( Type == "MC" )
		Latex_Simulation( latex );
	Latex_Info( latex, Type, region );

	c->cd();
	BottomPad->cd();

	Hist_NUM->h_ratio->Draw("EPSAME");
	SetHistFormat_BottomPad( Hist_NUM->h_ratio, "m [GeV]", "NUM/DEN", 0.95, 1.05);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}

void RemoveUnderOverFlow( TH1D* h )
{
	h->SetBinContent(0, 1);
	h->SetBinError(0, 1);
	h->SetBinContent( h->GetNbinsX()+1, 1);
	h->SetBinError( h->GetNbinsX()+1, 1);
}


TGraphAsymmErrors* EfficiencyGraph( TString Type, TString region )
{
	TString HistName = "h_mass_"+region;
	if( region == "All" )
		HistName = "h_mass";

	TString HistName_DEN = HistName + "_DEN";
	TString HistName_NUM = HistName + "_NUM";
	printf("[Numerator, Denominator] = [%s, %s]\n", HistName_NUM.Data(), HistName_DEN.Data() );

	TString FileName = "";
	if( Type == "Data" )
		FileName = "ROOTFile_DataRunBtoF.root";
	else if( Type == "MC" )
		FileName = "ROOTFile_DYPowheg.root";

	TFile *f_input = TFile::Open(FileName); f_input->cd();
	TH1D* h_DEN = Get_Hist( FileName, HistName_DEN );
	TH1D* h_NUM = Get_Hist( FileName, HistName_NUM );

	h_DEN = Rebin_Mass( h_DEN );
	h_NUM = Rebin_Mass( h_NUM );

	if( Type == "Data" )
	{
		h_DEN = BackgroundSubtraction( h_DEN, "DEN", region );
		h_NUM = BackgroundSubtraction( h_NUM, "NUM", region );
	}
	if( Type == "MC" )
	{
		h_DEN->Scale( SF_Zpeak_DirectlyExtracted( "DEN", region ) );
		h_NUM->Scale( SF_Zpeak_DirectlyExtracted( "NUM", region ) );
	}

	DrawCanvas_DEN_vs_NUM( Type, region, h_DEN, h_NUM );

	RemoveUnderOverFlow( h_DEN );
	RemoveUnderOverFlow( h_NUM );

	// -- TEfficiency -- //
	TEfficiency *TEff = new TEfficiency(*h_NUM, *h_DEN);

	TString GraphName = TString::Format("g_%s_%s", Type.Data(), region.Data());
	TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)TEff->CreateGraph()->Clone(GraphName);

	return g_Eff;
}

void DrawCanvas_Eff_Data_vs_MC( TString region, TGraphAsymmErrors* g_data, TGraphAsymmErrors *g_MC )
{
	GraphInfo *Graph_MC = new GraphInfo( kRed, "MC (DY)" );
	Graph_MC->Set_Graph( g_MC );

	GraphInfo *Graph_data = new GraphInfo( kBlack, "Data, RunB-F (Bkg.Sub.)" );
	Graph_data->Set_Graph( g_data );
	Graph_data->Calc_RatioGraph_Denominator( Graph_MC->g );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	TString CanvasName = TString::Format("c_Eff_Data_vs_MC_%s", region.Data());
	SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad );

	c->cd();
	TopPad->cd();

	Graph_MC->DrawGraph( "APSAME" );
	Graph_data->DrawGraph( "PSAME" );

	SetGraphFormat_TopPad( Graph_MC->g, "Efficiency" );
	Graph_MC->g->GetYaxis()->SetRangeUser( 0.5, 1.05 );

	Graph_MC->g->SetMarkerSize(1.5);
	Graph_data->g->SetMarkerSize(1.5);

	TLegend *legend;
	SetLegend( legend, 0.15, 0.30, 0.45, 0.45);

	legend->AddEntry( Graph_data->g, Graph_data->LegendName );
	legend->AddEntry( Graph_MC->g, Graph_MC->LegendName );

	legend->Draw();

	TLatex latex;
	Latex_Preliminary( latex, 19.7, 13 );
	Latex_Info( latex, "", region );

	c->cd();
	BottomPad->cd();

	Graph_data->g_ratio->Draw("APSAME");
	SetGraphFormat_BottomPad( Graph_data->g_ratio, "m [GeV]", "Data/MC", 0.8, 1.05 );

	Graph_data->g_ratio->SetMarkerSize(1.5);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");

	Print_Graph( Graph_data->g_ratio );
}









