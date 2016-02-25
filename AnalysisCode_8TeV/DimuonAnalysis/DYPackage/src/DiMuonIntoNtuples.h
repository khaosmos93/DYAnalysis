#ifndef DiMuonIntoNtuples_H
#define DiMuonIntoNtuples_H

/*
1. muon
- 3 vector momentum
- tag and probe info (for example, the muon passes the requirement of
"tag", tag = 1. If not, tag = 0. Same to the probe)

2. Jet
- kinematic variables (pt, 3 v momentum, eta, phi...) for each jet

3. global event information
- luminosity, run number, event number

4. Primary vertex (PV) information
- number of primary vertices
- PVz (z coordinate), # of tracks for the PV

5. track-matching information for the muon
- track z, phi, eta, pt,
- # of hits in trackers for each track

6. trigger efficiency
- when it's available

7. electron track information
- to know the electron track is sharing with muon track-matching (it can
be a kind of background)

8. for cosmic ray bg
- timing information ...

9. various topological variables (may be useful for the physics
background suppression such as ttbar, Z->tautau)
- H_T, Spericity, Centrality, Aplanarity ...
- I have a source code for these variables and I think we can implement
it to your code easily (or as another class).
*/
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/View.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

//pileup
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include <TTree.h>

//
// class decleration
//

using namespace std;
namespace reco { class CandCommonVertexFitterBase; class VertexCompositeCandidate; class CandCommonVertexFitter; }

class DiMuonIntoNtuples : public edm::EDAnalyzer {

   public:
      explicit DiMuonIntoNtuples(const edm::ParameterSet&);
      ~DiMuonIntoNtuples();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      virtual void beginRun(const edm::Run &, const edm::EventSetup & );

      int motherId(const reco::GenParticle& par) const;
      int motherId(const reco::GenParticleRef par) const;

      virtual void fillPrimaryVertex(const edm::Event &iEvent);  // fill primary vertex information
      virtual void fillMET(const edm::Event &iEvent);            // fill MET information
      virtual void fillJet(const edm::Event &iEvent);            // fill jet and b-tagging information
      double  passesVertexCuts(const edm::EventSetup&, const pat::Muon&, const pat::Electron&) const;
      virtual void hltReport(const edm::Event &iEvent);          // fill list of triggers fired in an event
      
      template <class T> float angleBetween(const T & lhs, const T & rhs) const;
      bool muonCountFilter(const edm::Handle<edm::View<pat::Muon> >& muonHandle);   // filter to check if there are at least 2 muons in an event

   private:
      int theDebugLevel_;                   // 0 no prints, 1 some, 2 lots
      std::string processName_;

      HLTConfigProvider hltConfig_;

      edm::InputTag theElectronLabel_;
      edm::InputTag theMuonLabel_;
      edm::InputTag theMETLabel_;
      edm::InputTag theJetLabel_;
      edm::InputTag theBeamSpot_;
      edm::InputTag theTrackLabel_;
      edm::InputTag inputMuonCosmicCompatibilityValueMap_;
      edm::InputTag metLabel_;

      double theBDiscriminant_;
      double theIntegratedLumi_;
      double theBDiscriminant_alg1_;
      double theBDiscriminant_alg2_;
      double theBDiscriminant_alg3_;
      
      //flags to select which info to store in the ntuple
      bool theCheckAtLeast2MuFlag_;            // Yes or No to check if there are at least 2 muons in an events
      bool theStorePriVtxFlag_;                // Yes or No to store primary vertex
      bool theStorePriVtx2MuFlag_;             // Yes or No to store primary vertex for dimuon candidates
      bool theStoreJetMETFlag_;                // Yes or No to store Jet and MET 
      bool theStoreHLTReportFlag_;             // Yes or No to store HLT reuslts (list of triggers fired)
      //bool theStoreMCtruthFlag_;               // Yes or No to store MC truth information
      bool theStoreElectronFlag_;
      bool theStoreGENFlag_;
      bool isMC_;  //turn gen on and off
      bool isAOD_; //inserted temporarily to resolve the issues with event content

      std::vector<std::string > MuonHLT_;
      std::vector<std::string > trigModuleNames_;
      std::vector<std::string > trigModuleNames_preFil_;

      TTree *DiMuonTree_;

      int nEvt_;
      static const int MPSIZE = 1000;
      int runNum_;
      int evtNum_;
      int lumiBlock_;
      int nPair_;
      int GENnPair_;
      int Njets_;
      int nEMuPair_;
      int Nmuons_;

      // PV
      int nVertices_;
      int PVtrackSize_;
      double PVchi2_;
      int PVndof_;
      double PVnormalizedChi2_;
      double PVx_;
      double PVy_;
      double PVz_;
      double PVprob_;

      // Pile-up Reweight
      edm::LumiReWeighting LumiWeights_;
      reweight::PoissonMeanShifter PShiftUp_;
      reweight::PoissonMeanShifter PShiftDown_;          
      
      std::vector<double> PileUpRD_;
      std::vector<double> PileUpMC_;

      //pileup
      double PUweight_;
      unsigned int nPileUp_;
      unsigned int nPileUpIn_;
      double pileUpReweightIn_;
      double pileUpReweightPlusIn_;
      double pileUpReweightMinusIn_;
      double pileUpReweight_;
      double pileUpReweightPlus_;
      double pileUpReweightMinus_;

      // for trigger matching
      int HLT_ntrig_;
      int HLT_trigType[MPSIZE];
      bool HLT_trigFired[MPSIZE];
      std::vector<std::string> HLT_trigName_;
      double HLT_trigPt[MPSIZE];
      double HLT_trigEta[MPSIZE];
      double HLT_trigPhi[MPSIZE];
      static const int nTrigName_ = 6;
      //int listUnavail_[nTrigName_];

      // Jet
      double JETbDiscriminant[MPSIZE];
      double JETcharge[MPSIZE];
      int JETflavour[MPSIZE];
      int JETntracks[MPSIZE];
      double JETpt[MPSIZE];
      double JETeta[MPSIZE];
      double JETphi[MPSIZE];
      // b-tagging
      double JETbDiscriminant_alg1[MPSIZE];
      double JETbDiscriminant_alg2[MPSIZE];
      double JETbDiscriminant_alg3[MPSIZE];

      // MET
      double MET_;

      // PAT Electron
      double Electron_E[MPSIZE];
      double Electron_pT[MPSIZE];
      double Electron_Px[MPSIZE];
      double Electron_Py[MPSIZE];
      double Electron_Pz[MPSIZE];
      double Electron_eta[MPSIZE];
      double Electron_SCeta[MPSIZE];
      double Electron_phi[MPSIZE];
      int Electron_charge[MPSIZE];
      double Electron_trkiso[MPSIZE];
      double Electron_hcaliso[MPSIZE];
      double Electron_ecaliso[MPSIZE];
      double Electron_isEcalDriven[MPSIZE];
      double Electron_dxyBS[MPSIZE];
      int Electron_trackerHits [MPSIZE];
      double Electron_SCSigmaIEtaIEta[MPSIZE];
      double Electron_dPhiSCTrackAtVtx[MPSIZE];
      double Electron_dEtaSCTrackAtVtx[MPSIZE];
      double Electron_HcalOverEcal[MPSIZE];
      double Electron_Brem[MPSIZE];
      double Electron_SCClusterOverP[MPSIZE];
      double EMu_CosAngle [MPSIZE];
      double Electron_passVtxCuts[MPSIZE];


      // Pat Muon
      double InvMass[MPSIZE];
      double Rapidity[MPSIZE];
      double Pt[MPSIZE];
      double Angle[MPSIZE];
      double CosAngle[MPSIZE];
      double DeltaR[MPSIZE];
      double Dalpha[MPSIZE];
      bool isOppSign[MPSIZE];

      // PF isolation
      double Muon1_PfChargedHadronIsoR03[MPSIZE];
      double Muon1_PfNeutralHadronIsoR03[MPSIZE];
      double Muon1_PfGammaIsoR03[MPSIZE];
      double Muon2_PfChargedHadronIsoR03[MPSIZE];
      double Muon2_PfNeutralHadronIsoR03[MPSIZE];
      double Muon2_PfGammaIsoR03[MPSIZE];

      int vtxNdof[MPSIZE];
      double vtxProb[MPSIZE];
      double vtxChi2[MPSIZE];
      double vtxPositionX[MPSIZE];
      double vtxPositionY[MPSIZE];
      double vtxPositionZ[MPSIZE];
      double vtxXerror[MPSIZE];
      double vtxYerror[MPSIZE];
      double vtxZerror[MPSIZE];
      double vtx_newPx[MPSIZE];
      double vtx_newPy[MPSIZE];
      double vtx_newPz[MPSIZE];
      double vtx_newMass[MPSIZE];
      double vtx_newMassErr[MPSIZE];

      int vtxTrkNdof[MPSIZE];
      double vtxTrkProb[MPSIZE];
      double vtxTrkChi2[MPSIZE];
      double vtxTrkPositionX[MPSIZE];
      double vtxTrkPositionY[MPSIZE];
      double vtxTrkPositionZ[MPSIZE];
      double vtxTrkXerror[MPSIZE];
      double vtxTrkYerror[MPSIZE];
      double vtxTrkZerror[MPSIZE];
      double vtxTrk_newPx[MPSIZE];
      double vtxTrk_newPy[MPSIZE];
      double vtxTrk_newPz[MPSIZE];
      double vtxTrk_newMass[MPSIZE];
      double vtxTrk_newMassErr[MPSIZE];

      int Muon1_muonType[MPSIZE];
      int Muon1_nTrig[MPSIZE];
      int Muon1_triggerObjectType[MPSIZE];
      int Muon1_filterName[MPSIZE];
      double Muon1_phi[MPSIZE];
      double Muon1_eta[MPSIZE];
      double Muon1_pT[MPSIZE];
      double Muon1_Px[MPSIZE];
      double Muon1_Py[MPSIZE];
      double Muon1_Pz[MPSIZE];
      double Muon1_sumtrkpt[MPSIZE];
      double Muon1_trkiso[MPSIZE];
      double Muon1_hcaliso[MPSIZE];
      double Muon1_ecaliso[MPSIZE];
      int Muon1_charge[MPSIZE];
      int Muon1_nChambers[MPSIZE];
      int Muon1_nTrackerLayers[MPSIZE];
      int Muon1_nMatches[MPSIZE];
      int Muon1_stationMask[MPSIZE];
      int Muon1_nSegments[MPSIZE];
      double Muon1_chi2dof[MPSIZE];
      int Muon1_nhits[MPSIZE];
      int Muon1_trackerHits[MPSIZE];
      int Muon1_pixelHits[MPSIZE];
      int Muon1_muonHits[MPSIZE];
      double Muon1_qoverp[MPSIZE];
      double Muon1_theta[MPSIZE];
      double Muon1_lambda[MPSIZE];
      double Muon1_dxy[MPSIZE];
      double Muon1_d0[MPSIZE];
      double Muon1_dsz[MPSIZE];
      double Muon1_dz[MPSIZE];
      double Muon1_dxyBS[MPSIZE];
      double Muon1_dzBS[MPSIZE];
      double Muon1_dszBS[MPSIZE];
      double Muon1_dxyVTX[MPSIZE];
      double Muon1_dzVTX[MPSIZE];
      double Muon1_dszVTX[MPSIZE];
      double Muon1_dxyDiMuVTX[MPSIZE];
      double Muon1_dzDiMuVTX[MPSIZE];
      double Muon1_dszDiMuVTX[MPSIZE];
      double Muon1_dxyDiMuVTXTrk[MPSIZE];
      double Muon1_dzDiMuVTXTrk[MPSIZE];
      double Muon1_dszDiMuVTXTrk[MPSIZE];
      double Muon1_vx[MPSIZE];
      double Muon1_vy[MPSIZE];
      double Muon1_vz[MPSIZE];
      double Muon1_MCtruth_pT[MPSIZE];
      double Muon1_MCtruth_Px[MPSIZE];
      double Muon1_MCtruth_Py[MPSIZE];
      double Muon1_MCtruth_Pz[MPSIZE];
      double Muon1_MCtruth_eta[MPSIZE];
      double Muon1_MCtruth_phi[MPSIZE];
      int Muon1_MCtruth_charge[MPSIZE];
      int Muon1_MCtruth_mother[MPSIZE];
      int Muon1_MCtruth_pdgId[MPSIZE];
      double Muon1_timeInsideOut[MPSIZE];
      double Muon1_timeOutsideIn[MPSIZE];
      double Muon1_caloCompatibility[MPSIZE];
      double Muon1_segCompatibility[MPSIZE];
      double Muon1_CaloEM[MPSIZE];
      double Muon1_CaloHad[MPSIZE];
      double Muon1_ID[MPSIZE];

      int Muon2_muonType[MPSIZE];
      int Muon2_nTrig[MPSIZE];
      int Muon2_triggerObjectType[MPSIZE];
      int Muon2_filterName[MPSIZE];
      double Muon2_phi[MPSIZE];
      double Muon2_eta[MPSIZE];
      double Muon2_pT[MPSIZE];
      double Muon2_Px[MPSIZE];
      double Muon2_Py[MPSIZE];
      double Muon2_Pz[MPSIZE];
      double Muon2_sumtrkpt[MPSIZE];
      double Muon2_trkiso[MPSIZE];
      double Muon2_hcaliso[MPSIZE];
      double Muon2_ecaliso[MPSIZE];
      int Muon2_charge[MPSIZE];
      int Muon2_nChambers[MPSIZE];
      int Muon2_nTrackerLayers[MPSIZE];
      int Muon2_nMatches[MPSIZE];
      int Muon2_stationMask[MPSIZE];
      int Muon2_nSegments[MPSIZE];
      double Muon2_chi2dof[MPSIZE];
      int Muon2_nhits[MPSIZE];
      int Muon2_trackerHits[MPSIZE];
      int Muon2_pixelHits[MPSIZE];
      int Muon2_muonHits[MPSIZE];
      double Muon2_qoverp[MPSIZE];
      double Muon2_theta[MPSIZE];
      double Muon2_lambda[MPSIZE];
      double Muon2_dxy[MPSIZE];
      double Muon2_d0[MPSIZE];
      double Muon2_dsz[MPSIZE];
      double Muon2_dz[MPSIZE];
      double Muon2_dxyBS[MPSIZE];
      double Muon2_dzBS[MPSIZE];
      double Muon2_dszBS[MPSIZE];
      double Muon2_dxyVTX[MPSIZE];
      double Muon2_dzVTX[MPSIZE];
      double Muon2_dszVTX[MPSIZE];
      double Muon2_dxyDiMuVTX[MPSIZE];
      double Muon2_dzDiMuVTX[MPSIZE];
      double Muon2_dszDiMuVTX[MPSIZE];
      double Muon2_dxyDiMuVTXTrk[MPSIZE];
      double Muon2_dzDiMuVTXTrk[MPSIZE];
      double Muon2_dszDiMuVTXTrk[MPSIZE];
      double Muon2_vx[MPSIZE];
      double Muon2_vy[MPSIZE];
      double Muon2_vz[MPSIZE];
      double Muon2_MCtruth_pT[MPSIZE];
      double Muon2_MCtruth_Px[MPSIZE];
      double Muon2_MCtruth_Py[MPSIZE];
      double Muon2_MCtruth_Pz[MPSIZE];
      double Muon2_MCtruth_eta[MPSIZE];
      double Muon2_MCtruth_phi[MPSIZE];
      int Muon2_MCtruth_charge[MPSIZE];
      int Muon2_MCtruth_mother[MPSIZE];
      int Muon2_MCtruth_pdgId[MPSIZE];
      double Muon2_timeInsideOut[MPSIZE];
      double Muon2_timeOutsideIn[MPSIZE];
      double Muon2_caloCompatibility[MPSIZE];
      double Muon2_segCompatibility[MPSIZE];
      double Muon2_CaloEM[MPSIZE];
      double Muon2_CaloHad[MPSIZE];
      double Muon2_ID[MPSIZE];

      // cosmic
      float Muon1_CosmicTimeTight[MPSIZE];
      float Muon2_CosmicTimeTight[MPSIZE];
      unsigned int Muon1_isBackToBack2LegCosmic[MPSIZE];
      unsigned int Muon2_isBackToBack2LegCosmic[MPSIZE];
      float Muon1_CosmicCompat[MPSIZE];
      float Muon2_CosmicCompat[MPSIZE];

      // Gen Muon
      double GENInvMass[MPSIZE];
      double GENRapidity[MPSIZE];
      double GENPt[MPSIZE];
      double GENAngle[MPSIZE];
      bool GENisOppSign[MPSIZE];

      double GENMuon1_phi[MPSIZE];
      double GENMuon1_eta[MPSIZE];
      double GENMuon1_pT[MPSIZE];
      double GENMuon1_Px[MPSIZE];
      double GENMuon1_Py[MPSIZE];
      double GENMuon1_Pz[MPSIZE];
      int GENMuon1_mother[MPSIZE];
      int GENMuon1_charge[MPSIZE];
      int GENMuon1_status[MPSIZE];

      double GENMuon2_phi[MPSIZE];
      double GENMuon2_eta[MPSIZE];
      double GENMuon2_pT[MPSIZE];
      double GENMuon2_Px[MPSIZE];
      double GENMuon2_Py[MPSIZE];
      double GENMuon2_Pz[MPSIZE];
      int GENMuon2_mother[MPSIZE];
      int GENMuon2_charge[MPSIZE];
      int GENMuon2_status[MPSIZE];
};
#endif
