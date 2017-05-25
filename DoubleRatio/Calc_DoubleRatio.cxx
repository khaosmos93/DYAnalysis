#include <Include/PlotTools.h>

void SetError_DoubleR( TFile *f_input, TH1D* h_DoubleR )
{
	TH1D* h_ExpUnc_8 = (TH1D*)f_input->Get("h_ExpUnc_8")->Clone();
	TH1D* h_ExpUnc_13 = (TH1D*)f_input->Get("h_ExpUnc_13")->Clone();
	TH1D* h_TheoUnc_8 = (TH1D*)f_input->Get("h_TheoUnc_8")->Clone();
	TH1D* h_TheoUnc_13 = (TH1D*)f_input->Get("h_TheoUnc_13")->Clone();

	Int_t nBin = h_ExpUnc_8->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t ExpUnc_8 = h_ExpUnc_8->GetBinContent(i_bin);
		Double_t ExpUnc_13 = h_ExpUnc_13->GetBinContent(i_bin);
		Double_t TheoUnc_8 = h_TheoUnc_8->GetBinContent(i_bin);
		Double_t TheoUnc_13 = h_TheoUnc_13->GetBinContent(i_bin);

		// -- all of them are combined by quadrature sum -- //
		Double_t TotUnc = sqrt( ExpUnc_8*ExpUnc_8 + ExpUnc_13*ExpUnc_13 +
								TheoUnc_8*TheoUnc_8 + TheoUnc_13*TheoUnc_13 );

		Double_t TotUncAbs = TotUnc * h_DoubleR->GetBinContent(i_bin);
		h_DoubleR->SetBinError(i_bin, TotUncAbs);
	}

	Print_Histogram( h_DoubleR );
}

void Calc_DoubleRatio(TString TStr_Channel = "LL")
{
	TFile *f_input = TFile::Open("ROOTFile_Input_DoubleRatio_"+TStr_Channel+".root");
	f_input->cd();

	TH1D* h_NormXSec_8 = (TH1D*)f_input->Get("h_NormXSec_8")->Clone();	
	TH1D* h_NormXSec_13 = (TH1D*)f_input->Get("h_NormXSec_13")->Clone();
	TH1D* h_NormXSec_NNLO_8 = (TH1D*)f_input->Get("h_NormXSec_NNLO_8")->Clone();	
	TH1D* h_NormXSec_NNLO_13 = (TH1D*)f_input->Get("h_NormXSec_NNLO_13")->Clone();

	/////////////////////////////////////////////
	// -- double ratio: experimental result -- //
	/////////////////////////////////////////////
	TH1D* h_DoubleR = (TH1D*)h_NormXSec_8->Clone("h_DoubleR");
	h_DoubleR->Divide( h_NormXSec_13, h_NormXSec_8 );
	Int_t nBin = h_DoubleR->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
		h_DoubleR->SetBinError(i+1, 0); // -- error will be set later -- //

	SetError_DoubleR( f_input, h_DoubleR );

	///////////////////////////////////////////
	// -- double ratio: theory prediction -- //
	///////////////////////////////////////////
	h_NormXSec_NNLO_8->Sumw2(); h_NormXSec_NNLO_13->Sumw2();
	TH1D* h_DoubleR_NNLO = (TH1D*)h_NormXSec_NNLO_8->Clone("h_DoubleR_NNLO");
	h_DoubleR_NNLO->Divide( h_NormXSec_NNLO_13, h_NormXSec_NNLO_8 ); // -- error of double ratio: quadrature sum -- //

	///////////////////////////
	// -- comparison plot -- //
	///////////////////////////
	HistInfo *HistInfo_data = new HistInfo( kBlack, "Data" );
	HistInfo_data->Set_Histogram( h_DoubleR );
	HistInfo_data->Set();

	// HistInfo *HistInfo_theory = new HistInfo( kBlue, "FEWZ, NNLO: CT10 (8TeV), NNPDF 3.0 (13TeV)" );
	HistInfo *HistInfo_theory = new HistInfo( kBlue, "FEWZ, NNLO NNPDF 3.0" );
	HistInfo_theory->Set_Histogram( h_DoubleR_NNLO );
	HistInfo_theory->Set();
	HistInfo_theory->h->SetFillColorAlpha( kBlue, 0.5 );
	HistInfo_theory->h->SetFillStyle(3144);
	HistInfo_theory->h->SetMarkerSize(0);
	HistInfo_theory->h->SetLineColorAlpha( kBlue, 0);

	HistInfo_data->CalcRatio_DEN( HistInfo_theory->h );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "c_DoubleR_Data_vs_Theory_"+TStr_Channel, TopPad, BottomPad, 1, 0 );
	c->cd();
	TopPad->cd();

	HistInfo_theory->Draw("E2SAME");
	HistInfo_data->Draw("EPSAME");

	SetHistFormat_TopPad( HistInfo_theory->h, "R" );
	HistInfo_theory->h->GetYaxis()->SetRangeUser( 0.3, 2.2 );
	

	TLegend *legend;
	SetLegend( legend, 0.17, 0.3, 0.70, 0.45 );
	HistInfo_data->AddToLegend( legend );
	HistInfo_theory->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	Latex_Preliminary_NoDataInfo( latex );
	if( TStr_Channel == "MM" )
	{
		latex.DrawLatexNDC( 0.17, 0.90, "#font[42]{#scale[0.7]{19.7 fb^{-1} #mu#mu (8 TeV)}}");
		latex.DrawLatexNDC( 0.17, 0.84, "#font[42]{#scale[0.7]{2.8 fb^{-1} #mu#mu (13 TeV)}}");
	}
	else if( TStr_Channel == "LL" )
	{
		latex.DrawLatexNDC( 0.17, 0.90, "#font[42]{#scale[0.7]{19.7 fb^{-1} ee and #mu#mu (8 TeV)}}");
		latex.DrawLatexNDC( 0.17, 0.84, "#font[42]{#scale[0.7]{2.3 fb^{-1} ee, 2.8 fb^{-1} #mu#mu (13 TeV)}}");
	}


	c->cd();
	BottomPad->cd();

	HistInfo_data->h_ratio->Draw("EPSAME");
	SetHistFormat_BottomPad( HistInfo_data->h_ratio, "m [GeV]", "Data/Theory", 0.7, 1.3);

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}