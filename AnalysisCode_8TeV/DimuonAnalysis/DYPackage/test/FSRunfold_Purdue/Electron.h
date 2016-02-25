#ifndef Electron_H
#define Electron_H
    
namespace purdue{
    
  class Electron{
  public:
      Electron():
      pt_(0), eta_(0), phi_(0), px_(0),py_(0),pz_(0),trkIso03_(0), emIso03_(0), hadIso03_(0),
      chIso_(0),puChIso_(0),neuHadIso_(0),gammaIso_(0),
      d0_(0), dz_(0), scEt_(0), scEta_(0), scPhi_(0), scE_(0),
      ecalE_(0), HoverE_(0), EoverP_(0), fBrem_(0), deltaEtaIn_(0), deltaPhiIn_(0), sigiEtaiEta_(0),
      partnerDeltaCot_(0), partnerDist_(0), mva_(0), q_(0), nExpHitsInner_(0), scID_(0), trkID_(0),
      typeBits_(0), isConv_(0)
      {}
      virtual ~Electron(){}
    
      double pt_, eta_, phi_;        // kinematics
      double px_,py_,pz_;
      double trkIso03_;            // track isolation
      double emIso03_;             // ECAL-based isolation
      double hadIso03_;            // HCAL-based isolation
      double chIso_;
      double puChIso_;
      double neuHadIso_;
      double gammaIso_;
      double AEffUser_;
      double rhoIso_;
      double d0_, dz_;              // impact parameter
      double scEt_, scEta_, scPhi_;  // supercluster
      double scE_;
      double ecalE_;               // ECAL energy
      double HoverE_;              // H / E
      double EoverP_;              // E / p
      double fBrem_;               // brem fraction  
      double deltaEtaIn_;          // eta difference between track (at vertex) and SC
      double deltaPhiIn_;          // phi difference between track (at vertex) and SC
      double sigiEtaiEta_;         // eta-width of shower in number of crystals
      double partnerDeltaCot_;     // cot(theta) difference with conversion partner track	
      double partnerDist_;         // distance in x-y plane to nearest conversion partner track
      double mva_;                 // MVA electron ID
      int   q_;                   // charge
      unsigned int  nExpHitsInner_;       // number of hits expected before first hit      	             
      unsigned int  scID_;                // supercluster ID (for matching to photon superclusters)
      unsigned int  trkID_;               // tracker track ID (for matching to muons)
      unsigned int  typeBits_;            // bits for electron type
      bool  isConv_;              // is conversion? (vertexing method)

      ClassDef(Electron, 1)                             // Electron class

  };
}
#endif
