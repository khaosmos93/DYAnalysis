#define EventSelector_CorrMass_cxx
// The class definition in EventSelector_CorrMass.h has been generated automatically
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
// Root > T->Process("EventSelector_CorrMass.C")
// Root > T->Process("EventSelector_CorrMass.C","some options")
// Root > T->Process("EventSelector_CorrMass.C+")
//

#include "EventSelector_CorrMass.h"
#include "correctionWeightToBin.C"
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

EventSelector_CorrMass::EventSelector_CorrMass(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
{
 
  trigPaths[0] = ""; 

  //alternative to ntuple weights
  //Each prior is defined using the following formula:
  //Prior_<MC_sample> = (Cross section*Filter efficiency/Number of GEN events)|<MC sample>
  //this information is manually retrieved for each sample from DAS: https://cmsweb-testbed.cern.ch/das/
  //The first two numbers are provided in the cfg file for "parent" datasets.
  //Alternatively, cross section can be obtainedi from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
  //or: http://cms.cern.ch/iCMS/jsp/mcprod/admin/requestmanagement.jsp?campid=Summer11 
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

EventSelector_CorrMass::~EventSelector_CorrMass() { }


void EventSelector_CorrMass::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_CorrMass::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   // Get the parameters object
   this_effType = dynamic_cast<TNamed *>(fInput->FindObject("effType"));
   effType = this_effType ? this_effType->GetTitle() : "";

   this_runSplit = dynamic_cast<TNamed *>(fInput->FindObject("runSplit"));
   runSplit = this_runSplit ? this_runSplit->GetTitle() : "";

   this_doSmear = dynamic_cast<TNamed *>(fInput->FindObject("doSmear"));
   sdoSmear = this_doSmear ? this_doSmear->GetTitle() : "";
   doSmear = atoi(sdoSmear.c_str());

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   finvM = new TH1D("invm", "invm", nbin, mass_xbin);
   fRap = new TH1D("rap", "rap", 100,-5,5);

   //2D sliced
   fAbsRap2030   = new TH1D("absrap2030", "absrap2030",24,0,2.4);
   fAbsRap3045   = new TH1D("absrap3045", "absrap3045",24,0,2.4);
   fAbsRap4560   = new TH1D("absrap4560", "absrap4560",24,0,2.4);
   fAbsRap60120  = new TH1D("absrap60120", "absrap60120",24,0,2.4);
   fAbsRap120200 = new TH1D("absrap120200", "absrap120200",24,0,2.4);
   fAbsRap200    = new TH1D("absrap200", "absrap200",12,0,2.4);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(finvM);
   GetOutputList()->Add(fRap);

   GetOutputList()->Add(fAbsRap2030);
   GetOutputList()->Add(fAbsRap3045);
   GetOutputList()->Add(fAbsRap4560);
   GetOutputList()->Add(fAbsRap60120);
   GetOutputList()->Add(fAbsRap120200);
   GetOutputList()->Add(fAbsRap200);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_CorrMass::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_CorrMass::GetEntry() or TBranch::GetEntry()
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
         for( int j = 0; j < GENnPair; j++ ) {
          if( GENMuon1_status[j] != 3 ) continue;
          if( GENMuon2_status[j] != 3 ) continue;

          genMass = GENInvMass[j];
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
        //Current 2012 way
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
           if (hlt_trigFired[2] == 1) isFired = true;
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
        if( isMatched < 2) continue;

        if ( InvMass[index] > 15 && InvMass[index] < 1500 ) fNumberOfGoodEvents++;

         //FIXME set up efficiency as a weight
         double dataMC_corr = 1.;

         // RECO+ID+PF
         double dataMC_corr_recoid_muon1 = -1;
         double dataMC_corr_recoid_muon2= -1;

         //if (TString(CORRECTION_TYPE) == "recoid" || TString(CORRECTION_TYPE) == "overall") {
         if (TString(effType) == "recoid" || TString(effType) == "overall") {

            dataMC_corr_recoid_muon1 = correction_RECOID(Muon1_pT[index], Muon1_eta[index], doSmear);
            dataMC_corr_recoid_muon2 = correction_RECOID(Muon2_pT[index], Muon2_eta[index], doSmear);

            if (TString(effType) != "overall") {
               dataMC_corr = dataMC_corr_recoid_muon1*dataMC_corr_recoid_muon2;
            } else {
               dataMC_corr = dataMC_corr_recoid_muon1*dataMC_corr_recoid_muon2;
            }
         }
         //ISO
         double dataMC_corr_iso_muon1= -1;
         double dataMC_corr_iso_muon2= -1;

         //if (TString(CORRECTION_TYPE) == "iso" || TString(CORRECTION_TYPE) == "overall") {
         if (TString(effType) == "iso" || TString(effType) == "overall") {

           dataMC_corr_iso_muon1 = correction_ISO(Muon1_pT[index], Muon1_eta[index], doSmear);
           dataMC_corr_iso_muon2 = correction_ISO(Muon2_pT[index], Muon2_eta[index], doSmear);

           if (TString(effType) != "overall") {

               dataMC_corr = dataMC_corr_iso_muon1*dataMC_corr_iso_muon2;
            } else {
               dataMC_corr *= dataMC_corr_iso_muon1*dataMC_corr_iso_muon2;
            }
         }
         //TRIGGER
         double dataMC_corr_trig_muon1_mu8leg= -1;
         double dataMC_corr_trig_muon2_mu8leg= -1;

         double dataMC_corr_trig_muon1_mu17leg= -1;
         double dataMC_corr_trig_muon2_mu17leg= -1;

         //if (TString(CORRECTION_TYPE) == "trig" || TString(CORRECTION_TYPE) == "overall") {
         if (TString(effType) == "trig" || TString(effType) == "overall") {
            dataMC_corr_trig_muon1_mu8leg = correction_TRIG_TkMu8leg(Muon1_pT[index], Muon1_eta[index], doSmear);//, trig);
            dataMC_corr_trig_muon2_mu8leg = correction_TRIG_TkMu8leg(Muon2_pT[index], Muon2_eta[index], doSmear);//, trig);

            dataMC_corr_trig_muon1_mu17leg = correction_TRIG_Mu17leg(Muon1_pT[index], Muon1_eta[index], doSmear);//, trig);
            dataMC_corr_trig_muon2_mu17leg = correction_TRIG_Mu17leg(Muon2_pT[index], Muon2_eta[index], doSmear);//, trig);
            
            if (TString(effType) != "overall") {
                dataMC_corr = dataMC_corr_trig_muon1_mu8leg*dataMC_corr_trig_muon2_mu17leg+dataMC_corr_trig_muon1_mu17leg*dataMC_corr_trig_muon2_mu8leg-dataMC_corr_trig_muon2_mu17leg*dataMC_corr_trig_muon1_mu17leg ;
              } else {
                  dataMC_corr *= dataMC_corr_trig_muon1_mu8leg*dataMC_corr_trig_muon2_mu17leg+dataMC_corr_trig_muon1_mu17leg*dataMC_corr_trig_muon2_mu8leg-dataMC_corr_trig_muon2_mu17leg*dataMC_corr_trig_muon1_mu17leg ;
             }
          }

           if( dataMC_corr > 0.) {
                 WEIGHT = WEIGHT*dataMC_corr;
           }


        //FIXME old part, just fill the histograms
        finvM->Fill(InvMass[index], WEIGHT);
        fRap->Fill(Rapidity[index], WEIGHT); 

        //2D sliced 
        if (20 < InvMass[index] && 30 > InvMass[index]) {
              fAbsRap2030->Fill(fabs(Rapidity[index]), WEIGHT);
           } else if (30 < InvMass[index] && 45 > InvMass[index]) {
              fAbsRap3045->Fill(fabs(Rapidity[index]), WEIGHT);
           } else if (45 < InvMass[index] && 60 > InvMass[index]) {
              fAbsRap4560->Fill(fabs(Rapidity[index]), WEIGHT);
           } else if (60 < InvMass[index] && 120 > InvMass[index]) {
              fAbsRap60120->Fill(fabs(Rapidity[index]), WEIGHT);
           } else if (120 < InvMass[index] && 200 > InvMass[index]) {
              fAbsRap120200->Fill(fabs(Rapidity[index]), WEIGHT);
           } else if (200 < InvMass[index]) { 
              fAbsRap200->Fill(fabs(Rapidity[index]), WEIGHT);
       }

   }//end split by trig path

   return kTRUE;
}

void EventSelector_CorrMass::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_CorrMass::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   finvM = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm")));
   fRap = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap")));

   fAbsRap2030 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap2030")));
   fAbsRap3045 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap3045")));
   fAbsRap4560 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap4560")));
   fAbsRap60120 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap60120")));
   fAbsRap120200 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap120200")));
   fAbsRap200 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("absrap200")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));
   //normalization independent on the sample size

   if (Nntuple->Integral() != 0) { 
      finvM->Scale(1./Nntuple->Integral());
      fRap->Scale(1./Nntuple->Integral());

      fAbsRap2030->Scale(1./Nntuple->Integral());
      fAbsRap3045->Scale(1./Nntuple->Integral());
      fAbsRap4560->Scale(1./Nntuple->Integral());
      fAbsRap60120->Scale(1./Nntuple->Integral());
      fAbsRap120200->Scale(1./Nntuple->Integral());
      fAbsRap200->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   this_effType = dynamic_cast<TNamed *>(fInput->FindObject("effType"));
   effType = this_effType ? this_effType->GetTitle() : "";

   this_runSplit = dynamic_cast<TNamed *>(fInput->FindObject("runSplit"));
   runSplit = this_runSplit ? this_runSplit->GetTitle() : "";

   this_doSmear = dynamic_cast<TNamed *>(fInput->FindObject("doSmear"));
   sdoSmear = this_doSmear ? this_doSmear->GetTitle() : "";
   doSmear = atoi(sdoSmear.c_str());

   TString systRun = "";
   if (doSmear > 0) {
      systRun = "_syst"+sdoSmear;
   };

   std::cout << "NAME = " << z << std::endl;
   TFile fout(effType+"/"+z+"_"+effType+runSplit+systRun+".root","recreate");
   Nntuple->Write();
   finvM->Write();
   fRap->Write();

   fAbsRap2030->Write();
   fAbsRap3045->Write();
   fAbsRap4560->Write();
   fAbsRap60120->Write();
   fAbsRap120200->Write();
   fAbsRap200->Write();

   fout.Close();
}
