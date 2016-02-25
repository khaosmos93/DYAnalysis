void chi2(void) {

   TString THFOLDER = "../Inputs/Theory/";
   TString MFOLDER = "./";

   TFile* f7 = new TFile(THFOLDER+"1Dabsxsec_NNLO_CTEQ12NNLO_7TeV.root");
   f7->cd();
   TH1D* tev7 = (TH1D*)invm_FEWZ->Clone();
   TFile* f8 = new TFile(THFOLDER+"1Dabsxsec_NNLO_CTEQ12NNLO.root");
   f8->cd();
   TH1D* tev8 = (TH1D*)invm_FEWZ->Clone();


   //TAKE MEASUREMENT
   //FIXME this inpout is divided by bin width and lumi
   //so needs to be multiplied by lumi and divided by peak cross section
   TFile* fd8 = new TFile(MFOLDER+"absex_full_EE.root");
   fd8->cd();
   TH1D* dtev8 = (TH1D*)hxsec->Clone();

   TFile* fd7 = new TFile(MFOLDER+"/rshape_full2011.root");
   fd7->cd();
   TH1D* dtev7 = (TH1D*)hxsec->Clone();   
   //DIVIDE BY BIN WIDTH
   for (int i = 0; i < dtev7->GetXaxis()->GetNbins(); i++) {
      dtev7->SetBinContent(i+1,dtev7->GetBinContent(i+1)/dtev7->GetBinWidth(i+1));
      dtev7->SetBinError(i+1,dtev7->GetBinError(i+1)/dtev7->GetBinWidth(i+1));

      cout << tev8->GetBinContent(i+1)/tev8->GetBinWidth(i+1) << " " << dtev8->GetBinContent(i+1) << endl;
      //cout << dtev7->GetBinError(i+1) << " " << dtev8->GetBinError(i+1) << endl;
   }

   //CALCULATE CHI2
   TH1D* chi2 = (TH1D*)tev8->Clone();
   double chisquared = 0.;

   for (int i =0; i < 40; i++) {
     chisquared+=pow((dtev8->GetBinContent(i+1)-tev8->GetBinContent(i+1)/tev8->GetBinWidth(i+1)),2)/pow(dtev8->GetBinError(i+1),2);

     chi2->SetBinContent(i+1,fabs(dtev8->GetBinContent(i+1)-tev8->GetBinContent(i+1)/tev8->GetBinWidth(i+1))/dtev8->GetBinError(i+1));
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
}
