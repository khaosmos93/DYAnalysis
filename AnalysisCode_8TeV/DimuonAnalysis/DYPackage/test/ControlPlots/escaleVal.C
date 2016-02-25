void escaleVal() { //TString Run) {

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

  TString Run = "ABCD";
 
  gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/plottools.C");

  TFile* fdata_es = new TFile(" /group/cms/users/asvyatko/BACKUPCOPY_LATEST/root_files_FEWZ_NEWILYA_1D/yields/DY_j22_19712pb/yields1D.root");
  fdata_es->cd();
  TH1D* hdata_es = (TH1D*)gDirectory->Get("peak_data");
  TH1D* hdata_es_bb = (TH1D*)gDirectory->Get("peak_bb_data");
  TH1D* hdata_es_ee = (TH1D*)gDirectory->Get("peak_ee_data");
  TH1D* hdata_es_oo = (TH1D*)gDirectory->Get("peak_oo_data");
  TH1D* hmc_es = (TH1D*)gDirectory->Get("peak_zee");
  TH1D* hmc_es_bb = (TH1D*)gDirectory->Get("peak_bb_zee");
  TH1D* hmc_es_ee = (TH1D*)gDirectory->Get("peak_ee_zee");
  TH1D* hmc_es_oo = (TH1D*)gDirectory->Get("peak_oo_zee");

  hmc_es->Add(peak_ztt);
  hmc_es->Add(peak_zz);
  hmc_es->Add(peak_ww);
  hmc_es->Add(peak_wz);
  hmc_es->Add(peak_ttbar);
  hmc_es->Add(peak_qcd);
  hmc_es->Add(peak_wjets);

  hmc_es_bb->Add(peak_bb_ztt);
  hmc_es_bb->Add(peak_bb_zz);
  hmc_es_bb->Add(peak_bb_ww);
  hmc_es_bb->Add(peak_bb_wz);
  hmc_es_bb->Add(peak_bb_ttbar);
  hmc_es_bb->Add(peak_bb_qcd);
  hmc_es_bb->Add(peak_bb_wjets);

  hmc_es_ee->Add(peak_ee_ztt);
  hmc_es_ee->Add(peak_ee_zz);
  hmc_es_ee->Add(peak_ee_ww);
  hmc_es_ee->Add(peak_ee_wz);
  hmc_es_ee->Add(peak_ee_ttbar);
  hmc_es_ee->Add(peak_ee_qcd);
  hmc_es_ee->Add(peak_ee_wjets);

  hmc_es_oo->Add(peak_oo_ztt);
  hmc_es_oo->Add(peak_oo_zz);
  hmc_es_oo->Add(peak_oo_ww);
  hmc_es_oo->Add(peak_oo_wz);
  hmc_es_oo->Add(peak_oo_ttbar);
  hmc_es_oo->Add(peak_oo_qcd);
  hmc_es_oo->Add(peak_oo_wjets);

  //scale
  hmc_es->Scale(hdata_es->Integral()/hmc_es->Integral());
  hmc_es_bb->Scale(hdata_es_bb->Integral()/hmc_es_bb->Integral());
  hmc_es_ee->Scale(hdata_es_ee->Integral()/hmc_es_ee->Integral());  
  hmc_es_oo->Scale(hdata_es_oo->Integral()/hmc_es_oo->Integral());

  //cout << "Chi2 Test " << hdata_es->Chi2Test(hmc_es,"UW CHI2/NDF") << endl;
  double chisquared = 0.;
  for (int i =0; i < 60; i++) {
     chisquared+=pow(hdata_es->GetBinContent(i+1)-hmc_es->GetBinContent(i+1),2)/pow(hdata_es->GetBinError(i+1),2);
  }
   //cout << "Chi2 test " << sqrt(chisquared)/60. << endl;


  hdata_es->SetMarkerStyle(22);
  hdata_es_bb->SetMarkerStyle(22);
  hdata_es_ee->SetMarkerStyle(22);
  //hmc_es_bb->SetMarkerStyle(22);
  //hmc_es_ee->SetMarkerStyle(22);
  hdata_es_oo->SetMarkerStyle(22);

  hmc_es->SetLineColor(kGreen);
  hmc_es->SetFillColor(kGreen);
  hmc_es_bb->SetLineColor(kGreen);
  hmc_es_ee->SetLineColor(kGreen);
  hmc_es_oo->SetLineColor(kGreen);
  hmc_es_bb->SetFillColor(kGreen);
  hmc_es_ee->SetFillColor(kGreen);
  hmc_es_oo->SetFillColor(kGreen);

//FIXME need ratio plots
//  customPlotter(hmc_es, hdata_es, "bb", Run);
  customPlotter(hmc_es_bb, hdata_es_bb, "bb", Run);
  customPlotter(hmc_es_ee, hdata_es_ee, "ee", Run);
  customPlotter(hmc_es_oo, hdata_es_oo, "oo", Run);


  TFile* fout = new TFile("yields_1PUFEWZ.root","recreate");
  fout->cd();
  hdata_es_bb->Write("hdata_bb");
  hmc_es_bb->Write("hsignal_bb");
  hdata_es_oo->Write("hdata_oo");
  hmc_es_oo->Write("hsignal_oo");
  hdata_es_ee->Write("hdata_ee");
  hmc_es_ee->Write("hsignal_ee");
  hdata_es->Write("hdata");
  hmc_es->Write("hsignal");
  fout->Close();

}


void customPlotter(TH1D* hmc, TH1D* hdata, TString etaclass, TString run) {

  //hdata->Rebin(3);
  //hmc->Rebin(3);
  hmc->Sumw2();
 
  TH1D* hmc_forratio = (TH1D*)hmc->Clone();
  TH1D* hdata_forratio = (TH1D*)hdata->Clone();

  cout << hmc->Integral() << " " << hdata->Integral() << endl;
  

//FIXME need ratio plots
  TCanvas *c1 = new TCanvas(etaclass,etaclass);
  c1->Draw();
  c1->cd();
  TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
  p1->Draw();
  p1->cd();
  p1->SetBottomMargin(0.01);
  p1->SetGridx(kFALSE);
  p1->SetGridy(kFALSE);  
  //p1->SetLogy();
  hmc->SetMaximum(1.3*hdata->GetMaximum());
  hmc->GetYaxis()->SetTitle("Events");
  //hmc->GetXaxis()->SetTitle("m [GeV]");
  hmc->SetLineWidth(1.5);
  hmc->SetMarkerSize(0.9);
  hmc->GetXaxis()->SetRangeUser(60,120); //75,105);
  hmc->Draw("E2");
  hdata->Draw("EPsame");

  int sum = 0;
  for (int i = 0; i < hdata->GetNbinsX(); i++) {
    //cout << "Bin # " << i+1 << " " << hdata->GetBinContent(i+1) - hmc->GetBinContent(i+1) << endl;
    sum += hdata->GetBinContent(i+1) - hmc->GetBinContent(i+1);
  }
  cout << "Excess in norm " << sum/hdata->Integral() << endl;

  TH1D* ratio_bb = (TH1D*)hdata_forratio->Clone();
  hmc_forratio->Sumw2();
  ratio_bb->Divide(hdata_forratio,hmc_forratio);

  //cout << "Chi2/ndf  " << hdata->Chi2Test(hmc,"UW CHI2/NDF") << endl;
  TLegend* leg = MakeLegend();
  leg->AddEntry(hdata,"Data Run"+run+", corrected","lp");
  leg->AddEntry(hmc,"MC, corrected","f");
  leg->Draw("same");
  TPaveText* pt = MakeTPave();
  if (etaclass == "bb") pt->AddText("Barrel-barrel");
  if (etaclass == "ee") pt->AddText("Ebdcap-endcap");
  if (etaclass == "oo") pt->AddText("Overlap-overlap");
  std::ostringstream pprint;
  pprint.str("");
  //pprint<<hdata->Chi2Test(hmc,"UW CHI2/NDF");
  double chisquared = 0.;
  for (int i =0; i < 60; i++) {
     chisquared+=pow(hdata->GetBinContent(i+1)-hmc->GetBinContent(i+1),2)/pow(hdata->GetBinError(i+1),2);
  }
  pprint<<chisquared/60.;
  pt->AddText("#chi^{2}/ndf = "+TString(pprint.str()));
  pt->Draw("same");

  gStyle->SetOptTitle(0);
  TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.249,0,0,0);
  c1->cd();
  p2->Draw();
  p2->cd();
  p2->SetGridx(kFALSE);
  p2->SetGridy(kFALSE);
  //p2->SetLogx();
  p2->SetTopMargin(0.007);
  p2->SetBottomMargin(0.35);
  p2->SetTickx();
  ratio_bb->GetYaxis()->SetTitle("R_{Data/MC}");
  ratio_bb->SetStats(kFALSE);
  ratio_bb->GetXaxis()->SetRangeUser(60,120); //75,105);
  ratio_bb->GetYaxis()->SetTitleOffset(0.29);
  ratio_bb->GetXaxis()->SetMoreLogLabels();
  ratio_bb->GetXaxis()->SetTitle("m [GeV]");
  ratio_bb->GetYaxis()->SetTitleSize(0.15);
  ratio_bb->GetXaxis()->SetTitleSize(0.15);
  ratio_bb->GetYaxis()->SetLabelSize(0.10);
  ratio_bb->GetXaxis()->SetLabelSize(0.15);
  ratio_bb->SetMarkerSize(0.9);
  ratio_bb->SetMaximum(1.12);
  ratio_bb->SetMinimum(0.88);
  ratio_bb->SetLineColor(kBlack);
  ratio_bb->SetMarkerStyle(20);
  ratio_bb->Draw("ep");
  p2->Update();
  TLine *line=new TLine();
  line->SetLineColor(kBlue);
  line->SetLineWidth(1);
  line->DrawLine(60,1,120,1);//75,1,106.5,1);
  c1->SaveAs(etaclass+run+".pdf"); //_DR53X_HCP2012.png");
}
