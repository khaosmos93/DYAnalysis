#include "StatUnc_StepByStep.h"

void StatUnc_StepByStep()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );
	
	Tool_StatUnc_StepByStep *tool = new Tool_StatUnc_StepByStep();
	tool->Calc_RelStatUnc_StepByStep();
	tool->Make_XCheckPlot();
	tool->Save_RelStatUnc();
}