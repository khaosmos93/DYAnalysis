/////////////////////////////////////////////////
// -- 2016.10.24 (v01): Include DYAnalyzer_v01.h
// -- 2016.11.04 (v02): include v02 headers
/////////////////////////////////////////////////

#pragma once

#include <TH1D.h>
#include <TLorentzVector.h>
#include <TFile.h>

#include <vector>

#include <DYAnalysis_76X/CommonCodes/DYAnalyzer.h>

class ControlPlots
{
public:
	DYAnalyzer *analyzer;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	TH1D *h_mass;
	TH1D *h_diPt;
	TH1D *h_diRap;

	TH1D *h_absdiRap_M20to30;
	TH1D *h_absdiRap_M30to45;
	TH1D *h_absdiRap_M45to60;
	TH1D *h_absdiRap_M60to120;
	TH1D *h_absdiRap_M120to200;
	TH1D *h_absdiRap_M200to1500;

	TH1D *h_lead_Pt;
	TH1D *h_sub_Pt;
	
	TH1D *h_lead_eta;
	TH1D *h_sub_eta;

	TH1D *h_lead_phi;
	TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;

	TH1D *h_endcap_Pt;
	TH1D *h_endcap_eta;
	TH1D *h_endcap_phi;

	TH1D *h_mass_OS;
	TH1D *h_mass_SS;

	TH1D *h_Pt_minusCharge;
	TH1D *h_eta_minusCharge;
	TH1D *h_phi_minusCharge;
	TH1D *h_Pt_plusCharge;
	TH1D *h_eta_plusCharge;
	TH1D *h_phi_plusCharge;

	TH1D *h_Pt_M70;
	TH1D *h_Pt_M70to90;
	TH1D *h_Pt_M90to110;
	TH1D *h_Pt_M110toInf;
	TH1D *h_Pt_M70to111;

	// -- Coarse binning -- // 
	TH1D *h_Pt_M15to60;
	TH1D *h_Pt_M60to120;
	TH1D *h_Pt_M120to3000;

	// -- Fine binning -- //
	TH1D *h_Pt_M15to30;
	TH1D *h_Pt_M30to45;
	TH1D *h_Pt_M45to60;
	TH1D *h_Pt_M120to200;
	TH1D *h_Pt_M200to3000;

	TH1D *h_Angle;

	TH1D *h_Pt_TrigLeg;
	TH1D *h_eta_TrigLeg;
	TH1D *h_phi_TrigLeg;

	TH1D *h_Pt_OtherLeg;
	TH1D *h_eta_OtherLeg;
	TH1D *h_phi_OtherLeg;

	TH1D *h_GenMass;
	TH1D *h_GenPt;
	TH1D *h_GenEta;
	TH1D *h_GenPhi;

	TH1D *h_GenPt_Lead;
	TH1D *h_GenEta_Lead;
	TH1D *h_GenPhi_Lead;

	TH1D *h_GenPt_Sub;
	TH1D *h_GenEta_Sub;
	TH1D *h_GenPhi_Sub;

	TH1D *h_GenDiPt;
	TH1D *h_GenDiRap;

	TH1D *h_VtxProb;
	TH1D *h_VtxNormChi2;
	TH1D *h_VtxProb_belowM600;
	TH1D *h_VtxNormChi2_belowM600;
	TH1D *h_VtxProb_overM600;
	TH1D *h_VtxNormChi2_overM600;

	TH1D* h_mass_OS_BB;
	TH1D* h_mass_OS_BE;
	TH1D* h_mass_OS_EE;

	TH1D *h_mass_OS_HLTv4p2;
	TH1D *h_mass_OS_HLTv4p2_BB;
	TH1D *h_mass_OS_HLTv4p2_BE;
	TH1D *h_mass_OS_HLTv4p2_EE;

	TH1D *h_mass_OS_HLTv4p3;
	TH1D *h_mass_OS_HLTv4p3_BB;
	TH1D *h_mass_OS_HLTv4p3_BE;
	TH1D *h_mass_OS_HLTv4p3_EE;
	TH1D *h_mass_OS_HLTv4p3_Split1;
	TH1D *h_mass_OS_HLTv4p3_Split2;
	TH1D *h_mass_OS_HLTv4p3_Split3;

	TH1D *h_muonHits;
	TH1D *h_nMatches;
	TH1D *h_RelPtError;
	TH1D *h_dxyVTX;
	TH1D *h_dzVTX;
	TH1D *h_pixelHits;
	TH1D *h_trackerLayers;
	TH1D *h_RelTrkIso;


	ControlPlots(TString Type, DYAnalyzer *dyanalyzer)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;

		h_Pt = new TH1D("h_Pt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		h_mass = new TH1D("h_mass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass );
		h_diPt = new TH1D("h_diPt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_diPt );
		h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );

		h_absdiRap_M20to30 = new TH1D("h_absdiRap_M20to30_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M20to30 );
		h_absdiRap_M30to45 = new TH1D("h_absdiRap_M30to45_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M30to45 );
		h_absdiRap_M45to60 = new TH1D("h_absdiRap_M45to60_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M45to60 );
		h_absdiRap_M60to120 = new TH1D("h_absdiRap_M60to120_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M60to120 );
		h_absdiRap_M120to200 = new TH1D("h_absdiRap_M120to200_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M120to200 );
		h_absdiRap_M200to1500 = new TH1D("h_absdiRap_M200to1500_"+Type, "", 12, 0, 2.4); Histo.push_back( h_absdiRap_M200to1500 );

		h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_lead_Pt );
		h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_sub_Pt );
		
		h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );

		h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );
		h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );

		h_endcap_Pt = new TH1D("h_endcap_Pt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_endcap_Pt );
		h_endcap_eta = new TH1D("h_endcap_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_endcap_eta );
		h_endcap_phi = new TH1D("h_endcap_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_endcap_phi );

		h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS );
		h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_SS );

		h_Pt_minusCharge = new TH1D("h_Pt_minusCharge_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_minusCharge );
		h_eta_minusCharge = new TH1D("h_eta_minusCharge_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_minusCharge );
		h_phi_minusCharge = new TH1D("h_phi_minusCharge_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_minusCharge );
		h_Pt_plusCharge = new TH1D("h_Pt_plusCharge_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_plusCharge );
		h_eta_plusCharge = new TH1D("h_eta_plusCharge_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_plusCharge );
		h_phi_plusCharge = new TH1D("h_phi_plusCharge_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_plusCharge );

		h_Pt_M70 = new TH1D("h_Pt_M70_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M70 );
		h_Pt_M70to90 = new TH1D("h_Pt_M70to90_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M70to90 );
		h_Pt_M90to110 = new TH1D("h_Pt_M90to110_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M90to110 );
		h_Pt_M110toInf = new TH1D("h_Pt_M110toInf_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M110toInf );
		h_Pt_M70to111 = new TH1D("h_Pt_M70to111_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M70to111 );

		h_Pt_M15to60 = new TH1D("h_Pt_M15to60_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M15to60 );
		h_Pt_M60to120 = new TH1D("h_Pt_M60to120_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M60to120 );
		h_Pt_M120to3000 = new TH1D("h_Pt_M120to3000_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M120to3000 );

		h_Pt_M15to30 = new TH1D("h_Pt_M15to30_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M15to30 );
		h_Pt_M30to45 = new TH1D("h_Pt_M30to45_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M30to45 );
		h_Pt_M45to60 = new TH1D("h_Pt_M45to60_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M45to60 );
		h_Pt_M120to200 = new TH1D("h_Pt_M120to200_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M120to200 );
		h_Pt_M200to3000 = new TH1D("h_Pt_M200to3000_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_M200to3000 );

		h_Angle = new TH1D("h_Angle_"+Type, "", 80, -4, 4); Histo.push_back( h_Angle );

		h_Pt_TrigLeg = new TH1D("h_Pt_TrigLeg_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_TrigLeg );
		h_eta_TrigLeg = new TH1D("h_eta_TrigLeg_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_TrigLeg );
		h_phi_TrigLeg = new TH1D("h_phi_TrigLeg_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_TrigLeg );

		h_Pt_OtherLeg = new TH1D("h_Pt_OtherLeg_"+Type, "", 10000, 0, 10000); Histo.push_back( h_Pt_OtherLeg );
		h_eta_OtherLeg = new TH1D("h_eta_OtherLeg_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_OtherLeg );
		h_phi_OtherLeg = new TH1D("h_phi_OtherLeg_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_OtherLeg );

		h_GenMass = new  TH1D("h_GenMass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenMass );
		h_GenPt = new  TH1D("h_GenPt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenPt );
		h_GenEta = new  TH1D("h_GenEta_"+Type, "", 200, -10, 10); Histo.push_back( h_GenEta );
		h_GenPhi = new  TH1D("h_GenPhi_"+Type, "", 80, -4, 4); Histo.push_back( h_GenPhi );

		h_GenPt_Lead = new  TH1D("h_GenPt_Lead_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenPt_Lead );
		h_GenEta_Lead = new  TH1D("h_GenEta_Lead_"+Type, "", 200, -10, 10); Histo.push_back( h_GenEta_Lead );
		h_GenPhi_Lead = new  TH1D("h_GenPhi_Lead_"+Type, "", 80, -4, 4); Histo.push_back( h_GenPhi_Lead );

		h_GenPt_Sub = new  TH1D("h_GenPt_Sub_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenPt_Sub );
		h_GenEta_Sub = new  TH1D("h_GenEta_Sub_"+Type, "", 200, -10, 10); Histo.push_back( h_GenEta_Sub );
		h_GenPhi_Sub = new  TH1D("h_GenPhi_Sub_"+Type, "", 80, -4, 4); Histo.push_back( h_GenPhi_Sub );

		h_GenDiPt = new TH1D("h_GenDiPt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenDiPt );
		h_GenDiRap = new TH1D("h_GenDiRap_"+Type, "", 200, -10, 10); Histo.push_back( h_GenDiRap );

		h_VtxProb = new TH1D("h_VtxProb_"+Type, "", 1000, 0, 1); Histo.push_back( h_VtxProb );
		h_VtxProb_belowM600 = new TH1D("h_VtxProb_belowM600_"+Type, "", 1000, 0, 1); Histo.push_back( h_VtxProb_belowM600 );
		h_VtxProb_overM600 = new TH1D("h_VtxProb_overM600_"+Type, "", 1000, 0, 1); Histo.push_back( h_VtxProb_overM600 );
		
		h_VtxNormChi2 = new TH1D("h_VtxNormChi2_"+Type, "", 1000, 0, 100); Histo.push_back( h_VtxNormChi2 );
		h_VtxNormChi2_belowM600 = new TH1D("h_VtxNormChi2_belowM600_"+Type, "", 1000, 0, 100); Histo.push_back( h_VtxNormChi2_belowM600 );
		h_VtxNormChi2_overM600 = new TH1D("h_VtxNormChi2_overM600_"+Type, "", 1000, 0, 100); Histo.push_back( h_VtxNormChi2_overM600 );

		h_mass_OS_BB = new TH1D("h_mass_OS_BB_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_BB );
		h_mass_OS_BE = new TH1D("h_mass_OS_BE_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_BE );
		h_mass_OS_EE = new TH1D("h_mass_OS_EE_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_EE );

		h_mass_OS_HLTv4p2 = new TH1D("h_mass_OS_HLTv4p2_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p2 );
		h_mass_OS_HLTv4p2_BB = new TH1D("h_mass_OS_HLTv4p2_BB_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p2_BB );
		h_mass_OS_HLTv4p2_BE = new TH1D("h_mass_OS_HLTv4p2_BE_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p2_BE );
		h_mass_OS_HLTv4p2_EE = new TH1D("h_mass_OS_HLTv4p2_EE_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p2_EE );

		h_mass_OS_HLTv4p3 = new TH1D("h_mass_OS_HLTv4p3_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3 );
		h_mass_OS_HLTv4p3_BB = new TH1D("h_mass_OS_HLTv4p3_BB_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3_BB );
		h_mass_OS_HLTv4p3_BE = new TH1D("h_mass_OS_HLTv4p3_BE_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3_BE );
		h_mass_OS_HLTv4p3_EE = new TH1D("h_mass_OS_HLTv4p3_EE_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3_EE );

		h_mass_OS_HLTv4p3_Split1 = new TH1D("h_mass_OS_HLTv4p3_Split1_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3_Split1 );
		h_mass_OS_HLTv4p3_Split2 = new TH1D("h_mass_OS_HLTv4p3_Split2_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3_Split2 );
		h_mass_OS_HLTv4p3_Split3 = new TH1D("h_mass_OS_HLTv4p3_Split3_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS_HLTv4p3_Split3 );

		h_muonHits = new TH1D("h_muonHits_"+Type, "", 50, 0, 50); Histo.push_back( h_muonHits );
		h_nMatches = new TH1D("h_nMatches_"+Type, "", 7, 0, 7); Histo.push_back( h_nMatches );
		h_RelPtError = new TH1D("h_RelPtError_"+Type, "", 50, 0, 0.5); Histo.push_back( h_RelPtError );
		h_dxyVTX = new TH1D("h_dxyVTX_"+Type, "", 100, -0.3, 0.3); Histo.push_back( h_dxyVTX );
		h_dzVTX = new TH1D("h_dzVTX_"+Type, "", 1000, -1.0, 1.0); Histo.push_back( h_dzVTX );
		h_pixelHits = new TH1D("h_pixelHits_"+Type, "", 15, 0, 15); Histo.push_back( h_pixelHits );
		h_trackerLayers = new TH1D("h_trackerLayers_"+Type, "", 20, 0, 20); Histo.push_back( h_trackerLayers );
		h_RelTrkIso = new TH1D("h_RelTrkIso_"+Type, "", 100, 0, 0.2); Histo.push_back( h_RelTrkIso ); 
	}

	void FillHistograms_GenDoubleMu(NtupleHandle *ntuple, GenLepton genlep1, GenLepton genlep2, Double_t weight)
	{
		FillHistograms_GenSingleMu( ntuple, genlep1, weight );
		FillHistograms_GenSingleMu( ntuple, genlep2, weight );

		TLorentzVector gen_v1 = genlep1.Momentum;
		TLorentzVector gen_v2 = genlep2.Momentum;
		Double_t gen_M = (gen_v1 + gen_v2).M();
		Double_t gen_DiPt = (gen_v1 + gen_v2).Pt();
		Double_t gen_DiRap = (gen_v1 + gen_v2).Rapidity();

		h_GenMass->Fill( gen_M, weight );
		h_GenDiPt->Fill( gen_DiPt, weight );
		h_GenDiRap->Fill( gen_DiRap, weight );

		if( genlep1.Pt > genlep2.Pt )
		{
			h_GenPt_Lead->Fill( genlep1.Pt, weight );
			h_GenEta_Lead->Fill( genlep1.eta, weight );
			h_GenPhi_Lead->Fill( genlep1.phi, weight );

			h_GenPt_Sub->Fill( genlep2.Pt, weight );
			h_GenEta_Sub->Fill( genlep2.eta, weight );
			h_GenPhi_Sub->Fill( genlep2.phi, weight );
		}
		else
		{
			h_GenPt_Lead->Fill( genlep2.Pt, weight );
			h_GenEta_Lead->Fill( genlep2.eta, weight );
			h_GenPhi_Lead->Fill( genlep2.phi, weight );

			h_GenPt_Sub->Fill( genlep1.Pt, weight );
			h_GenEta_Sub->Fill( genlep1.eta, weight );
			h_GenPhi_Sub->Fill( genlep1.phi, weight );
		}
	}

	void FillHistograms_GenSingleMu( NtupleHandle *ntuple, GenLepton genlep, Double_t weight )
	{
		h_GenPt->Fill( genlep.Pt, weight );
		h_GenEta->Fill( genlep.eta, weight );
		h_GenPhi->Fill( genlep.phi, weight );
	}

	void FillHistograms_SingleMu(NtupleHandle *ntuple, Muon recolep, Double_t weight)
	{
		//-- Pt ,eta, phi --//
		h_Pt->Fill( recolep.Pt, weight );
		h_eta->Fill( recolep.eta, weight );
		h_phi->Fill( recolep.phi, weight );

		//-- Barrel muons --//
		if( fabs(recolep.eta) < 0.9 )
		{
			h_barrel_Pt->Fill( recolep.Pt, weight );
			h_barrel_eta->Fill( recolep.eta, weight );
			h_barrel_phi->Fill( recolep.phi, weight );
		}
		else // -- endcap -- //
		{
			h_endcap_Pt->Fill( recolep.Pt, weight );
			h_endcap_eta->Fill( recolep.eta, weight );
			h_endcap_phi->Fill( recolep.phi, weight );
		}

		//-- Pt plots for negative/positive charged muons --//
		if( recolep.charge == -1 )
		{
			h_Pt_minusCharge->Fill( recolep.Pt, weight );
			h_eta_minusCharge->Fill( recolep.eta, weight );
			h_phi_minusCharge->Fill( recolep.phi, weight );
		}
		else if( recolep.charge == 1 )
		{
			h_Pt_plusCharge->Fill( recolep.Pt, weight );
			h_eta_plusCharge->Fill( recolep.eta, weight );
			h_phi_plusCharge->Fill( recolep.phi, weight );
		}

		//-- Trigger Matched objects --//
		if( analyzer->HLT == "HLT_IsoMu20_v* || HLT_IsoTkMu20_v*" )
		{
			if( recolep.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || recolep.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
			{
				h_Pt_TrigLeg->Fill( recolep.Pt, weight );
				h_eta_TrigLeg->Fill( recolep.eta, weight );
				h_phi_TrigLeg->Fill( recolep.phi, weight );
			}
			else
			{
				h_Pt_OtherLeg->Fill( recolep.Pt, weight );
				h_eta_OtherLeg->Fill( recolep.eta, weight );
				h_phi_OtherLeg->Fill( recolep.phi, weight );
			}
		}
		else
		{
			if( recolep.isTrigMatched(ntuple, analyzer->HLT) )
			{
				h_Pt_TrigLeg->Fill( recolep.Pt, weight );
				h_eta_TrigLeg->Fill( recolep.eta, weight );
				h_phi_TrigLeg->Fill( recolep.phi, weight );
			}
			else
			{
				h_Pt_OtherLeg->Fill( recolep.Pt, weight );
				h_eta_OtherLeg->Fill( recolep.eta, weight );
				h_phi_OtherLeg->Fill( recolep.phi, weight );
			}
		}

		h_muonHits->Fill( recolep.muonHits, weight );
		h_nMatches->Fill( recolep.nMatches, weight );
		h_RelPtError->Fill( recolep.Best_pTError / recolep.Best_pT, weight );
		h_dxyVTX->Fill( recolep.dxyVTX, weight );
		h_dzVTX->Fill( recolep.dzVTX, weight );
		h_pixelHits->Fill( recolep.pixelHits, weight );
		h_trackerLayers->Fill( recolep.trackerLayers, weight );
		h_RelTrkIso->Fill( recolep.trkiso, weight );
	}

	void FillHistograms_DoubleMu(NtupleHandle *ntuple, Muon recolep1, Muon recolep2, Double_t weight)
	{
		this->FillHistograms_SingleMu(ntuple, recolep1, weight);
		this->FillHistograms_SingleMu(ntuple, recolep2, weight);

		TLorentzVector reco_v1 = recolep1.Momentum;
		TLorentzVector reco_v2 = recolep2.Momentum;
		Double_t reco_M = (reco_v1 + reco_v2).M();

		//Dimuon Mass/Pt/Rapidity
		Double_t reco_Rap = (reco_v1 + reco_v2).Rapidity();
		Double_t reco_Pt = (reco_v1 + reco_v2).Pt();
		h_mass->Fill( reco_M, weight );
		h_diPt->Fill( reco_Pt, weight );
		h_diRap->Fill( reco_Rap, weight );

		// -- |y(ll)| distributions -- //
		if( reco_M >= 20 && reco_M < 30 )
			h_absdiRap_M20to30->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 30 && reco_M < 45 )
			h_absdiRap_M30to45->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 45 && reco_M < 60 )
			h_absdiRap_M45to60->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 60 && reco_M < 120 )
			h_absdiRap_M60to120->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 120 && reco_M < 200 )
			h_absdiRap_M120to200->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 200 && reco_M < 1500 )
			h_absdiRap_M200to1500->Fill( fabs(reco_Rap), weight);

		//leading, sub-leading muon pt,eta,phi distribution
		if( recolep1.Pt > recolep2.Pt )
		{
			h_lead_Pt->Fill( recolep1.Pt, weight );
			h_lead_eta->Fill( recolep1.eta, weight );
			h_lead_phi->Fill( recolep1.phi, weight );

			h_sub_Pt->Fill( recolep2.Pt, weight );
			h_sub_eta->Fill( recolep2.eta, weight );
			h_sub_phi->Fill( recolep2.phi, weight );
		}
		else
		{
			h_lead_Pt->Fill( recolep2.Pt, weight );
			h_lead_eta->Fill( recolep2.eta, weight );
			h_lead_phi->Fill( recolep2.phi, weight );

			h_sub_Pt->Fill( recolep1.Pt, weight );
			h_sub_eta->Fill( recolep1.eta, weight );
			h_sub_phi->Fill( recolep1.phi, weight );
		}

		//Same-Sign / Opposite Invariant Mass
		if( recolep1.charge != recolep2.charge )
		{
			h_mass_OS->Fill( reco_M, weight );

			Bool_t isBB = kFALSE;
			Bool_t isBE = kFALSE;
			Bool_t isEE = kFALSE;
			if( fabs(recolep1.eta) < 0.9 && fabs(recolep2.eta) < 0.9 )
				isBB = kTRUE;
			else if( fabs(recolep1.eta) > 0.9 && fabs(recolep2.eta) > 0.9 )
				isEE = kTRUE;
			else
				isBE = kTRUE;

			if( isBB == kTRUE )
				h_mass_OS_BB->Fill( reco_M, weight );
			else if( isBE == kTRUE )
				h_mass_OS_BE->Fill( reco_M, weight );
			else if( isEE == kTRUE )
				h_mass_OS_EE->Fill( reco_M, weight );

			if( isMC == kFALSE ) // -- Data -- //
			{
				if( ntuple->runNum < 257932.5 ) // -- HLT v4.2 -- //
				{
					h_mass_OS_HLTv4p2->Fill( reco_M, weight );

					if( isBB == kTRUE )
						h_mass_OS_HLTv4p2_BB->Fill( reco_M, weight );
					else if( isBE == kTRUE )
						h_mass_OS_HLTv4p2_BE->Fill( reco_M, weight );
					else if( isEE == kTRUE )
						h_mass_OS_HLTv4p2_EE->Fill( reco_M, weight );
				}
				else // -- HLT v4.3 -- //
				{
					h_mass_OS_HLTv4p3->Fill( reco_M, weight );

					if( isBB == kTRUE )
						h_mass_OS_HLTv4p3_BB->Fill( reco_M, weight );
					else if( isBE == kTRUE )
						h_mass_OS_HLTv4p3_BE->Fill( reco_M, weight );
					else if( isEE == kTRUE )
						h_mass_OS_HLTv4p3_EE->Fill( reco_M, weight );

					if( ntuple->runNum <= 258448 )
						h_mass_OS_HLTv4p3_Split1->Fill( reco_M, weight );
					else if( ntuple->runNum > 258448 && ntuple->runNum <= 259822 )
						h_mass_OS_HLTv4p3_Split2->Fill( reco_M, weight );
					else if( ntuple->runNum > 259822 )
						h_mass_OS_HLTv4p3_Split3->Fill( reco_M, weight );
				}
			}
			else // -- MC -- //
			{
				h_mass_OS_HLTv4p2->Fill( reco_M, weight );
				h_mass_OS_HLTv4p3->Fill( reco_M, weight );

				if( isBB == kTRUE )
				{
					h_mass_OS_HLTv4p2_BB->Fill( reco_M, weight );
					h_mass_OS_HLTv4p3_BB->Fill( reco_M, weight );
				}
				else if( isBE == kTRUE )
				{
					h_mass_OS_HLTv4p2_BE->Fill( reco_M, weight );
					h_mass_OS_HLTv4p3_BE->Fill( reco_M, weight );
				}
				else if( isEE == kTRUE )
				{
					h_mass_OS_HLTv4p2_EE->Fill( reco_M, weight );
					h_mass_OS_HLTv4p3_EE->Fill( reco_M, weight );
				}

				h_mass_OS_HLTv4p3_Split1->Fill( reco_M, weight );
				h_mass_OS_HLTv4p3_Split2->Fill( reco_M, weight );
				h_mass_OS_HLTv4p3_Split3->Fill( reco_M, weight );
			}


		}
		else
			h_mass_SS->Fill( reco_M, weight );

		//Pt plots with respect to their pair mass
		if( reco_M < 70 )
		{
			h_Pt_M70->Fill( recolep1.Pt, weight );
			h_Pt_M70->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 70 && reco_M < 90 )
		{
			h_Pt_M70to90->Fill( recolep1.Pt, weight );
			h_Pt_M70to90->Fill( recolep2.Pt, weight );

		}
		else if( reco_M > 90 && reco_M < 110 )
		{
			h_Pt_M90to110->Fill( recolep1.Pt, weight );
			h_Pt_M90to110->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 110 )
		{
			h_Pt_M110toInf->Fill( recolep1.Pt, weight );
			h_Pt_M110toInf->Fill( recolep2.Pt, weight );
		}

		if( reco_M > 70 && reco_M < 111 )
		{
			h_Pt_M70to111->Fill( recolep1.Pt, weight );
			h_Pt_M70to111->Fill( recolep2.Pt, weight );
		}

		if( reco_M > 15 && reco_M < 60 )
		{
			h_Pt_M15to60->Fill( recolep1.Pt, weight );
			h_Pt_M15to60->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 60 && reco_M < 120 )
		{
			h_Pt_M60to120->Fill( recolep1.Pt, weight );
			h_Pt_M60to120->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 120 && reco_M < 3000 )
		{
			h_Pt_M120to3000->Fill( recolep1.Pt, weight );
			h_Pt_M120to3000->Fill( recolep2.Pt, weight );
		}

		// -- Fine binning -- //
		if( reco_M > 15 && reco_M < 30 )
		{
			h_Pt_M15to30->Fill( recolep1.Pt, weight );
			h_Pt_M15to30->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 30 && reco_M < 45 )
		{
			h_Pt_M30to45->Fill( recolep1.Pt, weight );
			h_Pt_M30to45->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 45 && reco_M < 60 )
		{
			h_Pt_M45to60->Fill( recolep1.Pt, weight );
			h_Pt_M45to60->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 120 && reco_M < 200 )
		{
			h_Pt_M120to200->Fill( recolep1.Pt, weight );
			h_Pt_M120to200->Fill( recolep2.Pt, weight );
		}
		else if( reco_M > 200 && reco_M < 3000 )
		{
			h_Pt_M200to3000->Fill( recolep1.Pt, weight );
			h_Pt_M200to3000->Fill( recolep2.Pt, weight );
		}

		//Angle between two muons
		h_Angle->Fill( reco_v1.Angle( reco_v2.Vect() ), weight );

		Double_t VtxProb = 0;
		Double_t VtxNormChi2 = 0;
		DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

		h_VtxProb->Fill( VtxProb, weight );
		h_VtxNormChi2->Fill( VtxNormChi2, weight );
		if( reco_M < 600 )
		{
			h_VtxProb_belowM600->Fill( VtxProb, weight );
			h_VtxNormChi2_belowM600->Fill( VtxNormChi2, weight );
		}
		else if( reco_M > 600 )
		{
			h_VtxProb_overM600->Fill( VtxProb, weight );
			h_VtxNormChi2_overM600->Fill( VtxNormChi2, weight );
		}
		
	}

	void DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2)
	{
		vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
		vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
		vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

		Int_t NPt1 = (Int_t)PtCollection1->size();
		Int_t NPt2 = (Int_t)PtCollection2->size();
		Int_t NProb = (Int_t)VtxProbCollection->size();

		if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
			cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

		// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

		for(Int_t i=0; i<NProb; i++)
		{
			// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
			if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
			{
				*VtxProb = VtxProbCollection->at(i);
				*VtxNormChi2 = ntuple->vtxTrkChi2->at(i) / ntuple->vtxTrkNdof->at(i);
				break;
			}
		}

		return;
	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};

class ControlPlots_Electron
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	TH1D *h_mass;
	TH1D *h_diPt;
	TH1D *h_diRap;

	TH1D *h_lead_Pt;
	TH1D *h_lead_eta;
	TH1D *h_lead_phi;

	TH1D *h_sub_Pt;
	TH1D *h_sub_eta;
	TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;
	TH1D *h_barrel_mass;

	TH1D *h_endcap_Pt;
	TH1D *h_endcap_eta;
	TH1D *h_endcap_phi;
	TH1D *h_endcap_mass;

	TH1D *h_mass_OS;
	TH1D *h_mass_SS;

	TH1D *h_minusCharge_Pt;
	TH1D *h_minusCharge_eta;
	TH1D *h_minusCharge_phi;
	TH1D *h_plusCharge_Pt;
	TH1D *h_plusCharge_eta;
	TH1D *h_plusCharge_phi;

	TH1D *h_TrigLeg_Pt;
	TH1D *h_TrigLeg_eta;
	TH1D *h_TrigLeg_phi;

	TH1D *h_OtherLeg_Pt;
	TH1D *h_OtherLeg_eta;
	TH1D *h_OtherLeg_phi;

	TH1D *h_GenMass;

	ControlPlots_Electron(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;


		h_Pt = new TH1D("h_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		h_mass = new TH1D("h_mass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass );
		h_diPt = new TH1D("h_diPt_"+Type, "", 250, 0, 500); Histo.push_back( h_diPt );
		h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );

		h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_lead_Pt );
		h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );

		h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_sub_Pt );
		h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );
		h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );
		h_barrel_mass = new TH1D("h_barrel_mass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_barrel_mass );

		h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS );
		h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_SS );

		h_minusCharge_Pt = new TH1D("h_minusCharge_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_minusCharge_Pt );
		h_minusCharge_eta = new TH1D("h_minusCharge_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_minusCharge_eta );
		h_minusCharge_phi = new TH1D("h_phi_minusCharge_"+Type, "", 80, -4, 4); Histo.push_back( h_minusCharge_phi );
		h_plusCharge_Pt = new TH1D("h_plusCharge_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_plusCharge_Pt );
		h_plusCharge_eta = new TH1D("h_plusCharge_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_plusCharge_eta );
		h_plusCharge_phi = new TH1D("h_plusCharge_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_plusCharge_phi );

		h_TrigLeg_Pt = new TH1D("h_TrigLeg_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_TrigLeg_Pt );
		h_TrigLeg_eta = new TH1D("h_TrigLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_TrigLeg_eta );
		h_TrigLeg_phi = new TH1D("h_TrigLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_TrigLeg_phi );

		h_OtherLeg_Pt = new TH1D("h_OtherLeg_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_OtherLeg_Pt );
		h_OtherLeg_eta = new TH1D("h_OtherLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_OtherLeg_eta );
		h_OtherLeg_phi = new TH1D("h_OtherLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_OtherLeg_phi );

		h_GenMass = new  TH1D("h_GenMass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenMass );

	}

	void FillHistograms_GenElec(GenLepton genlep1, GenLepton genlep2, Double_t weight)
	{
		TLorentzVector gen_v1 = genlep1.Momentum;
		TLorentzVector gen_v2 = genlep2.Momentum;
		Double_t gen_M = (gen_v1 + gen_v2).M();

		h_GenMass->Fill( gen_M, weight );
	}

	void FillHistograms_SingleElec(Electron RecoObj, Double_t weight)
	{
		//-- Pt ,eta, phi --//
		h_Pt->Fill( RecoObj.Pt, weight );
		h_eta->Fill( RecoObj.eta, weight );
		h_phi->Fill( RecoObj.phi, weight );

		//-- Barrel muons --//
		if( fabs(RecoObj.eta) < 0.9 )
		{
			h_barrel_Pt->Fill( RecoObj.Pt, weight );
			h_barrel_eta->Fill( RecoObj.eta, weight );
			h_barrel_phi->Fill( RecoObj.phi, weight );
		}

		//-- Pt plots for negative/positive charged muons --//
		if( RecoObj.charge == -1 )
		{
			h_minusCharge_Pt->Fill( RecoObj.Pt, weight );
			h_minusCharge_eta->Fill( RecoObj.eta, weight );
			h_minusCharge_phi->Fill( RecoObj.phi, weight );
		}
		else if( RecoObj.charge == 1 )
		{
			h_plusCharge_Pt->Fill( RecoObj.Pt, weight );
			h_plusCharge_eta->Fill( RecoObj.eta, weight );
			h_plusCharge_phi->Fill( RecoObj.phi, weight );
		}

		TString HLT = analyzer->HLT;
		if( RecoObj.isTrigMatched(ntuple, HLT) )
		{
			h_TrigLeg_Pt->Fill( RecoObj.Pt, weight );
			h_TrigLeg_eta->Fill( RecoObj.eta, weight );
			h_TrigLeg_phi->Fill( RecoObj.phi, weight );
		}
		else
		{
			h_OtherLeg_Pt->Fill( RecoObj.Pt, weight );
			h_OtherLeg_eta->Fill( RecoObj.eta, weight );
			h_OtherLeg_phi->Fill( RecoObj.phi, weight );
		}
	}

	void FillHistograms_DoubleElec(Electron RecoObj1, Electron RecoObj2, Double_t weight)
	{
		this->FillHistograms_SingleElec(RecoObj1, weight);
		this->FillHistograms_SingleElec(RecoObj2, weight);

		TLorentzVector reco_v1 = RecoObj1.Momentum;
		TLorentzVector reco_v2 = RecoObj2.Momentum;
		Double_t reco_M = (reco_v1 + reco_v2).M();

		//Dielectron Mass/Pt/Rapidity
		Double_t reco_Rap = (reco_v1 + reco_v2).Rapidity();
		Double_t reco_Pt = (reco_v1 + reco_v2).Pt();
		h_mass->Fill( reco_M, weight );
		h_diPt->Fill( reco_Pt, weight );
		h_diRap->Fill( reco_Rap, weight );

		//leading, sub-leading electron pt,eta,phi distribution
		if( RecoObj1.Pt > RecoObj2.Pt )
		{
			h_lead_Pt->Fill( RecoObj1.Pt, weight );
			h_lead_eta->Fill( RecoObj1.eta, weight );
			h_lead_phi->Fill( RecoObj1.phi, weight );

			h_sub_Pt->Fill( RecoObj2.Pt, weight );
			h_sub_eta->Fill( RecoObj2.eta, weight );
			h_sub_phi->Fill( RecoObj2.phi, weight );
		}
		else
		{
			h_lead_Pt->Fill( RecoObj2.Pt, weight );
			h_lead_eta->Fill( RecoObj2.eta, weight );
			h_lead_phi->Fill( RecoObj2.phi, weight );

			h_sub_Pt->Fill( RecoObj1.Pt, weight );
			h_sub_eta->Fill( RecoObj1.eta, weight );
			h_sub_phi->Fill( RecoObj1.phi, weight );
		}

		//-- Mass of barrel Electrons --//
		if( fabs(RecoObj1.eta) < 0.9 && fabs(RecoObj2.eta) < 0.9 )
			h_barrel_mass->Fill( reco_M, weight );

		//Same-Sign / Opposite Invariant Mass
		if( RecoObj1.charge != RecoObj2.charge )
			h_mass_OS->Fill( reco_M, weight );
		else
			h_mass_SS->Fill( reco_M, weight );
	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};

class ControlPlots_Photon
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	// TH1D *h_mass;
	// TH1D *h_diPt;
	// TH1D *h_diRap;

	// TH1D *h_lead_Pt;
	// TH1D *h_lead_eta;
	// TH1D *h_lead_phi;

	// TH1D *h_sub_Pt;
	// TH1D *h_sub_eta;
	// TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;
	// TH1D *h_barrel_mass;

	TH1D *h_endcap_Pt;
	TH1D *h_endcap_eta;
	TH1D *h_endcap_phi;
	// TH1D *h_endcap_mass;

	// TH1D *h_mass_OS;
	// TH1D *h_mass_SS;

	// TH1D *h_TrigLeg_Pt;
	// TH1D *h_TrigLeg_eta;
	// TH1D *h_TrigLeg_phi;

	// TH1D *h_OtherLeg_Pt;
	// TH1D *h_OtherLeg_eta;
	// TH1D *h_OtherLeg_phi;

	// TH1D *h_GenMass;

	TH1D* h_hasPixelSeed;
	TH1D* h_etaSC;
	TH1D* h_phiSC;
	TH1D* h_HoverE;
	TH1D* h_Full5x5_SigmaIEtaIEta;
	TH1D* h_ChIso;
	TH1D* h_NhIso;
	TH1D* h_PhIso;
	TH1D* h_ChIsoWithEA;
	TH1D* h_NhIsoWithEA;
	TH1D* h_PhIsoWithEA;


	ControlPlots_Photon(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;

		h_Pt = new TH1D("h_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 80, -4, 4); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		// h_mass = new TH1D("h_mass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass );
		// h_diPt = new TH1D("h_diPt_"+Type, "", 250, 0, 500); Histo.push_back( h_diPt );
		// h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );

		// h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_lead_Pt );
		// h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		// h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );

		// h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_sub_Pt );
		// h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );
		// h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );
		// h_barrel_mass = new TH1D("h_barrel_mass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_barrel_mass );

		h_endcap_Pt = new TH1D("h_endcap_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_endcap_Pt );
		h_endcap_eta = new TH1D("h_endcap_eta_"+Type, "",  80, -4, 4); Histo.push_back( h_endcap_eta );
		h_endcap_phi = new TH1D("h_endcap_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_endcap_phi );

		// h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS );
		// h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_SS );

		// h_TrigLeg_Pt = new TH1D("h_TrigLeg_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_TrigLeg_Pt );
		// h_TrigLeg_eta = new TH1D("h_TrigLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_TrigLeg_eta );
		// h_TrigLeg_phi = new TH1D("h_TrigLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_TrigLeg_phi );

		// h_OtherLeg_Pt = new TH1D("h_OtherLeg_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_OtherLeg_Pt );
		// h_OtherLeg_eta = new TH1D("h_OtherLeg_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_OtherLeg_eta );
		// h_OtherLeg_phi = new TH1D("h_OtherLeg_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_OtherLeg_phi );

		// h_GenMass = new  TH1D("h_GenMass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenMass );

		h_hasPixelSeed = new TH1D("h_hasPixelSeed_"+Type, "", 2, 0, 2); Histo.push_back( h_hasPixelSeed );
		h_etaSC = new TH1D("h_etaSC_"+Type, "", 80, -4, 4); Histo.push_back( h_etaSC );
		h_phiSC = new TH1D("h_phiSC_"+Type, "", 80, -4, 4); Histo.push_back( h_phiSC );
		h_HoverE = new TH1D("h_HoverE_"+Type, "", 1000, 0, 1); Histo.push_back( h_HoverE );
		h_Full5x5_SigmaIEtaIEta = new TH1D("h_Full5x5_SigmaIEtaIEta_"+Type, "", 1000, 0, 1); Histo.push_back( h_Full5x5_SigmaIEtaIEta );
		h_ChIso = new TH1D("h_ChIso_"+Type, "", 1000, 0, 10); Histo.push_back( h_ChIso );
		h_NhIso = new TH1D("h_NhIso_"+Type, "", 1000, 0, 10); Histo.push_back( h_NhIso );
		h_PhIso = new TH1D("h_PhIso_"+Type, "", 1000, 0, 10); Histo.push_back( h_PhIso );
		h_ChIsoWithEA = new TH1D("h_ChIsoWithEA_"+Type, "", 1000, 0, 10); Histo.push_back( h_ChIsoWithEA );
		h_NhIsoWithEA = new TH1D("h_NhIsoWithEA_"+Type, "", 1000, 0, 10); Histo.push_back( h_NhIsoWithEA );
		h_PhIsoWithEA = new TH1D("h_PhIsoWithEA_"+Type, "", 1000, 0, 10); Histo.push_back( h_PhIsoWithEA );

	}

	// void FillHistograms_GenPhoton(GenLepton genlep1, GenLepton genlep2)
	// {
	// 	Double_t AllWeight = 1;

	// 	Double_t GenWeight;
	// 	ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

	// 	Double_t PUWeight = 1;
	// 	if( isMC == kTRUE )
	// 		PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

	// 	AllWeight = GenWeight * PUWeight;

	// 	TLorentzVector gen_v1 = genlep1.Momentum;
	// 	TLorentzVector gen_v2 = genlep2.Momentum;
	// 	Double_t gen_M = (gen_v1 + gen_v2).M();

	// 	h_GenMass->Fill( gen_M, AllWeight );
	// }

	void FillHistograms_SinglePhoton(Photon RecoObj)
	{
		Double_t AllWeight = 1;

		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		Double_t PUWeight = 1;
		if( isMC == kTRUE )
			PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

		AllWeight = GenWeight * PUWeight;

		//-- Pt ,eta, phi --//
		h_Pt->Fill( RecoObj.Pt, AllWeight );
		h_eta->Fill( RecoObj.eta, AllWeight );
		h_phi->Fill( RecoObj.phi, AllWeight );

		//-- Barrel photons --//
		if( fabs(RecoObj.eta) < 1.479 )
		{
			h_barrel_Pt->Fill( RecoObj.Pt, AllWeight );
			h_barrel_eta->Fill( RecoObj.eta, AllWeight );
			h_barrel_phi->Fill( RecoObj.phi, AllWeight );
		}
		else
		{
			h_endcap_Pt->Fill( RecoObj.Pt, AllWeight );
			h_endcap_eta->Fill( RecoObj.eta, AllWeight );
			h_endcap_phi->Fill( RecoObj.phi, AllWeight );
		}

		// TString HLT = analyzer->HLT;
		// if( RecoObj.isTrigMatched(ntuple, HLT) )
		// {
		// 	h_TrigLeg_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_TrigLeg_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_TrigLeg_phi->Fill( RecoObj.phi, AllWeight );
		// }
		// else
		// {
		// 	h_OtherLeg_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_OtherLeg_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_OtherLeg_phi->Fill( RecoObj.phi, AllWeight );
		// }

		h_hasPixelSeed->Fill( RecoObj.hasPixelSeed, AllWeight );
		h_etaSC->Fill( RecoObj.etaSC, AllWeight );
		h_phiSC->Fill( RecoObj.phiSC, AllWeight );
		h_HoverE->Fill( RecoObj.HoverE, AllWeight );
		h_Full5x5_SigmaIEtaIEta->Fill( RecoObj.Full5x5_SigmaIEtaIEta, AllWeight );
		h_ChIso->Fill( RecoObj.ChIso, AllWeight );
		h_NhIso->Fill( RecoObj.NhIso, AllWeight );
		h_PhIso->Fill( RecoObj.PhIso, AllWeight );
		h_ChIsoWithEA->Fill( RecoObj.ChIsoWithEA, AllWeight );
		h_NhIsoWithEA->Fill( RecoObj.NhIsoWithEA, AllWeight );
		h_PhIsoWithEA->Fill( RecoObj.PhIsoWithEA, AllWeight );
	}

	// void FillHistograms_DoublePhoton(Photon RecoObj1, Photon RecoObj2)
	// {
	// 	Double_t AllWeight = 1;

	// 	Double_t GenWeight;
	// 	ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

	// 	Double_t PUWeight = 1;
	// 	if( isMC == kTRUE )
	// 		PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

	// 	this->FillHistograms_SinglePhoton(RecoObj1);
	// 	this->FillHistograms_SinglePhoton(RecoObj2);

	// 	TLorentzVector reco_v1 = RecoObj1.Momentum;
	// 	TLorentzVector reco_v2 = RecoObj2.Momentum;
	// 	Double_t reco_M = (reco_v1 + reco_v2).M();

	// 	//DiPhoton Mass/Pt/Rapidity
	// 	Double_t reco_Rap = (reco_v1 + reco_v2).Rapidity();
	// 	Double_t reco_Pt = (reco_v1 + reco_v2).Pt();
	// 	h_mass->Fill( reco_M, AllWeight );
	// 	h_diPt->Fill( reco_Pt, AllWeight );
	// 	h_diRap->Fill( reco_Rap, AllWeight );

	// 	//leading, sub-leading Photon pt,eta,phi distribution
	// 	if( RecoObj1.Pt > RecoObj2.Pt )
	// 	{
	// 		h_lead_Pt->Fill( RecoObj1.Pt, AllWeight );
	// 		h_lead_eta->Fill( RecoObj1.eta, AllWeight );
	// 		h_lead_phi->Fill( RecoObj1.phi, AllWeight );

	// 		h_sub_Pt->Fill( RecoObj2.Pt, AllWeight );
	// 		h_sub_eta->Fill( RecoObj2.eta, AllWeight );
	// 		h_sub_phi->Fill( RecoObj2.phi, AllWeight );
	// 	}
	// 	else
	// 	{
	// 		h_lead_Pt->Fill( RecoObj2.Pt, AllWeight );
	// 		h_lead_eta->Fill( RecoObj2.eta, AllWeight );
	// 		h_lead_phi->Fill( RecoObj2.phi, AllWeight );

	// 		h_sub_Pt->Fill( RecoObj1.Pt, AllWeight );
	// 		h_sub_eta->Fill( RecoObj1.eta, AllWeight );
	// 		h_sub_phi->Fill( RecoObj1.phi, AllWeight );
	// 	}

	// 	//-- Mass of barrel Photons --//
	// 	if( fabs(RecoObj1.eta) < 0.9 && fabs(RecoObj2.eta) < 0.9 )
	// 		h_barrel_mass->Fill( reco_M, AllWeight );

	// 	//Same-Sign / Opposite Invariant Mass
	// 	if( RecoObj1.charge != RecoObj2.charge )
	// 		h_mass_OS->Fill( reco_M, AllWeight );
	// 	else
	// 		h_mass_SS->Fill( reco_M, AllWeight );
	// }

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};

class ControlPlots_Jet
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D *h_Jet_Pt;
	TH1D *h_Jet_eta;
	TH1D *h_Jet_phi;
	TH1D *h_Jet_nJets;

	TH1D *h_Jet_Charge;
	TH1D *h_Jet_Flavor;
	TH1D *h_Jet_bTag;
	TH1D *h_Jet_CHfrac;
	TH1D *h_Jet_NHfrac;
	TH1D *h_Jet_NHEMfrac;
	TH1D *h_Jet_CHEMfrac;
	TH1D *h_Jet_CHmulti;
	TH1D *h_Jet_NHmulti;

	// TH1D *h_barrel_Pt;
	// TH1D *h_barrel_eta;
	// TH1D *h_barrel_phi;

	// TH1D *h_endcap_Pt;
	// TH1D *h_endcap_eta;
	// TH1D *h_endcap_phi;


	ControlPlots_Jet(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;

		h_Jet_Pt = new TH1D("h_Jet_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_Jet_Pt );
		h_Jet_eta = new TH1D("h_Jet_eta_"+Type, "", 120, -6, 6); Histo.push_back( h_Jet_eta );
		h_Jet_phi = new TH1D("h_Jet_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_Jet_phi );
		h_Jet_nJets = new TH1D("h_Jet_nJets_"+Type, "", 100, 0, 100); Histo.push_back( h_Jet_nJets );

		// h_Jet_barrel_Pt = new TH1D("h_Jet_barrel_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_Jet_barrel_Pt );
		// h_Jet_barrel_eta = new TH1D("h_Jet_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_Jet_barrel_eta );
		// h_Jet_barrel_phi = new TH1D("h_Jet_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_Jet_barrel_phi );

		// h_Jet_endcap_Pt = new TH1D("h_Jet_endcap_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_Jet_endcap_Pt );
		// h_Jet_endcap_eta = new TH1D("h_Jet_endcap_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_Jet_endcap_eta );
		// h_Jet_endcap_phi = new TH1D("h_Jet_endcap_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_Jet_endcap_phi );

		h_Jet_Charge = new TH1D("h_Jet_Charge_"+Type, "", 40, -2, 2); Histo.push_back( h_Jet_Charge );
		h_Jet_Flavor = new TH1D("h_Jet_Flavor_"+Type, "", 200, -100, 100); Histo.push_back( h_Jet_Flavor );
		h_Jet_bTag = new TH1D("h_Jet_bTag_"+Type, "", 400, -2, 2); Histo.push_back( h_Jet_bTag );
		h_Jet_CHfrac = new TH1D("h_Jet_CHfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_CHfrac );
		h_Jet_NHfrac = new TH1D("h_Jet_NHfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_NHfrac );
		h_Jet_NHEMfrac = new TH1D("h_Jet_NHEMfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_NHEMfrac );
		h_Jet_CHEMfrac = new TH1D("h_Jet_CHEMfrac_"+Type, "", 200, 0, 2); Histo.push_back( h_Jet_CHEMfrac );
		h_Jet_CHmulti = new TH1D("h_Jet_CHmulti_"+Type, "", 100, 0, 100); Histo.push_back( h_Jet_CHmulti );
		h_Jet_NHmulti = new TH1D("h_Jet_NHmulti_"+Type, "", 100, 0, 100); Histo.push_back( h_Jet_NHmulti );

	}

	void FillHistograms_SingleJet(Jet RecoObj)
	{
		Double_t AllWeight = 1;

		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		Double_t PUWeight = 1;
		if( isMC == kTRUE )
			PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

		AllWeight = GenWeight * PUWeight;

		//-- Pt ,eta, phi --//
		h_Jet_Pt->Fill( RecoObj.Pt, AllWeight );
		h_Jet_eta->Fill( RecoObj.eta, AllWeight );
		h_Jet_phi->Fill( RecoObj.phi, AllWeight );
		h_Jet_nJets->Fill( ntuple->Njets, AllWeight );

		// //-- Barrel Jet --//
		// if( fabs(RecoObj.eta) < 1.479 )
		// {
		// 	h_Jet_barrel_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_Jet_barrel_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_Jet_barrel_phi->Fill( RecoObj.phi, AllWeight );
		// }
		// else
		// {
		// 	h_Jet_endcap_Pt->Fill( RecoObj.Pt, AllWeight );
		// 	h_Jet_endcap_eta->Fill( RecoObj.eta, AllWeight );
		// 	h_Jet_endcap_phi->Fill( RecoObj.phi, AllWeight );
		// }

		h_Jet_Charge->Fill( RecoObj.Charge, AllWeight );
		h_Jet_Flavor->Fill( RecoObj.Flavor, AllWeight );
		h_Jet_bTag->Fill( RecoObj.bTag, AllWeight );
		h_Jet_CHfrac->Fill( RecoObj.CHfrac, AllWeight );
		h_Jet_NHfrac->Fill( RecoObj.NHfrac, AllWeight );
		h_Jet_NHEMfrac->Fill( RecoObj.NHEMfrac, AllWeight );
		h_Jet_CHEMfrac->Fill( RecoObj.CHEMfrac, AllWeight );
		h_Jet_CHmulti->Fill( RecoObj.CHmulti, AllWeight );
		h_Jet_NHmulti->Fill( RecoObj.NHmulti, AllWeight );

	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};


class ControlPlots_MET
{
public:
	DYAnalyzer *analyzer;
	NtupleHandle *ntuple;
	Bool_t isMC;
	std::vector<TH1D*> Histo;

	TH1D* h_pfMET_pT;
	TH1D* h_pfMET_phi;
	TH1D* h_pfMET_Px;
	TH1D* h_pfMET_Py;
	TH1D* h_pfMET_SumEt;

	TH1D* h_pfMET_Type1_pT;
	TH1D* h_pfMET_Type1_phi;
	TH1D* h_pfMET_Type1_Px;
	TH1D* h_pfMET_Type1_Py;
	TH1D* h_pfMET_Type1_SumEt;

	ControlPlots_MET(TString Type, DYAnalyzer *dyanalyzer, NtupleHandle *ntuplehandle)
	{
		isMC = kFALSE;
		if( Type != "Data" )
			isMC = kTRUE;

		analyzer = dyanalyzer;
		ntuple = ntuplehandle;

		h_pfMET_pT = new TH1D("h_pfMET_pT_"+Type, "", 500, 0, 500); Histo.push_back( h_pfMET_pT );
		h_pfMET_phi = new TH1D("h_pfMET_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_pfMET_phi );
		h_pfMET_Px = new TH1D("h_pfMET_Px_"+Type, "", 1000, -500, 500); Histo.push_back( h_pfMET_Px );
		h_pfMET_Py = new TH1D("h_pfMET_Py_"+Type, "", 1000, -500, 500); Histo.push_back( h_pfMET_Py );
		h_pfMET_SumEt = new TH1D("h_pfMET_SumEt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_pfMET_SumEt );

		h_pfMET_Type1_pT = new TH1D("h_pfMET_Type1_pT_"+Type, "", 500, 0, 500); Histo.push_back( h_pfMET_Type1_pT );
		h_pfMET_Type1_phi = new TH1D("h_pfMET_Type1_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_pfMET_Type1_phi );
		h_pfMET_Type1_Px = new TH1D("h_pfMET_Type1_Px_"+Type, "", 1000, -500, 500); Histo.push_back( h_pfMET_Type1_Px );
		h_pfMET_Type1_Py = new TH1D("h_pfMET_Type1_Py_"+Type, "", 1000, -500, 500); Histo.push_back( h_pfMET_Type1_Py );
		h_pfMET_Type1_SumEt = new TH1D("h_pfMET_Type1_SumEt_"+Type, "", 10000, 0, 10000); Histo.push_back( h_pfMET_Type1_SumEt );
	}

	void FillHistograms_MET()
	{
		Double_t AllWeight = 1;

		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		Double_t PUWeight = 1;
		if( isMC == kTRUE )
			PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

		AllWeight = GenWeight * PUWeight;

		h_pfMET_pT->Fill( ntuple->pfMET_pT, AllWeight );
		h_pfMET_phi->Fill( ntuple->pfMET_phi, AllWeight );
		h_pfMET_Px->Fill( ntuple->pfMET_Px, AllWeight );
		h_pfMET_Py->Fill( ntuple->pfMET_Py, AllWeight );
		h_pfMET_SumEt->Fill( ntuple->pfMET_SumEt, AllWeight );

		h_pfMET_Type1_pT->Fill( ntuple->pfMET_Type1_pT, AllWeight );
		h_pfMET_Type1_phi->Fill( ntuple->pfMET_Type1_phi, AllWeight );
		h_pfMET_Type1_Px->Fill( ntuple->pfMET_Type1_Px, AllWeight );
		h_pfMET_Type1_Py->Fill( ntuple->pfMET_Type1_Py, AllWeight );
		h_pfMET_Type1_SumEt->Fill( ntuple->pfMET_Type1_SumEt, AllWeight );

	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};



































