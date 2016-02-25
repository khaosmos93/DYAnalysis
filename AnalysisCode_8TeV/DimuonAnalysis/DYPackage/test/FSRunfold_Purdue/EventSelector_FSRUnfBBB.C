#define EventSelector_FSRUnfBBB_cxx
// The class definition in EventSelector_FSRUnfBBB.h has been generated automatically
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
// Root > T->Process("EventSelector_FSRUnfBBB.C")
// Root > T->Process("EventSelector_FSRUnfBBB.C","some options")
// Root > T->Process("EventSelector_FSRUnfBBB.C+")
//

#include "EventSelector_FSRUnfBBB.h"
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

EventSelector_FSRUnfBBB::EventSelector_FSRUnfBBB(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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

EventSelector_FSRUnfBBB::~EventSelector_FSRUnfBBB() { }


void EventSelector_FSRUnfBBB::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_FSRUnfBBB::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   //FIXME change matrixces here
   hpostFSR = new TH1D("hpostFSR", "hpostFSR", nbin, mass_xbin);
   hpreFSR = new TH1D("hpreFSR", "hpreFSR", nbin, mass_xbin);

   hpostFSR_PU = new TH1D("hpostFSR_PU", "hpostFSR_PU", nbin, mass_xbin);
   hpreFSR_PU = new TH1D("hpreFSR_PU", "hpreFSR_PU", nbin, mass_xbin);

   hpostFSR_corr = new TH1D("hpostFSR_corr", "hpostFSR_corr", nbin, mass_xbin);
   hpreFSR_corr = new TH1D("hpreFSR_corr", "hpreFSR_corr", nbin, mass_xbin);


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

   hpostFSR2 = new TH1D("hpostFSR2", "hpostFSR2",  nbin2, 0, 132);
   hpreFSR2 = new TH1D("hpreFSR2", "hpreFSR2", nbin2, 0, 132);

   hpostFSR2_PU = new TH1D("hpostFSR2_PU", "hpostFSR2_PU",  nbin2, 0, 132);
   hpreFSR2_PU = new TH1D("hpreFSR2_PU", "hpreFSR2_PU", nbin2, 0, 132);

   hpostFSR_corr2 = new TH1D("hpostFSR2_corr", "hpostFSR2_corr", nbin2, 0, 132);
   hpreFSR_corr2 = new TH1D("hpreFSR2_corr", "hpreFSR2_corr", nbin2, 0, 132);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(hpostFSR);
   GetOutputList()->Add(hpreFSR);

   GetOutputList()->Add(hpostFSR_PU);
   GetOutputList()->Add(hpreFSR_PU);
   
   GetOutputList()->Add(hpostFSR_corr);
   GetOutputList()->Add(hpreFSR_corr);

   GetOutputList()->Add(hpostFSR2);
   GetOutputList()->Add(hpreFSR2);

   GetOutputList()->Add(hpostFSR2_PU);
   GetOutputList()->Add(hpreFSR2_PU);

   GetOutputList()->Add(hpostFSR_corr2);
   GetOutputList()->Add(hpreFSR_corr2);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_FSRUnfBBB::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;  //SHould alkways be true - we will always run it on data

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_FSRUnfBBB::GetEntry() or TBranch::GetEntry()
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
   Nntuple->Fill(0.5);

   // compute the total size of all events
   fTotalDataSize += eventSize;

   if ( fNumberOfEvents % 100000 == 0 ) std::cout << dataset << " : " << fNumberOfEvents << std::endl;

   //split below by trigger path
   for (Int_t trig = 0; trig < 1; trig++) {
    //reset weights
     Double_t WEIGHT = 1.;
     Double_t FEWZ_WEIGHT = 1.;
     Double_t PU_WEIGHT = 1.;

      //pick up the weight
      if (isMC) {
         if (filename.Contains("DYM1020")) {
              WEIGHT = Sigma_DY1020*FilterEff_DY1020;
           } else if ((filename.Contains("DYM200") && !filename.Contains("DYM2000"))) {
              WEIGHT = Sigma_DY200*FilterEff_DY200;
           } else if ((filename.Contains("DYM20") && !filename.Contains("DYM200") && !filename.Contains("DYM2000"))) {
              WEIGHT = Sigma_DY20*FilterEff_DY20;
           } else if (filename.Contains("DYM400")) {
              WEIGHT = Sigma_DY400*FilterEff_DY400;
           } else if (filename.Contains("DYM500")) {  
              WEIGHT = Sigma_DY500*FilterEff_DY500;
           } else if (filename.Contains("DYM700")) {  
              WEIGHT = Sigma_DY700*FilterEff_DY700;
           } else if (filename.Contains("DYM800")) { 
              WEIGHT = Sigma_DY800*FilterEff_DY800;
           } else if (filename.Contains("DYM1000")) { 
              WEIGHT = Sigma_DY1000*FilterEff_DY1000;
           } else if (filename.Contains("DYM1500")) {
              WEIGHT = Sigma_DY1500*FilterEff_DY1500;
           } else if (filename.Contains("DYM2000")) {
              WEIGHT = Sigma_DY2000*FilterEff_DY2000;
           };
       } //isMC

      //pileup
      if (isMC) {
        b_pileUpReweight->GetEntry(entry);
        PU_WEIGHT = pileUpReweight;
      }

       // gen. mass
       double genMass = -1;
       double genRapidity = -1;
       double genDiMuPt = -1;
       int GENIndex = -1;
       b_GENnPair->GetEntry(entry);
       for( int j = 0; j < GENnPair; j++ ) {

          b_GENLepton1_pdgID->GetEntry(entry);
          if( (fabs(GENLepton1_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton1_pdgID[j]) != 11 && mode == "EE")) continue;
          b_GENLepton2_pdgID->GetEntry(entry);
          if( (fabs(GENLepton2_pdgID[j]) != 13 && mode == "MuMu") || (fabs(GENLepton2_pdgID[j]) != 11 && mode == "EE")) continue;
          //pre FSR
          b_GENLepton1_status->GetEntry(entry);
          if( GENLepton1_status[j] != 3 ) continue;
          b_GENLepton2_status->GetEntry(entry);
          if( GENLepton2_status[j] != 3 ) continue;

          genMass = GENInvMass[j];
          genRapidity = GENRapidity[j];
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
       b_GENLepton1_Px->GetEntry(entry);
       double GENLep1_Px = GENLepton1_Px[GENIndex];
       b_GENLepton1_Py->GetEntry(entry);
       double GENLep1_Py = GENLepton1_Py[GENIndex];
       b_GENLepton2_Px->GetEntry(entry);
       double GENLep2_Px = GENLepton2_Px[GENIndex];
       b_GENLepton2_Py->GetEntry(entry);
       double GENLep2_Py = GENLepton2_Py[GENIndex];
       genDiMuPt = sqrt((GENLep1_Px+GENLep2_Px)*(GENLep1_Px+GENLep2_Px)+(GENLep1_Py+GENLep2_Py)*(GENLep1_Py+GENLep2_Py));
       FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);

       //construction begin
       double GENMass = -1;
       double simMass = -1;
       int simIndex = -1;
       for( int j = 0; j < GENnPair; j++ ) {
          b_GENLepton1_status->GetEntry(entry);
          b_GENLepton2_status->GetEntry(entry);
          if( GENLepton1_status[j] == 1 && GENLepton2_status[j] == 1 ) {
              simIndex = j;
          }
          if( GENLepton1_status[j] == 3 && GENLepton2_status[j] == 3 ) {
              GENIndex = j;
          }
       } 

        GENMass = GENInvMass[GENIndex];
        simMass = GENInvMass[simIndex];

        double simRapidity = GENRapidity[simIndex];

        hpreFSR->Fill(GENMass, WEIGHT);
        hpostFSR->Fill(simMass, WEIGHT);

        hpreFSR_PU->Fill(GENMass, PU_WEIGHT*WEIGHT);
        hpostFSR_PU->Fill(simMass, PU_WEIGHT*WEIGHT);

        hpreFSR_corr->Fill(GENMass, WEIGHT*PU_WEIGHT*FEWZ_WEIGHT);
        hpostFSR_corr->Fill(simMass, WEIGHT*PU_WEIGHT*FEWZ_WEIGHT);

        double val_gen = -1;
        double val_sim = -1;
        for( int j = 0; j < 6; j++ ) {
          if( GENMass > Mass_xbin2[j] && GENMass < Mass_xbin2[j+1] ) {
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
                if( fabs(genRapidity) > k*bin_size && fabs(genRapidity) < (k+1)*bin_size ) val_gen = k + j*24;
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
       
        hpreFSR2->Fill(val_gen, WEIGHT);
        hpostFSR2->Fill(val_sim, WEIGHT);

        hpreFSR2_PU->Fill(val_gen, PU_WEIGHT*WEIGHT);
        hpostFSR2_PU->Fill(val_sim, PU_WEIGHT*WEIGHT);

        hpreFSR_corr2->Fill(val_gen, WEIGHT*PU_WEIGHT*FEWZ_WEIGHT);
        hpostFSR_corr2->Fill(val_sim, WEIGHT*PU_WEIGHT*FEWZ_WEIGHT);

   }//end split by trig path

   return kTRUE;
}

void EventSelector_FSRUnfBBB::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_FSRUnfBBB::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   hpreFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR")));
   hpostFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR")));
   hpreFSR_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR_PU")));
   hpostFSR_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR_PU")));
   hpreFSR_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR_corr")));
   hpostFSR_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR_corr")));

   hpreFSR2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR2")));
   hpostFSR2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR2")));
   hpreFSR2_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR2_PU")));
   hpostFSR2_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR2_PU")));
   hpreFSR_corr2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR2_corr")));
   hpostFSR_corr2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR2_corr")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   //normalization independent on the sample size
   if (Nntuple->Integral() != 0) {
      hpostFSR->Scale(1./Nntuple->Integral());
      hpreFSR->Scale(1./Nntuple->Integral());
      hpostFSR_PU->Scale(1./Nntuple->Integral());
      hpreFSR_PU->Scale(1./Nntuple->Integral());
      hpostFSR_corr->Scale(1./Nntuple->Integral());
      hpreFSR_corr->Scale(1./Nntuple->Integral());

      hpostFSR2->Scale(1./Nntuple->Integral());
      hpreFSR2->Scale(1./Nntuple->Integral());
      hpostFSR2_PU->Scale(1./Nntuple->Integral());
      hpreFSR2_PU->Scale(1./Nntuple->Integral());
      hpostFSR_corr2->Scale(1./Nntuple->Integral());
      hpreFSR_corr2->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   std::cout << "NAME = " << z << std::endl;
   TFile fout(z+mode+"_BBB.root","recreate");
   Nntuple->Write();
   hpreFSR->Write();
   hpostFSR->Write();
   hpreFSR_PU->Write();
   hpostFSR_PU->Write();
   hpreFSR_corr->Write();
   hpostFSR_corr->Write();

   hpreFSR2->Write();
   hpostFSR2->Write();
   hpreFSR2_PU->Write();
   hpostFSR2_PU->Write();
   hpreFSR_corr2->Write();
   hpostFSR_corr2->Write();

   fout.Close();
}
