bool IS_RSHAPE = false;
bool IS_7TEV = false; 
TString has41 = "_41";
 
//set up plots
const int nbin = 41;
double fake_par[nbin] = {
    17.2655, 22.281, 27.3237, 32.3512, 37.364, 42.3807 
    ,47.3957, 52.4083, 57.4193, 61.8588, 65.9756, 70.1934
    ,74.2101, 78.6364, 84.0163, 89.0294, 92.892, 98.0758
    ,103.247, 107.881, 112.353,117.381 ,122.87 ,129.344
    ,136.824 ,145.308 ,154.795 ,165.308 ,177.695 ,192.178
    ,209.494 ,231.091 ,257.117 ,294.533 ,348.766 ,408.592
    ,473.467 ,551.11 ,754.288 ,1196.21, 1700.};
            
double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

void draw_bin_grid2( float ymin, float ymax, float xmin=0, float xmax=2000, int iV=1, int iCol=kGray+2 );
void draw_bin_grid( float ymin, float ymax, float xmin=0, float xmax=2000, int iV=1, int iCol=kGray+2 );
void draw_axis_labels( float pos=0., float size=0.60, int iV=1 );

void plot_Comb(void) {

   gROOT->ProcessLine(".L ../tools/plottools.C");
  gStyle->SetPadGridX(kFALSE);
  gStyle->SetPadGridY(kFALSE);

   //SET UP FINE THEORY
   ifstream in_xsec_massNNLO4("../style/DY/root/Plain/NNLO/8TeV_CTEQ12NNLO_41.txt",ios::in);
   const int nbins4  = 339+179+25;
   double mass_xbin4[nbins4] = {0.};
   double initial = 14.0;
   for( int i = 0; i < nbins4; i++ ) {
     if( i >= 0 && i < 11 ) {
       initial += 1.0;
     }
     else if( i >= 11 && i < 18 ) {
       initial += 5.0;
     }
     else if( i >= 18 && i < 118 ) {
       initial += 1.0;
     }
     else if( i >= 118 && i < 338 ) {
       initial += 2.0;
     }
     else if (i >= 338 && i < 542-24) {
       initial += 5.0;
     } else {
       initial += 20.0;
     }
     mass_xbin4[i] = initial;
   }
   TH1D* h_theory4  = new TH1D("xsec4_NNLO","xsec4_NNLO",nbins4-1,mass_xbin4);
   TH1D* h_theory4_band  = new TH1D("xsec4_bandNNLO","xsec4_bandNNLO",nbins4-1,mass_xbin4);
   double xsec_mass4[nbins4-1];
   double xsec_err_mass4[nbins4-1];

   for( int i = 0; i < nbins4-1; i++ ) {
     double dm_ = mass_xbin4[i+1]-mass_xbin4[i];

     //NNLO  
     in_xsec_massNNLO4 >> xsec_mass4[i] >>  xsec_err_mass4[i] ;
     //FIXME fix the kink at high mass, artificially
     xsec_mass4[i] *= weightProducer(mass_xbin4[i+1]);
                                  
     double peak = 1137.2;
     if (IS_RSHAPE) {
       h_theory4_band->SetBinContent(i+1,xsec_mass4[i]/dm_/peak);
       //controls error band
       h_theory4_band->SetBinError(i+1,0.1*h_theory4_band->GetBinContent(i+1));///peak); 
       h_theory4->SetBinContent(i+1,xsec_mass4[i]/dm_/peak);
       h_theory4->SetBinError(i+1,0.0);//sqrt(pow(xsec_mass4[i]*10./peak_val_theory/peak_val_theory,2)));
     } else { 
       h_theory4_band->SetBinContent(i+1,xsec_mass4[i]/dm_);
       h_theory4_band->SetBinError(i+1,0.1*h_theory4_band->GetBinContent(i+1));
       h_theory4->SetBinContent(i+1,xsec_mass4[i]/dm_);
       h_theory4->SetBinError(i+1,0.0);//sqrt(pow(xsec_mass4[i]*10./peak_val_theory/peak_val_theory,2)));
     }
   }
   average(h_theory4_band);
   average(h_theory4);
   
   //ACCESS VALUES
   TFile* f;
   if (IS_7TEV) f = new TFile("../Inputs/Theory/1Dabsxsec_NNLO_CTEQ12NNLO_7TeV.root");
   else f = new TFile("../Inputs/Theory/1Dabsxsec_NNLO_CTEQ12NNLO"+has41+".root");
   f->cd();
   TH1D* theory = (TH1D*)gDirectory->Get("invm_FEWZ"+has41);
   double peak_th = theory->Integral(10,22);
   for (int i = 0; i < 41; i++) {
      theory->SetBinContent(i+1,theory->GetBinContent(i+1)/theory->GetBinWidth(i+1));
   }
   TH1D* rshape_theory = (TH1D*)theory->Clone();
   for (int i = 0; i < 41; i++) {
      rshape_theory->SetBinContent(i+1,theory->GetBinContent(i+1)/peak_th);
      rshape_theory->SetBinError(i+1,theory->GetBinError(i+1)/peak_th);
   }
   if (IS_RSHAPE) theory = rshape_theory;

   TFile* g;
   if (!IS_RSHAPE) {
      if (IS_7TEV)  g = new TFile("/group/cms/users/asvyatko/CMSSW_4_2_8/src/DYPackage/Outputs/absex_full2011.root");
      else g = new TFile("../Outputs/absex_full_comb_PI_Bayesian.root");
   } else { 
      if (IS_7TEV) g = new TFile("/group/cms/users/asvyatko/CMSSW_4_2_8/src/DYPackage/Outputs/rshape_full_mumuCurrentMarch.root");
      else g = new TFile("../Outputs/absex_full_comb_PI_Bayesian.root");
   }
   g->cd();
   TH1D* data = (TH1D*)hxsec->Clone();
   double peak = 0;
   double peak_err = 0.01; //1% approx, for the plot only
   for (int i = 9; i < 22; i++) {
      peak += data->GetBinContent(i+1)*data->GetBinWidth(i+1);
   }
   cout << "Peak comb data: " << peak<< endl;
   TH1D* rshape_data = (TH1D*)hxsec->Clone();
   for (int i = 0; i < 41; i++) {
      //Note: it is already pre-divided by the bin width
      rshape_data->SetBinContent(i+1,data->GetBinContent(i+1)/peak);
      rshape_data->SetBinError(i+1,sqrt(pow(data->GetBinError(i+1)/peak,2)+pow(peak_err*rshape_data->GetBinContent(i+1),2)));
   }
   if (IS_RSHAPE && !IS_7TEV) data = rshape_data;

   TGraphAsymmErrors* gdata = new TGraphAsymmErrors(nbin-1);
   for( size_t ii=0; ii<nbin; ii++ )
   {
      double x_ = fake_par[ii];
      double y_ = data->GetBinContent(ii+1);
      double exl_ = fake_par[ii]-mass_xbin[ii];
      double exh_ = mass_xbin[ii+1]-fake_par[ii];
      double eyl_ = data->GetBinError(ii+1);
      double eyh_ = eyl_;
      gdata->SetPoint(ii,x_,y_);
      gdata->SetPointError(ii,exl_,exh_,eyl_,eyh_);
   }

//FIXME need ratio plots
  TCanvas *c1 = new TCanvas("CrossSect1D","CrossSect1D",600,600);
  c1->Draw();
  c1->cd();
  TPad *p1 = new TPad("p1", "",0.0,0.25,1.0,0.98,0,0,0);
  p1->Draw();
  p1->cd();
  //p1->SetTickx(kFALSE);

  //TAxis *xaxis = h_theory4->GetXaxis();
  //xaxis->SetMoreLogLabels();
  //xaxis->SetNoExponent(); 

  TAxis *axis = h_theory4->GetYaxis();
  axis->SetTickLength(axis->GetTickLength()/1.35);
  //axis->SetNdivisions(506);


  p1->SetRightMargin(0.055);
  p1->SetBottomMargin(0.01);
  p1->SetTopMargin(0.1);
  p1->SetLogy();
  p1->SetLogx();
  h_theory4->GetXaxis()->SetMoreLogLabels();
  if (IS_RSHAPE) h_theory4->GetYaxis()->SetTitle("1/#sigma_{Z}d#sigma/dm [GeV^{-1}]");
  else h_theory4->GetYaxis()->SetTitle("d#sigma/dm [pb/GeV]"); 
  h_theory4->GetXaxis()->SetTitle("m_{#mu#mu} [GeV]");
  h_theory4->SetLineColor(kBlue);
  h_theory4->SetLineWidth(0.1);
  h_theory4->SetMarkerSize(0);
  //h_theory4->SetMaximum(h_theory4->GetMaximum()*100.);
  h_theory4->GetYaxis()->SetRangeUser(h_theory4->GetMinimum()/10.,h_theory4->GetMaximum()*10.);
  h_theory4->SetMinimum(1.5*10E-9);
  h_theory4->Draw("L");

  TH1D* h_theory4_clone = (TH1D*)h_theory4->Clone();
  h_theory4_clone->SetLineWidth(1.5);
  //h_theory4_clone->GetXaxis()->SetRangeUser(40,180);
  h_theory4_clone->Draw("Lsame");
  h_theory4_band->SetLineColor(kBlue);
  h_theory4_band->SetLineWidth(3);
  h_theory4_band->SetMarkerSize(0);
  h_theory4_band->SetFillColor(kBlue);
  h_theory4_band->Draw("E3same");
  //data->Draw("Psame");

  gdata->SetMarkerStyle(20);
  gdata->SetMarkerSize(0.7);
  gdata->Draw("Psame");

  double yminl_ = h_theory4->GetMinimum();
  double ymaxl_ = h_theory4->GetMaximum();
  //draw_bin_grid2( yminl_, 1.7*yminl_ );
  //draw_bin_grid( yminl_, 1.2*yminl_ );
  //draw_bin_grid2( ymaxl_/1.7, ymaxl_ );
  //draw_bin_grid( ymaxl_/1.2, ymaxl_ );

  // text2
  size_t ntxt = 3;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS"; // Preliminary";
  txtSize[0] = 0.072;
  txtX[0] = 0.27;
  txtY[0] = 0.82;
  txtAlign[0] = 21;
  txtFont[0] = 61;

  //txt[1] = "19.7 fb^{-1} at #sqrt{s} = 8 TeV";
  txt[1] = "19.7 fb^{-1} ee and #mu#mu (8 TeV)";
  txtSize[1] = 0.042;
  txtX[1] = 0.79; 
  txtY[1] = 0.91;
  txtAlign[1] = 21;
  txtFont[1] = 42; 
      
  txt[2] = "#gamma*/Z #rightarrow e^{+}e^{-}, #mu^{+}#mu^{-}";
  //txt[2] = "#gamma*/Z #rightarrow #font[11]{l#lower[-0.72]{^{#font[122]{+}}}l^{#font[122]{-}}}");
  txtSize[2] = 0.05;
  txtX[2] = 0.80;
  txtY[2] = 0.82;
  txtAlign[2] = 21;
  txtFont[2] = 42;

  TLatex latex;
  latex.SetNDC();
  for( size_t ii=0; ii<ntxt; ii++ ) {
     latex.SetTextFont(txtFont[ii]);
     latex.SetTextSize(txtSize[ii]);
     latex.SetTextAlign(txtAlign[ii]);
     latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
  }

  TLegend *leg= new TLegend(0.21,0.10,0.51,0.30);
  leg->SetTextFont(42);
  leg->SetTextSize(0.055);
  //leg->SetTextAlign(31); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(gdata,"data","lp");
  leg->AddEntry(h_theory4_band,"FEWZ, NNLO CT10","f");
  leg->Draw("same");

  TH1D* ratio = (TH1D*)theory->Clone();
  ratio->Divide(data,theory);

  //cout << "XX " << ratio->GetNbinsX() << " " << theory->GetNbinsX() << endl;
  //for (int i = 0; i < ratio->GetNbinsX(); i++) {
  //   cout << i+1 << ratio->GetBinContent(i+1) << endl;
  //}

  //ratio should be a graph
   TGraphAsymmErrors* gratio = new TGraphAsymmErrors(nbin-1);
   for( size_t ii=0; ii<nbin; ii++ )
   {
      double x_ = fake_par[ii];
      double y_ = ratio->GetBinContent(ii+1);
      double exl_ = fake_par[ii]-mass_xbin[ii];
      double exh_ = mass_xbin[ii+1]-fake_par[ii];
      double eyl_ = ratio->GetBinError(ii+1);
      double eyh_ = eyl_;
      gratio->SetPoint(ii,x_,y_);
      gratio->SetPointError(ii,exl_,exh_,eyl_,eyh_);
   }

  gStyle->SetOptTitle(0);
  TPad *p2 = new TPad("p2", "",0.0,0.01,1.0,0.249,0,0,0);
  c1->cd();
  p2->Draw();
  p2->cd();
  p2->SetLogx();
  p2->SetTopMargin(0.007);
  p2->SetBottomMargin(0.32);
  p2->SetRightMargin(0.055);
  //p2->SetTickx(kFALSE);
  TAxis* ay_ = ratio->GetYaxis();

  //ax_->SetNdivisions(0);
  //ax_->SetTitleOffset(1.15);
  //ax_->SetLabelOffset(99);
  
//ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(506); //506);
  ay_->SetLabelOffset(0.019);  

  ratio->GetYaxis()->SetTitle("Data/theory");
  ratio->GetXaxis()->SetTitle("m [GeV]");
  ratio->SetStats(kFALSE);
  ratio->GetYaxis()->SetTitleOffset(0.41);
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->GetYaxis()->SetTitleSize(0.15);
  ratio->GetXaxis()->SetTitleSize(0.155);
  ratio->GetYaxis()->SetLabelSize(0.15);
  ratio->GetXaxis()->SetLabelSize(0);//0.15);
  ratio->GetXaxis()->SetTickLength(0.08);
  ratio->GetXaxis()->SetNoExponent(kTRUE);

  ratio->SetMarkerSize(0);
  ratio->SetMaximum(1.7);  //1.48); //1.75); //1.52);
  ratio->SetMinimum(0.3); ////0.52); //0.25);//0.4);
  ratio->SetLineColor(kBlack);
  ratio->Draw("hist");
  //draw_bin_grid(ratio->GetMinimum(),ratio->GetMaximum());
  yminl_ = ratio->GetMinimum();
  ymaxl_ = ratio->GetMaximum();
  //draw_bin_grid2( yminl_, 1.5*yminl_ );
  //draw_bin_grid( yminl_, 1.2*yminl_ );
  //draw_bin_grid2( ymaxl_/1.07, ymaxl_ );
  //draw_bin_grid( ymaxl_/1.03, ymaxl_ );
  float r0_ = 1.;
  float dr_ = 0.75;
  //draw_axis_labels(r0_-1.09*dr_,0.15);
  draw_axis_labels(r0_-1.0*dr_,0.15);
  gratio->SetMarkerColor(kViolet);
  gratio->SetMarkerSize(0.9);
  gratio->SetLineColor(kBlack);
  gratio->SetMarkerStyle(20);
  gratio->Draw("epsame");
  p2->Update();
  TLine *line=new TLine();
  line->SetLineColor(kBlue);
  line->SetLineWidth(2);
  line->DrawLine(15,1,2000,1);
  //c1->SaveAs(etaclass+run+".pdf"); //_DR53X_HCP2012.png");

   double chisquared = 0.;

   for (int i =0; i < data->GetNbinsX(); i++) {
     chisquared+=pow((data->GetBinContent(i+1)-theory->GetBinContent(i+1)),2)/(data->GetBinError(i+1)*data->GetBinError(i+1));
     //std::cout << data->GetBinError(i+1) << " " << theory->GetBinError(i+1) << std::endl;
     //chisquared+=(ee8->GetBinContent(i+1)-mumu8->GetBinContent(i+1))/sqrt(ee8->GetBinError(i+1)*mumu8->GetBinError(i+1));
   }
   cout << "Full chi2 " << chisquared/double(data->GetNbinsX()) << " " << TMath::Prob(chisquared,data->GetNbinsX())<< endl;

}

void draw_bin_grid2( float ymin, float ymax, float xmin, float xmax, int iV, int iCol )
{
  TLine grid_;
  grid_.SetLineColor(iCol);
  grid_.SetLineStyle(kSolid);
  float lim_[7] = {15,30,60,120,240,600,2000};
  if( iV==1 )
    {
      for( size_t ii=0; ii<7; ii++ )
        {
          if( lim_[ii]>xmin&&lim_[ii]<xmax )
            grid_.DrawLine(lim_[ii],ymin,lim_[ii],ymax);
        }
    }
  else
    {
      for( size_t ii=0; ii<7; ii++ )
        {
          if( lim_[ii]>ymin&&lim_[ii]<ymax )
            grid_.DrawLine(xmin, lim_[ii],xmax,lim_[ii]);
        }
    }
}

void draw_bin_grid( float ymin, float ymax, float xmin, float xmax, int iV, int iCol )
{
  TLine grid_;
  grid_.SetLineColor(iCol);
  grid_.SetLineStyle(kSolid);
  float lim_[40] = {
    20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
    96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
    200, 220, 240, 273, 320, 380, 440, 510, 600, 1000, 1500};
  if( iV==1 )
    {
      for( size_t ii=0; ii<40; ii++ )
        {
          if( lim_[ii]>xmin&&lim_[ii]<xmax )
            grid_.DrawLine(lim_[ii],ymin,lim_[ii],ymax);
        }
    }
  else
    {
      for( size_t ii=0; ii<40; ii++ )
        {
          if( lim_[ii]>ymin&&lim_[ii]<ymax )
            grid_.DrawLine(xmin, lim_[ii],xmax,lim_[ii]);
        }
    }
}

void draw_axis_labels( float pos, float size, int iV )
{         
  TLatex* txt_ = new TLatex();
  // draw axis labels
  //  float xlab_ = 70;
  txt_->SetTextFont(42);
  txt_->SetTextSize(size);
  txt_->SetTextAngle(0);
  if( iV==1 )
    {     
      txt_->SetTextAlign(23);
    }   
  else
    {
      txt_->SetTextAlign(32);
    }
  float mass[7] = {20,50,100,200,500,1000,2000};
  for( size_t ii=0; ii<7; ii++ )
    {
      float x_, y_;
      if( iV==1 )
        {
          x_ = mass[ii]; 
          y_ = pos;
        }
      else
        {
          y_ = mass[ii];
          x_ = pos;
        }
      txt_->SetText(x_,y_,Form("%-2.0f",mass[ii])); txt_->DrawClone();
    }
}


void average(TH1D* h) {

 for (int itimes = 0; itimes < 70; itimes++) {
   for (int i = 400; i < h->GetNbinsX()-25; i++) {
      h->SetBinContent(i+1,(h->GetBinContent(i)+h->GetBinContent(i+2))/2.);
   }
 }

 //for (int itimes = 0; itimes < 10; itimes++) {
 //  for (int i = 500; i < h->GetNbinsX()-20; i++) {
 //     h->SetBinContent(i+1,(h->GetBinContent(i)+h->GetBinContent(i+2))/2.);
 //  }
 //}
}


double weightProducer(double mass) {

/*    
     if (mass_xbin4[i+1] > 1000 && mass_xbin4[i+1] < 1020) xsec_mass4[i] *= 1.1;
     if (mass_xbin4[i+1] > 1020 && mass_xbin4[i+1] < 1050) xsec_mass4[i] *= 1.15;
     if (mass_xbin4[i+1] > 1050 && mass_xbin4[i+1] < 1080) xsec_mass4[i] *= 1.2;
     if (mass_xbin4[i+1] > 1080 && mass_xbin4[i+1] < 1100) xsec_mass4[i] *= 1.25;
     if (mass_xbin4[i+1] > 1100 && mass_xbin4[i+1] < 1150) xsec_mass4[i] *= 1.4;
     if (mass_xbin4[i+1] > 1150 && mass_xbin4[i+1] < 1200) xsec_mass4[i] *= 1.5;
     if (mass_xbin4[i+1] > 1200 && mass_xbin4[i+1] < 1300) xsec_mass4[i] *= 1.7;
     if (mass_xbin4[i+1] > 1300 && mass_xbin4[i+1] < 1400) xsec_mass4[i] *= 1.86;
     if (mass_xbin4[i+1] > 1400 && mass_xbin4[i+1] < 1500) xsec_mass4[i] *= 2.1;
     if (mass_xbin4[i+1] > 1500 && mass_xbin4[i+1] < 2000) xsec_mass4[i] /= 1.3;
*/    
     if (mass < 1000 || mass > 1500) return 1.;
     if (mass > 1000 && mass <= 1020) return 1.7*mass/1500.;
     if (mass > 1020 && mass <= 1040) return 1.8*mass/1500.;
     if (mass > 1040 && mass <= 1060) return 1.8*mass/1500.;
     if (mass > 1060 && mass <= 1080) return 1.8*mass/1500.;
     if (mass > 1080 && mass <= 1100) return 1.9*mass/1500.;
     if (mass > 1100 && mass <= 1120) return 1.9*mass/1500.;
     if (mass > 1120 && mass <= 1140) return 1.9*mass/1500.;
     if (mass > 1140 && mass <= 1160) return 2.*mass/1500.;
     if (mass > 1160 && mass <= 1180) return 2.*mass/1500.;
     if (mass > 1180 && mass <= 1200) return 2*mass/1500.;
     if (mass > 1200 && mass <= 1220) return 2*mass/1500.;
     if (mass > 1220 && mass <= 1240) return 2.1*mass/1500.;
     if (mass > 1240 && mass <= 1260) return 2.1*mass/1500.;
     if (mass > 1260 && mass <= 1280) return 2.2*mass/1500.;
     if (mass > 1280 && mass <= 1300) return 2.2*mass/1500.;
     if (mass > 1300 && mass <= 1320) return 2.3*mass/1500.;
     if (mass > 1320 && mass <= 1340) return 2.3*mass/1500.;
     if (mass > 1340 && mass <= 1360) return 2.4*mass/1500.;
     if (mass > 1360 && mass <= 1380) return 2.4*mass/1500.;
     if (mass > 1380 && mass <= 1400) return 2.5*mass/1500.;
     if (mass > 1400 && mass <= 1420) return 2.5*mass/1500.;
     if (mass > 1420 && mass <= 1440) return 2.6*mass/1500.;
     if (mass > 1440 && mass <= 1460) return 2.6*mass/1500.;
     if (mass > 1460 && mass <= 1480) return 2.7*mass/1500.;
     if (mass > 1480 && mass <= 1500) return 2.7*mass/1500.;

   return 1.;

}
