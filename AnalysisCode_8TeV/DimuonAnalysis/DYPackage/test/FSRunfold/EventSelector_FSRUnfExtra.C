#define EventSelector_FSRUnfExtra_cxx
// The class definition in EventSelector_FSRUnfExtra.h has been generated automatically
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
// Root > T->Process("EventSelector_FSRUnfExtra.C")
// Root > T->Process("EventSelector_FSRUnfExtra.C","some options")
// Root > T->Process("EventSelector_FSRUnfExtra.C+")
//

#include "EventSelector_FSRUnfExtra.h"
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

EventSelector_FSRUnfExtra::EventSelector_FSRUnfExtra(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
{
 
  trigPaths[0] = "";

  //alternative to ntuple weights
  //Each prior is defined using the following formula:
  //Prior_<MC_sample> = (Cross section*Filter efficiency/Number of GEN events)|<MC sample>
  //this information is manually retrieved for each sample from DAS: https://cmsweb-testbed.cern.ch/das/
  //The first two numbers are provided in the cfg file for "parent" datasets.
  //Alternatively, cross section can be obtainedi from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
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

EventSelector_FSRUnfExtra::~EventSelector_FSRUnfExtra() { }


void EventSelector_FSRUnfExtra::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_FSRUnfExtra::SlaveBegin(TTree*)
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

   //1D measurement histograms
   h1DFrac0 = new TH1D("h1DFrac0", "h1DFrac0", nbin, mass_xbin);
   h1DFrac1 = new TH1D("h1DFrac1", "h1DFrac1", nbin, mass_xbin);
   h1DFrac2 = new TH1D("h1DFrac2", "h1DFrac2", nbin, mass_xbin);

   h1DFrac0_corr = new TH1D("h1DFrac0_corr", "h1DFrac0_corr", nbin, mass_xbin);
   h1DFrac1_corr = new TH1D("h1DFrac1_corr", "h1DFrac1_corr", nbin, mass_xbin);
   h1DFrac2_corr = new TH1D("h1DFrac2_corr", "h1DFrac2_corr", nbin, mass_xbin);

   //2D measurement histograms
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 138 bins
   const int nbin2 = 138;
   double mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   hFrac0 = new TH2D("hFrac0", "hFrac0", 6, mass_xbin2, 24, 0., 2.4);
   hFrac1 = new TH2D("hFrac1", "hFrac1", 6, mass_xbin2, 24, 0., 2.4);
   hFrac2 = new TH2D("hFrac2", "hFrac2", 6, mass_xbin2, 24, 0., 2.4);

   hFrac0_corr = new TH2D("hFrac0_corr", "hFrac0_corr", 6, mass_xbin2, 24, 0., 2.4);
   hFrac1_corr = new TH2D("hFrac1_corr", "hFrac1_corr", 6, mass_xbin2, 24, 0., 2.4);
   hFrac2_corr = new TH2D("hFrac2_corr", "hFrac2_corr", 6, mass_xbin2, 24, 0., 2.4);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(h1DFrac0);
   GetOutputList()->Add(h1DFrac1);
   GetOutputList()->Add(h1DFrac2);

   GetOutputList()->Add(hFrac0);
   GetOutputList()->Add(hFrac1);
   GetOutputList()->Add(hFrac2);

   GetOutputList()->Add(h1DFrac0_corr);
   GetOutputList()->Add(h1DFrac1_corr);
   GetOutputList()->Add(h1DFrac2_corr);

   GetOutputList()->Add(hFrac0_corr);
   GetOutputList()->Add(hFrac1_corr);
   GetOutputList()->Add(hFrac2_corr);


   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_FSRUnfExtra::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;  //SHould alkways be true - we will always run it on data

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_FSRUnfExtra::GetEntry() or TBranch::GetEntry()
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
           } else if (filename.Contains("DYM1500") || filename.Contains("DYE1500")) {
              WEIGHT = Sigma_DY1500*FilterEff_DY1500;
           } else if (filename.Contains("DYM2000") || filename.Contains("DYE2000")) {
              WEIGHT = Sigma_DY2000*FilterEff_DY2000;
           };
       } //isMC

        // gen. mass
        double GENMass = -1;
        double GENrapidity = -999;
        bool isAccPassPostFSR = false;
        bool isAccPassPreFSR = false;
        int GENIndex = -1;
        for( int j = 0; j < GENnPair; j++ ) {
          //FEWZ needs: looks like a dupliocation of code to me, but certainly causes no err 
          if( GENMuon1_status[j] == 3 && GENMuon2_status[j] == 3 ) {
              GENIndex = j;
          }
 
          GENMass = GENInvMass[j];
          GENrapidity = GENRapidity[j];

          bool isPassAcc = false;
          if( fabs(GENMuon1_eta[j]) < 2.4 && fabs(GENMuon2_eta[j]) < 2.4 ) {
            if( (GENMuon1_pT[j] > 20 && GENMuon2_pT[j] > 10) 
              || (GENMuon2_pT[j] > 20 && GENMuon1_pT[j] > 10) ) isPassAcc = true;
          }
          if( isPassAcc ) {
            if( GENMuon1_status[j] == 1 && GENMuon2_status[j] == 1 ) isAccPassPostFSR = true;
            if( GENMuon1_status[j] == 3 && GENMuon2_status[j] == 3 ) isAccPassPreFSR = true;
          }
        } 

        double genMass = GENInvMass[GENIndex];

       //binned in GEN mass samples
        if ((filename.Contains("DYM200") || filename.Contains("DYE200")) && genMass > 400) { continue; }
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM800") || filename.Contains("DYE800")) && genMass > 1000) { continue; }
         else if ((filename.Contains("DYM1000") || filename.Contains("DYE1000")) && genMass > 1500) { continue; }
         else if ((filename.Contains("DYM1500") || filename.Contains("DYE1500")) && genMass > 2000) { continue; }


        //set up FEWZ
        double genRapidity = GENRapidity[GENIndex];
        double genDiMuPt = sqrt((GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])*(GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])+(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex])*(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex]));
        //look up FEWZ weight
        FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);
      
        // fraction1: pass acc for post-FSR but not pass it for pre-FSR
        if( isAccPassPostFSR && !isAccPassPreFSR ) {
          h1DFrac1->Fill(GENMass, WEIGHT);
          hFrac1->Fill(GENMass, fabs(GENrapidity), WEIGHT);

          h1DFrac1_corr->Fill(GENMass, WEIGHT*FEWZ_WEIGHT);
          hFrac1_corr->Fill(GENMass, fabs(GENrapidity), WEIGHT*FEWZ_WEIGHT);
        }
        // fraction2: not pass acc for post-FSR but pass it for pre-FSR
        if( !isAccPassPostFSR && isAccPassPreFSR ) {
          h1DFrac2->Fill(GENMass, WEIGHT);
          hFrac2->Fill(GENMass, fabs(GENrapidity), WEIGHT);

          h1DFrac2_corr->Fill(GENMass, WEIGHT*FEWZ_WEIGHT);
          hFrac2_corr->Fill(GENMass, fabs(GENrapidity), WEIGHT*FEWZ_WEIGHT);
        }
        // fraction3: pass both
        if( isAccPassPostFSR && isAccPassPreFSR ) {
          h1DFrac0->Fill(GENMass, WEIGHT);
          hFrac0->Fill(GENMass, fabs(GENrapidity), WEIGHT);

          h1DFrac0_corr->Fill(GENMass, WEIGHT*FEWZ_WEIGHT);
          hFrac0_corr->Fill(GENMass, fabs(GENrapidity), WEIGHT*FEWZ_WEIGHT);
        }
      //}

   }//end split by trig path

   return kTRUE;
}

void EventSelector_FSRUnfExtra::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_FSRUnfExtra::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   h1DFrac0 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("h1DFrac0")));
   h1DFrac1 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("h1DFrac1")));
   h1DFrac2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("h1DFrac2")));
   hFrac0 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hFrac0")));
   hFrac1 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hFrac1")));
   hFrac2 = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hFrac2")));

   h1DFrac0_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("h1DFrac0_corr")));
   h1DFrac1_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("h1DFrac1_corr")));
   h1DFrac2_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("h1DFrac2_corr")));
   hFrac0_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hFrac0_corr")));
   hFrac1_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hFrac1_corr")));
   hFrac2_corr = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hFrac2_corr")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   //normalization independent on the sample size
   if (Nntuple->Integral() != 0 && useNtupleWeightFlag != "True") {

      h1DFrac0->Scale(1./Nntuple->Integral());
      h1DFrac1->Scale(1./Nntuple->Integral());
      h1DFrac2->Scale(1./Nntuple->Integral());
      hFrac0->Scale(1./Nntuple->Integral());
      hFrac1->Scale(1./Nntuple->Integral());
      hFrac2->Scale(1./Nntuple->Integral());

      h1DFrac0_corr->Scale(1./Nntuple->Integral());
      h1DFrac1_corr->Scale(1./Nntuple->Integral());
      h1DFrac2_corr->Scale(1./Nntuple->Integral());
      hFrac0_corr->Scale(1./Nntuple->Integral());
      hFrac1_corr->Scale(1./Nntuple->Integral());
      hFrac2_corr->Scale(1./Nntuple->Integral());

   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   std::cout << "NAME = " << z << std::endl;
   TFile fout(z+"_2.root","recreate");
   Nntuple->Write();

   h1DFrac0->Write();
   h1DFrac1->Write();
   h1DFrac2->Write();
   hFrac0->Write();
   hFrac1->Write();
   hFrac2->Write();

   h1DFrac0_corr->Write();
   h1DFrac1_corr->Write();
   h1DFrac2_corr->Write();
   hFrac0_corr->Write();
   hFrac1_corr->Write();
   hFrac2_corr->Write();

   fout.Close();
}
