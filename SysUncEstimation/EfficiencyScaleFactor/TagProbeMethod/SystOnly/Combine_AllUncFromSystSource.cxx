#include "Combine_AllUncFromSystSource.h"

void Combine_AllUncFromSystSource()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );
	
	TFile *f_output = TFile::Open("ROOTFile_RelUnc_TnP_FromSystSource.root", "RECREATE");
	CombineTool *tool = new CombineTool();
	tool->OutputFile(f_output);
	tool->Set_Histograms();
	tool->Draw_SummaryPlot();
	tool->Save();
}