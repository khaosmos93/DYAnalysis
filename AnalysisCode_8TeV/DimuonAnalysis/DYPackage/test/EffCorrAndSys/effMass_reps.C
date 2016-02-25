void effCorrMass_reps(TString CORRTYPE, TString RUNSPLIT, TString WHICHHIST, TString BINNING, string REPLICA, TString PUreweighted) {

  gROOT->ProcessLine(".L ../tools/plottools.C");
  gROOT->ProcessLine(".L ../tools/histotools.C");

  TString SYST = "_";
  if (atoi(REPLICA.c_str()) > 0) {
     SYST = "_syst";
  }

  //Denom
  TFile *fden = new TFile("../EffCorrections/DY_base0116_1.root");
  fden->cd();
  TH1D* hden = (TH1D*)gDirectory->Get(WHICHHIST);
  
  //MC num
  TFile *fnum_mc = new TFile(BINNING+"/"+CORRTYPE+"/DY_mc_"+CORRTYPE+PUreweighted+SYST+TString(REPLICA)+".root");
  fnum_mc->cd();
  TH1D* hnum_mc = (TH1D*)gDirectory->Get(WHICHHIST);
  TH1D* hnum_mc_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");

  cout << BINNING+"/"+CORRTYPE+"/DY_mc_"+CORRTYPE+PUreweighted+SYST+TString(REPLICA)+".root" << endl; 
  cout << WHICHHIST << endl;
  cout << CORRTYPE+PUreweighted << endl;
  cout << RUNSPLIT << endl;

  //Data num
  //FIXME at that moment we do not have a special regime for runABCD
  if (RUNSPLIT == "_runABCD" || RUNSPLIT == "_Debug") {
    cout << "Running in " << RUNSPLIT << " mode" << endl;  
    TFile *fnum_dataAB = new TFile(BINNING+"/"+CORRTYPE+"/DY_data_"+CORRTYPE+RUNSPLIT+SYST+TString(REPLICA)+".root");
    fnum_dataAB->cd();
    TH1D* hnum_dataAB = (TH1D*)gDirectory->Get(WHICHHIST);
    TH1D* hnum_dataAB_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
    //create the desired histogram as a function of invariant mass
    TH1D* h_dataAB = (TH1D*)hnum_dataAB->Clone();
    hnum_dataAB_err->Divide(hnum_dataAB_err,hden);
    //h_dataAB->Sumw2();
    h_dataAB->Divide(hnum_dataAB,hden);
    histoCustomReset(h_dataAB,hnum_dataAB_err);
    //Cleaning
    //for (int i =0; i < h_dataAB->GetXaxis()->GetNbins(); i++) {
    //   if (h_dataAB->GetBinError(i+1) > 2.*h_dataAB->GetBinError(i) 
    //    && h_dataAB->GetBinError(i+1) > 2.*h_dataAB->GetBinError(i+2)) h_dataAB->SetBinError(i+1,(h_dataAB->GetBinError(i)+h_dataAB->GetBinError(i+2))/2.);
    //}
  } else {
  //if (RUNSPLIT == "_split" || RUNSPLIT == "_RunAB" || RUNSPLIT == "_RunC" || RUNSPLIT == "_RunD") { 
    cout << "Running in " << RUNSPLIT << " mode" << endl;

    //lumi weights
    double scale_AB = 0.;
    double scale_C = 0.;
    double scale_D = 0.;

    if (RUNSPLIT == "_split") {
       scale_AB = 0.2791994732119214;
       scale_C = 0.33869611015646672;
       scale_D = 0.38210441663161182;
    } else if (RUNSPLIT == "_RunAB") {
       scale_AB = 1.; 
       scale_C = 0.;
       scale_D = 0.;
    } else if (RUNSPLIT == "_RunC") { 
       scale_AB = 0.;
       scale_C = 1.;
       scale_D = 0.;
    } else if (RUNSPLIT == "_RunD") { 
       scale_AB = 0.;
       scale_C = 0.;
       scale_D = 1.;
    } else {
      cout << "WARNING: wrong label used!"<< endl;
    }

    TFile *fnum_dataAB = new TFile(BINNING+"/"+CORRTYPE+"/DY_data_"+CORRTYPE+"_RunAB.root");
    fnum_dataAB->cd();
    TH1D* hnum_dataAB = (TH1D*)gDirectory->Get(WHICHHIST);
    TH1D* hnum_dataAB_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
    TH1D* h_dataAB = (TH1D*)hnum_dataAB->Clone();
    h_dataAB->Divide(hnum_dataAB,hden);
    hnum_dataAB_err->Divide(hnum_dataAB_err,hden);
    histoCustomReset(h_dataAB,hnum_dataAB_err);

    TFile *fnum_dataC = new TFile(BINNING+"/"+CORRTYPE+"/DY_data_"+CORRTYPE+"_RunC.root");
    fnum_dataC->cd();
    TH1D* hnum_dataC = (TH1D*)gDirectory->Get(WHICHHIST);
    TH1D* hnum_dataC_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
    TH1D* h_dataC = (TH1D*)hnum_dataC->Clone();
    h_dataC->Divide(hnum_dataC,hden);
    hnum_dataC_err->Divide(hnum_dataC_err,hden);
    histoCustomReset(h_dataC,hnum_dataC_err);

    TFile *fnum_dataD = new TFile(BINNING+"/"+CORRTYPE+"/DY_data_"+CORRTYPE+"_RunD.root");
    fnum_dataD->cd();
    TH1D* hnum_dataD = (TH1D*)gDirectory->Get(WHICHHIST);
    TH1D* hnum_dataD_err = (TH1D*)gDirectory->Get(WHICHHIST+"_err");
    TH1D* h_dataD = (TH1D*)hnum_dataD->Clone();
    h_dataD->Divide(hnum_dataD,hden);
    hnum_dataD_err->Divide(hnum_dataD_err,hden);
    histoCustomReset(h_dataD,hnum_dataD_err);
 
    //final histo for split
    TH1D* h_data_split = (TH1D*)h_dataAB->Clone();
    for (int i = 0; i < h_dataAB->GetXaxis()->GetNbins(); i++) {
         h_data_split->SetBinContent(i+1,scale_AB*h_dataAB->GetBinContent(i+1) + scale_C*h_dataC->GetBinContent(i+1) + scale_D*h_dataD->GetBinContent(i+1));
         if (scale_AB == 1) { 
               h_data_split->SetBinError(i+1,h_data_split->GetBinContent(i+1)*sqrt(pow(h_dataAB->GetBinError(i+1)/h_dataAB->GetBinContent(i+1),2)));
         } else if (scale_C == 1) {
               h_data_split->SetBinError(i+1,h_data_split->GetBinContent(i+1)*sqrt(pow(h_dataC->GetBinError(i+1)/h_dataC->GetBinContent(i+1),2)));
         } else if (scale_D == 1) {
               h_data_split->SetBinError(i+1,h_data_split->GetBinContent(i+1)*sqrt(pow(h_dataD->GetBinError(i+1)/h_dataD->GetBinContent(i+1),2)));
         } else {
               h_data_split->SetBinError(i+1,h_data_split->GetBinContent(i+1)*sqrt(pow(scale_AB*h_dataAB->GetBinError(i+1)/h_dataAB->GetBinContent(i+1),2)+pow(scale_C*h_dataC->GetBinError(i+1)/h_dataC->GetBinContent(i+1),2)+ pow(scale_D*h_dataD->GetBinError(i+1)/h_dataD->GetBinContent(i+1),2)));
         }
     }
 
  } //end split run mode

  //create the desired histogram as a function of invariant mass
  TH1D* h_mc = (TH1D*)hnum_mc->Clone();
  //h_mc->Sumw2();
  h_mc->Divide(hnum_mc,hden);
  hnum_mc_err->Divide(hnum_mc_err,hden);
  histoCustomReset(h_mc,hnum_mc_err);
  //Cleaning
  //for (int i =0; i < h_mc->GetXaxis()->GetNbins(); i++) {
  //    if (h_mc->GetBinError(i+1) > 2.*h_mc->GetBinError(i)
  //     && h_mc->GetBinError(i+1) > 2.*h_mc->GetBinError(i+2)) h_mc->SetBinError(i+1,(h_mc->GetBinError(i)+h_mc->GetBinError(i+2))/2.);
  //}


  TString varType = "mass";
  TString dimen = "1D";
  if (WHICHHIST != "invm") {
     varType = "rapidity";
     dimen = "2D";
  } 

  //FIXME remove errors for now
  //removeErrors(h_mc,70.);
  //removeErrors(h_data_split,70.);

  //
  //Save weighted efficiencies
  //
  TFile *feffs = new TFile(BINNING+"/"+CORRTYPE+"/efficiency_"+CORRTYPE+WHICHHIST+RUNSPLIT+PUreweighted+dimen+SYST+TString(REPLICA)+".root","recreate");
  feffs->cd();
  if (RUNSPLIT != "_runABCD" && RUNSPLIT != "_Debug") {
    h_data_split->SetName("data_"+WHICHHIST);
    h_data_split->Write();
  } else {
    h_dataAB->SetName("data_"+WHICHHIST);
    h_dataAB->Write();
  }
  h_mc->Write();
  feffs->Close();

  //
  //Correction
  //
  //create the desired histogram as a function of invariant mass
  TH1D* hrat = (TH1D*)h_mc->Clone();
  //FindRatio(hrat,hrat,hmc);
  //hrat->Sumw2();
  if (RUNSPLIT != "_runABCD" && RUNSPLIT != "_Debug") {
    hrat->SetName(TString(hrat->GetName())+RUNSPLIT+PUreweighted);
    hrat->Divide(h_data_split,h_mc);
  } else {
    hrat->SetName(TString(hrat->GetName())+RUNSPLIT+PUreweighted);
    hrat->Divide(h_dataAB,h_mc);
  }

  //FIXME I am not sure what the file and histo name should be - check in the macro consumer  
  TFile *f = new TFile(BINNING+"/"+CORRTYPE+"/correction_"+CORRTYPE+WHICHHIST+RUNSPLIT+PUreweighted+dimen+SYST+TString(REPLICA)+".root","recreate");
  f->cd();
  hrat->Write();
  f->Close();

  //
  // Plot all 3
  //
  //FIXME we compare something
  //TFile* g = new TFile("../Inputs/acc/out.root");
  //g->cd();
  //TH1D* hmc_truth = (TH1D*)eff_postFSR->Clone();
  //TH1D* hmc_real = histoMultiply2(hmc_truth,hrat);
  //remove errors for now
  //removeErrors(hmc_real,1000.);

  //getEffComp(h_mc, h_data, varType, false, CORRTYPE, 0.8); //TH1D* h1, TH1D* h2, TString varType, bool dataOnly, TString INPUTDIR_PREFIX);
  if (RUNSPLIT != "_runABCD" && RUNSPLIT != "_Debug") {
     getEff3(h_mc, h_data_split, hrat, true, varType, BINNING+"/"+CORRTYPE, 0.7, 1.1); 
 } else {
     //getEff3(h_mc, h_dataAB, hrat, true, varType, BINNING+"/"+CORRTYPE, 0.7, 1.1);
     getEff3(h_mc, h_dataAB, hrat, true, varType, BINNING+"/"+CORRTYPE, 0.7, 1.1);
  }
}
