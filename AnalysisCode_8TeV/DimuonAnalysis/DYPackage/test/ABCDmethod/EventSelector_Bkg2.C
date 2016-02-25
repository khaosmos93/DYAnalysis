#define EventSelector_Bkg2_cxx
// The class definition in EventSelector_Bkg2.h has been generated automatically
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
// Root > T->Process("EventSelector_Bkg2.C")
// Root > T->Process("EventSelector_Bkg2.C","some options")
// Root > T->Process("EventSelector_Bkg2.C+")
//

#include "EventSelector_Bkg2.h"
#include <iostream>
#include <TProof.h>
#include <TProofServ.h>
#include <TDSet.h>
#include <TEventList.h>
#include <TParameter.h>
#include <TMath.h>

double err_calc( float v1, float v2, float v3, float e1, float e2, float e3 )
{
   // if v = v1*v2/v3, type is 0
   if( v3 == 0 ) return 0.;

   //float v = v1*v2/v3;
   float dv1 = (v2/v3);
   float dv2 = (v1/v3);
   float dv3 = -1.0*(v1*v2/v3/v3);
   float dv_2 = dv1*dv1*e1*e1 + dv2*dv2*e2*e2 + dv3*dv3*e3*e3;

   if( dv_2 == 0) dv_2 = 0.0000001;
   return TMath::Sqrt(dv_2);
}

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

EventSelector_Bkg2::EventSelector_Bkg2(TTree* ) : fNumberOfEvents(0), fNumberOfGoodEvents(0), fTotalDataSize(0)
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

EventSelector_Bkg2::~EventSelector_Bkg2() { }


void EventSelector_Bkg2::Begin(TTree*)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void EventSelector_Bkg2::SlaveBegin(TTree*)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";
   this_iso = dynamic_cast<TNamed *>(fInput->FindObject("iso"));
   iso = this_iso ? this_iso->GetTitle() : "";

   this_factorB = dynamic_cast<TNamed *>(fInput->FindObject("factorB"));
   sfactor_B = this_factorB ? this_factorB->GetTitle() : "";
   factor_B = atof(sfactor_B.c_str());
   this_factorBerr = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr"));
   sfactor_Berr = this_factorBerr ? this_factorBerr->GetTitle() : "";
   factor_Berr = atof(sfactor_Berr.c_str());
   //2D needs, temporary setup
   this_factorB2030 = dynamic_cast<TNamed *>(fInput->FindObject("factorB2030"));
   sfactor_B2030 = this_factorB2030 ? this_factorB2030->GetTitle() : "";
   factor_B2030 = atof(sfactor_B2030.c_str());
   this_factorBerr2030 = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr2030"));
   sfactor_Berr2030 = this_factorBerr2030 ? this_factorBerr2030->GetTitle() : "";
   factor_Berr2030 = atof(sfactor_Berr2030.c_str());
   this_factorB3045 = dynamic_cast<TNamed *>(fInput->FindObject("factorB3045"));
   sfactor_B3045 = this_factorB3045 ? this_factorB3045->GetTitle() : "";
   factor_B3045 = atof(sfactor_B3045.c_str());
   this_factorBerr3045 = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr3045"));
   sfactor_Berr3045 = this_factorBerr3045 ? this_factorBerr3045->GetTitle() : "";
   factor_Berr3045 = atof(sfactor_Berr3045.c_str());
   this_factorB4560 = dynamic_cast<TNamed *>(fInput->FindObject("factorB4560"));
   sfactor_B4560 = this_factorB4560 ? this_factorB4560->GetTitle() : "";
   factor_B4560 = atof(sfactor_B4560.c_str());
   this_factorBerr4560 = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr4560"));
   sfactor_Berr4560 = this_factorBerr4560 ? this_factorBerr4560->GetTitle() : "";
   factor_Berr4560 = atof(sfactor_Berr4560.c_str());
   this_factorB60120 = dynamic_cast<TNamed *>(fInput->FindObject("factorB60120"));
   sfactor_B60120 = this_factorB60120 ? this_factorB60120->GetTitle() : "";
   factor_B60120 = atof(sfactor_B60120.c_str());
   this_factorBerr60120 = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr60120"));
   sfactor_Berr60120 = this_factorBerr60120 ? this_factorBerr60120->GetTitle() : "";
   factor_Berr60120 = atof(sfactor_Berr60120.c_str());
   this_factorB120200 = dynamic_cast<TNamed *>(fInput->FindObject("factorB120200"));
   sfactor_B120200 = this_factorB120200 ? this_factorB120200->GetTitle() : "";
   factor_B120200 = atof(sfactor_B120200.c_str());
   this_factorBerr120200 = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr120200"));
   sfactor_Berr120200 = this_factorBerr120200 ? this_factorBerr120200->GetTitle() : "";
   factor_Berr120200 = atof(sfactor_Berr120200.c_str());
   this_factorB2001500 = dynamic_cast<TNamed *>(fInput->FindObject("factorB2001500"));
   sfactor_B2001500 = this_factorB2001500 ? this_factorB2001500->GetTitle() : "";
   factor_B2001500 = atof(sfactor_B2001500.c_str());
   this_factorBerr2001500 = dynamic_cast<TNamed *>(fInput->FindObject("factorBerr2001500"));
   sfactor_Berr2001500 = this_factorBerr2001500 ? this_factorBerr2001500->GetTitle() : "";
   factor_Berr2001500 = atof(sfactor_Berr2001500.c_str());

   /*this_factorC = dynamic_cast<TNamed *>(fInput->FindObject("factorC"));
   sfactor_C = this_factorC ? this_factorC->GetTitle() : "";
   factor_C = atof(sfactor_C.c_str());
   this_factorCerr = dynamic_cast<TNamed *>(fInput->FindObject("factorCerr"));
   sfactor_Cerr = this_factorCerr ? this_factorCerr->GetTitle() : "";
   factor_Cerr = atof(sfactor_Cerr.c_str());
   */
   this_factorD = dynamic_cast<TNamed *>(fInput->FindObject("factorD"));
   sfactor_D = this_factorD ? this_factorD->GetTitle() : "";
   factor_D = atof(sfactor_D.c_str());
   this_factorDerr = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr"));
   sfactor_Derr = this_factorDerr ? this_factorDerr->GetTitle() : "";
   factor_Derr = atof(sfactor_Derr.c_str());

   //2D needs, temporary setup
   this_factorD2030 = dynamic_cast<TNamed *>(fInput->FindObject("factorD2030"));
   sfactor_D2030 = this_factorD2030 ? this_factorD2030->GetTitle() : "";
   factor_D2030 = atof(sfactor_D2030.c_str());
   this_factorDerr2030 = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr2030"));
   sfactor_Derr2030 = this_factorDerr2030 ? this_factorDerr2030->GetTitle() : "";
   factor_Derr2030 = atof(sfactor_Derr2030.c_str());
   this_factorD3045 = dynamic_cast<TNamed *>(fInput->FindObject("factorD3045"));
   sfactor_D3045 = this_factorD3045 ? this_factorD3045->GetTitle() : "";
   factor_D3045 = atof(sfactor_D3045.c_str());
   this_factorDerr3045 = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr3045"));
   sfactor_Derr3045 = this_factorDerr3045 ? this_factorDerr3045->GetTitle() : "";
   factor_Derr3045 = atof(sfactor_Derr3045.c_str());
   this_factorD4560 = dynamic_cast<TNamed *>(fInput->FindObject("factorD4560"));
   sfactor_D4560 = this_factorD4560 ? this_factorD4560->GetTitle() : "";
   factor_D4560 = atof(sfactor_D4560.c_str());
   this_factorDerr4560 = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr4560"));
   sfactor_Derr4560 = this_factorDerr4560 ? this_factorDerr4560->GetTitle() : "";
   factor_Derr4560 = atof(sfactor_Derr4560.c_str());
   this_factorD60120 = dynamic_cast<TNamed *>(fInput->FindObject("factorD60120"));
   sfactor_D60120 = this_factorD60120 ? this_factorD60120->GetTitle() : "";
   factor_D60120 = atof(sfactor_D60120.c_str());
   this_factorDerr60120 = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr60120"));
   sfactor_Derr60120 = this_factorDerr60120 ? this_factorDerr60120->GetTitle() : "";
   factor_Derr60120 = atof(sfactor_Derr60120.c_str());
   this_factorD120200 = dynamic_cast<TNamed *>(fInput->FindObject("factorD120200"));
   sfactor_D120200 = this_factorD120200 ? this_factorD120200->GetTitle() : "";
   factor_D120200 = atof(sfactor_D120200.c_str());
   this_factorDerr120200 = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr120200"));
   sfactor_Derr120200 = this_factorDerr120200 ? this_factorDerr120200->GetTitle() : "";
   factor_Derr120200 = atof(sfactor_Derr120200.c_str());
   this_factorD2001500 = dynamic_cast<TNamed *>(fInput->FindObject("factorD2001500"));
   sfactor_D2001500 = this_factorD2001500 ? this_factorD2001500->GetTitle() : "";
   factor_D2001500 = atof(sfactor_D2001500.c_str());
   this_factorDerr2001500 = dynamic_cast<TNamed *>(fInput->FindObject("factorDerr2001500"));
   sfactor_Derr2001500 = this_factorDerr2001500 ? this_factorDerr2001500->GetTitle() : "";
   factor_Derr2001500 = atof(sfactor_Derr2001500.c_str());

   //SAV FIXME
   TFile* flookup = new TFile("/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ABCDmethod/factor_qcd_"+iso+"_C.root");
   flookup->cd();
   hlookup = (TH1D*)flookup->Get("hfactor");
   hlookup2030 = (TH1D*)flookup->Get("hfactor2030");
   hlookup3045 = (TH1D*)flookup->Get("hfactor3045");
   hlookup4560 = (TH1D*)flookup->Get("hfactor4560");
   hlookup60120 = (TH1D*)flookup->Get("hfactor60120");
   hlookup120200 = (TH1D*)flookup->Get("hfactor120200");
   hlookup2001500 = (TH1D*)flookup->Get("hfactor2001500");


   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   //normalization histogram
   Nntuple = new TH1D("norm", "norm", 1,0,1);

   totA = new TH1D("totA", "totA", nbin, mass_xbin);
   totB = new TH1D("totB", "totB", nbin, mass_xbin);
   totC = new TH1D("totC", "totC", nbin, mass_xbin);
   totD = new TH1D("totD", "totD", nbin, mass_xbin);

   //hmuonISO_OS = new TH1D("hmuonISO_OS", "hmuonISO_OS", nbin, mass_xbin);
   //hmuonISO_SS = new TH1D("hmuonISO_SS", "hmuonISO_SS", nbin, mass_xbin);

   hISOvsVTX = new TH2D("hISOvsVTX", "hISOvsVTX", 100, 0.0, 1.0, 2, 0.0, 2.0);
   hISOvsVTX_A = new TH2D("hISOvsVTX_A", "hISOvsVTX_A", 100, 0.0, 1.0, 2, 0.0, 2.0);
   hISOvsVTX_B = new TH2D("hISOvsVTX_B", "hISOvsVTX_B", 100, 0.0, 1.0, 2, 0.0, 2.0);
   hISOvsVTX_C = new TH2D("hISOvsVTX_C", "hISOvsVTX_C", 100, 0.0, 1.0, 2, 0.0, 2.0);
   hISOvsVTX_D = new TH2D("hISOvsVTX_D", "hISOvsVTX_D", 100, 0.0, 1.0, 2, 0.0, 2.0);

   //2D set up
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 132 bins 
   const int nbin2 = 132;

   totA2 = new TH1D("totA2", "totA2", nbin2, 0, 132);
   totB2 = new TH1D("totB2", "totB2", nbin2, 0, 132);
   totC2 = new TH1D("totC2", "totC2", nbin2, 0, 132);
   totD2 = new TH1D("totD2", "totD2", nbin2, 0, 132);

   GetOutputList()->Add(Nntuple);

   GetOutputList()->Add(totA);
   GetOutputList()->Add(totB);
   GetOutputList()->Add(totC);
   GetOutputList()->Add(totD);

   //GetOutputList()->Add(hmuonISO_OS);
   //GetOutputList()->Add(hmuonISO_SS);

   GetOutputList()->Add(hISOvsVTX);
   GetOutputList()->Add(hISOvsVTX_A);
   GetOutputList()->Add(hISOvsVTX_B);
   GetOutputList()->Add(hISOvsVTX_C);
   GetOutputList()->Add(hISOvsVTX_D);

   GetOutputList()->Add(totA2);
   GetOutputList()->Add(totB2);
   GetOutputList()->Add(totC2);
   GetOutputList()->Add(totD2);

   ds = new TNamed(TString("outfile"),dataset);
   GetOutputList()->Add(ds);

}

Bool_t EventSelector_Bkg2::Process(Long64_t entry)
{

   //Int_t trig = 0;
   bool isMC = true;

   double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector_Bkg2::GetEntry() or TBranch::GetEntry()
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

        double muon1_iso =  (Muon1_PfChargedHadronIsoR03[index]+Muon1_PfNeutralHadronIsoR03[index])/Muon1_pT[index];
        double muon2_iso =  (Muon2_PfChargedHadronIsoR03[index]+Muon2_PfNeutralHadronIsoR03[index])/Muon2_pT[index];
	double var1;
        double ISO_cut = 0.2;
	if( iso == "mu1iso" ) {
	  if( muon2_iso > ISO_cut ) continue;
	  var1 = muon1_iso;
	}
	if( iso == "mu2iso" ) {
	  if( muon1_iso > ISO_cut ) continue;
	  var1 = muon2_iso;
	}

	//double var1 = (muon1_iso+muon2_iso)/2.0;
	//double var2 = vtxTrkProb[index];
	bool var2 = isOppSign[index];
	//double var1 = vtxTrkProb[index];

	//if( sign[index] > 0.5 ) hmuonISO_OS->Fill(var1);
	//if( sign[index] < 0.5 ) hmuonISO_SS->Fill(var1);

        double mass = -1; // InvMass[index];
        double rapidity = -100; //Rapidity[index];
        int pickMassBin = -1; //hlookup->FindBin(mass);
        //per 2D mass bin or per 2D mass-rapidity bin
        double factor_B_2D = 1;
        double factor_C_2D = 1;
        double factor_D_2D = 1; 
        double factor_Berr_2D = 1;
        double factor_Cerr_2D = 1;
        double factor_Derr_2D = 1;
        //FIXME for the test
        //if (dataset == "DATA") { 
          factor_B = 1.0;
          factor_C = 1.0;
          factor_D = 1.0;
        //} else {
        //  factor_B = 0.961207;
        //  if( iso == "mu1iso" ) {
        //     factor_D = 0.923415;
        //  }
        //  if( iso == "mu2iso" ) {
        //     factor_D = 0.986589;
        //  }
        //}
        //end remove after test

        mass = InvMass[index];
        rapidity = fabs(Rapidity[index]);

        //cout << "mass " << mass << " rapidity " << rapidity << endl; 

        if (dataset != "DATA" && mass > 15.) {
          pickMassBin = hlookup->FindBin(mass);

          //cout << "pickMassBin " << pickMassBin << endl;
         
          //so we do not load this one from outside? Filling it from the histo
          factor_C = hlookup->GetBinContent(pickMassBin);
          factor_Cerr = hlookup->GetBinError(pickMassBin);

          //cout << "hlookup->GetBinContent " << hlookup->GetBinContent(1) << endl;
 
          //look up 
          if (mass >= 20 && mass <= 30) {
              //cout << "Will crash" << endl;
              //cout << "Value " << hlookup2030->GetBinContent(1) << " bin " << hlookup2030->FindBin(rapidity) << endl;
              factor_B_2D = factor_B2030;
              factor_C_2D = hlookup2030->GetBinContent(hlookup2030->FindBin(rapidity));
              factor_D_2D = factor_D2030;
              factor_Berr_2D = factor_Berr2030;
              factor_Cerr_2D = hlookup2030->GetBinError(hlookup2030->FindBin(rapidity));
              factor_Derr_2D = factor_Derr2030;

          } else if (mass >= 30 && mass <= 45) {
              //cout << "Value " << hlookup3045->GetBinContent(1) << " bin " << hlookup3045->FindBin(rapidity) << endl;

              factor_B_2D = factor_B3045;
              factor_C_2D = hlookup3045->GetBinContent(hlookup3045->FindBin(rapidity));
              factor_D_2D = factor_D3045;
              factor_Berr_2D = factor_Berr3045;
              factor_Cerr_2D = hlookup3045->GetBinError(hlookup3045->FindBin(rapidity));
              factor_Derr_2D = factor_Derr3045;

          } else if (mass >= 45 && mass <= 60) {
              //cout << "Value " << hlookup4560->GetBinContent(1) << " bin " << hlookup4560->FindBin(rapidity) << endl;

              factor_B_2D = factor_B4560;
              factor_C_2D = hlookup4560->GetBinContent(hlookup4560->FindBin(rapidity));
              factor_D_2D = factor_D4560;
              factor_Berr_2D = factor_Berr4560;
              factor_Cerr_2D = hlookup4560->GetBinError(hlookup4560->FindBin(rapidity));
              factor_Derr_2D = factor_Derr4560;

          } else if (mass >= 60 && mass <= 120) {
              //cout << "Value " << hlookup60120->GetBinContent(1) << " bin " << hlookup60120->FindBin(rapidity) << endl;

              factor_B_2D = factor_B60120;
              factor_C_2D = hlookup60120->GetBinContent(hlookup60120->FindBin(rapidity));
              factor_D_2D = factor_D60120;
              factor_Berr_2D = factor_Berr60120;
              factor_Cerr_2D = hlookup60120->GetBinError(hlookup60120->FindBin(rapidity));
              factor_Derr_2D = factor_Derr60120;

          } else if (mass >= 120 && mass <= 200) {
              //cout << "Value " << hlookup120200->GetBinContent(1) << " bin " << hlookup120200->FindBin(rapidity) << endl;

              factor_B_2D = factor_B120200;
              factor_C_2D = hlookup120200->GetBinContent(hlookup120200->FindBin(rapidity));
              factor_D_2D = factor_D120200;
              factor_Berr_2D = factor_Berr120200;
              factor_Cerr_2D = hlookup120200->GetBinError(hlookup120200->FindBin(rapidity));
              factor_Derr_2D = factor_Derr120200;

          } else if (mass >= 200 && mass <= 1500) {
              //cout << "Value " << hlookup2001500->GetBinContent(1) << " bin " << hlookup2001500->FindBin(rapidity) << endl;

              factor_B_2D = factor_B2001500;
              factor_C_2D = hlookup2001500->GetBinContent(hlookup2001500->FindBin(rapidity));
              factor_D_2D = factor_D2001500;
              factor_Berr_2D = factor_Berr2001500;
              factor_Cerr_2D = hlookup2001500->GetBinError(hlookup2001500->FindBin(rapidity));
              factor_Derr_2D = factor_Derr2001500;
          }
        }//end !is DATA

        //FIXME decide which rapidity-mass bin to pick up
        //FIXME can i pick up the correct factor for 2D here?
        double value = 0;
        for( int j = 0; j < 6; j++ ) {
          if( mass > Mass_xbin2[j] && mass < Mass_xbin2[j+1] ) {
            int nbins;
            double bin_size;
            if( j == 5 ) {
              nbins = 12;
              bin_size = 0.2;
            } else {
              nbins = 24;
              bin_size = 0.1;
            }
            for( int k = 0; k < nbins; k++ ) {
              if( fabs(rapidity) > k*bin_size && fabs(rapidity) < (k+1)*bin_size ) value = k + j*24;
            }
          }
        }

        //FIXME didnt need that loop
        //for( int j = 0; j < hISOvsVTX_A->GetXaxis()->GetNbins(); j++ ) {
        hISOvsVTX->Fill(var1, var2, WEIGHT*PU_WEIGHT);
        if( var1 < ISO_cut && var2) {
           totA->Fill(mass,WEIGHT*PU_WEIGHT);
           hISOvsVTX_A->Fill(var1, var2, WEIGHT*PU_WEIGHT);
           totA2->Fill(value,WEIGHT*PU_WEIGHT);
        }
        if( var1 < ISO_cut && !var2) {
           totB->Fill(mass,factor_B*WEIGHT*PU_WEIGHT);
           hISOvsVTX_B->Fill(var1, var2, factor_B*WEIGHT*PU_WEIGHT);
           totB2->Fill(value,factor_B_2D*WEIGHT*PU_WEIGHT);
        }
        if( var1 > ISO_cut && var2) {
           totC->Fill(mass,factor_C*WEIGHT*PU_WEIGHT);
           hISOvsVTX_C->Fill(var1, var2, factor_C*WEIGHT*PU_WEIGHT);
           totC2->Fill(value,factor_C_2D*WEIGHT*PU_WEIGHT);
        }
        if( var1 > ISO_cut && !var2) {
           totD->Fill(mass,factor_D*WEIGHT*PU_WEIGHT);
           hISOvsVTX_D->Fill(var1, var2, factor_D*WEIGHT*PU_WEIGHT);
           totD2->Fill(value,factor_D_2D*WEIGHT*PU_WEIGHT);
        }
       //}

     }//end trigger paths

   return kTRUE;
}

void EventSelector_Bkg2::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   

}

void EventSelector_Bkg2::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

//dynamic_cast<TPair*>(fInput->FindObject("PROOF_CurrentElement")))) {
   totA = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totA")));
   totB = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totB")));
   totC = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totC")));
   totD = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totD")));

   //hmuonISO_OS = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmuonISO_OS")));
   //hmuonISO_SS = dynamic_cast<TH1D*>(fOutput->FindObject(Form("hmuonISO_SS")));

   hISOvsVTX = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hISOvsVTX")));
   hISOvsVTX_A = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hISOvsVTX_A")));
   hISOvsVTX_B = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hISOvsVTX_B")));
   hISOvsVTX_C = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hISOvsVTX_C")));
   hISOvsVTX_D = dynamic_cast<TH2D*>(fOutput->FindObject(Form("hISOvsVTX_D")));

   totA2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totA2")));
   totB2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totB2")));
   totC2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totC2")));
   totD2 = dynamic_cast<TH1D*>(fOutput->FindObject(Form("totD2")));

   Nntuple = dynamic_cast<TH1D*>(fOutput->FindObject(Form("norm")));

   //normalization independent on the sample size
   if (dataset != "DATA" && Nntuple->Integral() != 0) { 
      totA->Scale(1./Nntuple->Integral());
      totB->Scale(1./Nntuple->Integral());
      totC->Scale(1./Nntuple->Integral());
      totD->Scale(1./Nntuple->Integral());

      //hmuonISO_OS->Scale(1./Nntuple->Integral());
      //hmuonISO_SS->Scale(1./Nntuple->Integral());

      hISOvsVTX->Scale(1./Nntuple->Integral());
      hISOvsVTX_A->Scale(1./Nntuple->Integral());
      hISOvsVTX_B->Scale(1./Nntuple->Integral());
      hISOvsVTX_C->Scale(1./Nntuple->Integral());
      hISOvsVTX_D->Scale(1./Nntuple->Integral());

      totA2->Scale(1./Nntuple->Integral());
      totB2->Scale(1./Nntuple->Integral());
      totC2->Scale(1./Nntuple->Integral());
      totD2->Scale(1./Nntuple->Integral());
   } //if samples isn't empty

   ds = dynamic_cast<TNamed*>(fOutput->FindObject(Form("outfile")));
   TString z = (TString(ds->GetTitle()) == "") ? dataset : TString(ds->GetTitle());

   this_useNtupleWeightFlag = dynamic_cast<TNamed *>(fInput->FindObject("useNtupleWeightFlag"));
   useNtupleWeightFlag = this_useNtupleWeightFlag ? this_useNtupleWeightFlag->GetTitle() : "";

   this_iso = dynamic_cast<TNamed *>(fInput->FindObject("iso"));
   iso = this_iso ? this_iso->GetTitle() : "";

   std::cout << "NAME = " << z << std::endl;
   TFile fout("step2_"+iso+"_"+z+".root","recreate");
   Nntuple->Write();

   totA->Write();
   totB->Write();
   totC->Write();
   totD->Write();

   //hmuonISO_OS->Write();
   //hmuonISO_SS->Write();

   hISOvsVTX->Write();
   hISOvsVTX_A->Write();
   hISOvsVTX_B->Write();
   hISOvsVTX_C->Write();
   hISOvsVTX_D->Write();

   totA2->Write();
   totB2->Write();
   totC2->Write();
   totD2->Write();

   fout.Close();

}
