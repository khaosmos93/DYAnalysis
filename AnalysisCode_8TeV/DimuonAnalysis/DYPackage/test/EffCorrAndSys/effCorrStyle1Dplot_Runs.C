void effCorrStyle1Dplot_Runs() { 

  TString runs[3] = {"RunAB","RunC","RunD"};

  TString BINNING1 = "./1D"; //"./binning15/";
  TString BINNING2 = "./1D"; //"./binning14/";

  gROOT->ProcessLine(".L ../../tools/plottools.C");
  gROOT->ProcessLine(".L ../../tools/histotools.C");

  TLegend* leg1D = MakeLegend();
  TCanvas* c = new TCanvas();
  c->cd();
  c->SetLogx();
  for (int i = 0; i < 3; i++) {
  //RunAB
  TFile* tp1 = new TFile(BINNING2+"/correction_isoinvm_"+runs[i]+"1D_TRMNov.root");
  tp1->cd();
  TH1D* hiso1 = (TH1D*)gDirectory->Get("invm");
  TFile* tp2 = new TFile(BINNING2+"/correction_triginvm_"+runs[i]+"1D_TRMNov.root");
  tp2->cd();
  TH1D* htrig1 = (TH1D*)gDirectory->Get("invm");
  TFile* tp3 = new TFile(BINNING1+"/correction_recoidinvm_"+runs[i]+"1D_TRMNov.root");
  tp3->cd();
  TH1D* hrecoid1 = (TH1D*)gDirectory->Get("invm");

  TH1D* htp1 = histoMultiply3(hrecoid1,hiso1,htrig1,true);
  //htp1 = htrig1;

  leg1D->AddEntry(htp1,runs[i],"lp");

  htp1->SetMarkerStyle(20);
  htp1->SetLineWidth(1.5);
  htp1->SetMarkerSize(1.1);
  htp1->SetLineColor(1+i);
  htp1->SetMarkerColor(1+i);
  htp1->GetYaxis()->SetTitle("#rho_{Data/MC}");
  htp1->GetXaxis()->SetTitle("M(#mu#mu) [GeV]");
  htp1->GetXaxis()->SetMoreLogLabels();
  htp1->GetXaxis()->SetNoExponent();
  htp1->GetYaxis()->SetRangeUser(0.89,1.11);
  //htp1->GetYaxis()->SetRangeUser(0.95,1.00);
  if (i==0) { 
    htp1->Draw("P");
  } else  {htp1->Draw("Psame");}

  }
  leg1D->Draw("same");
  c->SaveAs("rho1D_Runs.png");

 //Second plot is 2D
  TString BINNING3 = "./2D"; //"./binning15/";


  TString WHICH[6] = {"absrap2030","absrap3045","absrap4560","absrap60120","absrap120200","absrap200"};
  TString beautiful[6] = {"20-30 GeV","30-45 GeV","45-60 GeV","60-120 GeV","120-200 GeV","200-1500 GeV"};

  for (int iw = 0; iw < 6; iw++) {
    TString postfix = "2D";
    //get whats in dir 1
    //split
   //Set up a TCanvas
   TCanvas* c2 = new TCanvas(WHICH[iw],WHICH[iw]);
   c2->cd();
   TLegend* leg = MakeLegend();
  for (int i = 0; i < 3; i++) {
    TFile* tp1 = new TFile(BINNING3+"/correction_iso"+WHICH[iw]+"_"+runs[i]+"2D.root");
    tp1->cd();
    TH1D* hiso1 = (TH1D*)gDirectory->Get(WHICH[iw]);
    TFile* tp2 = new TFile(BINNING3+"/correction_trig"+WHICH[iw]+"_"+runs[i]+"2D.root");
    tp2->cd();
    TH1D* htrig1 = (TH1D*)gDirectory->Get(WHICH[iw]);
    TFile* tp3 = new TFile(BINNING3+"/correction_recoid"+WHICH[iw]+"_"+runs[i]+"2D.root");
    tp3->cd();
    TH1D* hrecoid1 = (TH1D*)gDirectory->Get(WHICH[iw]);

    TH1D* h = histoMultiply3(htrig1,hiso1,hrecoid1,true);

    leg->AddEntry(h,runs[i],"lp");

     h->SetMarkerStyle(20);
     h->SetLineWidth(1.5);
     h->SetMarkerSize(1.1);
     h->GetYaxis()->SetTitle("#rho_{Data/MC}");
     h->GetXaxis()->SetTitle("|Y(#mu#mu)|");
     h->GetXaxis()->SetMoreLogLabels();
     h->GetXaxis()->SetNoExponent();
     h->GetYaxis()->SetRangeUser(0.89,1.17);
     h->SetLineColor(1+i);
     h->SetMarkerColor(1+i);
     //leg->AddEntry(h,beautiful[iw],"pl");
    if (i==0) {
     h->Draw("P");
     //leg->AddEntry(h,beautiful[iw],"pl");
     } else {
       h->Draw("Psame");
     }
   }//run period loop
   leg->Draw("same");
   c2->SaveAs("rho2D_Runs"+WHICH[iw]+".png");

  TPaveText* pt = MakeTPave();

  TPaveText *pt = new TPaveText(0.83,0.77,0.88,0.92,"brNDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.055);
  pt->SetBorderSize(0);
  pt->SetTextAlign(31);
  pt->AddText("at #sqrt{s}=8 TeV, CMS Preliminary");
  pt->Draw("same");
  leg->Draw("same");

 }

}
