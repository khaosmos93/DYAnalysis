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

EventSelector::EventSelector(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
{
 
  //set the luminosity in a given run range
  //if normalization to luminosity is used  
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

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   finvM = new TH1D("invm", "invm", nbin, mass_xbin);
   finvM_PU = new TH1D("invm_PU", "invm_PU", nbin, mass_xbin);
   finvM_corr = new TH1D("invm_corr", "invm_corr", nbin, mass_xbin);

   finvMGEN = new TH1D("invm_GEN", "invm_GEN", nbin, mass_xbin);
   finvMGEN_corr = new TH1D("invm_GEN_corr", "invm_GEN_corr", nbin, mass_xbin);

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
   fAbsRap_corr = new TH1D("rap_corr", "rap_corr", nbin2, 0, 132);
   fAbsRap_PU = new TH1D("rap_PU", "rap_PU", nbin2, 0, 132);
   fAbsRapGEN = new TH1D("rap_GEN", "rap_GEN", nbin2, 0, 132);
   fAbsRapGEN_corr = new TH1D("rap_GEN_corr", "rap_GEN_corr", nbin2, 0, 132);


   //POWHEG cross section map;
   double Ybins[8] = {0.,0.2,0.4,0.7,1.1,1.9,2.4,1000.0};
   double PTbins[21] = {0.0,20.0,30.,35.,40.,45.,50.,55.,60.,65.,70.,75.,80.,85.,90.,100.,120.,150.,300.,600.,1000.};

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(finvM);
   GetOutputList()->Add(finvM_PU);
   GetOutputList()->Add(finvM_corr);

   GetOutputList()->Add(finvMGEN);
   GetOutputList()->Add(finvMGEN_corr);

   GetOutputList()->Add(fAbsRap);
   GetOutputList()->Add(fAbsRap_PU);
   GetOutputList()->Add(fAbsRap_corr);
   GetOutputList()->Add(fAbsRapGEN);
   GetOutputList()->Add(fAbsRapGEN_corr);

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

   Int_t eventSize = fChain->GetTree()->GetEntry(entry);
   ++fNumberOfEvents;

   //normalization purposes
   Nntuple->Fill(0.5);

//   cout << "Size of the Tree " << fChain->GetTree()->GetEntries() << << endl;

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
           }
      } //isMC

      //FIXME pileup
      if (isMC) {
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
        if ((filename.Contains("DYM200") || filename.Contains("DYE200")) && genMass > 400) { continue; }
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM800") || filename.Contains("DYE800")) && genMass > 1000) { continue; }

       //pre FSR values
       genRapidity = GENRapidity[GENIndex];
       genDiMuPt = sqrt((GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])*(GENMuon1_Px[GENIndex]+GENMuon2_Px[GENIndex])+(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex])*(GENMuon1_Py[GENIndex]+GENMuon2_Py[GENIndex]));  
       //look up FEWZ weight
       FEWZ_WEIGHT = weight(genDiMuPt, fabs(genRapidity), genMass, true);

       //special histogram
       finvMGEN->Fill(genMass, WEIGHT); 
       finvMGEN_corr->Fill(genMass,FEWZ_WEIGHT*WEIGHT); 

        //2D sliced 
        double gen_value = -1;

        for( int j = 0; j < 6; j++ ) {
          // preFSR
          if( genMass > Mass_xbin2[j] && genMass < Mass_xbin2[j+1] ) {
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
              if( fabs(GENRapidity[GENIndex]) > k*bin_size && fabs(GENRapidity[GENIndex]) < (k+1)*bin_size ) gen_value = k + j*24;
            }
          }
        } 
       fAbsRapGEN->Fill(gen_value,WEIGHT);
       fAbsRapGEN_corr->Fill(gen_value,FEWZ_WEIGHT*WEIGHT); 
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
        //FIXME 5 is Mu13Mu8, 2 is Mu17TkMu8
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

        double recoMass = InvMass[index];
        double recoRap = Rapidity( Muon1_Px[index], Muon1_Py[index], Muon1_Pz[index],Muon2_Px[index], Muon2_Py[index], Muon2_Pz[index]);

        finvM->Fill(recoMass, WEIGHT);
        finvM_PU->Fill(recoMass, WEIGHT*PU_WEIGHT);
        finvM_corr->Fill(recoMass, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);

        //2D sliced 
        double value = -1;

        for( int j = 0; j < 6; j++ ) {
          // preFSR
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
              if( fabs(recoRap) > k*bin_size && fabs(recoRap) < (k+1)*bin_size ) value = k + j*24;
            }
          }
        }
        fAbsRap->Fill(value, WEIGHT);
        fAbsRap_PU->Fill(value, PU_WEIGHT*WEIGHT);
        fAbsRap_corr->Fill(value, PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);

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

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   finvM = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm")));
   finvM_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_PU")));
   finvM_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_corr")));

   finvMGEN = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_GEN")));
   finvMGEN_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_GEN_corr")));

   fAbsRap = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap")));
   fAbsRap_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_corr")));
   fAbsRap_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_PU")));
   fAbsRapGEN = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_GEN")));
   fAbsRapGEN_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_GEN_corr")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   //normalization independent on the sample size
   if (z != "DATA" && Nntuple->Integral() != 0) {
      finvM->Scale(1./Nntuple->Integral());
      finvM_PU->Scale(1./Nntuple->Integral());
      finvM_corr->Scale(1./Nntuple->Integral());

      finvMGEN->Scale(1./Nntuple->Integral());
      finvMGEN_corr->Scale(1./Nntuple->Integral()); 

      fAbsRap->Scale(1./Nntuple->Integral());
      fAbsRap_PU->Scale(1./Nntuple->Integral());
      fAbsRap_corr->Scale(1./Nntuple->Integral());
      fAbsRapGEN->Scale(1./Nntuple->Integral());
      fAbsRapGEN_corr->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   std::cout << "NAME = " << z << std::endl;
   //FIXME must have this dir first
   TFile fout("./"+z+".root","recreate");
   Nntuple->Write();
   finvM->Write();
   finvM_PU->Write();
   finvM_corr->Write();

   finvMGEN->Write();
   finvMGEN_corr->Write();

   fAbsRap->Write();
   fAbsRap_PU->Write();
   fAbsRap_corr->Write();
   fAbsRapGEN->Write();
   fAbsRapGEN_corr->Write();

   fout.Close();
}

double EventSelector::Rapidity( double px1, double py1, double pz1, double px2, double py2, double pz2 ) {

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
