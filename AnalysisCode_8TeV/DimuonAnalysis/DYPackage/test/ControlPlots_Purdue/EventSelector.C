#define EventSelector_cxx
// The class definition in EventSelector.h has been generated automatically
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
// Root > T->Process("EventSelector.C")
// Root > T->Process("EventSelector.C","some options")
// Root > T->Process("EventSelector.C+")
//

#include "EventSelector.h"
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


EventSelector::EventSelector(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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
  Sigma_DY20   = 1871.0;

  FilterEff_DY20   = 1.;
}

EventSelector::~EventSelector() { }


void EventSelector::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector::SlaveBegin(TTree*)
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

   finvM = new TH1D("invm", "invm", nbin, mass_xbin);

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

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(finvM);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);
}

Bool_t EventSelector::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector::GetEntry() or TBranch::GetEntry()
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

     //split data mc
     if ( dataset == "DATA") {
        isMC = false;
        if (trig != 0) continue; 
     }
     

      //pick up the weight
      if (isMC) {
           if ((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) { 
              WEIGHT = Sigma_DY20*FilterEff_DY20;
           }
      } //isMC

         b_GENnPair->GetEntry(entry);
         b_GENInvMass->GetEntry(entry);
         b_GENLepton1_pdgID->GetEntry(entry);
         b_GENLepton1_status->GetEntry(entry);
         b_GENLepton2_pdgID->GetEntry(entry); 
         b_GENLepton2_status->GetEntry(entry); 


         // gen. mass
         double genMass = -1;
         double genRapidity = -1;
         double genDiMuPt = -1;
         int GENIndex = -1;
         for( int j = 0; j < GENnPair; j++ ) {
          cout << "Invariant mass " << GENInvMass[j] << endl;
          if( fabs(GENLepton1_pdgID[j]) != 13) continue;
          if( fabs(GENLepton2_pdgID[j]) != 13) continue;
          //pre FSR
          if( GENLepton1_status[j] != 3 ) continue;
          if( GENLepton2_status[j] != 3 ) continue;

          genMass = GENInvMass[j];
          break;
         }

         finvM->Fill(genMass);//,WEIGHT);
      //}
   }//end split by trig path

   return kTRUE;
}

void EventSelector::SlaveTerminate()
{

   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   finvM = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   this_mode = dynamic_cast<TNamed *>(fInput->FindObject("mode"));
   mode = this_mode ? this_mode->GetTitle() : "";

   //normalization independent on the sample size
   if (z != "DATA" && Nntuple->Integral() != 0) {
      finvM->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   std::cout << "NAME = " << z << std::endl;
   //FIXME must have this dir first
   TFile fout("./runfolder/"+z+mode+".root","recreate");
   Nntuple->Write();
   finvM->Write();
   fout.Close();
}
