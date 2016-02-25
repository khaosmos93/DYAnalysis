void QCDFrac_p1(const char* ISO, const char* REGION) {

  // directory with data
  TString protocol = "file://";
  //TString protocol = "root://xrootd.rcac.purdue.edu/";
  TString dirname = "/scratch/lustreA/a/asvyatko/DY2013/rootfiles/";

  // data
  TFileCollection* c1 = new TFileCollection("data","data");
  c1->Add(protocol+dirname+"Data_RunAJan2013"+"/*.root");
  c1->Add(protocol+dirname+"Data_RunBJan2013_p1"+"/*.root");
  c1->Add(protocol+dirname+"Data_RunBJan2013_p2"+"/*.root");
  c1->Add(protocol+dirname+"Data_RunCJan2013_p1"+"/*.root");
  c1->Add(protocol+dirname+"Data_RunCJan2013_p2"+"/*.root");
  c1->Add(protocol+dirname+"Data_RunDJan2013_p1"+"/*.root");
  c1->Add(protocol+dirname+"Data_RunDJan2013_p2"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ABCDmethod/proofbox_p1/");
  TProof* p = TProof::Open("workers=15"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("DATA", c1,"OV");

  p->ShowDataSets();

  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));
  TObjString* iso = new TObjString(ISO);
  p->AddInput(new TNamed("iso",ISO));
  TObjString* region = new TObjString(REGION);
  p->AddInput(new TNamed("region",REGION));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DATA#/recoTree/DiMuonTree","EventSelector_Bkg.C+");

}
