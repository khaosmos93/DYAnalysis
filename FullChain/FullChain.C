#include <Include/DiffXsecTools.h>

void FullChain()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );
	
	DiffXsecTools *tool = new DiffXsecTools();
	tool->MakeSignalMCHistograms();

	Bool_t isDataDriven = kTRUE;
	tool->GetYieldHistograms(isDataDriven);
	tool->SaveCanvas_RecoLevel_Data_vs_recoMC();

	tool->UnfoldingCorrection();
	tool->SaveCanvas_UnfoldingCorrection();

	tool->AccEffCorrection();
	tool->SaveCanvas_AccEffCorrection();

	tool->EfficiencyScaleFactor();
	tool->SaveCanvas_EfficiencyCorrection();

	tool->FSRCorrection();
	tool->SaveCanvas_FSRCorrection();

	tool->CalcXsec();
	tool->SaveCanvas_CalcXsec();

	tool->SaveHistograms();

	tool->FpoF_AllAnalysisProcedure();
}







