#ifndef PHOTON_H
#define PHOTON_H

namespace purdue
{
  class Photon {
    public:
      Photon():
      pt_(0), eta_(0), phi_(0), scEt_(0), scEta_(0), scPhi_(0), trkIso04_(0), trkIso04NoPV_(0), emIso04_(0), hadIso04_(0), HoverE_(0),
      R9_(0), sigiEtaiEta_(0), sigiPhiiPhi_(0), scID_(0), hasPixelSeed_(0), passSpikeFilter_(0), passEleVetoConvRec_(0), 
      passConvId_(0)
      {}
      virtual ~Photon(){}

      double pt_, eta_, phi_; 	              // kinematics
      double scEt_, scEta_, scPhi_;             // supercluster
      double trkIso04_;                       // track isolation (hollow cone)
      double trkIso04NoPV_;                   // track isolation without PV-constraint
      double emIso04_;                        // ECAL-based isolation
      double hadIso04_;                       // HCAL-based isolation
      double HoverE_;		              // H/E
      double R9_;		              // ratio of energies in 3x3 to SC
      double sigiEtaiEta_;                    // eta-width of shower in number of crystals
      double sigiPhiiPhi_;                    // phi-width of shower in number of crystals           
      unsigned int  scID_;                           // supercluster ID (for matching to electron superclusters)
      bool  hasPixelSeed_;                   // supercluster has pixel seed?
      bool  passSpikeFilter_;                // pass spike filter?
      bool  passEleVetoConvRec_;             // 
      bool  passConvId_;                     // 
    
  };  
}
#endif
