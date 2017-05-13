#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>
void DrawPlot_MCBkg()
{
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName = ROOTFilePath + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root";

	TH1D* h_WZ = Get_Hist( FileName, "h_mass_OS_WZ");
	TH1D* h_ZZ = Get_Hist( FileName, "h_mass_OS_ZZ");

	h_WZ = (TH1D*)h_WZ->Rebin(nMassBin, h_WZ->GetName(), MassBinEdges);
	h_ZZ = (TH1D*)h_ZZ->Rebin(nMassBin, h_ZZ->GetName(), MassBinEdges);

	HistInfo *Hist_WZ = new HistInfo(kBlue, "WZ");
	Hist_WZ->Set_Histogram( h_WZ );
	Hist_WZ->Set();

	HistInfo *Hist_ZZ = new HistInfo(kGreen+1, "ZZ");
	Hist_ZZ->Set_Histogram( h_ZZ );
	Hist_ZZ->Set();
	Hist_ZZ->h->SetMarkerStyle( 24 );

	TCanvas *c;
	SetCanvas_Square( c, "Muon_Bkg_MCBased", 1, 1 );
	c->cd();

	Hist_WZ->Draw("EPSAME");
	Hist_ZZ->Draw("EPSAME");

	SetHistFormat_SinglePad( Hist_WZ->h, "m [GeV]", "Entries per bin" );
	Hist_WZ->h->GetYaxis()->SetRangeUser( 0.5, 1e5 );

	TLegend *legend;
	SetLegend( legend, 0.75, 0.75, 0.95, 0.95 );
	Hist_WZ->AddToLegend( legend );
	Hist_ZZ->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	Latex_Simulation( latex );

	c->SaveAs(".pdf");
}