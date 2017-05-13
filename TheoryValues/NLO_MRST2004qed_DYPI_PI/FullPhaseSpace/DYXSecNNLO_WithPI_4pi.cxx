#include <Include/PlotTools.h>

void SetError( TH1D* h_divide, TH1D* h_ratio );

void DYXSecNNLO_WithPI_4pi()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
	printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_Ratio = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_DYNNLO = ROOTFilePath + "/ROOTFile_xSec_Theory.root";

	TH1D* h_DYNNLO = Get_Hist(FileName_DYNNLO, "h_DiffXsec_FEWZ_NNPDF_NNLO");
	TH1D* h_ratio = Get_Hist(FileName_Ratio, "h_ratio");
	TH1D* h_DYXSecNNLO_WithPI = (TH1D*)h_ratio->Clone("h_DYDiffXSecNNLO_WithPI");

	Int_t nBin = h_DYNNLO->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t NNLOXSec = h_DYNNLO->GetBinContent(i_bin);
		Double_t RelErr_NNLOXSec = h_DYNNLO->GetBinError(i_bin) / NNLOXSec;

		Double_t Ratio = h_ratio->GetBinContent(i_bin);
		Double_t RelErr_ratio = h_ratio->GetBinError(i_bin) / Ratio;

		Double_t NNLOXSec_DYPI = NNLOXSec * Ratio;
		// -- error propagation -- //
		Double_t RelErr_total = sqrt(RelErr_NNLOXSec*RelErr_NNLOXSec + RelErr_ratio*RelErr_ratio);

		h_DYXSecNNLO_WithPI->SetBinContent(i_bin, NNLOXSec_DYPI);
		h_DYXSecNNLO_WithPI->SetBinError(i_bin, RelErr_total * NNLOXSec_DYPI); // -- absolute error -- //
	}

	HistInfo *Hist_DYOnly = new HistInfo(kBlue, "DY (w/o PI)");
	Hist_DYOnly->Set_Histogram( h_DYNNLO );
	Hist_DYOnly->Set();

	HistInfo *Hist_DYPI = new HistInfo(kRed, "DY (w/ PI)");
	Hist_DYPI->Set_Histogram( h_DYXSecNNLO_WithPI );
	Hist_DYPI->Set();
	Hist_DYPI->CalcRatio_DEN( Hist_DYOnly->h );
	SetError( Hist_DYPI->h_ratio, h_ratio );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio(c, "c_DiffXSec_DYOnly_DYPI", TopPad, BottomPad, 1, 1 );

	// -- Top Pad -- //
	c->cd();
	TopPad->cd();
	Hist_DYOnly->Draw("EPSAME");
	Hist_DYPI->Draw("EPSAME");
	SetHistFormat_TopPad( Hist_DYOnly->h, "d#sigma/dm [pb/GeV]" );

	TLegend *legend;
	SetLegend( legend );
	Hist_DYOnly->AddToLegend( legend );
	Hist_DYPI->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	Latex_Simulation( latex );
	latex.DrawLatexNDC(0.18, 0.38, "#font[42]{#scale[0.8]{DY: NNPDF 3.0, NNLO}}");
	latex.DrawLatexNDC(0.18, 0.33, "#font[42]{#scale[0.8]{PI: MRST2004qed, NLO}}");

	// -- Bottom Pad -- //
	c->cd();
	BottomPad->cd();

	Hist_DYPI->h_ratio->SetMarkerColor(kBlack);
	Hist_DYPI->h_ratio->SetLineColor(kBlack);
	Hist_DYPI->DrawRatio("EPSAME");
	SetHistFormat_BottomPad( Hist_DYPI->h_ratio, "m [GeV]", "(DY+PI)/DY", 0.95, 1.15 );
	Hist_DYPI->h_ratio->GetYaxis()->SetNdivisions(504);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");

	TFile *f_output = TFile::Open("../ROOTFile_DYXSecNNLO_WithPI_4pi.root", "RECREATE");
	f_output->cd();

	h_DYNNLO->SetName("h_DYDiffXSecNNLO_WithoutPI");
	h_DYNNLO->Write();
	h_ratio->SetName("h_ratio_DYPI_PI");
	h_ratio->Write();
	h_DYXSecNNLO_WithPI->Write();
}

// -- correct the overestimated error of ratio -- //
void SetError( TH1D* h_divide, TH1D* h_ratio )
{
	Int_t nBin = h_ratio->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t divide = h_divide->GetBinContent(i_bin);
		Double_t ratio = h_ratio->GetBinContent(i_bin);
		if( divide != ratio )
			printf("[%02d bin] (divide = %lf) != (ratio = %lf)\n", i_bin, divide, ratio);
		
		Double_t Error = h_ratio->GetBinError(i_bin);
		h_divide->SetBinError(i_bin, Error);
	}
}