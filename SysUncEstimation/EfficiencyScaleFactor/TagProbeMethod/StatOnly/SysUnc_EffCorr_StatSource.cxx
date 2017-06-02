#include "../SysUncTool_EffCorr.h"

void SysUnc_EffCorr_StatSource()
{
	SysUncTool_EffCorr *SysTool = new SysUncTool_EffCorr();
	SysTool->SetIsDataDriven( kTRUE );

	SysTool->SetupCentralValueStatError("ROOTFile_TagProbeEfficiency_76X_v20160502.root");
	SysTool->Generate_SmearedEffMap_StatOnly();

	SysTool->CorrectedEff_AllMap();
	SysTool->CalcXsec_AllMap();
	SysTool->SaveResults();


	// -- Get efficiency scale factors from existing root file -- //
	// TFile *f_input = TFile::Open("/home/kplee/DYAnalysis_76X/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/v20160511_2nd_DebugCorr_DataDrivenBkg_M100to200/ROOTFile_Outputs_SysUncTool_EffCorr.root");
	// SysTool->LoadEffGraphs_fromROOTFile( f_input );
	// SysTool->CalcXsec_AllMap(version, Ver_CMSSW);
	// SysTool->SaveResults();
}