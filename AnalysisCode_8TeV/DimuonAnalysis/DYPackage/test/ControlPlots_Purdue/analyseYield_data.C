void analyseYield_data(const char* WHICHHIST, const char* NTUPLEWEIGHT, const char* MOMCORRTYPE, const char* MODE) {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles_Purdue/";

  // data
  TFileCollection* c2a = new TFileCollection("Data_RunAJan2013_Purdue","Data_RunAJan2013_Purdue");
  c2a->Add(protocol+dirname+"Data_RunAJan2013_Purdue"+"/ntuple_skim_84_1_yiY.root");

  gSystem->Load("Muon_cc.so");
  gSystem->Load("Electron_cc.so");
  gSystem->Load("Dimuon_cc.so");
  gSystem->Load("Dielectron_cc.so");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots_Purdue/proofbox/");
  TProof* p = TProof::Open("workers=1");
  p->RegisterDataSet("Data_RunAJan2013_Purdue", c2a,"OV");

  p->ShowDataSets();
  TObjString* useNtupleWeightFlag = new TObjString(NTUPLEWEIGHT);
  p->AddInput(new TNamed("useNtupleWeightFlag",NTUPLEWEIGHT));

  TObjString* histogramThis = new TObjString(WHICHHIST);
  p->AddInput(new TNamed("histogramThis",WHICHHIST));

  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  TObjString* mode = new TObjString(MODE);
  p->AddInput(new TNamed("mode",MODE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("Data_RunAJan2013_Purdue#/recoTree/DiLeptonTree","EventSelector_Purdue.C+");

}
