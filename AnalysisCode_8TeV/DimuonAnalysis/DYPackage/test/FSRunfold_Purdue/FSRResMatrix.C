void FSRResMatrix(const char* MODE,TString datasetName, TString chunk, TString Alt) {

  // directory with data
  TString protocol = "file://";
  //TString dirname = "/mnt/hadoop/store/user/asvyatko/DYstudy/dataAnalysis13/rootfiles/";
  TString dirnameEE  = "/scratch/scratch95/a/asvyatko/DY2013/rootfiles_Purdue/";
  TString dirnameMuMu = "/scratch/lustreC/a/asvyatko/DY2013/rootfiles_Purdue/";
  TString dirname = dirnameMuMu;
  if (datasetName.Contains("EE")) dirname = dirnameEE; 

  TString ncores = "3";
  if (chunk.Contains("_") || datasetName.Contains("00")) ncores = "1";
  if (datasetName.Contains("1020")) ncores = "1";

  TString loads = protocol+dirname+datasetName+"/ntuple_skim_"+chunk+"*.root";

  //standardize names
  beautifulDataset(datasetName);

  TFileCollection* c = new TFileCollection(datasetName+chunk,datasetName+chunk);
  c->Add(loads);

  gSystem->Load("Muon_cc.so");
  gSystem->Load("Electron_cc.so");
  gSystem->Load("Dimuon_cc.so");
  gSystem->Load("Dielectron_cc.so");

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/Analysis/DYPackage/test/FSRunfold_Purdue/proofbox/");
  TProof* p = TProof::Open("workers="+ncores);
  p->RegisterDataSet(datasetName+chunk, c,"OV");

  p->ShowDataSets();
  TObjString* mode = new TObjString(MODE);
  p->AddInput(new TNamed("mode",MODE));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process(datasetName+chunk+"#/recoTree/DiLeptonTree","EventSelector_FSRUnf.C+");

}


void beautifulDataset(TString& dataset) {

  if (dataset == "EEDYM2000_Purdue_ES") dataset = "EEDYM2000";
  if (dataset == "EEDYM1500_Purdue_ES") dataset = "EEDYM1500";
  if (dataset == "EEDYM1000_Purdue_ES") dataset = "EEDYM1000";
  if (dataset == "EEDYM800_Purdue_ES") dataset = "EEDYM800";
  if (dataset == "EEDYM700_Purdue_ES") dataset = "EEDYM700";
  if (dataset == "EEDYM500_Purdue_ES") dataset = "EEDYM500";
  if (dataset == "EEDYM400_Purdue_ES") dataset = "EEDYM400";
  if (dataset == "EEDYM200_Purdue_ES") dataset = "EEDYM200";
  if (dataset == "EEDYM1020_Purdue_ES") dataset = "EEDYM1020";
  if (dataset == "EEDYM20_Purdue_ES_Samp2") dataset = "EEDYM20";

  if (dataset == "DYM2000_Purdue_ES") dataset = "DYM2000";
  if (dataset == "DYM1500_Purdue_ES") dataset = "DYM1500";
  if (dataset == "DYM1000_Purdue_ES") dataset = "DYM1000";
  if (dataset == "DYM800_Purdue_ES") dataset = "DYM800";
  if (dataset == "DYM700_Purdue_ES") dataset = "DYM700";
  if (dataset == "DYM500_Purdue_ES") dataset = "DYM500";
  if (dataset == "DYM400_Purdue_ES") dataset = "DYM400";
  if (dataset == "DYM200_Purdue_ES") dataset = "DYM200";
  if (dataset == "DYM1020_Purdue_ES") dataset = "DYM1020";
  if (dataset == "DYM20_Purdue_ES_Samp2") dataset = "DYM20";


}
