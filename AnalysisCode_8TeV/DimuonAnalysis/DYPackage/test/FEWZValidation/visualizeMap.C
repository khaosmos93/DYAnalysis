void slicePlotter(double a[][20], string);
Double_t fitFCN(Double_t *x, Double_t *par);
TH1D* howerPointer(TH1D* h, TString label);
TH2D* howerPointer2D(TH2D* h, TString label);
void printWeightMap(TH1D* h1,TH1D* h2,TH1D* h3,TH1D* h4,TH1D* h5,TH1D* h6,TH1D* h7,TString label);
//template <typename TH, typename B>
//TH* howerPointer(TH* h, B label);
void histoSplitter7(TH1D* h1, TH1D* h2,TH1D* h3,TH1D* h4,TH1D* h5,TH1D* h6,TH1D* h7,TH2D* map, TString label);

void visualizeMap(void) {

  gStyle->SetOptTitle(0);
  gStyle->SetPaintTextFormat("4.3f");
  gROOT->ProcessLine(".L ../tools/histotools.C");

   slicePlotter2("1520");
   slicePlotter2("2030");
   slicePlotter2("3045");
   slicePlotter2("4560");
   slicePlotter2("6072");
   slicePlotter2("72106");
   slicePlotter2("106120");
   slicePlotter2("120133");
   slicePlotter2("133150");
   slicePlotter2("150171");
   slicePlotter2("171200");
   slicePlotter2("200400");
   slicePlotter2("400510");
   slicePlotter2("510600");
   slicePlotter2("6001000");
   slicePlotter2("10001500");

}

Double_t fitFCN(Double_t *x, Double_t *par) {
   //return  par[1]*TMath::Exp(par[2]*x[0]) + par[0]; //par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0];
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0];
}



TLegend*  MakeLegend() {

   //TLegend *_leg= new TLegend(0.40,0.17,0.65,0.37);
   TLegend *_leg= new TLegend(0.20,0.70,0.45,0.90);
   _leg->SetTextFont(72);
   _leg->SetTextSize(0.045);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);
   return _leg;

}

void slicePlotter2(string label) {

  const int nptbins = 21;
  const int nrapbins = 8;
  double rap_bin[nrapbins] = {0.,0.2,0.4,0.7,1.1,1.9,2.4,1000.0};
  double pt_bin[nptbins] = {0.0,20.0,30.,35.,40.,45.,50.,55.,60.,65.,70.,75.,80.,85.,90.,100.,120.,150.,300.,600.,1000.};

  //new binning
  const int nptbins_new1 = 11;
  double pt_bin_new1[nptbins_new1] = {0.0,20.0,30.,35.,40.,45.,50.,60.,90.,200.,1000.};

  const int nptbins_new2 = 4;
  double pt_bin_new2[nptbins_new2] = {0.0,20.,100.,1000.};

  TFile* fPOW = new TFile("./rootfiles/DYM_40mass8rap_2.root");
  fPOW->cd();
  TH2D* xsecPOW;
  if (label != "72106") { 
     xsecPOW = (TH2D*)gDirectory->Get("xsec_"+TString(label)+"bin");
  } else {
     xsecPOW = (TH2D*)gDirectory->Get("xsec_60120bin");
  }

  TFile* fFEW = new TFile("xsecFEW_"+TString(label)+"bin.root"); 
  fFEW->cd();
  TH2D* xsecFEW = (TH2D*)gDirectory->Get("xsecFEW_"+TString(label)+"bin"); 

  //Pre-slicing
  ostringstream istr;
  istr << label;

  TH1D* xfew_slice1 = new TH1D("fewz_"+TString(istr.str())+"_slice1","fewz_"+TString(istr.str())+"_slice1",nptbins-1, pt_bin);
  TH1D* xfew_slice2 = new TH1D("fewz_"+TString(istr.str())+"_slice2","fewz_"+TString(istr.str())+"_slice2",nptbins-1, pt_bin);
  TH1D* xfew_slice3 = new TH1D("fewz_"+TString(istr.str())+"_slice3","fewz_"+TString(istr.str())+"_slice3",nptbins-1, pt_bin);
  TH1D* xfew_slice4 = new TH1D("fewz_"+TString(istr.str())+"_slice4","fewz_"+TString(istr.str())+"_slice4",nptbins-1, pt_bin);
  TH1D* xfew_slice5 = new TH1D("fewz_"+TString(istr.str())+"_slice5","fewz_"+TString(istr.str())+"_slice5",nptbins-1, pt_bin);
  TH1D* xfew_slice6 = new TH1D("fewz_"+TString(istr.str())+"_slice6","fewz_"+TString(istr.str())+"_slice6",nptbins-1, pt_bin);
  TH1D* xfew_slice7 = new TH1D("fewz_"+TString(istr.str())+"_slice7","fewz_"+TString(istr.str())+"_slice7",nptbins-1, pt_bin);

  histoSplitter7(xfew_slice1,xfew_slice2,xfew_slice3,xfew_slice4,xfew_slice5,xfew_slice6,xfew_slice7,xsecFEW, TString(label));

  TH1D* xpow_slice1 = new TH1D("pow_"+TString(istr.str())+"_slice1","pow_"+TString(istr.str())+"_slice1",nptbins-1, pt_bin);
  TH1D* xpow_slice2 = new TH1D("pow_"+TString(istr.str())+"_slice2","pow_"+TString(istr.str())+"_slice2",nptbins-1, pt_bin);
  TH1D* xpow_slice3 = new TH1D("pow_"+TString(istr.str())+"_slice3","pow_"+TString(istr.str())+"_slice3",nptbins-1, pt_bin);
  TH1D* xpow_slice4 = new TH1D("pow_"+TString(istr.str())+"_slice4","pow_"+TString(istr.str())+"_slice4",nptbins-1, pt_bin);
  TH1D* xpow_slice5 = new TH1D("pow_"+TString(istr.str())+"_slice5","pow_"+TString(istr.str())+"_slice5",nptbins-1, pt_bin);
  TH1D* xpow_slice6 = new TH1D("pow_"+TString(istr.str())+"_slice6","pow_"+TString(istr.str())+"_slice6",nptbins-1, pt_bin);
  TH1D* xpow_slice7 = new TH1D("pow_"+TString(istr.str())+"_slice7","pow_"+TString(istr.str())+"_slice7",nptbins-1, pt_bin);

  histoSplitter7(xpow_slice1,xpow_slice2,xpow_slice3,xpow_slice4,xpow_slice5,xpow_slice6,xpow_slice7,xsecPOW, TString(label));

  //FIXME the best place to do rebin in rapidity is here
  //Rebin the Y binning at high mass
  if (label == "1520" || label == "2030" || label == "3045" || label == "4560") {
      //merge slices 1,2,3
      //0.,0.7,1.1,1.9,2.4,1000.0
      xfew_slice1->Add(xfew_slice2); 
      xfew_slice1->Add(xfew_slice3);
      xfew_slice2 = (TH1D*)xfew_slice4->Clone();
      xfew_slice3 = (TH1D*)xfew_slice5->Clone();
      xfew_slice4 = (TH1D*)xfew_slice6->Clone();
      xfew_slice5 = (TH1D*)xfew_slice7->Clone();
      xfew_slice6->Scale(0);
      xfew_slice7->Scale(0);

      xpow_slice1->Add(xpow_slice2);
      xpow_slice1->Add(xpow_slice3);
      xpow_slice2 = (TH1D*)xpow_slice4->Clone();
      xpow_slice3 = (TH1D*)xpow_slice5->Clone();
      xpow_slice4 = (TH1D*)xpow_slice6->Clone();
      xpow_slice5 = (TH1D*)xpow_slice7->Clone();
  } else if (label == "6072" || label == "72106") {
      //merge slices 1,2,3
      //merge slices 4,5
      //merge 6,7
      //0.,0.7,1.9,1000.0
      xfew_slice1->Add(xfew_slice2);
      xfew_slice1->Add(xfew_slice3);
      xfew_slice4->Add(xfew_slice5);
      xfew_slice6->Add(xfew_slice7);
      xfew_slice2 = (TH1D*)xfew_slice4->Clone();
      xfew_slice3 = (TH1D*)xfew_slice6->Clone();
      xfew_slice4->Scale(0);
      xfew_slice5->Scale(0);
      xfew_slice6->Scale(0);
      xfew_slice7->Scale(0);

      xpow_slice1->Add(xpow_slice2);
      xpow_slice1->Add(xpow_slice3);
      xpow_slice4->Add(xpow_slice5);
      xpow_slice6->Add(xpow_slice7);
      xpow_slice2 = (TH1D*)xpow_slice4->Clone();
      xpow_slice3 = (TH1D*)xpow_slice6->Clone();
  } else {
      //merge slices 0-0.7, 0.7-10000
      //merge slices 1,2,3, and merge slices 4,5,6,7
      //0.,0.7,1000.0
      xfew_slice1->Add(xfew_slice2);
      xfew_slice1->Add(xfew_slice3);
      xfew_slice4->Add(xfew_slice5);
      xfew_slice4->Add(xfew_slice6);
      xfew_slice4->Add(xfew_slice7);
      xfew_slice2 = (TH1D*)xfew_slice4->Clone();
      xfew_slice3->Scale(0);
      xfew_slice4->Scale(0); 
      xfew_slice5->Scale(0);
      xfew_slice6->Scale(0);
      xfew_slice7->Scale(0);

      xpow_slice1->Add(xpow_slice2);
      xpow_slice1->Add(xpow_slice3);
      xpow_slice4->Add(xpow_slice5);
      xpow_slice4->Add(xpow_slice6);
      xpow_slice4->Add(xpow_slice7);
      xpow_slice2 = (TH1D*)xpow_slice4->Clone();
  } 

  //calculate the weight map
  TH1D* map_slice1;
  TH1D* map_slice2;
  TH1D* map_slice3;
  TH1D* map_slice4;
  TH1D* map_slice5;
  TH1D* map_slice6;
  TH1D* map_slice7;


  //Rebin the PT binning at high mass
  if (label == "1520" || label == "2030" || label == "3045" || label == "4560" || label == "6072" || label == "72106") {

     //calculate the weight map
     map_slice1 = new TH1D("map_"+TString(istr.str())+"_slice1","map_"+TString(istr.str())+"_slice1",nptbins_new1-1, pt_bin_new1);
     map_slice2 = new TH1D("map_"+TString(istr.str())+"_slice2","map_"+TString(istr.str())+"_slice2",nptbins_new1-1, pt_bin_new1);
     map_slice3 = new TH1D("map_"+TString(istr.str())+"_slice3","map_"+TString(istr.str())+"_slice3",nptbins_new1-1, pt_bin_new1);
     map_slice4 = new TH1D("map_"+TString(istr.str())+"_slice4","map_"+TString(istr.str())+"_slice4",nptbins_new1-1, pt_bin_new1);
     map_slice5 = new TH1D("map_"+TString(istr.str())+"_slice5","map_"+TString(istr.str())+"_slice5",nptbins_new1-1, pt_bin_new1);
     map_slice6 = new TH1D("map_"+TString(istr.str())+"_slice6","map_"+TString(istr.str())+"_slice6",nptbins_new1-1, pt_bin_new1);
     map_slice7 = new TH1D("map_"+TString(istr.str())+"_slice7","map_"+TString(istr.str())+"_slice7",nptbins_new1-1, pt_bin_new1);

     //FIXME
     xfew_slice1->Rebin(nptbins_new1-1,"hnew_FEW1",pt_bin_new1);
     TH1D* xfew_slice1 = howerPointer(xfew_slice1, "hnew_FEW1");

     xfew_slice2->Rebin(nptbins_new1-1,"hnew_FEW2",pt_bin_new1);
     TH1D* xfew_slice2 = howerPointer(xfew_slice2, "hnew_FEW2");

     xfew_slice3->Rebin(nptbins_new1-1,"hnew_FEW3",pt_bin_new1);
     TH1D* xfew_slice3 = howerPointer(xfew_slice3, "hnew_FEW3");

     xfew_slice4->Rebin(nptbins_new1-1,"hnew_FEW4",pt_bin_new1);
     TH1D* xfew_slice4 = howerPointer(xfew_slice4, "hnew_FEW4");

     xfew_slice5->Rebin(nptbins_new1-1,"hnew_FEW5",pt_bin_new1);
     TH1D* xfew_slice5 = howerPointer(xfew_slice5, "hnew_FEW5");

     xfew_slice6->Rebin(nptbins_new1-1,"hnew_FEW6",pt_bin_new1);
     TH1D* xfew_slice6 = howerPointer(xfew_slice6, "hnew_FEW6");

     xfew_slice7->Rebin(nptbins_new1-1,"hnew_FEW7",pt_bin_new1);
     TH1D* xfew_slice7 = howerPointer(xfew_slice7, "hnew_FEW7");

     xpow_slice1->Rebin(nptbins_new1-1,"hnew_POW1",pt_bin_new1);
     TH1D* xpow_slice1 = howerPointer(xpow_slice1, "hnew_POW1");

     xpow_slice2->Rebin(nptbins_new1-1,"hnew_POW2",pt_bin_new1);
     TH1D* xpow_slice2 = howerPointer(xpow_slice2, "hnew_POW2");

     xpow_slice3->Rebin(nptbins_new1-1,"hnew_POW3",pt_bin_new1);
     TH1D* xpow_slice3 = howerPointer(xpow_slice3, "hnew_POW3");

     xpow_slice4->Rebin(nptbins_new1-1,"hnew_POW4",pt_bin_new1);
     TH1D* xpow_slice4 = howerPointer(xpow_slice4, "hnew_POW4");

     xpow_slice5->Rebin(nptbins_new1-1,"hnew_POW5",pt_bin_new1);
     TH1D* xpow_slice5 = howerPointer(xpow_slice5, "hnew_POW5");

     xpow_slice6->Rebin(nptbins_new1-1,"hnew_POW6",pt_bin_new1);
     TH1D* xpow_slice6 = howerPointer(xpow_slice6, "hnew_POW6");

     xpow_slice7->Rebin(nptbins_new1-1,"hnew_POW7",pt_bin_new1);
     TH1D* xpow_slice7 = howerPointer(xpow_slice7, "hnew_POW7");
  } else {

    //calculate the weight map
    map_slice1 = new TH1D("map_"+TString(istr.str())+"_slice1","map_"+TString(istr.str())+"_slice1",nptbins_new2-1, pt_bin_new2);
    map_slice2 = new TH1D("map_"+TString(istr.str())+"_slice2","map_"+TString(istr.str())+"_slice2",nptbins_new2-1, pt_bin_new2);
    map_slice3 = new TH1D("map_"+TString(istr.str())+"_slice3","map_"+TString(istr.str())+"_slice3",nptbins_new2-1, pt_bin_new2);
    map_slice4 = new TH1D("map_"+TString(istr.str())+"_slice4","map_"+TString(istr.str())+"_slice4",nptbins_new2-1, pt_bin_new2);
    map_slice5 = new TH1D("map_"+TString(istr.str())+"_slice5","map_"+TString(istr.str())+"_slice5",nptbins_new2-1, pt_bin_new2);
    map_slice6 = new TH1D("map_"+TString(istr.str())+"_slice6","map_"+TString(istr.str())+"_slice6",nptbins_new2-1, pt_bin_new2);
    map_slice7 = new TH1D("map_"+TString(istr.str())+"_slice7","map_"+TString(istr.str())+"_slice7",nptbins_new2-1, pt_bin_new2);

     xfew_slice1->Rebin(nptbins_new2-1,"hnew_FEW1",pt_bin_new2);
     TH1D* xfew_slice1 = howerPointer(xfew_slice1, "hnew_FEW1");
     
     xfew_slice2->Rebin(nptbins_new2-1,"hnew_FEW2",pt_bin_new2);
     TH1D* xfew_slice2 = howerPointer(xfew_slice2, "hnew_FEW2");
     
     xfew_slice3->Rebin(nptbins_new2-1,"hnew_FEW3",pt_bin_new2);
     TH1D* xfew_slice3 = howerPointer(xfew_slice3, "hnew_FEW3");
     
     xfew_slice4->Rebin(nptbins_new2-1,"hnew_FEW4",pt_bin_new2);
     TH1D* xfew_slice4 = howerPointer(xfew_slice4, "hnew_FEW4");
     
     xfew_slice5->Rebin(nptbins_new2-1,"hnew_FEW5",pt_bin_new2);
     TH1D* xfew_slice5 = howerPointer(xfew_slice5, "hnew_FEW5");
     
     xfew_slice6->Rebin(nptbins_new2-1,"hnew_FEW6",pt_bin_new2);
     TH1D* xfew_slice6 = howerPointer(xfew_slice6, "hnew_FEW6");
     
     xfew_slice7->Rebin(nptbins_new2-1,"hnew_FEW7",pt_bin_new2);
     TH1D* xfew_slice7 = howerPointer(xfew_slice7, "hnew_FEW7");

     xpow_slice1->Rebin(nptbins_new2-1,"hnew_POW1",pt_bin_new2);
     TH1D* xpow_slice1 = howerPointer(xpow_slice1, "hnew_POW1");
     
     xpow_slice2->Rebin(nptbins_new2-1,"hnew_POW2",pt_bin_new2);
     TH1D* xpow_slice2 = howerPointer(xpow_slice2, "hnew_POW2");
     
     xpow_slice3->Rebin(nptbins_new2-1,"hnew_POW3",pt_bin_new2);
     TH1D* xpow_slice3 = howerPointer(xpow_slice3, "hnew_POW3");
     
     xpow_slice4->Rebin(nptbins_new2-1,"hnew_POW4",pt_bin_new2);
     TH1D* xpow_slice4 = howerPointer(xpow_slice4, "hnew_POW4");
     
     xpow_slice5->Rebin(nptbins_new2-1,"hnew_POW5",pt_bin_new2);
     TH1D* xpow_slice5 = howerPointer(xpow_slice5, "hnew_POW5");
     
     xpow_slice6->Rebin(nptbins_new2-1,"hnew_POW6",pt_bin_new2);
     TH1D* xpow_slice6 = howerPointer(xpow_slice6, "hnew_POW6");

     xpow_slice7->Rebin(nptbins_new2-1,"hnew_POW7",pt_bin_new2);
     TH1D* xpow_slice7 = howerPointer(xpow_slice7, "hnew_POW7");
  }

  removeErrors(xfew_slice1,10000.);
  removeErrors(xfew_slice2,10000.);
  removeErrors(xfew_slice3,10000.);
  removeErrors(xfew_slice4,10000.);
  removeErrors(xfew_slice5,10000.);
  removeErrors(xfew_slice6,10000.);
  removeErrors(xfew_slice7,10000.);

  //xpow_slice1->Sumw2();
  //xpow_slice2->Sumw2();
  //xpow_slice3->Sumw2();
  //xpow_slice4->Sumw2();
  //xpow_slice5->Sumw2();
  //xpow_slice6->Sumw2();
  //xpow_slice7->Sumw2();

  map_slice1->Divide(xfew_slice1,xpow_slice1);
  map_slice2->Divide(xfew_slice2,xpow_slice2);
  map_slice3->Divide(xfew_slice3,xpow_slice3);
  map_slice4->Divide(xfew_slice4,xpow_slice4);
  map_slice5->Divide(xfew_slice5,xpow_slice5);
  map_slice6->Divide(xfew_slice6,xpow_slice6);
  map_slice7->Divide(xfew_slice7,xpow_slice7);

  for (int j = 0; j < map_slice1->GetXaxis()->GetNbins(); j++) {
        map_slice1->SetBinError(j+1,0.002*map_slice1->GetBinError(j+1));
        map_slice2->SetBinError(j+1,0.002*map_slice2->GetBinError(j+1));
        map_slice3->SetBinError(j+1,0.002*map_slice3->GetBinError(j+1));
        map_slice4->SetBinError(j+1,0.002*map_slice4->GetBinError(j+1));
        map_slice5->SetBinError(j+1,0.002*map_slice5->GetBinError(j+1));
        map_slice6->SetBinError(j+1,0.002*map_slice6->GetBinError(j+1));
        map_slice7->SetBinError(j+1,0.002*map_slice7->GetBinError(j+1));
  }

     TLegend* leg = MakeLegend();
     //leg->AddEntry(map_slice1,"|Y| < 0.7","p");
     //leg->AddEntry(map_slice2,"0.7 < |Y| < 2.4","p");
/*
     15-64 GeV mass region
     leg->AddEntry(map_slice1,"|Y| < 0.7","p");
     leg->AddEntry(map_slice2,"0.7 < |Y| < 1.1","p");
     leg->AddEntry(map_slice3,"1.1 < |Y| < 1.9","p");
     leg->AddEntry(map_slice4,"1.9 < |Y| < 2.4","p");
     leg->AddEntry(map_slice5,"2.4 < |Y| < 1000","p");
*/
     //64-120
/*
     leg->AddEntry(map_slice1,"|Y| < 0.7","p");
     leg->AddEntry(map_slice2,"0.7 < |Y| < 1.9","p");
     leg->AddEntry(map_slice3,"1.9 < |Y| < 1000","p");
*/
     //120+
     leg->AddEntry(map_slice1,"|Y| < 0.7","p");
     leg->AddEntry(map_slice2,"0.7 < |Y| < 1000","p");



     TCanvas *c = new TCanvas(map_slice1->GetName(),map_slice1->GetName(),800,600);
     c->cd();
     //c->SetLogx();
     c->SetRightMargin(0.13);
     gStyle->SetOptTitle(0);
     gStyle->SetPalette(1);
     gStyle->SetOptStat(kFALSE);
     map_slice1->GetXaxis()->SetTitle("Dimuon p_{T} [GeV]");
     map_slice1->GetYaxis()->SetTitle("k-factor");
     map_slice1->GetXaxis()->SetMoreLogLabels();
     map_slice1->SetMarkerStyle(22);
     map_slice1->GetYaxis()->SetRangeUser(0.5,2);
     map_slice1->Draw("P");
     map_slice2->SetMarkerStyle(22);
     map_slice2->SetMarkerColor(kRed);
     map_slice2->SetLineColor(kRed);
     map_slice2->Draw("Psame");
     map_slice3->SetMarkerStyle(22);
     map_slice3->SetMarkerColor(kOrange);
     map_slice3->SetLineColor(kOrange);
     map_slice3->Draw("Psame");
     map_slice4->SetMarkerStyle(22);
     map_slice4->SetMarkerColor(kBlue);
     map_slice4->SetLineColor(kBlue);
     map_slice4->Draw("Psame");
     map_slice5->SetMarkerStyle(22);
     map_slice5->SetMarkerColor(kGreen);
     map_slice5->SetLineColor(kGreen);
     map_slice5->Draw("Psame");
     map_slice6->SetMarkerStyle(22);
     map_slice6->SetMarkerColor(kViolet);
     map_slice6->SetLineColor(kViolet);
     map_slice6->Draw("Psame");
     map_slice7->SetMarkerStyle(22);
     map_slice7->SetMarkerColor(kGreen+2);
     map_slice7->SetLineColor(kGreen+2);
     map_slice7->Draw("Psame");
     //c->SaveAs(TString(label)+".png");
     leg->Draw("same");

     printWeightMap(map_slice1,map_slice2,map_slice3,map_slice4,map_slice5,map_slice6,map_slice7,label);

}


TH1D* howerPointer(TH1D* h, TString label) {
   TH1D* hb = (TH1D*)gDirectory->Get(label);
   delete h;
   TH1D* h = (TH1D*)hb->Clone();
   return h;
}

TH2D* howerPointer2D(TH2D* h, TString label) {
   TH2D* hb = (TH2D*)gDirectory->Get(label);
   delete h;
   TH2D* h = (TH2D*)hb->Clone();
   return h;
}


void histoSplitter7(TH1D* h1, TH1D* h2,TH1D* h3,TH1D* h4,TH1D* h5,TH1D* h6,TH1D* h7,TH2D* map, TString label) {

  for (int j = 0; j < map->GetYaxis()->GetNbins(); j++) {
     for (int i = 0; i < map->GetXaxis()->GetNbins(); i++) {
       switch(i) {
          case 0:
             h1->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h1->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          case 1:
             h2->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h2->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          case 2:
             h3->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h3->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          case 3:
             h4->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h4->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          case 4:
             h5->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h5->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          case 5:
             h6->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h6->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          case 6:
             h7->SetBinContent(j+1,map->GetBinContent(i+1,j+1));
             h7->SetBinError(j+1,map->GetBinError(i+1,j+1));
             break;
          default:
             cout << "Bla" << endl;
     }
   }
 }

}

void printWeightMap(TH1D* h1,TH1D* h2,TH1D* h3,TH1D* h4,TH1D* h5,TH1D* h6,TH1D* h7,TString label) {
 
   int netabins = 7;
   if (h7->Integral() == 0) netabins--;
   if (h6->Integral() == 0) netabins--;
   if (h5->Integral() == 0) netabins--;
   if (h4->Integral() == 0) netabins--;
   if (h3->Integral() == 0) netabins--;
   if (h2->Integral() == 0) netabins--;
   if (h1->Integral() == 0) netabins--;

   //ostringstream istr; 
   //istr << h1->GetXaxis()->GetNbins();

   cout  << "double weights_fewz_"+TString(label)+"[" << netabins << "][" << h1->GetXaxis()->GetNbins() << "] = {{" << endl;
   for (int ieta = 0; ieta < netabins; ieta++) {
      for (int ipt = 0; ipt < h1->GetXaxis()->GetNbins(); ipt++) {
       if (ipt != h1->GetXaxis()->GetNbins()-1) { //nptbins-2) {
           switch(ieta) {
             case(0):
                 cout << h1->GetBinContent(ipt+1) << ", ";
                 break;
             case(1):
                 cout << h2->GetBinContent(ipt+1) << ", ";
                 break;
             case(2):
                 cout << h3->GetBinContent(ipt+1) << ", ";
                 break;
             case(3):
                 cout << h4->GetBinContent(ipt+1) << ", ";
                 break;
             case(4):
                 cout << h5->GetBinContent(ipt+1) << ", ";
                 break;
             case(5):
                 cout << h6->GetBinContent(ipt+1) << ", ";
                 break;
             case(6):
                 cout << h7->GetBinContent(ipt+1) << ", ";
                 break;
             default:
                 cout << "No" << endl;  
            }
         } else {
           switch(ieta) {
             case(0):
                 cout << h1->GetBinContent(ipt+1);
                 break;
             case(1):
                 cout << h2->GetBinContent(ipt+1);
                 break;
             case(2):
                 cout << h3->GetBinContent(ipt+1);
                 break;
             case(3):
                 cout << h4->GetBinContent(ipt+1);
                 break;
             case(4):
                 cout << h5->GetBinContent(ipt+1);
                 break;
             case(5):
                 cout << h6->GetBinContent(ipt+1);
                 break;
             case(6):
                 cout << h7->GetBinContent(ipt+1);
                 break;
             default:
                 cout << "No" << endl;
            }
         }
        }
        if (ieta != netabins-1) { //nrapbins-2) {
           cout << "}, {" << endl;
        } else {
           cout << "}};" << endl;
        }
    }//x bins loop
    cout << endl;
    cout << endl;
}
