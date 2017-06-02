#include "MakeROOTFile_Syst_TnPEffs.h"

void MakeROOTFile_Syst_TnPEffs()
{
	TString BaseROOTFileName = "ROOTFile_ExtractEfficiency_CalcSysUnc_2D_";
	vector< TString > vec_Type = {"sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"};

	TFile *f_output = TFile::Open("ROOTFile_Syst_TnPEffs.root", "RECREATE");

	for( const auto &Type : vec_Type)
	{
		CalcSystTool *tool = new CalcSystTool(Type);
		tool->InputROOTFileName( "./Inputs/"+BaseROOTFileName+Type+".root");
		tool->OutputROOTFile( f_output );
		tool->Save_Syst_2DPlot();
	}

	f_output->Close();
}