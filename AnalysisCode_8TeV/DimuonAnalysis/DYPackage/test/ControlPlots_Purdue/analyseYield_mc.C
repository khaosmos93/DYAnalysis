void analyseYield_mc(const char* WHICHHIST, const char* NTUPLEWEIGHT, const char* MOMCORRTYPE, const char* MODE) {

  // directory with data
  TString protocol = "file://";
  //TString dirname = "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots_Purdue/";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles_Purdue/";

  // data
  TFileCollection* c2a = new TFileCollection("DYM20_Purdue","DYM20_Purdue");
  c2a->Add(protocol+dirname+"DYM20_Purdue/*9*.root");  //ntuple_skim_1_1_KzL.root"); //ntuple_skim_1_1_E3U.root");

  gSystem->Load("Muon_cc.so");
  gSystem->Load("Electron_cc.so");
  gSystem->Load("Dimuon_cc.so");
  gSystem->Load("Dielectron_cc.so");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots_Purdue/proofbox/");
  TProof* p = TProof::Open("workers=1");
  p->RegisterDataSet("DYM20_Purdue", c2a,"OV");

  p->ShowDataSets();
  TObjString* mode = new TObjString(MODE);
  p->AddInput(new TNamed("mode",MODE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DYM20_Purdue#/recoTree/DiLeptonTree","EventSelector.C+");

}
