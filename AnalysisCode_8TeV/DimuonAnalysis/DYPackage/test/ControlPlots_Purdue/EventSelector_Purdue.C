#define EventSelector_Purdue_cxx
// The class definition in EventSelector_Purdue.h has been generated automatically
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
// Root > T->Process("EventSelector_Purdue.C")
// Root > T->Process("EventSelector_Purdue.C","some options")
// Root > T->Process("EventSelector_Purdue.C+")
//

#include "EventSelector_Purdue.h"
#include "efficiencyWeightToBin2012.C"
#include "MomScaleCorrection.C"
#include <iostream>
#include <TProof.h>
#include <TProofServ.h>
#include <TDSet.h>
#include <TEventList.h>
#include <TParameter.h>
#include <TMath.h>
#include <vector>

#include "Muon.h"
#include "Electron.h"
#include "Dimuon.h"
#include "Dielectron.h"

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

EventSelector_Purdue::EventSelector_Purdue(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
{

  //set the luminosity in a given run range
  //if normalization to luminosity is used  
  //lumi normalization is deprecated as we use normalization
  //the integral in data
  trigPaths[0] = "";

  //alternative to ntuple weights
  //Each prior is defined using the following formula:
  //Prior_<MC_sample> = (Cross section*Filter efficiency/Number of GEN events)|<MC sample>
  //this information is manually retrieved for each sample from DAS: https://cmsweb-testbed.cern.ch/das/
  //The first two numbers are provided in the cfg file for "parent" datasets.
  //Alternatively, cross section can be obtainedi from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
  //or: http://cms.cern.ch/iCMS/jsp/mcprod/admin/requestmanagement.jsp?campid=Summer12 
  Sigma_DY1020 = 3708.0;
  Sigma_DY20   = 1871.0;
  Sigma_DY200  = 1.485;
  Sigma_DY400  = 0.1086;
  Sigma_DY500  = 0.04415;
  Sigma_DY700  = 0.01024;
  Sigma_DY800  = 0.005491;
  Sigma_DY1000 = 0.001796; 
  Sigma_tt700  = 18.19;
  Sigma_tt1000 = 3.44;
  Sigma_ttjets = 23.85;
  Sigma_tW     = 11.17;
  Sigma_tbarW  = 11.17;
  Sigma_WJets  = 37509.0;
  Sigma_DYtautau1020 = 3708.0; //promote to NNLO? 3795.4 
  Sigma_DYtautau20   = 1871.0; //promote to NNLO? 1915.1
  Sigma_QCD15to20    = 7.022E8;
  Sigma_QCD20to30    = 2.87E8;
  Sigma_QCD30to50    = 6.6285328E7;
  Sigma_QCD50to80    = 8082000.0;
  Sigma_QCD80to120   = 1033680.0;
  Sigma_QCD120to170  = 156293.3;
  Sigma_QCD170to300  = 34020.0;
  Sigma_WZJetsTo2L2Q =  2.31; 
  Sigma_WZJetsTo3LNu   = 1.11;
  Sigma_ZZJetsTo2L2Nu  =  0.364791;
  Sigma_ZZJetsTo2L2Q   =  2.448690;
  Sigma_ZZJetsTo4L     =  0.176908;
  Sigma_WWJetsTo2L2Nu  =  5.812;

  //numbers for NNLO scaling FIXME
  //10-20: (NLO powheg 10-20) * (NNLO m>20) / (NLO powheg m>20)
  //20-inf: NNLO m>20
  //500-inf: (NLO powheg 500-inf) * (NNLO m>20) / (NLO powheg m>20)
  //800-inf: (NLO powheg 800-inf) * (NNLO m>20) / (NLO powheg m>20)

  FilterEff_DY1020 = 1;
  FilterEff_DY20   = 1.;
  FilterEff_DY200  = 1.;
  FilterEff_DY400  = 1.;
  FilterEff_DY500  = 1.;
  FilterEff_DY700  = 1.;
  FilterEff_DY800  = 1.;
  FilterEff_DY1000 = 1.;
  FilterEff_tt700  = 0.074;
  FilterEff_tt1000 = 0.014;
  FilterEff_ttjets = 1.0;
  FilterEff_tW     = 0.996;
  FilterEff_tbarW  = 0.996;
  FilterEff_WJets  = 1.0;
  FilterEff_DYtautau1020 = 1.;
  FilterEff_DYtautau20   = 1.;
  FilterEff_QCD15to20    = 0.0039;
  FilterEff_QCD20to30    = 0.0065;
  FilterEff_QCD30to50    = 0.0122;
  FilterEff_QCD50to80    = 0.0218;
  FilterEff_QCD80to120   = 0.0395;
  FilterEff_QCD120to170  = 0.0473;
  FilterEff_QCD170to300  = 0.0676;
  FilterEff_WZJetsTo2L2Q      = 1.0;
  FilterEff_WZJetsTo3LNu      = 1.0;
  FilterEff_ZZJetsTo2L2Nu     = 1.0;
  FilterEff_ZZJetsTo2L2Q      = 1.0;
  FilterEff_ZZJetsTo4L        = 1.0;
  FilterEff_WWJetsTo2L2Nu     = 1.0; 

}

EventSelector_Purdue::~EventSelector_Purdue() { }


void EventSelector_Purdue::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_Purdue::SlaveBegin(TTree*)
{

   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   this_histogramThis = dynamic_cast<TNamed *>(fInput->FindObject("histogramThis"));
   histogramThis = this_histogramThis ? this_histogramThis->GetTitle() : "";

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   this_momCorrType = dynamic_cast<TNamed *>(fInput->FindObject("momCorrType"));
   momCorrType = this_momCorrType ? this_momCorrType->GetTitle() : "";

   //rochcorr
   rmcor = new rochcor2012();

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   finvM = new TH1D("invm", "invm", nbin, mass_xbin);
   finvM_Roch = new TH1D("invm_Roch", "invm_Roch", nbin, mass_xbin);
   finvM_PU = new TH1D("invm_PU", "invm_PU", nbin, mass_xbin);
   finvM_corr = new TH1D("invm_corr", "invm_corr", nbin, mass_xbin);
   finvM_fine_bb = new TH1D("invm_fine_bb", "invm_fine_bb", 120,60,120);
   finvM_fineRoch_bb = new TH1D("invm_fineRoch_bb", "invm_fineRoch_bb", 120,60,120);
   finvM_fine_ee = new TH1D("invm_fine_ee", "invm_fine_ee", 120,60,120);
   finvM_fineRoch_ee = new TH1D("invm_fineRoch_ee", "invm_fineRoch_ee", 120,60,120);
   finvM_fine_eb = new TH1D("invm_fine_eb", "invm_fine_eb", 120,60,120);
   finvM_fineRoch_eb = new TH1D("invm_fineRoch_eb", "invm_fineRoch_eb", 120,60,120);


   fRelCombIso = new TH1D("RelCombIso","RelCombIso",500,0,5);
   fRelCombIsoNoEGamma = new TH1D("RelCombIsoNoEGamma","RelCombIsoNoEGamma",500,0,5);

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

   fAbsRap = new TH1D("rap", "rap", nbin2, 0, 132);
   fAbsRap_PU = new TH1D("rap_PU", "rap_PU", nbin2, 0, 132);
   fAbsRap_corr = new TH1D("rap_corr", "rap_corr", nbin2, 0, 132);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(finvM);
   GetOutputList()->Add(finvM_Roch);
   GetOutputList()->Add(finvM_PU);
   GetOutputList()->Add(finvM_corr);
   GetOutputList()->Add(finvM_fine_ee);
   GetOutputList()->Add(finvM_fineRoch_ee);
   GetOutputList()->Add(finvM_fine_bb);
   GetOutputList()->Add(finvM_fineRoch_bb);
   GetOutputList()->Add(finvM_fine_eb);
   GetOutputList()->Add(finvM_fineRoch_eb);
   GetOutputList()->Add(fAbsRap);
   GetOutputList()->Add(fAbsRap_PU);
   GetOutputList()->Add(fAbsRap_corr);

   GetOutputList()->Add(fRelCombIso);
   GetOutputList()->Add(fRelCombIsoNoEGamma);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_Purdue::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

    //pick up the correct correction map, there is data & MC dependence of the maps!
    _runopt = 0;
    //if( momCorrType == "RunD")  _runopt = 1;
  
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_Purdue::GetEntry() or TBranch::GetEntry()
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

   //normalization purposes
   Nntuple->Fill(0.5);

   //hardcode number of trigger paths. 
   for (Int_t trig = 0; trig < 1; trig++) {
    //reset weights
     Double_t WEIGHT = 1.;
     Double_t PU_WEIGHT = 1.;
     Double_t FEWZ_WEIGHT = 1.;


     //split data mc
     if ( dataset == "DATA") {
        isMC = false;
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
           } else if (filename.Contains("tt700")) {
              WEIGHT = Sigma_tt700*FilterEff_tt700;
           } else if (filename.Contains("tt1000")) {
              WEIGHT = Sigma_tt1000*FilterEff_tt1000;
           } else if (filename.Contains("ttjets")) {
              WEIGHT = Sigma_ttjets*FilterEff_ttjets;
           } else if (filename.Contains("tbarW")) {
              WEIGHT = Sigma_tbarW*FilterEff_tbarW;
           } else if (filename.Contains("tW")) {
              WEIGHT = Sigma_tW*FilterEff_tW;
           } else if (filename.Contains("WJets") && !filename.Contains("WWJetsTo2L2Nu")) {
              WEIGHT = Sigma_WJets*FilterEff_WJets;
           } else if (filename.Contains("WZJetsTo2L2Q")) {
              WEIGHT = Sigma_WZJetsTo2L2Q*FilterEff_WZJetsTo2L2Q;
           } else if (filename.Contains("WZJetsTo3LNu")) {
              WEIGHT = Sigma_WZJetsTo3LNu*FilterEff_WZJetsTo3LNu;
           } else if (filename.Contains("ZZJetsTo2L2Nu")) {
              WEIGHT = Sigma_ZZJetsTo2L2Nu*FilterEff_ZZJetsTo2L2Nu;
           } else if (filename.Contains("ZZJetsTo2L2Q")) {
              WEIGHT = Sigma_ZZJetsTo2L2Q*FilterEff_ZZJetsTo2L2Q;
           } else if (filename.Contains("ZZJetsTo4L")) {
              WEIGHT = Sigma_ZZJetsTo4L*FilterEff_ZZJetsTo4L;
           } else if (filename.Contains("WWJetsTo2L2Nu")) {
              WEIGHT = Sigma_WWJetsTo2L2Nu*FilterEff_WWJetsTo2L2Nu;
           } else if (filename.Contains("DYtautau1020")) {
              WEIGHT = Sigma_DYtautau1020*FilterEff_DYtautau1020;
           } else if (filename.Contains("DYtautau20")) {
              WEIGHT = Sigma_DYtautau20*FilterEff_DYtautau20;
           } else if (filename.Contains("QCD15to20")) {
              WEIGHT = Sigma_QCD15to20*FilterEff_QCD15to20;
           } else if (filename.Contains("QCD20to30")) {
              WEIGHT = Sigma_QCD20to30*FilterEff_QCD20to30;
           } else if (filename.Contains("QCD30to50")) {
              WEIGHT = Sigma_QCD30to50*FilterEff_QCD30to50;
           } else if (filename.Contains("QCD50to80")) {
              WEIGHT = Sigma_QCD50to80*FilterEff_QCD50to80;
           } else if (filename.Contains("QCD80to120")) {
              WEIGHT = Sigma_QCD80to120*FilterEff_QCD80to120;
           } else if (filename.Contains("QCD120to170")) {
              WEIGHT = Sigma_QCD120to170*FilterEff_QCD120to170;
           } else if (filename.Contains("QCD170to300")) {
              WEIGHT = Sigma_QCD170to300*FilterEff_QCD170to300;
           }
      } //isMC
      //scale up if normalizing to lumi

      //pileup
      if (isMC) { 
        b_pileUpReweight->GetEntry(entry);
        PU_WEIGHT = pileUpReweight;
      } 

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

        if (mode == "MuMu") {

         if (hlt_trigFired[1] == 1) isTriggered = true;
         if( !isTriggered ) return kTRUE;

          muons->clear();
          b_muons->GetEntry(entry);
          dimuons->clear();
          b_dimuons->GetEntry(entry);

          if (muons->size()==0 || dimuons->size()==0) continue;  
          //lepton loop
          double best_val = -99999;
          std::vector<purdue::Dimuon>::const_iterator index;
          bool pass = false;
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

            if( dimu_it->vtxTrkProb_ > best_val ) {
              best_val = dimu_it->vtxTrkProb_;
              index = dimu_it;
            }
        }//dimuon loop
           if(best_val == -99999) continue;

           //for futher
           purdue::Muon* mu_index1 = &(muons->at(index->muon_links_.first));
           purdue::Muon* mu_index2 = &(muons->at(index->muon_links_.second));

           //trigger matching begin
           int isMatched = 0;
           double trigEta[2] = {-999};
           double trigPhi[2] = {-999};
           int _ntrig = 0;
           for( int k = 0; k < hlt_ntrig; k++ ) {
             bool isFired = false;
             if (hlt_trigFired[1] == 1) isFired = true;
             if( !isFired ) continue;
              //Just make sure that there is two objects trigger 
              //matched - nothing more! Don't care which ones exactly 
             if( _ntrig == 0 ) {
                trigEta[_ntrig] = hlt_trigEta[k];
                trigPhi[_ntrig] = hlt_trigPhi[k];
                _ntrig++;
             }
             else {
                if( deltaR(hlt_trigEta[k], hlt_trigPhi[k], trigEta[0], trigPhi[0]) < 0.001 ) continue;
                else {
                  if( _ntrig == 1 ) {
                    //trigType[_ntrig] = hlt_trigType[k];
                    //trigPt[_ntrig] = hlt_trigPt[k];
                   trigEta[_ntrig] = hlt_trigEta[k];
                     trigPhi[_ntrig] = hlt_trigPhi[k];
                    _ntrig++;
                  }
                  else {
                   if( deltaR(hlt_trigEta[k], hlt_trigPhi[k], trigEta[0], trigPhi[0]) < 0.001 ) continue;
                   if( deltaR(hlt_trigEta[k], hlt_trigPhi[k], trigEta[1], trigPhi[1]) < 0.001 ) continue;
                }
              }
            }
          }
           //if( _ntrig < 2 ) cout << "Less than 2 trig objects!!!" << endl;
          for( int k = 0; k < 2; k++ ) {
              double dR1 = deltaR(trigEta[k], trigPhi[k], mu_index1->eta_, mu_index1->phi_);
              double dR2 = deltaR(trigEta[k], trigPhi[k], mu_index2->eta_, mu_index2->phi_);
              if( dR1 < 0.2 || dR2 < 0.2 ) {
                  isMatched++;
              }
          }
          if( isMatched < 2) continue;

           // setup for momentum correction
           // Only for RECO, do nothing for GEN
           TLorentzVector muMinus;
           TLorentzVector muPlus;
           //take back the links of best muons
           purdue::Muon* bestMu1 = &(muons->at(index->muon_links_.first));
           purdue::Muon* bestMu2 = &(muons->at(index->muon_links_.second));

           //Use one for systematics       
           float dummy = 0;
           MomScaleCorrection(rmcor, muMinus, muPlus,
                bestMu1->px_,bestMu1->py_,bestMu1->pz_,bestMu1->q_,
                bestMu2->px_,bestMu2->py_,bestMu2->pz_,bestMu2->q_,
                dummy, _runopt, !isMC);                

           TLorentzVector recoDYcand = muPlus + muMinus;
    
           //in case you apply correction      
           double recoMass_corr = recoDYcand.M();
           double recoRap_corr = fabs(recoDYcand.Rapidity());

           //workaround for no correction 
           double recoMass = index->mass_;
           purdue::Muon* index_mu1 = &(muons->at(index->muon_links_.first));
           purdue::Muon* index_mu2 = &(muons->at(index->muon_links_.second));

           finvM->Fill(recoMass, WEIGHT);
           finvM_Roch->Fill(recoMass_corr, WEIGHT);
           finvM_PU->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);
           finvM_corr->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
           if ( fabs(index_mu1->eta_) < 0.8 && fabs(index_mu2->eta_) < 0.8) {
             finvM_fine_bb->Fill(recoMass, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
             finvM_fineRoch_bb->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
           } else if (fabs(index_mu1->eta_) >= 0.8 && fabs(index_mu2->eta_) >=  0.8) {
             finvM_fine_ee->Fill(recoMass, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
             finvM_fineRoch_ee->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
           } else {    
             finvM_fine_eb->Fill(recoMass, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
             finvM_fineRoch_eb->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
           }
           fRelCombIso->Fill( (bestMu1->chIso03_+bestMu1->neuHadIso03_+bestMu1->emIso03_)/bestMu1->pt_, WEIGHT);
           fRelCombIsoNoEGamma->Fill( (bestMu1->chIso03_+bestMu1->neuHadIso03_)/bestMu1->pt_, WEIGHT);
           fRelCombIso->Fill( (bestMu2->chIso03_+bestMu2->neuHadIso03_+bestMu2->emIso03_)/bestMu2->pt_, WEIGHT);
           fRelCombIsoNoEGamma->Fill( (bestMu2->chIso03_+bestMu2->neuHadIso03_)/bestMu2->pt_, WEIGHT);

           //2D sliced 
           double value = -1;

           for( int j = 0; j < 6; j++ ) {
           // preFSR
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
                if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) value = k + j*24;
              }
            }
          }
          fAbsRap->Fill(value, WEIGHT);
          fAbsRap_PU->Fill(value, WEIGHT*PU_WEIGHT);
          fAbsRap_corr->Fill(value, WEIGHT*PU_WEIGHT*FEWZ_WEIGHT);

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
             
             double recoMass_corr = diel_it->mass_;
             double recoRap_corr = diel_it->y_;

             finvM->Fill(recoMass_corr, WEIGHT);
             //finvM_Roch->Fill(recoMass_corr, WEIGHT);
             finvM_PU->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);
             finvM_corr->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        
             //2D sliced 
             double value = -1;
          
             for( int j = 0; j < 6; j++ ) {
             // preFSR
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
                  if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) value = k + j*24;
                }
              }
            } 
            fAbsRap->Fill(value, WEIGHT);
            fAbsRap_PU->Fill(value, WEIGHT*PU_WEIGHT);
            fAbsRap_corr->Fill(value, WEIGHT*PU_WEIGHT*FEWZ_WEIGHT);

          }//dielectron loop  
        } else if (mode == "EMu") {
          //FIXME don't remember which trigger 
          if (hlt_trigFired[1] == 1 || hlt_trigFired[11] == 1 || hlt_trigFired[12] == 1 || hlt_trigFired[13] == 1 || hlt_trigFired[6] == 1) isTriggered = true;
          if( !isTriggered ) return kTRUE;

          muons->clear();
          b_muons->GetEntry(entry);
          electrons->clear();
          b_electrons->GetEntry(entry);

          if (muons->size()==0 || electrons->size()==0) continue;
          //lepton loop
          int ncands = 0;
          std::vector<purdue::Muon>::const_iterator mu_index;
          std::vector<purdue::Electron>::const_iterator ele_index;
          for (std::vector<purdue::Muon>::const_iterator mu_it = muons->begin(); mu_it != muons->end(); ++mu_it) {

            if ( mu_it->pt_ < 20) continue; //|| mu2->pt_ < 10 ) {
            if (!goodMuon(*mu_it)) continue;

            //good vertex something

            //passed
            mu_index = mu_it;
            for (std::vector<purdue::Electron>::const_iterator ele_it = electrons->begin(); ele_it !=electrons->end(); ++ele_it) {

                if( ele_it->scEt_ < 20) continue; //|| ele1->scEt_ < 10 ) continue;
                if (!(goodElectron(*ele_it))) continue;
                //passed 
                ele_index = ele_it;
                ncands++;
              }//ele loop
            } //muon loop

            //make e-mu candidate
            if (ncands != 1) continue;
              
            double recoMass_corr = EMuMass(mu_index->px_, mu_index->py_, mu_index->pz_, ele_index->px_, ele_index->py_, ele_index->pz_);
            double recoRap_corr = EMuRapidity(mu_index->px_, mu_index->py_, mu_index->pz_, ele_index->px_, ele_index->py_, ele_index->pz_);

            finvM->Fill(recoMass_corr, WEIGHT);
            //finvM_Roch->Fill(recoMass_corr, WEIGHT);
            finvM_PU->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);
            finvM_corr->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

            //2D sliced 
            double value = -1;

            for( int j = 0; j < 6; j++ ) {
               // preFSR
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
                    if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) value = k + j*24;
                }
              }
            }
            fAbsRap->Fill(value, WEIGHT);
            fAbsRap_PU->Fill(value, WEIGHT*PU_WEIGHT);
            fAbsRap_corr->Fill(value, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

        }//EMu mode

   }//end split by trig path

   return kTRUE;
}

void EventSelector_Purdue::SlaveTerminate()
{

   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_Purdue::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   finvM = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm")));
   finvM_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_Roch")));
   finvM_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_PU")));
   finvM_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_corr")));
   finvM_fine_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fine_bb")));
   finvM_fineRoch_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineRoch_bb")));
   finvM_fine_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fine_ee")));
   finvM_fineRoch_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineRoch_ee")));
   finvM_fine_eb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fine_eb")));
   finvM_fineRoch_eb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineRoch_eb")));
   fAbsRap = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap")));
   fAbsRap_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_PU")));
   fAbsRap_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_corr")));

   fRelCombIso = dynamic_cast<TH1D*>(fOutput->FindObject(Form("RelCombIso")));
   fRelCombIsoNoEGamma = dynamic_cast<TH1D*>(fOutput->FindObject(Form("RelCombIsoNoEGamma")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   //normalization independent on the sample size
   if (z != "DATA" && Nntuple->Integral() != 0) {
      finvM->Scale(1./Nntuple->Integral());
      finvM_Roch->Scale(1./Nntuple->Integral());
      finvM_PU->Scale(1./Nntuple->Integral());
      finvM_corr->Scale(1./Nntuple->Integral());
      finvM_fineRoch_bb->Scale(1./Nntuple->Integral());
      finvM_fine_bb->Scale(1./Nntuple->Integral());
      finvM_fineRoch_ee->Scale(1./Nntuple->Integral());
      finvM_fine_ee->Scale(1./Nntuple->Integral());
      finvM_fineRoch_eb->Scale(1./Nntuple->Integral());
      finvM_fine_eb->Scale(1./Nntuple->Integral());
      fAbsRap->Scale(1./Nntuple->Integral());
      fAbsRap_PU->Scale(1./Nntuple->Integral());
      fAbsRap_corr->Scale(1./Nntuple->Integral());
      fRelCombIso->Scale(1./Nntuple->Integral());
      fRelCombIsoNoEGamma->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   this_histogramThis = dynamic_cast<TNamed *>(fInput->FindObject("histogramThis"));
   histogramThis = this_histogramThis ? this_histogramThis->GetTitle() : "";

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   this_momCorrType = dynamic_cast<TNamed *>(fInput->FindObject("momCorrType"));
   momCorrType = this_momCorrType ? this_momCorrType->GetTitle() : "";
   
   std::cout << "NAME = " << z << std::endl;
   //FIXME must have this dir first
   TFile fout("./runfolder/"+z+"_"+histogramThis+"_"+mode+TString(momCorrType)+".root","recreate");
   Nntuple->Write();
   finvM->Write();
   finvM_Roch->Write();
   finvM_PU->Write();
   finvM_corr->Write();
   finvM_fineRoch_bb->Write();
   finvM_fine_bb->Write();
   finvM_fineRoch_ee->Write();
   finvM_fine_ee->Write();
   finvM_fineRoch_eb->Write();
   finvM_fine_eb->Write();
   fAbsRap->Write();
   fAbsRap_PU->Write();
   fAbsRap_corr->Write();
   fRelCombIso->Write();
   fRelCombIsoNoEGamma->Write();
   fout.Close();
}

double EventSelector_Purdue::EMuRapidity( double px1, double py1, double pz1, double px2, double py2, double pz2 ) {

  double mu_mass = 0.105658;
  double e_mass  = 0.000511;

  double p1 = px1*px1 + py1*py1 + pz1*pz1;
  double e1 = sqrt(mu_mass*e_mass + p1);

  double p2 = px2*px2 + py2*py2 + pz2*pz2;
  double e2 = sqrt(mu_mass*e_mass + p2);

  // dimuon system
  double pz = pz1 + pz2;
  double e = e1 + e2;
  // rapidity
  double par = (e + pz)/(e - pz);
  return 0.5*TMath::Log(par);
}

double EventSelector_Purdue::EMuMass( double px1, double py1, double pz1, double px2, double py2, double pz2) {

  double mu_mass = 0.105658;
  double e_mass  = 0.000511;

  double p1 = px1*px1 + py1*py1 + pz1*pz1;
  double e1 = sqrt(mu_mass*e_mass + p1);

  double p2 = px2*px2 + py2*py2 + pz2*pz2;
  double e2 = sqrt(mu_mass*e_mass + p2);

  // dimuon system
  double px = px1 + px2;
  double py = py1 + py2;
  double pz = pz1 + pz2;
  double e = e1 + e2;
  double p3 = px*px + py*py + pz*pz;
  return sqrt( e*e - p3 );
}

bool EventSelector_Purdue::goodMuon(const purdue::Muon& mu1) {
 
   bool accepted = true;

    if( mu1.typeBits_ != 1  ||
        fabs(mu1.eta_) > 2.4 || 
        //muon id
        (mu1.muNchi2_ >= 10 || mu1.muNchi2_ < 0 ) || 
        mu1.nTkHits_ <= 10 || 
        mu1.nPixHits_ < 1  || 
        mu1.nMuHits_ < 1 || 
        mu1.nMatch_ < 2 || 
        mu1.nTrackerLayers_ < 6 || 
        fabs(mu1.d0_) > 0.2 || 
        // isolation
        (mu1.chIso03_+mu1.neuHadIso03_)/mu1.pt_ > 0.2) { accepted = false;}

   return accepted;
}


bool EventSelector_Purdue::goodElectron(const purdue::Electron& ele1) {

  
    if( fabs(ele1.scEta_) > 2.4 ) return false;
    //cut_mHits
    if (ele1.nExpHitsInner_ > 1) return false;
    //FIXME cut_d0vtx
    if (ele1.d0_ >= 0.02) return false;
    //FIXME cut_dzvtx
    if (ele1.dz_ >= 0.1) return false;

    double ooemoop1       = (1.0/ele1.scE_ - ele1.EoverP_/ele1.scE_);

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
        if (fabs(ele1.deltaEtaIn_)          > 0.007) return false;
        //cut_dPhiIn 
        if (fabs(ele1.deltaPhiIn_)          > 0.03) return false;
        //cut_sigmaIEtaIEta
        if (fabs(ele1.sigiEtaiEta_)         > 0.03) return false;
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

