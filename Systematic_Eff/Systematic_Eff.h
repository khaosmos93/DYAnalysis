#include <DYAnalysis_76X/Include/PlotTools.h>


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

		TH1D* h_data = Get_Hist( "ROOTFile_Data.root", HistName );
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
		Hist_data->h->Rebin( RebinSize );
		Hist_DY->h->Rebin( RebinSize );
		Hist_top->h->Rebin( RebinSize );
		Hist_diboson->h->Rebin( RebinSize );

		// -- apply scale factor (data/MC @ Z-peak) -- //
		Double_t SF = SF_Zpeak( region );
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

		SetHistFormat_TopPad( h_format, TString::Format("Events / %.0d GeV", RebinSize) );
		h_format->GetXaxis()->SetRangeUser(50, 2500);
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
		Latex_Preliminary( latex, 35.9, 13 );
		this->Latex_Info( latex, Type, region );

		c->cd();
		BottomPad->cd();
		TH1D* h_ratio = (TH1D*)Hist_data->h->Clone();
		h_ratio->Divide( Hist_data->h, h_totMC );
		h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( h_ratio, "m [GeV]", "Data/MC", 0, 2.5);
		h_ratio->GetXaxis()->SetRangeUser(50, 2500);

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}

protected:
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
};


// class CalcTool_Eff
// {
// public:
// 	TFile *f_output;
	
// 	CalcTool_Eff( TFile* _f_output )
// 	{
// 		(TH1D*)f_output = _f_output;
// 	}

// 	void DrawCanvas_Eff_Data_vs_MC( TString region );
// 	{

// 	}

// 	void EfficiencyGraph( TString Type, TString region )
// 	{
// 		if( Type == "Data" )
// 		{

// 		}

// 		TString HistName = "h_mass_"+
// 		TString HistName_DEN = HistName+"_Nminus1_"+Type+"_"+CutName;
// 		TString HistName_NUM = HistName+"_N_"+Type+"_"+CutName;
// 		printf("[NUMerator, DENinator] = [%s, %s]\n", HistName_NUM.Data(), HistName_DEN.Data() );

// 		TH1D* h_DEN = (TH1D*)this->f_input->Get( HistName_DEN )->Clone();
// 		TH1D* h_NUM = (TH1D*)this->f_input->Get( HistName_NUM )->Clone();

// 		const Int_t nMassBin = 8;
// 		Double_t MassBinEdges[nMassBin+1] = {50, 120, 200, 400, 600, 800, 1000, 1500, 3000};

// 		h_DEN = (TH1D*)h_DEN->Rebin(nMassBin, h_DEN->GetName(), MassBinEdges);
// 		h_NUM = (TH1D*)h_NUM->Rebin(nMassBin, h_NUM->GetName(), MassBinEdges);

// 		// -- TEfficiency -- //
// 		TEfficiency *TEff = new TEfficiency(*h_NUM, *h_DEN);

// 		TString TEffName = MakeGraphName( HistName, CutName );
// 		TEffName.ReplaceAll("g_", "TEff_");
// 		TEff->SetName( TEffName );

// 		TString GraphName = MakeGraphName( HistName, CutName );
// 		TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)TEff->CreateGraph()->Clone( GraphName );

// 	}


// };