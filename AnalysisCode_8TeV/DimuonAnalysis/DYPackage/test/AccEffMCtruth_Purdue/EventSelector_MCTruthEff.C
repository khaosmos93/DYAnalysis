#define EventSelector_MCTruthEff_cxx
// The class definition in EventSelector_MCTruthEff.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("EventSelector_MCTruthEff.C")
// Root > T->Process("EventSelector_MCTruthEff.C","some options")
// Root > T->Process("EventSelector_MCTruthEff.C+")
//

#include "EventSelector_MCTruthEff.h"
#include "../ControlPlots/efficiencyWeightToBin2012.C"
#include <iostream>
#include <TProof.h>
#include <TProofServ.h>
#include <TDSet.h>
#include <TEventList.h>
#include <TParameter.h>
#include <TMath.h>


double deltaPhi(double phi1, double phi2) {
  double result = phi1 - phi2;
  while (result > M_PI) result -= 2*M_PI;
  while (result <= -M_PI) result += 2*M_PI;
  return result;
}

double deltaR(double eta1, double phi1, double eta2, double phi2) {
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return sqrt(deta*deta + dphi*dphi);
}

EventSelector_MCTruthEff::EventSelector_MCTruthEff(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
{
 
  //trigPaths[0] = "HLT_DoubleMu6";
  //trigPaths[1] = "HLT_Mu13_Mu8";
  //trigPaths[0] = "HLT_Mu13_Mu8";
  trigPaths[0] = "";

  //alternative to ntuple weights
  //Each prior is defined using the following formula:
  //Prior_<MC_sample> = (Cross section*Filter efficiency/Number of GEN events)|<MC sample>
  //this information is manually retrieved for each sample from DAS: https://cmsweb-testbed.cern.ch/das/
  //The first two numbers are provided in the cfg file for "parent" datasets.
  //Alternatively, cross section can be obtainedi from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
  //or: http://cms.cern.ch/iCMS/jsp/mcprod/admin/requestmanagement.jsp?campid=Summer12 
  Sigma_DY1020 = 3708.0;
  Sigma_DY20   = 1871.0;//-1.485;
  Sigma_DY200  = 1.485;// WASNT-0.1086;
  Sigma_DY400  = 0.1086;//-0.04415;
  Sigma_DY500  = 0.04415;//WASNT -0.01024;
  Sigma_DY700  = 0.01024;//-0.005491;
  Sigma_DY800  = 0.005491;//-0.001796;
  Sigma_DY1000 = 0.001796;

  FilterEff_DY1020 = 1;
  FilterEff_DY20   = 1.;
  FilterEff_DY200  = 1.;
  FilterEff_DY400  = 1.;
  FilterEff_DY500  = 1.;
  FilterEff_DY700  = 1.;
  FilterEff_DY800  = 1.;
  FilterEff_DY1000 = 1.;

}

EventSelector_MCTruthEff::~EventSelector_MCTruthEff() { }


void EventSelector_MCTruthEff::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_MCTruthEff::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //pT binning 
   double pt_xbin[10] = {8,17,26,32,44,55,70,100,220,400};

   //eta binning 
   double eta_xbin[16] = {-2.4,-1.9,-1.5,-1.1,-0.9,-0.7,-0.5,-0.1,0.1,0.5,0.7,0.9,1.1,1.5,1.9,2.4};

   //pT-eta binning
   double eta_xbin2[20] = {-2.4,-2.1,-1.9,-1.5,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.5,1.9,2.1,2.4};
   double pt_xbin2[8] = {8,17,30,40,50,70,250,500};


   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   //1D 
   hacc = new TH1D("hacc", "hacc", nbin, mass_xbin);
   hsim = new TH1D("hsim", "hsim", nbin, mass_xbin);
   haccFSR = new TH1D("haccFSR", "haccFSR", nbin, mass_xbin);
   hsel = new TH1D("hsel", "hsel", nbin, mass_xbin);
   hfull = new TH1D("hfull", "hfull", nbin, mass_xbin);

   //FIXME
   hselpre = new TH1D("hselpre", "hselpre", nbin, mass_xbin);

   hacc_PU = new TH1D("hacc_PU", "hacc_PU", nbin, mass_xbin);
   hsim_PU = new TH1D("hsim_PU", "hsim_PU", nbin, mass_xbin);
   haccFSR_PU = new TH1D("haccFSR_PU", "haccFSR_PU", nbin, mass_xbin);
   hsel_PU = new TH1D("hsel_PU", "hsel_PU", nbin, mass_xbin);
   hfull_PU = new TH1D("hfull_PU", "hfull_PU", nbin, mass_xbin);

   hacc_corr = new TH1D("hacc_corr", "hacc_corr", nbin, mass_xbin);
   hsim_corr = new TH1D("hsim_corr", "hsim_corr", nbin, mass_xbin);
   haccFSR_corr = new TH1D("haccFSR_corr", "haccFSR_corr", nbin, mass_xbin);
   hsel_corr = new TH1D("hsel_corr", "hsel_corr", nbin, mass_xbin);
   hfull_corr = new TH1D("hfull_corr", "hfull_corr", nbin, mass_xbin);

   //PT or eta distributions
   hacc_pT = new TH1D("hacc_pT", "hacc_pT", 9, pt_xbin);
   hsim_pT = new TH1D("hsim_pT", "hsim_pT", 9, pt_xbin);
   haccFSR_pT = new TH1D("haccFSR_pT", "haccFSR_pT", 9, pt_xbin);
   hsel_pT = new TH1D("hsel_pT", "hsel_pT", 9, pt_xbin);
   hfull_pT = new TH1D("hfull_pT", "hfull_pT", 9, pt_xbin);
  
   hacc_pT_PU = new TH1D("hacc_pT_PU", "hacc_pT_PU",9, pt_xbin);
   hsim_pT_PU = new TH1D("hsim_pT_PU", "hsim_pT_PU", 9, pt_xbin);
   haccFSR_pT_PU = new TH1D("haccFSR_pT_PU", "haccFSR_pT_PU", 9, pt_xbin);
   hsel_pT_PU = new TH1D("hsel_pT_PU", "hsel_pT_PU", 9, pt_xbin);
   hfull_pT_PU = new TH1D("hfull_pT_PU", "hfull_pT_PU", 9, pt_xbin);

   hacc_pT_corr = new TH1D("hacc_pT_corr", "hacc_pT_corr",9, pt_xbin);
   hsim_pT_corr = new TH1D("hsim_pT_corr", "hsim_pT_corr", 9, pt_xbin);
   haccFSR_pT_corr = new TH1D("haccFSR_pT_corr", "haccFSR_pT_corr", 9, pt_xbin);
   hsel_pT_corr = new TH1D("hsel_pT_corr", "hsel_pT_corr", 9, pt_xbin);
   hfull_pT_corr = new TH1D("hfull_pT_corr", "hfull_pT_corr",9, pt_xbin);

   hacc_Eta = new TH1D("hacc_Eta", "hacc_Eta", 15, eta_xbin);
   hsim_Eta = new TH1D("hsim_Eta", "hsim_Eta", 15, eta_xbin);
   haccFSR_Eta = new TH1D("haccFSR_Eta", "haccFSR_Eta", 15, eta_xbin);
   hsel_Eta = new TH1D("hsel_Eta", "hsel_Eta", 15, eta_xbin);
   hfull_Eta = new TH1D("hfull_Eta", "hfull_Eta", 15, eta_xbin);

   hacc_Eta_PU = new TH1D("hacc_Eta_PU", "hacc_Eta_PU",15, eta_xbin);
   hsim_Eta_PU = new TH1D("hsim_Eta_PU", "hsim_Eta_PU", 15, eta_xbin);
   haccFSR_Eta_PU = new TH1D("haccFSR_Eta_PU", "haccFSR_Eta_PU", 15, eta_xbin);
   hsel_Eta_PU = new TH1D("hsel_Eta_PU", "hsel_Eta_PU", 15, eta_xbin);
   hfull_Eta_PU = new TH1D("hfull_Eta_PU", "hfull_Eta_PU", 15, eta_xbin);

   hacc_Eta_corr = new TH1D("hacc_Eta_corr", "hacc_Eta_corr",15, eta_xbin);
   hsim_Eta_corr = new TH1D("hsim_Eta_corr", "hsim_Eta_corr", 15, eta_xbin);
   haccFSR_Eta_corr = new TH1D("haccFSR_Eta_corr", "haccFSR_Eta_corr", 15, eta_xbin);
   hsel_Eta_corr = new TH1D("hsel_Eta_corr", "hsel_Eta_corr", 15, eta_xbin);
   hfull_Eta_corr = new TH1D("hfull_Eta_corr", "hfull_Eta_corr",15, eta_xbin);

   //pT-eta
   hacc_pTEta = new TH2D("hacc_pTEta", "hacc_pTEta", 7, pt_xbin2, 19, eta_xbin2);
   hsim_pTEta = new TH2D("hsim_pTEta", "hsim_pTEta", 7, pt_xbin2, 19, eta_xbin2);
   haccFSR_pTEta = new TH2D("haccFSR_pTEta", "haccFSR_pTEta",7, pt_xbin2, 19, eta_xbin2);
   hsel_pTEta = new TH2D("hsel_pTEta", "hsel_pTEta", 7, pt_xbin2, 19, eta_xbin2);
   hfull_pTEta = new TH2D("hfull_pTEta", "hfull_pTEta",7, pt_xbin2, 19, eta_xbin2);

   //2D sliced
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 132 bins
   const int nbin2 = 132;
   double mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   fAbsRap_full = new TH1D("rap_full", "rap_full", nbin2, 0, 132);
   fAbsRap_acc = new TH1D("rap_acc", "rap_acc", nbin2, 0, 132);
   fAbsRap_accFSR = new TH1D("rap_accFSR", "rap_accFSR", nbin2, 0, 132);
   fAbsRap_sim = new TH1D("rap_sim", "rap_sim", nbin2, 0, 132);
   fAbsRap_sel = new TH1D("rap_sel", "rap_sel", nbin2, 0, 132);

   //FIXME
   fAbsRap_selpre = new TH1D("rap_selpre", "rap_selpre", nbin2, 0, 132);

   fAbsRap_full_PU = new TH1D("rap_full_PU", "rap_full_PU", nbin2, 0, 132);
   fAbsRap_acc_PU = new TH1D("rap_acc_PU", "rap_acc_PU", nbin2, 0, 132);
   fAbsRap_accFSR_PU = new TH1D("rap_accFSR_PU", "rap_accFSR_PU", nbin2, 0, 132);
   fAbsRap_sim_PU = new TH1D("rap_sim_PU", "rap_sim_PU", nbin2, 0, 132);
   fAbsRap_sel_PU = new TH1D("rap_sel_PU", "rap_sel_PU", nbin2, 0, 132);

   fAbsRap_full_corr = new TH1D("rap_full_corr", "rap_full_corr", nbin2, 0, 132);
   fAbsRap_acc_corr = new TH1D("rap_acc_corr", "rap_acc_corr", nbin2, 0, 132);
   fAbsRap_accFSR_corr = new TH1D("rap_accFSR_corr", "rap_accFSR_corr", nbin2, 0, 132);
   fAbsRap_sim_corr = new TH1D("rap_sim_corr", "rap_sim_corr", nbin2, 0, 132);
   fAbsRap_sel_corr = new TH1D("rap_sel_corr", "rap_sel_corr", nbin2, 0, 132);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(hacc);
   GetOutputList()->Add(hsim);
   GetOutputList()->Add(haccFSR);
   GetOutputList()->Add(hsel);
   GetOutputList()->Add(hfull);

   //FIXME
   GetOutputList()->Add(hselpre);

   GetOutputList()->Add(hacc_PU);
   GetOutputList()->Add(hsim_PU);
   GetOutputList()->Add(haccFSR_PU);
   GetOutputList()->Add(hsel_PU);
   GetOutputList()->Add(hfull_PU);

   GetOutputList()->Add(hacc_corr);
   GetOutputList()->Add(hsim_corr);
   GetOutputList()->Add(haccFSR_corr);
   GetOutputList()->Add(hsel_corr);
   GetOutputList()->Add(hfull_corr);

   //pt-eta distributions
   GetOutputList()->Add(hacc_pT);
   GetOutputList()->Add(hsim_pT);
   GetOutputList()->Add(haccFSR_pT);
   GetOutputList()->Add(hsel_pT);
   GetOutputList()->Add(hfull_pT);

   GetOutputList()->Add(hacc_pT_PU);
   GetOutputList()->Add(hsim_pT_PU);
   GetOutputList()->Add(haccFSR_pT_PU);
   GetOutputList()->Add(hsel_pT_PU);
   GetOutputList()->Add(hfull_pT_PU);

   GetOutputList()->Add(hacc_pT_corr);
   GetOutputList()->Add(hsim_pT_corr);
   GetOutputList()->Add(haccFSR_pT_corr);
   GetOutputList()->Add(hsel_pT_corr);
   GetOutputList()->Add(hfull_pT_corr);

   GetOutputList()->Add(hacc_Eta);
   GetOutputList()->Add(hsim_Eta);
   GetOutputList()->Add(haccFSR_Eta);
   GetOutputList()->Add(hsel_Eta);
   GetOutputList()->Add(hfull_Eta);
   
   GetOutputList()->Add(hacc_Eta_PU);
   GetOutputList()->Add(hsim_Eta_PU);
   GetOutputList()->Add(haccFSR_Eta_PU);
   GetOutputList()->Add(hsel_Eta_PU);
   GetOutputList()->Add(hfull_Eta_PU);
   
   GetOutputList()->Add(hacc_Eta_corr);
   GetOutputList()->Add(hsim_Eta_corr);
   GetOutputList()->Add(haccFSR_Eta_corr);
   GetOutputList()->Add(hsel_Eta_corr);
   GetOutputList()->Add(hfull_Eta_corr);

   GetOutputList()->Add(hacc_pTEta);
   GetOutputList()->Add(hsim_pTEta);
   GetOutputList()->Add(haccFSR_pTEta);
   GetOutputList()->Add(hsel_pTEta);
   GetOutputList()->Add(hfull_pTEta);

   GetOutputList()->Add(fAbsRap_full);
   GetOutputList()->Add(fAbsRap_acc);
   GetOutputList()->Add(fAbsRap_accFSR);
   GetOutputList()->Add(fAbsRap_sel);
   GetOutputList()->Add(fAbsRap_sim);

   //FIXME
   GetOutputList()->Add(fAbsRap_selpre);

   GetOutputList()->Add(fAbsRap_full_PU);
   GetOutputList()->Add(fAbsRap_acc_PU);
   GetOutputList()->Add(fAbsRap_accFSR_PU);
   GetOutputList()->Add(fAbsRap_sel_PU);
   GetOutputList()->Add(fAbsRap_sim_PU);

   GetOutputList()->Add(fAbsRap_full_corr);
   GetOutputList()->Add(fAbsRap_acc_corr);
   GetOutputList()->Add(fAbsRap_accFSR_corr);
   GetOutputList()->Add(fAbsRap_sel_corr);
   GetOutputList()->Add(fAbsRap_sim_corr);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_MCTruthEff::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_MCTruthEff::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   // Link to current element, if any
   TString filename = dataset;

   TPair* elemPair = 0;
   if (fInput && (elemPair = dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
     TDSetElement* current = dynamic_cast<TDSetElement*>(elemPair->Value());
     if (current) {
       filename = current->GetFileName();
       if (current->TestBit(TDSetElement::kNewRun)) {
         Info("Process", "entry %lld: starting new run for dataset '%s'",
                          entry, current->GetDataSet());
       }
       if (current->TestBit(TDSetElement::kNewPacket)) {
         dataset = current->GetDataSet();
         ds->SetTitle(dataset);
         Info("Process", "entry %lld: new packet from: %s, first: %lld, last: %lld",
                          entry, current->GetName(), current->GetFirst(),
                          current->GetFirst()+current->GetNum()-1);
       }
     }
   }

   Int_t eventSize = fChain->GetTree()->GetEntry(entry);
   ++fNumberOfEvents;

   //normalization purposes
   if (dataset != "DATA") Nntuple->Fill(0.5);

   // compute the total size of all events
   fTotalDataSize += eventSize;

   if ( fNumberOfEvents % 100000 == 0 ) std::cout << dataset << " : " << fNumberOfEvents << std::endl;

   //FIXME hardcode number of trigger paths. 
   for (Int_t trig = 0; trig < 1; trig++) {
    //reset weights
     Double_t WEIGHT = 1.;
     Double_t FEWZ_WEIGHT = 1.;
     Double_t PU_WEIGHT = 1.;

     //split data mc
     if ( dataset == "DATA") isMC = false;

      if (isMC) {
           if (filename.Contains("DYM1020") || filename.Contains("DYE1020")) {
              WEIGHT = Sigma_DY1020*FilterEff_DY1020;
           } else if (filename.Contains("DYM200") || filename.Contains("DYE200")) {
              WEIGHT = Sigma_DY200*FilterEff_DY200;
           } else if ((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) {
              WEIGHT = Sigma_DY20*FilterEff_DY20;
           } else if (filename.Contains("DYM400") || filename.Contains("DYE400")) {
              WEIGHT = Sigma_DY400*FilterEff_DY400;
           } else if (filename.Contains("DYM500") || filename.Contains("DYE500")) {
              WEIGHT = Sigma_DY500*FilterEff_DY500;
           } else if (filename.Contains("DYM700") || filename.Contains("DYE700")) {
              WEIGHT = Sigma_DY700*FilterEff_DY700;
           } else if (filename.Contains("DYM800") || filename.Contains("DYE800")) {
              WEIGHT = Sigma_DY800*FilterEff_DY800;
           } else if (filename.Contains("DYM1000") || filename.Contains("DYE1000")) {
              WEIGHT = Sigma_DY1000*FilterEff_DY1000;
           }
      }


         b_GENnPair->GetEntry(entry);
         b_GENInvMass->GetEntry(entry);
         b_GENRapidity->GetEntry(entry);
         b_GENLepton1_eta->GetEntry(entry);
         b_GENLepton1_pT->GetEntry(entry);
         b_GENLepton1_pdgID->GetEntry(entry);
         b_GENLepton1_status->GetEntry(entry);
         b_GENLepton1_charge->GetEntry(entry);
         b_GENLepton2_eta->GetEntry(entry);
         b_GENLepton2_pT->GetEntry(entry);
         b_GENLepton2_pdgID->GetEntry(entry);
         b_GENLepton2_status->GetEntry(entry);


         // gen. mass
         double genDiMuPt = -1;
         double genRapidity = -1;
         double genMass = -1;
         int GENIndex = -1;
         double simMass = -1;
         double simRapidity = -999;
         int simIndex = -1;
         bool passAccpreFSR = false;
         bool passAccpostFSR = false;

         for( int j = 0; j < GENnPair; j++ ) {
          if( (fabs(GENLepton1_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton1_pdgID[j]) != 11 && mode == "EE")) continue;
          if( (fabs(GENLepton2_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton2_pdgID[j]) != 11 && mode == "EE")) continue;
          //pre FSR
          if( GENLepton1_status[j] != 3 ) continue;
          if( GENLepton2_status[j] != 3 ) continue;

          genMass = GENInvMass[j];
          GENIndex = j;
          break;
         }

         //binned in GEN mass samples

         //if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 400) { continue; }

         if ((filename.Contains("DYM200") || filename.Contains("DYE200")) && genMass > 400) { continue; }
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM800") || filename.Contains("DYE800")) && genMass > 1000) { continue; }

         //pre FSR values
         genRapidity = GENRapidity[GENIndex];
         genDiMuPt = sqrt((GENLepton1_Px[GENIndex]+GENLepton2_Px[GENIndex])*(GENLepton1_Px[GENIndex]+GENLepton2_Px[GENIndex])+(GENLepton1_Py[GENIndex]+GENLepton2_Py[GENIndex])*(GENLepton1_Py[GENIndex]+GENLepton2_Py[GENIndex]));
         //FIXME look up FEWZ weight
         FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);

        b_HLT_trigFired->GetEntry(entry);
        bool isTriggered = false;

        //2D sliced 
        double value_full = -1;
 
        for( int jj = 0; jj < 6; jj++ ) {
            if( genMass > Mass_xbin2[jj] && genMass < Mass_xbin2[jj+1] ) {
              int nbins;
              double bin_size;
              if( jj == 5 ) {
                  nbins = 12;
                  bin_size = 0.2;
              }
               else {
                 nbins = 24;
                 bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                 if( fabs(genRapidity) > k*bin_size && fabs(genRapidity) < (k+1)*bin_size ) value_full = k + jj*24;
              }
            }
          }
        fAbsRap_full->Fill(value_full, WEIGHT);
        fAbsRap_full_PU->Fill(value_full, PU_WEIGHT*WEIGHT);//, WEIGHT);
        fAbsRap_full_corr->Fill(value_full,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

        if( genMass > 15 && genMass < 1500 ) {
              hfull->Fill(genMass,WEIGHT);
              hfull_PU->Fill(genMass,PU_WEIGHT*WEIGHT);//*WEIGHT);
              hfull_corr->Fill(genMass,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
        }

        // FSR in Acc
        if( (GENLepton1_pT[GENIndex] > 20 && fabs(GENLepton1_eta[GENIndex]) < 2.4 && GENLepton2_pT[GENIndex] > 10 && fabs(GENLepton2_eta[GENIndex]) < 2.4)
            || (GENLepton1_pT[GENIndex] > 10 && fabs(GENLepton1_eta[GENIndex]) < 2.4 && GENLepton2_pT[GENIndex] > 20 && fabs(GENLepton2_eta[GENIndex]) < 2.4) ) {
             passAccpreFSR = true;
        }

       if (passAccpreFSR) {  
         //2D sliced 
         double value_acc = -1;

         for( int jj = 0; jj < 6; jj++ ) {
            if( genMass > Mass_xbin2[jj] && genMass < Mass_xbin2[jj+1] ) {
              int nbins;
              double bin_size;
              if( jj == 5 ) {
                 nbins = 12;
                 bin_size = 0.2;
              }
              else {
                 nbins = 24;
                 bin_size = 0.1;
              }
                for( int k = 0; k < nbins; k++ ) {
                  if( fabs(genRapidity) > k*bin_size && fabs(genRapidity) < (k+1)*bin_size ) value_acc = k + jj*24;
                }
             }
          }
          fAbsRap_acc->Fill(value_acc, WEIGHT);
          fAbsRap_acc_PU->Fill(value_acc,PU_WEIGHT*WEIGHT);//, WEIGHT);
          fAbsRap_acc_corr->Fill(value_acc,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

          if( genMass > 15 && genMass < 1500 ) {
              hacc->Fill(genMass,WEIGHT);
              hacc_PU->Fill(genMass,PU_WEIGHT*WEIGHT);//*WEIGHT);
              hacc_corr->Fill(genMass,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

              hacc_pT->Fill(GENLepton1_pT[GENIndex]);
              hacc_pT_PU->Fill(GENLepton1_pT[GENIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
              hacc_pT_corr->Fill(GENLepton1_pT[GENIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
              //hacc_pT->Fill(GENLepton2_pT[GENIndex]);
              //hacc_pT_PU->Fill(GENLepton2_pT[GENIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
              //hacc_pT_corr->Fill(GENLepton2_pT[GENIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

              hacc_Eta->Fill(GENLepton1_eta[GENIndex]);
              hacc_Eta_PU->Fill(GENLepton1_eta[GENIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
              hacc_Eta_corr->Fill(GENLepton1_eta[GENIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
              //hacc_Eta->Fill(GENLepton2_eta[GENIndex]);
              //hacc_Eta_PU->Fill(GENLepton2_eta[GENIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
              //hacc_Eta_corr->Fill(GENLepton2_eta[GENIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

              hacc_pTEta->Fill(GENLepton1_pT[GENIndex],GENLepton1_eta[GENIndex]);
          }
       } //end passAccpreFSR

       // sim. mass
       for( int j = 0; j < GENnPair; j++ ) {
          if( (fabs(GENLepton1_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton1_pdgID[j]) != 11 && mode == "EE")) continue;
          if( (fabs(GENLepton2_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton2_pdgID[j]) != 11 && mode == "EE")) continue;

          if( GENLepton1_status[j] != 1 ) continue;
          if( GENLepton2_status[j] != 1 ) continue;

          simIndex = j;
          break;
       }
       //post FSr values
       simMass = GENInvMass[simIndex];
       simRapidity = GENRapidity[simIndex];

       double value_sim = -1;
           
       for( int jj = 0; jj < 6; jj++ ) {
          if( simMass > Mass_xbin2[jj] && simMass < Mass_xbin2[jj+1] ) {
              int nbins;
              double bin_size;
            if( jj == 5 ) {
               nbins = 12;
               bin_size = 0.2;
           }
           else {
              nbins = 24;
              bin_size = 0.1;
          }
          for( int k = 0; k < nbins; k++ ) {
             if( fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) value_sim = k + jj*24;
           }
         }
       }
       fAbsRap_sim->Fill(value_sim, WEIGHT);
       fAbsRap_sim_PU->Fill(value_sim,PU_WEIGHT*WEIGHT);//, WEIGHT);
       fAbsRap_sim_corr->Fill(value_sim,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

       if( simMass > 15 && simMass < 1500 ) {
            hsim->Fill(simMass,WEIGHT);
            hsim_PU->Fill(simMass,PU_WEIGHT*WEIGHT);//*WEIGHT);
            hsim_corr->Fill(simMass,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

            hsim_pT->Fill(GENLepton1_pT[simIndex]);
            hsim_pT_PU->Fill(GENLepton1_pT[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
            hsim_pT_corr->Fill(GENLepton1_pT[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
            //hsim_pT->Fill(GENLepton2_pT[simIndex]);
            //hsim_pT_PU->Fill(GENLepton2_pT[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
            //hsim_pT_corr->Fill(GENLepton2_pT[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

            hsim_Eta->Fill(GENLepton1_eta[simIndex]);
            hsim_Eta_PU->Fill(GENLepton1_eta[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
            hsim_Eta_corr->Fill(GENLepton1_eta[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
            //hsim_Eta->Fill(GENLepton2_eta[simIndex]);
            //hsim_Eta_PU->Fill(GENLepton2_eta[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
            //hsim_Eta_corr->Fill(GENLepton2_eta[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

            hsim_pTEta->Fill(GENLepton1_pT[simIndex],GENLepton1_eta[simIndex]);
       }

       // FSR in Acc
       if( (GENLepton1_pT[simIndex] > 20 && fabs(GENLepton1_eta[simIndex]) < 2.4 && GENLepton2_pT[simIndex] > 10 && fabs(GENLepton2_eta[simIndex]) < 2.4)
              || (GENLepton1_pT[simIndex] > 10 && fabs(GENLepton1_eta[simIndex]) < 2.4 && GENLepton2_pT[simIndex] > 20 && fabs(GENLepton2_eta[simIndex]) < 2.4) ) {
             passAccpostFSR = true;
       }

       if (!passAccpostFSR) continue;

       //2D sliced 
       double value_accFSR = -1;

       for( int jj = 0; jj < 6; jj++ ) {
            if( simMass > Mass_xbin2[jj] && simMass < Mass_xbin2[jj+1] ) {
                  int nbins;
                  double bin_size;
                  if( jj == 5 ) {
                    nbins = 12;
                    bin_size = 0.2;
                 }
                  else {
                   nbins = 24;
                   bin_size = 0.1;
                 }
                 for( int k = 0; k < nbins; k++ ) {
                   if( fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) value_accFSR = k + jj*24;
              }
            }
          }
        fAbsRap_accFSR->Fill(value_accFSR, WEIGHT);
        fAbsRap_accFSR_PU->Fill(value_accFSR,PU_WEIGHT*WEIGHT);//, WEIGHT);
        fAbsRap_accFSR_corr->Fill(value_accFSR,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

        if( simMass > 15 && simMass < 1500 ) { 
           haccFSR->Fill(simMass,WEIGHT);
           haccFSR_PU->Fill(simMass,PU_WEIGHT*WEIGHT);//*WEIGHT);
           haccFSR_corr->Fill(simMass,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

           haccFSR_pT->Fill(GENLepton1_pT[simIndex]);
           haccFSR_pT_PU->Fill(GENLepton1_pT[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
           haccFSR_pT_corr->Fill(GENLepton1_pT[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
           //haccFSR_pT->Fill(GENLepton2_pT[simIndex]);
           //haccFSR_pT_PU->Fill(GENLepton2_pT[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
           //haccFSR_pT_corr->Fill(GENLepton2_pT[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

           haccFSR_Eta->Fill(GENLepton1_eta[simIndex]);
           haccFSR_Eta_PU->Fill(GENLepton1_eta[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
           haccFSR_Eta_corr->Fill(GENLepton1_eta[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
           //haccFSR_Eta->Fill(GENLepton2_eta[simIndex]);
           //haccFSR_Eta_PU->Fill(GENLepton2_eta[simIndex],PU_WEIGHT*WEIGHT);//*WEIGHT);
           //haccFSR_Eta_corr->Fill(GENLepton2_eta[simIndex],PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);//*WEIGHT);

           haccFSR_pTEta->Fill(GENLepton1_pT[simIndex],GENLepton1_eta[simIndex]);
        }

        double best_val_mu = -99999;
        std::vector<purdue::Dimuon>::const_iterator index_mu;
        double best_val_e = -99999; 
        std::vector<purdue::Dielectron>::const_iterator index_e;

        if (mode == "MuMu") {

          if (hlt_trigFired[1] == 1) isTriggered = true;
          if( !isTriggered ) return kTRUE;

          muons->clear();
          b_muons->GetEntry(entry);
          dimuons->clear();
          b_dimuons->GetEntry(entry);

          if (muons->size()==0 || dimuons->size()==0) continue;
          
          for (std::vector<purdue::Dimuon>::const_iterator dimu_it = dimuons->begin(); dimu_it != dimuons->end(); ++dimu_it) {
            //get the links
            purdue::Muon* mu1 = &(muons->at(dimu_it->muon_links_.first));
            purdue::Muon* mu2 = &(muons->at(dimu_it->muon_links_.second));

            if( mu1->pt_ < 20 || mu2->pt_ < 10 ) {
                if( mu2->pt_ < 20 || mu1->pt_ < 10 ) continue;
                 }

            if (!(goodMuon(*mu1) && goodMuon(*mu2))) continue;
            //Dimuon cuts section

          // 3D angle
            if( dimu_it->angle_ < 0.005 ) continue;
         
          // vtx prob
            if( dimu_it->vtxTrkProb_ < 0.02 ) continue;
            if( mu1->q_*mu2->q_ >= 0) continue;

            if( dimu_it->vtxTrkProb_ > best_val_mu ) {
              best_val_mu = dimu_it->vtxTrkProb_;
              index_mu = dimu_it;
            }
            //passed
          }
        } else if (mode == "EE") {
          //Up to date selection requirements
          //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/ikravchenko/DrellYanDMDY/Include/EleIDCuts.hh?revision=1.6&view=markup
          if (hlt_trigFired[6] == 1) isTriggered = true;
          if( !isTriggered ) return kTRUE;

          electrons->clear();
          b_electrons->GetEntry(entry);
          dielectrons->clear();
          b_dielectrons->GetEntry(entry);

          if (electrons->size()==0 || dielectrons->size()==0) continue;
          //lepton loop
          for (std::vector<purdue::Dielectron>::const_iterator diel_it = dielectrons->begin(); diel_it != dielectrons->end(); ++diel_it) {
            //get the links
            purdue::Electron* ele1 = &(electrons->at(diel_it->ele_links_.first));
            purdue::Electron* ele2 = &(electrons->at(diel_it->ele_links_.second));

            if( ele1->scEt_ < 20 || ele2->scEt_ < 10 ) {
               if( ele2->scEt_ < 20 || ele1->scEt_ < 10 ) continue;
              }
            if (!(goodElectron(*ele1) && goodElectron(*ele2))) continue;
            
            index_e = diel_it;
            best_val_e = 0;
         }
       }

        if(mode == "MuMu" && best_val_mu == -99999) continue;
        if(mode == "EE" && best_val_e == -99999) continue;

        //2D sliced 
        double value_sel = -1;

        for( int jj = 0; jj < 6; jj++ ) {
           if( simMass > Mass_xbin2[jj] && simMass < Mass_xbin2[jj+1] ) {
              int nbins;
              double bin_size;
              if( jj == 5 ) {
                  nbins = 12;
                  bin_size = 0.2;
              }
              else {
                 nbins = 24;
                 bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                if( fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) value_sel = k + jj*24;
              }
           }
        }
        fAbsRap_selpre->Fill(value_sel, WEIGHT);
        
        if( simMass > 15 && simMass < 1500 ) {
          hselpre->Fill(simMass,WEIGHT);
        }

        //2D sliced 
        value_sel = -1;

        for( int jj = 0; jj < 6; jj++ ) {
           if( simMass > Mass_xbin2[jj] && simMass < Mass_xbin2[jj+1] ) {
              int nbins; 
              double bin_size;
              if( jj == 5 ) { 
                  nbins = 12;
                  bin_size = 0.2;
              }
              else {
                 nbins = 24;
                 bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                if( fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) value_sel = k + jj*24;
              }
           }
        }
        fAbsRap_sel->Fill(value_sel, WEIGHT);
        fAbsRap_sel_PU->Fill(value_sel,PU_WEIGHT*WEIGHT); //, WEIGHT);
        fAbsRap_sel_corr->Fill(value_sel,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT); //, WEIGHT);

        if( simMass > 15 && simMass < 1500 ) {
          hsel->Fill(simMass,WEIGHT);
          hsel_PU->Fill(simMass,PU_WEIGHT*WEIGHT); //*WEIGHT);
          hsel_corr->Fill(simMass,PU_WEIGHT*WEIGHT*FEWZ_WEIGHT); //*WEIGHT);
        }

   }//end split by trig path

   return kTRUE;
}

void EventSelector_MCTruthEff::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_MCTruthEff::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {

   hacc = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc")));
   haccFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR")));
   hsel = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel")));
   hsim = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim")));
   hfull = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull")));

   //FIXME
   hselpre = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hselpre")));

   hacc_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_PU")));
   haccFSR_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_PU")));
   hsel_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_PU")));
   hsim_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_PU")));
   hfull_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_PU")));

   hacc_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_corr")));
   haccFSR_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_corr")));
   hsel_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_corr")));
   hsim_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_corr")));
   hfull_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_corr")));

   hacc_pT = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_pT")));
   haccFSR_pT = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_pT")));
   hsel_pT = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_pT")));
   hsim_pT = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_pT")));
   hfull_pT = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_pT")));

   hacc_pT_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_pT_PU")));
   haccFSR_pT_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_pT_PU")));
   hsel_pT_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_pT_PU")));
   hsim_pT_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_pT_PU")));
   hfull_pT_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_pT_PU")));

   hacc_pT_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_pT_corr")));
   haccFSR_pT_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_pT_corr")));
   hsel_pT_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_pT_corr")));
   hsim_pT_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_pT_corr")));
   hfull_pT_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_pT_corr")));

   hacc_Eta = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_Eta")));
   haccFSR_Eta = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_Eta")));
   hsel_Eta = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_Eta")));
   hsim_Eta = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_Eta")));
   hfull_Eta = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_Eta")));

   hacc_Eta_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_Eta_PU")));
   haccFSR_Eta_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_Eta_PU")));
   hsel_Eta_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_Eta_PU")));
   hsim_Eta_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_Eta_PU")));
   hfull_Eta_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_Eta_PU")));

   hacc_Eta_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hacc_Eta_corr")));
   haccFSR_Eta_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("haccFSR_Eta_corr")));
   hsel_Eta_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsel_Eta_corr")));
   hsim_Eta_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hsim_Eta_corr")));
   hfull_Eta_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hfull_Eta_corr")));

   hacc_pTEta = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hacc_pTEta")));
   haccFSR_pTEta = dynamic_cast<TH2D*>(fOutput->FindObject(Form("haccFSR_pTEta")));
   hsel_pTEta = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hsel_pTEta")));
   hsim_pTEta = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hsim_pTEta")));
   hfull_pTEta = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hfull_pTEta")));

   fAbsRap_full = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_full")));
   fAbsRap_acc = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_acc")));
   fAbsRap_accFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_accFSR")));
   fAbsRap_sel = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_sel")));
   fAbsRap_sim = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_sim")));

   //FIXME
   fAbsRap_selpre = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_selpre")));

   fAbsRap_full_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_full_corr")));
   fAbsRap_acc_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_acc_corr")));
   fAbsRap_accFSR_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_accFSR_corr")));
   fAbsRap_sel_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_sel_corr")));
   fAbsRap_sim_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_sim_corr")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   //normalization independent on the sample size
/*
   if (Nntuple->Integral() != 0) { 
      hsel->Scale(1./Nntuple->Integral());
      hsim->Scale(1./Nntuple->Integral());
      hacc->Scale(1./Nntuple->Integral());
      haccFSR->Scale(1./Nntuple->Integral());
      hfull->Scale(1./Nntuple->Integral());

      hsel_corr->Scale(1./Nntuple->Integral());
      hsim_corr->Scale(1./Nntuple->Integral());
      hacc_corr->Scale(1./Nntuple->Integral());
      haccFSR_corr->Scale(1./Nntuple->Integral());
      hfull_corr->Scale(1./Nntuple->Integral());

      //FIXME
      fRap2030_full->Scale(1./Nntuple->Integral());

   } //if samples isn't empty
*/

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   std::cout << "NAME = " << z << std::endl;
   TFile fout(z+"MCtruth.root","recreate");
   Nntuple->Write();
   hsel->Write();
   hsim->Write();
   hacc->Write();
   haccFSR->Write();
   hfull->Write();

   //FIXME
   hselpre->Write();

   hsel_PU->Write();
   hsim_PU->Write();
   hacc_PU->Write();
   haccFSR_PU->Write();
   hfull_PU->Write();

   hsel_corr->Write();
   hsim_corr->Write();
   hacc_corr->Write();
   haccFSR_corr->Write();
   hfull_corr->Write();

   hsel_pT->Write();
   hsim_pT->Write();
   hacc_pT->Write();
   haccFSR_pT->Write();
   hfull_pT->Write();

   hsel_pT_PU->Write();
   hsim_pT_PU->Write();
   hacc_pT_PU->Write();
   haccFSR_pT_PU->Write();
   hfull_pT_PU->Write();

   hsel_pT_corr->Write();
   hsim_pT_corr->Write();
   hacc_pT_corr->Write();
   haccFSR_pT_corr->Write();
   hfull_pT_corr->Write();

   hsel_Eta->Write();
   hsim_Eta->Write();
   hacc_Eta->Write();
   haccFSR_Eta->Write();
   hfull_Eta->Write();

   hsel_Eta_PU->Write();
   hsim_Eta_PU->Write();
   hacc_Eta_PU->Write();
   haccFSR_Eta_PU->Write();
   hfull_Eta_PU->Write();

   hsel_Eta_corr->Write();
   hsim_Eta_corr->Write();
   hacc_Eta_corr->Write();
   haccFSR_Eta_corr->Write();
   hfull_Eta_corr->Write();

   hsel_pTEta->Write();
   hsim_pTEta->Write();
   hacc_pTEta->Write();
   haccFSR_pTEta->Write();
   hfull_pTEta->Write();

   fAbsRap_full->Write();
   fAbsRap_acc->Write();
   fAbsRap_accFSR->Write();
   fAbsRap_sel->Write();
   fAbsRap_sim->Write();

   //FIXME
   fAbsRap_selpre->Write();

   fAbsRap_full_PU->Write();
   fAbsRap_acc_PU->Write();
   fAbsRap_accFSR_PU->Write();
   fAbsRap_sel_PU->Write();
   fAbsRap_sim_PU->Write();

   fAbsRap_full_corr->Write();
   fAbsRap_acc_corr->Write();
   fAbsRap_accFSR_corr->Write();
   fAbsRap_sel_corr->Write();
   fAbsRap_sim_corr->Write();

   fout.Close();
}

bool EventSelector_MCTruthEff::goodMuon(const purdue::Muon& mu1) {
 
   bool accepted = true;

    if( mu1.typeBits_ != 1 ||
        fabs(mu1.eta_) > 2.4 ||
        //muon id
        (mu1.muNchi2_ >= 10 || mu1.muNchi2_ < 0 ) ||
        mu1.nTkHits_ <= 10 ||
        mu1.nPixHits_ < 1 ||
        mu1.nMuHits_ < 1 ||
        mu1.nMatch_ < 2 ||
        mu1.nTrackerLayers_ < 6 ||
        fabs(mu1.d0_) > 0.2 ||
        // isolation
        (mu1.chIso03_+mu1.neuHadIso03_)/mu1.pt_ > 0.2) { accepted = false;}

   return accepted;
}


bool EventSelector_MCTruthEff::goodElectron(const purdue::Electron& ele1) {

  
    if( fabs(ele1.scEta_) > 2.4 ) return false;
    //cut_mHits
    if (ele1.nExpHitsInner_ > 1) return false;
    //FIXME cut_d0vtx
    if (ele1.d0_ >= 0.02) return false;
    //FIXME cut_dzvtx
    if (ele1.dz_ >= 0.1) return false;

    double ooemoop1 = (1.0/ele1.scE_ - ele1.EoverP_/ele1.scE_);

    //cut_ooemoop
    if (fabs(ooemoop1) >= 0.05) return false;

    //cut_vtxFit
    //not has matched conversion for both electrons
    if (ele1.isConv_) return false;

    double iso1_n = ele1.neuHadIso_+ele1.gammaIso_; //std::max(Electron1_neuthadiso[j]+Electron1_egammaiso[j]-Electron1_rho[j]*Electron1_AEff[j],0.0);
    double iso1 = ele1.chIso_+iso1_n; //(iso1_n + Electron1_chhadiso[j])/Electron1_pT[j];
        
    // Barrel/endcap switch
    if (fabs(ele1.scEta_) < 1.442) {
        //cut_dEtaIn
        if (fabs(ele1.deltaEtaIn_) >= 0.004) return false;
        //cut_dPhiIn
        if (fabs(ele1.deltaPhiIn_) >= 0.06) return false;
        //cut_sigmaIEtaIEta
        if (fabs(ele1.sigiEtaiEta_) > 0.01) return false;
        //cut_hoe
        if (ele1.HoverE_ > 0.12) return false;
    
       //cut_iso
       //no switch on pT in the barrel
        if (ele1.pt_ >= 20) {
          if (iso1 > 0.15) return false;
        } else {
          if (iso1 > 0.15) return false;
        }
    } else {
        //cut_dEtaIn
        if (fabs(ele1.deltaEtaIn_) > 0.007) return false;
        //cut_dPhiIn
        if (fabs(ele1.deltaPhiIn_) > 0.03) return false;
        //cut_sigmaIEtaIEta
        if (fabs(ele1.sigiEtaiEta_) > 0.03) return false;
        //cut_hoe
        if (ele1.HoverE_ > 0.10) return false;
        //cut_iso
        if (ele1.pt_ >= 20) {
          if (iso1 > 0.15) return false;
        } else {
          if (iso1 > 0.1) return false;
        }
    }

   return true;
}
