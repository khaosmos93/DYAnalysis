#ifndef ZEE_DATA_HH
#define ZEE_DATA_HH

#define ZeeData_is_TObject

#ifdef ZeeData_is_TObject
#include <TObject.h>
#include "TEventInfo.hh"
#include "TDielectron.hh"

class ZeeData_t : public TObject
#else
struct ZeeData
#endif
{
public:
  UInt_t  runNum;                          // run number in data
  UInt_t  evtNum;                          // event number in data
  UInt_t  lumiSec;                         // lumi section      
  UInt_t  nTracks0;                        // number of reconstructed tracks in event
  UInt_t  nCaloTowers0;                    // number of reconstructed calorimeter towers in event
  UInt_t  nPV;                             // number of valid reconstructed primary vertices in event                                          
  UInt_t  nGoodPV;                         // number of good PVs
  UInt_t  nJets;                           // number of jets (with some requirements)
  Float_t caloMEx, caloMEy, caloSumET;     // calorimeter MET 
  Float_t tcMEx, tcMEy, tcSumET;           // track-corrected MET
  Float_t pfMEx, pfMEy, pfSumET;           // particle flow MET

  Float_t mass, pt, y, phi;                // dielectron kinematics    
  
  Float_t pt_1, ptUncorr_1, eta_1, phi_1;              // leading electron
  Float_t scEt_1, scEta_1, scPhi_1;  
  UInt_t  hltMatchBits_1;
  Int_t   q_1;
  
  Float_t pt_2, ptUncorr_2, eta_2, phi_2;              // lagging electron
  Float_t scEt_2, scEta_2, scPhi_2;
  UInt_t  hltMatchBits_2;
  Int_t   q_2;

  Float_t relPFIso03_1, relPFIso03_2;
  Float_t sigiEtaiEta_1, sigiEtaiEta_2, deltaEtaIn_1, deltaEtaIn_2, deltaPhiIn_1, deltaPhiIn_2, HoverE_1, HoverE_2;

  Float_t d0_1, dz_1, ooeoop_1, nExpHitsInner_1;
  Float_t d0_2, dz_2, ooeoop_2, nExpHitsInner_2;

  Float_t weight;                          // event weight  
  //UInt_t nCands; 

#ifdef ZeeData_is_TObject
  ClassDef(ZeeData_t,2)
#endif
};

//"runNum/i:evtNum:lumiSec:nTracks0:nCaloTowers0:nPV:nGoodPV:nJets:caloMEx/F:caloMEy:caloSumET:tcMEx:tcMEy:tcSumET:pfMEx:pfMEy:pfSumET:mass:pt:y:phi:pt_1:eta_1:phi_1:scEt_1:scEta_1:scPhi_1:hltMatchBits_1/i:q_1/I:pt_2/F:eta_2:phi_2:scEt_2:scEta_2:scPhi_2:hltMatchBits_2/i:q_2/I:weight/F"
//"runNum/i:evtNum:lumiSec:nTracks0:nCaloTowers0:nPV:nJets:caloMEx/F:caloMEy:caloSumET:tcMEx:tcMEy:tcSumET:pfMEx:pfMEy:pfSumET:mass:pt:y:phi:pt_1:eta_1:phi_1:scEt_1:scEta_1:scPhi_1:hltMatchBits_1/i:q_1/I:pt_2/F:eta_2:phi_2:scEt_2:scEta_2:scPhi_2:hltMatchBits_2/i:q_2/I:weight/F"

#ifdef ZeeData_is_TObject
inline
void fillData(ZeeData_t *data, const mithep::TEventInfo *info, const mithep::TDielectron *dielectron, 
              const UInt_t npv, const UInt_t nGoodPV, const UInt_t njets, const Double_t weight) { //, const UInt_t ncands) {

  //FIXME needs to be modified for control plot purposes
  double chIso_1 = dielectron->chIso_00_01_1 + dielectron->chIso_01_02_1
    + dielectron->chIso_02_03_1;
  double gammaIso_1 = dielectron->gammaIso_00_01_1 + dielectron->gammaIso_01_02_1
    + dielectron->gammaIso_02_03_1;
  double neuHadIso_1 = dielectron->neuHadIso_00_01_1 + dielectron->neuHadIso_01_02_1
    + dielectron->neuHadIso_02_03_1;
  data->relPFIso03_1  = chIso_1 + gammaIso_1 + neuHadIso_1;

  double chIso_2 = dielectron->chIso_00_01_2 + dielectron->chIso_01_02_2
    + dielectron->chIso_02_03_2;
  double gammaIso_2 = dielectron->gammaIso_00_01_2 + dielectron->gammaIso_01_02_2
    + dielectron->gammaIso_02_03_2;
  double neuHadIso_2 = dielectron->neuHadIso_00_01_2 + dielectron->neuHadIso_01_02_2
    + dielectron->neuHadIso_02_03_2;
  data->relPFIso03_2  = chIso_2 + gammaIso_2 + neuHadIso_2;

  data->sigiEtaiEta_1  = dielectron->sigiEtaiEta_1;
  data->sigiEtaiEta_2  = dielectron->sigiEtaiEta_2;
  data->deltaEtaIn_1   = dielectron->deltaEtaIn_1;
  data->deltaEtaIn_2   = dielectron->deltaEtaIn_2;
  data->deltaPhiIn_1   = dielectron->deltaPhiIn_1;
  data->deltaPhiIn_2   = dielectron->deltaPhiIn_2;
  data->HoverE_1       = dielectron->HoverE_1;
  data->HoverE_2       = dielectron->HoverE_2;

  data->d0_1             =  dielectron->d0_1;
  data->dz_1             =  dielectron->dz_1;
  data->ooeoop_1         =  1./dielectron->ecalE_1*fabs(1-dielectron->EoverP_1);
  data->nExpHitsInner_1  =  dielectron->nExpHitsInner_1;
  data->d0_2             =  dielectron->d0_2;
  data->dz_2             =  dielectron->dz_2;
  data->ooeoop_2         =  1./dielectron->ecalE_2*fabs(1-dielectron->EoverP_2);
  data->nExpHitsInner_2  =  dielectron->nExpHitsInner_2;

  data->runNum         = info->runNum;
  data->evtNum         = info->evtNum;
  data->lumiSec        = info->lumiSec;
  data->nTracks0       = 0;
  data->nCaloTowers0   = 0;
  data->nPV            = npv;
  data->nGoodPV        = nGoodPV;
  data->nJets          = njets;
  data->caloMEx        = 0;
  data->caloMEy        = 0;
  data->caloSumET      = 0;
  data->tcMEx          = 0; //info->trkMET * cos(info->trkMETphi);
  data->tcMEy          = 0; //info->trkMET * sin(info->trkMETphi);
  data->tcSumET        = info->trkSumET;
  data->pfMEx          = 0; //info->pfMET * cos(info->pfMETphi);
  data->pfMEy          = 0; //info->pfMET * sin(info->pfMETphi);
  data->pfSumET        = info->pfSumET;
  data->mass           = dielectron->mass;
  data->pt             = dielectron->pt;
  data->y              = dielectron->y;
  data->phi            = dielectron->phi; 
  // no endorsement that the 1st electron is the leading one
  data->pt_1           = dielectron->pt_1;
  data->ptUncorr_1           = dielectron->ptUncorr_1;
  data->eta_1          = dielectron->eta_1;
  data->phi_1          = dielectron->phi_1;
  data->scEt_1         = dielectron->scEt_1;
  data->scEta_1        = dielectron->scEta_1;
  data->scPhi_1        = dielectron->scPhi_1;
  data->hltMatchBits_1 = dielectron->hltMatchBits_1;
  data->q_1            = dielectron->q_1;
  data->pt_2           = dielectron->pt_2;
  data->ptUncorr_2           = dielectron->ptUncorr_2;
  data->eta_2          = dielectron->eta_2;
  data->phi_2          = dielectron->phi_2;
  data->scEt_2         = dielectron->scEt_2;
  data->scEta_2        = dielectron->scEta_2;
  data->scPhi_2        = dielectron->scPhi_2;
  data->hltMatchBits_2 = dielectron->hltMatchBits_2;
  data->q_2            = dielectron->q_2;
  data->weight         = weight;
  //data->nCands         = ncands;
}
#endif

#endif
