#include "CombineUnc_DetRes.h"

void CombineUnc_DetRes()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	CombineTool *tool = new CombineTool();
	tool->SummaryPlot();
	tool->SummaryPlot_StatSyst();

	TFile *f_output = TFile::Open("ROOTFile_SysUnc_DetRes.root", "RECREATE");
	tool->Save( f_output );
	f_output->Close();
}