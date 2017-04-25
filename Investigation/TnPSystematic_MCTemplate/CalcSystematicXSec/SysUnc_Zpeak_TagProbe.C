#include "ZpeakSysUncTool_TagProbe.h"
// #include "ZpeakSysUncTool_BinningChoice.h"

void SysUnc_Zpeak_TagProbe(TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );

	////////////////////////////
	// -- Tag&Probe method -- //
	////////////////////////////
	ZpeakSysUncTool_TagProbe *tool_TagProbe = new ZpeakSysUncTool_TagProbe(HLTname);

	TString ROOTFile_TnPEff_CV = "ROOTFile_TagProbeEfficiency_76X_v20160502.root";
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString ROOTFile_TnPEff_SysUnc = AnalyzerPath+"/Investigation/TnPSystematic_MCTemplate/CalcSystematicTnP/ROOTFile_SysUnc_TagProbeMethod.root";
	tool_TagProbe->SetupROOTFile_TagProbeEff(ROOTFile_TnPEff_CV, ROOTFile_TnPEff_SysUnc);

	tool_TagProbe->SetIsDataDriven( kTRUE );
	tool_TagProbe->Calc_EffSF_AllMap();
	tool_TagProbe->Calc_ZpeakXsec_AllMap();
	tool_TagProbe->Calc_SysUnc();

	TFile *f_output = TFile::Open("ROOTFile_Output_SysUnc_Zpeak_TagProbe.root", "RECREATE");
	tool_TagProbe->Save_Results( f_output );

	f_output->Close();
}