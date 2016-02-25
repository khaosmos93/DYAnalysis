void ResMatrix(const char* MOMCORRTYPE, const char* MODE) {

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

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/Unfold_Purdue/proofbox/");
  TProof* p = TProof::Open("workers=20");
  p->RegisterDataSet("DYM20", c2b,"OV");

  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));

  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  TObjString* mode = new TObjString(MODE);
  p->AddInput(new TNamed("mode",MODE));


  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DYM20#/recoTree/DiLeptonTree","EventSelector_Unf.C+");

}
