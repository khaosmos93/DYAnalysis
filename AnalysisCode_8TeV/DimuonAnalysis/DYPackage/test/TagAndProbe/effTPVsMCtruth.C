#include "./include/getSimplePlot.C"
#include "./include/getFitCurve.C"
#include "./include/getFitHist.C"
#include "./include/getFitPlot.C"

void effTPVsMCtruth() {

  gROOT->ProcessLine(".L ../tools/plottools.C");
  gROOT->ProcessLine(".L ../tools/histotools.C");

  //get 1D pT and Eta distributions
  TH1D* hrecoid_mc = getSimplePlot("main_Summer13/outputFiles_1_53X_cbelVPlus/","Glb_Tight2012","datalike_mc","pt2","DoubleMu17Mu8_Mu17leg","");
  TH1D* hiso_mc = getSimplePlot("main_Summer13/outputFiles_2_53X_cbelVPlus/","Glb_Tight2012_IsolPFRelCombNoEGammaR03","datalike_mc","pt2","DoubleMu17Mu8_Mu17leg","");
  TH1D* hrecoid = getSimplePlot("main_Summer13/outputFiles_1_53X_cbelVPlus/","Glb_Tight2012","data_all","pt2","DoubleMu17Mu8_Mu17leg","RunAB");
  TH1D* hiso = getSimplePlot("main_Summer13/outputFiles_2_53X_cbelVPlus/","Glb_Tight2012_IsolPFRelCombNoEGammaR03","data_all","pt2","DoubleMu17Mu8_Mu17leg","RunAB");
  // TH1D* htrig1_mc = getSimplePlot("outputFiles_3_53X_cbelVPlus/","Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17Mu8_Mu17leg","datalike_mc","pt2","DoubleMu17Mu8_Mu17leg","");
  // TH1D* htrig2_mc = getSimplePlot("outputFiles_3_53X_cbelVPlus/","Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17Mu8_TkMu8leg","datalike_mc","pt2","DoubleMu17Mu8_Mu17leg","");

  //MULTIPLY HISTOS
  TH1D* h_rec1_mc = histoMultiply2(hrecoid_mc,hrecoid_mc, true); 
  TH1D* h_iso1_mc = histoMultiply2(hiso_mc,hiso_mc, true);
  TH1D* h_pT_mc = histoMultiply2(h_rec1_mc,h_iso1_mc, true);
  TH1D* h1_pT = histoMultiply2(hrecoid,hrecoid, true);
  TH1D* h2_pT = histoMultiply2(hiso,hiso, true);
  TH1D* h_pT = histoMultiply2(h1_pT,h2_pT, true);


  //TAKE MC TRUTH
  TFile* f = new TFile("../AccEffMCtruth/out1_JulyRep_w1_2011Sel_extended.root");
  f->cd();
  TH1D* heff_pT = (TH1D*)eff_postFSR_pT->Clone();

  getComparisonratio(h_pT_mc, h_pT, heff_pT);


}

void getComparisonratio(TH1D* h1, TH1D* h2, TH1D* h3) { 

  TLegend* leg1D = MakeLegend();
  TCanvas* c = new TCanvas();
  c->cd();
  c->SetLogx();
  leg1D->AddEntry(h1,"T&P, MC","lp");
  leg1D->AddEntry(h2,"T&P, Data","lp");
  leg1D->AddEntry(h3,"MC truth","lp");

  h1->SetMarkerStyle(20);
  h1->SetLineWidth(1.5);
  h1->SetMarkerSize(1.1);
  h1->GetYaxis()->SetTitle("#epsilon");
  h1->GetXaxis()->SetTitle("M(#mu#mu) [GeV]");
  h1->GetXaxis()->SetMoreLogLabels();
  h1->GetXaxis()->SetNoExponent();
  h1->Draw("P");
  h2->SetMarkerStyle(20);
  h2->SetLineWidth(1.5);
  h2->SetMarkerSize(1.1);
  h2->SetMarkerColor(kRed);
  h2->SetLineColor(kRed);
  h2->Draw("Psame");
  h3->SetMarkerStyle(20);
  h3->SetLineWidth(1.5);
  h3->SetMarkerSize(1.1);
  h3->SetMarkerColor(kBlue);
  h3->SetLineColor(kBlue);
  h3->Draw("Psame");

  leg1D->Draw("same");
  c->SaveAs("effTPvsMC.png");

}
