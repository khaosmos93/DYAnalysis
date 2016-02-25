void analyseMCtruth() {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles_Purdue/";

//   DY
  TFileCollection* c2b = new TFileCollection("DYM20","DYM20");
  c2b->Add(protocol+dirname+"DYM20_Purdue"+"/*.root");

  gSystem->Load("Muon_cc.so");
  gSystem->Load("Electron_cc.so");
  gSystem->Load("Dimuon_cc.so");
  gSystem->Load("Dielectron_cc.so");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/AccEffMCtruth_Purdue/proofbox/");
  TProof* p = TProof::Open("workers=5");
  p->RegisterDataSet("DYM20", c2b,"OV");

  p->ShowDataSets();

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DYM20#/recoTree/DiLeptonTree","EventSelector_MCTruthEff.C+");

}
