#include "CompPlots.h"

void CompPlots()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TString BasePath = gSystem->Getenv("KP_ROOTFILE_PATH");

	Tool_CompPlots *tool = new Tool_CompPlots();

	TH1D* h_before_ttbar = Get_Hist("../../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "ttbar");
	TH1D* h_after_ttbar = Get_Hist("../emu_final.root", "ttbar");

	tool->Setup( h_before_ttbar, h_after_ttbar );
	tool->DrawCanvas();	
}