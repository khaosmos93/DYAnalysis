#ifndef DiLeptonIntoNtuples_H
#define DiLeptonIntoNtuples_H

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
//Data formats
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//HLT
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
//pileup
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <TTree.h>

//
// class decleration
//
using namespace std;
namespace reco { class CandCommonVertexFitterBase; class VertexCompositeCandidate; class CandCommonVertexFitter; }
namespace purdue{class Muon; class Electron; class Dimuon; class Dielectron; class Photon; class Jet;}

//enums, typedfs
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;

enum EMuType
{
  kGlbTrkPF     = 1,
  kGlobal     = 2,
  kTracker    = 3,
  kStandalone = 4,
  kPFMuon     = 5
};

enum EEleType
{
  kEcalDriven    = 1,
  kTrackerDriven = 2
};
class DiLeptonIntoNtuples : public edm::EDAnalyzer {

   public:
      explicit DiLeptonIntoNtuples(const edm::ParameterSet&);
      ~DiLeptonIntoNtuples();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      virtual void beginRun(const edm::Run &, const edm::EventSetup & );

      void fillMET(const edm::Event &iEvent);            // fill MET information
      void fillJet(const pat::Jet* jet, const edm::Event &iEvent);            // fill jet and b-tagging information
      void fillMuon(const pat::Muon *mu, const edm::Event&);
      void fillElectron(const reco::GsfElectron* ele, const edm::Event&); 
      void fillDimuon(int mu1, int mu2, const edm::EventSetup& iSetup, const edm::Event&);
      void fillDielectron(int ele1, int ele2, const edm::Event&);
      void fillPhoton(const pat::Photon*);
      void setEleIsolations(const edm::Event&, const reco::GsfElectron*, purdue::Electron*&);

      double isolEffectAreasAl(double);
      double D0Corrected(const reco::GsfElectron *ele, const reco::Vertex *pv) const;
      double DzCorrected(const reco::GsfElectron *ele, const reco::Vertex *pv) const;
      void hltReport(const edm::Event &iEvent);          // fill list of triggers fired in an event
      template <class T> float angleBetween(const T & lhs, const T & rhs) const;
      int motherId(const reco::GenParticle& par) const;
      int motherId(const reco::GenParticleRef par) const;

   private:
      int theDebugLevel_;                   // 0 no prints, 1 some, 2 lots
      std::string processName_;

      edm::InputTag theElectronLabel_;
      edm::InputTag theMuonLabel_;
      edm::InputTag thePhotonLabel_;  
      edm::InputTag theMETLabel_;
      edm::InputTag theJetLabel_;
      edm::InputTag theBeamSpot_;
      edm::InputTag theTrackLabel_;
      edm::InputTag metLabel_;
      edm::InputTag theRhoIsoInputTag_;

      //flags to select which info to store in the ntuple
      bool theStorePriVtx2MuFlag_;             // Yes or No to store primary vertex for dimuon candidates
      bool isMC_;  //turn gen on and off
      bool isSignal_;
      bool runOnMuInput_;
      bool runOnEleInput_;
      bool isAOD_; //inserted temporarily to resolve the issues with event content

      std::vector<edm::InputTag>  isoValInputTags_;

      std::vector<std::string> HLT_trigName_;

      // Pile-up Reweight
      edm::LumiReWeighting LumiWeights_;
      reweight::PoissonMeanShifter PShiftUp_;
      reweight::PoissonMeanShifter PShiftDown_;

      std::vector<double> PileUpRD_;
      std::vector<double> PileUpMC_;

      static const int MPSIZE = 1000;
      int HLT_trigType[MPSIZE];
      HLTConfigProvider hltConfig_;
      std::vector<std::string > MuonHLT_;
      std::vector<std::string > trigModuleNames_;
      std::vector<std::string > trigModuleNames_preFil_;

      TTree *DiLeptonTree_;
      std::vector<purdue::Muon>* singleMuons;
      std::vector<purdue::Electron>* singleElectrons;
      std::vector<purdue::Dielectron>* diElectrons;
      std::vector<purdue::Dimuon>* diMuons;
      std::vector<purdue::Photon>* singlePhotons;
      std::vector<purdue::Jet>* singleJets;

      const reco::Vertex*           theVertex_;          // best primary vertex in the event

      //event info
      int runNum_;
      int evtNum_;
      int lumiBlock_; 
      int Nmuons_;
      int Nelectrons_; 

      //counts 
      int GENnPair_;
      int Njets_;

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

      double rhoIso_;

      double   pvx_;
      double   pvy_;
      double   pvz_;
      bool     hasGoodPV_;

      // for trigger matching
      int HLT_ntrig_;
      bool HLT_trigFired[MPSIZE];
      double HLT_trigPt[MPSIZE];
      double HLT_trigEta[MPSIZE];
      double HLT_trigPhi[MPSIZE];
      static const int nTrigName_ = 15;

      // MET
      double MET_;

      // Gen 
      double GENInvMass[MPSIZE];
      double GENRapidity[MPSIZE];
      double GENPt[MPSIZE];
      double GENAngle[MPSIZE];

      double GENLepton1_phi[MPSIZE];
      double GENLepton1_eta[MPSIZE];
      double GENLepton1_pT[MPSIZE];
      double GENLepton1_Px[MPSIZE];
      double GENLepton1_Py[MPSIZE];
      double GENLepton1_Pz[MPSIZE];
      int GENLepton1_charge[MPSIZE];
      int GENLepton1_status[MPSIZE];

      double GENLepton2_phi[MPSIZE];
      double GENLepton2_eta[MPSIZE];
      double GENLepton2_pT[MPSIZE];
      double GENLepton2_Px[MPSIZE];
      double GENLepton2_Py[MPSIZE];
      double GENLepton2_Pz[MPSIZE];
      int GENLepton2_charge[MPSIZE];
      int GENLepton2_status[MPSIZE];
      int GENLepton1_pdgID[MPSIZE];
      int GENLepton2_pdgID[MPSIZE];
};

#endif
