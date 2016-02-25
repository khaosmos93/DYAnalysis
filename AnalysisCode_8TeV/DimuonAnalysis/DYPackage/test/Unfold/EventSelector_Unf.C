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
  Sigma_DY20   = 1871.0;
  Sigma_DY200  = 1.485;
  Sigma_DY400  = 0.1086;
  Sigma_DY500  = 0.04415;
  Sigma_DY700  = 0.01024;
  Sigma_DY800  = 0.005491;
  Sigma_DY1000 = 0.001796;
  Sigma_DY1500 = 1.705E-4;
  Sigma_DY2000 = 2.208E-5;

  FilterEff_DY1020 = 1;
  FilterEff_DY20   = 1.;
  FilterEff_DY200  = 1.;
  FilterEff_DY400  = 1.;
  FilterEff_DY500  = 1.;
  FilterEff_DY700  = 1.;
  FilterEff_DY800  = 1.;
  FilterEff_DY1000 = 1.;
  FilterEff_DY1500 = 1.;
  FilterEff_DY2000 = 1.;

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

   //rochcorr
   rmcor = new rochcor2012(333);
   rnd = new TRandom(1551);

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   //1D set up
   const int nbin = 41;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

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
           } else if (filename.Contains("DYM1500")) {
              WEIGHT = Sigma_DY1500*FilterEff_DY1500;
           } else if (filename.Contains("DYM2000")) {
              WEIGHT = Sigma_DY2000*FilterEff_DY2000;
           };
      } //isMC

     //pileup. FIXME do not apply pile-up to QCD samples yet
     if (isMC && !filename.Contains("QCD")) {
        PU_WEIGHT = pileUpReweight;
     }

     if (filename.Contains("DYM")) {
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
      if ((filename.Contains("DYM200") && !filename.Contains("DYM2000")) && genMass > 400) { continue; }
         else if ((filename.Contains("DYM20") && !filename.Contains("DYM200") && !filename.Contains("DYM2000")) && genMass > 200) {continue;}
         else if (filename.Contains("DYM400") && genMass > 500) { continue; }
         else if (filename.Contains("DYM500") && genMass > 700) { continue; }
         else if (filename.Contains("DYM700")  && genMass > 800) { continue; }
         else if (filename.Contains("DYM800")  && genMass > 1000) { continue; }
         else if (filename.Contains("DYM1000") && genMass > 1500) { continue; }
         else if (filename.Contains("DYM1500") && genMass > 2000) { continue; }

     //pre FSR values
     genRapidity = GENRapidity[GENIndex];
     genDiMuPt = sqrt((GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])*(GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])+(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex])*(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex]));
       //look up FEWZ weight
     FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);
     }

     int index = -1;
     double best_val = -99999;
     for( int j = 0; j < nPair; j++ ) {
         //if( Muon1_muonType[j] != 1 ) continue;
         //if( Muon2_muonType[j] != 1 ) continue;

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
	  if( (Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.2) continue;
	  if( (Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.2) continue;

	  // 3D angle
	  if( CosAngle[j] < 0.005 ) continue;

	  // vtx prob
	  if( vtxTrkProb[j] < 0.02 ) continue;

          if(!isOppSign[j]) continue;

	    if( vtxTrkProb[j] > best_val ) {
	      best_val = vtxTrkProb[j];
              index = j;
            }
        }
	if( index == -1 ) continue;

        bool isTriggered = false;
        for( int k = 0; k < hlt_ntrig; k++ ) {
             if( (hlt_trigName->at((unsigned int)k)) == "HLT_Mu17_TkMu8_v*") {
                if( hlt_trigFired[k] == 1 ) {
                  isTriggered = true;
                  break;
               }
             }
         }
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
           if( (hlt_trigName->at((unsigned int)k)) == "HLT_Mu17_TkMu8_v*") {
               if( hlt_trigFired[k] == 1 ) {
                   isFired = true;
              }
           }
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

        // gen. mass
        double simMass = -1;
        int simIndex = -1;
        double simRapidity = -1;
        for( int j = 0; j < GENnPair; j++ ) {
            // post-FSR
           if( GENMuon1_status[j] != 1 ) continue;
           if( GENMuon2_status[j] != 1 ) continue;
            
           // cout << "genMass " << genMass << endl;
           simMass = GENInvMass[j];
           simRapidity = GENRapidity[j];
           break;
        }


        // set for momentum correction
        // Only for RECO, do nothing for GEN
        TLorentzVector muMinus;
        TLorentzVector muPlus;

        float dummy = 0;
        MomScaleCorrection(rmcor, muMinus, muPlus,
                Muon1_Px[index], Muon1_Py[index], Muon1_Pz[index], Muon1_charge[index],
                Muon2_Px[index], Muon2_Py[index], Muon2_Pz[index], Muon2_charge[index],
                dummy, dummy, !isMC);

        TLorentzVector recoDYcand = muPlus + muMinus;

        double recoMass_corr = recoDYcand.M();
        double recoRapidity_corr = fabs(recoDYcand.Rapidity());

        //put the fills 
        htrue->Fill(simMass, WEIGHT);
        hmeas->Fill(InvMass[index], WEIGHT);
        hden->Fill(simMass, InvMass[index], WEIGHT);

        htrue_Roch->Fill(simMass, WEIGHT);
        hmeas_Roch->Fill(recoMass_corr, WEIGHT);
        hden_Roch->Fill(simMass, recoMass_corr, WEIGHT);

        htrue_PU->Fill(simMass, PU_WEIGHT*WEIGHT);
        hmeas_PU->Fill(recoMass_corr, PU_WEIGHT*WEIGHT);
        hden_PU->Fill(simMass, recoMass_corr, PU_WEIGHT*WEIGHT);

        htrue_corr->Fill(simMass, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        hmeas_corr->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        hden_corr->Fill(simMass, recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

        //2D case
        double val_reco = -1;
        double val_sim = -1;
        double recoRapidity = Rapidity(Muon1_Px[index], Muon1_Py[index], Muon1_Pz[index],Muon2_Px[index], Muon2_Py[index], Muon2_Pz[index]);

        for( int j = 0; j < 6; j++ ) {
          if( InvMass[index] > Mass_xbin2[j] && InvMass[index] < Mass_xbin2[j+1] ) {
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
              if( fabs(recoRapidity) > k*bin_size && fabs(recoRapidity) < (k+1)*bin_size ) val_reco = k + j*24;
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
              if( fabs(recoRapidity_corr) > k*bin_size && fabs(recoRapidity_corr) < (k+1)*bin_size ) val_reco = k + j*24;
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

          hmeas2_PU->Fill(val_reco, PU_WEIGHT*WEIGHT);
          htrue2_PU->Fill(val_sim, PU_WEIGHT*WEIGHT);
          hden2_PU->Fill(val_reco, val_sim, PU_WEIGHT*WEIGHT);

          hmeas2_corr->Fill(val_reco, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          htrue2_corr->Fill(val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          hden2_corr->Fill(val_reco, val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

        //over under flows
        if( recoMass_corr > 0 && recoMass_corr < 20 ) {
            double bin_size = 0.1;
            //FIXME reset
            val_reco = -1;
            for( int k = 0; k < 24; k++ ) {
              if( fabs(recoRapidity_corr) > k*bin_size && fabs(recoRapidity_corr) < (k+1)*bin_size ) val_reco = k;
            }
            hmeas2_24_Roch->Fill(val_reco, WEIGHT);
            hmeas2_24_PU->Fill(val_reco, PU_WEIGHT*WEIGHT);
            hmeas2_24_corr->Fill(val_reco, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        }
        if( simMass > 0 && simMass < 20 ) {
            double bin_size = 0.1;
            //FIXME reset
            val_sim = -1;
            for( int k = 0; k < 24; k++ ) {
              if (fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) val_sim = k;
            }
            htrue2_24_Roch->Fill(val_sim, WEIGHT);
            htrue2_24_PU->Fill(val_sim, PU_WEIGHT*WEIGHT);
            htrue2_24_corr->Fill(val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        }
/*
        if( recoMass_corr > 1500 && recoMass_corr < 10000 ) {
            double bin_size = 0.2;
            for( int k = 0; k < 12; k++ ) {
              if( fabs(recoRapidity_corr) > k*bin_size && fabs(recoRapidity_corr) < (k+1)*bin_size ) val_reco = k;
            }
          hmeas2_12->Fill(val_reco, WEIGHT);
          hmeas2_12_corr->Fill(val_reco, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        }
        if( simMass > 1500 && simMass < 10000 ) {
            double bin_size = 0.2;
            for( int k = 0; k < 12; k++ ) {
              if (fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) val_sim = k;
            }
          htrue2_12->Fill(val_sim, WEIGHT);
          htrue2_12_corr->Fill(val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
        }
*/
        //2D case
        val_reco = -1;
        val_sim = -1;
        for( int jj = 0; jj < 7; jj++ ) {
          if( InvMass[index] > Mass_xbin2_156[jj] && InvMass[index] < Mass_xbin2_156[jj+1] ) {
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
              if( fabs(recoRapidity) > kk*bin_size && fabs(recoRapidity) < (kk+1)*bin_size ) val_reco = kk + jj*24;
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
              if( fabs(recoRapidity_corr) > kk*bin_size && fabs(recoRapidity_corr) < (kk+1)*bin_size ) val_reco = kk + jj*24;
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
        hden2_156_PU->Fill(val_reco, val_sim, PU_WEIGHT*WEIGHT);
        hden2_156_corr->Fill(val_reco, val_sim, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

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

   std::cout << "NAME = " << z << std::endl;
   TFile fout(z+"_"+".root","recreate");
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
