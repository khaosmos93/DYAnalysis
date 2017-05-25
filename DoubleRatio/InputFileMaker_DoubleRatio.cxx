#include "InputFileMaker_DoubleRatio.h"

void InputFileMaker_DoubleRatio()
{
	InputMaker *maker = new InputMaker();
	// -- muon channel -- //
	maker->Get_Histograms_MM();
	maker->MakePlots_CrossCheck();

	TFile *f_ouptut_MM = TFile::Open("ROOTFile_Input_DoubleRatio_MM.root", "RECREATE");
	maker->Save( f_ouptut_MM );

	// -- combination -- //
	maker->Get_Histograms_LL();
	maker->MakePlots_CrossCheck();

	TFile *f_ouptut_LL = TFile::Open("ROOTFile_Input_DoubleRatio_LL.root", "RECREATE");
	maker->Save( f_ouptut_LL );
}