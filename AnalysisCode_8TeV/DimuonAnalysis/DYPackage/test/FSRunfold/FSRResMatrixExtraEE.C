void FSRResMatrixExtraEE(const char* MOMCORRTYPE, TString datasetName, TString chunk, TString Alt) {

  // directory with data
  TString protocol = "file://";
  TString protocol_rootd = "root://xrootd.rcac.purdue.edu:1094/";
  TString dirname = "/mnt/hadoop/store/user/asvyatko/DYstudy/dataAnalysis13/rootfiles/";
  TString dirname_rootd = "/store/user/asvyatko/DYstudy/dataAnalysis13/rootfiles/"; //DYM400_PUOct/ntuple_skim_76_1_zFQ.root

  TString dataset = beautifulDataset(datasetName);

  if (Alt == "rootd" || Alt == "rootdhep") { 
      protocol = protocol_rootd; 
      dirname  = dirname_rootd;
  }

  TDSet* c = new TDSet("TTree", "DiElectronTree", "recoTree");//dataset,dataset);
  //Read filelist
  TString fileName = datasetName+"_"+chunk+".txt";
  string pprint;
  ifstream rfile("./input_lists/"+fileName);
  cout << fileName << endl;
  if (rfile.is_open())
  {
    while (getline(rfile,pprint) )
    {
      TString fpath = protocol+dirname+datasetName+"/"+TString(pprint.c_str());
      cout << "Adding " << fpath << endl; 
      c->Add(fpath.Data());   
    }
    rfile.close();
  }

  TString ncores = "38";
  if (Alt == "rootd") ncores = "23";
  if (chunk.Contains("_")) {
      if (dataset == "EEDYM20") ncores = "15";
      else ncores = "1";
  } else if (dataset.Contains("00")) {
      ncores = "1";
  } 
  if (dataset == "EEDYM1020") { 
    if (!chunk.Contains("_") || c->GetNumOfFiles() > 5) ncores = "10";
    else ncores = "1";
  } 
  if (dataset == "EEDYM20" && c->GetNumOfFiles() < 8 && !chunk.Contains("_")) { ncores = "18"; }
  if (dataset == "EEDYM20" && c->GetNumOfFiles() < 6 && !chunk.Contains("_")) { ncores = "14"; }
  if (dataset == "EEDYM20" && c->GetNumOfFiles() < 4 && !chunk.Contains("_")) { ncores = "8"; }

  gEnv->SetValue("ProofLite.Sandbox", "/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/FSRunfold/proofboxEE/");
  TProof* p = TProof::Open("workers="+ncores);
  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));
  TObjString* momCorrType = new TObjString(MOMCORRTYPE);
  p->AddInput(new TNamed("momCorrType",MOMCORRTYPE));
  dataset = dataset+chunk;
  TObjString* dName = new TObjString(string(dataset).c_str());
  p->AddInput(new TNamed("dName",string(dataset).c_str()));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  //p->Process(dataset+chunk+"#/recoTree/DiElectronTree","EleEventSelector_Unf.C+");
  p->Process(c,"EleEventSelector_FSRUnfExtra.C+");

}

TString beautifulDataset(TString dataset) {

  if (dataset == "EEDYM1000_PUOct") return "EEDYM1000";
  if (dataset == "EEDYM1500_PUOct") return "EEDYM1500";
  if (dataset == "EEDYM2000_PUOct") return "EEDYM2000";
  if (dataset == "EEDYM800_PUOct") return "EEDYM800";
  if (dataset == "EEDYM700_PUOct") return "EEDYM700";
  if (dataset == "EEDYM500_PUOct") return "EEDYM500";
  if (dataset == "EEDYM400_PUOct") return "EEDYM400";
  if (dataset == "EEDYM200_PUOct") return "EEDYM200";
  if (dataset == "EEDYM1020_PUOct_FULL") return "EEDYM1020";
  if (dataset == "EEDYM20_PUOct") return "EEDYM20";

  if (dataset.Contains("DYM20_PUOct")) return "DYM20";

  return "";
}
