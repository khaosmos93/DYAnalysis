void analyseYield_p3(const char* WHICHHIST, const char* NTUPLEWEIGHT, const char* MOMCORRTYPE) {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/lustreC/a/asvyatko/DY2013/rootfiles/";

  // EWK
  TFileCollection* c6 = new TFileCollection("WJets","WJets");
  c6->Add(protocol+dirname+"WJets_Light"+"/*.root");

  // diboson
  TFileCollection* c7a = new TFileCollection("WZJetsTo3LNu","WZJetsTo3LNu");
  c7a->Add(protocol+dirname+"WZJetsTo3LNu_Light"+"/*.root");

  TFileCollection* c7b = new TFileCollection("WZJetsTo2L2Q","WZJetsTo2L2Q");
  c7b->Add(protocol+dirname+"WZJetsTo2L2Q_Light"+"/*.root");

  TFileCollection* c7c = new TFileCollection("ZZJetsTo2L2Nu","ZZJetsTo2L2Nu");
  c7c->Add(protocol+dirname+"ZZJetsTo2L2Nu_Light"+"/*.root");

  TFileCollection* c7d = new TFileCollection("ZZJetsTo4L","ZZJetsTo4L");
  c7d->Add(protocol+dirname+"ZZJetsTo4L_Light"+"/*.root");

  TFileCollection* c7e = new TFileCollection("ZZJetsTo2L2Q","ZZJetsTo2L2Q");
  c7e->Add(protocol+dirname+"ZZJetsTo2L2Q_Light"+"/*.root");

  TFileCollection* c7f = new TFileCollection("WWJetsTo2L2Nu","WWJetsTo2L2Nu");
  c7f->Add(protocol+dirname+"WWJetsTo2L2Nu_Light"+"/*.root");

  //ttbar
  TFileCollection* c8a = new TFileCollection("tW","tW");
  c8a->Add(protocol+dirname+"tW_Light"+"/*.root");

  TFileCollection* c8b = new TFileCollection("tbarW","tbarW");
  c8b->Add(protocol+dirname+"tbarW_Light"+"/*.root");

  TFileCollection* c8c = new TFileCollection("tt1000","ttbar1000");
  c8c->Add(protocol+dirname+"tt1000_Light"+"/*.root");

  TFileCollection* c8d = new TFileCollection("tt700","ttbar700");
  c8d->Add(protocol+dirname+"tt700_Light"+"/*.root");

  TFileCollection* c8e = new TFileCollection("ttjets","ttjets");
  c8e->Add(protocol+dirname+"ttjets_v1_Light"+"/*.root");
  c8e->Add(protocol+dirname+"ttjets_v2_Light"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots/proofbox_p3/");
  TProof* p = TProof::Open("workers=15"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("WJets", c6,"OV");
  p->RegisterDataSet("WZJetsTo3LNu", c7a,"OV");
  p->RegisterDataSet("WZJetsTo2L2Q", c7b,"OV");
  p->RegisterDataSet("ZZJetsTo2L2Nu", c7c,"OV");
  p->RegisterDataSet("ZZJetsTo4L", c7d,"OV");
  p->RegisterDataSet("ZZJetsTo2L2Q", c7e,"OV");
  p->RegisterDataSet("WWJetsTo2L2Nu", c7f,"OV");
  p->RegisterDataSet("tW", c8a,"OV");
  p->RegisterDataSet("tbarW", c8b,"OV");
  p->RegisterDataSet("tt1000", c8c,"OV");
  p->RegisterDataSet("tt700", c8d,"OV");
  p->RegisterDataSet("ttjets", c8e,"OV");


  p->ShowDataSets();
  TObjString* useNtupleWeightFlag = new TObjString(NTUPLEWEIGHT);
  p->AddInput(new TNamed("useNtupleWeightFlag",NTUPLEWEIGHT));

  TObjString* histogramThis = new TObjString(WHICHHIST);
  p->AddInput(new TNamed("histogramThis",WHICHHIST));

  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("WJets#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("WZJetsTo3LNu#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("WZJetsTo2L2Q#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("ZZJetsTo2L2Nu#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("ZZJetsTo4L#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("ZZJetsTo2L2Q#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("WWJetsTo2L2Nu#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("tW#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("tbarW#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("tt1000#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("tt700#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process("ttjets#/recoTree/DiMuonTree","EventSelector_CP.C+");

}

