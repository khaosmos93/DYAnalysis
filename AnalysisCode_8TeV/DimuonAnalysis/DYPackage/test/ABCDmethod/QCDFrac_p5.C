void QCDFrac_p5(const char* ISO, const char* REGION) {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles/";

  // EWK
  TFileCollection* c6a = new TFileCollection("Wmunu","Wmunu");
  c6a->Add(protocol+dirname+"Wmunu"+"/*.root");

  TFileCollection* c6b = new TFileCollection("Wtaunu","Wtaunu");
  c6b->Add(protocol+dirname+"Wtaunu"+"/*.root");

  // diboson
  TFileCollection* c7a = new TFileCollection("WZ","WZ");
  c7a->Add(protocol+dirname+"WZ"+"/*.root");

  TFileCollection* c7b = new TFileCollection("ZZ","ZZ");
  c7b->Add(protocol+dirname+"ZZ"+"/*.root");

  TFileCollection* c7c = new TFileCollection("WW","WW");
  c7c->Add(protocol+dirname+"WW"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/ba01/u115/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ABCDmethod/proofbox_p5");
  TProof* p = TProof::Open("workers=10"); //"workers=1"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");

  p->RegisterDataSet("Wmunu", c6a,"OV");
  p->RegisterDataSet("Wtaunu", c6b,"OV");

  p->RegisterDataSet("WZ", c7a,"OV");
  p->RegisterDataSet("ZZ", c7b,"OV");
  p->RegisterDataSet("WW", c7c,"OV");

  p->ShowDataSets();

  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));
  TObjString* iso = new TObjString(ISO);
  p->AddInput(new TNamed("iso",ISO));
  TObjString* region = new TObjString(REGION);
  p->AddInput(new TNamed("region",REGION));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("Wmunu#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("Wtaunu#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("WW#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("WZ#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("ZZ#/recoTree/DiMuonTree","EventSelector_Bkg.C+");

}
