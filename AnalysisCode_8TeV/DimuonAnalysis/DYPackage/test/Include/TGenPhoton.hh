#ifndef EWKANA_NTUPLER_TGENPHOTON_HH
#define EWKANA_NTUPLER_TGENPHOTON_HH

#include <TObject.h>

namespace mithep 
{
  // Generator level photon data object
  class TGenPhoton : public TObject
  {
    public:
      TGenPhoton():
      status(0),parentId(0),pt(0),eta(0),phi(0)
      {}
      ~TGenPhoton(){}
      
      Int_t   status;                    // particle status
      Int_t   parentId;                  // PDG ID of parent
      Float_t pt, eta, phi;              // kinematics
          
    ClassDef(TGenPhoton,1)
  };
}
#endif
