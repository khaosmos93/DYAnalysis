#ifndef Dimuon_H
#define Dimuon_H

#include "DataFormats/Purdue/interface/Muon.h"

#include <assert.h>
#include <utility>

namespace purdue{

  class Dimuon {
   public:
      //default constructor
      Dimuon():
      mass_(0), pt_(0), y_(0), angle_(0), vtxTrkProb_(0),
      muon_links_(0,0)
      {}
      //constructor with parameters
      Dimuon(int mulink1, int mulink2, double mass=0, double pt = 0, double y=0,double vtxTrkProb=0,double angle=0) {
         assert(mulink1!=mulink2);
         muon_links_ = std::make_pair(mulink1,mulink2);
         mass_=mass;
         pt_=pt;
         y_=y;
         vtxTrkProb_ = vtxTrkProb;
         angle_ = angle;
      }
      virtual ~Dimuon(){}

      double mass_, pt_, y_, angle_, vtxTrkProb_;
      std::pair<int,int> muon_links_; 

      //ClassDef(Dimuon, 1)  
  };
}
#endif
