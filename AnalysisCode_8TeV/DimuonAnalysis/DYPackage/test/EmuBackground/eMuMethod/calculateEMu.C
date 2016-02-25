void calculateEMu() {

 TString regime = ""; //"_NODATANORM";
 bool closureTest = false;

 //get emu data and MC
 TFile* f = new TFile("true2eBkgDataPoints_1D_OS_try.root"); 
 f->cd();
 TH1F* h_emuData_pre = (TH1F*)datahistv->Clone();
 TH1F* h_emuData = add41(h_emuData_pre);
 THStack* hs_emuMC = (THStack*)eMuVMassTot->Clone();
 TH1F* h_emuMC_pre = stack_to_TH(hs_emuMC);
 TH1F* h_emuMC = add41(h_emuMC_pre);
 TH1F* h_emuData_full = (TH1F*)h_emuData->Clone();

 //recover the QCD contribution
 TFile* f2 = new TFile("true2eBkgDataPoints_1D_SS_try.root");
 f2->cd();
 TH1F* h_emuData_SS_pre = (TH1F*)datahistv->Clone();
 TH1F* h_emuData_SS = add41(h_emuData_SS_pre);
 THStack* hs_emuMC_SS = (THStack*)eMuVMassTot->Clone();
 TH1F* h_emuMC_SS_pre = stack_to_TH(hs_emuMC_SS);
 TH1F* h_emuMC_SS = add41(h_emuMC_SS_pre);
 TH1D* missing_QCD = (TH1D*)h_emuData_SS->Clone();
 missing_QCD->Add(h_emuMC_SS,-1);
 TH1D* missing_QCD_emu = (TH1D*)missing_QCD->Clone();
 missing_QCD_emu->Scale(6198.63/4536.87);
 missing_QCD_emu->Smooth(10,"R");
 h_emuData_full->Add(missing_QCD_emu,-1);

 //TFile* ff = new TFile("missingQCD.root","recreate");
 //ff->cd();
 //missing_QCD_emu->Write();
 //ff->Close();

 //reset errors
 for (int i = 0; i < 41; i++) {
   h_emuData_full->SetBinError(i+1,h_emuData->GetBinError(i+1));
 }
 if (closureTest) h_emuData = h_emuMC;

 //get dilepton MC 
 TFile* g = new TFile("../../Inputs/rawYield/DYspectrum_mass_41.root");
 g->cd();
 TH1F* h_mumuSignal = (TH1F*)hsig_fewz->Clone();  
 TH1F* h_mumuMC = (TH1F*)hDYtautau->Clone();
 h_mumuMC->Add(hWlepton);
 h_mumuMC->Add(hqcd);
 h_mumuMC->Add(hdiboson);
 h_mumuMC->Add(httbar_mc);
 //we are going to need the background subtracted
 h_mumuSignal->Add(h_mumuMC,-1);

 TH1F* h_emuPrediction = (TH1F*)h_mumuMC->Clone();
 TH1F* h_emuPrediction_full = (TH1F*)h_mumuMC->Clone();
 TH1F* h_emu_ratio = (TH1F*)h_emuMC->Clone();
 TH1F* h_emu_ratio_full = (TH1F*)h_emuMC->Clone();
 h_emu_ratio->Divide(h_emuMC,h_emuData);
 h_emu_ratio_full->Divide(h_emuMC,h_emuData_full);
 h_emuPrediction->Divide(h_emuPrediction,h_emu_ratio);
 h_emuPrediction_full->Divide(h_emuPrediction_full,h_emu_ratio_full);

 //FIXME systematics
 TH1D* hsyst1D= (TH1D*)h_mumuMC->Clone();
 for (int i = 0; i < 41; i++) {
      //if (h_emuPrediction->GetBinContent(i+1) != 0) h_emuPrediction->SetBinError(i+1,h_emuPrediction->GetBinContent(i+1)*sqrt(pow((h_emuPrediction->GetBinContent(i+1)-h_mumuMC->GetBinContent(i+1))/(2.*h_emuPrediction->GetBinContent(i+1)),2)+pow(h_emuPrediction->GetBinError(i+1)/h_emuPrediction->GetBinContent(i+1),2)));
      //h_emuPrediction_full->SetBinError(i+1,h_emuPrediction->GetBinError(i+1));
      // FIXME systematics
      //h_mumuMC->SetBinError(i+1,h_mumuMC->GetBinContent(i+1)*sqrt(pow(0.124*httbar_mc->GetBinContent(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.05*hDYtautau->GetBinContent(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.06*(hdiboson->GetBinContent(i+1)+hWlepton->GetBinContent(i+1))/h_mumuMC->GetBinContent(i+1),2)));
      if (h_emuPrediction->GetBinContent(i+1) != 0) h_emuPrediction->SetBinError(i+1,h_emuPrediction->GetBinContent(i+1)*sqrt(pow(h_emuPrediction->GetBinError(i+1)/h_emuPrediction->GetBinContent(i+1),2)));
      h_emuPrediction_full->SetBinError(i+1,h_emuPrediction->GetBinError(i+1));
      // FIXME systematics
      //h_mumuMC->SetBinError(i+1,h_mumuMC->GetBinContent(i+1)*sqrt(pow(0.124*httbar_mc->GetBinContent(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.05*hDYtautau->GetBinContent(i+1)/h_mumuMC->GetBinContent(i+1),2)+pow(0.06*(hdiboson->GetBinContent(i+1)+hWlepton->GetBinContent(i+1))/h_mumuMC->GetBinContent(i+1),2)));
     // cout << i+1 << " " << h_emuPrediction_full->GetBinError(i+1)/h_emuPrediction_full->GetBinContent(i+1) << " " << h_mumuMC->GetBinError(i+1)/h_mumuMC->GetBinContent(i+1) << " " << h_emuPrediction_full->GetBinError(i+1)/h_emuPrediction_full->GetBinContent(i+1)/(h_mumuMC->GetBinError(i+1)/h_mumuMC->GetBinContent(i+1)) << endl;
      // FIXME systematics
      hsyst1D->SetBinContent(i+1,h_emuPrediction_full->GetBinError(i+1)/h_mumuSignal->GetBinContent(i+1));
      hsyst1D->SetBinError(i+1,0.0000000001);
  }
  TFile* fsyst1D = new TFile("systematics1D_mumu.root","recreate");
  fsyst1D->cd();
  hsyst1D->Write("hsyst1D");
  fsyst1D->Close();


 if (regime != "_NODATANORM") {

 //All emu plots
 TCanvas* c0 = new TCanvas();
 c0->cd();
 h_emuData->SetMarkerStyle(22);
 h_emuData->SetMarkerColor(kBlack);
 //h_mumuMC->Scale(h_emuPrediction->Integral(10,40)/h_mumuMC->Integral(10,40));
 h_emuMC->SetMarkerStyle(20);
 h_emuMC->SetMarkerColor(kOrange+2);
 //h_mumuMC->SetFillColor(kOrange+2);
 h_emuData->GetXaxis()->SetTitle("M_{e#mu} [GeV]");
 h_emuData->GetYaxis()->SetTitle("Events");
 h_emuData->Draw("P");
 h_emuMC->Draw("Psame");

 TCanvas* c0b = new TCanvas();
 c0b->cd();
 h_emuData_full->SetMarkerStyle(22);
 h_emuData_full->SetMarkerColor(kBlack);
 h_emuMC->SetMarkerStyle(20);
 h_emuMC->SetMarkerColor(kOrange+2);
 h_emuData_full->GetXaxis()->SetTitle("M_{e#mu} [GeV]");
 h_emuData_full->GetYaxis()->SetTitle("Events");
 h_emuData_full->Draw("P");
 h_emuMC->Draw("Psame");

 TCanvas* c0c = new TCanvas();
 c0c->cd();
 TH1D* ratio0 = (TH1D*)h_emuMC->Clone();
 h_emuMC->Sumw2();
 h_emuData->Sumw2();  
 ratio0->Divide(h_emuData,h_emuMC);
 ratio0->SetMarkerStyle(22);
 //ratio0->SetLineColor(kBlack);
 ratio0->GetXaxis()->SetMoreLogLabels(); 
 ratio0->GetXaxis()->SetTitle("M_{e#mu} [GeV]");
 ratio0->GetYaxis()->SetTitle("N_{data}/N_{MC}");
 ratio0->Draw("PE");

 //ratio after correction 
 TCanvas* c0d = new TCanvas();
 c0d->cd();
 TH1D* ratio1 = (TH1D*)h_emuMC->Clone();
 h_emuData_full->Sumw2();
 h_emuMC->Sumw2();
 ratio1->Divide(h_emuData_full,h_emuMC);
 ratio1->SetMarkerStyle(22);
 ratio1->GetXaxis()->SetMoreLogLabels();
 ratio1->GetXaxis()->SetTitle("M_{e#mu} [GeV]");
 ratio1->GetYaxis()->SetTitle("N_{data}/N_{MC}");
 ratio1->Draw("PE");

 //All mumu plots
 TCanvas* c1 = new TCanvas("EmuPrediction");
 c1->cd();
 c1->SetLogx();
 h_emuPrediction->SetMarkerStyle(22);
 h_emuPrediction->SetMarkerColor(kBlack);
 //h_mumuMC->Scale(h_emuPrediction->Integral(10,40)/h_mumuMC->Integral(10,40));
 h_mumuMC->SetMarkerStyle(20);
 h_mumuMC->SetMarkerColor(kOrange+2);
 h_mumuMC->SetLineColor(kOrange+2);
 //h_mumuMC->SetFillColor(kOrange+2);
 h_mumuMC->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
 h_mumuMC->GetYaxis()->SetTitle("Events");
 h_mumuMC->Draw("P");
 h_emuPrediction->Draw("Psame");
 c1->SaveAs("EmuPrediction.png");

 TCanvas* c3 = new TCanvas("EmuPrediction_ratio");
 c3->cd();
 c3->SetLogx();
 TH1D* ratio = (TH1D*)h_mumuMC->Clone();
 h_mumuMC->Sumw2();
 h_emuPrediction->Sumw2();  
 ratio->Divide(h_emuPrediction,h_mumuMC);
 ratio->SetMarkerStyle(22);
 ratio->GetXaxis()->SetMoreLogLabels(); 
 ratio->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
 ratio->GetYaxis()->SetTitle("N_{e#mu-pred}/N_{MC}");
 ratio->Draw("PE"); 
 c3->SaveAs("EmuPrediction_ratio.png");

 TCanvas* c4 = new TCanvas("EmuPrediction_corr");
 c4->cd();
 c4->SetLogx();
 h_emuPrediction_full->SetMarkerStyle(22);
 h_emuPrediction_full->SetMarkerColor(kBlack);
 //h_mumuMC->Scale(h_emuPrediction->Integral(10,40)/h_mumuMC->Integral(10,40));
 h_mumuMC->SetMarkerStyle(20);
 h_mumuMC->SetMarkerColor(kOrange+2);
 h_mumuMC->SetFillColor(kOrange+2);
 h_mumuMC->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
 h_mumuMC->GetYaxis()->SetTitle("Events");
 h_mumuMC->GetXaxis()->SetMoreLogLabels();
 h_mumuMC->Draw("P");
 h_emuPrediction_full->Draw("Psame");
 c4->SaveAs("EmuPrediction_corr.png");

 TCanvas* c5 = new TCanvas("EmuPrediction_ratio_corr");
 c5->cd();
 c5->SetLogx(); 
 TH1D* ratio2 = (TH1D*)h_mumuMC->Clone();
 h_mumuMC->Sumw2();
 h_emuPrediction_full->Sumw2();  

 ratio2->Divide(h_emuPrediction_full,h_mumuMC);
 ratio2->SetMarkerStyle(22);
 ratio2->GetXaxis()->SetMoreLogLabels(); 
 ratio2->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
 ratio2->GetYaxis()->SetTitle("N_{e#mu-pred}/N_{MC}");
 ratio2->Draw("PE");
 c5->SaveAs("EmuPrediction_corr_ratio.png");

}//regime


}

TH1F* stack_to_TH(THStack* hs) {

   TList *histos = hs->GetHists();
   TH1F *sum;// = (TH1F*)prototype->Clone();
   TIter next(histos);
   TH1F *hist;
   int nhists = 0;
   while ((hist =(TH1F*)next())) {
     if (nhists == 0) {
      if (hist->GetName()==TString("histo0")) { 
         //hist->Scale(49312.6/54604.8);
         sum = hist;
      } else sum = hist;
     } else {
      //if (hist->GetName()==TString("histo1")) sum->Add(hist,9158.97/9157.56);
      //if (hist->GetName()==TString("histo2")) sum->Add(hist,1399.14/1400.22);
      //if (hist->GetName()==TString("histo3")) sum->Add(hist,29961.4/29739);
      //if (hist->GetName()==TString("histo4")) sum->Add(hist,289.576/289.197);
      //if (hist->GetName()==TString("histo5")) sum->Add(hist,2858.1/2885.5);
      sum->Add(hist);
     }
     nhists++;
     cout << "Adding " << hist->GetName() << " " << hist->GetFillColor()<< " " << hist->Integral() << " " << hist->Integral(17,25) << " " << hist->Integral(25,40) << endl;
   }
  return sum; 
}

shared_ptr<TH1F> getSpecificHist(THStack* hs, TString selectThis) {

 TIter next(hs->GetHists());
 TH1F* h;
 TH1F* h_return;
 int count = 0;
 while ((h=(TH1F*)next())) {
    cout << "Processing " << h->GetName() << " integral " << h->Integral() << endl;
    if (TString(h->GetName()) == selectThis && selectThis != "histodiboson") {
        h_return = (TH1F*)gDirectory->Get(selectThis);
    } else {
        if (TString(h->GetName()) == "histowz" || TString(h->GetName()) == "histozz" || TString(h->GetName()) == "histoww") { 
           if (count == 0) h_return = (TH1F*)h->Clone();
           else h_return->Add(h);
           count++;
        }
    }
 }
 //delete h;

 return h_return;

}


TH1F* add41(TH1F* h_pre) {

 const int nbin = 41;
 double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};
            
 TH1F* h = new TH1F(TString(h_pre->GetName()),TString(h_pre->GetName()),nbin,mass_xbin);
 
 for (int i = 0; i < nbin; i++) {
  if (i < nbin-1) {  
    h->SetBinContent(i+1,h_pre->GetBinContent(i+1));
    h->SetBinError(i+1,h_pre->GetBinError(i+1));
  } else {
    h->SetBinContent(i+1,h_pre->GetBinContent(40)*0.8);
    h->SetBinError(i+1,h_pre->GetBinError(40));
  } 
 }

 return h;
}
