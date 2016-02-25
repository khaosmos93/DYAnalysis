void plot_acc_2D(void) {

  TFile* f = new TFile("out2_JulyRep_w1_TRMNov.root");
  f->cd();
  TH1D* eff_postFSR = (TH1D*)gDirectory->Get("eff_postFSR");
  TH1D* acc_preFSR = (TH1D*)gDirectory->Get("acc_preFSR");
  //TH1D* acc_postFSR = (TH1D*)gDirectory->Get("acc_postFSR");
  TH1D* acceff_postFSR = (TH1D*)gDirectory->Get("acceff_postFSR");

  
  TCanvas* c = new TCanvas();
  c->cd();
  eff_postFSR->SetMarkerStyle(26);
  eff_postFSR->SetLineWidth(2);
  eff_postFSR->SetMarkerColor(kRed);
  eff_postFSR->SetLineColor(kRed);

  acc_preFSR->SetLineWidth(2);
  acc_preFSR->SetMarkerStyle(kFullCircle);
  acc_preFSR->SetMarkerColor(kBlue);
  acc_preFSR->SetLineColor(kBlue);

  acceff_postFSR->SetLineWidth(2);
  acceff_postFSR->SetMarkerStyle(25);


  acc_preFSR->GetXaxis()->SetTitle("Mass-rapidity index");
  acc_preFSR->Draw("PL"); 
  eff_postFSR->Draw("PLsame");
  acceff_postFSR->Draw("PLsame");


   gROOT->ProcessLine(".L ../../tools/plottools.C");
   TLegend* leg = MakeLegend();
   leg->AddEntry(acc_preFSR,"A","lp");
   leg->AddEntry(eff_postFSR,"#epsilon","lp");
   leg->AddEntry(acceff_postFSR,"A #times #epsilon","lp");
   leg->Draw("same");

   TPaveText* pt = MakeTPave();
   pt->SetTextSize(0.048);
   pt->AddText("at #sqrt{s}=8 TeV, CMS Simulation");
   pt->Draw("same");


   TPaveText* pt2 = MakeTPave();
   pt2->AddText("#gamma^{*}/Z #rightarrow #mu#mu");
   pt2->Draw("same");

}
