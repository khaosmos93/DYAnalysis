#include "SysUncTool_CombineAll.h"


class MyTool : public SysUncTool_Combine
{
public:
	MyTool(TString version) : SysUncTool_Combine(version)
	{

	}

	void SetupSources()
	{
		Sources.push_back( "Eff.SF."); FileNames.push_back("ROOTFile_SysUnc_EffSF.root"); HistNames.push_back("h_RelSysUnc_Tot"); Colors.push_back( kBlue ); Markers.push_back( 25 );
		Sources.push_back( "Det.Res"); FileNames.push_back("ROOTFile_SysUnc_DetRes.root"); HistNames.push_back("h_RelSysUnc_DetRes_Percent"); Colors.push_back( kViolet ); Markers.push_back( 26 );
		Sources.push_back( "Bkg.Est."); FileNames.push_back("ROOTFile_SysUnc_BkgEst.root"); HistNames.push_back("h_SysUnc_BkgEst_Percent"); Colors.push_back( kGreen+1 ); Markers.push_back( 27 );

		// LegendName.push_back( "Acceptance" );
		LegendName.push_back( "Efficiency SF" );
		LegendName.push_back( "Detector Res." );
		LegendName.push_back( "Background" );
		// LegendName.push_back( "FSR" );
	}
};

void FpoF_SysUnc_Combination(TString version)
{
	MyTool *tool = new MyTool(version);
	Bool_t FpoF = kTRUE;

	tool->MakeHistogram_LumiUnc();
	tool->SetupSources();
	tool->GetHistograms();
	tool->ApplySmooth();
	tool->MakeHistogram_TotalUnc();
	tool->MakeCanvas( FpoF );
	tool->DrawCanvas_DiffXsec_All( "FpoF" );
	// tool->DrawDiffXsec_WithAllUnc("FpoF_Powheg");
	tool->SaveResults( FpoF );

	tool->Print_FpoF_SysUnc_texFormat();
	tool->FpoF_PrintFinalResults_texFormat();

	// tool->PrintAll();
	// tool->PrintFinalResults();
}