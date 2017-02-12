#include "SysUncTool_BkgEst.h"

void SysUnc_BkgEst(TString version)
{
	SysUncTool_BkgEst *tool = new SysUncTool_BkgEst(version);
	tool->SysUncEstimation();
	
	tool->MakeCanvas_ForEachBkgMethod();
	tool->MakeCanvas_SysUnc_StatSystTotal();
	tool->Save_All();
}