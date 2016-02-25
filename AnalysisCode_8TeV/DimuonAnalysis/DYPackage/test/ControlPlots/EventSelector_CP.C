#define EventSelector_CP_cxx
// The class definition in EventSelector_CP.h has been generated automatically
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
// Root > T->Process("EventSelector_CP.C")
// Root > T->Process("EventSelector_CP.C","some options")
// Root > T->Process("EventSelector_CP.C+")
//

#include "EventSelector_CP.h"
#include "efficiencyWeightToBin2012.C"
#include "MomScaleCorrection.C"
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

EventSelector_CP::EventSelector_CP(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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
  Sigma_DY20   = 1871.0;
  Sigma_DY200  = 1.485;
  Sigma_DY400  = 0.1086;
  Sigma_DY500  = 0.04415;
  Sigma_DY700  = 0.01024;
  Sigma_DY800  = 0.005491;
  Sigma_DY1000 = 0.001796;
  Sigma_DY1500 = 1.705E-4;
  Sigma_DY2000 = 2.208E-5;

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
  FilterEff_DY1500 = 1.;
  FilterEff_DY2000 = 1.;

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

EventSelector_CP::~EventSelector_CP() { }


void EventSelector_CP::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   TString option = GetOption();

}

void EventSelector_CP::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
   TString option = GetOption();

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   this_histogramThis = dynamic_cast<TNamed *>(fInput->FindObject("histogramThis"));
   histogramThis = this_histogramThis ? this_histogramThis->GetTitle() : "";

   this_momCorrType = dynamic_cast<TNamed *>(fInput->FindObject("momCorrType"));
   momCorrType = this_momCorrType ? this_momCorrType->GetTitle() : "";

   //rochcorr
   rmcor = new rochcor2012(); //333);

   const int nbin = 41;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

   const int nbin_full = 42;
   double mass_xbin_full[nbin_full+1] = {0,15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500,10000};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   fPU_weights = new TH1D("PU_weights","PU_weights", 100, 0, 2);

   finvM_full = new TH1D("invm_full", "invm_full", nbin_full, mass_xbin_full);
   finvM = new TH1D("invm", "invm", nbin, mass_xbin);
   finvM_Roch = new TH1D("invm_Roch", "invm_Roch", nbin, mass_xbin);
   finvM_PU = new TH1D("invm_PU", "invm_PU", nbin, mass_xbin);
   finvM_corr = new TH1D("invm_corr", "invm_corr", nbin, mass_xbin);
   finvM_fine_bb = new TH1D("invm_fine_bb", "invm_fine_bb", 120,60,120);
   finvM_fineRoch_bb = new TH1D("invm_fineRoch_bb", "invm_fineRoch_bb", 120,60,120);
   finvM_fine_ee = new TH1D("invm_fine_ee", "invm_fine_ee", 120,60,120);
   finvM_fineRoch_ee = new TH1D("invm_fineRoch_ee", "invm_fineRoch_ee", 120,60,120);
   finvM_fine_eb = new TH1D("invm_fine_eb", "invm_fine_eb", 120,60,120);
   finvM_fineRoch_eb = new TH1D("invm_fineRoch_eb", "invm_fineRoch_eb", 120,60,120);
   finvM_fineAll_bb = new TH1D("invm_fineAll_bb", "invm_fineAll_bb", 120,60,120);
   finvM_fineAll_ee = new TH1D("invm_fineAll_ee", "invm_fineAll_ee", 120,60,120);
   finvM_fineAll_eb = new TH1D("invm_fineAll_eb", "invm_fineAll_eb", 120,60,120);

   //Rapidity deps
   finvM_Zfine_bb = new TH1D("invm_Zfine_bb", "invm_Zfine_bb", 120,60,120);
   finvM_ZfineRoch_bb = new TH1D("invm_ZfineRoch_bb", "invm_ZfineRoch_bb", 120,60,120);
   finvM_Zfine_ee = new TH1D("invm_Zfine_ee", "invm_Zfine_ee", 120,60,120);
   finvM_ZfineRoch_ee = new TH1D("invm_ZfineRoch_ee", "invm_ZfineRoch_ee", 120,60,120);

   finvMGEN = new TH1D("invm_GEN", "invm_GEN", nbin, mass_xbin);
   finvMGEN_corr = new TH1D("invm_GEN_corr", "invm_GEN_corr", nbin, mass_xbin);

   finvMGENF = new TH1D("invm_GENF", "invm_GENF", 742, 15, 2000);

   fPT = new TH1D("PT", "PT", 100, 0.,500.);
   fpt = new TH1D("pt", "pt", 100, 0.,500.);
   feta = new TH1D("eta", "eta", 50, -2.5,2.5);
   fangle3D = new TH1D("angle3D", "angle3D", 100, 0.,1.);
   fchi2dof = new TH1D("chi2dof","chi2dof",100,0,20);
   ftrackerHits = new TH1D("trackerHits","trackerHits",30,0,30);
   fpixelHits= new TH1D("pixelHits","pixelHits",10,0,10);
   fmuonHits= new TH1D("muonHits","muonHits",60,0,60);
   fnTrackerLayers= new TH1D("nTrackerLayers","nTrackerLayers",10,0,10);
   fdxyBS= new TH1D("dxyBS","dxyBS",100,-1.,1.);
   fvtxTrkProb= new TH1D("vtxTrkProb","vtxTrkProb",100,0,1);
   ftrigMatches= new TH1D("trigMatches","trigMatches",5,0,5);

   fRelCombIso = new TH1D("RelCombIso","RelCombIso",500,0,5);
   fRelCombIsoNoEGamma = new TH1D("RelCombIsoNoEGamma","RelCombIsoNoEGamma",500,0,5);
   fRelCombIso_Mass = new TH2D("RelCombIso_Mass","RelCombIso_Mass",nbin, mass_xbin,500,0,5);
   fRelCombIsoNoEGamma_Mass = new TH2D("RelCombIsoNoEGamma_Mass","RelCombIsoNoEGamma_Mass",nbin, mass_xbin,500,0,5);

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

   //underflow
   fAbsRap24 = new TH1D("rap24", "rap24", 24,0,24);
   fAbsRap24_corr = new TH1D("rap24_corr", "rap24_corr",24,0,24);
   fAbsRap24_PU = new TH1D("rap24_PU", "rap24_PU",24,0,24);
   fAbsRap24GEN = new TH1D("rap24_GEN", "rap24_GEN",24,0,24);
   fAbsRap24GEN_corr = new TH1D("rap24_GEN_corr", "rap24_GEN_corr",24,0,24);

   //overflow
   fAbsRap12 = new TH1D("rap12", "rap12", 12,0,12);
   fAbsRap12_corr = new TH1D("rap12_corr", "rap12_corr",12,0,12);
   fAbsRap12_PU = new TH1D("rap12_PU", "rap12_PU",12,0,12);
   fAbsRap12GEN = new TH1D("rap12_GEN", "rap12_GEN",12,0,12);
   fAbsRap12GEN_corr = new TH1D("rap12_GEN_corr", "rap12_GEN_corr",12,0,12);


   //POWHEG cross section map;
   double Ybins[8] = {0.,0.2,0.4,0.7,1.1,1.9,2.4,1000.0};
   double PTbins[21] = {0.0,20.0,30.,35.,40.,45.,50.,55.,60.,65.,70.,75.,80.,85.,90.,100.,120.,150.,300.,600.,1000.};

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(fPU_weights);

   GetOutputList()->Add(finvM_full);
   GetOutputList()->Add(finvM);
   GetOutputList()->Add(finvM_Roch);
   GetOutputList()->Add(finvM_PU);
   GetOutputList()->Add(finvM_corr);
   GetOutputList()->Add(finvM_fine_ee);
   GetOutputList()->Add(finvM_fineRoch_ee);
   GetOutputList()->Add(finvM_fine_bb);
   GetOutputList()->Add(finvM_fineRoch_bb);
   GetOutputList()->Add(finvM_fine_eb);
   GetOutputList()->Add(finvM_fineRoch_eb);
   GetOutputList()->Add(finvM_fineAll_ee);
   GetOutputList()->Add(finvM_fineAll_bb);
   GetOutputList()->Add(finvM_fineAll_eb);
   //Z rapidity
   GetOutputList()->Add(finvM_Zfine_ee);
   GetOutputList()->Add(finvM_ZfineRoch_ee);
   GetOutputList()->Add(finvM_Zfine_bb);
   GetOutputList()->Add(finvM_ZfineRoch_bb);

   GetOutputList()->Add(finvMGEN);
   GetOutputList()->Add(finvMGENF);
   GetOutputList()->Add(finvMGEN_corr);

   GetOutputList()->Add(fPT);

   GetOutputList()->Add(fpt);
   GetOutputList()->Add(feta);
   GetOutputList()->Add(fangle3D);
   GetOutputList()->Add(fchi2dof);
   GetOutputList()->Add(ftrackerHits);
   GetOutputList()->Add(fpixelHits);
   GetOutputList()->Add(fmuonHits);
   GetOutputList()->Add(fnTrackerLayers);
   GetOutputList()->Add(fdxyBS);
   GetOutputList()->Add(fvtxTrkProb);
   GetOutputList()->Add(ftrigMatches);

   GetOutputList()->Add(fRelCombIso);
   GetOutputList()->Add(fRelCombIsoNoEGamma);
   GetOutputList()->Add(fRelCombIso_Mass);
   GetOutputList()->Add(fRelCombIsoNoEGamma_Mass);

   GetOutputList()->Add(fAbsRap);
   GetOutputList()->Add(fAbsRap_PU);
   GetOutputList()->Add(fAbsRap_corr);
   GetOutputList()->Add(fAbsRapGEN);
   GetOutputList()->Add(fAbsRapGEN_corr);

   GetOutputList()->Add(fAbsRap24);
   GetOutputList()->Add(fAbsRap24_PU);
   GetOutputList()->Add(fAbsRap24_corr);
   GetOutputList()->Add(fAbsRap24GEN);
   GetOutputList()->Add(fAbsRap24GEN_corr);

   GetOutputList()->Add(fAbsRap12);
   GetOutputList()->Add(fAbsRap12_PU);
   GetOutputList()->Add(fAbsRap12_corr);
   GetOutputList()->Add(fAbsRap12GEN);
   GetOutputList()->Add(fAbsRap12GEN_corr);


   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_CP::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   //FIXME do this before we find a better option
   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

    //pick up the correct correction map, there is data & MC dependence of the maps!
    //FIXME deprecated, currently dummy
    _runopt = 0;
    //if( momCorrType == "RunD")  _runopt = 1;

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_CP::GetEntry() or TBranch::GetEntry()
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

   //FIXME hardcode number of trigger paths. 
   for (Int_t trig = 0; trig < 1; trig++) {
    //reset weights
     Double_t WEIGHT = 1.;
     Double_t FEWZ_WEIGHT = 1.;
     Double_t PU_WEIGHT = 1.;


     //split data mc
     if (dataset.Contains("Data") || dataset.Contains("DATA")) {
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
           } else if (filename.Contains("DYM1500")) {
              WEIGHT = Sigma_DY1500*FilterEff_DY1500;
           } else if (filename.Contains("DYM2000")) {
              WEIGHT = Sigma_DY2000*FilterEff_DY2000;
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
      //scale up if normalizing to lumi

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
          //if( (filename.Contains("DYM20") && !filename.Contains("DYM200")) && GENMuon1_mother[j] != 23 ) continue;
          //if( (filename.Contains("DYM20") && !filename.Contains("DYM200")) && GENMuon2_mother[j] != 23 ) continue;
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

       //special histogram
       finvMGEN->Fill(genMass, WEIGHT); 
       finvMGENF->Fill(genMass, WEIGHT); 
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

        if( genMass > 0 && genMass < 20 ) {
            double bin_size = 0.1;
            for( int k = 0; k < 24; k++ ) {
              if( fabs(GENRapidity[GENIndex]) > k*bin_size && fabs(GENRapidity[GENIndex]) < (k+1)*bin_size ) gen_value = k;
           }
        }
       fAbsRap24GEN->Fill(gen_value,WEIGHT);
       fAbsRap24GEN_corr->Fill(gen_value,FEWZ_WEIGHT*WEIGHT);

        if( genMass > 1500 && genMass < 10000 ) {
            double bin_size = 0.2;
            for( int k = 0; k < 12; k++ ) {
              if( fabs(GENRapidity[GENIndex]) > k*bin_size && fabs(GENRapidity[GENIndex]) < (k+1)*bin_size ) gen_value = k;
           }
        }
       fAbsRap12GEN->Fill(gen_value,WEIGHT);
       fAbsRap12GEN_corr->Fill(gen_value,FEWZ_WEIGHT*WEIGHT);

     }//signal MC only

     int index = -1;
     double best_val = -99999;
     int multiplePairsSurvive = 0;
     for( int j = 0; j < nPair; j++ ) {

         if (histogramThis != "pT") {
           if( Muon1_pT[j] < 20 || Muon2_pT[j] < 10 ) {
              if( Muon2_pT[j] < 20 || Muon1_pT[j] < 10 ) continue;
              }
           }

         if (histogramThis != "eta") {
  	   if( fabs(Muon1_eta[j]) > 2.4 ) continue;
	   if( fabs(Muon2_eta[j]) > 2.4 ) continue;
         }

         if (histogramThis != "chi2dof") {
	  // Muon id
	  if( Muon1_chi2dof[j] >= 10 || Muon1_chi2dof[j] < 0 ) continue;
	  if( Muon2_chi2dof[j] >= 10 || Muon2_chi2dof[j] < 0 ) continue;
         }
         if (histogramThis != "trackerHits") {
	  if( Muon1_trackerHits[j] <= 10 ) continue;
	  if( Muon2_trackerHits[j] <= 10 ) continue;
         }
         if (histogramThis != "pixelHits") {
          if( Muon1_pixelHits[j] < 1 ) continue;
	  if( Muon2_pixelHits[j] < 1 ) continue;
         }
         if (histogramThis != "muonHits") {
	  if( Muon1_muonHits[j] < 1 ) continue;
	  if( Muon2_muonHits[j] < 1 ) continue;
         }
         if (histogramThis != "nMatches") {
	  if( Muon1_nMatches[j] < 2 ) continue;
	  if( Muon2_nMatches[j] < 2 ) continue;
         }
         if (histogramThis != "nTrackerLayers") {
          if (Muon1_nTrackerLayers[j] < 6) continue;
          if (Muon2_nTrackerLayers[j] < 6) continue;
         }
         if (histogramThis != "dxyBS") {
	  if( fabs(Muon1_dxyBS[j]) > 0.2 ) continue;
	  if( fabs(Muon2_dxyBS[j]) > 0.2 ) continue;
	 }    
	  // isolation
         if (histogramThis != "relPFisoNoEGamma") {
          //if( (Muon1_PfGammaIsoR03[j]+Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.15) continue;
          //if( (Muon2_PfGammaIsoR03[j]+Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.15) continue;
          if( (Muon1_PfChargedHadronIsoR03[j]+Muon1_PfNeutralHadronIsoR03[j])/Muon1_pT[j] > 0.2) continue;
          if( (Muon2_PfChargedHadronIsoR03[j]+Muon2_PfNeutralHadronIsoR03[j])/Muon2_pT[j] > 0.2) continue;
         }

	  // 3D angle
         if (histogramThis != "CosAngle") {
	  if( CosAngle[j] < 0.005 ) continue;
         }
	  // vtx prob
         if (histogramThis != "vtxTrkProb") {
	  if( vtxTrkProb[j] < 0.02 ) continue;
         }

          if( !isOppSign[j]) continue;

          multiplePairsSurvive++;

	  if( vtxTrkProb[j] > best_val ) {
	      best_val = vtxTrkProb[j];
              index = j;
            }
        }
	if( index == -1 ) continue;

        bool isTriggered = false; 
        //for( int k = 0; k < hlt_ntrig; k++ ) {
        //     if( (hlt_trigName->at((unsigned int)k)) == "HLT_Mu17_TkMu8_v*") {
        //        if( hlt_trigFired[k] == 1 ) {
        //          isTriggered = true;
        //          break;
        //       }
        //     }
        // }
        // if( !isTriggered ) return kTRUE;

         //trigger matching begin
         int isMatched = 0;
/*
         //double trigType[2] = {-1};
         //double trigPt[2] = {-1};
         double trigEta[2] = {-999};
         double trigPhi[2] = {-999};
         int _ntrig = 0;
         for( int k = 0; k < hlt_ntrig; k++ ) {
           bool isFired = false;
           //if( (hlt_trigName->at((unsigned int)k)) == "HLT_Mu17_TkMu8_v*") {
           //    if( hlt_trigFired[k] == 1 ) {
           //        isFired = true;
           //   }
           //}
           //if( !isFired ) continue;

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
        if (histogramThis != "ntrigMatched") {
          if( isMatched < 2) continue;
        }
*/
        if ( InvMass[index] < 10) continue;

        // set for momentum correction
        // Only for RECO, do nothing for GEN
        TLorentzVector muMinus;
        TLorentzVector muPlus;
        //Use one for systematics       
        float dummy = 0;
        MomScaleCorrection(rmcor, muMinus, muPlus,
                Muon1_Px[index], Muon1_Py[index], Muon1_Pz[index], Muon1_charge[index],
                Muon2_Px[index], Muon2_Py[index], Muon2_Pz[index], Muon2_charge[index],
                dummy, _runopt, !isMC);
              
        TLorentzVector recoDYcand = muPlus + muMinus;
        
        double recoMass = InvMass[index];
        double recoRap = Rapidity( Muon1_Px[index], Muon1_Py[index], Muon1_Pz[index],Muon2_Px[index], Muon2_Py[index], Muon2_Pz[index]);
        double recoMass_corr = recoDYcand.M();
        double recoRap_corr = fabs(recoDYcand.Rapidity());

        if (histogramThis == "pT") {
          fpt->Fill(Muon1_pT[index], WEIGHT*FEWZ_WEIGHT);
          fpt->Fill(Muon2_pT[index], WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "eta") {
          feta->Fill(Muon1_eta[index], WEIGHT*FEWZ_WEIGHT);
          feta->Fill(Muon2_eta[index], WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "CosAngle") {
          fangle3D->Fill(CosAngle[index], WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "vtxTrkProb") {
          fvtxTrkProb->Fill(vtxTrkProb[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "chi2dof") {
	  fchi2dof->Fill(Muon1_chi2dof[index],WEIGHT*FEWZ_WEIGHT);
          fchi2dof->Fill(Muon2_chi2dof[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "trackerHits") {
	  ftrackerHits->Fill(Muon1_trackerHits[index],WEIGHT*FEWZ_WEIGHT);
	  ftrackerHits->Fill(Muon2_trackerHits[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "pixelHits") {
	  fpixelHits->Fill(Muon1_pixelHits[index],WEIGHT*FEWZ_WEIGHT);
	  fpixelHits->Fill(Muon2_pixelHits[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "muonHits") {
	  fmuonHits->Fill(Muon1_muonHits[index],WEIGHT*FEWZ_WEIGHT);
	  fmuonHits->Fill(Muon2_muonHits[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "nTrackerLayers") {
	  fnTrackerLayers->Fill(Muon2_nTrackerLayers[index],WEIGHT*FEWZ_WEIGHT);
	  fnTrackerLayers->Fill(Muon1_nTrackerLayers[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "dxyBS") {
	  fdxyBS->Fill(Muon1_dxyBS[index],WEIGHT*FEWZ_WEIGHT);
	  fdxyBS->Fill(Muon2_dxyBS[index],WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "relPFisoNoEGamma") {
          fRelCombIso->Fill((Muon1_PfChargedHadronIsoR03[index]+Muon1_PfNeutralHadronIsoR03[index]+Muon1_PfGammaIsoR03[index])/Muon1_pT[index],WEIGHT);
          fRelCombIsoNoEGamma->Fill((Muon1_PfChargedHadronIsoR03[index]+Muon1_PfNeutralHadronIsoR03[index])/Muon1_pT[index],WEIGHT);
          fRelCombIso->Fill((Muon2_PfChargedHadronIsoR03[index]+Muon2_PfNeutralHadronIsoR03[index]+Muon2_PfGammaIsoR03[index])/Muon2_pT[index],WEIGHT);
          fRelCombIsoNoEGamma->Fill((Muon2_PfChargedHadronIsoR03[index]+Muon2_PfNeutralHadronIsoR03[index])/Muon2_pT[index],WEIGHT);

          fRelCombIso_Mass->Fill(recoMass_corr,(Muon1_PfChargedHadronIsoR03[index]+Muon1_PfNeutralHadronIsoR03[index]+Muon1_PfGammaIsoR03[index])/Muon1_pT[index],WEIGHT);
          fRelCombIsoNoEGamma_Mass->Fill(recoMass_corr,(Muon1_PfChargedHadronIsoR03[index]+Muon1_PfNeutralHadronIsoR03[index])/Muon1_pT[index],WEIGHT);
          fRelCombIso_Mass->Fill(recoMass_corr,(Muon2_PfChargedHadronIsoR03[index]+Muon2_PfNeutralHadronIsoR03[index]+Muon2_PfGammaIsoR03[index])/Muon2_pT[index],WEIGHT);
          fRelCombIsoNoEGamma_Mass->Fill(recoMass_corr,(Muon2_PfChargedHadronIsoR03[index]+Muon2_PfNeutralHadronIsoR03[index])/Muon2_pT[index],WEIGHT);
        } else if (histogramThis == "trigMatches") {
          ftrigMatches->Fill(isMatched,WEIGHT*FEWZ_WEIGHT);
        } else if (histogramThis == "invm") { 

          if (recoMass_corr >= 15 && recoMass_corr <= 2000) fPU_weights->Fill(PU_WEIGHT,WEIGHT);

          fPT->Fill(sqrt(pow(Muon1_Px[index]+Muon2_Px[index],2)+pow(Muon1_Py[index]+Muon2_Py[index],2)),WEIGHT*FEWZ_WEIGHT); 
          finvM_full->Fill(recoMass, WEIGHT);
          finvM->Fill(recoMass, WEIGHT);
          finvM_Roch->Fill(recoMass_corr, WEIGHT);
          finvM_PU->Fill(recoMass_corr, WEIGHT*PU_WEIGHT);
          finvM_corr->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          if ( fabs(Muon1_eta[index]) < 0.8 && fabs(Muon2_eta[index]) < 0.8) {
             finvM_fine_bb->Fill(recoMass, WEIGHT);
             finvM_fineRoch_bb->Fill(recoMass_corr, WEIGHT);
             finvM_fineAll_bb->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          } else if (fabs(Muon1_eta[index]) >= 0.8 && fabs(Muon2_eta[index]) >=  0.8) {
             finvM_fine_ee->Fill(recoMass, WEIGHT);
             finvM_fineRoch_ee->Fill(recoMass_corr, WEIGHT);
             finvM_fineAll_ee->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          } else {
             finvM_fine_eb->Fill(recoMass, WEIGHT);
             finvM_fineRoch_eb->Fill(recoMass_corr, WEIGHT);
             finvM_fineAll_eb->Fill(recoMass_corr, FEWZ_WEIGHT*PU_WEIGHT*WEIGHT);
          }
          if (fabs(recoRap) < 1.2) { 
             finvM_Zfine_bb->Fill(recoMass, WEIGHT);
             finvM_ZfineRoch_bb->Fill(recoMass_corr, WEIGHT);
          } else {
             finvM_Zfine_ee->Fill(recoMass, WEIGHT);
             finvM_ZfineRoch_ee->Fill(recoMass_corr, WEIGHT);
          }

        //2D sliced 
        double value = -1;

        for( int j = 0; j < 6; j++ ) {
          // preFSR
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
              if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) value = k + j*24;
            }
          }
        }
        fAbsRap->Fill(value, WEIGHT);
        fAbsRap_PU->Fill(value, PU_WEIGHT*WEIGHT);
        fAbsRap_corr->Fill(value, PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);

        if( recoMass_corr > 0 && recoMass_corr < 20 ) {
            double bin_size = 0.1;
            for( int k = 0; k < 24; k++ ) {
              if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) value = k;
           }
          fAbsRap24->Fill(value, WEIGHT);
          fAbsRap24_PU->Fill(value, PU_WEIGHT*WEIGHT);
          fAbsRap24_corr->Fill(value, PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);
        }

        if( recoMass_corr > 1500 && recoMass_corr < 10000 ) {
            double bin_size = 0.2;
            for( int k = 0; k < 12; k++ ) {
              if( fabs(recoRap_corr) > k*bin_size && fabs(recoRap_corr) < (k+1)*bin_size ) value = k;
           }
          fAbsRap12->Fill(value, WEIGHT);
          fAbsRap12_PU->Fill(value, PU_WEIGHT*WEIGHT);
          fAbsRap12_corr->Fill(value, PU_WEIGHT*WEIGHT*FEWZ_WEIGHT);
        } 

      }//end invm 

   }//end split by trig path

   return kTRUE;
}

void EventSelector_CP::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_CP::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   fPU_weights = dynamic_cast<TH1D*>(fOutput->FindObject(Form("PU_weights")));   

   finvM_full = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_full")));
   finvM = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm")));
   finvM_Roch = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_Roch")));
   finvM_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_PU")));
   finvM_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_corr")));
   finvM_fine_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fine_bb")));
   finvM_fineRoch_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineRoch_bb")));
   finvM_fine_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fine_ee")));
   finvM_fineRoch_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineRoch_ee")));
   finvM_fine_eb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fine_eb")));
   finvM_fineRoch_eb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineRoch_eb")));
   finvM_fineAll_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineAll_bb")));
   finvM_fineAll_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineAll_ee")));
   finvM_fineAll_eb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_fineAll_eb")));

   finvM_Zfine_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_Zfine_bb")));
   finvM_ZfineRoch_bb = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_ZfineRoch_bb")));
   finvM_Zfine_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_Zfine_ee")));
   finvM_ZfineRoch_ee = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_ZfineRoch_ee")));

   fPT = dynamic_cast<TH1D*>(fOutput->FindObject(Form("PT")));

   finvMGEN = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_GEN")));
   finvMGENF = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_GENF")));
   finvMGEN_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("invm_GEN_corr")));
   fpt = dynamic_cast<TH1D*>(fOutput->FindObject(Form("pt")));
   feta = dynamic_cast<TH1D*>(fOutput->FindObject(Form("eta")));
   fangle3D = dynamic_cast<TH1D*>(fOutput->FindObject(Form("angle3D")));
   fchi2dof= dynamic_cast<TH1D*>(fOutput->FindObject(Form("chi2dof")));
   ftrackerHits= dynamic_cast<TH1D*>(fOutput->FindObject(Form("trackerHits")));
   fpixelHits= dynamic_cast<TH1D*>(fOutput->FindObject(Form("pixelHits")));
   fmuonHits= dynamic_cast<TH1D*>(fOutput->FindObject(Form("muonHits")));
   fnTrackerLayers= dynamic_cast<TH1D*>(fOutput->FindObject(Form("nTrackerLayers")));
   fdxyBS= dynamic_cast<TH1D*>(fOutput->FindObject(Form("dxyBS")));
   fRelCombIso = dynamic_cast<TH1D*>(fOutput->FindObject(Form("RelCombIso")));
   fRelCombIsoNoEGamma = dynamic_cast<TH1D*>(fOutput->FindObject(Form("RelCombIsoNoEGamma")));
   fRelCombIso_Mass = dynamic_cast<TH2D*>(fOutput->FindObject(Form("RelCombIso_Mass")));
   fRelCombIsoNoEGamma_Mass = dynamic_cast<TH2D*>(fOutput->FindObject(Form("RelCombIsoNoEGamma_Mass")));
   fvtxTrkProb= dynamic_cast<TH1D*>(fOutput->FindObject(Form("vtxTrkProb")));
   ftrigMatches= dynamic_cast<TH1D*>(fOutput->FindObject(Form("trigMatches")));

   fAbsRap = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap")));
   fAbsRap_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_corr")));
   fAbsRap_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_PU")));
   fAbsRapGEN = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_GEN")));
   fAbsRapGEN_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap_GEN_corr")));

   fAbsRap24 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap24")));
   fAbsRap24_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap24_corr")));
   fAbsRap24_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap24_PU")));
   fAbsRap24GEN = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap24_GEN")));
   fAbsRap24GEN_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap24_GEN_corr")));

   fAbsRap12 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap12")));
   fAbsRap12_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap12_corr")));
   fAbsRap12_PU = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap12_PU")));
   fAbsRap12GEN = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap12_GEN")));
   fAbsRap12GEN_corr = dynamic_cast<TH1D*>(fOutput->FindObject(Form("rap12_GEN_corr")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   //normalization independent on the sample size
   if (!z.Contains("DATA") && Nntuple->Integral()) { // != 0 && useNtupleWeightFlag != "True") { 
      fPU_weights->Scale(1./Nntuple->Integral());

      fPT->Scale(1./Nntuple->Integral());

      finvM_full->Scale(1./Nntuple->Integral());
      finvM->Scale(1./Nntuple->Integral());
      finvM_Roch->Scale(1./Nntuple->Integral());
      finvM_PU->Scale(1./Nntuple->Integral());
      finvM_corr->Scale(1./Nntuple->Integral());
      finvM_fineRoch_bb->Scale(1./Nntuple->Integral());
      finvM_fine_bb->Scale(1./Nntuple->Integral());
      finvM_fineRoch_ee->Scale(1./Nntuple->Integral());
      finvM_fine_ee->Scale(1./Nntuple->Integral());
      finvM_fineRoch_eb->Scale(1./Nntuple->Integral());
      finvM_fine_eb->Scale(1./Nntuple->Integral());
      finvM_fineAll_bb->Scale(1./Nntuple->Integral());
      finvM_fineAll_ee->Scale(1./Nntuple->Integral());
      finvM_fineAll_eb->Scale(1./Nntuple->Integral());

      finvM_ZfineRoch_bb->Scale(1./Nntuple->Integral());
      finvM_Zfine_bb->Scale(1./Nntuple->Integral());
      finvM_ZfineRoch_ee->Scale(1./Nntuple->Integral());
      finvM_Zfine_ee->Scale(1./Nntuple->Integral());

      finvMGEN->Scale(1./Nntuple->Integral());
      finvMGENF->Scale(1./Nntuple->Integral());
      finvMGEN_corr->Scale(1./Nntuple->Integral()); 
      fpt->Scale(1./Nntuple->Integral());
      feta->Scale(1./Nntuple->Integral());
      fangle3D->Scale(1./Nntuple->Integral());
      fchi2dof->Scale(1./Nntuple->Integral());
      ftrackerHits->Scale(1./Nntuple->Integral());
      fpixelHits->Scale(1./Nntuple->Integral());
      fmuonHits->Scale(1./Nntuple->Integral());
      fnTrackerLayers->Scale(1./Nntuple->Integral());
      fdxyBS->Scale(1./Nntuple->Integral());
      fRelCombIso->Scale(1./Nntuple->Integral());
      fRelCombIsoNoEGamma->Scale(1./Nntuple->Integral());
      fRelCombIso_Mass->Scale(1./Nntuple->Integral());
      fRelCombIsoNoEGamma_Mass->Scale(1./Nntuple->Integral());
      fvtxTrkProb->Scale(1./Nntuple->Integral());
      ftrigMatches->Scale(1./Nntuple->Integral());

      fAbsRap->Scale(1./Nntuple->Integral());
      fAbsRap_PU->Scale(1./Nntuple->Integral());
      fAbsRap_corr->Scale(1./Nntuple->Integral());
      fAbsRapGEN->Scale(1./Nntuple->Integral());
      fAbsRapGEN_corr->Scale(1./Nntuple->Integral());

      fAbsRap24->Scale(1./Nntuple->Integral());
      fAbsRap24_PU->Scale(1./Nntuple->Integral());
      fAbsRap24_corr->Scale(1./Nntuple->Integral());
      fAbsRap24GEN->Scale(1./Nntuple->Integral());
      fAbsRap24GEN_corr->Scale(1./Nntuple->Integral());

      fAbsRap12->Scale(1./Nntuple->Integral());
      fAbsRap12_PU->Scale(1./Nntuple->Integral());
      fAbsRap12_corr->Scale(1./Nntuple->Integral());
      fAbsRap12GEN->Scale(1./Nntuple->Integral());
      fAbsRap12GEN_corr->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   this_histogramThis = dynamic_cast<TNamed *>(fInput->FindObject("histogramThis"));
   histogramThis = this_histogramThis ? this_histogramThis->GetTitle() : "";

   this_momCorrType = dynamic_cast<TNamed *>(fInput->FindObject("momCorrType"));
   momCorrType = this_momCorrType ? this_momCorrType->GetTitle() : "";

   std::cout << "NAME = " << z << std::endl;
   TFile fout("./runfolder/"+z+"_"+histogramThis+"_"+TString(momCorrType)+".root","recreate");
   Nntuple->Write();
   fPU_weights->Write();
   finvM_full->Write();
   finvM->Write();
   finvM_Roch->Write();
   finvM_PU->Write();
   finvM_corr->Write();
   finvM_fineRoch_bb->Write();
   finvM_fine_bb->Write();
   finvM_fineRoch_ee->Write();
   finvM_fine_ee->Write();
   finvM_fineRoch_eb->Write();
   finvM_fine_eb->Write();
   finvM_fineAll_bb->Write();
   finvM_fineAll_ee->Write();
   finvM_fineAll_eb->Write();

   finvM_ZfineRoch_bb->Write();
   finvM_Zfine_bb->Write();
   finvM_ZfineRoch_ee->Write();
   finvM_Zfine_ee->Write();

   finvMGEN->Write();
   finvMGENF->Write();
   finvMGEN_corr->Write();
   fPT->Write();

   fpt->Write();
   feta->Write();
   fangle3D->Write();
   fchi2dof->Write();
   ftrackerHits->Write();
   fpixelHits->Write();
   fmuonHits->Write();
   fnTrackerLayers->Write();
   fdxyBS->Write();
   fRelCombIso->Write();
   fRelCombIsoNoEGamma->Write();
   fRelCombIso_Mass->Write();
   fRelCombIsoNoEGamma_Mass->Write();
   fvtxTrkProb->Write();
   ftrigMatches->Write();

   fAbsRap->Write();
   fAbsRap_PU->Write();
   fAbsRap_corr->Write();
   fAbsRapGEN->Write();
   fAbsRapGEN_corr->Write();

   fAbsRap24->Write();
   fAbsRap24_PU->Write();
   fAbsRap24_corr->Write();
   fAbsRap24GEN->Write();
   fAbsRap24GEN_corr->Write(); 

   fAbsRap12->Write();
   fAbsRap12_PU->Write();
   fAbsRap12_corr->Write();
   fAbsRap12GEN->Write();
   fAbsRap12GEN_corr->Write(); 

   fout.Close();
}

double EventSelector_CP::Rapidity( double px1, double py1, double pz1, double px2, double py2, double pz2 ) {

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
