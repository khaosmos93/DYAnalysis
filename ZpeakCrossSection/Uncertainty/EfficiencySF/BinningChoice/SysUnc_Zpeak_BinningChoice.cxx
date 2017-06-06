#include "ZpeakSysUncTool_BinningChoice.h"
#include <Include/PlotTools.h>

void SysUnc_Zpeak_BinningChoice(TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );
	
	//////////////////////////
	// -- Coarse Binning -- //
	//////////////////////////
	ZpeakSysUncTool_BinningChoice *tool_CoarseBinning = new ZpeakSysUncTool_BinningChoice(HLTname);
	// tool_CoarseBinning->Set_nEvent_Test( 1000000 );
	tool_CoarseBinning->SetIsDataDriven( kTRUE );

	TString ROOTFile_TnPEff_CV = "ROOTFile_TagProbeEfficiency_76X_v20160502.root";
	TString ROOTFile_TnPEff_CoarseBinning = "ROOTFile_TagProbeEff_SysUnc_BinningChoice_CoarseBinning.root";
	tool_CoarseBinning->SetROOTFiles_TnPEff(ROOTFile_TnPEff_CV, ROOTFile_TnPEff_CoarseBinning);

	const Int_t nPtBin1 = 4;
	Double_t PtBinEdges1[nPtBin1+1] = {10, 22, 40, 70, 250};

	const Int_t nEtaBin1 = 3;
	Double_t EtaBinEdges1[nEtaBin1+1] = {-2.4, -0.9, 0.9, 2.4};

	tool_CoarseBinning->SetSysUncTool("CoarseBinning", nPtBin1, PtBinEdges1, nEtaBin1, EtaBinEdges1);
	tool_CoarseBinning->Calc_EffSF();
	tool_CoarseBinning->Calc_ZpeakXsec_All();

	Double_t RelSysUnc_CoarseBinning = tool_CoarseBinning->RelSysUnc;


	///////////////////////
	// -- Fine binning - //
	///////////////////////
	ZpeakSysUncTool_BinningChoice *tool_FineBinning = new ZpeakSysUncTool_BinningChoice(HLTname);
	// tool_FineBinning->Set_nEvent_Test( 1000000 );
	tool_FineBinning->SetIsDataDriven( kTRUE );

	TString ROOTFile_TnPEff_FineBinning = "ROOTFile_TagProbeEff_SysUnc_BinningChoice_FineBinning.root";
	tool_FineBinning->SetROOTFiles_TnPEff(ROOTFile_TnPEff_CV, ROOTFile_TnPEff_FineBinning);

	const Int_t nPtBin2 = 8;
	Double_t PtBinEdges2[nPtBin2+1] = {10, 16, 22, 31, 40, 55, 70, 85, 250};

	const Int_t nEtaBin2 = 10;
	Double_t EtaBinEdges2[nEtaBin2+1] = {-2.4, -2.1, -1.2, -0.9, -0.3, 0, 0.3, 0.9, 1.2, 2.1, 2.4};

	tool_FineBinning->SetSysUncTool("FineBinning", nPtBin2, PtBinEdges2, nEtaBin2, EtaBinEdges2);
	tool_FineBinning->Calc_EffSF();
	tool_FineBinning->Calc_ZpeakXsec_All();

	Double_t RelSysUnc_FineBinning = tool_FineBinning->RelSysUnc;

	Double_t RelSysUnc_Binning = 0;
	if( RelSysUnc_CoarseBinning > RelSysUnc_FineBinning ) RelSysUnc_Binning = RelSysUnc_CoarseBinning;
	else RelSysUnc_Binning = RelSysUnc_FineBinning;

	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName = AnalyzerPath+"/ZpeakCrossSection/Uncertainty/EfficiencySF/BinningChoice/Summary_Unc_Zpeak_BinningChoice.txt";
	FILE *fp = fopen(FileName.Data(), "w");
	fprintf(fp, "Rel.Unc. from coarse binning: %9.5lf\n", RelSysUnc_CoarseBinning);
	fprintf(fp, "Rel.Unc. from fine binning: %9.5lf\n", RelSysUnc_FineBinning);
	fprintf(fp, "Rel.Unc. from binning (take larger one): %9.5lf\n", RelSysUnc_Binning);

	// cout << "======================================================================" <<endl;
	// printf("[Relative Systematic uncertainty from binning choice (total): %.5lf (%%)]\n", RelSysUnc_Binning*100);
	// cout << "====================================================================\n" <<endl;

	TFile *f_output = TFile::Open("ROOTFIle_RelUnc_Zpeak_BinningChoice.root", "RECREATE");
	SaveAsHist_OneContent( RelSysUnc_CoarseBinning, "h_RelUnc_Coarse", f_output );
	SaveAsHist_OneContent( RelSysUnc_FineBinning, "h_RelUnc_Fine", f_output );
	SaveAsHist_OneContent( RelSysUnc_Binning, "h_RelUnc", f_output );
}