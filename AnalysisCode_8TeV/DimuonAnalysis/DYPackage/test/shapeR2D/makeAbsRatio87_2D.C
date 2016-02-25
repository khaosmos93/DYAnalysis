void makeAbsRatio87_2D(void) {

   TString run_is_EE = ""; //"_comb";

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TString THFOLDER = "../Inputs/Theory/";
   TString MFOLDER = "./";

   gROOT->ProcessLine(".L ../tools/plottools.C");
   gROOT->ProcessLine(".L ../tools/histotools.C");

   const int nfiles = 6;
   //FIXME
   //TString results[nfiles] = {"hresult1","hresult2","hresult3","hresult4","hresult5","hresult6"};
   TString filenames[nfiles] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};
   TString beautiful[nfiles] = {"20-30 GeV","30-45 GeV","45-60 GeV","60-120 GeV","120-200 GeV","200-1500 GeV"};

   //normalization purposes
   TFile* f = new TFile(THFOLDER+"2Dabsxsec60to120_NNLO_cuts_CTEQ12NNLO_7TeV.root");
   f->cd();
   TH1D* tev = (TH1D*)xsec->Clone();
   double norm7 = tev->Integral(); 
   TFile* g = new TFile(THFOLDER+"2Dabsxsec60to120_NNLO_cuts_CTEQ12NNLO.root");
   g->cd();
   tev = (TH1D*)xsec->Clone();
   double norm8 = tev->Integral();

   TFile* f = new TFile(THFOLDER+"2Dabsxsec60to120_NNLO_cuts_CTEQ12NNLO_7TeV.root");
   f->cd();
   TH1D* tev2 = (TH1D*)xsec->Clone();
   double norm72 = tev2->Integral();
   TFile* g = new TFile(THFOLDER+"2Dabsxsec60to120_NNLO_cuts_NNPDF21nnlo.root");
   g->cd();
   tev2 = (TH1D*)xsec->Clone();
   double norm82 = tev->Integral();

   TFile* f8 = new TFile(MFOLDER+"absex_DET2D"+run_is_EE+".root");
   f8->cd();
   TH1D* dtev = (TH1D*)gDirectory->Get("60to120");
   for (int i = 0; i < dtev->GetXaxis()->GetNbins(); i++) {
      dtev->SetBinContent(i+1,dtev->GetBinContent(i+1)*dtev->GetXaxis()->GetBinWidth(i+1));
      dtev->SetBinError(i+1,dtev->GetBinError(i+1)*dtev->GetXaxis()->GetBinWidth(i+1));
   }
   double dnorm8 = dtev->Integral();

   for (int ifile = 0; ifile < nfiles; ifile++) {

   TLegend* leg = MakeLegend();

   //TAKE THEORY
   TFile* f7 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_CTEQ12NNLO_7TeV.root");
   f7->cd();
   TH1D* tev7 = (TH1D*)xsec->Clone();
   for (int i = 0; i < tev7->GetXaxis()->GetNbins(); i++) {
       tev7->SetBinContent(i+1,tev7->GetBinContent(i+1)/norm7/tev7->GetXaxis()->GetBinWidth(i+1));
   }
   TFile* f8 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_CTEQ12NNLO.root");
   f8->cd();
   TH1D* tev8 = (TH1D*)xsec->Clone();
   for (int i = 0; i < tev8->GetXaxis()->GetNbins(); i++) {
       tev8->SetBinContent(i+1,tev8->GetBinContent(i+1)/norm8/tev8->GetXaxis()->GetBinWidth(i+1));
   }

   if (ifile == 5) {
     tev8->Rebin();
     tev7->Rebin();
   }

   TH1D* ratio = (TH1D*)tev7->Clone();
   ratio->Divide(tev8,tev7);

   //TAKE THEORY 2
   TFile* f72 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_CTEQ12NNLO_7TeV.root");
   f72->cd();
   TH1D* tev72 = (TH1D*)xsec->Clone();
   for (int i = 0; i < tev72->GetXaxis()->GetNbins(); i++) {
       tev72->SetBinContent(i+1,tev72->GetBinContent(i+1)/norm72/tev72->GetXaxis()->GetBinWidth(i+1));
   }
   TFile* f82 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_NNPDF21nnlo.root");
   f82->cd();
   TH1D* tev82 = (TH1D*)xsec->Clone();
   for (int i = 0; i < tev82->GetXaxis()->GetNbins(); i++) {
       tev82->SetBinContent(i+1,tev82->GetBinContent(i+1)/norm82/tev82->GetXaxis()->GetBinWidth(i+1));
   }

   if (ifile == 5) {
     tev82->Rebin();
     tev72->Rebin();
   }

   TH1D* ratio2 = (TH1D*)tev72->Clone();
   ratio2->Divide(tev82,tev72);



   //TAKE MEASUREMENT
   //FIXME
   TFile* fd7 = new TFile(MFOLDER+"rshape_full2011_2D.root");
   fd7->cd();
   TH1D* hdnorm7 = (TH1D*)gDirectory->Get("h60to120");
   double dnorm7 = hdnorm7->Integral();
   TH1D* dtev7 = (TH1D*)gDirectory->Get("h"+filenames[ifile]);
   for (int i = 0; i < dtev7->GetNbinsX(); i++) {
     dtev7->SetBinContent(i+1,dtev7->GetBinContent(i+1)/dnorm7/dtev7->GetXaxis()->GetBinWidth(i+1));
     dtev7->SetBinError(i+1,dtev7->GetBinError(i+1)/dnorm7/dtev7->GetXaxis()->GetBinWidth(i+1));
     //cout << dtev7->GetBinContent(i+1) << endl;
   }
   TFile* fd8 = new TFile(MFOLDER+"absex_DET2D"+run_is_EE+".root"); //rshape_full2D.root");
   fd8->cd();
   TH1D* dtev8 = (TH1D*)gDirectory->Get(filenames[ifile]);
   for (int i = 0; i < dtev8->GetXaxis()->GetNbins(); i++) {
      dtev8->SetBinContent(i+1,dtev8->GetBinContent(i+1)/dnorm8);
      dtev8->SetBinError(i+1,dtev8->GetBinError(i+1)/dnorm8);
   }
   TH1D* dratio = (TH1D*)dtev7->Clone();
   dratio->Divide(dtev8,dtev7);

   //CHI2 TEST
/*
   cout << "Initial chi2 for 7 TeV " << filenames[ifile] << endl;
   TH1D* chi2_7 = (TH1D*)dtev7->Clone();
   double chisquared7 = 0.;

   for (int i =0; i < dtev7->GetNbinsX(); i++) {
     chisquared7+=pow((dtev7->GetBinContent(i+1)-tev7->GetBinContent(i+1)),2)/(dtev7->GetBinError(i+1)*tev7->GetBinError(i+1));
     chi2_7->SetBinContent(i+1,fabs(dtev7->GetBinContent(i+1)-tev7->GetBinContent(i+1))/sqrt(tev7->GetBinError(i+1)*dtev7->GetBinError(i+1)));
     chi2_7->SetBinError(i+1,0.0);
   }
   chisquared7 = sqrt(chisquared7);
   cout << "Full chi2 per NDF for 7 TeV " << chisquared7/double(dtev7->GetNbinsX()) << endl;

   cout << "Initial chi2 for 8 TeV " << filenames[ifile] << endl;
   TH1D* chi2_8 = (TH1D*)dtev8->Clone();
   double chisquared8 = 0.;

   for (int i =0; i < dtev8->GetNbinsX(); i++) {
     chisquared8+=pow((dtev8->GetBinContent(i+1)-tev8->GetBinContent(i+1)),2)/(dtev8->GetBinError(i+1)*tev8->GetBinError(i+1));
     chi2_8->SetBinContent(i+1,fabs(dtev8->GetBinContent(i+1)-tev8->GetBinContent(i+1))/sqrt(tev8->GetBinError(i+1)*dtev8->GetBinError(i+1)));
     chi2_8->SetBinError(i+1,0.0);
   }
   chisquared8 = sqrt(chisquared8);
   cout << "Full chi2 per NDF for 8 TeV " << chisquared8/double(dtev8->GetNbinsX()) << endl;
*/

   //CHI2 TEST
   cout << "Chi2 " << filenames[ifile] << endl;
   TH1D* chi2 = (TH1D*)ratio->Clone();
   TH1D* chi22 = (TH1D*)ratio2->Clone();
   double chisquared = 0.;
   double chisquared2 = 0.;

   for (int i =0; i < ratio->GetNbinsX(); i++) {
     chisquared+=fabs(dratio->GetBinContent(i+1)-ratio->GetBinContent(i+1))/sqrt(dratio->GetBinError(i+1)*ratio->GetBinError(i+1));
     chi2->SetBinContent(i+1,fabs(dratio->GetBinContent(i+1)-ratio->GetBinContent(i+1))/sqrt(dratio->GetBinError(i+1)*ratio->GetBinError(i+1)));
     chi2->SetBinError(i+1,0.0);

     chisquared2+=fabs(dratio->GetBinContent(i+1)-ratio2->GetBinContent(i+1))/sqrt(dratio->GetBinError(i+1)*ratio2->GetBinError(i+1));
     chi22->SetBinContent(i+1,fabs(dratio->GetBinContent(i+1)-ratio2->GetBinContent(i+1))/sqrt(dratio->GetBinError(i+1)*ratio2->GetBinError(i+1)));
     chi22->SetBinError(i+1,0.0);
   }
   cout << "Full chi2 per NDF " << chisquared/double(ratio->GetNbinsX()) << endl;
   cout << "Full chi2 per NDF " << chisquared2/double(ratio2->GetNbinsX()) << endl;

   TCanvas* c0 = new TCanvas("EE"+filenames[ifile]+"_chi2","EE"+filenames[ifile]+"_chi2");
   c0->cd();
   chi2->GetYaxis()->SetRangeUser(0,3.);
   chi2->GetYaxis()->SetTitle("#chi^{2}");
   chi2->GetXaxis()->SetTitle("|y|");
   TPaveText* pt0 = MakeTPave();
   std::ostringstream pprint;
   pprint.str("");
   pprint<<chisquared/double(ratio->GetNbinsX());
   pt0->AddText("#chi^{2}/ndf (CTEQ12) = "+TString(pprint.str()));
   pprint.str("");
   pprint<<chisquared2/double(ratio2->GetNbinsX());
   pt0->AddText("#chi^{2}/ndf (NNPDF2.1) = "+TString(pprint.str()));
   chi2->SetMarkerStyle(20);
   chi2->SetMarkerColor(kRed);
   chi2->SetLineColor(kRed);
   chi2->Draw("PL");
   chi22->SetMarkerStyle(20);
   chi22->SetMarkerColor(kBlue);
   chi22->SetLineColor(kBlue);
   chi22->Draw("PLsame");
   pt0->Draw("same");


   //MAKE PLOTS
   TCanvas* c = new TCanvas(filenames[ifile],filenames[ifile]);
   c->cd();

   ratio->SetBinError(1,ratio->GetBinError(2));
   ratio->SetBinError(9,0.8*ratio->GetBinError(9));

   ratio->SetMarkerStyle(20);
   ratio->SetStats(0);
   double min = ratio->GetMinimum();
   double max = ratio->GetMaximum();
   ratio->GetYaxis()->SetRangeUser(min-0.6,max+0.8);
   ratio->GetXaxis()->SetMoreLogLabels();
   ratio->GetYaxis()->SetTitle("8 TeV/7 TeV");
   ratio->GetXaxis()->SetTitle("|y|");
   ratio->SetLineColor(kRed);
   ratio->SetFillColor(kRed);
   ratio->SetLineWidth(1.2);
   ratio->SetMarkerSize(0);
   ratio->Draw("E2");

   //ratio2->SetMarkerStyle(20);
   //ratio2->SetMarkerColor(kBlue);
   //ratio2->SetLineColor(kBlue);

   dratio->SetMarkerStyle(20);
   TPaveText* pt = MakeTPave();
   pt->AddText(beautiful[ifile]);
   leg->AddEntry(ratio,"FEWZ3+CT12 NNLO","f");
   //leg->AddEntry(ratio2,"FEWZ3+NNPDF2.1 NNLO","pl");
   leg->AddEntry(dratio,"Data","pl");
   //ratio2->Draw("Psame");
   pt->Draw("same");
   dratio->Draw("Psame");
   leg->Draw("same");
   c->SaveAs("doubleR_"+filenames[ifile]+"_inAcc.pdf");

   for (int i = 0; i < ratio->GetNbinsX(); i++) {
      cout << dratio->GetBinContent(i+1)-ratio->GetBinContent(i+1) << ", ";
   } 


 } 
   //gROOT->ProcessLine(".L ../tools/plottools.C");
   //TLegend* leg = MakeLegend();
   //leg->AddEntry(ratio,"Measurement","lp");
   //leg->AddEntry(ratio_corr,"Measurement, correlated errors","lp");
   //leg->AddEntry(rth,"FEWZ+CT10 NLO","lp");
   //leg->Draw("same");
}
