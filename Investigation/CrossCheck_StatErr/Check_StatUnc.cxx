#include <Include/PlotTools.h>

TString BasePath = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval/"
TString FileName_Data = BasePath + "ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root"
TString HistName_Data = "h_mass_OS_Data";

TString FileName_CentralValues = BasePath + "ROOTFile_Results_DYAnalysis_76X.root";
TString HistName_Unfolded_HLT4p2 = "h_yield_HLTv4p2_Unfolded";
TString HistName_Unfolded_HLT4p3 = "h_yield_HLTv4p3_Unfolded";
TString HistName_Unfolded = "h_yield_Unfolded";
TString HistName_DiffXSec = "h_DiffXsec_FSRCorr";

TString FileName_FinalResult = BasePath + "ROOTFile_DiffXSec_FullUnc.root";
TString HistName_DiffXSec_Final = "h_DiffXsec_FSRCorr";
TString HistName_StatUnc_Final = "h_RelStatUnc_Percent";

const Int_t nMassBin = 43;
Double_t MassBinEdges[nMassBin+1] = {
	15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
	64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
	110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
	200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
	830, 1000, 1500, 3000};


// TH1D* Get_Hist(TString FileName, TString HistName )
// {
// 	TH1::AddDirectory(kFALSE);

// 	TFile *f_input = TFile::Open( FileName );
// 	TH1D* h_temp = (TH1D*)f_input->Get(HistName)->Clone();
// 	f_input->Close();

// 	return h_temp;
// }


// -- print-out all relavant values for stat. unc. calculation -- //
void Check_StatUnc()
{
	TH1D* h_ObservedYield = Get_Hist( FileName_Data, HistName_Data );
	h_ObservedYield = (TH1D*)h_ObservedYield->Rebin(nMassBin, h_ObservedYield->GetName(), MassBinEdges);

	TH1D* h_Unfolded_HLTv4p2 = Get_Hist( FileName_CentralValues, HistName_Unfolded_HLT4p2 );
	TH1D* h_Unfolded_HLTv4p3 = Get_Hist( FileName_CentralValues, HistName_Unfolded_HLT4p3 );
	TH1D* h_Unfolded = Get_Hist( FileName_CentralValues, HistName_Unfolded );

	TH1D* h_DiffXSec = Get_Hist( FileName_CentralValues, HistName_DiffXSec );

	TH1D* h_DiffXSec_Final = Get_Hist( FileName_FinalResult, HistName_DiffXSec_Final );
	TH1D* h_StatUnc_Final = Get_Hist( FileName_FinalResult, HistName_StatUnc_Final );

	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t ObservedYield = h_ObservedYield->GetBinContent(i_bin);

		Double_t UnfoldedYield_HLTv4p2 = h_Unfolded_HLTv4p2->GetBinContent(i_bin);
		Double_t UnfoldedYield_HLTv4p3 = h_Unfolded_HLTv4p3->GetBinContent(i_bin);
		Double_t UnfoldedYield = h_Unfolded->GetBinContent(i_bin);
		Double_t RelStatUnc = sqrt( ObservedYield ) / UnfoldedYield;

		Double_t DiffXSec = h_DiffXSec->GetBinContent(i_bin);
		Double_t AbsStatUnc = DiffXSec * RelStatUnc;

		Double_t DiffXSec_Final = h_DiffXSec_Final->GetBinContent(i_bin);
		Double_t RelStatUnc_Final = h_StatUnc_Final->GetBinContent(i_bin) / 100.0;
		Double_t AbsStatUnc_Final = DiffXSec_Final*RelStatUnc_Final;

		printf("[%02d bin: %04d < M < %04d GeV]\n", i_bin, MassBinEdges[i], MassBinEdges[i+1] );
		printf("\tYield: (Observed, Unfolded) = (%.0lf, %.0lf = %.0lf (HLTv4.2) + %.0lf (HLTv4.3))\n", 
				ObservedYield, UnfoldedYield, UnfoldedYield_HLTv4p2, UnfoldedYield_HLTv4p3 );
		printf("\tDiffXSec = %.3e, Stat.Unc = %.3e (%.3lf %%)\n", DiffXSec, AbsStatUnc, RelStatUnc*100 );
		printf("\tDiffXSec = %.3e, Stat.Unc = %.3e (%.3lf %%) <- values in final result\n", DiffXSec_Final, AbsStatUnc_Final, RelStatUnc_Final*100 );

		if( fabs(DiffXSec - DiffXSec_Final) != 0 || fabs(RelStatUnc - RelStatUnc_Final) ! = 0 )
			printf("\t\t[WARNING! final results are different with the results in intermediate step]\n");

		printf("\n");
	}
}

