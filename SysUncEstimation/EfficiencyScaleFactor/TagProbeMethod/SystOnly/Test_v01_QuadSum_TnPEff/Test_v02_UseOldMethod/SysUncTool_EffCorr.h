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
#include <TGraphAsymmErrors.h>
#include <TRandom3.h>

#include <vector>

// -- for Rochester Muon momentum correction -- //
#include <Include/RochesterMomCorr_76X/RoccoR.cc>
#include <Include/RochesterMomCorr_76X/rochcor2015.cc>

#include <src/RooUnfoldResponse.h>
#include <src/RooUnfoldBayes.h>
#include <src/RooUnfoldInvert.h>

#include <Include/MyCanvas.C>
#include <Include/DYAnalyzer.h>
#include <Include/DiffXsecTools.h>

#include <Include/PlotTools.h>

#define nEtaBin 5
#define nPtBin 4
// #define nEffMap 10
#define nEffMap 100

class MyDiffXsecTool : public DiffXsecTools
{
public:
	MyDiffXsecTool() : DiffXsecTools()
	{

	}

	void EfficiencyScaleFactor(TGraphAsymmErrors *g_Eff, TGraphAsymmErrors *g_Eff_Corr_HLTv4p2, TGraphAsymmErrors *g_Eff_Corr_HLTv4p3)
	{
		// -- Calculate efficiency scale factor for each mass bin: SF = Corrected Eff / Un-corrected Eff -- //
		TGraphAsymmErrors *g_EffCorr_HLTv4p2 = (TGraphAsymmErrors*)g_Eff->Clone();
		this->MakeRatioGraph(g_EffCorr_HLTv4p2, g_Eff_Corr_HLTv4p2, g_Eff);

		TGraphAsymmErrors *g_EffCorr_HLTv4p3 = (TGraphAsymmErrors*)g_Eff->Clone();
		this->MakeRatioGraph(g_EffCorr_HLTv4p3, g_Eff_Corr_HLTv4p3, g_Eff);

		// this->SaveCanvas_EffCorr_HLTv4p2_vs_HLTv4p3( g_EffCorr_HLTv4p2, g_EffCorr_HLTv4p3 );

		/////////////////////////////////////////////////////////////
		// -- Apply Efficiency correction factors to each Yield -- //
		/////////////////////////////////////////////////////////////
		h_yield_HLTv4p2_EffCorr = (TH1D*)h_yield_HLTv4p2_Unfolded_AccEff->Clone();
		this->ApplyEffCorr_Yield(h_yield_HLTv4p2_EffCorr, h_yield_HLTv4p2_Unfolded_AccEff, g_EffCorr_HLTv4p2);

		h_yield_HLTv4p3_EffCorr = (TH1D*)h_yield_HLTv4p3_Unfolded_AccEff->Clone();
		this->ApplyEffCorr_Yield(h_yield_HLTv4p3_EffCorr, h_yield_HLTv4p3_Unfolded_AccEff, g_EffCorr_HLTv4p3);

		/////////////////////////////
		// -- Combine the yield -- //
		/////////////////////////////
		h_yield_HLTv4p2_EffCorr->Sumw2(); h_yield_HLTv4p3_EffCorr->Sumw2();
		h_yield_EffCorr = (TH1D*)h_yield_HLTv4p2_EffCorr->Clone();
		h_yield_EffCorr->Add( h_yield_HLTv4p3_EffCorr );
	}

	void FpoF_EfficiencyScaleFactor(TGraphAsymmErrors *g_Eff, TGraphAsymmErrors *g_Eff_Corr_HLTv4p2, TGraphAsymmErrors *g_Eff_Corr_HLTv4p3)
	{
		// -- Calculate efficiency scale factor for each mass bin: SF = Corrected Eff / Un-corrected Eff -- //
		this->g_EffCorr_HLTv4p2 = (TGraphAsymmErrors*)g_Eff->Clone();
		this->MakeRatioGraph(g_EffCorr_HLTv4p2, g_Eff_Corr_HLTv4p2, g_Eff);

		this->g_EffCorr_HLTv4p3 = (TGraphAsymmErrors*)g_Eff->Clone();
		this->MakeRatioGraph(g_EffCorr_HLTv4p3, g_Eff_Corr_HLTv4p3, g_Eff);

		/////////////////////////////////////////////////////////////
		// -- Apply Efficiency correction factors to each Yield -- //
		/////////////////////////////////////////////////////////////
		this->h_FpoF_yield_HLTv4p2_EffCorr = (TH1D*)this->h_FpoF_yield_HLTv4p2_Unfolded_Eff->Clone( "h_FpoF_yield_HLTv4p2_EffCorr" );
		this->ApplyEffCorr_Yield(this->h_FpoF_yield_HLTv4p2_EffCorr, this->h_FpoF_yield_HLTv4p2_Unfolded_Eff, this->g_EffCorr_HLTv4p2);

		this->h_FpoF_yield_HLTv4p3_EffCorr = (TH1D*)this->h_FpoF_yield_HLTv4p3_Unfolded_Eff->Clone( "h_FpoF_yield_HLTv4p3_EffCorr" );
		this->ApplyEffCorr_Yield(this->h_FpoF_yield_HLTv4p3_EffCorr, this->h_FpoF_yield_HLTv4p3_Unfolded_Eff, this->g_EffCorr_HLTv4p3);

		/////////////////////////////
		// -- Combine the yield -- //
		/////////////////////////////
		this->h_FpoF_yield_HLTv4p2_EffCorr->Sumw2(); this->h_FpoF_yield_HLTv4p3_EffCorr->Sumw2();
		this->h_FpoF_yield_EffCorr = (TH1D*)this->h_FpoF_yield_HLTv4p2_EffCorr->Clone( "h_FpoF_yield_EffCorr" );
		this->h_FpoF_yield_EffCorr->Add( this->h_FpoF_yield_HLTv4p3_EffCorr );
	}
};


class SysUncTool_EffCorr
{
public:
	TString IncludePath;
	TString ROOTFilePath;
	Bool_t isDataDriven;
	Double_t MassBinEdges[nMassBin+1];

	Bool_t isSyst;
	TString SystType;

	// -- Reco + ID Efficiency & error -- //
	Double_t Eff_RecoID_Data[nEtaBin][nPtBin];
	Double_t EffErr_Stat_RecoID_Data[nEtaBin][nPtBin];
	Double_t EffErr_Sys_RecoID_Data[nEtaBin][nPtBin];
	Double_t EffErr_Tot_RecoID_Data[nEtaBin][nPtBin];

	Double_t Eff_RecoID_MC[nEtaBin][nPtBin];
	Double_t EffErr_Stat_RecoID_MC[nEtaBin][nPtBin];
	Double_t EffErr_Sys_RecoID_MC[nEtaBin][nPtBin];
	Double_t EffErr_Tot_RecoID_MC[nEtaBin][nPtBin];
	////////////////////////////////////////

	// -- Isolation Efficiency & error -- //
	Double_t Eff_Iso_Data[nEtaBin][nPtBin];
	Double_t EffErr_Stat_Iso_Data[nEtaBin][nPtBin];
	Double_t EffErr_Sys_Iso_Data[nEtaBin][nPtBin];
	Double_t EffErr_Tot_Iso_Data[nEtaBin][nPtBin];

	Double_t Eff_Iso_MC[nEtaBin][nPtBin];
	Double_t EffErr_Stat_Iso_MC[nEtaBin][nPtBin];
	Double_t EffErr_Sys_Iso_MC[nEtaBin][nPtBin];
	Double_t EffErr_Tot_Iso_MC[nEtaBin][nPtBin];
	////////////////////////////////////////

	// -- Trigger Efficiency & error -- //
	Double_t Eff_HLTv4p2_Data[nEtaBin][nPtBin];
	Double_t EffErr_Stat_HLTv4p2_Data[nEtaBin][nPtBin];
	Double_t EffErr_Sys_HLTv4p2_Data[nEtaBin][nPtBin];
	Double_t EffErr_Tot_HLTv4p2_Data[nEtaBin][nPtBin];

	Double_t Eff_HLTv4p2_MC[nEtaBin][nPtBin];
	Double_t EffErr_Stat_HLTv4p2_MC[nEtaBin][nPtBin];
	Double_t EffErr_Sys_HLTv4p2_MC[nEtaBin][nPtBin];
	Double_t EffErr_Tot_HLTv4p2_MC[nEtaBin][nPtBin];

	Double_t Eff_HLTv4p3_Data[nEtaBin][nPtBin];
	Double_t EffErr_Stat_HLTv4p3_Data[nEtaBin][nPtBin];
	Double_t EffErr_Sys_HLTv4p3_Data[nEtaBin][nPtBin];
	Double_t EffErr_Tot_HLTv4p3_Data[nEtaBin][nPtBin];

	Double_t Eff_HLTv4p3_MC[nEtaBin][nPtBin];
	Double_t EffErr_Stat_HLTv4p3_MC[nEtaBin][nPtBin];
	Double_t EffErr_Sys_HLTv4p3_MC[nEtaBin][nPtBin];
	Double_t EffErr_Tot_HLTv4p3_MC[nEtaBin][nPtBin];
	////////////////////////////////////////

	// -- Reco + ID Efficiency smeared map -- //
	Double_t Eff_RecoID_Data_Smeared[nEffMap][nEtaBin][nPtBin];
	Double_t Eff_RecoID_MC_Smeared[nEffMap][nEtaBin][nPtBin];
	////////////////////////////////////////////

	// -- Isolation Efficiency smeared map -- //
	Double_t Eff_Iso_Data_Smeared[nEffMap][nEtaBin][nPtBin];
	Double_t Eff_Iso_MC_Smeared[nEffMap][nEtaBin][nPtBin];
	////////////////////////////////////////////

	// -- Trigger Efficiency smeared map -- //
	Double_t Eff_HLTv4p2_Data_Smeared[nEffMap][nEtaBin][nPtBin];
	Double_t Eff_HLTv4p2_MC_Smeared[nEffMap][nEtaBin][nPtBin];

	Double_t Eff_HLTv4p3_Data_Smeared[nEffMap][nEtaBin][nPtBin];
	Double_t Eff_HLTv4p3_MC_Smeared[nEffMap][nEtaBin][nPtBin];
	////////////////////////////////////////////


	// -- Un-corrected efficiencies -- //
	TGraphAsymmErrors* g_Eff_Uncorrected;

	// -- Corrected Efficiencies -- //
	TGraphAsymmErrors* g_Eff_HLTv4p2_CV;
	TGraphAsymmErrors* g_Eff_HLTv4p3_CV;
	TGraphAsymmErrors* g_Eff_HLTv4p2_Smeared[nEffMap];
	TGraphAsymmErrors* g_Eff_HLTv4p3_Smeared[nEffMap];

	// -- Differential X-section: Calculated in "CalcXsec_AllMap" Method -- //
	TH1D *h_DiffXsec_CV;
	TH1D *h_DiffXsec_Smeared[nEffMap];


	// -- fiducial, post-FSR cross section -- //
	TH1D* h_FpoF_DiffXsec_CV;
	TH1D *h_FpoF_DiffXsec_Smeared[nEffMap];

	MyCanvas *myc_ValidPlot;
	MyCanvas *myc_FpoF_ValidPlot;

	TH2D* h_2D_temp; // -- for saving pt and eta bin edges -- //


	SysUncTool_EffCorr()
	{
		// Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
		// 									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
		// 									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
		// 									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
		// 									 830, 1000, 1200, 1500, 2000, 3000};

		
		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		// -- Setting the differential x-section histograms -- //
		h_DiffXsec_CV = new TH1D("h_DiffXsec_CV", "", nMassBin, MassBinEdges);
		for(Int_t i=0; i<nEffMap; i++)
			h_DiffXsec_Smeared[i] = new TH1D("h_DiffXsec_Smeared_"+TString::Format("%d", i), "", nMassBin, MassBinEdges);

		// -- Setting the fiducial, post-FSR differential x-section histograms -- //
		h_FpoF_DiffXsec_CV = new TH1D("h_FpoF_DiffXsec_CV", "", nMassBin, MassBinEdges);
		for(Int_t i=0; i<nEffMap; i++)
			h_FpoF_DiffXsec_Smeared[i] = new TH1D("h_FpoF_DiffXsec_Smeared_"+TString::Format("%d", i), "", nMassBin, MassBinEdges);

		isDataDriven = kTRUE;
		cout << "Default value for isDataDriven: " << isDataDriven << endl;

		this->IncludePath = gSystem->Getenv("KP_INCLUDE_PATH");
		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

		this->isSyst = kFALSE;
		this->SystType = "";
	}

	void SetIsDataDriven(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;
		cout << "\n=================================" << endl;
		cout << "isDataDriven = " << isDataDriven << endl;
		cout << "=================================\n\n" << endl;

	}

	void SetupCentralValueStatError(TString ROOTFileName)
	{
		TFile *f = new TFile(this->IncludePath + "/" + ROOTFileName);
		TH2D *h_RecoID_Data = (TH2D*)f->Get("h_2D_Eff_RecoID_Data");
		TH2D *h_RecoID_MC = (TH2D*)f->Get("h_2D_Eff_RecoID_MC");

		TH2D *h_Iso_Data = (TH2D*)f->Get("h_2D_Eff_Iso_Data");
		TH2D *h_Iso_MC = (TH2D*)f->Get("h_2D_Eff_Iso_MC");

		TH2D *h_HLTv4p2_Data = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_Data");
		TH2D *h_HLTv4p2_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_MC");

		TH2D *h_HLTv4p3_Data = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_Data");
		TH2D *h_HLTv4p3_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_MC");

		Int_t nEtaBins = h_RecoID_Data->GetNbinsX();
		Int_t nPtBins = h_RecoID_Data->GetNbinsY();

		if( nEtaBin != nEtaBins || nPtBin != nPtBins )
		{
			printf("(nEtaBin, nPtBin) != (GetNbinsX, GetNbinsY) ... (%d, %d) != (%d, %d)\n", nEtaBin, nPtBin, nEtaBins, nPtBins);
			return;
		}

		for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
		{
			for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
			{
				Int_t i_etabin = iter_x + 1;
				Int_t i_ptbin = iter_y + 1;

				// -- Reco + ID cetral value & error -- // 
				Double_t RecoID_Data = h_RecoID_Data->GetBinContent(i_etabin, i_ptbin);
				Double_t RecoID_Data_Error = h_RecoID_Data->GetBinError(i_etabin, i_ptbin);

				Eff_RecoID_Data[iter_x][iter_y] = RecoID_Data;
				EffErr_Stat_RecoID_Data[iter_x][iter_y] = RecoID_Data_Error;

				Double_t RecoID_MC = h_RecoID_MC->GetBinContent(i_etabin, i_ptbin);
				Double_t RecoID_MC_Error = h_RecoID_MC->GetBinError(i_etabin, i_ptbin);

				Eff_RecoID_MC[iter_x][iter_y] = RecoID_MC;
				EffErr_Stat_RecoID_MC[iter_x][iter_y] = RecoID_MC_Error;
				//////////////////////////////////////////////////////////////////////////////

				// -- Isolation central value & error -- //
				Double_t Iso_Data = h_Iso_Data->GetBinContent(i_etabin, i_ptbin);
				Double_t Iso_Data_Error = h_Iso_Data->GetBinError(i_etabin, i_ptbin);

				Eff_Iso_Data[iter_x][iter_y] = Iso_Data;
				EffErr_Stat_Iso_Data[iter_x][iter_y] = Iso_Data_Error;

				Double_t Iso_MC = h_Iso_MC->GetBinContent(i_etabin, i_ptbin);
				Double_t Iso_MC_Error = h_Iso_MC->GetBinError(i_etabin, i_ptbin);

				Eff_Iso_MC[iter_x][iter_y] = Iso_MC;
				EffErr_Stat_Iso_MC[iter_x][iter_y] = Iso_MC_Error;
				//////////////////////////////////////////////////////////////////////////////

				// -- HLTv4p2 central value & error -- //
				Double_t HLTv4p2_Data = h_HLTv4p2_Data->GetBinContent(i_etabin, i_ptbin);
				Double_t HLTv4p2_Data_Error = h_HLTv4p2_Data->GetBinError(i_etabin, i_ptbin);

				Eff_HLTv4p2_Data[iter_x][iter_y] = HLTv4p2_Data;
				EffErr_Stat_HLTv4p2_Data[iter_x][iter_y] = HLTv4p2_Data_Error;

				Double_t HLTv4p2_MC = h_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);
				Double_t HLTv4p2_MC_Error = h_HLTv4p2_MC->GetBinError(i_etabin, i_ptbin);

				Eff_HLTv4p2_MC[iter_x][iter_y] = HLTv4p2_MC;
				EffErr_Stat_HLTv4p2_MC[iter_x][iter_y] = HLTv4p2_MC_Error;
				//////////////////////////////////////////////////////////////////////////////

				// -- HLTv4p3 central value & error -- //
				Double_t HLTv4p3_Data = h_HLTv4p3_Data->GetBinContent(i_etabin, i_ptbin);
				Double_t HLTv4p3_Data_Error = h_HLTv4p3_Data->GetBinError(i_etabin, i_ptbin);

				Eff_HLTv4p3_Data[iter_x][iter_y] = HLTv4p3_Data;
				EffErr_Stat_HLTv4p3_Data[iter_x][iter_y] = HLTv4p3_Data_Error;

				Double_t HLTv4p3_MC = h_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);
				Double_t HLTv4p3_MC_Error = h_HLTv4p3_MC->GetBinError(i_etabin, i_ptbin);

				Eff_HLTv4p3_MC[iter_x][iter_y] = HLTv4p3_MC;
				EffErr_Stat_HLTv4p3_MC[iter_x][iter_y] = HLTv4p3_MC_Error;
				//////////////////////////////////////////////////////////////////////////////
			}
		}
		cout << "========================================================================" << endl;
		cout << "[Setting for efficiency cetral value and statistical error is completed]" << endl;
		cout << "========================================================================" << endl;
		cout << endl;

		this->h_2D_temp = (TH2D*)h_RecoID_Data->Clone("h_2D_temp");
		this->h_2D_temp->Reset("ICES");
	}

	void SetUpSysUnc(TString ROOTFileName)
	{
		TFile *f_input = new TFile(this->IncludePath + "/" + ROOTFileName);

		TH2D *h_RelDiff_RecoID_Data = (TH2D*)f_input->Get("h_2D_RelDiff_RecoID_Data_Tot")->Clone();
		TH2D *h_RelDiff_RecoID_MC = (TH2D*)f_input->Get("h_2D_RelDiff_RecoID_MC_Tot")->Clone();

		TH2D *h_RelDiff_Iso_Data = (TH2D*)f_input->Get("h_2D_RelDiff_Iso_Data_Tot")->Clone();
		TH2D *h_RelDiff_Iso_MC = (TH2D*)f_input->Get("h_2D_RelDiff_Iso_MC_Tot")->Clone();

		TH2D *h_RelDiff_HLTv4p2_Data = (TH2D*)f_input->Get("h_2D_RelDiff_HLTv4p2_Data_Tot")->Clone();
		TH2D *h_RelDiff_HLTv4p2_MC = (TH2D*)f_input->Get("h_2D_RelDiff_HLTv4p2_MC_Tot")->Clone();

		TH2D *h_RelDiff_HLTv4p3_Data = (TH2D*)f_input->Get("h_2D_RelDiff_HLTv4p3_Data_Tot")->Clone();
		TH2D *h_RelDiff_HLTv4p3_MC = (TH2D*)f_input->Get("h_2D_RelDiff_HLTv4p3_MC_Tot")->Clone();

		Int_t nEtaBins = h_RelDiff_RecoID_Data->GetNbinsX();
		Int_t nPtBins = h_RelDiff_RecoID_Data->GetNbinsY();

		if( nEtaBin != nEtaBins || nPtBin != nPtBins )
		{
			printf("(nEtaBin, nPtBin) != (GetNbinsX, GetNbinsY) ... (%d, %d) != (%d, %d)\n", nEtaBin, nPtBin, nEtaBins, nPtBins);
			return;
		}

		for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
		{
			for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
			{
				Int_t i_etabin = iter_x + 1;
				Int_t i_ptbin = iter_y + 1;

				// -- Convert from relative uncertainty to absolute uncertainty -- //
				EffErr_Sys_RecoID_Data[iter_x][iter_y] = Eff_RecoID_Data[iter_x][iter_y] * h_RelDiff_RecoID_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_RecoID_MC[iter_x][iter_y] = Eff_RecoID_MC[iter_x][iter_y] * h_RelDiff_RecoID_MC->GetBinContent(i_etabin, i_ptbin);

				EffErr_Sys_Iso_Data[iter_x][iter_y] = Eff_Iso_Data[iter_x][iter_y] * h_RelDiff_Iso_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_Iso_MC[iter_x][iter_y] = Eff_Iso_MC[iter_x][iter_y] * h_RelDiff_Iso_MC->GetBinContent(i_etabin, i_ptbin);

				EffErr_Sys_HLTv4p2_Data[iter_x][iter_y] = Eff_HLTv4p2_Data[iter_x][iter_y] * h_RelDiff_HLTv4p2_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_HLTv4p2_MC[iter_x][iter_y] = Eff_HLTv4p2_MC[iter_x][iter_y] * h_RelDiff_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);

				EffErr_Sys_HLTv4p3_Data[iter_x][iter_y] = Eff_HLTv4p3_Data[iter_x][iter_y] * h_RelDiff_HLTv4p3_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_HLTv4p3_MC[iter_x][iter_y] = Eff_HLTv4p3_MC[iter_x][iter_y] * h_RelDiff_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);
			}
		}

		cout << "===========================================" << endl;
		cout << "[Setting for sys. uncertainty is completed]" << endl;
		cout << "===========================================" << endl;
		cout << endl;
	}

	void SetUpSysUnc_EachSource( TString _SystType )
	{
		this->isSyst = kTRUE;
		this->SystType = _SystType;

		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString FileName = AnalyzerPath + "/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly/TnPEffs/ROOTFile_Syst_TnPEffs.root";

		TH2D *h_Diff_RecoID_Data = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_RecoID_Data" );
		TH2D *h_Diff_RecoID_MC = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_RecoID_MC" );

		TH2D *h_Diff_Iso_Data = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_Iso_Data" );
		TH2D *h_Diff_Iso_MC = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_Iso_MC" );

		TH2D *h_Diff_HLTv4p2_Data = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_HLTv4p2_Data" );
		TH2D *h_Diff_HLTv4p2_MC = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_HLTv4p2_MC" );

		TH2D *h_Diff_HLTv4p3_Data = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_HLTv4p3_Data" );
		TH2D *h_Diff_HLTv4p3_MC = Get_Hist_2D( FileName, this->SystType+"/h_2D_EffDiff_HLTv4p3_MC" );

		Int_t nEtaBins = h_Diff_RecoID_Data->GetNbinsX();
		Int_t nPtBins = h_Diff_RecoID_Data->GetNbinsY();

		if( nEtaBin != nEtaBins || nPtBin != nPtBins )
		{
			printf("(nEtaBin, nPtBin) != (GetNbinsX, GetNbinsY) ... (%d, %d) != (%d, %d)\n", nEtaBin, nPtBin, nEtaBins, nPtBins);
			return;
		}

		for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
		{
			for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
			{
				Int_t i_etabin = iter_x + 1;
				Int_t i_ptbin = iter_y + 1;

				EffErr_Sys_RecoID_Data[iter_x][iter_y] = h_Diff_RecoID_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_RecoID_MC[iter_x][iter_y] = h_Diff_RecoID_MC->GetBinContent(i_etabin, i_ptbin);

				EffErr_Sys_Iso_Data[iter_x][iter_y] = h_Diff_Iso_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_Iso_MC[iter_x][iter_y] = h_Diff_Iso_MC->GetBinContent(i_etabin, i_ptbin);

				EffErr_Sys_HLTv4p2_Data[iter_x][iter_y] = h_Diff_HLTv4p2_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_HLTv4p2_MC[iter_x][iter_y] = h_Diff_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);

				EffErr_Sys_HLTv4p3_Data[iter_x][iter_y] = h_Diff_HLTv4p3_Data->GetBinContent(i_etabin, i_ptbin);
				EffErr_Sys_HLTv4p3_MC[iter_x][iter_y] = h_Diff_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);
			}
		}

		cout << "================================================" << endl;
		cout << "[Setting for sys. uncertainty is completed: Type = " << this->SystType << "]" << endl;
		cout << "================================================" << endl;
		cout << endl;
	}

	void RemoveStatError()
	{
		for(Int_t i_ptbin=0; i_ptbin<nPtBin; i_ptbin++)
		{
			for(Int_t i_etabin=0; i_etabin<nEtaBin; i_etabin++)
			{
				EffErr_Stat_RecoID_Data[i_etabin][i_ptbin] = 0;
				EffErr_Stat_RecoID_MC[i_etabin][i_ptbin] = 0;

				EffErr_Stat_Iso_Data[i_etabin][i_ptbin] = 0;
				EffErr_Stat_Iso_MC[i_etabin][i_ptbin] = 0;

				EffErr_Stat_HLTv4p2_Data[i_etabin][i_ptbin] = 0;
				EffErr_Stat_HLTv4p2_MC[i_etabin][i_ptbin] = 0;

				EffErr_Stat_HLTv4p3_Data[i_etabin][i_ptbin] = 0;
				EffErr_Stat_HLTv4p3_MC[i_etabin][i_ptbin] = 0;
			}
		}
	}

	void RemoveSystError()
	{
		for(Int_t i_ptbin=0; i_ptbin<nPtBin; i_ptbin++)
		{
			for(Int_t i_etabin=0; i_etabin<nEtaBin; i_etabin++)
			{
				EffErr_Sys_RecoID_Data[i_etabin][i_ptbin] = 0;
				EffErr_Sys_RecoID_MC[i_etabin][i_ptbin] = 0;

				EffErr_Sys_Iso_Data[i_etabin][i_ptbin] = 0;
				EffErr_Sys_Iso_MC[i_etabin][i_ptbin] = 0;

				EffErr_Sys_HLTv4p2_Data[i_etabin][i_ptbin] = 0;
				EffErr_Sys_HLTv4p2_MC[i_etabin][i_ptbin] = 0;

				EffErr_Sys_HLTv4p3_Data[i_etabin][i_ptbin] = 0;
				EffErr_Sys_HLTv4p3_MC[i_etabin][i_ptbin] = 0;
			}
		}
	}

	void CalcTotalUnc()
	{
		for(Int_t iter_x=0; iter_x< nEtaBin; iter_x++)
		{
			for(Int_t iter_y=0; iter_y<nPtBin; iter_y++)
			{
				// -- Reco+ID Efficinecy Error -- //
				Double_t RecoID_Data_StatErr = EffErr_Stat_RecoID_Data[iter_x][iter_y];
				Double_t RecoID_Data_SysErr = EffErr_Sys_RecoID_Data[iter_x][iter_y];

				// -- (absolute uncertainty) -- //
				Double_t TotErrSquare_RecoID_Data = RecoID_Data_StatErr * RecoID_Data_StatErr + 
													RecoID_Data_SysErr * RecoID_Data_SysErr;

				EffErr_Tot_RecoID_Data[iter_x][iter_y] = sqrt(TotErrSquare_RecoID_Data);

				Double_t RecoID_MC_StatErr = EffErr_Stat_RecoID_MC[iter_x][iter_y];
				Double_t RecoID_MC_SysErr = EffErr_Sys_RecoID_MC[iter_x][iter_y];

				Double_t TotErrSquare_RecoID_MC = RecoID_MC_StatErr * RecoID_MC_StatErr + 
													RecoID_MC_SysErr * RecoID_MC_SysErr;

				EffErr_Tot_RecoID_MC[iter_x][iter_y] = sqrt(TotErrSquare_RecoID_MC);
				//////////////////////////////////////////////////////////////////////////////

				// -- Isolation Efficinecy Error -- //
				Double_t Iso_Data_StatErr = EffErr_Stat_Iso_Data[iter_x][iter_y];
				Double_t Iso_Data_SysErr = EffErr_Sys_Iso_Data[iter_x][iter_y];

				// -- (absolute uncertainty) -- //
				Double_t TotErrSquare_Iso_Data = Iso_Data_StatErr * Iso_Data_StatErr + 
													Iso_Data_SysErr * Iso_Data_SysErr;

				EffErr_Tot_Iso_Data[iter_x][iter_y] = sqrt(TotErrSquare_Iso_Data);

				Double_t Iso_MC_StatErr = EffErr_Stat_Iso_MC[iter_x][iter_y];
				Double_t Iso_MC_SysErr = EffErr_Sys_Iso_MC[iter_x][iter_y];

				Double_t TotErrSquare_Iso_MC = Iso_MC_StatErr * Iso_MC_StatErr + 
													Iso_MC_SysErr * Iso_MC_SysErr;

				EffErr_Tot_Iso_MC[iter_x][iter_y] = sqrt(TotErrSquare_Iso_MC);
				//////////////////////////////////////////////////////////////////////////////

				// -- HLTv4p2 Efficinecy Error -- //
				Double_t HLTv4p2_Data_StatErr = EffErr_Stat_HLTv4p2_Data[iter_x][iter_y];
				Double_t HLTv4p2_Data_SysErr = EffErr_Sys_HLTv4p2_Data[iter_x][iter_y];

				// -- (absolute uncertainty) -- //
				Double_t TotErrSquare_HLTv4p2_Data = HLTv4p2_Data_StatErr * HLTv4p2_Data_StatErr + 
													HLTv4p2_Data_SysErr * HLTv4p2_Data_SysErr;

				EffErr_Tot_HLTv4p2_Data[iter_x][iter_y] = sqrt(TotErrSquare_HLTv4p2_Data);

				Double_t HLTv4p2_MC_StatErr = EffErr_Stat_HLTv4p2_MC[iter_x][iter_y];
				Double_t HLTv4p2_MC_SysErr = EffErr_Sys_HLTv4p2_MC[iter_x][iter_y];

				Double_t TotErrSquare_HLTv4p2_MC = HLTv4p2_MC_StatErr * HLTv4p2_MC_StatErr + 
													HLTv4p2_MC_SysErr * HLTv4p2_MC_SysErr;

				EffErr_Tot_HLTv4p2_MC[iter_x][iter_y] = sqrt(TotErrSquare_HLTv4p2_MC);
				//////////////////////////////////////////////////////////////////////////////

				// -- HLTv4p3 Efficinecy Error -- //
				Double_t HLTv4p3_Data_StatErr = EffErr_Stat_HLTv4p3_Data[iter_x][iter_y];
				Double_t HLTv4p3_Data_SysErr = EffErr_Sys_HLTv4p3_Data[iter_x][iter_y];

				// -- (absolute uncertainty) -- //
				Double_t TotErrSquare_HLTv4p3_Data = HLTv4p3_Data_StatErr * HLTv4p3_Data_StatErr + 
													HLTv4p3_Data_SysErr * HLTv4p3_Data_SysErr;

				EffErr_Tot_HLTv4p3_Data[iter_x][iter_y] = sqrt(TotErrSquare_HLTv4p3_Data);

				Double_t HLTv4p3_MC_StatErr = EffErr_Stat_HLTv4p3_MC[iter_x][iter_y];
				Double_t HLTv4p3_MC_SysErr = EffErr_Sys_HLTv4p3_MC[iter_x][iter_y];

				Double_t TotErrSquare_HLTv4p3_MC = HLTv4p3_MC_StatErr * HLTv4p3_MC_StatErr + 
													HLTv4p3_MC_SysErr * HLTv4p3_MC_SysErr;

				EffErr_Tot_HLTv4p3_MC[iter_x][iter_y] = sqrt(TotErrSquare_HLTv4p3_MC);
				//////////////////////////////////////////////////////////////////////////////

			} // -- End of for(Int_t iter_y=0; iter_y<nPtBin; iter_y++) -- //

		} // -- End of for(Int_t iter_x=0; iter_x< nEtaBin; iter_x++) -- // 
		cout << "===========================================================" << endl;
		cout << "[Calculation of total uncertainty for each bin is completed]" << endl;
		cout << "===========================================================" << endl;
		cout << endl;
	}

	void Generate_SmearedEffMap_StatOnly()
	{
		TRandom3 eran;
		eran.SetSeed(0);

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++)
			{
				for(Int_t iter_y=0; iter_y<nPtBin; iter_y++)
				{
					Eff_RecoID_Data_Smeared[i_map][iter_x][iter_y] = Eff_RecoID_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_RecoID_MC[iter_x][iter_y];
					Eff_RecoID_MC_Smeared[i_map][iter_x][iter_y] = Eff_RecoID_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_RecoID_MC[iter_x][iter_y];

					Eff_Iso_Data_Smeared[i_map][iter_x][iter_y] = Eff_Iso_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_Iso_Data[iter_x][iter_y];
					Eff_Iso_MC_Smeared[i_map][iter_x][iter_y] = Eff_Iso_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_Iso_MC[iter_x][iter_y];

					Eff_HLTv4p2_Data_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p2_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_HLTv4p2_Data[iter_x][iter_y];
					Eff_HLTv4p2_MC_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p2_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_HLTv4p2_MC[iter_x][iter_y];

					Eff_HLTv4p3_Data_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p3_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_HLTv4p3_Data[iter_x][iter_y];
					Eff_HLTv4p3_MC_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p3_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Stat_HLTv4p3_MC[iter_x][iter_y];

				} // -- end of for(Int_t iter_y=0; iter_y<nPtBin; iter_y++) -- //

			} // -- end of for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++) -- //

		} // -- end of for(Int_t i_map=0; i_map<nEffMap; i_map++) -- //
		cout << "==================================================================" << endl;
		cout << "[" << nEffMap << " Smeared Maps are produced with stat. unc. only]" << endl;
		cout << "==================================================================" << endl;
		cout << endl;
	}

	// -- generator efficiency maps with taking into account the correlation between bins
	// -- all pt-eta bins moves in same direction & distance
	// -- by using same random seed among them -- //
	void Generate_SmearedEffMap_SystOnly_Correlated()
	{
		TRandom3 eran;
		eran.SetSeed(0);

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			Double_t rnd = eran.Gaus(0.0, 1.0); // -- share same random number for one efficiency map -- //

			for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++)
			{
				for(Int_t iter_y=0; iter_y<nPtBin; iter_y++)
				{
					Eff_RecoID_Data_Smeared[i_map][iter_x][iter_y] = Eff_RecoID_Data[iter_x][iter_y] + rnd * EffErr_Sys_RecoID_MC[iter_x][iter_y];
					Eff_RecoID_MC_Smeared[i_map][iter_x][iter_y] = Eff_RecoID_MC[iter_x][iter_y] + rnd * EffErr_Sys_RecoID_MC[iter_x][iter_y];

					Eff_Iso_Data_Smeared[i_map][iter_x][iter_y] = Eff_Iso_Data[iter_x][iter_y] + rnd * EffErr_Sys_Iso_Data[iter_x][iter_y];
					Eff_Iso_MC_Smeared[i_map][iter_x][iter_y] = Eff_Iso_MC[iter_x][iter_y] + rnd * EffErr_Sys_Iso_MC[iter_x][iter_y];

					Eff_HLTv4p2_Data_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p2_Data[iter_x][iter_y] + rnd * EffErr_Sys_HLTv4p2_Data[iter_x][iter_y];
					Eff_HLTv4p2_MC_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p2_MC[iter_x][iter_y] + rnd * EffErr_Sys_HLTv4p2_MC[iter_x][iter_y];

					Eff_HLTv4p3_Data_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p3_Data[iter_x][iter_y] + rnd * EffErr_Sys_HLTv4p3_Data[iter_x][iter_y];
					Eff_HLTv4p3_MC_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p3_MC[iter_x][iter_y] + rnd * EffErr_Sys_HLTv4p3_MC[iter_x][iter_y];

				} // -- end of for(Int_t iter_y=0; iter_y<nPtBin; iter_y++) -- //

			} // -- end of for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++) -- //

		} // -- end of for(Int_t i_map=0; i_map<nEffMap; i_map++) -- //
		cout << "=======================================================================" << endl;
		cout << "[" << nEffMap << " Smeared Maps are produced with syst. unc. only (Type=" << this->SystType << "]" << endl;
		cout << "=======================================================================" << endl;
		cout << endl;
	}

	void MakeSmearedEffMap()
	{
		TRandom3 eran;
		eran.SetSeed(0);

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++)
			{
				for(Int_t iter_y=0; iter_y<nPtBin; iter_y++)
				{
					Eff_RecoID_Data_Smeared[i_map][iter_x][iter_y] = Eff_RecoID_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_RecoID_Data[iter_x][iter_y];
					Eff_RecoID_MC_Smeared[i_map][iter_x][iter_y] = Eff_RecoID_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_RecoID_MC[iter_x][iter_y];

					Eff_Iso_Data_Smeared[i_map][iter_x][iter_y] = Eff_Iso_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_Iso_Data[iter_x][iter_y];
					Eff_Iso_MC_Smeared[i_map][iter_x][iter_y] = Eff_Iso_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_Iso_MC[iter_x][iter_y];

					Eff_HLTv4p2_Data_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p2_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_HLTv4p2_Data[iter_x][iter_y];
					Eff_HLTv4p2_MC_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p2_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_HLTv4p2_MC[iter_x][iter_y];

					Eff_HLTv4p3_Data_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p3_Data[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_HLTv4p3_Data[iter_x][iter_y];
					Eff_HLTv4p3_MC_Smeared[i_map][iter_x][iter_y] = Eff_HLTv4p3_MC[iter_x][iter_y] + eran.Gaus(0.0, 1.0) * EffErr_Tot_HLTv4p3_MC[iter_x][iter_y];

				} // -- end of for(Int_t iter_y=0; iter_y<nPtBin; iter_y++) -- //

			} // -- end of for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++) -- //

		} // -- end of for(Int_t i_map=0; i_map<nEffMap; i_map++) -- //
		cout << "======================================" << endl;
		cout << "[" << nEffMap << " Smeared Maps are produced]" << endl;
		cout << "======================================" << endl;
		cout << endl;
	}

	Double_t EfficiencySF_Smeared_EventWeight_HLTv4p2(Int_t i_map, Muon mu1, Muon mu2)
	{
		Double_t weight = -999;

		// -- Muon1 -- //
		Double_t Pt1 = mu1.Pt;
		Double_t eta1 = mu1.eta;

		Int_t ptbin1 = FindPtBin( Pt1 );
		Int_t etabin1 = FindEtaBin( eta1 );

		if( ptbin1 == 9999 || etabin1 == 9999 )
		{
			printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
			return -999;
		}

		Double_t Eff_muon1_Data = Eff_RecoID_Data_Smeared[i_map][etabin1][ptbin1] * Eff_Iso_Data_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_muon1_MC = Eff_RecoID_MC_Smeared[i_map][etabin1][ptbin1] * Eff_Iso_MC_Smeared[i_map][etabin1][ptbin1];

		// -- Muon2 -- //
		Double_t Pt2 = mu2.Pt;
		Double_t eta2 = mu2.eta;

		Int_t ptbin2 = FindPtBin( Pt2 );
		Int_t etabin2 = FindEtaBin( eta2 );

		if( ptbin2 == 9999 || etabin2 == 9999 )
		{
			printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
			return -999;
		}
		Double_t Eff_muon2_Data = Eff_RecoID_Data_Smeared[i_map][etabin2][ptbin2] * Eff_Iso_Data_Smeared[i_map][etabin2][ptbin2];
		Double_t Eff_muon2_MC = Eff_RecoID_MC_Smeared[i_map][etabin2][ptbin2] * Eff_Iso_MC_Smeared[i_map][etabin2][ptbin2];


		Double_t Eff_EventTrig_Data = 0;
		Double_t Eff_EventTrig_MC = 0;

		Double_t Eff_Trig_muon1_Data = Eff_HLTv4p2_Data_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_Trig_muon2_Data = Eff_HLTv4p2_Data_Smeared[i_map][etabin2][ptbin2];
		Eff_EventTrig_Data = Eff_Trig_muon1_Data + Eff_Trig_muon2_Data - Eff_Trig_muon1_Data * Eff_Trig_muon2_Data;

		Double_t Eff_Trig_muon1_MC = Eff_HLTv4p2_MC_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_Trig_muon2_MC = Eff_HLTv4p2_MC_Smeared[i_map][etabin2][ptbin2];
		Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;

		Double_t Eff_Data_all = Eff_muon1_Data * Eff_muon2_Data * Eff_EventTrig_Data;
		Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

		// cout << "Eff_Data_all: " << Eff_Data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
		weight = Eff_Data_all / Eff_MC_all;

		if( weight > 2 )
		{
			printf("[Data]\n");
			printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_Data_Smeared[i_map][etabin1][ptbin1], Eff_Iso_Data_Smeared[i_map][etabin1][ptbin1]);
			printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_Data_Smeared[i_map][etabin2][ptbin2], Eff_Iso_Data_Smeared[i_map][etabin2][ptbin2]);
			printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_Data, Eff_Data_all);

			printf("[MC]\n");
			printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC_Smeared[i_map][etabin1][ptbin1], Eff_Iso_MC_Smeared[i_map][etabin1][ptbin1]);
			printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC_Smeared[i_map][etabin2][ptbin2], Eff_Iso_MC_Smeared[i_map][etabin2][ptbin2]);
			printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

			printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
			
			printf("[SF] Weight = %.3lf\n", weight);
		}
		return weight;
	}

	Double_t EfficiencySF_Smeared_EventWeight_HLTv4p3(Int_t i_map, Muon mu1, Muon mu2)
	{
		Double_t weight = -999;

		// -- Muon1 -- //
		Double_t Pt1 = mu1.Pt;
		Double_t eta1 = mu1.eta;

		Int_t ptbin1 = FindPtBin( Pt1 );
		Int_t etabin1 = FindEtaBin( eta1 );

		if( ptbin1 == 9999 || etabin1 == 9999 )
		{
			printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
			return -999;
		}

		Double_t Eff_muon1_Data = Eff_RecoID_Data_Smeared[i_map][etabin1][ptbin1] * Eff_Iso_Data_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_muon1_MC = Eff_RecoID_MC_Smeared[i_map][etabin1][ptbin1] * Eff_Iso_MC_Smeared[i_map][etabin1][ptbin1];

		// -- Muon2 -- //
		Double_t Pt2 = mu2.Pt;
		Double_t eta2 = mu2.eta;

		Int_t ptbin2 = FindPtBin( Pt2 );
		Int_t etabin2 = FindEtaBin( eta2 );

		if( ptbin2 == 9999 || etabin2 == 9999 )
		{
			printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
			return -999;
		}
		Double_t Eff_muon2_Data = Eff_RecoID_Data_Smeared[i_map][etabin2][ptbin2] * Eff_Iso_Data_Smeared[i_map][etabin2][ptbin2];
		Double_t Eff_muon2_MC = Eff_RecoID_MC_Smeared[i_map][etabin2][ptbin2] * Eff_Iso_MC_Smeared[i_map][etabin2][ptbin2];


		Double_t Eff_EventTrig_Data = 0;
		Double_t Eff_EventTrig_MC = 0;

		Double_t Eff_Trig_muon1_Data = Eff_HLTv4p3_Data_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_Trig_muon2_Data = Eff_HLTv4p3_Data_Smeared[i_map][etabin2][ptbin2];
		Eff_EventTrig_Data = Eff_Trig_muon1_Data + Eff_Trig_muon2_Data - Eff_Trig_muon1_Data * Eff_Trig_muon2_Data;

		Double_t Eff_Trig_muon1_MC = Eff_HLTv4p3_MC_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_Trig_muon2_MC = Eff_HLTv4p3_MC_Smeared[i_map][etabin2][ptbin2];
		Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;

		Double_t Eff_Data_all = Eff_muon1_Data * Eff_muon2_Data * Eff_EventTrig_Data;
		Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

		// cout << "Eff_Data_all: " << Eff_Data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
		weight = Eff_Data_all / Eff_MC_all;

		if( weight > 2 )
		{
			printf("[Data]\n");
			printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_Data_Smeared[i_map][etabin1][ptbin1], Eff_Iso_Data_Smeared[i_map][etabin1][ptbin1]);
			printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_Data_Smeared[i_map][etabin2][ptbin2], Eff_Iso_Data_Smeared[i_map][etabin2][ptbin2]);
			printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_Data, Eff_Data_all);

			printf("[MC]\n");
			printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC_Smeared[i_map][etabin1][ptbin1], Eff_Iso_MC_Smeared[i_map][etabin1][ptbin1]);
			printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC_Smeared[i_map][etabin2][ptbin2], Eff_Iso_MC_Smeared[i_map][etabin2][ptbin2]);
			printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

			printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
			
			printf("[SF] Weight = %.3lf\n", weight);
		}
		return weight;
	}


	Double_t EfficiencySF_Smeared_EventWeight_RecoIdIso(Int_t i_map, Muon mu1, Muon mu2)
	{
		Double_t weight = -999;

		// -- Muon1 -- //
		Double_t Pt1 = mu1.Pt;
		Double_t eta1 = mu1.eta;

		Int_t ptbin1 = FindPtBin( Pt1 );
		Int_t etabin1 = FindEtaBin( eta1 );

		if( ptbin1 == 9999 || etabin1 == 9999 )
		{
			printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
			return -999;
		}

		Double_t Eff_muon1_Data = Eff_RecoID_Data_Smeared[i_map][etabin1][ptbin1] * Eff_Iso_Data_Smeared[i_map][etabin1][ptbin1];
		Double_t Eff_muon1_MC = Eff_RecoID_MC_Smeared[i_map][etabin1][ptbin1] * Eff_Iso_MC_Smeared[i_map][etabin1][ptbin1];

		// -- Muon2 -- //
		Double_t Pt2 = mu2.Pt;
		Double_t eta2 = mu2.eta;

		Int_t ptbin2 = FindPtBin( Pt2 );
		Int_t etabin2 = FindEtaBin( eta2 );

		if( ptbin2 == 9999 || etabin2 == 9999 )
		{
			printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
			return -999;
		}
		Double_t Eff_muon2_Data = Eff_RecoID_Data_Smeared[i_map][etabin2][ptbin2] * Eff_Iso_Data_Smeared[i_map][etabin2][ptbin2];
		Double_t Eff_muon2_MC = Eff_RecoID_MC_Smeared[i_map][etabin2][ptbin2] * Eff_Iso_MC_Smeared[i_map][etabin2][ptbin2];

		Double_t Eff_Data_all = Eff_muon1_Data * Eff_muon2_Data;
		Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC;

		weight = Eff_Data_all / Eff_MC_all;

		return weight;
	}

	Int_t FindPtBin(Double_t Pt)
	{
		Double_t PtBinEdges[nPtBin+1] = {10, 22, 40, 70, 250};

		Int_t ptbin = 9999;

		// -- if Pt is larger than the largest Pt bin edge, SF is same with the value for the last bin -- // 
		if( Pt > PtBinEdges[nPtBin] )
			ptbin = nPtBin-1;
		else
		{
			for(Int_t i=0; i<nPtBin; i++)
			{
				if( Pt > PtBinEdges[i] && Pt < PtBinEdges[i+1] )
				{
					ptbin = i;
					break;
				}
			}
		}

		return ptbin;
	}

	Int_t FindEtaBin(Double_t eta)
	{
		Double_t EtaBinEdges[nEtaBin+1] = {-2.4, -1.2, -0.3, 0.3, 1.2, 2.4};

		Int_t etabin = 9999;

		for(Int_t i=0; i<nEtaBin; i++)
		{
			if( eta > EtaBinEdges[i] && eta < EtaBinEdges[i+1] )
			{
				etabin = i;
				break;
			}
		}

		return etabin;
	}

	void CorrectedEff_AllMap(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO", TString HLTname = "IsoMu20_OR_IsoTkMu20" )
	{
		cout << "===============================================================================" << endl;
		cout << "[Start the calculation of *Corrected* MC-truth efficiency for each Smeared map]" << endl;
		cout << "===============================================================================" << endl;
		cout << endl;

		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
		cout << "Sample: " << Sample << endl;

		TString isApplyMomCorr = "";
		if( isCorrected == kTRUE )
		{
			cout << "Apply Roochester Muon Momentum Correction..." << endl;
			isApplyMomCorr = "MomCorr";
		}
		else
		{
			cout << "DO *NOT* Apply Roochester Muon Momentum Correction..." << endl;
			isApplyMomCorr = "MomUnCorr";
		}

		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( HLTname );
		analyzer->SetupEfficiencyScaleFactor( "ROOTFile_TagProbeEfficiency_76X_v20160502.root" );

		TH1D *h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", nMassBin, MassBinEdges);
		TH1D *h_mass_EffPass = new TH1D("h_mass_EffPass", "", nMassBin, MassBinEdges); // -- Uncorrected mass distribution -- //

		TH1D *h_mass_EffPass_Corr_HLTv4p2_CV = new TH1D("h_mass_EffPass_Corr_HLTv4p2_CentralValue", "", nMassBin, MassBinEdges);
		TH1D *h_mass_EffPass_Corr_HLTv4p3_CV = new TH1D("h_mass_EffPass_Corr_HLTv4p3_CentralValue", "", nMassBin, MassBinEdges);

		TH1D *h_mass_EffPass_Corr_HLTv4p2[nEffMap];
		TH1D *h_mass_EffPass_Corr_HLTv4p3[nEffMap];
		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			h_mass_EffPass_Corr_HLTv4p2[i_map] = new TH1D("h_mass_EffPass_Corr_HLTv4p2_"+TString::Format("%d", i_map), "", nMassBin, MassBinEdges);
			h_mass_EffPass_Corr_HLTv4p3[i_map] = new TH1D("h_mass_EffPass_Corr_HLTv4p3_"+TString::Format("%d", i_map), "", nMassBin, MassBinEdges);
		}

		TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
		// -- Each ntuple directory & corresponding Tags -- //
			// -- GenWeights are already taken into account in nEvents -- //
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

		if( Sample == "aMCNLO" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}
		if( Sample == "Powheg" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}

		// -- Loop for all samples -- //
		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_HLT();
			ntuple->TurnOnBranches_Muon();
			ntuple->TurnOnBranches_GenLepton();

			rochcor2015 *rmcor = new rochcor2015();

			Bool_t isMC;
			Tag[i_tup] == "Data" ? isMC = kFALSE : isMC = kTRUE;
			analyzer->SetupPileUpReWeighting_76X( isMC );

			Bool_t isNLO = 0;
			if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
			{
				isNLO = 1;
				cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
			}

			Double_t SumWeights = 0;
			Double_t SumWeights_Separated = 0;

			Int_t NEvents = chain->GetEntries();
			cout << "\t[Total Events: " << NEvents << "]" << endl; 

			Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << norm << "]" << endl;

			if( NEvents > 3000000 ) NEvents = 3000000;
			// NEvents = 20000;
			// -- Event loop starts -- //
			for(Int_t i=0; i<NEvents; i++)
			{
				this->loadBar(i+1, NEvents, 100, 100);

				ntuple->GetEvent(i);

				//Bring weights for NLO MC events
				Double_t GenWeight;
				if( isNLO == 1 )
					ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
				else
					GenWeight = 1;

				SumWeights += GenWeight;

				Int_t PU = ntuple->nPileUp;
				Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

				Double_t TotWeight = norm * GenWeight;

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

				if( GenFlag == kTRUE )
				{
					SumWeights_Separated += GenWeight;

					// -- Collect gen-level information -- //
					vector<GenLepton> GenLeptonCollection;
					Int_t NGenLeptons = ntuple->gnpair; 
					for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
					{
						GenLepton genlep;
						genlep.FillFromNtuple(ntuple, i_gen);
						if( genlep.isMuon() && genlep.fromHardProcessFinalState )
							GenLeptonCollection.push_back( genlep );
					}
					GenLepton genlep1 = GenLeptonCollection[0];
					GenLepton genlep2 = GenLeptonCollection[1];
					Double_t gen_M = (genlep1.Momentum + genlep2.Momentum).M();
					
					// -- Flags -- //
					Bool_t Flag_PassAcc = kFALSE;
					Bool_t Flag_PassAccEff = kFALSE;

					Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);


					Double_t Eff_SF_HLTv4p2_CV = -999; // -- Efficiency correction factor for the central value -- //
					Double_t Eff_SF_HLTv4p3_CV = -999; // -- Efficiency correction factor for the central value -- //

					Double_t Eff_SF_HLTv4p2[nEffMap] = {-999}; // -- Efficiency correction factor for each smeared map -- //
					Double_t Eff_SF_HLTv4p3[nEffMap] = {-999}; // -- Efficiency correction factor for each smeared map -- //

					// -- Calculate the efficiency among the events passing acceptance condition -- //
					if( Flag_PassAcc == kTRUE )
					{
						Bool_t Flag_PassEff = kFALSE;

						if( ntuple->isTriggered( analyzer->HLT ) )
						{
							// -- Collect Reconstruction level information -- //
							vector< Muon > MuonCollection;
							Int_t NLeptons = ntuple->nMuon;
							for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
							{
								Muon mu;
								mu.FillFromNtuple(ntuple, i_reco);
								// -- Apply Rochester momentum scale correction -- //
								if( isCorrected == kTRUE )
								{
									float qter = 1.0;
									
									if( Tag[i_tup] == "Data" )
										rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
									else
										rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

									// -- Change Muon pT, eta and phi with updated(corrected) one -- //
									mu.Pt = mu.Momentum.Pt();
									mu.eta = mu.Momentum.Eta();
									mu.phi = mu.Momentum.Phi();
								}
									MuonCollection.push_back( mu );
							}

							// -- Event Selection -- //
							vector< Muon > SelectedMuonCollection;
							Bool_t isPassEventSelection = kFALSE;
							isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

							if( isPassEventSelection == kTRUE )
							{
								Muon DYmuon1 = SelectedMuonCollection[0];
								Muon DYmuon2 = SelectedMuonCollection[1];
								Eff_SF_HLTv4p2_CV = analyzer->EfficiencySF_EventWeight_HLTv4p2( DYmuon1, DYmuon2 );
								Eff_SF_HLTv4p3_CV = analyzer->EfficiencySF_EventWeight_HLTv4p3( DYmuon1, DYmuon2 );

								for(Int_t i_map=0; i_map<nEffMap; i_map++)
								{
									Eff_SF_HLTv4p2[i_map] = this->EfficiencySF_Smeared_EventWeight_HLTv4p2( i_map, DYmuon1, DYmuon2 );
									Eff_SF_HLTv4p3[i_map] = this->EfficiencySF_Smeared_EventWeight_HLTv4p3( i_map, DYmuon1, DYmuon2 );
								}

								// printf("( SF_HLTv4p2, SF_HLTv4p3 ) = (%.5lf, %.5lf)\n", Eff_SF_HLTv4p2, Eff_SF_HLTv4p3);

								Flag_PassEff = kTRUE;
								Flag_PassAccEff = kTRUE;
							}

						} // -- End of if( ntuple->isTriggered( HLT ) ) -- //

						// -- Efficiency Calculation -- //
						if( Flag_PassEff == kTRUE)
						{
							h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );
							h_mass_EffPass->Fill( gen_M, TotWeight * PUWeight );
							h_mass_EffPass_Corr_HLTv4p2_CV->Fill( gen_M, TotWeight * PUWeight * Eff_SF_HLTv4p2_CV );
							h_mass_EffPass_Corr_HLTv4p3_CV->Fill( gen_M, TotWeight * PUWeight * Eff_SF_HLTv4p3_CV );

							for(Int_t i_map=0; i_map<nEffMap; i_map++)
							{
								h_mass_EffPass_Corr_HLTv4p2[i_map]->Fill( gen_M, TotWeight * PUWeight * Eff_SF_HLTv4p2[i_map] );
								h_mass_EffPass_Corr_HLTv4p3[i_map]->Fill( gen_M, TotWeight * PUWeight * Eff_SF_HLTv4p3[i_map] );
							}
						}
						else
							h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );

					} // -- End of if( Flag_PassAcc )

				} // -- End of if( GenFlag == kTRUE )

			} //End of event iteration

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
		} //end of i_tup iteration

		TEfficiency *Eff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_EffTotal);
		this->g_Eff_Uncorrected = (TGraphAsymmErrors*)Eff_Mass->CreateGraph()->Clone();

		// -- Corrected efficiency for RecoID + Iso + HLTv4p2 -- //
		TEfficiency *Eff_Mass_Corr_HLTv4p2_CV = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p2_CV, *h_mass_EffTotal);
		this->g_Eff_HLTv4p2_CV = (TGraphAsymmErrors*)Eff_Mass_Corr_HLTv4p2_CV->CreateGraph()->Clone();
		///////////////////////////////////////////////////////////////////

		// -- Corrected efficiency for RecoID + Iso + HLTv4p3 -- //
		TEfficiency *Eff_Mass_Corr_HLTv4p3_CV = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p3_CV, *h_mass_EffTotal);
		this->g_Eff_HLTv4p3_CV = (TGraphAsymmErrors*)Eff_Mass_Corr_HLTv4p3_CV->CreateGraph()->Clone();
		///////////////////////////////////////////////////////////////////

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			TString Numbering;
			Numbering.Form("%d", i_map);

			// -- Corrected efficiency factor for RecoID + Iso + HLTv4p2 -- //
			TEfficiency *Eff_Mass_Corr_HLTv4p2_Smeared = new TEfficiency( *h_mass_EffPass_Corr_HLTv4p2[i_map], *h_mass_EffTotal ); // -- (i_map)th corrected MC-truth Efficiency -- //
			this->g_Eff_HLTv4p2_Smeared[i_map] = (TGraphAsymmErrors*)Eff_Mass_Corr_HLTv4p2_Smeared->CreateGraph()->Clone();

			// -- Corrected efficiency factor for RecoID + Iso + HLTv4p3 -- //
			TEfficiency *Eff_Mass_Corr_HLTv4p3_Smeared = new TEfficiency(*h_mass_EffPass_Corr_HLTv4p3[i_map], *h_mass_EffTotal ); // -- (i_map)th corrected MC-truth Efficiency -- //
			this->g_Eff_HLTv4p3_Smeared[i_map] = (TGraphAsymmErrors*)Eff_Mass_Corr_HLTv4p3_Smeared->CreateGraph()->Clone();
		}


		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

	void CalcXsec_AllMap()
	{
		cout << "=========================================================" << endl;
		cout << "[Start the calculation of x-seciton for each Smeared map]" << endl;
		cout << "=========================================================" << endl;
		cout << endl;

		/////////////////////////////////////////////////////////////////
		// -- Calculation of the central value: for the cross check -- //
		/////////////////////////////////////////////////////////////////
		this->CalcDiffXsec_GivenEffSF( g_Eff_Uncorrected, g_Eff_HLTv4p2_CV, g_Eff_HLTv4p3_CV,
								 	   h_DiffXsec_CV);

		// h_DiffXsec_CV->SetName("h_DiffXsec_CV");
		TFile *f_cv = TFile::Open(this->ROOTFilePath + "/ROOTFile_Results_DYAnalysis_76X.root");
		f_cv->cd();
		TH1D *h_DiffXsec_Original = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr")->Clone(); h_DiffXsec_Original->SetDirectory(0);
		f_cv->Close();

		myc_ValidPlot = new MyCanvas("c_CrossCheck_DiffXsec_CV_vs_FromSysUncTool", "m [GeV]", "d#sigma/dM [pb/GeV]");
		myc_ValidPlot->SetLogx();
		myc_ValidPlot->SetLogy(0);
		myc_ValidPlot->SetRatioRange(0.9, 1.1);
		myc_ValidPlot->CanvasWithHistogramsRatioPlot(h_DiffXsec_CV, h_DiffXsec_Original, "from SysUncTool", "Central value", "fromSyst/CV");
		myc_ValidPlot->PrintCanvas();

		//////////////////////////////////////////////////////////////////
		// -- Calculation of the smeared differential cross sections -- //
		//////////////////////////////////////////////////////////////////
		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			this->CalcDiffXsec_GivenEffSF( g_Eff_Uncorrected, g_Eff_HLTv4p2_Smeared[i_map], g_Eff_HLTv4p3_Smeared[i_map],
										   h_DiffXsec_Smeared[i_map] );
		}

		cout << "=========================================================" << endl;
		cout << "[End of the calculation of x-seciton for each Smeared map]" << endl;
		cout << "=========================================================" << endl;
		cout << endl;
	}

	void CalcDiffXsec_GivenEffSF( TGraphAsymmErrors* g_Eff_UnCorr, TGraphAsymmErrors* g_Eff_HLTv4p2, TGraphAsymmErrors* g_Eff_HLTv4p3,
								  TH1D *h_DiffXsec)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool();
		XsecTool->MakeSignalMCHistograms();

		XsecTool->GetYieldHistograms(isDataDriven);
		XsecTool->UnfoldingCorrection();
		XsecTool->AccEffCorrection();

		XsecTool->EfficiencyScaleFactor(g_Eff_UnCorr, g_Eff_HLTv4p2, g_Eff_HLTv4p3);
		XsecTool->FSRCorrection();
		XsecTool->CalcXsec();

		// *h_DiffXsec = *(TH1D*)XsecTool->h_DiffXsec_FSRCorr->Clone();
		// h_DiffXsec = (TH1D*)XsecTool->h_DiffXsec_FSRCorr->Clone(); h_DiffXsec->SetDirectory(0);
		// XsecTool->h_DiffXsec_FSRCorr->Copy( *h_DiffXsec );
		CopyDiffXsecHistogram( XsecTool->h_DiffXsec_FSRCorr, h_DiffXsec );
	}

	void CopyDiffXsecHistogram( TH1D* h_Original, TH1D* h_Empty )
	{
		Int_t nBin = h_Original->GetNbinsX();
		Int_t nBin2 = h_Empty->GetNbinsX();

		if( nBin != nBin2 )
		{
			printf( "[ERROR] # bins of DiffXsec histogram from DiffXsecTool and this class! ... (nBin1, nBin2) = (%d, %d)\n", nBin, nBin2 );
			return;
		}

		for(Int_t i=-1; i<nBin+1; i++) // -- including under/overflow -- //
		{
			Int_t i_bin = i+1;
			Double_t value = h_Original->GetBinContent(i_bin);
			Double_t error = h_Original->GetBinError(i_bin);

			h_Empty->SetBinContent(i_bin, value);
			h_Empty->SetBinError(i_bin, error);
		}
	}

	void SaveResults()
	{
		TString OutputFileName = "ROOTFile_Outputs_SysUncTool_EffCorr_Stat.root";
		if( this->isSyst )
			OutputFileName.ReplaceAll( "_Stat.root", TString::Format("_Syst_%s.root", this->SystType.Data()) );

		TFile *f_output = new TFile(OutputFileName, "RECREATE"); f_output->cd();

		f_output->mkdir("EffMap_PtEtaBin");
		f_output->cd("EffMap_PtEtaBin");
		this->SaveEffMap_CV(f_output);

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
			this->SaveEffMap_Smeared(i_map);

		f_output->cd();
		f_output->mkdir("Eff_MassBin");
		f_output->cd("Eff_MassBin");
		// -- Un-corrected efficiencies -- //
		this->g_Eff_Uncorrected->SetName("g_Eff_Uncorrected");
		this->g_Eff_Uncorrected->Write();

		// -- Corrected Efficiencies -- //
		this->g_Eff_HLTv4p2_CV->SetName("g_Eff_HLTv4p2_CV");
		this->g_Eff_HLTv4p2_CV->Write();

		this->g_Eff_HLTv4p3_CV->SetName("g_Eff_HLTv4p3_CV");
		this->g_Eff_HLTv4p3_CV->Write();

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			TString Numbering;
			Numbering.Form("%d", i_map);

			this->g_Eff_HLTv4p2_Smeared[i_map]->SetName("g_Eff_HLTv4p2_Smeared_"+Numbering);
			this->g_Eff_HLTv4p2_Smeared[i_map]->Write();

			this->g_Eff_HLTv4p3_Smeared[i_map]->SetName("g_Eff_HLTv4p3_Smeared_"+Numbering);
			this->g_Eff_HLTv4p3_Smeared[i_map]->Write();
		}

		// -- Scale factors as a function of mass -- //
		f_output->cd();
		f_output->mkdir("SF_MassBin");
		f_output->cd("SF_MassBin");
		TGraphAsymmErrors* g_SF_HLTv4p2_CV = MakeGraph_Ratio( this->g_Eff_HLTv4p2_CV, this->g_Eff_Uncorrected, "g_SF_HLTv4p2_CV" );
		TGraphAsymmErrors* g_SF_HLTv4p3_CV = MakeGraph_Ratio( this->g_Eff_HLTv4p3_CV, this->g_Eff_Uncorrected, "g_SF_HLTv4p3_CV" );
		g_SF_HLTv4p2_CV->Write();
		g_SF_HLTv4p3_CV->Write();

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			TString Numbering;
			Numbering.Form("%d", i_map);

			TGraphAsymmErrors* g_SF_HLTv4p2_Smeared = MakeGraph_Ratio( this->g_Eff_HLTv4p2_Smeared[i_map], this->g_Eff_Uncorrected, "g_SF_HLTv4p2_Smeared_"+Numbering );
			TGraphAsymmErrors* g_SF_HLTv4p3_Smeared = MakeGraph_Ratio( this->g_Eff_HLTv4p3_Smeared[i_map], this->g_Eff_Uncorrected, "g_SF_HLTv4p3_Smeared_"+Numbering );
			g_SF_HLTv4p2_Smeared->Write();
			g_SF_HLTv4p3_Smeared->Write();
		}

		f_output->cd();

		// -- Recalculated cross sections -- //
		h_DiffXsec_CV->Write();
		for(Int_t i_map=0; i_map<nEffMap; i_map++)
			h_DiffXsec_Smeared[i_map]->Write();
	}

	void SaveEffMap_CV( TFile *f_output, TString ROOTFileName = "ROOTFile_TagProbeEfficiency_76X_v20160502.root" )
	{
		TFile *f = new TFile(this->IncludePath + "/" + ROOTFileName);
		TH2D *h_RecoID_Data = (TH2D*)f->Get("h_2D_Eff_RecoID_Data")->Clone("h_2D_Eff_RecoID_Data_CV");
		TH2D *h_RecoID_MC = (TH2D*)f->Get("h_2D_Eff_RecoID_MC")->Clone("h_2D_Eff_RecoID_MC_CV");

		TH2D *h_Iso_Data = (TH2D*)f->Get("h_2D_Eff_Iso_Data")->Clone("h_2D_Eff_Iso_Data_CV");
		TH2D *h_Iso_MC = (TH2D*)f->Get("h_2D_Eff_Iso_MC")->Clone("h_2D_Eff_Iso_MC_CV");

		TH2D *h_HLTv4p2_Data = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_Data")->Clone("h_2D_Eff_HLTv4p2_Data_CV");
		TH2D *h_HLTv4p2_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_MC")->Clone("h_2D_Eff_HLTv4p2_MC_CV");

		TH2D *h_HLTv4p3_Data = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_Data")->Clone("h_2D_Eff_HLTv4p3_Data_CV");
		TH2D *h_HLTv4p3_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_MC")->Clone("h_2D_Eff_HLTv4p3_MC_CV");

		f_output->cd("EffMap_PtEtaBin");
		h_RecoID_Data->Write();
		h_RecoID_MC->Write();

		h_Iso_Data->Write();
		h_Iso_MC->Write();

		h_HLTv4p2_Data->Write();
		h_HLTv4p2_MC->Write();

		h_HLTv4p3_Data->Write();
		h_HLTv4p3_MC->Write();
	}

	void SaveEffMap_Smeared( Int_t i_map )
	{
		TString Numbering;
		Numbering.Form("%d", i_map);

		TH2D* h_2D_Eff_RecoID_Data = this->Make2DHist_EffMap( Eff_RecoID_Data_Smeared[i_map], "h_2D_Eff_RecoID_Data_Smeared_"+Numbering );
		TH2D* h_2D_Eff_RecoID_MC = this->Make2DHist_EffMap( Eff_RecoID_MC_Smeared[i_map], "h_2D_Eff_RecoID_MC_Smeared_"+Numbering );

		TH2D* h_2D_Eff_Iso_Data = this->Make2DHist_EffMap( Eff_Iso_Data_Smeared[i_map], "h_2D_Eff_Iso_Data_Smeared_"+Numbering );
		TH2D* h_2D_Eff_Iso_MC = this->Make2DHist_EffMap( Eff_Iso_MC_Smeared[i_map], "h_2D_Eff_Iso_MC_Smeared_"+Numbering );

		TH2D* h_2D_Eff_HLTv4p2_Data = this->Make2DHist_EffMap( Eff_HLTv4p2_Data_Smeared[i_map], "h_2D_Eff_HLTv4p2_Data_Smeared_"+Numbering );
		TH2D* h_2D_Eff_HLTv4p2_MC = this->Make2DHist_EffMap( Eff_HLTv4p2_MC_Smeared[i_map], "h_2D_Eff_HLTv4p2_MC_Smeared_"+Numbering );

		TH2D* h_2D_Eff_HLTv4p3_Data = this->Make2DHist_EffMap( Eff_HLTv4p3_Data_Smeared[i_map], "h_2D_Eff_HLTv4p3_Data_Smeared_"+Numbering );
		TH2D* h_2D_Eff_HLTv4p3_MC = this->Make2DHist_EffMap( Eff_HLTv4p3_MC_Smeared[i_map], "h_2D_Eff_HLTv4p3_MC_Smeared_"+Numbering );

		h_2D_Eff_RecoID_Data->Write();
		h_2D_Eff_RecoID_MC->Write();

		h_2D_Eff_Iso_Data->Write();
		h_2D_Eff_Iso_MC->Write();

		h_2D_Eff_HLTv4p2_Data->Write();
		h_2D_Eff_HLTv4p2_MC->Write();

		h_2D_Eff_HLTv4p3_Data->Write();
		h_2D_Eff_HLTv4p3_MC->Write();
	}

	TH2D* Make2DHist_EffMap( Double_t EffMap[nEtaBin][nPtBin], TString HistName = "" )
	{
		TH2D* h_2D = (TH2D*)this->h_2D_temp->Clone("h_2D");
		if( HistName != "" )
			h_2D->SetName( HistName );

		for(Int_t i_eta = 0; i_eta<nEtaBin; i_eta++)
		{
			for(Int_t i_pt = 0; i_pt<nPtBin; i_pt++)
			{
				Int_t i_etabin = i_eta + 1;
				Int_t i_ptbin =i_pt + 1;

				h_2D->SetBinContent( i_etabin, i_ptbin, EffMap[i_eta][i_pt] );
				h_2D->SetBinError( i_etabin, i_ptbin, 0 );
			}
		}

		return h_2D;
	}

	void LoadEffGraphs_fromROOTFile( TFile *f_effs )
	{
		f_effs->cd();
		this->g_Eff_Uncorrected = (TGraphAsymmErrors*)f_effs->Get("g_Eff_Uncorrected")->Clone();
		this->g_Eff_HLTv4p2_CV = (TGraphAsymmErrors*)f_effs->Get("g_Eff_HLTv4p2_CV")->Clone();
		this->g_Eff_HLTv4p3_CV = (TGraphAsymmErrors*)f_effs->Get("g_Eff_HLTv4p3_CV")->Clone();

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			TString Numbering = TString::Format("%d", i_map);
			this->g_Eff_HLTv4p2_Smeared[i_map] = (TGraphAsymmErrors*)f_effs->Get("g_Eff_HLTv4p2_Smeared_"+Numbering)->Clone();
			this->g_Eff_HLTv4p3_Smeared[i_map] = (TGraphAsymmErrors*)f_effs->Get("g_Eff_HLTv4p3_Smeared_"+Numbering)->Clone();
		}
	}

	static inline void loadBar(int x, int n, int r, int w)
	{
	    // Only update r times.
	    if( x == n )
	    	cout << endl;

	    if ( x % (n/r +1) != 0 ) return;

	 
	    // Calculuate the ratio of complete-to-incomplete.
	    float ratio = x/(float)n;
	    int   c     = ratio * w;
	 
	    // Show the percentage complete.
	    printf("%3d%% [", (int)(ratio*100) );
	 
	    // Show the load bar.
	    for (int x=0; x<c; x++) cout << "=";
	 
	    for (int x=c; x<w; x++) cout << " ";
	 
	    // ANSI Control codes to go back to the
	    // previous line and clear it.
		cout << "]\r" << flush;
	}

	// void FpoF_CalcXsec_AllMap(TString version, TString Ver_CMSSW)
	// {
	// 	cout << "=========================================================" << endl;
	// 	cout << "[Start the calculation of x-seciton for each Smeared map]" << endl;
	// 	cout << "=========================================================" << endl;
	// 	cout << endl;

	// 	/////////////////////////////////////////////////////////////////
	// 	// -- Calculation of the central value: for the cross check -- //
	// 	/////////////////////////////////////////////////////////////////
	// 	h_FpoF_DiffXsec_CV = FpoF_CalcDiffXsec_GivenEffSF(version, Ver_CMSSW, g_Eff_Uncorrected, g_Eff_HLTv4p2_CV, g_Eff_HLTv4p3_CV);

	// 	// h_DiffXsec_CV->SetName("h_DiffXsec_CV");
	// 	TString FileLocation = "";
	// 	TFile *f_cv = NULL;

	// 	if( Ver_CMSSW == "74X" )
	// 	{
	// 		FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles/"+version;
	// 		f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_74X.root");
	// 	}
	// 	else if( Ver_CMSSW == "76X" )
	// 	{
	// 		FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/"+version;
	// 		f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
	// 	}
	// 	f_cv->cd();
	// 	TH1D *h_DiffXsec_Original = (TH1D*)f_cv->Get("h_FpoF_DiffXsec_Data")->Clone(); h_DiffXsec_Original->SetDirectory(0);
	// 	delete f_cv;

	// 	myc_FpoF_ValidPlot = new MyCanvas("c_CrossCheck_FpoF_DiffXsec_CV_vs_FromSysUncTool", "Dimuon Mass (fiducial, post-FSR) [GeV]", "d#sigma/dM [pb/GeV]");
	// 	myc_FpoF_ValidPlot->SetLogx();
	// 	myc_FpoF_ValidPlot->SetLogy(0);
	// 	myc_FpoF_ValidPlot->SetRatioRange(0.9, 1.1);
	// 	myc_FpoF_ValidPlot->CanvasWithHistogramsRatioPlot(h_FpoF_DiffXsec_CV, h_DiffXsec_Original, "from SysUncTool", "Central value", "fromSyst/CV");
	// 	myc_FpoF_ValidPlot->PrintCanvas();

	// 	//////////////////////////////////////////////////////////////////
	// 	// -- Calculation of the smeared differential cross sections -- //
	// 	//////////////////////////////////////////////////////////////////
	// 	for(Int_t i_map=0; i_map<nEffMap; i_map++)
	// 	{
	// 		h_FpoF_DiffXsec_Smeared[i_map] = FpoF_CalcDiffXsec_GivenEffSF( version, Ver_CMSSW, g_Eff_Uncorrected, 
	// 																	   g_Eff_HLTv4p2_Smeared[i_map], g_Eff_HLTv4p3_Smeared[i_map] );
	// 	}

	// 	cout << "=========================================================" << endl;
	// 	cout << "[End of the calculation of x-seciton for each Smeared map]" << endl;
	// 	cout << "=========================================================" << endl;
	// 	cout << endl;
	// }

	// TH1D* FpoF_CalcDiffXsec_GivenEffSF(TString version, TString Ver_CMSSW, 
	// 							 TGraphAsymmErrors* g_Eff_UnCorr, TGraphAsymmErrors* g_Eff_HLTv4p2, TGraphAsymmErrors* g_Eff_HLTv4p3)
	// {
	// 	MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
	// 	XsecTool->MakeSignalMCHistograms();
	// 	XsecTool->GetYieldHistograms(isDataDriven);
	// 	XsecTool->UnfoldingCorrection();

	// 	XsecTool->FpoF_GetTheoryHist();
	// 	XsecTool->FpoF_EffCorrection();
	// 	XsecTool->FpoF_EfficiencyScaleFactor(g_Eff_UnCorr, g_Eff_HLTv4p2, g_Eff_HLTv4p3);
	// 	XsecTool->FpoF_CalcXsec();

	// 	return XsecTool->h_FpoF_DiffXsec_Data;
	// }

	// void FpoF_SaveResults()
	// {
	// 	TFile *f_output = new TFile("ROOTFile_FpoF_Outputs_SysUncTool_EffCorr.root", "RECREATE"); f_output->cd();
	// 	// -- Un-corrected efficiencies -- //
	// 	this->g_Eff_Uncorrected->SetName("g_Eff_Uncorrected");
	// 	this->g_Eff_Uncorrected->Write();

	// 	// -- Corrected Efficiencies -- //
	// 	this->g_Eff_HLTv4p2_CV->SetName("g_Eff_HLTv4p2_CV");
	// 	this->g_Eff_HLTv4p2_CV->Write();

	// 	this->g_Eff_HLTv4p3_CV->SetName("g_Eff_HLTv4p3_CV");
	// 	this->g_Eff_HLTv4p3_CV->Write();

	// 	myc_FpoF_ValidPlot->c->Write();

	// 	for(Int_t i_map=0; i_map<nEffMap; i_map++)
	// 	{
	// 		TString Numbering;
	// 		Numbering.Form("%d", i_map);

	// 		this->g_Eff_HLTv4p2_Smeared[i_map]->SetName("g_Eff_HLTv4p2_Smeared_"+Numbering);
	// 		this->g_Eff_HLTv4p2_Smeared[i_map]->Write();

	// 		this->g_Eff_HLTv4p3_Smeared[i_map]->SetName("g_Eff_HLTv4p3_Smeared_"+Numbering);
	// 		this->g_Eff_HLTv4p3_Smeared[i_map]->Write();
	// 	}

	// 	// -- Recalculated cross sections -- //
	// 	h_FpoF_DiffXsec_CV->SetName("h_FpoF_DiffXsec_CV");
	// 	h_FpoF_DiffXsec_CV->Write();
	// 	for(Int_t i_map=0; i_map<nEffMap; i_map++)
	// 	{
	// 		h_FpoF_DiffXsec_Smeared[i_map]->SetName("h_FpoF_DiffXsec_Smeared_"+TString::Format("%d", i_map));
	// 		h_FpoF_DiffXsec_Smeared[i_map]->Write();
	// 	}
	// }
};