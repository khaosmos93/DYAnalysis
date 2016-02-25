#define EventSelector_MCTruthEff_cxx
//Author: Alexey Svyatkovskiy
//Class for MC truth efficiency and acceptance calculation
//Uses flat ntuple

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

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   //1D 
   hacc = new TH1D("hacc", "hacc", nbin, mass_xbin);
   hsim = new TH1D("hsim", "hsim", nbin, mass_xbin);
   haccFSR = new TH1D("haccFSR", "haccFSR", nbin, mass_xbin);
   hsel = new TH1D("hsel", "hsel", nbin, mass_xbin);
   hfull = new TH1D("hfull", "hfull", nbin, mass_xbin);

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

   GetOutputList()->Add(fAbsRap_full);
   GetOutputList()->Add(fAbsRap_acc);
   GetOutputList()->Add(fAbsRap_accFSR);
   GetOutputList()->Add(fAbsRap_sel);
   GetOutputList()->Add(fAbsRap_sim);

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

   bool isMC = true;

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

      //pileup. FIXME do not apply pile-up to QCD samples yet
      if (isMC && !filename.Contains("QCD")) {
        PU_WEIGHT = pileUpReweight;
      }    

      //construction begin
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
         if( GENMuon1_status[j] != 3 ) continue;
         if( GENMuon2_status[j] != 3 ) continue;
              
         GENIndex = j;
         break;
      }   

       //pre FSr values
       genMass = GENInvMass[GENIndex];
       genRapidity = GENRapidity[GENIndex];

       //binned in GEN mass samples
        if ((filename.Contains("DYM200") || filename.Contains("DYE200")) && genMass > 400) { continue; }
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM800") || filename.Contains("DYE800")) && genMass > 1000) { continue; }

       //pre FSr values
       genDiMuPt = sqrt((GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])*(GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])+(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex])*(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex]));

       //set up FEWZ reweight
       FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);

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
       fAbsRap_full->Fill(value_full);//, WEIGHT);
       fAbsRap_full_PU->Fill(value_full, PU_WEIGHT);//, WEIGHT);
       fAbsRap_full_corr->Fill(value_full,PU_WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

       if( genMass > 15 && genMass < 1500 ) {
              hfull->Fill(genMass);//,WEIGHT);
              hfull_PU->Fill(genMass,PU_WEIGHT);//*WEIGHT);
              hfull_corr->Fill(genMass,PU_WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
       }

       // FSR in Acc
       if( (GENMuon1_pT[GENIndex] > 20 && fabs(GENMuon1_eta[GENIndex]) < 2.4 && GENMuon2_pT[GENIndex] > 10 && fabs(GENMuon2_eta[GENIndex]) < 2.4)
            || (GENMuon1_pT[GENIndex] > 10 && fabs(GENMuon1_eta[GENIndex]) < 2.4 && GENMuon2_pT[GENIndex] > 20 && fabs(GENMuon2_eta[GENIndex]) < 2.4) ) {
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
          fAbsRap_acc->Fill(value_acc);//, WEIGHT);
          fAbsRap_acc_PU->Fill(value_acc,PU_WEIGHT);//, WEIGHT);
          fAbsRap_acc_corr->Fill(value_acc,PU_WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

          if( genMass > 15 && genMass < 1500 ) {
              hacc->Fill(genMass);//,WEIGHT);
              hacc_PU->Fill(genMass,PU_WEIGHT);//*WEIGHT);
              hacc_corr->Fill(genMass,PU_WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
          }
       } //end passAccpreFSR

       // sim. mass
       for( int j = 0; j < GENnPair; j++ ) {
          //if( GENMuon1_mother[j] != 23 ) continue;
          //if( GENMuon2_mother[j] != 23 ) continue;
          if( GENMuon1_status[j] != 1 ) continue;
          if( GENMuon2_status[j] != 1 ) continue;

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
       fAbsRap_sim->Fill(value_sim);//, WEIGHT);
       fAbsRap_sim_PU->Fill(value_sim,PU_WEIGHT);//, WEIGHT);
       fAbsRap_sim_corr->Fill(value_sim,PU_WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

       if( simMass > 15 && simMass < 1500 ) {
            hsim->Fill(simMass);//,WEIGHT);
            hsim_PU->Fill(simMass,PU_WEIGHT);//*WEIGHT);
            hsim_corr->Fill(simMass,PU_WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
       }

       // FSR in Acc
       if( (GENMuon1_pT[simIndex] > 20 && fabs(GENMuon1_eta[simIndex]) < 2.4 && GENMuon2_pT[simIndex] > 10 && fabs(GENMuon2_eta[simIndex]) < 2.4)
              || (GENMuon1_pT[simIndex] > 10 && fabs(GENMuon1_eta[simIndex]) < 2.4 && GENMuon2_pT[simIndex] > 20 && fabs(GENMuon2_eta[simIndex]) < 2.4) ) {
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
        fAbsRap_accFSR->Fill(value_accFSR);//, WEIGHT);
        fAbsRap_accFSR_PU->Fill(value_accFSR,PU_WEIGHT);//, WEIGHT);
        fAbsRap_accFSR_corr->Fill(value_accFSR,PU_WEIGHT*FEWZ_WEIGHT);//, WEIGHT);

        if( simMass > 15 && simMass < 1500 ) { 
           haccFSR->Fill(simMass);//,WEIGHT);
           haccFSR_PU->Fill(simMass,PU_WEIGHT);//*WEIGHT);
           haccFSR_corr->Fill(simMass,PU_WEIGHT*FEWZ_WEIGHT);//*WEIGHT);
        }

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
          if( (Muon1_PfGammaIsoR03[j]+Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.15) continue;
          if( (Muon2_PfGammaIsoR03[j]+Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.15) continue;
          //if( (Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.2) continue;
          //if( (Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.2) continue;

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
         double trigEta[2] = {-999};
         double trigPhi[2] = {-999};
         int _ntrig = 0;
         for( int k = 0; k < hlt_ntrig; k++ ) {
           bool isFired = false;
           if (hlt_trigFired[2] == 1) isFired = true;
           if( !isFired ) continue;
           if( _ntrig == 0 ) {
              trigEta[_ntrig] = hlt_trigEta[k];
              trigPhi[_ntrig] = hlt_trigPhi[k];
              _ntrig++;
           }
           else {
              if( deltaR(hlt_trigEta[k], hlt_trigPhi[k], trigEta[0], trigPhi[0]) < 0.001 ) continue;
              else {
                if( _ntrig == 1 ) {
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
        for( int k = 0; k < 2; k++ ) {
            double dR1 = deltaR(trigEta[k], trigPhi[k], Muon1_eta[index], Muon1_phi[index]);
            double dR2 = deltaR(trigEta[k], trigPhi[k], Muon2_eta[index], Muon2_phi[index]);
            if( dR1 < 0.2 || dR2 < 0.2 ) {
                isMatched++;
            }
        }
        //if( isMatched < 2) continue;

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
        fAbsRap_sel->Fill(value_sel); //, WEIGHT);
        fAbsRap_sel_PU->Fill(value_sel,PU_WEIGHT); //, WEIGHT);
        fAbsRap_sel_corr->Fill(value_sel,PU_WEIGHT*FEWZ_WEIGHT); //, WEIGHT);

        if( simMass > 15 && simMass < 1500 ) {
          hsel->Fill(simMass); //,WEIGHT);
          hsel_PU->Fill(simMass,PU_WEIGHT); //*WEIGHT);
          hsel_corr->Fill(simMass,PU_WEIGHT*FEWZ_WEIGHT); //*WEIGHT);
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

   fAbsRap_full = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_full")));
   fAbsRap_acc = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_acc")));
   fAbsRap_accFSR = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_accFSR")));
   fAbsRap_sel = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_sel")));
   fAbsRap_sim = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_sim")));

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

   fAbsRap_full->Write();
   fAbsRap_acc->Write();
   fAbsRap_accFSR->Write();
   fAbsRap_sel->Write();
   fAbsRap_sim->Write();

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

double EventSelector_MCTruthEff::Rapidity( double px1, double py1, double pz1, double px2, double py2, double pz2 ) {

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
