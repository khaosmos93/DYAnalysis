#include <Include/PlotTools.h>

void Combine_Print()
{
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString MiddlePath = "/ZpeakCrossSection/Uncertainty/EfficiencySF";
	TString FileName_Stat = AnalyzerPath+MiddlePath+"/TagProbe/StatOnly/ROOTFile_Output_SysUnc_Zpeak_TagProbe_StatSource.root";
	TString FileName_TnP =  AnalyzerPath+MiddlePath+"/TagProbe/SystOnly/ROOTFile_RelUnc_Zpeak_TnP_FromSystSource.root";
	TString FileName_Bin = AnalyzerPath+MiddlePath+"/BinningChoice/ROOTFile_RelUnc_Zpeak_BinningChoice.root";

	Double_t RelUnc_Stat = GetContent_OneBinHist( FileName_Stat, "h_RelUnc" );
	Double_t RelUnc_TnP = GetContent_OneBinHist( FileName_TnP, "h_RelUnc_Tot");
	Double_t RelUnc_Bin = GetContent_OneBinHist( FileName_Bin, "h_RelUnc");
	Double_t RelUnc_Tot = sqrt(
		RelUnc_Stat	* RelUnc_Stat +
		RelUnc_TnP	* RelUnc_TnP +
		RelUnc_Bin	* RelUnc_Bin );

	FILE *fp = fopen("Summary_RelUnc_EffSF.txt", "w");
	fprintf( fp, "Stat.Unc: %9.5lf\n", RelUnc_Stat);
	fprintf( fp, "TnP. Unc: %9.5lf\n", RelUnc_TnP);
	fprintf( fp, "Bin. Unc: %9.5lf\n", RelUnc_Bin);
	fprintf( fp, "Total. unc: %9.5lf\n", RelUnc_Tot);

	TFile *f_output = TFile::Open("ROOTFile_SysUnc_Zpeak_EffSF.root", "RECREATE");
	SaveAsHist_OneContent( RelUnc_Stat, "h_RelUnc_Stat", f_output );
	SaveAsHist_OneContent( RelUnc_TnP, "h_RelUnc_TnP", f_output );
	SaveAsHist_OneContent( RelUnc_Bin, "h_RelUnc_Bin", f_output );
	SaveAsHist_OneContent( RelUnc_Tot, "h_RelUnc_Tot", f_output );
}