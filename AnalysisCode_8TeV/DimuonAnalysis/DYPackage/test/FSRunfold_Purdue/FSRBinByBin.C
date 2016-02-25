void FSRBinByBin(const char* MODE,TString datasetName, char* chunk, TString Alt) {

  // directory with data
  TString protocol = "file://";
  //TString dirname = "/mnt/hadoop/store/user/asvyatko/DYstudy/dataAnalysis13/rootfiles/";
  TString dirnameEE  = "/scratch/scratch95/a/asvyatko/DY2013/rootfiles_Purdue/";
  TString dirnameMuMu = "/scratch/lustreC/a/asvyatko/DY2013/rootfiles_Purdue/";
  TString dirname = dirnameMuMu;
  if (datasetName.Contains("EE")) dirname = dirnameEE; 

  TString ncores = "30";
  //if (!datasetName.Contains("samp") && (chunk.Contains("_") || datasetName.Contains("00"))) ncores = "1";
  if (!(datasetName.Contains("1020") || datasetName.Contains("samp"))) ncores = "3";
  if (datasetName.Contains("1020")) ncores = "10";

  TString beautiful = beautifulDataset(datasetName);
  TFileCollection* c = loadChunks(chunk[0],protocol,dirname,datasetName,beautiful);

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
  p->Process(datasetName+chunk+"#/recoTree/DiLeptonTree","EventSelector_FSRUnfBBB.C+");

}


TString beautifulDataset(TString dataset) {

  if (dataset == "EEDYM2000_Purdue_ES") return "EEDYM2000";
  if (dataset == "EEDYM1500_Purdue_ES") return "EEDYM1500";
  if (dataset == "EEDYM1000_Purdue_ES") return "EEDYM1000";
  if (dataset == "EEDYM800_Purdue_ES") return "EEDYM800";
  if (dataset == "EEDYM700_Purdue_ES") return "EEDYM700";
  if (dataset == "EEDYM500_Purdue_ES") return "EEDYM500";
  if (dataset == "EEDYM400_Purdue_ES") return "EEDYM400";
  if (dataset == "EEDYM200_Purdue_ES") return "EEDYM200";
  if (dataset == "EEDYM1020_Purdue_ES") return "EEDYM1020";
  if (dataset == "EEDYM20_Purdue_ES_samp2") return "EEDYM20";

  if (dataset == "DYM2000_Purdue_ES") return "DYM2000";
  if (dataset == "DYM1500_Purdue_ES") return "DYM1500";
  if (dataset == "DYM1000_Purdue_ES") return "DYM1000";
  if (dataset == "DYM800_Purdue_ES") return "DYM800";
  if (dataset == "DYM700_Purdue_ES") return "DYM700";
  if (dataset == "DYM500_Purdue_ES") return "DYM500";
  if (dataset == "DYM400_Purdue_ES") return "DYM400";
  if (dataset == "DYM200_Purdue_ES") return "DYM200";
  if (dataset == "DYM1020_Purdue_ES") return "DYM1020";
  if (dataset == "DYM20_Purdue_ES_samp2") return "DYM20";

  return "";
}


TFileCollection* loadChunks(char chunk, TString protocol, TString dirname, TString datasetName, TString beautiful) {

  TFileCollection* c = new TFileCollection(beautiful+chunk,beautiful+chunk);

 //DYM20 EE
 if (beautifulDataset(datasetName) == "EEDYM20") {
  switch (chunk) {
    case '1':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_10*.root");
      break;
    case '2':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_11*.root");
      break;
    case '3':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_12*.root");
      break;
    case '4':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_13*.root");
      break;
    case '5':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_14*.root");
      break;
    case '6':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_15*.root");
      break;
    case '7':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_16*.root");
      break;
    case '8':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_17*.root");
      break;
    case '9':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_18*.root");
      break;
    case '10':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_19*.root");
      break;
    case '11':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_1_*.root");
      break;
    default:
      cout << "Wrong chunk" << endl;
      break;
    }
  //DYM20 MuMu
  } else if (beautifulDataset(datasetName) == "DYM20") {
  switch (chunk) {
    case '1':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_10*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_20*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_30*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_40*.root");
      break;
    case '2':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_11*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_21*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_31*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_41*.root");
      break;
    case '3':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_12*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_22*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_32*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_42*.root");
      break;
    case '4':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_13*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_23*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_33*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_43*.root");
      break;
    case '5':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_14*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_24*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_34*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_44*.root");
      break;
    case '6':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_15*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_25*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_35*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_45*.root");
      break;
    case '7':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_16*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_26*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_36*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_46*.root");
      break;
    case '8':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_17*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_27*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_37*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_47*.root");
      break;
    case '9':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_18*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_28*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_38*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_48*.root");
      break;
    case '10':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_19*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_29*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_39*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_49*.root");
      break;
    case '11':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_1_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_2_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_3_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_4_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_5*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_6*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_7*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_8*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_9*.root");
      break;
    default:
      cout << "Wrong chunk" << endl;
      break;
    }
  } else if (beautifulDataset(datasetName) == "DYM1020" || beautifulDataset(datasetName) == "EEDYM1020") {
  switch (chunk) {
    case '1':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_10*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_20*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_30*.root");
      break;
    case '2':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_11*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_21*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_31*.root");
      break;
    case '3':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_12*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_22*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_32*.root");
      break;
    case '4':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_13*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_23*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_33*.root");
      break;
    case '5':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_14*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_24*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_34*.root");
      break;
    case '6':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_15*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_25*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_35*.root");
      break;
    case '7':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_16*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_26*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_36*.root");
      break;
    case '8':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_17*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_27*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_37*.root");
      break;
    case '9':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_18*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_28*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_38*.root");
      break;
    case '10':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_19*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_29*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_39*.root");
      break;
    case '11':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_1_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_2_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_3_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_4*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_5*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_6*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_7*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_8*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_9*.root");
      break;
    default:
      cout << "Wrong chunk" << endl;
      break;
    }
  } else {
   //the rest ?
  switch (chunk) {
    case '1':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_1_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_2_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_3_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_4_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_5_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_6_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_7_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_8_*.root");
      break;
    case '2':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_9_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_10_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_11_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_12_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_13_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_14_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_15_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_16_*.root");
      break;
    case '3':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_20_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_21_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_22_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_23_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_24_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_25_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_26_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_27_*.root");
      break;
    case '4':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_30_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_31_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_32_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_33_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_34_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_35_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_36_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_37_*.root");
      break;
    case '5':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_40_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_41_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_42_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_43_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_44_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_45_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_46_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_47_*.root");
      break;
    case '6':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_50_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_51_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_52_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_53_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_54_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_55_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_56_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_57_*.root");
      break;
    case '7':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_60_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_61_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_62_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_63_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_64_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_65_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_66_*.root");
      break;
    case '8':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_70_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_71_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_72_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_73_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_74_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_75_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_76_*.root");
      break;
    case '9':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_80_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_81_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_28_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_29_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_38_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_39_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_48_*.root");
      break;
    case '10':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_58_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_59_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_68_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_69_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_78_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_79_*.root");
      break;
    case '11':
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_17_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_18_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_19_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_49_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_77_*.root");
      c->Add(protocol+dirname+datasetName+"/ntuple_skim_67_*.root");
      break;
    default:
      cout << "Wrong chunk" << endl;
      break;
    }
  }

  return c;

}
