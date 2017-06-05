#include "Combine_RelUnc_Zpeak_TnPSyst.h"

void Combine_RelUnc_Zpeak_TnPSyst()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );
	
	// vector< TString > vec_Type = {"sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"};

	CombineTool *tool = new CombineTool();
	tool->Print_All();
	TFile *f_output = TFile::Open("ROOTFile_RelUnc_Zpeak_TnP_FromSystSource.root", "RECREATE");
	tool->Save( f_output );
}