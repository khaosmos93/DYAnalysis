void plotMCtruth() { 

  TString compar = ""; //_4compar";
  TString weights = "_w1";
  TString NOW = "JulyRep";

  gROOT->ProcessLine(".L ../tools/plottools.C");
  gROOT->ProcessLine(".L ../tools/histotools.C");

  TFile *f = new TFile("DYMCtruth2013_"+NOW+compar+weights+".root");
  f->cd();
  //1D
  TH1D* hden_1d = (TH1D*)gDirectory->Get("haccFSR");
  TH1D* hnum_1d = (TH1D*)gDirectory->Get("hsel");
  TH1D* hdenAcc_1d = (TH1D*)gDirectory->Get("hfull");
  TH1D* hnumAcc_1d = (TH1D*)gDirectory->Get("hacc");
  TH1D* hdenPostFSRAcc_1d = (TH1D*)gDirectory->Get("hsim");
  TH1D* hnumPostFSRAcc_1d = (TH1D*)gDirectory->Get("haccFSR");
  TH1D* hdenPostFSRAccEff_1d = (TH1D*)gDirectory->Get("hsim");
  TH1D* hnumPostFSRAccEff_1d = (TH1D*)gDirectory->Get("hsel");

  TH1D* hden_1d_PU = (TH1D*)gDirectory->Get("haccFSR_PU");
  TH1D* hnum_1d_PU = (TH1D*)gDirectory->Get("hsel_PU");
  TH1D* hdenAcc_1d_PU = (TH1D*)gDirectory->Get("hfull_PU");
  TH1D* hnumAcc_1d_PU = (TH1D*)gDirectory->Get("hacc_PU");
  TH1D* hdenPostFSRAcc_1d_PU = (TH1D*)gDirectory->Get("hsim_PU");
  TH1D* hnumPostFSRAcc_1d_PU = (TH1D*)gDirectory->Get("haccFSR_PU");
  TH1D* hdenPostFSRAccEff_1d_PU = (TH1D*)gDirectory->Get("hsim_PU");
  TH1D* hnumPostFSRAccEff_1d_PU = (TH1D*)gDirectory->Get("hsel_PU");

  TH1D* hden_1d_corr = (TH1D*)gDirectory->Get("haccFSR_corr");
  TH1D* hnum_1d_corr = (TH1D*)gDirectory->Get("hsel_corr");
  TH1D* hdenAcc_1d_corr = (TH1D*)gDirectory->Get("hfull_corr");
  TH1D* hnumAcc_1d_corr = (TH1D*)gDirectory->Get("hacc_corr");
  TH1D* hdenPostFSRAcc_1d_corr = (TH1D*)gDirectory->Get("hsim_corr");
  TH1D* hnumPostFSRAcc_1d_corr = (TH1D*)gDirectory->Get("haccFSR_corr");
  TH1D* hdenPostFSRAccEff_1d_corr = (TH1D*)gDirectory->Get("hsim_corr");
  TH1D* hnumPostFSRAccEff_1d_corr = (TH1D*)gDirectory->Get("hsel_corr");

  //pT
  TH1D* hden_pt_1d = (TH1D*)gDirectory->Get("haccFSR_pT");
  TH1D* hnum_pt_1d = (TH1D*)gDirectory->Get("hsel_pT");
  TH1D* hdenAcc_pt_1d = (TH1D*)gDirectory->Get("hfull_pT");
  TH1D* hnumAcc_pt_1d = (TH1D*)gDirectory->Get("hacc_pT");
  TH1D* hdenPostFSRAcc_pt_1d = (TH1D*)gDirectory->Get("hsim_pT");
  TH1D* hnumPostFSRAcc_pt_1d = (TH1D*)gDirectory->Get("haccFSR_pT");
  TH1D* hdenPostFSRAccEff_pt_1d = (TH1D*)gDirectory->Get("hsim_pT");
  TH1D* hnumPostFSRAccEff_pt_1d = (TH1D*)gDirectory->Get("hsel_pT");

  //eta
  TH1D* hden_eta_1d = (TH1D*)gDirectory->Get("haccFSR_Eta");
  TH1D* hnum_eta_1d = (TH1D*)gDirectory->Get("hsel_Eta");
  TH1D* hdenAcc_eta_1d = (TH1D*)gDirectory->Get("hfull_Eta");
  TH1D* hnumAcc_eta_1d = (TH1D*)gDirectory->Get("hacc_Eta");
  TH1D* hdenPostFSRAcc_eta_1d = (TH1D*)gDirectory->Get("hsim_Eta");
  TH1D* hnumPostFSRAcc_eta_1d = (TH1D*)gDirectory->Get("haccFSR_Eta");
  TH1D* hdenPostFSRAccEff_eta_1d = (TH1D*)gDirectory->Get("hsim_Eta");
  TH1D* hnumPostFSRAccEff_eta_1d = (TH1D*)gDirectory->Get("hsel_Eta");

  //pT-Eta
  TH1D* hden_pteta_1d = (TH1D*)gDirectory->Get("haccFSR_pTEta");
  TH1D* hnum_pteta_1d = (TH1D*)gDirectory->Get("hsel_pTEta");
  TH1D* hdenAcc_pteta_1d = (TH1D*)gDirectory->Get("hfull_pTEta");
  TH1D* hnumAcc_pteta_1d = (TH1D*)gDirectory->Get("hacc_pTEta");
  TH1D* hdenPostFSRAcc_pteta_1d = (TH1D*)gDirectory->Get("hsim_pTEta");
  TH1D* hnumPostFSRAcc_pteta_1d = (TH1D*)gDirectory->Get("haccFSR_pTEta");
  TH1D* hdenPostFSRAccEff_pteta_1d = (TH1D*)gDirectory->Get("hsim_pTEta");
  TH1D* hnumPostFSRAccEff_pteta_1d = (TH1D*)gDirectory->Get("hsel_pTEta");

  //create the desired histogram as a function of invariant mass
  TH1D* heff_1d = (TH1D*)hnum_1d->Clone();
  heff_1d->Sumw2();
  hden_1d->Sumw2();
  heff_1d->Divide(hnum_1d,hden_1d);

  TH1D* heff_1d_PU = (TH1D*)hnum_1d_PU->Clone();
  heff_1d_PU->Sumw2();
  hden_1d_PU->Sumw2();
  heff_1d_PU->Divide(hnum_1d_PU,hden_1d_PU);

  TH1D* heff_1d_corr = (TH1D*)hnum_1d_corr->Clone();
  heff_1d_corr->Sumw2();
  hden_1d_corr->Sumw2();
  heff_1d_corr->Divide(hnum_1d_corr,hden_1d_corr);

  //pT
  TH1D* heff_pt_1d = (TH1D*)hnum_pt_1d->Clone();
  heff_pt_1d->Sumw2();
  hden_pt_1d->Sumw2();
  heff_pt_1d->Divide(hnum_pt_1d,hden_pt_1d);

  //eta
  TH1D* heff_eta_1d = (TH1D*)hnum_eta_1d->Clone();
  heff_eta_1d->Sumw2();
  hden_eta_1d->Sumw2();
  heff_eta_1d->Divide(hnum_eta_1d,hden_eta_1d);

  //pT-Eta
  TH1D* heff_pteta_1d = (TH1D*)hnum_pteta_1d->Clone();
  heff_pteta_1d->Sumw2();
  hden_pteta_1d->Sumw2();
  heff_pteta_1d->Divide(hnum_pteta_1d,hden_pteta_1d);


  //Acceptance
  //FIXME rebin
/*
  double mass_xbin[3] = {15,30,1500};
  hdenAcc_1d->Rebin(2,"hdenAcc_1d_new",mass_xbin);
  hnumAcc_1d->Rebin(2,"hnumAcc_1d_new",mass_xbin);
  hdenAcc_1d_corr->Rebin(2,"hdenAcc_1d_corr_new",mass_xbin);
  hnumAcc_1d_corr->Rebin(2,"hnumAcc_1d_corr_new",mass_xbin);

  hdenAcc_1d = howerPointer(hdenAcc_1d, "hdenAcc_1d_new");
  hnumAcc_1d = howerPointer(hnumAcc_1d, "hnumAcc_1d_new");
  hdenAcc_1d_corr = howerPointer(hdenAcc_1d_corr, "hdenAcc_1d_corr_new");
  hnumAcc_1d_corr = howerPointer(hnumAcc_1d_corr, "hnumAcc_1d_corr_new");
*/
  //FIXME end construction

  TH1D* hacc_1d = (TH1D*)hdenAcc_1d->Clone();
  hdenAcc_1d->Sumw2();
  hnumAcc_1d->Sumw2();
  hacc_1d->Divide(hnumAcc_1d,hdenAcc_1d);

  TH1D* hacc_1d_PU = (TH1D*)hdenAcc_1d_PU->Clone();
  hdenAcc_1d_PU->Sumw2();
  hnumAcc_1d_PU->Sumw2();
  hacc_1d_PU->Divide(hnumAcc_1d_PU,hdenAcc_1d_PU);

  TH1D* hacc_1d_corr = (TH1D*)hdenAcc_1d_corr->Clone();
  hdenAcc_1d_corr->Sumw2();
  hnumAcc_1d_corr->Sumw2();
  hacc_1d_corr->Divide(hnumAcc_1d_corr,hdenAcc_1d_corr);

  //pT
  TH1D* hacc_pt_1d = (TH1D*)hdenAcc_pt_1d->Clone();
  hdenAcc_pt_1d->Sumw2();
  hnumAcc_pt_1d->Sumw2();
  hacc_pt_1d->Divide(hnumAcc_pt_1d,hdenAcc_pt_1d);

  //eta
  TH1D* hacc_eta_1d = (TH1D*)hdenAcc_eta_1d->Clone();
  hdenAcc_eta_1d->Sumw2();
  hnumAcc_eta_1d->Sumw2();
  hacc_eta_1d->Divide(hnumAcc_eta_1d,hdenAcc_eta_1d);

  //pT-Eta
  TH1D* hacc_pteta_1d = (TH1D*)hdenAcc_pteta_1d->Clone();
  hdenAcc_pteta_1d->Sumw2();
  hnumAcc_pteta_1d->Sumw2();
  hacc_pteta_1d->Divide(hnumAcc_pteta_1d,hdenAcc_pteta_1d);

  //post FSR acceptance
  TH1D* hpostFSRacc_1d = (TH1D*)hdenPostFSRAcc_1d->Clone();
  hdenPostFSRAcc_1d->Sumw2();
  hnumPostFSRAcc_1d->Sumw2();
  hpostFSRacc_1d->Divide(hnumPostFSRAcc_1d,hdenPostFSRAcc_1d);

  TH1D* hpostFSRacc_1d_PU = (TH1D*)hdenPostFSRAcc_1d_PU->Clone();
  hdenPostFSRAcc_1d_PU->Sumw2();
  hnumPostFSRAcc_1d_PU->Sumw2();
  hpostFSRacc_1d_PU->Divide(hnumPostFSRAcc_1d_PU,hdenPostFSRAcc_1d_PU);

  TH1D* hpostFSRacc_1d_corr = (TH1D*)hdenPostFSRAcc_1d_corr->Clone();
  hdenPostFSRAcc_1d_corr->Sumw2();
  hnumPostFSRAcc_1d_corr->Sumw2();
  hpostFSRacc_1d_corr->Divide(hnumPostFSRAcc_1d_corr,hdenPostFSRAcc_1d_corr);

  //pT
  TH1D* hpostFSRacc_pt_1d = (TH1D*)hdenPostFSRAcc_pt_1d->Clone();
  hdenPostFSRAcc_pt_1d->Sumw2();
  hnumPostFSRAcc_pt_1d->Sumw2();
  hpostFSRacc_pt_1d->Divide(hnumPostFSRAcc_pt_1d,hdenPostFSRAcc_pt_1d);

  //eta
  TH1D* hpostFSRacc_eta_1d = (TH1D*)hdenPostFSRAcc_eta_1d->Clone();
  hdenPostFSRAcc_eta_1d->Sumw2();
  hnumPostFSRAcc_eta_1d->Sumw2();
  hpostFSRacc_eta_1d->Divide(hnumPostFSRAcc_eta_1d,hdenPostFSRAcc_eta_1d);

  //pT-Eta
  TH1D* hpostFSRacc_pteta_1d = (TH1D*)hdenPostFSRAcc_pteta_1d->Clone();
  hdenPostFSRAcc_pteta_1d->Sumw2();
  hnumPostFSRAcc_pteta_1d->Sumw2();
  hpostFSRacc_pteta_1d->Divide(hnumPostFSRAcc_pteta_1d,hdenPostFSRAcc_pteta_1d);


  //post FSR acceptance-efficiency
  TH1D* hpostFSRacceff_1d = (TH1D*)hdenPostFSRAccEff_1d->Clone();
  hdenPostFSRAccEff_1d->Sumw2();
  hnumPostFSRAccEff_1d->Sumw2();
  hpostFSRacceff_1d->Divide(hnumPostFSRAccEff_1d,hdenPostFSRAccEff_1d);

  TH1D* hpostFSRacceff_1d_PU = (TH1D*)hdenPostFSRAccEff_1d_PU->Clone();
  hdenPostFSRAccEff_1d_PU->Sumw2();
  hnumPostFSRAccEff_1d_PU->Sumw2();
  hpostFSRacceff_1d_PU->Divide(hnumPostFSRAccEff_1d_PU,hdenPostFSRAccEff_1d_PU);

  TH1D* hpostFSRacceff_1d_corr = (TH1D*)hdenPostFSRAccEff_1d_corr->Clone();
  hdenPostFSRAccEff_1d_corr->Sumw2();
  hnumPostFSRAccEff_1d_corr->Sumw2();
  hpostFSRacceff_1d_corr->Divide(hnumPostFSRAccEff_1d_corr,hdenPostFSRAccEff_1d_corr);

  //pT
  TH1D* hpostFSRacceff_pt_1d = (TH1D*)hdenPostFSRAccEff_pt_1d->Clone();
  hdenPostFSRAccEff_pt_1d->Sumw2();
  hnumPostFSRAccEff_pt_1d->Sumw2();
  hpostFSRacceff_pt_1d->Divide(hnumPostFSRAccEff_pt_1d,hdenPostFSRAccEff_pt_1d);

  //eta
  TH1D* hpostFSRacceff_eta_1d = (TH1D*)hdenPostFSRAccEff_eta_1d->Clone();
  hdenPostFSRAccEff_eta_1d->Sumw2();
  hnumPostFSRAccEff_eta_1d->Sumw2();
  hpostFSRacceff_eta_1d->Divide(hnumPostFSRAccEff_eta_1d,hdenPostFSRAccEff_eta_1d);

  //pT-Eta
  TH1D* hpostFSRacceff_pteta_1d = (TH1D*)hdenPostFSRAccEff_pteta_1d->Clone();
  hdenPostFSRAccEff_pteta_1d->Sumw2();
  hnumPostFSRAccEff_pteta_1d->Sumw2();
  hpostFSRacceff_pteta_1d->Divide(hnumPostFSRAccEff_pteta_1d,hdenPostFSRAccEff_pteta_1d);


  //
  // Plot all 1D
  //
  //getEff1(heff_1d, false, "mass", ".", 0.5, 1.0);
  //getEff1(hacc_1d, false, "mass", ".", 0.001, 1.0);
  //getEff1(hpostFSRacc_1d, false, "mass", ".", 0.001, 1.0);
  //getEff1(hpostFSRacceff_1d, false, "mass", ".", 0.001, 1.0);
  //getEff1(hacc_1d_corr, false, "mass", ".", 0.001, 1.0);
  //getEff1(hpostFSRacc_1d_corr, false, "mass", ".", 0.001, 1.0);
  //getEff1(hpostFSRacceff_1d_corr, false, "mass", ".", 0.001, 1.0);


  //
  //Save 1D
  //
  TFile *fout1 = new TFile("out1_"+NOW+compar+weights+".root","recreate"); //out1_0713.root","recreate");
  fout1->cd();
  //FIXME not sure which name do you want
  heff_1d->SetName("eff_postFSR");
  heff_1d->Write();
  heff_pt_1d->SetName("eff_postFSR_pT");
  heff_pt_1d->Write();
  heff_eta_1d->SetName("eff_postFSR_Eta");
  heff_eta_1d->Write();
  heff_pteta_1d->SetName("eff_postFSR_pTEta");
  heff_pteta_1d->Write();
  heff_1d_PU->SetName("eff_postFSRPU");
  heff_1d_PU->Write();
  heff_1d_corr->SetName("eff_postFSRcorr");
  heff_1d_corr->Write();
  hacc_1d->SetName("acc_preFSR");
  hacc_1d->Write();
  hacc_pt_1d->SetName("acc_preFSR_pT");
  hacc_pt_1d->Write();
  hacc_pteta_1d->SetName("acc_preFSR_pTEta");
  hacc_pteta_1d->Write();
  hacc_eta_1d->SetName("acc_preFSR_Eta");
  hacc_eta_1d->Write();
  hacc_1d_PU->SetName("acc_preFSRPU");
  hacc_1d_PU->Write();
  hacc_1d_corr->SetName("acc_preFSRcorr");
  hacc_1d_corr->Write();
  hpostFSRacc_1d->SetName("acc_postFSR");
  hpostFSRacc_1d->Write();
  hpostFSRacc_pt_1d->SetName("acc_postFSR_pT");
  hpostFSRacc_pt_1d->Write();
  hpostFSRacc_pteta_1d->SetName("acc_postFSR_pTEta");
  hpostFSRacc_pteta_1d->Write();
  hpostFSRacc_eta_1d->SetName("acc_postFSR_Eta");
  hpostFSRacc_eta_1d->Write();
  hpostFSRacc_1d_PU->SetName("acc_postFSRPU");
  hpostFSRacc_1d_PU->Write();
  hpostFSRacc_1d_corr->SetName("acc_postFSRcorr");
  hpostFSRacc_1d_corr->Write();
  hpostFSRacceff_1d->SetName("acceff_postFSR");
  hpostFSRacceff_1d->Write();
  hpostFSRacceff_pt_1d->SetName("acceff_postFSR_pT");
  hpostFSRacceff_pt_1d->Write();
  hpostFSRacceff_pteta_1d->SetName("acceff_postFSR_pTEta");
  hpostFSRacceff_pteta_1d->Write();
  hpostFSRacceff_eta_1d->SetName("acceff_postFSR_Eta");
  hpostFSRacceff_eta_1d->Write();
  hpostFSRacceff_1d_PU->SetName("acceff_postFSRPU");
  hpostFSRacceff_1d_PU->Write();
  hpostFSRacceff_1d_corr->SetName("acceff_postFSRcorr");
  hpostFSRacceff_1d_corr->Write();
  fout1->Close();

}
