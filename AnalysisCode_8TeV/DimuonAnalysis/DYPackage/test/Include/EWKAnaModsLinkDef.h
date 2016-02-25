#ifndef EWKANA_NTUPLER_LINKDEF_H
#define EWKANA_NTUPLER_LINKDEF_H
#include "../Include/BambuGenDumperMod.hh"
#include "../Include/NtuplerMod.hh"
#include "../Include/TEventInfo.hh"
#include "../Include/TGenInfo.hh"
#include "../Include/TGenPhoton.hh"
#include "../Include/TMuon.hh"
#include "../Include/TElectron.hh"
#include "../Include/TDielectron.hh"
#include "../Include/TJet.hh"
#include "../Include/TPhoton.hh"
#include "../Include/TVertex.hh"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::BambuGenDumperMod+;
#pragma link C++ class mithep::NtuplerMod+;
#pragma link C++ class mithep::TEventInfo+;
#pragma link C++ class mithep::TGenInfo+;
#pragma link C++ class mithep::TGenPhoton+;
#pragma link C++ class mithep::TMuon+;
#pragma link C++ class mithep::TElectron+;
#pragma link C++ class mithep::TDielectron+;
#pragma link C++ class mithep::TJet+;
#pragma link C++ class mithep::TPhoton+;
#pragma link C++ class mithep::TVertex+;
#endif
