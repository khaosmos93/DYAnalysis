//set up plots
const int nbin = 41;
double fake_par[nbin] = {
    17.2655, 22.281, 27.3237, 32.3512, 37.364, 42.3807
    ,47.3957, 52.4083, 57.4193, 61.8588, 65.9756, 70.1934
    ,74.2101, 78.6364, 84.0163, 89.0294, 92.892, 98.0758
    ,103.247, 107.881, 112.353,117.381 ,122.87 ,129.344
    ,136.824 ,145.308 ,154.795 ,165.308 ,177.695 ,192.178
    ,209.494 ,231.091 ,257.117 ,294.533 ,348.766 ,408.592
    ,473.467 ,551.11 ,754.288 ,1196.21, 1700.}; //FIXME  

double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

void draw_bin_grid( float ymin, float ymax, float xmin=0, float xmax=2000, int iV=1, int iCol=kGray+2 );
void draw_bin_grid2( float ymin, float ymax, float xmin=0, float xmax=2000, int iV=1, int iCol=kGray+2 );
void draw_axis_labels( float pos=0., float size=0.60, int iV=1 );


void plot_dratio(void) {

   TString THFOLDER = "../Inputs/Theory/";
   TString MFOLDER = "./";

   TFile* f_pdf = new TFile("../Inputs/sys/pdfu_8TeV.root");
   f_pdf->cd();
   TH1D* hpdf = (TH1D*)gDirectory->Get("pdfu");


   TFile* f7 = new TFile(THFOLDER+"1Dabsxsec_NNLO_CTEQ12NNLO_7TeV.root");
   f7->cd();
   TH1D* tev7 = (TH1D*)invm_FEWZ->Clone();
   double th_norm7 = tev7->Integral(10,22);
   TFile* f8 = new TFile(THFOLDER+"1Dabsxsec_NNLO_CTEQ12NNLO.root");
   f8->cd();
   TH1D* tev8 = (TH1D*)invm_FEWZ->Clone();
   double th_norm8 = tev8->Integral(10,22);
   TH1D* ratio = (TH1D*)tev7->Clone();
   for (int i = 0; i < tev8->GetXaxis()->GetNbins(); i++) {
      tev7->SetBinContent(i+1,tev7->GetBinContent(i+1)/tev7->GetXaxis()->GetBinWidth(i+1)/th_norm7);
      tev7->SetBinError(i+1,sqrt(pow(tev7->GetBinError(i+1)/tev7->GetBinWidth(i+1)/th_norm7,2)+0.04*pow(hpdf->GetBinContent(i+1)/100.*tev7->GetBinContent(i+1),2)));

      tev8->SetBinContent(i+1,tev8->GetBinContent(i+1)/tev8->GetXaxis()->GetBinWidth(i+1)/th_norm8);
      tev8->SetBinError(i+1,sqrt(pow(tev8->GetBinError(i+1)/tev8->GetBinWidth(i+1)/th_norm8,2)+0.04*pow(hpdf->GetBinContent(i+1)/100.*tev8->GetBinContent(i+1),2)));
   }
   ratio->Divide(tev8,tev7);

   //TAKE MEASUREMENT
   //FIXME this inpout is divided by bin width and lumi
   TString fname = "doubleratio_1Dcomb_full_Bayesian.root";
   //so needs to be multiplied by lumi and divided by peak cross section
   //TFile* fd78 = new TFile(MFOLDER+"doubleratio_1Dcomb.root");
   TFile* fd78 = new TFile(MFOLDER+fname);
   fd78->cd();
   TH1D* dratio = (TH1D*)gDirectory->Get("hxsec");

   //// 
   TH1D* lumi_band = (TH1D*)dratio->Clone();
   for (int i = 0; i < dratio->GetNbinsX(); i++) {
      lumi_band->SetBinError(i+1,lumi_band->GetBinContent(i+1)*sqrt(pow(0.034,2)+pow(dratio->GetBinError(i+1)/dratio->GetBinContent(i+1),2)));
   }  

   TGraphAsymmErrors* gratio = new TGraphAsymmErrors(nbin-1);
   for( size_t ii=0; ii<nbin; ii++ )
   {
      double x_ = fake_par[ii];
      double y_ = dratio->GetBinContent(ii+1);
      double exl_ = fake_par[ii]-mass_xbin[ii];
      double exh_ = mass_xbin[ii+1]-fake_par[ii];
      double eyl_ = dratio->GetBinError(ii+1);
      double eyh_ = eyl_;
      gratio->SetPoint(ii,x_,y_);
      gratio->SetPointError(ii,exl_,exh_,eyl_,eyh_);
   }

   //MAKE PLOTS
   TCanvas* c = new TCanvas("dratio","dratio",800,600);
   c->cd();
   c->SetLogx();
   c->SetRightMargin(0.04);
   c->SetGridx(kFALSE);
   c->SetGridy(kFALSE);
/*
   lumi_band->SetFillColor(kMagenta+1);
   lumi_band->SetLineColor(kMagenta+1);
   lumi_band->SetFillStyle(3001);
   lumi_band->GetXaxis()->SetMoreLogLabels();
   lumi_band->GetYaxis()->SetRangeUser(0.4,2.0);
   lumi_band->GetYaxis()->SetTitleSize(0.04);
   lumi_band->GetYaxis()->SetTitleOffset(1.35);
   //cout << ratio->GetYaxis()->GetTitleOffset()<< endl;
   lumi_band->GetYaxis()->SetTitle("(1/#sigma_{Z}d#sigma/dm(8 TeV))/(1/#sigma_{Z}d#sigma/dm(7 TeV))");
   lumi_band->GetXaxis()->SetTitle("m [GeV]");
   lumi_band->Draw("E3");
*/

   ratio->GetXaxis()->SetMoreLogLabels();
   ratio->GetYaxis()->SetRangeUser(0.25,2.0);
   ratio->GetYaxis()->SetTitleSize(0.05);
   ratio->GetYaxis()->SetTitleOffset(1.12);
   ratio->GetXaxis()->SetTitleOffset(1.);
   //cout << ratio->GetYaxis()->GetTitleOffset()<< endl;
   ratio->GetYaxis()->SetTitle("R"); //1/#sigma_{Z}d#sigma/dm(8 TeV)/(1/#sigma_{Z}d#sigma/dm(7 TeV))");
   ratio->GetXaxis()->SetTitle("m [GeV]");
   ratio->GetXaxis()->SetLabelSize(0.);

   ratio->SetLineColor(kBlue);
   //ratio->SetLineWidth(3);
   ratio->SetMarkerSize(0);
   ratio->SetFillColor(kBlue);
   ratio->SetFillStyle(3144);
   ratio->Draw("E3");
    

  float r0_ = 1.;
  float dr_ = 0.75;
  draw_axis_labels(r0_-1.05*dr_,0.047);
  gratio->SetMarkerColor(kViolet);
  gratio->SetMarkerSize(0.9);
  gratio->SetLineColor(kBlack);
  gratio->SetMarkerStyle(20);
   //gratio->SetMarkerColor(kRed);
   //gratio->SetLineColor(kRed);
   gratio->Draw("Psame");

  TAxis *xaxis = ratio->GetXaxis();
  //xaxis->SetTickLength(0);
  xaxis->SetMoreLogLabels();
  xaxis->SetNoExponent();

  double yminl_ = ratio->GetMinimum();
  double ymaxl_ = ratio->GetMaximum();
  //draw_bin_grid2( ymaxl_/1.033, ymaxl_ );
  //draw_bin_grid( ymaxl_/1.015, ymaxl_ );
  //draw_bin_grid2( yminl_, 1.2*yminl_ );
  //draw_bin_grid( yminl_, 1.1*yminl_ );
  //draw_bin_grid( yminl_, 1.15*yminl_ );
  //draw_bin_grid( ymaxl_/1.02, ymaxl_ );
  


   TLegend *leg= new TLegend(0.17,0.20,0.35,0.40);
  leg->SetTextFont(42);
  leg->SetTextSize(0.055);
  //leg->SetTextAlign(31); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
   leg->AddEntry(gratio,"Data","lp");
   //leg->AddEntry(lumi_band,"Data, syst. and lumi. errors","f");
   leg->AddEntry(ratio,"FEWZ, NNLO CT10","f");
   leg->Draw("same");

   TPaveText *pt = new TPaveText(0.22,0.86,0.25,0.89,"brNDC");
   pt->SetFillColor(0);
   pt->SetTextFont(61);
   pt->SetTextSize(0.062);
   pt->SetBorderSize(0);
   pt->SetTextAlign(21);
   pt->AddText("CMS");
   pt->Draw("same");

   TPaveText *pt2 = new TPaveText(0.55,0.73,0.95,0.91,"brNDC");
   pt2->SetFillColor(0);
   pt2->SetTextFont(42);
   pt2->SetTextSize(0.042);
   pt2->SetBorderSize(0);
   pt2->SetTextAlign(21);
   //pt2->AddText("CMS Preliminary, #intLdt = 19.7 fb^{-1}, at #sqrt{s}=8 TeV, "+getBinLabel(hsys->GetName(),inputIsEE));
   if (fname.Contains("comb")) { 
     pt2->AddText("19.7 fb^{-1} ee and #mu#mu (8 TeV)");
     pt2->AddText("4.8 fb^{-1} ee, 4.5 fb^{-1} #mu#mu (7 TeV)");
   } else if (fname.Contains("mu")) {
     pt2->AddText("19.7 fb^{-1} #mu#mu (8 TeV),");
     pt2->AddText("4.5 fb^{-1} #mu#mu (7 TeV)");
   } else if (fname.Contains("ee")) {
     pt2->AddText("19.7 fb^{-1} ee (8 TeV),");
     pt2->AddText("4.8 fb^{-1} ee (7 TeV)");
   }
   pt2->Draw("same");

   TLine *line=new TLine();
   //line->SetLineColor(kBlue);
   line->SetLineWidth(1);
   line->SetLineStyle(2);
   line->DrawLine(15,1,1500,1);

   double chisquared = 0.;

   for (int i =0; i < dratio->GetNbinsX(); i++) {
     chisquared+=pow((dratio->GetBinContent(i+1)-ratio->GetBinContent(i+1)),2)/(dratio->GetBinError(i+1)*dratio->GetBinError(i+1));
     //std::cout << data->GetBinError(i+1) << " " << theory->GetBinError(i+1) << std::endl;
     //chisquared+=(ee8->GetBinContent(i+1)-mumu8->GetBinContent(i+1))/sqrt(ee8->GetBinError(i+1)*mumu8->GetBinError(i+1));
   }
   cout << "Full chi2 " << chisquared/double(dratio->GetNbinsX()) << " " << TMath::Prob(chisquared,dratio->GetNbinsX()) << " " << ratio->GetNbinsX()<< endl;


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
  float mass[6] = {20,50,100,200,500,1000};
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
