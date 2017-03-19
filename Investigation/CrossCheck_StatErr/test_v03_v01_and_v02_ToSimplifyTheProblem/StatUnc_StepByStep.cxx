#include "StatUnc_StepByStep.h"

void StatUnc_StepByStep()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );
	

	Bool_t Do_Calc = kFALSE;
	Bool_t Do_ReCalcAll = kFALSE;
	Bool_t Do_Test = kTRUE;

	TFile *f_IO;
	if( Do_ReCalcAll )
		f_IO = TFile::Open("ROOTFile_CentralValue_and_Replicas.root", "RECREATE");
	else
		f_IO = TFile::Open("ROOTFile_CentralValue_and_Replicas.root", "READ");

	if( Do_Calc )
	{
		Tool_StatUnc_StepByStep *tool = new Tool_StatUnc_StepByStep();
		
		if( Do_ReCalcAll )
			tool->MakeAndSave_Histogram( f_IO );
		else
			tool->Load_Histogram_StepByStep( f_IO ); // -- once f_IO exists, you can just turn on this without making them from scratch -- //
		
		tool->Calc_RelStatUnc_StepByStep();
		tool->Make_XCheckPlot();

		TFile *f_output = TFile::Open("ROOTFile_Output_StatUnc_StepByStep.root", "RECREATE");
		tool->Save_RelStatUnc( f_output );
		f_IO->Close();
		f_output->Close();
	}

	if( Do_Test )
	{
		// -- test & x-check -- //
		TestTool_StatUnc_StepByStep *testTool = new TestTool_StatUnc_StepByStep();
		testTool->Load_Histogram_StepByStep( f_IO );

		testTool->Validate_MatrixInversion();
	}
}