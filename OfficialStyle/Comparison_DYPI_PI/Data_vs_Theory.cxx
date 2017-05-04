#include <Include/PlotTools.h>

void AssignErrors_NoError( TH1D* h );
// void RemoveBelowThreshold( TH1D* h, Double_t MassCut );
void Data_vs_Theory()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
	printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

	TString FileName_Data = ROOTFilePath + "/ROOTFile_DiffXSec_FullUnc.root";
	TString FileName_DYPI = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYXSecNNLO_WithPI.root";

	TH1D* h_data = Get_Hist(FileName_Data, "h_DiffXsec_FSRCorr_woLumi");
	TH1D* h_DY = Get_Hist(FileName_Data, "h_DiffXsec_FEWZ_NNPDF_NNLO");
	TH1D* h_DYPI = Get_Hist(FileName_DYPI, "h_DYDiffXSecNNLO_WithPI");

	TH1D* h_RelTotUnc = Extract_RelUnc( h_data, "h_RelTotUnc_woLumi" );
	TH1D* h_RelStatUnc = Get_Hist( FileName_Data, "h_RelStatUnc_Percent" ); h_RelStatUnc->Scale( 1.0 / 100 );
	TH1D* h_RelTheoUnc = Extract_RelUnc( h_DYPI, "h_RelTheoUnc_DYwithPI");

	TH1D* h_StatUnc = (TH1D*)h_data->Clone();
	h_StatUnc->Divide( h_StatUnc, h_StatUnc ); // -- ratio = 1 line -- //
	AssignErrors( h_StatUnc, h_RelStatUnc );
	h_StatUnc->SetMarkerStyle(20);
	h_StatUnc->SetMarkerColor(kBlack);

	TH1D* h_TotUnc = (TH1D*)h_StatUnc->Clone();
	AssignErrors( h_TotUnc, h_RelTotUnc );
	h_TotUnc->SetMarkerColorAlpha(kWhite, 0);
	h_TotUnc->SetLineColorAlpha(kWhite, 0);
	h_TotUnc->SetFillColorAlpha( kBlack, 1 );
	h_TotUnc->SetFillStyle( 3354 );

	TH1D* h_ratio_noPI = (TH1D*)h_DY->Clone();
	h_ratio_noPI->Divide( h_DY, h_data ); 
	AssignErrors_NoError( h_ratio_noPI ); // -- theory / data, uncertainty will not be displayed -- //
	// h_ratio_noPI->SetMarkerColorAlpha(kWhite, 0);
	h_ratio_noPI->SetMarkerStyle(20);
	h_ratio_noPI->SetMarkerSize(0);
	h_ratio_noPI->SetLineColor(kRed);
	h_ratio_noPI->SetLineStyle( 2 );
	h_ratio_noPI->SetFillColorAlpha(kWhite, 0);
	
	TH1D* h_ratio_withPI = (TH1D*)h_DYPI->Clone();
	h_ratio_withPI->Divide( h_DYPI, h_data );
	AssignErrors( h_ratio_withPI, h_RelTheoUnc ); // -- uncertainty: only theoretical uncertianty -- //
	h_ratio_withPI->SetMarkerColorAlpha(kWhite, 0);
	h_ratio_withPI->SetLineColorAlpha(kWhite, 0);
	h_ratio_withPI->SetFillColorAlpha( kOrange, 0.4 );

	TCanvas *c;
	SetCanvas_Square( c, "c_ratio_theoryData", 1, 0 );

	h_TotUnc->Draw("E2PSAME");
	h_StatUnc->Draw("E1PSAME");
	h_ratio_withPI->Draw("E2SAME");
	h_ratio_noPI->Draw("EPSAME");

	h_TotUnc->SetTitle("");
	SetHistFormat_SinglePad(h_TotUnc, "m (#mu#mu) [GeV]", "Theory/Data");

	TLegend *legend;
	SetLegend( legend, 0.15, 0.15, 0.70, 0.30 );
	legend->SetNColumns( 2 );
	legend->AddEntry( h_StatUnc, "Stat. Unc.");
	legend->AddEntry( h_TotUnc, "Tot. Unc.");
	legend->AddEntry( h_ratio_withPI, "Theory(w/ PI)/Data");
	legend->AddEntry( h_ratio_noPI, "Theory(w/o PI)/Data");
	legend->Draw();

	TLatex latex;
	Latex_Preliminary( latex, 2.8, 13 );

	c->SaveAs(".pdf");

	TString CanvasName = c->GetName();
	// c->SetLogx(kFALSE);
	h_TotUnc->GetXaxis()->SetRangeUser(15, 320);
	h_TotUnc->GetYaxis()->SetRangeUser(0.8, 1.2);
	c->SaveAs(CanvasName+"_M15to320.pdf");

	// RemoveBelowThreshold( h_TotUnc, 320 );
	// RemoveBelowThreshold( h_StatUnc, 320 );
	// RemoveBelowThreshold( h_ratio_withPI, 320 );
	// RemoveBelowThreshold( h_ratio_noPI, 320 );

	// c->SetLogx(kTRUE);
	h_TotUnc->GetXaxis()->SetRangeUser(320, 3000);
	h_StatUnc->GetXaxis()->SetRangeUser(320, 3000);
	h_ratio_withPI->GetXaxis()->SetRangeUser(320, 3000);
	h_ratio_noPI->GetXaxis()->SetRangeUser(320, 3000);
	h_TotUnc->GetYaxis()->SetRangeUser(0, 3);
	c->SaveAs(CanvasName+"_M320to3000.pdf");
}

void AssignErrors_NoError( TH1D* h )
{
	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		h->SetBinError(i_bin, 1e-30);
	}
}

// TH1D* RemoveBelowThreshold( TH1D* h )
// {
// 	const Int_t nMassBin_temp = 9;
// 	Double_t MassBinEdges_temp[nMassBin_temp+1] = 
// 	{320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

// 	h = (TH1D*)h->Rebin(nMassBin_temp, h->GetName(), MassBinEdges_temp);
// }
