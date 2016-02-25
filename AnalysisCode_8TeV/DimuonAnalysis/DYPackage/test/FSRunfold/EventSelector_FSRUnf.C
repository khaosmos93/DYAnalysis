#define EventSelector_FSRUnf_cxx
// The class definition in EventSelector_FSRUnf.h has been generated automatically
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
// Root > T->Process("EventSelector_FSRUnf.C")
// Root > T->Process("EventSelector_FSRUnf.C","some options")
// Root > T->Process("EventSelector_FSRUnf.C+")
//

#include "EventSelector_FSRUnf.h"
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

EventSelector_FSRUnf::EventSelector_FSRUnf(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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

EventSelector_FSRUnf::~EventSelector_FSRUnf() { }


void EventSelector_FSRUnf::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_FSRUnf::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   const int nbin = 41;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   //FIXME change matrixces here
   hpostFSR = new TH1D("hpostFSR", "hpostFSR", nbin, mass_xbin);
   hpreFSR = new TH1D("hpreFSR", "hpreFSR", nbin, mass_xbin);
   hden = new TH2D("hden", "hden", nbin, mass_xbin, nbin, mass_xbin);
   //FIXME change matrixces here
   hpostFSR_corr = new TH1D("hpostFSR_corr", "hpostFSR_corr", nbin, mass_xbin);
   hpreFSR_corr = new TH1D("hpreFSR_corr", "hpreFSR_corr", nbin, mass_xbin);
   hden_corr = new TH2D("hden_corr", "hden_corr", nbin, mass_xbin, nbin, mass_xbin);

   //2D set up
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 138 bins
   const int nbin2 = 138;
   double mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   hpostFSR2 = new TH1D("hpostFSR2", "hpostFSR2",  nbin2, 0, 138);
   hpreFSR2 = new TH1D("hpreFSR2", "hpreFSR2", nbin2, 0, 138);
   hden2 = new TH2D("hden2", "hden2", nbin2, 0, 138, nbin2, 0, 138);
   //FIXME change matrixces here
   hpostFSR_corr2 = new TH1D("hpostFSR_corr2", "hpostFSR_corr2", nbin2, 0, 138);
   hpreFSR_corr2 = new TH1D("hpreFSR_corr2", "hpreFSR_corr2", nbin2, 0, 138);
   hden_corr2 = new TH2D("hden_corr2", "hden_corr2", nbin2, 0, 138, nbin2, 0, 138);


   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(hpostFSR);
   GetOutputList()->Add(hpreFSR);
   GetOutputList()->Add(hden);
   
   GetOutputList()->Add(hpostFSR_corr);
   GetOutputList()->Add(hpreFSR_corr);
   GetOutputList()->Add(hden_corr);

   GetOutputList()->Add(hpostFSR2);
   GetOutputList()->Add(hpreFSR2);
   GetOutputList()->Add(hden2);

   GetOutputList()->Add(hpostFSR_corr2);
   GetOutputList()->Add(hpreFSR_corr2);
   GetOutputList()->Add(hden_corr2);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_FSRUnf::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;  //SHould alkways be true - we will always run it on data

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_FSRUnf::GetEntry() or TBranch::GetEntry()
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

      //FIXME just a hack to make sure samples are right
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
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200") && !filename.Contains("DYM2000"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM1000") || filename.Contains("DYE1000")) && genMass > 1500) { continue; }
         else if ((filename.Contains("DYM1500") || filename.Contains("DYE1500")) && genMass > 2000) { continue; }


        //pre FSR values
         genRapidity = GENRapidity[GENIndex];
         genDiMuPt = sqrt((GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])*(GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])+(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex])*(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex]));
         //look up FEWZ weight
         FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);

       //construction begin
       double GENMass = -1;
       double simMass = -1;
       int simIndex = -1;
       bool inAcc = false;
       bool inAccPreFSR = false;
       bool inAccPostFSR = false;
       for( int j = 0; j < GENnPair; j++ ) {
           //FIXME you basically only need inAcc case. No need to check...
           if( fabs(GENMuon1_eta[j]) < 2.4 && fabs(GENMuon2_eta[j]) < 2.4 ) {
              if( (GENMuon1_pT[j] > 20 && GENMuon2_pT[j] > 10) || (GENMuon2_pT[j] > 20 && GENMuon1_pT[j] > 10) ) {
               inAcc= true; 
              }
            }  
            if( inAcc ) {
             if( GENMuon1_status[j] == 1 && GENMuon2_status[j] == 1 ) {
              inAccPostFSR = true;
              simIndex = j;
            }
            if( GENMuon1_status[j] == 3 && GENMuon2_status[j] == 3 ) {
              inAccPreFSR = true;
              GENIndex = j;
           }
         }
       } 
       //COMMENT: here we have pre and post FSR index falling in acceptance

       if (!(inAccPostFSR && inAccPreFSR)) continue;

        // pass both
        if( inAccPostFSR && inAccPreFSR) { 
          //may be you can have a global variable here 
          GENMass = GENInvMass[GENIndex];
          simMass = GENInvMass[simIndex];

          double simRapidity = GENRapidity[simIndex];

          //case where you eliminate "not in acceptance" 
          //if( GENMass < 0 ) continue;

          hpreFSR->Fill(GENMass, WEIGHT);
          hpostFSR->Fill(simMass, WEIGHT);
          hden->Fill(GENMass, simMass, WEIGHT);

          hpreFSR_corr->Fill(GENMass, WEIGHT*FEWZ_WEIGHT);
          hpostFSR_corr->Fill(simMass, WEIGHT*FEWZ_WEIGHT);
          hden_corr->Fill(GENMass, simMass, WEIGHT*FEWZ_WEIGHT);

          //FIXME construction ahead
          double val_gen = -1;
          double val_sim = -1;
          for( int j = 0; j < 6; j++ ) {
            if( GENMass > Mass_xbin2[j] && GENMass < Mass_xbin2[j+1] ) {
              int nbins;
              double bin_size;
              if( j == 5 ) {
                nbins = 13;
                bin_size = 0.2;
              }
              else {
                nbins = 25;
                bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                if( fabs(genRapidity) > k*bin_size && fabs(genRapidity) < (k+1)*bin_size ) val_gen = k + j*25;
              }
            }
            if( simMass > Mass_xbin2[j] && simMass < Mass_xbin2[j+1] ) {
              int nbins;
              double bin_size;
              if( j == 5 ) {
                nbins = 13;
                bin_size = 0.2;
              }
              else {
                nbins = 25;
                bin_size = 0.1;
              }
              for( int k = 0; k < nbins; k++ ) {
                if (fabs(simRapidity) > k*bin_size && fabs(simRapidity) < (k+1)*bin_size ) val_sim = k + j*25;
              }
            }
          }
       
          hpreFSR2->Fill(val_gen, WEIGHT);
          hpostFSR2->Fill(val_sim, WEIGHT);
          hden2->Fill(val_gen, val_sim, WEIGHT);

          hpreFSR_corr2->Fill(val_gen, WEIGHT*FEWZ_WEIGHT);
          hpostFSR_corr2->Fill(val_sim, WEIGHT*FEWZ_WEIGHT);
          hden_corr2->Fill(val_gen, val_sim, WEIGHT*FEWZ_WEIGHT);
        }

   }//end split by trig path

   return kTRUE;
}

void EventSelector_FSRUnf::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_FSRUnf::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   hpreFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR")));
   hpostFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR")));
   hden = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden")));
   hpreFSR_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR_corr")));
   hpostFSR_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR_corr")));
   hden_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden_corr")));

   hpreFSR2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR2")));
   hpostFSR2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR2")));
   hden2 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden2")));
   hpreFSR_corr2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpreFSR_corr2")));
   hpostFSR_corr2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hpostFSR_corr2")));
   hden_corr2 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hden_corr2")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   //normalization independent on the sample size
   if (Nntuple->Integral() != 0 && useNtupleWeightFlag != "True") {
      hpostFSR->Scale(1./Nntuple->Integral());
      hpreFSR->Scale(1./Nntuple->Integral());
      hden->Scale(1./Nntuple->Integral());
      hpostFSR_corr->Scale(1./Nntuple->Integral());
      hpreFSR_corr->Scale(1./Nntuple->Integral());
      hden_corr->Scale(1./Nntuple->Integral());

      hpostFSR2->Scale(1./Nntuple->Integral());
      hpreFSR2->Scale(1./Nntuple->Integral());
      hden2->Scale(1./Nntuple->Integral());
      hpostFSR_corr2->Scale(1./Nntuple->Integral());
      hpreFSR_corr2->Scale(1./Nntuple->Integral());
      hden_corr2->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   std::cout << "NAME = " << z << std::endl;
   TFile fout(z+".root","recreate");
   Nntuple->Write();
   hpreFSR->Write();
   hpostFSR->Write();
   hden->Write();
   hpreFSR_corr->Write();
   hpostFSR_corr->Write();
   hden_corr->Write();

   hpreFSR2->Write();
   hpostFSR2->Write();
   hden2->Write();
   hpreFSR_corr2->Write();
   hpostFSR_corr2->Write();
   hden_corr2->Write();

   fout.Close();
}
