void mapRMSParser(TString CORRTYPE, TString RUNSPLIT, TString WHICHHIST, const char* REPNO) {

  gROOT->ProcessLine(".L ../tools/plottools.C");
  gROOT->ProcessLine(".L ../tools/histotools.C");

  TFile *fden = new TFile("DYMBase_1andall_TRM.root");
  fden->cd();
  TString pre_hist = WHICHHIST;
  if (WHICHHIST != "invm") pre_hist = "rap";
  TH1D* hden_pre = (TH1D*)gDirectory->Get(pre_hist);
  //FIXME deal with correction starting ehre
  TH1D* hden_init = get2DSlice(hden_pre, WHICHHIST,true);

  int replicas = atoi(REPNO);

  //central replica
  TFile* fi = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+".root");
  cout << "Central   ./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+".root" << endl;
  fi->cd();
  TH1D* central = (TH1D*)gDirectory->Get(WHICHHIST);
  central->Divide(central,hden_init); 

  TH1D* rms = (TH1D*)central->Clone(); 

  cout << "Evaluate uncertainties using " << replicas << " replicas" << endl;
  for (int ir = 0; ir < replicas; ir++) {
    std::ostringstream pprint;
    pprint.str("");
    pprint<<ir+1;
    string label = pprint.str();

    //open recoid, iso and trig files 
    TFile* f = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+"_syst"+TString(label)+".root");
    //cout << WHICHHIST << " Systematics "<< "./binning"+binlabel+"/"+CORRTYPE+"/DY_"+DATA_MC+"_"+CORRTYPE+RUNSPLIT+"_syst"+TString(label)+".root" << endl;
    f->cd();
    TH1D* this_h = (TH1D*)gDirectory->Get(WHICHHIST);
    //FIXME deal with correction starting ehre
    this_h->Divide(this_h,hden_init);

    //calculate quantities
    for (int i = 0; i < rms->GetXaxis()->GetNbins(); i++) {
      if (fabs(this_h->GetBinContent(i+1)-central->GetBinContent(i+1))/central->GetBinContent(i+1) > 0.5) cout << "Mark replica " << ir << " as bad!" << endl;
      if (ir==0) {
        //first replica
        rms->SetBinContent(i+1,pow(this_h->GetBinContent(i+1)-central->GetBinContent(i+1),2)); 
        rms->SetBinError(i+1,0.000001);
      } else { 
        //other replicas
        rms->SetBinContent(i+1,rms->GetBinContent(i+1)+pow(this_h->GetBinContent(i+1)-central->GetBinContent(i+1),2));     
        rms->SetBinError(i+1,0.000001);
      } 
    }
  }
  //postprocessing
  for (int i = 0; i < rms->GetXaxis()->GetNbins(); i++) {
      //cout << "Values  " << i << " " << sqrt(rms->GetBinContent(i+1)/double(replicas))/central->GetBinContent(i+1) << endl;
      //FIXME should be dividing them by unweighted
      rms->SetBinContent(i+1,sqrt(rms->GetBinContent(i+1)/double(replicas)));
  }
  //cout << "WARNING: here we have two option - change the rest of the software to accept absolute errors or to multiply all the bin contents by denominator" << endl;
  //FIXME I want to replace it with the function
  //TFile *fden = new TFile("DYMBase_1andall_TRM.root"); //it is safer that way
  //fden->cd();
  //TString pre_hist = WHICHHIST;
  //if (WHICHHIST != "invm") pre_hist = "rap";
  //TH1D* hden_pre = (TH1D*)gDirectory->Get(pre_hist);
  //TH1D* hden_init = get2DSlice(hden_pre, WHICHHIST);
  //for (int i = 0; i < rms->GetXaxis()->GetNbins(); i++) {
  //  rms->SetBinContent(i+1,(rms->GetBinContent(i+1))*hden_init->GetBinContent(i+1));
  //}

  //then cleanup trash in python config  
  cout << "Write the output " << endl;
  //Write the output
  //fi->Close();
  TFile* fout = new TFile("./outputs/"+CORRTYPE+"/DY_"+CORRTYPE+RUNSPLIT+"_err.root","UPDATE");
  fout->cd();
  rms->SetName(WHICHHIST+"_err");
  rms->Write();
  fout->Close();
}
