void dressingValidator() {

  TString channel = "_MuMu"; //"_EE";//"_MuMu";

  TFile* f = new TFile("../root_files_preFSR"+channel+"/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants1D_PU.root");
  f->cd();
  TH1D* hpreFSR = new TH1D(*yieldsMcPreFsrGenDETFIArray);
  TH1D* hpostFSR = new TH1D(*yieldsMcPostFsrGenDETFIArray);

  TFile* ff = new TFile("../root_files_R01_emothers"+channel+"/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants1D_PU.root");
  ff->cd();
  TH1D* hdressedR01 = new TH1D(*yieldsMcPreFsrGenDETFIArray);

  TFile* ff = new TFile("../root_files_R4PI_emothers"+channel+"/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants1D_PU.root");
  ff->cd();
  TH1D* hdressedR4PI = new TH1D(*yieldsMcPreFsrGenDETFIArray);

  //extrapolate missing sample
  hpreFSR->SetBinContent(1,hpreFSR->GetBinContent(2));
  hpreFSR->SetBinError(1,hpreFSR->GetBinError(2));
  hdressedR4PI->SetBinContent(1,hdressedR4PI->GetBinContent(2));
  hdressedR4PI->SetBinError(1,hdressedR4PI->GetBinError(2));


  const int nbin = 41;
  double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};
  TH1D* hpreFSR_mass = new TH1D("hpreFSR_mass","hpreFSR_mass",nbin,mass_xbin);
  TH1D* hpostFSR_mass = new TH1D("hpostFSR_mass","hpostFSR_mass",nbin,mass_xbin);
  TH1D* hdressedR4PI_mass = new TH1D("hdressedR4PI_mass","hdressedR4PI_mass",nbin,mass_xbin);
  TH1D* hdressedR01_mass = new TH1D("hdressedR01_mass","hdressedR01_mass",nbin,mass_xbin);
  for (int i = 0; i < hpreFSR->GetNbinsX(); i++) {
      hpreFSR_mass->SetBinContent(i+1,hpreFSR->GetBinContent(i+1));
      hpreFSR_mass->SetBinError(i+1,hpreFSR->GetBinError(i+1));
      hpostFSR_mass->SetBinContent(i+1,hpostFSR->GetBinContent(i+1));
      hpostFSR_mass->SetBinError(i+1,hpostFSR->GetBinError(i+1));
      hdressedR4PI_mass->SetBinContent(i+1,hdressedR4PI->GetBinContent(i+1));
      hdressedR4PI_mass->SetBinError(i+1,hdressedR4PI->GetBinError(i+1));
      hdressedR01_mass->SetBinContent(i+1,hdressedR01->GetBinContent(i+1));
      hdressedR01_mass->SetBinError(i+1,hdressedR01->GetBinError(i+1));
  }

  //validation test: postFSR vs 4pi dressing
  TCanvas* c = new TCanvas("val1");
  c->cd();
  hpreFSR_mass->GetXaxis()->SetTitle("m [GeV]");
  if (channel == "") {
   hpreFSR_mass->GetYaxis()->SetTitle("Dielectron events");
  } else {
   hpreFSR_mass->GetYaxis()->SetTitle("Dimuon events"); 
  }
  hpreFSR_mass->Draw("hist");
  hdressedR4PI_mass->SetLineColor(kRed);
  hdressedR4PI_mass->Draw("histsame"); 

  //hdressedR4PI->SetBinContent(32,hpreFSR->GetBinContent(32));

  //add legend
  TLegend *leg= new TLegend(0.20,0.15,0.35,0.30);
  leg->SetTextFont(72);
  leg->SetTextSize(0.060);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(hpostFSR_mass,"Standard pre-FSR","l");
  leg->AddEntry(hdressedR4PI_mass,"Dressed in 4#pi cone","l");
  leg->Draw("same");
  //if (channel == "") c->SaveAs("closureEE.png");

  //pre-FSR, post-FSR and dressed R01
  hpostFSR_mass->SetBinContent(1,hpostFSR_mass->GetBinContent(2));
  hpostFSR_mass->SetBinError(1,hpostFSR_mass->GetBinError(2));
  hdressedR01_mass->SetBinContent(1,hdressedR01_mass->GetBinContent(2));
  hdressedR01_mass->SetBinError(1,hdressedR01_mass->GetBinError(2));

  //rebin quantities for 6 mass bins
  //double mass_xbin2[7] = {1,3,6,9,22,30,40};
  //hpostFSR->Rebin(6,"hpostFSR_new",mass_xbin2);
  //hpreFSR->Rebin(6,"hpreFSR_new",mass_xbin2);
  //hdressedR01->Rebin(6,"hdressedR01_new",mass_xbin2);

  //cout << hdressedR01_new->GetNbinsX() << endl;

  //ratio corrections
  TH1D* hpostpreFSR_mass = (TH1D*)hpostFSR_mass->Clone();
  hpostpreFSR_mass->Divide(hpostFSR_mass,hpreFSR_mass);

  TH1D* hpostFSRdressed_mass = (TH1D*)hpostFSR_mass->Clone();
  hpostFSRdressed_mass->Divide(hpostFSR_mass,hdressedR01_mass);

  TCanvas* c = new TCanvas("val2");
  c->cd();
  hpostFSR_mass->GetXaxis()->SetTitle("m_{#mu#mu} [GeV]");
  if (channel == "") {
   hpostFSR_mass->GetYaxis()->SetTitle("Dielectron events");
  } else {
   hpostFSR_mass->GetYaxis()->SetTitle("Dimuon events");
  }
  hpostFSR_mass->Draw("hist");
  hdressedR01_mass->SetLineColor(kRed);
  hdressedR01_mass->Draw("histsame");
  hpreFSR_mass->SetLineColor(kBlue);
  hpreFSR_mass->Draw("histsame");

  //add legend
  TLegend *leg2= new TLegend(0.20,0.15,0.35,0.30);
  leg2->SetTextFont(72);
  leg2->SetTextSize(0.060);
  leg2->SetBorderSize(0);
  leg2->SetFillColor(0);
  leg2->AddEntry(hpostFSR_mass,"Standard post-FSR","l");
  leg2->AddEntry(hpreFSR_mass,"Standard pre-FSR","l");
  leg2->AddEntry(hdressedR01_mass,"Dressed in #DeltaR < 0.1 cone","l");
  leg2->Draw("same");
  //if (channel == "") c->SaveAs("yieldsEE.png");

  TCanvas* c3 = new TCanvas("val3");
  c3->cd();
  hpostpreFSR_mass->GetXaxis()->SetTitle("m_{#mu#mu} [GeV]");
  if (channel == "") {
   hpostpreFSR_mass->GetYaxis()->SetTitle("FSR correction"); //Dielectron event ratios");
  } else {
   hpostpreFSR_mass->GetYaxis()->SetTitle("FSR correction"); //Dimuon event ratios");
  }
  hpostpreFSR_mass->GetYaxis()->SetRangeUser(0.7,3.0);
  hpostpreFSR_mass->SetMarkerStyle(22);
  hpostpreFSR_mass->GetXaxis()->SetMoreLogLabels();
  hpostpreFSR_mass->Draw("PE");
  hpostFSRdressed_mass->SetLineColor(kRed);
  hpostFSRdressed_mass->SetMarkerColor(kRed);
  hpostFSRdressed_mass->SetMarkerStyle(22);
  hpostFSRdressed_mass->Draw("PEsame");

  //add legend
  TLegend *leg3= new TLegend(0.20,0.15,0.35,0.30);
  leg3->SetTextFont(72);
  leg3->SetTextSize(0.060);
  leg3->SetBorderSize(0);
  leg3->SetFillColor(0);
  leg3->AddEntry(hpostFSRdressed_mass,"N_{post-FSR}/N_{dressed, #DeltaR<0.1}","lp");
  leg3->AddEntry(hpostpreFSR_mass,"N_{post-FSR}/N_{pre-FSR}","lp");
  leg3->Draw("same");
  //if (channel == "") c3->SaveAs("ratiosEE.png");

}
