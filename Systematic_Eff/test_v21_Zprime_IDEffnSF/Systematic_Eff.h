#include <Include/PlotTools.h>
#include <fstream>

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

class Tool_Systematic_Eff
{
public:
	Int_t *i_canvas;
	TString InputFileName;
	TFile *f_output;

	TString DENStr;
	TString NUMStr;
	TString Region;

	TString Legend_Data;

	TH1D* h_data_DEN_BkgSub;
	TH1D* h_data_NUM_BkgSub;

	TH1D* h_DY_DEN;
	TH1D* h_DY_NUM;

	Double_t Lumi;
	Double_t Lumi_Total;
	Double_t Lumi_RunBtoF;
	Double_t Lumi_RunGtoH;
	Double_t Lumi_Scale;

	Bool_t isRunBtoF;
	Bool_t isRunGtoH;

	TGraphAsymmErrors* g_data;
	TGraphAsymmErrors* g_MC;

	ofstream *outFile;

	Int_t RebinSize;
	Double_t xMin;
	Double_t xMax;
	Double_t yMin;
	Double_t yMax;
	TString XTitle;
	TString YTitle;
	Bool_t isLogx;
	Bool_t isLogy;

	Tool_Systematic_Eff()
	{
		this->Lumi = 0;
		this->Lumi_Total = 35867.060;
		this->Lumi_RunBtoF = 19720.882;
		this->Lumi_RunGtoH = 16146.178;

		this->Init();
	}

	void Set_OutFile( ofstream *_outFile )
	{
		this->outFile = _outFile;
	}

	void Set_iCanvas( Int_t *_i_canvas )
	{
		this->i_canvas = _i_canvas;
	}

	void Set_InputFileName( TString _InputFileName )
	{
		this->InputFileName = _InputFileName;
	}

	void Set_Output( TFile *_f_output )
	{
		this->f_output = _f_output;
	}

	void Set_DEN_NUM( TString _DENStr, TString _NUMStr )
	{
		this->DENStr = _DENStr;
		this->NUMStr = _NUMStr;

		this->isRunBtoF = kFALSE;
		this->isRunGtoH = kFALSE;

		if( this->DENStr.Contains("RunBtoF") )
		{
			this->isRunBtoF = kTRUE;
			this->Lumi = this->Lumi_RunBtoF;
			this->Lumi_Scale = this->Lumi_RunBtoF / this->Lumi_Total;
			this->Legend_Data = "Data, RunB-F";
		}
		else if( this->DENStr.Contains("RunGtoH") )
		{
			this->isRunGtoH = kTRUE;
			this->Lumi = this->Lumi_RunGtoH;
			this->Lumi_Scale = this->Lumi_RunGtoH / this->Lumi_Total;
			this->Legend_Data = "Data, RunG-H";
		}
		else
		{
			this->Lumi = this->Lumi_Total;
			this->Lumi_Scale = 1.0;
			this->Legend_Data = "Data";
		}
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

		GraphInfo *Graph_data = new GraphInfo( kBlack, this->Legend_Data + " (Bkg.Sub.)" );
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
		Graph_MC->g->GetYaxis()->SetRangeUser( 0.5, 1.05 );

		Graph_MC->g->SetMarkerSize(1.5);
		Graph_data->g->SetMarkerSize(1.5);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.30, 0.50, 0.45);

		legend->AddEntry( Graph_data->g, Graph_data->LegendName );
		legend->AddEntry( Graph_MC->g, Graph_MC->LegendName );

		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, this->Lumi / 1000.0, 13 );
		Latex_Info( latex, "", this->Region );

		c->cd();
		BottomPad->cd();

		Graph_data->g_ratio->Draw("APSAME");
		SetGraphFormat_BottomPad( Graph_data->g_ratio, "m [GeV]", "Data/MC", 0.6, 1.1 );
		Graph_data->g_ratio->GetYaxis()->SetNdivisions(505);

		Graph_data->g_ratio->SetMarkerSize(1.5);

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");

		// Print_Graph( Graph_data->g_ratio );
	}

	void DrawCanvas_Data_vs_MC( TString HistName_Base, TString Type )
	{
		this->Init();

		this->SetHistStyle( HistName_Base );

		TString HistName_Data = TString::Format( "%s_%s", HistName_Base.Data(), Type.Data() );
		TString HistName_MC = HistName_Data;
		if( this->isRunBtoF || this->isRunGtoH )
		{
			TString TStr_Type = "";
			if( Type.Contains("DEN") ) TStr_Type = "DEN";
			if( Type.Contains("NUM") ) TStr_Type = "NUM";
			HistName_MC = TString::Format( "%s_%s", HistName_Base.Data(), TStr_Type.Data() );
		}

		TH1D* h_data = Get_Hist( this->InputFileName, "Data/"+HistName_Data );
		HistInfo *Hist_data = new HistInfo( kBlack, this->Legend_Data );
		Hist_data->Set_Histogram( h_data );
		Hist_data->Set();

		TH1D* h_DY = Get_Hist( this->InputFileName, "DYPowheg/"+HistName_MC );
		HistInfo *Hist_DY = new HistInfo( kPink, "Z/#gamma*#rightarrow#mu#mu" );
		Hist_DY->Set_Histogram( h_DY );
		Hist_DY->Set();
		Hist_DY->h->SetFillColorAlpha( Hist_DY->Color, 1 );

		// -- ttbar + tW + tbarW -- //
		TH1D* h_ttbar = Get_Hist( this->InputFileName, "ttbarTo2L2Nu/"+HistName_MC);
		TH1D* h_tW = Get_Hist( this->InputFileName, "tW/"+HistName_MC );
		TH1D* h_tbarW = Get_Hist( this->InputFileName, "tbarW/"+HistName_MC );

		TH1D* h_top = (TH1D*)h_ttbar->Clone(); h_top->Sumw2();
		h_top->Add( h_tW );
		h_top->Add( h_tbarW );

		HistInfo *Hist_top = new HistInfo( kYellow, "t#bar{t}+tW+#bar{t}W" );
		Hist_top->Set_Histogram( h_top );
		Hist_top->Set();
		Hist_top->h->SetFillColorAlpha( Hist_top->Color, 1 );

		// -- dibosons -- //
		TH1D* h_WW = Get_Hist(this->InputFileName, "WWTo2L2Nu/"+HistName_MC );
		TH1D* h_WZ = Get_Hist(this->InputFileName, "WZ/"+HistName_MC );
		TH1D* h_ZZ = Get_Hist(this->InputFileName, "ZZ/"+HistName_MC );

		TH1D* h_diboson = (TH1D*)h_WW->Clone(); h_diboson->Sumw2();
		h_diboson->Add( h_WZ );
		h_diboson->Add( h_ZZ );

		HistInfo *Hist_diboson = new HistInfo( kBlue, "Diboson" );
		Hist_diboson->Set_Histogram( h_diboson );
		Hist_diboson->Set();
		Hist_diboson->h->SetFillColorAlpha( Hist_diboson->Color, 1 );

		// -- scale dowwn if it is RunB-F or RunG-H -- //
		Hist_DY->h->Scale( this->Lumi_Scale );
		Hist_top->h->Scale( this->Lumi_Scale );
		Hist_diboson->h->Scale( this->Lumi_Scale );

		// -- rebin -- //
		Hist_data->h->Rebin( this->RebinSize );
		Hist_DY->h->Rebin( this->RebinSize );
		Hist_top->h->Rebin( this->RebinSize );
		Hist_diboson->h->Rebin( this->RebinSize );

		// TString YTitle = "Entries per bin";
		// Hist_data->h = Rebin_Mass( Hist_data->h );
		// Hist_DY->h = Rebin_Mass( Hist_DY->h );
		// Hist_top->h = Rebin_Mass( Hist_top->h );
		// Hist_diboson->h = Rebin_Mass( Hist_diboson->h );
		// Double_t xMin = 60;

		// -- apply scale factor (data/MC @ Z-peak) -- //
		// Double_t SF = SF_Zpeak( region );
		Double_t SF = this->SF_Zpeak_DirectlyExtracted( Type );
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
		TString CanvasName = TString::Format("c%02d_%s_%s", *i_canvas, HistName_Base.Data(), Type.Data() ); (*i_canvas)++;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, this->isLogx, this->isLogy );

		c->cd();
		TopPad->cd();

		TH1D* h_format = (TH1D*)Hist_data->h->Clone();
		h_format->Reset("ICES");
		h_format->Draw("");
		hs->Draw("histsame");
		Hist_data->h->Draw("EPsame");
		h_format->Draw("axissame");

		SetHistFormat_TopPad( h_format,  this->YTitle);
		h_format->GetXaxis()->SetRangeUser(this->xMin, this->xMax);
		h_format->GetYaxis()->SetRangeUser(this->yMin, this->yMax);

		TLegend *legend;
		SetLegend( legend, 0.65, 0.80, 0.95, 0.95 );
		legend->SetTextFont(62);
		legend->AddEntry( Hist_data->h, Hist_data->LegendName );
		legend->AddEntry( Hist_DY->h, Hist_DY->LegendName );
		legend->AddEntry( Hist_top->h, Hist_top->LegendName );
		legend->AddEntry( Hist_diboson->h, Hist_diboson->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, this->Lumi / 1000.0, 13 );
		Latex_Info( latex, Type, "" );

		c->cd();
		BottomPad->cd();
		TH1D* h_ratio = (TH1D*)Hist_data->h->Clone();
		h_ratio->Divide( Hist_data->h, h_totMC );
		h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( h_ratio, this->XTitle, "Data/MC", 0, 2.5);
		h_ratio->GetXaxis()->SetRangeUser(this->xMin, this->xMax);

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");

		TH1D* h_data_bkgSub = BackgroundSubtraction( Hist_data->h, Hist_top->h, Hist_diboson->h );

		this->DrawCanvas_DataBkgSub_vs_DYMC( HistName_Base, Type, h_data_bkgSub, Hist_DY->h );
	}

protected:
	void DrawCanvas_EachTypeAndRegion( TString Type, TString region )
	{
		if( !(region == "All" || region == "BB" || region == "BEEE" ) )
		{
			printf("[%s is not correct region]\n", region.Data() );
			return;
		}

		TString HistName_Base = TString::Format("h_mass_%s", region.Data());
		if( region == "All" )
			HistName_Base = "h_mass";

		TString HistName_Data = TString::Format( "%s_%s", HistName_Base.Data(), Type.Data() );
		TString HistName_MC = HistName_Data;
		if( this->isRunBtoF || this->isRunGtoH )
		{
			TString TStr_Type = "";
			if( Type.Contains("DEN") ) TStr_Type = "DEN";
			if( Type.Contains("NUM") ) TStr_Type = "NUM";
			HistName_MC = TString::Format( "%s_%s", HistName_Base.Data(), TStr_Type.Data() );
		}

		TH1D* h_data = Get_Hist( this->InputFileName, "Data/"+HistName_Data );
		HistInfo *Hist_data = new HistInfo( kBlack, this->Legend_Data );
		Hist_data->Set_Histogram( h_data );
		Hist_data->Set();

		TH1D* h_DY = Get_Hist( this->InputFileName, "DYPowheg/"+HistName_MC );
		HistInfo *Hist_DY = new HistInfo( kPink, "Z/#gamma*#rightarrow#mu#mu" );
		Hist_DY->Set_Histogram( h_DY );
		Hist_DY->Set();
		Hist_DY->h->SetFillColorAlpha( Hist_DY->Color, 1 );

		// -- ttbar + tW + tbarW -- //
		TH1D* h_ttbar = Get_Hist( this->InputFileName, "ttbarTo2L2Nu/"+HistName_MC);
		TH1D* h_tW = Get_Hist( this->InputFileName, "tW/"+HistName_MC );
		TH1D* h_tbarW = Get_Hist( this->InputFileName, "tbarW/"+HistName_MC );

		TH1D* h_top = (TH1D*)h_ttbar->Clone(); h_top->Sumw2();
		h_top->Add( h_tW );
		h_top->Add( h_tbarW );

		HistInfo *Hist_top = new HistInfo( kYellow, "t#bar{t}+tW+#bar{t}W" );
		Hist_top->Set_Histogram( h_top );
		Hist_top->Set();
		Hist_top->h->SetFillColorAlpha( Hist_top->Color, 1 );

		// -- dibosons -- //
		TH1D* h_WW = Get_Hist(this->InputFileName, "WWTo2L2Nu/"+HistName_MC );
		TH1D* h_WZ = Get_Hist(this->InputFileName, "WZ/"+HistName_MC );
		TH1D* h_ZZ = Get_Hist(this->InputFileName, "ZZ/"+HistName_MC );

		TH1D* h_diboson = (TH1D*)h_WW->Clone(); h_diboson->Sumw2();
		h_diboson->Add( h_WZ );
		h_diboson->Add( h_ZZ );

		HistInfo *Hist_diboson = new HistInfo( kBlue, "Diboson" );
		Hist_diboson->Set_Histogram( h_diboson );
		Hist_diboson->Set();
		Hist_diboson->h->SetFillColorAlpha( Hist_diboson->Color, 1 );

		// -- W+jets -- //
		TH1D* h_WJets = Get_Hist( this->InputFileName, "WJets/"+HistName_MC );
		HistInfo *Hist_WJets = new HistInfo( kGreen+2, "W+jets (H_{T} >100 GeV)");
		Hist_WJets->Set_Histogram( h_WJets );
		Hist_WJets->Set();
		Hist_WJets->h->SetFillColorAlpha( Hist_WJets->Color, 1 );

		// -- scale dowwn if it is RunB-F or RunG-H -- //
		Hist_DY->h->Scale( this->Lumi_Scale );
		Hist_top->h->Scale( this->Lumi_Scale );
		Hist_diboson->h->Scale( this->Lumi_Scale );
		Hist_WJets->h->Scale( this->Lumi_Scale );

		// -- rebin -- //
		Int_t RebinSize = 50;
		TString YTitle = TString::Format("Events / %.0d GeV", RebinSize);
		Hist_data->h->Rebin( RebinSize );
		Hist_DY->h->Rebin( RebinSize );
		Hist_top->h->Rebin( RebinSize );
		Hist_diboson->h->Rebin( RebinSize );
		Hist_WJets->h->Rebin( RebinSize );
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
		Hist_WJets->h->Scale( SF );

		// -- MC stack -- //
		THStack *hs = new THStack("hs", "");
		hs->Add( Hist_WJets->h );
		hs->Add( Hist_diboson->h );
		hs->Add( Hist_top->h );
		hs->Add( Hist_DY->h );

		// -- total MC (for ratio) -- //
		TH1D* h_totMC = (TH1D*)Hist_DY->h->Clone("h_totMC"); h_totMC->Sumw2();
		h_totMC->Add( Hist_top->h );
		h_totMC->Add( Hist_diboson->h );
		h_totMC->Add( Hist_WJets->h );

		// -- draw canvas -- //
		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = TString::Format("c%02d_Mass_%s_%s", *i_canvas, Type.Data(), region.Data() ); (*i_canvas)++;
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
		SetLegend( legend, 0.62, 0.77, 0.95, 0.95 );
		legend->SetTextFont(62);
		legend->AddEntry( Hist_data->h, Hist_data->LegendName );
		legend->AddEntry( Hist_DY->h, Hist_DY->LegendName );
		legend->AddEntry( Hist_top->h, Hist_top->LegendName );
		legend->AddEntry( Hist_diboson->h, Hist_diboson->LegendName );
		legend->AddEntry( Hist_WJets->h, Hist_WJets->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, this->Lumi / 1000.0, 13 );
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
			this->h_data_DEN_BkgSub = this->BackgroundSubtraction( Hist_data->h, Hist_top->h, Hist_diboson->h, Hist_WJets->h );
		}
		else if( Type == this->NUMStr )
		{
			this->h_DY_NUM = (TH1D*)Hist_DY->h->Clone(); // -- after applying SF -- //
			this->h_data_NUM_BkgSub = this->BackgroundSubtraction( Hist_data->h, Hist_top->h, Hist_diboson->h, Hist_WJets->h );
		}
	}



	void DrawCanvas_DataBkgSub_vs_DYMC( TString HistName_Base, TString Type, TH1D* h_data_BkgSub, TH1D* h_DY )
	{
		HistInfo *Hist_DY = new HistInfo( kRed, "MC (DY)" );
		Hist_DY->Set_Histogram( h_DY );
		Hist_DY->Set();

		HistInfo *Hist_data = new HistInfo( kBlack, "Data (Bkg. Sub)" );
		Hist_data->Set_Histogram( h_data_BkgSub );
		Hist_data->Set();
		Hist_data->Calc_RatioHist_Denominator( Hist_DY->h );

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = TString::Format( "c_DataBkgSub_vs_DYMC_%s_%s", HistName_Base.Data(), Type.Data() );
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, this->isLogx, this->isLogy );

		c->cd();
		TopPad->cd();

		Hist_DY->h->Draw("EPSAME");
		Hist_data->h->Draw("EPSAME");

		SetHistFormat_TopPad( Hist_DY->h, this->YTitle );
		Hist_DY->h->GetXaxis()->SetRangeUser( this->xMin, this->xMax );
		Hist_DY->h->GetYaxis()->SetRangeUser( this->yMin, this->yMax );

		TLegend *legend;
		SetLegend( legend );
		legend->AddEntry( Hist_data->h, Hist_data->LegendName );
		legend->AddEntry( Hist_DY->h, Hist_DY->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, this->Lumi / 1000.0, 13 );
		Latex_Info( latex, Type, "" );

		c->cd();
		BottomPad->cd();
		Hist_data->h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( Hist_data->h_ratio, this->XTitle, "Data/MC", 0, 2.5);
		Hist_data->h_ratio->GetXaxis()->SetRangeUser(this->xMin, this->xMax);

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");

		Int_t nBin = Hist_data->h->GetNbinsX();
		*this->outFile << TString::Format("[HistName, Type] = [%s, %s]", HistName_Base.Data(), Type.Data() ) << endl;
		*this->outFile << TString::Format("[overFlow]: (Data, MC) = (%.1lf, %.1lf)", Hist_data->h->GetBinContent(nBin+1), Hist_DY->h->GetBinContent(nBin+1)) << endl;

		*this->outFile << "Data (Bkg.Sub)" << endl; 
		Print_FractionOfEvents( Hist_data->h, 2 );
		Print_FractionOfEvents( Hist_data->h, 3 );
		Print_FractionOfEvents( Hist_data->h, 4 );
		Print_FractionOfEvents( Hist_data->h, 5 );

		*this->outFile << "MC (DY)" << endl;
		Print_FractionOfEvents( Hist_DY->h, 2 );
		Print_FractionOfEvents( Hist_DY->h, 3 );
		Print_FractionOfEvents( Hist_DY->h, 4 );
		Print_FractionOfEvents( Hist_DY->h, 5 );
	}

	void Print_FractionOfEvents( TH1D* h, Double_t cut )
	{
		Int_t nBin = h->GetNbinsX();
		Double_t TotEntries = h->GetBinContent(0) + h->GetBinContent(nBin+1); // -- take into account under & overflow -- //
		Double_t nPassEntries = 0.0;
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t nEvent = h->GetBinContent(i_bin);
			TotEntries = TotEntries + nEvent;

			Double_t BinCenter = h->GetBinCenter(i_bin);
			if( BinCenter > cut )
				nPassEntries = nPassEntries + h->GetBinContent(i_bin);
		}
		nPassEntries = nPassEntries + h->GetBinContent(nBin+1); // -- take into account overflow -- //

		Double_t Frac = nPassEntries / TotEntries;

		*this->outFile << TString::Format("Fraction = %1.lf / %1.lf = %.2lf", nPassEntries, TotEntries, Frac*100) << endl;
	}

	// -- no W+jets contribution -- //
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

		// -- remove negative bins -- //
		Int_t nBin = h_BkgSub->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			
			Double_t value = h_BkgSub->GetBinContent(i_bin);
			if( value < 0 )
			{
				h_BkgSub->SetBinContent(i_bin, 0);
				h_BkgSub->SetBinError(i_bin, 0);
			}
		}

		return h_BkgSub;
	}

	TH1D* BackgroundSubtraction( TH1D* h_data, TH1D* h_top, TH1D* h_diboson, TH1D* h_WJets )
	{
		TString HistName = h_data->GetName();
		HistName = HistName  + "_BkgSub";
		TH1D* h_BkgSub = (TH1D*)h_data->Clone(HistName);

		h_BkgSub->Sumw2();
		h_top->Sumw2();
		h_diboson->Sumw2();
		h_WJets->Sumw2();

		h_BkgSub->Add( h_top, -1 );
		h_BkgSub->Add( h_diboson, -1);
		h_BkgSub->Add( h_WJets, -1 );

		// -- remove negative bins -- //
		Int_t nBin = h_BkgSub->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			
			Double_t value = h_BkgSub->GetBinContent(i_bin);
			if( value < 0 )
			{
				h_BkgSub->SetBinContent(i_bin, 0);
				h_BkgSub->SetBinError(i_bin, 0);
			}
		}

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

		// DrawCanvas_DEN_vs_NUM( Type, this->Region, h_DEN, h_NUM );

		this->RemoveUnderOverFlow( h_DEN );
		this->RemoveUnderOverFlow( h_NUM );

		this->CheckConsistency( h_DEN, h_NUM );

		// -- TEfficiency -- //
		TEfficiency *TEff = new TEfficiency(*h_NUM, *h_DEN);

		TString GraphName = TString::Format("g_%s_%s_%s_%s", Type.Data(), this->DENStr.Data(), this->NUMStr.Data(), this->Region.Data());
		TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)TEff->CreateGraph()->Clone(GraphName);

		return g_Eff;
	}

	void CheckConsistency( TH1D* h_DEN, TH1D* h_NUM )
	{
		Int_t nBin = h_DEN->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t DEN = h_DEN->GetBinContent(i_bin);
			Double_t NUM = h_NUM->GetBinContent(i_bin);

			if( DEN < NUM )
			{
				Double_t LowerEdge = h_DEN->GetBinLowEdge(i_bin);
				Double_t UpperEdge = h_DEN->GetBinLowEdge(i_bin+1);

				printf("[%02d bin: %.0lf < M < %.0lf]: DEN < NUM! ... (DEN, NUM) = (%.1lf, %.1lf)\n", i_bin, LowerEdge, UpperEdge, DEN, NUM);
				printf("\t NUM is set to have same value with DEN\n");

				h_NUM->SetBinContent(i_bin, DEN);
				h_NUM->SetBinError(i_bin, h_DEN->GetBinError(i_bin) );
			}
		}
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

		*this->outFile << TString::Format("[%s, %s]", Type.Data(), this->Region.Data()) << endl;
		*this->outFile << TString::Format("[# events in Zpeak: (data, MC) = (%.0lf, %.0lf)] -> SF = %lf\n", nEvent_Data, nEvent_MC, SF) << endl;

		return SF;
	}

	Double_t SF_Zpeak_DirectlyExtracted( TString Type )
	{
		TString HistName_Base = "h_mass";

		TString HistName_Data = TString::Format( "%s_%s", HistName_Base.Data(), Type.Data() );
		TString HistName_MC = HistName_Data;
		if( this->isRunBtoF || this->isRunGtoH )
		{
			TString TStr_Type = "";
			if( Type.Contains("DEN") ) TStr_Type = "DEN";
			if( Type.Contains("NUM") ) TStr_Type = "NUM";
			HistName_MC = TString::Format( "%s_%s", HistName_Base.Data(), TStr_Type.Data() );
		}

		TH1D* h_data = Get_Hist( this->InputFileName, "Data/"+HistName_Data );
		TH1D* h_DY = Get_Hist( this->InputFileName, "DYPowheg/"+HistName_MC );

		// -- ttbar + tW + tbarW -- //
		TH1D* h_ttbar = Get_Hist( this->InputFileName, "ttbarTo2L2Nu/"+HistName_MC);
		TH1D* h_tW = Get_Hist( this->InputFileName, "tW/"+HistName_MC );
		TH1D* h_tbarW = Get_Hist( this->InputFileName, "tbarW/"+HistName_MC );
		TH1D* h_top = (TH1D*)h_ttbar->Clone(); h_top->Sumw2();

		// -- dibosons -- //
		TH1D* h_WW = Get_Hist(this->InputFileName, "WWTo2L2Nu/"+HistName_MC );
		TH1D* h_WZ = Get_Hist(this->InputFileName, "WZ/"+HistName_MC );
		TH1D* h_ZZ = Get_Hist(this->InputFileName, "ZZ/"+HistName_MC );

		TH1D* h_diboson = (TH1D*)h_WW->Clone(); h_diboson->Sumw2();
		h_diboson->Add( h_WZ );
		h_diboson->Add( h_ZZ );

		// -- total MC (for ratio) -- //
		TH1D* h_totMC = (TH1D*)h_DY->Clone("h_totMC"); h_totMC->Sumw2();
		h_totMC->Add( h_top );
		h_totMC->Add( h_diboson );
		
		// -- scale dowwn if it is RunB-F or RunG-H -- //
		h_totMC->Scale( this->Lumi_Scale );

		Double_t nEvent_Data = this->nEvent_Zpeak( h_data );
		Double_t nEvent_MC = this->nEvent_Zpeak( h_totMC );
		Double_t SF = nEvent_Data / nEvent_MC;

		*this->outFile << TString::Format("[%s, %s]", Type.Data(), this->Region.Data()) << endl;
		*this->outFile << TString::Format("[# events in Zpeak: (data, MC) = (%.0lf, %.0lf)] -> SF = %lf\n", nEvent_Data, nEvent_MC, SF) << endl;

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
			Latex_Preliminary( latex, 35.9, 13 );
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

	void SetHistStyle( TString HistName_Base )
	{
		if( HistName_Base == "h_RatioPt" )
		{
			this->RebinSize = 5;
			this->xMin = 1;
			this->xMax = 100;
			this->yMin = 0.5;
			this->yMax = 1e6;
			this->XTitle = "P_{T}^{lead} / P_{T}^{sub}";
			this->YTitle = TString::Format("Entries / %.2lf", (Double_t)this->RebinSize * 0.01);
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
		}

		if( HistName_Base == "h_RatioPt_M400to2500" )
		{
			this->RebinSize = 5;
			this->xMin = 1;
			this->xMax = 100;
			this->yMin = 0.5;
			this->yMax = 1e4;
			this->XTitle = "P_{T}^{lead} / P_{T}^{sub} (400 < M < 2500 GeV)";
			this->YTitle = TString::Format("Entries / %.2lf", (Double_t)this->RebinSize * 0.01);
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
		}

		if( HistName_Base == "h_dPtOverPt" )
		{
			this->RebinSize = 5;
			this->xMin = 1;
			this->xMax = 100;
			this->yMin = 0.5;
			this->yMax = 1e4;
			this->XTitle = "#DeltaP_{T} / P_{T}^{sub}";
			this->YTitle = TString::Format("Entries / %.2lf", (Double_t)this->RebinSize * 0.01);
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
		}

		if( HistName_Base == "h_dPtOverPt_M400to2500" )
		{
			this->RebinSize = 5;
			this->xMin = 0.05;
			this->xMax = 100;
			this->yMin = 0.5;
			this->yMax = 1e4;
			this->XTitle = "#DeltaP_{T} / P_{T}^{sub} (400 < M < 2500 GeV)";
			this->YTitle = TString::Format("Entries / %.2lf", (Double_t)this->RebinSize * 0.01);
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
		}

		if( HistName_Base.Contains("h_mass_PtRatio") )
		{
			this->RebinSize = 50;
			this->xMin = 50;
			this->xMax = 2500;
			this->yMin = 0.5;
			this->yMax = 1e4;
			if( HistName_Base == "h_mass_PtRatio3toInf" )
				this->XTitle = "m [GeV] (P_{T}^{lead} / P_{T}^{sub} > 3)";
			if( HistName_Base == "h_mass_PtRatio4toInf" )
				this->XTitle = "m [GeV] (P_{T}^{lead} / P_{T}^{sub} > 4)";
			if( HistName_Base == "h_mass_PtRatio5toInf" )
				this->XTitle = "m [GeV] (P_{T}^{lead} / P_{T}^{sub} > 5)";		
			this->YTitle = TString::Format("Entries / %.0lf GeV", (Double_t)this->RebinSize);
			this->isLogx = kFALSE;
			this->isLogy = kTRUE;
		}
	}

	void Init()
	{
		this->RebinSize = -1;
		this->xMin = 0;
		this->xMax = 0;
		this->yMin = 0;
		this->yMax = 0;
		this->XTitle = "";
		this->YTitle = "";
		this->isLogx = kFALSE;
		this->isLogy = kFALSE;
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
		Latex_Preliminary( latex, 35.9, 13 );
	else
		Latex_Simulation( latex );

	Latex_Info( latex, "", Region );

	c->SaveAs(".pdf");
}












