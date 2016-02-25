void FSRBinByBin() {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles/";


//   DY
  //TFileCollection* c2a = new TFileCollection("DYM1020_LightPhoton","DYM1020_LightPhoton");
  //c2a->Add(protocol+dirname+"DYM1020_LightPhoton"+"/*.root");

  //TFileCollection* c2b = new TFileCollection("DYM20_LightPhoton","DYM20_LightPhoton");
  //c2b->Add(protocol+dirname+"DYM20_LightPhoton"+"/*.root");

  TFileCollection* c2c = new TFileCollection("DYM200_LightPhoton","DYM200_LightPhoton");
  c2c->Add(protocol+dirname+"DYM200_LightPhoton"+"/*9*.root");

  //TFileCollection* c2d = new TFileCollection("DYM400_LightPhoton","DYM400_LightPhoton");
  //c2d->Add(protocol+dirname+"DYM400_LightPhoton"+"/*.root");

  //TFileCollection* c2e = new TFileCollection("DYM500_LightPhoton","DYM500_LightPhoton");
  //c2e->Add(protocol+dirname+"DYM500_LightPhoton"+"/*.root");

  //TFileCollection* c2f = new TFileCollection("DYM700_LightPhoton","DYM700_LightPhoton");
  //c2f->Add(protocol+dirname+"DYM700_LightPhoton"+"/*.root");

  //TFileCollection* c2g = new TFileCollection("DYM800_LightPhoton","DYM800_LightPhoton");
  //c2g->Add(protocol+dirname+"DYM800_LightPhoton"+"/*.root");

  //TFileCollection* c2h = new TFileCollection("DYM1000_LightPhoton","DYM1000_LightPhoton");
  //c2h->Add(protocol+dirname+"DYM1000_LightPhoton"+"/*.root");

  //TFileCollection* c2i = new TFileCollection("DYM1500_LightPhoton","DYM1500_LightPhoton");
  //c2i->Add(protocol+dirname+"DYM1500_LightPhoton"+"/*.root");

  //TFileCollection* c2j = new TFileCollection("DYM2000_LightPhoton","DYM2000_LightPhoton");
  //c2j->Add(protocol+dirname+"DYM2000_LightPhoton"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/FSRunfold/proofbox/");
  TProof* p = TProof::Open("workers=1");
  //p->RegisterDataSet(beautifulDataset("DYM1020_LightPhoton"), c2a,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM20_LightPhoton"), c2b,"OV");
  p->RegisterDataSet(beautifulDataset("DYM200_LightPhoton"), c2c,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM400_LightPhoton"), c2d,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM500_LightPhoton"), c2e,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM700_LightPhoton"), c2f,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM800_LightPhoton"), c2g,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM1000_LightPhoton"), c2h,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM1500_LightPhoton"), c2i,"OV");
  //p->RegisterDataSet(beautifulDataset("DYM2000_LightPhoton"), c2j,"OV");


  p->ShowDataSets();
  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");

  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));
  //TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  //p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  //p->Process("DYM1020#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM20#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  p->Process("DYM200#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB_new.C+");
  //p->Process("DYM400#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM500#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM700#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM800#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM1000#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM1500#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");
  //p->Process("DYM2000#/recoTree/DiMuonTree","EventSelector_FSRUnfBBB.C+");

}

TString beautifulDataset(TString dataset) {

  if (dataset == "DYM1000_LightPhoton") return "DYM1000";
  if (dataset == "DYM1500_LightPhoton") return "DYM1500";
  if (dataset == "DYM2000_LightPhoton") return "DYM2000";
  if (dataset == "DYM800_LightPhoton") return "DYM800";
  if (dataset == "DYM700_LightPhoton") return "DYM700";
  if (dataset == "DYM500_LightPhoton") return "DYM500";
  if (dataset == "DYM400_LightPhoton") return "DYM400";
  if (dataset == "DYM200_LightPhoton") return "DYM200";
  if (dataset == "DYM1020_LightPhoton") return "DYM1020";
  if (dataset == "DYM20_LightPhoton") return "DYM20";

  return "";
}
