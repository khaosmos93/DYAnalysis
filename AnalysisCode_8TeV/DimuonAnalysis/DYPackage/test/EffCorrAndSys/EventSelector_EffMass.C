#define EventSelector_EffMass_cxx
// The class definition in EventSelector_EffMass.h has been generated automatically
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
// Root > T->Process("EventSelector_EffMass.C")
// Root > T->Process("EventSelector_EffMass.C","some options")
// Root > T->Process("EventSelector_EffMass.C+")
//

#include "EventSelector_EffMass.h"
#include "efficiencyWeightToBin.C"
#include <iostream>
#include <TProof.h>
#include <TProofServ.h>
#include <TDSet.h>
#include <TEventList.h>
#include <TParameter.h>
#include <TMath.h>
#include <TRandom.h>

//#define USE_NTUPLE_WEIGHTS false

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

EventSelector_EffMass::EventSelector_EffMass(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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
  Sigma_DY20   = 1871.0;
  Sigma_DY200  = 1.485;
  Sigma_DY400  = 0.1086;
  Sigma_DY500  = 0.04415;
  Sigma_DY700  = 0.01024;
  Sigma_DY800  = 0.005491;
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

EventSelector_EffMass::~EventSelector_EffMass() { }


void EventSelector_EffMass::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_EffMass::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   // Get the parameters object
   this_effType = dynamic_cast<TNamed *>(fInput->FindObject("effType"));
   effType = this_effType ? this_effType->GetTitle() : "";

   this_scenario = dynamic_cast<TNamed *>(fInput->FindObject("scenario"));
   scenario = this_scenario ? this_scenario->GetTitle() : "";

   this_runSplit = dynamic_cast<TNamed *>(fInput->FindObject("runSplit"));
   runSplit = this_runSplit ? this_runSplit->GetTitle() : "";

   this_doSmear = dynamic_cast<TNamed *>(fInput->FindObject("doSmear"));
   doSmear = this_doSmear ? this_doSmear->GetTitle() : "";

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   finvM = new TH1D("invm", "invm", nbin, mass_xbin);
   finvMErr = new TH1D("invm_err", "invm_err", nbin, mass_xbin);
   fRap = new TH1D("rap", "rap", 100,-5,5);

   //2D sliced
   fAbsRap2030   = new TH1D("absrap2030", "absrap2030",24,0,2.4);
   fAbsRap3045   = new TH1D("absrap3045", "absrap3045",24,0,2.4);
   fAbsRap4560   = new TH1D("absrap4560", "absrap4560",24,0,2.4);
   fAbsRap60120  = new TH1D("absrap60120", "absrap60120",24,0,2.4);
   fAbsRap120200 = new TH1D("absrap120200", "absrap120200",24,0,2.4);
   fAbsRap200    = new TH1D("absrap200", "absrap200",12,0,2.4);

   fAbsRap2030Err   = new TH1D("absrap2030_err", "absrap2030_err",24,0,2.4);
   fAbsRap3045Err   = new TH1D("absrap3045_err", "absrap3045_err",24,0,2.4);
   fAbsRap4560Err   = new TH1D("absrap4560_err", "absrap4560_err",24,0,2.4);
   fAbsRap60120Err  = new TH1D("absrap60120_err", "absrap60120_err",24,0,2.4);
   fAbsRap120200Err = new TH1D("absrap120200_err", "absrap120200_err",24,0,2.4);
   fAbsRap200Err    = new TH1D("absrap200_err", "absrap200_err",12,0,2.4);


   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(finvM);
   GetOutputList()->Add(finvMErr);
   GetOutputList()->Add(fRap);

   GetOutputList()->Add(fAbsRap2030);
   GetOutputList()->Add(fAbsRap3045);
   GetOutputList()->Add(fAbsRap4560);
   GetOutputList()->Add(fAbsRap60120);
   GetOutputList()->Add(fAbsRap120200);
   GetOutputList()->Add(fAbsRap200);

   GetOutputList()->Add(fAbsRap2030Err);
   GetOutputList()->Add(fAbsRap3045Err);
   GetOutputList()->Add(fAbsRap4560Err);
   GetOutputList()->Add(fAbsRap60120Err);
   GetOutputList()->Add(fAbsRap120200Err);
   GetOutputList()->Add(fAbsRap200Err);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_EffMass::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_EffMass::GetEntry() or TBranch::GetEntry()
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
     Double_t WEIGHT_ERR = 1.;

     //split data mc
     if ( dataset == "DATA") isMC = false;

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
           }
      } //isMC

        // gen. mass
        double genMass = -1;
        double genRapidity = -1;
        double genDiMuPt = -1;
        int GENIndex = -1;
         for( int j = 0; j < GENnPair; j++ ) {
          if( GENMuon1_status[j] != 3 ) continue;
          if( GENMuon2_status[j] != 3 ) continue;

          genMass = GENInvMass[j];
          GENIndex = j;
          break;
       }

       //binned in GEN mass samples
         if ((filename.Contains("DYM200") || filename.Contains("DYE200")) && genMass > 400) { continue; }
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM800") || filename.Contains("DYE800")) && genMass > 1000) { continue; }

     int index = -1;
     double best_val = -99999;
     for( int j = 0; j < nPair; j++ ) {
         if( Muon1_muonType[j] != 1 ) continue;
         if( Muon2_muonType[j] != 1 ) continue;

         if( Muon1_pT[j] < 20 || Muon2_pT[j] < 10 ) {
            if( Muon2_pT[j] < 20 || Muon1_pT[j] < 10 ) continue;
            }

	 if( fabs(Muon1_eta[j]) > 2.4 ) continue;
	 if( fabs(Muon2_eta[j]) > 2.4 ) continue;

	  // Muon id
	  if( Muon1_chi2dof[j] >= 10 || Muon1_chi2dof[j] < 0 ) continue;
	  if( Muon2_chi2dof[j] >= 10 || Muon2_chi2dof[j] < 0 ) continue;
    
	  if( Muon1_trackerHits[j] <= 10 ) continue;
	  if( Muon2_trackerHits[j] <= 10 ) continue;
	  if( Muon1_pixelHits[j] < 1 ) continue;
	  if( Muon2_pixelHits[j] < 1 ) continue;
	  if( Muon1_muonHits[j] < 1 ) continue;
	  if( Muon2_muonHits[j] < 1 ) continue;
	  if( Muon1_nMatches[j] < 2 ) continue;
	  if( Muon2_nMatches[j] < 2 ) continue;
          if (Muon1_nTrackerLayers[j] < 6) continue;
          if (Muon2_nTrackerLayers[j] < 6) continue;

	  if( fabs(Muon1_dxyBS[j]) > 0.2 ) continue;
	  if( fabs(Muon2_dxyBS[j]) > 0.2 ) continue;
	    
	  // isolation
	  //if( (Muon1_PfGammaIsoR03[j]+Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.15) continue;
	  //if( (Muon2_PfGammaIsoR03[j]+Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.15) continue;
          if( (Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.2) continue;
          if( (Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.2) continue;

	  // 3D angle
	  if( CosAngle[j] < 0.005 ) continue;

	  // vtx prob
	  if( vtxTrkProb[j] < 0.02 ) continue;

          if( !isOppSign[j]) continue;

	    if( vtxTrkProb[j] > best_val ) {
	      best_val = vtxTrkProb[j];
              index = j;
            }
        }
	if( index == -1 ) continue;

        bool isTriggered = false;
        if (hlt_trigFired[2] == 1) isTriggered = true;
        if( !isTriggered ) return kTRUE;

         //trigger matching begin
         int isMatched = 0;
         //double trigType[2] = {-1};
         //double trigPt[2] = {-1};
         double trigEta[2] = {-999};
         double trigPhi[2] = {-999};
         int _ntrig = 0;
         for( int k = 0; k < hlt_ntrig; k++ ) {
           bool isFired = false;
           if (hlt_trigFired[3] == 1) isFired = true;
           if( !isFired ) continue;
            //Just make sure that there is two objects trigger 
            //matched - nothing more! Don't care which ones exactly 
           if( _ntrig == 0 ) {
              //trigType[_ntrig] = hlt_trigType[k];
              //trigPt[_ntrig] = hlt_trigPt[k];
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
            //cout << "trig: " << k << " " << trigType[k] << " " << trigPt[k] << " " << trigEta[k] << " " << trigPhi[k] << endl;
            double dR1 = deltaR(trigEta[k], trigPhi[k], Muon1_eta[index], Muon1_phi[index]);
            double dR2 = deltaR(trigEta[k], trigPhi[k], Muon2_eta[index], Muon2_phi[index]);
            if( dR1 < 0.2 || dR2 < 0.2 ) {
                isMatched++;
            }
        }
        //if( isMatched < 2) continue;

        if ( InvMass[index] > 15 && InvMass[index] < 1500 ) fNumberOfGoodEvents++;

         //FIXME set up efficiency as a weight

         double data_eff = -1; double mc_eff = -1;
         double data_eff_err = -1; double mc_eff_err = -1;
         double scale_factor = 1.;

         // RECO+ID+PF
         double data_eff_recoid_muon1 = -1;
         double data_eff_recoid_muon2= -1;
         double mc_eff_recoid_muon1= -1;
         double mc_eff_recoid_muon2= -1;

         double data_eff_recoid_muon1_err= -1;
         double data_eff_recoid_muon2_err= -1;
         double mc_eff_recoid_muon1_err= -1;
         double mc_eff_recoid_muon2_err= -1;

         //if (TString(CORRECTION_TYPE) == "recoid" || TString(CORRECTION_TYPE) == "overall") {
         if (TString(effType) == "recoid" || TString(effType) == "overall") {

          if (scenario == "data") {
            data_eff_recoid_muon1 = efficiency_RECOID(Muon1_pT[index], Muon1_eta[index], false, runSplit);
            data_eff_recoid_muon2 = efficiency_RECOID(Muon2_pT[index], Muon2_eta[index], false, runSplit);
            //data_eff = data_eff_recoid_muon1*data_eff_recoid_muon2;

            data_eff_recoid_muon1_err = efficiencyError_RECOID(Muon1_pT[index], Muon1_eta[index], false, runSplit);
            data_eff_recoid_muon2_err = efficiencyError_RECOID(Muon2_pT[index], Muon2_eta[index], false, runSplit);

            //FIXME
            if (doSmear == "True") {
               data_eff_recoid_muon1 = gRandom->Gaus(data_eff_recoid_muon1, data_eff_recoid_muon1_err); 
               data_eff_recoid_muon2 = gRandom->Gaus(data_eff_recoid_muon2, data_eff_recoid_muon2_err);
            }

          } else {
            mc_eff_recoid_muon1 = efficiency_RECOID(Muon1_pT[index], Muon1_eta[index], true, runSplit);
            mc_eff_recoid_muon2 = efficiency_RECOID(Muon2_pT[index], Muon2_eta[index], true, runSplit);
            //mc_eff = mc_eff_recoid_muon1*mc_eff_recoid_muon2;

            mc_eff_recoid_muon1_err = efficiencyError_RECOID(Muon1_pT[index], Muon1_eta[index], true, runSplit);
            mc_eff_recoid_muon2_err = efficiencyError_RECOID(Muon2_pT[index], Muon2_eta[index], true, runSplit);

            //FIXME
            if (doSmear == "True") {
               mc_eff_recoid_muon1 = gRandom->Gaus(mc_eff_recoid_muon1, mc_eff_recoid_muon1_err);
               mc_eff_recoid_muon2 = gRandom->Gaus(mc_eff_recoid_muon2, mc_eff_recoid_muon2_err);
            }

          }
            if (TString(effType) != "overall") {
              if (scenario == "data") {
                 data_eff = data_eff_recoid_muon1*data_eff_recoid_muon2;
                 data_eff_err = sqrt(pow(data_eff_recoid_muon1_err/data_eff_recoid_muon1,2)+ pow(data_eff_recoid_muon2_err/data_eff_recoid_muon2,2));
              } else {
                 mc_eff = mc_eff_recoid_muon1*mc_eff_recoid_muon2;
                 mc_eff_err = sqrt(pow(mc_eff_recoid_muon1_err/mc_eff_recoid_muon1,2)+ pow(mc_eff_recoid_muon2_err/mc_eff_recoid_muon2,2));
              }
            } else {
              if (scenario == "data") {
                data_eff = data_eff_recoid_muon1*data_eff_recoid_muon2;
                data_eff_err = pow(data_eff_recoid_muon1_err/data_eff_recoid_muon1,2)+ pow(data_eff_recoid_muon2_err/data_eff_recoid_muon2,2);  
              } else {
                mc_eff = mc_eff_recoid_muon1*mc_eff_recoid_muon2;
                mc_eff_err = pow(data_eff_recoid_muon1_err/data_eff_recoid_muon1,2)+ pow(data_eff_recoid_muon2_err/data_eff_recoid_muon2,2);
              }
            }
         }
         //ISO
         double data_eff_iso_muon1= -1;
         double data_eff_iso_muon2= -1;
         double mc_eff_iso_muon1= -1;
         double mc_eff_iso_muon2= -1;

         double data_eff_iso_muon1_err= -1;
         double data_eff_iso_muon2_err= -1;
         double mc_eff_iso_muon1_err= -1;
         double mc_eff_iso_muon2_err= -1;

         //if (TString(CORRECTION_TYPE) == "iso" || TString(CORRECTION_TYPE) == "overall") {
         if (TString(effType) == "iso" || TString(effType) == "overall") {

          if (scenario == "data") {
            data_eff_iso_muon1 = efficiency_ISO(Muon1_pT[index], Muon1_eta[index], false, runSplit);
            data_eff_iso_muon2 = efficiency_ISO(Muon2_pT[index], Muon2_eta[index], false, runSplit);

            data_eff_iso_muon1_err = efficiencyError_ISO(Muon1_pT[index], Muon1_eta[index], false, runSplit);
            data_eff_iso_muon2_err = efficiencyError_ISO(Muon2_pT[index], Muon2_eta[index], false, runSplit);

            //FIXME
            if (doSmear == "True") {

               data_eff_iso_muon1 = efficiency_ISO(Muon1_pT[index], Muon1_eta[index], false, runSplit);
               data_eff_iso_muon2 = efficiency_ISO(Muon2_pT[index], Muon2_eta[index], false, runSplit);
            }

          } else {
            mc_eff_iso_muon1 = efficiency_ISO(Muon1_pT[index], Muon1_eta[index], true, runSplit);
            mc_eff_iso_muon2 = efficiency_ISO(Muon2_pT[index], Muon2_eta[index], true, runSplit);

            mc_eff_iso_muon1_err = efficiencyError_ISO(Muon1_pT[index], Muon1_eta[index], true, runSplit);
            mc_eff_iso_muon2_err = efficiencyError_ISO(Muon2_pT[index], Muon2_eta[index], true, runSplit);

            //FIXME
            if (doSmear == "True") {

               mc_eff_iso_muon1 = efficiency_ISO(Muon1_pT[index], Muon1_eta[index], true, runSplit);
               mc_eff_iso_muon2 = efficiency_ISO(Muon2_pT[index], Muon2_eta[index], true, runSplit);
            }

          }
            //if (TString(CORRECTION_TYPE) != "overall") {
            if (TString(effType) != "overall") {

              if (scenario == "data") {
                 data_eff = data_eff_iso_muon1*data_eff_iso_muon2;
                 data_eff_err = sqrt(pow(data_eff_iso_muon1_err/data_eff_iso_muon1,2)+ pow(data_eff_iso_muon2_err/data_eff_iso_muon2,2));
              } else {
                 mc_eff = mc_eff_iso_muon1*mc_eff_iso_muon2;
                 mc_eff_err = sqrt(pow(mc_eff_iso_muon1_err/mc_eff_iso_muon1,2)+ pow(mc_eff_iso_muon2_err/mc_eff_iso_muon2,2));
              }
            } else {
              if (scenario == "data") {
                data_eff *= data_eff_iso_muon1*data_eff_iso_muon2;
                data_eff_err += pow(data_eff_iso_muon1_err/data_eff_iso_muon1,2)+ pow(data_eff_iso_muon2_err/data_eff_iso_muon2,2);
              } else {
                mc_eff *= mc_eff_iso_muon1*mc_eff_iso_muon2;
                mc_eff_err += pow(mc_eff_iso_muon1_err/mc_eff_iso_muon1,2)+ pow(mc_eff_iso_muon2_err/mc_eff_iso_muon2,2);
              }
            }
         }
         //TRIGGER
         double data_eff_trig_muon1_mu8leg= -1;
         double data_eff_trig_muon2_mu8leg= -1;
         double mc_eff_trig_muon1_mu8leg= -1;
         double mc_eff_trig_muon2_mu8leg= -1;

         double data_eff_trig_muon1_mu8leg_err= -1;
         double data_eff_trig_muon2_mu8leg_err= -1;
         double mc_eff_trig_muon1_mu8leg_err= -1;
         double mc_eff_trig_muon2_mu8leg_err= -1;

         double data_eff_trig_muon1_mu17leg= -1;
         double data_eff_trig_muon2_mu17leg= -1;
         double mc_eff_trig_muon1_mu17leg= -1;
         double mc_eff_trig_muon2_mu17leg= -1;

         double data_eff_trig_muon1_mu17leg_err= -1;
         double data_eff_trig_muon2_mu17leg_err= -1;
         double mc_eff_trig_muon1_mu17leg_err= -1;
         double mc_eff_trig_muon2_mu17leg_err= -1;
/*
         double data_eff_dz = -1;
         double mc_eff_dz = -1;
         double data_eff_dz_err = -1;
         double mc_eff_dz_err = -1;
         double data_eff_trig_muon1_mu8= -1;
         double data_eff_trig_muon2_mu8= -1;
         double mc_eff_trig_muon1_mu8= -1;
         double mc_eff_trig_muon2_mu8= -1;
          
         double data_eff_trig_muon1_mu8_err= -1;
         double data_eff_trig_muon2_mu8_err= -1;
         double mc_eff_trig_muon1_mu8_err= -1;
         double mc_eff_trig_muon2_mu8_err= -1;
         
         double data_eff_trig_muon1_tagmu17leg= -1;
         double data_eff_trig_muon2_tagmu17leg= -1;
         double mc_eff_trig_muon1_tagmu17leg= -1;
         double mc_eff_trig_muon2_tagmu17leg= -1;

         double data_eff_trig_muon1_tagmu17leg_err= -1;
         double data_eff_trig_muon2_tagmu17leg_err= -1;
         double mc_eff_trig_muon1_tagmu17leg_err= -1;
         double mc_eff_trig_muon2_tagmu17leg_err= -1;

         double data_eff_trig_muon1_tagmu17= -1;
         double data_eff_trig_muon2_tagmu17= -1;
         double mc_eff_trig_muon1_tagmu17= -1;
         double mc_eff_trig_muon2_tagmu17= -1;

         double data_eff_trig_muon1_tagmu17_err= -1;
         double data_eff_trig_muon2_tagmu17_err= -1;
         double mc_eff_trig_muon1_tagmu17_err= -1;
         double mc_eff_trig_muon2_tagmu17_err= -1;

         //DZ filter set up
         //formula should look like this:  probe*tag/probe_leg*tag_leg
         //where the way probe determined is
         // if (mu1_pT > mu2_pT) probe is mu1
         double probe_mc = 0;
         double tag_mc = 0;
         double probe_mc_leg = 0;
         double tag_mc_leg = 0;
         double probe_data = 0;
         double tag_data = 0;
         double probe_data_leg = 0;
         double tag_data_leg = 0;

         double probe_mc_err = 0;
         double tag_mc_err = 0;
         double probe_mc_leg_err = 0;
         double tag_mc_leg_err = 0;
         double probe_data_err = 0;
         double tag_data_err = 0;
         double probe_data_leg_err = 0;
         double tag_data_leg_err = 0;
*/

         //if (TString(CORRECTION_TYPE) == "trig" || TString(CORRECTION_TYPE) == "overall") {
         if (TString(effType) == "trig" || TString(effType) == "overall") {
          if (scenario == "data") {
            data_eff_trig_muon1_mu8leg = efficiency_TRIG_TkMu8leg(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_mu8leg = efficiency_TRIG_TkMu8leg(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_mu8leg_err = efficiencyError_TRIG_TkMu8leg(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_mu8leg_err = efficiencyError_TRIG_TkMu8leg(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            //FIXME
            if (doSmear == "True") {
               data_eff_trig_muon1_mu8leg = gRandom->Gaus(data_eff_trig_muon1_mu8leg, data_eff_trig_muon1_mu8leg_err);
               data_eff_trig_muon2_mu8leg = gRandom->Gaus(data_eff_trig_muon2_mu8leg, data_eff_trig_muon2_mu8leg_err);
            }

            data_eff_trig_muon1_mu17leg = efficiency_TRIG_Mu17leg(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_mu17leg = efficiency_TRIG_Mu17leg(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_mu17leg_err = efficiencyError_TRIG_Mu17leg(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_mu17leg_err = efficiencyError_TRIG_Mu17leg(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            //FIXME
            if (doSmear == "True") {
               data_eff_trig_muon1_mu17leg = gRandom->Gaus(data_eff_trig_muon1_mu17leg, data_eff_trig_muon1_mu17leg_err);
               data_eff_trig_muon2_mu17leg = gRandom->Gaus(data_eff_trig_muon2_mu17leg, data_eff_trig_muon2_mu17leg_err);
            } 
/*
            //DZ filter
            data_eff_trig_muon1_mu8 = efficiency_TRIG_TkMu8(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_mu8 = efficiency_TRIG_TkMu8(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_mu8_err = efficiencyError_TRIG_TkMu8(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_mu8_err = efficiencyError_TRIG_TkMu8(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_tagmu17 = efficiency_TRIG_tag_Mu17(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_tagmu17 = efficiency_TRIG_tag_Mu17(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_tagmu17_err = efficiencyError_TRIG_tag_Mu17(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_tagmu17_err = efficiencyError_TRIG_tag_Mu17(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_tagmu17leg = efficiency_TRIG_tag_Mu17leg(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_tagmu17leg = efficiency_TRIG_tag_Mu17leg(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            data_eff_trig_muon1_tagmu17leg_err = efficiencyError_TRIG_tag_Mu17leg(Muon1_pT[index], Muon1_eta[index], false, runSplit);//, trig);
            data_eff_trig_muon2_tagmu17leg_err = efficiencyError_TRIG_tag_Mu17leg(Muon2_pT[index], Muon2_eta[index], false, runSplit);//, trig);

            if (Muon1_pT[index] > Muon2_pT[index]) {
              probe_data = data_eff_trig_muon1_mu8;
              tag_data = data_eff_trig_muon2_tagmu17;

              probe_data_leg = data_eff_trig_muon1_mu8leg;
              tag_data_leg = data_eff_trig_muon2_tagmu17leg;

              probe_data_err = data_eff_trig_muon1_mu8_err;
              tag_data_err = data_eff_trig_muon2_tagmu17_err;

              probe_data_leg_err = data_eff_trig_muon1_mu8leg_err;
              tag_data_leg_err = data_eff_trig_muon2_tagmu17leg_err;
            } else {
              probe_data = data_eff_trig_muon2_mu8;
              tag_data = data_eff_trig_muon1_tagmu17;

              probe_data_leg = data_eff_trig_muon2_mu8leg;
              tag_data_leg = data_eff_trig_muon1_tagmu17leg;

              probe_data_err = data_eff_trig_muon2_mu8_err;
              tag_data_err = data_eff_trig_muon1_tagmu17_err;

              probe_data_leg_err = data_eff_trig_muon2_mu8leg_err;
              tag_data_leg_err = data_eff_trig_muon1_tagmu17leg_err;
            }
*/
          } else {
             mc_eff_trig_muon1_mu8leg = efficiency_TRIG_TkMu8leg(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
             mc_eff_trig_muon2_mu8leg = efficiency_TRIG_TkMu8leg(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

             mc_eff_trig_muon1_mu8leg_err = efficiencyError_TRIG_TkMu8leg(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
             mc_eff_trig_muon2_mu8leg_err = efficiencyError_TRIG_TkMu8leg(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            //FIXME
            if (doSmear == "True") {
               mc_eff_trig_muon1_mu8leg = gRandom->Gaus(mc_eff_trig_muon1_mu8leg, mc_eff_trig_muon1_mu8leg_err);
               mc_eff_trig_muon2_mu8leg = gRandom->Gaus(mc_eff_trig_muon2_mu8leg, mc_eff_trig_muon2_mu8leg_err);
            } 

             mc_eff_trig_muon1_mu17leg = efficiency_TRIG_Mu17leg(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
             mc_eff_trig_muon2_mu17leg = efficiency_TRIG_Mu17leg(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

             mc_eff_trig_muon1_mu17leg_err = efficiencyError_TRIG_Mu17leg(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
             mc_eff_trig_muon2_mu17leg_err = efficiencyError_TRIG_Mu17leg(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            //FIXME
            if (doSmear == "True") {
               mc_eff_trig_muon1_mu17leg = gRandom->Gaus(mc_eff_trig_muon1_mu17leg, mc_eff_trig_muon1_mu17leg_err);
               mc_eff_trig_muon2_mu17leg = gRandom->Gaus(mc_eff_trig_muon2_mu17leg, mc_eff_trig_muon2_mu17leg_err);
            }
/*
            //DZ filter
            mc_eff_trig_muon1_mu8 = efficiency_TRIG_TkMu8(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
            mc_eff_trig_muon2_mu8 = efficiency_TRIG_TkMu8(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            mc_eff_trig_muon1_mu8_err = efficiencyError_TRIG_TkMu8(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
            mc_eff_trig_muon2_mu8_err = efficiencyError_TRIG_TkMu8(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            mc_eff_trig_muon1_tagmu17 = efficiency_TRIG_tag_Mu17(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
            mc_eff_trig_muon2_tagmu17 = efficiency_TRIG_tag_Mu17(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            mc_eff_trig_muon1_tagmu17_err = efficiencyError_TRIG_tag_Mu17(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
            mc_eff_trig_muon2_tagmu17_err = efficiencyError_TRIG_tag_Mu17(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            mc_eff_trig_muon1_tagmu17leg = efficiency_TRIG_tag_Mu17leg(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
            mc_eff_trig_muon2_tagmu17leg = efficiency_TRIG_tag_Mu17leg(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            mc_eff_trig_muon1_tagmu17leg_err = efficiencyError_TRIG_tag_Mu17leg(Muon1_pT[index], Muon1_eta[index], true, runSplit);//, trig);
            mc_eff_trig_muon2_tagmu17leg_err = efficiencyError_TRIG_tag_Mu17leg(Muon2_pT[index], Muon2_eta[index], true, runSplit);//, trig);

            if (Muon1_pT[index] > Muon2_pT[index]) {
              probe_mc = mc_eff_trig_muon1_mu8;
              tag_mc = mc_eff_trig_muon2_tagmu17;

              probe_mc_leg = mc_eff_trig_muon1_mu8leg;
              tag_mc_leg = mc_eff_trig_muon2_tagmu17leg;

              probe_mc_err = mc_eff_trig_muon1_mu8_err;
              tag_mc_err = mc_eff_trig_muon2_tagmu17_err;

              probe_mc_leg_err = mc_eff_trig_muon1_mu8leg_err;
              tag_mc_leg_err = mc_eff_trig_muon2_tagmu17leg_err;
            } else {
              probe_mc = mc_eff_trig_muon2_mu8;
              tag_mc = mc_eff_trig_muon1_tagmu17;

              probe_mc_leg = mc_eff_trig_muon2_mu8leg;
              tag_mc_leg = mc_eff_trig_muon1_tagmu17leg;

              probe_mc_err = mc_eff_trig_muon2_mu8_err;
              tag_mc_err = mc_eff_trig_muon1_tagmu17_err;

              probe_mc_leg_err = mc_eff_trig_muon2_mu8leg_err;
              tag_mc_leg_err = mc_eff_trig_muon1_tagmu17leg_err;
            }
*/
          }
            //if (TString(CORRECTION_TYPE) != "overall") {
            if (TString(effType) != "overall") {
                  //FIXME decide by lower pT muon, i.e. by probe essentially?
                  double muon_pT = (Muon1_pT[index] < Muon2_pT[index] ? Muon1_pT[index] : Muon2_pT[index]);
                  double muon_eta1 = (Muon1_pT[index] < Muon2_pT[index] ? Muon1_eta[index] : Muon2_eta[index]);
                  double muon_eta2 = (Muon1_pT[index] < Muon2_pT[index] ? Muon2_eta[index] : Muon1_eta[index]);

              if (scenario == "data") {
                  data_eff = data_eff_trig_muon1_mu8leg*data_eff_trig_muon2_mu17leg+data_eff_trig_muon1_mu17leg*data_eff_trig_muon2_mu8leg-data_eff_trig_muon2_mu17leg*data_eff_trig_muon1_mu17leg ;
                  data_eff_err = sqrt(pow(data_eff_trig_muon1_mu8leg_err/data_eff_trig_muon1_mu8leg,2)+ pow(data_eff_trig_muon2_mu8leg_err/data_eff_trig_muon2_mu8leg,2));

/*
                 //dz filter
                  data_eff_dz = 1;//efficiency_TRIG_DZ(muon_pT,muon_eta1,muon_eta2, false , ""); //probe_data*tag_data/(probe_data_leg*tag_data_leg);
                  data_eff_dz_err = sqrt(pow(probe_data_err/probe_data,2)+pow(tag_data_err/tag_data,2)+pow(probe_data_leg_err/probe_data_leg_err,2)+pow(tag_data_leg_err/tag_data_leg,2));
                  //FIXME temporary hack, sets the trigger efficiency to DZ filter relative to trigger efficiency
                  //IS OFF WHEN COMMENTED
                  //data_eff_err = sqrt(pow(data_eff_err/data_eff,2)+pow(data_eff_dz_err/data_eff_dz,2));  
                  //data_eff = data_eff_dz;
*/
              } else {
                  mc_eff = mc_eff_trig_muon1_mu8leg*mc_eff_trig_muon2_mu17leg+mc_eff_trig_muon1_mu17leg*mc_eff_trig_muon2_mu8leg-mc_eff_trig_muon2_mu17leg*mc_eff_trig_muon1_mu17leg ; 
                  mc_eff_err = sqrt(pow(mc_eff_trig_muon1_mu8leg_err/mc_eff_trig_muon1_mu8leg,2)+ pow(mc_eff_trig_muon2_mu8leg_err/mc_eff_trig_muon2_mu8leg,2));
/*
                  //dz filter
                  mc_eff_dz = 1;//efficiency_TRIG_DZ(muon_pT,muon_eta1,muon_eta2, true , ""); //probe_mc*tag_mc/(probe_mc_leg*tag_mc_leg);
                  mc_eff_dz_err = sqrt(pow(probe_mc_err/probe_mc,2)+pow(tag_mc_err/tag_mc,2)+pow(probe_mc_leg_err/probe_mc_leg_err,2)+pow(tag_mc_leg_err/tag_mc_leg,2));

                  //FIXME temporary hack, sets the trigger efficiency to DZ filter relative to trigger efficiency
                  //mc_eff_err = sqrt(pow(mc_eff_err/mc_eff,2)+pow(mc_eff_dz_err/mc_eff_dz,2));
                  //mc_eff = mc_eff_dz;
*/
                }
              } else {
                //FIXME DZ filter efficiency is not part of the overall yet
                if (scenario == "data") {
                  data_eff *= data_eff_trig_muon1_mu8leg*data_eff_trig_muon2_mu17leg+data_eff_trig_muon1_mu17leg*data_eff_trig_muon2_mu8leg-data_eff_trig_muon2_mu17leg*data_eff_trig_muon1_mu17leg ;
                  data_eff_err += pow(data_eff_trig_muon1_mu8leg_err/data_eff_trig_muon1_mu8leg,2)+ pow(data_eff_trig_muon2_mu8leg_err/data_eff_trig_muon2_mu8leg,2); 
                  data_eff_err = sqrt(data_eff_err);
               } else {
                  mc_eff *= mc_eff_trig_muon1_mu8leg*mc_eff_trig_muon2_mu17leg+mc_eff_trig_muon1_mu17leg*mc_eff_trig_muon2_mu8leg-mc_eff_trig_muon2_mu17leg*mc_eff_trig_muon1_mu17leg ;
                  mc_eff_err += pow(mc_eff_trig_muon1_mu8leg_err/mc_eff_trig_muon1_mu8leg,2)+ pow(mc_eff_trig_muon2_mu8leg_err/mc_eff_trig_muon2_mu8leg,2); 
                  mc_eff_err = sqrt(mc_eff_err);
                }
             }
          }

	  //FIXME now modify the weights by combining the MC event weights with the efficiency weights
           if (scenario == "mc") {
              if( mc_eff_err > 0.) {
                   WEIGHT_ERR = WEIGHT_ERR*mc_eff_err;
              }
              if( mc_eff > 0.) {
                   WEIGHT = WEIGHT*mc_eff;
              }
           } else {
              if( data_eff_err > 0.) {
                   WEIGHT_ERR = WEIGHT_ERR*data_eff_err;
              }
              if( data_eff > 0.) {
                   WEIGHT = WEIGHT*data_eff;
              }
           }


        //FIXME old part, just fill the histograms
        finvM->Fill(InvMass[index], WEIGHT);
        finvMErr->Fill(InvMass[index], WEIGHT_ERR);
        fRap->Fill(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index]), WEIGHT); 

        //2D sliced 
        if (20 < InvMass[index] && 30 > InvMass[index]) {
              fAbsRap2030->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT);
              fAbsRap2030Err->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT_ERR);
           } else if (30 < InvMass[index] && 45 > InvMass[index]) {
              fAbsRap3045->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT);
              fAbsRap3045Err->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT_ERR);
           } else if (45 < InvMass[index] && 60 > InvMass[index]) {
              fAbsRap4560->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT);
              fAbsRap4560Err->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT_ERR);
           } else if (60 < InvMass[index] && 120 > InvMass[index]) {
              fAbsRap60120->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT);
              fAbsRap60120Err->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT_ERR);
           } else if (120 < InvMass[index] && 200 > InvMass[index]) {
              fAbsRap120200->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT);
              fAbsRap120200Err->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT_ERR);
           } else if (200 < InvMass[index]) { 
              fAbsRap200->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT);
              fAbsRap200Err->Fill(fabs(Rapidity(Muon1_Px[index],Muon1_Py[index],Muon1_Pz[index],Muon2_Px[index],Muon2_Py[index],Muon2_Pz[index])), WEIGHT_ERR);
       }

   }//end split by trig path

   return kTRUE;
}

void EventSelector_EffMass::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_EffMass::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   finvM = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm")));
   finvMErr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_err")));
   fRap = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap")));

   fAbsRap2030 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap2030")));
   fAbsRap3045 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap3045")));
   fAbsRap4560 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap4560")));
   fAbsRap60120 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap60120")));
   fAbsRap120200 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap120200")));
   fAbsRap200 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap200")));

   fAbsRap2030Err = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap2030_err")));
   fAbsRap3045Err = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap3045_err")));
   fAbsRap4560Err = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap4560_err")));
   fAbsRap60120Err = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap60120_err")));
   fAbsRap120200Err = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap120200_err")));
   fAbsRap200Err = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap200_err")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));
   //normalization independent on the sample size

   if (Nntuple->Integral() != 0) { 
      finvM->Scale(1./Nntuple->Integral());
      finvMErr->Scale(1./Nntuple->Integral());
      fRap->Scale(1./Nntuple->Integral());

      fAbsRap2030->Scale(1./Nntuple->Integral());
      fAbsRap3045->Scale(1./Nntuple->Integral());
      fAbsRap4560->Scale(1./Nntuple->Integral());
      fAbsRap60120->Scale(1./Nntuple->Integral());
      fAbsRap120200->Scale(1./Nntuple->Integral());
      fAbsRap200->Scale(1./Nntuple->Integral());

      fAbsRap2030Err->Scale(1./Nntuple->Integral());
      fAbsRap3045Err->Scale(1./Nntuple->Integral());
      fAbsRap4560Err->Scale(1./Nntuple->Integral());
      fAbsRap60120Err->Scale(1./Nntuple->Integral());
      fAbsRap120200Err->Scale(1./Nntuple->Integral());
      fAbsRap200Err->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   this_effType = dynamic_cast<TNamed *>(fInput->FindObject("effType"));
   effType = this_effType ? this_effType->GetTitle() : "";

   this_scenario = dynamic_cast<TNamed *>(fInput->FindObject("scenario"));
   scenario = this_scenario ? this_scenario->GetTitle() : "";

   this_runSplit = dynamic_cast<TNamed *>(fInput->FindObject("runSplit"));
   runSplit = this_runSplit ? this_runSplit->GetTitle() : "";

   this_doSmear = dynamic_cast<TNamed *>(fInput->FindObject("doSmear"));
   doSmear = this_doSmear ? this_doSmear->GetTitle() : "";

   this_binChoice = dynamic_cast<TNamed *>(fInput->FindObject("binChoice"));
   binChoice = this_binChoice ? this_binChoice->GetTitle() : "";

   TString systDir = "";
   if (doSmear == "True") {
      systDir = "_syst";
   } 

   std::cout << "NAME = " << z << std::endl;
   TFile fout(effType+systDir+"/"+z+"_"+scenario+"_"+effType+runSplit+"_"+binChoice+".root","recreate");
   Nntuple->Write();
   finvM->Write();
   finvMErr->Write();
   fRap->Write();

   fAbsRap2030->Write();
   fAbsRap3045->Write();
   fAbsRap4560->Write();
   fAbsRap60120->Write();
   fAbsRap120200->Write();
   fAbsRap200->Write();

   fAbsRap2030Err->Write();
   fAbsRap3045Err->Write();
   fAbsRap4560Err->Write();
   fAbsRap60120Err->Write();
   fAbsRap120200Err->Write();
   fAbsRap200Err->Write();

   fout.Close();
}

double EventSelector_EffMass::Rapidity( double px1, double py1, double pz1, double px2, double py2, double pz2 ) {

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
