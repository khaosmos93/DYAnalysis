//-------------------------------------------------
//
//   Class: NtupleMaker
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
#include "DiLeptonIntoNtuples.h"

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
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
//data formats
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
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
//Data formats extra
#include "DataFormats/Purdue/interface/Muon.h"
#include "DataFormats/Purdue/interface/Electron.h"
#include "DataFormats/Purdue/interface/Dimuon.h"
#include "DataFormats/Purdue/interface/Dielectron.h"
#include "DataFormats/Purdue/interface/Photon.h"
#include "DataFormats/Purdue/interface/Jet.h"
//pileup
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/InvariantMassFromVertex.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "PhysicsTools/RecoUtils/interface/CandCommonVertexFitter.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <TTree.h>
#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TVector3.h>

//
// class decleration
//

using namespace std;
using namespace edm;
using namespace pat;
using namespace reco;

//
DiLeptonIntoNtuples::DiLeptonIntoNtuples(const edm::ParameterSet& iConfig)
{
  processName_ 			     = iConfig.getUntrackedParameter<string>("processName", "HLT");
  theDebugLevel_                     = iConfig.getUntrackedParameter<int>("DebugLevel", 0);

  theMuonLabel_                      = iConfig.getUntrackedParameter<edm::InputTag>("Muon", edm::InputTag("selectedPatMuonsPFlow"));
  theMETLabel_                       = iConfig.getUntrackedParameter<edm::InputTag>("MET", edm::InputTag("patMETs"));
  theJetLabel_                       = iConfig.getUntrackedParameter<edm::InputTag>("Jet", edm::InputTag("selectedPatJets"));
  theElectronLabel_                  = iConfig.getUntrackedParameter<edm::InputTag>("Electron", edm::InputTag("selectedPatElectrons"));
  thePhotonLabel_                    = iConfig.getUntrackedParameter<edm::InputTag>("Photon", edm::InputTag("selectedPatPhotons"));
  theBeamSpot_                       = iConfig.getUntrackedParameter<edm::InputTag>("BeamSpot", edm::InputTag("offlineBeamSpot"));
  theTrackLabel_                     = iConfig.getUntrackedParameter<edm::InputTag>("Track", edm::InputTag("generalTracks"));

  theStorePriVtx2MuFlag_             =  iConfig.getUntrackedParameter<bool>("StorePriVtx2MuFlag", true);

  runOnEleInput_                     =  iConfig.getUntrackedParameter<bool>("runOnEleInput");
  runOnMuInput_                      =  iConfig.getUntrackedParameter<bool>("runOnMuInput");

  isSignal_                          =  iConfig.getUntrackedParameter<bool>("isSignal");
  isMC_                              =  iConfig.getUntrackedParameter<bool>("isMC");
  if( isMC_ ) {                      
    PileUpRD_                        = iConfig.getParameter< std::vector<double> >("PileUpRD");
    PileUpMC_                        = iConfig.getParameter< std::vector<double> >("PileUpMC");
  }
  isAOD_                             =  iConfig.getUntrackedParameter<bool>("isAOD", true);
  metLabel_                          =  iConfig.getUntrackedParameter<edm::InputTag>("metLabel");

  isoValInputTags_                   = iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags");
  theRhoIsoInputTag_                 = iConfig.getUntrackedParameter<edm::InputTag>("rhoIsoInputTag", edm::InputTag("kt6PFJetsForIsolation","rho"));
  //containers
  singleMuons = new std::vector<purdue::Muon>();
  singleElectrons = new std::vector<purdue::Electron>();
  diMuons = new std::vector<purdue::Dimuon>();
  diElectrons = new std::vector<purdue::Dielectron>();
  singlePhotons = new std::vector<purdue::Photon>();
  singleJets = new std::vector<purdue::Jet>();
}


DiLeptonIntoNtuples::~DiLeptonIntoNtuples()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void DiLeptonIntoNtuples::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //containers
  singleMuons->clear();
  singleElectrons->clear();
  diMuons->clear();
  diElectrons->clear();  
  singlePhotons->clear();
  singleJets->clear();

  // incremented variables
  GENnPair_ = 0; 
  Njets_ = 0;

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

  // rho for isolation
  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(theRhoIsoInputTag_,rhoHandle);
  rhoIso_           = *rhoHandle;

  //
  // Get primary vertices
  // Assumes primary vertices are ordered by sum-pT^2 (as should be in CMSSW)
  // NOTE: if no PV is found from fitting tracks, the beamspot is used
  //
  theVertex_ = 0;
  bool hasGoodPV = false;
  edm::Handle<reco::VertexCollection> pvHandle;
  iEvent.getByLabel("offlinePrimaryVertices", pvHandle);
  for(reco::VertexCollection::const_iterator pv = pvHandle->begin(); pv != pvHandle->end(); ++pv) {
    // Select best PV for corrected d0; if no PV passing cuts, the first PV in the collection will be used
    //if(!pv->IsValid()) continue;
    //if(pv->nTracks()        < 0)              continue;
    if(pv->ndof()	    < 4)	      continue;
    if(fabs(pv->z())	    > 24)	      continue;
    if(pv->position().Rho() > 2)	      continue;    
    hasGoodPV = true;
    
    if(!theVertex_) theVertex_ = &*pv;
  }
  if(!theVertex_) theVertex_ = &(pvHandle->at(0));

  pvx_              = theVertex_->position().x();
  pvy_              = theVertex_->position().y();
  pvz_              = theVertex_->position().z();
  hasGoodPV_        = hasGoodPV;


  if (runOnMuInput_) {

  //muon
  edm::Handle< pat::MuonCollection > muonHandle;
  iEvent.getByLabel(theMuonLabel_, muonHandle);
  Nmuons_ = muonHandle->size();
  //
  // Loop through muons 
  //
  if (!muonHandle.failedToGet()) {
   for (unsigned int i1 = 0; i1 < muonHandle->size(); i1++) {
      const pat::Muon it1 = muonHandle->at(i1);
      fillMuon(&it1,iEvent);
      for (unsigned int i2 = i1+1; i2 < muonHandle->size(); i2++) {
       const pat::Muon it2 = muonHandle->at(i2);  
       fillDimuon(i1,i2,iSetup,iEvent);
     }
    }
   }
  }


  //
  // Loop through electrons.
  //
  // electrons
  if (runOnEleInput_) {  
  edm::Handle<reco::GsfElectronCollection> electronHandle;
  iEvent.getByLabel("calibratedElectrons","calibratedGsfElectrons", electronHandle);
  //iEvent.getByLabel("gsfElectrons",electronHandle);
  
  Nelectrons_ = electronHandle->size();
  if (!electronHandle.failedToGet()) {
   for (unsigned int i1 = 0; i1 < electronHandle->size(); i1++) {
      const reco::GsfElectron it1 = electronHandle->at(i1);
      fillElectron(&it1,iEvent);
      for (unsigned int i2 = i1+1; i2 < electronHandle->size(); i2++) {
       const reco::GsfElectron it2 = electronHandle->at(i2);
       fillDielectron(i1,i2,iEvent);
     }
    }
   }
  }
  //for(pat::PhotonCollection::const_iterator it = photonHandle->begin(); it != photonHandle->end(); ++it) {
  //  fillPhoton(&*it);
  //} 

  //Jet
  //for(pat::JetCollection::const_iterator it = jetHandle->begin(); it != jetHandle->end(); ++it) {
  //  fillJet(&*it,iEvent);
  // }

    // Gen 
  if ( isSignal_) { 

     edm::Handle <reco::GenParticleCollection> particles;
     iEvent.getByLabel("genParticles", particles);

     for ( size_t m1 = 0; m1 < particles->size(); m1++ ) {
       const reco::GenParticle &MClepton1 = (*particles)[m1];
       if ( !(abs(motherId(MClepton1)) == 23 || abs(motherId(MClepton1)) == 22) ) continue;
       if (!( abs(MClepton1.pdgId()) == 11 || abs(MClepton1.pdgId()) == 13)) continue;
       if (!(MClepton1.status() == 1 || MClepton1.status() == 3)) continue;
       for ( size_t m2 = m1+1; m2 < particles->size(); m2++ ) {
         const reco::GenParticle& MClepton2 = (*particles)[m2];

         if (!(MClepton2.status() == 1 || MClepton2.status() == 3)) continue;
         if ( !(abs(motherId(MClepton2)) == 23 || abs(motherId(MClepton2)) == 22) ) continue;
         if ( !(abs(MClepton2.pdgId()) == 11 || abs(MClepton2.pdgId()) == 13)) continue;
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
         ////cout << "gen mass = " << aDYcand.mass() << " " << MClepton1.status() << " " << MClepton2.status() << endl;

         GENAngle[GENnPair_] = angleBetween(MClepton1, MClepton2);

         // muon1 kinematics
         GENLepton1_pT[GENnPair_] = MClepton1.pt();
         GENLepton1_Px[GENnPair_] = MClepton1.px();
         GENLepton1_Py[GENnPair_] = MClepton1.py();
         GENLepton1_Pz[GENnPair_] = MClepton1.pz();
         GENLepton1_eta[GENnPair_] = MClepton1.eta();
         GENLepton1_phi[GENnPair_] = MClepton1.phi();
         GENLepton1_charge[GENnPair_] = MClepton1.charge();
         GENLepton1_status[GENnPair_] = MClepton1.status();
         GENLepton1_pdgID[GENnPair_] = MClepton1.pdgId();

         // muon2 kinematics
         GENLepton2_pT[GENnPair_] = MClepton2.pt();
         GENLepton2_Px[GENnPair_] = MClepton2.px();
         GENLepton2_Py[GENnPair_] = MClepton2.py();
         GENLepton2_Pz[GENnPair_] = MClepton2.pz();
         GENLepton2_eta[GENnPair_] = MClepton2.eta();
         GENLepton2_phi[GENnPair_] = MClepton2.phi();
         GENLepton2_charge[GENnPair_] = MClepton2.charge();
         GENLepton2_status[GENnPair_] = MClepton2.status();
         GENLepton2_pdgID[GENnPair_] = MClepton2.pdgId();

         GENnPair_++;
       }
     }
   }//the store GEN
  fillMET(iEvent);
  hltReport(iEvent);

  DiLeptonTree_->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
DiLeptonIntoNtuples::beginJob()
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
  DiLeptonTree_ = fs->make<TTree>("DiLeptonTree","DiLeptonTree");

  DiLeptonTree_->Branch("runNum",&runNum_,"runNum_/I");
  DiLeptonTree_->Branch("evtNum",&evtNum_,"evtNum_/I");
  DiLeptonTree_->Branch("lumiBlock",&lumiBlock_,"lumiBlock_/I");
  DiLeptonTree_->Branch("Nmuons", &Nmuons_,"Nmuons_/I");
  DiLeptonTree_->Branch("Nelectrons", &Nelectrons_,"Nelectrons_/I");
  DiLeptonTree_->Branch("rhoIso",&rhoIso_,"rhoIso_/D");

  // for trigger matching
  DiLeptonTree_->Branch("HLT_ntrig", &HLT_ntrig_,"HLT_ntrig_/I");
  DiLeptonTree_->Branch("HLT_trigType", &HLT_trigType,"HLT_trigType[HLT_ntrig_]/I");
  DiLeptonTree_->Branch("HLT_trigFired", &HLT_trigFired,"HLT_trigFired[HLT_ntrig_]/B");
  DiLeptonTree_->Branch("HLT_trigName", &HLT_trigName_);
  DiLeptonTree_->Branch("HLT_trigPt", &HLT_trigPt,"HLT_trigPt[HLT_ntrig_]/D");
  DiLeptonTree_->Branch("HLT_trigEta", &HLT_trigEta,"HLT_trigEta[HLT_ntrig_]/D");
  DiLeptonTree_->Branch("HLT_trigPhi", &HLT_trigPhi,"HLT_trigPhi[HLT_ntrig_]/D");

  DiLeptonTree_->Branch("Njets", &Njets_,"Njets_/I");
  //MET
  DiLeptonTree_->Branch("MET", &MET_,"MET_/D");

  DiLeptonTree_->Branch("hasGoodPV", &hasGoodPV_,"hasGoodPV_/B");
  DiLeptonTree_->Branch("pvx",&pvx_,"pvx_/D");
  DiLeptonTree_->Branch("pvy",&pvy_,"pvy_/D");
  DiLeptonTree_->Branch("pvz",&pvz_,"pvz_/D");

  DiLeptonTree_->Branch("Muon","std::vector<purdue::Muon>", &singleMuons);
  DiLeptonTree_->Branch("Electron","std::vector<purdue::Electron>", &singleElectrons);
  DiLeptonTree_->Branch("Dimuon","std::vector<purdue::Dimuon>", &diMuons);
  DiLeptonTree_->Branch("Dielectron","std::vector<purdue::Dielectron>", &diElectrons);
  DiLeptonTree_->Branch("Photon","std::vector<purdue::Photon>", &singlePhotons);
  DiLeptonTree_->Branch("Jet","std::vector<purdue::Jet>", &singleJets);

  // global event varialbes (for GEN)
  DiLeptonTree_->Branch("GENnPair",&GENnPair_,"GENnPair_/I");
  DiLeptonTree_->Branch("GENInvMass", &GENInvMass,"GENInvMass[GENnPair_]/D");
  DiLeptonTree_->Branch("GENRapidity", &GENRapidity,"GENRapidity[GENnPair_]/D");
  DiLeptonTree_->Branch("GENPt", &GENPt,"GENPt[GENnPair_]/D");
  DiLeptonTree_->Branch("GENAngle", &GENAngle,"GENAngle[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_pdgID", &GENLepton1_pdgID,"GENLepton1_pdgID[GENnPair_]/I");
  DiLeptonTree_->Branch("GENLepton2_pdgID", &GENLepton2_pdgID,"GENLepton2_pdgID[GENnPair_]/I");
  DiLeptonTree_->Branch("GENLepton1_phi", &GENLepton1_phi,"GENLepton1_phi[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_eta", &GENLepton1_eta,"GENLepton1_eta[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_pT", &GENLepton1_pT,"GENLepton1_pT[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_Px", &GENLepton1_Px,"GENLepton1_Px[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_Py", &GENLepton1_Py,"GENLepton1_Py[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_Pz", &GENLepton1_Pz,"GENLepton1_Pz[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton1_charge", &GENLepton1_charge,"GENLepton1_charge[GENnPair_]/I");
  DiLeptonTree_->Branch("GENLepton1_status", &GENLepton1_status,"GENLepton1_status[GENnPair_]/I");
  DiLeptonTree_->Branch("GENLepton2_phi", &GENLepton2_phi,"GENLepton2_phi[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton2_eta", &GENLepton2_eta,"GENLepton2_eta[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton2_pT", &GENLepton2_pT,"GENLepton2_pT[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton2_Px", &GENLepton2_Px,"GENLepton2_Px[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton2_Py", &GENLepton2_Py,"GENLepton2_Py[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton2_Pz", &GENLepton2_Pz,"GENLepton2_Pz[GENnPair_]/D");
  DiLeptonTree_->Branch("GENLepton2_charge", &GENLepton2_charge,"GENLepton2_charge[GENnPair_]/I");
  DiLeptonTree_->Branch("GENLepton2_status", &GENLepton2_status,"GENLepton2_status[GENnPair_]/I");

  // Pile-up Reweight
  DiLeptonTree_->Branch("nPileUp",&nPileUp_,"nPileUp_/I");
  DiLeptonTree_->Branch("nPileUpIn",&nPileUpIn_,"nPileUpIn_/I");
  DiLeptonTree_->Branch("pileUpReweightIn",&pileUpReweightIn_,"pileUpReweightIn_/D");
  DiLeptonTree_->Branch("pileUpReweightPlusIn",&pileUpReweightPlusIn_,"pileUpReweightPlusIn_/D");
  DiLeptonTree_->Branch("pileUpReweightMinusIn",&pileUpReweightMinusIn_,"pileUpReweightMinusIn_/D");
  DiLeptonTree_->Branch("pileUpReweight",&pileUpReweight_,"pileUpReweight_/D");
  DiLeptonTree_->Branch("pileUpReweightPlus",&pileUpReweightPlus_,"pileUpReweightPlus_/D");
  DiLeptonTree_->Branch("pileUpReweightMinus",&pileUpReweightMinus_,"pileUpReweightMinus_/D");
  DiLeptonTree_->Branch("PUweight",&PUweight_,"PUweight_/D");

}

void
DiLeptonIntoNtuples::beginRun(const Run & iRun, const EventSetup & iSetup)
{

   const int nTrigName = 6;
   string trigs[nTrigName] = {
     "HLT_Mu17_TkMu8_v*",
     "HLT_Mu13_Mu8_v*",
     "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
     "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
     "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v*",
     "HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*"
   };
   MuonHLT_.clear();

   for( int i = 0; i < nTrigName; i++ ) {
     MuonHLT_.push_back(trigs[i]);
   }

  int listRemoval[nTrigName] = {-1};
  int ntrigName = MuonHLT_.size();
  bool changedConfig;
  if (!hltConfig_.init(iRun, iSetup, processName_, changedConfig)) {
    LogError("HLTMuonVal") << "Initialization of HLTConfigProvider failed!!";
    return;
  }
  else {
    std::vector<std::string> triggerNames = hltConfig_.triggerNames();
    for( int itrigName = 0; itrigName < ntrigName; itrigName++ ) {
      listRemoval[itrigName] = 0;

      // check list of triggers
      ////cout << "trigger = " << itrigName << " " << MuonHLT_[itrigName] << endl;
      bool isMatched = false;
      for( size_t i = 0; i < triggerNames.size(); i++) {
        std::vector<std::string> moduleNames = hltConfig_.moduleLabels( triggerNames[i] );
        ////cout << "Trigger Path: " << triggerNames[i] << endl;
	// find using wild card
        std::vector<std::vector<std::string>::const_iterator> matches = edm::regexMatch(triggerNames, MuonHLT_[itrigName]);
        if( !matches.empty() ) {
	  BOOST_FOREACH(std::vector<std::string>::const_iterator match, matches) {
	    ////cout << "trigger match = " << *match << endl;
	    if( *match == triggerNames[i] ) {
	      isMatched = true;
	      /*
              //cout << "Filter name: " << trigModuleNames_[moduleNames.size()-2] << endl;
              for( size_t j = 0; j < moduleNames.size(); j++) {
                TString name = moduleNames[j];
                //cout << "\t  Fliter Name: "<<moduleNames[j] << endl;
              }
	      */
	      int nsize = moduleNames.size();
	      if( nsize-2 >= 0 ) {
		////cout << "module names = " << moduleNames[nsize-2] << " " << moduleNames[nsize-3] << endl;
                trigModuleNames_.push_back(moduleNames[nsize-2]);
                ////cout << "Filter name: " << trigModuleNames_[trigModuleNames_.size()-1] << endl;
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
      if( !isMatched ) listRemoval[itrigName] = 1;
    }
  }

  // remove unavailable triggers
  int itmp = 0;
  for( vector<string>::iterator iter = MuonHLT_.begin(); iter != MuonHLT_.end(); ) {
    if( listRemoval[itmp] > 0 ) 
      iter = MuonHLT_.erase(iter);
    else 
      ++iter;
    itmp++;
  }
  ntrigName = MuonHLT_.size();

  // trigger filters
  //for( int itrig = 0; itrig < ntrigName; itrig++ ) {
  //  //cout << "Filter name: " << itrig << " " << MuonHLT_[itrig] << " " << trigModuleNames_[itrig] << " " << trigModuleNames__preFil[itrig] << endl;
  //}

}


// ------------ method called once each job just after ending the event loop  ------------
void 
DiLeptonIntoNtuples::endJob() {

  std::cout <<"++++++++++++++++++++++++++++++++++++++" << std::endl;

}

int DiLeptonIntoNtuples::motherId(const reco::GenParticle& par) const
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


int DiLeptonIntoNtuples::motherId(const reco::GenParticleRef par) const
{
    int motherid = -9999;
    return motherid;
}


//////////////////////////////////////////////////////////////////////////////
// calculates 3D angle between two muon momentum vectors using cosine theorem
//////////////////////////////////////////////////////////////////////////////
template <class T>
float DiLeptonIntoNtuples::angleBetween(const T & lhs, const T & rhs) const {

   GlobalVector mom1(lhs.px(), lhs.py(), lhs.pz());
   GlobalVector mom2(rhs.px(), rhs.py(), rhs.pz());

   GlobalVector dmom = mom1 - mom2;
   return acos( ( mom1.mag() * mom1.mag() + mom2.mag() * mom2.mag() - dmom.mag() * dmom.mag() ) / (2*mom1.mag()*mom2.mag() ));

}

///////////////////////////////////////////////////////////////
// makes hlt report and fills it to the ntuple
///////////////////////////////////////////////////////////////
void DiLeptonIntoNtuples::hltReport(const edm::Event &iEvent) {
   //
   // trigger
   //

   int ntrigName = MuonHLT_.size();
   ////cout << "ntrigName = " << ntrigName << " " << HLT_trigName_.max_size() << endl;
   //for( int itrig = 0; itrig < ntrigName; itrig++ ) {
   ////cout << "itrig = " << itrig << " " << MuonHLT_[itrig] << endl;
   //}
   /*
   int index_hlt_mu17_mu8 = -1;
   int index_hlt_mu17_tkmu8 = -1;
   int index_hlt_mu22_tkmu8 = -1;
   int index_hlt_mu22_tkmu22 = -1;
   int index_hlt_mu13_tkmu8 = -1;
   int index_hlt_isomu24 = -1;
   int index_hlt_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = 1;
   int index_hlt_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = -1;
   int index_hlt_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = -1;
   int index_hlt_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = -1;
   int index_hlt_Ele27_WP80 = -1;
   int index_hlt_Mu15_Photon20_CaloIdL = -1;
   int index_hlt_Mu8_Ele17_CaloIdT_CaloIsoVL = -1;

   for( int itrig = 0; itrig < ntrigName; itrig++ ) {
     if( MuonHLT_[itrig] == "HLT_Mu17_Mu8_v*" ) index_hlt_mu17_mu8 = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu17_TkMu8_v*" ) index_hlt_mu17_tkmu8 = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu22_TkMu8_v*" ) index_hlt_mu22_tkmu8 = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu22_TkMu22_v*" ) index_hlt_mu22_tkmu22 = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu13_TkMu8_v*" ) index_hlt_mu13_tkmu8 = itrig;
     if( MuonHLT_[itrig] == "HLT_IsoMu24_v*" ) index_hlt_isomu24 = itrig;
     if( MuonHLT_[itrig] == "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*" ) index_hlt_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = itrig;
     if( MuonHLT_[itrig] == "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*" ) index_hlt_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*" ) index_hlt_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*" ) index_hlt_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = itrig;
     if( MuonHLT_[itrig] == "HLT_Ele27_WP80_v*" ) index_hlt_Ele27_WP80 = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu15_Photon20_CaloIdL_v*" ) index_hlt_Mu15_Photon20_CaloIdL = itrig;
     if( MuonHLT_[itrig] == "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*" ) index_hlt_Mu8_Ele17_CaloIdT_CaloIsoVL = itrig;
   }
   ////cout << "index = " << index_hlt_mu17_mu8 << " " << index_hlt_mu17_tkmu8 << endl;
   */

   // read the whole HLT trigger lists fired in an event
   bool *trigFired = new bool[ntrigName];
   for( int i = 0; i < ntrigName; i++ ) trigFired[i] = false;
   Handle<TriggerResults> trigResult;
   iEvent.getByLabel(edm::InputTag("TriggerResults","",processName_), trigResult);
   if( !trigResult.failedToGet() ) {
       int ntrigs = trigResult->size();
       const edm::TriggerNames trigName = iEvent.triggerNames(*trigResult);
       for( int itrig = 0; itrig != ntrigs; ++itrig) {
	   ////cout << "trigName = " << trigName.triggerName(itrig) << " " << itrig << endl;
	   for( int itrigName = 0; itrigName < ntrigName; itrigName++ ) {
             std::vector<std::vector<std::string>::const_iterator> matches = edm::regexMatch(trigName.triggerNames(), MuonHLT_[itrigName]);
             if( !matches.empty() ) {
	       BOOST_FOREACH(std::vector<std::string>::const_iterator match, matches) {
		 ////cout << "trigger match = " << *match << endl;
	         if( trigName.triggerIndex(*match) >= (unsigned int)ntrigs ) continue;
	         if( trigResult->accept(trigName.triggerIndex(*match)) ) trigFired[itrigName] = true;
                 ////cout << "trigger fire = " << trigFired[itrigName] << endl;
	       }
	     }
	   }
       }
   }

   edm::Handle< trigger::TriggerEvent > triggerObject;
   iEvent.getByLabel(edm::InputTag("hltTriggerSummaryAOD","",processName_), triggerObject);
   const trigger::TriggerObjectCollection & toc(triggerObject->getObjects());
   int ntrigTot = 0;
   ////cout << "size filter = " << triggerObject->sizeFilters() << endl;
   for( size_t k = 0; k < triggerObject->sizeFilters(); ++ k ) {
       std::string fullname = triggerObject->filterTag(k).encode();
       std::string filterName;
       ////cout << "filterName = " << filterName << endl;
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
	   double hlt_eta = toc[*ky].eta();
	   ////cout << "hlt kine = " << hlt_pt << " " << hlt_eta << endl;

	   for( int itf = 0; itf < ntrigName; itf++ ) {
	     string names = "";
             ////cout << "filterName = " << k << " " << filterName << " " << trigModuleNames_[itf] << " " << trigModuleNames_preFil_[itf] << " " << HLT_trigName_.size() << endl;
	       if( filterName == trigModuleNames_[itf] ) { 
	         ////cout << "filter name = " << filterName << " " << ntrigTot << " " << itf << " " << hlt_pt << " " << toc[*ky].eta() << " " << toc[*ky].phi() << endl;
		 names = MuonHLT_[itf];
	         HLT_trigType[ntrigTot] = itf;
	         HLT_trigFired[ntrigTot] = trigFired[itf];
	         HLT_trigPt[ntrigTot] = hlt_pt;
	         HLT_trigEta[ntrigTot] = toc[*ky].eta();
	         HLT_trigPhi[ntrigTot] = toc[*ky].phi();
		 HLT_trigName_.push_back(names);
	         ntrigTot++;
	       }
	   }
	 }
       }
   }
   HLT_ntrig_ = ntrigTot;
   
   //for( int i = 0; i < HLT_ntrig_; i++ ) {
     ////cout << "trig = " << i << " " << HLT_trigType[i] << " " << HLT_trigPt[i] << " " << HLT_trigEta[i] << " " << HLT_trigPhi[i] << " " << _HLT_trigName[i] << endl;
   //}
}



//////////////////////////////////////////////////////////////
// Get METs info
//////////////////////////////////////////////////////////////
void DiLeptonIntoNtuples::fillMET(const edm::Event &iEvent) {

   edm::Handle<pat::METCollection> metHandle;
   iEvent.getByLabel(theMETLabel_,metHandle);

   pat::METCollection::const_iterator iMET = metHandle->begin();
   MET_ = iMET->pt();

}

//////////////////////////////////////////////////////////////
// Get Jets info
//////////////////////////////////////////////////////////////
void DiLeptonIntoNtuples::fillJet(const pat::Jet* jet, const edm::Event &iEvent) {

   purdue::Jet* pJet= new purdue::Jet();

   edm::Handle<edm::View<pat::Muon> > muonHandle;
   iEvent.getByLabel(theMuonLabel_,muonHandle);

   pJet->pt_ = jet->pt();
   pJet->eta_ = jet->eta();
   pJet->phi_ = jet->phi();
   pJet->q_ = jet->jetCharge();
   pJet->flavour_ = jet->partonFlavour();
   pJet->ntracks_ = jet->associatedTracks().size();
   pJet->bDiscriminant_Thr_alg1_ = 0.7;
   pJet->bDiscriminant_Thr_alg2_ = 0.7;
   pJet->bDiscriminant_Thr_alg3_ = 2.05;
   pJet->bDiscriminant_alg1_ = jet->bDiscriminator("trackCountingHighEffBJetTags");
   pJet->bDiscriminant_alg2_ = jet->bDiscriminator("trackCountingHighPurBJetTags");
   pJet->bDiscriminant_alg3_ = jet->bDiscriminator("simpleSecondaryVertexBJetTags");

   singleJets->push_back(*pJet);
}

//--------------------------------------------------------------------------------------------------
void DiLeptonIntoNtuples::fillMuon(const pat::Muon *mu, const edm::Event& iEvent)
{

  edm::Handle<reco::BeamSpot> beamSpotHandle;
  iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
  reco::BeamSpot beamSpot = (*beamSpotHandle);

  edm::Handle<reco::VertexCollection> pvHandle;
  iEvent.getByLabel("offlinePrimaryVertices", pvHandle);

  purdue::Muon *pMuon = new purdue::Muon();//purdue::LorentzVector(mu->p4()));

  pMuon->pt_              = mu->pt();
  pMuon->eta_             = mu->eta();
  pMuon->phi_             = mu->phi();
  pMuon->px_              = mu->px();
  pMuon->py_              = mu->py();
  pMuon->pz_              = mu->pz();
  pMuon->trkIso03_ = mu->isolationR03().sumPt;
  pMuon->emIso03_  = mu->isolationR03().emEt;
  pMuon->hadIso03_ = mu->isolationR03().hadEt;
  pMuon->chIso03_ = mu->pfIsolationR03().sumPUPt;
  pMuon->puChIso03_ = mu->pfIsolationR03().sumChargedHadronPt;
  pMuon->neuHadIso03_ = mu->pfIsolationR03().sumNeutralHadronEt;
  pMuon->gammaIso03_ = mu->pfIsolationR03().sumPhotonEt;
  pMuon->q_        = mu->charge();
  pMuon->nMatch_       = mu->numberOfMatches();

  pMuon->typeBits_ = 0;
  if(mu->isGlobalMuon() && mu->isTrackerMuon() && mu->isPFMuon())     { pMuon->typeBits_ = kGlbTrkPF; }
  else if(mu->isGlobalMuon())                               { pMuon->typeBits_ = kGlobal;}
  else if(mu->isTrackerMuon())    			    { pMuon->typeBits_ = kTracker; }
  else if(mu->isStandAloneMuon()) 			    { pMuon->typeBits_ = kStandalone; }
  else if(mu->isPFMuon())       			    { pMuon->typeBits_ = kPFMuon; }
 
  // global, track and STA
  reco::TrackRef muonTrk;
  if ( pMuon->typeBits_ ==  kGlobal || pMuon->typeBits_ ==  kGlbTrkPF) muonTrk = mu->globalTrack();
  else if ( pMuon->typeBits_ == kTracker) muonTrk = mu->track();
  else if ( pMuon->typeBits_ == kStandalone) muonTrk = mu->standAloneMuon();
  // reco track information
  if ( muonTrk.isNonnull() ) {
     if ( muonTrk->normalizedChi2() < 1000 ) pMuon->muNchi2_ = muonTrk->normalizedChi2();

     pMuon->d0BS_       = muonTrk->dxy(beamSpot.position());
     pMuon->dzBS_       = muonTrk->dz(beamSpot.position());
     if ( !pvHandle->empty() && !pvHandle->front().isFake() ) {
         const reco::Vertex& vtx = pvHandle->front();
         pMuon->d0BS_ = muonTrk->dxy(vtx.position());
         pMuon->dzBS_ = muonTrk->dz(vtx.position());
        }
 
     //global muon
     if ( pMuon->typeBits_ == 1 ) {
          reco::TrackRef trackerTrack = mu->innerTrack();
          reco::TrackRef glbTrack     = mu->globalTrack();
          const reco::HitPattern& inhit = trackerTrack->hitPattern();
          const reco::HitPattern& glbhit = glbTrack->hitPattern();
          pMuon->nTrackerLayers_  = glbhit.trackerLayersWithMeasurement();
          pMuon->nTkHits_ = inhit.numberOfValidTrackerHits();
          pMuon->nPixHits_ = inhit.numberOfValidPixelHits();
          pMuon->nMuHits_ = glbhit.numberOfValidMuonHits();
          ////cout << pMuon->nTrackerLayers_ << " " << pMuon->nTkHits_ << " " << pMuon->nPixHits_ << " " << pMuon->nMuHits_ << endl;
     }
 }//track nonnull
 singleMuons->push_back(*pMuon); 
}


void DiLeptonIntoNtuples::fillDimuon(int mu1, int mu2, const edm::EventSetup& iSetup, const edm::Event& iEvent)
{
  // electrons
  edm::Handle<pat::MuonCollection> muonHandle;
  iEvent.getByLabel(theMuonLabel_, muonHandle);

  purdue::Dimuon* pDimuon = new purdue::Dimuon(mu1,mu2);
  const pat::Muon muon1 = muonHandle->at(mu1);
  const pat::Muon muon2 = muonHandle->at(mu2);

  reco::NamedCompositeCandidate aDYcand;
  aDYcand.setP4(muon1.p4() + muon2.p4());
  pDimuon->mass_ = aDYcand.mass();
  pDimuon->y_ = aDYcand.rapidity();
  pDimuon->pt_ = aDYcand.pt();

 if (muon1.track().isNonnull() && muon2.track().isNonnull())   {
     pDimuon->angle_ = acos(-muon1.track()->momentum().Dot(muon2.track()->momentum()/
                                  muon1.track()->p()/muon2.track()->p()));
  }

     // dimuon vertex with tracker track
     reco::TrackRef mu1trkerTrk = muon1.track();
     reco::TrackRef mu2trkerTrk = muon2.track();
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
         const CachingVertex<5>& vtxTrk = vertexTrk;
         pDimuon->vtxTrkProb_  = TMath::Prob(vtxTrk.totalChiSquared(),(int)vtxTrk.degreesOfFreedom());
      }
   }

 diMuons->push_back(*pDimuon);
}


//--------------------------------------------------------------------------------------------------
double DiLeptonIntoNtuples::D0Corrected(const reco::GsfElectron *ele, const reco::Vertex *pv) const
{
  // Return corrected d0 with respect to primary vertex or beamspot.
  double lXM =  -TMath::Sin(ele->bestTrack()->phi()) * ele->bestTrack()->d0();
  double lYM =   TMath::Cos(ele->bestTrack()->phi()) * ele->bestTrack()->d0();
  double lDX = (lXM + pv->x());
  double lDY = (lYM + pv->y());
  double d0Corr = (ele->bestTrack()->px()*lDY - ele->bestTrack()->py()*lDX)/ele->bestTrack()->pt();
 
  return d0Corr;
}

double DiLeptonIntoNtuples::DzCorrected(const reco::GsfElectron *ele, const reco::Vertex *pv) const
{
  // Compute Dxy with respect to a given position
  TVector3 momPerp(ele->bestTrack()->px(),ele->bestTrack()->py(),0);
  TVector3 posPerp(ele->bestTrack()->d0()*TMath::Sin(ele->bestTrack()->phi())-pv->x(),-ele->bestTrack()->d0()*TMath::Cos(ele->bestTrack()->phi())-pv->y(),0);
  return ele->bestTrack()->dsz()/TMath::Cos(ele->bestTrack()->lambda()) - pv->z() - posPerp.Dot(momPerp)/ele->bestTrack()->pt() * (ele->bestTrack()->pz()/ele->bestTrack()->pt());
}

//--------------------------------------------------------------------------------------------------
void DiLeptonIntoNtuples::fillElectron(const reco::GsfElectron* ele, const edm::Event& iEvent)
{
  //cout << "4" << endl;
  edm::Handle<reco::BeamSpot> beamSpotHandle;
  iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
  reco::BeamSpot beamSpot = (*beamSpotHandle);

  //conversions
  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);

  purdue::Electron *pElectron = new purdue::Electron();

  pElectron->pt_              = ele->pt();
  pElectron->eta_             = ele->eta();
  pElectron->phi_             = ele->phi();
  pElectron->px_              = ele->px();
  pElectron->py_              = ele->py();
  pElectron->pz_              = ele->pz();
  pElectron->trkIso03_        = ele->dr03TkSumPt();
  pElectron->emIso03_         = ele->dr03EcalRecHitSumEt();
  pElectron->hadIso03_        = ele->dr03HcalTowerSumEt();
  pElectron->d0BS_            = D0Corrected(ele,&*theVertex_);
  pElectron->dzBS_            = DzCorrected(ele,&*theVertex_);  
  pElectron->d0VTX_           = D0Corrected(ele,&*theVertex_);
  pElectron->dzVTX_           = DzCorrected(ele,&*theVertex_);  
  //cout << "5" << endl;
  pElectron->scEt_            = (ele->superCluster()->energy())*(ele->pt())/(ele->p());
  pElectron->scEta_           = ele->superCluster()->eta();
  pElectron->scPhi_           = ele->superCluster()->phi();
  pElectron->scE_           = ele->superCluster()->energy();
  pElectron->ecalE_           = ele->ecalEnergy();
  pElectron->HoverE_          = ele->hadronicOverEm();
  pElectron->EoverP_          = ele->eSuperClusterOverP();
  pElectron->fBrem_           = ele->fbrem();
  pElectron->deltaEtaIn_      = ele->deltaEtaSuperClusterTrackAtVtx();
  pElectron->deltaPhiIn_      = ele->deltaPhiSuperClusterTrackAtVtx();
  pElectron->sigiEtaiEta_     = ele->scSigmaIEtaIEta();
  pElectron->nExpHitsInner_   = ele->bestTrack()->trackerExpectedHitsInner().numberOfHits();
  pElectron->partnerDeltaCot_ = 0;//ele->ConvPartnerDCotTheta();
  pElectron->partnerDist_     = 0;//ele->ConvPartnerDist();
  pElectron->q_               = ele->charge();  

  //cout << "6" << endl; 
  pElectron->scID_            = 0;//ele->superCluster()->GetUniqueID();
  pElectron->trkID_           = 0;//(ele->HasTrackerTrk()) ? ele->TrackerTrk()->GetUniqueID() : 0;
  pElectron->isConv_          = ConversionTools::hasMatchedConversion(*ele,hConversions,beamSpot.position());
 
  pElectron->typeBits_=0;
  if(ele->ecalDriven())    { pElectron->typeBits_ |= kEcalDriven; }
  if(ele->trackerDrivenSeed()) { pElectron->typeBits_ |= kTrackerDriven; }
  pElectron->mva_ = 0; //fEleMVA->MVAValue(ele, fVertex);

  //PF based isolations
  setEleIsolations(iEvent, ele, pElectron);
  pElectron->AEffUser_ = isolEffectAreasAl((ele)->superCluster()->eta());
  singleElectrons->push_back(*pElectron);

}

//--------------------------------------------------------------------------------------------------
void DiLeptonIntoNtuples::fillDielectron(int ele1, int ele2, const edm::Event& iEvent)
{
  //conversions
  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);

  // electrons
  edm::Handle<reco::GsfElectronCollection> electronHandle;
  iEvent.getByLabel("calibratedElectrons","calibratedGsfElectrons", electronHandle);
  //iEvent.getByLabel("gsfElectrons",electronHandle);

  purdue::Dielectron* pDielectron = new purdue::Dielectron(ele1,ele2);
  const reco::GsfElectron electron1 = electronHandle->at(ele1);
  const reco::GsfElectron electron2 = electronHandle->at(ele2);

  reco::NamedCompositeCandidate aDYcand;
  aDYcand.setP4(electron1.p4() + electron2.p4());
  pDielectron->mass_ = aDYcand.mass();
  pDielectron->y_ = aDYcand.rapidity();
  pDielectron->pt_ = aDYcand.pt();

  diElectrons->push_back(*pDielectron);
}
  
//--------------------------------------------------------------------------------------------------
void DiLeptonIntoNtuples::fillPhoton(const pat::Photon *pho) 
{

  purdue::Photon *pPhoton = new purdue::Photon();
  
  pPhoton->pt_		= pho->pt(); 
  pPhoton->eta_  	= pho->eta();
  pPhoton->phi_  	= pho->phi();
  pPhoton->scEt_        = (pho->superCluster()->energy())*(pho->pt())/(pho->p());
  pPhoton->scEta_  	= pho->superCluster()->eta();
  pPhoton->scPhi_  	= pho->superCluster()->phi();
  pPhoton->trkIso04_    = pho->trkSumPtHollowConeDR04();
  pPhoton->trkIso04NoPV_= 0; ///IsolationTools::TrackIsolationNoPV(pho, fBeamSpot->At(0), 0.4, 0.04, 0.0, 0.015, 0.1, TrackQuality::highPurity, fTracks);
  pPhoton->emIso04_     = pho->ecalRecHitSumEtConeDR04();
  pPhoton->hadIso04_	= pho->hcalDepth1TowerSumEtBcConeDR04();
  pPhoton->HoverE_	= pho->hadronicOverEm();
  pPhoton->R9_		= pho->r9();
  pPhoton->sigiEtaiEta_  = pho->sigmaIetaIeta();
  pPhoton->sigiPhiiPhi_  = 0;//sqrt(pho->superCluster()->seed()->CoviPhiiPhi());
  //pPhoton->scID         = pho->superCluster()->GetUniqueID();
  pPhoton->hasPixelSeed_ = pho->hasPixelSeed();
  
  pPhoton->passEleVetoConvRec_ = 0;//PhotonTools::PassElectronVetoConvRecovery(pho,fElectrons,fConversions,fBeamSpot->At(0));
  pPhoton->passConvId_         = 0;//PhotonTools::PassConversionId(pho,PhotonTools::MatchedConversion(pho,fConversions,fBeamSpot->At(0)));
  
  //bool passFilter=true;
  //pho->superCluster()->seed();
  //pho->superCluster()->seed()->energy();
  //eMax( pho->superCluster()->seed(), getEcalRecHitCollection(pho->superCluster()->seed()));
  //if((pho->superCluster()->seed()->energy() > 5.0) && 
  //   (eMax( pho->superCluster()->seed(), getEcalRecHitCollection(pho->superCluster()->seed())  // pho->superCluster()->seed()->EMax() / pho->superCluster()->seed()->e3x3() > 0.95))
  //   passFilter=false;
  
  pPhoton->passSpikeFilter_ = 0;//passFilter;

  singlePhotons->push_back(*pPhoton); 
}

double DiLeptonIntoNtuples::isolEffectAreasAl(double eta) {
        double Aeff = 0.;
        if (fabs(eta)<=1.0) {
             Aeff= 0.13;
        } else if (fabs(eta)>1.0 && fabs(eta)<=1.479) {  Aeff= 0.14;
        } else if (fabs(eta)>1.479 && fabs(eta)<=2.0) {  Aeff= 0.07;
        } else if (fabs(eta)>2.0 && fabs(eta)<=2.2) {    Aeff= 0.09;
        } else if (fabs(eta)>2.2 && fabs(eta)<=2.3) {    Aeff= 0.11;
        } else if (fabs(eta)>2.3 && fabs(eta)<=2.4) {    Aeff= 0.11;
        } else if (fabs(eta)>2.4) {Aeff= 0.14;}
      return Aeff;
}

void DiLeptonIntoNtuples::setEleIsolations(const edm::Event& iEvent, const reco::GsfElectron* thisEle, purdue::Electron* &myEle) 
{
  // iso deposits
  IsoDepositVals isoVals(isoValInputTags_.size());
  for (size_t j = 0; j < isoValInputTags_.size(); ++j) {
        iEvent.getByLabel(isoValInputTags_[j], isoVals[j]);
  }

  // electrons
  edm::Handle<reco::GsfElectronCollection> electronHand;
  iEvent.getByLabel("gsfElectrons", electronHand);
  Nelectrons_ = electronHand->size();

   // loop on electrons
  for(int i = 0; i < Nelectrons_; ++i) {

   // get reference to electron
   reco::GsfElectronRef ele(electronHand, i);
   //match 
   if (fabs((*&ele)->pt() - thisEle->pt()) > 0.0000001) continue;

   myEle->chIso_ = (*(isoVals)[0])[ele];
   myEle->gammaIso_ = (*(isoVals)[1])[ele];
   myEle->neuHadIso_ = (*(isoVals)[2])[ele];
   myEle->puChIso_ = 0; // (*(isoVals)[0])[ele];
  }
}
//define this as a plug-in
DEFINE_FWK_MODULE(DiLeptonIntoNtuples);
