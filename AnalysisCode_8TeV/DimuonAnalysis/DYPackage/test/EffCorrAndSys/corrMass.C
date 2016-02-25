void corrMass(TString CORRTYPE, TString RUNSPLIT, TString WHICHHIST) {

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

  cout << "Running in " << RUNSPLIT << " mode" << endl;
  //lumi weights
  double scale_AB = 0.;
  double scale_C = 0.;
  double scale_D = 0.;

  TH1D* h_corrAB ;
  TH1D* h_corrC;
  TH1D* h_corrD;
  TH1D* h_corr_split;

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
       TFile *fnum_corrAB = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+"_RunAB.root");
       fnum_corrAB->cd();
       TH1D* hnum_corrAB = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_corrAB_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_corrAB = (TH1D*)hnum_corrAB->Clone();
       h_corrAB->Divide(hnum_corrAB,hden);
       //set error of h_corrAB to value of hnum_corrAB_err
       histoCustomReset(h_corrAB,hnum_corrAB_err);
       h_corr_split = (TH1D*)h_corrAB->Clone();
  }
  if (RUNSPLIT == "_RunC" || RUNSPLIT == "_split") { 
       scale_AB = 0.;
       scale_C = 1.;
       scale_D = 0.;
       TFile *fnum_corrC = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+"_RunC.root");
       fnum_corrC->cd();
       TH1D* hnum_corrC = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_corrC_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_corrC = (TH1D*)hnum_corrC->Clone();
       h_corrC->Divide(hnum_corrC,hden);
       //set error of h_corrAB to value of hnum_corrAB_err
       histoCustomReset(h_corrC,hnum_corrC_err);
       h_corr_split = (TH1D*)h_corrC->Clone();
  } 
  if (RUNSPLIT == "_RunD" || RUNSPLIT == "_split") { 
       scale_AB = 0.;
       scale_C = 0.;
       scale_D = 1.;
       TFile *fnum_corrD = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+"_RunD.root");
       fnum_corrD->cd();
       TH1D* hnum_corrD = (TH1D*)gDirectory->Get(WHICHHIST);
       TH1D* hnum_corrD_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
       h_corrD = (TH1D*)hnum_corrD->Clone();
       h_corrD->Divide(hnum_corrD,hden);
       //set error of h_corrAB to value of hnum_corrAB_err
       histoCustomReset(h_corrD,hnum_corrD_err);
       h_corr_split = (TH1D*)h_corrD->Clone();
  }
  if (RUNSPLIT == "_split") {
       scale_AB = 0.2695; //0.2791994732119214;
       scale_C = 0.3622; //0.33869611015646672;
       scale_D = 0.3682; //0.38210441663161182;
       for (int i = 0; i < h_corr_split->GetXaxis()->GetNbins(); i++) {
           h_corr_split->SetBinContent(i+1,scale_AB*h_corrAB->GetBinContent(i+1) + scale_C*h_corrC->GetBinContent(i+1) + scale_D*h_corrD->GetBinContent(i+1));
           //FIXME that would assume that the error has 0 scaling with statistics, which is not true, so it has to be corrected
           h_corr_split->SetBinError(i+1,sqrt(pow(scale_AB*h_corrAB->GetBinError(i+1),2)+pow(scale_C*h_corrC->GetBinError(i+1),2)+ pow(scale_D*h_corrD->GetBinError(i+1),2)));
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
  TFile *f = new TFile("./outputs/"+CORRTYPE+"/correction_"+CORRTYPE+WHICHHIST+RUNSPLIT+dimen+".root","recreate");
  f->cd();
  h_corr_split->Write();
  f->Close();

  //
  // Plot
  //
  //getEff1(h_corrAB, true, varType, "./outputs/"+CORRTYPE, 0.7, 1.1);
}
