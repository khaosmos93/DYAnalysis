#pragma once

// #define nMassBin 45
#define nMassBin 43

class UncertantyCalcTool
{
public:
	Double_t MassBinEdges[45+1];
	TH1D* h_RelUnc_Stat;
	TH1D* h_RelUnc_Bkg;

	UncertantyCalcTool();
	void StatUnc( TH1D* h_unfolded, TH1D* h_data );
	void BkgUnc( TH1D* h_unfolded, TH1D *h_ttbar, TH1D *h_DYtautau, TH1D *h_tW, TH1D *h_WJets, TH1D *h_QCD, TH1D *h_ZZ, TH1D *h_WZ, TH1D *h_WW);


};

UncertantyCalcTool::UncertantyCalcTool()
{
	// Double_t MassBinEdges_Clone[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
	// 											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
	// 											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
	// 											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
	// 											 830, 1000, 1200, 1500, 2000, 3000};

	Double_t MassBinEdges_Clone[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
												 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
												 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
												 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
												 830, 1000, 1500, 3000};

	for(Int_t i=0; i<nMassBin+1; i++)
		MassBinEdges[i] = MassBinEdges_Clone[i];

	h_RelUnc_Stat = new TH1D("h_RelUnc_Stat", "", nMassBin, MassBinEdges);
	h_RelUnc_Bkg = new TH1D("h_RelUnc_Bkg", "", nMassBin, MassBinEdges);
}


void UncertantyCalcTool::StatUnc( TH1D* h_unfolded, TH1D* h_data )
{
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t nEvent_Unfold = h_unfolded->GetBinContent(i_bin);
		Int_t nEvent_Data = h_data->GetBinContent(i_bin);

		Double_t RelUnc = sqrt(nEvent_Data) / nEvent_Unfold;

		h_RelUnc_Stat->SetBinContent(i_bin, RelUnc);
		h_RelUnc_Stat->SetBinError(i_bin, 0);

		printf( "[%.2d bin: (%.4lf, %.4lf)] ( nEvent_Unfolded, nEvent_Data, sqrt(nEvent_Data), StatUnc(%%) ) = (%12.1lf, %12.d, %12.1lf, %12.3lf)\n", 
			i_bin, MassBinEdges[i], MassBinEdges[i+1], nEvent_Unfold, nEvent_Data, sqrt(nEvent_Data), RelUnc*100 );
	}
}


void UncertantyCalcTool::BkgUnc( TH1D* h_unfolded, TH1D *h_ttbar, TH1D *h_DYtautau, TH1D *h_tW, TH1D *h_WJets, TH1D *h_QCD, TH1D *h_ZZ, TH1D *h_WZ, TH1D *h_WW)
{
	// cout << "Uncertainty of WZ & ZZ cross section is not filled yet!" << endl;

	Double_t RelUnc_xSec_WW = 0.0799; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-14-016/index.html -- //
	Double_t RelUnc_xSec_WZ = 0.4052; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-011/index.html -- //
	Double_t RelUnc_xSec_ZZ = 0.1087; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-005/index.html -- //

	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t nEvent_Unfold = h_unfolded->GetBinContent(i_bin);

		// -- estimated from data-driven backgrounds -- //
		Double_t AbsUnc_ttbar = h_ttbar->GetBinError(i_bin);
		Double_t AbsUnc_DYtautau = h_DYtautau->GetBinError(i_bin);
		Double_t AbsUnc_tW = h_tW->GetBinError(i_bin);
		Double_t AbsUnc_WJets = h_WJets->GetBinError(i_bin);
		Double_t AbsUnc_QCD = h_QCD->GetBinError(i_bin);

		// -- estimated by MC: sys.uncertainty = stat.uncertainty & uncertainty of x-section -- //
		Double_t nEvent_ZZ = h_ZZ->GetBinContent(i_bin);
		Double_t nEvent_WZ = h_WZ->GetBinContent(i_bin);
		Double_t nEvent_WW = h_WW->GetBinContent(i_bin);

		Double_t AbsUnc_Stat_ZZ = sqrt( nEvent_ZZ );
		Double_t AbsUnc_xSec_ZZ = RelUnc_xSec_ZZ * nEvent_ZZ;
		Double_t AbsUnc_Tot_ZZ = sqrt( AbsUnc_Stat_ZZ*AbsUnc_Stat_ZZ + AbsUnc_xSec_ZZ*AbsUnc_xSec_ZZ );

		Double_t AbsUnc_Stat_WZ = sqrt( nEvent_WZ );
		Double_t AbsUnc_xSec_WZ = RelUnc_xSec_WZ * nEvent_WZ;
		Double_t AbsUnc_Tot_WZ = sqrt( AbsUnc_Stat_WZ*AbsUnc_Stat_WZ + AbsUnc_xSec_WZ*AbsUnc_xSec_WZ );

		Double_t AbsUnc_Stat_WW = sqrt( nEvent_WW );
		Double_t AbsUnc_xSec_WW = RelUnc_xSec_WW * nEvent_WW;
		Double_t AbsUnc_Tot_WW = sqrt( AbsUnc_Stat_WW*AbsUnc_Stat_WW + AbsUnc_xSec_WW*AbsUnc_xSec_WW );

		// printf("(nEvent_WW, AbsUnc_Stat_WW, AbsUnc_xSec_WW, AbsUnc_Tot_WW) = (%6.2lf, %6.2lf, %6.2lf, %6.2lf)\n", 
			// nEvent_WW, AbsUnc_Stat_WW, AbsUnc_xSec_WW, AbsUnc_Tot_WW);

		Double_t QuadSum_AbsUnc_Bkg = sqrt( AbsUnc_ttbar * AbsUnc_ttbar +
											AbsUnc_DYtautau * AbsUnc_DYtautau +
											AbsUnc_tW * AbsUnc_tW + 
											AbsUnc_WJets * AbsUnc_WJets + 
											AbsUnc_QCD * AbsUnc_QCD +
											AbsUnc_Tot_ZZ * AbsUnc_Tot_ZZ + 
											AbsUnc_Tot_WZ * AbsUnc_Tot_WZ + 
											AbsUnc_Tot_WW * AbsUnc_Tot_WW );

		Double_t RelUnc = QuadSum_AbsUnc_Bkg / nEvent_Unfold;

		h_RelUnc_Bkg->SetBinContent(i_bin, RelUnc);
		h_RelUnc_Bkg->SetBinError(i_bin, 0);

		printf( "[%.2d bin: (%.4lf, %.4lf)] ( nEvent_Unfolded, QuadSum_AbsUnc_Bkg, SysUnc(%%) ) = (%12.3lf, %12.3lf, %12.3lf)\n", 
			i_bin, MassBinEdges[i], MassBinEdges[i+1], nEvent_Unfold, QuadSum_AbsUnc_Bkg, RelUnc*100 );

		printf("\t[AbsUnc] (ttbar, DYtautau, tW, WJets, QCD, ZZ, WZ, WW) = (%6.2lf, %6.2lf, %6.2lf, %6.2lf, %6.2lf, %6.2lf, %6.2lf, %6.2lf)\n",
			AbsUnc_ttbar, AbsUnc_DYtautau, AbsUnc_tW, AbsUnc_WJets, AbsUnc_QCD, AbsUnc_Tot_ZZ, AbsUnc_Tot_WZ, AbsUnc_Tot_WW);

		Double_t nEvent_WJets = h_WJets->GetBinContent(i_bin);
		printf("\t[WJet] (# events, AbsUnc, RelUnc) = (%6.2lf, %6.2lf, %12.5lf)\n\n", nEvent_WJets, AbsUnc_WJets, AbsUnc_WJets/nEvent_WJets );
	}
}










