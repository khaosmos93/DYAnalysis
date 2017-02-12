#include "SysUncTool_Unfolding_MCModel.h"

void SysUnc_Unfolding_MCModel(TString version)
{
	SysUncTool_Unfolding_MCModel *tool = new SysUncTool_Unfolding_MCModel(version);
	// tool->nEvent_Test = 1000000;
	tool->Calc_RespM_AltMC();
	tool->ClosureTest();
	tool->Calc_SysUnc();
}