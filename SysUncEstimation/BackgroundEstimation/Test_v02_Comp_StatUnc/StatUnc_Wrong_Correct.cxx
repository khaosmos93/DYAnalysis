#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h>

Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
									 830, 1000, 1500, 3000};

void StatUnc_Wrong_Correct()
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName = ROOTFilePath + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root";
	TH1D* h_WZ = Get_Hist( FileName, "h_mass_OS_WZ" );
	h_WZ = (TH1D*)h_WZ->Rebin(nMassBin, h_WZ->GetName(), MassBinEdges);

	Double_t normFactor = (47.13 * Lumi ) / 999996;
	h_WZ->Scale( normFactor );

	Int_t nBin = h_WZ->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t nEvent = h_WZ->GetBinContent(i_bin);

		Double_t WrongUnc = sqrt( nEvent );
		Double_t CorrectUnc = h_WZ->GetBinError(i_bin);

		Double_t WrongRelUnc = 0;
		Double_t CorrectRelUnc = 0;
		if( nEvent != 0 )
		{
			WrongRelUnc = WrongUnc / nEvent;
			CorrectRelUnc = CorrectUnc / nEvent;
		}
		

		printf("[%02d bin] (Wrong, Correct) = (%8.2lf, %8.2lf) = (%9.5lf %%, %9.5lf %%)\n", i_bin, WrongUnc, CorrectUnc, WrongRelUnc, CorrectRelUnc);
	}
}