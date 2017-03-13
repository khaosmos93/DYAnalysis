#include "CompPlots.h"

void CompPlots()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TString BasePath = gSystem->Getenv("KP_ROOTFILE_PATH");

	TH1D* h_before_yield = Get_Hist("../Results_Before/ROOTFile_YieldHistogram.root", "h_yield_OS_DataDrivenBkg");
	TH1D* h_after_yield = Get_Hist(BasePath+"/ROOTFile_YieldHistogram.root", "h_yield_OS_DataDrivenBkg");

	Tool_CompPlots *tool = new Tool_CompPlots();

	tool->Setup( h_before_yield, h_after_yield );
	tool->DrawCanvas();	

	TH1D* h_before_DiffXSec = Get_Hist("../Results_Before/ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr");
	TH1D* h_after_DiffXSec = Get_Hist(BasePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr");

	tool->Setup( h_before_DiffXSec, h_after_DiffXSec );
	tool->DrawCanvas();	

	// -- emu method -- //
	TH1D* h_before_ttbar = Get_Hist("../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "ttbar");
	TH1D* h_after_ttbar = Get_Hist(BasePath+"/ROOTFile_Bkg_DataDrivenMethod.root", "ttbar");

	tool->Setup( h_before_ttbar, h_after_ttbar );
	tool->DrawCanvas();	

	TH1D* h_before_tW = Get_Hist("../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "tW");
	TH1D* h_after_tW = Get_Hist(BasePath+"/ROOTFile_Bkg_DataDrivenMethod.root", "tW");

	tool->Setup( h_before_tW, h_after_tW );
	tool->DrawCanvas();

	TH1D* h_before_DYtautau = Get_Hist("../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "DYtautau");
	TH1D* h_after_DYtautau = Get_Hist(BasePath+"/ROOTFile_Bkg_DataDrivenMethod.root", "DYtautau");

	tool->Setup( h_before_DYtautau, h_after_DYtautau );
	tool->DrawCanvas();

	TH1D* h_before_WW = Get_Hist("../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "WW");
	TH1D* h_after_WW = Get_Hist(BasePath+"/ROOTFile_Bkg_DataDrivenMethod.root", "WW");

	tool->Setup( h_before_WW, h_after_WW );
	tool->DrawCanvas();

	// -- fake rate method -- //
	TH1D* h_before_dijet = Get_Hist("../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "dijet");
	TH1D* h_after_dijet = Get_Hist(BasePath+"/ROOTFile_Bkg_DataDrivenMethod.root", "dijet");

	tool->Setup( h_before_dijet, h_after_dijet );
	tool->DrawCanvas();

	TH1D* h_before_wjets = Get_Hist("../Results_Before/ROOTFile_Bkg_DataDrivenMethod.root", "wjets");
	TH1D* h_after_wjets = Get_Hist(BasePath+"/ROOTFile_Bkg_DataDrivenMethod.root", "wjets");

	Print_Histogram( h_before_wjets );
	Print_Histogram( h_after_wjets );

	tool->Setup( h_before_wjets, h_after_wjets );
	tool->DrawCanvas();


	// -- MC backgrounds -- //
	const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};



	TH1D* h_before_WZ = Get_Hist("../Results_Before/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root", "h_mass_OS_Norm_WZ");
	h_before_WZ = (TH1D*)h_before_WZ->Rebin(nMassBin, h_before_WZ->GetName(), MassBinEdges);

	TH1D* h_after_WZ = Get_Hist(BasePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root", "h_mass_OS_Norm_WZ");
	h_after_WZ = (TH1D*)h_after_WZ->Rebin(nMassBin, h_after_WZ->GetName(), MassBinEdges);

	tool->Setup( h_before_WZ, h_after_WZ );
	tool->DrawCanvas();

	Print_Histogram( h_before_WZ );
	Print_Histogram( h_after_WZ );


	TH1D* h_before_ZZ = Get_Hist("../Results_Before/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root", "h_mass_OS_Norm_ZZ");
	h_before_ZZ = (TH1D*)h_before_ZZ->Rebin(nMassBin, h_before_ZZ->GetName(), MassBinEdges);

	TH1D* h_after_ZZ = Get_Hist(BasePath+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root", "h_mass_OS_Norm_ZZ");
	h_after_ZZ = (TH1D*)h_after_ZZ->Rebin(nMassBin, h_after_ZZ->GetName(), MassBinEdges);

	tool->Setup( h_before_ZZ, h_after_ZZ );
	tool->DrawCanvas();

	TH1D* h_before_RelSystUnc_BkgSub = Get_Hist("../Results_Before/ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Bkg.Est._Percent");
	TH1D* h_after_RelSystUnc_BkgSub = Get_Hist(BasePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Bkg.Est._Percent");

	h_before_RelSystUnc_BkgSub->SetMinimum(1e-3);
	h_after_RelSystUnc_BkgSub->SetMinimum(1e-3);

	tool->Setup( h_before_RelSystUnc_BkgSub, h_after_RelSystUnc_BkgSub );
	tool->DrawCanvas();

}