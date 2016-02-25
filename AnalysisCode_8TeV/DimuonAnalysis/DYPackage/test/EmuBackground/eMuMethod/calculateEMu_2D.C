#include <map.h>

void calculateEMu_2D() {

 TString regime = ""; //"_NODATANORM";
 bool subtractQCD = true;
 bool closureTest = false;

 //get emu data and MC
 TFile* f = new TFile("true2eBkgDataPoints_2D_OS_try2.root");
 f->cd();
 //numbering is shifted 
 TH1F* h_emuData020 = (TH1F*)data_dmdyHistv20->Clone();
 THStack* hs_emuMC020 = (THStack*)eMuDMDYTot20->Clone();
 TH1F* h_emuMC020 = stack_to_TH(hs_emuMC020);

 TH1F* h_emuData2030 = (TH1F*)data_dmdyHistv30->Clone();
 THStack* hs_emuMC2030 = (THStack*)eMuDMDYTot30->Clone();
 TH1F* h_emuMC2030 = stack_to_TH(hs_emuMC2030);

 TH1F* h_emuData3045 = (TH1F*)data_dmdyHistv45->Clone();
 THStack* hs_emuMC3045 = (THStack*)eMuDMDYTot45->Clone();
 TH1F* h_emuMC3045 = stack_to_TH(hs_emuMC3045);

 TH1F* h_emuData4560 = (TH1F*)data_dmdyHistv60->Clone();
 THStack* hs_emuMC4560 = (THStack*)eMuDMDYTot60->Clone();
 TH1F* h_emuMC4560 = stack_to_TH(hs_emuMC4560);

 TH1F* h_emuData60120 = (TH1F*)data_dmdyHistv120->Clone();
 THStack* hs_emuMC60120 = (THStack*)eMuDMDYTot120->Clone();
 TH1F* h_emuMC60120 = stack_to_TH(hs_emuMC60120);

 TH1F* h_emuData120200 = (TH1F*)data_dmdyHistv200->Clone();
 THStack* hs_emuMC120200 = (THStack*)eMuDMDYTot200->Clone();
 TH1F* h_emuMC120200 = stack_to_TH(hs_emuMC120200);

 //this one has 10 bins 
 TH1F* h_emuData2001500 = (TH1F*)data_dmdyHistv1500->Clone();
 THStack* hs_emuMC2001500 = (THStack*)eMuDMDYTot1500->Clone();
 TH1F* h_emuMC2001500 = stack_to_TH(hs_emuMC2001500);

//
//
//TMP
//
//
 //FIXME do we still need this?
 //this is a tmp hack to just make 24 rapidity bins in the last mass bin
 TH1F* h_emuData2001500_24s = new TH1F("h_emuData2001500_24s","h_emuData2001500_24s",12,0,2.4);
 for (int i = 0; i < 10; i++) {
   h_emuData2001500_24s->SetBinContent(i+1,h_emuData2001500->GetBinContent(i+1));
   h_emuData2001500_24s->SetBinError(i+1,h_emuData2001500->GetBinError(i+1));
 } 
 h_emuData2001500_24s->SetBinContent(11,h_emuData2001500_24s->GetBinContent(10)*0.9); 
 h_emuData2001500_24s->SetBinContent(12,h_emuData2001500_24s->GetBinContent(10)*1.4);

 //custom smoothing
 h_emuData2001500_24s->Smooth(1000,"R");
 
 TH1F* h_emuMC2001500_24s = new TH1F("h_emuMC2001500_24s","h_emuMC2001500_24s",12,0,2.4);
 for (int i = 0; i < 10; i++) {
    h_emuMC2001500_24s->SetBinContent(i+1, h_emuMC2001500->GetBinContent(i+1));
    h_emuMC2001500_24s->SetBinError(i+1, h_emuMC2001500->GetBinError(i+1));
 } 
 h_emuMC2001500_24s->SetBinContent(11,h_emuMC2001500_24s->GetBinContent(10)*0.9);
 h_emuMC2001500_24s->SetBinContent(12,h_emuMC2001500_24s->GetBinContent(10)*0.8);

 //custom smoothing
  h_emuMC2001500_24s->Smooth(1000,"R");

 
 //recover the QCD contribution
 TFile* f2 = new TFile("true2eBkgDataPoints_2D_SS_try2.root"); //true2eBkgDataPoints_2D_SS_2014.root"); //true2eBkgDataPoints_2D_SS_try2.root");
 f2->cd();
 TH1F* h_emuData_SS020 = (TH1F*)data_dmdyHistv20->Clone();
 THStack* hs_emuMC_SS020 = (THStack*)eMuDMDYTot20->Clone();
 TH1F* h_emuMC_SS020 = stack_to_TH(hs_emuMC_SS020);

 TH1F* h_emuData_SS2030 = (TH1F*)data_dmdyHistv30->Clone();
 THStack* hs_emuMC_SS2030 = (THStack*)eMuDMDYTot30->Clone();
 TH1F* h_emuMC_SS2030 = stack_to_TH(hs_emuMC_SS2030);

 TH1F* h_emuData_SS3045 = (TH1F*)data_dmdyHistv45->Clone();
 THStack* hs_emuMC_SS3045 = (THStack*)eMuDMDYTot45->Clone();
 TH1F* h_emuMC_SS3045 = stack_to_TH(hs_emuMC_SS3045);

 TH1F* h_emuData_SS4560 = (TH1F*)data_dmdyHistv60->Clone();
 THStack* hs_emuMC_SS4560 = (THStack*)eMuDMDYTot60->Clone();
 TH1F* h_emuMC_SS4560 = stack_to_TH(hs_emuMC_SS4560);

 TH1F* h_emuData_SS60120 = (TH1F*)data_dmdyHistv120->Clone();
 THStack* hs_emuMC_SS60120 = (THStack*)eMuDMDYTot120->Clone();
 TH1F* h_emuMC_SS60120 = stack_to_TH(hs_emuMC_SS60120);

 TH1F* h_emuData_SS120200 = (TH1F*)data_dmdyHistv200->Clone();
 THStack* hs_emuMC_SS120200 = (THStack*)eMuDMDYTot200->Clone();
 TH1F* h_emuMC_SS120200 = stack_to_TH(hs_emuMC_SS120200);

 TH1F* h_emuData_SS2001500 = (TH1F*)data_dmdyHistv1500->Clone();
 THStack* hs_emuMC_SS2001500 = (THStack*)eMuDMDYTot1500->Clone();
 TH1F* h_emuMC_SS2001500 = stack_to_TH(hs_emuMC_SS2001500);

 //missing QCD
 TH1F* missing_QCD020 = (TH1F*)h_emuData_SS020->Clone();
 missing_QCD020->Add(h_emuMC_SS020,-1);
 TH1F* missing_QCD_emu020 = (TH1F*)missing_QCD020->Clone();
 if (subtractQCD) h_emuData020->Add(missing_QCD_emu020,-1);

 TH1F* missing_QCD2030 = (TH1F*)h_emuData_SS2030->Clone();
 missing_QCD2030->Add(h_emuMC_SS2030,-1); 
 TH1F* missing_QCD_emu2030 = (TH1F*)missing_QCD2030->Clone(); 
 if (subtractQCD) h_emuData2030->Add(missing_QCD_emu2030,-1);

 TH1F* missing_QCD3045 = (TH1F*)h_emuData_SS3045->Clone();
 missing_QCD3045->Add(h_emuMC_SS3045,-1);
 TH1F* missing_QCD_emu3045 = (TH1F*)missing_QCD3045->Clone();
 if (subtractQCD) h_emuData3045->Add(missing_QCD_emu3045,-1);

 TH1F* missing_QCD4560 = (TH1F*)h_emuData_SS4560->Clone();
 missing_QCD4560->Add(h_emuMC_SS4560,-1);
 TH1F* missing_QCD_emu4560 = (TH1F*)missing_QCD4560->Clone();
 if (subtractQCD) h_emuData4560->Add(missing_QCD_emu4560,-1);

 TH1F* missing_QCD60120 = (TH1F*)h_emuData_SS60120->Clone();
 missing_QCD60120->Add(h_emuMC_SS60120,-1);
 TH1F* missing_QCD_emu60120 = (TH1F*)missing_QCD60120->Clone();
 if (subtractQCD) h_emuData60120->Add(missing_QCD_emu60120,-1);

 TH1F* missing_QCD120200 = (TH1F*)h_emuData_SS120200->Clone();
 missing_QCD120200->Add(h_emuMC_SS120200,-1);
 TH1F* missing_QCD_emu120200 = (TH1F*)missing_QCD120200->Clone();
 //if (subtractQCD) h_emuData120200->Add(missing_QCD_emu120200,-1);

 TH1F* missing_QCD2001500 = (TH1F*)h_emuData_SS2001500->Clone();
 missing_QCD2001500->Add(h_emuMC_SS2001500,-1);
 TH1F* missing_QCD_emu2001500 = (TH1F*)missing_QCD2001500->Clone();
 //if (subtractQCD) h_emuData2001500->Add(missing_QCD_emu2001500,-1);

//
//
//
//TMP
//
//
//
 TH1F* missing_QCD2001500_24s = (TH1F*)h_emuData120200->Clone();
 TH1F* missing_QCD_emu2001500_24s = (TH1F*)h_emuData120200->Clone();

 gROOT->ProcessLine(".L ../../tools/histotools.C");
 //TH1F* sliceCombiner(TH1F* h_2030, TH1F* h_3045, TH1F* h_4560, TH1F* h_60120, TH1F* h_120200, TH1F* h_2001500, bool flag) {

 //insude the combiner we have to unfold the 10 bin to 24 bin last slice histo

if (!closureTest) {
 //FIXME Smoothing
 missing_QCD_emu020->Smooth(1000,"R");
 missing_QCD_emu2030->Smooth(1000,"R");
 missing_QCD_emu3045->Smooth(1000,"R");
 missing_QCD_emu4560->Smooth(1000,"R");
 missing_QCD_emu60120->Smooth(1000,"R");
 missing_QCD_emu120200->Smooth(1000,"R");
 missing_QCD_emu2001500->Smooth(1000,"R"); 

 h_emuData020->Smooth(1000,"R");
 h_emuData2030->Smooth(1000,"R");
 h_emuData3045->Smooth(1000,"R");
 h_emuData4560->Smooth(1000,"R");
 h_emuData60120->Smooth(1000,"R");
 h_emuData120200->Smooth(1000,"R");
 h_emuData2001500_24s->Smooth(1000,"R");

 h_emuMC020->Smooth(1000,"R");
 h_emuMC2030->Smooth(1000,"R");
 h_emuMC3045->Smooth(1000,"R");
 h_emuMC4560->Smooth(1000,"R");
 h_emuMC60120->Smooth(1000,"R");
 h_emuMC120200->Smooth(1000,"R");
 h_emuMC2001500_24s->Smooth(1000,"R");
}

 TH1F* h_emuData = sliceCombiner(h_emuData2030,h_emuData3045,h_emuData4560,h_emuData60120,h_emuData120200,h_emuData2001500_24s,true);
 TH1F* h_emuMC = sliceCombiner(h_emuMC2030,h_emuMC3045,h_emuMC4560,h_emuMC60120,h_emuMC120200,h_emuMC2001500_24s,true);
 TH1F* missing_QCD_emu = sliceCombiner(missing_QCD_emu2030,missing_QCD_emu3045,missing_QCD_emu4560,missing_QCD_emu60120,missing_QCD_emu120200,missing_QCD_emu2001500_24s,true);

 if (closureTest) h_emuData = h_emuMC;

 //get ee MC 
 TFile* g = new TFile("../../Inputs/rawYield/DYspectrum_Rap_v2.root");
 g->cd();
 //FOR SYSTEMATICS PUSPOSES
 TH1F* h_mumuSignal = (TH1F*)hsig_fewz->Clone();
 TH1F* h_mumuMC = (TH1F*)hDYtautau->Clone();
 h_mumuMC->Add(hWlepton);
 h_mumuMC->Add(hdiboson);
 TH1F* httbar_mc = (TH1F*)httbar_mc->Clone();
 h_mumuMC->Add(httbar_mc);
 h_mumuSignal->Add(h_mumuMC,-1);

 //h_mumuMC->Add(missing_QCD);
 //FIXME get slices first
 TH1F* h_mumuMC_slice1 = get2DSlice(h_mumuMC, "absrap2030",false,true);
 TH1F* h_mumuMC_slice2 = get2DSlice(h_mumuMC, "absrap3045",false,true);
 TH1F* h_mumuMC_slice3 = get2DSlice(h_mumuMC, "absrap4560",false,true);
 TH1F* h_mumuMC_slice4 = get2DSlice(h_mumuMC, "absrap60120",false,true);
 TH1F* h_mumuMC_slice5 = get2DSlice(h_mumuMC, "absrap120200",false,true);
 TH1F* h_mumuMC_slice6 = get2DSlice(h_mumuMC, "absrap200",false,true);
if (!closureTest) {
 h_mumuMC_slice1->Smooth(100,"R");
 h_mumuMC_slice2->Smooth(100,"R");
 h_mumuMC_slice3->Smooth(100,"R");
 h_mumuMC_slice4->Smooth(100,"R");
 h_mumuMC_slice5->Smooth(100,"R");
 h_mumuMC_slice6->Smooth(100,"R");
}
 h_mumuMC = sliceCombiner(h_mumuMC_slice1,h_mumuMC_slice2,h_mumuMC_slice3,h_mumuMC_slice4,h_mumuMC_slice5,h_mumuMC_slice6,true);  //TH1F* h_2030, TH1F* h_3045, TH1F* h_4560, TH1F* h_60120, TH1F* h_120200, TH1F* h_2001500, bool flag) 

 //TFile* f = new TFile("test.root","recreate"); 
 //f->cd();
 //h_mumuMC->Write("data");
 //f->Close(); 

 TH1F* h_emuPrediction_full = (TH1F*)h_mumuMC->Clone();
 TH1F* h_emuMC2 = (TH1F*)h_emuMC->Clone();
 h_emuMC2->Divide(h_emuMC,h_emuData);
 h_emuPrediction_full->Divide(h_emuPrediction_full,h_emuMC2);

  TH1D* hsyst2D= (TH1D*)h_mumuMC->Clone();

 for (int i = 0; i < 156; i++) {
      if (h_emuPrediction_full->GetBinContent(i+1) != 0) h_emuPrediction_full->SetBinError(i+1,h_emuPrediction_full->GetBinContent(i+1)*sqrt(pow((h_emuPrediction_full->GetBinContent(i+1)-h_mumuMC->GetBinContent(i+1))/(2.*h_emuPrediction_full->GetBinContent(i+1)),2)+pow(h_emuPrediction_full->GetBinError(i+1)/h_emuPrediction_full->GetBinContent(i+1),2)));
      h_emuPrediction_full->SetBinError(i+1,h_emuPrediction_full->GetBinError(i+1));
      if (h_mumuMC->GetBinContent(i+1) != 0) h_mumuMC->SetBinError(i+1,h_mumuMC->GetBinContent(i+1)*sqrt(pow(h_mumuMC->GetBinError(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.14*httbar_mc->GetBinContent(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.08*hDYtautau->GetBinContent(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.09*(hdiboson->GetBinContent(i+1)+hWlepton->GetBinContent(i+1))/h_mumuMC->GetBinContent(i+1),2)));
      hsyst2D->SetBinContent(i+1,h_emuPrediction_full->GetBinError(i+1)/h_mumuSignal->GetBinContent(i+1));
      hsyst2D->SetBinError(i+1,0.0000000001);
  }
 TFile* fsyst2D = new TFile("systematics2D.root","recreate");
 fsyst2D->cd();
 hsyst2D->Write("hsyst2D");
 fsyst2D->Close();

 if (regime != "_NODATANORM") {

 gROOT->ProcessLine(".L ../../tools/histotools.C");
 gROOT->ProcessLine(".L ../../tools/plottools.C");

 TH1F* h1_mc = get2DSlice(h_mumuMC, "absrap2030",false,true);
 TH1F* h1_dd = get2DSlice(h_emuPrediction_full, "absrap2030",false,true);
 TH1F* h2_mc = get2DSlice(h_mumuMC, "absrap3045",false,true);
 TH1F* h2_dd = get2DSlice(h_emuPrediction_full, "absrap3045",false,true); 
 TH1F* h3_mc = get2DSlice(h_mumuMC, "absrap4560",false,true);
 TH1F* h3_dd = get2DSlice(h_emuPrediction_full, "absrap4560",false,true);
 TH1F* h4_mc = get2DSlice(h_mumuMC, "absrap60120",false,true);
 TH1F* h4_dd = get2DSlice(h_emuPrediction_full, "absrap60120",false,true);
 TH1F* h5_mc = get2DSlice(h_mumuMC, "absrap120200",false,true);
 TH1F* h5_dd = get2DSlice(h_emuPrediction_full, "absrap120200",false,true); 
 TH1F* h6_mc = get2DSlice(h_mumuMC, "absrap200",false,true);
 TH1F* h6_dd = get2DSlice(h_emuPrediction_full, "absrap200",false,true);

 if(!closureTest) { 
  h1_dd->Smooth(3,"R");
  h1_mc->Smooth(3,"R");
  h3_dd->Smooth(3,"R");
  h3_mc->Smooth(3,"R");
  h5_dd->Smooth(3,"R");
  h5_mc->Smooth(3,"R");

  makeAPlot(h1_mc,h1_dd);  //h_mumuMC, h_emuPrediction_full);
  makeAPlot(h2_mc,h2_dd); 
  makeAPlot(h3_mc,h3_dd); 
  makeAPlot(h4_mc,h4_dd); 
  makeAPlot(h5_mc,h5_dd); 
  makeAPlot(h6_mc,h6_dd); 
 } else {
  //All mumu plots
  TCanvas* c1 = new TCanvas("EmuPrediction");
  c1->cd();
  c1->SetLogx();
  h_emuPrediction_full->SetMarkerStyle(22);
  h_emuPrediction_full->SetMarkerColor(kBlack);
  //h_mumuMC->Scale(h_emuPrediction_full->Integral(10,40)/h_mumuMC->Integral(10,40));
  h_mumuMC->SetMarkerStyle(20);
  h_mumuMC->SetMarkerColor(kOrange+2);
  h_mumuMC->SetLineColor(kOrange+2);
  //h_mumuMC->SetFillColor(kOrange+2);
  h_mumuMC->GetXaxis()->SetTitle("Mass-rapidity index");
  h_mumuMC->GetYaxis()->SetTitle("Events");
  h_mumuMC->GetXaxis()->SetMoreLogLabels();
  h_mumuMC->Draw("P");
  h_emuPrediction_full->Draw("Psame");
  c1->SaveAs("EmuPrediction.png");

  TCanvas* c3 = new TCanvas("EmuPrediction_ratio");
  c3->cd();
  c3->SetLogx();
  TH1D* ratio = (TH1D*)h_mumuMC->Clone();
  h_mumuMC->Sumw2();
  h_emuPrediction_full->Sumw2();
  ratio->Divide(h_emuPrediction_full,h_mumuMC);
  ratio->SetMarkerStyle(22);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetXaxis()->SetTitle("Mass-rapidity index");
  ratio->GetYaxis()->SetTitle("N_{e#mu-pred}/N_{MC}");
  ratio->Draw("PE");
  c3->SaveAs("EmuPrediction_ratio.png");
 }

 //makeAPlot(h_mumuMC,h_emuPrediction_full);
 }//plot if not NONORM

 //SAVE
 TFile* m = new TFile("emu_prediction_2D.root","recreate");
 m->cd();
 h_emuPrediction_full->Write("emu_full");
 h_emuPrediction_full->Write("emu");
 m->Close();

}

void makeAPlot(TH1F* hmc, TH1F* h_emuPrediction) {

  cout << hmc->GetName() << endl;

//FIXME nmumud ratio plots
  TCanvas *c1 = new TCanvas(hmc->GetName(), hmc->GetName());
  c1->Draw();
  c1->cd();
  TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
  p1->Draw();
  p1->cd();
  p1->SetBottomMargin(0.02);
  p1->SetGridx(kFALSE);
  p1->SetGridy(kFALSE);
  //p1->SetLogy();

  h_emuPrediction->SetMarkerStyle(22);
  h_emuPrediction->SetMarkerColor(kBlack);
  hmc->SetMarkerStyle(20);
  hmc->SetMarkerColor(kOrange+2);
  hmc->SetLineColor(kOrange+2);
  hmc->GetXaxis()->SetTitle("Bin number");
  hmc->GetYaxis()->SetTitle("Events");
  hmc->Draw("EP");

  TAxis* ay_ = hmc->GetYaxis();
  ay_->SetNdivisions(410);
  ay_->SetLabelSize(ay_->GetLabelSize()/1.2);

  TAxis* ax_ = hmc->GetXaxis();
  ax_->SetLabelSize(0);


  h_emuPrediction->Draw("EPsame");
  hmc->SetMaximum(1.3*h_emuPrediction->GetMaximum());

  //cout << "Chi2/ndf  " << hdata->Chi2Test(hmc,"UW CHI2/NDF") << endl;
  TLegend* leg = MakeLegend();
  leg->AddEntry(h_emuPrediction,"Data-driven","lp");
  leg->AddEntry(hmc,"MC","lp");
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
  ratio->SetMarkerStyle(22);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetXaxis()->SetTitle("Absolute dimuon rapidity |y|");
  ratio->GetYaxis()->SetTitle("N_{e#mu-pred}/N_{MC}");
  ratio->GetYaxis()->SetTitleOffset(0.29);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetYaxis()->SetTitleSize(0.17);
  ratio->GetXaxis()->SetTitleSize(0.17);
  ratio->GetYaxis()->SetLabelSize(0.10);
  ratio->GetXaxis()->SetLabelSize(0.15);
  ratio->SetMaximum(1.9);
  ratio->SetMinimum(0.1);
  ratio->Draw("ep");

  TAxis* ay_ = ratio->GetYaxis();
  ay_->SetNdivisions(406);

  p2->Update();
  TLine *line=new TLine();
  line->SetLineColor(kBlue);
  line->SetLineWidth(1);
  line->DrawLine(0,1,2.4,1);
  //c1->SaveAs(etaclass+run+".pdf"); //_DR53X_HCP2012.png");

}

TH1F* stack_to_TH(THStack* hs) {

   TList *histos = hs->GetHists();
   TH1F *sum;// = (TH1F*)prototype->Clone();
   TIter next(histos);
   TH1F *hist;
   int nhists = 0;
   while ((hist =(TH1F*)next())) {
     //cout << "Adding " << hist->GetName() << endl;
     if (nhists == 0) {
      sum = hist;
     } else {
      sum->Add(hist);
     }
     nhists++;
   }
  return sum; 
}

TH1F* customGetFromStack(THStack* hs, TString except) {

 TIter next(hs->GetHists());
 TH1F* h;
 TH1F* h_eeMC;
 int count = 0;
 while ((h=(TH1F*)next())) {
    //cout << h->GetName() << endl;
    if (TString(h->GetName()) != except) { 
        cout << "X Adding "  << h->GetName() << endl;
        if (count == 0) h_eeMC = (TH1F*)h->Clone();
        else h_eeMC->Add(h);
    }
    count++;
 }
 return h_eeMC; 

}

TH1F* getSpecificHist(THStack* hs, TString selectThis) {

 TIter next(hs->GetHists());
 TH1F* h;
 TH1F* h_return;
 int count = 0;
 while ((h=(TH1F*)next())) {
    //cout << h->GetName() << endl;
    if (TString(h->GetName()).Contains(selectThis) && !selectThis.Contains("histodiboson")) {
        h_return = (TH1F*)h->Clone();
    } else {
        if (TString(h->GetName()).Contains("histowz") || TString(h->GetName()).Contains("histozz") || TString(h->GetName()).Contains("histoww")) {
           if (count == 0) h_return = (TH1F*)h->Clone();
           else h_return->Add(h);
           count++;
        }
    }
 }
 return h_return;

}
