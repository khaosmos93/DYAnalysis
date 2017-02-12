#include "SysUncTool_CombineAll.h"


class MyTool : public SysUncTool_Combine
{
public:
	MyTool(TString version) : SysUncTool_Combine(version)
	{

	}

	void SetupSources()
	{
		Sources.push_back( "Acc."); FileNames.push_back("ROOTFile_SysUnc_Acceptance.root"); HistNames.push_back("h_RelSysUnc_Tot_Percent"); Colors.push_back( kGreen-9 );  Markers.push_back( 24 );
		Sources.push_back( "Eff.SF."); FileNames.push_back("ROOTFile_SysUnc_EffSF.root"); HistNames.push_back("h_RelSysUnc_Tot"); Colors.push_back( kBlue ); Markers.push_back( 25 );
		Sources.push_back( "Det.Res"); FileNames.push_back("ROOTFile_SysUnc_DetRes.root"); HistNames.push_back("h_RelSysUnc_DetRes_Percent"); Colors.push_back( kViolet ); Markers.push_back( 26 );
		Sources.push_back( "Bkg.Est."); FileNames.push_back("ROOTFile_SysUnc_BkgEst.root"); HistNames.push_back("h_SysUnc_BkgEst_Percent"); Colors.push_back( kGreen+1 );  Markers.push_back( 27 );
		Sources.push_back( "FSR"); FileNames.push_back("ROOTFile_SysUnc_FSR.root"); HistNames.push_back("h_SysUnc_Tot_Percent"); Colors.push_back( kBlue-9 ); Markers.push_back( 28 );

		LegendName.push_back( "Acceptance" );
		LegendName.push_back( "Efficiency SF" );
		LegendName.push_back( "Detector Res." );
		LegendName.push_back( "Background" );
		LegendName.push_back( "FSR" );
	}
};

void SysUnc_Combination(TString version)
{
	MyTool *tool = new MyTool(version);

	tool->MakeHistogram_LumiUnc();
	tool->SetupSources();
	tool->GetHistograms();
	// tool->ApplySmooth();
	tool->MakeHistogram_TotalUnc();
	tool->MakeCanvas();
	tool->DrawCanvas_DiffXsec_All( "FullAcc" );
	// tool->DrawDiffXsec_WithAllUnc("FEWZ");
	// tool->DrawDiffXsec_WithAllUnc("aMCNLO");
	// tool->DrawDiffXsec_WithAllUnc_aMCNLO();
	// tool->Comparison_FEWZ_vs_aMCNLO();
	tool->SaveResults();
	tool->PrintAll();
	tool->Print_SysUnc_texFormat();
	tool->PrintFinalResults();
	tool->PrintFinalResults_texFormat();
}