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

		// for(Int_t i_smeared=0; i_smeared<nMap; i_smeared++)
		// 	testTool->Print_BkgSub_Unfolded_CV_Smeared(15, i_smeared);

		// testTool->Validate_MatrixInversion();
		// testTool->Test_InvertMDiagAs1();
		// testTool->Print_Compare_nEvents_CV_Smeared(15);
		// testTool->Print_Compare_nEvents_CV_Smeared(16);
		// testTool->Print_Compare_nEvents_CV_Smeared(17);
		// testTool->Print_Compare_nEvents_CV_Smeared(18);
		// testTool->Print_Compare_nEvents_CV_Smeared(19);
		// testTool->Print_Compare_nEvents_CV_Smeared(20);
		// testTool->DrawCanvas_ContributionFromNextBin();

		testTool->Print_CompContribution_Diag_OtherTerm( 15 );
	}
}