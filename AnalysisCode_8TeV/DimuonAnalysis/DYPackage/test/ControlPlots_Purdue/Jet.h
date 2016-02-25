#ifndef JET_H
#define JET_H

namespace purdue
{
  class Jet{ 
    public:
      Jet():
      pt_(0),eta_(0),phi_(0),q_(0),flavour_(0),ntracks_(0),bDiscriminant_Thr_alg1_(0),
      bDiscriminant_Thr_alg2_(0),bDiscriminant_Thr_alg3_(0),bDiscriminant_alg1_(0),bDiscriminant_alg2_(0),bDiscriminant_alg3_(0)
      {}
      virtual ~Jet(){}

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

      ClassDef(Jet, 1)
  };  
}
#endif
