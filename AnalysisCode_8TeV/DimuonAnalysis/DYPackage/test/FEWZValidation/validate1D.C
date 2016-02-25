void validate1D(void) {

  gROOT->ProcessLine(".L ../tools/plottools.C");

  TFile* ffewz= new TFile("../Inputs/Theory/1Dabsxsec_NNLO_CTEQ12NNLO.root");
  ffewz->cd();
  TH1D* hfewz = (TH1D*)gDirectory->Get("invm_FEWZ");

  TFile* fpowheg = new TFile("rootfiles/DYM_40mass8rap.root");
  fpowheg->cd();
  TH1D* hpowheg_corr = (TH1D*)gDirectory->Get("invm_GEN_corr");
  TH1D* hpowheg = (TH1D*)gDirectory->Get("invm_GEN");

  //renormalize
  cout << hpowheg_corr->Integral() << " " << hfewz->Integral() << endl;
  //hpowheg_corr->Scale(hfewz->Integral(4,29)/hpowheg_corr->Integral(4,29)); //hfewz->Integral(10,22)/hpowheg_corr->Integral(10,22));

  TH1D* hmodeling = (TH1D*)hfewz->Clone();
  TH1D* hsigned_diff = (TH1D*)hfewz->Clone();
  for (int i =0; i < hmodeling->GetXaxis()->GetNbins(); i++) {
    hmodeling->SetBinContent(i+1,fabs(hfewz->GetBinContent(i+1)-hpowheg_corr->GetBinContent(i+1))/hfewz->GetBinContent(i+1)); //fabs(hfewz->GetBinContent(i+1)-hpowheg_corr->GetBinContent(i+1))/hfewz->GetBinContent(i+1));
    hmodeling->SetBinError(i+1,0.00001);
    hsigned_diff->SetBinContent(i+1,hfewz->GetBinContent(i+1)-hpowheg_corr->GetBinContent(i+1));
    hsigned_diff->SetBinError(i+1,20.*sqrt(pow(hfewz->GetBinError(i+1),2))); //+pow(hpowheg_corr->GetBinError(i+1),2)));
  }

   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(2);
   gStyle->SetErrorX(0.5);

  TCanvas* c1 = new TCanvas("xsec","xsec");
  c1->cd();
  hfewz->GetXaxis()->SetTitle("#sigma");
  hfewz->GetXaxis()->SetTitle("M_{ll}");
  hfewz->GetXaxis()->SetMoreLogLabels();
  hfewz->SetMarkerStyle(20);
  hfewz->Draw("LP");
  hpowheg_corr->SetMarkerStyle(22);
  hpowheg_corr->SetLineColor(kRed);
  hpowheg_corr->SetMarkerColor(kRed);
  hpowheg_corr->Draw("LPsame");

   TLegend *_leg= new TLegend(0.40,0.17,0.65,0.37);
   //TLegend *_leg= new TLegend(0.20,0.37,0.45,0.57);
   _leg->SetTextFont(72);
   _leg->SetTextSize(0.060);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);
   _leg->AddEntry(hfewz,"FEWZ, NLO theory","lp");
   _leg->AddEntry(hpowheg_corr,"Reweighted POWHEG MC","lp");
   _leg->Draw("same");

  TCanvas* c2 = new TCanvas("modeling","modeling");
  c2->cd();
  hmodeling->SetMarkerStyle(20);
  hmodeling->GetXaxis()->SetMoreLogLabels();
  hmodeling->GetYaxis()->SetRangeUser(0.0,0.01);
  hmodeling->GetXaxis()->SetTitle("#delta #sigma/#sigma ");
  hmodeling->GetXaxis()->SetTitle("M_{ll}");
  hmodeling->Draw("P");

  TFile* fout = new TFile("modeling_1D.root","recreate");
  fout->cd();
  hmodeling->Write();
  fout->Close();

  TCanvas* c3 = new TCanvas("signed_diff","signed_diff");
  c3->cd();
  hsigned_diff->SetMarkerStyle(20);
  hsigned_diff->GetXaxis()->SetMoreLogLabels();
  hsigned_diff->GetXaxis()->SetTitle("#delta #sigma");
  hsigned_diff->GetXaxis()->SetTitle("M_{ll}");
  hsigned_diff->Draw("P");

  TCanvas* c4 = new TCanvas("Aux ratios","Aux ratios");
  c4->cd();
  TH1D* corr_inPOW = (TH1D*)hpowheg->Clone();
  corr_inPOW->Divide(hpowheg_corr,hpowheg);

  TH1D* theorPOW = (TH1D*)hpowheg->Clone();
  theorPOW->Divide(hfewz,hpowheg);

  double weights[40] = {1.01326, 1.01248, 1.01743, 1.0229, 1.02451, 1.02588, 1.0309, 1.03372, 1.03668, 1.04234, 1.04499, 1.04884, 1.05284, 1.05685, 1.05853, 1.06021, 1.05998, 1.05953, 1.05902, 1.05883, 1.05858, 1.05683, 1.05622, 1.05672, 1.05626, 1.05841, 1.05719, 1.05237, 1.04764, 1.04275, 1.03353, 1.02844, 1.02534, 1.02179, 1.01824, 1.01352, 1.00786, 1.00114, 0.990184, 0.965076};

//default
//1.01327, 1.01248, 1.01744, 1.02312, 1.0245, 1.02588, 1.03415, 1.03371, 1.03334, 1.04545, 1.04499, 1.04416, 1.06226, 1.06154, 1.0609, 1.06021, 1.05998, 1.05953, 1.05902, 1.05887, 1.05851, 1.0579, 1.05487, 1.05395, 1.0589, 1.05794, 1.06203, 1.06042, 1.05264, 1.0509, 1.03467, 1.03232, 1.02915, 1.02508, 1.01979, 1.01193, 1.00881, 1.00226, 0.991809, 0.967282};

  double weights_err[40] = {0.000108384, 2.18365e-05, 3.84671e-05, 2.62103e-05, 3.92606e-05, 5.51089e-05, 4.26955e-05, 5.3587e-05, 6.42819e-05, 8.25573e-05, 8.75403e-05, 8.89046e-05, 5.96393e-05, 4.74967e-05, 3.22209e-05, 1.06187e-05, 9.7674e-06, 3.52426e-05, 5.86398e-05, 0.000104307, 0.000117871, 0.000147958, 0.000199938, 0.000216119, 0.000203198, 0.000228658, 0.000794714, 0.00087132, 0.000435273, 0.000521549, 0.000290876, 0.000515324, 0.000386159, 0.000650684, 0.000578781, 0.000375967, 0.000364921, 0.000342508, 0.0002098, 0.00033438};

  TH1D* hweights = (TH1D*)corr_inPOW->Clone();

  for (int i =0; i < hweights->GetXaxis()->GetNbins(); i++) {
    hweights->SetBinContent(i+1,weights[i]);
    hweights->SetBinError(i+1,weights_err[i]);
  }

  TH1D* theorPOW_corr = (TH1D*)hpowheg->Clone();
  theorPOW_corr->Divide(hfewz,hpowheg_corr);

  theorPOW->Smooth(50);

  //corr_inPOW->SetMarkerStyle(20);
  //corr_inPOW->GetXaxis()->SetMoreLogLabels();
  //corr_inPOW->GetXaxis()->SetTitle("#delta #sigma");
  //corr_inPOW->GetXaxis()->SetTitle("M_{ll}");
  //corr_inPOW->Draw("P");
  theorPOW->GetXaxis()->SetMoreLogLabels();
  theorPOW->GetXaxis()->SetTitle("#delta #sigma");
  theorPOW->GetXaxis()->SetTitle("M_{ll}");
  theorPOW->SetMarkerStyle(20);
  theorPOW->SetMarkerColor(kRed);
  theorPOW->SetLineColor(kRed);
  theorPOW_corr->SetMarkerStyle(20);
  theorPOW_corr->SetMarkerColor(kBlue);
  theorPOW_corr->SetLineColor(kBlue);
  //theorPOW_corr->Draw("Psame");
  theorPOW->Draw("P");   
  hweights->SetMarkerStyle(20);
  hweights->SetLineColor(kBlue);
  hweights->SetMarkerColor(kBlue);
  hweights->Draw("Psame");

  TLegend* leg = MakeLegend();
  //leg->AddEntry(corr_inPOW,"POWHEG corr/POWHEG uncorr","p");
  leg->AddEntry(theorPOW,"k-factor","p");
  leg->AddEntry(hweights,"Average weight","p");

  leg->Draw("same");

}
