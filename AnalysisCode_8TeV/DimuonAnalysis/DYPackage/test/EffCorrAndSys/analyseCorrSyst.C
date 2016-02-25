void analyseCorrSyst(const char* EFFTYPE, const char* RUNSPLIT, const char* DOSMEAR) {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/scratch95/a/asvyatko/DY2013/rootfiles/";

//   DY
  TFileCollection* c2a = new TFileCollection("DYM1020","DYM1020");
  c2a->Add(protocol+dirname+"DYM1020_PU"+"/*.root");

  TFileCollection* c2b = new TFileCollection("DYM20","DYM20");
  c2b->Add(protocol+dirname+"DYM20_PU"+"/ntuple_skim_1*.root");

  TFileCollection* c2c = new TFileCollection("DYM200","DYM200");
  c2c->Add(protocol+dirname+"DYM200_PU"+"/*.root");

  TFileCollection* c2d = new TFileCollection("DYM400","DYM400");
  c2d->Add(protocol+dirname+"DYM400_PU"+"/*.root");

  TFileCollection* c2e = new TFileCollection("DYM500","DYM500");
  c2e->Add(protocol+dirname+"DYM500_PU"+"/*.root");

  TFileCollection* c2f = new TFileCollection("DYM700","DYM700");
  c2f->Add(protocol+dirname+"DYM700_PU"+"/*.root");

  TFileCollection* c2g = new TFileCollection("DYM800","DYM800");
  c2g->Add(protocol+dirname+"DYM800_PU"+"/*.root");

  TFileCollection* c2h = new TFileCollection("DYM1000","DYM1000");
  c2h->Add(protocol+dirname+"DYM1000_PU"+"/*.root");


  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/EffCorrAndSys/proofbox");
  TProof* p = TProof::Open("workers=5"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("DYM1020", c2a,"OV");
  p->RegisterDataSet("DYM20", c2b,"OV");
  p->RegisterDataSet("DYM200", c2c,"OV");
  p->RegisterDataSet("DYM400", c2d,"OV");
  p->RegisterDataSet("DYM500", c2e,"OV");
  p->RegisterDataSet("DYM700", c2f,"OV");
  p->RegisterDataSet("DYM800", c2g,"OV");
  p->RegisterDataSet("DYM1000", c2h,"OV");

  p->ShowDataSets();

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");

  TObjString* effType = new TObjString(EFFTYPE);
  p->AddInput(new TNamed("effType",EFFTYPE));
  TObjString* doSmear = new TObjString(DOSMEAR);
  p->AddInput(new TNamed("doSmear",DOSMEAR));
  TObjString* runSplit = new TObjString(RUNSPLIT);
  p->AddInput(new TNamed("runSplit",RUNSPLIT));

  p->Process("DYM1020#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM20#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM200#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM400#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM500#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM700#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM800#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");
  p->Process("DYM1000#/recoTree/DiMuonTree","EventSelector_CorrMass.C+");

}
