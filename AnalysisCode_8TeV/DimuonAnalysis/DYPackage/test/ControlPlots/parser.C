void fillQCD(TH1D* hqcd);

void parser(TString TYPE) {

   TString INPUT_PREFIX = "../runfolder/";
   plot(TYPE,INPUT_PREFIX);

}

void plot(char* h, TString prefix) {

    TFile f1(prefix+"DATA_CP.root", "read");
    TH1D* hdata = (TH1D*)f1.Get(h);
    hdata->SetDirectory(0);

    //read unnormalized histograms for MC
    TFile f2(prefix+"DYM_CP.root", "read");
    TH1D* hsignal = (TH1D*)f2.Get(TString(h));
    hsignal->SetDirectory(0);

    TFile f4_mc(prefix+"TT_CP.root", "read");
    TH1D* httbar_mc = (TH1D*)f4_mc.Get(h);
    httbar_mc->SetDirectory(0);

    TFile f5(prefix+"TAU_CP.root", "read");
    TH1D* hZtautau = (TH1D*)f5.Get(h);
    hZtautau->SetDirectory(0);
    
    TFile f6(prefix+"EWK_CP.root", "read");
    TH1D* hWleptonnu = (TH1D*)f6.Get(h);
    hWleptonnu->SetDirectory(0);
    
    TFile f7(prefix+"DIBOSON_CP.root", "read");
    TH1D* hDibosons = (TH1D*)f7.Get(h);
    hDibosons->SetDirectory(0);

    TH1D* hqcd = (TH1D*)hDibosons->Clone();
    fillQCD(hqcd);
   
    //normalized them to data in the Z peak
 
    const double ttbarNorm = httbar_mc->Integral();
    const double WleptonnuNorm = hWleptonnu->Integral();
    const double ZtautauNorm = hZtautau->Integral();
    const double qcdNorm = hqcd->Integral();
    const double dibosonNorm = hDibosons->Integral();
    const double signalNorm = hsignal->Integral();
    double fullMCintegral = ttbarNorm+WleptonnuNorm+ZtautauNorm+qcdNorm+dibosonNorm+signalNorm;

    TH1D* hWleptonnu_copy = (TH1D*)hWleptonnu->Clone();
    TH1D* hZtautau_copy = (TH1D*)hZtautau->Clone();
    TH1D* hDibosons_copy = (TH1D*)hDibosons->Clone();
    TH1D* hqcd_copy = (TH1D*)hqcd->Clone();
    TH1D* hsignal_copy = (TH1D*)hsignal->Clone();

    httbar_mc->Scale(hdata->Integral()/fullMCintegral);
    hWleptonnu->Scale(hdata->Integral()/fullMCintegral);
    hZtautau->Scale(hdata->Integral()/fullMCintegral);
    hqcd->Scale(hdata->Integral()/fullMCintegral);
    hsignal->Scale(hdata->Integral()/fullMCintegral);
    hDibosons->Scale(hdata->Integral()/fullMCintegral);

    normalizeToPeak(httbar_mc, hWleptonnu, hqcd, hZtautau, hsignal, hDibosons, hdata);

    std::ostringstream pprint;

//save important histograms
    TFile* f = new TFile("runfolder/stack_"+TString(h)+"CP.root","recreate");
    f->cd();
    hsignal->SetName("hsig_fewz");
    hsignal->SetTitle("hsig_fewz");
    hsignal->Write();

    httbar_mc->SetName("httbar_mc");
    httbar_mc->SetTitle("httbar_mc");
    httbar_mc->Write();
    
    hZtautau->SetName("hDYtautau");
    hZtautau->SetTitle("hDYtautau");
    hZtautau->Write();
    
    hWleptonnu->SetName("hWlepton");
    hWleptonnu->SetTitle("hWlepton");
    hWleptonnu->Write();
    
    hDibosons->SetName("hdiboson");
    hDibosons->SetTitle("hdiboson");
    hDibosons->Write();
    
    hqcd->SetName("hqcd");
    hqcd->SetTitle("hqcd");
    hqcd->Write();
  
    hdata->SetName("hdata");
    hdata->SetTitle("hdata");
    hdata->Write();
    f->Close();
}

double Round(double d) {
    double nd = (double) floor(d*10.)/10.;
    return nd;
}

void FindRatio(TH1D* ratio, TH1D* numer, TH1D* denom) {
   for (int ibin = 0; ibin < numer->GetXaxis()->GetNbins()+1;ibin++) {
       if (denom->GetBinContent(ibin+1) != 0) {
        double rat = (double)(numer->GetBinContent(ibin+1)/denom->GetBinContent(ibin+1));
        ratio->SetBinContent(ibin+1,rat);
        ratio->SetBinError(ibin+1,sqrt(numer->GetBinContent(ibin+1))/denom->GetBinContent(ibin+1) - numer->GetBinContent(ibin+1)*sqrt(denom->GetBinContent(ibin+1))/pow(denom->GetBinContent(ibin+1),2));
        } else {
        ratio->SetBinContent(ibin+1,0.);
       }
  }
}


void resetHisto(TH1D* h) {

   for (int i = 0; i < h->GetXaxis()->GetNbins();i++) {
      h->SetBinContent(i+1,0.0);
      h->SetBinError(i+1,0.0);
   }

}

void normalizeToPeak(TH1D* hmc1, TH1D* hmc2, TH1D* hmc3, TH1D* hmc4, TH1D* hmc5, TH1D* hmc6, TH1D* hdata) {
   double this_mc = hmc1->Integral(10,22)+hmc2->Integral(10,22)+hmc3->Integral(10,22)+hmc4->Integral(10,22)+hmc5->Integral(10,22)+hmc6->Integral(10,22);
   double this_data = hdata->Integral(10,22);
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc1->Scale(this_data/this_mc);
      hmc2->Scale(this_data/this_mc);
      hmc3->Scale(this_data/this_mc);
      hmc4->Scale(this_data/this_mc);
      hmc5->Scale(this_data/this_mc);
      hmc6->Scale(this_data/this_mc);
      this_mc = hmc1->Integral(10,22)+hmc2->Integral(10,22)+hmc3->Integral(10,22)+hmc4->Integral(10,22)+hmc5->Integral(10,22)+hmc6->Integral(10,22);
   }
}



void fillQCD(TH1D* hqcd) {

 double values[41] = {0.06994,0.120086,0.163339,0.291288,0.399186,0.390006,0.33735,0.279662,0.2608,0.241937,0.215055,0.188174,0.161292,0.13441,0.107528,0.0806458,0.0672048,0.0577962,0.0537639,0.0470434,0.0421509,0.0283915,0.0253535,0.0196811,0.0246472,0.0190043,0.0134009,0.0146824,0.0120834,0.0118611,0.00819394,0.00594396,0.00491986,0.00235569,0.00307031,0.00203922,0.00100814,0.00047047,0.000376376,0.000263463,0.000158078};

 double errors[41] = {0.00323898,0.00424415,0.00494983,0.00661008,0.00773808,0.00764859,0.00711355,0.00647683,0.0062546,0.00602417,0.00567964,0.00531282,0.00491871,0.00449015,0.00401611,0.00347806,0.00317502,0.00294439,0.00283982,0.00265641,0.00251449,0.00206367,0.00195013,0.00171819,0.00192278,0.00168839,0.00141779,0.00148403,0.0013463,0.00133386,0.00110864,0.000944243,0.000859057,0.000594436,0.000678635,0.000553067,0.000388871,0.000265651,0.000212521,0.000148765,8.92587e-05};

 for (int i =0; i < hqcd->GetNbinsX(); i++) {
   hqcd->SetBinContent(i+1,values[i]);
   hqcd->SetBinError(i+1,errors[i]);
 } 

}
