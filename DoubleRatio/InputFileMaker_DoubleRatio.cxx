#include "InputFileMaker_DoubleRatio.h"

void InputFileMaker_DoubleRatio()
{
	InputMaker *maker = new InputMaker();
	maker->Get_Histograms();

	TFile *f_ouptut = TFile::Open("ROOTFile_Input_DoubleRatio.root", "RECREATE");
	maker->Save( f_ouptut );
}