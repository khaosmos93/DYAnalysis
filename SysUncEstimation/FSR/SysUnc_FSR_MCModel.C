#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <stdio.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TPaletteAxis.h>
#include <vector>
#include <TPad.h>
#include <TList.h>
#include <vector>

#include <Unfolding/src/RooUnfoldResponse.h>
#include <Unfolding/src/RooUnfoldBayes.h>
#include <Unfolding/src/RooUnfoldInvert.h>

#include <DYAnalysis_76X/CommonCodes/DYAnalyzer_v01.h>
#include <DYAnalysis_76X/CommonCodes/DiffXsecTools_v01.h>
#include <DYAnalysis_76X/CommonCodes/MyCanvas.C>

class MyDiffXsecTool : public DiffXsecTools
{
public:
	MyDiffXsecTool(TString version, TString Ver_CMSSW) : DiffXsecTools(version, Ver_CMSSW)
	{

	}

	void FSRCorrection(TH1D* h_preFSR, RooUnfoldResponse *UnfoldRes_FSR)
	{
		//////////////////////////
		// -- FSR Correction -- //
		//////////////////////////
		// f_FSR = new TFile(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
		// f_FSR = TFile::Open(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
		h_Truth_preFSR = (TH1D*)h_preFSR->Clone();

		// RooUnfoldResponse *UnfoldRes_FSR = (RooUnfoldResponse*)f_FSR->Get("UnfoldRes")->Clone();

		// delete f_FSR;

		// RooUnfoldBayes *UnfoldBayes_FSR = new RooUnfoldBayes(UnfoldRes_FSR, h_yield_EffCorr, 4);
		// h_yield_FSRCorr = (TH1D*)UnfoldBayes_FSR->Hreco();

		RooUnfoldInvert *UnfoldInvert_FSR = new RooUnfoldInvert(UnfoldRes_FSR, h_yield_EffCorr); // -- use inverting method after following recommendation by stat.committee -- //
		h_yield_FSRCorr = (TH1D*)UnfoldInvert_FSR->Hreco();
		
	}
};

TH1D* CalcDiffXsec_GivenFSRCorr(TString version, TH1D* h_Truth_RooUnfold, RooUnfoldResponse *UnfoldRes_FSR);

void SysUnc_FSR_MCModel(TString version)
{
	// -- Get photos information -- //
	TFile *f_input = TFile::Open("ROOTFile_FSRCorrections_DressedLepton_PHOTOS.root");
	TH1D* h_Truth_RooUnfold = (TH1D*)f_input->Get("h_Truth_RooUnfold")->Clone();
	RooUnfoldResponse* UnfoldRes = (RooUnfoldResponse*)f_input->Get("UnfoldRes")->Clone();

	// -- Calc. diff.x-sec using FSR correction from photos -- //
	TH1D* h_DiffXsec_AltMC = CalcDiffXsec_GivenFSRCorr(version, h_Truth_RooUnfold, UnfoldRes);
	h_DiffXsec_AltMC->SetName("h_DiffXsec_AltMC");

	TString FileLocation = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/" + version;

	TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
	TH1D* h_cv = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr");

	MyCanvas *myc = new MyCanvas("c_DiffXsec_FSRCorr_CentralMC_vs_AltMC", "Dimuon Mass [GeV]", "d#sigma/dM [pb/GeV]");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetRatioRange(0.9, 1.1);
	myc->CanvasWithHistogramsRatioPlot( h_DiffXsec_AltMC, h_cv, "FSR Corr. from PHOTOS", "Central value (pythia8)", "PHOTOS/C.V.");
	myc->PrintCanvas();

	cout << "===================================================" << endl;
	cout << "Relative uncertainty from FSR correction (MC model)" << endl;
	cout << "===================================================" << endl;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //
	
	TH1D* h_RelSysUnc = new TH1D("h_RelSysUnc", "", nMassBin, MassBinEdges);
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t CV = h_cv->GetBinContent(i_bin);
		Double_t altV = h_DiffXsec_AltMC->GetBinContent(i_bin);

		Double_t RelSysUnc = fabs(CV - altV) / CV;

		printf("\t[%.1lf, %.1lf] %.3lf (%%)\n", MassBinEdges[i], MassBinEdges[i+1], RelSysUnc * 100 );

		h_RelSysUnc->SetBinContent(i_bin, RelSysUnc);
		h_RelSysUnc->SetBinError(i_bin, 0);
	}
	h_RelSysUnc->Scale( 100 );
	h_RelSysUnc->SetName("h_RelSysUnc_FSRCorr_AltMC_Percent");

	TFile *f_output = TFile::Open("ROOTFile_SysUnc_FSR_Modeling.root", "RECREATE");
	f_output->cd();
	h_DiffXsec_AltMC->Write();
	h_RelSysUnc->Write();
}

TH1D* CalcDiffXsec_GivenFSRCorr(TString version, TH1D* h_Truth_RooUnfold, RooUnfoldResponse *UnfoldRes_FSR)
{
	MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, "76X");
	XsecTool->MakeSignalMCHistograms();

	XsecTool->GetYieldHistograms( kTRUE );

	XsecTool->UnfoldingCorrection();
	XsecTool->AccEffCorrection();
	XsecTool->EfficiencyScaleFactor();
	XsecTool->FSRCorrection(h_Truth_RooUnfold, UnfoldRes_FSR);
	XsecTool->CalcXsec();

	// CopyHistogram( XsecTool->h_DiffXsec_FSRCorr, h_DiffXsec );
	return XsecTool->h_DiffXsec_FSRCorr;
}