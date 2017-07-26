#include "CalcAcc_Dressed.h"

void CalcAcc_Dressed_EE()
{
	AccTool *tool = new AccTool( "Electron" );

	TFile *f_output = TFile::Open("ROOTFile_Acc_Dressed_EE.root", "RECREATE");
	// tool->Set_nEvent_Test( 1000000 );

	tool->Set_Output( f_output );	
	tool->Analyze();
	f_output->Close();
}