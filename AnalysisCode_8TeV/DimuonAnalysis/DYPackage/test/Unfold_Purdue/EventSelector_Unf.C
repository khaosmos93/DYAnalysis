#define EventSelector_Unf_cxx
// The class definition in EventSelector_Unf.h has been generated automatically
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
// Root > T->Process("EventSelector_Unf.C")
// Root > T->Process("EventSelector_Unf.C","some options")
// Root > T->Process("EventSelector_Unf.C+")
//

#include "EventSelector_Unf.h"
#include "MomScaleCorrection.C"
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

EventSelector_Unf::EventSelector_Unf(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
{
 
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

EventSelector_Unf::~EventSelector_Unf() { }


void EventSelector_Unf::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_Unf::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   this_momCorrType = dynamic_cast<TNamed *>(fInput->FindObject("momCorrType"));
   momCorrType = this_momCorrType ? this_momCorrType->GetTitle() : "";

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   //rochcorr
   rmcor = new rochcor2012();

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   //1D set up
   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   hmeas = new TH1D("hmeas", "hmeas", nbin, mass_xbin);
   htrue = new TH1D("htrue", "htrue", nbin, mass_xbin);
   hden = new TH2D("hden", "hden", nbin, mass_xbin, nbin, mass_xbin);

   hmeas_Roch = new TH1D("hmeas_Roch", "hmeas_Roch", nbin, mass_xbin);
   htrue_Roch = new TH1D("htrue_Roch", "htrue_Roch", nbin, mass_xbin);
   hden_Roch = new TH2D("hden_Roch", "hden_Roch", nbin, mass_xbin, nbin, mass_xbin);

   hmeas_PU = new TH1D("hmeas_PU", "hmeas_PU", nbin, mass_xbin);
   htrue_PU = new TH1D("htrue_PU", "htrue_PU", nbin, mass_xbin);
   hden_PU = new TH2D("hden_PU", "hden_PU", nbin, mass_xbin, nbin, mass_xbin);

   hmeas_corr = new TH1D("hmeas_corr", "hmeas_corr", nbin, mass_xbin);
   htrue_corr = new TH1D("htrue_corr", "htrue_corr", nbin, mass_xbin);
   hden_corr = new TH2D("hden_corr", "hden_corr", nbin, mass_xbin, nbin, mass_xbin);

   //2D set up
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 132 bins
   const int nbin2 = 132;
   double mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   hmeas2 = new TH1D("hmeas2", "hmeas2",  nbin2, 0, 132);
   htrue2 = new TH1D("htrue2", "htrue2",  nbin2, 0, 132);
   hden2 = new TH2D("hden2", "hden2", nbin2, 0, 132, nbin2, 0, 132);

   hmeas2_Roch = new TH1D("hmeas2_Roch", "hmeas2_Roch",  nbin2, 0, 132);
   htrue2_Roch = new TH1D("htrue2_Roch", "htrue2_Roch",  nbin2, 0, 132);
   hden2_Roch = new TH2D("hden2_Roch", "hden2_Roch", nbin2, 0, 132, nbin2, 0, 132);

   hmeas2_PU = new TH1D("hmeas2_PU", "hmeas2_PU",  nbin2, 0, 132);
   htrue2_PU = new TH1D("htrue2_PU", "htrue2_PU",  nbin2, 0, 132);
   hden2_PU = new TH2D("hden2_PU", "hden2_PU", nbin2, 0, 132, nbin2, 0, 132);

   hmeas2_corr = new TH1D("hmeas2_corr", "hmeas2_corr",  nbin2, 0, 132);
   htrue2_corr = new TH1D("htrue2_corr", "htrue2_corr",  nbin2, 0, 132);
   hden2_corr = new TH2D("hden2_corr", "hden2_corr", nbin2, 0, 132, nbin2, 0, 132);

   //FIXME over under flow
   hmeas2_24 = new TH1D("hmeas2_24", "hmeas2_24", 24,0,24);
   htrue2_24 = new TH1D("htrue2_24", "htrue2_24", 24,0,24);
   //hmeas2_12 = new TH1D("hmeas2_12", "hmeas2_12", 12,0,24);
   //htrue2_12 = new TH1D("htrue2_12", "htrue2_12", 12,0,24);
   hden2_156 = new TH2D("hden2_156", "hden2_156", 156,0,156,156,0,156);

   hmeas2_24_Roch = new TH1D("hmeas2_24_Roch", "hmeas2_24_Roch", 24,0,24);
   htrue2_24_Roch = new TH1D("htrue2_24_Roch", "htrue2_24_Roch", 24,0,24);
   //hmeas2_12_Roch = new TH1D("hmeas2_12_Roch", "hmeas2_12_Roch", 12,0,24);
   //htrue2_12_Roch = new TH1D("htrue2_12_Roch", "htrue2_12_Roch", 12,0,24);
   hden2_156_Roch = new TH2D("hden2_156_Roch", "hden2_156_Roch", 156,0,156,156,0,156);

   hmeas2_24_PU = new TH1D("hmeas2_24_PU", "hmeas2_24_PU", 24,0,24);
   htrue2_24_PU = new TH1D("htrue2_24_PU", "htrue2_24_PU", 24,0,24);
   //hmeas2_12_PU = new TH1D("hmeas2_12_PU", "hmeas2_12_PU", 12,0,24);
   //htrue2_12_PU = new TH1D("htrue2_12_PU", "htrue2_12_PU", 12,0,24);
   hden2_156_PU = new TH2D("hden2_156_PU", "hden2_156_PU", 156,0,156,156,0,156);

   hmeas2_24_corr = new TH1D("hmeas2_24_corr", "hmeas2_24_corr", 24,0,24);
   htrue2_24_corr = new TH1D("htrue2_24_corr", "htrue2_24_corr", 24,0,24);
   //hmeas2_12_corr = new TH1D("hmeas2_12_corr", "hmeas2_12_corr", 12,0,24);
   //htrue2_12_corr = new TH1D("htrue2_12_corr", "htrue2_12_corr", 12,0,24);
   hden2_156_corr = new TH2D("hden2_156_corr", "hden2_156_corr", 156,0,156,156,0,156);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(hmeas);
   GetOutputList()->Add(htrue);
   GetOutputList()->Add(hden);
   GetOutputList()->Add(hmeas2);
   GetOutputList()->Add(htrue2);
   GetOutputList()->Add(hden2);

   GetOutputList()->Add(hmeas_Roch);
   GetOutputList()->Add(htrue_Roch);
   GetOutputList()->Add(hden_Roch);
   GetOutputList()->Add(hmeas2_Roch);
   GetOutputList()->Add(htrue2_Roch);
   GetOutputList()->Add(hden2_Roch);

   GetOutputList()->Add(hmeas_PU);
   GetOutputList()->Add(htrue_PU);
   GetOutputList()->Add(hden_PU);
   GetOutputList()->Add(hmeas2_PU);
   GetOutputList()->Add(htrue2_PU);
   GetOutputList()->Add(hden2_PU);

   GetOutputList()->Add(hmeas_corr);
   GetOutputList()->Add(htrue_corr);
   GetOutputList()->Add(hden_corr);
   GetOutputList()->Add(hmeas2_corr);
   GetOutputList()->Add(htrue2_corr);
   GetOutputList()->Add(hden2_corr);

   GetOutputList()->Add(hmeas2_24);
   GetOutputList()->Add(htrue2_24);
   //GetOutputList()->Add(hmeas2_12);
   //GetOutputList()->Add(htrue2_12);
   GetOutputList()->Add(hden2_156);

   GetOutputList()->Add(hmeas2_24_Roch);
   GetOutputList()->Add(htrue2_24_Roch);
   //GetOutputList()->Add(hmeas2_12_Roch);
   //GetOutputList()->Add(htrue2_12_Roch);
   GetOutputList()->Add(hden2_156_Roch);

   GetOutputList()->Add(hmeas2_24_PU);
   GetOutputList()->Add(htrue2_24_PU);
   //GetOutputList()->Add(hmeas2_12_PU);
   //GetOutputList()->Add(htrue2_12_PU);
   GetOutputList()->Add(hden2_156_PU);

   GetOutputList()->Add(hmeas2_24_corr);
   GetOutputList()->Add(htrue2_24_corr);
   //GetOutputList()->Add(hmeas2_12_corr);
   //GetOutputList()->Add(htrue2_12_corr);
   GetOutputList()->Add(hden2_156_corr);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_Unf::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;  //SHould alkways be true - we will always run it on data

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};
   double Mass_xbin2_156[8] = {0,20, 30, 45, 60, 120, 200, 1500};

   //FIXME deprecated
   // _runopt = 0;
   // if( momCorrType == "RunC")  _runopt = 1;
   
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_Unf::GetEntry() or TBranch::GetEntry()
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

   //split below by trigger path
   for (Int_t trig = 0; trig < 1; trig++) {
    //reset weights
     Double_t WEIGHT = 1.;
     Double_t FEWZ_WEIGHT = 1.;
     Double_t PU_WEIGHT = 1.;

     //split data mc 
     if ( dataset == "DATA") {
        isMC = false; 
        //FIXME important requirement
        if (trig != 0) continue;
     }

      //pick up the weight
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
           };
      } //isMC

     if (filename.Contains("DYM") || filename.Contains("DYE")) {

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
         double genMass = -1;
         double genRapidity = -1;
         double genDiMuPt = -1;
         int GENIndex = -1;
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
        }

        b_HLT_trigFired->GetEntry(entry);
        bool isTriggered = false;
 
        //lepton loop
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

        // setup for momentum correction
        // Only for RECO, do nothing for GEN
        TLorentzVector muMinus;
        TLorentzVector muPlus;
        TLorentzVector recoDYcand;

        //in case you apply correction
        double recoMass_corr = -1.;
        double recoRap_corr = -1.;
        double recoMass = -1.;
        double recoRap = -1.;

        if(mode == "MuMu") {
          //take back the links of best muons
          purdue::Muon* bestMu1 = &(muons->at(index_mu->muon_links_.first));
          purdue::Muon* bestMu2 = &(muons->at(index_mu->muon_links_.second));

          //Use one for systematics
          float dummy = 0;
          MomScaleCorrection(rmcor, muMinus, muPlus,
                bestMu1->px_,bestMu1->py_,bestMu1->pz_,bestMu1->q_,
                bestMu2->px_,bestMu2->py_,bestMu2->pz_,bestMu2->q_,
                dummy, _runopt, !isMC);

          recoDYcand = muPlus + muMinus;
    
          //in case you apply correction
          recoMass_corr = recoDYcand.M();
          recoRap_corr = fabs(recoDYcand.Rapidity());
          //Common for mumu and ee but diferenbt pointer of course
          recoMass = index_mu->mass_;
          recoRap = index_mu->y_;
        } //mom scale correction - for muons only 
        else if (mode == "EE") {
          recoMass = index_e->mass_;
          recoRap = index_e->y_;
        } 

        // gen. mass
        double simMass = -1;
        int simIndex = -1;
        double simRapidity = -1;
        for( int j = 0; j < GENnPair; j++ ) {
            if( (fabs(GENLepton1_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton1_pdgID[j]) != 11 && mode == "EE")) continue;
            if( (fabs(GENLepton2_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton2_pdgID[j]) != 11 && mode == "EE")) continue;
            //pre FSR
            if( GENLepton1_status[j] != 1 ) continue;
            if( GENLepton2_status[j] != 1 ) continue;
             
            // cout << "genMass " << genMass << endl;
            simMass = GENInvMass[j];
            simRapidity = GENRapidity[j];
            break;
          }

          //put the fills 
  	  htrue->Fill(simMass, WEIGHT);
 	  hmeas->Fill(recoMass, WEIGHT);
	  hden->Fill(simMass, recoMass, WEIGHT);

          htrue_PU->Fill(simMass, PU_WEIGHT*WEIGHT);
          hmeas_PU->Fill(recoMass, PU_WEIGHT*WEIGHT);
          hden_PU->Fill(simMass, recoMass, PU_WEIGHT*WEIGHT);

          if (mode == "MuMu") {
            htrue_Roch->Fill(simMass, WEIGHT);
            hmeas_Roch->Fill(recoMass_corr, WEIGHT);
            hden_Roch->Fill(simMass, recoMass_corr, WEIGHT);

            htrue_corr->Fill(simMass, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
            hmeas_corr->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
            hden_corr->Fill(simMass, recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          }

          //2D case
          double val_reco = -1;
          double val_sim = -1;

          for( int j = 0; j < 6; j++ ) {
            if( recoMass > Mass_xbin2[j] && recoMass < Mass_xbin2[j+1] ) {
              int nbins; 
              double bin_size;
              if( j == 5 ) {
                nbins = 12;
                bin_size = 0.2;
              }
              else {
                nbins = 24;
                bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                if( fabs(recoRap) > k*bin_size && fabs(recoRap) < (k+1)*bin_size ) val_reco = k + j*24;
              }   
            }     
            if( simMass > Mass_xbin2[j] && simMass < Mass_xbin2[j+1] ) {
              int nbins;
              double bin_size;
              if( j == 5 ) {
                nbins = 12;
                bin_size = 0.2;
              }
              else {
                nbins = 24;
                bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                if (fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) val_sim = k + j*24;
              }
            }
          }

          hmeas2->Fill(val_reco, WEIGHT);
          htrue2->Fill(val_sim, WEIGHT);
          hden2->Fill(val_reco, val_sim, WEIGHT);

          hmeas2_PU->Fill(val_reco, PU_WEIGHT*WEIGHT);
          htrue2_PU->Fill(val_sim, PU_WEIGHT*WEIGHT);
          hden2_PU->Fill(val_reco, val_sim, PU_WEIGHT*WEIGHT);

          if (mode == "MuMu") {
            //with the rochester correction
            val_reco = -1;
            val_sim = -1;
            for( int j = 0; j < 6; j++ ) {
              if( recoMass_corr > Mass_xbin2[j] && recoMass_corr < Mass_xbin2[j+1] ) {
                int nbins;
                double bin_size;
               if( j == 5 ) {
                  nbins = 12;
                  bin_size = 0.2;
                }
                else {
                  nbins = 24;
                  bin_size = 0.1;
                }
                for( int k = 0; k < nbins; k++ ) {
                  if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) val_reco = k + j*24;
                }
              }
             if( simMass > Mass_xbin2[j] && simMass < Mass_xbin2[j+1] ) {
               int nbins;
               double bin_size;
               if( j == 5 ) {
                 nbins = 12;
                 bin_size = 0.2;
               }
               else {
                 nbins = 24;
                 bin_size = 0.1;
               }
               for( int k = 0; k < nbins; k++ ) {
                 if (fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) val_sim = k + j*24;
               }
             }
           }

           hmeas2_Roch->Fill(val_reco, WEIGHT);
           htrue2_Roch->Fill(val_sim, WEIGHT);
           hden2_Roch->Fill(val_reco, val_sim, WEIGHT);
  
           hmeas2_corr->Fill(val_reco, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
           htrue2_corr->Fill(val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
           hden2_corr->Fill(val_reco, val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

          //over under flows
           if( recoMass_corr > 0 && recoMass_corr < 20 ) {
             double bin_size = 0.1;
            //FIXME reset
            val_reco = -1;
            for( int k = 0; k < 24; k++ ) {
              if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) val_reco = k;
            }

            hmeas2_24_Roch->Fill(val_reco, WEIGHT);
            hmeas2_24_corr->Fill(val_reco, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
         }
         if( simMass > 0 && simMass < 20 ) {
            double bin_size = 0.1;
            //FIXME reset
            val_sim = -1;
            for( int k = 0; k < 24; k++ ) {
              if (fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) val_sim = k;
            }

            htrue2_24->Fill(val_sim, WEIGHT);
            htrue2_24_Roch->Fill(val_sim, WEIGHT);
            htrue2_24_corr->Fill(val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
         }
       } //mumu only


         //over under flows
         val_reco = -1;
         val_sim = -1;
         if( recoMass > 0 && recoMass < 20 ) {
             double bin_size = 0.1;
            //FIXME reset
            val_reco = -1;
            for( int k = 0; k < 24; k++ ) {
              if( fabs(recoRap) > k*bin_size && fabs(recoRap) < (k+1)*bin_size ) val_reco = k;
            }

            hmeas2_24->Fill(val_reco, WEIGHT);
         }
         val_reco = -1;
         val_sim = -1;
         for( int jj = 0; jj < 7; jj++ ) {
           if( recoMass > Mass_xbin2_156[jj] && recoMass < Mass_xbin2_156[jj+1] ) {
            int nbins;
            double bin_size;
            if( jj == 6) {
              nbins = 12;
              bin_size = 0.2;
            }
            else {
              nbins = 24;
              bin_size = 0.1;
            }
            for( int kk = 0; kk < nbins; kk++ ) {
              if( fabs(recoRap) > kk*bin_size && fabs(recoRap) < (kk+1)*bin_size ) val_reco = kk + jj*24;
            }
          }   
          if( simMass > Mass_xbin2_156[jj] && simMass < Mass_xbin2_156[jj+1] ) {
            int nbins;
            double bin_size;
            if(jj == 6) {
              nbins = 12;
              bin_size = 0.2;
            }
            else {
              nbins = 24;
              bin_size = 0.1;
            }
            for( int kk = 0; kk < nbins; kk++ ) {
              if (fabs(simRapidity) > kk*bin_size && fabs(simRapidity) < (kk+1)*bin_size ) val_sim = kk + jj*24;
            }
          }
         }
         hden2_156->Fill(val_reco, val_sim, WEIGHT);
         hden2_156_PU->Fill(val_reco, val_sim, PU_WEIGHT*WEIGHT);


       if (mode == "MuMu") {
         val_reco = -1;
         val_sim = -1;
         for( int jj = 0; jj < 7; jj++ ) {
          if( recoMass_corr > Mass_xbin2_156[jj] && recoMass_corr < Mass_xbin2_156[jj+1] ) {
            int nbins;
            double bin_size;
            if( jj == 6) {
              nbins = 12;
              bin_size = 0.2;
            }
            else { 
              nbins = 24;
              bin_size = 0.1;
            }
            for( int kk = 0; kk < nbins; kk++ ) {
              if( fabs(recoRap_corr) > kk*bin_size && fabs(recoRap_corr) < (kk+1)*bin_size ) val_reco = kk + jj*24;
            }
          }
          if( simMass > Mass_xbin2_156[jj] && simMass < Mass_xbin2_156[jj+1] ) {
            int nbins;
            double bin_size;
            if(jj == 6) {
              nbins = 12;
              bin_size = 0.2;
            }
            else {
              nbins = 24;
              bin_size = 0.1;
            }
            for( int kk = 0; kk < nbins; kk++ ) {
              if (fabs(simRapidity) > kk*bin_size && fabs(simRapidity) < (kk+1)*bin_size ) val_sim = kk + jj*24;
            }
          }
        }
        hden2_156_Roch->Fill(val_reco, val_sim, WEIGHT);
        hden2_156_corr->Fill(val_reco, val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
      } //mumu only

   }//end split by trig path

   return kTRUE;
}

void EventSelector_Unf::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_Unf::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   hmeas = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas")));
   htrue = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue")));
   hden = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden")));
   hmeas2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2")));
   htrue2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2")));
   hden2 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2")));

   hmeas_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas_Roch")));
   htrue_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue_Roch")));
   hden_Roch = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden_Roch")));
   hmeas2_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_Roch")));
   htrue2_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_Roch")));
   hden2_Roch = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_Roch")));

   hmeas_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas_PU")));
   htrue_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue_PU")));
   hden_PU = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden_PU")));
   hmeas2_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_PU")));
   htrue2_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_PU")));
   hden2_PU = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_PU")));

   hmeas_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas_corr")));
   htrue_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue_corr")));
   hden_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden_corr")));
   hmeas2_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_corr")));
   htrue2_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_corr")));
   hden2_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_corr")));

   hmeas2_24 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_24")));
   htrue2_24 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_24")));
   //hmeas2_12 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_12")));
   //htrue2_12 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_12")));
   hden2_156 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_156")));

   hmeas2_24_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_24_Roch")));
   htrue2_24_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_24_Roch")));
   //hmeas2_12_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_12_Roch")));
   //htrue2_12_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_12_Roch")));
   hden2_156_Roch = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_156_Roch")));

   hmeas2_24_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_24_PU")));
   htrue2_24_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_24_PU")));
   //hmeas2_12_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_12_PU")));
   //htrue2_12_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_12_PU")));
   hden2_156_PU = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_156_PU")));

   hmeas2_24_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_24_corr")));
   htrue2_24_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_24_corr")));
   //hmeas2_12_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmeas2_12_corr")));
   //htrue2_12_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("htrue2_12_corr")));
   hden2_156_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2_156_corr")));

   //normalization independent on the sample size
    if (Nntuple->Integral() != 0) { 
      hmeas->Scale(1./Nntuple->Integral());
      htrue->Scale(1./Nntuple->Integral());
      hden->Scale(1./Nntuple->Integral());
      hmeas2->Scale(1./Nntuple->Integral());
      htrue2->Scale(1./Nntuple->Integral());
      hden2->Scale(1./Nntuple->Integral());

      hmeas_Roch->Scale(1./Nntuple->Integral());
      htrue_Roch->Scale(1./Nntuple->Integral());
      hden_Roch->Scale(1./Nntuple->Integral());
      hmeas2_Roch->Scale(1./Nntuple->Integral());
      htrue2_Roch->Scale(1./Nntuple->Integral());
      hden2_Roch->Scale(1./Nntuple->Integral());

      hmeas_PU->Scale(1./Nntuple->Integral());
      htrue_PU->Scale(1./Nntuple->Integral());
      hden_PU->Scale(1./Nntuple->Integral());
      hmeas2_PU->Scale(1./Nntuple->Integral());
      htrue2_PU->Scale(1./Nntuple->Integral());
      hden2_PU->Scale(1./Nntuple->Integral());

      hmeas_corr->Scale(1./Nntuple->Integral());
      htrue_corr->Scale(1./Nntuple->Integral());
      hden_corr->Scale(1./Nntuple->Integral());
      hmeas2_corr->Scale(1./Nntuple->Integral());
      htrue2_corr->Scale(1./Nntuple->Integral());
      hden2_corr->Scale(1./Nntuple->Integral());

      hmeas2_24->Scale(1./Nntuple->Integral());
      htrue2_24->Scale(1./Nntuple->Integral());
      //hmeas2_12->Scale(1./Nntuple->Integral());
      //htrue2_12->Scale(1./Nntuple->Integral());
      hden2_156->Scale(1./Nntuple->Integral());
   
      hmeas2_24_Roch->Scale(1./Nntuple->Integral());
      htrue2_24_Roch->Scale(1./Nntuple->Integral());
      //hmeas2_12_Roch->Scale(1./Nntuple->Integral());
      //htrue2_12_Roch->Scale(1./Nntuple->Integral());
      hden2_156_Roch->Scale(1./Nntuple->Integral());
   
      hmeas2_24_PU->Scale(1./Nntuple->Integral());
      htrue2_24_PU->Scale(1./Nntuple->Integral());
      //hmeas2_12_PU->Scale(1./Nntuple->Integral());
      //htrue2_12_PU->Scale(1./Nntuple->Integral());
      hden2_156_PU->Scale(1./Nntuple->Integral());

      hmeas2_24_corr->Scale(1./Nntuple->Integral());
      htrue2_24_corr->Scale(1./Nntuple->Integral());
      //hmeas2_12_corr->Scale(1./Nntuple->Integral());
      //htrue2_12_corr->Scale(1./Nntuple->Integral());
      hden2_156_corr->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   this_momCorrType = dynamic_cast<TNamed *>(fInput->FindObject("momCorrType"));
   momCorrType = this_momCorrType ? this_momCorrType->GetTitle() : "";

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   std::cout << "NAME = " << z << std::endl;
   //FIXME must have this dir first
   TFile fout(z+"_"+histogramThis+"_"+mode+TString(momCorrType)+".root","recreate");
   Nntuple->Write();

   hmeas->Write();
   htrue->Write();
   hden->Write();
   hmeas2->Write();
   htrue2->Write();
   hden2->Write();

   hmeas_Roch->Write();
   htrue_Roch->Write();
   hden_Roch->Write();
   hmeas2_Roch->Write();
   htrue2_Roch->Write();
   hden2_Roch->Write();

   hmeas_PU->Write();
   htrue_PU->Write();
   hden_PU->Write();
   hmeas2_PU->Write();
   htrue2_PU->Write();
   hden2_PU->Write();

   hmeas_corr->Write();
   htrue_corr->Write();
   hden_corr->Write();
   hmeas2_corr->Write();
   htrue2_corr->Write();
   hden2_corr->Write();

   hmeas2_24->Write();
   htrue2_24->Write();
   //hmeas2_12->Write();
   //htrue2_12->Write();
   hden2_156->Write();
   
   hmeas2_24_Roch->Write();
   htrue2_24_Roch->Write();
   //hmeas2_12_Roch->Write();
   //htrue2_12_Roch->Write();
   hden2_156_Roch->Write();
   
   hmeas2_24_PU->Write();
   htrue2_24_PU->Write();
   //hmeas2_12_PU->Write();
   //htrue2_12_PU->Write();
   hden2_156_PU->Write();

   hmeas2_24_corr->Write();
   htrue2_24_corr->Write();
   //hmeas2_12_corr->Write();
   //htrue2_12_corr->Write();
   hden2_156_corr->Write();

   fout.Close();
}
/*
double EventSelector_Unf::Rapidity( double px1, double py1, double pz1, double px2, double py2, double pz2 ) {

  double mass = 0.105658;

  double p1 = px1*px1 + py1*py1 + pz1*pz1;
  double e1 = sqrt(mass*mass + p1);

  double p2 = px2*px2 + py2*py2 + pz2*pz2;
  double e2 = sqrt(mass*mass + p2);

  // dimuon system
  //double px = px1 + px2;
  //double py = py1 + py2;
  double pz = pz1 + pz2;
  double e = e1 + e2;
  //double p3 = px*px + py*py + pz*pz;
  //double invmas = sqrt( e*e - p3 );

  // rapidity
  double par = (e + pz)/(e - pz);
  double rapidity = 0.5*TMath::Log(par);

  return rapidity;
}
*/

bool EventSelector_Unf::goodMuon(const purdue::Muon& mu1) {
 
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


bool EventSelector_Unf::goodElectron(const purdue::Electron& ele1) {

  
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
    double iso1 = (ele1.chIso_+iso1_n)/ele1.pt_; //(iso1_n + Electron1_chhadiso[j])/Electron1_pT[j];
        
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
