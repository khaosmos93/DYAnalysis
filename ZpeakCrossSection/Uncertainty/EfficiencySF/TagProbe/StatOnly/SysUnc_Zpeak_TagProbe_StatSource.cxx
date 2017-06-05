#include <ZpeakCrossSection/Uncertainty/EfficiencySF/ZpeakSysUncTool_TagProbe.h>
// #include "ZpeakSysUncTool_BinningChoice.h"

void SysUnc_Zpeak_TagProbe_StatSource(TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );

	////////////////////////////
	// -- Tag&Probe method -- //
	////////////////////////////
	ZpeakSysUncTool_TagProbe *tool_TagProbe = new ZpeakSysUncTool_TagProbe(HLTname);

	TString ROOTFile_TnPEff_CV = "ROOTFile_TagProbeEfficiency_76X_v20160502.root";
	tool_TagProbe->InputFileName_CV( ROOTFile_TnPEff_CV );
	tool_TagProbe->SetupEffMaps_StatOnly();

	tool_TagProbe->SetIsDataDriven( kTRUE );
	tool_TagProbe->Calc_EffSF_AllMap();
	tool_TagProbe->Calc_ZpeakXsec_AllMap();
	tool_TagProbe->Calc_SysUnc();

	TFile *f_output = TFile::Open("ROOTFile_Output_SysUnc_Zpeak_TagProbe_StatSource.root", "RECREATE");
	tool_TagProbe->Save_Results( f_output );

	f_output->Close();
}