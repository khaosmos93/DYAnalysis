#include <Include/PlotTools.h>

void Printout_MCBkg()
{
	TString FileName = "ROOTFile_Histograms_Bkg.root";
	TH1D* h_ZZ_HLTv4p2 = Get_Hist( FileName, "h_ZZ_HLTv4p2" );
	Print_Histogram( h_ZZ_HLTv4p2 );
}