void chi2plotter() {

   const int nbin = 6; 
   double massing_corr[nbin+1] =       {20,  30,45, 60,  120, 200, 1300};
   double massing_uncorr[nbin+1] =     {22.5,33,48, 75,  140, 500, 1400};
   double massing_PDF_corr[nbin+1] =   {25,  36,51, 90,  160, 800, 1500};
   double massing_PDF_uncorr[nbin+1] = {27.5,39,54, 105, 180, 1100,1600};

   double hera_corr[nbin] = {0.787111,2.26547,1.27074,3.82553,0.698609,1.17293};
   double hera_uncorr[nbin] = {0.861275,3.83213,1.21243,1.55671,0.817307,0.941914};
   double hera_PDF_corr[nbin] = {0.393555,1.13274,0.635369,1.91277,0.349304,0.586466};
   double hera_PDF_uncorr[nbin] = {0.861275,3.83213,1.21243,1.55671,0.817307,0.941914};

   double ct10_corr[nbin] = {0.806563,2.13728,1.53733,7.19164,1.05075,1.32624};
   double ct10_uncorr[nbin] = {1.12454,1.11207,2.09781,1.22246,1.42578,1.29772};

   double abkm_corr[nbin] = {0.811431,2.7307,1.76213,10.6328,1.10514,1.25605};
   double abkm_uncorr[nbin] = {1.24465,1.95401,1.88788,2.21696,1.25519,1.09132};

   double jr09_corr[nbin] = {1.03474,2.94333,1.20056,23.5608,0.782931,1.13575};
   double jr09_uncorr[nbin] = {1.96312,2.34727,1.42243,7.43095,1.1261,1.03174};

   double mstw2008_corr[nbin] = {0.825068,2.014,1.23614,11.0539,0.954699,1.11712};
   double mstw2008_uncorr[nbin] = {0.803621,1.96023,1.44531,3.75437,1.27622,0.951804};

   double nnpdf_corr[nbin] = {0.925843,2.24115,1.74451,8.32083,0.881597,1.10211};
   double nnpdf_uncorr[nbin] = {0.941286,3.30785,1.42016,2.5177,1.12683,0.913288};
   double nnpdf_PDF_corr[nbin] = {0.462921,1.12057,0.872254,4.16042,0.440799,0.551057};
   double nnpdf_PDF_uncorr[nbin] = {0.941286,3.30785,1.42016,2.5177,1.12683,0.913288};

   double ct10w_corr[nbin] = {0.800224,1.84931,1.17055,7.70148,0.777689,1.15069};
   double ct10w_uncorr[nbin] = {0.792281,0.971117,2.05616,1.67019,0.934291,1.00338};

   TH1D* hhera_corr = new TH1D("hera_corr","hera_corr",nbin,massing_corr);
   TH1D* hhera_uncorr = new TH1D("hera_ucnorr","hera_uncorr",nbin,massing_corr);
   TH1D* hhera_PDF_corr = new TH1D("hera_PDF_corr","hera_PDF_corr",nbin,massing_corr);
   TH1D* hhera_PDF_uncorr = new TH1D("hera_PDF_uncorr","hera_PDF_uncorr",nbin,massing_corr);

   TH1D* hct10_corr = new TH1D("ct10_corr","ct10_corr",nbin,massing_corr);
   TH1D* hct10_uncorr = new TH1D("ct10_uncorr","ct10_uncorr",nbin,massing_corr);

   TH1D* hct10w_corr = new TH1D("ct10w_corr","ct10w_corr",nbin,massing_corr);
   TH1D* hct10w_uncorr = new TH1D("ct10w_uncorr","ct10w_uncorr",nbin,massing_corr);

   TH1D* habkm_corr = new TH1D("abkm_corr","abkm_corr",nbin,massing_corr);
   TH1D* habkm_uncorr = new TH1D("abkm_uncorr","abkm_uncorr",nbin,massing_corr);

   TH1D* hnnpdf_corr = new TH1D("nnpdf_corr","nnpdf_corr",nbin,massing_corr);
   TH1D* hnnpdf_uncorr = new TH1D("nnpdf_ucnorr","nnpdf_uncorr",nbin,massing_corr);
   TH1D* hnnpdf_PDF_corr = new TH1D("nnpdf_PDF_corr","nnpdf_PDF_corr",nbin,massing_corr);
   TH1D* hnnpdf_PDF_uncorr = new TH1D("nnpdf_PDF_uncorr","nnpdf_PDF_uncorr",nbin,massing_corr);

   TH1D* hjr09_corr = new TH1D("jr09_corr","jr09_corr",nbin,massing_corr);
   TH1D* hjr09_uncorr = new TH1D("jr09_ucnorr","jr09_uncorr",nbin,massing_corr);

   TH1D* hmstw2008_corr = new TH1D("mstw2008_corr","mstw2008_corr",nbin,massing_corr);
   TH1D* hmstw2008_uncorr = new TH1D("mstw2008_ucnorr","mstw2008_uncorr",nbin,massing_corr);

   for (int i = 0; i < nbin; i++) {
     hhera_corr->SetBinContent(i+1,hera_corr[i]);
     hhera_corr->SetBinError(i+1,0.000001);
     hhera_uncorr->SetBinContent(i+1,hera_uncorr[i]);
     hhera_uncorr->SetBinError(i+1,0.000001);
     hhera_PDF_corr->SetBinContent(i+1,hera_PDF_corr[i]);
     hhera_PDF_corr->SetBinError(i+1,0.000001);
     hhera_PDF_uncorr->SetBinContent(i+1,hera_PDF_uncorr[i]);
     hhera_PDF_uncorr->SetBinError(i+1,0.000001);

     hnnpdf_corr->SetBinContent(i+1,nnpdf_corr[i]);
     hnnpdf_corr->SetBinError(i+1,0.000001);
     hnnpdf_uncorr->SetBinContent(i+1,nnpdf_uncorr[i]);
     hnnpdf_uncorr->SetBinError(i+1,0.000001);
     hnnpdf_PDF_corr->SetBinContent(i+1,nnpdf_PDF_corr[i]);
     hnnpdf_PDF_corr->SetBinError(i+1,0.000001);
     hnnpdf_PDF_uncorr->SetBinContent(i+1,nnpdf_PDF_uncorr[i]);
     hnnpdf_PDF_uncorr->SetBinError(i+1,0.000001);

     habkm_corr->SetBinContent(i+1,abkm_corr[i]);
     habkm_corr->SetBinError(i+1,0.000001);
     habkm_uncorr->SetBinContent(i+1,abkm_uncorr[i]);
     habkm_uncorr->SetBinError(i+1,0.000001);

     hjr09_corr->SetBinContent(i+1,jr09_corr[i]);
     hjr09_corr->SetBinError(i+1,0.000001);
     hjr09_uncorr->SetBinContent(i+1,jr09_uncorr[i]);
     hjr09_uncorr->SetBinError(i+1,0.000001);

     hmstw2008_corr->SetBinContent(i+1,mstw2008_corr[i]);
     hmstw2008_corr->SetBinError(i+1,0.000001);
     hmstw2008_uncorr->SetBinContent(i+1,mstw2008_uncorr[i]);
     hmstw2008_uncorr->SetBinError(i+1,0.000001);

     hct10_corr->SetBinContent(i+1,ct10_corr[i]);
     hct10_corr->SetBinError(i+1,0.000001);
     hct10_uncorr->SetBinContent(i+1,ct10_uncorr[i]);
     hct10_uncorr->SetBinError(i+1,0.000001);

     hct10w_corr->SetBinContent(i+1,ct10w_corr[i]);
     hct10w_corr->SetBinError(i+1,0.000001);
     hct10w_uncorr->SetBinContent(i+1,ct10w_uncorr[i]);
     hct10w_uncorr->SetBinError(i+1,0.000001);

   }


   TCanvas* c = new TCanvas();
   c->cd();
    
   hct10w_uncorr->SetMarkerStyle(20);
   hct10w_uncorr->SetMarkerColor(kBlack);
   hct10w_uncorr->SetMarkerSize(1.5);
   hct10w_uncorr->SetLineColor(kBlack);

   hct10w_corr->SetMarkerStyle(33);
   hct10w_corr->SetMarkerColor(kBlack);
   hct10w_corr->SetMarkerSize(1.5);
   hct10w_corr->SetLineColor(kBlack);

   hct10_uncorr->SetMarkerStyle(20);
   hct10_uncorr->SetMarkerColor(kBlue);
   hct10_uncorr->SetMarkerSize(1.5);
   hct10_uncorr->SetLineColor(kBlue);

   hct10_corr->SetMarkerStyle(33);
   hct10_corr->SetMarkerColor(kBlue);
   hct10_corr->SetMarkerSize(1.5);
   hct10_corr->SetLineColor(kBlue);

   habkm_uncorr->SetMarkerStyle(20);
   habkm_uncorr->SetMarkerColor(kOrange);
   habkm_uncorr->SetMarkerSize(1.5);
   habkm_uncorr->SetLineColor(kOrange);

   habkm_corr->SetMarkerStyle(33);
   habkm_corr->SetMarkerColor(kOrange);
   habkm_corr->SetMarkerSize(1.5);
   habkm_corr->SetLineColor(kOrange);

   hjr09_uncorr->SetMarkerStyle(20);
   hjr09_uncorr->SetMarkerColor(kViolet);
   hjr09_uncorr->SetMarkerSize(1.5);
   hjr09_uncorr->SetLineColor(kViolet);

   hjr09_corr->SetMarkerStyle(33);
   hjr09_corr->SetMarkerColor(kViolet);
   hjr09_corr->SetMarkerSize(1.5);
   hjr09_corr->SetLineColor(kViolet);

   hmstw2008_uncorr->SetMarkerStyle(20);
   hmstw2008_uncorr->SetMarkerColor(kGreen);
   hmstw2008_uncorr->SetMarkerSize(1.5);
   hmstw2008_uncorr->SetLineColor(kGreen);

   hmstw2008_corr->SetMarkerStyle(33);
   hmstw2008_corr->SetMarkerColor(kGreen);
   hmstw2008_corr->SetMarkerSize(1.5);
   hmstw2008_corr->SetLineColor(kGreen);


   hhera_uncorr->SetMarkerStyle(20);
   hhera_uncorr->SetMarkerColor(kRed);
   hhera_uncorr->SetMarkerSize(1.5);
   hhera_uncorr->SetLineColor(kRed);

   hhera_corr->SetMarkerStyle(33);
   hhera_corr->SetMarkerColor(kRed);
   hhera_corr->SetMarkerSize(1.5);
   hhera_corr->SetLineColor(kRed);

   hhera_PDF_uncorr->SetMarkerStyle(24);
   hhera_PDF_uncorr->SetMarkerColor(kRed);
   hhera_PDF_uncorr->SetMarkerSize(1.5);
   hhera_PDF_uncorr->SetLineColor(kRed);

   hhera_PDF_corr->SetMarkerStyle(27);
   hhera_PDF_corr->SetMarkerColor(kRed);
   hhera_PDF_corr->SetMarkerSize(1.5);
   hhera_PDF_corr->SetLineColor(kRed);


   hnnpdf_uncorr->SetMarkerStyle(20);
   hnnpdf_uncorr->SetMarkerColor(38);
   hnnpdf_uncorr->SetMarkerSize(1.5);
   hnnpdf_uncorr->SetLineColor(38);

   hnnpdf_corr->SetMarkerStyle(33);
   hnnpdf_corr->SetMarkerColor(38);
   hnnpdf_corr->SetMarkerSize(1.5);
   hnnpdf_corr->SetLineColor(38);

   hnnpdf_PDF_uncorr->SetMarkerStyle(24);
   hnnpdf_PDF_uncorr->SetMarkerColor(38);
   hnnpdf_PDF_uncorr->SetMarkerSize(1.5);
   hnnpdf_PDF_uncorr->SetLineColor(38);

   hnnpdf_PDF_corr->SetMarkerStyle(27);
   hnnpdf_PDF_corr->SetMarkerColor(38);
   hnnpdf_PDF_corr->SetMarkerSize(1.5);
   hnnpdf_PDF_corr->SetLineColor(38);

   hnnpdf_corr->GetXaxis()->SetMoreLogLabels();
   hnnpdf_corr->GetXaxis()->SetNoExponent();
   hnnpdf_corr->GetXaxis()->SetTitle("M (#mu#mu) [GeV]");
   hnnpdf_corr->GetYaxis()->SetTitle("#chi^{2}/ndf");

   hnnpdf_corr->Draw("P");
   hnnpdf_uncorr->Draw("Psame");
   hnnpdf_PDF_corr->Draw("Psame");
   hnnpdf_PDF_uncorr->Draw("Psame"); 
 
   hhera_corr->Draw("Psame");
   hhera_uncorr->Draw("Psame");
   hhera_PDF_corr->Draw("Psame");
   hhera_PDF_uncorr->Draw("Psame");

   habkm_corr->Draw("Psame");
   habkm_uncorr->Draw("Psame");

   hct10_corr->Draw("Psame");
   hct10_uncorr->Draw("Psame");

   hct10w_corr->Draw("Psame");
   hct10w_uncorr->Draw("Psame");

   hjr09_corr->Draw("Psame");
   hjr09_uncorr->Draw("Psame");

   hmstw2008_corr->Draw("Psame");
   hmstw2008_uncorr->Draw("Psame");


   TLegend *_leg= new TLegend(0.50,0.15,0.75,0.35);
   _leg->SetTextFont(72);
   _leg->SetTextSize(0.060);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);

   TLegend *_leg2= new TLegend(0.50,0.15,0.75,0.35);
   _leg2->SetTextFont(72);
   _leg2->SetTextSize(0.060);
   _leg2->SetBorderSize(0);
   _leg2->SetFillColor(0);

   TLegend *_leg3= new TLegend(0.50,0.15,0.75,0.35);
   _leg3->SetTextFont(72);
   _leg3->SetTextSize(0.060);
   _leg3->SetBorderSize(0);
   _leg3->SetFillColor(0);


   _leg->AddEntry(habkm_corr,"ABKM","lp");
   _leg2->AddEntry(habkm_uncorr,"ABKM","lp");
   _leg->AddEntry(hjr09_corr,"JR09","lp");
   _leg2->AddEntry(hjr09_uncorr,"JR09","lp");
   _leg->AddEntry(hmstw2008_corr,"MSTW2008","lp");
   _leg2->AddEntry(hmstw2008_uncorr,"MSTW2008","lp");
   _leg->AddEntry(hct10_corr,"CT10","lp");
   _leg2->AddEntry(hct10_uncorr,"CT10","lp");
   _leg->AddEntry(hct10w_corr,"CT10W","lp");
   _leg2->AddEntry(hct10w_uncorr,"CT10W","lp");
   _leg->AddEntry(hhera_corr,"HERA","lp");
   _leg2->AddEntry(hhera_uncorr,"HERA","lp");
   _leg3->AddEntry(hhera_PDF_corr,"HERA, with corr","lp");
   _leg3->AddEntry(hhera_PDF_uncorr,"HERA, no corr","lp");

   _leg->AddEntry(hnnpdf_corr,"NNPDF2.1","lp");
   _leg2->AddEntry(hnnpdf_uncorr,"NNPDF2.1","lp");
   _leg3->AddEntry(hnnpdf_PDF_corr,"NNPDF2.1, with corr","lp");
   _leg3->AddEntry(hnnpdf_PDF_uncorr,"NNPDF2.1, no corr","lp");
   _leg2->Draw("same");
   _leg->Draw("same");
   _leg3->Draw("same");

  TPaveText *pt = new TPaveText(0.75,0.77,0.80,0.92,"brNDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.055);
  pt->SetBorderSize(0);
  pt->SetTextAlign(31);
  pt->AddText("With corr.");
  pt->Draw("same");

  TPaveText *pt2 = new TPaveText(0.75,0.77,0.80,0.92,"brNDC");
  pt2->SetFillColor(0);
  pt2->SetTextFont(42);
  pt2->SetTextSize(0.055);
  pt2->SetBorderSize(0);
  pt2->SetTextAlign(31);
  pt2->AddText("No corr.");
  pt2->Draw("same");


}
