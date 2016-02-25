void analyseYield_mc(const char* WHICHHIST, const char* NTUPLEWEIGHT, const char* MOMCORRTYPE) {

  // directory with data
  TString protocol = "file://";
  //TString protocol = "root://xrootd.rcac.purdue.edu/";
  //TString dirname = "/mnt/hadoop/store/group/ewk/DY2013/";
  TString dirname = "/mnt/hadoop/store/user/asvyatko/DYstudy/dataAnalysis13/rootfiles/";

  // DY
  TFileCollection* c1 = new TFileCollection("DYM1020_PUOct","DYM1020_PUOct");
  c1->Add(protocol+dirname+"DYM1020_PUOct_FULL"+"/*.root");

  TFileCollection* c2 = new TFileCollection("DYM20_PUOct","DYM20_PUOct");
  c2->Add(protocol+dirname+"DYM20_PUOct_Stefano"+"/ntuple_skim_*.root");

  TFileCollection* c3 = new TFileCollection("DYM200_PUOct","DYM200_PUOct");
  c3->Add(protocol+dirname+"DYM200_PUOct"+"/*.root");

  TFileCollection* c4 = new TFileCollection("DYM400_PUOct","DYM400_PUOct");
  c4->Add(protocol+dirname+"DYM400_PUOct"+"/*.root");

  TFileCollection* c5 = new TFileCollection("DYM500_PUOct","DYM500_PUOct");
  c5->Add(protocol+dirname+"DYM500_PUOct"+"/*.root");

  TFileCollection* c6 = new TFileCollection("DYM700_PUOct","DYM700_PUOct");
  c6->Add(protocol+dirname+"DYM700_PUOct"+"/*.root");

  TFileCollection* c7 = new TFileCollection("DYM800_PUOct","DYM800_PUOct");
  c7->Add(protocol+dirname+"DYM800_PUOct"+"/*.root");

  TFileCollection* c8 = new TFileCollection("DYM1000_PUOct","DYM1000_PUOct");
  c8->Add(protocol+dirname+"DYM1000_PUOct"+"/*.root");

  TFileCollection* c9 = new TFileCollection("DYM1500_PUOct","DYM1500_PUOct");
  c9->Add(protocol+dirname+"DYM1500_PUOct"+"/*.root");

  //TFileCollection* c10 = new TFileCollection("DYM2000","DYM2000");
  //c10->Add(protocol+dirname+"DYM2000"+"/*.root");

  TString ncores = "20";

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots/proofbox/");
  TProof* p = TProof::Open("workers=20"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet(beautifulDataset("DYM1020_PUOct"), c1,"OV");
  p->RegisterDataSet(beautifulDataset("DYM20_PUOct"), c2,"OV");
  p->RegisterDataSet(beautifulDataset("DYM200_PUOct"), c3,"OV");
  p->RegisterDataSet(beautifulDataset("DYM400_PUOct"), c4,"OV");
  p->RegisterDataSet(beautifulDataset("DYM500_PUOct"), c5,"OV");
  p->RegisterDataSet(beautifulDataset("DYM700_PUOct"), c6,"OV");
  p->RegisterDataSet(beautifulDataset("DYM800_PUOct"), c7,"OV");
  p->RegisterDataSet(beautifulDataset("DYM1000_PUOct"), c8,"OV");
  p->RegisterDataSet(beautifulDataset("DYM1500_PUOct"), c9,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM2000"), c10,"OV");
  p->ShowDataSets();
  TObjString* useNtupleWeightFlag = new TObjString(NTUPLEWEIGHT);
  p->AddInput(new TNamed("useNtupleWeightFlag",NTUPLEWEIGHT));

  TObjString* histogramThis = new TObjString(WHICHHIST);
  p->AddInput(new TNamed("histogramThis",WHICHHIST));

  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process(beautifulDataset("DYM1020_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM20_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM200_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM400_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM500_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM700_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM800_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM1000_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  p->Process(beautifulDataset("DYM1500_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");
  //p->Process(beautifulDataset("DYM2000_PUOct")+"#/recoTree/DiMuonTree","EventSelector_CP.C+");

}

TString beautifulDataset(TString dataset) {

  if (dataset == "DYM1000_PUOct") return "DYM1000";
  if (dataset == "DYM1500_PUOct") return "DYM1500";
  if (dataset == "DYM2000_PUOct") return "DYM2000";
  if (dataset == "DYM800_PUOct") return "DYM800";
  if (dataset == "DYM700_PUOct") return "DYM700";
  if (dataset == "DYM500_PUOct") return "DYM500";
  if (dataset == "DYM400_PUOct") return "DYM400";
  if (dataset == "DYM200_PUOct") return "DYM200";
  if (dataset == "DYM1020_PUOct") return "DYM1020";
  if (dataset == "DYM20_PUOct") return "DYM20";

  return "";
}
