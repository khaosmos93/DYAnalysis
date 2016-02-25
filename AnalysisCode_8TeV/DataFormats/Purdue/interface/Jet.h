#ifndef JET_H
#define JET_H
#include "TObject.h"
#include "TLorentzVector.h"

namespace purdue
{
  class Jet: public TObject{
    public:
      Jet():
      pt_(0),eta_(0),phi_(0),q_(0),flavour_(0),ntracks_(0),bDiscriminant_Thr_alg1_(0),
      bDiscriminant_Thr_alg2_(0),bDiscriminant_Thr_alg3_(0),bDiscriminant_alg1_(0),bDiscriminant_alg2_(0),bDiscriminant_alg3_(0)
      {}
      virtual ~Jet(){}

      TLorentzVector p4;
	  float emf;
	  float area;
	  double dR;
	  float et;

      double pt_;
      double eta_;
      double phi_;
      int q_;
      int flavour_;
      int ntracks_;
      double bDiscriminant_Thr_alg1_;
      double bDiscriminant_Thr_alg2_;
      double bDiscriminant_Thr_alg3_;
      double bDiscriminant_alg1_;
      double bDiscriminant_alg2_;
      double bDiscriminant_alg3_;    

      ClassDef(Jet, 2)
  };  
}
#endif
