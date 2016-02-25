void plottools(void) {}

//
//Legends are hard coded right now!
//

void getEff1(TH1D* h1, bool isData, TString varType, TString INPUTDIR_PREFIX, double from, double to) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();

   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   if (!isData) {
     h1->SetLineColor(kRed);
     h1->SetFillColor(kRed);
   } else {
     h1->SetMarkerStyle(20);
   } 

   //if you compare data to data
   if (!isData) {
     h1->Draw("E2");
   } else {
     h1->Draw("PE"); 
   }
   cout << "WARNING: I hardcode that this is used for T&P" << endl;
   h1->GetXaxis()->SetTitle(getTitle(varType,true));
   if (!isData) {
     leg->AddEntry(h1,"MC","f");
   } else {
     leg->AddEntry(h1,"MC","pl");
   }
   //leg->Draw("COLZsame");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();
   //Save
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h1->GetName()+getTitle(varType, true)+".png");

}

void getEff2(TH1D* h1, TH1D* h2, bool isData, TString varType, TString INPUTDIR_PREFIX, double from, double to) {
   
   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);
   
   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();
   
   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   h1->SetLineColor(kRed);
   h1->SetFillColor(kRed);
   if (!isData) {
     h2->SetLineColor(kGreen+1);
     h2->SetFillColor(kGreen+1);
     h2->SetFillStyle(3144);
   } else {
     h2->SetMarkerStyle(20);
   }
   
   //if you compare data to data
   h1->Draw("E2");
   h1->GetXaxis()->SetTitle(getTitle(varType,true));
   if (!isData) {
     h2->Draw("E2same");
   } else {
     h2->Draw("PEsame");
   }
   leg->AddEntry(h1,"MC, combined PF iso","f");
   if (!isData) {
     leg->AddEntry(h2,"MC, PF iso no e-#gamma","f");
   } else {
     leg->AddEntry(h2,"T&P data","pl");
   }
   leg->Draw("same");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();
   //Save
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h1->GetName()+getTitle(varType,true)+".png");

}

//Assume h3 a ratio
void getEff3(TH1D* h1, TH1D* h2, TH1D* h3, bool isData, TString varType, TString INPUTDIR_PREFIX, double from, double to) { 

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();

   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   h1->SetLineColor(kRed);
   h1->SetFillColor(kRed);
   if (!isData) {
     h2->SetLineColor(kBlue);
     h2->SetFillColor(kBlue);
   } else {
     h2->SetMarkerStyle(20);
     h2->SetMarkerSize(1.1);
   }
   h3->SetLineColor(kGreen);
   h3->SetFillColor(kGreen);
   h3->SetFillStyle(3144);

   //if you compare data to data
   h1->Draw("E2");
   h1->GetXaxis()->SetTitle(getTitle(varType, true));
   if (!isData) {
     h2->Draw("E2same");
   } else {
     h2->Draw("samePE");
   }
   h3->Draw("E2same");
   leg->AddEntry(h1,"MC T&P","f");
   //leg->AddEntry(h1,"MC truth","f");
   if (!isData) {
     leg->AddEntry(h2,"MC T&P","f");
   } else {
     leg->AddEntry(h2,"Data T&P","pl");
     //leg->AddEntry(h2,h2->GetName(),"pl");
   }
   leg->AddEntry(h3,"Ratio","f");
   //leg->AddEntry(h3,"T&P correction map","f");
   leg->Draw("same");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();

   //Save
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h3->GetName()+varType+".png");
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h3->GetName()+varType+".C");
}

//Assume heff_mct, htp, htpd, rat
void getEff3Stats(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4, bool isData, TString varType, TString INPUTDIR_PREFIX, double from, double to) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();

   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   h1->SetLineColor(kRed);
   h1->SetFillColor(kRed);
   h2->SetLineColor(kBlue);
   h2->SetFillColor(kBlue);
   h4->SetLineColor(kGreen);
   h4->SetFillColor(kGreen);
   h4->SetFillStyle(3144);

   //if you compare data to data
   h1->Draw("E2");
   h1->GetXaxis()->SetTitle(getTitle(varType, true));
   h2->Draw("E2same");
   h4->Draw("E2same");
   leg->AddEntry(h1,"MC Truth","f");
   leg->AddEntry(h2,"MC T&P","f");
   leg->AddEntry(h4,"Ratio","f");
   leg->Draw("same");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();

   gROOT->ProcessLine(".L ../tools/stattools.C");
   //Make TPave
   ostringstream tmpstrm1;
   ostringstream tmpstrm2;
   ostringstream tmpstrm3;

   tmpstrm3 << INPUTDIR_PREFIX << " " << h1->GetName();
   tmpstrm1 << estimatorPerMassHisto1(h2,h3);
   tmpstrm2 << estimatorPerMassHisto2(h2,h3,h1);

   TPaveText *pt = new TPaveText(0.25,0.77,0.40,0.92,"brNDC");
   pt->SetFillColor(0);
//  pt->SetTextFont(72);
   pt->SetTextSize(0.040);
   pt->SetBorderSize(0);
   pt->SetTextAlign(12);
   pt->AddText(TString(tmpstrm3.str().c_str()));
   pt->AddText("Simple estimator: "+TString(tmpstrm1.str().c_str()));
   pt->AddText("Combined estimator: "+TString(tmpstrm2.str().c_str()));
   pt->Draw("same");

   //Save
   c1->SaveAs(INPUTDIR_PREFIX+"/stats"+h1->GetName()+varType+".png");
}

  //getEff5(h_mc,h_dataA,h_dataB,h_dataAB,h_data_split, varType, BINNING+"/"+CORRTYPE, 0.7, 1.1);

//FIXME special for Hwidong
void getEffSpecial(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4, TString varType, TString INPUTDIR_PREFIX, double from, double to) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();

   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity" && varType != "eta") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   h1->SetLineColor(kRed);
   h1->SetFillColor(kRed);
   h2->SetLineColor(kBlue);
   h2->SetFillColor(kBlue);
   h3->SetLineColor(kGreen);
   h3->SetFillColor(kGreen);
   h4->SetLineColor(kBlack);
   h4->SetFillColor(kBlack);
//   h5->SetLineColor(6);
//   h5->SetFillColor(6);

   //if you compare data to data
   h1->Draw("E2");
   h1->GetXaxis()->SetTitle(getTitle(varType, true));
   h2->Draw("E2same");
   h3->Draw("E2same");
   h4->Draw("E2same");
//   h5->Draw("E2same");
   leg->AddEntry(h1,"MC, p_{T} bin 1","f");
   leg->AddEntry(h2,"MC, p_{T} bin 2","f");
   leg->AddEntry(h3,"MC, p_{T} bin 3","f");
   leg->AddEntry(h4,"MC, p_{T} bin 4","f");
//   leg->AddEntry(h5,"MC, binning 4","f");
   leg->Draw("same");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();

   //Save
   cout << "Trying to save" << endl;
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h1->GetName()+varType+".png");
}

//FIXME special for Hwidong
void getEffRatSpecial(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4, TString varType, TString INPUTDIR_PREFIX, double from, double to) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();

   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   h1->SetLineColor(kBlack);
   h1->SetFillColor(kBlack);
   h2->SetLineColor(kBlue);
   h2->SetFillColor(kBlue);
   h2->SetFillStyle(3144);
   h3->SetLineColor(kRed);
   h3->SetFillColor(kRed);
   h3->SetFillStyle(3244);
   h4->SetLineColor(kGreen);
   h4->SetFillColor(kGreen);
   h4->SetFillStyle(3344);


   //if you compare data to data
   h1->Draw("E2");
   h1->GetXaxis()->SetTitle(getTitle(varType, true));
   h2->Draw("E2same");
   h3->Draw("E2same");
   h4->Draw("E2same");
   leg->AddEntry(h1,"Binning 1","f");
   leg->AddEntry(h2,"Binning 2","f");
   leg->AddEntry(h3,"Binning 3","f");
   leg->AddEntry(h4,"Binning 4","f");
   leg->Draw("same");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();

   //Save
   cout << "Trying to save" << endl;
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h1->GetName()+varType+".png");
}


void getEff5(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4, TH1D* h5, TString varType, TString INPUTDIR_PREFIX, double from, double to) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TLegend* leg = MakeLegend();
   TPaveText* pt = MakeTPave();

   TCanvas* c1 = new TCanvas("c1","c1",800,600);
   c1->cd();
   if (varType != "rapidity") c1->SetLogx();
   h1->SetMarkerSize(0);
   h1->SetLineWidth(1.2);
   //range
   if (varType == "pt") h1->GetXaxis()->SetRangeUser(8,180);
   else if (varType == "vtx") h1->GetXaxis()->SetRangeUser(0,20.4);
   else if (varType == "mass") h1->GetXaxis()->SetRangeUser(15, 999);
   h1->GetYaxis()->SetRangeUser(from,to);
   h1->GetXaxis()->SetMoreLogLabels();
   h1->GetYaxis()->SetTitle("#epsilon");
   h1->SetLineColor(kRed);
   h1->SetFillColor(kRed);
   h2->SetMarkerStyle(20);
   h2->SetMarkerSize(1.1);
   h2->SetMarkerColor(kBlack);
   h2->SetLineColor(kBlack);
   h3->SetMarkerStyle(20);
   h3->SetMarkerSize(1.1);
   h3->SetMarkerColor(kGreen);
   h3->SetLineColor(kGreen);
   h4->SetMarkerStyle(20);
   h4->SetMarkerSize(1.1);
   h4->SetMarkerColor(kBlue);
   h4->SetLineColor(kBlue);
   h5->SetMarkerStyle(20);
   h5->SetMarkerSize(1.1);
   h5->SetMarkerColor(kViolet);
   h5->SetLineColor(kViolet);

   //if you compare data to data
   h1->Draw("E2");
   h1->GetXaxis()->SetTitle(getTitle(varType, true));
   h2->Draw("PEsame");
   h3->Draw("PEsame");
   h4->Draw("PEsame");
   h5->Draw("PEsame");
   leg->AddEntry(h1,"MC","f");
   leg->AddEntry(h2,"Data, Run A","pl");
   leg->AddEntry(h3,"Data, Run B","pl");
   leg->AddEntry(h4,"Data, Run A+B combined","pl");
   leg->AddEntry(h5,"Data, Run A+B split","pl");
   leg->Draw("same");
   //if (dataOnly && !(varType == "rrr")) pt->Draw("same");
   //if (varType == "pt") c1->SetLogx();

   //Save
   c1->SaveAs(INPUTDIR_PREFIX+"/"+h1->GetName()+varType+".png");
}

void makeAPlot(TH1F* hmc, TH1F* h_emuPrediction, TString first, TString second, TString ratio_label) {

  cout << hmc->GetName() << endl;

//FIXME need ratio plots
  TCanvas *c1 = new TCanvas(hmc->GetName(), hmc->GetName());
  c1->Draw();
  c1->cd();
  TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
  p1->Draw();
  p1->cd();
  p1->SetBottomMargin(0.01);
  p1->SetGridx(kFALSE);
  p1->SetGridy(kFALSE);
  //p1->SetLogy();

  h_emuPrediction->SetMarkerStyle(22);
  h_emuPrediction->SetMarkerColor(kBlack);
  hmc->SetMarkerStyle(20);
  hmc->SetMarkerColor(kOrange+2);
  hmc->SetLineColor(kOrange+2);
  hmc->GetXaxis()->SetTitle("Bin number");
  //hmc->GetYaxis()->SetTitle("Events");
  hmc->GetYaxis()->SetTitle("#epsilon");
  //hmc->GetYaxis()->SetTitleSize(0.18);
  hmc->Draw("EP");
  h_emuPrediction->Draw("EPsame");
  hmc->SetMaximum(1.3*h_emuPrediction->GetMaximum());

  //cout << "Chi2/ndf  " << hdata->Chi2Test(hmc,"UW CHI2/NDF") << endl;
  TLegend* leg = MakeLegend();
  leg->AddEntry(h_emuPrediction,first,"lp");
  leg->AddEntry(hmc,second,"lp");
  leg->Draw("same");
/*
  std::map<std::string,std::string> beauty_whiches;
  string value[6] = {"20-30 GeV","30-45 GeV","45-60 GeV","60-120 GeV","120-200 GeV","200-1500 GeV"};
  string key[6] = {"retHistabsrap2030","retHistabsrap3045","retHistabsrap4560","retHistabsrap60120","retHistabsrap120200","retHistabsrap2001500"};

  std::pair<string, string> key_value[6] = {make_pair("20-30 GeV","retHistabsrap2030"),make_pair("20-30 GeV","retHistabsrap2030"),make_pair("4560 GeV","retHistabsrap4560"),make_pair("60-120 GeV","retHistabsrap60120"),make_pair("120-200 GeV","retHistabsrap120200"),make_pair("200-1500 GeV","retHistabsrap200")}

  for (int i = 0; i < 6; i++) {
      beauty_whiches.insert(key[i]) = value[i];
     //cout < << endl;
  }

  //TPaveText* pt = MakeTPave();
  //pt->AddText("Overlap-overlap");
  //pt->AddText(TString(beauty_whiches[string(h_mc->GetName())]));
  //pt->Draw("same");
*/

  gStyle->SetOptTitle(0);
  TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.249,0,0,0);
  c1->cd();
  p2->Draw();
  p2->cd();
  //p2->SetLogx();
  p2->SetTopMargin(0.007);
  p2->SetBottomMargin(0.35);
  p2->SetTickx();
  p2->SetGridx(kFALSE);
  p2->SetGridy(kFALSE);
  TH1F* ratio = (TH1F*)h_emuPrediction->Clone();
  hmc->Sumw2();
  h_emuPrediction->Sumw2();
  ratio->Divide(h_emuPrediction,hmc);
  //ratio->Add(hmc,-1); 
  ratio->SetMarkerStyle(22);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetXaxis()->SetTitle("Dielectron Absolute Rapidity");
  ratio->GetYaxis()->SetTitle(ratio_label);
  ratio->GetYaxis()->SetTitleOffset(0.29);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetYaxis()->SetTitleSize(0.18);
  ratio->GetXaxis()->SetTitleSize(0.18);
  ratio->GetYaxis()->SetLabelSize(0.10);
  ratio->GetXaxis()->SetLabelSize(0.15);
  ratio->SetMaximum(1.3);
  ratio->SetMinimum(0.7);
  ratio->Draw("ep");
  p2->Update();
  TLine *line=new TLine();
  line->SetLineColor(kBlue);
  line->SetLineWidth(1);
  line->DrawLine(0,1,2.4,1);
  c1->SaveAs(""+TString(ratio->GetName())+".png"); //_DR53X_HCP2012.png");

}


TLegend*  MakeCustomLegend() {

   TLegend *_leg = new TLegend(0.44, 0.80, 0.79, 0.95);
   _leg->SetTextFont(42);
   _leg->SetTextSize(0.035);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);
   return _leg;

}

TLegend*  MakeLegend() {

   //TLegend *_leg= new TLegend(0.40,0.40,0.65,0.60);
   //TLegend *_leg= new TLegend(0.20,0.70,0.35,0.90);
   TLegend *_leg= new TLegend(0.20,0.15,0.35,0.30);
   //TLegend *_leg= new TLegend(0.17,0.40,0.35,0.60); 
   //TLegend *_leg= new TLegend(0.17,0.20,0.35,0.40); 
   _leg->SetTextFont(72);
   _leg->SetTextSize(0.060);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);
   return _leg;

}

TPaveText* MakeTPave() {
    //TPaveText *_pt = new TPaveText(0.17,0.32,0.35,0.47,"brNDC");
    //TPaveText *_pt = new TPaveText(0.20,0.15,0.35,0.30,"brNDC");
    //TPaveText *_pt = new TPaveText(0.65,0.70,0.85,0.90,"brNDC");
    TPaveText *_pt = new TPaveText(0.65,0.15,0.85,0.30,"brNDC");
    //TPaveText *_pt = new TPaveText(0.17,0.70,0.35,0.90,"brNDC");
    _pt->SetFillColor(0);
    _pt->SetTextFont(72);
    _pt->SetTextSize(0.060);
    _pt->SetBorderSize(0);
    _pt->SetTextAlign(12);
    return _pt;
}

TString getTitle(TString varType, bool isTPplot) {

  if (varType == "pt")  {
     if (isTPplot)  return "Probe p_{T} [GeV]";
     else return "p_{T} [GeV]";
  } else if (varType == "eta") {
     if (isTPplot)  return "Probe #eta";
     else return "#eta";
  } else if (varType == "vtx") {
     return "N_{vtx}";
  } else if (varType == "mass" || varType == "invm") {
     return "M_{#mu#mu} [GeV]"; 
  } else if (varType == "rapidity" || varType == "rap") {
     if (isTPplot)  return "|y|";
  } else if (varType == "angle3D") {
     return "#alpha [rad]"; 
  } else if (varType == "chi2dof") {
     return "#chi^{2}/N_{d.o.f.}"; 
  } else if (varType == "trackerHits") {
     return "N_{tracker} hits"; 
  } else if (varType == "pixelHits") {
     return "N_{pixel} hits";
  } else if (varType == "muonHits") {
     return "N_{muon} hits";
  } else if (varType == "nMatches") {
     return "N_{seg. matches}";
  } else if (varType.Contains("dxy")) {
     return "d_{xy}(BS) [cm]"; 
  } else if (varType.Contains("dz")) {
     return "d_{z}(BS) [cm]";
  } else if (varType == "relPFisoNoEGamma") {
     return "#sum_{i}(p_{T}^{i, ch} + E_{T}^{i,neut})/p_{T}^{#mu}";
  } else if (varType == "vtxTrkProb") {
     return "P_{vertex}(#chi^{2},N_{d.o.f.})"; 
  } else if (varType == "trigMatches") {
     return "N_{tracker}";
  } else if (varType == "absrap2030" || varType == "absrap3045" || varType == "absrap4560" || 
              varType == "absrap60120" || varType == "absrap120200" || varType == "absrap200") {
      return "|Y_{#mu#mu}|";
  } else if (varType.Contains("scEt")) {
      return "E_{T}^{sc} [GeV]";
  } else if (varType.Contains("scEta")) {
      return "#eta^{sc}";
  } else if (varType.Contains("relPFIso03")) {
      return "I/p_{T}";
  } else if (varType.Contains("sigiEtaiEta")) {
      return "#sigma_{i#eta i#eta}"; 
  } else if (varType.Contains("deltaEtaIn")) {
      return "#Delta#eta_{track, SC}";
  } else if (varType.Contains("deltaPhiIn")) {
      return "#Delta#phi_{track, SC}";
  } else if (varType.Contains("HoverE")) {
      return "H/E";
  } else if (varType.Contains("ExpHitsInner")) {
      return "N_{Exp.Hits Inner.}";
  } else if (varType.Contains("OOEOOP")) {
      return "|1/E-1/p|";
  }

  return "LOL_Mistake";
}


//TString WHICHHIST1, TString WHICHHIST2, assume 2 is data
void getDataMC(TH1D* h1, TH1D* h2, bool isData2, bool drawRatio) {


  gStyle->SetEndErrorSize(2);
  gStyle->SetErrorX(0.5);

  TCanvas* c = new TCanvas();
  c->cd();
  TPad *p1 = new TPad("p1", "",0.0,0.242,1.0,1.0,0,0,0);
  if (drawRatio) {
    p1->Draw();
    p1->cd();
    p1->SetBottomMargin(0.013);
    //p1->SetLogx();
  }

  double max = h1->GetMaximum() > h2->GetMaximum() ? h1->GetMaximum() : h2->GetMaximum(); 

  h1->SetMaximum(1.2*max);
  h1->GetYaxis()->SetTitleSize(0.065);
  h1->GetYaxis()->SetTitle("Events/bin");
  h1->SetLineColor(kOrange+3);
  h1->SetFillColor(kOrange-2);
  h1->SetMarkerSize(0);
  h1->SetLineWidth(1.2);
  h1->Draw("E2");
  h2->SetMarkerSize(1.3);
  h2->SetMarkerStyle(20);
  h2->Draw("EPsame");
  TLegend* leg = MakeLegend();
  leg->AddEntry(h1,"MC based estimation","f");
  leg->AddEntry(h2,"Data-driven prediction","lp");
  leg->Draw("same");

//Draw ratios
  if (drawRatio) {
    gStyle->SetOptTitle(0);
    gStyle->SetPadGridY(false);

    TH1D* ratio = (TH1D*)h1->Clone();
    ratio->Divide(h2,h1);

    TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.24,0,0,0);
    c->cd();
    p2->Draw();
    p2->cd();
    //p2->SetLogx();
    p2->SetTopMargin(0.005);
    p2->SetBottomMargin(0.35);
    //p2->SetTickx();
    ratio->GetYaxis()->SetTitle("e#mu/MC");
    ratio->SetStats(kFALSE);
    double ranger = ratio->GetBinError(1) > ratio->GetBinError(ratio->GetXaxis()->GetNbins()-1) ? ratio->GetBinError(1) : ratio->GetBinError(ratio->GetXaxis()->GetNbins()-1);
    ratio->GetYaxis()->SetRangeUser(0.01,1.2*ranger+fabs(ratio->GetMaximum()));
    ratio->GetYaxis()->SetNdivisions(5);
    ratio->GetYaxis()->SetTitleOffset(0.29);
    ratio->GetXaxis()->SetTitle("|Y_{#mu#mu}|");
    ratio->GetXaxis()->SetMoreLogLabels();
    ratio->GetYaxis()->SetTitleSize(0.23);
    ratio->GetXaxis()->SetTitleSize(0.18);
    //control digit sizes
    ratio->GetYaxis()->SetLabelSize(1.2*0.15);
    ratio->GetXaxis()->SetLabelSize(0.15);
    //ratio->SetTickLength(ratio->GetTickLength()*2.);
    ratio->SetFillStyle(3001);
    ratio->SetLineColor(kBlack);
    ratio->SetFillColor(kBlack);
    ratio->SetMarkerSize(0);
    ratio->SetLineWidth(1.2);
    ratio->Draw("E3");
  }

  c->SaveAs(TString(h1->GetName())+".png");

}

//three last parameters are those that are set inside
void clopper_pearson(double num, double den, double alpha=1-0.6827, bool equal_tailed=true, double ratio, double low_rat_err, double up_rat_err) {

    double alpha_min = 0.0;
    if (equal_tailed) {
        alpha_min = alpha/2.;
    } else {
        alpha_min = alpha;
    }
    cout << "Inside clopper pearson " << num << " " << den << endl;

    low_rat_err = 0;
    up_rat_err = 1;

    if (num > 0) {
        low_rat_err = ROOT::Math::beta_quantile(alpha_min, num, den - num + 1);
    }
    if (den - num > 0) {
        up_rat_err = ROOT::Math::beta_quantile_c(alpha_min, num + 1, den - num);
    }
    if (num == 0 && den == 0) {
        ratio = 0.;
    } else {
        ratio = double(num)/den;
    }
    cout << "Ratio inside the clopper pearson " << ratio << endl;
}

/*
  txt[0] = "CMS"; // Preliminary";
  txtSize[0] = 0.042;
  txtX[0] = 0.915;
  txtY[0] = 0.875;
  txtAlign[0] = 21;
  txtFont[0] = 42;
  
  //txt[1] = "19.7 fb^{-1} at #sqrt{s} = 8 TeV";
  txt[1] = "19.7 fb^{-1} ee, 19.7 fb^{-1} #mu#mu, (8 TeV)";
  txtSize[1] = 0.042;
  txtX[1] = 0.40; 
  txtY[1] = 0.83;
  txtAlign[1] = 21;
  txtFont[1] = 42; 

  txtSize[2] = 0.06;
  txtX[2] = 0.70;
  txtY[2] = 0.65;
  txtAlign[2] = 21;
  txtFont[2] = 42;
*/

void drawComparison(bool compareToPOWHEG, TString name, int shaperType, TH1D* hresult, TH1D* hfewz, TH1D* hfewzb, TH1D* hsys, TString RR, TString inputIsEE, bool normalizeToZ) {

  //book the canvas
  TCanvas* c = new TCanvas("c"+TString(hsys->GetName()), "c"+TString(hsys->GetName()), 600, 600);
  c->cd();
  TPaveText *pt = new TPaveText(0.50,0.95,0.87,0.98,"brNDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.035);
  pt->SetBorderSize(0);
  pt->SetTextAlign(12);
  pt->AddText("19.7 fb^{-1} ee and #mu#mu (8 TeV)");
  pt->AddText("");
  //pt->AddText("CMS Preliminary, 19.7 fb^{-1} ee (8 TeV)");
  //pt->AddText("CMS Preliminary, 19.7 fb^{-1} #mu#mu (8 TeV)"); 
  pt->Draw();

  TPaveText *pt2 = new TPaveText(0.17,0.78,0.25,0.97,"brNDC");
  pt2->SetFillColor(0);
  pt2->SetTextFont(61);
  pt2->SetTextSize(0.055);
  pt2->SetBorderSize(0);
  pt2->SetTextAlign(21);
  pt2->AddText("CMS");
  pt2->Draw();

  TPad *c_1 = new TPad("c_1"+TString(hsys->GetName()), "newpad"+TString(hsys->GetName()),0.01,0.05,0.99,0.96);
  c_1->Draw();
  c_1->cd();
  c_1->SetTopMargin(0.01);
  c_1->SetBottomMargin(0.3);
  c_1->SetRightMargin(0.1);
  c_1->SetFillStyle(0);

  TPaveText *pt1 = new TPaveText(0.14,0.75,0.54,0.85,"brNDC");
  pt1->SetFillColor(0);
  pt1->SetTextFont(22);
  pt1->SetTextSize(0.035);
  pt1->SetBorderSize(0);
  pt1->SetTextAlign(12);
  //pt->AddText("CMS Preliminary, 19.7 fb^{-1} ee (8 TeV)");
  //pt->AddText("CMS Preliminary, 19.7 fb^{-1} #mu#mu (8 TeV)"); 
  pt1->AddText(getBinLabel(hsys->GetName(),inputIsEE));

//  changeCentralValues(hresult,hsys);
  hfewz->GetXaxis()->SetTitle("");
  hfewz->GetYaxis()->SetTitleOffset(0.9);
  hfewz->GetXaxis()->SetTitleOffset(1.1);
  hfewz->SetTitle("");
  hfewz->GetYaxis()->SetTitleSize(0.05);
  hfewz->SetMaximum(hfewz->GetMaximum()*1.9);
  hfewz->SetMinimum(0.0);
  if (!normalizeToZ) hfewz->GetYaxis()->SetTitle("d#sigma/d|y| [pb]");
  else hfewz->GetYaxis()->SetTitle("1/#sigma_{Z}d#sigma/d|y|"); 
  hfewz->SetLabelSize(0.0);
  hfewz->GetYaxis()->SetLabelSize(0.030);
  hfewz->SetLineColor(kBlue+1);
  hfewz->SetFillColor(kBlue+1);
  hfewz->SetLineWidth(0);
  hfewz->SetMarkerSize(0);
  hfewz->SetFillStyle(3004);//3244); 
  hfewz->Draw("E2");
  pt1->Draw("same");

  hfewzb->SetLineColor(kRed);
  hfewzb->SetFillColor(kRed);
  hfewzb->SetLineWidth(0);
  hfewzb->SetMarkerSize(0);
  hfewzb->SetFillStyle(3005);//3244);
  hfewzb->Draw("E2same");

  hresult->SetMarkerStyle(20);
  hresult->SetMarkerSize(1.0);
  hresult->Draw("epsame");
  //simplePrintOut(hresult);

  ostringstream tmpstrm;
  TLegend *legend = MakeCustomLegend();
  tmpstrm.str("");
  //if (inputIsEE == "") tmpstrm << "Data (#mu, 2012) " ;
  //else if (inputIsEE == "_EE") tmpstrm << "Data (e, 2012) " ;
  //else tmpstrm << "Data (e, #mu, 2012) " ;
  tmpstrm << "Data ";
  legend->AddEntry(hresult, tmpstrm.str().c_str(), "PL");
  tmpstrm.str("");
  tmpstrm << "FEWZ, NNLO CT10" ;
  legend->AddEntry(hfewz, tmpstrm.str().c_str(), "F");
  tmpstrm.str("");
  tmpstrm << "FEWZ, NNLO NNPDF2.1" ;
  legend->AddEntry(hfewzb, tmpstrm.str().c_str(), "F");
  legend->SetFillColor(0);
  legend->Draw("0");

  c->cd();
  c_2 = new TPad("c_2"+TString(hsys->GetName()), "newpad"+TString(hsys->GetName()),0.01,0.05,0.99,0.32);
  c_2->Draw();
  c_2->cd();
  c_2->SetTopMargin(0.1);
  c_2->SetBottomMargin(0.30);
  c_2->SetRightMargin(0.1);
  c_2->SetFillStyle(0);
  c_2->SetGrid();

  TH1D* hdiff = hresult->Clone();
  hdiff->Divide(hresult, hfewz);
  TH1D* hdiffb = hresult->Clone();
  hdiffb->Divide(hresult, hfewzb);

  double max = hdiff->GetMaximum();
  double min = hdiff->GetMinimum();
  hdiff->GetYaxis()->SetNdivisions(406); //506);
  hdiff->SetMaximum(1.27);//hdiff->GetMaximum()*1.3);
  hdiff->SetMinimum(0.73); //hdiff->GetMinimum()*0.7);
  hdiff->SetLineColor(kBlue+1);
  hdiff->SetMarkerColor(kBlue+1);
  hdiff->SetTitle("");
  //if (inputIsEE == "") hdiff->GetXaxis()->SetTitle("Absolute dimuon rapidity, |y|");
  //else hdiff->GetXaxis()->SetTitle("Absolute dielectron rapidity, |y|"); 
  hdiff->GetXaxis()->SetTitle("Absolute dilepton rapidity |y|");
  hdiff->GetYaxis()->SetTitle("Data/theory");
  hdiff->GetXaxis()->SetTitleSize(0.13);
  hdiff->GetYaxis()->SetTitleSize(0.11);
  hdiff->GetYaxis()->SetTitleOffset(0.4);
  hdiff->SetLineWidth(1.2);
  hdiff->GetXaxis()->SetLabelSize(0.11);
  hdiff->GetYaxis()->SetLabelSize(0.09);
  hdiff->Draw("PE");

  hdiffb->SetLineColor(kRed);
  hdiffb->SetMarkerColor(kRed);
  hdiffb->SetLineWidth(1.2);
  hdiffb->Draw("PEsame");

  //TLine *line=new TLine();
  ////line->SetLineColor(kViolet);
  //line->SetLineWidth(2);
  //line->DrawLine(0,1,2.4,1);

  //c->Print("./EE_M"+TString(hsys->GetName())+name+".pdf");

  if (inputIsEE == "") c->Print("./shapeR_M"+TString(hsys->GetName())+name+".pdf");
  else if (inputIsEE == "_EE") c->Print("./EEshapeR_M"+TString(hsys->GetName())+name+".pdf");
  else c->Print("./CEMushapeR_M"+TString(hsys->GetName())+name+".pdf");


}

void changeCentralValues(TH1D* hfewz, TH1D* hsys) {

   for (int i = 0; i < hfewz->GetNbinsX(); i++) {
        hsys->SetBinContent(i+1,hfewz->GetBinContent(i+1));
        double pdf_err = hfewz->GetBinContent(i+1)*hsys->GetBinError(i+1);
        hsys->SetBinError(i+1,hfewz->GetBinError(i+1)+pdf_err);
    }
}

