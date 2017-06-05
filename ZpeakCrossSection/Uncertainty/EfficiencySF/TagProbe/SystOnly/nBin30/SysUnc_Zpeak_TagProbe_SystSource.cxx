#include <ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/ZpeakSysUncTool_TagProbe.h>
// #include "ZpeakCrossSectionTool.h"
// #include "ZpeakSysUncTool_BinningChoice.h"

void SysUnc_Zpeak_TagProbe_SystSource(TString SystType, TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );

	////////////////////////////
	// -- Tag&Probe method -- //
	////////////////////////////
	ZpeakSysUncTool_TagProbe *tool_TagProbe = new ZpeakSysUncTool_TagProbe(HLTname);
	// tool_TagProbe->Set_nEvent_Test( 3000000 );

	TString ROOTFile_TnPEff_CV = "ROOTFile_TagProbeEfficiency_76X_v20160502.root";
	tool_TagProbe->InputFileName_CV( ROOTFile_TnPEff_CV );
	tool_TagProbe->SetupEffMaps_SystOnly( SystType );

	tool_TagProbe->SetIsDataDriven( kTRUE );
	tool_TagProbe->Calc_EffSF_AllMap();
	tool_TagProbe->Calc_ZpeakXsec_AllMap();
	tool_TagProbe->Calc_SysUnc();

	TFile *f_output = TFile::Open("ROOTFile_Output_SysUnc_Zpeak_TagProbe_SystSource.root", "RECREATE");
	tool_TagProbe->Save_Results( f_output );

	f_output->Close();
}