void ControlPlots(TString folder, const char* h, TString norm2data) {

    TFile f1(folder+"DATA_CP.root", "read");
    TH1D* hdata = (TH1D*)f1.Get(h);
    hdata->SetDirectory(0);

    TFile f2(folder+"DYM_CP.root", "read");
    TH1D* hsignal = (TH1D*)f2.Get(h);
    hsignal->SetDirectory(0);

    TFile f3(folder+"QCD_CP_dd.root", "read");
    //TFile f3(folder+"QCD_CP.root", "read");
    //TH1D* hqcd = (TH1D*)f3.Get(h);
    //hqcd->SetDirectory(0);

    TFile f4(folder+"TT_CP_dd.root", "read");
    //TFile f4(folder+"TT_CP.root", "read");
    TH1D* httbar = (TH1D*)f4.Get(h);
    httbar->SetDirectory(0);

    TFile f5(folder+"TAU_CP.root", "read");
    TH1D* hZtautau = (TH1D*)f5.Get(h);
    hZtautau->SetDirectory(0);
    
    TFile f6(folder+"EWK_CP.root", "read");
    TH1D* hWleptonnu = (TH1D*)f6.Get(h);
    hWleptonnu->SetDirectory(0);
    
    TFile f7(folder+"DIBOSON_CP.root", "read");
    TH1D* hDibosons = (TH1D*)f7.Get(h);
    hDibosons->SetDirectory(0);
    
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    gROOT->ProcessLine(".L /home/ba01/u115/asvyatko/DYStudy/CMSSW_4_2_8/src/DYPackage/tools/setTDRStyle.C");
    setTDRStyle();
    gROOT->SetStyle("tdrStyle");
    gROOT->ForceStyle(true);
    gROOT->ProcessLine(".L /home/ba01/u115/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/plottools.C"); 

    // signal
    hsignal->SetLineColor(kOrange);
    hsignal->SetFillColor(kOrange);
    
    // qcd
    //hqcd->SetLineColor(9);
    //hqcd->SetFillColor(9);

    // tt
    httbar->SetLineColor(kRed+2);
    httbar->SetFillColor(kRed+2);

    // tau
    hZtautau->SetLineColor(kGreen);
    hZtautau->SetFillColor(kGreen);

    // EWK
    hWleptonnu->SetLineColor(38);
    hWleptonnu->SetFillColor(38);

    //hupsilon->SetLineColor(51);
    //hupsilon->SetFillColor(51);

    // diboson
    hDibosons->SetLineColor(40);
    hDibosons->SetFillColor(40);

    //
    //The histograms come properly weighted with priors
    //This would normalize to number of events in data
    //
    if (norm2data == "True" && !(TString(h) == "dxyBS" || TString(h) == "trigMatches")) {
	    const double ttbarNorm = httbar->Integral();
	    const double WleptonnuNorm = hWleptonnu->Integral();
	    const double ZtautauNorm = hZtautau->Integral();
	    const double qcdNorm = hqcd->Integral();
 	    const double dibosonNorm = hDibosons->Integral();
	    const double signalNorm = hsignal->Integral();
 	    const double fullMCintegral = ttbarNorm+WleptonnuNorm+ZtautauNorm+qcdNorm+dibosonNorm+signalNorm;

	    httbar->Scale(hdata->Integral()/fullMCintegral);
	    hWleptonnu->Scale(hdata->Integral()/fullMCintegral);
	    hZtautau->Scale(hdata->Integral()/fullMCintegral);
	    hqcd->Scale(hdata->Integral()/fullMCintegral);
	    hsignal->Scale(hdata->Integral()/fullMCintegral);
	    hDibosons->Scale(hdata->Integral()/fullMCintegral);
    } else {
            const double ttbarNorm = httbar->Integral();
            const double WleptonnuNorm = hWleptonnu->Integral();
            const double ZtautauNorm = hZtautau->Integral();
            const double qcdNorm = hqcd->Integral();
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

    // mc
    TH1D* hmc = (TH1D*)hsignal->Clone();
    hmc->Add(hqcd);
    hmc->Add(httbar);
    hmc->Add(hZtautau);
    hmc->Add(hWleptonnu);
    hmc->Add(hDibosons);

    //set up ratio check
    TH1D* hratio = (TH1D*)httbar->Clone();
    FindRatio(hratio,hdata,hmc);

    THStack* hstack = new THStack("hstack", "hstack");
    hstack->Add(httbar);
    hstack->Add(hWleptonnu);
    hstack->Add(hDibosons);
    hstack->Add(hZtautau);
    hstack->Add(hqcd);
    hstack->Add(hsignal);

    std::ostringstream pprint;
    //pprint.str("");
    //pprint<<Lumi;
    //string lumiLabel = pprint.str();

    TCanvas *c1 = new TCanvas(TString(h), TString(h), 800, 840);
    c1->Draw();
    c1->cd();
    //TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
    //p1->Draw();
    //p1->cd();
    //p1->SetBottomMargin(0.01);
    c1->SetLogy();
    cout << TString(h) << endl;
    if (TString(h) == "invm") c1->SetLogx();

    hstack->SetMaximum(3.*hdata->GetMaximum());
    hstack->SetMinimum(1.0*10E-2);
    hstack->SetTitle(""); //, Lumi = "+TString(lumiLabel)+" pb^{-1}");
    hstack->Draw("");
    hstack->GetXaxis()->SetMoreLogLabels();
    hstack->GetXaxis()->SetTitle(getTitle(TString(h),false));
    hstack->GetYaxis()->SetTitle("Number of Events");
    hstack->Draw("");

    THStack* hstack2 = new THStack("stack2","stack2");
    //hupsilon->SetMarkerSize(0);
    httbar->SetMarkerSize(0);
    hWleptonnu->SetMarkerSize(0);
    hZtautau->SetMarkerSize(0);
    hqcd->SetMarkerSize(0);
    hsignal->SetMarkerSize(0);
    hDibosons->SetMarkerSize(0);
    hstack2->Add(httbar);
    hstack2->Add(hWleptonnu);
    hstack2->Add(hDibosons);
    hstack2->Add(hZtautau);
    hstack2->Add(hqcd);
    hstack2->Add(hsignal);
    hstack2->Draw("Esame");

    hdata->SetMarkerStyle(20);
    hdata->GetXaxis()->SetMoreLogLabels();
    hdata->Draw("epsame");

//SHORTCUT DRAW
/*    TFile *fshortcut = new TFile("invmass_HLT_Mu13_Mu8_CP.root","r");
    fshortcut->cd();
    TH1D* hdata_shortcut1 = (TH1D*)gDirectory->Get("h1_HLT_Double_PromptReco_v4");
    TH1D* hdata_shortcut2 = (TH1D*)gDirectory->Get("h1_HLT_Double_PromptReco_v6");
    TH1D* hdata_shortcut3 = (TH1D*)gDirectory->Get("h1_HLT_Double_Aug05ReReco");
    TH1D* hdata_shortcut4 = (TH1D*)gDirectory->Get("h1_HLT_Double_Run2011B");
    TH1D* hdata_shortcut = (TH1D*)hdata_shortcut1->Clone();
    hdata_shortcut->Add(hdata_shortcut2);
    hdata_shortcut->Add(hdata_shortcut3);
    hdata_shortcut->Add(hdata_shortcut4);
*/  //  hdata->Draw("epsame");

    TPaveText *pt = new TPaveText(0.30,0.75,0.45,0.90,"brNDC");
    pt->SetFillColor(0);
//   pt->SetTextFont(72);
    pt->SetTextSize(0.040);
    pt->SetBorderSize(0);
    pt->SetTextAlign(12);
    pt->AddText("CMS Preliminary, #intLdt = 15 pb^{-1}");

/*    gStyle->SetOptTitle(0);
    TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.24,0,0,0);
    c1->cd();
    p2->Draw();
    p2->cd();
    p2->SetLogx();
    p2->SetTopMargin(0.007);
    p2->SetBottomMargin(0.35);
    p2->SetTickx();
    hratio->GetYaxis()->SetTitle("R_{Data/MC}");
    hratio->SetStats(kFALSE);
    hratio->GetYaxis()->SetTitleOffset(0.29);
    hratio->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
    hratio->GetYaxis()->SetTitleSize(0.15);
    hratio->GetXaxis()->SetTitleSize(0.15);
    hratio->GetYaxis()->SetLabelSize(0.10);
    hratio->GetXaxis()->SetLabelSize(0.15);
    hratio->SetMarkerSize(1.3);
    hratio->SetMaximum(3.0);
    hratio->SetMinimum(-0.5);
    hratio->SetMarkerStyle(20);
    hratio->Draw("ep");
*/
    //add to stack
    TH1D* hstack4fit = (TH1D*)hsignal->Clone();
    hstack4fit->Add(hWleptonnu);
    hstack4fit->Add(hDibosons);
    hstack4fit->Add(httbar);
    hstack4fit->Add(hZtautau);
    hstack4fit->Add(hqcd);

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
//    tmpstrm << "Upsilon" << Round(hupsilon->Integral());
//    legend->AddEntry(hupsilon, tmpstrm.str().c_str(), "F");
//    tmpstrm.str("");

    legend->SetFillColor(0);
    //if (TString(h) == "invm") 
    legend->Draw("0");

    c1->Print(TString(h)+".png");
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

