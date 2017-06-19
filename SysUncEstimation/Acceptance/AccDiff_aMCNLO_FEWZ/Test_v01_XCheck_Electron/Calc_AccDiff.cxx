#include <Include/PlotTools.h>

void Calc_AccDiff()
{
	TString FileName_aMCNLO = "ROOTFIle_Acc_Dressed_EE.root";
	TGraphAsymmErrors *g_Acc = Get_Graph( FileName_aMCNLO, "g_Acc" );
	TH1D* h_aMCNLO = Convert_GraphToHist( g_Acc );

	TString FileName_FEWZ = "ROOTFile_Acc_Electron_FEWZ_PDF4LHC15.root";
	TH1D* h_FEWZ = Get_Hist( FileName_FEWZ, "h_Acc" );

	TH1D* h_RelDiff = Calc_RelDiff( h_aMCNLO, h_FEWZ );

	// -- Comparison between aMC@NLO and FEWZ -- //
	HistInfo *Hist_aMCNLO = new HistInfo( kRed, "aMC@NLO", h_aMCNLO );
	HistInfo *Hist_FEWZ = new HistInfo( kBlue, "FEWZ (NNLO)", h_FEWZ );

	TString CanvasName = "Acc_aMCNLO_FEWZ_EE";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( CanvasName, Hist_FEWZ, Hist_aMCNLO );
	canvas->SetTitle( "m [GeV]", "Acceptance" "FEWZ/aMC@NLO");
	canvas->SetLegendPosition( 0.55, 0.20, 0.95, 0.30 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw();
}

TH1D* h_RelDiff( TH1D* h_aMCNLO, TH1D* h_FEWZ )
{
	TH1D* h_RelDiff = h_aMCNLO->Clone();
	Int_t nBin = h_aMCNLO->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t aMCNLO = h_aMCNLO->GetBinContent(i_bin);
		Double_t FEWZ = h_FEWZ->GetBinContent(i_bin);

		Double_t RelDiff = fabs( aMCNLO - FEWZ ) / aMCNLO;

		h_RelDiff->SetBinContent(i_bin, RelDiff);
		h_RelDiff->SetBinError(i_bin);
	}

	return h_RelDiff;
}
