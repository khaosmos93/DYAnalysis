void parser_2D(TString TYPE) {

   TString INPUT_PREFIX = "../runfolder/";
   bool norm2data = true;
   plot(TYPE,norm2data,INPUT_PREFIX);

}

void plot(char* h, bool norm2data, TString prefix) {

    TFile* f1 = new TFile(prefix+"DATA_CP.root");
    f1->cd();
    TH1D* hdata = (TH1D*)gDirectory->Get(TString(h));
    
    TFile* f2 = new TFile(prefix+"DYM_CP.root", "read");
    f2->cd();
    TH1D* hsignal = (TH1D*)gDirectory->Get(TString(h));
    
    TFile* f4_mc = new TFile(prefix+"TT_CP.root", "read");
    f4_mc->cd();
    TH1D* httbar = (TH1D*)gDirectory->Get(TString(h));

    TFile* f5 = new TFile(prefix+"TAU_CP.root", "read");
    f5->cd();
    TH1D* hZtautau = (TH1D*)gDirectory->Get(TString(h));
    
    TFile* f6 = new TFile(prefix+"EWK_CP.root", "read");
    f6->cd();
    TH1D* hWleptonnu = (TH1D*)gDirectory->Get(TString(h));
    
    TFile* f7 = new TFile(prefix+"DIBOSON_CP.root", "read");
    f7->cd();
    TH1D* hDibosons = (TH1D*)gDirectory->Get(TString(h));
  
    //QCD
    TH1D* hqcd = (TH1D*)hDibosons->Clone();
    fillQCD(hqcd);
 
    // signal
    hsignal->SetLineColor(kOrange);
    hsignal->SetFillColor(kOrange);
    
    // qcd
    hqcd->SetLineColor(9);
    hqcd->SetFillColor(9);

    // tt
    httbar->SetLineColor(kRed+2);
    httbar->SetFillColor(kRed+2);

    // tau
    hZtautau->SetLineColor(kGreen);
    hZtautau->SetFillColor(kGreen);

    // EWK
    hWleptonnu->SetLineColor(38);
    hWleptonnu->SetFillColor(38);

    // diboson
    hDibosons->SetLineColor(40);
    hDibosons->SetFillColor(40);

    //
    //The histograms come properly weighted with priors
    //This would normalize to number of events in data
    //
    const double ttbarNorm = httbar->Integral();
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

    httbar->Scale(hdata->Integral()/fullMCintegral);
    hWleptonnu->Scale(hdata->Integral()/fullMCintegral);
    hZtautau->Scale(hdata->Integral()/fullMCintegral);
    hqcd->Scale(hdata->Integral()/fullMCintegral);
    hsignal->Scale(hdata->Integral()/fullMCintegral);
    hDibosons->Scale(hdata->Integral()/fullMCintegral);

    normalizeToPeak(httbar, hWleptonnu, hqcd, hZtautau, hsignal, hDibosons, hdata);


    //add to stack
    TH1D* hstack4fit = (TH1D*)hsignal->Clone();
    hstack4fit->Add(hWleptonnu);
    hstack4fit->Add(hDibosons);
    hstack4fit->Add(httbar);
    hstack4fit->Add(hZtautau);
    hstack4fit->Add(hqcd);

//save important histograms
    TFile* f = new TFile("runfolder/stack_"+TString(h)+"CP.root","recreate");
    f->cd();

    hsignal->SetName("hsig_fewz");
    hsignal->SetTitle("hsig_fewz");
    hsignal->Write();
    hqcd->SetName("hqcd");
    hqcd->SetTitle("hqcd");
    hqcd->Write();
    hZtautau->SetName("hDYtautau");
    hZtautau->SetTitle("hDYtautau");
    hZtautau->Write();
    hWleptonnu->SetName("hWlepton");
    hWleptonnu->SetTitle("hWlepton");
    hWleptonnu->Write();
    hDibosons->SetName("hdiboson");
    hDibosons->SetTitle("hdiboson");
    hDibosons->Write();
    httbar->SetName("httbar_mc");
    httbar->SetTitle("httbar_mc");
    httbar->Write();
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
   int from  = 73;
   int to = 96;
   if (hmc1==156) {
     from  = 73+24;
     to = 96+24;
   }
   double this_mc = hmc1->Integral(from,to)+hmc2->Integral(from,to)+hmc3->Integral(from,to)+hmc4->Integral(from,to)+hmc5->Integral(from,to)+hmc6->Integral(from,to);
   double this_data = hdata->Integral(from,to);
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc1->Scale(this_data/this_mc);
      hmc2->Scale(this_data/this_mc);
      hmc3->Scale(this_data/this_mc);
      hmc4->Scale(this_data/this_mc);
      hmc5->Scale(this_data/this_mc);
      hmc6->Scale(this_data/this_mc);
      this_mc = hmc1->Integral(from,to)+hmc2->Integral(from,to)+hmc3->Integral(from,to)+hmc4->Integral(from,to)+hmc5->Integral(from,to)+hmc6->Integral(from,to);
   }
}


// This replaces the 132 bin histogram with 168, adding 24 rapidity bins at low mass and 12 at high mass.
// It sets the initial 132 bin contents to the ones starting bin number #24
// Here underflow and overflow histograms simply 24 and 12 bin histos correspondingly which are filled for 0-20 GeV mass and 1500-10000 GeV mass
TH1D* rebinWithOU1_2D(TH1D* h_initial, TH1D* underflow) { //, TH1D* overflow) {
        //
        //switch to under-overlow inclusion will be provided by changing the matrix
        // 
        //FIXME right now the final binning is hard coded, and depends on the initial binning
        const int nbins = 156;
        TH1D* h_ou = new TH1D(h_initial->GetName(),h_initial->GetName(),nbins,0,nbins);
        //fill underflow
        for (int i = 0; i < underflow->GetXaxis()->GetNbins(); i++) {
             h_ou->SetBinContent(i+1,underflow->GetBinContent(i+1));
             h_ou->SetBinError(i+1,underflow->GetBinError(i+1));
        }       
        //fill main histogram
        for (int i = 0; i < h_initial->GetXaxis()->GetNbins(); i++) {
             h_ou->SetBinContent(i+1+24,h_initial->GetBinContent(i+1));
             h_ou->SetBinError(i+1+24,h_initial->GetBinError(i+1));
        }
        //fill underflow
        //for (int i = 0; i < overflow->GetXaxis()->GetNbins(); i++) {
        //     h_ou->SetBinContent(i+1+132+24,overflow->GetBinContent(i+1));
        //     h_ou->SetBinError(i+1+132+24,overflow->GetBinError(i+1));
        //}

        return h_ou;

}


void fillQCD(TH1D* hqcd) {

 double values[132] = {0.0226694, 0.0226483, 0.0226316, 0.0226236, 0.0226249, 0.0226244, 0.022595, 0.0224971, 0.022291, 0.0219528, 0.0214836, 0.0209083, 0.0202653, 0.0195932, 0.0189176, 0.0182386, 0.0175167, 0.0166612, 0.015531, 0.0139595, 0.0118067, 0.00902266, 0.00569513, 0.00205166, 0.0642255, 0.0641903, 0.0641835, 0.0642262, 0.0643205, 0.064439, 0.0645203, 0.0644817, 0.0642516, 0.0638087, 0.0632094, 0.0625788, 0.0620566, 0.0617122, 0.0614598, 0.0610134, 0.0599068, 0.0575807, 0.0535112, 0.0473399, 0.0389703, 0.0286067, 0.0167343, 0, 0.0592302, 0.0587466, 0.0583802, 0.0582077, 0.0582376, 0.0584031, 0.0585719, 0.0585652, 0.0581753, 0.0571834, 0.0553807, 0.052603, 0.0487786, 0.0439756, 0.0384287, 0.0325233, 0.0267328, 0.0215262, 0.0172732, 0.014178, 0.0106591, 0.0075741, 0.004484, 0, 0.042594, 0.0424548, 0.0424325, 0.0425955, 0.0429295, 0.0433281, 0.0436175, 0.0436125, 0.0431895, 0.0423491, 0.0412392, 0.0401159, 0.0392455, 0.038782, 0.03867, 0.0386256, 0.0382117, 0.0369838, 0.0346419, 0.0311207, 0.0265801, 0.0213111, 0.0156182, 0, 0.00903409, 0.0105617, 0.0117926, 0.0125215, 0.01269, 0.0123839, 0.0117796, 0.011066, 0.0103764, 0.00975673, 0.00917594, 0.00856634, 0.00786904, 0.00706419, 0.00617626, 0.0052589, 0.0043718, 0.00356204, 0.00285605, 0.0022608, 0.00176914, 0.0013643, 0.00102237, 0, 0.00774523, 0.00716492, 0.00652372, 0.00578856, 0.00496861, 0.00411003, 0.00327525, 0.00251821, 0.0018671, 0.00132032, 0.000853162, 0};

 double errors[132] = {0.00184402, 0.00184316, 0.00184248, 0.00184216, 0.00184221, 0.00184219, 0.00184099, 0.001837, 0.00182857, 0.00181464, 0.00179514, 0.00177094, 0.0017435, 0.00171435, 0.00168453, 0.00165402, 0.00162096, 0.00158088, 0.00152632, 0.00144704, 0.00133079, 0.00116336, 0.000924267, 0.000554752, 0.00310384, 0.00310299, 0.00310283, 0.00310386, 0.00310614, 0.003109, 0.00311096, 0.00311003, 0.00310447, 0.00309375, 0.00307919, 0.00306379, 0.00305098, 0.0030425, 0.00303628, 0.00302523, 0.00299767, 0.0029389, 0.00283314, 0.00266477, 0.00241776, 0.00207147, 0.00158434, 0, 0.00298069, 0.0029685, 0.00295923, 0.00295485, 0.00295561, 0.00295981, 0.00296408, 0.00296391, 0.00295403, 0.00292874, 0.0028822, 0.002809, 0.00270496, 0.00256833, 0.0024009, 0.00220873, 0.00200248, 0.00179692, 0.00160965, 0.00145832, 0.00126446, 0.00106589, 0.000820122, 0, 0.00252767, 0.00252353, 0.00252287, 0.00252771, 0.0025376, 0.00254936, 0.00255786, 0.00255771, 0.00254527, 0.00252039, 0.00248714, 0.00245304, 0.00242628, 0.00241191, 0.00240842, 0.00240704, 0.00239411, 0.00235533, 0.00227954, 0.00216058, 0.00199675, 0.00178792, 0.0015306, 0, 0.00116409, 0.00125867, 0.00132999, 0.00137048, 0.00137967, 0.00136293, 0.00132926, 0.00128837, 0.00124758, 0.00120976, 0.0011732, 0.00113356, 0.00108644, 0.00102938, 0.000962517, 0.000888164, 0.000809796, 0.000730962, 0.000654528, 0.000582341, 0.000515141, 0.000452378, 0.000391606, 0, 0.00107786, 0.0010367, 0.000989221, 0.000931818, 0.000863303, 0.000785178, 0.000700919, 0.000614598, 0.000529211, 0.000445026, 0.000357735, 0};

 for (int i =0; i < hqcd->GetNbinsX(); i++) {
   hqcd->SetBinContent(i+1,values[i]);
   hqcd->SetBinError(i+1,errors[i]);
 }

}
