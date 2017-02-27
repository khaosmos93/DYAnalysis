#include "SysUncTool_BkgEst.h"

void SysUnc_BkgEst()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );

	SysUncTool_BkgEst *tool = new SysUncTool_BkgEst();
	tool->SysUncEstimation();
	
	tool->MakeCanvas_ForEachBkgMethod();
	tool->MakeCanvas_SysUnc_StatSystTotal();
	tool->Save_All();
}