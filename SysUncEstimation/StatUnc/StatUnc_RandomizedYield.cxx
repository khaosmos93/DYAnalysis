#include "StatUnc_RandomizedYield.h"

void StatUnc_RandomizedYield()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	StatUncTool* tool = new StatUncTool();
	tool->Estimate();
	tool->Make_XCheckPlot_DiffXSec();
	tool->SetStatUnc_ByHand();
	
	TFile *f_output = TFile::Open("../ROOTFile_RelStatUnc.root", "RECREATE");
	tool->Save_Results( f_output );
}