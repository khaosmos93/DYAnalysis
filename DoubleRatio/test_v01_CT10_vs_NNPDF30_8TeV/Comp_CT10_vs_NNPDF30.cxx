#include <Include/PlotTools.h>

void DrawCanvas_DoubleR( TString AnalyzerPath, TH1D* h_CT10, TH1D* h_NNPDF );
TH1D* NormXSec_Theory( TH1D* h_DiffXSec );

void Comp_CT10_vs_NNPDF30()
{
	TString cwd = gSystem->GetWorkingDirectory();
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_NNPDF = AnalyzerPath+"/TheoryValues/NNLO_NNPDF30_8TeV_forDoubleRatio/ROOTFile_DY_FEWZ_NNLO_NNPDF30_nnlo_as_0118.root";
	TH1D* h_NNPDF = Get_Hist(FileName_NNPDF, "h_DiffXsec");

	// -- differential cross section comparison -- //
	TH1D* h_CT10_AbsXSec = Get_Hist("../../Input_8TeV/1Dabsxsec_NNLO_CTEQ12NNLO.root", "invm_FEWZ");
	TH1D* h_CT10 = (TH1D*)h_CT10_AbsXSec->Clone();
	Int_t nBin = h_CT10_AbsXSec->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t AbsXSec = h_CT10_AbsXSec->GetBinContent(i_bin);
		Double_t Err_AbsXSec = h_CT10_AbsXSec->GetBinError(i_bin);
		Double_t BinWidth = h_CT10_AbsXSec->GetBinWidth(i_bin);

		Double_t DiffXSec = AbsXSec / BinWidth;
		Double_t Err_DiffXSec = Err_AbsXSec / BinWidth;

		h_CT10->SetBinContent(i_bin, DiffXSec);
		h_CT10->SetBinError(i_bin, Err_DiffXSec);
	}

	HistInfo *Hist_CT = new HistInfo( kGreen+2, "CT10" );
	Hist_CT->Set_Histogram( h_CT10 );
	Hist_CT->Set();

	HistInfo *Hist_NNPDF = new HistInfo( kBlue+1, "NNPDF 3.0" );
	Hist_NNPDF->Set_Histogram( h_NNPDF );
	Hist_NNPDF->Set();
	Hist_NNPDF->Calc_RatioHist_Numerator( Hist_CT->h );

	// -- make canvas -- //
	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "c_CT10_vs_NNPDF30_8TeV", TopPad, BottomPad, 1, 1 );

	c->cd();
	TopPad->cd();

	Hist_CT->h->Draw("EPSAME");
	Hist_NNPDF->h->Draw("EPSAME");
	SetHistFormat_TopPad( Hist_CT->h, "d#sigma/dm [pb/GeV]");

	TLegend *legend;
	SetLegend( legend );
	legend->AddEntry( Hist_CT->h, Hist_CT->LegendName );
	legend->AddEntry( Hist_NNPDF->h, Hist_NNPDF->LegendName );
	legend->Draw();

	TLatex latex;
	Latex_Simulation( latex, 8 );

	c->cd();
	BottomPad->cd();
	Hist_NNPDF->h_ratio->Draw("EPSAME");
	SetHistFormat_BottomPad( Hist_NNPDF->h_ratio, "m [GeV]", "CT10/NNPDF3.0", 0.6, 1.2 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");

	DrawCanvas_DoubleR( AnalyzerPath, h_CT10, h_NNPDF );
}

TH1D* NormXSec_Theory( TH1D* h_DiffXSec )
{
	TH1D* h_NormXSec = (TH1D*)h_DiffXSec->Clone();
	// -- Z-peak x-sec -- //
	Double_t XSec_Zpeak = 0;
	Int_t nBin = h_DiffXSec->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t DiffXSec = h_DiffXSec->GetBinContent(i_bin);
		Double_t BinWidth = h_DiffXSec->GetBinWidth(i_bin);
		Double_t AbsXSec = DiffXSec * BinWidth;

		Double_t BinCenter = h_DiffXSec->GetBinCenter(i_bin);
		if( BinCenter > 60 && BinCenter < 120 )
			XSec_Zpeak = XSec_Zpeak + AbsXSec;
		else if( BinCenter > 120 )
			break;
	}

	printf("[Z-peak x-section: %lf]\n", XSec_Zpeak);

	// -- normalized x-section -- //
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t DiffXSec = h_DiffXSec->GetBinContent(i_bin);
		Double_t NormXSec = DiffXSec / XSec_Zpeak;

		h_NormXSec->SetBinContent( i_bin, NormXSec );

		Double_t DiffXSecAbsError = h_DiffXSec->GetBinError(i_bin);
		Double_t NormXSecAbsError = DiffXSecAbsError / XSec_Zpeak;

		h_NormXSec->SetBinError( i_bin, NormXSecAbsError );
	}

	return h_NormXSec;
}


void DrawCanvas_DoubleR( TString AnalyzerPath, TH1D* h_CT10, TH1D* h_NNPDF )
{
	// -- double ratio calculation -- //
	TH1D* h_NormXSec_NNPDF_8 = NormXSec_Theory( h_NNPDF );
	TH1D* h_NormXSec_CT_8 = NormXSec_Theory( h_CT10 );
	TH1D* h_NormXSec_NNPDF_13 = Get_Hist( AnalyzerPath+"/DoubleRatio/ROOTFile_Input_DoubleRatio_LL.root", "h_NormXSec_NNLO_13");

	TH1D* h_DoubleR_NNPDF = (TH1D*)h_NormXSec_NNPDF_13->Clone();
	h_DoubleR_NNPDF->Divide( h_NormXSec_NNPDF_13, h_NormXSec_NNPDF_8 );

	TH1D* h_DoubleR_CT = (TH1D*)h_NormXSec_NNPDF_13->Clone();
	h_DoubleR_CT->Divide( h_NormXSec_NNPDF_13, h_NormXSec_CT_8 );

	HistInfo *Hist_DoubleR_NNPDF = new HistInfo( kBlue+1, "NNPDF3.0 (13 TeV) / NNPDF3.0 (8 TeV)");
	Hist_DoubleR_NNPDF->Set_Histogram( h_DoubleR_NNPDF );
	Hist_DoubleR_NNPDF->Set();

	HistInfo *Hist_DoubleR_CT = new HistInfo( kGreen+2, "NNPDF3.0 (13 TeV) / CT10 (8 TeV)");
	Hist_DoubleR_CT->Set_Histogram( h_DoubleR_CT );
	Hist_DoubleR_CT->Set();

	TCanvas *c;
	SetCanvas_Square( c, "c_DoubleR", 1, 0 );

	c->cd();
	Hist_DoubleR_CT->Draw( "EPSAME" );
	Hist_DoubleR_NNPDF->Draw( "EPSAME" );

	SetHistFormat_SinglePad( Hist_DoubleR_CT->h, "m [GeV]", "R" );

	TLegend *legend;
	SetLegend( legend, 0.15, 0.75, 0.75, 0.95 );
	Hist_DoubleR_CT->AddToLegend( legend );
	Hist_DoubleR_NNPDF->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.25, 0.96, "#font[42]{#it{#scale[0.8]{Simulation}}}");

	c->SaveAs(".pdf");
}
