//-------------------------------------------------
//
//   Class: DiMuonNtupleMaker
//
//   Description: Ntuple maker for Drell-Yan dimuon analysis
//
//
//   Author:
//   H. Yoo           Purdue University
//   A. Svyatkovskiy  Purdue University
//   N. Neumeister    Purdue University
//
//--------------------------------------------------

#include "DiMuonIntoNtuples.h"

// system include files
#include <memory>
#include <iostream>
#include <boost/foreach.hpp>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RegexMatch.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Vertexing.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtraMap.h"
#include "DataFormats/MuonReco/interface/MuonCosmicCompatibility.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/InvariantMassFromVertex.h"

#include "PhysicsTools/RecoUtils/interface/CandCommonVertexFitter.h"

//pileup
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <TTree.h>
#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>

//
// class decleration
//

using namespace std;
using namespace edm;
using namespace pat;


//
DiMuonIntoNtuples::DiMuonIntoNtuples(const edm::ParameterSet& iConfig)
{
  nEvt_ = 0;

  processName_ 			     = iConfig.getUntrackedParameter<string>("processName", "HLT");
  theDebugLevel_                     = iConfig.getUntrackedParameter<int>("DebugLevel", 0);

  theMuonLabel_                      = iConfig.getUntrackedParameter<edm::InputTag>("Muon", edm::InputTag("selectedPatMuonsPFlow"));
  theMETLabel_                       = iConfig.getUntrackedParameter<edm::InputTag>("MET", edm::InputTag("patMETs"));
  theJetLabel_                       = iConfig.getUntrackedParameter<edm::InputTag>("Jet", edm::InputTag("selectedPatJets"));
  theElectronLabel_                  =  iConfig.getUntrackedParameter<edm::InputTag>("Electron", edm::InputTag("selectedPatElectrons"));
  theBeamSpot_                       = iConfig.getUntrackedParameter<edm::InputTag>("BeamSpot", edm::InputTag("offlineBeamSpot"));
  theTrackLabel_                     = iConfig.getUntrackedParameter<edm::InputTag>("Track", edm::InputTag("generalTracks"));

  theBDiscriminant_alg1_             = iConfig.getUntrackedParameter<double>("BDiscriminant_tcheff", 0.7);
  theBDiscriminant_alg2_             = iConfig.getUntrackedParameter<double>("BDiscriminant_tchpur", 0.7);
  theBDiscriminant_alg3_             = iConfig.getUntrackedParameter<double>("BDiscriminant_ssv", 2.05);

  theCheckAtLeast2MuFlag_            =  iConfig.getUntrackedParameter<bool>("CheckAtLeast2MuFlag", false);

  theStorePriVtxFlag_                =  iConfig.getUntrackedParameter<bool>("StorePriVtxFlag", false);
  theStorePriVtx2MuFlag_             =  iConfig.getUntrackedParameter<bool>("StorePriVtx2MuFlag", true);
  theStoreJetMETFlag_                =  iConfig.getUntrackedParameter<bool>("StoreJetMETFlag", true);
  theStoreHLTReportFlag_             =  iConfig.getUntrackedParameter<bool>("StoreHLTReportFlag", true);
  //theStoreMCtruthFlag_             =  iConfig.getParameter<bool>("StoreMCtruthFlag");
  isMC_                              =  iConfig.getUntrackedParameter<bool>("isMC");
  isAOD_                             =  iConfig.getUntrackedParameter<bool>("isAOD", true);
  metLabel_                          =  iConfig.getUntrackedParameter<edm::InputTag>("metLabel");
  if( isMC_ ) {                      
    PileUpRD_                        = iConfig.getParameter< std::vector<double> >("PileUpRD");
    PileUpMC_                        = iConfig.getParameter< std::vector<double> >("PileUpMC");
  }

  theStoreGENFlag_                   =  iConfig.getUntrackedParameter<bool>("StoreGENFlag", false);

  inputMuonCosmicCompatibilityValueMap_ = iConfig.getUntrackedParameter<edm::InputTag>("inputMuonCosmicCompatibilityValueMap", edm::InputTag("cosmicsVeto","","PAT"));
}


DiMuonIntoNtuples::~DiMuonIntoNtuples()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void DiMuonIntoNtuples::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // incremented variables
  nPair_ = 0; 
  GENnPair_ = 0; 
  nEMuPair_ = 0;
  Njets_ = 0;

  nEvt_++;
  runNum_ = iEvent.id().run();
  evtNum_ = iEvent.id().event();
  lumiBlock_ = iEvent.id().luminosityBlock();

  //
  // Fill event info tree
  //   
  edm::Handle<double> weight;        
  iEvent.getByLabel("PUweight", weight);
  if (weight.isValid()) {
    PUweight_ = *weight;
  } else {
    PUweight_ = 1.0;
  }
  
  // Pile-up reweight
  if( isMC_ ) {
     edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
     iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
     std::vector<PileupSummaryInfo>::const_iterator PVI;

     int npv = -1;
     int npvin = -1;
     for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

       int BX = PVI->getBunchCrossing();

       if(BX == 0) {
         npvin = PVI->getPU_NumInteractions(); // in time only
         npv = PVI->getTrueNumInteractions(); // in and out of time
         continue;
       }
     }
  
     nPileUp_ = npv;
     nPileUpIn_ = npvin;
     pileUpReweightIn_ = LumiWeights_.weight( npvin );
     pileUpReweightPlusIn_  = PShiftUp_.ShiftWeight( npvin );
     pileUpReweightMinusIn_ = PShiftDown_.ShiftWeight( npvin );
     pileUpReweight_ = LumiWeights_.weight( npv );
     pileUpReweightPlus_  = PShiftUp_.ShiftWeight( npv );
     pileUpReweightMinus_ = PShiftDown_.ShiftWeight( npv );
  }

  edm::Handle< pat::MuonCollection > muonHandle;
  iEvent.getByLabel(theMuonLabel_, muonHandle);

  Nmuons_ = muonHandle->size();
  //if (Nmuons_ < 2) return;

  edm::Handle< pat::ElectronCollection > electronHandle;
  iEvent.getByLabel(theElectronLabel_, electronHandle);

  edm::Handle<reco::BeamSpot> beamSpotHandle;
  iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
  reco::BeamSpot beamSpot = (*beamSpotHandle);

  // vertex
  math::XYZPoint refVtx;
  refVtx.SetXYZ(0, 0, 0);

  edm::Handle<reco::VertexCollection> pvHandle;
  iEvent.getByLabel("offlinePrimaryVertices", pvHandle);
  refVtx = (*pvHandle).at(0).position();
  nVertices_ = pvHandle->size();

  // B field
  ESHandle<MagneticField> B;
  iSetup.get<IdealMagneticFieldRecord>().get(B);

  //
  // DiMuonTree 
  //
  for (pat::MuonCollection::const_iterator it1 = muonHandle->begin(); it1 != muonHandle->end(); ++it1) {

    if (!it1->isPFMuon()) continue;  

    for (pat::ElectronCollection::const_iterator it_el2 = electronHandle->begin(); it_el2 != electronHandle->end(); ++it_el2) {
    
      // Electron
      if (!it_el2->gsfTrack().isNonnull()) continue;
      Electron_E[nEMuPair_] = it_el2->superCluster()->energy();
      Electron_pT[nEMuPair_] = it_el2->pt();
      Electron_Px[nEMuPair_] = it_el2->px();
      Electron_Py[nEMuPair_] = it_el2->py();
      Electron_Pz[nEMuPair_] = it_el2->pz();
      Electron_eta[nEMuPair_] = it_el2->eta(); 
      Electron_SCeta[nEMuPair_] = it_el2->superCluster()->eta();
      Electron_phi[nEMuPair_] = it_el2->phi();
      Electron_charge[nEMuPair_] = it_el2->charge();
      Electron_trkiso[nEMuPair_] = it_el2->trackIso();
      Electron_hcaliso[nEMuPair_] = it_el2->hcalIso();
      Electron_ecaliso[nEMuPair_] = it_el2->ecalIso();
      Electron_isEcalDriven[nEMuPair_] = it_el2->ecalDrivenSeed();
      Electron_dxyBS[nEMuPair_] = it_el2->gsfTrack()->dxy(beamSpot.position());
      Electron_trackerHits[nEMuPair_] = it_el2->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
      Electron_SCSigmaIEtaIEta[nEMuPair_] = it_el2->scSigmaIEtaIEta();
      Electron_dPhiSCTrackAtVtx[nEMuPair_] = it_el2->deltaPhiSuperClusterTrackAtVtx();
      Electron_dEtaSCTrackAtVtx[nEMuPair_] = it_el2->deltaEtaSuperClusterTrackAtVtx();
      Electron_HcalOverEcal[nEMuPair_] = it_el2->hcalOverEcal();
      Electron_Brem[nEMuPair_] = it_el2->fbrem();
      Electron_SCClusterOverP[nEMuPair_] = it_el2->eSuperClusterOverP();
      // angle between tracks
      if ( it1->track().isNonnull()) {
        EMu_CosAngle[nEMuPair_] = acos(-it1->track()->momentum().Dot(it_el2->gsfTrack()->momentum()/
                                       it1->track()->p()/it_el2->gsfTrack()->p()));
      }
      Electron_passVtxCuts[nEMuPair_] = passesVertexCuts(iSetup, *it1, *it_el2);
      nEMuPair_++;
    }

    for(pat::MuonCollection::const_iterator it2 = it1+1; it2 != muonHandle->end(); ++it2) {

      if (!it2->isPFMuon()) continue; 

      // muon1 kinematics
      Muon1_pT[nPair_] = it1->pt();
      Muon1_Px[nPair_] = it1->px();
      Muon1_Py[nPair_] = it1->py();
      Muon1_Pz[nPair_] = it1->pz();
      Muon1_eta[nPair_] = it1->eta();
      Muon1_phi[nPair_] = it1->phi();
      Muon1_trkiso[nPair_] = it1->isolationR03().sumPt;
      Muon1_hcaliso[nPair_] = it1->isolationR03().hadEt;
      Muon1_ecaliso[nPair_] = it1->isolationR03().emEt;
 
      Muon1_PfChargedHadronIsoR03[nPair_] = it1->pfIsolationR03().sumChargedHadronPt;
      Muon1_PfNeutralHadronIsoR03[nPair_] = it1->pfIsolationR03().sumNeutralHadronEt;
      Muon1_PfGammaIsoR03[nPair_] = it1->pfIsolationR03().sumPhotonEt;

      Muon1_charge[nPair_] = it1->charge();
      Muon1_nChambers[nPair_] = it1->numberOfChambers(); // # of chambers
      Muon1_nMatches[nPair_] = it1->numberOfMatches(); // # of chambers with matched segments
      Muon1_stationMask[nPair_] = it1->stationMask(); // bit map of stations with matched segments
/*    if (!isAOD && it1->isTimeValid() ) {
         Muon1_timeInsideOut[nPair_] = it1->time().timeAtIpInOut; 
         Muon1_timeOutsideIn[nPair_] = it1->time().timeAtIpOutIn; 
      } 
*/
      // bits 0-1-2-3 = DT stations 1-2-3-4
      // bits 4-5-6-7 = CSC stations 1-2-3-4
      int loc_segments = 0;
      for (int idet = 1; idet < 4; idet++) {
        // DT (1), CSC (2), RPC (3)
        for (int istation = 1; istation < 5; istation++ ) {
          // station 1, 2, 3, 4
          loc_segments += it1->numberOfSegments(istation, idet);
        }
      }
      Muon1_nSegments[nPair_] = loc_segments;
      if ( it1->isGlobalMuon() && it1->isTrackerMuon() ) Muon1_muonType[nPair_] = 1; // global muon
      else if( it1->isStandAloneMuon() ) Muon1_muonType[nPair_] = 2; // STA muon
      else if( it1->isTrackerMuon() ) Muon1_muonType[nPair_] = 3; // track muon
      else Muon1_muonType[nPair_] = 0;

      // muon ID
      if ( it1->isCaloCompatibilityValid() ) Muon1_caloCompatibility[nPair_] = it1->caloCompatibility();
      Muon1_segCompatibility[nPair_] = muon::segmentCompatibility(*it1);
      Muon1_CaloEM[nPair_] = it1->calEnergy().em;
      Muon1_CaloHad[nPair_] = it1->calEnergy().had;
      if ( muon::isGoodMuon(*it1, muon::All) ) Muon1_ID[nPair_] = 0; // dummy options - always true
      if ( muon::isGoodMuon(*it1, muon::AllGlobalMuons) ) Muon1_ID[nPair_] = 1; // checks isGlobalMuon flag
      if ( muon::isGoodMuon(*it1, muon::AllStandAloneMuons) ) Muon1_ID[nPair_] = 2; // checks isStandAloneMuon flag
      if ( muon::isGoodMuon(*it1, muon::AllTrackerMuons) ) Muon1_ID[nPair_] = 3; // checks isTrackerMuon flag
      if ( muon::isGoodMuon(*it1, muon::TrackerMuonArbitrated) ) Muon1_ID[nPair_] = 4; // resolve ambiguity of sharing segments
      if ( muon::isGoodMuon(*it1, muon::AllArbitrated) ) Muon1_ID[nPair_] = 5; // all muons with the tracker muon arbitrated
      if ( muon::isGoodMuon(*it1, muon::GlobalMuonPromptTight) ) Muon1_ID[nPair_] = 6; // global muons with tighter fit requirements
      if ( muon::isGoodMuon(*it1, muon::TMLastStationLoose) ) Muon1_ID[nPair_] = 7; // penetration depth loose selector
      if ( muon::isGoodMuon(*it1, muon::TMLastStationTight) ) Muon1_ID[nPair_] = 8; // penetration depth tight selector
      if ( muon::isGoodMuon(*it1, muon::TM2DCompatibilityLoose) ) Muon1_ID[nPair_] = 9; // likelihood based loose selector
      if ( muon::isGoodMuon(*it1, muon::TM2DCompatibilityTight) ) Muon1_ID[nPair_] = 10; // likelihood based tight selector
      if ( muon::isGoodMuon(*it1, muon::TMOneStationLoose) ) Muon1_ID[nPair_] = 11; // require one well matched segment
      if ( muon::isGoodMuon(*it1, muon::TMOneStationTight) ) Muon1_ID[nPair_] = 12; // require one well matched segment
      if ( muon::isGoodMuon(*it1, muon::TMLastStationOptimizedLowPtLoose) ) Muon1_ID[nPair_] = 13; // combination of TMLastStation and TMOneStation
      if ( muon::isGoodMuon(*it1, muon::TMLastStationOptimizedLowPtTight) ) Muon1_ID[nPair_] = 14; // combination of TMLastStation and TMOneStation
      if ( muon::isGoodMuon(*it1, muon::GMTkChiCompatibility) ) Muon1_ID[nPair_] = 15;    // require tk stub have good chi2 relative to glb track
      if ( muon::isGoodMuon(*it1, muon::GMStaChiCompatibility) ) Muon1_ID[nPair_] = 16;   // require sta stub have good chi2 compatibility relative to glb track
      if ( muon::isGoodMuon(*it1, muon::GMTkKinkTight) ) Muon1_ID[nPair_] = 17;           // require a small kink value in the tracker stub
      if ( muon::isGoodMuon(*it1, muon::TMLastStationAngLoose) ) Muon1_ID[nPair_] = 18;   // TMLastStationLoose with additional angular cuts
      if ( muon::isGoodMuon(*it1, muon::TMLastStationAngTight) ) Muon1_ID[nPair_] = 19;   // TMLastStationTight with additional angular cuts
      if ( muon::isGoodMuon(*it1, muon::TMOneStationAngLoose) ) Muon1_ID[nPair_] = 20;    // TMOneStationLoose with additional angular cuts
      if ( muon::isGoodMuon(*it1, muon::TMOneStationAngTight) ) Muon1_ID[nPair_] = 21;    // TMOneStationTight with additional angular cuts
	    // The two algorithms that follow are identical to what were known as
	    // TMLastStationOptimizedLowPt* (sans the Barrel) as late as revision
	    // 1.7 of this file. The names were changed because indeed the low pt
	    // optimization applies only to the barrel region, whereas the sel-
	    // ectors above are more efficient at low pt in the endcaps, which is
	    // what we feel is more suggestive of the algorithm name. This will be
	    // less confusing for future generations of CMS members, I hope...
      if ( muon::isGoodMuon(*it1, muon::TMLastStationOptimizedBarrelLowPtLoose) ) Muon1_ID[nPair_] = 22; // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only
      if ( muon::isGoodMuon(*it1, muon::TMLastStationOptimizedBarrelLowPtTight) ) Muon1_ID[nPair_] = 23; // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only

      // global, track and STA
      reco::TrackRef muon1Trk;
      if ( Muon1_muonType[nPair_] == 1 ) muon1Trk = it1->globalTrack();
      if ( Muon1_muonType[nPair_] == 2 ) muon1Trk = it1->standAloneMuon();
      if ( Muon1_muonType[nPair_] == 3 ) muon1Trk = it1->track();
      // reco track information
      if ( muon1Trk.isNonnull() ) {
        if ( muon1Trk->normalizedChi2() < 1000 ) Muon1_chi2dof[nPair_] = muon1Trk->normalizedChi2();
        Muon1_nhits[nPair_] = muon1Trk->numberOfValidHits();
        if ( Muon1_muonType[nPair_] == 1 ) {
          reco::TrackRef trackerTrack = it1->innerTrack();
          reco::TrackRef glbTrack     = it1->globalTrack();
          const reco::HitPattern& inhit = trackerTrack->hitPattern();
          const reco::HitPattern& glbhit = glbTrack->hitPattern();
          Muon1_nTrackerLayers[nPair_] = glbhit.trackerLayersWithMeasurement();
          Muon1_trackerHits[nPair_] = inhit.numberOfValidTrackerHits();
          Muon1_pixelHits[nPair_] = inhit.numberOfValidPixelHits();
          Muon1_muonHits[nPair_] = glbhit.numberOfValidMuonHits();
        }
        Muon1_qoverp[nPair_] = muon1Trk->qoverp();
        Muon1_theta[nPair_] = muon1Trk->theta();
        Muon1_lambda[nPair_] = muon1Trk->lambda();
        Muon1_dxy[nPair_] = muon1Trk->dxy();
        Muon1_d0[nPair_] = muon1Trk->d0();
        Muon1_dsz[nPair_] = muon1Trk->dsz();
        Muon1_dz[nPair_] = muon1Trk->dz();
        Muon1_dxyBS[nPair_] = muon1Trk->dxy(beamSpot.position());
        Muon1_dszBS[nPair_] = muon1Trk->dsz(beamSpot.position());
        Muon1_dzBS[nPair_] = muon1Trk->dz(beamSpot.position());
	if ( !pvHandle->empty() && !pvHandle->front().isFake() ) {
          const reco::Vertex& vtx = pvHandle->front();
          Muon1_dxyVTX[nPair_] = muon1Trk->dxy(vtx.position());
          Muon1_dszVTX[nPair_] = muon1Trk->dsz(vtx.position());
          Muon1_dzVTX[nPair_] = muon1Trk->dz(vtx.position());
	}
        Muon1_vx[nPair_] = muon1Trk->vx();
        Muon1_vy[nPair_] = muon1Trk->vy();
        Muon1_vz[nPair_] = muon1Trk->vz();
      }

	    // MC truthMatch
	    /*if( isMC && theStoreGENFlag ) {
            	reco::GenParticleRef genMuon1 = it1->genParticleRef();
		if( genMuon1.isNonnull() ) {
	    	    Muon1_MCtruth_pT[nPair_] = genMuon1->pt();
	    	    Muon1_MCtruth_Px[nPair_] = genMuon1->px();
	    	    Muon1_MCtruth_Py[nPair_] = genMuon1->py();
	    	    Muon1_MCtruth_Pz[nPair_] = genMuon1->pz();
	    	    Muon1_MCtruth_eta[nPair_] = genMuon1->eta();
	    	    Muon1_MCtruth_phi[nPair_] = genMuon1->phi();
	    	    Muon1_MCtruth_charge[nPair_] = genMuon1->charge();
	    	    Muon1_MCtruth_mother[nPair_] = motherId(genMuon1);
	    	    Muon1_MCtruth_pdgId[nPair_] = genMuon1->pdgId();
		}
	    }
           */

        // dimuon candidate
        reco::NamedCompositeCandidate aDYcand;
        aDYcand.setP4(it1->p4() + it2->p4());
        InvMass[nPair_] = aDYcand.mass();
        Rapidity[nPair_] = aDYcand.rapidity();
        Pt[nPair_] = aDYcand.pt();
        Angle[nPair_] = angleBetween(*it1, *it2);
        DeltaR[nPair_] = sqrt( pow(it1->eta() - it2->eta(), 2) + pow(it1->phi() - it2->phi(), 2) );
        Dalpha[nPair_] = (it1->theta() - it2->theta()) + (it1->phi() - it2->phi()) - 2*TMath::Pi();
        if ( it1->charge() != it2->charge() ) isOppSign[nPair_] = true;
        else isOppSign[nPair_] = false;

        // angle between tracks
        if ( it1->track().isNonnull() && it2->track().isNonnull() ) {
          CosAngle[nPair_] = acos(-it1->track()->momentum().Dot(it2->track()->momentum()/
	                          it1->track()->p()/it2->track()->p()));
        }

        // muon2 kinematics
        Muon2_pT[nPair_] = it2->pt();
        Muon2_Px[nPair_] = it2->px();
        Muon2_Py[nPair_] = it2->py();
        Muon2_Pz[nPair_] = it2->pz();
        Muon2_eta[nPair_] = it2->eta();
        Muon2_phi[nPair_] = it2->phi();

        Muon2_trkiso[nPair_] = it1->isolationR03().sumPt;
        Muon2_hcaliso[nPair_] = it1->isolationR03().hadEt;
        Muon2_ecaliso[nPair_] = it1->isolationR03().emEt;
            
        Muon2_PfChargedHadronIsoR03[nPair_] = it2->pfIsolationR03().sumChargedHadronPt;
        Muon2_PfNeutralHadronIsoR03[nPair_] = it2->pfIsolationR03().sumNeutralHadronEt;
        Muon2_PfGammaIsoR03[nPair_] = it2->pfIsolationR03().sumPhotonEt;

        Muon2_charge[nPair_] = it2->charge();
        Muon2_nChambers[nPair_] = it2->numberOfChambers(); // # of chambers
        Muon2_nMatches[nPair_] = it2->numberOfMatches(); // # of chambers with matched segments
        Muon2_stationMask[nPair_] = it2->stationMask(); // bit map of stations with matched segments
/*            if (!isAOD) {
    	        if( it2->isTimeValid() ) {
	            Muon2_timeInsideOut[nPair_] = it2->time().timeAtIpInOut; 
	            Muon2_timeOutsideIn[nPair_] = it2->time().timeAtIpOutIn; 
	        }
            }*/
	       // bits 0-1-2-3 = DT stations 1-2-3-4
	       // bits 4-5-6-7 = CSC stations 1-2-3-4
	       loc_segments = 0;
	       for ( int idet = 1; idet < 4; idet++ ) {
	  	  // DT (1), CSC (2), RPC (3)
		  for ( int istation = 1; istation < 5; istation++ ) {
		    // station 1, 2, 3, 4
		    loc_segments += it2->numberOfSegments(istation, idet);
		   }
	       }
	       Muon2_nSegments[nPair_] = loc_segments;

	       // muon ID
	       if ( it2->isCaloCompatibilityValid() ) Muon2_caloCompatibility[nPair_] = it2->caloCompatibility();
	       Muon2_segCompatibility[nPair_] = muon::segmentCompatibility(*it2);
	       Muon2_CaloEM[nPair_] = it2->calEnergy().em;
	       Muon2_CaloHad[nPair_] = it2->calEnergy().had;
	       if( muon::isGoodMuon(*it2, muon::All) ) Muon2_ID[nPair_] = 0; // dummy options - always true
	       if( muon::isGoodMuon(*it2, muon::AllGlobalMuons) ) Muon2_ID[nPair_] = 1; // checks isGlobalMuon flag
	       if( muon::isGoodMuon(*it2, muon::AllStandAloneMuons) ) Muon2_ID[nPair_] = 2; // checks isStandAloneMuon flag
	       if( muon::isGoodMuon(*it2, muon::AllTrackerMuons) ) Muon2_ID[nPair_] = 3; // checks isTrackerMuon flag
	       if( muon::isGoodMuon(*it2, muon::TrackerMuonArbitrated) ) Muon2_ID[nPair_] = 4; // resolve ambiguity of sharing segments
	       if( muon::isGoodMuon(*it2, muon::AllArbitrated) ) Muon2_ID[nPair_] = 5; // all muons with the tracker muon arbitrated
	       if( muon::isGoodMuon(*it2, muon::GlobalMuonPromptTight) ) Muon2_ID[nPair_] = 6; // global muons with tighter fit requirements
	       if( muon::isGoodMuon(*it2, muon::TMLastStationLoose) ) Muon2_ID[nPair_] = 7; // penetration depth loose selector
	       if( muon::isGoodMuon(*it2, muon::TMLastStationTight) ) Muon2_ID[nPair_] = 8; // penetration depth tight selector
	       if( muon::isGoodMuon(*it2, muon::TM2DCompatibilityLoose) ) Muon2_ID[nPair_] = 9; // likelihood based loose selector
	       if( muon::isGoodMuon(*it2, muon::TM2DCompatibilityTight) ) Muon2_ID[nPair_] = 10; // likelihood based tight selector
	       if( muon::isGoodMuon(*it2, muon::TMOneStationLoose) ) Muon2_ID[nPair_] = 11; // require one well matched segment
	       if( muon::isGoodMuon(*it2, muon::TMOneStationTight) ) Muon2_ID[nPair_] = 12; // require one well matched segment
	       if( muon::isGoodMuon(*it2, muon::TMLastStationOptimizedLowPtLoose) ) Muon2_ID[nPair_] = 13; // combination of TMLastStation and TMOneStation
	       if( muon::isGoodMuon(*it2, muon::TMLastStationOptimizedLowPtTight) ) Muon2_ID[nPair_] = 14; // combination of TMLastStation and TMOneStation
	       if( muon::isGoodMuon(*it2, muon::GMTkChiCompatibility) ) Muon2_ID[nPair_] = 15;    // require tk stub have good chi2 relative to glb track
	       if( muon::isGoodMuon(*it2, muon::GMStaChiCompatibility) ) Muon2_ID[nPair_] = 16;   // require sta stub have good chi2 compatibility relative to glb track
	       if( muon::isGoodMuon(*it2, muon::GMTkKinkTight) ) Muon2_ID[nPair_] = 17;           // require a small kink value in the tracker stub
	       if( muon::isGoodMuon(*it2, muon::TMLastStationAngLoose) ) Muon2_ID[nPair_] = 18;   // TMLastStationLoose with additional angular cuts
	       if( muon::isGoodMuon(*it2, muon::TMLastStationAngTight) ) Muon2_ID[nPair_] = 19;   // TMLastStationTight with additional angular cuts
	       if( muon::isGoodMuon(*it2, muon::TMOneStationAngLoose) ) Muon2_ID[nPair_] = 20;    // TMOneStationLoose with additional angular cuts
	       if( muon::isGoodMuon(*it2, muon::TMOneStationAngTight) ) Muon2_ID[nPair_] = 21;    // TMOneStationTight with additional angular cuts
	       // The two algorithms that follow are identical to what were known as
	       // TMLastStationOptimizedLowPt* (sans the Barrel) as late as revision
	       // 1.7 of this file. The names were changed because indeed the low pt
	       // optimization applies only to the barrel region, whereas the sel-
	       // ectors above are more efficient at low pt in the endcaps, which is
	       // what we feel is more suggestive of the algorithm name. This will be
	       // less confusing for future generations of CMS members, I hope...
	       if( muon::isGoodMuon(*it2, muon::TMLastStationOptimizedBarrelLowPtLoose) ) Muon2_ID[nPair_] = 22; // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only
	       if( muon::isGoodMuon(*it2, muon::TMLastStationOptimizedBarrelLowPtTight) ) Muon2_ID[nPair_] = 23; // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only

	       if( it2->isGlobalMuon() && it2->isTrackerMuon() ) Muon2_muonType[nPair_] = 1; // global muon
	       else if( it2->isStandAloneMuon() ) Muon2_muonType[nPair_] = 2; // STA muon
	       else if( it2->isTrackerMuon() ) Muon2_muonType[nPair_] = 3; // track muon

	       // global, track and STA
	       reco::TrackRef muon2Trk;
	       if ( Muon2_muonType[nPair_] == 1 ) muon2Trk = it2->globalTrack();
	       if ( Muon2_muonType[nPair_] == 2 ) muon2Trk = it2->standAloneMuon();
	       if ( Muon2_muonType[nPair_] == 3 ) muon2Trk = it2->track();
	       // reco track information
	       if ( muon2Trk.isNonnull() ) {
	          if ( muon2Trk->normalizedChi2() < 1000 ) Muon2_chi2dof[nPair_] = muon2Trk->normalizedChi2();
    	          Muon2_nhits[nPair_] = muon2Trk->numberOfValidHits();
 		  if ( Muon2_muonType[nPair_] == 1 ) {
		    reco::TrackRef trackerTrack = it2->innerTrack();
		    reco::TrackRef glbTrack     = it2->globalTrack();

		    const reco::HitPattern & inhit = trackerTrack->hitPattern();
		    const reco::HitPattern & glbhit = glbTrack->hitPattern();

                    Muon2_nTrackerLayers[nPair_] = glbhit.trackerLayersWithMeasurement();
    	            Muon2_trackerHits[nPair_] = inhit.numberOfValidTrackerHits();
    	            Muon2_pixelHits[nPair_] = inhit.numberOfValidPixelHits();
    	            Muon2_muonHits[nPair_] = glbhit.numberOfValidMuonHits();
		 }
	         Muon2_qoverp[nPair_] = muon2Trk->qoverp();
	         Muon2_theta[nPair_] = muon2Trk->theta();
	         Muon2_lambda[nPair_] = muon2Trk->lambda();
	         Muon2_dxy[nPair_] = muon2Trk->dxy();
	         Muon2_dxyBS[nPair_] = muon2Trk->dxy(beamSpot.position());
	         Muon2_dszBS[nPair_] = muon2Trk->dsz(beamSpot.position());
	         Muon2_dzBS[nPair_] = muon2Trk->dz(beamSpot.position());
		 if ( !pvHandle->empty() && !pvHandle->front().isFake() ) {
		   const reco::Vertex &vtx = pvHandle->front();
	           Muon2_dxyVTX[nPair_] = muon2Trk->dxy(vtx.position());
	           Muon2_dszVTX[nPair_] = muon2Trk->dsz(vtx.position());
	           Muon2_dzVTX[nPair_] = muon2Trk->dz(vtx.position());
		 }
	         Muon2_d0[nPair_] = muon2Trk->d0();
	         Muon2_dsz[nPair_] = muon2Trk->dsz();
	         Muon2_dz[nPair_] = muon2Trk->dz();
	         Muon2_vx[nPair_] = muon2Trk->vx();
	         Muon2_vy[nPair_] = muon2Trk->vy();
	         Muon2_vz[nPair_] = muon2Trk->vz();
	      }

	     // dimuon vertex with global track
	     if ( muon1Trk.isNonnull() && muon2Trk.isNonnull() ) {
	    	reco::TransientTrack muon1Transient( muon1Trk, B.product());
	    	reco::TransientTrack muon2Transient( muon2Trk, B.product());
	    
	    	vector<reco::TransientTrack> dimuonTracks;
	    	dimuonTracks.push_back(muon1Transient);
	    	dimuonTracks.push_back(muon2Transient);
	    	KalmanVertexFitter KalmanFitter(true);
		CachingVertex<5> vertex;
		TransientVertex vtxtmp;
	    	bool isVertex = true;
	    	try {
		vertex = KalmanFitter.vertex(dimuonTracks);
		vtxtmp = KalmanFitter.vertex(dimuonTracks);
	    	}	
	    	catch( exception & err ) {
		isVertex = false;
	    	}
	    	if ( isVertex && vertex.isValid() ) {
			// inv. mass refit using the dimuon vtx
			InvariantMassFromVertex imfv;
			static const double muon_mass = 0.1056583;
			const CachingVertex<5>& vtx = vertex;
			InvariantMassFromVertex::LorentzVector new_p4 = imfv.p4(vtx, muon_mass);
			Measurement1D new_mass = imfv.invariantMass(vtx, muon_mass);

			vtxChi2[nPair_] = vtx.totalChiSquared();
			vtxNdof[nPair_] = vtx.degreesOfFreedom();
			vtxPositionX[nPair_] = vtx.position().x();
			vtxPositionY[nPair_] = vtx.position().y();
			vtxPositionZ[nPair_] = vtx.position().z();
			vtxXerror[nPair_] = sqrt(vtx.error().cxx());
			vtxYerror[nPair_] = sqrt(vtx.error().cyy());
			vtxZerror[nPair_] = sqrt(vtx.error().czz());
			vtxProb[nPair_]  = TMath::Prob(vtx.totalChiSquared(),(int)vtx.degreesOfFreedom());

			// inv. mass using dimuon vertex
			vtx_newPx[nPair_] = new_p4.X();
			vtx_newPy[nPair_] = new_p4.Y();
			vtx_newPz[nPair_] = new_p4.Z();
			vtx_newMass[nPair_] = new_mass.value();
			vtx_newMassErr[nPair_] = new_mass.error();

			// dxy 
			reco::Vertex reco_vtx = vtxtmp;
			Muon1_dxyDiMuVTX[nPair_] = muon1Trk->dxy(reco_vtx.position());
	                Muon1_dszDiMuVTX[nPair_] = muon1Trk->dsz(reco_vtx.position());
	                Muon1_dzDiMuVTX[nPair_] = muon1Trk->dz(reco_vtx.position());

			Muon2_dxyDiMuVTX[nPair_] = muon2Trk->dxy(reco_vtx.position());
	                Muon2_dszDiMuVTX[nPair_] = muon2Trk->dsz(reco_vtx.position());
	                Muon2_dzDiMuVTX[nPair_] = muon2Trk->dz(reco_vtx.position());
	     	}
	     }

	     // dimuon vertex with tracker track
	     reco::TrackRef mu1trkerTrk = it1->track();
	     reco::TrackRef mu2trkerTrk = it2->track();
	     if ( mu1trkerTrk.isNonnull() && mu2trkerTrk.isNonnull() ) {
	        ESHandle<MagneticField> B;
	    	iSetup.get<IdealMagneticFieldRecord>().get(B);

	    	reco::TransientTrack muon1TransientTrk( mu1trkerTrk, B.product());
	    	reco::TransientTrack muon2TransientTrk( mu2trkerTrk, B.product());
	    
	    	vector<reco::TransientTrack> dimuonTracksTrk;
	    	dimuonTracksTrk.push_back(muon1TransientTrk);
	    	dimuonTracksTrk.push_back(muon2TransientTrk);
	    	KalmanVertexFitter KalmanFitterTrk(true);
		CachingVertex<5> vertexTrk;
		TransientVertex vtxtmpTrk;
	    	bool isVertexTrk = true;
	    	try {
			vertexTrk = KalmanFitterTrk.vertex(dimuonTracksTrk);
			vtxtmpTrk = KalmanFitterTrk.vertex(dimuonTracksTrk);
	    	}	
	    	catch( exception & err ) {
			isVertexTrk = false;
	    	}
	    	if ( isVertexTrk && vertexTrk.isValid() ) {
			// inv. mass refit using the dimuon vtx
			InvariantMassFromVertex imfvTrk;
			static const double muon_mass = 0.1056583;
			const CachingVertex<5>& vtxTrk = vertexTrk;
			InvariantMassFromVertex::LorentzVector new_p4Trk = imfvTrk.p4(vtxTrk, muon_mass);
			Measurement1D new_massTrk = imfvTrk.invariantMass(vtxTrk, muon_mass);

			vtxTrkChi2[nPair_] = vtxTrk.totalChiSquared();
			vtxTrkNdof[nPair_] = vtxTrk.degreesOfFreedom();
			vtxTrkPositionX[nPair_] = vtxTrk.position().x();
			vtxTrkPositionY[nPair_] = vtxTrk.position().y();
			vtxTrkPositionZ[nPair_] = vtxTrk.position().z();
			vtxTrkXerror[nPair_] = sqrt(vtxTrk.error().cxx());
			vtxTrkYerror[nPair_] = sqrt(vtxTrk.error().cyy());
			vtxTrkZerror[nPair_] = sqrt(vtxTrk.error().czz());
			vtxTrkProb[nPair_]  = TMath::Prob(vtxTrk.totalChiSquared(),(int)vtxTrk.degreesOfFreedom());

			// inv. mass using dimuon vertex
			vtxTrk_newPx[nPair_] = new_p4Trk.X();
			vtxTrk_newPy[nPair_] = new_p4Trk.Y();
			vtxTrk_newPz[nPair_] = new_p4Trk.Z();
			vtxTrk_newMass[nPair_] = new_massTrk.value();
			vtxTrk_newMassErr[nPair_] = new_massTrk.error();

			// dxy 
			reco::Vertex reco_vtxTrk = vtxtmpTrk;
			Muon1_dxyDiMuVTXTrk[nPair_] = mu1trkerTrk->dxy(reco_vtxTrk.position());
	                Muon1_dszDiMuVTXTrk[nPair_] = mu1trkerTrk->dsz(reco_vtxTrk.position());
	                Muon1_dzDiMuVTXTrk[nPair_] = mu1trkerTrk->dz(reco_vtxTrk.position());

			Muon2_dxyDiMuVTXTrk[nPair_] = mu2trkerTrk->dxy(reco_vtxTrk.position());
	                Muon2_dszDiMuVTXTrk[nPair_] = mu2trkerTrk->dsz(reco_vtxTrk.position());
	                Muon2_dzDiMuVTXTrk[nPair_] = mu2trkerTrk->dz(reco_vtxTrk.position());
	    	 }
	     }

	    // MC truthMatch
	    /*if( isMC && theStoreGENFlag ) {
            	reco::GenParticleRef genMuon2 = it2->genParticleRef();
		if( genMuon2.isNonnull() ) {
	    	    Muon2_MCtruth_pT[nPair_] = genMuon2->pt();
	    	    Muon2_MCtruth_Px[nPair_] = genMuon2->px();
	    	    Muon2_MCtruth_Py[nPair_] = genMuon2->py();
	    	    Muon2_MCtruth_Pz[nPair_] = genMuon2->pz();
	    	    Muon2_MCtruth_eta[nPair_] = genMuon2->eta();
	    	    Muon2_MCtruth_phi[nPair_] = genMuon2->phi();
	    	    Muon2_MCtruth_charge[nPair_] = genMuon2->charge();
	    	    Muon2_MCtruth_mother[nPair_] = motherId(genMuon2);
	    	    Muon2_MCtruth_pdgId[nPair_] = genMuon2->pdgId();
		}
	    }
            */
	   nPair_++;
       }
   }
   // Gen Muon
    // Gen 
  if ( isMC_) { 
     edm::Handle <reco::GenParticleCollection> particles;
     iEvent.getByLabel("genParticles", particles);

     for ( size_t m1 = 0; m1 < particles->size(); m1++ ) {
       const reco::GenParticle &MClepton1 = (*particles)[m1];
       if ( !(abs(motherId(MClepton1)) == 23 || abs(motherId(MClepton1)) == 22) ) continue;
       if (!(abs(MClepton1.pdgId()) == 13)) continue;
       if (!(MClepton1.status() == 1 || MClepton1.status() == 3)) continue;
       for ( size_t m2 = m1+1; m2 < particles->size(); m2++ ) {
         const reco::GenParticle& MClepton2 = (*particles)[m2];

         if (!(MClepton2.status() == 1 || MClepton2.status() == 3)) continue;
         if ( !(abs(motherId(MClepton2)) == 23 || abs(motherId(MClepton2)) == 22) ) continue;
         if ( !(abs(MClepton2.pdgId()) == 13)) continue;
         if ( MClepton1.charge() == MClepton2.charge()) continue;
         if (abs(MClepton1.pdgId()) != abs(MClepton2.pdgId())) continue;
         if (MClepton1.status() != MClepton2.status()) continue;

         // 4 vectors with muon Gen information
         TVector3 mu1Gen3V(MClepton1.px(), MClepton1.py(), MClepton1.pz());
         double mu1mass = MClepton1.mass();
         double mu1E = sqrt(mu1Gen3V.Mag2() + mu1mass*mu1mass);
         reco::Particle::LorentzVector mu1Gen4V(MClepton1.px(), MClepton1.py(), MClepton1.pz(), mu1E);

         TVector3 mu2Gen3V(MClepton2.px(), MClepton2.py(), MClepton2.pz());
         double mu2mass = MClepton2.mass();
         double mu2E = sqrt(mu2Gen3V.Mag2() + mu2mass*mu2mass);
         reco::Particle::LorentzVector mu2Gen4V(MClepton2.px(), MClepton2.py(), MClepton2.pz(), mu2E);

         // candidate
         reco::NamedCompositeCandidate aDYcand;
         aDYcand.setP4(mu1Gen4V + mu2Gen4V);

         GENInvMass[GENnPair_] = aDYcand.mass();
         GENRapidity[GENnPair_] = aDYcand.rapidity();
         GENPt[GENnPair_] = aDYcand.pt();
         //cout << "gen mass = " << aDYcand.mass() << endl; //" " << MClepton1.status() << " " << MClepton2.status() << endl;

         GENAngle[GENnPair_] = angleBetween(MClepton1, MClepton2);

         // muon1 kinematics
         GENMuon1_pT[GENnPair_] = MClepton1.pt();
         GENMuon1_Px[GENnPair_] = MClepton1.px();
         GENMuon1_Py[GENnPair_] = MClepton1.py();
         GENMuon1_Pz[GENnPair_] = MClepton1.pz();
         GENMuon1_eta[GENnPair_] = MClepton1.eta();
         GENMuon1_phi[GENnPair_] = MClepton1.phi();
         GENMuon1_charge[GENnPair_] = MClepton1.charge();
         GENMuon1_status[GENnPair_] = MClepton1.status();

         // muon2 kinematics
         GENMuon2_pT[GENnPair_] = MClepton2.pt();
         GENMuon2_Px[GENnPair_] = MClepton2.px();
         GENMuon2_Py[GENnPair_] = MClepton2.py();
         GENMuon2_Pz[GENnPair_] = MClepton2.pz();
         GENMuon2_eta[GENnPair_] = MClepton2.eta();
         GENMuon2_phi[GENnPair_] = MClepton2.phi();
         GENMuon2_charge[GENnPair_] = MClepton2.charge();
         GENMuon2_status[GENnPair_] = MClepton2.status();

         GENnPair_++;
       }
     }
   }//the store GEN
   // fill the tree
   if ( theStoreHLTReportFlag_ ) hltReport(iEvent);
   if ( theStorePriVtxFlag_ ) fillPrimaryVertex(iEvent);
   if ( theStoreJetMETFlag_ ) {
     fillMET(iEvent);
     fillJet(iEvent);
   }
   DiMuonTree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
DiMuonIntoNtuples::beginJob()
{

  if( isMC_ ) {
    // Pileup Reweight: 2012, Summer12_S10
    std::vector< float > _PUreweightRun2012 ;
    std::vector< float > _MC2012;
  
    for( int i = 0; i < 100; ++i) {
      _MC2012.push_back((float)PileUpRD_[i]);
      _PUreweightRun2012.push_back((float)PileUpMC_[i]);
    }
  
    LumiWeights_ = edm::LumiReWeighting(_PUreweightRun2012, _MC2012);
  
    PShiftDown_ = reweight::PoissonMeanShifter(-0.5);
    PShiftUp_ = reweight::PoissonMeanShifter(0.5);
  }

  edm::Service<TFileService> fs;
  DiMuonTree_ = fs->make<TTree>("DiMuonTree","DiMuonTree");

  // global event varialbes
  DiMuonTree_->Branch("runNum",&runNum_,"runNum_/I");
  DiMuonTree_->Branch("evtNum",&evtNum_,"evtNum_/I");
  DiMuonTree_->Branch("lumiBlock",&lumiBlock_,"lumiBlock_/I");
  DiMuonTree_->Branch("nPair",&nPair_,"nPair_/I");
  DiMuonTree_->Branch("Nmuons", &Nmuons_,"Nmuons_/I");
  DiMuonTree_->Branch("PUweight",&PUweight_,"PUweight_/D");
  DiMuonTree_->Branch("nVertices",&nVertices_,"nVertices_/I");
 
  if (theStorePriVtxFlag_) {
    DiMuonTree_->Branch("PVtrackSize", &PVtrackSize_,"PVtrackSize_/I");
    DiMuonTree_->Branch("PVchi2", &PVchi2_,"PVchi2_/D");
    DiMuonTree_->Branch("PVndof", &PVndof_,"PVndof_/I");
    DiMuonTree_->Branch("PVnormalizedChi2", &PVnormalizedChi2_,"PVnormalizedChi2_/D");
    DiMuonTree_->Branch("PVx", &PVx_,"PVx_/D");
    DiMuonTree_->Branch("PVy", &PVy_,"PVy_/D");
    DiMuonTree_->Branch("PVz", &PVz_,"PVz_/D");
  }

  if (theStoreHLTReportFlag_) {
    // for trigger matching
    DiMuonTree_->Branch("HLT_ntrig", &HLT_ntrig_,"HLT_ntrig_/I");
    //DiMuonTree_->Branch("HLT_trigType", &HLT_trigType,"HLT_trigType[HLT_ntrig_]/I");
    DiMuonTree_->Branch("HLT_trigFired", &HLT_trigFired,"HLT_trigFired[HLT_ntrig_]/B");
    //DiMuonTree_->Branch("HLT_trigName", &HLT_trigName_);
    DiMuonTree_->Branch("HLT_trigPt", &HLT_trigPt,"HLT_trigPt[HLT_ntrig_]/D");
    DiMuonTree_->Branch("HLT_trigEta", &HLT_trigEta,"HLT_trigEta[HLT_ntrig_]/D");
    DiMuonTree_->Branch("HLT_trigPhi", &HLT_trigPhi,"HLT_trigPhi[HLT_ntrig_]/D");
  }

  if (theStoreJetMETFlag_) {
  // Jet
    DiMuonTree_->Branch("Njets", &Njets_,"Njets_/I");
    DiMuonTree_->Branch("JETbDiscriminant", &JETbDiscriminant, "JETbDiscriminant[Njets_]/D");
    DiMuonTree_->Branch("JETbDiscriminant_alg1", &JETbDiscriminant_alg1, "JETbDiscriminant_alg1[Njets_]/D");
    DiMuonTree_->Branch("JETbDiscriminant_alg2", &JETbDiscriminant_alg2, "JETbDiscriminant_alg2[Njets_]/D");
    DiMuonTree_->Branch("JETbDiscriminant_alg3", &JETbDiscriminant_alg3, "JETbDiscriminant_alg3[Njets_]/D");
    DiMuonTree_->Branch("JETflavour", &JETflavour, "JETflavour[Njets_]/I");
    DiMuonTree_->Branch("JETcharge", &JETcharge, "JETcharge[Njets_]/D");
    DiMuonTree_->Branch("JETntracks", &JETntracks, "JETntracks[Njets_]/I");
    DiMuonTree_->Branch("JETpt", &JETpt, "JETpt[Njets_]/D");
    DiMuonTree_->Branch("JETeta", &JETeta, "JETeta[Njets_]/D");
    DiMuonTree_->Branch("JETphi", &JETphi, "JETphi[Njets_]/D");

  // MET
    DiMuonTree_->Branch("MET", &MET_,"MET_/D");
  }

  // Electron
  DiMuonTree_->Branch("nEMuPair", &nEMuPair_,"nEMuPair_/I");
  DiMuonTree_->Branch("Electron_E", &Electron_E, "Electron_E[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_pT", &Electron_pT, "Electron_pT[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_Px", &Electron_Px, "Electron_Px[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_Py", &Electron_Py, "Electron_Py[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_Pz", &Electron_Pz, "Electron_Pz[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_eta", &Electron_eta, "Electron_eta[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_SCeta", &Electron_SCeta, "Electron_SCeta[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_phi", &Electron_phi, "Electron_phi[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_charge", &Electron_charge, "Electron_charge[nEMuPair_]/I");
  DiMuonTree_->Branch("Electron_trkiso", &Electron_trkiso, "Electron_trkiso[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_hcaliso", &Electron_hcaliso, "Electron_hcaliso[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_ecaliso", &Electron_ecaliso, "Electron_ecaliso[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_isEcalDriven", &Electron_isEcalDriven, "Electron_isEcalDriven[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_dxyBS", &Electron_dxyBS, "Electron_dxyBS[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_trackerHits", &Electron_trackerHits, "Electron_trackerHits[nEMuPair_]/I");
  DiMuonTree_->Branch("Electron_SCSigmaIEtaIEta", &Electron_SCSigmaIEtaIEta, "Electron_SCSigmaIEtaIEta[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_dPhiSCTrackAtVtx", &Electron_dPhiSCTrackAtVtx, "Electron_dPhiSCTrackAtVtx[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_dEtaSCTrackAtVtx", &Electron_dEtaSCTrackAtVtx, "Electron_dEtaSCTrackAtVtx[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_HcalOverEcal", &Electron_HcalOverEcal, "Electron_HcalOverEcal[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_Brem", &Electron_Brem, "Electron_Brem[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_SCClusterOverP", &Electron_SCClusterOverP, "Electron_SCClusterOverP[nEMuPair_]/D");
  DiMuonTree_->Branch("EMu_CosAngle", &EMu_CosAngle, "EMu_CosAngle[nEMuPair_]/D");
  DiMuonTree_->Branch("Electron_passVtxCuts", &Electron_passVtxCuts, "Electron_passVtxCuts[nEMuPair_]/D");

  // Global Muon
  // muon pair variables
  DiMuonTree_->Branch("InvMass", &InvMass,"InvMass[nPair_]/D");
  DiMuonTree_->Branch("Rapidity", &Rapidity,"Rapidity[nPair_]/D");
  DiMuonTree_->Branch("Pt", &Pt,"Pt[nPair_]/D");
  DiMuonTree_->Branch("Angle", &Angle,"Angle[nPair_]/D");
  DiMuonTree_->Branch("CosAngle", &CosAngle,"CosAngle[nPair_]/D");
  DiMuonTree_->Branch("DeltaR", &DeltaR,"DeltaR[nPair_]/D");
  DiMuonTree_->Branch("Dalpha", &Dalpha,"Dalpha[nPair_]/D");
  DiMuonTree_->Branch("isOppSign", &isOppSign,"isOppSign[nPair_]/B");

  if (theStorePriVtx2MuFlag_) {
    // dimuon vtx with global track
    DiMuonTree_->Branch("vtxChi2", &vtxChi2,"vtxChi2[nPair_]/D");
    DiMuonTree_->Branch("vtxProb", &vtxProb,"vtxProb[nPair_]/D");
    DiMuonTree_->Branch("vtxNdof", &vtxNdof,"vtxNdof[nPair_]/I");
    DiMuonTree_->Branch("vtxPositionX", &vtxPositionX,"vtxPositionX[nPair_]/D");
    DiMuonTree_->Branch("vtxPositionY", &vtxPositionY,"vtxPositionY[nPair_]/D");
    DiMuonTree_->Branch("vtxPositionZ", &vtxPositionZ,"vtxPositionZ[nPair_]/D");
    DiMuonTree_->Branch("vtxXerror", &vtxXerror,"vtxXerror[nPair_]/D");
    DiMuonTree_->Branch("vtxYerror", &vtxYerror,"vtxYerror[nPair_]/D");
    DiMuonTree_->Branch("vtxZerror", &vtxZerror,"vtxZerror[nPair_]/D");
    DiMuonTree_->Branch("vtx_newPx", &vtx_newPx,"vtx_newPx[nPair_]/D");
    DiMuonTree_->Branch("vtx_newPy", &vtx_newPy,"vtx_newPy[nPair_]/D");
    DiMuonTree_->Branch("vtx_newPz", &vtx_newPz,"vtx_newPz[nPair_]/D");
    DiMuonTree_->Branch("vtx_newMass", &vtx_newMass,"vtx_newMass[nPair_]/D");
    DiMuonTree_->Branch("vtx_newMassErr", &vtx_newMassErr,"vtx_newMassErr[nPair_]/D");

    // dimuon vtx with tracker track
    DiMuonTree_->Branch("vtxTrkChi2", &vtxTrkChi2,"vtxTrkChi2[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkProb", &vtxTrkProb,"vtxTrkProb[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkNdof", &vtxTrkNdof,"vtxTrkNdof[nPair_]/I");
    DiMuonTree_->Branch("vtxTrkPositionX", &vtxTrkPositionX,"vtxTrkPositionX[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkPositionY", &vtxTrkPositionY,"vtxTrkPositionY[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkPositionZ", &vtxTrkPositionZ,"vtxTrkPositionZ[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkXerror", &vtxTrkXerror,"vtxTrkXerror[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkYerror", &vtxTrkYerror,"vtxTrkYerror[nPair_]/D");
    DiMuonTree_->Branch("vtxTrkZerror", &vtxTrkZerror,"vtxTrkZerror[nPair_]/D");
    DiMuonTree_->Branch("vtxTrk_newPx", &vtxTrk_newPx,"vtxTrk_newPx[nPair_]/D");
    DiMuonTree_->Branch("vtxTrk_newPy", &vtxTrk_newPy,"vtxTrk_newPy[nPair_]/D");
    DiMuonTree_->Branch("vtxTrk_newPz", &vtxTrk_newPz,"vtxTrk_newPz[nPair_]/D");
    DiMuonTree_->Branch("vtxTrk_newMass", &vtxTrk_newMass,"vtxTrk_newMass[nPair_]/D");
    DiMuonTree_->Branch("vtxTrk_newMassErr", &vtxTrk_newMassErr,"vtxTrk_newMassErr[nPair_]/D");
  }

  // object variables
  DiMuonTree_->Branch("Muon1_muonType", &Muon1_muonType,"Muon1_muonType[nPair_]/I");
  DiMuonTree_->Branch("Muon1_nTrig", &Muon1_nTrig,"Muon1_nTrig[nPair_]/I");
  DiMuonTree_->Branch("Muon1_triggerObjectType", &Muon1_triggerObjectType,"Muon1_triggerObjectType[nPair_]/I");
  DiMuonTree_->Branch("Muon1_filterName", &Muon1_filterName,"Muon1_filterName[nPair_]/I");
  DiMuonTree_->Branch("Muon1_phi", &Muon1_phi,"Muon1_phi[nPair_]/D");
  DiMuonTree_->Branch("Muon1_eta", &Muon1_eta,"Muon1_eta[nPair_]/D");
  DiMuonTree_->Branch("Muon1_pT", &Muon1_pT,"Muon1_pT[nPair_]/D");
  DiMuonTree_->Branch("Muon1_Px", &Muon1_Px,"Muon1_Px[nPair_]/D");
  DiMuonTree_->Branch("Muon1_Py", &Muon1_Py,"Muon1_Py[nPair_]/D");
  DiMuonTree_->Branch("Muon1_Pz", &Muon1_Pz,"Muon1_Pz[nPair_]/D");
  //DiMuonTree_->Branch("Muon1_CosmicCompat", &Muon1_CosmicCompat,"Muon1_CosmicCompat[nPair_]/F");
  DiMuonTree_->Branch("Muon1_sumtrkpt", &Muon1_sumtrkpt,"Muon1_sumtrkpt[nPair_]/D");
  DiMuonTree_->Branch("Muon1_trkiso", &Muon1_trkiso,"Muon1_trkiso[nPair_]/D");
  DiMuonTree_->Branch("Muon1_hcaliso", &Muon1_hcaliso,"Muon1_hcaliso[nPair_]/D");
  DiMuonTree_->Branch("Muon1_ecaliso", &Muon1_ecaliso,"Muon1_ecaliso[nPair_]/D");
  //pf iso
  DiMuonTree_->Branch("Muon1_PfChargedHadronIsoR03", &Muon1_PfChargedHadronIsoR03,"Muon1_PfChargedHadronIsoR03[nPair_]/D");
  DiMuonTree_->Branch("Muon1_PfNeutralHadronIsoR03", &Muon1_PfNeutralHadronIsoR03,"Muon1_PfNeutralHadronIsoR03[nPair_]/D");
  DiMuonTree_->Branch("Muon1_PfGammaIsoR03", &Muon1_PfGammaIsoR03,"Muon1_PfGammaIsoR03[nPair_]/D");

  DiMuonTree_->Branch("Muon1_charge", &Muon1_charge,"Muon1_charge[nPair_]/I");
  DiMuonTree_->Branch("Muon1_nChambers", &Muon1_nChambers,"Muon1_nChambers[nPair_]/I");
  DiMuonTree_->Branch("Muon1_nTrackerLayers", &Muon1_nTrackerLayers,"Muon1_nTrackerLayers[nPair_]/I");
  DiMuonTree_->Branch("Muon1_nMatches", &Muon1_nMatches,"Muon1_nMatches[nPair_]/I");
  DiMuonTree_->Branch("Muon1_stationMask", &Muon1_stationMask,"Muon1_stationMask[nPair_]/I");
  DiMuonTree_->Branch("Muon1_nSegments", &Muon1_nSegments,"Muon1_nSegments[nPair_]/I");
  DiMuonTree_->Branch("Muon1_chi2dof", &Muon1_chi2dof,"Muon1_chi2dof[nPair_]/D");
  DiMuonTree_->Branch("Muon1_nhits", &Muon1_nhits,"Muon1_nhits[nPair_]/I");
  DiMuonTree_->Branch("Muon1_trackerHits", &Muon1_trackerHits,"Muon1_trackerHits[nPair_]/I");
  DiMuonTree_->Branch("Muon1_pixelHits", &Muon1_pixelHits,"Muon1_pixelHits[nPair_]/I");
  DiMuonTree_->Branch("Muon1_muonHits", &Muon1_muonHits,"Muon1_muonHits[nPair_]/I");
  DiMuonTree_->Branch("Muon1_qoverp", &Muon1_qoverp,"Muon1_qoverp[nPair_]/D");
  DiMuonTree_->Branch("Muon1_theta", &Muon1_theta,"Muon1_theta[nPair_]/D");
  DiMuonTree_->Branch("Muon1_lambda", &Muon1_lambda,"Muon1_lambda[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dxy", &Muon1_dxy,"Muon1_dxy[nPair_]/D");
  DiMuonTree_->Branch("Muon1_d0", &Muon1_d0,"Muon1_d0[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dsz", &Muon1_dsz,"Muon1_dsz[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dz", &Muon1_dz,"Muon1_dz[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dxyBS", &Muon1_dxyBS,"Muon1_dxyBS[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dszBS", &Muon1_dszBS,"Muon1_dszBS[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dzBS", &Muon1_dzBS,"Muon1_dzBS[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dxyVTX", &Muon1_dxyVTX,"Muon1_dxyVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dszVTX", &Muon1_dszVTX,"Muon1_dszVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dzVTX", &Muon1_dzVTX,"Muon1_dzVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dxyDiMuVTX", &Muon1_dxyDiMuVTX,"Muon1_dxyDiMuVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dszDiMuVTX", &Muon1_dszDiMuVTX,"Muon1_dszDiMuVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon1_dzDiMuVTX", &Muon1_dzDiMuVTX,"Muon1_dzDiMuVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon1_vx", &Muon1_vx,"Muon1_vx[nPair_]/D");
  DiMuonTree_->Branch("Muon1_vy", &Muon1_vy,"Muon1_vy[nPair_]/D");
  DiMuonTree_->Branch("Muon1_vz", &Muon1_vz,"Muon1_vz[nPair_]/D");
/*  DiMuonTree_->Branch("Muon1_MCtruth_pT", &Muon1_MCtruth_pT,"Muon1_MCtruth_pT[nPair_]/D");
  DiMuonTree_->Branch("Muon1_MCtruth_Px", &Muon1_MCtruth_Px,"Muon1_MCtruth_Px[nPair_]/D");
  DiMuonTree_->Branch("Muon1_MCtruth_Py", &Muon1_MCtruth_Py,"Muon1_MCtruth_Py[nPair_]/D");
  DiMuonTree_->Branch("Muon1_MCtruth_Pz", &Muon1_MCtruth_Pz,"Muon1_MCtruth_Pz[nPair_]/D");
  DiMuonTree_->Branch("Muon1_MCtruth_eta", &Muon1_MCtruth_eta,"Muon1_MCtruth_eta[nPair_]/D");
  DiMuonTree_->Branch("Muon1_MCtruth_phi", &Muon1_MCtruth_phi,"Muon1_MCtruth_phi[nPair_]/D");
  DiMuonTree_->Branch("Muon1_MCtruth_charge", &Muon1_MCtruth_charge,"Muon1_MCtruth_charge[nPair_]/I");
  DiMuonTree_->Branch("Muon1_MCtruth_mother", &Muon1_MCtruth_mother,"Muon1_MCtruth_mother[nPair_]/I");
  DiMuonTree_->Branch("Muon1_MCtruth_pdgId", &Muon1_MCtruth_pdgId,"Muon1_MCtruth_pdgId[nPair_]/I");
*/  DiMuonTree_->Branch("Muon1_timeInsideOut", &Muon1_timeInsideOut,"Muon1_timeInsideOut[nPair_]/D");
  DiMuonTree_->Branch("Muon1_timeOutsideIn", &Muon1_timeOutsideIn,"Muon1_timeOutsideIn[nPair_]/D");
  DiMuonTree_->Branch("Muon1_caloCompatibility", &Muon1_caloCompatibility,"Muon1_caloCompatibility[nPair_]/D");
  DiMuonTree_->Branch("Muon1_segCompatibility", &Muon1_segCompatibility,"Muon1_segCompatibility[nPair_]/D");
  DiMuonTree_->Branch("Muon1_CaloEM", &Muon1_CaloEM,"Muon1_CaloEM[nPair_]/D");
  DiMuonTree_->Branch("Muon1_CaloHad", &Muon1_CaloHad,"Muon1_CaloHad[nPair_]/D");
  DiMuonTree_->Branch("Muon1_ID", &Muon1_ID,"Muon1_ID[nPair_]/D");

  DiMuonTree_->Branch("Muon2_muonType", &Muon2_muonType,"Muon2_muonType[nPair_]/I");
  DiMuonTree_->Branch("Muon2_nTrig", &Muon2_nTrig,"Muon2_nTrig[nPair_]/I");
  DiMuonTree_->Branch("Muon2_triggerObjectType", &Muon2_triggerObjectType,"Muon2_triggerObjectType[nPair_]/I");
  DiMuonTree_->Branch("Muon2_filterName", &Muon2_filterName,"Muon2_filterName[nPair_]/I");
  DiMuonTree_->Branch("Muon2_phi", &Muon2_phi,"Muon2_phi[nPair_]/D");
  DiMuonTree_->Branch("Muon2_eta", &Muon2_eta,"Muon2_eta[nPair_]/D");
  DiMuonTree_->Branch("Muon2_pT", &Muon2_pT,"Muon2_pT[nPair_]/D");
  DiMuonTree_->Branch("Muon2_Px", &Muon2_Px,"Muon2_Px[nPair_]/D");
  DiMuonTree_->Branch("Muon2_Py", &Muon2_Py,"Muon2_Py[nPair_]/D");
  DiMuonTree_->Branch("Muon2_Pz", &Muon2_Pz,"Muon2_Pz[nPair_]/D");
  DiMuonTree_->Branch("Muon2_CosmicCompat", &Muon2_CosmicCompat,"Muon2_CosmicCompat[nPair_]/F");
  DiMuonTree_->Branch("Muon2_sumtrkpt", &Muon2_sumtrkpt,"Muon2_sumtrkpt[nPair_]/D");
  DiMuonTree_->Branch("Muon2_trkiso", &Muon2_trkiso,"Muon2_trkiso[nPair_]/D");
  DiMuonTree_->Branch("Muon2_hcaliso", &Muon2_hcaliso,"Muon2_hcaliso[nPair_]/D");
  DiMuonTree_->Branch("Muon2_ecaliso", &Muon2_ecaliso,"Muon2_ecaliso[nPair_]/D");
  //pf iso
  DiMuonTree_->Branch("Muon2_PfChargedHadronIsoR03", &Muon2_PfChargedHadronIsoR03,"Muon2_PfChargedHadronIsoR03[nPair_]/D");
  DiMuonTree_->Branch("Muon2_PfNeutralHadronIsoR03", &Muon2_PfNeutralHadronIsoR03,"Muon2_PfNeutralHadronIsoR03[nPair_]/D");
  DiMuonTree_->Branch("Muon2_PfGammaIsoR03", &Muon2_PfGammaIsoR03,"Muon2_PfGammaIsoR03[nPair_]/D");

  DiMuonTree_->Branch("Muon2_charge", &Muon2_charge,"Muon2_charge[nPair_]/I");
  DiMuonTree_->Branch("Muon2_nChambers", &Muon2_nChambers,"Muon2_nChambers[nPair_]/I");
  DiMuonTree_->Branch("Muon2_nTrackerLayers", &Muon2_nTrackerLayers,"Muon2_nTrackerLayers[nPair_]/I");
  DiMuonTree_->Branch("Muon2_nMatches", &Muon2_nMatches,"Muon2_nMatches[nPair_]/I");
  DiMuonTree_->Branch("Muon2_stationMask", &Muon2_stationMask,"Muon2_stationMask[nPair_]/I");
  DiMuonTree_->Branch("Muon2_nSegments", &Muon2_nSegments,"Muon2_nSegments[nPair_]/I");
  DiMuonTree_->Branch("Muon2_chi2dof", &Muon2_chi2dof,"Muon2_chi2dof[nPair_]/D");
  DiMuonTree_->Branch("Muon2_nhits", &Muon2_nhits,"Muon2_nhits[nPair_]/I");
  DiMuonTree_->Branch("Muon2_trackerHits", &Muon2_trackerHits,"Muon2_trackerHits[nPair_]/I");
  DiMuonTree_->Branch("Muon2_pixelHits", &Muon2_pixelHits,"Muon2_pixelHits[nPair_]/I");
  DiMuonTree_->Branch("Muon2_muonHits", &Muon2_muonHits,"Muon2_muonHits[nPair_]/I");
  DiMuonTree_->Branch("Muon2_qoverp", &Muon2_qoverp,"Muon2_qoverp[nPair_]/D");
  DiMuonTree_->Branch("Muon2_theta", &Muon2_theta,"Muon2_theta[nPair_]/D");
  DiMuonTree_->Branch("Muon2_lambda", &Muon2_lambda,"Muon2_lambda[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dxy", &Muon2_dxy,"Muon2_dxy[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dxyBS", &Muon2_dxyBS,"Muon2_dxyBS[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dszBS", &Muon2_dszBS,"Muon2_dszBS[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dzBS", &Muon2_dzBS,"Muon2_dzBS[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dxyVTX", &Muon2_dxyVTX,"Muon2_dxyVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dszVTX", &Muon2_dszVTX,"Muon2_dszVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dzVTX", &Muon2_dzVTX,"Muon2_dzVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dxyDiMuVTX", &Muon2_dxyDiMuVTX,"Muon2_dxyDiMuVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dszDiMuVTX", &Muon2_dszDiMuVTX,"Muon2_dszDiMuVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dzDiMuVTX", &Muon2_dzDiMuVTX,"Muon2_dzDiMuVTX[nPair_]/D");
  DiMuonTree_->Branch("Muon2_d0", &Muon2_d0,"Muon2_d0[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dsz", &Muon2_dsz,"Muon2_dsz[nPair_]/D");
  DiMuonTree_->Branch("Muon2_dz", &Muon2_dz,"Muon2_dz[nPair_]/D");
  DiMuonTree_->Branch("Muon2_vx", &Muon2_vx,"Muon2_vx[nPair_]/D");
  DiMuonTree_->Branch("Muon2_vy", &Muon2_vy,"Muon2_vy[nPair_]/D");
  DiMuonTree_->Branch("Muon2_vz", &Muon2_vz,"Muon2_vz[nPair_]/D");
/*  DiMuonTree_->Branch("Muon2_MCtruth_pT", &Muon2_MCtruth_pT,"Muon2_MCtruth_pT[nPair_]/D");
  DiMuonTree_->Branch("Muon2_MCtruth_Px", &Muon2_MCtruth_Px,"Muon2_MCtruth_Px[nPair_]/D");
  DiMuonTree_->Branch("Muon2_MCtruth_Py", &Muon2_MCtruth_Py,"Muon2_MCtruth_Py[nPair_]/D");
  DiMuonTree_->Branch("Muon2_MCtruth_Pz", &Muon2_MCtruth_Pz,"Muon2_MCtruth_Pz[nPair_]/D");
  DiMuonTree_->Branch("Muon2_MCtruth_eta", &Muon2_MCtruth_eta,"Muon2_MCtruth_eta[nPair_]/D");
  DiMuonTree_->Branch("Muon2_MCtruth_phi", &Muon2_MCtruth_phi,"Muon2_MCtruth_phi[nPair_]/D");
  DiMuonTree_->Branch("Muon2_MCtruth_charge", &Muon2_MCtruth_charge,"Muon2_MCtruth_charge[nPair_]/I");
  DiMuonTree_->Branch("Muon2_MCtruth_mother", &Muon2_MCtruth_mother,"Muon2_MCtruth_mother[nPair_]/I");
  DiMuonTree_->Branch("Muon2_MCtruth_pdgId", &Muon2_MCtruth_pdgId,"Muon2_MCtruth_pdgId[nPair_]/I");
*/  DiMuonTree_->Branch("Muon2_timeInsideOut", &Muon2_timeInsideOut,"Muon2_timeInsideOut[nPair_]/D");
  DiMuonTree_->Branch("Muon2_timeOutsideIn", &Muon2_timeOutsideIn,"Muon2_timeOutsideIn[nPair_]/D");
  DiMuonTree_->Branch("Muon2_caloCompatibility", &Muon2_caloCompatibility,"Muon2_caloCompatibility[nPair_]/D");
  DiMuonTree_->Branch("Muon2_segCompatibility", &Muon2_segCompatibility,"Muon2_segCompatibility[nPair_]/D");
  DiMuonTree_->Branch("Muon2_CaloEM", &Muon2_CaloEM,"Muon2_CaloEM[nPair_]/D");
  DiMuonTree_->Branch("Muon2_CaloHad", &Muon2_CaloHad,"Muon2_CaloHad[nPair_]/D");
  DiMuonTree_->Branch("Muon2_ID", &Muon2_ID,"Muon2_ID[nPair_]/D");

  // Gen Muon
  // global event varialbes (for GEN)
  DiMuonTree_->Branch("GENnPair",&GENnPair_,"GENnPair_/I");
  // muon pair variables
  DiMuonTree_->Branch("GENInvMass", &GENInvMass,"GENInvMass[GENnPair_]/D");
  DiMuonTree_->Branch("GENRapidity", &GENRapidity,"GENRapidity[GENnPair_]/D");
  DiMuonTree_->Branch("GENPt", &GENPt,"GENPt[GENnPair_]/D");
  DiMuonTree_->Branch("GENAngle", &GENAngle,"GENAngle[GENnPair_]/D");
  DiMuonTree_->Branch("GENisOppSign", &GENisOppSign,"GENisOppSign[GENnPair_]/B");
  // object variables
  DiMuonTree_->Branch("GENMuon1_phi", &GENMuon1_phi,"GENMuon1_phi[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon1_eta", &GENMuon1_eta,"GENMuon1_eta[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon1_pT", &GENMuon1_pT,"GENMuon1_pT[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon1_Px", &GENMuon1_Px,"GENMuon1_Px[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon1_Py", &GENMuon1_Py,"GENMuon1_Py[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon1_Pz", &GENMuon1_Pz,"GENMuon1_Pz[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon1_mother", &GENMuon1_mother,"GENMuon1_mother[GENnPair_]/I");
  DiMuonTree_->Branch("GENMuon1_charge", &GENMuon1_charge,"GENMuon1_charge[GENnPair_]/I");
  DiMuonTree_->Branch("GENMuon1_status", &GENMuon1_status,"GENMuon1_status[GENnPair_]/I");
  DiMuonTree_->Branch("GENMuon2_phi", &GENMuon2_phi,"GENMuon2_phi[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon2_eta", &GENMuon2_eta,"GENMuon2_eta[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon2_pT", &GENMuon2_pT,"GENMuon2_pT[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon2_Px", &GENMuon2_Px,"GENMuon2_Px[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon2_Py", &GENMuon2_Py,"GENMuon2_Py[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon2_Pz", &GENMuon2_Pz,"GENMuon2_Pz[GENnPair_]/D");
  DiMuonTree_->Branch("GENMuon2_mother", &GENMuon2_mother,"GENMuon2_mother[GENnPair_]/I");
  DiMuonTree_->Branch("GENMuon2_charge", &GENMuon2_charge,"GENMuon2_charge[GENnPair_]/I");
  DiMuonTree_->Branch("GENMuon2_status", &GENMuon2_status,"GENMuon2_status[GENnPair_]/I");

  // Pile-up Reweight
  DiMuonTree_->Branch("nPileUp",&nPileUp_,"nPileUp_/I");
  DiMuonTree_->Branch("nPileUpIn",&nPileUpIn_,"nPileUpIn_/I");
  DiMuonTree_->Branch("pileUpReweightIn",&pileUpReweightIn_,"pileUpReweightIn_/D");
  DiMuonTree_->Branch("pileUpReweightPlusIn",&pileUpReweightPlusIn_,"pileUpReweightPlusIn_/D");
  DiMuonTree_->Branch("pileUpReweightMinusIn",&pileUpReweightMinusIn_,"pileUpReweightMinusIn_/D");
  DiMuonTree_->Branch("pileUpReweight",&pileUpReweight_,"pileUpReweight_/D");
  DiMuonTree_->Branch("pileUpReweightPlus",&pileUpReweightPlus_,"pileUpReweightPlus_/D");
  DiMuonTree_->Branch("pileUpReweightMinus",&pileUpReweightMinus_,"pileUpReweightMinus_/D");
  DiMuonTree_->Branch("PUweight",&PUweight_,"PUweight_/D");

}


void
DiMuonIntoNtuples::beginRun(const Run & iRun, const EventSetup & iSetup)
{

   //Preselect list of triggers that we are interested in
   string trigs[nTrigName_] = {
     "HLT_Mu22_Photon22_CaloIdL_v*",
     "HLT_Mu17_Mu8_v*",
     "HLT_Mu17_TkMu8_v*",
     "HLT_Mu13_Mu8_v*",
     "HLT_Mu22_TkMu8_v*",
     "HLT_Mu22_TkMu22_v*"
   };

  //Push back into trigger list
   for( int i = 0; i < nTrigName_; i++ ) {
     HLT_trigName_.push_back(trigs[i]);
     //listUnavail_[i] = -1;
   }

  bool changedConfig;
  if (!hltConfig_.init(iRun, iSetup, processName_, changedConfig)) {
    LogError("HLTMuonVal") << "Initialization of HLTConfigProvider failed!!";
    return;
  } else {
    //get trigger names in  the trigger menu
    std::vector<std::string> triggerNames = hltConfig_.triggerNames();
    //loop over preselected trigger names
    for( int itrigName = 0; itrigName < nTrigName_; itrigName++ ) {
      //listUnavail_[itrigName] = 0;

      //bool isMatched = false;
      //and check them agains 
      //what is in the trigger menu 
      for( size_t i = 0; i < triggerNames.size(); i++) {
        std::vector<std::string> moduleNames = hltConfig_.moduleLabels( triggerNames[i] );
        //match using regex, we want to be sure a given trigger name is still in the trigger list
        //store unavailable triggers to remove them on the next step
        std::vector<std::vector<std::string>::const_iterator> matches = edm::regexMatch(triggerNames, HLT_trigName_[itrigName]);
        if( !matches.empty() ) {
          BOOST_FOREACH(std::vector<std::string>::const_iterator match, matches) {
            if( *match == triggerNames[i] ) {
              //isMatched = true;
              int nsize = moduleNames.size();
              if( nsize-2 >= 0 ) {
                //cout << "module names = " << moduleNames[nsize-2] << " " << moduleNames[nsize-3] << endl;
                trigModuleNames_.push_back(moduleNames[nsize-2]);
                //cout << "Filter name: " << trigModuleNames_[trigModuleNames_.size()-1] << endl;
                if( nsize-3 >= 0 ) {
                  trigModuleNames_preFil_.push_back(moduleNames[nsize-3]);
                }
                else {
                  trigModuleNames_preFil_.push_back("");
                }
              }
              break;
            }
          }
        }
      }
     // if( !isMatched ) listUnavail_[itrigName] = 1;
    }
  }

  // remove unavailable triggers
  //int itmp = 0;
  //for( vector<string>::iterator iter = MuonHLT_.begin(); iter != MuonHLT_.end(); ) {
  //  if( listUnavail_[itmp] > 0 )
  //    iter = MuonHLT_.erase(iter);
  //  else
  //    ++iter;
  //  itmp++;
  //}
  //re-adjust the size of the array that we store
  //as we might have removed some trigger names
  //ntrigName = MuonHLT_.size();
}


// ------------ method called once each job just after ending the event loop  ------------
void 
DiMuonIntoNtuples::endJob() {

  std::cout <<"++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout <<"analyzed " << nEvt_ << " events: " << std::endl;
  std::cout <<"++++++++++++++++++++++++++++++++++++++" << std::endl;

}

int DiMuonIntoNtuples::motherId(const reco::GenParticle& par) const
{
    int motherid = -9999;
    const reco::Candidate* motherCand = par.mother(0);

    if( !motherCand ) return -9999;
    while( motherCand ) {
	if(  fabs(motherCand->pdgId()) == 2212 ) break;
	if(  fabs(motherCand->pdgId()) < 7 ) break;
	motherid = motherCand->pdgId();
	motherCand = motherCand->mother(0);
	if( fabs(motherid) == 23 || fabs(motherid) == 22 ) return motherid;
    }
    return motherid;
}


int DiMuonIntoNtuples::motherId(const reco::GenParticleRef par) const
{
    int motherid = -9999;
    /*
    if( par->numberOfMothers() > 0 ) {
	reco::GenParticleRef motherCand = par->motherRef();
	while( motherCand.isNonnull() ) {
	    _motherid = motherCand->pdgId();
	    if( fabs(_motherid) == 13 ) {
	      motherCand = motherCand->motherRef();
	    }
	    else break;
	  //if( fabs(_motherid) == 23 || fabs(_motherid) == 22 ) return motherid;
	}
    }
    */
    return motherid;
}


//////////////////////////////////////////////////////////////////////////////
// calculates 3D angle between two muon momentum vectors using cosine theorem
//////////////////////////////////////////////////////////////////////////////
template <class T>
float DiMuonIntoNtuples::angleBetween(const T & lhs, const T & rhs) const {

   GlobalVector mom1(lhs.px(), lhs.py(), lhs.pz());
   GlobalVector mom2(rhs.px(), rhs.py(), rhs.pz());

   GlobalVector dmom = mom1 - mom2;
   return acos( ( mom1.mag() * mom1.mag() + mom2.mag() * mom2.mag() - dmom.mag() * dmom.mag() ) / (2*mom1.mag()*mom2.mag() ));

}


////////////////////////////////////////////////////////////////////////////////
// checks if there were at least two PAT muons in an event, returns false if no
////////////////////////////////////////////////////////////////////////////////
bool DiMuonIntoNtuples::muonCountFilter(const edm::Handle<edm::View<pat::Muon> >& muonHandle){

   bool hasAtLeast2Mu = true;
   Nmuons_ = muonHandle->size();
   if (Nmuons_ < 2) hasAtLeast2Mu = false;
   return hasAtLeast2Mu;

}

///////////////////////////////////////////////////////////////
// makes hlt report and fills it to the ntuple
///////////////////////////////////////////////////////////////
void DiMuonIntoNtuples::hltReport(const edm::Event &iEvent) {

   //array to store the trigger decision for a given trigger name 
   bool trigFired[nTrigName_];
   for (int i = 0; i < nTrigName_; i++) trigFired[i] = false; 

   //Read the trigger information in an event
   Handle<TriggerResults> trigResult;
   iEvent.getByLabel(edm::InputTag("TriggerResults","",processName_), trigResult);
   if( !trigResult.failedToGet() ) {
       int ntrigs = trigResult->size();
       const edm::TriggerNames trigName = iEvent.triggerNames(*trigResult);
       for( int itrig = 0; itrig != ntrigs; ++itrig) {
           //cout << "trigName = " << trigName.triggerName(itrig) << " " << itrig << endl;
           for( int itrigName = 0; itrigName < nTrigName_; itrigName++ ) {
             //match using regex, we want to be sure a given trigger name is still in the trigger list
             //store unavailable triggers to remove them on the next step
             std::vector<std::vector<std::string>::const_iterator> matches = edm::regexMatch(trigName.triggerNames(), HLT_trigName_[itrigName]);
             if( !matches.empty() ) {
               BOOST_FOREACH(std::vector<std::string>::const_iterator match, matches) {
                 if( trigName.triggerIndex(*match) >= (unsigned int)ntrigs ) continue;
                 //checked weather it was fired in ana event
                 if( trigResult->accept(trigName.triggerIndex(*match)) ) trigFired[itrigName] = true;
               }
            }
         }
      }
   }

   //for (int i = 0; i < nTrigName_; i++) cout << trigFired[i] << endl;

   //below is to store the trigger object kinematics for trigger matching purposes 
   edm::Handle< trigger::TriggerEvent > triggerObject;
   iEvent.getByLabel(edm::InputTag("hltTriggerSummaryAOD","",processName_), triggerObject);
   const trigger::TriggerObjectCollection & toc(triggerObject->getObjects());
   int ntrigTot = 0;
   for( size_t k = 0; k < triggerObject->sizeFilters(); ++ k ) {
       std::string fullname = triggerObject->filterTag(k).encode();
       std::string filterName;
       size_t m = fullname.find_first_of(':');
       if( m != std::string::npos ) {
         filterName = fullname.substr(0, m);
       }
       else {
         filterName = fullname;
       }

       if( &toc != 0 ) {
         const trigger::Keys & it = triggerObject->filterKeys(k);
         for( trigger::Keys::const_iterator ky = it.begin(); ky !=it.end(); ++ky ) {
           double hlt_pt = toc[*ky].pt();
           //double hlt_eta = toc[*ky].eta();

           for( int itf = 0; itf < nTrigName_; itf++ ) {
               if( filterName == trigModuleNames_[itf]) { 
                 //HLT_trigName_.push_back(names);
                 HLT_trigType[ntrigTot] = itf;
                 HLT_trigFired[ntrigTot] = trigFired[itf];

                 HLT_trigPt[ntrigTot] = hlt_pt;
                 HLT_trigEta[ntrigTot] = toc[*ky].eta();
                 HLT_trigPhi[ntrigTot] = toc[*ky].phi();
                 //cout << "Debug printout for event: " << iEvent.id() << endl;
                 //cout << "ntrigTot: " << ntrigTot << endl; //" " << HLT_trigName_.at(ntrigTot) << endl;
                 //cout << "HLT_trigType[ntrigTot]: " << HLT_trigType[ntrigTot] << endl;
                 //cout << "HLT_trigFired[ntrigTot]: " << HLT_trigFired[ntrigTot]<< endl;
                 //cout << "HLT_trigPt[ntrigTot]: " << HLT_trigPt[ntrigTot]<< endl;
                 //cout << "HLT_trigEta[ntrigTot]: " << HLT_trigEta[ntrigTot]<< endl;
                 //cout << "HLT_trigPhi[ntrigTot] : " << HLT_trigPhi[ntrigTot] << endl;
                 ntrigTot++;
               }
            }
         }
      }
   }
   HLT_ntrig_ = ntrigTot;
}


//////////////////////////////////////////////////////////////
// Get Primary vertex info
//////////////////////////////////////////////////////////////
void DiMuonIntoNtuples::fillPrimaryVertex(const edm::Event &iEvent) {

   edm::Handle<reco::VertexCollection> pvHandle;
   iEvent.getByLabel("offlinePrimaryVertices", pvHandle);
   const reco::VertexCollection vtx = *(pvHandle.product());

   if( vtx.size() > 2 && theDebugLevel_ > 0) cout << "Reconstructed "<< vtx.size() << " vertices" << endl;
   if (vtx.size() > 0 ){
       PVtrackSize_ = vtx.front().tracksSize();
       PVchi2_ = vtx.front().chi2();
       PVndof_ = vtx.front().ndof();
       PVnormalizedChi2_ = vtx.front().normalizedChi2();
       PVx_ = vtx.front().x();
       PVy_ = vtx.front().y();
       PVz_ = vtx.front().z();
       PVprob_ = TMath::Prob(PVchi2_,(int)PVndof_) ;
   }
}


//////////////////////////////////////////////////////////////
// Get METs info
//////////////////////////////////////////////////////////////
void DiMuonIntoNtuples::fillMET(const edm::Event &iEvent) {

   edm::Handle<pat::METCollection> metHandle;
   iEvent.getByLabel(theMETLabel_,metHandle);

   pat::METCollection::const_iterator iMET = metHandle->begin();
   MET_ = iMET->pt();

}

//////////////////////////////////////////////////////////////
// Get Jets info
//////////////////////////////////////////////////////////////
void DiMuonIntoNtuples::fillJet(const edm::Event &iEvent) {

   edm::Handle<edm::View<pat::Jet> > jetHandle;
   iEvent.getByLabel(theJetLabel_,jetHandle);

   edm::Handle<edm::View<pat::Muon> > muonHandle;
   iEvent.getByLabel(theMuonLabel_,muonHandle);

   if( jetHandle->size() > 0 && theDebugLevel_ > 0) cout << "# of Jets = " << jetHandle->size() << endl;
   //Njets_ = jetHandle->size();
   if (jetHandle->size() == 0) return; 

   for(edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin(); iJet != jetHandle->end(); ++iJet) {
	//track counting high efficiency algorithm
        JETbDiscriminant_alg1[Njets_] = iJet->bDiscriminator("trackCountingHighEffBJetTags");
	JETflavour[Njets_] = iJet->partonFlavour();
	JETcharge[Njets_] = iJet->jetCharge();
	JETntracks[Njets_] = iJet->associatedTracks().size();
	JETpt[Njets_] = iJet->pt();
	JETeta[Njets_] = iJet->eta();
	JETphi[Njets_] = iJet->phi();
	//track counting high purity algorithm
        JETbDiscriminant_alg2[Njets_] = iJet->bDiscriminator("trackCountingHighPurBJetTags");
	//simple secondary vertex algorithm
        JETbDiscriminant_alg3[Njets_] = iJet->bDiscriminator("simpleSecondaryVertexBJetTags");
	Njets_++;
   }

}

//ASV electron methods
double DiMuonIntoNtuples::passesVertexCuts(const edm::EventSetup& eventSetup, const pat::Muon& mu0, const pat::Electron& e1) const {

  double vertexComp = -1.;

  // dimuon vertex
  if( mu0.innerTrack().isNonnull() && e1.gsfTrack().isNonnull()) {

    ESHandle<MagneticField> B;
    eventSetup.get<IdealMagneticFieldRecord>().get(B);

    ESHandle<TransientTrackBuilder> trackBuilder;
    eventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

    reco::GsfTrackRef etrack = e1.gsfTrack();
    reco::TransientTrack electron2Transient = trackBuilder->build(etrack);	

    reco::TransientTrack muon1Transient( mu0.innerTrack(), B.product());

    vector<reco::TransientTrack> dimuonTracks;
    dimuonTracks.push_back(muon1Transient);
    dimuonTracks.push_back(electron2Transient);
    KalmanVertexFitter KalmanFitter;
    TransientVertex vertex;
    bool isVertex = true;
    try {
      vertex = KalmanFitter.vertex(dimuonTracks);
    }	
    catch( exception & err ) {
      isVertex = false;
    }
    if( isVertex ) {
      reco::Vertex vtx = vertex;
      vertexComp  = TMath::Prob(vtx.chi2(),(int)vtx.ndof());
    }
  }

  return vertexComp;

}

//define this as a plug-in
DEFINE_FWK_MODULE(DiMuonIntoNtuples);
