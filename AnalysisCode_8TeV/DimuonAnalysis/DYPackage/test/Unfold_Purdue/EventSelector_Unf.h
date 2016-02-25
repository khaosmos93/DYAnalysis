#ifndef EventSelector_Unf_h
#define EventSelector_Unf_h

#include <TROOT.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TSystem.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <vector>
#include <string>

//can I forward declare no?
#include "Muon.h"
#include "Electron.h"
#include "Dimuon.h"
#include "Dielectron.h"

#include "rochcor2012v2.C"

using namespace std;

class EventSelector_Unf : public TSelector {
public :
   //set up priors for the case when we do not use the ntuple weights
   double Sigma_DY1020;
   double Sigma_DY20;
   double Sigma_DY200;
   double Sigma_DY400;
   double Sigma_DY500;
   double Sigma_DY700;
   double Sigma_DY800;
   double Sigma_DY1000;

   double FilterEff_DY1020;
   double FilterEff_DY20;
   double FilterEff_DY200;
   double FilterEff_DY400;
   double FilterEff_DY500;
   double FilterEff_DY700;
   double FilterEff_DY800;
   double FilterEff_DY1000;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   TNamed*        ds;
   TNamed*        this_histogramThis;
   TNamed*        this_mode; 
   TNamed         *this_momCorrType;

   TString        dataset;
   string         histogramThis;
   string         mode;
   TString        momCorrType;

   //rochcorr
   rochcor2012    *rmcor;
   Int_t          _runopt;

   //define histo here
   TH1D*          Nntuple;

   TH1D*          hmeas;
   TH1D*          htrue;
   TH2D*          hden;
   TH1D*          hmeas2;
   TH1D*          htrue2;
   TH2D*          hden2;

   TH1D*          hmeas_Roch;
   TH1D*          htrue_Roch;
   TH2D*          hden_Roch;
   TH1D*          hmeas2_Roch;
   TH1D*          htrue2_Roch;
   TH2D*          hden2_Roch;

   TH1D*          hmeas_PU;
   TH1D*          htrue_PU;
   TH2D*          hden_PU;
   TH1D*          hmeas2_PU;
   TH1D*          htrue2_PU;
   TH2D*          hden2_PU;

   TH1D*          hmeas_corr;
   TH1D*          htrue_corr;
   TH2D*          hden_corr;
   TH1D*          hmeas2_corr;
   TH1D*          htrue2_corr;
   TH2D*          hden2_corr;

   TH1D*          hmeas2_24;
   TH1D*          htrue2_24;
   TH1D*          hmeas2_12;
   TH1D*          htrue2_12;
   TH2D*          hden2_156;

   TH1D*          hmeas2_24_Roch;
   TH1D*          htrue2_24_Roch;
   TH1D*          hmeas2_12_Roch;
   TH1D*          htrue2_12_Roch;
   TH2D*          hden2_156_Roch;

   TH1D*          hmeas2_24_PU;
   TH1D*          htrue2_24_PU;
   TH1D*          hmeas2_12_PU;
   TH1D*          htrue2_12_PU;
   TH2D*          hden2_156_PU;

   TH1D*          hmeas2_24_corr;
   TH1D*          htrue2_24_corr;
   TH1D*          hmeas2_12_corr;
   TH1D*          htrue2_12_corr;
   TH2D*          hden2_156_corr;

   Int_t          fNumberOfEvents; // Total number of events
   Int_t          fNumberOfGoodEvents; // Total number of good events
   Int_t          fTotalDataSize; // Sum of data size (in bytes) of all events

   TString        trigPaths[100];

   // Declaration of leaf types
   Int_t           runNum;
   Int_t           evtNum;
   Int_t           lumiBlock;

   Int_t           nPair;
   Double_t        ntupleWeight;
   Int_t           Njets;

   Double_t        pileUpReweight;
   Double_t        pileUpReweightIn;

   Int_t           hlt_ntrig;
   Bool_t          hlt_trigFired[1000000];
   Double_t        hlt_trigPt[1000000];
   Double_t        hlt_trigEta[1000000];
   Double_t        hlt_trigPhi[1000000];

   Int_t           GENnPair;
   Double_t        GENRapidity[2];   //[GENnPair]
   Double_t        GENInvMass[2];   //[GENnPair]
   Double_t        GENAngle[1];   //[GENnPair]
   Double_t        GENLepton1_phi[1];   //[GENnPair]
   Double_t        GENLepton1_eta[1];   //[GENnPair]
   Double_t        GENLepton1_pT[1];   //[GENnPair]
   Double_t        GENLepton1_Px[1];   //[GENnPair]
   Double_t        GENLepton1_Py[1];   //[GENnPair]
   Double_t        GENLepton1_Pz[1];   //[GENnPair]
   Int_t           GENLepton1_pdgID[2];   //[GENnPair]
   Int_t           GENLepton1_status[2];
   Int_t           GENLepton1_charge[1];   //[GENnPair]
   Double_t        GENLepton2_phi[1];   //[GENnPair]
   Double_t        GENLepton2_eta[1];   //[GENnPair]
   Double_t        GENLepton2_pT[1];   //[GENnPair]
   Double_t        GENLepton2_Px[1];   //[GENnPair]
   Double_t        GENLepton2_Py[1];   //[GENnPair]
   Double_t        GENLepton2_Pz[1];   //[GENnPair]

   Int_t           GENLepton2_pdgID[2];   //[GENnPair]
   Int_t           GENLepton2_status[2];

   std::vector<purdue::Muon>* muons;
   std::vector<purdue::Electron>* electrons;
   std::vector<purdue::Dimuon>* dimuons;
   std::vector<purdue::Dielectron>* dielectrons;

   TBranch* b_muons;
   TBranch* b_electrons;
   TBranch* b_dimuons;
   TBranch* b_dielectrons;

   // List of branches
   TBranch        *fEventSizeBranch;  // Pointer to the event.fEventSize branch
   TBranch        *b_runNum;   //!
   TBranch        *b_evtNum;   //!
   TBranch        *b_lumiBlock;   //!

   TBranch        *b_pileUpReweight;
   TBranch        *b_pileUpReweightIn;

   TBranch        *b_HLT_ntrig;
   TBranch        *b_HLT_trigFired;
   TBranch        *b_HLT_trigPt;
   TBranch        *b_HLT_trigEta;
   TBranch        *b_HLT_trigPhi;


   TBranch        *b_GENnPair;   //!
   TBranch        *b_GENInvMass;   //!
   TBranch        *b_GENRapidity;   //!

   TBranch        *b_GENAngle;   //!
   TBranch        *b_GENLepton1_phi;   //!
   TBranch        *b_GENLepton1_eta;   //!
   TBranch        *b_GENLepton1_pT;   //!
   TBranch        *b_GENLepton1_Px;   //!
   TBranch        *b_GENLepton1_Py;   //!
   TBranch        *b_GENLepton1_Pz;   //!
   TBranch        *b_GENLepton1_pdgID;   //!
   TBranch        *b_GENLepton1_status;
   TBranch        *b_GENLepton1_charge;   //!
   TBranch        *b_GENLepton2_phi;   //!
   TBranch        *b_GENLepton2_eta;   //!
   TBranch        *b_GENLepton2_pT;   //!
   TBranch        *b_GENLepton2_Px;   //!
   TBranch        *b_GENLepton2_Py;   //!
   TBranch        *b_GENLepton2_Pz;   //!
   TBranch        *b_GENLepton2_pdgID;   //!
   TBranch        *b_GENLepton2_status;

   EventSelector_Unf(TTree* =  0);
   virtual ~EventSelector_Unf();
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
   bool goodMuon(const purdue::Muon&);
   bool goodElectron(const purdue::Electron&);

   ClassDef(EventSelector_Unf,0);
};

#endif

#ifdef EventSelector_Unf_cxx
void EventSelector_Unf::Init(TTree *tree)
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
   //fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Muon",&muons,&b_muons);
   fChain->SetBranchAddress("Electron",&electrons,&b_electrons);
   fChain->SetBranchAddress("Dimuon",&dimuons,&b_dimuons);
   fChain->SetBranchAddress("Dielectron",&dielectrons,&b_dielectrons);

   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("evtNum", &evtNum, &b_evtNum);
   fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);

   fChain->SetBranchAddress("pileUpReweight", &pileUpReweight, &b_pileUpReweight);
   fChain->SetBranchAddress("pileUpReweightIn", &pileUpReweightIn, &b_pileUpReweightIn);

   fChain->SetBranchAddress("HLT_ntrig", &hlt_ntrig, &b_HLT_ntrig);
   fChain->SetBranchAddress("HLT_trigFired",hlt_trigFired,&b_HLT_trigFired);
   fChain->SetBranchAddress("HLT_trigPt", hlt_trigPt,&b_HLT_trigPt);
   fChain->SetBranchAddress("HLT_trigEta", hlt_trigEta, &b_HLT_trigEta);
   fChain->SetBranchAddress("HLT_trigPhi", hlt_trigPhi, &b_HLT_trigPhi);

   fChain->SetBranchAddress("GENnPair", &GENnPair, &b_GENnPair);
   fChain->SetBranchAddress("GENInvMass", GENInvMass, &b_GENInvMass);
   fChain->SetBranchAddress("GENRapidity", GENRapidity, &b_GENRapidity);

   fChain->SetBranchAddress("GENAngle", &GENAngle, &b_GENAngle);
   fChain->SetBranchAddress("GENLepton1_phi", &GENLepton1_phi, &b_GENLepton1_phi);
   fChain->SetBranchAddress("GENLepton1_eta", &GENLepton1_eta, &b_GENLepton1_eta);
   fChain->SetBranchAddress("GENLepton1_pT", &GENLepton1_pT, &b_GENLepton1_pT);
   fChain->SetBranchAddress("GENLepton1_Px", &GENLepton1_Px, &b_GENLepton1_Px);
   fChain->SetBranchAddress("GENLepton1_Py", &GENLepton1_Py, &b_GENLepton1_Py);
   fChain->SetBranchAddress("GENLepton1_Pz", &GENLepton1_Pz, &b_GENLepton1_Pz);
   fChain->SetBranchAddress("GENLepton1_pdgID", GENLepton1_pdgID, &b_GENLepton1_pdgID);
   fChain->SetBranchAddress("GENLepton1_status", GENLepton1_status, &b_GENLepton1_status);
   fChain->SetBranchAddress("GENLepton1_charge", &GENLepton1_charge, &b_GENLepton1_charge);
   fChain->SetBranchAddress("GENLepton2_phi", &GENLepton2_phi, &b_GENLepton2_phi);
   fChain->SetBranchAddress("GENLepton2_eta", &GENLepton2_eta, &b_GENLepton2_eta);
   fChain->SetBranchAddress("GENLepton2_pT", &GENLepton2_pT, &b_GENLepton2_pT);
   fChain->SetBranchAddress("GENLepton2_Px", &GENLepton2_Px, &b_GENLepton2_Px);
   fChain->SetBranchAddress("GENLepton2_Py", &GENLepton2_Py, &b_GENLepton2_Py);
   fChain->SetBranchAddress("GENLepton2_Pz", &GENLepton2_Pz, &b_GENLepton2_Pz);

   fChain->SetBranchAddress("GENLepton2_pdgID", GENLepton2_pdgID, &b_GENLepton2_pdgID);
   fChain->SetBranchAddress("GENLepton2_status", GENLepton2_status, &b_GENLepton2_status);

   // get dataset name
   dataset = fChain->GetTitle();

   //fChain->SetBranchStatus("Elec*",0);
   //fChain->SetBranchStatus("Nb*",0);
   //fChain->SetBranchStatus("JET*",0);
   //fChain->SetBranchStatus("GEN*",0);
   //fChain->SetBranchStatus("*elec*",0);

   //fChain->SetCacheSize(300000);

   fChain->AddBranchToCache("GEN*");
   fChain->AddBranchToCache("Muon*");
   fChain->AddBranchToCache("Electron*");
   fChain->AddBranchToCache("Di*");
}

Bool_t EventSelector_Unf::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef EventSelector_Unf_cxx
