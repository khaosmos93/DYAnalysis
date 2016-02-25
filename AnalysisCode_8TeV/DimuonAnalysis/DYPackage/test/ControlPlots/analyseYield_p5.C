void analyseYield_p5(const char* WHICHHIST, const char* NTUPLEWEIGHT, const char* MOMCORRTYPE) {

  // directory with data
  TString protocol = "file://";
  // TString protocol = "root://xrootd.rcac.purdue.edu/";
//from Norbert root://xrootd.rcac.purdue.edu//store/group/ewk/DY/DYM20/ntuple_skim_96_2_z5w.root my.root
  //TString protocol = "dcap://dcache.rcac.purdue.edu:22125/pnfs/rcac.purdue.edu/data"; 
  TString dirname = "/mnt/hadoop/store/user/asvyatko/DYstudy/dataAnalysis13/rootfiles/";
  //TString dirname = "/scratch/scratch95/a/asvyatko/DY2012_testdir/rootfiles/";
  //TString dirname = "/store/group/ewk/DY/";
  //TString dirname = "/store/group/ewk/DY/";

  // QCD
  TFileCollection* c3a = new TFileCollection("GGmumuElast","GGmumuElast");
  c3a->Add(protocol+dirname+"GGmumuElast"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/ba01/u115/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/ControlPlots/proofbox_p5");
  TProof* p = TProof::Open("workers=1"); //"workers=5"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("GGmumuElast", c3a,"OV");

  p->ShowDataSets();
  TObjString* useNtupleWeightFlag = new TObjString(NTUPLEWEIGHT);
  p->AddInput(new TNamed("useNtupleWeightFlag",NTUPLEWEIGHT));

  TObjString* histogramThis = new TObjString(WHICHHIST);
  p->AddInput(new TNamed("histogramThis",WHICHHIST));

  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("GGmumuElast#/recoTree/DiMuonTree","EventSelector_CP.C+");

}
