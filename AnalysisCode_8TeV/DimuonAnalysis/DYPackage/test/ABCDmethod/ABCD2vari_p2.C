void ABCD2vari_p2(const char* ISO) { //Only iso is external, all the regions go in a loop!!! to be fed in the macro at once

  
  TString PREFIX = "./";

  //Get the fractions
  //gStyle->SetOptStat(0);
  TFile* fB = new TFile(PREFIX+"factor_qcd_"+TString(ISO)+"_B.root", "open");
  fB->cd();
  TH1D* hRatioB = (TH1D*)gDirectory->Get("hfactor");
  hRatioB->Fit("pol0");
  //2D needs, preliminary
  TH1D* hRatioB2030 = (TH1D*)gDirectory->Get("hfactor2030");
  hRatioB2030->Fit("pol0");
  TH1D* hRatioB3045 = (TH1D*)gDirectory->Get("hfactor3045");
  hRatioB3045->Fit("pol0");
  TH1D* hRatioB4560 = (TH1D*)gDirectory->Get("hfactor4560");
  hRatioB4560->Fit("pol0");
  TH1D* hRatioB60120 = (TH1D*)gDirectory->Get("hfactor60120");
  hRatioB60120->Fit("pol0");
  TH1D* hRatioB120200 = (TH1D*)gDirectory->Get("hfactor120200");
  hRatioB120200->Fit("pol0");
  TH1D* hRatioB2001500 = (TH1D*)gDirectory->Get("hfactor2001500");
  hRatioB2001500->Fit("pol0");

  TF1 *myfitB = (TF1*)hRatioB->GetFunction("pol0");
  Double_t factor_B = myfitB->GetParameter(0);
  Double_t factor_B_err = myfitB->GetParError(0);
  //2D needs, preliminary
  TF1 *myfitB2030 = (TF1*)hRatioB2030->GetFunction("pol0");
  Double_t factor_B2030 = myfitB2030->GetParameter(0);
  Double_t factor_B_err2030 = myfitB2030->GetParError(0);
  TF1 *myfitB3045 = (TF1*)hRatioB3045->GetFunction("pol0");
  Double_t factor_B3045 = myfitB3045->GetParameter(0);
  Double_t factor_B_err3045 = myfitB3045->GetParError(0);
  TF1 *myfitB4560 = (TF1*)hRatioB4560->GetFunction("pol0");
  Double_t factor_B4560 = myfitB4560->GetParameter(0);
  Double_t factor_B_err4560 = myfitB4560->GetParError(0);
  TF1 *myfitB60120 = (TF1*)hRatioB60120->GetFunction("pol0");
  Double_t factor_B60120 = myfitB60120->GetParameter(0);
  Double_t factor_B_err60120 = myfitB60120->GetParError(0);
  TF1 *myfitB120200 = (TF1*)hRatioB120200->GetFunction("pol0");
  Double_t factor_B120200 = myfitB120200->GetParameter(0);
  Double_t factor_B_err120200 = myfitB120200->GetParError(0);
  TF1 *myfitB2001500 = (TF1*)hRatioB2001500->GetFunction("pol0");
  Double_t factor_B2001500 = myfitB2001500->GetParameter(0);
  Double_t factor_B_err2001500 = myfitB2001500->GetParError(0);


  //repeat for region D
  TFile* fD = new TFile(PREFIX+"factor_qcd_"+TString(ISO)+"_D.root", "open");
  fD->cd();
  TH1D* hRatioD = (TH1D*)gDirectory->Get("hfactor");
  hRatioD->Fit("pol0");

  //2D needs, preliminary
  TH1D* hRatioD2030 = (TH1D*)gDirectory->Get("hfactor2030");
  hRatioD2030->Fit("pol0");
  TH1D* hRatioD3045 = (TH1D*)gDirectory->Get("hfactor3045");
  hRatioD3045->Fit("pol0");
  TH1D* hRatioD4560 = (TH1D*)gDirectory->Get("hfactor4560");
  hRatioD4560->Fit("pol0");
  TH1D* hRatioD60120 = (TH1D*)gDirectory->Get("hfactor60120");
  hRatioD60120->Fit("pol0");
  TH1D* hRatioD120200 = (TH1D*)gDirectory->Get("hfactor120200");
  hRatioD120200->Fit("pol0");
  TH1D* hRatioD2001500 = (TH1D*)gDirectory->Get("hfactor2001500");
  hRatioD2001500->Fit("pol0");

  TF1 *myfitD = (TF1*)hRatioD->GetFunction("pol0");
  Double_t factor_D = myfitD->GetParameter(0);
  Double_t factor_D_err = myfitD->GetParError(0);
  //2D needs, preliminary
  TF1 *myfitD2030 = (TF1*)hRatioD2030->GetFunction("pol0");
  Double_t factor_D2030 = myfitD2030->GetParameter(0);
  Double_t factor_D_err2030 = myfitD2030->GetParError(0);
  TF1 *myfitD3045 = (TF1*)hRatioD3045->GetFunction("pol0");
  Double_t factor_D3045 = myfitD3045->GetParameter(0);
  Double_t factor_D_err3045 = myfitB3045->GetParError(0);
  TF1 *myfitD4560 = (TF1*)hRatioD4560->GetFunction("pol0");
  Double_t factor_D4560 = myfitD4560->GetParameter(0);
  Double_t factor_D_err4560 = myfitD4560->GetParError(0);
  TF1 *myfitD60120 = (TF1*)hRatioD60120->GetFunction("pol0");
  Double_t factor_D60120 = myfitD60120->GetParameter(0);
  Double_t factor_D_err60120 = myfitD60120->GetParError(0);
  TF1 *myfitD120200 = (TF1*)hRatioD120200->GetFunction("pol0");
  Double_t factor_D120200 = myfitD120200->GetParameter(0);
  Double_t factor_D_err120200 = myfitD120200->GetParError(0);
  TF1 *myfitD2001500 = (TF1*)hRatioD2001500->GetFunction("pol0");
  Double_t factor_D2001500 = myfitD2001500->GetParameter(0);
  Double_t factor_D_err2001500 = myfitD2001500->GetParError(0);

  //Run the step2
  TString protocol = "file://";
  TString dirname = "/scratch/scratch95/a/asvyatko/DY2013/rootfiles/";

//   DY
  TFileCollection* c2a = new TFileCollection("DYM1020","DYM1020");
  c2a->Add(protocol+dirname+"DYM1020_PU"+"/*.root");

  TFileCollection* c2b = new TFileCollection("DYM20","DYM20");
  c2b->Add(protocol+dirname+"DYM20_PU"+"/*.root");

  TFileCollection* c2c = new TFileCollection("DYM200","DYM200");
  c2c->Add(protocol+dirname+"DYM200_PU"+"/*.root");

  TFileCollection* c2d = new TFileCollection("DYM400","DYM400");
  c2d->Add(protocol+dirname+"DYM400_PU"+"/*.root");

  TFileCollection* c2e = new TFileCollection("DYM500","DYM500");
  c2e->Add(protocol+dirname+"DYM500_PU"+"/*.root");

  TFileCollection* c2f = new TFileCollection("DYM700","DYM700");
  c2f->Add(protocol+dirname+"DYM700_PU"+"/*.root");

  TFileCollection* c2g = new TFileCollection("DYM800","DYM800");
  c2g->Add(protocol+dirname+"DYM800_PU"+"/*.root");

  TFileCollection* c2h = new TFileCollection("DYM1000","DYM1000");
  c2h->Add(protocol+dirname+"DYM1000_PU"+"/*.root");

//DYtau
  TFileCollection* c5a = new TFileCollection("DYtautau1020","DYtautau1020");
  c5a->Add(protocol+dirname+"DYtautau1020_PU"+"/*.root");

  TFileCollection* c5b = new TFileCollection("DYtautau20","DYtautau20");
  c5b->Add(protocol+dirname+"DYtautau20_PU"+"/*.root");

  gEnv->SetValue("ProofLite.Sandbox", "/home/ba01/u115/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ABCDmethod/proofbox_p2");
  TProof* p = TProof::Open("workers=10"); //"workers=1"); ////proof://pccmspurdue3:-1"); // set number of works to 2:  TProof::Open("//lite:///?workers=2");
  p->RegisterDataSet("DYM1020", c2a,"OV");
  p->RegisterDataSet("DYM20", c2b,"OV");
  p->RegisterDataSet("DYM200", c2c,"OV");
  p->RegisterDataSet("DYM400", c2d,"OV");
  p->RegisterDataSet("DYM500", c2e,"OV");
  p->RegisterDataSet("DYM700", c2f,"OV");
  p->RegisterDataSet("DYM800", c2g,"OV");
  p->RegisterDataSet("DYM1000", c2h,"OV");
  p->RegisterDataSet("DYtautau1020", c5a,"OV");
  p->RegisterDataSet("DYtautau20", c5b,"OV");

  p->ShowDataSets();

  //FIXME try not to ue the class, just define the parameters you need and load them with AdInoput
  //this is correct
  TObjString* useNtupleWeightFlag = new TObjString("False");
  p->AddInput(new TNamed("useNtupleWeightFlag","False"));
  TObjString* iso = new TObjString(ISO); 
  p->AddInput(new TNamed("iso",ISO));
  gROOT->ProcessLine(".L ../tools/misctools.C");
  TObjString* factorB = new TObjString(fota(factor_B));
  p->AddInput(new TNamed("factorB",fota(factor_B)));
  TObjString* factorB_err = new TObjString(fota(factor_B_err));
  p->AddInput(new TNamed("factorB_err",fota(factor_B_err)));

  TObjString* factorB2030 = new TObjString(fota(factor_B2030));
  p->AddInput(new TNamed("factorD2030",fota(factor_B2030)));
  TObjString* factorB_err2030 = new TObjString(fota(factor_B_err2030));
  p->AddInput(new TNamed("factorB_err2030",fota(factor_B_err2030)));
  TObjString* factorB3045 = new TObjString(fota(factor_B3045));
  p->AddInput(new TNamed("factorB3045",fota(factor_B3045)));
  TObjString* factorB_err3045 = new TObjString(fota(factor_B_err3045));
  p->AddInput(new TNamed("factorB_err3045",fota(factor_B_err3045)));
  TObjString* factorB4560 = new TObjString(fota(factor_B4560));
  p->AddInput(new TNamed("factorB4560",fota(factor_B4560)));
  TObjString* factorB_err4560 = new TObjString(fota(factor_B_err4560));
  p->AddInput(new TNamed("factorB_err4560",fota(factor_B_err4560)));
  TObjString* factorB60120 = new TObjString(fota(factor_B60120));
  p->AddInput(new TNamed("factorB60120",fota(factor_B60120)));
  TObjString* factorB_err60120 = new TObjString(fota(factor_B_err60120));
  p->AddInput(new TNamed("factorB_err60120",fota(factor_B_err60120)));
  TObjString* factorB120200 = new TObjString(fota(factor_B120200));
  p->AddInput(new TNamed("factorB120200",fota(factor_B120200)));
  TObjString* factorB_err120200 = new TObjString(fota(factor_B_err120200));
  p->AddInput(new TNamed("factorB_err120200",fota(factor_B_err120200)));
  TObjString* factorB2001500 = new TObjString(fota(factor_B2001500));
  p->AddInput(new TNamed("factorB2001500",fota(factor_B2001500)));
  TObjString* factorB_err2001500 = new TObjString(fota(factor_B_err2001500));
  p->AddInput(new TNamed("factorB_err2001500",fota(factor_B_err2001500)));

  /*TObjString* factorC = new TObjString(fota(factor_C));
  p->AddInput(new TNamed("factorC",fota(factor_C)));
  TObjString* factorC_err = new TObjString(fota(factor_C_err));
  p->AddInput(new TNamed("factorC_err",fota(factor_C_err)));
  */
  TObjString* factorD = new TObjString(fota(factor_D));
  p->AddInput(new TNamed("factorD",fota(factor_D)));
  TObjString* factorD_err = new TObjString(fota(factor_D_err));
  p->AddInput(new TNamed("factorD_err",fota(factor_D_err)));

  TObjString* factorD2030 = new TObjString(fota(factor_D2030));
  p->AddInput(new TNamed("factorD2030",fota(factor_D2030)));
  TObjString* factorD_err2030 = new TObjString(fota(factor_D_err2030));
  p->AddInput(new TNamed("factorD_err2030",fota(factor_D_err2030)));
  TObjString* factorD3045 = new TObjString(fota(factor_D3045));
  p->AddInput(new TNamed("factorD3045",fota(factor_D3045)));
  TObjString* factorD_err3045 = new TObjString(fota(factor_D_err3045));
  p->AddInput(new TNamed("factorD_err3045",fota(factor_D_err3045)));
  TObjString* factorD4560 = new TObjString(fota(factor_D4560));
  p->AddInput(new TNamed("factorD4560",fota(factor_D4560)));
  TObjString* factorD_err4560 = new TObjString(fota(factor_D_err4560));
  p->AddInput(new TNamed("factorD_err4560",fota(factor_D_err4560)));
  TObjString* factorD60120 = new TObjString(fota(factor_D60120));
  p->AddInput(new TNamed("factorD60120",fota(factor_D60120)));
  TObjString* factorD_err60120 = new TObjString(fota(factor_D_err60120));
  p->AddInput(new TNamed("factorD_err60120",fota(factor_D_err60120)));
  TObjString* factorD120200 = new TObjString(fota(factor_D120200));
  p->AddInput(new TNamed("factorD120200",fota(factor_D120200)));
  TObjString* factorD_err120200 = new TObjString(fota(factor_D_err120200));
  p->AddInput(new TNamed("factorD_err120200",fota(factor_D_err120200)));
  TObjString* factorD2001500 = new TObjString(fota(factor_D2001500));
  p->AddInput(new TNamed("factorD2001500",fota(factor_D2001500)));
  TObjString* factorD_err2001500 = new TObjString(fota(factor_D_err2001500));
  p->AddInput(new TNamed("factorD_err2001500",fota(factor_D_err2001500)));

  gROOT->Time();
  p->SetParameter("PROOF_LookupOpt", "all");
  p->Process("DYM1020#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM20#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM200#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM400#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM500#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM700#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM800#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYM1000#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYtautau1020#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");
  p->Process("DYtautau20#/recoTree/DiMuonTree","EventSelector_Bkg2.C+");

}
