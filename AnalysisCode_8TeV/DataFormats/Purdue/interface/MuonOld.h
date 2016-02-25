#ifndef Muon_H
#define Muon_H

namespace purdue{

  class Muon { 
   public:
      //default constructor
      Muon():
      pt_(0), eta_(0), phi_(0), px_(0), py_(0), pz_(0), trkIso03_(0), emIso03_(0), hadIso03_(0),
      chIso03_(0),puChIso03_(0),neuHadIso03_(0),gammaIso03_(0),
      AEffUser_(0), d0BS_(0), dzBS_(0),
      d0VTX_(0), dzVTX_(0), muNchi2_(0), vtxTrkProb_(0), nTrackerLayers_(0), q_(0),
      nMuHits_(0), typeBits_(0), nTkHits_(0), nPixHits_(0), nMatch_(0)
      {}
      virtual ~Muon(){}

      double pt_, eta_, phi_;         // kinematics
      double px_,py_,pz_;
      double trkIso03_;               // track isolation
      double emIso03_;                // ECAL-based isolation
      double hadIso03_;               // HCAL-based isolation
      double chIso03_;
      double puChIso03_;
      double neuHadIso03_;
      double gammaIso03_;
      double AEffUser_;
      double d0BS_, dzBS_;                // impact parameter
      double d0VTX_, dzVTX_;                // impact parameter
      double muNchi2_;                // global muon chi^2/ndf
      double vtxTrkProb_;
      int nTrackerLayers_;
      int   q_;                       // charge
      int   nMuHits_;                 // number of valid hits in muon system
      unsigned int  typeBits_;        // global muon, tracker muon, or standalone muon
      unsigned int  nTkHits_;         // number of inner tracker hits
      unsigned int  nPixHits_;        // number of pixel hits
      unsigned int  nMatch_;          // number of muon chambers matched to segments     


      //ClassDef(Muon, 1)    
   };
}
#endif
