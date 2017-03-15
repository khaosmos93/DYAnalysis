#include "../SysUncTool_BkgEst.h"
#include <Include/PlotTools.h>

void PrintOut_StatUnc_MCBkg()
{
	SysUncTool_BkgEst *tool = new SysUncTool_BkgEst();
	Print_Histogram( tool->Hists_WZ->h_nEvent );

	TFile *f_MC = TFile::Open(tool->FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root"); f_MC->cd();
	h_bkg = (TH1D*)f_MC->Get("h_mass_OS_Norm_WZ")->Clone();
	h_bkg = (TH1D*)h_bkg->Rebin(nMassBin, h_bkg->GetName(), MassBinEdges);

	Print_Histogram( h_bkg );
}