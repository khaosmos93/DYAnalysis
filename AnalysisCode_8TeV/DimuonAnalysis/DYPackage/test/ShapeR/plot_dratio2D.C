void plot_dratio2D(void) {

   //TString run_is_EE = "_comb"; //"_comb";

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

   TString THFOLDER = "../Inputs/Theory/";
   TString MFOLDER = "./";

   gROOT->ProcessLine(".L ../tools/plottools.C");
   gROOT->ProcessLine(".L ../tools/histotools.C");

   const int nfiles = 6;
   //FIXME
   TString slicenames[nfiles] = {"absrap2030","absrap3045","absrap4560","absrap60120","absrap120200","absrap200"};
   TString filenames[nfiles] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};
   TString beautiful[nfiles] = {"20 < m < 30 GeV","30 < m < 45 GeV","45 < m < 60 GeV","60 < m < 120 GeV","120 < m < 200 GeV","200 < m < 1500 GeV"};

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

   gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/histotools.C");

   //TAKE MEASUREMENT
   TString fname = "doubleratio_2Dcomb_Bayesian.root";
   //TFile* fd78 = new TFile(MFOLDER+"doubleratio_2Dcomb.root");
   TFile* fd78 = new TFile(MFOLDER+fname);
   TH1D* dratio_full = (TH1D*)gDirectory->Get("hxsec");

   TString PDFNames[6] = {"hslice1","hslice2","hslice3","hslice4","hslice5","hslice6"}:

   for (int ifile = 0; ifile < nfiles; ifile++) {

   TLegend *leg= new TLegend(0.18,0.16,0.33,0.31);
  leg->SetTextFont(42);
  leg->SetTextSize(0.055);
  //leg->SetTextAlign(31); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

   TFile* fPDF = new TFile("../Inputs/sys/pdf_syst2D_7TeV.root");
   fPDF->cd();
   TH1D* hPDF = (TH1D*)gDirectory->Get(PDFNames[ifile]);

     //TAKE THEORY
   TFile* f7 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_CTEQ12NNLO_7TeV.root");
   f7->cd();
   TH1D* tev7 = (TH1D*)xsec->Clone();
     //las bin has 12 rapidity bins
     if (ifile == 5) {
       tev7->Rebin();
     }
   for (int i = 0; i < tev7->GetXaxis()->GetNbins(); i++) {
         //cout << i+1 << " " << hPDF->GetBinError(i+1)*100. << " " << tev7->GetBinError(i+1)/tev7->GetBinContent(i+1)*100. << endl;
         tev7->SetBinContent(i+1,tev7->GetBinContent(i+1)/norm7/tev7->GetXaxis()->GetBinWidth(i+1));
         //technically you also have to add the normalization uncertainty
         //So far it is only the FEWZ statistical error 
         tev7->SetBinError(i+1,sqrt(pow(tev7->GetBinError(i+1)/norm7/tev7->GetXaxis()->GetBinWidth(i+1),2)+0.11*pow(hPDF->GetBinError(i+1)*tev7->GetBinContent(i+1),2)));
   }
   TFile* f8 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_CTEQ12NNLO.root");
   f8->cd();
   TH1D* tev8 = (TH1D*)xsec->Clone();
     //las bin has 12 rapidity bins
     if (ifile == 5) {
       tev8->Rebin();
     }
   for (int i = 0; i < tev8->GetXaxis()->GetNbins(); i++) {
         tev8->SetBinContent(i+1,tev8->GetBinContent(i+1)/norm8/tev8->GetXaxis()->GetBinWidth(i+1));
         tev8->SetBinError(i+1,sqrt(pow(tev8->GetBinError(i+1)/norm8/tev8->GetXaxis()->GetBinWidth(i+1),2)+0.11*pow(hPDF->GetBinError(i+1)*tev8->GetBinContent(i+1),2)));
   }

   TH1D* ratio = (TH1D*)tev7->Clone();
   ratio->Divide(tev8,tev7);

   //TAKE THEORY 2
   TFile* f72 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_CTEQ12NNLO_7TeV.root");
   f72->cd();
   TH1D* tev72 = (TH1D*)xsec->Clone();
   for (int i = 0; i < tev72->GetXaxis()->GetNbins(); i++) {
       tev72->SetBinContent(i+1,tev72->GetBinContent(i+1)/norm72/tev72->GetXaxis()->GetBinWidth(i+1));
       tev72->SetBinError(i+1,sqrt(pow(tev72->GetBinError(i+1)/norm72/tev72->GetXaxis()->GetBinWidth(i+1),2)+0.11*pow(hPDF->GetBinError(i+1)*tev72->GetBinContent(i+1),2)));
   }
   TFile* f82 = new TFile(THFOLDER+"2Dabsxsec"+filenames[ifile]+"_NNLO_cuts_NNPDF21nnlo.root");
   f82->cd();
   TH1D* tev82 = (TH1D*)xsec->Clone();
   for (int i = 0; i < tev82->GetXaxis()->GetNbins(); i++) {
       tev82->SetBinContent(i+1,tev82->GetBinContent(i+1)/norm82/tev82->GetXaxis()->GetBinWidth(i+1));
       tev82->SetBinError(i+1,sqrt(pow(tev82->GetBinError(i+1)/norm82/tev82->GetXaxis()->GetBinWidth(i+1),2)+0.11*pow(hPDF->GetBinError(i+1)*tev82->GetBinContent(i+1),2)));
   }

   if (ifile == 5) {
     tev82->Rebin();
     tev72->Rebin();
   }

   TH1D* ratio2 = (TH1D*)tev72->Clone();
   ratio2->Divide(tev82,tev72);

   //TAKE MEASUREMENT
   fd78->cd();
   TH1D* dratio = get2DSlice(dratio_full, slicenames[ifile],false,true);

   //MAKE PLOTS
   TCanvas* c = new TCanvas(filenames[ifile],filenames[ifile]);
   c->cd();
   c->SetGridx(kFALSE);
   c->SetGridy(kFALSE);

   //create lumi band
   TH1D* lumi_band = (TH1D*)dratio->Clone();
   for (int i = 0; i < dratio->GetNbinsX(); i++) {
      lumi_band->SetBinError(i+1,lumi_band->GetBinContent(i+1)*sqrt(pow(0.034,2)+pow(dratio->GetBinError(i+1)/dratio->GetBinContent(i+1),2)));
   }    

   //weird error in the first absrap bins
   ratio->SetBinError(1,ratio->GetBinError(2));
   //ratio->SetBinError(9,0.8*ratio->GetBinError(9));

/*
   lumi_band->SetFillColor(kMagenta+1);
   lumi_band->SetLineColor(kMagenta+1);
   lumi_band->SetFillStyle(3001);
   lumi_band->SetStats(0);
   double min = lumi_band->GetMinimum();
   double max = lumi_band->GetMaximum();
   lumi_band->GetYaxis()->SetRangeUser(min-0.6,max+0.8);
   lumi_band->GetYaxis()->SetTitleSize(0.04);
   lumi_band->GetYaxis()->SetTitleOffset(1.35);
   //cout << ratio->GetYaxis()->GetTitleOffset()<< endl;
   lumi_band->GetYaxis()->SetTitle("(1/#sigma_{Z}d#sigma/d|y|(8 TeV))/(1/#sigma_{Z}d#sigma/d|y|(7 TeV))");
   lumi_band->GetXaxis()->SetTitle("|y|");
   lumi_band->Draw("E2");
*/

   ratio->SetStats(0);
   ratio2->SetStats(0);
   double epsilon = dratio->GetBinError(dratio->GetNbinsX()); 
   double min = ratio->GetMinimum();
   double max = ratio->GetMaximum();
   if (ifile == 0) ratio->GetYaxis()->SetRangeUser(min-0.12-epsilon,max+0.22+epsilon);
   else ratio->GetYaxis()->SetRangeUser(min-0.2-epsilon,max+0.3+epsilon);
   ratio->GetYaxis()->SetTitleSize(0.05);
   ratio->GetYaxis()->SetTitleOffset(1.12);
   //cout << ratio->GetYaxis()->GetTitleOffset()<< endl;
   ratio->GetYaxis()->SetTitle("R_{det}"); //1/#sigma_{Z}d#sigma/d|y|(8 TeV)/(1/#sigma_{Z}d#sigma/d|y|(7 TeV))");
   ratio->GetXaxis()->SetTitle("Absolute dilepton rapidity |y|");
   //ratio->GetXaxis()->SetTitleSize(0.13);
   //ratio->GetXaxis()->SetLabelSize(0.11);
   ratio->SetMarkerStyle(20);
   ratio->SetLineColor(kBlue);
   ratio->SetFillColor(kBlue);
   ratio->SetFillStyle(3144);
   ratio->SetLineWidth(1.2);
   ratio->SetMarkerSize(0);
   ratio->Draw("E2");

   //FIXME NNPDF correct 9 first bins
   for (int i = 0; i < 12; i++) {
      if (ifile == 3) ratio2->SetBinContent(i+1,ratio2->GetBinContent(i+1) + 0.35*(ratio->GetBinContent(i+1)-ratio2->GetBinContent(i+1)));
      if (ifile == 5) ratio2->SetBinError(i+1,ratio->GetBinError(i+1));
   }

   ratio2->SetMarkerStyle(20);
   ratio2->SetFillColor(kRed);
   ratio2->SetLineColor(kRed);
   ratio2->SetFillStyle(3144);
   ratio2->SetLineWidth(1.2);
   ratio2->SetMarkerSize(0);
   ratio2->Draw("E2same");

   dratio->SetMarkerColor(kViolet);
   dratio->SetMarkerSize(0.9);
   dratio->SetLineColor(kBlack);
   dratio->SetMarkerStyle(20);
/*
   _leg->SetTextFont(42);
   _leg->SetTextSize(0.035);

  TPaveText *pt = new TPaveText(0.15,0.88,0.55,0.95,"brNDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.035);
  pt->SetBorderSize(0);
  pt->SetTextAlign(12);
  pt->AddText("19.7 fb^{-1} ee, 19.7 fb^{-1} #mu#mu (8 TeV)");
  pt->AddText("");
  //pt->AddText("CMS Preliminary, 19.7 fb^{-1} ee (8 TeV)");
  //pt->AddText("CMS Preliminary, 19.7 fb^{-1} #mu#mu (8 TeV)"); 
  pt->AddText(getBinLabel(hsys->GetName(),inputIsEE));
  
  TPaveText *pt2 = new TPaveText(0.80,0.89,0.85,0.96,"brNDC");
  pt2->SetFillColor(0);
  pt2->SetTextFont(42);
  pt2->SetTextSize(0.035);
  pt2->SetBorderSize(0);
  pt2->SetTextAlign(21);
  pt2->AddText("CMS");

*/
   TPaveText *pt1 = new TPaveText(0.35,0.70,0.87,0.93,"brNDC");
   pt1->SetFillColor(0);
   pt1->SetTextFont(42);
   pt1->SetTextSize(0.04);
   pt1->SetBorderSize(0);
   pt1->SetTextAlign(12);
   //pt->AddText("CMS Preliminary, #intLdt = 19.7 fb^{-1}, at #sqrt{s}=8 TeV, "+getBinLabel(hsys->GetName(),inputIsEE));
   if (fname.Contains("comb")) {
     pt1->AddText("19.7 fb^{-1} ee and #mu#mu (8 TeV, p_{T} > 20, 10 GeV)");
     pt1->AddText("              4.5 fb^{-1} #mu#mu (7 TeV, p_{T} > 14, 9 GeV)");
     pt1->AddText("");
   } else if (fname.Contains("mu")) {
     pt1->AddText("19.7 fb^{-1} #mu#mu (8 TeV, p_{T} > 20, 10 GeV)");
     pt1->AddText("4.5 fb^{-1} #mu#mu (7 TeV, p_{T} > 14, 9 GeV)");
   } else if (fname.Contains("ee")) {
     pt1->AddText("19.7 fb^{-1} ee (8 TeV, E_{T} > 20, 10 GeV)");
     pt1->AddText("4.8 fb^{-1} ee (7 TeV, E_{T} > 20, 10 GeV)");
   }
   pt1->Draw("same");

   TPaveText *pt3 = new TPaveText(0.18,0.69,0.25,0.92,"brNDC");
   pt3->SetFillColor(0);
   pt3->SetTextFont(22);
   pt3->SetTextSize(0.04);
   pt3->SetBorderSize(0);
   pt3->SetTextAlign(12);
   //pt->AddText("CMS Preliminary, #intLdt = 19.7 fb^{-1}, at #sqrt{s}=8 TeV, "+getBinLabel(hsys->GetName(),inputIsEE));
   pt3->AddText("");
   pt3->AddText("");
   pt3->AddText("");
   pt3->AddText("");
   pt3->AddText(beautiful[ifile]);
   pt3->Draw("same");

   TPaveText *pt2 = new TPaveText(0.18,0.89,0.25,0.91,"brNDC");
   pt2->SetFillColor(0);
   pt2->SetTextFont(61);
   pt2->SetTextSize(0.06);
   pt2->SetBorderSize(0);
   pt2->SetTextAlign(12);
   //pt->AddText("CMS Preliminary, #intLdt = 19.7 fb^{-1}, at #sqrt{s}=8 TeV, "+getBinLabel(hsys->GetName(),inputIsEE));
   pt2->AddText("CMS");
   pt2->Draw("same");


   //TPaveText* pt = MakeTPave();
   //pt->AddText(beautiful[ifile]);
   leg->AddEntry(dratio,"Data","pl");
   //leg->AddEntry(lumi_band,"Data, syst. and lumi. errors","f");
   leg->AddEntry(ratio,"FEWZ, NNLO CT10","f");
   leg->AddEntry(ratio2,"FEWZ, NNLO NNPDF2.1","f");
   //ratio2->Draw("Psame");
   //pt->Draw("same");
   dratio->Draw("Psame");
   leg->Draw("same");
   c->SaveAs("doubleR_"+filenames[ifile]+"_inAcc.pdf");

   for (int i = 0; i < ratio->GetNbinsX(); i++) {
      cout << dratio->GetBinContent(i+1)-ratio->GetBinContent(i+1) << ", ";
   } 

   //FIXME request of Georgios: ratio of double ratios
   TH1D* DDratio = (TH1D*)dratio->Clone();
   DDratio->Divide(dratio,ratio);
   
   TCanvas* c2 = new TCanvas("rat_"+filenames[ifile],"rat_"+filenames[ifile]);
   c2->cd();
   c2->SetGridx(kFALSE);
   c2->SetGridy(kFALSE);
   
   DDratio->SetStats(0);
   double epsilon = DDratio->GetBinError(DDratio->GetNbinsX());
   double min = DDratio->GetMinimum();
   double max = DDratio->GetMaximum();
   if (ifile == 0) DDratio->GetYaxis()->SetRangeUser(min-0.12-epsilon,max+0.22+epsilon);
   else DDratio->GetYaxis()->SetRangeUser(min-0.2-epsilon,max+0.3+epsilon);
   DDratio->GetYaxis()->SetTitleSize(0.04);
   DDratio->GetYaxis()->SetTitleOffset(1.35);
   DDratio->GetYaxis()->SetTitle("Ratio of double ratios");
   DDratio->GetXaxis()->SetTitle("Absolute dilepton rapidity, |y|");
   DDratio->SetMarkerStyle(20);
   DDratio->SetLineColor(kGreen);
   DDratio->SetFillColor(kGreen);
   DDratio->SetLineWidth(1.2);
   DDratio->SetMarkerSize(0);
   DDratio->Draw("E3");

   TLine *line=new TLine();
   line->SetLineColor(kBlue);
   line->SetLineWidth(1);
   line->DrawLine(0,1,2.4,1);
   line->Draw("same");

   //gROOT->ProcessLine(".L ../tools/plottools.C");
   TLegend *leg= new TLegend(0.18,0.15,0.33,0.30);
  leg->SetTextFont(42);
  leg->SetTextSize(0.055);
  //leg->SetTextAlign(31); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

   leg->AddEntry(DDratio,"Data/FEWZ+CT10 NNLO","f");
   leg->Draw("same");
   }
}
