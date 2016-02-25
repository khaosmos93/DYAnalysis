void garbageCollector(TString CORRTYPE, TString RUNSPLIT) { 

  //central replica
  cout << "./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+".root" << endl;
  TFile* fi_val = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+".root");
  fi_val->cd();

//migrate the contents
  TH1D* norm = (TH1D*)norm->Clone();
  TH1D* invm = (TH1D*)invm->Clone();
  //TH1D* rap = (TH1D*)rap->Clone();
  //TH1D* rap_err = (TH1D*)rap_err->Clone();
  TH1D* absrap2030 = (TH1D*)absrap2030->Clone();
  TH1D* absrap3045 = (TH1D*)absrap3045->Clone();
  TH1D* absrap4560 = (TH1D*)absrap4560->Clone();
  TH1D* absrap60120 = (TH1D*)absrap60120->Clone();
  TH1D* absrap120200 = (TH1D*)absrap120200->Clone();
  TH1D* absrap200 = (TH1D*)absrap200->Clone();

  //central replica
  TFile* fi_err = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+"_err.root");
  fi_err->cd();

  //how do you know which one are you rewriting??
  TH1D* invm_err = (TH1D*)invm_err->Clone();
  TH1D* absrap2030_err = (TH1D*)absrap2030_err->Clone();
  TH1D* absrap3045_err = (TH1D*)absrap3045_err->Clone();
  TH1D* absrap4560_err = (TH1D*)absrap4560_err->Clone();
  TH1D* absrap60120_err = (TH1D*)absrap60120_err->Clone();
  TH1D* absrap120200_err = (TH1D*)absrap120200_err->Clone();
  TH1D* absrap200_err = (TH1D*)absrap200_err->Clone();


  //then cleanup trash in python config  
  cout << "Write the output " << endl;
  //Write the output
  //fi->Close();
  TFile* fout = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+"_fin.root","RECREATE");
  fout->cd();
  norm->Write();
  invm->Write();
  //rap->Write();
  //rap_err->Write();
  absrap2030->Write();
  absrap3045->Write();
  absrap4560->Write();
  absrap60120->Write();
  absrap120200->Write();
  absrap200->Write();
  invm_err->Write();
  absrap2030_err->Write();
  absrap3045_err->Write();
  absrap4560_err->Write();
  absrap60120_err->Write();
  absrap120200_err->Write();
  absrap200_err->Write();
  fout->Close();
  fi_val->Close();
  fi_err->Close();
}
