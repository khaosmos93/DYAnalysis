#ifndef Dielectron_H
#define Dielectron_H
   
#include "Electron.h"

#include <assert.h>
#include <utility>

namespace purdue{
    
  class Dielectron {
  public:
      //default constructor
      Dielectron():
      mass_(0), pt_(0), y_(0),
      ele_links_(0,0)
      {}
      Dielectron(int elink1, int elink2, double mass=0, double pt = 0, double y=0) { 
         assert(elink1!=elink2); 
         ele_links_ = std::make_pair(elink1,elink2);
         mass_=mass;
         pt_=pt;
         y_=y; 
      }
      virtual ~Dielectron(){}
   
      double mass_, pt_, y_;

      std::pair<int,int> ele_links_;
      ClassDef(Dielectron, 1)   
  };
}
#endif
