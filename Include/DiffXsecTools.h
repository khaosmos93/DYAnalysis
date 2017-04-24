#pragma once

#include "MyCanvas.C"
#include "UncertantyCalcTool.h"
#include "DYAnalyzer.h"
#include <src/RooUnfold.cxx>
#include <src/RooUnfoldResponse.cxx>
#include <src/RooUnfoldBayes.cxx>
#include <src/RooUnfoldInvert.cxx>

class DiffXsecTools
{
public:
	Double_t MassBinEdges[nMassBin+1];

	TString FileLocation;
	TFile *f_MC;
	TFile *f_data;
	TFile *f_bkg_dataDriven;
	TFile *f_yield;
	TFile *f_AccEff;
	TFile *f_Unfold;
	TFile *f_FSR;
	TFile *f_theory;

	TFile *f_output;

	// TString SampleType;
	vector< TString > ntupleDirectory; 
	vector< TString > Tag; 
	vector< Double_t > Xsec; 
	vector< Double_t > nEvents;

	TH1D *h_totSignalMC;
	TH1D *h_totSignalMC_HLTv4p2;
	TH1D *h_totSignalMC_HLTv4p3;

	TH1D *h_totSignalMC_GenLevel;
	TH1D *h_totSignalMC_GenLevel_HLTv4p2;
	TH1D *h_totSignalMC_GenLevel_HLTv4p3;
	TH1D *h_totSignalMC_GenLevel_WithinAcc;
	TH1D *h_totSignalMC_GenLevel_WithinAcc_HLTv4p2;
	TH1D *h_totSignalMC_GenLevel_WithinAcc_HLTv4p3;

	TH1D *h_yield_Raw;
	TH1D *h_yield_HLTv4p2;
	TH1D *h_yield_HLTv4p3;

	TH1D *h_NoGen_Reco;

	TH1D *h_yield_Unfolded;
	TH1D *h_yield_HLTv4p2_Unfolded;
	TH1D *h_yield_HLTv4p3_Unfolded;

	// -- Acc*Eff corrected without unfolding correction -- //
	TH1D *h_yield_AccEff;
	TH1D *h_yield_HLTv4p2_AccEff;
	TH1D *h_yield_HLTv4p3_AccEff;
	// -- Acc*Eff corrected with unfolding correction -- //
	TH1D *h_yield_Unfolded_AccEff;
	TH1D *h_yield_HLTv4p2_Unfolded_AccEff;
	TH1D *h_yield_HLTv4p3_Unfolded_AccEff;

	TGraphAsymmErrors *g_Eff_Corr_HLTv4p2;
	TGraphAsymmErrors *g_Eff_Corr_HLTv4p3;
	TGraphAsymmErrors *g_EffCorr_HLTv4p2;
	TGraphAsymmErrors *g_EffCorr_HLTv4p3;
	TH1D *h_yield_EffCorr;
	TH1D *h_yield_HLTv4p2_EffCorr;
	TH1D *h_yield_HLTv4p3_EffCorr;

	TH1D *h_Truth_preFSR;
	TH1D *h_yield_FSRCorr;

	TH1D *h_xSec_Raw;
	TH1D *h_xSec_Unfolded;
	TH1D *h_xSec_Unfolded_AccEff;
	TH1D *h_xSec_FSRCorr;
	TH1D *h_xSec_aMCNLO;

	TH1D *h_DiffXsec_Raw;
	TH1D *h_DiffXsec_Unfolded;
	TH1D *h_DiffXsec_Unfolded_AccEff;
	TH1D *h_DiffXsec_FSRCorr;
	TH1D *h_DiffXsec_aMCNLO;
	TH1D *h_DiffXsec_FEWZ_NLO;
	TH1D *h_DiffXsec_FEWZ_NNLO;

	TH1D *h_RelUnc_Stat;

	// -- fiducial, post-FSR results -- //
	TH1D *h_FpoF_yield_aMCNLO;
	TH1D *h_FpoF_DiffXsec_aMCNLO;

	TH1D *h_FpoF_yield_Unfolded_Eff;
	TH1D *h_FpoF_yield_HLTv4p2_Unfolded_Eff;
	TH1D *h_FpoF_yield_HLTv4p3_Unfolded_Eff;

	TH1D *h_FpoF_yield_EffCorr;
	TH1D *h_FpoF_yield_HLTv4p2_EffCorr;
	TH1D *h_FpoF_yield_HLTv4p3_EffCorr;

	TH1D *h_FpoF_DiffXsec_Data;

	DiffXsecTools();
	// ~DiffXsecTools();
	virtual void MakeSignalMCHistograms();
	virtual void GetYieldHistograms(Bool_t isDataDriven);
	virtual void SaveCanvas_RecoLevel_Data_vs_recoMC();
	virtual void BinByBinCorr_NoGenAcc();
	virtual void UnfoldingCorrection();
	virtual void SaveCanvas_UnfoldingCorrection();

	virtual void AccEffCorrection();
	virtual void SaveCanvas_AccEffCorrection();

	virtual void AccEffCorrection_ToPreUnfolded();
	virtual void SaveCanvas_AccEffCorrection_ToPreUnfolded();

	virtual void Calc_EffSF();
	virtual void EfficiencyScaleFactor();
	virtual void SaveCanvas_EfficiencyCorrection();

	virtual void FSRCorrection();
	virtual void SaveCanvas_FSRCorrection();

	virtual void CalcXsec();
	virtual void SaveCanvas_CalcXsec();

	virtual void SaveHistograms();
	// virtual void EstimationUncertainty();

	// -- fiducial, post-FSR -- //
	virtual void FpoF_AllAnalysisProcedure();
	virtual void FpoF_SaveResults();

	virtual void FpoF_SaveCanvas_DiffXsec_Data_vs_aMCNLO();
	virtual void FpoF_CalcXsec();
	virtual void FpoF_EfficiencyScaleFactor();
	virtual void FpoF_EffCorrection();
	virtual void FpoF_GetTheoryHist();

protected:
	void BinByBinCorrection_NoGen_ButReco( TH1D *h_yield );
	void SaveCanvas_NoGenAcc_vs_TotalMC();

	void SaveCanvas_preUnfold_vs_genMC( TString Type, TH1D* h_yield, TH1D* h_Truth_WithinAcc );
	void SaveCanvas_Unfold_vs_genMC( TString Type, TH1D* h_yield_Unfolded, TH1D* h_Truth_WithinAcc );
	void SaveCanvas_preUnfold_vs_Unfold( TString Type, TH1D* h_yield, TH1D* h_yield_Unfolded );
	
	void Correction_AccEff(TH1D *h_yield_AccEff, TH1D *h_yield, TGraphAsymmErrors *g_AccEff);
	Double_t CalcError_Yield_AccEff(Double_t Yield_AfterAccEff, Double_t Yield, Double_t sigma_Yield, Double_t AccEff, Double_t sigma_AccEff);
	void SaveCanvas_YieldAccEff_Unfold_vs_genMC( TString Type, TH1D* h_yield_Unfolded_AccEff, TH1D* h_totSignalMC_GenLevel );
	void SaveCanvas_YieldAccEff_preUnfold_vs_genMC( TString Type, TH1D* h_yield_AccEff, TH1D* h_totSignalMC_GenLevel);
	
	void MakeRatioGraph(TGraphAsymmErrors *g_ratio, TGraphAsymmErrors *g1, TGraphAsymmErrors *g2);
	Double_t ReturnLargerValue(Double_t a, Double_t b);
	Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B);
	void SaveCanvas_EffCorr_HLTv4p2_vs_HLTv4p3();
	void ApplyEffCorr_Yield(TH1D *h_yield_EffCorr, TH1D *h_yield, TGraphAsymmErrors* g_EffCorr);
	void SaveCanvas_Yield_UnCorr_vs_EffCorr( TString Type, TH1D* h_yield_EffCorr, TH1D* h_yield );
	void SaveCanvas_Yield_EffCorr_vs_genMC( TString Type, TH1D *h_yield_EffCorr, TH1D *h_totSignalMC_GenLevel  );
	
	void SaveCanvas_beforeFSRCorr_vs_AfterFSRCorr( TH1D* h_FSRCorrected, TH1D* h_yield_Unfolded_AccEff );
	void SaveCanvas_beforeFSRCorr_vs_genMC_preFSR( TH1D* h_yield_Unfolded_AccEff, TH1D* h_Truth_preFSR );
	void SaveCanvas_afterFSRCorr_vs_genMC_preFSR( TH1D* h_FSRCorrected, TH1D* h_Truth_preFSR );
	
	void Obtain_dSigma_dM(TH1D *h);
	void CalcHistogram_RelStatUnc();
	void SetStatUnc_DiffXsec(TH1D* h_DiffXsec);
	void SaveCanvas_xSec_Data_vs_aMCNLO();
	void SaveCanvas_DiffXsec_Data_AllCorrStep();
	virtual void SaveCanvas_DiffXsec_Data_vs_FEWZ(TString Type, TH1D *h_DiffXsec_FEWZ);
	void SaveCanvas_DiffXsec_Data_vs_aMCNLO();

	void PrintHistogram( TH1D *h );

};

// -- default contructor -- //
DiffXsecTools::DiffXsecTools()
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
										 830, 1000, 1500, 3000};

	for(Int_t i=0; i<nMassBin+1; i++)
		MassBinEdges[i] = MassBinEdges_temp[i];


	f_MC = NULL;
	f_data = NULL;
	f_bkg_dataDriven = NULL;
	f_yield = NULL;
	f_AccEff = NULL;
	f_Unfold = NULL;
	f_FSR = NULL;
	f_theory = NULL;

	g_EffCorr_HLTv4p2 = NULL;
	g_EffCorr_HLTv4p3 = NULL;
	h_RelUnc_Stat = NULL;

	// -- Setup input files -- //
	TString Path_CommonCodes = gSystem->Getenv("KP_INCLUDE_PATH");
	FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");

	// -- Setup sample information -- //
	DYAnalyzer *analyzer = new DYAnalyzer( "None" );
	analyzer->SetupMCsamples_v20160309_76X_MiniAODv2( "Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents );

	TH1::AddDirectory(kFALSE);
}

void DiffXsecTools::MakeSignalMCHistograms()
{
	f_MC = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root"); f_MC->cd();

	if( f_MC == NULL )
	{
		cout << "f_MC is not loaded properly!" << endl;
		return;
	}

	///////////////////////////////////////////
 	// -- Make reco-level total signal MC -- //
	///////////////////////////////////////////
 	h_totSignalMC = NULL;
 	
 	Int_t nTag = Tag.size();
 	for(Int_t i_tag=0; i_tag < nTag; i_tag++)
 	{
 		TH1D *h_temp = (TH1D*)f_MC->Get( "h_mass_OS_"+Tag[i_tag] )->Clone();

 		// -- Rebin using DY analysis binning -- //
 		h_temp = (TH1D*)h_temp->Rebin(nMassBin, h_temp->GetName(), MassBinEdges);
 		
 		// -- Normalized to the integrated luminosity of the data -- //
 		Double_t Norm = (Lumi * Xsec[i_tag]) / nEvents[i_tag];
 		h_temp->Scale( Norm );

 		if( Tag[i_tag].Contains("DYMuMu") ) // -- if this is signal MC -- //
 		{
 			// -- Sum of all signal MC histogram -- //
 			if( h_totSignalMC == NULL )
 				h_totSignalMC = (TH1D*)h_temp->Clone();
 			else
 				h_totSignalMC->Add( h_temp ); 			
 		}
 	}

 	h_totSignalMC_HLTv4p2 = (TH1D*)h_totSignalMC->Clone();
 	h_totSignalMC_HLTv4p2->Scale( Lumi_HLTv4p2 / Lumi );

 	h_totSignalMC_HLTv4p3 = (TH1D*)h_totSignalMC->Clone();
 	h_totSignalMC_HLTv4p3->Scale( (Lumi - Lumi_HLTv4p2) / Lumi );


 	////////////////////////////////////////////////////////
 	// -- Get the gen-level MC distribution (post-FSR) -- //
 	////////////////////////////////////////////////////////
 	h_totSignalMC_GenLevel = NULL;
 	// for(Int_t i_tag=0; i_tag < nTag; i_tag++)
 	// {
 	// 	if( Tag[i_tag].Contains("DYMuMu") )
 	// 	{
 	// 		TH1D *h_temp = (TH1D*)f_MC->Get( "h_GenMass_"+Tag[i_tag] )->Clone();

 	// 		// -- Rebin using DY analysis binning -- //
 	// 		h_temp = (TH1D*)h_temp->Rebin(nMassBin, h_temp->GetName(), MassBinEdges);
 			
 	// 		// -- Normalized to the integrated luminosity of the data -- //
 	// 		Double_t Norm = (Lumi * Xsec[i_tag]) / nEvents[i_tag];
 	// 		h_temp->Scale( Norm );

 	// 		// -- Sum of all background MC histogram -- //
 	// 		if( h_totSignalMC_GenLevel == NULL )
 	// 			h_totSignalMC_GenLevel = (TH1D*)h_temp->Clone();
 	// 		else
 	// 			h_totSignalMC_GenLevel->Add( h_temp );
 	// 	}
 	// }

 	f_FSR = TFile::Open(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
 	h_totSignalMC_GenLevel = (TH1D*)f_FSR->Get("h_mass_postFSR")->Clone();

 	h_totSignalMC_GenLevel_HLTv4p2 = (TH1D*)h_totSignalMC_GenLevel->Clone();
 	h_totSignalMC_GenLevel_HLTv4p2->Scale( Lumi_HLTv4p2 / Lumi );

 	h_totSignalMC_GenLevel_HLTv4p3 = (TH1D*)h_totSignalMC_GenLevel->Clone();
 	h_totSignalMC_GenLevel_HLTv4p3->Scale( (Lumi - Lumi_HLTv4p2) / Lumi );

 	delete f_MC;
 	delete f_FSR;
}

////////////////////////////////////////////////////////////////
// -- Background subtraction for each HLTv4.2 and 4.3 data -- //
////////////////////////////////////////////////////////////////
void DiffXsecTools::GetYieldHistograms(Bool_t isDataDriven)
{
	TString Type = "";
	
	if( isDataDriven ) 
		Type = "_DataDrivenBkg";
	else
		Type = "_MCBasedBkg";

	// f_yield = new TFile(FileLocation + "/ROOTFile_YieldHistogram.root"); f_yield->cd();
	f_yield = TFile::Open(FileLocation + "/ROOTFile_YieldHistogram.root"), f_yield->cd();
	h_yield_HLTv4p2 = (TH1D*)f_yield->Get("h_yield_OS_HLTv4p2"+Type)->Clone();
	h_yield_HLTv4p3 = (TH1D*)f_yield->Get("h_yield_OS_HLTv4p3"+Type)->Clone();
	delete f_yield;

	h_yield_Raw = (TH1D*)h_yield_HLTv4p2->Clone();
	h_yield_Raw->Add( h_yield_HLTv4p3 );
}

void DiffXsecTools::SaveCanvas_RecoLevel_Data_vs_recoMC()
{
	// -- HLT v4.2 + HLT v4.3 -- //
	MyCanvas *myc = new MyCanvas("c_RecoLevel_Data_vs_recoMC", "Dimuon Mass [GeV]", "Events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_Raw->Clone(), (TH1D*)h_totSignalMC->Clone(),
											"Data", "Signal MC(reco-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc->PrintCanvas();


	// -- HLT v4.2 -- //
	MyCanvas *myc_HLTv4p2 = new MyCanvas("c_RecoLevel_Data_vs_recoMC_HLTv4p2", "Dimuon Mass [GeV]", "Events");
	myc_HLTv4p2->SetLogx(1);
	myc_HLTv4p2->SetLogy(0);
	myc_HLTv4p2->SetYRange(2e-2, 5e6);

	myc_HLTv4p2->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_HLTv4p2->Clone(), (TH1D*)h_totSignalMC_HLTv4p2->Clone(),
											"Data (HLT v4.2)", "Signal MC(reco-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc_HLTv4p2->PrintCanvas();


	// -- HLT v4.3 -- //
	MyCanvas *myc_HLTv4p3 = new MyCanvas("c_RecoLevel_Data_vs_recoMC_HLTv4p3", "Dimuon Mass [GeV]", "Events");
	myc_HLTv4p3->SetLogx(1);
	myc_HLTv4p3->SetLogy(0);
	myc_HLTv4p3->SetYRange(2e-2, 5e6);

	myc_HLTv4p3->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_HLTv4p3->Clone(), (TH1D*)h_totSignalMC_HLTv4p3->Clone(),
											"Data (HLT v4.3)", "Signal MC(reco-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc_HLTv4p3->PrintCanvas();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -- bin-by-bin correction for the events which has reconstructed events without passing acceptance cut in generator level -- //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DiffXsecTools::BinByBinCorr_NoGenAcc()
{
	// f_Unfold = new TFile(FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root"); f_Unfold->cd();
	f_Unfold = TFile::Open(FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root"); f_Unfold->cd();
	h_NoGen_Reco = (TH1D*)f_Unfold->Get("h_NoGen_Reco")->Clone();
	delete f_Unfold;

	this->BinByBinCorrection_NoGen_ButReco( h_yield_HLTv4p2 );
	this->BinByBinCorrection_NoGen_ButReco( h_yield_HLTv4p3 );
}

// -- Comparison: signal MC vs. "NoGenAcc" Events -- //
void DiffXsecTools::SaveCanvas_NoGenAcc_vs_TotalMC()
{
	MyCanvas *myc = new MyCanvas("c_Measured_vs_NoGenButReco", "Dimuon Mass (Reco-level) [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);
	myc->SetRatioRange(0, 0.05);

	myc->CanvasWithHistogramsRatioPlot(h_NoGen_Reco, h_totSignalMC, "NoGenAcc (MC)", "Total reco-level (MC)", "NoGenAcc/Total");
	myc->PrintCanvas();
}

void DiffXsecTools::BinByBinCorrection_NoGen_ButReco( TH1D *h_yield )
{
	TH1D *h_yield_beforeCorr = (TH1D*)h_yield->Clone();
	TH1D *h_ratio = (TH1D*)h_totSignalMC->Clone();
	h_ratio->SetName("h_ratio");

	h_totSignalMC->Sumw2(); h_NoGen_Reco->Sumw2();
	h_ratio->Divide( h_NoGen_Reco, h_totSignalMC );

	this->PrintHistogram( h_ratio );

	Int_t nBin = h_yield->GetNbinsX();
	for(Int_t i=-1; i<nBin+1; i++) // -- Include under/overflow -- //
	{
		Int_t i_bin = i+1;
		Double_t yield_beforeCorr = h_yield->GetBinContent(i_bin);
		Double_t error_beforeCorr = h_yield->GetBinError(i_bin);

		Double_t ratio = h_ratio->GetBinContent(i_bin);
		Double_t Corr = 0;
		if( ratio < 0 )
			Corr = 1;
		else
			Corr = 1 - h_ratio->GetBinContent(i_bin);

		Double_t yield_afterCorr = yield_beforeCorr * Corr;
		Double_t error_afterCorr = error_beforeCorr * Corr;

		h_yield->SetBinContent(i_bin, yield_afterCorr );
		h_yield->SetBinError(i_bin, error_afterCorr );

		printf("[%2d bin: Corr = %9.5lf] (yield: %8.1lf -> %8.1lf), (error: %10.5lf -> %10.5lf)\n", i_bin, Corr, yield_beforeCorr, yield_afterCorr, error_beforeCorr, error_afterCorr);
	}

	TString Type = "";
	TString HistName = h_yield->GetName();
	if( HistName.Contains("HLTv4p2") ) Type = "HLTv4p2";
	else if( HistName.Contains("HLTv4p3") ) Type = "HLTv4p3";

	MyCanvas *myc = new MyCanvas("c_RecoYield_BinbyBinCorr_Before_vs_After_"+Type, "Dimuon Mass (Reco-level) [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetRatioRange(0.9, 1.1);
	myc->CanvasWithHistogramsRatioPlot( h_yield, h_yield_beforeCorr, "Data("+Type+") (After Corr.)", "Data("+Type+") (Before Corr.)", "after/before");
	myc->PrintCanvas();
}

void DiffXsecTools::PrintHistogram( TH1D *h )
{
	cout << "========================================================================================================================" << endl;
	TString HistName = h->GetName();
	cout << "HistName = " << HistName << endl;

	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Int_t i_bin = i+1;

		Double_t value = h->GetBinContent(i_bin);
		Double_t error = h->GetBinError(i_bin);

		Double_t RelError = 0;
		if( value == 0 )
			RelError = 0;
		else
			RelError = error / value;

		printf("[%.2d bin] ", i_bin);
		printf("(value, error, RelError) = (%12.3lf, %12.5lf, %12.5lf)\n", value, error, RelError);
	}
	cout << "========================================================================================================================" << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -- Unfolding correction for detector resolution: apply on the data before applying Acc*Eff correction -- //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DiffXsecTools::UnfoldingCorrection()
{
	if( f_Unfold == NULL )
		// f_Unfold = new TFile(FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root");
		f_Unfold = TFile::Open(FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root");
	
	f_Unfold->cd();
	h_totSignalMC_GenLevel_WithinAcc = (TH1D*)f_Unfold->Get("h_Truth_RooUnfold")->Clone();

	// -- HLTv4.2 -- //
	RooUnfoldResponse *UnfoldRes_HLTv4p2 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
	// RooUnfoldBayes *UnfoldBayes_HLTv4p2 = new RooUnfoldBayes(UnfoldRes_HLTv4p2, h_yield_HLTv4p2, 4);
	RooUnfoldBayes *UnfoldBayes_HLTv4p2 = new RooUnfoldBayes(UnfoldRes_HLTv4p2, h_yield_HLTv4p2, 17); // -- iteration until convergence -- //
	h_yield_HLTv4p2_Unfolded = (TH1D*)UnfoldBayes_HLTv4p2->Hreco();
	h_yield_HLTv4p2_Unfolded->SetName("h_yield_HLTv4p2_Unfolded");
	
	h_totSignalMC_GenLevel_WithinAcc_HLTv4p2 = (TH1D*)h_totSignalMC_GenLevel_WithinAcc->Clone();
	h_totSignalMC_GenLevel_WithinAcc_HLTv4p2->Scale( Lumi_HLTv4p2 / Lumi );

	// -- HLTv4.3 -- //
	RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
	// RooUnfoldBayes *UnfoldBayes_HLTv4p3 = new RooUnfoldBayes(UnfoldRes_HLTv4p3, h_yield_HLTv4p3, 4);
	RooUnfoldBayes *UnfoldBayes_HLTv4p3 = new RooUnfoldBayes(UnfoldRes_HLTv4p3, h_yield_HLTv4p3, 17); // -- iteration until convergence -- //
	h_yield_HLTv4p3_Unfolded = (TH1D*)UnfoldBayes_HLTv4p3->Hreco();
	h_yield_HLTv4p3_Unfolded->SetName("h_yield_HLTv4p3_Unfolded");

	h_totSignalMC_GenLevel_WithinAcc_HLTv4p3 = (TH1D*)h_totSignalMC_GenLevel_WithinAcc->Clone();
	h_totSignalMC_GenLevel_WithinAcc_HLTv4p3->Scale( (Lumi - Lumi_HLTv4p2) / Lumi );

	delete f_Unfold;

	// -- Combined -- //
	h_yield_HLTv4p2_Unfolded->Sumw2(); h_yield_HLTv4p3_Unfolded->Sumw2();
	h_yield_Unfolded = (TH1D*)h_yield_HLTv4p2_Unfolded->Clone();
	h_yield_Unfolded->Add( h_yield_HLTv4p3_Unfolded );
}

void DiffXsecTools::SaveCanvas_UnfoldingCorrection()
{
	this->SaveCanvas_preUnfold_vs_Unfold( "HLTv4p2", h_yield_HLTv4p2, h_yield_HLTv4p2_Unfolded );
	this->SaveCanvas_preUnfold_vs_Unfold( "HLTv4p3", h_yield_HLTv4p3, h_yield_HLTv4p3_Unfolded );

	this->SaveCanvas_preUnfold_vs_genMC( "HLTv4p2", h_yield_HLTv4p2, h_totSignalMC_GenLevel_WithinAcc_HLTv4p2 );
	this->SaveCanvas_preUnfold_vs_genMC( "HLTv4p3", h_yield_HLTv4p3, h_totSignalMC_GenLevel_WithinAcc_HLTv4p3 );

	this->SaveCanvas_Unfold_vs_genMC( "HLTv4p2", h_yield_HLTv4p2_Unfolded, h_totSignalMC_GenLevel_WithinAcc_HLTv4p2 );
	this->SaveCanvas_Unfold_vs_genMC( "HLTv4p3", h_yield_HLTv4p3_Unfolded, h_totSignalMC_GenLevel_WithinAcc_HLTv4p3 );
}

void DiffXsecTools::SaveCanvas_preUnfold_vs_Unfold( TString Type, TH1D* h_yield, TH1D* h_yield_Unfolded )
{
	MyCanvas *myc = new MyCanvas("c_preUnfold_vs_Unfold_"+Type, "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield->Clone(), (TH1D*)h_yield_Unfolded->Clone(),
											"Yield("+Type+", Pre-Unfolded)", "Yield("+Type+", Unfolded)", "PreUnfolded/Unfolded",
											kBlack, kRed,
											kFALSE, kFALSE,
											"HIST", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_preUnfold_vs_genMC( TString Type, TH1D* h_yield, TH1D* h_Truth_WithinAcc )
{
	MyCanvas *myc = new MyCanvas("c_preUnfold_vs_genMC_"+Type, "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield->Clone(), (TH1D*)h_Truth_WithinAcc->Clone(),
											"Yield(" + Type + ")", "MC(gen-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_Unfold_vs_genMC( TString Type, TH1D* h_yield_Unfolded, TH1D* h_Truth_WithinAcc )
{
	MyCanvas *myc = new MyCanvas("c_Unfold_vs_genMC_"+Type, "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_Unfolded->Clone(), (TH1D*)h_Truth_WithinAcc->Clone(),
											"Yield("+Type+",Unfolded)", "MC(gen-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::AccEffCorrection()
{
	//////////////////////////////
	// -- Acc*Eff Correction -- //
	//////////////////////////////
	// f_AccEff = new TFile(FileLocation + "/ROOTFile_AccEff.root"); f_AccEff->cd();
	f_AccEff = TFile::Open(FileLocation + "/ROOTFile_AccEff.root"); f_AccEff->cd();
	TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)f_AccEff->Get("g_AccEff");
	// f_AccEff->Close();

	// -- HLTv4p2 -- //
	h_yield_HLTv4p2_Unfolded_AccEff = (TH1D*)h_yield_HLTv4p2_Unfolded->Clone();
	h_yield_HLTv4p2_Unfolded_AccEff->SetName("h_yield_HLTv4p2_Unfolded_AccEff");
	this->Correction_AccEff(h_yield_HLTv4p2_Unfolded_AccEff, h_yield_HLTv4p2_Unfolded, g_AccEff);

	// -- HLTv4p3 -- //
	h_yield_HLTv4p3_Unfolded_AccEff = (TH1D*)h_yield_HLTv4p3_Unfolded->Clone();
	h_yield_HLTv4p3_Unfolded_AccEff->SetName("h_yield_HLTv4p3_Unfolded_AccEff");
	this->Correction_AccEff(h_yield_HLTv4p3_Unfolded_AccEff, h_yield_HLTv4p3_Unfolded, g_AccEff);

	// -- Combine -- //
	h_yield_HLTv4p2_Unfolded_AccEff->Sumw2(); h_yield_HLTv4p3_Unfolded_AccEff->Sumw2();
	h_yield_Unfolded_AccEff = (TH1D*)h_yield_HLTv4p2_Unfolded_AccEff->Clone();
	h_yield_Unfolded_AccEff->Add( h_yield_HLTv4p3_Unfolded_AccEff );

}

void DiffXsecTools::AccEffCorrection_ToPreUnfolded()
{
	if( f_AccEff == NULL )
		// f_AccEff = new TFile(FileLocation + "/ROOTFile_AccEff.root");
		f_AccEff = TFile::Open(FileLocation + "/ROOTFile_AccEff.root"); 

	f_AccEff->cd();
	TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)f_AccEff->Get("g_AccEff");
	// f_AccEff->Close();

	// -- HLT v4.2 -- //
	h_yield_HLTv4p2_AccEff = (TH1D*)h_yield_HLTv4p2->Clone();
	h_yield_HLTv4p2_AccEff->SetName("h_yield_HLTv4p2_AccEff");
	this->Correction_AccEff(h_yield_HLTv4p2_AccEff, h_yield_HLTv4p2, g_AccEff);

	// -- HLT v4.3 -- //
	h_yield_HLTv4p3_AccEff = (TH1D*)h_yield_HLTv4p3->Clone();
	h_yield_HLTv4p3_AccEff->SetName("h_yield_HLTv4p3_AccEff");
	this->Correction_AccEff(h_yield_HLTv4p3_AccEff, h_yield_HLTv4p3, g_AccEff);

	// -- Combine -- //
	h_yield_HLTv4p2_AccEff->Sumw2(); h_yield_HLTv4p3_AccEff->Sumw2();
	h_yield_AccEff = (TH1D*)h_yield_HLTv4p2_AccEff->Clone();
	h_yield_AccEff->Add( h_yield_HLTv4p3_AccEff );
}

void DiffXsecTools::SaveCanvas_AccEffCorrection()
{
	this->SaveCanvas_YieldAccEff_Unfold_vs_genMC( "HLTv4p2", h_yield_HLTv4p2_Unfolded_AccEff, h_totSignalMC_GenLevel_HLTv4p2 );
	this->SaveCanvas_YieldAccEff_Unfold_vs_genMC( "HLTv4p3", h_yield_HLTv4p3_Unfolded_AccEff, h_totSignalMC_GenLevel_HLTv4p3 );


}

void DiffXsecTools::SaveCanvas_AccEffCorrection_ToPreUnfolded()
{
	this->SaveCanvas_YieldAccEff_preUnfold_vs_genMC( "HLTv4p2", h_yield_HLTv4p2_AccEff, h_totSignalMC_GenLevel_HLTv4p2 );
	this->SaveCanvas_YieldAccEff_preUnfold_vs_genMC( "HLTv4p3", h_yield_HLTv4p3_AccEff, h_totSignalMC_GenLevel_HLTv4p3 );
}


void DiffXsecTools::Correction_AccEff(TH1D *h_yield_AccEff, TH1D *h_yield, TGraphAsymmErrors *g_AccEff)
{
	Int_t nBins = h_yield->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Int_t i_bin = i+1;

		Double_t x_AccEff, y_AccEff;
		g_AccEff->GetPoint(i, x_AccEff, y_AccEff);

		Double_t Yield = h_yield->GetBinContent(i_bin);
		Double_t Yield_AfterAccEff = Yield / y_AccEff;

		// -- Set the central value -- //
		h_yield_AccEff->SetBinContent( i_bin, Yield_AfterAccEff );

		// -- Calculate the error -- //
		Double_t Error = CalcError_Yield_AccEff(Yield_AfterAccEff, 
												Yield, sqrt(Yield), 
												y_AccEff, g_AccEff->GetErrorYhigh(i) );
		if( Error != 0 )
			h_yield_AccEff->SetBinError(i_bin, Error);
	}
}

Double_t DiffXsecTools::CalcError_Yield_AccEff(Double_t Yield_AfterAccEff, Double_t Yield, Double_t sigma_Yield, Double_t AccEff, Double_t sigma_AccEff)
{
	if( Yield <= 0 || AccEff == 0 )
		return 0;
	Double_t Partial_Yield = ( sigma_Yield / Yield ) * ( sigma_Yield / Yield );
	Double_t Partial_AccEff = ( sigma_AccEff / AccEff ) * ( sigma_AccEff / AccEff );

	Double_t error = fabs(Yield_AfterAccEff) * sqrt( Partial_Yield + Partial_AccEff );
	return error;
}

void DiffXsecTools::SaveCanvas_YieldAccEff_Unfold_vs_genMC( TString Type, TH1D* h_yield_Unfolded_AccEff, TH1D* h_totSignalMC_GenLevel )
{
	MyCanvas *myc = new MyCanvas("c_YieldAccEff_Unfold_vs_genMC_"+Type, "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_Unfolded_AccEff->Clone(), (TH1D*)h_totSignalMC_GenLevel->Clone(),
											"Yield("+Type+",Unfolded,AccEff)", "MC(gen-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_YieldAccEff_preUnfold_vs_genMC( TString Type, TH1D* h_yield_AccEff, TH1D* h_totSignalMC_GenLevel)
{
	MyCanvas *myc = new MyCanvas("c_YieldAccEff_preUnfold_vs_genMC_"+Type, "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx(1);
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_AccEff->Clone(), (TH1D*)h_totSignalMC_GenLevel->Clone(),
											"Yield("+Type+",AccEff)", "MC(gen-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::Calc_EffSF()
{
	///////////////////////////////////////////////////////
	// -- Calculation of efficiency correction factor -- //
	///////////////////////////////////////////////////////
	if( this->f_AccEff == NULL )
		this->f_AccEff = TFile::Open(FileLocation + "/ROOTFile_AccEff.root"); 

	TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff")->Clone();
	this->g_Eff_Corr_HLTv4p2 = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff_Corr_HLTv4p2")->Clone();
	this->g_Eff_Corr_HLTv4p3 = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff_Corr_HLTv4p3")->Clone();
	delete f_AccEff;

	// -- Calculate efficiency scale factor for each mass bin: SF = Corrected Eff / Un-corrected Eff -- //
	this->g_EffCorr_HLTv4p2 = (TGraphAsymmErrors*)g_Eff->Clone();
	this->MakeRatioGraph(g_EffCorr_HLTv4p2, g_Eff_Corr_HLTv4p2, g_Eff);

	this->g_EffCorr_HLTv4p3 = (TGraphAsymmErrors*)g_Eff->Clone();
	this->MakeRatioGraph(g_EffCorr_HLTv4p3, g_Eff_Corr_HLTv4p3, g_Eff);
}

void DiffXsecTools::EfficiencyScaleFactor()
{
	if( g_EffCorr_HLTv4p2 == NULL || g_EffCorr_HLTv4p3 == NULL )
		this->Calc_EffSF();

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

void DiffXsecTools::MakeRatioGraph(TGraphAsymmErrors *g_ratio, TGraphAsymmErrors *g1, TGraphAsymmErrors *g2)
{
	g_ratio->Set(0); // -- Remove all points (reset) -- //

	Int_t NPoints = g1->GetN();
	for(Int_t i_p=0; i_p<NPoints; i_p++)
	{
		// -- Get g1 point -- //
		Double_t x1, y1;
		g1->GetPoint(i_p, x1, y1);
		Double_t error1 = ReturnLargerValue( g1->GetErrorYhigh(i_p), g1->GetErrorYlow(i_p) );

		// -- Get g2 point -- //
		Double_t x2, y2;
		g2->GetPoint(i_p, x2, y2);
		Double_t error2 = ReturnLargerValue( g2->GetErrorYhigh(i_p), g2->GetErrorYlow(i_p) );

		Double_t ratio;
		Double_t ratio_error;
		if(y1 != 0 && error1 != 0 && y2 != 0 && error2 != 0)
		{
			// -- calculate ratio & error -- //
			ratio = y1 / y2;
			ratio_error = Error_PropagatedAoverB(y1, error1, y2, error2);
		}
		else if( y1 != 0 && y2 != 0 && (error1 == 0 || error2 == 0) )
		{
			ratio = y1 / y2;
			ratio_error = 0;
		}
		else
		{
			ratio = 0;
			ratio_error = 0;
		}

		// -- Set Central value -- //
		g_ratio->SetPoint(i_p, x1, ratio);

		// -- Set the error -- //
		Double_t error_XLow = g1->GetErrorXlow(i_p);
		Double_t error_Xhigh = g1->GetErrorXhigh(i_p);
		g_ratio->SetPointError(i_p, error_XLow, error_Xhigh, ratio_error, ratio_error);

	}
}

void DiffXsecTools::SaveCanvas_EffCorr_HLTv4p2_vs_HLTv4p3()
{
	MyCanvas *myc_EffCorr = new MyCanvas("c_EffCorr_HLTv4p2_vs_HLTv4p3", "Dimuon Mass [GeV]", "Correction");
	myc_EffCorr->isLogX = kTRUE;
	myc_EffCorr->LowerEdge_Ratio = 0.95; myc_EffCorr->UpperEdge_Ratio = 1.05;
	myc_EffCorr->LowerEdge_Y = 0.8; myc_EffCorr->UpperEdge_Y = 1.1;
	myc_EffCorr->Legend_x1 = 0.55;

	myc_EffCorr->CanvasWithGraphRatioPlot(g_EffCorr_HLTv4p2, g_EffCorr_HLTv4p3,
									"EffCorr (HLTv4.2)", "EffCorr (HLTv4.3)", "HLTv4.2/HLTv4.3",
									kOrange+1, kGreen+1 );

	myc_EffCorr->c->SaveAs("c_EffCorr_HLTv4p2_vs_HLTv4p3.pdf");
}

void DiffXsecTools::ApplyEffCorr_Yield(TH1D *h_yield_EffCorr, TH1D *h_yield, TGraphAsymmErrors* g_EffCorr)
{
	Int_t NPoints = g_EffCorr->GetN();
	Int_t nBins = h_yield->GetNbinsX();

	if( NPoints != nBins )
	{
		cout << "# Points in EffCorr != # Bins of yield histogram!" << endl;
		return;
	}

	for(Int_t i_p=0; i_p<NPoints; i_p++)
	{
		// -- Get g_EffCorr point -- //
		Double_t mass, EffCorr;
		g_EffCorr->GetPoint(i_p, mass, EffCorr);
		Double_t error_EffCorr = ReturnLargerValue( g_EffCorr->GetErrorYhigh(i_p), g_EffCorr->GetErrorYlow(i_p) );

		Int_t i_bin = i_p + 1;
		Double_t yield = h_yield->GetBinContent(i_bin);
		Double_t error_yield = h_yield->GetBinError(i_bin);

		Double_t yield_EffCorr = yield / EffCorr;
		Double_t error_yield_EffCorr = CalcError_Yield_AccEff(yield_EffCorr, yield, error_yield, EffCorr, error_EffCorr);

		// printf("[%d bin] (yield, EffCorr, yield_EffCorr) = (%.3lf, %.3lf, %.3lf)\n", i_bin, yield, EffCorr, yield_EffCorr);

		h_yield_EffCorr->SetBinContent(i_bin, yield_EffCorr);
		h_yield_EffCorr->SetBinError(i_bin, error_yield_EffCorr);
	}

}

void DiffXsecTools::SaveCanvas_EfficiencyCorrection()
{
	this->SaveCanvas_EffCorr_HLTv4p2_vs_HLTv4p3();

	this->SaveCanvas_Yield_UnCorr_vs_EffCorr("HLTv4p2", h_yield_HLTv4p2_EffCorr, h_yield_HLTv4p2_Unfolded_AccEff);
	this->SaveCanvas_Yield_UnCorr_vs_EffCorr("HLTv4p3", h_yield_HLTv4p3_EffCorr, h_yield_HLTv4p3_Unfolded_AccEff);

	this->SaveCanvas_Yield_EffCorr_vs_genMC("HLTv4p2", h_yield_HLTv4p2_EffCorr, h_totSignalMC_GenLevel_HLTv4p2);
	this->SaveCanvas_Yield_EffCorr_vs_genMC("HLTv4p3", h_yield_HLTv4p3_EffCorr, h_totSignalMC_GenLevel_HLTv4p3);
}

void DiffXsecTools::SaveCanvas_Yield_UnCorr_vs_EffCorr( TString Type, TH1D* h_yield_EffCorr, TH1D* h_yield )
{
	MyCanvas *myc = new MyCanvas("c_Yield_UnCorr_vs_EffCorr_"+Type, "Dimuon Mass [GeV]", "Events");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);
	myc->SetRatioRange(0.95, 1.1);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_EffCorr->Clone(), (TH1D*)h_yield->Clone(),
											"Yield("+Type+", EffCorr)", "Yield("+Type+", UnCorr)", "Corr/UnCorr",
											kRed, kBlack,
											kFALSE, kFALSE, 
											"EP", "EPSAME");
	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_Yield_EffCorr_vs_genMC( TString Type, TH1D *h_yield_EffCorr, TH1D *h_totSignalMC_GenLevel  )
{
	MyCanvas *myc = new MyCanvas("c_yield_EffCorr_vs_genMC_"+Type, "Dimuon Mass [GeV]", "Events");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_EffCorr->Clone(), (TH1D*)h_totSignalMC_GenLevel->Clone(),
											"Yield("+Type+", EffCorr)", "Signal MC(gen-level)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::FSRCorrection()
{
	//////////////////////////
	// -- FSR Correction -- //
	//////////////////////////
	// f_FSR = new TFile(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
	f_FSR = TFile::Open(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
	h_Truth_preFSR = (TH1D*)f_FSR->Get("h_mass_preFSR")->Clone();

	RooUnfoldResponse *UnfoldRes_FSR = (RooUnfoldResponse*)f_FSR->Get("UnfoldRes")->Clone();

	delete f_FSR;

	// RooUnfoldBayes *UnfoldBayes_FSR = new RooUnfoldBayes(UnfoldRes_FSR, h_yield_EffCorr, 4);
	// h_yield_FSRCorr = (TH1D*)UnfoldBayes_FSR->Hreco();

	RooUnfoldInvert *UnfoldInvert_FSR = new RooUnfoldInvert(UnfoldRes_FSR, h_yield_EffCorr); // -- use inverting method after following recommendation by stat.committee -- //
	h_yield_FSRCorr = (TH1D*)UnfoldInvert_FSR->Hreco();
	
}

void DiffXsecTools::SaveCanvas_FSRCorrection()
{
	this->SaveCanvas_beforeFSRCorr_vs_genMC_preFSR( h_yield_EffCorr, h_Truth_preFSR );
	this->SaveCanvas_afterFSRCorr_vs_genMC_preFSR( h_yield_FSRCorr, h_Truth_preFSR );
	this->SaveCanvas_beforeFSRCorr_vs_AfterFSRCorr( h_yield_FSRCorr, h_yield_EffCorr );
}

void DiffXsecTools::SaveCanvas_beforeFSRCorr_vs_AfterFSRCorr( TH1D* h_FSRCorrected, TH1D* h_yield_Unfolded_AccEff )
{
	MyCanvas *myc = new MyCanvas("c_beforeFSRCorr_vs_AfterFSRCorr", "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_FSRCorrected->Clone(), (TH1D*)h_yield_Unfolded_AccEff->Clone(),
											"Yield(Data,BeforeFSRCorr)", "Yield(Data,AfterFSRCorr)", "After/Before",
											kBlack, kRed,
											kFALSE, kFALSE,
											"HIST", "HISTSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_beforeFSRCorr_vs_genMC_preFSR( TH1D* h_yield_Unfolded_AccEff, TH1D* h_Truth_preFSR )
{
	MyCanvas *myc = new MyCanvas("c_beforeFSRCorr_vs_genMC_preFSR", "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);

	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_yield_Unfolded_AccEff->Clone(), (TH1D*)h_Truth_preFSR->Clone(),
											"Yield(Data,BeforeFSRCorr)", "MC(gen-level, preFSR)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );

	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_afterFSRCorr_vs_genMC_preFSR( TH1D* h_FSRCorrected, TH1D* h_Truth_preFSR )
{
	MyCanvas *myc = new MyCanvas("c_afterFSRCorr_vs_genMC_preFSR", "Dimuon Mass [GeV]", "Number of events");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->SetYRange(2e-2, 5e6);
	
	myc->CanvasWithHistogramsRatioPlot((TH1D*)h_FSRCorrected->Clone(), (TH1D*)h_Truth_preFSR->Clone(),
											"Yield(Data, AfterFSRCorr)", "MC(gen-level, preFSR)", "Data/MC",
											kBlack, kOrange,
											kFALSE, kTRUE,
											"EP", "HISTSAME" );

	myc->PrintCanvas();
}

void DiffXsecTools::CalcXsec()
{
	////////////////////////////////////
	// -- Make x-section histogram -- //
	////////////////////////////////////
	h_xSec_Raw = (TH1D*)h_yield_Raw->Clone();
	h_xSec_Raw->Sumw2();
	h_xSec_Raw->Scale( 1 / Lumi );

	h_xSec_Unfolded = (TH1D*)h_yield_Unfolded->Clone();
	h_xSec_Unfolded->Sumw2();
	h_xSec_Unfolded->Scale( 1 / Lumi );

	h_xSec_Unfolded_AccEff = (TH1D*)h_yield_Unfolded_AccEff->Clone();
	h_xSec_Unfolded_AccEff->Sumw2();
	h_xSec_Unfolded_AccEff->Scale( 1 / Lumi );

	h_xSec_FSRCorr = (TH1D*)h_yield_FSRCorr->Clone();
	h_xSec_FSRCorr->Sumw2();
	h_xSec_FSRCorr->Scale( 1 / Lumi );

 	h_xSec_aMCNLO = (TH1D*)h_Truth_preFSR->Clone();
	h_xSec_aMCNLO->Sumw2();
	h_xSec_aMCNLO->Scale( 1 / Lumi );

	////////////////////////////////
	// -- X-sec/dM Calculation -- //
	////////////////////////////////
	h_DiffXsec_Raw = (TH1D*)h_xSec_Raw->Clone();
	h_DiffXsec_Raw->Sumw2();
	Obtain_dSigma_dM(h_DiffXsec_Raw);

	h_DiffXsec_Unfolded = (TH1D*)h_xSec_Unfolded->Clone();
	h_DiffXsec_Unfolded->Sumw2();
	Obtain_dSigma_dM(h_DiffXsec_Unfolded);

	h_DiffXsec_Unfolded_AccEff = (TH1D*)h_xSec_Unfolded_AccEff->Clone();
	h_DiffXsec_Unfolded_AccEff->Sumw2();
	Obtain_dSigma_dM(h_DiffXsec_Unfolded_AccEff);

	h_DiffXsec_FSRCorr = (TH1D*)h_xSec_FSRCorr->Clone();
	h_DiffXsec_FSRCorr->Sumw2();
	Obtain_dSigma_dM(h_DiffXsec_FSRCorr);
	SetStatUnc_DiffXsec( h_DiffXsec_FSRCorr );

	h_DiffXsec_aMCNLO = (TH1D*)h_xSec_aMCNLO->Clone();
	h_DiffXsec_aMCNLO->Sumw2();
	Obtain_dSigma_dM(h_DiffXsec_aMCNLO);

	// f_theory = new TFile(FileLocation + "/ROOTFile_xSec_Theory.root"); f_theory->cd();
	f_theory = TFile::Open(FileLocation + "/ROOTFile_xSec_Theory.root"); f_theory->cd();
	h_DiffXsec_FEWZ_NLO = (TH1D*)f_theory->Get("h_DiffXsec_FEWZ_NNPDF_NLO")->Clone();
	h_DiffXsec_FEWZ_NNLO = (TH1D*)f_theory->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone();
	delete f_theory;
}

void DiffXsecTools::Obtain_dSigma_dM(TH1D *h)
{
	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Int_t i_bin = i+1;
		Double_t BinWidth = h->GetBinWidth(i_bin);

		Double_t low = h->GetBinLowEdge(i_bin);
		Double_t high = h->GetBinLowEdge(i_bin + 1);

		Double_t xSec = h->GetBinContent(i_bin);
		Double_t xSec_dM = xSec / BinWidth;

		Double_t error_before = h->GetBinError(i_bin);
		Double_t error_after = error_before / BinWidth;

		h->SetBinContent(i_bin, xSec_dM);
		h->SetBinError(i_bin, error_after);

		// printf("%2dth bin [%5.lf, %5.lf] (xSec, BinWidth, dSigma/dM) = (%15.9lf, %6.1lf, %15.9lf), (error_before, error_after) = (%8.5lf, %8.5lf)\n", 
			// i_bin, low, high, xSec, BinWidth, xSec_dM, error_before, error_after );
	}
}

void DiffXsecTools::SaveCanvas_CalcXsec()
{
	this->SaveCanvas_xSec_Data_vs_aMCNLO();
	this->SaveCanvas_DiffXsec_Data_AllCorrStep();
	this->SaveCanvas_DiffXsec_Data_vs_FEWZ("NLO", h_DiffXsec_FEWZ_NLO);
	this->SaveCanvas_DiffXsec_Data_vs_FEWZ("NNLO", h_DiffXsec_FEWZ_NNLO);
	this->SaveCanvas_DiffXsec_Data_vs_aMCNLO();

}

void DiffXsecTools::SaveCanvas_xSec_Data_vs_aMCNLO()
{
	MyCanvas *myc = new MyCanvas("c_xSec_Data_vs_aMCNLO", "Dimuon Mass [GeV]", "Cross Section [pb]");
	myc->SetLogx(1);
	myc->SetLogy(0);

	// -- X-section comparison between data and MC -- //
	myc->CanvasWithHistogramsRatioPlot( (TH1D*)h_xSec_FSRCorr->Clone(), (TH1D*)h_xSec_aMCNLO->Clone(),
											"Data (Unfold+FSRCorr)", "aMC@NLO (pre-FSR)", "Data/aMC@NLO",
											kBlack, kRed,
											kFALSE, kFALSE,
											"EP", "EPSAME" );

	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_DiffXsec_Data_AllCorrStep()
{
	TCanvas *c = new TCanvas("c_DiffXsec_Data_AllCorrStep");
	c->cd();
	// -- Top Pad -- //
	TPad *TopPad = new TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99);
	TopPad->Draw();
	TopPad->cd();

	gPad->SetLogx();
	gPad->SetLogy();

	// -- ensure additional space at the bottom side for ratio plot -- //
	TopPad->SetBottomMargin(0.32);
	TopPad->SetRightMargin(0.05);

	h_DiffXsec_FSRCorr->Draw("EP");
	h_DiffXsec_Raw->Draw("EPSAME");
	h_DiffXsec_Unfolded->Draw("EPSAME");
	h_DiffXsec_Unfolded_AccEff->Draw("EPSAME");
	h_DiffXsec_FSRCorr->Draw("AXISSAME");
	h_DiffXsec_FSRCorr->Draw("EPSAME");

	// -- General Setting for both plots -- //
	h_DiffXsec_FSRCorr->SetXTitle( "Dimuon Mass [GeV]" );
	h_DiffXsec_FSRCorr->SetYTitle( "d#sigma/dM [pb/GeV]" );

	h_DiffXsec_FSRCorr->SetStats(kFALSE);
	h_DiffXsec_Unfolded->SetStats(kFALSE);
	h_DiffXsec_Unfolded_AccEff->SetStats(kFALSE);
	h_DiffXsec_Raw->SetStats(kFALSE);

	h_DiffXsec_FSRCorr->SetMarkerSize(1);
	h_DiffXsec_FSRCorr->SetMarkerStyle(20);
	h_DiffXsec_FSRCorr->SetLineColor(kBlack);
	h_DiffXsec_FSRCorr->SetMarkerColor(kBlack);
	h_DiffXsec_FSRCorr->SetFillColorAlpha(kWhite, 0);

	h_DiffXsec_Unfolded_AccEff->SetMarkerSize(1);
	h_DiffXsec_Unfolded_AccEff->SetMarkerStyle(20);
	h_DiffXsec_Unfolded_AccEff->SetLineColor(kBlue);
	h_DiffXsec_Unfolded_AccEff->SetMarkerColor(kBlue);
	h_DiffXsec_Unfolded_AccEff->SetFillColorAlpha(kWhite, 0);

	h_DiffXsec_Unfolded->SetMarkerSize(1);
	h_DiffXsec_Unfolded->SetMarkerStyle(20);
	h_DiffXsec_Unfolded->SetLineColor(kGreen+1);
	h_DiffXsec_Unfolded->SetMarkerColor(kGreen+1);
	h_DiffXsec_Unfolded->SetFillColorAlpha(kWhite, 0);

	h_DiffXsec_Raw->SetMarkerSize(1);
	h_DiffXsec_Raw->SetMarkerStyle(20);
	h_DiffXsec_Raw->SetLineColor(kRed);
	h_DiffXsec_Raw->SetMarkerColor(kRed);
	h_DiffXsec_Raw->SetFillColorAlpha(kWhite, 0);

	// -- X-axis Setting -- //
	h_DiffXsec_FSRCorr->GetXaxis()->SetLabelSize(0);
	h_DiffXsec_FSRCorr->GetXaxis()->SetTitleSize(0);
	h_DiffXsec_FSRCorr->GetXaxis()->SetNoExponent();
	h_DiffXsec_FSRCorr->GetXaxis()->SetMoreLogLabels();

	// -- Y-axis Setting -- //
	h_DiffXsec_FSRCorr->GetYaxis()->SetTitleSize(0.06);
	h_DiffXsec_FSRCorr->GetYaxis()->SetTitleOffset(1.25);
	// h_DiffXsec_FSRCorr->GetYaxis()->SetNoExponent();
	// h_DiffXsec_FSRCorr->GetYaxis()->SetMoreLogLabels();

	// -- Add Legend -- //
	TLegend *legend2 = new TLegend(0.55, 0.75, 0.95, 0.95);
	legend2->SetBorderSize(0);
	legend2->SetFillStyle(0);
	legend2->AddEntry(h_DiffXsec_FSRCorr, "Data (FSR)");
	legend2->AddEntry(h_DiffXsec_Unfolded_AccEff, "Data (Acc*Eff)");
	legend2->AddEntry(h_DiffXsec_Unfolded, "Data (Unfolding)");
	legend2->AddEntry(h_DiffXsec_Raw, "Data (Raw)");
	legend2->Draw();


	// -- Bottom Pad -- //
	TPad *BottomPad = new TPad("BottomPad","BottomPad",0.01,0.01,0.99,0.3);
	BottomPad->Draw();
	BottomPad->cd();

	BottomPad->SetBottomMargin(0.4);
	BottomPad->SetRightMargin(0.04);
	BottomPad->SetLeftMargin(0.15);	

	gPad->SetLogx();
	gPad->SetLogy();
	
	// -- Make Ratio plot & Draw it -- //
	TH1D* h_ratio = (TH1D*)h_DiffXsec_FSRCorr->Clone();
	h_ratio->SetName("h_ratio");
	h_DiffXsec_FSRCorr->Sumw2(); h_DiffXsec_Raw->Sumw2();
	h_ratio->Divide(h_DiffXsec_FSRCorr, h_DiffXsec_Raw);
	h_ratio->Draw("EP");

	// -- Make Ratio plot & Draw it -- //
	TH1D* h_ratio_Unfold = (TH1D*)h_DiffXsec_FSRCorr->Clone();
	h_ratio_Unfold->SetName("h_ratio_Unfold");
	h_DiffXsec_Unfolded->Sumw2();
	h_ratio_Unfold->Divide(h_DiffXsec_Unfolded, h_DiffXsec_Raw);
	h_ratio_Unfold->Draw("EPSAME");

	TH1D* h_ratio_AccEff = (TH1D*)h_DiffXsec_FSRCorr->Clone();
	h_ratio_AccEff->SetName("h_ratio_AccEff");
	h_DiffXsec_Unfolded_AccEff->Sumw2();
	h_ratio_AccEff->Divide(h_DiffXsec_Unfolded_AccEff, h_DiffXsec_Raw);
	h_ratio_AccEff->Draw("EPSAME");

	// -- General Setting -- //
	h_ratio->SetLineColor(kBlack);
	h_ratio->SetMarkerStyle(20);
	h_ratio->SetMarkerSize(1);
	h_ratio->SetMarkerColor(kBlack);
	h_ratio->SetStats(kFALSE);

	h_ratio_Unfold->SetLineColor(kGreen+1);
	h_ratio_Unfold->SetMarkerStyle(20);
	h_ratio_Unfold->SetMarkerSize(1);
	h_ratio_Unfold->SetMarkerColor(kGreen+1);
	h_ratio_Unfold->SetStats(kFALSE);

	h_ratio_AccEff->SetLineColor(kBlue);
	h_ratio_AccEff->SetMarkerStyle(20);
	h_ratio_AccEff->SetMarkerSize(1);
	h_ratio_AccEff->SetMarkerColor(kBlue);
	h_ratio_AccEff->SetStats(kFALSE);

	// -- X-axis Setting -- //		
	h_ratio->GetXaxis()->SetTitle( "Dimuon Mass [GeV]" );
	h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
	h_ratio->GetXaxis()->SetTitleSize( 0.2 );
	h_ratio->GetXaxis()->SetLabelColor(1);
	h_ratio->GetXaxis()->SetLabelFont(42);
	h_ratio->GetXaxis()->SetLabelOffset(0.007);
	h_ratio->GetXaxis()->SetLabelSize(0.15);
	h_ratio->GetXaxis()->SetMoreLogLabels(); 
	h_ratio->GetXaxis()->SetNoExponent();

	// -- Y-axis Setting -- //
	h_ratio->GetYaxis()->SetTitle( "Ratio to Raw" );
	h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
	h_ratio->GetYaxis()->SetTitleSize( 0.1);
	h_ratio->GetYaxis()->SetLabelSize( 0.07 );
	// h_ratio->GetYaxis()->SetRangeUser( 0, 3 );

	// -- flat line = 1.00 -- //
	TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	f_line->SetLineColor(kRed);
	f_line->SetLineWidth(1);
	f_line->Draw("SAME");

	h_ratio->Draw("EPSAME");

	c->SaveAs("c_DiffXsec_Data_AllCorrStep.pdf");
}

void DiffXsecTools::CalcHistogram_RelStatUnc()
{
	f_data = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root"); f_data->cd();
	TH1D *h_data = (TH1D*)f_data->Get("h_mass_OS_Data")->Clone();
	h_data = (TH1D*)h_data->Rebin(nMassBin, h_data->GetName(), MassBinEdges);

	h_RelUnc_Stat = new TH1D("h_RelUnc_Stat", "", nMassBin, MassBinEdges);

	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t nEvent_Unfold = h_yield_Unfolded->GetBinContent(i_bin);
		Int_t nEvent_Data = h_data->GetBinContent(i_bin);

		Double_t RelUnc = sqrt(nEvent_Data) / nEvent_Unfold;

		h_RelUnc_Stat->SetBinContent(i_bin, RelUnc);
		h_RelUnc_Stat->SetBinError(i_bin, 0);

		printf( "[%.2d bin: (%.4lf, %.4lf)] ( nEvent_Unfolded, nEvent_Data, sqrt(nEvent_Data), StatUnc(%%) ) = (%12.1lf, %12.d, %12.1lf, %12.3lf)\n", 
			i_bin, MassBinEdges[i], MassBinEdges[i+1], nEvent_Unfold, nEvent_Data, sqrt(nEvent_Data), RelUnc*100 );
	}

	delete f_data;
}

void DiffXsecTools::SetStatUnc_DiffXsec(TH1D* h_DiffXsec)
{
	if( h_RelUnc_Stat == NULL )
		this->CalcHistogram_RelStatUnc();

	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t DiffXsec = h_DiffXsec->GetBinContent(i_bin);
		Double_t RelUnc = h_RelUnc_Stat->GetBinContent(i_bin);

		h_DiffXsec->SetBinError(i_bin, RelUnc*DiffXsec);
	}
}

void DiffXsecTools::SaveCanvas_DiffXsec_Data_vs_FEWZ(TString Type, TH1D *h_DiffXsec_FEWZ)
{
	MyCanvas *myc = new MyCanvas("c_DiffXsec_Data_vs_FEWZ_"+Type, "Dimuon Mass [GeV]", "d#sigma/dM [pb/GeV]");
	myc->SetLogx();
	myc->SetLogy(0);
	myc->CanvasWithHistogramsRatioPlot( (TH1D*)h_DiffXsec_FSRCorr->Clone(), (TH1D*)h_DiffXsec_FEWZ->Clone(),
											"Data", "FEWZ ("+Type+")", "Data/FEWZ",
											kBlack, kRed,
											kFALSE, kFALSE,
											"EP", "EPSAME" );

	myc->PrintCanvas();
}

void DiffXsecTools::SaveCanvas_DiffXsec_Data_vs_aMCNLO()
{
	MyCanvas *myc = new MyCanvas("c_DiffXsec_Data_vs_aMCNLO", "Dimuon Mass [GeV]", "d#sigma/dM [pb/GeV]");
	myc->SetLogx();
	myc->SetLogy(0);

	myc->CanvasWithHistogramsRatioPlot( (TH1D*)h_DiffXsec_FSRCorr->Clone(), (TH1D*)h_DiffXsec_aMCNLO->Clone(),
											"Data", "aMC@NLO", "Data/aMC@NLO",
											kBlack, kRed,
											kFALSE, kFALSE,
											"EP", "EPSAME" );
	myc->PrintCanvas();
}


void DiffXsecTools::SaveHistograms()
{
	// -- output file -- //	
	f_output = new TFile("ROOTFile_Results_DYAnalysis_76X.root", "RECREATE");

	//////////////////////////
	// -- Save histogram -- //
	//////////////////////////
	f_output->cd();

	h_totSignalMC->SetName("h_DYMC_Reco");
	h_totSignalMC->Write();

	h_totSignalMC_HLTv4p2->SetName("h_DYMC_Reco_HLTv4p2");
	h_totSignalMC_HLTv4p2->Write();

	h_totSignalMC_HLTv4p3->SetName("h_DYMC_Reco_HLTv4p3");
	h_totSignalMC_HLTv4p3->Write();

	h_totSignalMC_GenLevel->SetName("h_DYMC_Gen_postFSR");
	h_totSignalMC_GenLevel->Write();

	h_totSignalMC_GenLevel_HLTv4p2->SetName("h_DYMC_Gen_postFSR_HLTv4p2");
	h_totSignalMC_GenLevel_HLTv4p2->Write();

	h_totSignalMC_GenLevel_HLTv4p3->SetName("h_DYMC_Gen_postFSR_HLTv4p3");
	h_totSignalMC_GenLevel_HLTv4p3->Write();

	h_totSignalMC_GenLevel_WithinAcc->SetName("h_DYMC_Gen_postFSR_WithinAcc");
	h_totSignalMC_GenLevel_WithinAcc->Write();

	h_totSignalMC_GenLevel_WithinAcc_HLTv4p2->SetName("h_DYMC_Gen_postFSR_WithinAcc_HLTv4p2");
	h_totSignalMC_GenLevel_WithinAcc_HLTv4p2->Write();

	h_totSignalMC_GenLevel_WithinAcc_HLTv4p3->SetName("h_DYMC_Gen_postFSR_WithinAcc_HLTv4p3");
	h_totSignalMC_GenLevel_WithinAcc_HLTv4p3->Write();

	h_yield_Raw->SetName("h_yield_Raw");
	h_yield_Raw->Write();

	h_yield_HLTv4p2->SetName("h_yield_Raw_HLTv4p2");
	h_yield_HLTv4p2->Write();

	h_yield_HLTv4p3->SetName("h_yield_Raw_HLTv4p3");
	h_yield_HLTv4p3->Write();

	// h_NoGen_Reco->SetName("h_NoGen_Reco");
	// h_NoGen_Reco->Write();

	h_yield_Unfolded->SetName("h_yield_Unfolded");
	h_yield_Unfolded->Write();

	h_yield_HLTv4p2_Unfolded->SetName("h_yield_HLTv4p2_Unfolded");
	h_yield_HLTv4p2_Unfolded->Write();

	h_yield_HLTv4p3_Unfolded->SetName("h_yield_HLTv4p3_Unfolded");
	h_yield_HLTv4p3_Unfolded->Write();

	h_yield_Unfolded_AccEff->SetName("h_yield_Unfolded_AccEff");
	h_yield_Unfolded_AccEff->Write();

	h_yield_HLTv4p2_Unfolded_AccEff->SetName("h_yield_HLTv4p2_Unfolded_AccEff");
	h_yield_HLTv4p2_Unfolded_AccEff->Write();

	h_yield_HLTv4p3_Unfolded_AccEff->SetName("h_yield_HLTv4p3_Unfolded_AccEff");
	h_yield_HLTv4p3_Unfolded_AccEff->Write();

	h_yield_EffCorr->SetName("h_yield_EffCorr");
	h_yield_EffCorr->Write();

	h_yield_HLTv4p2_EffCorr->SetName("h_yield_HLTv4p2_EffCorr");
	h_yield_HLTv4p2_EffCorr->Write();

	h_yield_HLTv4p3_EffCorr->SetName("h_yield_HLTv4p3_EffCorr");
	h_yield_HLTv4p3_EffCorr->Write();

	h_Truth_preFSR->SetName("h_DYMC_Gen_preFSR");
	h_Truth_preFSR->Write();

	h_yield_FSRCorr->SetName("h_yield_FSRCorr");	
	h_yield_FSRCorr->Write();

	h_xSec_Raw->SetName("h_xSec_Raw");
	h_xSec_Raw->Write();

	h_xSec_Unfolded->SetName("h_xSec_Unfolded");
	h_xSec_Unfolded->Write();

	h_xSec_Unfolded_AccEff->SetName("h_xSec_Unfolded_AccEff");
	h_xSec_Unfolded_AccEff->Write();

	h_xSec_FSRCorr->SetName("h_xSec_FSRCorr");
	h_xSec_FSRCorr->Write();

	h_xSec_aMCNLO->SetName("h_xSec_aMCNLO");
	h_xSec_aMCNLO->Write();

	h_DiffXsec_Raw->SetName("h_DiffXsec_Raw");
	h_DiffXsec_Raw->Write();

	h_DiffXsec_Unfolded->SetName("h_DiffXsec_Unfolded");
	h_DiffXsec_Unfolded->Write();

	h_DiffXsec_Unfolded_AccEff->SetName("h_DiffXsec_Unfolded_AccEff");
	h_DiffXsec_Unfolded_AccEff->Write();

	h_DiffXsec_FSRCorr->SetName("h_DiffXsec_FSRCorr");
	h_DiffXsec_FSRCorr->Write();

	h_DiffXsec_aMCNLO->SetName("h_DiffXsec_aMCNLO");
	h_DiffXsec_aMCNLO->Write();

	h_DiffXsec_FEWZ_NLO->SetName("h_DiffXsec_FEWZ_NLO");
	h_DiffXsec_FEWZ_NLO->Write();

	h_DiffXsec_FEWZ_NNLO->SetName("h_DiffXsec_FEWZ_NNLO");
	h_DiffXsec_FEWZ_NNLO->Write();

	h_RelUnc_Stat->Write();
}

// void DiffXsecTools::EstimationUncertainty()
// {
// 	//////////////////////////////////
// 	// -- Uncertainty Estimation -- //
// 	//////////////////////////////////
// 	UncertantyCalcTool *UncTool = new UncertantyCalcTool();

// 	///////////////////////////////////
// 	// -- Statistical Uncertainty -- //
// 	///////////////////////////////////
// 	f_data = new TFile(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root"); f_data->cd();
// 	TH1D *h_data = (TH1D*)f_data->Get("h_mass_OS_Data")->Clone();
// 	h_data = (TH1D*)h_data->Rebin(nMassBin, h_data->GetName(), MassBinEdges);
// 	UncTool->StatUnc( h_yield_Unfolded, h_data );

// 	/////////////////////////////////////////////////
// 	// -- Uncertainty from background estimation-- //
// 	/////////////////////////////////////////////////
// 	f_bkg_dataDriven = new TFile(FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root"); f_bkg_dataDriven->cd();
// 	TH1D* h_ttbar = (TH1D*)f_bkg_dataDriven->Get("ttbar")->Clone();
// 	TH1D* h_DYTauTau = (TH1D*)f_bkg_dataDriven->Get("DYtautau")->Clone();
// 	TH1D* h_tW = (TH1D*)f_bkg_dataDriven->Get("tW")->Clone();
// 	TH1D* h_WJets = (TH1D*)f_bkg_dataDriven->Get("wjets")->Clone();
// 	TH1D* h_QCD = (TH1D*)f_bkg_dataDriven->Get("dijet")->Clone();

// 	f_MC->cd();
// 	TH1D *h_ZZ = (TH1D*)f_MC->Get("h_mass_OS_ZZ")->Clone();
// 	h_ZZ = (TH1D*)h_ZZ->Rebin(nMassBin, h_ZZ->GetName(), MassBinEdges);
// 	TH1D *h_WZ = (TH1D*)f_MC->Get("h_mass_OS_WZ")->Clone();
// 	h_WZ = (TH1D*)h_WZ->Rebin(nMassBin, h_WZ->GetName(), MassBinEdges);
// 	TH1D *h_WW = (TH1D*)f_MC->Get("h_mass_OS_WW")->Clone();
// 	h_WW = (TH1D*)h_WW->Rebin(nMassBin, h_WW->GetName(), MassBinEdges);

// 	Int_t nTag = (Int_t)Tag.size();
// 	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
// 	{
// 		Double_t norm = ( Lumi * Xsec[i_tag] ) / nEvents[i_tag];
// 		if( Tag[i_tag] == "ZZ" )
// 			h_ZZ->Scale( norm );
// 		else if( Tag[i_tag] == "WZ" )
// 			h_WZ->Scale( norm );
// 		else if( Tag[i_tag] == "WW" )
// 			h_WW->Scale( norm );
// 	}

// 	UncTool->BkgUnc( h_yield_Unfolded, h_ttbar, h_DYTauTau, h_tW, h_WJets, h_QCD, h_ZZ, h_WZ, h_WW);

// 	vector< TH1D* > Histos_Unc; vector< TString > Names_Unc;
// 	TH1D *h_Unc_Stat = (TH1D*)UncTool->h_RelUnc_Stat->Clone(); Histos_Unc.push_back( h_Unc_Stat ); Names_Unc.push_back( "Stat." );
// 	TH1D *h_Unc_Bkg = (TH1D*)UncTool->h_RelUnc_Bkg->Clone(); Histos_Unc.push_back( h_Unc_Bkg ); Names_Unc.push_back( "Bkg.Syst." );

// 	h_Unc_Stat->Scale( 100 );
// 	h_Unc_Bkg->Scale( 100 );

// 	MyCanvas *myc = new MyCanvas("c_Unc", "Dimuon Mass [GeV]", "Uncertainty (%)");
// 	myc->SetLogx();
// 	myc->Legend_x1 = 0.20; myc->Legend_x2 = 0.50;

// 	myc->CanvasWithMultipleHistograms( Histos_Unc, Names_Unc, "LP" );
// 	myc->PrintCanvas();

// 	//////////////////////////////////////////
// 	// -- Store Uncertainty distribution -- //
// 	//////////////////////////////////////////
// 	f_output->cd();
// 	h_Unc_Stat->Write();
// 	h_Unc_Bkg->Write();
// }

Double_t DiffXsecTools::ReturnLargerValue(Double_t a, Double_t b)
{
	if( a > b )
		return a;
	else
		return b;
}

Double_t DiffXsecTools::Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
{
	Double_t ratio_A = (sigma_A) / A;
	Double_t ratio_B = (sigma_B) / B;

	Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

	return (A/B) * sqrt(errorSquare);
}

void DiffXsecTools::FpoF_GetTheoryHist()
{
	TFile *f_FpoF_aMCNLO = TFile::Open(FileLocation + "/ROOTFile_Histogram_Acc_Eff_aMCNLO_IsoMu20_OR_IsoTkMu20.root");
	f_FpoF_aMCNLO->cd();

	this->h_FpoF_yield_aMCNLO = (TH1D*)f_FpoF_aMCNLO->Get("h_mass_AccPass")->Clone("h_FpoF_yield_aMCNLO");

	delete f_FpoF_aMCNLO;
}

void DiffXsecTools::FpoF_EffCorrection()
{
	//////////////////////////////
	// -- Acc*Eff Correction -- //
	//////////////////////////////
	// f_AccEff = new TFile(FileLocation + "/ROOTFile_AccEff.root"); f_AccEff->cd();

	TFile *f_Eff = TFile::Open(FileLocation + "/ROOTFile_AccEff.root");

	f_Eff->cd();
	TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)f_Eff->Get("g_Eff")->Clone();

	// -- HLTv4p2 -- //
	this->h_FpoF_yield_HLTv4p2_Unfolded_Eff = (TH1D*)h_yield_HLTv4p2_Unfolded->Clone("h_FpoF_yield_HLTv4p2_Unfolded_Eff");
	this->Correction_AccEff(h_FpoF_yield_HLTv4p2_Unfolded_Eff, h_yield_HLTv4p2_Unfolded, g_Eff);

	// -- HLTv4p3 -- //
	this->h_FpoF_yield_HLTv4p3_Unfolded_Eff = (TH1D*)h_yield_HLTv4p3_Unfolded->Clone("h_FpoF_yield_HLTv4p3_Unfolded_Eff");
	this->Correction_AccEff(h_FpoF_yield_HLTv4p3_Unfolded_Eff, h_yield_HLTv4p3_Unfolded, g_Eff);

	// -- Combine -- //
	h_FpoF_yield_HLTv4p2_Unfolded_Eff->Sumw2(); h_FpoF_yield_HLTv4p3_Unfolded_Eff->Sumw2();
	this->h_FpoF_yield_Unfolded_Eff = (TH1D*)h_FpoF_yield_HLTv4p2_Unfolded_Eff->Clone( "h_FpoF_yield_Unfolded_Eff" );
	this->h_FpoF_yield_Unfolded_Eff->Add( h_FpoF_yield_HLTv4p3_Unfolded_Eff );

	delete f_Eff;
}

void DiffXsecTools::FpoF_EfficiencyScaleFactor()
{
	if( g_EffCorr_HLTv4p2 == NULL || g_EffCorr_HLTv4p3 == NULL )
		this->Calc_EffSF();

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

void DiffXsecTools::FpoF_CalcXsec()
{
	////////////////////////////////////
	// -- Make x-section histogram -- //
	////////////////////////////////////
	this->h_FpoF_DiffXsec_Data = (TH1D*)this->h_FpoF_yield_EffCorr->Clone("h_FpoF_DiffXsec_Data");
	this->h_FpoF_DiffXsec_Data->Sumw2();
	this->h_FpoF_DiffXsec_Data->Scale( 1 / Lumi );
	this->Obtain_dSigma_dM(this->h_FpoF_DiffXsec_Data);
	this->SetStatUnc_DiffXsec( this->h_FpoF_DiffXsec_Data );

	this->h_FpoF_DiffXsec_aMCNLO = (TH1D*)this->h_FpoF_yield_aMCNLO->Clone("h_FpoF_DiffXsec_aMCNLO");
	this->h_FpoF_DiffXsec_aMCNLO->Sumw2();
	this->h_FpoF_DiffXsec_aMCNLO->Scale( 1 / Lumi );
	this->Obtain_dSigma_dM(this->h_FpoF_DiffXsec_aMCNLO);
}

void DiffXsecTools::FpoF_SaveCanvas_DiffXsec_Data_vs_aMCNLO()
{
	MyCanvas *myc = new MyCanvas("c_FpoF_DiffXsec_Data_vs_aMCNLO", "Dimuon Mass [GeV]", "d#sigma/dM [pb/GeV]");
	myc->SetLogx();
	myc->SetLogy(0);

	myc->CanvasWithHistogramsRatioPlot( (TH1D*)this->h_FpoF_DiffXsec_Data->Clone(), (TH1D*)this->h_FpoF_DiffXsec_aMCNLO->Clone(),
											"Data", "aMC@NLO", "Data/aMC@NLO",
											kBlack, kRed,
											kFALSE, kFALSE,
											"EP", "EPSAME" );
	myc->PrintCanvas();
}

void DiffXsecTools::FpoF_SaveResults()
{
	f_output->cd();

	h_FpoF_yield_aMCNLO->Write();

	h_FpoF_yield_HLTv4p2_Unfolded_Eff->Write();
	h_FpoF_yield_HLTv4p3_Unfolded_Eff->Write();
	h_FpoF_yield_Unfolded_Eff->Write();

	h_FpoF_yield_HLTv4p2_EffCorr->Write();
	h_FpoF_yield_HLTv4p3_EffCorr->Write();
	h_FpoF_yield_EffCorr->Write();

	h_FpoF_DiffXsec_Data->Write();
	h_FpoF_DiffXsec_aMCNLO->Write();
}

void DiffXsecTools::FpoF_AllAnalysisProcedure()
{
	this->FpoF_GetTheoryHist();
	this->FpoF_EffCorrection();
	this->FpoF_EfficiencyScaleFactor();
	this->FpoF_CalcXsec();
	this->FpoF_SaveCanvas_DiffXsec_Data_vs_aMCNLO();
	this->FpoF_SaveResults();
}