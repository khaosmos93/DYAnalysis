#include <Include/PlotTools.h>

void ZpeakXSec_WithAllUnc()
{
	TString FileName_CV = "ROOTFile_Output_ZpeakCrossSection.root";
	TString FileName_Stat = "./Uncertainty/ROOTFile_StatUnc_Zpeak.root";
	TString FileName_EffSF = "./Uncertainty/EfficiencySF/ROOTFile_SysUnc_Zpeak_EffSF.root";
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_Acc = ROOTFilePath+"/ROOTFile_SysUnc_Acceptance.root";

	Double_t XSec = GetContent_OneBinHist( FileName_CV, "h_XSec" );
	Double_t RelUnc_Stat = GetContent_OneBinHist( FileName_Stat, "h_RelUnc" );
	Double_t RelUnc_Bkg = GetContent_OneBinHist( FileName_CV, "h_RelSystUnc_BkgEst" );
	Double_t RelUnc_EffSF = GetContent_OneBinHist( FileName_EffSF, "h_RelUnc_Tot" );
	Double_t RelUnc_TotExp = sqrt( RelUnc_Bkg*RelUnc_Bkg + RelUnc_EffSF*RelUnc_EffSF );

	// Double_t RelUnc_Acc = 1.57197 / 100.0; // -- should be fixed -- //
	Double_t RelUnc_Acc = GetContent_OneBinHist( FileName_Acc, "h_RelUnc_Zpeak_Tot" );
	Double_t RelUnc_TotSyst = sqrt( RelUnc_Acc*RelUnc_Acc + RelUnc_TotExp*RelUnc_TotExp ); 
	Double_t RelUnc_Lumi = GetContent_OneBinHist( FileName_CV, "h_RelLumiError" );

	Double_t FpoF_XSec = GetContent_OneBinHist( FileName_CV, "h_FpoF_XSec" );
	Double_t RelUnc_FpoF_Stat = GetContent_OneBinHist( FileName_Stat, "h_FpoF_RelUnc" ); // -- actually it is same with RelUnc_Stat -- //

	FILE *fp = fopen("Summary_Zpeak_WithAllUnc.txt", "w");
	fprintf(fp, "[4pi]\n");
	fprintf(fp, "Cross section: %.0lf (%.3lf) [pb]\n", XSec, XSec);
	fprintf(fp, "\tStat.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_Stat, XSec*RelUnc_Stat, RelUnc_Stat*100);
	fprintf(fp, "\t(TotExp + Theo.): %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_TotSyst, XSec*RelUnc_TotSyst, RelUnc_TotSyst*100);
	fprintf(fp, "\t\tTotExp.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_TotExp, XSec*RelUnc_TotExp, RelUnc_TotExp*100);
	fprintf(fp, "\t\t\tBkg.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_Bkg, XSec*RelUnc_Bkg, RelUnc_Bkg*100);
	fprintf(fp, "\t\t\tEffSF.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_EffSF, XSec*RelUnc_EffSF, RelUnc_EffSF*100);
	fprintf(fp, "\t\tAcc.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_Acc, XSec*RelUnc_Acc, RelUnc_Acc*100);
	
	fprintf(fp, "\tLumi.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", XSec*RelUnc_Lumi, XSec*RelUnc_Lumi, RelUnc_Lumi*100);

	fprintf(fp, "\n[fiducial]\n");
	fprintf(fp, "Cross section: %.0lf (%.3lf) [pb]\n", FpoF_XSec, FpoF_XSec);
	fprintf(fp, "\tStat.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", FpoF_XSec*RelUnc_FpoF_Stat, FpoF_XSec*RelUnc_FpoF_Stat, RelUnc_FpoF_Stat*100);
	fprintf(fp, "\tTotExp.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", FpoF_XSec*RelUnc_TotExp, FpoF_XSec*RelUnc_TotExp, RelUnc_TotExp*100);
	fprintf(fp, "\t\tBkg.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", FpoF_XSec*RelUnc_Bkg, FpoF_XSec*RelUnc_Bkg, RelUnc_Bkg*100);
	fprintf(fp, "\t\tEffSF.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", FpoF_XSec*RelUnc_EffSF, FpoF_XSec*RelUnc_EffSF, RelUnc_EffSF*100);
	fprintf(fp, "\tLumi.: %.0lf (%.3lf) [pb] (%.3lf %%)\n", FpoF_XSec*RelUnc_Lumi, FpoF_XSec*RelUnc_Lumi, RelUnc_Lumi*100);
}