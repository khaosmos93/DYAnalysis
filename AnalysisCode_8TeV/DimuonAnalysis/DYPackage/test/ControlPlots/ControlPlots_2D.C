void ControlPlots_2D(TString folder, const char* h, TString norm2data) {

    bool do_DD = false;
    bool debug = true;

    TFile f1(folder+"DATA_CP.root", "read");
    TH1D* hdata = (TH1D*)f1.Get(h);
    hdata->SetDirectory(0);

    //TFile f2(folder+"DY_CP_fewz.root", "read");
    TFile f2(folder+"DYM_CP.root", "read");
    TH1D* hsignal = (TH1D*)f2.Get(h);
    hsignal->SetDirectory(0);

    TFile f3(folder+"QCD_CP_2D_dd.root", "read");
    TH1D* hqcd = (TH1D*)f3.Get("hqcd"); //h);
    hqcd->SetDirectory(0);
    TH1D* httbar;
    TH1D* hZtautau;
    TH1D* hWleptonnu;
    TH1D* hDibosons;

    if (do_DD) {

      TFile f4a(folder+"TT_CP_2D_dd.root", "read");
      httbar = (TH1D*)f4a.Get("httbar");
      httbar->SetDirectory(0);

      TFile f4(folder+"TT_CP.root", "read");
      httbar_mc = (TH1D*)f4.Get(h);
      httbar_mc->SetDirectory(0);

      TFile f5(folder+"TAU_CP.root", "read");
      hZtautau = (TH1D*)f5.Get(h);
      hZtautau->SetDirectory(0);

      TFile f6(folder+"EWK_CP.root", "read");
      hWleptonnu = (TH1D*)f6.Get(h);
      hWleptonnu->SetDirectory(0);

      TFile f7(folder+"DIBOSON_CP.root", "read");
      hDibosons = (TH1D*)f7.Get(h);
      hDibosons->SetDirectory(0);

    } else { 

      TFile f4(folder+"TT_CP.root", "read");
      httbar = (TH1D*)f4.Get(h);
      httbar->SetDirectory(0);

      TFile f5(folder+"TAU_CP.root", "read");
      hZtautau = (TH1D*)f5.Get(h);
      hZtautau->SetDirectory(0);
    
      TFile f6(folder+"EWK_CP.root", "read");
      hWleptonnu = (TH1D*)f6.Get(h);
      hWleptonnu->SetDirectory(0);
    
      TFile f7(folder+"DIBOSON_CP.root", "read");
      hDibosons = (TH1D*)f7.Get(h);
      hDibosons->SetDirectory(0);
    }
  
    gStyle->SetOptStat(0);
    //gStyle->SetPalette(1);
    //gROOT->ProcessLine(".L ../tools/setTDRStyle.C");
    //setTDRStyle();
    //gROOT->SetStyle("tdrStyle");
    //gROOT->ForceStyle(true);
    gROOT->ProcessLine(".L ../tools/plottools.C"); 
    gROOT->ProcessLine(".L ../tools/histotools.C");

    //
    //The histograms come properly weighted with priors
    //This would normalize to number of events in data
    //

    if (norm2data == "True" && !(TString(h) == "dxyBS" || TString(h) == "trigMatches")) {

            //FIXME that is ttbar_mc in MC case and ttbar in data driven case
            double ttbarNorm = httbar->Integral();
            const double WleptonnuNorm = hWleptonnu->Integral();
            const double ZtautauNorm = hZtautau->Integral();
            const double qcdNorm = hqcd->Integral();
            const double dibosonNorm = hDibosons->Integral();
            const double signalNorm = hsignal->Integral();
            double fullMCintegral = ttbarNorm+WleptonnuNorm+ZtautauNorm+qcdNorm+dibosonNorm+signalNorm;

           if (do_DD) {
            //repeat the same but using the data driven non-QCD.
            ttbarNorm = httbar->Integral();
            fullMCintegral = ttbarNorm+qcdNorm+signalNorm;

            httbar->Scale(hdata->Integral()/fullMCintegral);
            hWleptonnu->Scale(0);
            hZtautau->Scale(0);
            hqcd->Scale(hdata->Integral()/fullMCintegral);
            hsignal->Scale(hdata->Integral()/fullMCintegral);
            hDibosons->Scale(0);

            normalizeToPeak(httbar, hWleptonnu, hqcd, hZtautau, hsignal, hDibosons, hdata);
           } else {
            httbar->Scale(hdata->Integral()/fullMCintegral);
            hWleptonnu->Scale(hdata->Integral()/fullMCintegral);
            hZtautau->Scale(hdata->Integral()/fullMCintegral);
            hqcd->Scale(hdata->Integral()/fullMCintegral);
            hsignal->Scale(hdata->Integral()/fullMCintegral);
            hDibosons->Scale(hdata->Integral()/fullMCintegral);

            normalizeToPeak(httbar, hWleptonnu, hqcd, hZtautau, hsignal, hDibosons, hdata);
           }

    } else {
            const double ttbarNorm = httbar->Integral();
            const double WleptonnuNorm = hWleptonnu->Integral();
            const double ZtautauNorm = hZtautau->Integral();
            const double qcdNorm = hqcd->Integral(2,132);
            const double dibosonNorm = hDibosons->Integral();
            const double signalNorm = hsignal->Integral();
            const double fullMCintegral = ttbarNorm+WleptonnuNorm+ZtautauNorm+qcdNorm+dibosonNorm+signalNorm;

            httbar->Scale(hdata->Integral()/fullMCintegral);
            hWleptonnu->Scale(hdata->Integral()/fullMCintegral);
            hZtautau->Scale(hdata->Integral()/fullMCintegral);
            hqcd->Scale(hdata->Integral()/fullMCintegral);
            hsignal->Scale(hdata->Integral()/fullMCintegral);
            hDibosons->Scale(hdata->Integral()/fullMCintegral);
   }

    if (do_DD) { 
       cout << "non-QCD DD " << httbar->Integral() << endl;
       cout << "QCD DD " << hqcd->Integral() << endl;
    }
    else       cout << "non-QCD DD " << httbar->Integral()+hZtautau->Integral()+hWleptonnu->Integral()+hDibosons->Integral() << endl;

    //DO IT 7TEV STYLE
    //make stack for each slice instead
    THStack* hstack;
    THStack* hstack2;
    TCanvas* c1;
    TString WHICH[6] = {"absrap2030","absrap3045","absrap4560","absrap60120","absrap120200","absrap200"};
   
    for (int iw = 0; iw < 6; iw++) {
       //APPLY EFFICINECY CORRECTION
       TFile* tp1 = new TFile("../Inputs/effcorr/2D/correction_iso"+WHICH[iw]+"_split2D.root");
       tp1->cd();
       TH1D* hiso1 = (TH1D*)gDirectory->Get(WHICH[iw]);
       TFile* tp2 = new TFile("../Inputs/effcorr/2D/correction_trig"+WHICH[iw]+"_split2D.root");
       tp2->cd();
       TH1D* htrig1 = (TH1D*)gDirectory->Get(WHICH[iw]);
       TFile* tp3 = new TFile("../Inputs/effcorr/2D/correction_recoid"+WHICH[iw]+"_split2D.root");
       tp3->cd();
       TH1D* hrecoid1 = (TH1D*)gDirectory->Get(WHICH[iw]);

       TH1D* hcorr = histoMultiply3(htrig1,hiso1,hrecoid1,true);
       hstack = new THStack("hstack"+WHICH[iw],"hstack"+WHICH[iw]);

       TH1D* hslice_data = get2DSlice(hdata, WHICH[iw],false);
       //hslice_data->Divide(hslice_data,hcorr);

       TH1D* hslice_ttbar = get2DSlice(httbar, WHICH[iw],false);
       hslice_ttbar->SetName("hslice_ttbar");
       cout << "Slice ttbar " << hslice_ttbar->Integral() << endl;
       TH1D* hslice_Wleptonnu = get2DSlice(hWleptonnu, WHICH[iw],false);
       hslice_Wleptonnu->SetName("hslice_leptonnu");
       TH1D* hslice_Dibosons = get2DSlice(hDibosons, WHICH[iw],false);
       hslice_Dibosons->SetName("hslice_Diboson");
       TH1D* hslice_Ztautau = get2DSlice(hZtautau, WHICH[iw],false);
       hslice_Ztautau->SetName("hslice_Ztautau");
       TH1D* hslice_qcd = get2DSlice(hqcd, WHICH[iw],false);
       hslice_qcd->SetName("hslice_qcd");
       TH1D* hslice_signal = get2DSlice(hsignal, WHICH[iw],false); 
       hslice_signal->SetName("hslice_signal");
       cout << "This " << iw << hslice_signal->GetNbinsX() << endl;

       // signal
       hslice_signal->SetLineColor(kOrange);
       hslice_signal->SetFillColor(kOrange);
             
       // qcd  
       hslice_qcd->SetLineColor(9);
       hslice_qcd->SetFillColor(9);

       // tt
       hslice_ttbar->SetLineColor(kRed+2);
       hslice_ttbar->SetFillColor(kRed+2);
            
       // tau  
       hslice_Ztautau->SetLineColor(kGreen);
       hslice_Ztautau->SetFillColor(kGreen);
            
       // EWK  
       hslice_Wleptonnu->SetLineColor(38);
       hslice_Wleptonnu->SetFillColor(38);
            
       // diboson
       hslice_Dibosons->SetLineColor(40);
       hslice_Dibosons->SetFillColor(40);

       hstack->Add(hslice_qcd);
       hstack->Add(hslice_ttbar);
       hstack->Add(hslice_Wleptonnu);
       hstack->Add(hslice_Dibosons);
       hstack->Add(hslice_Ztautau);
       hstack->Add(hslice_signal);

       //std::ostringstream pprint;
       //pprint.str("");
       //pprint<<Lumi;
       //string lumiLabel = pprint.str();

       c1 = new TCanvas(TString(h)+WHICH[iw], TString(h)+WHICH[iw], 800, 840);
       c1->Draw();
       c1->cd();
       TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
       p1->Draw();
       p1->cd();
       //p1->SetLogy();
       p1->SetBottomMargin(0.01);

       hstack->SetMaximum(1.2*hslice_data->GetMaximum());
       hstack->SetMinimum(1.0*10E-2);
       hstack->SetTitle(""); //, Lumi = "+TString(lumiLabel)+" pb^{-1}");
       hstack->Draw("");
       hstack->GetXaxis()->SetMoreLogLabels();
       hstack->GetXaxis()->SetTitle(getTitle(TString(h),false));
       hstack->GetYaxis()->SetTitle("Number of Events");
       hstack->Draw("");

       //error stack
       hstack2 = new THStack("stack2"+WHICH[iw],"stack2"+WHICH[iw]);
       hstack2->Add(hslice_qcd);
       hstack2->Add(hslice_ttbar);
       hstack2->Add(hslice_Wleptonnu);
       hstack2->Add(hslice_Dibosons);
       hstack2->Add(hslice_Ztautau);
       hstack2->Add(hslice_signal);

       hslice_ttbar->SetMarkerSize(0);
       hslice_Wleptonnu->SetMarkerSize(0);
       hslice_Ztautau->SetMarkerSize(0);
       hslice_qcd->SetMarkerSize(0);
       hslice_signal->SetMarkerSize(0);
       hslice_Dibosons->SetMarkerSize(0);
       hstack2->Draw("Esame");

       hslice_data->SetMarkerStyle(20);
       hslice_data->Draw("epsame");

       if (debug) {
        //For debug
        TH1D* background = (TH1D*)hslice_ttbar->Clone();
        background->Add(hslice_qcd);
        background->Add(hslice_Wleptonnu);
        background->Add(hslice_Ztautau);
        background->Add(hslice_Dibosons);
        cout << "Background " << background->Integral() << endl;
        background->SetName(WHICH[iw]);

        TString postfix = "";

        if (do_DD) postfix = "_DD";
        if (iw == 0) { 
           TFile* fdebug = new TFile("debugOut"+postfix+".root","recreate");
           cout << "Recreating" << endl;
        } else { 
           TFile* fdebug = new TFile("debugOut"+postfix+".root","update"); 
           cout << "Updating" << endl;
        }
        fdebug->cd();
        background->Write(WHICH[iw]);
        fdebug->Close();
       }


       //Set up the ratio
       TH1D* hmc = (TH1D*)hslice_signal->Clone();
       hmc->Add(hslice_qcd);
       hmc->Add(hslice_ttbar);
       hmc->Add(hslice_Wleptonnu);
       hmc->Add(hslice_Ztautau);
       hmc->Add(hslice_Dibosons);

       TH1D* hratio = (TH1D*)hmc->Clone();
       FindRatio(hratio,hslice_data,hmc);

       gStyle->SetOptTitle(0);
       TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.247,0,0,0);
       c1->cd();
       p2->Draw();
       p2->cd();
       //p2->SetLogx();
       p2->SetTopMargin(0.007);
       p2->SetBottomMargin(0.35);
       p2->SetTickx();
       hratio->GetYaxis()->SetTitle("R_{Data/MC}");
       hratio->SetStats(kFALSE);
       hratio->GetYaxis()->SetTitleOffset(0.29);
       hratio->GetXaxis()->SetTitle("|Y_{#mu#mu}|");
       hratio->GetYaxis()->SetTitleSize(0.15);
       hratio->GetXaxis()->SetTitleSize(0.15);
       hratio->GetYaxis()->SetLabelSize(0.10);
       hratio->GetXaxis()->SetLabelSize(0.15);
       hratio->SetMarkerSize(1.3);
       hratio->SetMaximum(1.45);
       hratio->SetMinimum(0.55);
       hratio->SetMarkerStyle(20);
       hratio->Draw("ep");
/*
       TPaveText *pt = new TPaveText(0.30,0.75,0.45,0.90,"brNDC");
       pt->SetFillColor(0);
   //   pt->SetTextFont(72);
       pt->SetTextSize(0.040);
       pt->SetBorderSize(0);
       pt->SetTextAlign(12);
       pt->AddText("CMS Preliminary, #intLdt = 15 pb^{-1}");

       ostringstream tmpstrm;
       TLegend *legend = new TLegend(0.61, 0.63, 0.93, 0.93);
       legend->SetTextFont(72);
       legend->SetTextSize(0.026);
       tmpstrm.str("");
       tmpstrm << "DATA   " << hdata->Integral();
       legend->AddEntry(hdata, tmpstrm.str().c_str(), "PL");
       tmpstrm.str("");
       tmpstrm << "DY#rightarrow#mu#mu   " << Round(hsignal->Integral());
       legend->AddEntry(hsignal, tmpstrm.str().c_str(), "F");
       tmpstrm.str("");
    tmpstrm << "QCD#rightarrow#mu+X   " << Round(hqcd->Integral());
    legend->AddEntry(hqcd, tmpstrm.str().c_str(), "F");
    tmpstrm.str("");
    tmpstrm << "Z#rightarrow#tau#tau   " << Round(hZtautau->Integral());
    legend->AddEntry(hZtautau, tmpstrm.str().c_str(), "F");
    tmpstrm.str("");
    tmpstrm << "t#bar{t}   " << Round(httbar->Integral());
    legend->AddEntry(httbar, tmpstrm.str().c_str(), "F");
    tmpstrm.str("");
    tmpstrm << "W#rightarrow l#nu   " << Round(hWleptonnu->Integral());
    legend->AddEntry(hWleptonnu, tmpstrm.str().c_str(), "F");
    tmpstrm.str("");
    tmpstrm << "Dibosons   " << Round(hDibosons->Integral());
    legend->AddEntry(hDibosons, tmpstrm.str().c_str(), "F");
    tmpstrm.str("");
    legend->SetFillColor(0);
    if (TString(h) == "invm") legend->Draw("0");
*/
    c1->Update(); 
    c1->Print(TString(h)+WHICH[iw]+".png");
  }//6 slices
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
double Round(double d) {
    double nd = (double) floor(d*10.)/10.;
    return nd;
}


void normalizeToPeak(TH1D* hmc1, TH1D* hmc2, TH1D* hmc3, TH1D* hmc4, TH1D* hmc5, TH1D* hmc6, TH1D* hdata) {
   double this_mc = hmc1->Integral(73,96)+hmc2->Integral(73,96)+hmc3->Integral(73,96)+hmc4->Integral(73,96)+hmc5->Integral(73,96)+hmc6->Integral(73,96);
   double this_data = hdata->Integral(73,96);
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc1->Scale(this_data/this_mc);
      hmc2->Scale(this_data/this_mc);
      hmc3->Scale(this_data/this_mc);
      hmc4->Scale(this_data/this_mc);
      hmc5->Scale(this_data/this_mc);
      hmc6->Scale(this_data/this_mc);
      this_mc = hmc1->Integral(73,96)+hmc2->Integral(73,96)+hmc3->Integral(73,96)+hmc4->Integral(73,96)+hmc5->Integral(73,96)+hmc6->Integral(73,96);
   }
}

