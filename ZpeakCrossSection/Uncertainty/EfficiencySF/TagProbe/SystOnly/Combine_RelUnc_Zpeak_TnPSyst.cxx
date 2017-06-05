#include "Combine_RelUnc_ZPeak_TnPSyst.h"

void Combine_RelUnc_Zpeak_TnPSyst()
{
	// vector< TString > vec_Type = {"sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"};

	CombineTool *tool = new CombineTool();
	tool->Print_All();
	TFile *f_output = TFile::Open("ROOTFile_RelUnc_Zpeak_TnP_FromSystSource.root", "RECREATE");
	tool->Save( f_output );
}