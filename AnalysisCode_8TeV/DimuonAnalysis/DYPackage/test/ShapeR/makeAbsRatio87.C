void makeAbsRatio87(void) {

   TString THFOLDER = "../Inputs/Theory/";
   TString MFOLDER = "./";

   //TAKE THEORY
/*
   //NLO for VALIDATION PURPOSES
   TFile* f7 = new TFile(THFOLDER+"1Dabsxsec_NLO_CTEQ12NNLO_7TeV.root");
   f7->cd();
   TH1D* tev7 = (TH1D*)invm_FEWZ->Clone();
   double th_norm7 = tev7->Integral(10,22);
   TFile* f8 = new TFile(THFOLDER+"1Dabsxsec_NLO_CTEQ12NNLO.root");
   f8->cd();
   TH1D* tev8 = (TH1D*)invm_FEWZ->Clone();
   double th_norm8 = tev8->Integral(10,22);
   //DIVIDE BY BIN WIDTH
   for (int i = 0; i < tev8->GetXaxis()->GetNbins(); i++) {
      tev7->SetBinContent(i+1,tev7->GetBinContent(i+1)/tev7->GetXaxis()->GetBinWidth(i+1)/th_norm7);
      tev7->SetBinError(i+1,tev7->GetBinError(i+1)/tev7->GetBinWidth(i+1)/th_norm7);

      tev8->SetBinContent(i+1,tev8->GetBinContent(i+1)/tev8->GetXaxis()->GetBinWidth(i+1)/th_norm8);
      tev8->SetBinError(i+1,tev8->GetBinError(i+1)/tev8->GetBinWidth(i+1)/th_norm8);
   }

   TH1D* ratio_NLO = (TH1D*)tev8->Clone();
   ratio_NLO->Divide(tev8,tev7);
*/

   TFile* f7 = new TFile(THFOLDER+"1Dabsxsec_NNLO_CTEQ12NNLO_7TeV.root");
   f7->cd();
   TH1D* tev7 = (TH1D*)invm_FEWZ->Clone();
   double th_norm7 = tev7->Integral(10,22);
   TFile* f8 = new TFile(THFOLDER+"1Dabsxsec_NNLO_CTEQ12NNLO.root");
   f8->cd();
   TH1D* tev8 = (TH1D*)invm_FEWZ->Clone();
   double th_norm8 = tev8->Integral(10,22);
   TH1D* ratio = (TH1D*)tev7->Clone();
   for (int i = 0; i < tev8->GetXaxis()->GetNbins(); i++) {
      tev7->SetBinContent(i+1,tev7->GetBinContent(i+1)/tev7->GetXaxis()->GetBinWidth(i+1)/th_norm7);
      tev7->SetBinError(i+1,tev7->GetBinError(i+1)/tev7->GetBinWidth(i+1)/th_norm7);

      tev8->SetBinContent(i+1,tev8->GetBinContent(i+1)/tev8->GetXaxis()->GetBinWidth(i+1)/th_norm8);
      tev8->SetBinError(i+1,tev8->GetBinError(i+1)/tev8->GetBinWidth(i+1)/th_norm8);
   }
   ratio->Divide(tev8,tev7);


   //TAKE MEASUREMENT
   //FIXME this inpout is divided by bin width and lumi
   //so needs to be multiplied by lumi and divided by peak cross section
   TFile* fd8 = new TFile(MFOLDER+"absex_full.root");
   fd8->cd();
   TH1D* dtev8 = (TH1D*)hxsec->Clone();
   for (int i = 0; i < dtev8->GetXaxis()->GetNbins(); i++) {
      dtev8->SetBinContent(i+1,dtev8->GetBinContent(i+1)*dtev8->GetXaxis()->GetBinWidth(i+1));
      dtev8->SetBinError(i+1,dtev8->GetBinError(i+1)*dtev8->GetXaxis()->GetBinWidth(i+1));
   }
   double norm8 = dtev8->Integral(10,22);
   cout << "Normalization " << norm8 << endl;
   //double lumi8 = 18100.;
   for (int i = 0; i < dtev8->GetXaxis()->GetNbins(); i++) {
      dtev8->SetBinContent(i+1,dtev8->GetBinContent(i+1)/norm8/dtev8->GetXaxis()->GetBinWidth(i+1));
      dtev8->SetBinError(i+1,dtev8->GetBinError(i+1)/norm8/dtev8->GetXaxis()->GetBinWidth(i+1));
   }

   //TFile* fd7 = new TFile("/group/cms/users/asvyatko/CMSSW_4_2_8/src/DYPackage/Outputs/rshape_comb_MarchMain14Acc.root"); //rshape_full2011.root");
   //TFile* fd7 = new TFile("/group/cms/users/asvyatko/CMSSW_4_2_8/src/DYPackage/Outputs/rshape_full_NOFEWZtry.root");
   //TFile* fd7 = new TFile("/group/cms/users/asvyatko/CMSSW_4_2_8/src/DYPackage/Outputs/rshape_ee_MarchModOn.root");
   TFile* fd7 = new TFile("/group/cms/users/asvyatko/CMSSW_4_2_8/src/DYPackage/Outputs/rshape_full_mumuCurrentMarch.root");
   fd7->cd();
   TH1D* dtev7 = (TH1D*)hxsec->Clone();   
   double norm7 = dtev7->Integral(10,22);
   //double lumi7 = 4481.;   
   //DIVIDE BY BIN WIDTH
   //for (int i = 0; i < dtev7->GetXaxis()->GetNbins(); i++) {
   //   dtev7->SetBinContent(i+1,dtev7->GetBinContent(i+1)/dtev7->GetBinWidth(i+1)/norm7);
   //   dtev7->SetBinError(i+1,dtev7->GetBinError(i+1)/dtev7->GetBinWidth(i+1)/norm7);
   //}
   TH1D* dratio = (TH1D*)dtev7->Clone();
   dratio->Divide(dtev8,dtev7);

   //MAKE PLOTS
   ratio->SetLineColor(kBlue);
   ratio->SetLineWidth(3);
   ratio->SetMarkerSize(0);
   ratio->SetFillColor(kBlue);
   ratio->GetXaxis()->SetMoreLogLabels();
   ratio->GetYaxis()->SetRangeUser(0.4,2.0);
   ratio->GetYaxis()->SetTitle("8 TeV/7 TeV");
   ratio->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
   ratio->Draw("E3");

   dratio->SetMarkerStyle(20);
   dratio->SetMarkerColor(kRed);
   dratio->Draw("Psame");
/*
   //CALCULATE CHI2
   TH1D* chi2 = (TH1D*)tev8->Clone();
   double chisquared = 0.;

   for (int i =0; i < 40; i++) {
     chisquared+=pow((dtev8->GetBinContent(i+1)-tev8->GetBinContent(i+1)),2)/pow(dtev8->GetBinError(i+1),2);

     chi2->SetBinContent(i+1,fabs(dtev8->GetBinContent(i+1)-tev8->GetBinContent(i+1))/dtev8->GetBinError(i+1));
     chi2->SetBinError(i+1,0.0);
   }
   cout << "Full chi2 " << chisquared/40. << endl;

   gROOT->ProcessLine(".L ../tools/plottools.C");

   chi2->SetMarkerStyle(20);
   //chi2->GetYaxis()->SetRangeUser(0.4,1.6);
   chi2->GetXaxis()->SetMoreLogLabels();
   chi2->GetYaxis()->SetTitle("#chi^{2}"); //ee/ee ratio"); //#rho_{Data/MC}"); //"#epsilon");
   chi2->GetXaxis()->SetTitle("m_{ee} [GeV]"); //getTitle(varType,true));
   chi2->SetMarkerStyle(20);
   chi2->SetMarkerColor(kRed);
   chi2->SetLineColor(kRed);
   chi2->Draw("PL");
*/
}
