#include <Include/PlotTools.h>

void Compare_PreviousTotUnc_TnPEff()
{
	TString IncludePath = gSystem->Getenv("KP_INCLUDE_PATH");
	TString FileName_CV = IncludePath + "/ROOTFile_TagProbeEfficiency_76X_v20160502.root";
	TString FileName_Old = IncludePath + "/ROOTFile_SysUnc_TagProbeMethod_76X_v20160504.root";
	TString FileName_New = "ROOTFile_Syst_TnPEffs_QuadSumAll.root";

	TString HistName_CV = "h_2D_Eff_RecoID_Data";
	TString HistName_Old = "h_2D_RelDiff_RecoID_Data_Tot";
	TString HistName_New = "h_2D_EffDiff_RecoID_Data";

	TH2D* h_CV = Get_Hist_2D( FileName_CV, HistName_CV );
	TH2D* h_Old = Get_Hist_2D( FileName_Old, HistName_Old );
	TH2D* h_New = Get_Hist_2D( FileName_New, HistName_New );

	Int_t nBinX = h_CV->GetNbinsX();
	Int_t nBinY = h_CV->GetNbinsY();
	for(Int_t i_eta=0; i_eta<nBinX; i_eta++)
	{
		for(Int_t i_pt=0; i_pt<nBinY; i_pt++)
		{
			Int_t i_etabin = i_eta+1;
			Int_t i_ptbin = i_pt+1;

			Double_t Eff_CV = h_CV->GetBinContent(i_etabin, i_ptbin);
			Double_t RelDiff_Old = h_Old->GetBinContent(i_etabin, i_ptbin);
			Double_t AbsDiff_Old = Eff_CV * RelDiff_Old;

			Double_t AbsDiff_New = h_New->GetBinContent(i_etabin, i_ptbin);

			printf("[(etabin, ptbin) = (%d, %d)] AbsDiff: (abs(Old), New) = (%9.5lf, %9.5lf)\n", i_etabin, i_ptbin, AbsDiff_Old, AbsDiff_New);
		}
	}
}