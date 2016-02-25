void plotMCtruth_2D(){ //TString NOW) {
  
  TString compar = ""; //_4compar";
  TString weights = "_w1";
  TString NOW = "JulyRep";

  gROOT->ProcessLine(".L ../tools/plottools.C");

  TFile *f = new TFile("DYMCtruth2013_"+NOW+compar+weights+"_2_NODIMU_7CLOSE.root");
  f->cd();
  //2D 
  TH1D* hden_2d = (TH1D*)gDirectory->Get("rap_accFSR");
  TH1D* hnum_2d = (TH1D*)gDirectory->Get("rap_sel");
  TH1D* hdenAcc_2d = (TH1D*)gDirectory->Get("rap_full");
  TH1D* hnumAcc_2d = (TH1D*)gDirectory->Get("rap_acc");
  TH1D* hdenPostFSRAcc_2d = (TH1D*)gDirectory->Get("rap_sim");
  TH1D* hnumPostFSRAcc_2d = (TH1D*)gDirectory->Get("rap_accFSR");
  TH1D* hdenPostFSRAccEff_2d = (TH1D*)gDirectory->Get("rap_sim");
  TH1D* hnumPostFSRAccEff_2d = (TH1D*)gDirectory->Get("rap_sel");

  //2D PU corrected
  TH1D* hden_2d_PU = (TH1D*)gDirectory->Get("rap_accFSR_PU");
  TH1D* hnum_2d_PU = (TH1D*)gDirectory->Get("rap_sel_PU");
  TH1D* hdenAcc_2d_PU = (TH1D*)gDirectory->Get("rap_full_PU");
  TH1D* hnumAcc_2d_PU = (TH1D*)gDirectory->Get("rap_acc_PU");
  TH1D* hdenPostFSRAcc_2d_PU = (TH1D*)gDirectory->Get("rap_sim_PU");
  TH1D* hnumPostFSRAcc_2d_PU = (TH1D*)gDirectory->Get("rap_accFSR_PU");
  TH1D* hdenPostFSRAccEff_2d_PU = (TH1D*)gDirectory->Get("rap_sim_PU");
  TH1D* hnumPostFSRAccEff_2d_PU = (TH1D*)gDirectory->Get("rap_sel_PU");
 

  //2D FEWZ corrected 
  TH1D* hden_2d_corr = (TH1D*)gDirectory->Get("rap_accFSR_corr");
  TH1D* hnum_2d_corr = (TH1D*)gDirectory->Get("rap_sel_corr");
  TH1D* hdenAcc_2d_corr = (TH1D*)gDirectory->Get("rap_full_corr");
  TH1D* hnumAcc_2d_corr = (TH1D*)gDirectory->Get("rap_acc_corr");
  TH1D* hdenPostFSRAcc_2d_corr = (TH1D*)gDirectory->Get("rap_sim_corr");
  TH1D* hnumPostFSRAcc_2d_corr = (TH1D*)gDirectory->Get("rap_accFSR_corr");
  TH1D* hdenPostFSRAccEff_2d_corr = (TH1D*)gDirectory->Get("rap_sim_corr");
  TH1D* hnumPostFSRAccEff_2d_corr = (TH1D*)gDirectory->Get("rap_sel_corr");

  // 
  //2D business
  //
  //create the desired histogram as a function of invariant mass
  TH1D* heff_2d = (TH1D*)hnum_2d->Clone();
  heff_2d->Sumw2();
  hden_2d->Sumw2();
  heff_2d->Divide(hnum_2d,hden_2d);
  //PU
  TH1D* heff_2d_PU = (TH1D*)hnum_2d_PU->Clone();
  heff_2d_PU->Sumw2();
  hden_2d_PU->Sumw2();
  heff_2d_PU->Divide(hnum_2d_PU,hden_2d_PU);

  //corr
  TH1D* heff_2d_corr = (TH1D*)hnum_2d_corr->Clone();
  heff_2d_corr->Sumw2();
  hden_2d_corr->Sumw2();
  heff_2d_corr->Divide(hnum_2d_corr,hden_2d_corr);

  //Acceptance
  TH1D* hacc_2d = (TH1D*)hdenAcc_2d->Clone();
  hdenAcc_2d->Sumw2();
  hnumAcc_2d->Sumw2();
  hacc_2d->Divide(hnumAcc_2d,hdenAcc_2d);
  //PU
  TH1D* hacc_2d_PU = (TH1D*)hdenAcc_2d_PU->Clone();
  hdenAcc_2d_PU->Sumw2();
  hnumAcc_2d_PU->Sumw2();
  hacc_2d_PU->Divide(hnumAcc_2d_PU,hdenAcc_2d_PU);

  //corr
  TH1D* hacc_2d_corr = (TH1D*)hdenAcc_2d_corr->Clone();
  hdenAcc_2d_corr->Sumw2();
  hnumAcc_2d_corr->Sumw2();
  hacc_2d_corr->Divide(hnumAcc_2d_corr,hdenAcc_2d_corr);

  //post FSR acceptance
  TH1D* hpostFSRacc_2d = (TH1D*)hdenPostFSRAcc_2d->Clone();
  hdenPostFSRAcc_2d->Sumw2();
  hnumPostFSRAcc_2d->Sumw2();
  hpostFSRacc_2d->Divide(hnumPostFSRAcc_2d,hdenPostFSRAcc_2d);

  TH1D* hpostFSRacc_2d_PU = (TH1D*)hdenPostFSRAcc_2d_PU->Clone();
  hdenPostFSRAcc_2d_PU->Sumw2();
  hnumPostFSRAcc_2d_PU->Sumw2();
  hpostFSRacc_2d_PU->Divide(hnumPostFSRAcc_2d_PU,hdenPostFSRAcc_2d_PU);

  TH1D* hpostFSRacc_2d_corr = (TH1D*)hdenPostFSRAcc_2d_corr->Clone();
  hdenPostFSRAcc_2d_corr->Sumw2();
  hnumPostFSRAcc_2d_corr->Sumw2();
  hpostFSRacc_2d_corr->Divide(hnumPostFSRAcc_2d_corr,hdenPostFSRAcc_2d_corr);

  //post FSR acceptance-efficiency
  TH1D* hpostFSRacceff_2d = (TH1D*)hdenPostFSRAccEff_2d->Clone();
  hdenPostFSRAccEff_2d->Sumw2();
  hnumPostFSRAccEff_2d->Sumw2();
  hpostFSRacceff_2d->Divide(hnumPostFSRAccEff_2d,hdenPostFSRAccEff_2d);

  TH1D* hpostFSRacceff_2d_PU = (TH1D*)hdenPostFSRAccEff_2d_PU->Clone();
  hdenPostFSRAccEff_2d_PU->Sumw2();
  hnumPostFSRAccEff_2d_PU->Sumw2();
  hpostFSRacceff_2d_PU->Divide(hnumPostFSRAccEff_2d_PU,hdenPostFSRAccEff_2d_PU);

  TH1D* hpostFSRacceff_2d_corr = (TH1D*)hdenPostFSRAccEff_2d_corr->Clone();
  hdenPostFSRAccEff_2d_corr->Sumw2();
  hnumPostFSRAccEff_2d_corr->Sumw2();
  hpostFSRacceff_2d_corr->Divide(hnumPostFSRAccEff_2d_corr,hdenPostFSRAccEff_2d_corr);

  //
  // Plot all 2D
  //
  //getEff1(heff_2d, false, "rapidity", ".", 0.5, 1.0);
  //getEff1(hpostFSRacceff_2d, false, "rapidity", ".", 0.001, 1.0);
  //getEff1(hpostFSRacc_2d, false, "rapidity", ".", 0.001, 1.0);

  //
  //Save 2D
  //
  TFile *fout2 = new TFile("out2_"+NOW+compar+weights+".root","recreate"); //out1_0713.root","recreate");
  fout2->cd();
  heff_2d->SetName("eff_postFSR");
  heff_2d->Write();
  heff_2d_PU->SetName("eff_postFSRPU");
  heff_2d_PU->Write();
  heff_2d_corr->SetName("eff_postFSRcorr");
  heff_2d_corr->Write();
  hacc_2d->SetName("acc_preFSR");
  hacc_2d->Write();
  hacc_2d_PU->SetName("acc_preFSRPU");
  hacc_2d_PU->Write();
  hacc_2d_corr->SetName("acc_preFSRcorr");
  hacc_2d_corr->Write();
  hpostFSRacc_2d->SetName("acc_postFSR");
  hpostFSRacc_2d->Write();
  hpostFSRacc_2d_PU->SetName("acc_postFSRPU");
  hpostFSRacc_2d_PU->Write();
  hpostFSRacc_2d_corr->SetName("acc_postFSRcorr");
  hpostFSRacc_2d_corr->Write();
  hpostFSRacceff_2d->SetName("acceff_postFSR");
  hpostFSRacceff_2d->Write();
  hpostFSRacceff_2d_PU->SetName("acceff_postFSRPU");
  hpostFSRacceff_2d_PU->Write();
  hpostFSRacceff_2d_corr->SetName("acceff_postFSRcorr");
  hpostFSRacceff_2d_corr->Write();
  fout2->Close();

}
