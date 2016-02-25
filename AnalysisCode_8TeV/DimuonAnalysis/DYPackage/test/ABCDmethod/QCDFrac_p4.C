void QCDFrac_p4(const char* ISO, const char* REGION) {

  // directory with data
  TString protocol = "file://";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles/";

  // QCD
  TFileCollection* c3a = new TFileCollection("QCD15to20","QCD15to20");
  c3a->Add(protocol+dirname+"QCD15to20"+"/*.root");

  TFileCollection* c3b = new TFileCollection("QCD20to30","QCD20to30");
  c3b->Add(protocol+dirname+"QCD20to30"+"/*.root");

  TFileCollection* c3c = new TFileCollection("QCD30to50","QCD30to50");
  c3c->Add(protocol+dirname+"QCD30to50"+"/*.root");

  TFileCollection* c3d = new TFileCollection("QCD50to80","QCD50to80");
  c3d->Add(protocol+dirname+"QCD50to80"+"/*.root");

  TFileCollection* c3e = new TFileCollection("QCD80to120","QCD80to120");
  c3e->Add(protocol+dirname+"QCD80to120"+"/*.root");
//
  TFileCollection* c3f = new TFileCollection("QCD120to170","QCD120to170");
  c3f->Add(protocol+dirname+"QCD120to170"+"/*.root");

  TFileCollection* c3g = new TFileCollection("QCD170to300","QCD170to300");
  c3g->Add(protocol+dirname+"QCD170to300"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/ba01/u115/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ABCDmethod/proofbox_p4");
  TProof* p = TProof::Open("workers=15"); //"workers=1"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("QCD15to20", c3a,"OV");
  p->RegisterDataSet("QCD20to30", c3b,"OV");
  p->RegisterDataSet("QCD30to50", c3c,"OV");
  p->RegisterDataSet("QCD50to80", c3d,"OV");
  p->RegisterDataSet("QCD80to120", c3e,"OV");
  p->RegisterDataSet("QCD120to170", c3f,"OV");
  p->RegisterDataSet("QCD170to300", c3g,"OV");

  p->ShowDataSets();

  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));
  TObjString* iso = new TObjString(ISO);
  p->AddInput(new TNamed("iso",ISO));
  TObjString* region = new TObjString(REGION);
  p->AddInput(new TNamed("region",REGION));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("QCD15to20#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("QCD20to30#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("QCD30to50#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("QCD50to80#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("QCD80to120#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("QCD120to170#/recoTree/DiMuonTree","EventSelector_Bkg.C+");
  p->Process("QCD170to300#/recoTree/DiMuonTree","EventSelector_Bkg.C+");

}
