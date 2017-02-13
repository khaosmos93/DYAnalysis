void DrawPlot_xShifted()
{
	TFile *f_fewz = TFile::Open("../ROOTFile_DY_FEWZ_NNLO.root");
	f_fewz->cd();
	TH1D* h_fewz = (TH1D*)f_fewz->Get("h_DiffXsec")->Clone();

	TFile *f_data = TFile::Open("ROOTFile_Histograms_XDataPoint.root");
	f_data->cd();
	// TH1D* h_data = (TH1D*)f_data->Get("h_DiffXsec_FSRCorr21")->Clone();
	TGraphAsymmErrors *g_data = (TGraphAsymmErrors*)f_data->Get("g_DiffXSec_xShifted")->Clone();

	TCanvas *c = new TCanvas("Comp_data_xShifted_vs_NNLO", "", 800, 800);
	c->cd();

	TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
	TopPad->Draw();
	TopPad->cd();

	TopPad->SetTopMargin(0.04);
	TopPad->SetBottomMargin(0.305);
	TopPad->SetRightMargin(0.03);
	TopPad->SetLeftMargin(0.13);
	TopPad->SetLogx();
	TopPad->SetLogy();

	h_fewz->Draw("HISTL");
	g_data->Draw("EPSAME");
	// h_fewz->Draw("LSAME");
	// h_fewz->Draw("CSAME");

	h_fewz->SetXTitle("m (#mu#mu) [GeV]");
	h_fewz->SetYTitle("d#sigma/dm [pb/GeV]");

	h_fewz->GetXaxis()->SetNoExponent();
	h_fewz->GetXaxis()->SetMoreLogLabels();
	h_fewz->GetXaxis()->SetRangeUser(15, 3000);
	// h_fewz->GetYaxis()->SetTitleOffset(1.3);
	h_fewz->GetYaxis()->SetTitleSize(0.05);
	h_fewz->GetXaxis()->SetLabelSize(0);

	h_fewz->SetLineColor(kBlue);
	h_fewz->SetLineWidth(0.5);
	h_fewz->SetMarkerColor(kBlue);
	h_fewz->SetMarkerStyle(20);
	h_fewz->SetFillColorAlpha(kWhite, 0);

	g_data->SetLineColor(kBlack);
	g_data->SetMarkerColor(kBlack);
	g_data->SetMarkerStyle(20);
	g_data->SetFillColorAlpha(kWhite, 0);

	TLegend *legend = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	legend->AddEntry( g_data, "Data" );
	legend->AddEntry( h_fewz, "FEWZ (NNLO, NNPDF3.0)");
	legend->Draw();

	TLatex latex; DrawLatexNDC( latex );

	// -- for the ratio plot -- //
	TFile *f_data2 = TFile::Open("../ROOTFile_DataXsec_AllUnc.root");
	f_data2->cd();
	TH1D* h_data = (TH1D*)f_data2->Get("h_DiffXsec_FSRCorr21")->Clone(); h_data->Sumw2();

	TFile *f_Dimitri = TFile::Open("ROOTFile_Results_DYAnalysis_76X.root");
	f_Dimitri->cd();
	TH1D* h_fewz_Dimitri = (TH1D*)f_Dimitri->Get("h_DiffXsec_FEWZ_NNLO")->Clone(); h_fewz_Dimitri->Sumw2();

	TH1D* h_ratio = (TH1D*)h_data->Clone("h_ratio");
	h_ratio->Divide( h_data, h_fewz_Dimitri );

	TGraphAsymmErrors* g_ratio = Convert_XShiftGraph( g_data, h_ratio );

	BottomPad = new TPad("BottomPad","BottomPad",0.01,0.01,0.99,0.3);
	BottomPad->Draw();
	BottomPad->cd();

	BottomPad->SetTopMargin(0.01);
	BottomPad->SetBottomMargin(0.4);
	BottomPad->SetRightMargin(0.02);
	BottomPad->SetLeftMargin(0.12);
	BottomPad->SetLogx();
	// BottomPad->SetGridx();
	// BottomPad->SetGridy();

	h_ratio->Draw("AXIS");
	g_ratio->Draw("EPSAME");

	h_ratio->SetTitle("");
	h_ratio->SetLineColor(kBlack);
	h_ratio->SetMarkerStyle(20);
	h_ratio->SetMarkerSize(1);
	h_ratio->SetMarkerColor(kBlack);
	// h_ratio->SetStats(kFALSE);

	h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
	h_ratio->GetXaxis()->SetTitleSize( 0.2 );
	h_ratio->GetXaxis()->SetLabelColor(1);
	h_ratio->GetXaxis()->SetLabelFont(42);
	h_ratio->GetXaxis()->SetLabelOffset(0.007);
	h_ratio->GetXaxis()->SetLabelSize(0.14);
	h_ratio->GetXaxis()->SetMoreLogLabels(); 
	h_ratio->GetXaxis()->SetNoExponent();
	h_ratio->GetXaxis()->SetRangeUser( 15, 3000 );

	h_ratio->GetYaxis()->SetTitle( "Data/MC" );
	h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
	h_ratio->GetYaxis()->SetTitleSize( 0.15 );
	h_ratio->GetYaxis()->SetLabelSize( 0.07 );
	h_ratio->GetYaxis()->SetRangeUser( 0.31, 1.69 );

	// -- flat line = 1.00 -- //
	TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	f_line->SetLineColor(kRed);
	f_line->SetLineWidth(1);
	f_line->Draw("SAME");

	g_ratio->Draw("EPSAME");

	c->SaveAs(".pdf");

	TString CanvasName = c->GetName();
	Double_t LowerEdge, UpperEdge;
	TString TStr_MassRange;

	///////////////////////
	// -- 15 < M < 60 -- //
	///////////////////////
	LowerEdge = 15;
	UpperEdge = 60;

	TStr_MassRange = TString::Format("M%.0lfto%.0lf", LowerEdge, UpperEdge );
	h_fewz->GetXaxis()->SetRangeUser( LowerEdge, UpperEdge );
	h_ratio->GetXaxis()->SetRangeUser( LowerEdge, UpperEdge );
	TopPad->SetLogx(kFALSE);
	BottomPad->SetLogx(kFALSE);
	legend->SetX1NDC( 0.55 );
	c->SaveAs(CanvasName + "_" + TStr_MassRange + ".pdf" );

	///////////////////////
	// -- 60 < M < 120 -- //
	///////////////////////
	LowerEdge = 60;
	UpperEdge = 120;

	TStr_MassRange = TString::Format("M%.0lfto%.0lf", LowerEdge, UpperEdge );
	h_fewz->GetXaxis()->SetRangeUser( LowerEdge, UpperEdge );
	h_ratio->GetXaxis()->SetRangeUser( LowerEdge, UpperEdge );
	TopPad->SetLogx(kFALSE);
	BottomPad->SetLogx(kFALSE);
	legend->SetX1NDC( 0.65 );
	c->SaveAs(CanvasName + "_" + TStr_MassRange + ".pdf" );

	///////////////////////
	// -- 120 < M < 3000 -- //
	///////////////////////
	LowerEdge = 120;
	UpperEdge = 3000;

	TStr_MassRange = TString::Format("M%.0lfto%.0lf", LowerEdge, UpperEdge );
	h_fewz->GetXaxis()->SetRangeUser( LowerEdge, UpperEdge );
	h_ratio->GetXaxis()->SetRangeUser( LowerEdge, UpperEdge );
	TopPad->SetLogx(kTRUE);
	BottomPad->SetLogx(kTRUE);
	legend->SetX1NDC( 0.55 );
	c->SaveAs(CanvasName + "_" + TStr_MassRange + ".pdf" );
}

TGraphAsymmErrors* Convert_XShiftGraph( TGraphAsymmErrors* g_data, TH1D* h_ratio )
{
	TGraphAsymmErrors* g_ratio = (TGraphAsymmErrors*)g_data->Clone("g_ratio");

	Int_t nPoint = g_data->GetN();
	for(Int_t i=0; i<nPoint; i++)
	{
		Double_t x, y, xErr_low, xErr_high;
		g_data->GetPoint( i, x, y );
		xErr_low = g_data->GetErrorXlow(i);
		xErr_high = g_data->GetErrorXhigh(i);

		Int_t i_bin = i+1;
		Double_t ratio = h_ratio->GetBinContent(i_bin);
		Double_t error = h_ratio->GetBinError(i_bin);

		g_ratio->SetPoint(i, x, ratio );
		g_ratio->SetPointError( i, xErr_low, xErr_high, error, error );
	}

	return g_ratio;
}

void DrawLatexNDC( TLatex &latex )
{
	Double_t Lumi = 2832.673;
	latex.DrawLatexNDC(0.69, 0.965, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi/1000.0 ) );
	latex.DrawLatexNDC(0.14, 0.965, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.25, 0.965, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
}