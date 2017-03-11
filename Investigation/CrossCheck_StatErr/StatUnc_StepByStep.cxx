#include "StatUnc_StepByStep.h"

void StatUnc_StepByStep()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );
	
	TFile *f_IO = TFile::Open("ROOTFile_CentralValue_and_Replicas.root", "RECREATE");

	Tool_StatUnc_StepByStep *tool = new Tool_StatUnc_StepByStep();
	tool->MakeAndSave_Histogram( f_IO );
	// tool->Load_Histogram_StepByStep( f_IO ); // -- one f_IO exists, you can just turn on this without making them from scratch -- //
	tool->Calc_RelStatUnc_StepByStep();
	tool->Make_XCheckPlot();

	TFile *f_output = TFile::Open("ROOTFile_Output_StatUnc_StepByStep.root", "RECREATE");
	tool->Save_RelStatUnc( f_output );

	f_IO->Close();
	f_output->Close();
}