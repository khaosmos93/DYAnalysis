void effCorrStyle1Dplot() { 

  TString run = "split";
  bool inputIsEE = false;

  TString BINNING1 = "./1D"; //"./binning15/";
  //TString BINNING2 = "./1D"; //"./binning14/";

  gROOT->ProcessLine(".L ../../tools/plottools.C");
  gROOT->ProcessLine(".L ../../tools/histotools.C");

  //RunAB
  TFile* tp1 = new TFile(BINNING1+"/correction_isoinvm_"+run+"1D.root");
  tp1->cd();
  TH1D* hiso1 = (TH1D*)gDirectory->Get("invm41");
  TFile* tp2 = new TFile(BINNING1+"/correction_triginvm_"+run+"1D.root");
  tp2->cd();
  TH1D* htrig1 = (TH1D*)gDirectory->Get("invm41");
  TFile* tp3 = new TFile(BINNING1+"/correction_recoidinvm_"+run+"1D.root");
  cout << BINNING1+"/correction_recoidinvm_"+run+"1D.root" << endl; 
  tp3->cd();
  TH1D* hrecoid1 = (TH1D*)gDirectory->Get("invm41");

  TH1D* htp1 = histoMultiply3(hrecoid1,hiso1,htrig1,true);
  //htp1 = hrecoid1;

  TFile* fee = new TFile("../effcorrEE/effCorrAll_EE.root");
  fee->cd();
  TH1D* htp1_ee = (TH1D*)gDirectory->Get("invm41");

  if (inputIsEE) htp1 = htp1_ee;

  //create an error histogram for validation
  //TH1D* htp1_err = (TH1D*)htp1->Clone();
  //for (int i = 0; i < 41; i++) {
  //   htp1_err->SetBinContent(i+1,100.*htp1->GetBinError(i+1)/htp1->GetBinContent(i+1));
  //   htp1_err->SetBinError(i+1,0.0000001);
  //}
  //TFile* fout = new TFile("effCorrAll"+run+".root","recreate");
  //fout->cd();
  //htp1->Write("invm");
  ////htp1_err->Write("invm_err");
  //fout->Close();

  TCanvas* c = new TCanvas();
  c->cd();
  c->SetLogx(); 
  c->SetRightMargin(0.05);
  htp1->SetMarkerStyle(20);
  htp1->SetLineWidth(1.5);
  htp1->SetMarkerSize(1.1);
  htp1->GetYaxis()->SetTitle("#rho_{Data/MC}");
  if (inputIsEE) htp1->GetXaxis()->SetTitle("M(ee) [GeV]");
  else htp1->GetXaxis()->SetTitle("M(#mu#mu) [GeV]");
  htp1->GetXaxis()->SetMoreLogLabels();
  htp1->GetXaxis()->SetNoExponent();
  //htp1->GetYaxis()->SetRangeUser(0.89,1.11);
  htp1->GetYaxis()->SetRangeUser(0.79,1.21);
  htp1->Draw("P");

  TPaveText* pt = MakeTPave();

  TPaveText *pt = new TPaveText(0.75,0.77,0.80,0.92,"brNDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.055);
  pt->SetBorderSize(0);
  pt->SetTextAlign(31);
  pt->AddText("at #sqrt{s}=8 TeV, CMS Preliminary");
  pt->Draw("same");
  c->SaveAs("rho1D_"+run+".png");

 //Second plot is 2D
  TString BINNING3 = "./2D"; //"./binning15/";

  TString WHICH[6] = {"absrap2030","absrap3045","absrap4560","absrap60120","absrap120200","absrap200"};
  TString beautiful[6] = {"20-30 GeV","30-45 GeV","45-60 GeV","60-120 GeV","120-200 GeV","200-1500 GeV"};

  //Set up a TCanvas
  TCanvas* c2 = new TCanvas();
  c2->cd();

   TLegend* leg = MakeLegend();

  for (int iw = 0; iw < 6; iw++) {
    TString postfix = "2D";
    //get whats in dir 1
    //split
    TFile* tp1 = new TFile(BINNING3+"/correction_iso"+WHICH[iw]+"_"+run+"2D_TRMNov_2014.root");
    tp1->cd();
    TH1D* hiso1 = (TH1D*)gDirectory->Get(WHICH[iw]);
    TFile* tp2 = new TFile(BINNING3+"/correction_trig"+WHICH[iw]+"_"+run+"2D_TRMNov_2014.root");
    tp2->cd();
    TH1D* htrig1 = (TH1D*)gDirectory->Get(WHICH[iw]);
    TFile* tp3 = new TFile(BINNING3+"/correction_recoid"+WHICH[iw]+"_"+run+"2D_TRMNov_2014.root");
    tp3->cd();
    TH1D* hrecoid1 = (TH1D*)gDirectory->Get(WHICH[iw]);

    TH1D* h = histoMultiply3(htrig1,hiso1,hrecoid1,true); // ,factor[iw]);
    //h = hrecoid1;

  //create an error histogram for validation
    //TH1D* h_err = (TH1D*)h->Clone();
    //for (int i = 0; i < h->GetNbinsX(); i++) {
    // h_err->SetBinContent(i+1,100.*h->GetBinError(i+1)/h->GetBinContent(i+1));
    // h_err->SetBinError(i+1,0.0000001);
    //}
    //TFile* fout2 = new TFile("effCorrAll"+run+TString(WHICH[iw])+"_2D.root","recreate");
    //fout2->cd();
    //h->Write("rap");
    //h_err->Write("rap_err");
    //fout2->Close();

    //FIXME for individual efficiencies is applied to all the efficiencies excpet for iso
    if (iw==0) { 
     h->SetMarkerStyle(20);
     h->SetLineWidth(1.5);
     h->SetMarkerSize(1.1);
     h->GetYaxis()->SetTitle("#rho_{Data/MC}");
     h->GetXaxis()->SetTitle("|y|");
     h->GetXaxis()->SetMoreLogLabels();
     h->GetXaxis()->SetNoExponent();
     h->GetYaxis()->SetRangeUser(0.79,1.21);
     h->Draw("P");
     leg->AddEntry(h,beautiful[iw],"pl");

    } else {
     h->SetMarkerStyle(20);
     h->SetLineWidth(1.5);
     h->SetLineColor(1+iw);
     h->SetMarkerSize(1.1);
     h->SetMarkerColor(1+iw);
     leg->AddEntry(h,beautiful[iw],"pl");
     h->Draw("Psame");
     
    }
  }

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

  c2->SaveAs("rho2D_"+run+".png");
}
