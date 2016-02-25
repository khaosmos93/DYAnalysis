void ControlPlots_2D() {

    gStyle->SetOptStat(0);
    gROOT->ProcessLine(".L ../tools/setTDRStyle.C");
    setTDRStyle();
    gROOT->SetStyle("tdrStyle");
    gROOT->ForceStyle(true);
    tdrStyle->SetTitleX(0.14);
    tdrStyle->SetTitleY(0.935); // for c4, Z peak plot

    gROOT->ProcessLine(".L ../tools/plottools.C"); 
    gROOT->ProcessLine(".L ../tools/histotools.C");

    TString doDD = "";//"_DDMC"; //"_DD";
    //get the root files and read into histos used throughout
    TFile* f = new TFile("../Inputs/rawYield/DYspectrum_Rap_v2.root");
    f->cd();

    TH1D* httbar = (TH1D*)gDirectory->Get("httbar_mc");
    TH1D* hWleptonnu = (TH1D*)gDirectory->Get("hWlepton");
    TH1D* hDibosons = (TH1D*)gDirectory->Get("hdiboson");
    TH1D* hZtautau  = (TH1D*)gDirectory->Get("hDYtautau");
    TH1D* hqcd = (TH1D*)gDirectory->Get("hqcd");
    TH1D* hsignal = (TH1D*)gDirectory->Get("hsig_fewz");   
    TH1D* hdata = (TH1D*)gDirectory->Get("hdata");

    //DO IT 7TEV STYLE
    //make stack for each slice instead
    THStack* hstack;
    THStack* hstack2;
    TCanvas* c1;
    TString WHICH[6] = {"absrap2030","absrap3045","absrap4560","absrap60120","absrap120200","absrap200"};
    TString beautiful[6] = {"20 < m < 30 GeV","30 < m < 45 GeV","45 < m < 60 GeV","60 < m < 120 GeV","120 < m < 200 GeV","200 < m < 1500 GeV"};
   
    for (int iw = 0; iw < 6; iw++) {

       hstack = new THStack("hstack"+WHICH[iw],"hstack"+WHICH[iw]);
       TH1D* hslice_data = get2DSlice(hdata, WHICH[iw],true,false);
       TH1D* hslice_ttbar = get2DSlice(httbar, WHICH[iw],true,false);
       hslice_ttbar->SetName("hslice_ttbar");
       TH1D* hslice_Wleptonnu = get2DSlice(hWleptonnu, WHICH[iw],true,false);
       hslice_Wleptonnu->SetName("hslice_leptonnu");
       TH1D* hslice_Dibosons = get2DSlice(hDibosons, WHICH[iw],true,false);
       hslice_Dibosons->SetName("hslice_Diboson");
       TH1D* hslice_Ztautau = get2DSlice(hZtautau, WHICH[iw],true,false);
       hslice_Ztautau->SetName("hslice_Ztautau");
       TH1D* hslice_qcd = get2DSlice(hqcd, WHICH[iw],true,false);
       hslice_qcd->SetName("hslice_qcd");
       TH1D* hslice_signal = get2DSlice(hsignal, WHICH[iw],true,false); 
       hslice_signal->SetName("hslice_signal");

       //FIXME turn off backgrounds
       //hslice_ttbar->Scale(0);
       //hslice_Wleptonnu->Scale(0);
       //hslice_Ztautau->Scale(0);
       //hslice_qcd->Scale(0);
       //hslice_Dibosons->Scale(0);

       // signal
       hslice_signal->SetLineColor(kOrange-2);
       hslice_signal->SetFillColor(kOrange-2);

       // qcd  
       hslice_qcd->SetLineColor(kViolet-4);
       hslice_qcd->SetFillColor(kViolet-4);

       // tt
       hslice_ttbar->SetLineColor(kRed+2);
       hslice_ttbar->SetFillColor(kRed+2);

       // tau  
       hslice_Ztautau->SetLineColor(kOrange+3);
       hslice_Ztautau->SetFillColor(kOrange+3);
             
       // EWK  
       TH1D* hslice_EWK = (TH1D*)hslice_Wleptonnu->Clone();
       hslice_EWK->Add(hslice_Dibosons);
       hslice_EWK->SetLineColor(kOrange+10);
       hslice_EWK->SetFillColor(kOrange+10);

       hstack->Add(hslice_qcd);
       hstack->Add(hslice_ttbar);
       //hstack->Add(hslice_Wleptonnu);
       //hstack->Add(hslice_Dibosons);
       hstack->Add(hslice_EWK);
       hstack->Add(hslice_Ztautau);
       hstack->Add(hslice_signal);

       c1 = new TCanvas(TString(hdata->GetName())+WHICH[iw], TString(hdata->GetName())+WHICH[iw], 600,600);
       c1->Draw();
       c1->cd();
       TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
       p1->Draw();
       p1->cd();
       //p1->SetLogy();
       p1->SetBottomMargin(0.007);
       p1->SetRightMargin(0.1);
       p1->SetFillStyle(0);


       hstack->SetMaximum(1.5*hslice_data->GetMaximum());
       hstack->SetMinimum(1.0*10E-2);
       hstack->SetTitle(""); //, Lumi = "+TString(lumiLabel)+" pb^{-1}");
       hstack->Draw("");
       hstack->GetXaxis()->SetMoreLogLabels();
       //hstack->GetXaxis()->SetTitle(getTitle(TString(hdata->GetName()),false));
       hstack->GetYaxis()->SetTitle("Number of Events");
       hstack->Draw("");

       //error stack
       hstack2 = new THStack("stack2"+WHICH[iw],"stack2"+WHICH[iw]);
       hstack2->Add(hslice_qcd);
       hstack2->Add(hslice_ttbar);
       //hstack2->Add(hslice_Wleptonnu);
       //hstack2->Add(hslice_Dibosons);
       hstack2->Add(hslice_EWK);
       hstack2->Add(hslice_Ztautau);
       hstack2->Add(hslice_signal);

       hslice_ttbar->SetMarkerSize(0);
       //hslice_Wleptonnu->SetMarkerSize(0);
       hslice_Ztautau->SetMarkerSize(0);
       hslice_qcd->SetMarkerSize(0);
       hslice_signal->SetMarkerSize(0);
       //hslice_Dibosons->SetMarkerSize(0);
       hslice_EWK->SetMarkerSize(0);
       hstack2->Draw("Esame");

       hslice_data->SetMarkerStyle(20);
       hslice_data->Draw("epsame");

       //Set up the ratio
       TH1D* hmc = (TH1D*)hslice_signal->Clone();
       hmc->Add(hslice_qcd);
       hmc->Add(hslice_ttbar);
       //hmc->Add(hslice_Wleptonnu);
       hmc->Add(hslice_Ztautau);
       //hmc->Add(hslice_Dibosons);
       hmc->Add(hslice_EWK);

       TH1D* hratio = (TH1D*)hslice_data->Clone();
       //FindRatio(hratio,hslice_data,hmc);
       hmc->Sumw2();
       hslice_data->Sumw2();
       hratio->Divide(hslice_data,hmc);
       //cout << "Check uncertainties " << hslice_data->GetBinError(24)/hslice_data->GetBinContent(24)*100. << " " << 100.*hslice_signal->GetBinError(24)/hslice_signal->GetBinContent(24) << " " << hratio->GetBinError(24)/hratio->GetBinContent(24)*100. << " " << sqrt(pow(hslice_data->GetBinError(24)/hslice_data->GetBinContent(24),2)+pow(hslice_signal->GetBinError(24)/hslice_signal->GetBinContent(24),2))*100. << endl; 

      TPaveText *pt = new TPaveText(0.15,0.76,0.55,0.90,"brNDC");
      pt->SetFillColor(0);
      pt->SetTextFont(72);
      pt->SetTextSize(0.045);
      pt->SetBorderSize(0);
      pt->SetTextAlign(12);
      pt->AddText("CMS Preliminary, #intLdt = 19.7 fb^{-1}, at #sqrt{s}=8 TeV");
      pt->Draw("same");

      TPaveText *pt2 = new TPaveText(0.16,0.73,0.35,0.75,"brNDC");
      pt2->SetFillColor(0);
      pt2->SetTextFont(72);
      pt2->SetTextSize(0.045);
      pt2->SetBorderSize(0);
      pt2->SetTextAlign(12);
      pt2->AddText(beautiful[iw]);
      pt2->Draw("same");


      ostringstream tmpstrm;

      TLegend *legend = new TLegend(0.65, 0.50, 0.80, 0.75);
      legend->SetTextFont(72);
      legend->SetTextSize(0.045);
      legend->SetBorderSize(0);
      tmpstrm.str("");
      tmpstrm << "Data   " ;
      legend->AddEntry(hslice_data, tmpstrm.str().c_str(), "PL");
      tmpstrm.str("");
      tmpstrm << "#gamma*/Z #rightarrow#mu#mu   " ;
      legend->AddEntry(hslice_signal, tmpstrm.str().c_str(), "F");
      tmpstrm.str("");
      tmpstrm << "#gamma*/Z #rightarrow#tau#tau   " ;
      legend->AddEntry(hslice_Ztautau, tmpstrm.str().c_str(), "F");
      tmpstrm.str("");
      tmpstrm << "EW  " ;
      legend->AddEntry(hslice_EWK, tmpstrm.str().c_str(), "F");
      tmpstrm.str("");
      tmpstrm << "t#bar{t}+tW+#bar{t}W   " ;
      legend->AddEntry(hslice_ttbar, tmpstrm.str().c_str(), "F");
      tmpstrm.str("");
      tmpstrm << "QCD   " ;
      legend->AddEntry(hslice_qcd, tmpstrm.str().c_str(), "F");
      tmpstrm.str("");
       
      legend->SetFillColor(0);
      legend->Draw("0");


       gStyle->SetOptTitle(0);
       TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.247,0,0,0);
       c1->cd(); 
       p2->Draw();
       p2->cd();
       //p2->SetLogx();
       p2->SetTopMargin(0.001);
       p2->SetBottomMargin(0.35);
       p2->SetTickx();
       p2->SetRightMargin(0.1);
       p2->SetFillStyle(0);
       p2->SetGrid();

       hratio->GetXaxis()->SetTitle("Absolute dimuon rapidity, |y|");
       hratio->GetYaxis()->SetTitle("Data/MC");
       hratio->GetXaxis()->SetTitleSize(0.14);
       hratio->GetYaxis()->SetTitleSize(0.11);
       hratio->GetYaxis()->SetTitleOffset(0.5);
       hratio->GetYaxis()->SetLabelSize(0.11);
       hratio->GetXaxis()->SetLabelSize(0.11);
       hratio->SetMaximum(1.52);
       hratio->SetMinimum(0.48);

       //hratio->GetYaxis()->SetTitleOffset(0.29);
       //hratio->GetYaxis()->SetTitleSize(0.15);
       //hratio->GetXaxis()->SetTitleSize(0.15);
       //hratio->GetYaxis()->SetLabelSize(0.10);
       //hratio->GetXaxis()->SetLabelSize(0.15);
       //hratio->SetMarkerSize(1.3);
       //hratio->SetMarkerStyle(20);
       hratio->Draw("ep");

       //TLine *line=new TLine();
       //line->SetLineColor(kBlue);
       //line->SetLineWidth(1);
       //line->DrawLine(0,1,2.4,1);

    //c1->Update(); 
    //c1->Print(TString(hdata->GetName())+WHICH[iw]+".png");
    c1->Print("EErap_corr"+WHICH[iw]+".png");

  }//6 slices
}

void FindRatio(TH1D* ratio, TH1D* numer, TH1D* denom) {
   for (int ibin = 0; ibin < numer->GetXaxis()->GetNbins()+1;ibin++) {
       //if (denom->GetBinContent(ibin+1) != 0) {
                double rat = (double)(numer->GetBinContent(ibin+1)/denom->GetBinContent(ibin+1));
                ratio->SetBinContent(ibin+1,rat);
                ratio->SetBinError(ibin+1,ratio->GetBinContent(ibin+1)*sqrt(pow(numer->GetBinError(ibin+1)/numer->GetBinContent(ibin+1),2)+pow(denom->GetBinError(ibin+1)/denom->GetBinContent(ibin+1),2)));
       // } else {
       //         ratio->SetBinContent(ibin+1,0.);
      // }
  }
}
double Round(double d) {
    double nd = (double) floor(d*10.)/10.;
    return nd;
}


void normalizeToPeak(TH1D* hmc1, TH1D* hmc2, TH1D* hmc3, TH1D* hmc4, TH1D* hmc5, TH1D* hmc6, TH1D* hdata) {
   double this_mc = hmc1->Integral()+hmc2->Integral()+hmc3->Integral()+hmc4->Integral()+hmc5->Integral()+hmc6->Integral();
   double this_data = hdata->Integral();
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc1->Scale(this_data/this_mc);
      hmc2->Scale(this_data/this_mc);
      hmc3->Scale(this_data/this_mc);
      hmc4->Scale(this_data/this_mc);
      hmc5->Scale(this_data/this_mc);
      hmc6->Scale(this_data/this_mc);
      this_mc = hmc1->Integral()+hmc2->Integral()+hmc3->Integral()+hmc4->Integral()+hmc5->Integral()+hmc6->Integral();
   }
}


void checkPoissontError(TH1D* input) {
  for (int i = 0; i < input->GetNbinsX(); i++) {
    input->SetBinError(i+1,sqrt(input->GetBinContent(i+1)));
  } 
}
