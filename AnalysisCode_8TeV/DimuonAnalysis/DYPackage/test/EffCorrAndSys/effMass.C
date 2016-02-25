void effMass(TString CORRTYPE, TString RUNSPLIT, TString WHICHHIST) {

  gROOT->ProcessLine(".L ../tools/plottools.C");
  gROOT->ProcessLine(".L ../tools/histotools.C");

  //TString CORRTYPE = "iso";
  //TString RUNSPLIT = "_split";
  //TString WHICHHIST = "invm";

  //Denom
  TFile *fden = new TFile("DYMBase_1andall_TRM.root");
  fden->cd();
  TString pre_hist = WHICHHIST;
  if (WHICHHIST != "invm") pre_hist = "rap";
  TH1D* hden_pre = (TH1D*)gDirectory->Get(pre_hist);
  TH1D* hden = get2DSlice(hden_pre, WHICHHIST,true);
  hden->Sumw2();

  cout << "Running in " << RUNSPLIT << " mode " << hden->Integral() << endl;

  //lumi weights
  double scale_AB = 0.;
  double scale_C = 0.;
  double scale_D = 0.;

  TH1D* h_eff;
  TH1D* h_effAB ;
  TH1D* h_effC;
  TH1D* h_effD;
  TH1D* h_eff_split;

/*
RunA          0.89 fb-1
RunB    4.43 fb-1
RunC    7.15 fb-1
RunD    7.27 fb-1
*/
  if (RUNSPLIT == "_RunAB" || RUNSPLIT == "_split") {
       scale_AB = 1.; 
       scale_C = 0.;
       scale_D = 0.;
       TFile *fnum_effAB = new TFile("./EfficienciesTP/"+CORRTYPE+"/DY_"+CORRTYPE+"_RunAB.root");
       fnum_effAB->cd();
       TH1D* hnum_effAB = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_effAB_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_effAB = (TH1D*)hnum_effAB->Clone();
       h_effAB->Divide(hnum_effAB,hden);
       //set error of h_effAB to value of hnum_effAB_err
       histoCustomReset(h_effAB,hnum_effAB_err);
       h_eff_split = (TH1D*)h_effAB->Clone();
  }
  if (RUNSPLIT == "_RunC" || RUNSPLIT == "_split") { 
       scale_AB = 0.;
       scale_C = 1.;
       scale_D = 0.;
       TFile *fnum_effC = new TFile("./EfficienciesTP/"+CORRTYPE+"/DY_"+CORRTYPE+"_RunC.root");
       fnum_effC->cd();
       TH1D* hnum_effC = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_effC_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_effC = (TH1D*)hnum_effC->Clone();
       h_effC->Divide(hnum_effC,hden);
       //set error of h_effAB to value of hnum_effAB_err
       histoCustomReset(h_effC,hnum_effC_err);
       h_eff_split = (TH1D*)h_effC->Clone();
  } 
  if (RUNSPLIT == "_RunD" || RUNSPLIT == "_split") { 
       scale_AB = 0.;
       scale_C = 0.;
       scale_D = 1.;
       TFile *fnum_effD = new TFile("./EfficienciesTP/"+CORRTYPE+"/DY_"+CORRTYPE+"_RunD.root");
       fnum_effD->cd();
       TH1D* hnum_effD = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_effD_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_effD = (TH1D*)hnum_effD->Clone();
       h_effD->Divide(hnum_effD,hden);
       //set error of h_effAB to value of hnum_effAB_err
       histoCustomReset(h_effD,hnum_effD_err);
       h_eff_split = (TH1D*)h_effD->Clone();
  }
  if (RUNSPLIT == "_POWHEG") {
       scale_AB = 1.; 
       scale_C = 0.;
       scale_D = 0.;
       TFile *fnum = new TFile("./EfficienciesTP/"+CORRTYPE+"/DY_"+CORRTYPE+".root");
       fnum->cd();
       TH1D* hnum = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_eff = (TH1D*)hnum->Clone();
       h_eff->Divide(hnum,hden);
       //set error of h_effAB to value of hnum_effAB_err
       histoCustomReset(h_eff,hnum_err);
       h_eff_split = (TH1D*)h_eff->Clone();
  }
  if (RUNSPLIT == "_split") {
       scale_AB = 0.2695; //0.2791994732119214;
       scale_C = 0.3622; //0.33869611015646672;
       scale_D = 0.3682; //0.38210441663161182;
       for (int i = 0; i < h_eff_split->GetXaxis()->GetNbins(); i++) {
           h_eff_split->SetBinContent(i+1,scale_AB*h_effAB->GetBinContent(i+1) + scale_C*h_effC->GetBinContent(i+1) + scale_D*h_effD->GetBinContent(i+1));
           //FIXME that would assume that the error has 0 scaling with statistics, which is not true, so it has to be effected
           h_eff_split->SetBinError(i+1,sqrt(pow(scale_AB*h_effAB->GetBinError(i+1),2)+pow(scale_C*h_effC->GetBinError(i+1),2)+ pow(scale_D*h_effD->GetBinError(i+1),2)));
        } 
   }

  TString varType = "mass";
  TString dimen = "1D";
  if (WHICHHIST != "invm") {
     varType = "rapidity";
     dimen = "2D";
  } 
  //
  //Correction
  //
  //FIXME I am not sure what the file and histo name should be - check in the macro consumer  
  TFile *f = new TFile("./EfficienciesTP/"+CORRTYPE+"/efficiency_"+CORRTYPE+WHICHHIST+RUNSPLIT+dimen+".root","recreate");
  f->cd();
  h_eff_split->Write();
  f->Close();

  //
  // Plot
  //
  //getEff1(h_effAB, true, varType, "./EfficienciesTP/"+CORRTYPE, 0.7, 1.1);
}
