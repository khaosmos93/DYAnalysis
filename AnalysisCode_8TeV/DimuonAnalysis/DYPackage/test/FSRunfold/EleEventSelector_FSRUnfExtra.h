#ifndef EleEventSelector_FSRUnfExtra_h
#define EleEventSelector_FSRUnfExtra_h

#include <TROOT.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

class EleEventSelector_FSRUnfExtra : public TSelector {
public :
   //set up priors for the case when we do not use the ntuple weights
   double Sigma_DY1020;
   double Sigma_DY20 ;
   double Sigma_DY200;
   double Sigma_DY400;
   double Sigma_DY500;
   double Sigma_DY700;
   double Sigma_DY800;
   double Sigma_DY1000;
   double Sigma_DY1500;
   double Sigma_DY2000;

   double FilterEff_DY1020;
   double FilterEff_DY20 ;
   double FilterEff_DY200;
   double FilterEff_DY400;
   double FilterEff_DY500;
   double FilterEff_DY700;
   double FilterEff_DY800;
   double FilterEff_DY1000;
   double FilterEff_DY1500;
   double FilterEff_DY2000;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   TNamed*        ds;
   TNamed*        this_useNtupleWeightFlag;
   TNamed*        this_histogramThis;
   TNamed         *this_momCorrType;
   TNamed*        this_dName;

   string        dName;
   string        useNtupleWeightFlag;
   string        histogramThis;
   TString        momCorrType;

   Double_t        pileUpReweight;

   //define histo here
   TH1D*          Nntuple;

   TH1D*          h1DFrac0;
   TH1D*          h1DFrac1;
   TH1D*          h1DFrac2;
   TH2D*          hFrac0;
   TH2D*          hFrac1;
   TH2D*          hFrac2;

   TH1D*          h1DFrac0_corr;
   TH1D*          h1DFrac1_corr;
   TH1D*          h1DFrac2_corr;
   TH2D*          hFrac0_corr;
   TH2D*          hFrac1_corr;
   TH2D*          hFrac2_corr;

   Int_t          fTotalDataSize; // Sum of data size (in bytes) of all events

   TString        trigPaths[100];

   // Declaration of leaf types
   Int_t           runNum;
   Int_t           evtNum;
   Double_t        rhoIso;   //!
   Int_t           lumiBlock;

   Int_t           nPair;
   Double_t        ntupleWeight;

   Double_t        pileUpReweightIn;

   Int_t           hlt_ntrig;
   std::vector<std::string>* hlt_trigName;
   Bool_t           hlt_trigFired[1000000];
   Int_t           hlt_trigType[1000000];
   Double_t        hlt_trigPt[1000000];
   Double_t        hlt_trigEta[1000000];
   Double_t        hlt_trigPhi[1000000];

   Double_t        InvMass[1000000];   //[nPair]
   Double_t        CosAngle[1000000];   //[nPair]
   Bool_t           isOppSign[1000000];   //[nPair]

   Double_t        Electron2_pt[1000000];   //[nPair_]
   Double_t        Electron2_eta[1000000];   //[nPair_]
   Double_t        Electron2_phi[1000000];   //[nPair_]
   Double_t        Electron2_px[1000000];   //[nPair_]
   Double_t        Electron2_py[1000000];   //[nPair_]
   Double_t        Electron2_pz[1000000];   //[nPair_]
   Double_t        Electron2_trkIso03[1000000];   //[nPair_]
   Double_t        Electron2_emIso03[1000000];   //[nPair_]
   Double_t        Electron2_hadIso03[1000000];   //[nPair_]
   Double_t        Electron2_d0BS[1000000];   //[nPair_]
   Double_t        Electron2_dzBS[1000000];   //[nPair_]
   Double_t        Electron2_d0VTX[1000000];   //[nPair_]
   Double_t        Electron2_dzVTX[1000000];   //[nPair_]
   Double_t        Electron2_scEt[1000000];   //[nPair_]
   Double_t        Electron2_scEta[1000000];   //[nPair_]
   Double_t        Electron2_scPhi[1000000];   //[nPair_]
   Double_t        Electron2_scE[1000000];   //[nPair_]
   Double_t        Electron2_ecalE[1000000];   //[nPair_]
   Double_t        Electron2_HoverE[1000000];   //[nPair_]
   Double_t        Electron2_EoverP[1000000];   //[nPair_]
   Double_t        Electron2_fBrem[1000000];   //[nPair_]
   Double_t        Electron2_deltaEtaIn[1000000];   //[nPair_]
   Double_t        Electron2_deltaPhiIn[1000000];   //[nPair_]
   Double_t        Electron2_sigiEtaiEta[1000000];   //[nPair_]
   Double_t        Electron2_nExpHitsInner[1000000];   //[nPair_]
   Double_t        Electron2_partnerDeltaCot[1000000];   //[nPair_]
   Double_t        Electron2_partnerDist[1000000];   //[nPair_]
   Double_t        Electron2_q[1000000];   //[nPair_]
   Double_t        Electron2_trkID[1000000];   //[nPair_]
   Double_t        Electron2_scID[1000000];   //[nPair_]
   Double_t        Electron2_isConv[1000000];   //[nPair_]
   Double_t        Electron2_typeBits[1000000];   //[nPair_]
   Double_t        Electron2_mva[1000000];   //[nPair_]
   Double_t        Electron2_chIso[1000000];   //[nPair_]
   Double_t        Electron2_gammaIso[1000000];   //[nPair_]
   Double_t        Electron2_neuHadIso[1000000];   //[nPair_]
   Double_t        Electron2_AEffUser[1000000];   //[nPair_]

   Double_t        Electron1_pt[1000000];   //[nPair_]
   Double_t        Electron1_eta[1000000];   //[nPair_]
   Double_t        Electron1_phi[1000000];   //[nPair_]
   Double_t        Electron1_px[1000000];   //[nPair_]
   Double_t        Electron1_py[1000000];   //[nPair_]
   Double_t        Electron1_pz[1000000];   //[nPair_]
   Double_t        Electron1_trkIso03[1000000];   //[nPair_]
   Double_t        Electron1_emIso03[1000000];   //[nPair_]
   Double_t        Electron1_hadIso03[1000000];   //[nPair_]
   Double_t        Electron1_d0BS[1000000];   //[nPair_]
   Double_t        Electron1_dzBS[1000000];   //[nPair_]
   Double_t        Electron1_d0VTX[1000000];   //[nPair_]
   Double_t        Electron1_dzVTX[1000000];   //[nPair_]
   Double_t        Electron1_scEt[1000000];   //[nPair_]
   Double_t        Electron1_scEta[1000000];   //[nPair_]
   Double_t        Electron1_scPhi[1000000];   //[nPair_]
   Double_t        Electron1_scE[1000000];   //[nPair_]
   Double_t        Electron1_ecalE[1000000];   //[nPair_]
   Double_t        Electron1_HoverE[1000000];   //[nPair_]
   Double_t        Electron1_EoverP[1000000];   //[nPair_]
   Double_t        Electron1_fBrem[1000000];   //[nPair_]
   Double_t        Electron1_deltaEtaIn[1000000];   //[nPair_]
   Double_t        Electron1_deltaPhiIn[1000000];   //[nPair_]
   Double_t        Electron1_sigiEtaiEta[1000000];   //[nPair_]
   Double_t        Electron1_nExpHitsInner[1000000];   //[nPair_]
   Double_t        Electron1_partnerDeltaCot[1000000];   //[nPair_]
   Double_t        Electron1_partnerDist[1000000];   //[nPair_]
   Double_t        Electron1_q[1000000];   //[nPair_]
   Double_t        Electron1_trkID[1000000];   //[nPair_]
   Double_t        Electron1_scID[1000000];   //[nPair_]
   Double_t        Electron1_isConv[1000000];   //[nPair_]
   Double_t        Electron1_typeBits[1000000];   //[nPair_]
   Double_t        Electron1_mva[1000000];   //[nPair_]
   Double_t        Electron1_chIso[1000000];   //[nPair_]
   Double_t        Electron1_gammaIso[1000000];   //[nPair_]
   Double_t        Electron1_neuHadIso[1000000];   //[nPair_]
   Double_t        Electron1_AEffUser[1000000];   //[nPair_]


   Int_t           GENnPair;
   Double_t        GENRapidity[1000000];   //[GENnPair]
   Double_t        GENInvMass[1000000];   //[GENnPair]
   Double_t        GENAngle[100];   //[GENnPair]
   Int_t           GENisOppSign[100];   //[GENnPair]
   Double_t        GENElectron1_phi[100];   //[GENnPair]
   Double_t        GENElectron1_eta[100];   //[GENnPair]
   Double_t        GENElectron1_pT[100];   //[GENnPair]
   Double_t        GENElectron1_Px[100];   //[GENnPair]
   Double_t        GENElectron1_Py[100];   //[GENnPair]
   Double_t        GENElectron1_Pz[100];   //[GENnPair]
   Int_t        GENElectron1_mother[1000000];   //[GENnPair]
   Int_t        GENElectron1_status[1000000];
   Int_t           GENElectron1_charge[100];   //[GENnPair]
   Double_t        GENElectron2_phi[100];   //[GENnPair]
   Double_t        GENElectron2_eta[100];   //[GENnPair]
   Double_t        GENElectron2_pT[100];   //[GENnPair]
   Double_t        GENElectron2_Px[100];   //[GENnPair]
   Double_t        GENElectron2_Py[100];   //[GENnPair]
   Double_t        GENElectron2_Pz[100];   //[GENnPair]

   Int_t        GENElectron2_mother[1000000];   //[GENnPair]
   Int_t        GENElectron2_status[1000000];

   // List of branches
   TBranch    *fEleEventSizeBranch;  // Pointer to the event.fEleEventSize branch
   TBranch        *b_runNum;   //!
   TBranch        *b_evtNum;   //!
   TBranch        *b_rhoIso;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_nPair;   //!
   TBranch        *b_ntupleWeight;   //!
   TBranch        *b_pileUpReweight;
   TBranch        *b_pileUpReweightIn;

   TBranch        *b_HLT_ntrig;
   TBranch        *b_HLT_trigName;
   TBranch        *b_HLT_trigFired;
   TBranch        *b_HLT_trigType;
   TBranch        *b_HLT_trigPt;
   TBranch        *b_HLT_trigEta;
   TBranch        *b_HLT_trigPhi;

   TBranch        *b_InvMass;   //!
   TBranch        *b_CosAngle;   //!
   TBranch        *b_isOppSign;   //!
  
   TBranch        *b_Electron2_pt;   //!
   TBranch        *b_Electron2_eta;   //!
   TBranch        *b_Electron2_phi;   //!
   TBranch        *b_Electron2_px;   //!
   TBranch        *b_Electron2_py;   //! 
   TBranch        *b_Electron2_pz;   //! 
   TBranch        *b_Electron2_trkIso03;   //!
   TBranch        *b_Electron2_emIso03;   //!
   TBranch        *b_Electron2_hadIso03;   //!
   TBranch        *b_Electron2_d0BS;   //!
   TBranch        *b_Electron2_dzBS;   //!
   TBranch        *b_Electron2_d0VTX;   //!
   TBranch        *b_Electron2_dzVTX;   //!   
   TBranch        *b_Electron2_scEt;   //!
   TBranch        *b_Electron2_scEta;   //!
   TBranch        *b_Electron2_scPhi;   //!
   TBranch        *b_Electron2_scE;   //!
   TBranch        *b_Electron2_ecalE;   //!
   TBranch        *b_Electron2_HoverE;   //!
   TBranch        *b_Electron2_EoverP;   //!
   TBranch        *b_Electron2_fBrem;   //!
   TBranch        *b_Electron2_deltaEtaIn;   //!
   TBranch        *b_Electron2_deltaPhiIn;   //!
   TBranch        *b_Electron2_sigiEtaiEta;   //!
   TBranch        *b_Electron2_nExpHitsInner;   //!
   TBranch        *b_Electron2_partnerDeltaCot;   //!
   TBranch        *b_Electron2_partnerDist;   //!
   TBranch        *b_Electron2_q;   //!
   TBranch        *b_Electron2_trkID;   //!
   TBranch        *b_Electron2_scID;   //!
   TBranch        *b_Electron2_isConv;   //!
   TBranch        *b_Electron2_typeBits;   //!
   TBranch        *b_Electron2_mva;   //!
   TBranch        *b_Electron2_chIso;   //!
   TBranch        *b_Electron2_gammaIso;   //!
   TBranch        *b_Electron2_neuHadIso;   //!
   TBranch        *b_Electron2_AEffUser;   //!

   TBranch        *b_Electron1_pt;   //!
   TBranch        *b_Electron1_eta;   //!
   TBranch        *b_Electron1_phi;   //!
   TBranch        *b_Electron1_px;   //!
   TBranch        *b_Electron1_py;   //! 
   TBranch        *b_Electron1_pz;   //! 
   TBranch        *b_Electron1_trkIso03;   //!
   TBranch        *b_Electron1_emIso03;   //!
   TBranch        *b_Electron1_hadIso03;   //!
   TBranch        *b_Electron1_d0BS;   //!
   TBranch        *b_Electron1_dzBS;   //!
   TBranch        *b_Electron1_d0VTX;   //!
   TBranch        *b_Electron1_dzVTX;   //!   
   TBranch        *b_Electron1_scEt;   //!
   TBranch        *b_Electron1_scEta;   //!
   TBranch        *b_Electron1_scPhi;   //!
   TBranch        *b_Electron1_scE;   //!
   TBranch        *b_Electron1_ecalE;   //!
   TBranch        *b_Electron1_HoverE;   //!
   TBranch        *b_Electron1_EoverP;   //!
   TBranch        *b_Electron1_fBrem;   //!
   TBranch        *b_Electron1_deltaEtaIn;   //!
   TBranch        *b_Electron1_deltaPhiIn;   //!
   TBranch        *b_Electron1_sigiEtaiEta;   //!
   TBranch        *b_Electron1_nExpHitsInner;   //!
   TBranch        *b_Electron1_partnerDeltaCot;   //!
   TBranch        *b_Electron1_partnerDist;   //!
   TBranch        *b_Electron1_q;   //!
   TBranch        *b_Electron1_trkID;   //!
   TBranch        *b_Electron1_scID;   //!
   TBranch        *b_Electron1_isConv;   //!
   TBranch        *b_Electron1_typeBits;   //!
   TBranch        *b_Electron1_mva;   //!
   TBranch        *b_Electron1_chIso;   //!
   TBranch        *b_Electron1_gammaIso;   //!
   TBranch        *b_Electron1_neuHadIso;   //!
   TBranch        *b_Electron1_AEffUser;   //!
 
   TBranch        *b_GENnPair;   //!
   TBranch        *b_GENInvMass;   //!
   TBranch        *b_GENRapidity;   //!

   TBranch        *b_GENAngle;   //!
   TBranch        *b_GENisOppSign;   //!
   TBranch        *b_GENElectron1_phi;   //!
   TBranch        *b_GENElectron1_eta;   //!
   TBranch        *b_GENElectron1_pT;   //!
   TBranch        *b_GENElectron1_Px;   //!
   TBranch        *b_GENElectron1_Py;   //!
   TBranch        *b_GENElectron1_Pz;   //!
   TBranch        *b_GENElectron1_mother;   //!
   TBranch        *b_GENElectron1_status;
   TBranch        *b_GENElectron1_charge;   //!
   TBranch        *b_GENElectron2_phi;   //!
   TBranch        *b_GENElectron2_eta;   //!
   TBranch        *b_GENElectron2_pT;   //!
   TBranch        *b_GENElectron2_Px;   //!
   TBranch        *b_GENElectron2_Py;   //!
   TBranch        *b_GENElectron2_Pz;   //!
   TBranch        *b_GENElectron2_mother;   //!
   TBranch        *b_GENElectron2_status;
/*
   TBranch        *b_GENElectron2_charge;   //!
*/

   EleEventSelector_FSRUnfExtra(TTree* =  0);
   virtual ~EleEventSelector_FSRUnfExtra();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree*);
   virtual void    SlaveBegin(TTree*);
   virtual void    Init(TTree*);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(EleEventSelector_FSRUnfExtra,0);
};

#endif

#ifdef EleEventSelector_FSRUnfExtra_cxx
void EleEventSelector_FSRUnfExtra::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   //fChain = dynamic_cast<TChain *>(tree);
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("evtNum", &evtNum, &b_evtNum);
   fChain->SetBranchAddress("rhoIso", &rhoIso, &b_rhoIso);
   fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
   fChain->SetBranchAddress("nPair", &nPair, &b_nPair);
   fChain->SetBranchAddress("weight", &ntupleWeight, &b_ntupleWeight);
   fChain->SetBranchAddress("pileUpReweight", &pileUpReweight, &b_pileUpReweight);
   fChain->SetBranchAddress("pileUpReweightIn", &pileUpReweightIn, &b_pileUpReweightIn);

   fChain->SetBranchAddress("HLT_ntrig", &hlt_ntrig, &b_HLT_ntrig);
   fChain->SetBranchAddress("HLT_trigName", &hlt_trigName, &b_HLT_trigName);
   fChain->SetBranchAddress("HLT_trigFired",hlt_trigFired,&b_HLT_trigFired);
   fChain->SetBranchAddress("HLT_trigType", hlt_trigType, &b_HLT_trigType);
   fChain->SetBranchAddress("HLT_trigPt", hlt_trigPt,&b_HLT_trigPt);
   fChain->SetBranchAddress("HLT_trigEta", hlt_trigEta, &b_HLT_trigEta);
   fChain->SetBranchAddress("HLT_trigPhi", hlt_trigPhi, &b_HLT_trigPhi);

   fChain->SetBranchAddress("InvMass", InvMass, &b_InvMass);
   fChain->SetBranchAddress("CosAngle", CosAngle, &b_CosAngle);
   fChain->SetBranchAddress("isOppSign", isOppSign, &b_isOppSign);
 
   fChain->SetBranchAddress("Electron2_pt", Electron2_pt, &b_Electron2_pt);
   fChain->SetBranchAddress("Electron2_eta", Electron2_eta, &b_Electron2_eta);
   fChain->SetBranchAddress("Electron2_phi", Electron2_phi, &b_Electron2_phi);
   fChain->SetBranchAddress("Electron2_px", Electron2_px, &b_Electron2_px);
   fChain->SetBranchAddress("Electron2_py", Electron2_py, &b_Electron2_py);
   fChain->SetBranchAddress("Electron2_pz", Electron2_pz, &b_Electron2_pz);
   fChain->SetBranchAddress("Electron2_trkIso03", Electron2_trkIso03, &b_Electron2_trkIso03);
   fChain->SetBranchAddress("Electron2_emIso03", Electron2_emIso03, &b_Electron2_emIso03);
   fChain->SetBranchAddress("Electron2_hadIso03", Electron2_hadIso03, &b_Electron2_hadIso03);
   fChain->SetBranchAddress("Electron2_d0BS", Electron2_d0BS, &b_Electron2_d0BS);
   fChain->SetBranchAddress("Electron2_dzBS", Electron2_dzBS, &b_Electron2_dzBS);
   fChain->SetBranchAddress("Electron2_d0VTX", Electron2_d0VTX, &b_Electron2_d0VTX);
   fChain->SetBranchAddress("Electron2_dzVTX", Electron2_dzVTX, &b_Electron2_dzVTX);
   fChain->SetBranchAddress("Electron2_scEt", Electron2_scEt, &b_Electron2_scEt);
   fChain->SetBranchAddress("Electron2_scEta", Electron2_scEta, &b_Electron2_scEta);
   fChain->SetBranchAddress("Electron2_scPhi", Electron2_scPhi, &b_Electron2_scPhi);
   fChain->SetBranchAddress("Electron2_scE", Electron2_scE, &b_Electron2_scE);
   fChain->SetBranchAddress("Electron2_ecalE", Electron2_ecalE, &b_Electron2_ecalE);
   fChain->SetBranchAddress("Electron2_HoverE", Electron2_HoverE, &b_Electron2_HoverE);
   fChain->SetBranchAddress("Electron2_EoverP", Electron2_EoverP, &b_Electron2_EoverP);
   fChain->SetBranchAddress("Electron2_fBrem", Electron2_fBrem, &b_Electron2_fBrem);
   fChain->SetBranchAddress("Electron2_deltaEtaIn", Electron2_deltaEtaIn, &b_Electron2_deltaEtaIn);
   fChain->SetBranchAddress("Electron2_deltaPhiIn", Electron2_deltaPhiIn, &b_Electron2_deltaPhiIn);
   fChain->SetBranchAddress("Electron2_sigiEtaiEta", Electron2_sigiEtaiEta, &b_Electron2_sigiEtaiEta);
   fChain->SetBranchAddress("Electron2_nExpHitsInner", Electron2_nExpHitsInner, &b_Electron2_nExpHitsInner);
   fChain->SetBranchAddress("Electron2_partnerDeltaCot", Electron2_partnerDeltaCot, &b_Electron2_partnerDeltaCot);
   fChain->SetBranchAddress("Electron2_partnerDist", Electron2_partnerDist, &b_Electron2_partnerDist);
   fChain->SetBranchAddress("Electron2_q", Electron2_q, &b_Electron2_q);
   fChain->SetBranchAddress("Electron2_trkID", Electron2_trkID, &b_Electron2_trkID);
   fChain->SetBranchAddress("Electron2_scID", Electron2_scID, &b_Electron2_scID);
   fChain->SetBranchAddress("Electron2_isConv", Electron2_isConv, &b_Electron2_isConv);
   fChain->SetBranchAddress("Electron2_typeBits", Electron2_typeBits, &b_Electron2_typeBits);
   fChain->SetBranchAddress("Electron2_mva", Electron2_mva, &b_Electron2_mva);
   fChain->SetBranchAddress("Electron2_chIso", Electron2_chIso, &b_Electron2_chIso);
   fChain->SetBranchAddress("Electron2_gammaIso", Electron2_gammaIso, &b_Electron2_gammaIso);
   fChain->SetBranchAddress("Electron2_neuHadIso", Electron2_neuHadIso, &b_Electron2_neuHadIso);
   fChain->SetBranchAddress("Electron2_AEffUser", Electron2_AEffUser, &b_Electron2_AEffUser);

   fChain->SetBranchAddress("Electron1_pt", Electron1_pt, &b_Electron1_pt);
   fChain->SetBranchAddress("Electron1_eta", Electron1_eta, &b_Electron1_eta);
   fChain->SetBranchAddress("Electron1_phi", Electron1_phi, &b_Electron1_phi);
   fChain->SetBranchAddress("Electron1_px", Electron1_px, &b_Electron1_px);
   fChain->SetBranchAddress("Electron1_py", Electron1_py, &b_Electron1_py);
   fChain->SetBranchAddress("Electron1_pz", Electron1_pz, &b_Electron1_pz);
   fChain->SetBranchAddress("Electron1_trkIso03", Electron1_trkIso03, &b_Electron1_trkIso03);
   fChain->SetBranchAddress("Electron1_emIso03", Electron1_emIso03, &b_Electron1_emIso03);
   fChain->SetBranchAddress("Electron1_hadIso03", Electron1_hadIso03, &b_Electron1_hadIso03);
   fChain->SetBranchAddress("Electron1_d0BS", Electron1_d0BS, &b_Electron1_d0BS);
   fChain->SetBranchAddress("Electron1_dzBS", Electron1_dzBS, &b_Electron1_dzBS);
   fChain->SetBranchAddress("Electron1_d0VTX", Electron1_d0VTX, &b_Electron1_d0VTX);
   fChain->SetBranchAddress("Electron1_dzVTX", Electron1_dzVTX, &b_Electron1_dzVTX);
   fChain->SetBranchAddress("Electron1_scEt", Electron1_scEt, &b_Electron1_scEt);
   fChain->SetBranchAddress("Electron1_scEta", Electron1_scEta, &b_Electron1_scEta);
   fChain->SetBranchAddress("Electron1_scPhi", Electron1_scPhi, &b_Electron1_scPhi);
   fChain->SetBranchAddress("Electron1_scE", Electron1_scE, &b_Electron1_scE);
   fChain->SetBranchAddress("Electron1_ecalE", Electron1_ecalE, &b_Electron1_ecalE);
   fChain->SetBranchAddress("Electron1_HoverE", Electron1_HoverE, &b_Electron1_HoverE);
   fChain->SetBranchAddress("Electron1_EoverP", Electron1_EoverP, &b_Electron1_EoverP);
   fChain->SetBranchAddress("Electron1_fBrem", Electron1_fBrem, &b_Electron1_fBrem);
   fChain->SetBranchAddress("Electron1_deltaEtaIn", Electron1_deltaEtaIn, &b_Electron1_deltaEtaIn);
   fChain->SetBranchAddress("Electron1_deltaPhiIn", Electron1_deltaPhiIn, &b_Electron1_deltaPhiIn);
   fChain->SetBranchAddress("Electron1_sigiEtaiEta", Electron1_sigiEtaiEta, &b_Electron1_sigiEtaiEta);
   fChain->SetBranchAddress("Electron1_nExpHitsInner", Electron1_nExpHitsInner, &b_Electron1_nExpHitsInner);
   fChain->SetBranchAddress("Electron1_partnerDeltaCot", Electron1_partnerDeltaCot, &b_Electron1_partnerDeltaCot);
   fChain->SetBranchAddress("Electron1_partnerDist", Electron1_partnerDist, &b_Electron1_partnerDist);
   fChain->SetBranchAddress("Electron1_q", Electron1_q, &b_Electron1_q);
   fChain->SetBranchAddress("Electron1_trkID", Electron1_trkID, &b_Electron1_trkID);
   fChain->SetBranchAddress("Electron1_scID", Electron1_scID, &b_Electron1_scID);
   fChain->SetBranchAddress("Electron1_isConv", Electron1_isConv, &b_Electron1_isConv);
   fChain->SetBranchAddress("Electron1_typeBits", Electron1_typeBits, &b_Electron1_typeBits);
   fChain->SetBranchAddress("Electron1_mva", Electron1_mva, &b_Electron1_mva);
   fChain->SetBranchAddress("Electron1_chIso", Electron1_chIso, &b_Electron1_chIso);
   fChain->SetBranchAddress("Electron1_gammaIso", Electron1_gammaIso, &b_Electron1_gammaIso);
   fChain->SetBranchAddress("Electron1_neuHadIso", Electron1_neuHadIso, &b_Electron1_neuHadIso);
   fChain->SetBranchAddress("Electron1_AEffUser", Electron1_AEffUser, &b_Electron1_AEffUser);

   fChain->SetBranchAddress("GENnPair", &GENnPair, &b_GENnPair);
   fChain->SetBranchAddress("GENInvMass", GENInvMass, &b_GENInvMass);
   fChain->SetBranchAddress("GENRapidity", GENRapidity, &b_GENRapidity);
   fChain->SetBranchAddress("GENAngle", &GENAngle, &b_GENAngle);
   fChain->SetBranchAddress("GENisOppSign", &GENisOppSign, &b_GENisOppSign);

   fChain->SetBranchAddress("GENElectron1_phi", &GENElectron1_phi, &b_GENElectron1_phi);
   fChain->SetBranchAddress("GENElectron1_eta", &GENElectron1_eta, &b_GENElectron1_eta);
   fChain->SetBranchAddress("GENElectron1_pT", &GENElectron1_pT, &b_GENElectron1_pT);
   fChain->SetBranchAddress("GENElectron1_Px", &GENElectron1_Px, &b_GENElectron1_Px);
   fChain->SetBranchAddress("GENElectron1_Py", &GENElectron1_Py, &b_GENElectron1_Py);
   fChain->SetBranchAddress("GENElectron1_Pz", &GENElectron1_Pz, &b_GENElectron1_Pz);
   fChain->SetBranchAddress("GENElectron1_mother", GENElectron1_mother, &b_GENElectron1_mother);
   fChain->SetBranchAddress("GENElectron1_status", GENElectron1_status, &b_GENElectron1_status);
   fChain->SetBranchAddress("GENElectron1_charge", &GENElectron1_charge, &b_GENElectron1_charge);

   fChain->SetBranchAddress("GENElectron2_phi", &GENElectron2_phi, &b_GENElectron2_phi);
   fChain->SetBranchAddress("GENElectron2_eta", &GENElectron2_eta, &b_GENElectron2_eta);
   fChain->SetBranchAddress("GENElectron2_pT", &GENElectron2_pT, &b_GENElectron2_pT);
   fChain->SetBranchAddress("GENElectron2_Px", &GENElectron2_Px, &b_GENElectron2_Px);
   fChain->SetBranchAddress("GENElectron2_Py", &GENElectron2_Py, &b_GENElectron2_Py);
   fChain->SetBranchAddress("GENElectron2_Pz", &GENElectron2_Pz, &b_GENElectron2_Pz);

   fChain->SetBranchAddress("GENElectron2_mother", GENElectron2_mother, &b_GENElectron2_mother);
   fChain->SetBranchAddress("GENElectron2_status", GENElectron2_status, &b_GENElectron2_status);

   fChain->AddBranchToCache("Electron1_*");
   fChain->AddBranchToCache("Electron2_*");
   fChain->AddBranchToCache("runNum");
   fChain->AddBranchToCache("evtNum");
   fChain->AddBranchToCache("rhoIso");
   fChain->AddBranchToCache("ntupleWeight");
   fChain->AddBranchToCache("HLT*");
   fChain->AddBranchToCache("CosAngle");
   fChain->AddBranchToCache("vtxProb");
   fChain->AddBranchToCache("InvMass");
   fChain->AddBranchToCache("GEN*");

}

Bool_t EleEventSelector_FSRUnfExtra::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef EleEventSelector_FSRUnfExtra_cxx
