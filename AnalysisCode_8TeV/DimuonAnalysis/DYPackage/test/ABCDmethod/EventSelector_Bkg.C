#define EventSelector_Bkg_cxx
// The class definition in EventSelector_Bkg.h has been generated automatically
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
// Root > T->Process("EventSelector_Bkg.C")
// Root > T->Process("EventSelector_Bkg.C","some options")
// Root > T->Process("EventSelector_Bkg.C+")
//

#include "EventSelector_Bkg.h"
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

EventSelector_Bkg::EventSelector_Bkg(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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
  Sigma_tt700  = 18.19; //NLO 211.0;
  Sigma_tt1000 = 3.44; //NLO 211.0;
  Sigma_ttjets = 23.85;
  Sigma_tW     = 11.17;
  Sigma_tbarW  = 11.17;
  Sigma_WJets  = 37509.0; //NLO 30400.0; 
  Sigma_DYtautau1020 = 3708.0; //promote to NNLO? 3795.4 
  Sigma_DYtautau20   = 1871.0; //promote to NNLO? 1915.1
  Sigma_QCD15to20    = 7.022E8;
  Sigma_QCD20to30    = 2.87E8;
  Sigma_QCD30to50    = 6.6285328E7;
  Sigma_QCD50to80    = 8082000.0;
  Sigma_QCD80to120   = 1033680.0;
  Sigma_QCD120to170  = 156293.3;
  Sigma_QCD170to300  = 34020.0;
  Sigma_WZJetsTo2L2Q =  2.31; //1.755;
  Sigma_WZJetsTo3LNu   = 1.11; //0.8674; 
  Sigma_ZZJetsTo2L2Nu  =  0.364791; //0.28;
  Sigma_ZZJetsTo2L2Q   =  2.448690; //0.91;
  Sigma_ZZJetsTo4L     =  0.176908; //0.1296;
  Sigma_WWJetsTo2L2Nu  =  5.812; //4.7;

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
  FilterEff_WZJetsTo2L2Q      = 1.0; //1.755
  FilterEff_WZJetsTo3LNu      = 1.0; //0.8674 
  FilterEff_ZZJetsTo2L2Nu     = 1.0;  //0.28
  FilterEff_ZZJetsTo2L2Q      = 1.0;  //0.91
  FilterEff_ZZJetsTo4L        = 1.0;  //0.1296 
  FilterEff_WWJetsTo2L2Nu     = 1.0;  //4.7

}

EventSelector_Bkg::~EventSelector_Bkg() { }


void EventSelector_Bkg::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_Bkg::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   this_iso = dynamic_cast<TNamed *>(fInput->FindObject("iso"));
   iso = this_iso ? this_iso->GetTitle() : "";

   this_region = dynamic_cast<TNamed *>(fInput->FindObject("region"));
   region = this_region ? this_region->GetTitle() : "";

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple     = new TH1D("norm", "norm", 1,0,1);
   fIsoRegMass = new TH1D("template", "template", nbin, mass_xbin);

   GetOutputList()->Add(Nntuple);
   GetOutputList()->Add(fIsoRegMass);

   //2D set up
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 132 bins
   fIsoRegMass2030 = new TH1D("template2030", "template2030", 24, 0,2.4);
   fIsoRegMass3045 = new TH1D("template3045", "template3045", 24, 0,2.4);
   fIsoRegMass4560 = new TH1D("template4560", "template4560", 24, 0,2.4);
   fIsoRegMass60120 = new TH1D("template60120", "template60120", 24, 0,2.4);
   fIsoRegMass120200 = new TH1D("template120200", "template120200", 24, 0,2.4);
   fIsoRegMass2001500 = new TH1D("template2001500", "template2001500", 12, 0,2.4);


   GetOutputList()->Add(Nntuple);
   GetOutputList()->Add(fIsoRegMass);
   GetOutputList()->Add(fIsoRegMass2030);
   GetOutputList()->Add(fIsoRegMass3045);
   GetOutputList()->Add(fIsoRegMass4560);
   GetOutputList()->Add(fIsoRegMass60120);
   GetOutputList()->Add(fIsoRegMass120200);
   GetOutputList()->Add(fIsoRegMass2001500);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_Bkg::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_Bkg::GetEntry() or TBranch::GetEntry()
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
        if ((filename.Contains("DYM200") || filename.Contains("DYE200")) && genMass > 400) { continue; }
         else if (((filename.Contains("DYM20") && !filename.Contains("DYM200")) || (filename.Contains("DYE20") && !filename.Contains("DYE200"))) && genMass > 200) {continue;}
         else if ((filename.Contains("DYM400") || filename.Contains("DYE400")) && genMass > 500) { continue; }
         else if ((filename.Contains("DYM500") || filename.Contains("DYE500")) && genMass > 700) { continue; }
         else if ((filename.Contains("DYM700") || filename.Contains("DYE700")) && genMass > 800) { continue; }
         else if ((filename.Contains("DYM800") || filename.Contains("DYE800")) && genMass > 1000) { continue; }
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
	    
	  // NO isolation cut here
	  //if( (Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.2) continue;
	  //if( (Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.2) continue;

	  // 3D angle
	  if( CosAngle[j] < 0.005 ) continue;

          // vtx prob
          if( vtxTrkProb[j] < 0.02 ) continue;

          //if( isOppSign[j] == 0 ) continue;

            if( vtxTrkProb[j] > best_val ) {
              best_val = vtxTrkProb[j];
              index = j;
            }
        }
	if( index == -1 ) continue;

        bool isTriggered = false;
        //2011 way
        //for( int k = 0; k < hlt_ntrig; k++ ) {
        //    //if( (hlt_trigName->at((unsigned int)k)) == "HLT_Mu17_Mu8_v*" ) { 
        //    if (k==1) {
        //      if( hlt_trigFired[k] == 1 ) {
        //        isTriggered = true;
        //        break;
        //    }
        //  } 
        //}
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

        //access isolation variables for the cut
	double muon1_iso =  (Muon1_PfChargedHadronIsoR03[index]+Muon1_PfNeutralHadronIsoR03[index])/Muon1_pT[index];
	double muon2_iso =  (Muon2_PfChargedHadronIsoR03[index]+Muon2_PfNeutralHadronIsoR03[index])/Muon2_pT[index];

	double var1 = -1;
        double ISO_cut = 0.2;
        if( iso == "mu1iso" ) {
	  if( muon2_iso > ISO_cut) continue;
	  var1 = muon1_iso;
	}
        if(iso == "mu2iso" ) {
	  if( muon1_iso > ISO_cut) continue;
	  var1 = muon2_iso;
	}

        //FIXME is this variable being deprecated?
        //0 - is Same Sign, 1 - is Opposite Sign
	bool var2 = isOppSign[index];

        double recoMass_corr = InvMass[index]; //recoDYcand.M();
        double recoRap_corr = Rapidity[index]; //recoDYcand.Rapidity();

	if( region == "B" ) {
	  if( var1 < ISO_cut && !var2) {

            fIsoRegMass->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);

            fIsoRegMass2030->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass3045->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass4560->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass60120->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass120200->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass2001500->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
          } 
	}
	if( region == "C" ) {
	  if( var1 > ISO_cut && var2) {
            fIsoRegMass->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);

            fIsoRegMass2030->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass3045->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass4560->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass60120->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass120200->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass2001500->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
          }
	}
	if( region == "D" ) {
	  if( var1 > ISO_cut && !var2) {
            fIsoRegMass->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);

            fIsoRegMass2030->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass3045->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass4560->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass60120->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass120200->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
            fIsoRegMass2001500->Fill(recoRap_corr, WEIGHT*PU_WEIGHT);
          }
	}

   }//end split by trig path

   return kTRUE;
}

void EventSelector_Bkg::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_Bkg::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   this_iso = dynamic_cast<TNamed *>(fInput->FindObject("iso"));
   iso = this_iso ? this_iso->GetTitle() : "";

   this_region = dynamic_cast<TNamed *>(fInput->FindObject("region"));
   region = this_region ? this_region->GetTitle() : "";

   fIsoRegMass = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template")));
   Nntuple     = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   fIsoRegMass2030 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template2030")));
   fIsoRegMass3045 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template3045")));
   fIsoRegMass4560 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template4560")));
   fIsoRegMass60120 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template60120")));
   fIsoRegMass120200 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template120200")));
   fIsoRegMass2001500 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("template2001500")));

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   //normalization independent on the sample size
   if (z != "DATA" && Nntuple->Integral() != 0) {
      //FIXME
      fIsoRegMass->Scale(1./Nntuple->Integral()); 
      fIsoRegMass2030->Scale(1./Nntuple->Integral());
      fIsoRegMass3045->Scale(1./Nntuple->Integral());
      fIsoRegMass4560->Scale(1./Nntuple->Integral());
      fIsoRegMass60120->Scale(1./Nntuple->Integral());
      fIsoRegMass120200->Scale(1./Nntuple->Integral());
      fIsoRegMass2001500->Scale(1./Nntuple->Integral());
   } //if samples isn't empty
   
   std::cout << "NAME = " << z << std::endl;
   TFile fout("rootfiles1/"+z+"_"+iso+"_"+region+".root","recreate");
   Nntuple->Write();
   fIsoRegMass->Write();
   fIsoRegMass2030->Write();
   fIsoRegMass3045->Write();
   fIsoRegMass4560->Write();
   fIsoRegMass60120->Write();
   fIsoRegMass120200->Write();
   fIsoRegMass2001500->Write();

   fout.Close();
}
