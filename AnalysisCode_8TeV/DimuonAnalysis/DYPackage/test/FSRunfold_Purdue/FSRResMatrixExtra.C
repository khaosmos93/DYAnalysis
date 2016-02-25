void FSRResMatrixExtra(const char* MODE) {

  // directory with data
  TString protocol = "file://";
  TString dirnameEE  = "/scratch/scratch95/a/asvyatko/DY2013/rootfiles_Purdue/";
  TString dirnameMuMu = "/scratch/lustreC/a/asvyatko/DY2013/rootfiles_Purdue/";
  TString dirname = dirnameMuMu;
  if (MODE == "EE") { 
    dirname = dirnameEE + TString(MODE);
  }
 
//   DY
  TFileCollection* c2a = new TFileCollection("DYM1020","DYM1020");
  c2a->Add(protocol+dirname+"DYM1020_Purdue_ES"+"/*.root");

  TFileCollection* c2b = new TFileCollection("DYM20","DYM20");
  c2b->Add(protocol+dirname+"DYM20_Purdue_ES_samp"+"/*.root");

  TFileCollection* c2c = new TFileCollection("DYM200","DYM200");
  c2c->Add(protocol+dirname+"DYM200_Purdue_ES"+"/*.root");

  TFileCollection* c2d = new TFileCollection("DYM400","DYM400");
  c2d->Add(protocol+dirname+"DYM400_Purdue_ES"+"/*.root");

  TFileCollection* c2e = new TFileCollection("DYM500","DYM500");
  c2e->Add(protocol+dirname+"DYM500_Purdue_ES"+"/*.root");

  TFileCollection* c2f = new TFileCollection("DYM700","DYM700");
  c2f->Add(protocol+dirname+"DYM700_Purdue_ES"+"/*.root");

  TFileCollection* c2g = new TFileCollection("DYM800","DYM800");
  c2g->Add(protocol+dirname+"DYM800_Purdue_ES"+"/*.root");

  TFileCollection* c2h = new TFileCollection("DYM1000","DYM1000");
  c2h->Add(protocol+dirname+"DYM1000_Purdue_ES"+"/*.root");

  TFileCollection* c2i = new TFileCollection("DYM1500","DYM1500");
  c2i->Add(protocol+dirname+"DYM1500_Purdue_ES"+"/*.root");

  TFileCollection* c2j = new TFileCollection("DYM2000","DYM2000");
  c2j->Add(protocol+dirname+"DYM2000_Purdue_ES"+"/*.root");

  gSystem->Load("Muon_cc.so");
  gSystem->Load("Electron_cc.so");
  gSystem->Load("Dimuon_cc.so");
  gSystem->Load("Dielectron_cc.so");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/Analysis/DYPackage/test/FSRunfold_Purdue/proofbox/");
  TProof* p = TProof::Open("workers=30");
  p->RegisterDataSet("DYM1020", c2a,"OV");
  p->RegisterDataSet("DYM20", c2b,"OV");
  p->RegisterDataSet("DYM200", c2c,"OV");
  p->RegisterDataSet("DYM400", c2d,"OV");
  p->RegisterDataSet("DYM500", c2e,"OV");
  p->RegisterDataSet("DYM700", c2f,"OV");
  p->RegisterDataSet("DYM800", c2g,"OV");
  p->RegisterDataSet("DYM1000", c2h,"OV");
  p->RegisterDataSet("DYM1500", c2i,"OV");
  p->RegisterDataSet("DYM2000", c2j,"OV");

  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));

  TObjString* mode = new TObjString(MODE);
  p->AddInput(new TNamed("mode",MODE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DYM1020#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM20#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM200#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM400#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM500#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM700#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM800#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM1000#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM1500#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");
  p->Process("DYM2000#/recoTree/DiLeptonTree","EventSelector_FSRUnfExtra.C+");

}
