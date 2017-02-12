#include "SysUncTool_MCMomCorr.h"

void SysUnc_MCMomCorr(TString version, TString Ver_CMSSW = "76X")
{
	SysUncTool_MCMomCorr *tool = new SysUncTool_MCMomCorr(version, Ver_CMSSW);
	tool->SetIsDataDriven( kTRUE );
	tool->Calc_ResponseMatrix();
	tool->Calc_DiffXsec_All();
	tool->Calc_SysUnc();
	tool->SaveResults();
	tool->ValidationPlots();
}