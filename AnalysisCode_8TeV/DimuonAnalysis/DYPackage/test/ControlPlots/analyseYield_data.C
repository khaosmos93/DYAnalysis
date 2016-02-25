void analyseYield_data(const char* WHICHHIST, const char* NTUPLEWEIGHT, const char* MOMCORRTYPE) {

  // directory with data
  TString protocol = "file://";
  //TString protocol = "root://xrootd.rcac.purdue.edu/";
  TString dirname = "/mnt/hadoop/store/group/ewk/DY2013/";

  // data
  TFileCollection* c1 = new TFileCollection("data","data");
  //FIXME splitting by runs/eras is happening here switch to RunAB, RunC, RunD
  if (MOMCORRTYPE == "RunAB") c1->Add(protocol+dirname+"Data_RunAJan2013_Oct"+"/*.root");
  if (MOMCORRTYPE == "RunAB") c1->Add(protocol+dirname+"Data_RunBJan2013_Oct_p1"+"/*.root");
  if (MOMCORRTYPE == "RunAB") c1->Add(protocol+dirname+"Data_RunBJan2013_Oct_p2"+"/*.root");
  if (MOMCORRTYPE == "RunC1") c1->Add(protocol+dirname+"Data_RunCJan2013_Oct_p1"+"/*.root");
  if (MOMCORRTYPE == "RunC2") c1->Add(protocol+dirname+"Data_RunCJan2013_Oct_p2"+"/*.root");
  if (MOMCORRTYPE == "RunD1") c1->Add(protocol+dirname+"Data_RunDJan2013_Oct_p1"+"/*.root");
  if (MOMCORRTYPE == "RunD2") c1->Add(protocol+dirname+"Data_RunDJan2013_Oct_p2"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots/proofbox/");
  TProof* p = TProof::Open("workers=20"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("DATA", c1,"OV");

  p->ShowDataSets();
  TObjString* useNtupleWeightFlag = new TObjString(NTUPLEWEIGHT);
  p->AddInput(new TNamed("useNtupleWeightFlag",NTUPLEWEIGHT));

  TObjString* histogramThis = new TObjString(WHICHHIST);
  p->AddInput(new TNamed("histogramThis",WHICHHIST));

  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DATA#/recoTree/DiMuonTree","EventSelector_CP.C+");

}
