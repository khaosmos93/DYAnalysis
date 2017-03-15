#include "ZpeakCrossSectionTool.h"
void ZpeakCrossSection(TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );
	
	ZpeakCrossSectionTool *tool = new ZpeakCrossSectionTool(HLTname);

	tool->ObtainYield(kTRUE);

	// -- calculate acceptance & efficiency -- //
	// tool->Calc_AccEff();
	// tool->ApplyAccEffCorrection();
	// tool->EfficiencyScaleFactor();
	// tool->CalcXSec();
	// tool->Calc_SysUnc_BkgEst();

	
	// -- insert acceptance & efficinecy -- // 
	Double_t Acc = 0.453734;
	Double_t Eff = 0.914200;
	Double_t EffSF_HLTv4p2 = 0.958053;
	Double_t EffSF_HLTv4p3 = 0.964876;

	tool->ApplyAccEffCorrection(Acc, Eff);
	tool->EfficiencyScaleFactor(EffSF_HLTv4p2, EffSF_HLTv4p3);
	tool->CalcXSec();
	tool->FpoF_CalcXSec();
	tool->FpoF_CalcXsec_aMCNLO();
	tool->Calc_SysUnc_BkgEst();

	TFile *f_output = TFile::Open("ROOTFile_Output_ZpeakCrossSection.root", "RECREATE");
	tool->Save_Results( f_output );

	f_output->Close();
}