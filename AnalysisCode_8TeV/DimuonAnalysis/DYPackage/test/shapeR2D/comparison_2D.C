#include <vector>
#include <TH1D.h>

void comparison_2D(void) {

   bool DRAW_CHI2_ONLY = false;

   gROOT->ProcessLine(".L ../../tools/histotools.C");
   gROOT->ProcessLine(".L ../../tools/plottools.C");

   TString result_type = "_fsrUnfGood"; //_fsrBbB
   TString beautiful[6] = {"20-30 GeV","30-45 GeV","45-60 GeV","60-120 GeV","120-200 GeV","200-1500 GeV"};
   TString filenames[6] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};

   TFile* input_2D = new TFile(result_type+"/xSecDET_results_2D.root"); 

   input_2D->cd();
   TMatrixT<double> absex_input = (TMatrixT<double>)gDirectory->Get("XSec");   
   TMatrixT<double> absex_input_err = (TMatrixT<double>)gDirectory->Get("XSecErr");
   TMatrixT<double> absex_input_syserr = (TMatrixT<double>)gDirectory->Get("XSecSystErr");

   TMatrixT<double> rshape_input = (TMatrixT<double>)gDirectory->Get("normXSecByBin");
   TMatrixT<double> rshape_input_err = (TMatrixT<double>)gDirectory->Get("normXSecErrByBin");
   TMatrixT<double> rshape_input_staterr = (TMatrixT<double>)gDirectory->Get("normXSecErrByBinStat");
   TMatrixT<double> rshape_input_syserr = (TMatrixT<double>)gDirectory->Get("normXSecErrByBinSyst");

   TH1D* hrshape = new TH1D("rshape","rshape",132,0,132);
   TH1D* habsex = new TH1D("absex","absex",132,0,132);

   for (int imass = 0; imass < 7; imass++) {
     double binw = 0.1;
     if (imass == 6) binw = 0.2;
     for (int irap = 0; irap < 24; irap++) {
        //WARNING: there was a mess with shifts/dimentions. Please check it first!
        if (imass != 6) {
          if (imass !=0) {
          hrshape->SetBinContent(irap+1+(imass-1)*24,rshape_input(imass,irap)); 
          hrshape->SetBinError(irap+1+(imass-1)*24,rshape_input_err(imass,irap));   

          habsex->SetBinContent(irap+1+(imass-1)*24,absex_input(imass,irap)/binw);
          habsex->SetBinError(irap+1+(imass-1)*24,absex_input_err(imass,irap)/binw); 
          }
        } else {
          if (irap >= 12) continue; 
          hrshape->SetBinContent(irap+1+120,rshape_input(imass,irap));
          hrshape->SetBinError(irap+1+120,rshape_input_err(imass,irap));

          habsex->SetBinContent(irap+1+120,absex_input(imass,irap)/binw);
          habsex->SetBinError(irap+1+120,absex_input_err(imass,irap)/binw);
        }
     }
   }

   TH1D* h1_ee = get2DSlice(habsex, "absrap2030",false, true); //WHAT IS THIS FALSE?? adds +23 shift! 
   TH1D* h2_ee = get2DSlice(habsex, "absrap3045",false, true);
   TH1D* h3_ee = get2DSlice(habsex, "absrap4560",false, true);
   TH1D* h4_ee = get2DSlice(habsex, "absrap60120",false, true);
   TH1D* h5_ee = get2DSlice(habsex, "absrap120200",false, true);
   TH1D* h6_ee = get2DSlice(habsex, "absrap200",false, true);

   TFile* fout2 = new TFile("../absex_DET2D_EE.root","recreate");
   fout2->cd();
   h1_ee->Write("20to30");
   h2_ee->Write("30to45");
   h3_ee->Write("45to60");
   h4_ee->Write("60to120");
   h5_ee->Write("120to200");
   h6_ee->Write("200to1500");
   habsex->Write("hxsec");
   fout2->Close();

   TH1D* h1_er = get2DSlice(hrshape, "absrap2030",false, true); //WHAT IS THIS FALSE?? adds +23 shift! 
   TH1D* h2_er = get2DSlice(hrshape, "absrap3045",false, true);
   TH1D* h3_er = get2DSlice(hrshape, "absrap4560",false, true);
   TH1D* h4_er = get2DSlice(hrshape, "absrap60120",false, true);
   TH1D* h5_er = get2DSlice(hrshape, "absrap120200",false, true);
   TH1D* h6_er = get2DSlice(hrshape, "absrap200",false, true);

   //Save output in the format same as muons
   TFile* fout1 = new TFile("rshape_DET2D_EE.root","recreate");
   fout1->cd();
   h1_er->Write("20to30");
   h1_er->Write("30to45");
   h1_er->Write("45to60");
   h1_er->Write("60to120");
   h1_er->Write("120to200");
   h1_er->Write("200to1500");
   fout1->Close();


   ////////////////
   //NOW PLOT THE COMPARISON TO MUONS, FOR ABSOLUTES
   ////////////////

   //NORMALIZATION CROSS SECTION
   cout << "Normalization cross section in the dielectron channel is: " << h4_ee->Integral() << endl;

   std::vector<TH1D*> h__ee;
   h__ee.push_back(h1_ee); 
   h__ee.push_back(h2_ee);
   h__ee.push_back(h3_ee);
   h__ee.push_back(h4_ee);
   h__ee.push_back(h5_ee);
   h__ee.push_back(h6_ee);  


   //NOW make a comparison
   TFile* fmumu = new TFile("../absex_DET2D.root");
   fmumu->cd();
   TH1D* h1 = (TH1D*)gDirectory->Get("20to30");
   TH1D* h2 = (TH1D*)gDirectory->Get("30to45");
   TH1D* h3 = (TH1D*)gDirectory->Get("45to60");
   TH1D* h4 = (TH1D*)gDirectory->Get("60to120");
   cout << "Normalization cross section in the dimuon channel is: " << h4->Integral() << endl;
   TH1D* h5 = (TH1D*)gDirectory->Get("120to200");
   TH1D* h6 = (TH1D*)gDirectory->Get("200to1500");
   std::vector<TH1D*> h__mumu;
   h__mumu.push_back(h1);
   h__mumu.push_back(h2);
   h__mumu.push_back(h3);
   h__mumu.push_back(h4);
   h__mumu.push_back(h5);
   h__mumu.push_back(h6);

   //TH1D* hmumu = sliceCombiner(h1,h2,h3,h4,h5,h6,true);

   //PLOT superimposed
   TString WHICHES[6] = {"2030","3045","4560","60120","120200","2001500"};
   TH1D* hratio[6];
   for (int i = 0; i < h__mumu.size(); i++) {
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
     //h__mumu.at(i)->SetMaximum(640);
     h__mumu.at(i)->SetMaximum(h__mumu.at(i)->GetMaximum()*1.9);
     h__mumu.at(i)->SetMarkerStyle(22);
     h__mumu.at(i)->GetYaxis()->SetTitle("d#sigma/dMdy");
     h__mumu.at(i)->GetXaxis()->SetTitle("|y|");
     h__mumu.at(i)->Draw("P");
     //h__ee.at(i)->Scale(h__mumu.at(i)->Integral()/h__ee.at(i)->Integral());
     h__ee.at(i)->SetMarkerStyle(21);
     h__ee.at(i)->SetMarkerColor(kRed);
     h__ee.at(i)->SetLineColor(kRed);
     h__ee.at(i)->Draw("Psame");
 
     TLegend* leg = MakeLegend();
     leg->AddEntry(h__mumu.at(i),"Dimuons","lp");
     leg->AddEntry(h__ee.at(i),"Dielectrons","lp");
     leg->Draw("same");
     TPaveText* pt = MakeTPave();
     pt->AddText(beautiful[i]);
     pt->Draw("");

   }
     //Set up the ratio
     //cout << h__mumu.at(i)->GetNbinsX() << " XX " << h__ee.at(i)->GetNbinsX() << endl;
     hratio[i] = (TH1D*)h__mumu.at(i)->Clone();
     //FindRatio(hratio[i],h__mumu.at(i),h__ee.at(i));
     hratio[i]->Divide(h__mumu.at(i),h__ee.at(i)); 

   if (!DRAW_CHI2_ONLY ) { 
     //tmp fix of errors at high mass
     hratio[i]->SetBinError(22,1.5*hratio[i]->GetBinError(22));
     hratio[i]->SetBinError(23,1.7*hratio[i]->GetBinError(23));
     hratio[i]->SetBinError(24,2.5*hratio[i]->GetBinError(24));

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
     hratio[i]->GetYaxis()->SetTitle("Mu/Ele");
     hratio[i]->SetStats(kFALSE);
     hratio[i]->GetYaxis()->SetTitleOffset(0.29);
     hratio[i]->GetXaxis()->SetTitle("|Y_{ll}|");
     hratio[i]->GetYaxis()->SetTitleSize(0.15);
     hratio[i]->GetXaxis()->SetTitleSize(0.15);
     hratio[i]->GetYaxis()->SetLabelSize(0.10);
     hratio[i]->GetXaxis()->SetLabelSize(0.15);
     hratio[i]->SetMarkerSize(1.3);
     hratio[i]->SetMaximum(1.45);
     hratio[i]->SetMinimum(0.55);
     hratio[i]->SetMarkerStyle(20);
     hratio[i]->Draw("ep");
     //c->Update();
     TLine *line=new TLine();
     line->SetLineColor(kBlue);
     line->SetLineWidth(1);
     line->DrawLine(0,1,2.4,1);
     std::ostringstream pprint;
     pprint.str("");
     pprint<<i+1;
     string label = pprint.str();
     c->SaveAs("EMushapeR_Mhslice"+TString(label)+"_inAcc.pdf"); 
     //c->SaveAs("EMu"+TString(label)+"_inAcc.png");
   }

     //CHI2 TEST
     TH1D* chi2 = new TH1D("chi2","chi2",h__mumu.at(i)->GetNbinsX(),0,2.4);


     for (int ii =0; ii < h__mumu.at(i)->GetNbinsX(); ii++) {
       chi2->SetBinContent(ii+1,fabs(h__mumu.at(i)->GetBinContent(ii+1)-h__ee.at(i)->GetBinContent(ii+1))/sqrt(h__mumu.at(i)->GetBinError(ii+1)*h__ee.at(i)->GetBinError(ii+1)));
       chi2->SetBinError(ii+1,0.0);
     }
     cout << "Full chi2 per NDF " << chi2->Integral()/double(hratio[i]->GetNbinsX()) << " " << hratio[i]->GetNbinsX() << endl;

     TCanvas* c0 = new TCanvas("EMu"+filenames[i]+"_chi2","EMu"+filenames[i]+"_chi2");
     c0->cd();
     chi2->GetYaxis()->SetRangeUser(0,4.);
     chi2->GetYaxis()->SetTitle("#chi^{2}");
     chi2->GetXaxis()->SetTitle("|y|");
     TPaveText* pt0 = MakeTPave();
     std::ostringstream pprint;
     pprint.str("");
     pprint<<chi2->Integral()/double(hratio[i]->GetNbinsX());
     pt0->AddText("#chi^{2}/ndf (CTEQ12) = "+TString(pprint.str()));
     pprint.str("");
     chi2->SetMarkerStyle(20);
     chi2->SetMarkerColor(kRed);
     chi2->SetLineColor(kRed);
     chi2->Draw("PL");
     pt0->Draw("same");
   }

}
