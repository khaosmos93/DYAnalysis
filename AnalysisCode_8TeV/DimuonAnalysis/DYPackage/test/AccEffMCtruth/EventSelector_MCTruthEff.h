#ifndef EventSelector_MCTruthEff_h
#define EventSelector_MCTruthEff_h

#include <TROOT.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

class EventSelector_MCTruthEff : public TSelector {
public :
   //set up priors for the case when we do not use the ntuple weights
   double Sigma_DY1020;
   double Sigma_DY20 ;
   double Sigma_DY200;
   double Sigma_DY400;
   double Sigma_DY500;
   double Sigma_DY700;
   double Sigma_DY800;
   double Sigma_DY1000;

   double FilterEff_DY1020;
   double FilterEff_DY20 ;
   double FilterEff_DY200;
   double FilterEff_DY400;
   double FilterEff_DY500;
   double FilterEff_DY700;
   double FilterEff_DY800;
   double FilterEff_DY1000;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TString        dataset;

   TNamed*        ds;
   TNamed*        this_useNtupleWeightFlag;

   TString        useNtupleWeightFlag;

   //define histo here
   TH1D*          Nntuple;

   TH1D*          hsel;
   TH1D*          hsim;
   TH1D*          hacc;
   TH1D*          haccFSR;
   TH1D*          hfull;

   TH1D*    hacc_PU;
   TH1D*    hsim_PU;
   TH1D*    haccFSR_PU;
   TH1D*    hsel_PU;
   TH1D*    hfull_PU;

   TH1D*    hacc_corr;
   TH1D*    hsim_corr;
   TH1D*    haccFSR_corr;
   TH1D*    hsel_corr;
   TH1D*    hfull_corr;
  
   TH1D*          fAbsRap_full;
   TH1D*          fAbsRap_acc;
   TH1D*          fAbsRap_accFSR;
   TH1D*          fAbsRap_sel;
   TH1D*          fAbsRap_sim;

   TH1D*          fAbsRap_full_PU;
   TH1D*          fAbsRap_acc_PU;
   TH1D*          fAbsRap_accFSR_PU;
   TH1D*          fAbsRap_sel_PU;
   TH1D*          fAbsRap_sim_PU;

   TH1D*          fAbsRap_full_corr;
   TH1D*          fAbsRap_acc_corr;
   TH1D*          fAbsRap_accFSR_corr;
   TH1D*          fAbsRap_sel_corr;
   TH1D*          fAbsRap_sim_corr;

   Int_t          fNumberOfEvents; // Total number of events
   Int_t          fNumberOfGoodEvents; // Total number of good events
   Int_t          fTotalDataSize; // Sum of data size (in bytes) of all events

   TString        trigPaths[100];

   // Declaration of leaf types
   Int_t           runNum;
   Int_t           evtNum;
   Int_t           lumiBlock;

   Int_t           nPair;
   Double_t        ntupleWeight;

   Double_t        pileUpReweight;
   Double_t        pileUpReweightIn;

/*   Int_t           Nmuons;
   //Double_t        PUweight;
   Double_t        sumEt;
   Double_t        photonEt;
   Double_t        chargedHadronEt;
   Double_t        neutralHadronEt;
   Double_t        nVertices;
   Int_t           PVtrackSize;
   Double_t        PVchi2;
   Double_t        PVndof;
   Double_t        PVnormalizedChi2;
   Double_t        PVx;
   Double_t        PVy;
   Double_t        PVz;
   //trigger
*/
   Double_t        HLT_Mu17_Mu8;
   Int_t           hlt_ntrig;
   Int_t           hlt_trigType[500000];
   Bool_t           hlt_trigFired[500000];
   Double_t        hlt_trigPt[500000];
   Double_t        hlt_trigEta[500000];
   Double_t        hlt_trigPhi[500000];

/*
   //Int_t           Njets;
   //Double_t        MET;
*/
/*   Int_t           Nelectrons;
   Double_t        Electron_E[2];   //[Nelectrons]
   Double_t        Electron_pT[2];   //[Nelectrons]
   Double_t        Electron_Px[2];   //[Nelectrons]
   Double_t        Electron_Py[2];   //[Nelectrons]
   Double_t        Electron_Pz[2];   //[Nelectrons]
   Double_t        Electron_eta[2];   //[Nelectrons]
   Double_t        Electron_phi[2];   //[Nelectrons]
   Double_t        Electron_charge[2];   //[Nelectrons]
   Double_t        Electron_eID[2];   //[Nelectrons]
   Double_t        Electron_trkiso[2];   //[Nelectrons]
   Double_t        Electron_hcaliso[2];   //[Nelectrons]
   Double_t        Electron_ecaliso[2];   //[Nelectrons]
*/
   Double_t        InvMass[500000];   //[nPair]
//   Double_t        Angle[500000];   //[nPair]
   Double_t        CosAngle[500000];   //[nPair]
/*   Double_t        DeltaR[500000];   //[nPair]
   Double_t        Dalpha[500000];   //[nPair]
*/
   Bool_t           isOppSign[500000];   //[nPair]
//   Int_t           MuonPairIndex[500000];   //[nPair]
//   Double_t        vtxChi2[500000];   //[nPair]
   Double_t        vtxProb[500000];   //[nPair]
   Double_t        vtxTrkProb[500000];   //[nPair]
/*   Double_t        vtxNdof[500000];   //[nPair]
   Double_t        vtxPositionX[500000];   //[nPair]
   Double_t        vtxPositionY[500000];   //[nPair]
   Double_t        vtxPositionZ[500000];   //[nPair]
   Double_t        vtxXerror[500000];   //[nPair]
   Double_t        vtxYerror[500000];   //[nPair]
   Double_t        vtxZerror[500000];   //[nPair]
   Double_t        vtx_newPx[500000];   //[nPair]
   Double_t        vtx_newPy[500000];   //[nPair]
   Double_t        vtx_newPz[500000];   //[nPair]
   Double_t        vtx_newMass[500000];   //[nPair]
   Double_t        vtx_newMassErr[500000];   //[nPair]
*/
   Int_t           Muon1_muonType[500000];   //[nPair]
/*   Int_t           Muon1_nTrig[500000];   //[nPair]
   Int_t           Muon1_triggerObjectType[500000];   //[nPair]
   Int_t           Muon1_filterName[500000];   //[nPair]
*/ Double_t        Muon1_phi[500000];   //[nPair]


   Double_t        Muon1_eta[500000];   //[nPair]
   Double_t        Muon1_pT[500000];   //[nPair]
   Double_t        Muon1_Px[500000];   //[nPair]
   Double_t        Muon1_Py[500000];   //[nPair]
   Double_t        Muon1_Pz[500000];   //[nPair]
/*   Float_t         Muon1_CosmicTimeTight[500000];   //[nPair]
   Int_t           Muon1_isBackToBack2LegCosmic[500000];   //[nPair]
   Float_t         Muon1_CosmicCompat[500000];   //[nPair]
   Double_t        Muon1_sumtrkpt[500000];   //[nPair]
   Double_t        Muon1_trkiso[500000];   //[nPair]
   Double_t        Muon1_hcaliso[500000];   //[nPair]
   Double_t        Muon1_ecaliso[500000];   //[nPair]
   Double_t        Muon1_trkisoR05[500000];   //[nPair]
   Double_t        Muon1_hcalisoR05[500000];   //[nPair]
   Double_t        Muon1_ecalisoR05[500000];   //[nPair]
*/
   //Double_t        Muon1_PfChargedHadronIsoR05[500000];   //[nPair]
   //Double_t        Muon1_PfNeutralHadronIsoR05[500000];   //[nPair]
   //Double_t        Muon1_PfGammaIsoR05[500000];   //[nPair]
   Double_t        Muon1_PfChargedHadronIsoR03[500000];   //[nPair]
   Double_t        Muon1_PfNeutralHadronIsoR03[500000];   //[nPair]
   Double_t        Muon1_PfGammaIsoR03[500000];   //[nPair]
   //Int_t           Muon1_charge[500000];   //[nPair]
   Int_t           Muon1_nChambers[500000];   //[nPair]
   Int_t           Muon1_nMatches[500000];   //[nPair]
   Int_t           Muon1_nTrackerLayers[1000000];
   Int_t           Muon1_stationMask[500000];   //[nPair]
   Int_t           Muon1_nSegments[500000];   //[nPair]
   Double_t        Muon1_chi2dof[500000];   //[nPair]
   Int_t           Muon1_nhits[500000];   //[nPair]
   Int_t           Muon1_trackerHits[500000];   //[nPair]
   Int_t           Muon1_pixelHits[500000];   //[nPair]
   Int_t           Muon1_muonHits[500000];   //[nPair]
   /*Double_t        Muon1_qoverp[500000];   //[nPair]
   Double_t        Muon1_theta[500000];   //[nPair]
   Double_t        Muon1_lambda[500000];   //[nPair]
   Double_t        Muon1_dxy[500000];   //[nPair]
   Double_t        Muon1_d0[500000];   //[nPair]
   Double_t        Muon1_dsz[500000];   //[nPair]
   Double_t        Muon1_dz[500000];   //[nPair]
   */
   Double_t        Muon1_dxyBS[500000];   //[nPair]
   /*Double_t        Muon1_dszBS[500000];   //[nPair]
   Double_t        Muon1_dzBS[500000];   //[nPair]
   Double_t        Muon1_dxyVTX[500000];   //[nPair]
   Double_t        Muon1_dszVTX[500000];   //[nPair]
   Double_t        Muon1_dzVTX[500000];   //[nPair]
   Double_t        Muon1_dxyDiMuVTX[500000];   //[nPair]
   Double_t        Muon1_dszDiMuVTX[500000];   //[nPair]
   Double_t        Muon1_dzDiMuVTX[500000];   //[nPair]
   Double_t        Muon1_vx[500000];   //[nPair]
   Double_t        Muon1_vy[500000];   //[nPair]
   Double_t        Muon1_vz[500000];   //[nPair]
   Double_t        Muon1_MCtruth_pT[500000];   //[nPair]
   Double_t        Muon1_MCtruth_Px[500000];   //[nPair]
   Double_t        Muon1_MCtruth_Py[500000];   //[nPair]
   Double_t        Muon1_MCtruth_Pz[500000];   //[nPair]
   Double_t        Muon1_MCtruth_eta[500000];   //[nPair]
   Double_t        Muon1_MCtruth_phi[500000];   //[nPair]
   Int_t           Muon1_MCtruth_charge[500000];   //[nPair]
   Int_t           Muon1_MCtruth_mother[500000];   //[nPair]
   Int_t           Muon1_MCtruth_pdgId[500000];   //[nPair]
   Double_t        Muon1_timeInsideOut[500000];   //[nPair]
   Double_t        Muon1_timeOutsideIn[500000];   //[nPair]
   Int_t           Muon1_combtime_nDof[500000];   //[nPair]
   Int_t           Muon1_combtime_direction[500000];   //[nPair]
   Double_t        Muon1_combtime_inverseBeta[500000];   //[nPair]
   Double_t        Muon1_combtime_freeInverseBeta[500000];   //[nPair]
   Double_t        Muon1_combtime_timeAtIpInOut[500000];   //[nPair]
   Double_t        Muon1_combtime_timeAtIpOutIn[500000];   //[nPair]
   Double_t        Muon1_combtime_inverseBetaErr[500000];   //[nPair]
   Double_t        Muon1_combtime_freeInverseBetaErr[500000];   //[nPair]
   Double_t        Muon1_combtime_timeAtIpOutInErr[500000];   //[nPair]
   Double_t        Muon1_combtime_timeAtIpInOutErr[500000];   //[nPair]
   Int_t           Muon1_dttime_nDof[500000];   //[nPair]
   Int_t           Muon1_dttime_direction[500000];   //[nPair]
   Double_t        Muon1_dttime_inverseBeta[500000];   //[nPair]
   Double_t        Muon1_dttime_freeInverseBeta[500000];   //[nPair]
   Double_t        Muon1_dttime_timeAtIpInOut[500000];   //[nPair]
   Double_t        Muon1_dttime_timeAtIpOutIn[500000];   //[nPair]
   Double_t        Muon1_dttime_inverseBetaErr[500000];   //[nPair]
   Double_t        Muon1_dttime_freeInverseBetaErr[500000];   //[nPair]
   Double_t        Muon1_dttime_timeAtIpOutInErr[500000];   //[nPair]
   Double_t        Muon1_dttime_timeAtIpInOutErr[500000];   //[nPair]
   Int_t           Muon1_csctime_nDof[500000];   //[nPair]
   Int_t           Muon1_csctime_direction[500000];   //[nPair]
   Double_t        Muon1_csctime_inverseBeta[500000];   //[nPair]
   Double_t        Muon1_csctime_freeInverseBeta[500000];   //[nPair]
   Double_t        Muon1_csctime_timeAtIpInOut[500000];   //[nPair]
   Double_t        Muon1_csctime_timeAtIpOutIn[500000];   //[nPair]
   Double_t        Muon1_csctime_inverseBetaErr[500000];   //[nPair]
   Double_t        Muon1_csctime_freeInverseBetaErr[500000];   //[nPair]
   Double_t        Muon1_csctime_timeAtIpOutInErr[500000];   //[nPair]
   Double_t        Muon1_csctime_timeAtIpInOutErr[500000];   //[nPair]
   Double_t        Muon1_caloCompatibility[500000];   //[nPair]
   Double_t        Muon1_segCompatibility[500000];   //[nPair]
   Double_t        Muon1_CaloEM[500000];   //[nPair]
   Double_t        Muon1_CaloHad[500000];   //[nPair]
   Double_t        Muon1_ID[500000];   //[nPair]
   */

   Int_t           Muon2_muonType[500000];   //[nPair]
   /*
   Int_t           Muon2_nTrig[500000];   //[nPair]
   Int_t           Muon2_triggerObjectType[500000];   //[nPair]
   Int_t           Muon2_filterName[500000];   //[nPair]
*/
   Double_t        Muon2_phi[500000];   //[nPair]

   Double_t        Muon2_eta[500000];   //[nPair]
   Double_t        Muon2_pT[500000];   //[nPair]
   Double_t        Muon2_Px[500000];   //[nPair]
   Double_t        Muon2_Py[500000];   //[nPair]
   Double_t        Muon2_Pz[500000];   //[nPair]
/*   Float_t         Muon2_CosmicTimeTight[500000];   //[nPair]
   Int_t           Muon2_isBackToBack2LegCosmic[500000];   //[nPair]
   Float_t         Muon2_CosmicCompat[500000];   //[nPair]
   Double_t        Muon2_sumtrkpt[500000];   //[nPair]
   Double_t        Muon2_trkiso[500000];   //[nPair]
   Double_t        Muon2_hcaliso[500000];   //[nPair]
   Double_t        Muon2_ecaliso[500000];   //[nPair]
   Double_t        Muon2_trkisoR05[500000];   //[nPair]
   Double_t        Muon2_hcalisoR05[500000];   //[nPair]
   Double_t        Muon2_ecalisoR05[500000];   //[nPair]
*/
   //Double_t        Muon2_PfChargedHadronIsoR05[500000];   //[nPair]
   //Double_t        Muon2_PfNeutralHadronIsoR05[500000];   //[nPair]
   //Double_t        Muon2_PfGammaIsoR05[500000];   //[nPair]
   Double_t        Muon2_PfChargedHadronIsoR03[500000];   //[nPair]
   Double_t        Muon2_PfNeutralHadronIsoR03[500000];   //[nPair]
   Double_t        Muon2_PfGammaIsoR03[500000];   //[nPair]
   //Int_t           Muon2_charge[500000];   //[nPair]
   Int_t           Muon2_nChambers[500000];   //[nPair]
   Int_t           Muon2_nMatches[500000];   //[nPair]
   Int_t           Muon2_nTrackerLayers[1000000];
   Int_t           Muon2_stationMask[500000];   //[nPair]
   Int_t           Muon2_nSegments[500000];   //[nPair]
   Double_t        Muon2_chi2dof[500000];   //[nPair]
   Int_t           Muon2_nhits[500000];   //[nPair]
   Int_t           Muon2_trackerHits[500000];   //[nPair]
   Int_t           Muon2_pixelHits[500000];   //[nPair]
   Int_t           Muon2_muonHits[500000];   //[nPair]
   /*Double_t        Muon2_qoverp[500000];   //[nPair]
   Double_t        Muon2_theta[500000];   //[nPair]
   Double_t        Muon2_lambda[500000];   //[nPair]
   Double_t        Muon2_dxy[500000];   //[nPair]
   */
   Double_t        Muon2_dxyBS[500000];   //[nPair]
   /*
   Double_t        Muon2_dszBS[500000];   //[nPair]
   Double_t        Muon2_dzBS[500000];   //[nPair]
   Double_t        Muon2_dxyVTX[500000];   //[nPair]
   Double_t        Muon2_dszVTX[500000];   //[nPair]
   Double_t        Muon2_dzVTX[500000];   //[nPair]
   Double_t        Muon2_dxyDiMuVTX[500000];   //[nPair]
   Double_t        Muon2_dszDiMuVTX[500000];   //[nPair]
   Double_t        Muon2_dzDiMuVTX[500000];   //[nPair]
   Double_t        Muon2_d0[500000];   //[nPair]
   Double_t        Muon2_dsz[500000];   //[nPair]
   Double_t        Muon2_dz[500000];   //[nPair]
   Double_t        Muon2_vx[500000];   //[nPair]
   Double_t        Muon2_vy[500000];   //[nPair]
   Double_t        Muon2_vz[500000];   //[nPair]
   Double_t        Muon2_MCtruth_pT[500000];   //[nPair]
   Double_t        Muon2_MCtruth_Px[500000];   //[nPair]
   Double_t        Muon2_MCtruth_Py[500000];   //[nPair]
   Double_t        Muon2_MCtruth_Pz[500000];   //[nPair]
   Double_t        Muon2_MCtruth_eta[500000];   //[nPair]
   Double_t        Muon2_MCtruth_phi[500000];   //[nPair]
   Int_t           Muon2_MCtruth_charge[500000];   //[nPair]
   Int_t           Muon2_MCtruth_mother[500000];   //[nPair]
   Int_t           Muon2_MCtruth_pdgId[500000];   //[nPair]
   Double_t        Muon2_timeInsideOut[500000];   //[nPair]
   Double_t        Muon2_timeOutsideIn[500000];   //[nPair]
   Int_t           Muon2_combtime_nDof[500000];   //[nPair]
   Int_t           Muon2_combtime_direction[500000];   //[nPair]
   Double_t        Muon2_combtime_inverseBeta[500000];   //[nPair]
   Double_t        Muon2_combtime_freeInverseBeta[500000];   //[nPair]
   Double_t        Muon2_combtime_timeAtIpInOut[500000];   //[nPair]
   Double_t        Muon2_combtime_timeAtIpOutIn[500000];   //[nPair]
   Double_t        Muon2_combtime_inverseBetaErr[500000];   //[nPair]
   Double_t        Muon2_combtime_freeInverseBetaErr[500000];   //[nPair]
   Double_t        Muon2_combtime_timeAtIpOutInErr[500000];   //[nPair]
   Double_t        Muon2_combtime_timeAtIpInOutErr[500000];   //[nPair]
   Int_t           Muon2_dttime_nDof[500000];   //[nPair]
   Int_t           Muon2_dttime_direction[500000];   //[nPair]
   Double_t        Muon2_dttime_inverseBeta[500000];   //[nPair]
   Double_t        Muon2_dttime_freeInverseBeta[500000];   //[nPair]
   Double_t        Muon2_dttime_timeAtIpInOut[500000];   //[nPair]
   Double_t        Muon2_dttime_timeAtIpOutIn[500000];   //[nPair]
   Double_t        Muon2_dttime_inverseBetaErr[500000];   //[nPair]
   Double_t        Muon2_dttime_freeInverseBetaErr[500000];   //[nPair]
   Double_t        Muon2_dttime_timeAtIpOutInErr[500000];   //[nPair]
   Double_t        Muon2_dttime_timeAtIpInOutErr[500000];   //[nPair]
   Int_t           Muon2_csctime_nDof[500000];   //[nPair]
   Int_t           Muon2_csctime_direction[500000];   //[nPair]
   Double_t        Muon2_csctime_inverseBeta[500000];   //[nPair]
   Double_t        Muon2_csctime_freeInverseBeta[500000];   //[nPair]
   Double_t        Muon2_csctime_timeAtIpInOut[500000];   //[nPair]
   Double_t        Muon2_csctime_timeAtIpOutIn[500000];   //[nPair]
   Double_t        Muon2_csctime_inverseBetaErr[500000];   //[nPair]
   Double_t        Muon2_csctime_freeInverseBetaErr[500000];   //[nPair]
   Double_t        Muon2_csctime_timeAtIpOutInErr[500000];   //[nPair]
   Double_t        Muon2_csctime_timeAtIpInOutErr[500000];   //[nPair]
   Double_t        Muon2_caloCompatibility[500000];   //[nPair]
   Double_t        Muon2_segCompatibility[500000];   //[nPair]
   Double_t        Muon2_CaloEM[500000];   //[nPair]
   Double_t        Muon2_CaloHad[500000];   //[nPair]
   Double_t        Muon2_ID[500000];   //[nPair]
*/
   Int_t           GENnPair;
   Double_t        GENInvMass[500000];   //[GENnPair]
   Double_t        GENRapidity[1000000];   //[GENnPair]

//   Double_t        GENAngle[1];   //[GENnPair]
   Bool_t           GENisOppSign[500000];   //[GENnPair]
   Double_t        GENMuon1_phi[500000];   //[GENnPair]
   Double_t        GENMuon1_eta[500000];   //[GENnPair]
   Double_t        GENMuon1_pT[500000];   //[GENnPair]
   Double_t        GENMuon1_Px[500000];   //[GENnPair]
   Double_t        GENMuon1_Py[500000];   //[GENnPair]
   Double_t        GENMuon1_Pz[500000];   //[GENnPair]

   Int_t        GENMuon1_mother[500000];   //[GENnPair]
   Int_t        GENMuon1_status[500000];
/*
   Int_t           GENMuon1_charge[1];   //[GENnPair]
*/
   Double_t        GENMuon2_phi[500000];   //[GENnPair]
   Double_t        GENMuon2_eta[500000];   //[GENnPair]
   Double_t        GENMuon2_pT[500000];   //[GENnPair]
   Double_t        GENMuon2_Px[500000];   //[GENnPair]
   Double_t        GENMuon2_Py[500000];   //[GENnPair]
   Double_t        GENMuon2_Pz[500000];   //[GENnPair]

   Int_t        GENMuon2_mother[500000];   //[GENnPair]
   Int_t        GENMuon2_status[500000];
/*
   Int_t           GENMuon2_charge[1];   //[GENnPair]
*/
   // List of branches
   TBranch    *fEventSizeBranch;  // Pointer to the event.fEventSize branch
   TBranch        *b_runNum;   //!
   TBranch        *b_evtNum;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_nPair;   //!
   TBranch        *b_ntupleWeight;   //!
/*   TBranch        *b_Nmuons;   //!
   TBranch        *b_PUweight;   //!
   TBranch        *b_sumEt;   //!
   TBranch        *b_photonEt;   //!
   TBranch        *b_chargedHadronEt;   //!
   TBranch        *b_neutralHadronEt;   //!
   TBranch        *b_nVertices;   //!
   TBranch        *b_PVtrackSize;   //!
   TBranch        *b_PVchi2;   //!
   TBranch        *b_PVndof;   //!
   TBranch        *b_PVnormalizedChi2;   //!
   TBranch        *b_PVx;   //!
   TBranch        *b_PVy;   //!
   TBranch        *b_PVz;   //!
*/
   TBranch        *b_HLT_Mu17_Mu8;   //!
   //TBranch        *b_HLT_Mu13_Mu8;   //!
   //TBranch        *b_HLT_DoubleMu6;
   TBranch        *b_HLT_ntrig;
   TBranch        *b_HLT_trigFired;
   TBranch        *b_HLT_trigType;
   TBranch        *b_HLT_trigPt;
   TBranch        *b_HLT_trigEta;
   TBranch        *b_HLT_trigPhi;

   TBranch        *b_pileUpReweight;
   TBranch        *b_pileUpReweightIn;

/*
   TBranch        *b_Njets;   //!
   TBranch        *b_MET;   //!
*/
/*   TBranch        *b_Nelectrons;   //!
   TBranch        *b_Electron_E;   //!
   TBranch        *b_Electron_pT;   //!
   TBranch        *b_Electron_Px;   //!
   TBranch        *b_Electron_Py;   //!
   TBranch        *b_Electron_Pz;   //!
   TBranch        *b_Electron_eta;   //!
   TBranch        *b_Electron_phi;   //!
   TBranch        *b_Electron_charge;   //!
   TBranch        *b_Electron_eID;   //!
   TBranch        *b_Electron_trkiso;   //!
   TBranch        *b_Electron_hcaliso;   //!
   TBranch        *b_Electron_ecaliso;   //!
*/
   TBranch        *b_InvMass;   //!
   //TBranch        *b_Angle;   //!
   TBranch        *b_CosAngle;   //!
   //TBranch        *b_DeltaR;   //!
   //TBranch        *b_Dalpha;   //!
   TBranch        *b_isOppSign;   //!
   //TBranch        *b_MuonPairIndex;   //!
   //TBranch        *b_vtxChi2;   //!
   TBranch        *b_vtxProb;   //!
   TBranch        *b_vtxTrkProb;   //!
   /*TBranch        *b_vtxNdof;   //!
   TBranch        *b_vtxPositionX;   //!
   TBranch        *b_vtxPositionY;   //!
   TBranch        *b_vtxPositionZ;   //!
   TBranch        *b_vtxXerror;   //!
   TBranch        *b_vtxYerror;   //!
   TBranch        *b_vtxZerror;   //!
   TBranch        *b_vtx_newPx;   //!
   TBranch        *b_vtx_newPy;   //!
   TBranch        *b_vtx_newPz;   //!
   TBranch        *b_vtx_newMass;   //!
   TBranch        *b_vtx_newMassErr;   //!
   */
   TBranch        *b_Muon1_muonType;   //!
   /*TBranch        *b_Muon1_nTrig;   //!
   TBranch        *b_Muon1_triggerObjectType;   //!
   TBranch        *b_Muon1_filterName;   //!
   */
   TBranch        *b_Muon1_phi;   //!
   TBranch        *b_Muon1_eta;   //!
   TBranch        *b_Muon1_pT;   //!
   TBranch        *b_Muon1_Px;   //!
   TBranch        *b_Muon1_Py;   //!
   TBranch        *b_Muon1_Pz;   //!
   /*TBranch        *b_Muon1_CosmicTimeTight;   //!
   TBranch        *b_Muon1_isBackToBack2LegCosmic;   //!
   TBranch        *b_Muon1_CosmicCompat;   //!
   TBranch        *b_Muon1_sumtrkpt;   //!
   TBranch        *b_Muon1_trkiso;   //!
   TBranch        *b_Muon1_hcaliso;   //!
   TBranch        *b_Muon1_ecaliso;   //!
   TBranch        *b_Muon1_trkisoR05;   //!
   TBranch        *b_Muon1_hcalisoR05;   //!
   TBranch        *b_Muon1_ecalisoR05;   //!
*/
   //TBranch        *b_Muon1_PfChargedHadronIsoR05;   //!
   //TBranch        *b_Muon1_PfNeutralHadronIsoR05;   //!
   //TBranch        *b_Muon1_PfGammaIsoR05;   //!
   TBranch        *b_Muon1_PfChargedHadronIsoR03;   //!
   TBranch        *b_Muon1_PfNeutralHadronIsoR03;   //!
   TBranch        *b_Muon1_PfGammaIsoR03;   //!
   //TBranch        *b_Muon1_charge;   //!
   TBranch        *b_Muon1_nChambers;   //!
   TBranch        *b_Muon1_nMatches;   //!
   TBranch        *b_Muon1_nTrackerLayers;
   TBranch        *b_Muon1_stationMask;   //!
   TBranch        *b_Muon1_nSegments;   //!
   TBranch        *b_Muon1_chi2dof;   //!
   TBranch        *b_Muon1_nhits;   //!
   TBranch        *b_Muon1_trackerHits;   //!
   TBranch        *b_Muon1_pixelHits;   //!
   TBranch        *b_Muon1_muonHits;   //!
   /*TBranch        *b_Muon1_qoverp;   //!
   TBranch        *b_Muon1_theta;   //!
   TBranch        *b_Muon1_lambda;   //!
   TBranch        *b_Muon1_dxy;   //!
   TBranch        *b_Muon1_d0;   //!
   TBranch        *b_Muon1_dsz;   //!
   TBranch        *b_Muon1_dz;   //!
*/   
   TBranch        *b_Muon1_dxyBS;   //!
/*   TBranch        *b_Muon1_dszBS;   //!
   TBranch        *b_Muon1_dzBS;   //!
   TBranch        *b_Muon1_dxyVTX;   //!
   TBranch        *b_Muon1_dszVTX;   //!
   TBranch        *b_Muon1_dzVTX;   //!
   TBranch        *b_Muon1_dxyDiMuVTX;   //!
   TBranch        *b_Muon1_dszDiMuVTX;   //!
   TBranch        *b_Muon1_dzDiMuVTX;   //!
   TBranch        *b_Muon1_vx;   //!
   TBranch        *b_Muon1_vy;   //!
   TBranch        *b_Muon1_vz;   //!
   TBranch        *b_Muon1_MCtruth_pT;   //!
   TBranch        *b_Muon1_MCtruth_Px;   //!
   TBranch        *b_Muon1_MCtruth_Py;   //!
   TBranch        *b_Muon1_MCtruth_Pz;   //!
   TBranch        *b_Muon1_MCtruth_eta;   //!
   TBranch        *b_Muon1_MCtruth_phi;   //!
   TBranch        *b_Muon1_MCtruth_charge;   //!
   TBranch        *b_Muon1_MCtruth_mother;   //!
   TBranch        *b_Muon1_MCtruth_pdgId;   //!
   TBranch        *b_Muon1_timeInsideOut;   //!
   TBranch        *b_Muon1_timeOutsideIn;   //!
   TBranch        *b_Muon1_combtime_nDof;   //!
   TBranch        *b_Muon1_combtime_direction;   //!
   TBranch        *b_Muon1_combtime_inverseBeta;   //!
   TBranch        *b_Muon1_combtime_freeInverseBeta;   //!
   TBranch        *b_Muon1_combtime_timeAtIpInOut;   //!
   TBranch        *b_Muon1_combtime_timeAtIpOutIn;   //!
   TBranch        *b_Muon1_combtime_inverseBetaErr;   //!
   TBranch        *b_Muon1_combtime_freeInverseBetaErr;   //!
   TBranch        *b_Muon1_combtime_timeAtIpOutInErr;   //!
   TBranch        *b_Muon1_combtime_timeAtIpInOutErr;   //!
   TBranch        *b_Muon1_dttime_nDof;   //!
   TBranch        *b_Muon1_dttime_direction;   //!
   TBranch        *b_Muon1_dttime_inverseBeta;   //!
   TBranch        *b_Muon1_dttime_freeInverseBeta;   //!
   TBranch        *b_Muon1_dttime_timeAtIpInOut;   //!
   TBranch        *b_Muon1_dttime_timeAtIpOutIn;   //!
   TBranch        *b_Muon1_dttime_inverseBetaErr;   //!
   TBranch        *b_Muon1_dttime_freeInverseBetaErr;   //!
   TBranch        *b_Muon1_dttime_timeAtIpOutInErr;   //!
   TBranch        *b_Muon1_dttime_timeAtIpInOutErr;   //!
   TBranch        *b_Muon1_csctime_nDof;   //!
   TBranch        *b_Muon1_csctime_direction;   //!
   TBranch        *b_Muon1_csctime_inverseBeta;   //!
   TBranch        *b_Muon1_csctime_freeInverseBeta;   //!
   TBranch        *b_Muon1_csctime_timeAtIpInOut;   //!
   TBranch        *b_Muon1_csctime_timeAtIpOutIn;   //!
   TBranch        *b_Muon1_csctime_inverseBetaErr;   //!
   TBranch        *b_Muon1_csctime_freeInverseBetaErr;   //!
   TBranch        *b_Muon1_csctime_timeAtIpOutInErr;   //!
   TBranch        *b_Muon1_csctime_timeAtIpInOutErr;   //!
   TBranch        *b_Muon1_caloCompatibility;   //!
   TBranch        *b_Muon1_segCompatibility;   //!
   TBranch        *b_Muon1_CaloEM;   //!
   TBranch        *b_Muon1_CaloHad;   //!
   TBranch        *b_Muon1_ID;   //!
   */
   TBranch        *b_Muon2_muonType;   //!
   /*TBranch        *b_Muon2_nTrig;   //!
   TBranch        *b_Muon2_triggerObjectType;   //!
   TBranch        *b_Muon2_filterName;   //!
   */
   TBranch        *b_Muon2_phi;   //!
   
   TBranch        *b_Muon2_eta;   //!
   TBranch        *b_Muon2_pT;   //!
   TBranch        *b_Muon2_Px;   //!
   TBranch        *b_Muon2_Py;   //!
   TBranch        *b_Muon2_Pz;   //!
/*   TBranch        *b_Muon2_CosmicTimeTight;   //!
   TBranch        *b_Muon2_isBackToBack2LegCosmic;   //!
   TBranch        *b_Muon2_CosmicCompat;   //!
   TBranch        *b_Muon2_sumtrkpt;   //!
   TBranch        *b_Muon2_trkiso;   //!
   TBranch        *b_Muon2_hcaliso;   //!
   TBranch        *b_Muon2_ecaliso;   //!
   TBranch        *b_Muon2_trkisoR05;   //!
   TBranch        *b_Muon2_hcalisoR05;   //!
   TBranch        *b_Muon2_ecalisoR05;   //!
*/
   //TBranch        *b_Muon2_PfChargedHadronIsoR05;   //!
   //TBranch        *b_Muon2_PfNeutralHadronIsoR05;   //!
   //TBranch        *b_Muon2_PfGammaIsoR05;   //!
   TBranch        *b_Muon2_PfChargedHadronIsoR03;   //!
   TBranch        *b_Muon2_PfNeutralHadronIsoR03;   //!
   TBranch        *b_Muon2_PfGammaIsoR03;   //!
   //TBranch        *b_Muon2_charge;   //!
   TBranch        *b_Muon2_nChambers;   //!
   TBranch        *b_Muon2_nMatches;   //!
   TBranch        *b_Muon2_nTrackerLayers;
   TBranch        *b_Muon2_stationMask;   //!
   TBranch        *b_Muon2_nSegments;   //!
   TBranch        *b_Muon2_chi2dof;   //!
   TBranch        *b_Muon2_nhits;   //!
   TBranch        *b_Muon2_trackerHits;   //!
   TBranch        *b_Muon2_pixelHits;   //!
   TBranch        *b_Muon2_muonHits;   //!
   /*TBranch        *b_Muon2_qoverp;   //!
   TBranch        *b_Muon2_theta;   //!
   TBranch        *b_Muon2_lambda;   //!
   TBranch        *b_Muon2_dxy;   //!
   */
   TBranch        *b_Muon2_dxyBS;   //!
   /*TBranch        *b_Muon2_dszBS;   //!
   TBranch        *b_Muon2_dzBS;   //!
   TBranch        *b_Muon2_dxyVTX;   //!
   TBranch        *b_Muon2_dszVTX;   //!
   TBranch        *b_Muon2_dzVTX;   //!
   TBranch        *b_Muon2_dxyDiMuVTX;   //!
   TBranch        *b_Muon2_dszDiMuVTX;   //!
   TBranch        *b_Muon2_dzDiMuVTX;   //!
   TBranch        *b_Muon2_d0;   //!
   TBranch        *b_Muon2_dsz;   //!
   TBranch        *b_Muon2_dz;   //!
   TBranch        *b_Muon2_vx;   //!
   TBranch        *b_Muon2_vy;   //!
   TBranch        *b_Muon2_vz;   //!
   TBranch        *b_Muon2_MCtruth_pT;   //!
   TBranch        *b_Muon2_MCtruth_Px;   //!
   TBranch        *b_Muon2_MCtruth_Py;   //!
   TBranch        *b_Muon2_MCtruth_Pz;   //!
   TBranch        *b_Muon2_MCtruth_eta;   //!
   TBranch        *b_Muon2_MCtruth_phi;   //!
   TBranch        *b_Muon2_MCtruth_charge;   //!
   TBranch        *b_Muon2_MCtruth_mother;   //!
   TBranch        *b_Muon2_MCtruth_pdgId;   //!
   TBranch        *b_Muon2_timeInsideOut;   //!
   TBranch        *b_Muon2_timeOutsideIn;   //!
   TBranch        *b_Muon2_combtime_nDof;   //!
   TBranch        *b_Muon2_combtime_direction;   //!
   TBranch        *b_Muon2_combtime_inverseBeta;   //!
   TBranch        *b_Muon2_combtime_freeInverseBeta;   //!
   TBranch        *b_Muon2_combtime_timeAtIpInOut;   //!
   TBranch        *b_Muon2_combtime_timeAtIpOutIn;   //!
   TBranch        *b_Muon2_combtime_inverseBetaErr;   //!
   TBranch        *b_Muon2_combtime_freeInverseBetaErr;   //!
   TBranch        *b_Muon2_combtime_timeAtIpOutInErr;   //!
   TBranch        *b_Muon2_combtime_timeAtIpInOutErr;   //!
   TBranch        *b_Muon2_dttime_nDof;   //!
   TBranch        *b_Muon2_dttime_direction;   //!
   TBranch        *b_Muon2_dttime_inverseBeta;   //!
   TBranch        *b_Muon2_dttime_freeInverseBeta;   //!
   TBranch        *b_Muon2_dttime_timeAtIpInOut;   //!
   TBranch        *b_Muon2_dttime_timeAtIpOutIn;   //!
   TBranch        *b_Muon2_dttime_inverseBetaErr;   //!
   TBranch        *b_Muon2_dttime_freeInverseBetaErr;   //!
   TBranch        *b_Muon2_dttime_timeAtIpOutInErr;   //!
   TBranch        *b_Muon2_dttime_timeAtIpInOutErr;   //!
   TBranch        *b_Muon2_csctime_nDof;   //!
   TBranch        *b_Muon2_csctime_direction;   //!
   TBranch        *b_Muon2_csctime_inverseBeta;   //!
   TBranch        *b_Muon2_csctime_freeInverseBeta;   //!
   TBranch        *b_Muon2_csctime_timeAtIpInOut;   //!
   TBranch        *b_Muon2_csctime_timeAtIpOutIn;   //!
   TBranch        *b_Muon2_csctime_inverseBetaErr;   //!
   TBranch        *b_Muon2_csctime_freeInverseBetaErr;   //!
   TBranch        *b_Muon2_csctime_timeAtIpOutInErr;   //!
   TBranch        *b_Muon2_csctime_timeAtIpInOutErr;   //!
   TBranch        *b_Muon2_caloCompatibility;   //!
   TBranch        *b_Muon2_segCompatibility;   //!
   TBranch        *b_Muon2_CaloEM;   //!
   TBranch        *b_Muon2_CaloHad;   //!
   TBranch        *b_Muon2_ID;   //!
*/
   TBranch        *b_GENnPair;   //!
   TBranch        *b_GENInvMass;   //!
   TBranch        *b_GENRapidity; //!


//   TBranch        *b_GENAngle;   //!
   TBranch        *b_GENisOppSign;   //!
   TBranch        *b_GENMuon1_phi;   //!
   TBranch        *b_GENMuon1_eta;   //!
   TBranch        *b_GENMuon1_pT;   //!
   TBranch        *b_GENMuon1_Px;   //!
   TBranch        *b_GENMuon1_Py;   //!
   TBranch        *b_GENMuon1_Pz;   //!

   TBranch        *b_GENMuon1_mother;   //!
   TBranch        *b_GENMuon1_status;

//   TBranch        *b_GENMuon1_charge;   //!
   TBranch        *b_GENMuon2_phi;   //!
   TBranch        *b_GENMuon2_eta;   //!
   TBranch        *b_GENMuon2_pT;   //!
   TBranch        *b_GENMuon2_Px;   //!
   TBranch        *b_GENMuon2_Py;   //!
   TBranch        *b_GENMuon2_Pz;   //!

   TBranch        *b_GENMuon2_mother;   //!
   TBranch        *b_GENMuon2_status;
/*
   TBranch        *b_GENMuon2_charge;   //!
*/

   EventSelector_MCTruthEff(TTree* =  0);
   virtual ~EventSelector_MCTruthEff();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree*);
   virtual void    SlaveBegin(TTree*);
   virtual void    Init(TTree*);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   double Rapidity( double , double, double, double, double, double);

   ClassDef(EventSelector_MCTruthEff,0);
};

#endif

#ifdef EventSelector_MCTruthEff_cxx
void EventSelector_MCTruthEff::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   //fChain = dynamic_cast<TChain *>(tree);
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("evtNum", &evtNum, &b_evtNum);
   fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
   fChain->SetBranchAddress("nPair", &nPair, &b_nPair);
   //fChain->SetBranchAddress("weight", &ntupleWeight, &b_ntupleWeight);
/*   fChain->SetBranchAddress("Nmuons", &Nmuons, &b_Nmuons);
   fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
   fChain->SetBranchAddress("sumEt", &sumEt, &b_sumEt);
   fChain->SetBranchAddress("photonEt", &photonEt, &b_photonEt);
   fChain->SetBranchAddress("chargedHadronEt", &chargedHadronEt, &b_chargedHadronEt);
   fChain->SetBranchAddress("neutralHadronEt", &neutralHadronEt, &b_neutralHadronEt);
   fChain->SetBranchAddress("nVertices", &nVertices, &b_nVertices);
   fChain->SetBranchAddress("PVtrackSize", &PVtrackSize, &b_PVtrackSize);
   fChain->SetBranchAddress("PVchi2", &PVchi2, &b_PVchi2);
   fChain->SetBranchAddress("PVndof", &PVndof, &b_PVndof);
   fChain->SetBranchAddress("PVnormalizedChi2", &PVnormalizedChi2, &b_PVnormalizedChi2);
   fChain->SetBranchAddress("PVx", &PVx, &b_PVx);
   fChain->SetBranchAddress("PVy", &PVy, &b_PVy);
   fChain->SetBranchAddress("PVz", &PVz, &b_PVz);
*/
   fChain->SetBranchAddress("HLT_Mu17_Mu8", &HLT_Mu17_Mu8, &b_HLT_Mu17_Mu8);
   //fChain->SetBranchAddress("HLT_Mu13_Mu8", &HLT_Mu13_Mu8, &b_HLT_Mu13_Mu8);
   //fChain->SetBranchAddress("HLT_DoubleMu6", &HLT_DoubleMu6, &b_HLT_DoubleMu6);
   fChain->SetBranchAddress("HLT_ntrig", &hlt_ntrig, &b_HLT_ntrig);
   fChain->SetBranchAddress("HLT_trigType", hlt_trigType, &b_HLT_trigType);
   fChain->SetBranchAddress("HLT_trigFired",hlt_trigFired,&b_HLT_trigFired);
   fChain->SetBranchAddress("HLT_trigPt", hlt_trigPt,&b_HLT_trigPt);
   fChain->SetBranchAddress("HLT_trigEta", hlt_trigEta, &b_HLT_trigEta);
   fChain->SetBranchAddress("HLT_trigPhi", hlt_trigPhi, &b_HLT_trigPhi);


/*   fChain->SetBranchAddress("Njets", &Njets, &b_Njets);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
*/
/*   fChain->SetBranchAddress("Nelectrons", &Nelectrons, &b_Nelectrons);
   fChain->SetBranchAddress("Electron_E", Electron_E, &b_Electron_E);
   fChain->SetBranchAddress("Electron_pT", Electron_pT, &b_Electron_pT);
   fChain->SetBranchAddress("Electron_Px", Electron_Px, &b_Electron_Px);
   fChain->SetBranchAddress("Electron_Py", Electron_Py, &b_Electron_Py);
   fChain->SetBranchAddress("Electron_Pz", Electron_Pz, &b_Electron_Pz);
   fChain->SetBranchAddress("Electron_eta", Electron_eta, &b_Electron_eta);
   fChain->SetBranchAddress("Electron_phi", Electron_phi, &b_Electron_phi);
   fChain->SetBranchAddress("Electron_charge", Electron_charge, &b_Electron_charge);
   fChain->SetBranchAddress("Electron_eID", Electron_eID, &b_Electron_eID);
   fChain->SetBranchAddress("Electron_trkiso", Electron_trkiso, &b_Electron_trkiso);
   fChain->SetBranchAddress("Electron_hcaliso", Electron_hcaliso, &b_Electron_hcaliso);
   fChain->SetBranchAddress("Electron_ecaliso", Electron_ecaliso, &b_Electron_ecaliso);
*/

   fChain->SetBranchAddress("InvMass", InvMass, &b_InvMass);
   //fChain->SetBranchAddress("Angle", Angle, &b_Angle);
   fChain->SetBranchAddress("CosAngle", CosAngle, &b_CosAngle);
   //fChain->SetBranchAddress("DeltaR", DeltaR, &b_DeltaR);
   //fChain->SetBranchAddress("Dalpha", Dalpha, &b_Dalpha);
   fChain->SetBranchAddress("isOppSign", isOppSign, &b_isOppSign);
   //fChain->SetBranchAddress("MuonPairIndex", MuonPairIndex, &b_MuonPairIndex);
   //fChain->SetBranchAddress("vtxChi2", vtxChi2, &b_vtxChi2);
   fChain->SetBranchAddress("vtxProb", vtxProb, &b_vtxProb);
   fChain->SetBranchAddress("vtxTrkProb", vtxTrkProb, &b_vtxTrkProb);
   /*fChain->SetBranchAddress("vtxNdof", vtxNdof, &b_vtxNdof);
   fChain->SetBranchAddress("vtxPositionX", vtxPositionX, &b_vtxPositionX);
   fChain->SetBranchAddress("vtxPositionY", vtxPositionY, &b_vtxPositionY);
   fChain->SetBranchAddress("vtxPositionZ", vtxPositionZ, &b_vtxPositionZ);
   fChain->SetBranchAddress("vtxXerror", vtxXerror, &b_vtxXerror);
   fChain->SetBranchAddress("vtxYerror", vtxYerror, &b_vtxYerror);
   fChain->SetBranchAddress("vtxZerror", vtxZerror, &b_vtxZerror);
   fChain->SetBranchAddress("vtx_newPx", vtx_newPx, &b_vtx_newPx);
   fChain->SetBranchAddress("vtx_newPy", vtx_newPy, &b_vtx_newPy);
   fChain->SetBranchAddress("vtx_newPz", vtx_newPz, &b_vtx_newPz);
   fChain->SetBranchAddress("vtx_newMass", vtx_newMass, &b_vtx_newMass);
   fChain->SetBranchAddress("vtx_newMassErr", vtx_newMassErr, &b_vtx_newMassErr);
   */
   fChain->SetBranchAddress("Muon1_muonType", Muon1_muonType, &b_Muon1_muonType);
   //fChain->SetBranchAddress("Muon1_nTrig", Muon1_nTrig, &b_Muon1_nTrig);
   //fChain->SetBranchAddress("Muon1_triggerObjectType", Muon1_triggerObjectType, &b_Muon1_triggerObjectType);
   //fChain->SetBranchAddress("Muon1_filterName", Muon1_filterName, &b_Muon1_filterName);
   fChain->SetBranchAddress("Muon1_phi", Muon1_phi, &b_Muon1_phi);
   fChain->SetBranchAddress("Muon1_eta", Muon1_eta, &b_Muon1_eta);
   
   fChain->SetBranchAddress("Muon1_pT", Muon1_pT, &b_Muon1_pT);
   fChain->SetBranchAddress("Muon1_Px", Muon1_Px, &b_Muon1_Px);
   fChain->SetBranchAddress("Muon1_Py", Muon1_Py, &b_Muon1_Py);
   fChain->SetBranchAddress("Muon1_Pz", Muon1_Pz, &b_Muon1_Pz);
   /*fChain->SetBranchAddress("Muon1_CosmicTimeTight", Muon1_CosmicTimeTight, &b_Muon1_CosmicTimeTight);
   fChain->SetBranchAddress("Muon1_isBackToBack2LegCosmic", Muon1_isBackToBack2LegCosmic, &b_Muon1_isBackToBack2LegCosmic);
   fChain->SetBranchAddress("Muon1_CosmicCompat", Muon1_CosmicCompat, &b_Muon1_CosmicCompat);
   fChain->SetBranchAddress("Muon1_sumtrkpt", Muon1_sumtrkpt, &b_Muon1_sumtrkpt);
   fChain->SetBranchAddress("Muon1_trkiso", Muon1_trkiso, &b_Muon1_trkiso);
   fChain->SetBranchAddress("Muon1_hcaliso", Muon1_hcaliso, &b_Muon1_hcaliso);
   fChain->SetBranchAddress("Muon1_ecaliso", Muon1_ecaliso, &b_Muon1_ecaliso);
   fChain->SetBranchAddress("Muon1_trkisoR05", Muon1_trkisoR05, &b_Muon1_trkisoR05);
   fChain->SetBranchAddress("Muon1_hcalisoR05", Muon1_hcalisoR05, &b_Muon1_hcalisoR05);
   fChain->SetBranchAddress("Muon1_ecalisoR05", Muon1_ecalisoR05, &b_Muon1_ecalisoR05);
   */
   fChain->SetBranchAddress("pileUpReweight", &pileUpReweight, &b_pileUpReweight);
   fChain->SetBranchAddress("pileUpReweightIn", &pileUpReweightIn, &b_pileUpReweightIn);
   //fChain->SetBranchAddress("Muon1_PfChargedHadronIsoR05", Muon1_PfChargedHadronIsoR05, &b_Muon1_PfChargedHadronIsoR05);
   //fChain->SetBranchAddress("Muon1_PfNeutralHadronIsoR05", Muon1_PfNeutralHadronIsoR05, &b_Muon1_PfNeutralHadronIsoR05);
   //fChain->SetBranchAddress("Muon1_PfGammaIsoR05", Muon1_PfGammaIsoR05, &b_Muon1_PfGammaIsoR05);
   fChain->SetBranchAddress("Muon1_PfChargedHadronIsoR03", Muon1_PfChargedHadronIsoR03, &b_Muon1_PfChargedHadronIsoR03);
   fChain->SetBranchAddress("Muon1_PfNeutralHadronIsoR03", Muon1_PfNeutralHadronIsoR03, &b_Muon1_PfNeutralHadronIsoR03);
   fChain->SetBranchAddress("Muon1_PfGammaIsoR03", Muon1_PfGammaIsoR03, &b_Muon1_PfGammaIsoR03);
   //fChain->SetBranchAddress("Muon1_charge", Muon1_charge, &b_Muon1_charge);
   fChain->SetBranchAddress("Muon1_nChambers", Muon1_nChambers, &b_Muon1_nChambers);
   fChain->SetBranchAddress("Muon1_nMatches", Muon1_nMatches, &b_Muon1_nMatches);
   fChain->SetBranchAddress("Muon1_nTrackerLayers", Muon1_nTrackerLayers, &b_Muon1_nTrackerLayers);
   fChain->SetBranchAddress("Muon1_stationMask", Muon1_stationMask, &b_Muon1_stationMask);
   fChain->SetBranchAddress("Muon1_nSegments", Muon1_nSegments, &b_Muon1_nSegments);
   fChain->SetBranchAddress("Muon1_chi2dof", Muon1_chi2dof, &b_Muon1_chi2dof);
   fChain->SetBranchAddress("Muon1_nhits", Muon1_nhits, &b_Muon1_nhits);
   fChain->SetBranchAddress("Muon1_trackerHits", Muon1_trackerHits, &b_Muon1_trackerHits);
   fChain->SetBranchAddress("Muon1_pixelHits", Muon1_pixelHits, &b_Muon1_pixelHits);
   fChain->SetBranchAddress("Muon1_muonHits", Muon1_muonHits, &b_Muon1_muonHits);
   /*fChain->SetBranchAddress("Muon1_qoverp", Muon1_qoverp, &b_Muon1_qoverp);
   fChain->SetBranchAddress("Muon1_theta", Muon1_theta, &b_Muon1_theta);
   fChain->SetBranchAddress("Muon1_lambda", Muon1_lambda, &b_Muon1_lambda);
   fChain->SetBranchAddress("Muon1_dxy", Muon1_dxy, &b_Muon1_dxy);
   fChain->SetBranchAddress("Muon1_d0", Muon1_d0, &b_Muon1_d0);
   fChain->SetBranchAddress("Muon1_dsz", Muon1_dsz, &b_Muon1_dsz);
   fChain->SetBranchAddress("Muon1_dz", Muon1_dz, &b_Muon1_dz);
   */
   fChain->SetBranchAddress("Muon1_dxyBS", Muon1_dxyBS, &b_Muon1_dxyBS);
   /*fChain->SetBranchAddress("Muon1_dszBS", Muon1_dszBS, &b_Muon1_dszBS);
   fChain->SetBranchAddress("Muon1_dzBS", Muon1_dzBS, &b_Muon1_dzBS);
   fChain->SetBranchAddress("Muon1_dxyVTX", Muon1_dxyVTX, &b_Muon1_dxyVTX);
   fChain->SetBranchAddress("Muon1_dszVTX", Muon1_dszVTX, &b_Muon1_dszVTX);
   fChain->SetBranchAddress("Muon1_dzVTX", Muon1_dzVTX, &b_Muon1_dzVTX);
   fChain->SetBranchAddress("Muon1_dxyDiMuVTX", Muon1_dxyDiMuVTX, &b_Muon1_dxyDiMuVTX);
   fChain->SetBranchAddress("Muon1_dszDiMuVTX", Muon1_dszDiMuVTX, &b_Muon1_dszDiMuVTX);
   fChain->SetBranchAddress("Muon1_dzDiMuVTX", Muon1_dzDiMuVTX, &b_Muon1_dzDiMuVTX);
   fChain->SetBranchAddress("Muon1_vx", Muon1_vx, &b_Muon1_vx);
   fChain->SetBranchAddress("Muon1_vy", Muon1_vy, &b_Muon1_vy);
   fChain->SetBranchAddress("Muon1_vz", Muon1_vz, &b_Muon1_vz);
   fChain->SetBranchAddress("Muon1_MCtruth_pT", Muon1_MCtruth_pT, &b_Muon1_MCtruth_pT);
   fChain->SetBranchAddress("Muon1_MCtruth_Px", Muon1_MCtruth_Px, &b_Muon1_MCtruth_Px);
   fChain->SetBranchAddress("Muon1_MCtruth_Py", Muon1_MCtruth_Py, &b_Muon1_MCtruth_Py);
   fChain->SetBranchAddress("Muon1_MCtruth_Pz", Muon1_MCtruth_Pz, &b_Muon1_MCtruth_Pz);
   fChain->SetBranchAddress("Muon1_MCtruth_eta", Muon1_MCtruth_eta, &b_Muon1_MCtruth_eta);
   fChain->SetBranchAddress("Muon1_MCtruth_phi", Muon1_MCtruth_phi, &b_Muon1_MCtruth_phi);
   fChain->SetBranchAddress("Muon1_MCtruth_charge", Muon1_MCtruth_charge, &b_Muon1_MCtruth_charge);
   fChain->SetBranchAddress("Muon1_MCtruth_mother", Muon1_MCtruth_mother, &b_Muon1_MCtruth_mother);
   fChain->SetBranchAddress("Muon1_MCtruth_pdgId", Muon1_MCtruth_pdgId, &b_Muon1_MCtruth_pdgId);
   fChain->SetBranchAddress("Muon1_timeInsideOut", Muon1_timeInsideOut, &b_Muon1_timeInsideOut);
   fChain->SetBranchAddress("Muon1_timeOutsideIn", Muon1_timeOutsideIn, &b_Muon1_timeOutsideIn);
   fChain->SetBranchAddress("Muon1_combtime_nDof", Muon1_combtime_nDof, &b_Muon1_combtime_nDof);
   fChain->SetBranchAddress("Muon1_combtime_direction", Muon1_combtime_direction, &b_Muon1_combtime_direction);
   fChain->SetBranchAddress("Muon1_combtime_inverseBeta", Muon1_combtime_inverseBeta, &b_Muon1_combtime_inverseBeta);
   fChain->SetBranchAddress("Muon1_combtime_freeInverseBeta", Muon1_combtime_freeInverseBeta, &b_Muon1_combtime_freeInverseBeta);
   fChain->SetBranchAddress("Muon1_combtime_timeAtIpInOut", Muon1_combtime_timeAtIpInOut, &b_Muon1_combtime_timeAtIpInOut);
   fChain->SetBranchAddress("Muon1_combtime_timeAtIpOutIn", Muon1_combtime_timeAtIpOutIn, &b_Muon1_combtime_timeAtIpOutIn);
   fChain->SetBranchAddress("Muon1_combtime_inverseBetaErr", Muon1_combtime_inverseBetaErr, &b_Muon1_combtime_inverseBetaErr);
   fChain->SetBranchAddress("Muon1_combtime_freeInverseBetaErr", Muon1_combtime_freeInverseBetaErr, &b_Muon1_combtime_freeInverseBetaErr);
   fChain->SetBranchAddress("Muon1_combtime_timeAtIpOutInErr", Muon1_combtime_timeAtIpOutInErr, &b_Muon1_combtime_timeAtIpOutInErr);
   fChain->SetBranchAddress("Muon1_combtime_timeAtIpInOutErr", Muon1_combtime_timeAtIpInOutErr, &b_Muon1_combtime_timeAtIpInOutErr);
   fChain->SetBranchAddress("Muon1_dttime_nDof", Muon1_dttime_nDof, &b_Muon1_dttime_nDof);
   fChain->SetBranchAddress("Muon1_dttime_direction", Muon1_dttime_direction, &b_Muon1_dttime_direction);
   fChain->SetBranchAddress("Muon1_dttime_inverseBeta", Muon1_dttime_inverseBeta, &b_Muon1_dttime_inverseBeta);
   fChain->SetBranchAddress("Muon1_dttime_freeInverseBeta", Muon1_dttime_freeInverseBeta, &b_Muon1_dttime_freeInverseBeta);
   fChain->SetBranchAddress("Muon1_dttime_timeAtIpInOut", Muon1_dttime_timeAtIpInOut, &b_Muon1_dttime_timeAtIpInOut);
   fChain->SetBranchAddress("Muon1_dttime_timeAtIpOutIn", Muon1_dttime_timeAtIpOutIn, &b_Muon1_dttime_timeAtIpOutIn);
   fChain->SetBranchAddress("Muon1_dttime_inverseBetaErr", Muon1_dttime_inverseBetaErr, &b_Muon1_dttime_inverseBetaErr);
   fChain->SetBranchAddress("Muon1_dttime_freeInverseBetaErr", Muon1_dttime_freeInverseBetaErr, &b_Muon1_dttime_freeInverseBetaErr);
   fChain->SetBranchAddress("Muon1_dttime_timeAtIpOutInErr", Muon1_dttime_timeAtIpOutInErr, &b_Muon1_dttime_timeAtIpOutInErr);
   fChain->SetBranchAddress("Muon1_dttime_timeAtIpInOutErr", Muon1_dttime_timeAtIpInOutErr, &b_Muon1_dttime_timeAtIpInOutErr);
   fChain->SetBranchAddress("Muon1_csctime_nDof", Muon1_csctime_nDof, &b_Muon1_csctime_nDof);
   fChain->SetBranchAddress("Muon1_csctime_direction", Muon1_csctime_direction, &b_Muon1_csctime_direction);
   fChain->SetBranchAddress("Muon1_csctime_inverseBeta", Muon1_csctime_inverseBeta, &b_Muon1_csctime_inverseBeta);
   fChain->SetBranchAddress("Muon1_csctime_freeInverseBeta", Muon1_csctime_freeInverseBeta, &b_Muon1_csctime_freeInverseBeta);
   fChain->SetBranchAddress("Muon1_csctime_timeAtIpInOut", Muon1_csctime_timeAtIpInOut, &b_Muon1_csctime_timeAtIpInOut);
   fChain->SetBranchAddress("Muon1_csctime_timeAtIpOutIn", Muon1_csctime_timeAtIpOutIn, &b_Muon1_csctime_timeAtIpOutIn);
   fChain->SetBranchAddress("Muon1_csctime_inverseBetaErr", Muon1_csctime_inverseBetaErr, &b_Muon1_csctime_inverseBetaErr);
   fChain->SetBranchAddress("Muon1_csctime_freeInverseBetaErr", Muon1_csctime_freeInverseBetaErr, &b_Muon1_csctime_freeInverseBetaErr);
   fChain->SetBranchAddress("Muon1_csctime_timeAtIpOutInErr", Muon1_csctime_timeAtIpOutInErr, &b_Muon1_csctime_timeAtIpOutInErr);
   fChain->SetBranchAddress("Muon1_csctime_timeAtIpInOutErr", Muon1_csctime_timeAtIpInOutErr, &b_Muon1_csctime_timeAtIpInOutErr);
   fChain->SetBranchAddress("Muon1_caloCompatibility", Muon1_caloCompatibility, &b_Muon1_caloCompatibility);
   fChain->SetBranchAddress("Muon1_segCompatibility", Muon1_segCompatibility, &b_Muon1_segCompatibility);
   fChain->SetBranchAddress("Muon1_CaloEM", Muon1_CaloEM, &b_Muon1_CaloEM);
   fChain->SetBranchAddress("Muon1_CaloHad", Muon1_CaloHad, &b_Muon1_CaloHad);
   fChain->SetBranchAddress("Muon1_ID", Muon1_ID, &b_Muon1_ID);
   */
   fChain->SetBranchAddress("Muon2_muonType", Muon2_muonType, &b_Muon2_muonType);
   /*fChain->SetBranchAddress("Muon2_nTrig", Muon2_nTrig, &b_Muon2_nTrig);
   fChain->SetBranchAddress("Muon2_triggerObjectType", Muon2_triggerObjectType, &b_Muon2_triggerObjectType);
   fChain->SetBranchAddress("Muon2_filterName", Muon2_filterName, &b_Muon2_filterName);
   */

   fChain->SetBranchAddress("Muon2_phi", Muon2_phi, &b_Muon2_phi);
   fChain->SetBranchAddress("Muon2_eta", Muon2_eta, &b_Muon2_eta);
   fChain->SetBranchAddress("Muon2_pT", Muon2_pT, &b_Muon2_pT);
   fChain->SetBranchAddress("Muon2_Px", Muon2_Px, &b_Muon2_Px);
   fChain->SetBranchAddress("Muon2_Py", Muon2_Py, &b_Muon2_Py);
   fChain->SetBranchAddress("Muon2_Pz", Muon2_Pz, &b_Muon2_Pz);
/*   fChain->SetBranchAddress("Muon2_CosmicTimeTight", Muon2_CosmicTimeTight, &b_Muon2_CosmicTimeTight);
   fChain->SetBranchAddress("Muon2_isBackToBack2LegCosmic", Muon2_isBackToBack2LegCosmic, &b_Muon2_isBackToBack2LegCosmic);
   fChain->SetBranchAddress("Muon2_CosmicCompat", Muon2_CosmicCompat, &b_Muon2_CosmicCompat);
   fChain->SetBranchAddress("Muon2_sumtrkpt", Muon2_sumtrkpt, &b_Muon2_sumtrkpt);
   fChain->SetBranchAddress("Muon2_trkiso", Muon2_trkiso, &b_Muon2_trkiso);
   fChain->SetBranchAddress("Muon2_hcaliso", Muon2_hcaliso, &b_Muon2_hcaliso);
   fChain->SetBranchAddress("Muon2_ecaliso", Muon2_ecaliso, &b_Muon2_ecaliso);
   fChain->SetBranchAddress("Muon2_trkisoR05", Muon2_trkisoR05, &b_Muon2_trkisoR05);
   fChain->SetBranchAddress("Muon2_hcalisoR05", Muon2_hcalisoR05, &b_Muon2_hcalisoR05);
   fChain->SetBranchAddress("Muon2_ecalisoR05", Muon2_ecalisoR05, &b_Muon2_ecalisoR05);
*/   
   //fChain->SetBranchAddress("Muon2_PfChargedHadronIsoR05", Muon2_PfChargedHadronIsoR05, &b_Muon2_PfChargedHadronIsoR05);
   //fChain->SetBranchAddress("Muon2_PfNeutralHadronIsoR05", Muon2_PfNeutralHadronIsoR05, &b_Muon2_PfNeutralHadronIsoR05);
   //fChain->SetBranchAddress("Muon2_PfGammaIsoR05", Muon2_PfGammaIsoR05, &b_Muon2_PfGammaIsoR05);
   fChain->SetBranchAddress("Muon2_PfChargedHadronIsoR03", Muon2_PfChargedHadronIsoR03, &b_Muon2_PfChargedHadronIsoR03);
   fChain->SetBranchAddress("Muon2_PfNeutralHadronIsoR03", Muon2_PfNeutralHadronIsoR03, &b_Muon2_PfNeutralHadronIsoR03);
   fChain->SetBranchAddress("Muon2_PfGammaIsoR03", Muon2_PfGammaIsoR03, &b_Muon2_PfGammaIsoR03);
   //fChain->SetBranchAddress("Muon2_charge", Muon2_charge, &b_Muon2_charge);
   fChain->SetBranchAddress("Muon2_nChambers", Muon2_nChambers, &b_Muon2_nChambers);
   fChain->SetBranchAddress("Muon2_nMatches", Muon2_nMatches, &b_Muon2_nMatches);
   fChain->SetBranchAddress("Muon2_nTrackerLayers", Muon2_nTrackerLayers, &b_Muon2_nTrackerLayers);
   fChain->SetBranchAddress("Muon2_stationMask", Muon2_stationMask, &b_Muon2_stationMask);
   fChain->SetBranchAddress("Muon2_nSegments", Muon2_nSegments, &b_Muon2_nSegments);
   fChain->SetBranchAddress("Muon2_chi2dof", Muon2_chi2dof, &b_Muon2_chi2dof);
   fChain->SetBranchAddress("Muon2_nhits", Muon2_nhits, &b_Muon2_nhits);
   fChain->SetBranchAddress("Muon2_trackerHits", Muon2_trackerHits, &b_Muon2_trackerHits);
   fChain->SetBranchAddress("Muon2_pixelHits", Muon2_pixelHits, &b_Muon2_pixelHits);
   fChain->SetBranchAddress("Muon2_muonHits", Muon2_muonHits, &b_Muon2_muonHits);
   /*fChain->SetBranchAddress("Muon2_qoverp", Muon2_qoverp, &b_Muon2_qoverp);
   fChain->SetBranchAddress("Muon2_theta", Muon2_theta, &b_Muon2_theta);
   fChain->SetBranchAddress("Muon2_lambda", Muon2_lambda, &b_Muon2_lambda);
   fChain->SetBranchAddress("Muon2_dxy", Muon2_dxy, &b_Muon2_dxy);
   */
   fChain->SetBranchAddress("Muon2_dxyBS", Muon2_dxyBS, &b_Muon2_dxyBS);
   /*fChain->SetBranchAddress("Muon2_dszBS", Muon2_dszBS, &b_Muon2_dszBS);
   fChain->SetBranchAddress("Muon2_dzBS", Muon2_dzBS, &b_Muon2_dzBS);
   fChain->SetBranchAddress("Muon2_dxyVTX", Muon2_dxyVTX, &b_Muon2_dxyVTX);
   fChain->SetBranchAddress("Muon2_dszVTX", Muon2_dszVTX, &b_Muon2_dszVTX);
   fChain->SetBranchAddress("Muon2_dzVTX", Muon2_dzVTX, &b_Muon2_dzVTX);
   fChain->SetBranchAddress("Muon2_dxyDiMuVTX", Muon2_dxyDiMuVTX, &b_Muon2_dxyDiMuVTX);
   fChain->SetBranchAddress("Muon2_dszDiMuVTX", Muon2_dszDiMuVTX, &b_Muon2_dszDiMuVTX);
   fChain->SetBranchAddress("Muon2_dzDiMuVTX", Muon2_dzDiMuVTX, &b_Muon2_dzDiMuVTX);
   fChain->SetBranchAddress("Muon2_d0", Muon2_d0, &b_Muon2_d0);
   fChain->SetBranchAddress("Muon2_dsz", Muon2_dsz, &b_Muon2_dsz);
   fChain->SetBranchAddress("Muon2_dz", Muon2_dz, &b_Muon2_dz);
   fChain->SetBranchAddress("Muon2_vx", Muon2_vx, &b_Muon2_vx);
   fChain->SetBranchAddress("Muon2_vy", Muon2_vy, &b_Muon2_vy);
   fChain->SetBranchAddress("Muon2_vz", Muon2_vz, &b_Muon2_vz);
   fChain->SetBranchAddress("Muon2_MCtruth_pT", Muon2_MCtruth_pT, &b_Muon2_MCtruth_pT);
   fChain->SetBranchAddress("Muon2_MCtruth_Px", Muon2_MCtruth_Px, &b_Muon2_MCtruth_Px);
   fChain->SetBranchAddress("Muon2_MCtruth_Py", Muon2_MCtruth_Py, &b_Muon2_MCtruth_Py);
   fChain->SetBranchAddress("Muon2_MCtruth_Pz", Muon2_MCtruth_Pz, &b_Muon2_MCtruth_Pz);
   fChain->SetBranchAddress("Muon2_MCtruth_eta", Muon2_MCtruth_eta, &b_Muon2_MCtruth_eta);
   fChain->SetBranchAddress("Muon2_MCtruth_phi", Muon2_MCtruth_phi, &b_Muon2_MCtruth_phi);
   fChain->SetBranchAddress("Muon2_MCtruth_charge", Muon2_MCtruth_charge, &b_Muon2_MCtruth_charge);
   fChain->SetBranchAddress("Muon2_MCtruth_mother", Muon2_MCtruth_mother, &b_Muon2_MCtruth_mother);
   fChain->SetBranchAddress("Muon2_MCtruth_pdgId", Muon2_MCtruth_pdgId, &b_Muon2_MCtruth_pdgId);
   fChain->SetBranchAddress("Muon2_timeInsideOut", Muon2_timeInsideOut, &b_Muon2_timeInsideOut);
   fChain->SetBranchAddress("Muon2_timeOutsideIn", Muon2_timeOutsideIn, &b_Muon2_timeOutsideIn);
   fChain->SetBranchAddress("Muon2_combtime_nDof", Muon2_combtime_nDof, &b_Muon2_combtime_nDof);
   fChain->SetBranchAddress("Muon2_combtime_direction", Muon2_combtime_direction, &b_Muon2_combtime_direction);
   fChain->SetBranchAddress("Muon2_combtime_inverseBeta", Muon2_combtime_inverseBeta, &b_Muon2_combtime_inverseBeta);
   fChain->SetBranchAddress("Muon2_combtime_freeInverseBeta", Muon2_combtime_freeInverseBeta, &b_Muon2_combtime_freeInverseBeta);
   fChain->SetBranchAddress("Muon2_combtime_timeAtIpInOut", Muon2_combtime_timeAtIpInOut, &b_Muon2_combtime_timeAtIpInOut);
   fChain->SetBranchAddress("Muon2_combtime_timeAtIpOutIn", Muon2_combtime_timeAtIpOutIn, &b_Muon2_combtime_timeAtIpOutIn);
   fChain->SetBranchAddress("Muon2_combtime_inverseBetaErr", Muon2_combtime_inverseBetaErr, &b_Muon2_combtime_inverseBetaErr);
   fChain->SetBranchAddress("Muon2_combtime_freeInverseBetaErr", Muon2_combtime_freeInverseBetaErr, &b_Muon2_combtime_freeInverseBetaErr);
   fChain->SetBranchAddress("Muon2_combtime_timeAtIpOutInErr", Muon2_combtime_timeAtIpOutInErr, &b_Muon2_combtime_timeAtIpOutInErr);
   fChain->SetBranchAddress("Muon2_combtime_timeAtIpInOutErr", Muon2_combtime_timeAtIpInOutErr, &b_Muon2_combtime_timeAtIpInOutErr);
   fChain->SetBranchAddress("Muon2_dttime_nDof", Muon2_dttime_nDof, &b_Muon2_dttime_nDof);
   fChain->SetBranchAddress("Muon2_dttime_direction", Muon2_dttime_direction, &b_Muon2_dttime_direction);
   fChain->SetBranchAddress("Muon2_dttime_inverseBeta", Muon2_dttime_inverseBeta, &b_Muon2_dttime_inverseBeta);
   fChain->SetBranchAddress("Muon2_dttime_freeInverseBeta", Muon2_dttime_freeInverseBeta, &b_Muon2_dttime_freeInverseBeta);
   fChain->SetBranchAddress("Muon2_dttime_timeAtIpInOut", Muon2_dttime_timeAtIpInOut, &b_Muon2_dttime_timeAtIpInOut);
   fChain->SetBranchAddress("Muon2_dttime_timeAtIpOutIn", Muon2_dttime_timeAtIpOutIn, &b_Muon2_dttime_timeAtIpOutIn);
   fChain->SetBranchAddress("Muon2_dttime_inverseBetaErr", Muon2_dttime_inverseBetaErr, &b_Muon2_dttime_inverseBetaErr);
   fChain->SetBranchAddress("Muon2_dttime_freeInverseBetaErr", Muon2_dttime_freeInverseBetaErr, &b_Muon2_dttime_freeInverseBetaErr);
   fChain->SetBranchAddress("Muon2_dttime_timeAtIpOutInErr", Muon2_dttime_timeAtIpOutInErr, &b_Muon2_dttime_timeAtIpOutInErr);
   fChain->SetBranchAddress("Muon2_dttime_timeAtIpInOutErr", Muon2_dttime_timeAtIpInOutErr, &b_Muon2_dttime_timeAtIpInOutErr);
   fChain->SetBranchAddress("Muon2_csctime_nDof", Muon2_csctime_nDof, &b_Muon2_csctime_nDof);
   fChain->SetBranchAddress("Muon2_csctime_direction", Muon2_csctime_direction, &b_Muon2_csctime_direction);
   fChain->SetBranchAddress("Muon2_csctime_inverseBeta", Muon2_csctime_inverseBeta, &b_Muon2_csctime_inverseBeta);
   fChain->SetBranchAddress("Muon2_csctime_freeInverseBeta", Muon2_csctime_freeInverseBeta, &b_Muon2_csctime_freeInverseBeta);
   fChain->SetBranchAddress("Muon2_csctime_timeAtIpInOut", Muon2_csctime_timeAtIpInOut, &b_Muon2_csctime_timeAtIpInOut);
   fChain->SetBranchAddress("Muon2_csctime_timeAtIpOutIn", Muon2_csctime_timeAtIpOutIn, &b_Muon2_csctime_timeAtIpOutIn);
   fChain->SetBranchAddress("Muon2_csctime_inverseBetaErr", Muon2_csctime_inverseBetaErr, &b_Muon2_csctime_inverseBetaErr);
   fChain->SetBranchAddress("Muon2_csctime_freeInverseBetaErr", Muon2_csctime_freeInverseBetaErr, &b_Muon2_csctime_freeInverseBetaErr);
   fChain->SetBranchAddress("Muon2_csctime_timeAtIpOutInErr", Muon2_csctime_timeAtIpOutInErr, &b_Muon2_csctime_timeAtIpOutInErr);
   fChain->SetBranchAddress("Muon2_csctime_timeAtIpInOutErr", Muon2_csctime_timeAtIpInOutErr, &b_Muon2_csctime_timeAtIpInOutErr);
   fChain->SetBranchAddress("Muon2_caloCompatibility", Muon2_caloCompatibility, &b_Muon2_caloCompatibility);
   fChain->SetBranchAddress("Muon2_segCompatibility", Muon2_segCompatibility, &b_Muon2_segCompatibility);
   fChain->SetBranchAddress("Muon2_CaloEM", Muon2_CaloEM, &b_Muon2_CaloEM);
   fChain->SetBranchAddress("Muon2_CaloHad", Muon2_CaloHad, &b_Muon2_CaloHad);
   fChain->SetBranchAddress("Muon2_ID", Muon2_ID, &b_Muon2_ID);
*/
   fChain->SetBranchAddress("GENnPair", &GENnPair, &b_GENnPair);
   fChain->SetBranchAddress("GENInvMass", GENInvMass, &b_GENInvMass);
   fChain->SetBranchAddress("GENRapidity", GENRapidity, &b_GENRapidity);

//   fChain->SetBranchAddress("GENAngle", &GENAngle, &b_GENAngle);
   fChain->SetBranchAddress("GENisOppSign", &GENisOppSign, &b_GENisOppSign);
   fChain->SetBranchAddress("GENMuon1_phi", &GENMuon1_phi, &b_GENMuon1_phi);
   fChain->SetBranchAddress("GENMuon1_eta", &GENMuon1_eta, &b_GENMuon1_eta);
   fChain->SetBranchAddress("GENMuon1_pT", &GENMuon1_pT, &b_GENMuon1_pT);
   fChain->SetBranchAddress("GENMuon1_Px", &GENMuon1_Px, &b_GENMuon1_Px);
   fChain->SetBranchAddress("GENMuon1_Py", &GENMuon1_Py, &b_GENMuon1_Py);
   fChain->SetBranchAddress("GENMuon1_Pz", &GENMuon1_Pz, &b_GENMuon1_Pz);

   fChain->SetBranchAddress("GENMuon1_mother", GENMuon1_mother, &b_GENMuon1_mother);
   fChain->SetBranchAddress("GENMuon1_status", GENMuon1_status, &b_GENMuon1_status);

//   fChain->SetBranchAddress("GENMuon1_charge", &GENMuon1_charge, &b_GENMuon1_charge);
   fChain->SetBranchAddress("GENMuon2_phi", &GENMuon2_phi, &b_GENMuon2_phi);
   fChain->SetBranchAddress("GENMuon2_eta", &GENMuon2_eta, &b_GENMuon2_eta);
   fChain->SetBranchAddress("GENMuon2_pT", &GENMuon2_pT, &b_GENMuon2_pT);
   fChain->SetBranchAddress("GENMuon2_Px", &GENMuon2_Px, &b_GENMuon2_Px);
   fChain->SetBranchAddress("GENMuon2_Py", &GENMuon2_Py, &b_GENMuon2_Py);
   fChain->SetBranchAddress("GENMuon2_Pz", &GENMuon2_Pz, &b_GENMuon2_Pz);

   fChain->SetBranchAddress("GENMuon2_mother", GENMuon2_mother, &b_GENMuon2_mother);
   fChain->SetBranchAddress("GENMuon2_status", GENMuon2_status, &b_GENMuon2_status);
/*
   fChain->SetBranchAddress("GENMuon2_charge", &GENMuon2_charge, &b_GENMuon2_charge);
*/
   // get dataset name
   dataset = fChain->GetTitle();

   //fChain->SetBranchStatus("Elec*",0);
   //fChain->SetBranchStatus("Nb*",0);
   //fChain->SetBranchStatus("JET*",0);
   //fChain->SetBranchStatus("GEN*",0);
   //fChain->SetBranchStatus("*elec*",0);
   //fChain->SetBranchStatus("*time*",0);

   //fChain->SetCacheSize(300000);

   fChain->AddBranchToCache("Muon1_*");
   fChain->AddBranchToCache("Muon2_*");
   fChain->AddBranchToCache("runNum");
   fChain->AddBranchToCache("evtNum");
   fChain->AddBranchToCache("ntupleWeight");
   fChain->AddBranchToCache("HLT*");
   fChain->AddBranchToCache("CosAngle");
   fChain->AddBranchToCache("vtxTrkProb");
   fChain->AddBranchToCache("InvMass");
   fChain->AddBranchToCache("GEN*");

}

Bool_t EventSelector_MCTruthEff::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef EventSelector_MCTruthEff_cxx
