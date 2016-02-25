#include <vector>
#include <TH1D.h>

void comparison_1D(void) {

   bool DRAW_CHI2_ONLY = true;

   gROOT->ProcessLine(".L ../../tools/histotools.C");
   gROOT->ProcessLine(".L ../../tools/plottools.C");

   TString result_type = "_fsrUnfGood"; //_fsrBbB
   TString acc = ""; //"DET"; //""
   TString fsr = ""; //"PostFsr"; //""

   cout << "Note: the normalization numbers are a part of the bambu printout" << endl;
   //pre-FSR detector acceptance
   TFile* input_2D = new TFile(result_type+"/xSec_results_1D.root");
   input_2D->cd();
   TMatrixT<double> absex_input = (TMatrixT<double>)gDirectory->Get("XSec");   
   TMatrixT<double> absex_input_err = (TMatrixT<double>)gDirectory->Get("XSecErr");
   TMatrixT<double> absex_input_syserr = (TMatrixT<double>)gDirectory->Get("XSecSystErr");

   const int nbin = 40;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   TH1D* hrshape = new TH1D("rshape","rshape",nbin,mass_xbin);
   TH1D* habsex = new TH1D("absex","absex",nbin,mass_xbin);
   TH1D* hnorm = new TH1D("norm","norm",nbin,mass_xbin);

   for (int imass = 0; imass < 40; imass++) {
     double binw = mass_xbin[imass+1]-mass_xbin[imass];
     for (int irap = 0; irap < 1; irap++) {
          habsex->SetBinContent(imass+1,absex_input(imass,irap)/binw); 
          habsex->SetBinError(imass+1,habsex->GetBinContent(imass+1)*herr->GetBinContent(imass+1)/100.);
     }
   }

   for (int imass = 0; imass < 40; imass++) {
     double binw = mass_xbin[imass+1]-mass_xbin[imass];
     for (int irap = 0; irap < 1; irap++) {
          hnorm->SetBinContent(imass+1,habsex->GetBinContent(imass+1)*binw);
          hnorm->SetBinError(imass+1,habsex->GetBinError(imass+1)*binw);
     }
   }

   cout << "Normalization cross section " << hnorm->Integral(10,22)<< endl;

   ////////////////
   //NOW PLOT THE COMPARISON TO MUONS, FOR ABSOLUTES
   ////////////////

   //NOW make a comparison
   TFile* fmumu = new TFile("../absex_full.root");
   fmumu->cd();
   TH1D* habsex_mu = (TH1D*)gDirectory->Get("hxsec");

   //PLOT superimposed
   TString WHICHES[1] = {"1Dcase"};
   TH1D* hratio[1];
   for (int i = 0; i < 1; i++) {

    if (!DRAW_CHI2_ONLY) {

     TCanvas* c = new TCanvas(WHICHES[i],WHICHES[i]);
     c->Draw();
     c->cd();
     TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,1.0,0,0,0);
     p1->Draw();
     p1->cd();
     //p1->SetLogy();
     p1->SetGridx(kFALSE);
     p1->SetGridy(kFALSE);
     p1->SetBottomMargin(0.01);
     habsex_mu->SetMaximum(habsex_mu->GetMaximum()*1.5);
     habsex_mu->SetMarkerStyle(22);
     habsex_mu->GetYaxis()->SetTitle("d#sigma/dM");
     habsex_mu->GetXaxis()->SetTitle("m [GeV]");
     habsex_mu->Draw("P");
     habsex->SetMarkerStyle(21);
     habsex->SetMarkerColor(kRed);
     habsex->SetLineColor(kRed);
     habsex->Draw("Psame");
 
     TLegend* leg = MakeLegend();
     leg->AddEntry(habsex_mu,"Dimuons","lp");
     leg->AddEntry(habsex,"Dielectrons","lp");
     leg->Draw("same");
   }

     //Set up the ratio
     hratio[i] = (TH1D*)habsex_mu->Clone();
     hratio[i]->Divide(habsex_mu,habsex);
     //FindRatio(hratio[i],habsex_mu,habsex);

   if (!DRAW_CHI2_ONLY) {

     gStyle->SetOptTitle(0);
     TPad *p2 = new TPad("p2", "",0.0,0.0,1.0,0.247,0,0,0);
     c->cd();
     p2->Draw();
     p2->cd();
     //p2->SetLogx();
     p2->SetTopMargin(0.007);
     p2->SetBottomMargin(0.35);
     p2->SetTickx();
     //p2->SetGridx(kFALSE);
     p2->SetGridy(kFALSE);
     hratio[i]->GetXaxis()->SetMoreLogLabels();
     hratio[i]->GetYaxis()->SetTitle("Mu/Ele");
     hratio[i]->SetStats(kFALSE);
     hratio[i]->GetYaxis()->SetTitleOffset(0.29);
     hratio[i]->GetXaxis()->SetTitle("m_{ll}");
     hratio[i]->GetYaxis()->SetTitleSize(0.15);
     hratio[i]->GetXaxis()->SetTitleSize(0.15);
     hratio[i]->GetYaxis()->SetLabelSize(0.10);
     hratio[i]->GetXaxis()->SetLabelSize(0.15);
     hratio[i]->SetMarkerSize(1.3);
     hratio[i]->SetMaximum(1.52);
     hratio[i]->SetMinimum(0.48);
     hratio[i]->SetMarkerStyle(20);
     hratio[i]->Draw("ep");
     //c->Update();
     TLine *line=new TLine();
     line->SetLineColor(kBlue);
     line->SetLineWidth(1);
     line->DrawLine(15,1,1500,1);
   }

     //CHI2 TEST
     TH1D* chi2 = new TH1D("chi2","chi2",nbin,mass_xbin);

     for (int ii =0; ii < habsex_mu->GetNbinsX(); ii++) {
       chi2->SetBinContent(ii+1,fabs(habsex_mu->GetBinContent(ii+1)-habsex->GetBinContent(ii+1))/sqrt(habsex_mu->GetBinError(ii+1)*habsex->GetBinError(ii+1)));
       chi2->SetBinError(ii+1,0.0);
     }
     cout << "Full chi2 per NDF " << chi2->Integral()/double(hratio[i]->GetNbinsX()) << " " << hratio[i]->GetNbinsX()<< endl;

     TCanvas* c0 = new TCanvas("chi2","chi2");
     c0->cd();
     chi2->GetYaxis()->SetRangeUser(0,3.);
     chi2->GetYaxis()->SetTitle("#chi^{2}");
     chi2->GetXaxis()->SetTitle("m [GeV]");
     TPaveText* pt0 = MakeTPave();
     std::ostringstream pprint;
     pprint.str("");
     pprint<<chi2->Integral()/double(hratio[i]->GetNbinsX());
     pt0->AddText("#chi^{2}/ndf (CTEQ12) = "+TString(pprint.str()));
     pprint.str("");
     chi2->SetMarkerStyle(20);
     chi2->SetMarkerColor(kRed);
     chi2->SetLineColor(kRed);
     chi2->GetXaxis()->SetMoreLogLabels();
     chi2->Draw("PL");
     pt0->Draw("same");

   }
}
