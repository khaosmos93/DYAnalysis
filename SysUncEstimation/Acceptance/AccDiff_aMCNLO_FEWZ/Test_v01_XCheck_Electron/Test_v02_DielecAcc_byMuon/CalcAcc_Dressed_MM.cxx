#include "CalcAcc_Dressed.h"

void CalcAcc_Dressed_MM()
{
	AccTool *tool = new AccTool( "Muon_ElecAcc" );

	TFile *f_output = TFile::Open("ROOTFile_Acc_Dressed_MM_ElecAcc.root", "RECREATE");
	
	tool->Analyze();
	tool->Save( f_output );
	f_output->Close();
}