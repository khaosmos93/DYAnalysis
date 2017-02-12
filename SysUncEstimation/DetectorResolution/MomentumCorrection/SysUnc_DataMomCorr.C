#include "SysUncTool_DataMomCorr.h"

void SysUnc_DataMomCorr(TString version, TString Ver_CMSSW = "76X")
{
	// -- Calc the smeared mass histogram -- //
	// SysUncTool_DataMomCorr *tool = new SysUncTool_DataMomCorr(version, Ver_CMSSW);
	// tool->SetIsDataDriven( kTRUE );
	// tool->Calc_DYMassSpectrum();
	// tool->Calc_DiffXsec_All();
	// tool->Calc_SysUnc();
	// tool->SaveResults();
	// tool->ValidationPlots();


	// -- uses the smeared mass histogram stored in a root file -- //
	SysUncTool_DataMomCorr *tool = new SysUncTool_DataMomCorr(version, Ver_CMSSW);
	tool->SetIsDataDriven( kTRUE );

	TFile *f_input = TFile::Open("/home/kplee/ntupleMaker/DYAnalysis_76X/SysUncEstimation/DetectorResolution/MomentumCorrection/v20160602_1st_LatestResults/ROOTFile_Histograms_SysUnc_DataMomCorr.root");
	tool->GetHistograms_fromROOTFile( f_input );
	tool->Calc_DiffXsec_All();
	tool->Calc_SysUnc();
	tool->SaveResults();
	tool->ValidationPlots();
}