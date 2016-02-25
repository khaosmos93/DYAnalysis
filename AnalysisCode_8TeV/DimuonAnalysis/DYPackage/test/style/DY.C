#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <map>

#include <TROOT.h>
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TString.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TArrow.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TMarker.h"
#include "TFrame.h"
//#include "RooHist.h"

TH2F* getMigMatrix(TH1F* htrue, TH2F* hden);

using namespace std;

bool units_with_c = false;
bool GHMStyle =false;
bool salamanderColors = true;
bool dataMCRatio = true;
bool grayScale = true;
bool drawGrid  = true;
bool logScaleZ = true;

enum { Fig_1_a=0, 
       Fig_1_b, 
       Fig_9_a, 
       Fig_9_b, 
       Fig_2, 
       Fig_2_a, 
       Fig_2_b, 
       Fig_3, 
       Fig_3_a, 
       Fig_3_b, 
       Fig_4_a, 
       Fig_4_b, 
       Fig_10_a, 
       Fig_10_b, 
       Fig_10_c, 
       Fig_10_d, 
       Fig_10_e, 
       Fig_10_f, 
       Fig_5, 
       Fig_6_a, 
       Fig_6_b, 
       Fig_7_a, 
       Fig_7_b, 
       Fig_8 
};

void draw_mass_bins( TH1* data, float ymin );
void draw_bin_grid( float ymin, float ymax, float xmin=0, float xmax=1500, int iV=1, int iCol=kGray+2 );
void draw_axis_labels( float pos=0., float size=0.60, int iV=1 );

void setGrayScalePalette( int ncol, int nwhite );
void setBlueToRedPalette( const int ncoltot, int nwhite );
void setHessPalette();

//#include "Mass.C"
TCanvas* Mass( int mode )
{
  TGaxis::SetMaxDigits(3);

  float r0_ = 1.;
  float dr_ = 0.75;

  // channels, ordered as in the legend
  vector<TString> channels;  
  vector<TString> hnames;
  vector<TString> type;
  vector<size_t>  order;  

  map<TString,int> fillColor_;
  map<TString,int> lineColor_;
  int lineWidth1(1);
  int lineWidth2(1);

  if( salamanderColors )
    {
      lineWidth1 = 1;
      lineWidth2 = 1;

      fillColor_["Signal"] = kOrange-2;
      lineColor_["Signal"] = kOrange+3;
      
      fillColor_["Ztautau"] = kOrange+7;
      lineColor_["Ztautau"] = kOrange+3;

      fillColor_["VV"]     = kOrange+7;
      lineColor_["VV"]     = kOrange+3;

      fillColor_["EWK"] = kOrange+10;
      lineColor_["EWK"] = kOrange+3;

      fillColor_["QCD"] = kViolet-5;
      lineColor_["QCD"] = kViolet+3;
      
      fillColor_["ttbar"] = kRed+2;
      lineColor_["ttbar"] = kRed+4;      

      fillColor_["Y"] = kOrange+10;
      lineColor_["Y"] = kOrange+3;

      //fillColor_["Y"] = kViolet-4;
      //lineColor_["Y"] = kViolet+3;

    }
  else
    {
      lineWidth1 = 1;
      lineWidth2 = 1;

      fillColor_["Signal"] = kSpring-3;
      lineColor_["Signal"] = kSpring-3;
      
      fillColor_["Ztautau"] = kOrange;
      lineColor_["Ztautau"] = kOrange;
      
      fillColor_["EWK"] = kCyan-4;
      lineColor_["EWK"] = kCyan-4;
      
      fillColor_["VV"] = kPink-1;
      lineColor_["VV"] = kPink-1;
      
      fillColor_["QCD"] =  kMagenta-3;
      lineColor_["QCD"] =  kMagenta-3;
      
      fillColor_["Y"]   =  kBlue+2;
      lineColor_["Y"]   =  kBlue+2;
      
      fillColor_["ttbar"] = kAzure;
      lineColor_["ttbar"] = kAzure;
    }

  // text2
  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS Preliminary";
  txtSize[0] = 0.055;
  txtX[0] = 0.88;
  txtY[0] = 0.84;
  txtAlign[0] = 31;
  txtFont[0] = 42;

  if( GHMStyle )
    {
      //txt[1] = "36 pb^{-1}  at  #sqrt{s} = 8 TeV";
      txt[1] = "19.7 fb^{-1} at #sqrt{s} = 8 TeV";
      //txt[1] = "#int#font[12]{L}dt = 36 pb^{-1} at #sqrt{s} = 8 TeV";
      txtSize[1] = 0.042;
      txtX[1] = 0.40;
      txtY[1] = 0.83;
      txtAlign[1] = 21;
      txtFont[1] = 42;
    }
  else
    {
      //txt[1] = "36 pb^{-1}";
      txt[1] = "4.6 fb^{-1} at #sqrt{s} = 8 TeV";
      //txt[1] = "#int#font[12]{L}dt = 36 pb^{-1}";
      txtSize[1] = 0.04;
      txtX[1] = 0.25;
      txtY[1] = 0.85;
      txtAlign[1] = 11;
      txtFont[1] = 42;
    }
  
  txt[2] = "#gamma*/Z #rightarrow";
  txtSize[2] = 0.06;
  txtX[2] = 0.65;
  txtY[2] = 0.78;
  txtAlign[2] = 21;
  txtFont[2] = 42;
  
  // histogram limits, in linear and logarithmic
  int nbin_(0);
  float xmin_(0.), xmax_(0.); 
  //  float ymin_(0.), ymax_(0.); 
  float yminl_(0.), ymaxl_(0.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(510);
  int ndivy(510);
  float markerSize(1.2);
  float titleOffsetY(1.40);
  if( dataMCRatio )
    titleOffsetY=1.00;
  //  float titleOffsetX(1.00);

  // canvas name
  TString ctitle;

  // legend position and scale;
  float xl_(0);
  float yl_(0);
  float dxl_(0);
  float dyl_(0);
  float scalel_ = 0.070;
  int legendTextFont=42;
  float legendTextSize=0.040;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString fname("../Inputs/rawYield/");
  TString cname;
  TString dataHistName("hdata");

  bool dataHist = true;
  bool stackHists = true;

  //  enum { k_DY=0, k_DYtt, k_QCD, k_W, k_VV, k_tt, k_Y }; 
  enum { k_DY=0, k_DYtt, k_W, k_VV, k_tt, k_QCD}; 

  size_t nChan=6;
  if( mode == Fig_1_b ) nChan = 6;
  channels.resize( nChan );
  hnames.resize( nChan );
  type.resize( nChan );
  order.resize( nChan );

  type[k_DY]  ="Signal"; 
  type[k_DYtt]="Ztautau"; 
  type[k_W]   ="EWK"; 
  type[k_VV]  ="VV"; 
  type[k_tt]  ="ttbar"; 
  type[k_QCD] ="QCD"; 

  size_t jj = 0;
  order[jj++] = k_DY;
  order[jj++] = k_QCD;
  order[jj++] = k_DYtt;
  order[jj++] = k_VV;
  order[jj++] = k_W;
  order[jj++] = k_tt;
  if( mode == Fig_1_a )
    {

      channels[k_DY]  ="hsig_fewz"; 
      channels[k_W]   ="hWlepton"; 
      //channels[k_Y]   ="hWlepton";

      hnames[k_DY]    ="  DY #rightarrow #mu#mu"; 
      hnames[k_W]     ="  W #rightarrow #mu#nu"; 
      //hnames[k_Y]     ="  #Upsilon"; 

      //type[k_Y]       ="Y"; 
      
      //order[jj++] = k_Y;
    }
  else
    {
      channels[k_DY]  ="zee"; 
      channels[k_W]   ="wjets"; 

      hnames[k_DY]    ="  DY #rightarrow #mu#mu"; 
      hnames[k_W]     ="  W+jets"; 

    }

  channels[k_DYtt]="hDYtautau"; 
  channels[k_VV]  ="hdiboson"; 
  //FIXME this is if we want MC based non-QCD! 
  channels[k_tt]  ="httbar_mc"; 
  channels[k_QCD] ="hqcd"; 

  hnames[k_DYtt]="  DY #rightarrow #tau#tau"; 
  hnames[k_VV]  ="  dibosons"; 
  hnames[k_tt]  ="  t#bar{t}+tW+#bar{t}W"; 
  hnames[k_QCD] ="  QCD"; 

  if( GHMStyle )
    {
      hnames[k_DYtt]= "  #gamma*/Z #rightarrow #tau#tau"; 
      hnames[k_W]   = "  EWK";
      hnames[k_VV]  = "NOT_IN_LEGEND";

      jj = 0;
      order[jj++] = k_DY;
      order[jj++] = k_DYtt;
      order[jj++] = k_W;
      order[jj++] = k_VV;
      order[jj++] = k_tt;
      order[jj++] = k_QCD;
      if( mode == Fig_1_a )
	{
	  hnames[k_DY]    ="  #gamma*/Z #rightarrow #mu#mu"; 
	  //hnames[k_Y] = "NOT_IN_LEGEND";
	  //order[jj++] = k_Y;
	}
      else
	{
	  hnames[k_DY]    ="  #gamma*/Z #rightarrow #mu#mu"; 
	}
    }

  cname += "Fig_1";
  if( mode == Fig_1_a )
    {    
      cname += "_a";
      fname += "DYspectrum_mass_41.root"; 
      //fname += "DYspectrum_mass_41_EE.root";
      ctitle = "DY to mu-mu analysis";
      txt[2] += "#mu#mu";
      xtitle = "M(#mu#mu)  [";
	        
    }
  else if( mode == Fig_1_b )
    {      
      cname += "_b";
      fname += "massHists_ee.root";
      ctitle = "DY to e-e analysis";
      txt[2] += "#mu#mu";
      xtitle = "M(#mu#mu)  [";
    }
  else 
    return 0;

  xtitle += massUnit;       
  xtitle += "]";
  ytitle = "entries per bin"; 

  nbin_ = 100;
  xmin_ = 15.;
  xmax_ = 1500.;
  //  ndivx = 506;
  ndivx = 0;
  ndivy = 506;
	  
  markerSize = 1.1;
    
  //  yminl_ = 0.5;
  //  ymaxl_ = 500000.;

  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=400;

  if( GHMStyle )
    {

      yminl_ = 0.5;
      ymaxl_ = 10E7;

      xl_  = 0.65;
      yl_  = 0.42;
      dxl_ = 3.;
      dyl_ = 5.0;

      legendTextSize *= 1.1;
    }
  else
    {
      yminl_ = 0.15;
      ymaxl_ = 15000.;

      xl_  = 0.68;
      yl_  = 0.55;
      dxl_ = 3.5;
      dyl_ = 5.0;
    }

  //  size_t nChan=channels.size();
   
  // open the root file containing histograms and graphs
  TFile* f_ = TFile::Open(fname,"READ");

  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin( 110./600 );
  c_->SetRightMargin( 42./600 );
  c_->SetTopMargin(  40./600 );
  c_->SetBottomMargin( 80./600 ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);

  Double_t scale = 4;
  Double_t wbin = 42*scale;
  Double_t left  = 8*scale;
  Double_t right = 5*scale;
  Double_t h1 = 135*scale;
  Double_t h2 = 45*scale;
  Double_t top1 = 15*scale;
  Double_t bot1 = 3*scale;
  Double_t top2 = 3*scale;
  //  Double_t bot1 = 0*scale;
  //  Double_t top2 = 0*scale;
  Double_t bot2 = 80*scale;
  Double_t W = left + wbin + right;
  Double_t H = h1 + h2;
  Double_t s[2] = {1, h1/h2 };

  TPad* pad[2];
  pad[0] = new TPad( "top", "top", 
		     0, h2/H, 1, 1,
		     kWhite,0,0);
  pad[0]->SetLeftMargin(  left/W );
  pad[0]->SetRightMargin( right/W );
  pad[0]->SetTopMargin(  top1/H );
  pad[0]->SetBottomMargin( bot1/H );

  pad[1] = new TPad( "bottom", "bottom", 
		     0, 0, 1, h2/H,
		     kWhite,0,0);
  pad[1]->SetLeftMargin(  left/W );
  pad[1]->SetRightMargin( right/W );
  pad[1]->SetTopMargin(  top2/H );
  pad[1]->SetBottomMargin( bot2/H );
  //  pad[1]->SetGridy();

  for( int ii=0; ii<2; ii++ )
    {
      pad[ii]->SetFillColor(0);
      pad[ii]->SetTickx(1);
      pad[ii]->SetTicky(1);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
    }


  //  return c_;
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon1", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  //  ax_->SetTitleOffset(titleOffsetX);

  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(99);
  

  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);


  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetLabelOffset(0.008);


  // fetch histograms and dress them
  vector<TH1F*> histos(nChan);
  for( size_t iis=0; iis<nChan; iis++ )
    {

      //cout << "Channel[" << iis << "]=" << channels[iis] << endl;
      TH1F* tmp = (TH1F*)f_->Get(channels[iis]);

            //if( tmp ) cout << "OK" << endl;
      tmp->SetStats(kFALSE);
      tmp->SetName(channels[iis]);
      tmp->UseCurrentStyle();
      tmp->SetFillStyle( 1001 );
      tmp->SetFillColor( fillColor_[type[iis]] );
      tmp->SetLineColor( lineColor_[type[iis]] );
      tmp->SetLineWidth( lineWidth2 );
      histos[iis]=tmp;
      //cout << "Integral " << tmp->Integral() << endl;

    }

  TH1* h_tot = (TH1*) histos[order[0]]->Clone();
  h_tot->SetFillStyle(0);

  //
  // stack histogram
  //



  TH1* totalHisto(0);
  THStack* stackedHisto(0);
  vector<TH1*> listOfStackedHists(nChan);

  if( stackHists )
    {      

      size_t ii = order[nChan-1];
      
      TH1* h_stack = (TH1*) histos[ii]->Clone();
      h_stack -> Reset();

      TString stackName_ = TString("Mll");
      for( size_t jj=0; jj<nChan; jj++ )
	{
	  // ii = order[jj];
	  ii = order[nChan-jj-1];
	  TH1* hh_ = (TH1*) histos[ii]->Clone();

	  stackName_ += "_";
	  stackName_ += hh_->GetName();	  	  

	  TAxis* xaxis = h_stack->GetXaxis();
	  for( int iBin=1; iBin<=xaxis->GetNbins(); iBin++ )
	    {
	      hh_ -> AddBinContent( iBin, h_stack->GetBinContent( iBin ) );
	    }        

	  //	  cout << jj << " stack name=" << stackName_ << endl;
	  hh_->SetName( stackName_ );
	  delete h_stack;
	  h_stack = hh_;
	  listOfStackedHists[jj] =  (TH1*)hh_->Clone();
	  //      cout << endl;
	}
      delete h_stack;

      stackedHisto=new THStack("stackedHisto","XXX");
      for( size_t jj=0; jj<nChan; jj++ ) 
	{
	  size_t ii=order[nChan-jj-1];
          cout << "Stacked histo add " << histos[ii]->Integral() << endl;
	  stackedHisto->Add(histos[ii],"ah");
	  
	  if(jj==0)
	    {
	      totalHisto = (TH1*)histos[ii]->Clone();
	    }
	  else
	    {
	      totalHisto->Add(histos[ii]);
	    }
	}
    }
  else
    {
      totalHisto = h_tot;
    }
  
  // colors the stacked histogram
  totalHisto->SetLineColor( lineColor_["Signal"] );
  totalHisto->SetFillColor( 0 );
  totalHisto->SetLineWidth( lineWidth1 );
 
  // The data points are presented as a TGraph 
  // possibly a TGraph with asymmetric errors where
  // - error bars indicate the Poisson confidence interval at 68%
  // - bins with zero entry are removed
  
  TH1* hdata;
  TGraphAsymmErrors* dataGraph(0);
  if( dataHist )
    {      
      hdata = (TH1*) f_->Get( dataHistName );  
      assert( hdata );

      hdata->SetLineColor(kBlack);
      hdata->SetLineWidth(1);

      //      RooHist* roohist;      
      //      roohist = new RooHist((*hdata));
      int Nn0=0;
      vector<double> vY;
      vector<double> vX;
      vector<double > veY;
      vector<double > veX;
      vector<double> tmp(0,2);

      for(int ip=1;ip<=hdata->GetNbinsX();ip++) 
	{

	  double Y,X;
	  X = hdata->GetBinCenter(ip);
	  Y = hdata->GetBinContent(ip);
	  //    double eY[2],eX[2];
	  //	  roohist->GetPoint(ip,X,Y);
	
	  if(Y!=0) 
	    {
	      Nn0++;
	      
	      vY.push_back(Y);
	      vX.push_back(X);
	      veX.push_back( 0 );
	      veX.push_back( 0 );
	      veY.push_back( hdata->GetBinError(ip) );
	      veY.push_back( hdata->GetBinError(ip) );
	    }
	}

      dataGraph =new TGraphAsymmErrors(Nn0);
      for(int ip=0;ip<Nn0;ip++) 
	{
	  dataGraph->SetPoint(ip,vX[ip],vY[ip]);
	  dataGraph->SetPointError(ip,veX[ip*2],veX[ip*2+1],veY[ip*2],veY[ip*2+1]);
	}

      for( int ii=0; ii<dataGraph->GetN(); ii++ )
	{
	  dataGraph->SetPointEXlow(ii,0);
	  dataGraph->SetPointEXhigh(ii,0);
	}

    }
  else
    {
      TGraph* g_ = (TGraph*) f_->Get( dataHistName );  

      int jj=0;

      double x_(0), y_(0);

      dataGraph=new TGraphAsymmErrors();
      for( int ii=0; ii<g_->GetN(); ii++ )
	{
	  g_->GetPoint(ii,x_,y_ );
	  //	  if( y_<=0 ) continue;
	  dataGraph->SetPoint(jj,x_,y_);
	  dataGraph->SetPointEXlow(jj,0);
	  dataGraph->SetPointEXhigh(jj,0);
	  dataGraph->SetPointEYlow(jj,g_->GetErrorYlow(ii));
	  dataGraph->SetPointEYhigh(jj,g_->GetErrorYhigh(ii));
	  jj++;
	}
    }
  assert( dataGraph );

  // Remove the null bins
  double x_(0), y_(0);
  for( int ii=0; ii<dataGraph->GetN(); ii++ )
    {
      dataGraph->GetPoint(ii,x_,y_ );
      if( y_==0 )
	{
	  dataGraph->RemovePoint( ii );
	  ii--;
	}	  
    }
    
  dataGraph->SetName("dataGraph");
  dataGraph->SetMarkerStyle(kFullCircle);
  dataGraph->SetMarkerColor(kBlack);
  dataGraph->SetMarkerSize(markerSize);
  
  TGraph* dummyGraph = (TGraph*) dataGraph->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);

  // get the ratio data/fit
  TGraphAsymmErrors* ratioGraph = (TGraphAsymmErrors*) dataGraph->Clone("ratio");
  TH1* hfit = totalHisto;
  for( int ii=0; ii<dataGraph->GetN(); ii++ )
    {
      dataGraph->GetPoint(ii,x_,y_ );
      ratioGraph->SetPointEYlow(ii,0);
      ratioGraph->SetPointEYhigh(ii,0);
      ratioGraph->SetPoint(ii,x_,0 );
      double eyl_ = dataGraph->GetErrorYlow(ii);
      double eyh_ = dataGraph->GetErrorYhigh(ii);
      int jj = hfit->FindBin(x_);
      float fit_ = hfit->GetBinContent( jj );
      float binWidth_ = hfit->GetBinWidth(jj);
      ratioGraph->SetPointEXlow(ii,binWidth_/2.);
      ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
      if( fit_>0 )
	{
	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
	}
      //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
      //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
      //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
    }
  
  TH1* hratio_ = (TH1*) h_->Clone("hratio");
  ax_->SetLabelOffset(99);
  ax_->SetTitleOffset(99);

  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

 
  TPad* p_ = pad[0];
  p_->Draw();
  p_->cd();

  p_->SetLogy(true);
  p_->SetLogx(true);

  h_->GetYaxis()->SetRangeUser(yminl_,ymaxl_);
  h_->Draw("hist");
  //  draw_axis_labels(0.9*yminl_,0.050);

  dxl_ *= scalel_;
  dyl_ *= scalel_;
  TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
  legend->SetTextFont(legendTextFont);
  legend->SetTextSize(legendTextSize);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  
  legend->AddEntry(dummyGraph,"  data","PL");
  legend->AddEntry(dummyGraph,"      ","0"); // skip a line
  for( size_t jj=0; jj<nChan; jj++ ) 
    {
      size_t ii = order[jj];
      if( !hnames[jj].Contains("NOT_IN_LEGEND") )
	legend->AddEntry(histos[ii],hnames[ii],"F");
    }
  legend->Draw("same");

  if( stackHists )
    {
      //      stackedHisto->Draw("samehist");
      //      for( size_t jj=0; jj<nChan; jj++ )
      for( size_t jj=0; jj<nChan; jj++ )
	{
	  size_t kk = nChan-jj-1;
	  size_t ii = order[jj];
	  if( hnames[ii].Contains("NOT_IN_LEGEND") ) continue;
	  TH1* hh_= listOfStackedHists[kk];
	  //	  cout << jj << " " << hh_->GetName() << endl;
	  hh_->SetLineWidth(1);
	  hh_->Draw("Samehist");
	  if( GHMStyle ) draw_mass_bins( hh_, yminl_ );
	}
    }
  else
    {      
      //       for( size_t jj=0; jj<nChan; jj++ ) 
      // 	{
      // 	  size_t ii = order[jj];
      // 	  if( !hnames[ii].Contains("NOT_IN_LEGEND") )
      // 	    histos[ii]->Draw("samehist");
      // 	}
    }

  if( GHMStyle )
    totalHisto->Draw("samehist");

  //draw the data points
  dataGraph->Draw("PE");

  //  h_->Draw("Same");

  if( !GHMStyle ) draw_mass_bins( hdata, yminl_ );


  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  p_->RedrawAxis();
  p_->GetFrame()->Draw();
  c_->cd();
  
  p_ = pad[1];
  p_->SetLogx();
  p_->Draw();
  p_->cd();

  TAxis* xratio_ = hratio_->GetXaxis();
  TAxis* yratio_ = hratio_->GetYaxis();

  //XX r0_ 1 dr_ 0.75 0.250075 1.74992
  r0_ = 1;
  dr_ = 0.35;
  yratio_->SetRangeUser(r0_-0.9999*dr_,r0_+0.9999*dr_);
  yratio_->SetLabelSize( s[1]*yratio_->GetLabelSize() );
  yratio_->SetTitleSize( s[1]*yratio_->GetTitleSize() );
  yratio_->SetLabelOffset( yratio_->GetLabelOffset() );
  yratio_->SetTitleOffset( yratio_->GetTitleOffset()/s[1] );
  yratio_->SetTitle("Data/MC");
  yratio_->SetNdivisions(3);

  xratio_->SetLabelSize( s[1]*xratio_->GetLabelSize() );
  xratio_->SetTitleSize( s[1]*xratio_->GetTitleSize() );
  xratio_->SetTitleOffset( 1.0 );
  //  xratio_->CenterTitle();
  xratio_->SetLabelOffset(99);
  xratio_->SetTickLength( xratio_->GetTickLength()*s[1] );

  hratio_->Draw();
  draw_axis_labels(r0_-1.15*dr_,0.15);

  draw_bin_grid( r0_-dr_, r0_+dr_ );
  TLine grid_;
  grid_.SetLineColor(kBlue);
  grid_.SetLineStyle(kDashed);
  grid_.SetLineWidth(1);
  grid_.DrawLine(15,1,1500,1);

  ratioGraph->SetMarkerSize( ratioGraph->GetMarkerSize()*1. );
  ratioGraph->SetLineColor( kBlack );
  ratioGraph->SetLineWidth( 1 );

  ratioGraph->SetMarkerColor( kOrange+7 );
  ratioGraph->SetMarkerStyle( kFullCircle );
  ratioGraph->DrawClone("PE");
  ratioGraph->SetMarkerColor( kBlack );
  ratioGraph->SetMarkerStyle( kOpenCircle );
  ratioGraph->DrawClone("PE");


  p_->RedrawAxis();
  p_->GetFrame()->Draw();

  c_->cd();
  return c_;
}

// mass after acceptance correction
TCanvas* MassCorr( int mode )
{
  TGaxis::SetMaxDigits(3);

  float r0_ = 1.;
  float dr_ = 0.75;

  // channels, ordered as in the legend
  vector<TString> channels;  
  vector<TString> hnames;
  vector<TString> type;
  vector<size_t>  order;  

  map<TString,int> fillColor_;
  map<TString,int> lineColor_;
  int lineWidth1(1);
  int lineWidth2(1);

  if( salamanderColors )
    {
      lineWidth1 = 1;
      lineWidth2 = 1;

      fillColor_["Signal"] = kOrange-2;
      lineColor_["Signal"] = kOrange+3;
      
      fillColor_["Ztautau"] = kOrange+7;
      lineColor_["Ztautau"] = kOrange+3;

      fillColor_["VV"]     = kOrange+7;
      lineColor_["VV"]     = kOrange+3;

      fillColor_["EWK"] = kOrange+10;
      lineColor_["EWK"] = kOrange+3;

      fillColor_["QCD"] = kViolet-5;
      lineColor_["QCD"] = kViolet+3;
      
      fillColor_["ttbar"] = kRed+2;
      lineColor_["ttbar"] = kRed+4;      

      fillColor_["Y"] = kViolet-4;
      lineColor_["Y"] = kViolet+3;
    }
  else
    {
      lineWidth1 = 1;
      lineWidth2 = 1;

      fillColor_["Signal"] = kSpring-3;
      lineColor_["Signal"] = kSpring-3;
      
      fillColor_["Ztautau"] = kOrange;
      lineColor_["Ztautau"] = kOrange;
      
      fillColor_["EWK"] = kCyan-4;
      lineColor_["EWK"] = kCyan-4;
      
      fillColor_["VV"] = kPink-1;
      lineColor_["VV"] = kPink-1;
      
      fillColor_["QCD"] =  kMagenta-3;
      lineColor_["QCD"] =  kMagenta-3;
      
      fillColor_["Y"]   =  kBlue+2;
      lineColor_["Y"]   =  kBlue+2;
      
      fillColor_["ttbar"] = kAzure;
      lineColor_["ttbar"] = kAzure;
    }

  // text2
  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS";
  txtSize[0] = 0.055;
  txtX[0] = 0.915;
  txtY[0] = 0.935;
  txtAlign[0] = 31;
  txtFont[0] = 42;

  if( GHMStyle )
    {
      //txt[1] = "36 pb^{-1}  at  #sqrt{s} = 8 TeV";
      txt[1] = "36 pb^{-1} at #sqrt{s} = 8 TeV";
      //txt[1] = "#int#font[12]{L}dt = 36 pb^{-1} at #sqrt{s} = 8 TeV";
      txtSize[1] = 0.048;
      txtX[1] = 0.70;
      txtY[1] = 0.85;
      txtAlign[1] = 21;
      txtFont[1] = 42;
    }
  else
    {
      txt[1] = "36 pb^{-1}";
      //txt[1] = "#int#font[12]{L}dt = 36 pb^{-1}";
      txtSize[1] = 0.04;
      txtX[1] = 0.25;
      txtY[1] = 0.85;
      txtAlign[1] = 11;
      txtFont[1] = 42;
    }
  
  txt[2] = "#gamma*/Z #rightarrow";
  txtSize[2] = 0.06;
  txtX[2] = 0.65;
  txtY[2] = 0.78;
  txtAlign[2] = 21;
  txtFont[2] = 42;
  
  // histogram limits, in linear and logarithmic
  int nbin_(0);
  float xmin_(0.), xmax_(0.); 
  //  float ymin_(0.), ymax_(0.); 
  float yminl_(0.), ymaxl_(0.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(510);
  int ndivy(510);
  float markerSize(1.2);
  float titleOffsetY(1.40);
  if( dataMCRatio )
    titleOffsetY=1.00;
  //  float titleOffsetX(1.00);

  // canvas name
  TString ctitle;

  // legend position and scale;
  float xl_(0);
  float yl_(0);
  float dxl_(0);
  float dyl_(0);
  float scalel_ = 0.070;
  int legendTextFont=42;
  float legendTextSize=0.040;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString fname("root/");
  TString cname;
  TString dataHistName("data");

  bool dataHist = true;
  bool stackHists = true;

  //  enum { k_DY=0, k_DYtt, k_QCD, k_W, k_VV, k_tt, k_Y }; 
  enum { k_DY=0, k_DYtt, k_W, k_VV, k_tt, k_QCD, k_Y }; 

  size_t nChan=7;
  if( mode == Fig_9_b ) nChan = 6;
  channels.resize( nChan );
  hnames.resize( nChan );
  type.resize( nChan );
  order.resize( nChan );

  type[k_DY]  ="Signal"; 
  type[k_DYtt]="Ztautau"; 
  type[k_W]   ="EWK"; 
  type[k_VV]  ="VV"; 
  type[k_tt]  ="ttbar"; 
  type[k_QCD] ="QCD"; 

  size_t jj = 0;
  order[jj++] = k_DY;
  order[jj++] = k_QCD;
  order[jj++] = k_DYtt;
  order[jj++] = k_VV;
  order[jj++] = k_W;
  order[jj++] = k_tt;
  if( mode == Fig_9_a )
    {
      channels[k_DY]  ="zll"; 
      channels[k_W]   ="wmunu"; 
      channels[k_Y]   ="hupsilon";

      hnames[k_DY]    ="  DY #rightarrow #mu#mu"; 
      hnames[k_W]     ="  W #rightarrow #mu#nu"; 
      hnames[k_Y]     ="  #Upsilon"; 

      type[k_Y]       ="Y"; 
      
      order[jj++] = k_Y;
    }
  else
    {
      channels[k_DY]  ="zee"; 
      channels[k_W]   ="wjets"; 

      hnames[k_DY]    ="  DY #rightarrow #mu#mu"; 
      hnames[k_W]     ="  W+jets"; 

    }
  channels[k_DYtt]="ztt"; 
  channels[k_VV]  ="diboson"; 
  channels[k_tt]  ="ttbar"; 
  channels[k_QCD] ="qcd"; 

  hnames[k_DYtt]="  DY #rightarrow #tau#tau"; 
  hnames[k_VV]  ="  dibosons"; 
  hnames[k_tt]  ="  t#bar{t}+tW+#bar{t}W";
  hnames[k_QCD] ="  QCD"; 

  if( GHMStyle )
    {
      hnames[k_DYtt]= "  #gamma*/Z #rightarrow #tau#tau"; 
      hnames[k_W]   = "  EWK";
      hnames[k_VV]  = "NOT_IN_LEGEND";

      jj = 0;
      order[jj++] = k_DY;
      order[jj++] = k_DYtt;
      order[jj++] = k_W;
      order[jj++] = k_VV;
      order[jj++] = k_tt;
      order[jj++] = k_QCD;
      if( mode == Fig_9_a )
	{
	  hnames[k_DY]    ="  #gamma*/Z #rightarrow ee"; 
	  hnames[k_Y] = "NOT_IN_LEGEND";
	  order[jj++] = k_Y;
	}
      else
	{
	  hnames[k_DY]    ="  #gamma*/Z #rightarrow ee"; 
	}
    }

  cname += "Fig_9";
  if( mode == Fig_9_a )
    {    
      cname += "_a";
      fname += "massHists_mumu_reweighted.root";
      ctitle = "DY to mu-mu analysis";
      txt[2] += "ee";
      xtitle = "M(ee)  [";
	        
    }
  else if( mode == Fig_9_b )
    {      
      cname += "_b";
      fname += "massHist_DY_n04_v2_fewz_weighted.root";
      ctitle = "DY to e-e analysis";
      txt[2] += "ee";
      xtitle = "M(ee)  [";
    }
  else 
    return 0;

  xtitle += massUnit;       
  xtitle += "]";
  ytitle = "entries per bin"; 

  nbin_ = 100;
  xmin_ = 15.;
  xmax_ = 600.;
  //  ndivx = 506;
  ndivx = 0;
  ndivy = 506;
	  
  markerSize = 1.1;
    
  //  yminl_ = 0.5;
  //  ymaxl_ = 500000.;

  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=400;

  if( GHMStyle )
    {

      yminl_ = 0.5;
      ymaxl_ = 100000.;

      xl_  = 0.65;
      yl_  = 0.45;
      dxl_ = 3.;
      dyl_ = 5.0;

      legendTextSize *= 1.1;
    }
  else
    {
      yminl_ = 0.15;
      ymaxl_ = 15000.;

      xl_  = 0.68;
      yl_  = 0.55;
      dxl_ = 3.5;
      dyl_ = 5.0;
    }

  //  size_t nChan=channels.size();
   
  // open the root file containing histograms and graphs
  TFile* f_ = TFile::Open(fname,"READ");
  
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin( 110./600 );
  c_->SetRightMargin( 42./600 );
  c_->SetTopMargin(  40./600 );
  c_->SetBottomMargin( 80./600 ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);

  Double_t scale = 4;
  Double_t wbin = 42*scale;
  Double_t left  = 8*scale;
  Double_t right = 5*scale;
  Double_t h1 = 135*scale;
  Double_t h2 = 45*scale;
  Double_t top1 = 15*scale;
  Double_t bot1 = 3*scale;
  Double_t top2 = 3*scale;
  //  Double_t bot1 = 0*scale;
  //  Double_t top2 = 0*scale;
  Double_t bot2 = 80*scale;
  Double_t W = left + wbin + right;
  Double_t H = h1 + h2;
  Double_t s[2] = {1, h1/h2 };

  TPad* pad[2];
  pad[0] = new TPad( "top", "top", 
		     0, h2/H, 1, 1,
		     kWhite,0,0);
  pad[0]->SetLeftMargin(  left/W );
  pad[0]->SetRightMargin( right/W );
  pad[0]->SetTopMargin(  top1/H );
  pad[0]->SetBottomMargin( bot1/H );

  pad[1] = new TPad( "bottom", "bottom", 
		     0, 0, 1, h2/H,
		     kWhite,0,0);
  pad[1]->SetLeftMargin(  left/W );
  pad[1]->SetRightMargin( right/W );
  pad[1]->SetTopMargin(  top2/H );
  pad[1]->SetBottomMargin( bot2/H );
  //  pad[1]->SetGridy();

  for( int ii=0; ii<2; ii++ )
    {
      pad[ii]->SetFillColor(0);
      pad[ii]->SetTickx(1);
      pad[ii]->SetTicky(1);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
    }


  //  return c_;
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon1", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  //  ax_->SetTitleOffset(titleOffsetX);

  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(99);
  

  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);


  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetLabelOffset(0.008);


  // fetch histograms and dress them
  vector<TH1F*> histos(nChan);
  for( size_t ii=0; ii<nChan; ii++ )
    {
      //      cout << "Channel[" << ii << "]=" << channels[ii] << endl;
      TH1F* tmp = (TH1F*)f_->Get(channels[ii]);
      //      if( tmp ) cout << "OK" << endl;
      tmp->SetStats(kFALSE);
      tmp->SetName(channels[ii]);
      tmp->UseCurrentStyle();
      tmp->SetFillStyle( 1001 );
      tmp->SetFillColor( fillColor_[type[ii]] );
      tmp->SetLineColor( lineColor_[type[ii]] );
      tmp->SetLineWidth( lineWidth2 );
      histos[ii]=tmp;
    }

  TH1* h_tot = (TH1*) histos[order[0]]->Clone();
  h_tot->SetFillStyle(0);

  //
  // stack histogram
  //



  TH1* totalHisto(0);
  THStack* stackedHisto(0);
  vector<TH1*> listOfStackedHists(nChan);

  if( stackHists )
    {      
      size_t ii = order[nChan-1];
      
      TH1* h_stack = (TH1*) histos[ii]->Clone();
      h_stack -> Reset();

      TString stackName_ = TString("Mll");
      for( size_t jj=0; jj<nChan; jj++ )
	{
	  // ii = order[jj];
	  ii = order[nChan-jj-1];
	  TH1* hh_ = (TH1*) histos[ii]->Clone();

	  stackName_ += "_";
	  stackName_ += hh_->GetName();	  	  

	  TAxis* xaxis = h_stack->GetXaxis();
	  for( int iBin=1; iBin<=xaxis->GetNbins(); iBin++ )
	    {
	      hh_ -> AddBinContent( iBin, h_stack->GetBinContent( iBin ) );
	    }        

	  //	  cout << jj << " stack name=" << stackName_ << endl;
	  hh_->SetName( stackName_ );
	  delete h_stack;
	  h_stack = hh_;
	  listOfStackedHists[jj] =  (TH1*)hh_->Clone();
	  //      cout << endl;
	}
      delete h_stack;

      stackedHisto=new THStack("stackedHisto","XXX");
      for( size_t jj=0; jj<nChan; jj++ ) 
	{
	  size_t ii=order[nChan-jj-1];
	  stackedHisto->Add(histos[ii],"ah");
	  
	  if(jj==0)
	    {
	      totalHisto = (TH1*)histos[ii]->Clone();
	    }
	  else
	    {
	      totalHisto->Add(histos[ii]);
	    }
	}
    }
  else
    {
      totalHisto = h_tot;
    }
  
  // colors the stacked histogram
  totalHisto->SetLineColor( lineColor_["Signal"] );
  totalHisto->SetFillColor( 0 );
  totalHisto->SetLineWidth( lineWidth1 );
  
  // The data points are presented as a TGraph 
  // possibly a TGraph with asymmetric errors where
  // - error bars indicate the Poisson confidence interval at 68%
  // - bins with zero entry are removed
  
  TH1* hdata;
  TGraphAsymmErrors* dataGraph(0);
  if( dataHist )
    {      
      hdata = (TH1*) f_->Get( dataHistName );  
      assert( hdata );

      hdata->SetLineColor(kBlack);
      hdata->SetLineWidth(1);

      //      RooHist* roohist;      
      //      roohist = new RooHist((*hdata));
      int Nn0=0;
      vector<double> vY;
      vector<double> vX;
      vector<double > veY;
      vector<double > veX;
      vector<double> tmp(0,2);

      for(int ip=1;ip<=hdata->GetNbinsX();ip++) 
	{
	  double Y,X;
	  X = hdata->GetBinCenter(ip);
	  Y = hdata->GetBinContent(ip);
	  //    double eY[2],eX[2];
	  //	  roohist->GetPoint(ip,X,Y);
	
	  if(Y!=0) 
	    {
	      Nn0++;
	      
	      vY.push_back(Y);
	      vX.push_back(X);
	      veX.push_back( 0 );
	      veX.push_back( 0 );
	      veY.push_back( hdata->GetBinError(ip) );
	      veY.push_back( hdata->GetBinError(ip) );
	    }
	}

      dataGraph =new TGraphAsymmErrors(Nn0);
      for(int ip=0;ip<Nn0;ip++) 
	{
	  dataGraph->SetPoint(ip,vX[ip],vY[ip]);
	  dataGraph->SetPointError(ip,veX[ip*2],veX[ip*2+1],veY[ip*2],veY[ip*2+1]);
	}

      for( int ii=0; ii<dataGraph->GetN(); ii++ )
	{
	  dataGraph->SetPointEXlow(ii,0);
	  dataGraph->SetPointEXhigh(ii,0);
	}

    }
  else
    {
      TGraph* g_ = (TGraph*) f_->Get( dataHistName );  

      int jj=0;

      double x_(0), y_(0);

      dataGraph=new TGraphAsymmErrors();
      for( int ii=0; ii<g_->GetN(); ii++ )
	{
	  g_->GetPoint(ii,x_,y_ );
	  //	  if( y_<=0 ) continue;
	  dataGraph->SetPoint(jj,x_,y_);
	  dataGraph->SetPointEXlow(jj,0);
	  dataGraph->SetPointEXhigh(jj,0);
	  dataGraph->SetPointEYlow(jj,g_->GetErrorYlow(ii));
	  dataGraph->SetPointEYhigh(jj,g_->GetErrorYhigh(ii));
	  jj++;
	}
    }
  assert( dataGraph );

  // Remove the null bins
  double x_(0), y_(0);
  for( int ii=0; ii<dataGraph->GetN(); ii++ )
    {
      dataGraph->GetPoint(ii,x_,y_ );
      if( y_==0 )
	{
	  dataGraph->RemovePoint( ii );
	  ii--;
	}	  
    }
    
  dataGraph->SetName("dataGraph");
  dataGraph->SetMarkerStyle(kFullCircle);
  dataGraph->SetMarkerColor(kBlack);
  dataGraph->SetMarkerSize(markerSize);
  
  TGraph* dummyGraph = (TGraph*) dataGraph->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);

  // get the ratio data/fit
  TGraphAsymmErrors* ratioGraph = (TGraphAsymmErrors*) dataGraph->Clone("ratio");
  TH1* hfit = totalHisto;
  for( int ii=0; ii<dataGraph->GetN(); ii++ )
    {
      dataGraph->GetPoint(ii,x_,y_ );
      ratioGraph->SetPointEYlow(ii,0);
      ratioGraph->SetPointEYhigh(ii,0);
      ratioGraph->SetPoint(ii,x_,0 );
      double eyl_ = dataGraph->GetErrorYlow(ii);
      double eyh_ = dataGraph->GetErrorYhigh(ii);
      int jj = hfit->FindBin(x_);
      float fit_ = hfit->GetBinContent( jj );
      float binWidth_ = hfit->GetBinWidth(jj);
      ratioGraph->SetPointEXlow(ii,binWidth_/2.);
      ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
      if( fit_>0 )
	{
	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
	}
      //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
      //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
      //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
    }
  
  TH1* hratio_ = (TH1*) h_->Clone("hratio");
  ax_->SetLabelOffset(99);
  ax_->SetTitleOffset(99);

  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

 
  TPad* p_ = pad[0];
  p_->Draw();
  p_->cd();

  p_->SetLogy(true);
  p_->SetLogx(true);

  h_->GetYaxis()->SetRangeUser(yminl_,ymaxl_);
  h_->Draw("hist");
  //  draw_axis_labels(0.9*yminl_,0.050);

  dxl_ *= scalel_;
  dyl_ *= scalel_;
  TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
  legend->SetTextFont(legendTextFont);
  legend->SetTextSize(legendTextSize);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  
  legend->AddEntry(dummyGraph,"  data","PL");
  legend->AddEntry(dummyGraph,"      ","0"); // skip a line
  for( size_t jj=0; jj<nChan; jj++ ) 
    {
      size_t ii = order[jj];
      if( !hnames[jj].Contains("NOT_IN_LEGEND") )
	legend->AddEntry(histos[ii],hnames[ii],"F");
    }
  legend->Draw("same");

  if( stackHists )
    {
      //      stackedHisto->Draw("samehist");
      //      for( size_t jj=0; jj<nChan; jj++ )
      for( size_t jj=0; jj<nChan; jj++ )
	{
	  size_t kk = nChan-jj-1;
	  size_t ii = order[jj];
	  if( hnames[ii].Contains("NOT_IN_LEGEND") ) continue;
	  TH1* hh_= listOfStackedHists[kk];
	  //	  cout << jj << " " << hh_->GetName() << endl;
	  hh_->SetLineWidth(1);
	  hh_->Draw("Samehist");
	  if( GHMStyle ) draw_mass_bins( hh_, yminl_ );
	}
    }
  else
    {      
      //       for( size_t jj=0; jj<nChan; jj++ ) 
      // 	{
      // 	  size_t ii = order[jj];
      // 	  if( !hnames[ii].Contains("NOT_IN_LEGEND") )
      // 	    histos[ii]->Draw("samehist");
      // 	}
    }

  if( GHMStyle )
    totalHisto->Draw("samehist");

  // draw the data points
  dataGraph->Draw("PE");

  //  h_->Draw("Same");

  if( !GHMStyle ) draw_mass_bins( hdata, yminl_ );


  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  p_->RedrawAxis();
  p_->GetFrame()->Draw();
  c_->cd();
  
  p_ = pad[1];
  p_->SetLogx();
  p_->Draw();
  p_->cd();

  TAxis* xratio_ = hratio_->GetXaxis();
  TAxis* yratio_ = hratio_->GetYaxis();

  yratio_->SetRangeUser(r0_-0.9999*dr_,r0_+0.9999*dr_);
  yratio_->SetLabelSize( s[1]*yratio_->GetLabelSize() );
  yratio_->SetTitleSize( s[1]*yratio_->GetTitleSize() );
  yratio_->SetLabelOffset( yratio_->GetLabelOffset() );
  yratio_->SetTitleOffset( yratio_->GetTitleOffset()/s[1] );
  yratio_->SetTitle("Data/MC");
  yratio_->SetNdivisions(3);

  xratio_->SetLabelSize( s[1]*xratio_->GetLabelSize() );
  xratio_->SetTitleSize( s[1]*xratio_->GetTitleSize() );
  xratio_->SetTitleOffset( 1.0 );
  //  xratio_->CenterTitle();
  xratio_->SetLabelOffset(99);
  xratio_->SetTickLength( xratio_->GetTickLength()*s[1] );

  hratio_->Draw();
  draw_axis_labels(r0_-1.15*dr_,0.15);

  draw_bin_grid( r0_-dr_, r0_+dr_ );
  TLine grid_;
  grid_.SetLineColor(kBlue);
  grid_.SetLineStyle(kDashed);
  grid_.SetLineWidth(1);
  grid_.DrawLine(15,1,600,1);

  ratioGraph->SetMarkerSize( ratioGraph->GetMarkerSize()*1. );
  ratioGraph->SetLineColor( kBlack );
  ratioGraph->SetLineWidth( 1 );

  ratioGraph->SetMarkerColor( kOrange+7 );
  ratioGraph->SetMarkerStyle( kFullCircle );
  ratioGraph->DrawClone("PE");
  ratioGraph->SetMarkerColor( kBlack );
  ratioGraph->SetMarkerStyle( kOpenCircle );
  ratioGraph->DrawClone("PE");


  p_->RedrawAxis();
  p_->GetFrame()->Draw();

  c_->cd();
  return c_;
}

//#include "Acceptance.C"
TCanvas* Acceptance( int mode )
{
  TGraph* gra;
  TGraph* gra1;
  TGraph* gra2;

  TGraphAsymmErrors* grae = new TGraphAsymmErrors(40);
  TGraphAsymmErrors* grae1 = new TGraphAsymmErrors(40);
  TGraphAsymmErrors* grae2 = new TGraphAsymmErrors(40);
  TGraphErrors* gre  = new TGraphErrors(40);
  TGraphErrors* gre1  = new TGraphErrors(40);
  TGraphErrors* gre2  = new TGraphErrors(40);
/*
  TGraphAsymmErrors* grae = new TGraphAsymmErrors(132);
  TGraphAsymmErrors* grae1 = new TGraphAsymmErrors(132);
  TGraphAsymmErrors* grae2 = new TGraphAsymmErrors(132);
  TGraphErrors* gre  = new TGraphErrors(132);
  TGraphErrors* gre1  = new TGraphErrors(132);
  TGraphErrors* gre2  = new TGraphErrors(132);
*/
  if( mode==Fig_2 )
  {

  cout << "In the right mode out1_1017_w1" << endl;
  //TFile* f = new TFile("../Inputs/acc/out1.root");
  TFile* f = new TFile("../Inputs/accEE/out1_EE.root");
  f->cd();
  TH1D* eff_postFSR = (TH1D*)gDirectory->Get("eff_postFSRcorr");
  TH1D* acc_postFSRcorr = (TH1D*)gDirectory->Get("acc_postFSRcorr"); //corr");
  TH1D* acceff_postFSRcorr = (TH1D*)gDirectory->Get("acceff_postFSRcorr"); //corr");
  for (int i = 0; i < eff_postFSR->GetNbinsX(); i++) {
    cout << i << " " << eff_postFSR->GetBinContent(i+1) << endl;
    grae1->SetPoint(i,acceff_postFSRcorr->GetBinCenter(i+1),acceff_postFSRcorr->GetBinContent(i+1));
    cout << i << endl;
    grae1->SetPointError(i,acceff_postFSRcorr->GetBinWidth(i+1)/2.,acceff_postFSRcorr->GetBinWidth(i+1)/2.,acceff_postFSRcorr->GetBinError(i+1),acceff_postFSRcorr->GetBinError(i+1));
    cout << i << endl;
    grae2->SetPoint(i,eff_postFSR->GetBinCenter(i+1),eff_postFSR->GetBinContent(i+1));
    cout << i << endl;
    grae2->SetPointError(i,eff_postFSR->GetBinWidth(i+1)/2.,eff_postFSR->GetBinWidth(i+1)/2.,eff_postFSR->GetBinError(i+1),eff_postFSR->GetBinError(i+1)
);
    cout << i << endl;
    grae->SetPoint(i,acc_postFSRcorr->GetBinCenter(i+1),acc_postFSRcorr->GetBinContent(i+1));
    cout << i << endl;
    grae->SetPointError(i,acc_postFSRcorr->GetBinWidth(i+1)/2.,acc_postFSRcorr->GetBinWidth(i+1)/2.,acc_postFSRcorr->GetBinError(i+1),acc_postFSRcorr->GetBinError(i+1));
    cout << i << endl;
  }
    gra = grae;
    gra1 = grae1;
    gra2 = grae2;

    }
  else if( mode==Fig_2_a )
    {
      //   grae->SetTitle("Drell-Yan acceptance;dimuon (post-FSR) mass, GeV");
      //      grae->SetFillStyle(3001);
      grae->SetPoint(0,7.5,0.002324331);
      grae->SetPointError(0,7.5,7.5,2.727149e-05,2.748576e-05);
      grae->SetPoint(1,17.5,0.0123);
      grae->SetPointError(1,2.5,2.5,0.0001, 0.0001);
      grae->SetPoint(2,25,0.0569);
      grae->SetPointError(2,5,5,0.0003,0.0003);
      grae->SetPoint(3,35,0.2353);
      grae->SetPointError(3,5,5,0.0010,0.0010);
      grae->SetPoint(4,45,0.3479);
      grae->SetPointError(4,5,5,0.0018,0.0018);
      grae->SetPoint(5,55,0.4119);
      grae->SetPointError(5,5,5,0.0024,0.0024);
      grae->SetPoint(6,68,0.4735);
      grae->SetPointError(6,8,8,0.0019,0.0019);
      grae->SetPoint(7,81,0.5062);
      grae->SetPointError(7,5,5,0.0014,0.0014);
      grae->SetPoint(8,91,0.5178);
      grae->SetPointError(8,5,5,0.0005,0.0005);
      grae->SetPoint(9,101,0.5314);
      grae->SetPointError(9,5,5,0.0021,0.0021);
      grae->SetPoint(10,113,0.5461);
      grae->SetPointError(10,7,7,0.0041,0.0041);
      grae->SetPoint(11,135,0.5660);
      grae->SetPointError(11,15,15,0.0055,0.0055);
      grae->SetPoint(12,175,0.6083);
      grae->SetPointError(12,25,25,0.0087,0.0087);
      grae->SetPoint(13,400,0.6769);
      grae->SetPointError(13,200,200,0.0121,0.0121);
      gra = grae;
    }
  else if( mode==Fig_2_b )
    {
      grae->SetPoint(0,7.5,0.001857528);
      grae->SetPointError(0,7.5,7.5,2.437393e-05,2.458857e-05);
      grae->SetPoint(1,17.5,0.0100);
      grae->SetPointError(1,2.5,2.5,0.0001, 0.0001);
      grae->SetPoint(2,25,0.0444);
      grae->SetPointError(2,5,5,0.0003,0.0003);
      grae->SetPoint(3,35,0.1956);
      grae->SetPointError(3,5,5,0.0010,0.0010);
      grae->SetPoint(4,45,0.3006);
      grae->SetPointError(4,5,5,0.0017,0.0017);
      grae->SetPoint(5,55,0.3620);
      grae->SetPointError(5,5,5,0.0023,0.0023);
      grae->SetPoint(6,68,0.4187);
      grae->SetPointError(6,8,8,0.0019,0.0019);
      grae->SetPoint(7,81,0.4554);
      grae->SetPointError(7,5,5,0.0014,0.0014);
      grae->SetPoint(8,91,0.4705);
      grae->SetPointError(8,5,5,0.0005,0.0005);
      grae->SetPoint(9,101,0.4850);
      grae->SetPointError(9,5,5,0.0021,0.0021);
      grae->SetPoint(10,113,0.4961);
      grae->SetPointError(10,7,7,0.0042,0.0042);
      grae->SetPoint(11,135,0.5177);
      grae->SetPointError(11,15,15,0.0056,0.0056);
      grae->SetPoint(12,175,0.5495);
      grae->SetPointError(12,25,25,0.0089,0.0089);
      grae->SetPoint(13,400,0.6091);
      grae->SetPointError(13,200,200,0.0127,0.0127);
      gra = grae;
    }
  else if( mode==Fig_3 )
    {
      gre->SetPoint(0,17.5,0.00457615);
      gre->SetPointError(0,2.5,7.52741e-05);
      gre->SetPoint(1,22.5,0.0085904);
      gre->SetPointError(1,2.5,5.97549e-05);
      gre->SetPoint(2,27.5,0.0188386);
      gre->SetPointError(2,2.5,0.000113234);
      gre->SetPoint(3,32.5,0.0594503);
      gre->SetPointError(3,2.5,0.000242111);
      gre->SetPoint(4,37.5,0.138917);
      gre->SetPointError(4,2.5,0.000442083);
      gre->SetPoint(5,42.5,0.233721);
      gre->SetPointError(5,2.5,0.000660271);
      gre->SetPoint(6,47.5,0.301732);
      gre->SetPointError(6,2.5,0.000839842);
      gre->SetPoint(7,52.5,0.353453);
      gre->SetPointError(7,2.5,0.000974256);
      gre->SetPoint(8,57.5,0.390809);
      gre->SetPointError(8,2.5,0.00104457);
      gre->SetPoint(9,62,0.417891);
      gre->SetPointError(9,2,0.00118365);
      gre->SetPoint(10,66,0.435302);
      gre->SetPointError(10,2,0.00114024);
      gre->SetPoint(11,70,0.455042);
      gre->SetPointError(11,2,0.00106067);
      gre->SetPoint(12,74,0.464006);
      gre->SetPointError(12,2,0.000951606);
      gre->SetPoint(13,78.5,0.474434);
      gre->SetPointError(13,2.5,0.000706111);
      gre->SetPoint(14,83.5,0.48174);
      gre->SetPointError(14,2.5,0.000513058);
      gre->SetPoint(15,88.5,0.488298);
      gre->SetPointError(15,2.5,0.000243999);
      gre->SetPoint(16,93.5,0.49389);
      gre->SetPointError(16,2.5,0.000261913);
      gre->SetPoint(17,98.5,0.503109);
      gre->SetPointError(17,2.5,0.000864549);
      gre->SetPoint(18,103.5,0.510052);
      gre->SetPointError(18,2.5,0.00146792);
      gre->SetPoint(19,108,0.518931);
      gre->SetPointError(19,2,0.00222736);
      gre->SetPoint(20,112.5,0.520451);
      gre->SetPointError(20,2.5,0.00250426);
      gre->SetPoint(21,117.5,0.526712);
      gre->SetPointError(21,2.5,0.00305745);
      gre->SetPoint(22,123,0.535808);
      gre->SetPointError(22,3,0.00334201);
      gre->SetPoint(23,129.5,0.544094);
      gre->SetPointError(23,3.5,0.00374214);
      gre->SetPoint(24,137,0.545375);
      gre->SetPointError(24,3.5,0.00419747);
      gre->SetPoint(25,145.5,0.56073);
      gre->SetPointError(25,4.5,0.0046915);
      gre->SetPoint(26,155,0.567352);
      gre->SetPointError(26,5,0.00528601);
      gre->SetPoint(27,165.5,0.572535);
      gre->SetPointError(27,5.5,0.00604782);
      gre->SetPoint(28,178,0.586097);
      gre->SetPointError(28,7,0.00643433);
      gre->SetPoint(29,192.5,0.610066);
      gre->SetPointError(29,7.5,0.00741897);
      gre->SetPoint(30,210,0.623904);
      gre->SetPointError(30,10,0.00785313);
      gre->SetPoint(31,231.5,0.626313);
      gre->SetPointError(31,11.5,0.0091484);
      gre->SetPoint(32,258,0.644784);
      gre->SetPointError(32,15,0.0102693);
      gre->SetPoint(33,296.5,0.670133);
      gre->SetPointError(33,23.5,0.0112656);
      gre->SetPoint(34,350,0.713734);
      gre->SetPointError(34,30,0.0145879);
      gre->SetPoint(35,410,0.756664);
      gre->SetPointError(35,30,0.0200062);
      gre->SetPoint(36,475,0.776723);
      gre->SetPointError(36,35,0.0238466);
      gre->SetPoint(37,555,0.775042);
      gre->SetPointError(37,45,0.0033067);
      gre->SetPoint(38,800,0.806968);
      gre->SetPointError(38,200,0.0030793);
      gre->SetPoint(39,1250,0.856437);
      gre->SetPointError(39,250,0.00274068);
      gra = gre;

      gre1->SetPoint(0,17.5,0.001133018);
      gre1->SetPointError(0,2.5,2.296913e-05);
      gre1->SetPoint(1,22.5,0.002115025);
      gre1->SetPointError(1,2.5,2.889121e-05);
      gre1->SetPoint(2,27.5,0.0040996);
      gre1->SetPointError(2,2.5,5.170503e-05);
      gre1->SetPoint(3,32.5,0.01114212);
      gre1->SetPointError(3,2.5,0.0001065153);
      gre1->SetPoint(4,37.5,0.03018903);
      gre1->SetPointError(4,2.5,0.000218068);
      gre1->SetPoint(5,42.5,0.06166168);
      gre1->SetPointError(5,2.5,0.0003737026);
      gre1->SetPoint(6,47.5,0.08958755);
      gre1->SetPointError(6,2.5,0.0005194895);
      gre1->SetPoint(7,52.5,0.1137748);
      gre1->SetPointError(7,2.5,0.0006406036);
      gre1->SetPoint(8,57.5,0.1327563);
      gre1->SetPointError(8,2.5,0.0007183607);
      gre1->SetPoint(9,62,0.1470972);
      gre1->SetPointError(9,2,0.0008385401);
      gre1->SetPoint(10,66,0.1585366);
      gre1->SetPointError(10,2,0.000826943);
      gre1->SetPoint(11,70,0.1698949);
      gre1->SetPointError(11,2,0.0007894006);
      gre1->SetPoint(12,74,0.1813442);
      gre1->SetPointError(12,2,0.0007231588);
      gre1->SetPoint(13,78.5,0.1970405);
      gre1->SetPointError(13,2.5,0.0005533904);
      gre1->SetPoint(14,83.5,0.2200001);
      gre1->SetPointError(14,2.5,0.0004185867);
      gre1->SetPoint(15,88.5,0.2512264);
      gre1->SetPointError(15,2.5,0.0002085323);
      gre1->SetPoint(16,93.5,0.2622665);
      gre1->SetPointError(16,2.5,0.0002269648);
      gre1->SetPoint(17,98.5,0.2727092);
      gre1->SetPointError(17,2.5,0.0007584292);
      gre1->SetPoint(18,103.5,0.2811146);
      gre1->SetPointError(18,2.5,0.001299516);
      gre1->SetPoint(19,108,0.2890093);
      gre1->SetPointError(19,2,0.001988427);
      gre1->SetPoint(20,112.5,0.2933434);
      gre1->SetPointError(20,2.5,0.002244253);
      gre1->SetPoint(21,117.5,0.3009016);
      gre1->SetPointError(21,2.5,0.00276478);
      gre1->SetPoint(22,123,0.309704);
      gre1->SetPointError(22,3,0.003044554);
      gre1->SetPoint(23,129.5,0.3193114);
      gre1->SetPointError(23,3.5,0.003446219);
      gre1->SetPoint(24,137,0.3230502);
      gre1->SetPointError(24,3.5,0.003879273);
      gre1->SetPoint(25,145.5,0.3351169);
      gre1->SetPointError(25,4.5,0.004389016);
      gre1->SetPoint(26,155,0.345177);
      gre1->SetPointError(26,5,0.004981428);
      gre1->SetPoint(27,165.5,0.3515834);
      gre1->SetPointError(27,5.5,0.005726223);
      gre1->SetPoint(28,178,0.3700312);
      gre1->SetPointError(28,7,0.006186131);
      gre1->SetPoint(29,192.5,0.3882545);
      gre1->SetPointError(29,7.5,0.007256355);
      gre1->SetPoint(30,210,0.4077643);
      gre1->SetPointError(30,10,0.007822008);
      gre1->SetPoint(31,231.5,0.4195145);
      gre1->SetPointError(31,11.5,0.009162355);
      gre1->SetPoint(32,258,0.4358559);
      gre1->SetPointError(32,15,0.01043092);
      gre1->SetPoint(33,296.5,0.4614134);
      gre1->SetPointError(33,23.5,0.01164807);
      gre1->SetPoint(34,350,0.4996502);
      gre1->SetPointError(34,30,0.01578117);
      gre1->SetPoint(35,410,0.5543858);
      gre1->SetPointError(35,30,0.02289531);
      gre1->SetPoint(36,475,0.5625113);
      gre1->SetPointError(36,35,0.02938374);
      gre1->SetPoint(37,555,0.5557113);
      gre1->SetPointError(37,45,0.03072507);
      gre1->SetPoint(38,800,0.5881385);
      gre1->SetPointError(38,200,0.03336735);
      gre1->SetPoint(39,1250,0.639409);
      gre1->SetPointError(39,250,0.122104);
      gra1 = gre1;

      gre2->SetPoint(0,17.5,0.247592);
      gre2->SetPointError(0,2.5,0.00293371);
      gre2->SetPoint(1,22.5,0.246208);
      gre2->SetPointError(1,2.5,0.00289448);
      gre2->SetPoint(2,27.5,0.217617);
      gre2->SetPointError(2,2.5,0.00241289);
      gre2->SetPoint(3,32.5,0.187419);
      gre2->SetPointError(3,2.5,0.00162096);
      gre2->SetPoint(4,37.5,0.217317);
      gre2->SetPointError(4,2.5,0.00140922);
      gre2->SetPoint(5,42.5,0.263826);
      gre2->SetPointError(5,2.5,0.00141459);
      gre2->SetPoint(6,47.5,0.296911);
      gre2->SetPointError(6,2.5,0.00151038);
      gre2->SetPoint(7,52.5,0.321895);
      gre2->SetPointError(7,2.5,0.00158038);
      gre2->SetPoint(8,57.5,0.339696);
      gre2->SetPointError(8,2.5,0.00159824);
      gre2->SetPoint(9,62,0.351999);
      gre2->SetPointError(9,2,0.00174138);
      gre2->SetPoint(10,66,0.364199);
      gre2->SetPointError(10,2,0.00164279);
      gre2->SetPoint(11,70,0.373361);
      gre2->SetPointError(11,2,0.0015007);
      gre2->SetPoint(12,74,0.390823);
      gre2->SetPointError(12,2,0.00133661);
      gre2->SetPoint(13,78.5,0.415317);
      gre2->SetPointError(13,2.5,0.000989172);
      gre2->SetPoint(14,83.5,0.456678);
      gre2->SetPointError(14,2.5,0.000720031);
      gre2->SetPoint(15,88.5,0.514494);
      gre2->SetPointError(15,2.5,0.000341006);
      gre2->SetPoint(16,93.5,0.531022);
      gre2->SetPointError(16,2.5,0.000363154);
      gre2->SetPoint(17,98.5,0.542048);
      gre2->SetPointError(17,2.5,0.00118528);
      gre2->SetPoint(18,103.5,0.551149);
      gre2->SetPointError(18,2.5,0.00199382);
      gre2->SetPoint(19,108,0.556932);
      gre2->SetPointError(19,2,0.00299469);
      gre2->SetPoint(20,112.5,0.563633);
      gre2->SetPointError(20,2.5,0.00335251);
      gre2->SetPoint(21,117.5,0.571283);
      gre2->SetPointError(21,2.5,0.00406895);
      gre2->SetPoint(22,123,0.578013);
      gre2->SetPointError(22,3,0.00439195);
      gre2->SetPoint(23,129.5,0.586868);
      gre2->SetPointError(23,3.5,0.00488118);
      gre2->SetPoint(24,137,0.592345);
      gre2->SetPointError(24,3.5,0.00545995);
      gre2->SetPoint(25,145.5,0.597644);
      gre2->SetPointError(25,4.5,0.00602192);
      gre2->SetPoint(26,155,0.6084);
      gre2->SetPointError(26,5,0.00670518);
      gre2->SetPoint(27,165.5,0.614082);
      gre2->SetPointError(27,5.5,0.00761271);
      gre2->SetPoint(28,178,0.631348);
      gre2->SetPointError(28,7,0.00796011);
      gre2->SetPoint(29,192.5,0.636414);
      gre2->SetPointError(29,7.5,0.00903206);
      gre2->SetPoint(30,210,0.653569);
      gre2->SetPointError(30,10,0.00946074);
      gre2->SetPoint(31,231.5,0.669816);
      gre2->SetPointError(31,11.5,0.0108759);
      gre2->SetPoint(32,258,0.675972);
      gre2->SetPointError(32,15,0.0120748);
      gre2->SetPoint(33,296.5,0.68854);
      gre2->SetPointError(33,23.5,0.012967);
      gre2->SetPoint(34,350,0.700051);
      gre2->SetPointError(34,30,0.016857);
      gre2->SetPoint(35,410,0.732671);
      gre2->SetPointError(35,30,0.0232442);
      gre2->SetPoint(36,475,0.724211);
      gre2->SetPointError(36,35,0.0306067);
      gre2->SetPoint(37,555,0.717008);
      gre2->SetPointError(37,45,0.0395249);
      gre2->SetPoint(38,800,0.728825);
      gre2->SetPointError(38,200,0.0412554);
      gre2->SetPoint(39,1250,0.746592);
      gre2->SetPointError(39,250,0.142552);
      gra2 = gre2;
    }
  else if( mode==Fig_3_a )
    {
      gre->SetPoint(0,17.5,0.0056);
      gre->SetPointError(0,2.5,0.0001);
      gre->SetPoint(1,25,0.0119);
      gre->SetPointError(1,5,0.0001);
      gre->SetPoint(2,35,0.0907);
      gre->SetPointError(2,5,0.0007);
      gre->SetPoint(3,45,0.2652);
      gre->SetPointError(3,5,0.0016);
      gre->SetPoint(4,55,0.3746);
      gre->SetPointError(4,5,0.0022);
      gre->SetPoint(5,68,0.4511);
      gre->SetPointError(5,8,0.0016);
      gre->SetPoint(6,81,0.4785);
      gre->SetPointError(6,5,0.0012);
      gre->SetPoint(7,91,0.4834);
      gre->SetPointError(7,5,0.0005);
      gre->SetPoint(8,101,0.5081);
      gre->SetPointError(8,5,0.0022);
      gre->SetPoint(9,113,0.5264);
      gre->SetPointError(9,7,0.0043);
      gre->SetPoint(10,135,0.5419);
      gre->SetPointError(10,15,0.0058);
      gre->SetPoint(11,175,0.5808);
      gre->SetPointError(11,25,0.0089);
      gre->SetPoint(12,400,0.6717);
      gre->SetPointError(12,200,0.0127);
      gra = gre;
    }
  else if( mode==Fig_3_b )
    {
      gre->SetPoint(0,17.5,0.0010);
      gre->SetPointError(0,2.5,0.0001);
      gre->SetPoint(1,25,0.0054);
      gre->SetPointError(1,5,0.0001);
      gre->SetPoint(2,35,0.0385);
      gre->SetPointError(2,5,0.0007);
      gre->SetPoint(3,45,0.1281);
      gre->SetPointError(3,5,0.0016);
      gre->SetPoint(4,55,0.1829);
      gre->SetPointError(4,5,0.0022);
      gre->SetPoint(5,68,0.2241);
      gre->SetPointError(5,8,0.0016);
      gre->SetPoint(6,81,0.2640);
      gre->SetPointError(6,5,0.0012);
      gre->SetPoint(7,91,0.3056);
      gre->SetPointError(7,5,0.0005);
      gre->SetPoint(8,101,0.3157);
      gre->SetPointError(8,5,0.0022);
      gre->SetPoint(9,113,0.3326);
      gre->SetPointError(9,7,0.0043);
      gre->SetPoint(10,135,0.3335);
      gre->SetPointError(10,15,0.0058);
      gre->SetPoint(11,175,0.3614);
      gre->SetPointError(11,25,0.0089);
      gre->SetPoint(12,400,0.4242);
      gre->SetPointError(12,200,0.0127);
      gra = gre;
    }
  else
    return 0;

  gra->SetName("Graph");
  gra->SetMarkerStyle(kFullCircle);
  gra->SetLineWidth(2);
  gra->SetLineColor(kBlue);
  gra->SetMarkerColor(kBlue);

  if( mode == Fig_2 || mode == Fig_3 ) {
    gra1->SetName("Graph1");
    gra1->SetMarkerStyle(25);
    gra1->SetLineWidth(2);
    gra1->SetLineColor(kBlack);
    gra1->SetMarkerColor(kBlack);

    gra2->SetName("Graph1");
    gra2->SetMarkerStyle(26);
    gra2->SetLineWidth(2);
    gra2->SetLineColor(kRed);
    gra2->SetMarkerColor(kRed);
  }

  // text2
  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "at #sqrt{s} = 8 TeV, CMS Simulation";
  txtSize[0] = 0.04;
  txtX[0] = 0.88;
  txtY[0] = 0.85;
  txtAlign[0] = 31;
  txtFont[0] = 42;
  
  txt[1] = "#gamma*/Z #rightarrow ";
  if( mode==Fig_2_a || mode==Fig_2_b || mode == Fig_2 )
    //txt[1] += "ee";
    txt[1] += "ee";
  else   if( mode==Fig_3_a || mode==Fig_3_b || mode == Fig_3 )
    txt[1] += "ee";
  else return 0;
  txtSize[1] = 0.06;
  txtX[1] = 0.20;
  txtY[1] = 0.85;
  txtAlign[1] = 11;
  txtFont[1] = 42;

  // canvas
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=250;

  // histogram limits, in linear and logarithmic
  int nbin_(100);
  float xmin_(15), xmax_(1500); 
  float ymin_(0.), ymax_(1.05); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(0);
  int ndivy(510);
  float markerSize(1.2);

  // canvas name
  TString ctitle;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString cname;
  if( mode == Fig_2 )
    {    
      cname += "Fig_2";
      ctitle = "Acceptance, mu-mu analysis";
      //xtitle = "M(ee) (post-FSR)  [";	        
      xtitle = "M(ee) (post-FSR)  [";
      ytitle = ""; 
    }
  else if( mode == Fig_2_a )
    {    
      cname += "Fig_2_a";
      ctitle = "Acceptance, mu-mu analysis";
      //xtitle = "M(ee) (post-FSR)  [";	        
      xtitle = "M(ee) (post-FSR)  [";  
      ytitle = "A"; 
    }
  else if( mode == Fig_2_b )
    {      
      cname += "Fig_2_b";
      ctitle = "Acceptance times efficiency, mu-mu analysis";
      //xtitle = "M(ee) (post-FSR)  [";	        
      xtitle = "M(ee) (post-FSR)  ["; 
      ytitle = "A #times #epsilon"; 
    }
  else if( mode == Fig_3 )
    {
      cname += "Fig_3";
      ctitle = "Acceptance, e-e analysis";
      xtitle = "M(ee) (post-FSR)  [";	        
      ytitle = ""; 
    } 
  else if( mode == Fig_3_a )
    {
      cname += "Fig_3_a";
      ctitle = "Acceptance, e-e analysis";
      xtitle = "M(ee) (post-FSR)  [";	        
      ytitle = "A"; 
    } 
  else if( mode == Fig_3_b )
    {
      cname += "Fig_3_b";
      ctitle = "Acceptance times efficiency, e-e analysis";
      xtitle = "M(ee) (post-FSR)  [";	        
      ytitle = "A #times #epsilon"; 
    } 
  else
    return 0;

  xtitle += massUnit;       
  xtitle += "]";

  nbin_ = 100;
  xmin_ = 15.;
  xmax_ = 1500.;
  ndivx = 0;
  ndivy = 506;	  
  markerSize = 1.2;

  //  size_t nChan=channels.size();
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,
			  xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin(   60./dxcanv_ );
  c_->SetRightMargin(  30./dxcanv_ );
  c_->SetTopMargin(    30./dxcanv_ );
  c_->SetBottomMargin( 60./dxcanv_ ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);
   
  float titleOffsetY(1.0);
  float titleSizeY(xcanv_*0.060/ycanv_);
  float labelOffsetY(0.01);
  float titleOffsetX(1.0);
  float titleSizeX(0.060);
  float labelOffsetX(99);
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon2", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  ax_->SetTitleOffset(titleOffsetX);
  ax_->SetTitleSize(titleSizeX);
  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(labelOffsetX);
  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetLabelOffset(labelOffsetY);


  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

  c_->SetLogy(false);
  c_->SetLogx(true);
  h_->GetYaxis()->SetRangeUser(ymin_,ymax_);

  h_->Draw("hist");
  draw_axis_labels(ymin_-0.015,0.050);

  //draw_bin_grid( ymin_, 0.68, 0, 60);
  //draw_bin_grid( ymin_, 1.05, 50, 600 );

  bool drawHGrid = false;
  if( drawHGrid )
    {
      TLine grid_;
      grid_.SetLineColor(kGray+2);
      grid_.SetLineWidth(1);
      float y_;
      grid_.SetLineStyle(kDashed);
      y_=0.2;  grid_.DrawLine(15,y_,600,y_);
      y_=0.4;  grid_.DrawLine(15,y_,600,y_);
      y_=0.6;  grid_.DrawLine(15,y_,600,y_);
      grid_.SetLineStyle(kDotted);
      y_=0.1;  grid_.DrawLine(15,y_,600,y_);
      y_=0.3;  grid_.DrawLine(15,y_,600,y_);
      y_=0.5;  grid_.DrawLine(15,y_,600,y_);
      y_=0.7;  grid_.DrawLine(15,y_,600,y_);
    }

  gra->Draw("P");
  if( mode == Fig_2 || mode == Fig_3 ) {
    gra2->Draw("Psame");
    gra1->Draw("Psame");

    // legend position and scale;
    int legendTextFont=42;
    float legendTextSize=0.040;
    double xl_ = 0.7;
    double yl_ = 0.2;
    double dxl_ = 0.25;
    double dyl_ = 0.20;
    TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
    legend->SetTextFont(legendTextFont);
    legend->SetTextSize(legendTextSize);
    legend->SetLineColor(0);
    legend->SetFillColor(0);

    legend->AddEntry(gra," A","PL");
    legend->AddEntry(gra2," #epsilon","PL");
    legend->AddEntry(gra1," A #times #epsilon","PL");
    legend->Draw("Same");
  }

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  c_->RedrawAxis();
  c_->GetFrame()->Draw();

  c_->cd();
  return c_;
}

//#include "Efficiency.C"
TCanvas* Efficiency( int mode )
{
  TGraph* graph[2];
  TGraphAsymmErrors* grae;
  TGraphErrors* gre;
  //  TGraphErrors* gre  = new TGraphErrors(14);
  if( mode==Fig_4_a )
    {
      // les donnees
      grae = new TGraphAsymmErrors(60);
      grae->SetName("Graph_eta_data");
      grae->SetTitle("RECO (+ ID) selection efficiency vs eta");
      grae->SetPoint(0,-2.36,0.92);
      grae->SetPointError(0,0.04,0.04,0.02404081,0.02031018);
      grae->SetPoint(1,-2.28,0.9349112);
      grae->SetPointError(1,0.04,0.04,0.02075034,0.01731186);
      grae->SetPoint(2,-2.2,0.9655172);
      grae->SetPointError(2,0.04,0.04,0.01580232,0.01218441);
      grae->SetPoint(3,-2.12,0.9526066);
      grae->SetPointError(3,0.04,0.04,0.01614681,0.01326916);
      grae->SetPoint(4,-2.04,0.9724771);
      grae->SetPointError(4,0.04,0.04,0.01269567,0.009757213);
      grae->SetPoint(5,-1.96,0.9658537);
      grae->SetPointError(5,0.04,0.04,0.01434589,0.01127811);
      grae->SetPoint(6,-1.88,0.9710744);
      grae->SetPointError(6,0.04,0.04,0.01220931,0.009577073);
      grae->SetPoint(7,-1.8,0.9786325);
      grae->SetPointError(7,0.04,0.04,0.01101196,0.008222661);
      grae->SetPoint(8,-1.72,0.8848921);
      grae->SetPointError(8,0.04,0.04,0.02005081,0.01820835);
      grae->SetPoint(9,-1.64,0.9635628);
      grae->SetPointError(9,0.04,0.04,0.01327662,0.01075068);
      grae->SetPoint(10,-1.56,0.960396);
      grae->SetPointError(10,0.04,0.04,0.01228595,0.01024624);
      grae->SetPoint(11,-1.48,0.9836066);
      grae->SetPointError(11,0.04,0.04,0.007641001,0.005842101);
      grae->SetPoint(12,-1.4,0.9876161);
      grae->SetPointError(12,0.04,0.04,0.007345055,0.005265252);
      grae->SetPoint(13,-1.32,0.980663);
      grae->SetPointError(13,0.04,0.04,0.008231887,0.006430882);
      grae->SetPoint(14,-1.24,0.9781931);
      grae->SetPointError(14,0.04,0.04,0.009263001,0.007243995);
      grae->SetPoint(15,-1.16,0.9586777);
      grae->SetPointError(15,0.04,0.04,0.0113396,0.009645681);
      grae->SetPoint(16,-1.08,0.9458689);
      grae->SetPointError(16,0.04,0.04,0.01296,0.01126114);
      grae->SetPoint(17,-1,0.9562982);
      grae->SetPointError(17,0.04,0.04,0.01118982,0.009618574);
      grae->SetPoint(18,-0.92,0.9811321);
      grae->SetPointError(18,0.04,0.04,0.00803553,0.006276237);
      grae->SetPoint(19,-0.84,0.9488491);
      grae->SetPointError(19,0.04,0.04,0.01194047,0.01040483);
      grae->SetPoint(20,-0.76,0.986911);
      grae->SetPointError(20,0.04,0.04,0.006801485,0.005056858);
      grae->SetPoint(21,-0.68,0.9894737);
      grae->SetPointError(21,0.04,0.04,0.006255958,0.004479549);
      grae->SetPoint(22,-0.6,0.9820051);
      grae->SetPointError(22,0.04,0.04,0.007669633,0.005988232);
      grae->SetPoint(23,-0.52,0.9814324);
      grae->SetPointError(23,0.04,0.04,0.007909747,0.006177205);
      grae->SetPoint(24,-0.44,0.9904306);
      grae->SetPointError(24,0.04,0.04,0.005693169,0.00407423);
      grae->SetPoint(25,-0.36,0.9862385);
      grae->SetPointError(25,0.04,0.04,0.006429975,0.004910176);
      grae->SetPoint(26,-0.28,0.8894231);
      grae->SetPointError(26,0.04,0.04,0.01599779,0.01475055);
      grae->SetPoint(27,-0.2,0.9088785);
      grae->SetPointError(27,0.04,0.04,0.01455299,0.0132792);
      grae->SetPoint(28,-0.12,0.9877751);
      grae->SetPointError(28,0.04,0.04,0.006357952,0.004724973);
      grae->SetPoint(29,-0.04,0.9771689);
      grae->SetPointError(29,0.04,0.04,0.007934261,0.006464707);
      grae->SetPoint(30,0.04,0.9672131);
      grae->SetPointError(30,0.04,0.04,0.009399742,0.00793047);
      grae->SetPoint(31,0.12,0.9813953);
      grae->SetPointError(31,0.04,0.04,0.007349248,0.005833418);
      grae->SetPoint(32,0.2,0.91);
      grae->SetPointError(32,0.04,0.04,0.01499851,0.01363187);
      grae->SetPoint(33,0.28,0.9017857);
      grae->SetPointError(33,0.04,0.04,0.01466136,0.01346581);
      grae->SetPoint(34,0.36,0.9932432);
      grae->SetPointError(34,0.04,0.04,0.004801627,0.003248978);
      grae->SetPoint(35,0.44,0.9974425);
      grae->SetPointError(35,0.04,0.04,0.003816995,0.001868922);
      grae->SetPoint(36,0.52,0.9877751);
      grae->SetPointError(36,0.04,0.04,0.006357952,0.004724973);
      grae->SetPoint(37,0.6,0.9876543);
      grae->SetPointError(37,0.04,0.04,0.006419975,0.004771366);
      grae->SetPoint(38,0.68,0.986911);
      grae->SetPointError(38,0.04,0.04,0.006801485,0.005056858);
      grae->SetPoint(39,0.76,0.9863388);
      grae->SetPointError(39,0.04,0.04,0.007094776,0.005276486);
      grae->SetPoint(40,0.84,0.933162);
      grae->SetPointError(40,0.04,0.04,0.013425,0.01193811);
      grae->SetPoint(41,0.92,0.9786667);
      grae->SetPointError(41,0.04,0.04,0.008407612,0.006680633);
      grae->SetPoint(42,1,0.9636872);
      grae->SetPointError(42,0.04,0.04,0.01081067,0.00907173);
      grae->SetPoint(43,1.08,0.9749304);
      grae->SetPointError(43,0.04,0.04,0.009220988,0.007435419);
      grae->SetPoint(44,1.16,0.972067);
      grae->SetPointError(44,0.04,0.04,0.009667675,0.007890989);
      grae->SetPoint(45,1.24,0.9684543);
      grae->SetPointError(45,0.04,0.04,0.0108864,0.008896893);
      grae->SetPoint(46,1.32,0.9650794);
      grae->SetPointError(46,0.04,0.04,0.01140373,0.009418885);
      grae->SetPoint(47,1.4,0.9781931);
      grae->SetPointError(47,0.04,0.04,0.009263001,0.007243995);
      grae->SetPoint(48,1.48,0.9748428);
      grae->SetPointError(48,0.04,0.04,0.009882384,0.007864286);
      grae->SetPoint(49,1.56,0.9488055);
      grae->SetPointError(49,0.04,0.04,0.01394629,0.01189587);
      grae->SetPoint(50,1.64,0.973384);
      grae->SetPointError(50,0.04,0.04,0.01125757,0.008821835);
      grae->SetPoint(51,1.72,0.9352518);
      grae->SetPointError(51,0.04,0.04,0.01583499,0.01374365);
      grae->SetPoint(52,1.8,0.9184549);
      grae->SetPointError(52,0.04,0.04,0.01914056,0.0167464);
      grae->SetPoint(53,1.88,0.9913793);
      grae->SetPointError(53,0.04,0.04,0.007893294,0.004873673);
      grae->SetPoint(54,1.96,0.9619048);
      grae->SetPointError(54,0.04,0.04,0.01479972,0.01183774);
      grae->SetPoint(55,2.04,0.9666667);
      grae->SetPointError(55,0.04,0.04,0.01401451,0.01101377);
      grae->SetPoint(56,2.12,0.9760766);
      grae->SetPointError(56,0.04,0.04,0.0122977,0.009194963);
      grae->SetPoint(57,2.2,0.9405941);
      grae->SetPointError(57,0.04,0.04,0.01814629,0.01522908);
      grae->SetPoint(58,2.28,0.9489796);
      grae->SetPointError(58,0.04,0.04,0.01733116,0.01426059);
      grae->SetPoint(59,2.36,0.9047619);
      grae->SetPointError(59,0.04,0.04,0.02424399,0.02103911);

      for( int ii=0; ii<grae->GetN(); ii++ )
	{
	  grae->SetPointEXlow(ii,0);
	  grae->SetPointEXhigh(ii,0);
	}
      graph[0] = grae;
   
      grae = new TGraphAsymmErrors(60);
      grae->SetName("Graph_eta_mc");
      grae->SetTitle("RECO (+ ID) selection efficiency vs eta");

      grae->SetPoint(0,-2.36,0.92951);
      grae->SetPointError(0,0.04,0.04,0.003229015,0.003140375);
      grae->SetPoint(1,-2.28,0.971831);
      grae->SetPointError(1,0.04,0.04,0.002020318,0.00193066);
      grae->SetPoint(2,-2.2,0.9597503);
      grae->SetPointError(2,0.04,0.04,0.002307935,0.002226384);
      grae->SetPoint(3,-2.12,0.9644715);
      grae->SetPointError(3,0.04,0.04,0.002126341,0.002047637);
      grae->SetPoint(4,-2.04,0.9690161);
      grae->SetPointError(4,0.04,0.04,0.001923054,0.001848981);
      grae->SetPoint(5,-1.96,0.9839566);
      grae->SetPointError(5,0.04,0.04,0.001373719,0.00130066);
      grae->SetPoint(6,-1.88,0.9856596);
      grae->SetPointError(6,0.04,0.04,0.001281109,0.001210067);
      grae->SetPoint(7,-1.8,0.9737177);
      grae->SetPointError(7,0.04,0.04,0.001681939,0.00161489);
      grae->SetPoint(8,-1.72,0.9630639);
      grae->SetPointError(8,0.04,0.04,0.001927429,0.001865027);
      grae->SetPoint(9,-1.64,0.9841797);
      grae->SetPointError(9,0.04,0.04,0.001266154,0.001202982);
      grae->SetPoint(10,-1.56,0.9805024);
      grae->SetPointError(10,0.04,0.04,0.001325289,0.00126892);
      grae->SetPoint(11,-1.48,0.9880668);
      grae->SetPointError(11,0.04,0.04,0.001013182,0.0009595093);
      grae->SetPoint(12,-1.4,0.99026);
      grae->SetPointError(12,0.04,0.04,0.0009087849,0.000856041);
      grae->SetPoint(13,-1.32,0.9928092);
      grae->SetPointError(13,0.04,0.04,0.000778445,0.0007263762);
      grae->SetPoint(14,-1.24,0.9904097);
      grae->SetPointError(14,0.04,0.04,0.0008800228,0.0008297373);
      grae->SetPoint(15,-1.16,0.9825603);
      grae->SetPointError(15,0.04,0.04,0.00115274,0.001104907);
      grae->SetPoint(16,-1.08,0.9810081);
      grae->SetPointError(16,0.04,0.04,0.001184237,0.001137845);
      grae->SetPoint(17,-1,0.9752301);
      grae->SetPointError(17,0.04,0.04,0.001315884,0.001271975);
      grae->SetPoint(18,-0.92,0.9786413);
      grae->SetPointError(18,0.04,0.04,0.001223179,0.001179136);
      grae->SetPoint(19,-0.84,0.9488874);
      grae->SetPointError(19,0.04,0.04,0.001829896,0.001789484);
      grae->SetPoint(20,-0.76,0.9829819);
      grae->SetPointError(20,0.04,0.04,0.001087922,0.001044184);
      grae->SetPoint(21,-0.68,0.9903423);
      grae->SetPointError(21,0.04,0.04,0.0008210168,0.0007773752);
      grae->SetPoint(22,-0.6,0.9905599);
      grae->SetPointError(22,0.04,0.04,0.0008026193,0.000759942);
      grae->SetPoint(23,-0.52,0.9924943);
      grae->SetPointError(23,0.04,0.04,0.0007166116,0.0006740178);
      grae->SetPoint(24,-0.44,0.9954114);
      grae->SetPointError(24,0.04,0.04,0.0005617849,0.0005195476);
      grae->SetPoint(25,-0.36,0.9907313);
      grae->SetPointError(25,0.04,0.04,0.0007881236,0.0007462062);
      grae->SetPoint(26,-0.28,0.9165415);
      grae->SetPointError(26,0.04,0.04,0.002206402,0.002171614);
      grae->SetPoint(27,-0.2,0.9371286);
      grae->SetPointError(27,0.04,0.04,0.001939402,0.001902893);
      grae->SetPoint(28,-0.12,0.994357);
      grae->SetPointError(28,0.04,0.04,0.0006149306,0.0005734833);
      grae->SetPoint(29,-0.04,0.9782952);
      grae->SetPointError(29,0.04,0.04,0.001178648,0.001138346);
      grae->SetPoint(30,0.04,0.9787695);
      grae->SetPointError(30,0.04,0.04,0.001154895,0.00111532);
      grae->SetPoint(31,0.12,0.994395);
      grae->SetPointError(31,0.04,0.04,0.0006108077,0.0005696362);
      grae->SetPoint(32,0.2,0.9229412);
      grae->SetPointError(32,0.04,0.04,0.002136042,0.002100464);
      grae->SetPoint(33,0.28,0.9125024);
      grae->SetPointError(33,0.04,0.04,0.002258974,0.002224359);
      grae->SetPoint(34,0.36,0.9900498);
      grae->SetPointError(34,0.04,0.04,0.0008092633,0.0007680271);
      grae->SetPoint(35,0.44,0.9940947);
      grae->SetPointError(35,0.04,0.04,0.0006397424,0.0005968699);
      grae->SetPoint(36,0.52,0.9916894);
      grae->SetPointError(36,0.04,0.04,0.0007538775,0.0007112112);
      grae->SetPoint(37,0.6,0.9894976);
      grae->SetPointError(37,0.04,0.04,0.0008430246,0.0008006234);
      grae->SetPoint(38,0.68,0.9898176);
      grae->SetPointError(38,0.04,0.04,0.0008418197,0.0007982624);
      grae->SetPoint(39,0.76,0.9803015);
      grae->SetPointError(39,0.04,0.04,0.001160016,0.001117037);
      grae->SetPoint(40,0.84,0.9516151);
      grae->SetPointError(40,0.04,0.04,0.001789358,0.00174844);
      grae->SetPoint(41,0.92,0.9775866);
      grae->SetPointError(41,0.04,0.04,0.001256292,0.001212035);
      grae->SetPoint(42,1,0.9742876);
      grae->SetPointError(42,0.04,0.04,0.001361393,0.001316155);
      grae->SetPoint(43,1.08,0.9750401);
      grae->SetPointError(43,0.04,0.04,0.001357033,0.001310734);
      grae->SetPoint(44,1.16,0.9745984);
      grae->SetPointError(44,0.04,0.04,0.001384842,0.001337491);
      grae->SetPoint(45,1.24,0.9784727);
      grae->SetPointError(45,0.04,0.04,0.001286677,0.001238408);
      grae->SetPoint(46,1.32,0.9845035);
      grae->SetPointError(46,0.04,0.04,0.001125083,0.001073911);
      grae->SetPoint(47,1.4,0.9861856);
      grae->SetPointError(47,0.04,0.04,0.001079814,0.001027031);
      grae->SetPoint(48,1.48,0.9816092);
      grae->SetPointError(48,0.04,0.04,0.001245096,0.001192281);
      grae->SetPoint(49,1.56,0.9849953);
      grae->SetPointError(49,0.04,0.04,0.001154795,0.00109924);
      grae->SetPoint(50,1.64,0.9806452);
      grae->SetPointError(50,0.04,0.04,0.001394955,0.001332194);
      grae->SetPoint(51,1.72,0.9552742);
      grae->SetPointError(51,0.04,0.04,0.002121369,0.002059295);
      grae->SetPoint(52,1.8,0.9708688);
      grae->SetPointError(52,0.04,0.04,0.001746461,0.00168127);
      grae->SetPoint(53,1.88,0.9871183);
      grae->SetPointError(53,0.04,0.04,0.001226185,0.001153889);
      grae->SetPoint(54,1.96,0.984375);
      grae->SetPointError(54,0.04,0.04,0.001363097,0.001289279);
      grae->SetPoint(55,2.04,0.980177);
      grae->SetPointError(55,0.04,0.04,0.001553726,0.001478035);
      grae->SetPoint(56,2.12,0.974922);
      grae->SetPointError(56,0.04,0.04,0.001787914,0.001708768);
      grae->SetPoint(57,2.2,0.974359);
      grae->SetPointError(57,0.04,0.04,0.001836856,0.001755217);
      grae->SetPoint(58,2.28,0.9780389);
      grae->SetPointError(58,0.04,0.04,0.001780019,0.001690687);
      grae->SetPoint(59,2.36,0.9460373);
      grae->SetPointError(59,0.04,0.04,0.002785608,0.002698043);

      graph[1] = grae;

    }
  else if( mode==Fig_4_b )
    {
      // les donnees
      grae = new TGraphAsymmErrors(56);
      grae->SetName("Graph_pt_data");
      grae->SetTitle("Single muon trigger efficiency vs pt (GeV)");
      grae->SetPoint(0,7,0);
      grae->SetPointError(0,1,1,0,0.08458096);
      grae->SetPoint(1,9,0.25);
      grae->SetPointError(1,1,1,0.09352039,0.1125048);
      grae->SetPoint(2,11,0.3589744);
      grae->SetPointError(2,1,1,0.07272137,0.07734176);
      grae->SetPoint(3,13,0.5285714);
      grae->SetPointError(3,1,1,0.05913219,0.05860094);
      grae->SetPoint(4,15,0.7226891);
      grae->SetPointError(4,1,1,0.04199273,0.03952849);
      grae->SetPoint(5,17,0.8876404);
      grae->SetPointError(5,1,1,0.02508935,0.02219588);
      grae->SetPoint(6,19,0.9142857);
      grae->SetPointError(6,1,1,0.01902132,0.01676779);
      grae->SetPoint(7,21,0.8858131);
      grae->SetPointError(7,1,1,0.01958632,0.01780946);
      grae->SetPoint(8,23,0.8867925);
      grae->SetPointError(8,1,1,0.01713981,0.0157512);
      grae->SetPoint(9,25,0.9422311);
      grae->SetPointError(9,1,1,0.01102063,0.00984351);
      grae->SetPoint(10,27,0.9170306);
      grae->SetPointError(10,1,1,0.0109358,0.01012583);
      grae->SetPoint(11,29,0.8975469);
      grae->SetPointError(11,1,1,0.01190527,0.01114021);
      grae->SetPoint(12,31,0.9263804);
      grae->SetPointError(12,1,1,0.009504838,0.008806508);
      grae->SetPoint(13,33,0.9259259);
      grae->SetPointError(13,1,1,0.008960575,0.008341253);
      grae->SetPoint(14,35,0.9253035);
      grae->SetPointError(14,1,1,0.008304789,0.007774717);
      grae->SetPoint(15,37,0.9271071);
      grae->SetPointError(15,1,1,0.007385475,0.006952555);
      grae->SetPoint(16,39,0.9278623);
      grae->SetPointError(16,1,1,0.006849882,0.006471869);
      grae->SetPoint(17,41,0.9279279);
      grae->SetPointError(17,1,1,0.006514175,0.00617107);
      grae->SetPoint(18,43,0.9219736);
      grae->SetPointError(18,1,1,0.006590457,0.006267294);
      grae->SetPoint(19,45,0.9177546);
      grae->SetPointError(19,1,1,0.00720585,0.006841888);
      grae->SetPoint(20,47,0.9252419);
      grae->SetPointError(20,1,1,0.008055417,0.007556182);
      grae->SetPoint(21,49,0.9194245);
      grae->SetPointError(21,1,1,0.01073458,0.009929265);
      grae->SetPoint(22,51,0.9327354);
      grae->SetPointError(22,1,1,0.01252466,0.01122915);
      grae->SetPoint(23,53,0.9205298);
      grae->SetPointError(23,1,1,0.01650643,0.0146492);
      grae->SetPoint(24,55,0.9108911);
      grae->SetPointError(24,1,1,0.02140571,0.01869679);
      grae->SetPoint(25,57,0.9638554);
      grae->SetPointError(25,1,1,0.01653795,0.01276155);
      grae->SetPoint(26,59,0.9142857);
      grae->SetPointError(26,1,1,0.02995479,0.02470859);
      grae->SetPoint(27,61,0.9514563);
      grae->SetPointError(27,1,1,0.02434198,0.01843664);
      grae->SetPoint(28,63,0.9473684);
      grae->SetPointError(28,1,1,0.02986365,0.0219328);
      grae->SetPoint(29,65,0.9322034);
      grae->SetPointError(29,1,1,0.03782348,0.02803534);
      grae->SetPoint(30,67,0.9090909);
      grae->SetPointError(30,1,1,0.04364427,0.03380741);
      grae->SetPoint(31,69,0.9848485);
      grae->SetPointError(31,1,1,0.02192489,0.01097634);
      grae->SetPoint(32,71,0.9375);
      grae->SetPointError(32,1,1,0.05263088,0.0342453);
      grae->SetPoint(33,73,0.9375);
      grae->SetPointError(33,1,1,0.05263088,0.0342453);
      grae->SetPoint(34,75,0.875);
      grae->SetPointError(34,1,1,0.07645406,0.05640078);
      grae->SetPoint(35,77,0.8148148);
      grae->SetPointError(35,1,1,0.08034842,0.06553542);
      grae->SetPoint(36,79,0.7777778);
      grae->SetPointError(36,1,1,0.1035062,0.08449391);
      grae->SetPoint(37,81,1);
      grae->SetPointError(37,1,1,0.06928609,0);
      grae->SetPoint(38,83,0.9411765);
      grae->SetPointError(38,1,1,0.07664419,0.04132487);
      grae->SetPoint(39,85,1);
      grae->SetPointError(39,1,1,0.1085317,0);
      grae->SetPoint(40,87,1);
      grae->SetPointError(40,1,1,0.1513615,0);
      grae->SetPoint(41,89,1);
      grae->SetPointError(41,1,1,0.09917226,0);
      grae->SetPoint(42,91,1);
      grae->SetPointError(42,1,1,0.119839,0);
      grae->SetPoint(43,93,0.8571429);
      grae->SetPointError(43,1,1,0.1533797,0.09435654);
      grae->SetPoint(44,95,0.9);
      grae->SetPointError(44,1,1,0.1183613,0.0681878);
      grae->SetPoint(45,97,1);
      grae->SetPointError(45,1,1,0.1337716,0);
      grae->SetPoint(46,99,1);
      grae->SetPointError(46,1,1,0.09917226,0);
      grae->SetPoint(47,101,1);
      grae->SetPointError(47,1,1,0.1337716,0);
      grae->SetPoint(48,103,1);
      grae->SetPointError(48,1,1,0.3181538,0);
      grae->SetPoint(49,105,1);
      grae->SetPointError(49,1,1,0.1742602,0);
      grae->SetPoint(50,107,1);
      grae->SetPointError(50,1,1,0.1742602,0);
      grae->SetPoint(51,109,0.6666667);
      grae->SetPointError(51,1,1,0.2366508,0.189572);
      grae->SetPoint(52,111,1);
      grae->SetPointError(52,1,1,0.2496484,0);
      grae->SetPoint(53,113,0.6);
      grae->SetPointError(53,1,1,0.1952219,0.1755773);
      grae->SetPoint(54,115,1);
      grae->SetPointError(54,1,1,0.2496484,0);
      grae->SetPoint(55,119,1);
      grae->SetPointError(55,1,1,0.4369725,0);

      for( int ii=0; ii<grae->GetN(); ii++ )
	{
	  grae->SetPointEXlow(ii,0);
	  grae->SetPointEXhigh(ii,0);
	}
      graph[0] = grae;

      grae = new TGraphAsymmErrors(57);
      grae->SetName("Graph_pt_mc");
      grae->SetTitle("Single muon trigger efficiency vs pt (GeV);pt (GeV);;efficiency");
      grae->SetPointError(0,1,1,0,0.004168935);
      grae->SetPoint(1,9,0.5360624);
      grae->SetPointError(1,1,1,0.01558739,0.01554055);
      grae->SetPoint(2,11,0.8946809);
      grae->SetPointError(2,1,1,0.007223228,0.006943066);
      grae->SetPoint(3,13,0.9059436);
      grae->SetPointError(3,1,1,0.005195461,0.005029452);
      grae->SetPoint(4,15,0.9272835);
      grae->SetPointError(4,1,1,0.003714162,0.003601139);
      grae->SetPoint(5,17,0.9254616);
      grae->SetPointError(5,1,1,0.003124088,0.003045831);
      grae->SetPoint(6,19,0.9395746);
      grae->SetPointError(6,1,1,0.002406189,0.002347863);
      grae->SetPoint(7,21,0.9403599);
      grae->SetPointError(7,1,1,0.002109783,0.002064187);
      grae->SetPoint(8,23,0.9453125);
      grae->SetPointError(8,1,1,0.001795684,0.001759394);
      grae->SetPoint(9,25,0.9474463);
      grae->SetPointError(9,1,1,0.001582551,0.001553101);
      grae->SetPoint(10,27,0.9512906);
      grae->SetPointError(10,1,1,0.001402406,0.001377322);
      grae->SetPoint(11,29,0.9513379);
      grae->SetPointError(11,1,1,0.001283077,0.001262027);
      grae->SetPoint(12,31,0.9524714);
      grae->SetPointError(12,1,1,0.001177949,0.001159743);
      grae->SetPoint(13,33,0.9566386);
      grae->SetPointError(13,1,1,0.001048962,0.001033056);
      grae->SetPoint(14,35,0.9545954);
      grae->SetPointError(14,1,1,0.001003816,0.0009899205);
      grae->SetPoint(15,37,0.9551398);
      grae->SetPointError(15,1,1,0.0009336709,0.0009214858);
      grae->SetPoint(16,39,0.954199);
      grae->SetPointError(16,1,1,0.0008913097,0.0008804344);
      grae->SetPoint(17,41,0.9533635);
      grae->SetPointError(17,1,1,0.0008574246,0.0008475441);
      grae->SetPoint(18,43,0.953584);
      grae->SetPointError(18,1,1,0.0008309553,0.0008216272);
      grae->SetPoint(19,45,0.9546014);
      grae->SetPointError(19,1,1,0.0008661043,0.0008557381);
      grae->SetPoint(20,47,0.9529293);
      grae->SetPointError(20,1,1,0.001023983,0.001010059);
      grae->SetPoint(21,49,0.9533758);
      grae->SetPointError(21,1,1,0.001269415,0.001247872);
      grae->SetPoint(22,51,0.9536957);
      grae->SetPointError(22,1,1,0.001566743,0.001533822);
      grae->SetPoint(23,53,0.9519041);
      grae->SetPointError(23,1,1,0.001964615,0.001915094);
      grae->SetPoint(24,55,0.9537718);
      grae->SetPointError(24,1,1,0.002288288,0.002218614);
      grae->SetPoint(25,57,0.9531007);
      grae->SetPointError(25,1,1,0.002688339,0.002594071);
      grae->SetPoint(26,59,0.9525715);
      grae->SetPointError(26,1,1,0.003069873,0.002948935);
      grae->SetPoint(27,61,0.9491353);
      grae->SetPointError(27,1,1,0.00358329,0.003430761);
      grae->SetPoint(28,63,0.9522735);
      grae->SetPointError(28,1,1,0.003929417,0.00373464);
      grae->SetPoint(29,65,0.9549618);
      grae->SetPointError(29,1,1,0.00417199,0.003940054);
      grae->SetPoint(30,67,0.9493433);
      grae->SetPointError(30,1,1,0.004895096,0.004613612);
      grae->SetPoint(31,69,0.9517931);
      grae->SetPointError(31,1,1,0.005377385,0.005022588);
      grae->SetPoint(32,71,0.9531583);
      grae->SetPointError(32,1,1,0.0058518,0.005422103);
      grae->SetPoint(33,73,0.9350541);
      grae->SetPointError(33,1,1,0.007359554,0.006875885);
      grae->SetPoint(34,75,0.9521436);
      grae->SetPointError(34,1,1,0.007052403,0.006449981);
      grae->SetPoint(35,77,0.9540598);
      grae->SetPointError(35,1,1,0.007179589,0.006531169);
      grae->SetPoint(36,79,0.9447174);
      grae->SetPointError(36,1,1,0.008386826,0.007656946);
      grae->SetPoint(37,81,0.955414);
      grae->SetPointError(37,1,1,0.008741598,0.007771553);
      grae->SetPoint(38,83,0.9511344);
      grae->SetPointError(38,1,1,0.009553382,0.008500575);
      grae->SetPoint(39,85,0.9338677);
      grae->SetPointError(39,1,1,0.01172057,0.01055956);
      grae->SetPoint(40,87,0.9426752);
      grae->SetPointError(40,1,1,0.01136013,0.01010415);
      grae->SetPoint(41,89,0.9355609);
      grae->SetPointError(41,1,1,0.01270844,0.0113201);
      grae->SetPoint(42,91,0.9429429);
      grae->SetPointError(42,1,1,0.01363201,0.01185368);
      grae->SetPoint(43,93,0.9492063);
      grae->SetPointError(43,1,1,0.01336796,0.01145925);
      grae->SetPoint(44,95,0.9529412);
      grae->SetPointError(44,1,1,0.01451433,0.01213323);
      grae->SetPoint(45,97,0.9618644);
      grae->SetPointError(45,1,1,0.01387583,0.01124294);
      grae->SetPoint(46,99,0.9419643);
      grae->SetPointError(46,1,1,0.01699306,0.01435393);
      grae->SetPoint(47,101,0.9384615);
      grae->SetPointError(47,1,1,0.01876615,0.01576004);
      grae->SetPoint(48,103,0.9428571);
      grae->SetPointError(48,1,1,0.01931378,0.01592626);
      grae->SetPoint(49,105,0.9856115);
      grae->SetPointError(49,1,1,0.01305689,0.008107657);
      grae->SetPoint(50,107,0.9496403);
      grae->SetPointError(50,1,1,0.02085123,0.01650674);
      grae->SetPoint(51,109,0.9752066);
      grae->SetPointError(51,1,1,0.01722677,0.01181053);
      grae->SetPoint(52,111,0.9285714);
      grae->SetPointError(52,1,1,0.02695496,0.0218495);
      grae->SetPoint(53,113,0.950495);
      grae->SetPointError(53,1,1,0.02479979,0.01879289);
      grae->SetPoint(54,115,0.8681319);
      grae->SetPointError(54,1,1,0.03797934,0.03264037);
      grae->SetPoint(55,117,0.9726027);
      grae->SetPointError(55,1,1,0.02436437,0.01532266);
      grae->SetPoint(56,119,0.9634146);
      grae->SetPointError(56,1,1,0.02504684,0.01732011);


      graph[1] = grae;
    }
  else if( mode == Fig_5 )
    {
      gre = new TGraphErrors(50);
      gre->SetName("Graph_iso_pT");
      gre->SetTitle("isolation efficiency");
   gre->SetPoint(0,7,0.9038598);
   gre->SetPointError(0,1,0.002487645);
   gre->SetPoint(1,9,0.9332612);
   gre->SetPointError(1,1,0.001609822);
   gre->SetPoint(2,11,0.9510225);
   gre->SetPointError(2,1,0.001133047);
   gre->SetPoint(3,13,0.961353);
   gre->SetPointError(3,1,0.0008431042);
   gre->SetPoint(4,15,0.9677534);
   gre->SetPointError(4,1,0.0006938944);
   gre->SetPoint(5,17,0.9740622);
   gre->SetPointError(5,1,0.0005310168);
   gre->SetPoint(6,19,0.9787928);
   gre->SetPointError(6,1,0.0004455008);
   gre->SetPoint(7,21,0.982823);
   gre->SetPointError(7,1,0.0003764708);
   gre->SetPoint(8,23,0.9847131);
   gre->SetPointError(8,1,0.0003314238);
   gre->SetPoint(9,25,0.9868793);
   gre->SetPointError(9,1,0.0002954734);
   gre->SetPoint(10,27,0.9881812);
   gre->SetPointError(10,1,0.0002690398);
   gre->SetPoint(11,29,0.9892804);
   gre->SetPointError(11,1,0.0002479544);
   gre->SetPoint(12,31,0.9903388);
   gre->SetPointError(12,1,0.0002228088);
   gre->SetPoint(13,33,0.9913921);
   gre->SetPointError(13,1,0.0002037189);
   gre->SetPoint(14,35,0.9923295);
   gre->SetPointError(14,1,0.0001811096);
   gre->SetPoint(15,37,0.9926529);
   gre->SetPointError(15,1,0.0001704655);
   gre->SetPoint(16,39,0.993178);
   gre->SetPointError(16,1,0.0001574709);
   gre->SetPoint(17,41,0.9938738);
   gre->SetPointError(17,1,0.0001429644);
   gre->SetPoint(18,43,0.9943293);
   gre->SetPointError(18,1,0.0001368219);
   gre->SetPoint(19,45,0.9946256);
   gre->SetPointError(19,1,0.0001393718);
   gre->SetPoint(20,47,0.9948041);
   gre->SetPointError(20,1,0.00015654);
   gre->SetPoint(21,49,0.9955145);
   gre->SetPointError(21,1,0.0001760829);
   gre->SetPoint(22,51,0.9957955);
   gre->SetPointError(22,1,0.0002072097);
   gre->SetPoint(23,53,0.9953827);
   gre->SetPointError(23,1,0.0002483741);
   gre->SetPoint(24,55,0.996158);
   gre->SetPointError(24,1,0.0002665249);
   gre->SetPoint(25,57,0.9966463);
   gre->SetPointError(25,1,0.0002761455);
   gre->SetPoint(26,59,0.9966724);
   gre->SetPointError(26,1,0.0003257521);
   gre->SetPoint(27,61,0.9969477);
   gre->SetPointError(27,1,0.0003325274);
   gre->SetPoint(28,63,0.9962641);
   gre->SetPointError(28,1,0.0003909006);
   gre->SetPoint(29,65,0.9967587);
   gre->SetPointError(29,1,0.0004403692);
   gre->SetPoint(30,67,0.9963795);
   gre->SetPointError(30,1,0.0004704976);
   gre->SetPoint(31,69,0.997082);
   gre->SetPointError(31,1,0.0004790127);
   gre->SetPoint(32,71,0.9973983);
   gre->SetPointError(32,1,0.0005196682);
   gre->SetPoint(33,73,0.9978017);
   gre->SetPointError(33,1,0.0005175836);
   gre->SetPoint(34,75,0.9972321);
   gre->SetPointError(34,1,0.0006031672);
   gre->SetPoint(35,77,0.9966651);
   gre->SetPointError(35,1,0.000726525);
   gre->SetPoint(36,79,0.9981492);
   gre->SetPointError(36,1,0.0005847399);
   gre->SetPoint(37,81,0.9958602);
   gre->SetPointError(37,1,0.0008262361);
   gre->SetPoint(38,83,0.9974837);
   gre->SetPointError(38,1,0.0007254646);
   gre->SetPoint(39,85,0.9978182);
   gre->SetPointError(39,1,0.0007264789);
   gre->SetPoint(40,87,0.9993663);
   gre->SetPointError(40,1,0.0004479611);
   gre->SetPoint(41,89,0.9960924);
   gre->SetPointError(41,1,0.001175893);
   gre->SetPoint(42,91,0.997189);
   gre->SetPointError(42,1,0.0009924276);
   gre->SetPoint(43,93,0.9979863);
   gre->SetPointError(43,1,0.0008996438);
   gre->SetPoint(44,95,0.9955609);
   gre->SetPointError(44,1,0.001335454);
   gre->SetPoint(45,97,0.9974039);
   gre->SetPointError(45,1,0.001159483);
   gre->SetPoint(46,99,0.9978142);
   gre->SetPointError(46,1,0.001091701);
   gre->SetPoint(47,101,0.9989107);
   gre->SetPointError(47,1,0.0007698492);
   gre->SetPoint(48,103,0.9977986);
   gre->SetPointError(48,1,0.001099503);
   gre->SetPoint(49,105,0.9971264);
   gre->SetPointError(49,1,0.001656667);
   
   
   
      /*
      gre->SetPoint(0,7,0.913878);
      gre->SetPointError(0,1,0.006223568);
      gre->SetPoint(1,9,0.9393252);
      gre->SetPointError(1,1,0.004019846);
      gre->SetPoint(2,11,0.9584196);
      gre->SetPointError(2,1,0.002738235);
      gre->SetPoint(3,13,0.960792);
      gre->SetPointError(3,1,0.002215167);
      gre->SetPoint(4,15,0.9713596);
      gre->SetPointError(4,1,0.001708391);
      gre->SetPoint(5,17,0.9755486);
      gre->SetPointError(5,1,0.001345859);
      gre->SetPoint(6,19,0.9804558);
      gre->SetPointError(6,1,0.001115445);
      gre->SetPoint(7,21,0.9829812);
      gre->SetPointError(7,1,0.0009774501);
      gre->SetPoint(8,23,0.9845826);
      gre->SetPointError(8,1,0.0008674757);
      gre->SetPoint(9,25,0.9869452);
      gre->SetPointError(9,1,0.0007682372);
      gre->SetPoint(10,27,0.989163);
      gre->SetPointError(10,1,0.0006723221);
      gre->SetPoint(11,29,0.9898721);
      gre->SetPointError(11,1,0.0006261203);
      gre->SetPoint(12,31,0.989827);
      gre->SetPointError(12,1,0.0005933134);
      gre->SetPoint(13,33,0.9907978);
      gre->SetPointError(13,1,0.0005483777);
      gre->SetPoint(14,35,0.9928026);
      gre->SetPointError(14,1,0.0004572352);
      gre->SetPoint(15,37,0.9921757);
      gre->SetPointError(15,1,0.0004576579);
      gre->SetPoint(16,39,0.9930587);
      gre->SetPointError(16,1,0.0004133825);
      gre->SetPoint(17,41,0.9941408);
      gre->SetPointError(17,1,0.0003637102);
      gre->SetPoint(18,43,0.9946418);
      gre->SetPointError(18,1,0.000346388);
      gre->SetPoint(19,45,0.9939766);
      gre->SetPointError(19,1,0.0003836574);
      gre->SetPoint(20,47,0.9950535);
      gre->SetPointError(20,1,0.0003976149);
      gre->SetPoint(21,49,0.9949896);
      gre->SetPointError(21,1,0.0004854323);
      gre->SetPoint(22,51,0.9965991);
      gre->SetPointError(22,1,0.0004850143);
      gre->SetPoint(23,53,0.9950082);
      gre->SetPointError(23,1,0.0006714163);
      gre->SetPoint(24,55,0.9951005);
      gre->SetPointError(24,1,0.0007826232);
      gre->SetPoint(25,57,0.9970629);
      gre->SetPointError(25,1,0.0006728232);
      gre->SetPoint(26,59,0.9976278);
      gre->SetPointError(26,1,0.0007144034);
      gre->SetPoint(27,61,0.9965424);
      gre->SetPointError(27,1,0.0009224952);
      gre->SetPoint(28,63,0.9974867);
      gre->SetPointError(28,1,0.0008367014);
      gre->SetPoint(29,65,0.9979839);
      gre->SetPointError(29,1,0.0009007309);
      gre->SetPoint(30,67,0.9995833);
      gre->SetPointError(30,1,0.0004165799);
      gre->SetPoint(31,69,0.9962121);
      gre->SetPointError(31,1,0.00142897);
      gre->SetPoint(32,71,0.9978814);
      gre->SetPointError(32,1,0.001221903);
      gre->SetPoint(33,73,0.9966887);
      gre->SetPointError(33,1,0.001652886);
      gre->SetPoint(34,75,0.9973404);
      gre->SetPointError(34,1,0.001533463);
      gre->SetPoint(35,77,0.9978402);
      gre->SetPointError(35,1,0.001525578);
      gre->SetPoint(36,79,0.9987562);
      gre->SetPointError(36,1,0.001243007);
      gre->SetPoint(37,81,0.9976905);
      gre->SetPointError(37,1,0.001631154);
      gre->SetPoint(38,83,0.9985632);
      gre->SetPointError(38,1,0.001435749);
      gre->SetPoint(39,85,0.998366);
      gre->SetPointError(39,1,0.001632651);
      gre->SetPoint(40,87,0.9978495);
      gre->SetPointError(40,1,0.002148224);
      gre->SetPoint(41,89,1);
      gre->SetPointError(41,1,0);
      gre->SetPoint(42,91,1);
      gre->SetPointError(42,1,0);
      gre->SetPoint(43,93,1);
      gre->SetPointError(43,1,0);
      gre->SetPoint(44,95,0.9945799);
      gre->SetPointError(44,1,0.003822157);
      gre->SetPoint(45,97,0.9965398);
      gre->SetPointError(45,1,0.003454216);
      gre->SetPoint(46,99,1);
      gre->SetPointError(46,1,0);
      gre->SetPoint(47,101,1);
      gre->SetPointError(47,1,0);
      gre->SetPoint(48,103,0.9961977);
      gre->SetPointError(48,1,0.003795046);
      gre->SetPoint(49,105,1);
      gre->SetPointError(49,1,0);
      */

      graph[1] = gre;

      gre = new TGraphErrors(50);
   gre->SetPoint(0,7,0.8548118);
   gre->SetPointError(0,1,0.003392116);
   gre->SetPoint(1,9,0.9099109);
   gre->SetPointError(1,1,0.002123316);
   gre->SetPoint(2,11,0.9265792);
   gre->SetPointError(2,1,0.001996572);
   gre->SetPoint(3,13,0.9394687);
   gre->SetPointError(3,1,0.001178374);
   gre->SetPoint(4,15,0.9536041);
   gre->SetPointError(4,1,0.001128097);
   gre->SetPoint(5,17,0.9606448);
   gre->SetPointError(5,1,0.0009766097);
   gre->SetPoint(6,19,0.9662289);
   gre->SetPointError(6,1,0.0008500605);
   gre->SetPoint(7,21,0.9734445);
   gre->SetPointError(7,1,0.0006438425);
   gre->SetPoint(8,23,0.9779689);
   gre->SetPointError(8,1,0.0005126724);
   gre->SetPoint(9,25,0.9799031);
   gre->SetPointError(9,1,0.0004919952);
   gre->SetPoint(10,27,0.9826706);
   gre->SetPointError(10,1,0.0004384624);
   gre->SetPoint(11,29,0.9859669);
   gre->SetPointError(11,1,0.0003730757);
   gre->SetPoint(12,31,0.9877018);
   gre->SetPointError(12,1,0.000329612);
   gre->SetPoint(13,33,0.9884872);
   gre->SetPointError(13,1,0.0003296042);
   gre->SetPoint(14,35,0.9899241);
   gre->SetPointError(14,1,0.0002686974);
   gre->SetPoint(15,37,0.9906304);
   gre->SetPointError(15,1,0.0002888963);
   gre->SetPoint(16,39,0.9913658);
   gre->SetPointError(16,1,0.0002566515);
   gre->SetPoint(17,41,0.9919796);
   gre->SetPointError(17,1,0.0002376333);
   gre->SetPoint(18,43,0.992843);
   gre->SetPointError(18,1,0.0002025985);
   gre->SetPoint(19,45,0.9932541);
   gre->SetPointError(19,1,0.0002181253);
   gre->SetPoint(20,47,0.9937462);
   gre->SetPointError(20,1,0.000234131);
   gre->SetPoint(21,49,0.9938962);
   gre->SetPointError(21,1,0.0003061758);
   gre->SetPoint(22,51,0.9946181);
   gre->SetPointError(22,1,0.0003028983);
   gre->SetPoint(23,53,0.9944963);
   gre->SetPointError(23,1,0.0003234176);
   gre->SetPoint(24,55,0.9946809);
   gre->SetPointError(24,1,0.000398747);
   gre->SetPoint(25,57,0.9954531);
   gre->SetPointError(25,1,0.0004267645);
   gre->SetPoint(26,59,0.9953187);
   gre->SetPointError(26,1,0.0006029348);
   gre->SetPoint(27,61,0.9952099);
   gre->SetPointError(27,1,0.0006020545);
   gre->SetPoint(28,63,0.9954454);
   gre->SetPointError(28,1,0.0006929868);
   gre->SetPoint(29,65,0.9955296);
   gre->SetPointError(29,1,0.0006185494);
   gre->SetPoint(30,67,0.9961493);
   gre->SetPointError(30,1,0.000640543);
   gre->SetPoint(31,69,0.9961378);
   gre->SetPointError(31,1,0.001112779);
   gre->SetPoint(32,71,0.9979354);
   gre->SetPointError(32,1,0.0006218467);
   gre->SetPoint(33,73,0.9959897);
   gre->SetPointError(33,1,0.00106965);
   gre->SetPoint(34,75,0.99647);
   gre->SetPointError(34,1,0.00133186);
   gre->SetPoint(35,77,0.9969611);
   gre->SetPointError(35,1,0.0007834469);
   gre->SetPoint(36,79,0.9979054);
   gre->SetPointError(36,1,0.0007908375);
   gre->SetPoint(37,81,0.9977044);
   gre->SetPointError(37,1,0.0008106683);
   gre->SetPoint(38,83,0.9974694);
   gre->SetPointError(38,1,0.001031796);
   gre->SetPoint(39,85,0);
   gre->SetPointError(39,1,0);
   gre->SetPoint(40,87,0.998199);
   gre->SetPointError(40,1,0.000899684);
   gre->SetPoint(41,89,0.9972924);
   gre->SetPointError(41,1,0.001561105);
   gre->SetPoint(42,91,0.9985876);
   gre->SetPointError(42,1,0.0009980328);
   gre->SetPoint(43,93,0.9976077);
   gre->SetPointError(43,1,0.001379568);
   gre->SetPoint(44,95,0.9982533);
   gre->SetPointError(44,1,0.001234042);
   gre->SetPoint(45,97,0.9960057);
   gre->SetPointError(45,1,0.001065386);
   gre->SetPoint(46,99,0);
   gre->SetPointError(46,1,0);
   gre->SetPoint(47,101,0.9994496);
   gre->SetPointError(47,1,0.0005502063);
   gre->SetPoint(48,103,0);
   gre->SetPointError(48,1,0);
   gre->SetPoint(49,105,0);
   gre->SetPointError(49,1,0);

      /*
      gre->SetPoint(0,7,0.8820755);
      gre->SetPointError(0,0,0.006143493);
      gre->SetPoint(1,9,0.923298);
      gre->SetPointError(1,0,0.003956517);
      gre->SetPoint(2,11,0.9475546);
      gre->SetPointError(2,0,0.003215948);
      gre->SetPoint(3,13,0.945274);
      gre->SetPointError(3,0,0.001980176);
      gre->SetPoint(4,15,0.9582634);
      gre->SetPointError(4,0,0.001986501);
      gre->SetPoint(5,17,0.9709139);
      gre->SetPointError(5,0,0.001627779);
      gre->SetPoint(6,19,0.9700381);
      gre->SetPointError(6,0,0.001550993);
      gre->SetPoint(7,21,0.9772776);
      gre->SetPointError(7,0,0.001089605);
      gre->SetPoint(8,23,0.9815929);
      gre->SetPointError(8,0,0.000907322);
      gre->SetPoint(9,25,0.9811615);
      gre->SetPointError(9,0,0.001014981);
      gre->SetPoint(10,27,0.9857008);
      gre->SetPointError(10,0,0.0007768017);
      gre->SetPoint(11,29,0.98699);
      gre->SetPointError(11,0,0.0006948554);
      gre->SetPoint(12,31,0.9882788);
      gre->SetPointError(12,0,0.0005923216);
      gre->SetPoint(13,33,0.9897012);
      gre->SetPointError(13,0,0.0006200924);
      gre->SetPoint(14,35,0.9908547);
      gre->SetPointError(14,0,0.0004751917);
      gre->SetPoint(15,37,0.9908223);
      gre->SetPointError(15,0,0.0005529051);
      gre->SetPoint(16,39,0.9906819);
      gre->SetPointError(16,0,0.0005542633);
      gre->SetPoint(17,41,0.9928415);
      gre->SetPointError(17,0,0.0004118164);
      gre->SetPoint(18,43,0.9935343);
      gre->SetPointError(18,0,0.000350548);
      gre->SetPoint(19,45,0.9937025);
      gre->SetPointError(19,0,0.0004251743);
      gre->SetPoint(20,47,0.994493);
      gre->SetPointError(20,0,0.0004212003);
      gre->SetPoint(21,49,0.9925571);
      gre->SetPointError(21,0,0.0006741088);
      gre->SetPoint(22,51,0.9944265);
      gre->SetPointError(22,0,0.0005503153);
      gre->SetPoint(23,53,0.9946905);
      gre->SetPointError(23,0,0.0005847781);
      gre->SetPoint(24,55,0.9949223);
      gre->SetPointError(24,0,0.0006282136);
      gre->SetPoint(25,57,0.9939523);
      gre->SetPointError(25,0,0.0008204967);
      gre->SetPoint(26,59,0.9961249);
      gre->SetPointError(26,0,0.0009116065);
      gre->SetPoint(27,61,0.9964867);
      gre->SetPointError(27,0,0.001057424);
      gre->SetPoint(28,63,0.9950719);
      gre->SetPointError(28,0,0.001419119);
      gre->SetPoint(29,65,0.996209);
      gre->SetPointError(29,0,0.0007889785);
      gre->SetPoint(30,67,0.9931357);
      gre->SetPointError(30,0,0.001458437);
      gre->SetPoint(31,69,0.9961905);
      gre->SetPointError(31,0,0.00190113);
      gre->SetPoint(32,71,0.9976668);
      gre->SetPointError(32,0,0.001042211);
      gre->SetPoint(33,73,0.9947313);
      gre->SetPointError(33,0,0.002350021);
      gre->SetPoint(34,75,1);
      gre->SetPointError(34,0,0);
      gre->SetPoint(35,77,0.9978495);
      gre->SetPointError(35,0,0.001519024);
      gre->SetPoint(36,79,0.9992045);
      gre->SetPointError(36,0,0.0007952284);
      gre->SetPoint(37,81,1);
      gre->SetPointError(37,0,0);
      gre->SetPoint(38,83,1);
      gre->SetPointError(38,0,0);
      gre->SetPoint(39,85,0);
      gre->SetPointError(39,0,0);
      gre->SetPoint(40,87,0.9934924);
      gre->SetPointError(40,0,0.003744915);
      gre->SetPoint(41,89,1);
      gre->SetPointError(41,0,0);
      gre->SetPoint(42,91,0.9962264);
      gre->SetPointError(42,0,0.003766458);
      gre->SetPoint(43,93,0.9956236);
      gre->SetPointError(43,0,0.00308778);
      gre->SetPoint(44,95,0.9962264);
      gre->SetPointError(44,0,0.003766458);
      gre->SetPoint(45,97,0.9973873);
      gre->SetPointError(45,0,0.001304628);
      gre->SetPoint(46,99,0);
      gre->SetPointError(46,0,0);
      gre->SetPoint(47,101,1);
      gre->SetPointError(47,0,0);
      gre->SetPoint(48,103,0);
      gre->SetPointError(48,0,0);
      gre->SetPoint(49,105,0);
      gre->SetPointError(49,0,0);
      */

      graph[0] = gre;

    }
  else 
    return 0;

  
  //  int ci = kOrange+7;
  //  int ci = kOrange+7;
  int ci = kTeal+10;
  int fillColor[2]   = { 0, ci };
  int fillStyle[2]   = { 0, 1001 }; 
  //  int lineWidth[2]   = { 2, 0 }; 
  int lineColor[2]   = { kBlack, ci }; 
  //  int lineStyle[2]   = { kSolid, 0 }; 
  //  int markerStyle[2] = { kOpenCircle, 0 };
  //  int markerColor[2] = { kBlack, ci };

  for( size_t ii=0; ii<2; ii++ )
    {
      graph[ii]->SetFillColor(fillColor[ii]);
      graph[ii]->SetFillStyle(fillStyle[ii]);
      graph[ii]->SetLineColor(lineColor[ii]);
      graph[ii]->SetMarkerSize(0);
      //      graph[ii]->SetMarkerColor(markerColor[ii]);
      //      graph[ii]->SetMarkerStyle(markerStyle[ii]);
    }


  // text2
  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS";
  txtSize[0] = 0.055;
  txtX[0] = 0.93;
  txtY[0] = 0.95;
  txtAlign[0] = 31;
  txtFont[0] = 42;
  
  txt[1] = "#gamma*/Z #rightarrow ";
  txt[1] += "ee";
  txtSize[1] = 0.06;
  txtX[1] = 0.20;
  txtY[1] = 0.85;
  txtAlign[1] = 11;
  txtFont[1] = 42;

  // canvas
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=250;

  // histogram limits, in linear and logarithmic
  int nbin_(100);
  float xmin_(0.), xmax_(0.); 
  float ymin_(0.), ymax_(1.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(506);
  int ndivy(506);

  // canvas name
  TString ctitle;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  float xl_(0.35);
  float yl_(0.3);
  float dxl_(0.2);
  float dyl_(0.1);

  // root file, where the data is
  TString cname;
  nbin_ = 100;
  if( mode == Fig_4_a )
    {    
      cname += "Fig_4_a";
      ctitle = "Reco (+ID) selection efficiency versus eta, mu-mu analysis";
      xtitle = "#mu pseudorapidity, #eta";	        
      ytitle = "#mu selection efficiency (reco+ID)"; 
      xmin_ = -2.5;
      xmax_ =  2.5;
      ymin_ = 0.86;
      ymax_ = 1.02;
      xl_=0.65;
      yl_=0.2;
    }
  else if( mode == Fig_4_b )
    {      
      cname += "Fig_4_b";
      ctitle = "Single muon trigger efficiency";
      //xtitle = "#mu transverse momentum, p#lower[0.3]{#scale[0.6]{T}}  [";	        
      xtitle = "#mu transverse momentum, p_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "single-#mu trigger efficiency"; 
      xmin_ =   16;
      xmax_ = 100;
      ymin_ = 0.65;
      ymax_ = 1.05;
    }
  else if( mode == Fig_5 )
    {    
      cname += "Fig_5";
      ctitle = "isolation efficiency versus pT, mu-mu analysis";
      xtitle = "#mu transverse momentum, p_{T}  [";	        
      //xtitle = "#mu transverse momentum, p#lower[0.3]#scale[0.6]{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "#mu isolation efficiency"; 
      xmin_ =   0;
      xmax_ = 100;
      ymin_ = 0.84;
      ymax_ = 1.025;
      //      ndivy = 503;
    }
  else
    return 0;

  //  size_t nChan=channels.size();
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,
			  xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin(   60./dxcanv_ );
  c_->SetRightMargin(  30./dxcanv_ );
  c_->SetTopMargin(    30./dxcanv_ );
  c_->SetBottomMargin( 60./dxcanv_ ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);
   
  float titleOffsetY(1.2);
  float titleSizeY(xcanv_*0.060/ycanv_);
  float labelOffsetY(0.01);
  float titleOffsetX(1.0);
  float titleSizeX(0.060);
  float labelOffsetX(0.01);
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon3", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  ax_->SetTitleOffset(titleOffsetX);
  ax_->SetTitleSize(titleSizeX);
  //  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(labelOffsetX);

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetLabelOffset(labelOffsetY);

  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

  c_->SetLogy(false);
  c_->SetLogx(false);
  h_->GetYaxis()->SetRangeUser(ymin_,ymax_);

  h_->Draw("hist");

  bool drawHGrid = true;
  if( drawHGrid )
    {
      TLine grid_;
      grid_.SetLineColor(kGray+2);
      grid_.SetLineWidth(1);
      float y_;
      grid_.SetLineStyle(kSolid);
      y_=1.0;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      grid_.SetLineStyle(kDotted);
      //      y_=0.9;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      y_=0.8;  grid_.DrawLine(xmin_,y_,xmax_,y_);
    }

  int markerSize = 1.0;
  graph[1]->Draw("e2");
  graph[0]->DrawClone("p");
  c_->RedrawAxis();

  graph[0]->SetMarkerSize( markerSize );
  graph[0]->SetMarkerColor( kWhite );
  graph[0]->SetMarkerStyle( kFullCircle );
  graph[0]->DrawClone("PE");
  graph[0]->SetMarkerColor( kBlack );
  graph[0]->SetMarkerStyle( kOpenCircle );
  graph[0]->DrawClone("PE");

  // legend position and scale;
  int legendTextFont=42;
  float legendTextSize=0.040;

  TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
  legend->SetTextFont(legendTextFont);
  legend->SetTextSize(legendTextSize);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  
  TGraph* dummyGraph = (TGraph*) graph[0]->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);
  legend->AddEntry(dummyGraph," data","PL");
  legend->AddEntry(graph[1]," DY simulation","F"); 

  legend->Draw("Same");

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  c_->GetFrame()->Draw();

  c_->cd();
  return c_;
}

TCanvas* Efficiency_Electron( int mode )
{
  TGraph* graph[2];
  TGraphAsymmErrors* grae;
  TGraphErrors* gre;
  //  TGraphErrors* gre  = new TGraphErrors(14);
  if( mode==Fig_10_a )
    {
      // barrel
      // les donnees
      grae = new TGraphAsymmErrors(5);
      grae->SetName("Graph_pt_data");
      grae->SetTitle("RECO efficiency vs pt");
      grae->SetPoint(0,15,100*0.01);
      grae->SetPointError(0,5,5,9.5*0.01,0*0.01);
      grae->SetPoint(1,25,98.7*0.01);
      grae->SetPointError(1,5,5,1.4*0.01,1.3*0.01);
      grae->SetPoint(2,35,98.5*0.01);
      grae->SetPointError(2,5,5,0.3*0.01,0.3*0.01);
      grae->SetPoint(3,45,98.9*0.01);
      grae->SetPointError(3,5,5,0.2*0.01,0.2*0.01);
      grae->SetPoint(4,275,99.4*0.01);
      grae->SetPointError(4,225,225,0.3*0.01,0.3*0.01);

      for( int ii=0; ii<grae->GetN(); ii++ )
	{
	  grae->SetPointEXlow(ii,0);
	  grae->SetPointEXhigh(ii,0);
	}
      graph[0] = grae;
   
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_mc");
      gre->SetTitle("RECO efficiency vs pt");
      gre->SetPoint(0,15,89*0.01);
      gre->SetPointError(0,5,0.2*0.01);
      gre->SetPoint(1,25,95.4*0.01);
      gre->SetPointError(1,5,0.1*0.01);
      gre->SetPoint(2,35,97.7*0.01);
      gre->SetPointError(2,5,0*0.01);
      gre->SetPoint(3,45,98.3*0.01);
      gre->SetPointError(3,5,0*0.01);
      gre->SetPoint(4,275,98.7*0.01);
      gre->SetPointError(4,225,0*0.01);
      
      graph[1] = gre;

    }
  else if( mode==Fig_10_b )
    {
      // endcap
      // les donnees
      grae = new TGraphAsymmErrors(5);
      grae->SetName("Graph_pt_data");
      grae->SetTitle("RECO efficiency vs pt");
      grae->SetPoint(0,15,97.4*0.01);
      grae->SetPointError(0,5,5,4.8*0.01,2.6*0.01);
      grae->SetPoint(1,25,96.6*0.01);
      grae->SetPointError(1,5,5,1*0.01,1*0.01);
      grae->SetPoint(2,35,98.2*0.01);
      grae->SetPointError(2,5,5,0.4*0.01,0.4*0.01);
      grae->SetPoint(3,45,98.4*0.01);
      grae->SetPointError(3,5,5,0.4*0.01,0.4*0.01);
      grae->SetPoint(4,275,98.4*0.01);
      grae->SetPointError(4,225,225,0.9*0.01,0.9*0.01);
      
      for( int ii=0; ii<grae->GetN(); ii++ )
	{
	  grae->SetPointEXlow(ii,0);
	  grae->SetPointEXhigh(ii,0);
	}
      graph[0] = grae;
   
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_mc");
      gre->SetTitle("RECO efficiency vs pt");
      gre->SetPoint(0,15,88.5*0.01);
      gre->SetPointError(0,5,0.2*0.01);
      gre->SetPoint(1,25,92.2*0.01);
      gre->SetPointError(1,5,0.1*0.01);
      gre->SetPoint(2,35,94.3*0.01);
      gre->SetPointError(2,5,0.1*0.01);
      gre->SetPoint(3,45,95.6*0.01);
      gre->SetPointError(3,5,0.1*0.01);
      gre->SetPoint(4,275,96.2*0.01);
      gre->SetPointError(4,225,0.1*0.01);
            
      graph[1] = gre;

    }
  else if( mode==Fig_10_c )
    {
      // barrel
      // les donnees
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_data");
      gre->SetTitle("trigger efficiency vs pt");
      gre->SetPoint(0,15,57*0.01);
      gre->SetPointError(0,5,2.4*0.01);
      gre->SetPoint(1,25,96.7*0.01);
      gre->SetPointError(1,5,0.4*0.01);
      gre->SetPoint(2,35,97.6*0.01);
      gre->SetPointError(2,5,0.2*0.01);
      gre->SetPoint(3,45,98.2*0.01);
      gre->SetPointError(3,5,0.2*0.01);
      gre->SetPoint(4,275,98.5*0.01);
      gre->SetPointError(4,225,0.3*0.01);
      
      /*
      for( int ii=0; ii<gre->GetN(); ii++ )
	{
	  gre->SetPointEXlow(ii,0);
	  gre->SetPointEXhigh(ii,0);
	}
	*/
      graph[0] = gre;
   
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_mc");
      gre->SetTitle("trigger efficiency vs pt");
      gre->SetPoint(0,15,53.8*0.01);
      gre->SetPointError(0,5,0.5*0.01);
      gre->SetPoint(1,25,97.5*0.01);
      gre->SetPointError(1,5,0.1*0.01);
      gre->SetPoint(2,35,97.3*0.01);
      gre->SetPointError(2,5,0*0.01);
      gre->SetPoint(3,45,97*0.01);
      gre->SetPointError(3,5,0*0.01);
      gre->SetPoint(4,275,96.9*0.01);
      gre->SetPointError(4,225,0.1*0.01);
            
      graph[1] = gre;

    }
  else if( mode==Fig_10_d )
    {
      // endcap
      // les donnees
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_data");
      gre->SetTitle("trigger efficiency vs pt");
      gre->SetPoint(0,15,54.4*0.01);
      gre->SetPointError(0,5,3.2*0.01);
      gre->SetPoint(1,25,94.5*0.01);
      gre->SetPointError(1,5,0.7*0.01);
      gre->SetPoint(2,35,97.3*0.01);
      gre->SetPointError(2,5,0.4*0.01);
      gre->SetPoint(3,45,97.5*0.01);
      gre->SetPointError(3,5,0.4*0.01);
      gre->SetPoint(4,275,97.2*0.01);
      gre->SetPointError(4,225,0.8*0.01);
      
      /*
      for( int ii=0; ii<gre->GetN(); ii++ )
	{
	  gre->SetPointEXlow(ii,0);
	  gre->SetPointEXhigh(ii,0);
	}
	*/
      graph[0] = gre;
   
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_mc");
      gre->SetTitle("trigger efficiency vs pt");
      gre->SetPoint(0,15,52.9*0.01);
      gre->SetPointError(0,5,0.6*0.01);
      gre->SetPoint(1,25,98.2*0.01);
      gre->SetPointError(1,5,0.1*0.01);
      gre->SetPoint(2,35,98.1*0.01);
      gre->SetPointError(2,5,0.1*0.01);
      gre->SetPoint(3,45,98.2*0.01);
      gre->SetPointError(3,5,0.1*0.01);
      gre->SetPoint(4,275,98*0.01);
      gre->SetPointError(4,225,0.1*0.01);
            
      graph[1] = gre;

    }
  else if( mode==Fig_10_e )
    {
      // barrel
      // les donnees
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_data");
      gre->SetTitle("ID efficiency vs pt");
      gre->SetPoint(0,15,63.7*0.01);
      gre->SetPointError(0,5,4.5*0.01);
      gre->SetPoint(1,25,76*0.01);
      gre->SetPointError(1,5,1.2*0.01);
      gre->SetPoint(2,35,83.7*0.01);
      gre->SetPointError(2,5,0.5*0.01);
      gre->SetPoint(3,45,87.9*0.01);
      gre->SetPointError(3,5,0.4*0.01);
      gre->SetPoint(4,275,86.8*0.01);
      gre->SetPointError(4,225,0.8*0.01);
      
      graph[0] = gre;
   
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_mc");
      gre->SetTitle("ID efficiency vs pt");
      gre->SetPoint(0,15,64.1*0.01);
      gre->SetPointError(0,5,0.4*0.01);
      gre->SetPoint(1,25,79.2*0.01);
      gre->SetPointError(1,5,0.1*0.01);
      gre->SetPoint(2,35,86.6*0.01);
      gre->SetPointError(2,5,0.1*0.01);
      gre->SetPoint(3,45,91.2*0.01);
      gre->SetPointError(3,5,0.1*0.01);
      gre->SetPoint(4,275,92*0.01);
      gre->SetPointError(4,225,0.1*0.01);
            
      graph[1] = gre;

    }
  else if( mode==Fig_10_f )
    {
      // endcap
      // les donnees
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_data");
      gre->SetTitle("ID efficiency vs pt");
      gre->SetPoint(0,15,54.2*0.01);
      gre->SetPointError(0,5,4.1*0.01);
      gre->SetPoint(1,25,66.3*0.01);
      gre->SetPointError(1,5,1.5*0.01);
      gre->SetPoint(2,35,73*0.01);
      gre->SetPointError(2,5,0.9*0.01);
      gre->SetPoint(3,45,79.5*0.01);
      gre->SetPointError(3,5,0.9*0.01);
      gre->SetPoint(4,275,81.2*0.01);
      gre->SetPointError(4,225,1.9*0.01);
      
      graph[0] = gre;
   
      gre = new TGraphErrors(5);
      gre->SetName("Graph_pt_mc");
      gre->SetTitle("ID efficiency vs pt");
      gre->SetPoint(0,15,49.8*0.01);
      gre->SetPointError(0,5,0.4*0.01);
      gre->SetPoint(1,25,66.8*0.01);
      gre->SetPointError(1,5,0.2*0.01);
      gre->SetPoint(2,35,74.7*0.01);
      gre->SetPointError(2,5,0.2*0.01);
      gre->SetPoint(3,45,80.7*0.01);
      gre->SetPointError(3,5,0.1*0.01);
      gre->SetPoint(4,275,82.8*0.01);
      gre->SetPointError(4,225,0.3*0.01);
            
      graph[1] = gre;

    }
  else 
    return 0;

  
  //  int ci = kOrange+7;
  //  int ci = kOrange+7;
  int ci = kTeal+10;
  int fillColor[2]   = { 0, ci };
  int fillStyle[2]   = { 0, 1001 }; 
  //  int lineWidth[2]   = { 2, 0 }; 
  int lineColor[2]   = { kBlack, ci }; 
  //  int lineStyle[2]   = { kSolid, 0 }; 
  //  int markerStyle[2] = { kOpenCircle, 0 };
  //  int markerColor[2] = { kBlack, ci };

  for( size_t ii=0; ii<2; ii++ )
    {
      graph[ii]->SetFillColor(fillColor[ii]);
      graph[ii]->SetFillStyle(fillStyle[ii]);
      graph[ii]->SetLineColor(lineColor[ii]);
      graph[ii]->SetMarkerSize(0);
      //      graph[ii]->SetMarkerColor(markerColor[ii]);
      //      graph[ii]->SetMarkerStyle(markerStyle[ii]);
    }


  // text2
  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS";
  txtSize[0] = 0.055;
  txtX[0] = 0.93;
  txtY[0] = 0.95;
  txtAlign[0] = 31;
  txtFont[0] = 42;
  
  txt[1] = "#gamma*/Z #rightarrow ";
  txt[1] += "ee";
  txtSize[1] = 0.06;
  txtX[1] = 0.20;
  txtY[1] = 0.85;
  txtAlign[1] = 11;
  txtFont[1] = 42;

  // canvas
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=250;

  // histogram limits, in linear and logarithmic
  int nbin_(100);
  float xmin_(0.), xmax_(0.); 
  float ymin_(0.), ymax_(1.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(506);
  int ndivy(506);

  // canvas name
  TString ctitle;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  float xl_(0.35);
  float yl_(0.3);
  float dxl_(0.2);
  float dyl_(0.1);

  // root file, where the data is
  TString cname;
  nbin_ = 100;
  if( mode == Fig_10_a )
    {    
      cname += "Fig_10_a";
      ctitle = "Reco efficiency versus p_{T}, e-e analysis";
      xtitle = "electron E_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "e RECO efficiency (barrel)"; 
      xmin_ = 10;
      xmax_ = 500;
      ymin_ = 0.85;
      ymax_ = 1.02;
      xl_=0.65;
      yl_=0.2;
    }
  else if( mode == Fig_10_b )
    {    
      cname += "Fig_10_b";
      ctitle = "Reco efficiency versus p_{T}, e-e analysis";
      xtitle = "electron E_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "e RECO efficiency (endcap)"; 
      xmin_ = 10;
      xmax_ = 500;
      ymin_ = 0.85;
      ymax_ = 1.02;
      xl_=0.65;
      yl_=0.2;
    }
  else if( mode == Fig_10_c )
    {    
      cname += "Fig_10_c";
      ctitle = "trigger efficiency versus p_{T}, e-e analysis";
      xtitle = "electron E_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "e trigger efficiency (barrel)"; 
      xmin_ = 20;
      xmax_ = 500;
      ymin_ = 0.85;
      ymax_ = 1.02;
      xl_=0.65;
      yl_=0.2;
    }
  else if( mode == Fig_10_d )
    {    
      cname += "Fig_10_d";
      ctitle = "trigger efficiency versus p_{T}, e-e analysis";
      xtitle = "electron E_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "e trigger efficiency (endcap)"; 
      xmin_ = 20;
      xmax_ = 500;
      ymin_ = 0.85;
      ymax_ = 1.02;
      xl_=0.65;
      yl_=0.2;
    }
  else if( mode == Fig_10_e )
    {    
      cname += "Fig_10_e";
      ctitle = "ID efficiency versus p_{T}, e-e analysis";
      xtitle = "electron E_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "e ID efficiency (barrel)"; 
      xmin_ = 10;
      xmax_ = 500;
      ymin_ = 0.5;
      ymax_ = 1.07;
      xl_=0.65;
      yl_=0.2;
    }
  else if( mode == Fig_10_f )
    {    
      cname += "Fig_10_f";
      ctitle = "ID efficiency versus p_{T}, e-e analysis";
      xtitle = "electron E_{T}  [";	        
      xtitle += momUnit;       
      xtitle += "]";
      ytitle = "e ID efficiency (endcap)"; 
      xmin_ = 10;
      xmax_ = 500;
      ymin_ = 0.4;
      ymax_ = 1.09;
      xl_=0.65;
      yl_=0.2;
    }
  else
    return 0;

  //  size_t nChan=channels.size();
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,
			  xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin(   60./dxcanv_ );
  c_->SetRightMargin(  30./dxcanv_ );
  c_->SetTopMargin(    30./dxcanv_ );
  c_->SetBottomMargin( 60./dxcanv_ ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);
   
  float titleOffsetY(1.2);
  float titleSizeY(xcanv_*0.060/ycanv_);
  float labelOffsetY(0.01);
  float titleOffsetX(1.0);
  float titleSizeX(0.060);
  float labelOffsetX(0.01);
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon3", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  ax_->SetTitleOffset(titleOffsetX);
  ax_->SetTitleSize(titleSizeX);
  //  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(labelOffsetX);
  ax_->SetMoreLogLabels();
  ax_->SetNoExponent();

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetLabelOffset(labelOffsetY);

  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

  c_->SetLogy(false);
  c_->SetLogx(true);
  h_->GetYaxis()->SetRangeUser(ymin_,ymax_);

  h_->Draw("hist");

  bool drawHGrid = true;
  if( drawHGrid )
    {
      TLine grid_;
      grid_.SetLineColor(kGray+2);
      grid_.SetLineWidth(1);
      float y_;
      grid_.SetLineStyle(kSolid);
      y_=1.0;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      grid_.SetLineStyle(kDotted);
      //      y_=0.9;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      y_=0.8;  grid_.DrawLine(xmin_,y_,xmax_,y_);
    }

  int markerSize = 1.0;
  graph[1]->Draw("e2");
  graph[0]->DrawClone("p");
  c_->RedrawAxis();

  graph[0]->SetMarkerSize( markerSize );
  graph[0]->SetMarkerColor( kWhite );
  graph[0]->SetMarkerStyle( kFullCircle );
  graph[0]->DrawClone("PE");
  graph[0]->SetMarkerColor( kBlack );
  graph[0]->SetMarkerStyle( kOpenCircle );
  graph[0]->DrawClone("PE");

  // legend position and scale;
  int legendTextFont=42;
  float legendTextSize=0.040;

  TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
  legend->SetTextFont(legendTextFont);
  legend->SetTextSize(legendTextSize);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  
  TGraph* dummyGraph = (TGraph*) graph[0]->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);
  legend->AddEntry(dummyGraph," data","PL");
  legend->AddEntry(graph[1]," DY simulation","F"); 

  legend->Draw("Same");

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  c_->GetFrame()->Draw();

  c_->cd();
  return c_;
}

//#include "FSR.C"
TCanvas* FSR( int mode )
{
  TH1* histos[4];
  TH1* data;
  if( mode==Fig_7_a )
    {
      TH1 *Eflow_pf_fsrMatched_ltp3_zmumu20 = new TH1F("Eflow_pf_fsrMatched_ltp3_zmumu20","Sum PF Photon E in cone",25,0,50);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(2,97.30183);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(3,85.80401);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(4,64.99096);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(5,49.04138);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(6,38.33727);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(7,32.39243);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(8,26.66355);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(9,21.26736);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(10,18.84127);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(11,16.90131);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(12,14.24165);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(13,11.00809);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(14,9.804636);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(15,8.203126);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(16,7.915201);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(17,6.894815);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(18,5.544249);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(19,4.936789);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(20,3.909352);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(21,3.289933);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(22,2.593741);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(23,3.126428);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(24,2.146547);
   Eflow_pf_fsrMatched_ltp3_zmumu20->SetBinContent(25,25.31397);
      histos[0] = Eflow_pf_fsrMatched_ltp3_zmumu20;

      TH1 *Eflow_pf_partfsrMatched_ltp3_zmumu20 = new TH1F("Eflow_pf_partfsrMatched_ltp3_zmumu20","Sum PF Photon E in cone",25,0,50);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(2,2.561743);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(3,2.287983);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(4,1.346549);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(5,1.388265);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(6,1.263328);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(7,0.8074449);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(8,0.7273118);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(9,0.4307563);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(10,0.4873671);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(11,0.2971912);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(12,0.2212057);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(13,0.2208425);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(14,0.1158552);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(15,0.2149695);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(16,0.2031024);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(17,0.1228786);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(18,0.1962607);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(19,0.04362357);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(20,0.06796316);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(21,0.06796316);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(22,0.1224548);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(23,0.02433959);
   Eflow_pf_partfsrMatched_ltp3_zmumu20->SetBinContent(25,0.4265181);
      histos[1] = Eflow_pf_partfsrMatched_ltp3_zmumu20;

      TH1 *Eflow_pf_phoMatched_ltp3_zmumu20 = new TH1F("Eflow_pf_phoMatched_ltp3_zmumu20","Sum PF Photon E in cone",25,0,50);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(2,33.31425);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(3,12.32097);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(4,7.156163);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(5,5.132745);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(6,3.498696);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(7,2.64896);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(8,1.340494);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(9,0.8847625);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(10,0.6683096);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(11,0.4652071);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(12,0.3614005);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(13,0.3371821);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(14,0.1399829);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(15,0.1640198);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(16,0.1031708);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(17,0.2313169);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(18,0.05491545);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(19,0.09324122);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(20,0.05491545);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(21,0.1357144);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(22,0.02433959);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(23,0.03057585);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(24,0.03057585);
   Eflow_pf_phoMatched_ltp3_zmumu20->SetBinContent(25,0.3348511);
      histos[2] = Eflow_pf_phoMatched_ltp3_zmumu20;
      
      TH1 *Eflow_pf_unmatched_ltp3_zmumu20 = new TH1F("Eflow_pf_unmatched_ltp3_zmumu20","Sum PF Photon E in cone",25,0,50);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(2,29.22158);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(3,9.635251);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(4,5.456939);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(5,3.833064);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(6,2.819791);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(7,1.940023);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(8,0.914188);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(9,0.7581602);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(10,0.3498362);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(11,0.3468089);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(12,0.2521449);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(13,0.1162185);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(14,0.09626853);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(15,0.07489571);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(16,0.08527939);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(17,0.04846727);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(18,0.02412768);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(19,0.04846727);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(20,0.03057585);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(22,0.07419943);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(23,0.06796316);
   Eflow_pf_unmatched_ltp3_zmumu20->SetBinContent(25,0.3914618);
      histos[3] = Eflow_pf_unmatched_ltp3_zmumu20;

      TH1 *Eflow_pf_ltp3_data = new TH1F("Eflow_pf_ltp3_data","Sum PF Photon E in cone",25,0,50);
      Eflow_pf_ltp3_data->GetXaxis()->SetTitle("Energy (GeV)");
   Eflow_pf_ltp3_data->SetBinContent(2,150);
   Eflow_pf_ltp3_data->SetBinContent(3,113);
   Eflow_pf_ltp3_data->SetBinContent(4,91);
   Eflow_pf_ltp3_data->SetBinContent(5,58);
   Eflow_pf_ltp3_data->SetBinContent(6,54);
   Eflow_pf_ltp3_data->SetBinContent(7,49);
   Eflow_pf_ltp3_data->SetBinContent(8,34);
   Eflow_pf_ltp3_data->SetBinContent(9,27);
   Eflow_pf_ltp3_data->SetBinContent(10,22);
   Eflow_pf_ltp3_data->SetBinContent(11,17);
   Eflow_pf_ltp3_data->SetBinContent(12,16);
   Eflow_pf_ltp3_data->SetBinContent(13,13);
   Eflow_pf_ltp3_data->SetBinContent(14,14);
   Eflow_pf_ltp3_data->SetBinContent(15,7);
   Eflow_pf_ltp3_data->SetBinContent(16,12);
   Eflow_pf_ltp3_data->SetBinContent(17,7);
   Eflow_pf_ltp3_data->SetBinContent(18,3);
   Eflow_pf_ltp3_data->SetBinContent(19,6);
   Eflow_pf_ltp3_data->SetBinContent(20,4);
   Eflow_pf_ltp3_data->SetBinContent(21,3);
   Eflow_pf_ltp3_data->SetBinContent(22,2);
   Eflow_pf_ltp3_data->SetBinContent(23,4);
   Eflow_pf_ltp3_data->SetBinContent(24,1);
   Eflow_pf_ltp3_data->SetBinContent(25,18);

      data = Eflow_pf_ltp3_data;

    }
  else if( mode==Fig_7_b )
    {
      TH1 *pfPho_dR_fsrMatched_gt5GeV_zmumu20 = new TH1F("pfPho_dR_fsrMatched_gt5GeV_zmumu20","Sim Photon Delta R (muon and pfPhoton)",25,0,1);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(1,69.66815);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(2,67.92098);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(3,50.36446);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(4,37.27424);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(5,28.60013);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(6,23.6219);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(7,18.34398);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(8,16.75336);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(9,16.1538);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(10,12.49656);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(11,11.52315);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(12,10.12042);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(13,9.020871);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(14,9.435335);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(15,9.743793);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(16,8.33969);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(17,6.854492);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(18,7.346339);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(19,6.729375);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(20,5.998069);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(21,6.647728);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(22,5.768237);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(23,5.859389);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(24,5.177943);
   pfPho_dR_fsrMatched_gt5GeV_zmumu20->SetBinContent(25,5.709962);
      histos[0] = pfPho_dR_fsrMatched_gt5GeV_zmumu20;

      TH1 *pfPho_dR_partfsrMatched_gt5GeV_zmumu20 = new TH1F("pfPho_dR_partfsrMatched_gt5GeV_zmumu20","Sim Photon Delta R (muon and pfPhoton)",25,0,1);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(1,1.847387);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(2,2.07271);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(3,0.8411086);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(4,0.60519);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(5,0.4877909);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(6,0.2741534);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(7,0.3548011);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(8,0.1722843);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(9,0.4772559);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(10,0.2566253);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(11,0.4227642);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(12,0.2203278);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(13,0.1330806);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(14,0.1942323);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(15,0.0854913);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(16,0.1401948);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(17,0.1665022);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(18,0.1098309);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(19,0.109619);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(20,0.2344653);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(21,0.03057585);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(22,0.1098309);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(23,0.02346167);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(24,0.1777941);
   pfPho_dR_partfsrMatched_gt5GeV_zmumu20->SetBinContent(25,0.0854913);
      histos[1] = pfPho_dR_partfsrMatched_gt5GeV_zmumu20;

      TH1 *pfPho_dR_phoMatched_gt5GeV_zmumu20 = new TH1F("pfPho_dR_phoMatched_gt5GeV_zmumu20","Sim Photon Delta R (muon and pfPhoton)",25,0,1);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(1,0.2874131);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(2,0.3746602);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(3,0.5454006);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(4,0.4641173);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(5,0.9016852);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(6,1.552951);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(7,1.582679);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(8,2.55036);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(9,4.352126);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(10,5.710235);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(11,7.016243);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(12,8.433626);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(13,9.10191);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(14,10.28041);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(15,11.13202);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(16,12.16086);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(17,12.21256);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(18,12.92074);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(19,14.91862);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(20,15.89847);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(21,16.25876);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(22,18.3981);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(23,17.2749);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(24,19.2071);
   pfPho_dR_phoMatched_gt5GeV_zmumu20->SetBinContent(25,18.47463);
      histos[2] = pfPho_dR_phoMatched_gt5GeV_zmumu20;

      TH1 *pfPho_dR_unmatched_gt5GeV_zmumu20 = new TH1F("pfPho_dR_unmatched_gt5GeV_zmumu20","Sim Photon Delta R (muon and pfPhoton)",25,0,1);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(1,2.029692);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(2,0.2019217);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(3,0.2256559);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(4,0.2446674);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(5,0.2745167);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(6,0.5243911);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(7,0.4485266);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(8,0.6370375);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(9,0.7393909);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(10,0.9324729);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(11,1.206384);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(12,1.486653);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(13,1.249191);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(14,1.465431);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(15,1.402282);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(16,1.810454);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(17,1.818537);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(18,1.706799);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(19,2.049036);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(20,2.661493);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(21,2.139493);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(22,2.177516);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(23,2.242331);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(24,2.551208);
   pfPho_dR_unmatched_gt5GeV_zmumu20->SetBinContent(25,2.341385);
      histos[3] = pfPho_dR_unmatched_gt5GeV_zmumu20;

      TH1 *pfPho_dR_gt5GeV_data = new TH1F("pfPho_dR_gt5GeV_data","Sim Photon Delta R (muon and pfPhoton)",25,0,1);
      pfPho_dR_gt5GeV_data->GetXaxis()->SetTitle("#Delta R");
   pfPho_dR_gt5GeV_data->SetBinContent(1,77);
   pfPho_dR_gt5GeV_data->SetBinContent(2,74);
   pfPho_dR_gt5GeV_data->SetBinContent(3,44);
   pfPho_dR_gt5GeV_data->SetBinContent(4,45);
   pfPho_dR_gt5GeV_data->SetBinContent(5,34);
   pfPho_dR_gt5GeV_data->SetBinContent(6,23);
   pfPho_dR_gt5GeV_data->SetBinContent(7,29);
   pfPho_dR_gt5GeV_data->SetBinContent(8,29);
   pfPho_dR_gt5GeV_data->SetBinContent(9,20);
   pfPho_dR_gt5GeV_data->SetBinContent(10,27);
   pfPho_dR_gt5GeV_data->SetBinContent(11,16);
   pfPho_dR_gt5GeV_data->SetBinContent(12,22);
   pfPho_dR_gt5GeV_data->SetBinContent(13,19);
   pfPho_dR_gt5GeV_data->SetBinContent(14,25);
   pfPho_dR_gt5GeV_data->SetBinContent(15,33);
   pfPho_dR_gt5GeV_data->SetBinContent(16,27);
   pfPho_dR_gt5GeV_data->SetBinContent(17,26);
   pfPho_dR_gt5GeV_data->SetBinContent(18,27);
   pfPho_dR_gt5GeV_data->SetBinContent(19,26);
   pfPho_dR_gt5GeV_data->SetBinContent(20,22);
   pfPho_dR_gt5GeV_data->SetBinContent(21,17);
   pfPho_dR_gt5GeV_data->SetBinContent(22,35);
   pfPho_dR_gt5GeV_data->SetBinContent(23,35);
   pfPho_dR_gt5GeV_data->SetBinContent(24,32);
   pfPho_dR_gt5GeV_data->SetBinContent(25,33);
      data = pfPho_dR_gt5GeV_data;
    }
  else
    return 0;

  //  int fillColor[4]   = { kViolet-5, kViolet-4, kOrange+7, kOrange-2 };
  int fillColor[4]   = { 
    kOrange-2, 
    //    kOrange-2, 
    kOrange+7, 
    kMagenta-7, 
    kViolet-5 
  };
  int fillStyle[4]   = { 1001, 1001, 1001, 1001 }; 
  int lineWidth[4]   = { 1, 0, 1, 1 }; 
  int lineColor[4]   = { 
    kOrange+7, 
    kOrange-2, 
    // kOrange+3, 
    kViolet+3, 
    kViolet+3 
  }; 
  int lineStyle[4]   = { kSolid, kSolid, kSolid, kSolid }; 

  for( size_t ii=0; ii<4; ii++ )
    {
      histos[ii]->SetFillColor(fillColor[ii]);
      histos[ii]->SetFillStyle(fillStyle[ii]);
      histos[ii]->SetLineColor(lineColor[ii]);
      histos[ii]->SetLineWidth(lineWidth[ii]);
      histos[ii]->SetLineStyle(lineStyle[ii]);
      histos[ii]->SetMarkerSize(0);
    }
  
  // text2
  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS";
  txtSize[0] = 0.055;
  txtX[0] = 0.93;
  txtY[0] = 0.95;
  txtAlign[0] = 31;
  txtFont[0] = 42;
  
  txt[1] = "#gamma*/Z #rightarrow ";
  txt[1] += "ee";
  //txt[1] += "ee";
  txtSize[1] = 0.06;
  txtX[1] = 0.35;
  txtY[1] = 0.85;
  txtAlign[1] = 11;
  txtFont[1] = 42;

  // canvas
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=250;

  // histogram limits, in linear and logarithmic
  int nbin_(100);
  float xmin_(0.), xmax_(0.); 
  float ymin_(0.), ymax_(1.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(506);
  int ndivy(506);

  // canvas name
  TString ctitle;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  float xl_(0.35);
  float yl_(0.3);
  float dxl_(0.2);
  float dyl_(0.3);

  bool logScaleY = false;
  float markerSize;
      
  // root file, where the data is
  TString cname;
  nbin_ = 100;
  if( mode == Fig_7_a )
    {    
      cname += "Fig_7_a";
      ctitle = "Sum of reconstructed energy, mu-mu analysis";
      xtitle = "energy in #DeltaR(#mu-#gamma) < 0.3 cone [GeV]";	        
      ytitle = "entries / (2 GeV)"; 
      xmin_ = 0;
      xmax_ = 40;
      ymin_ = 0;
      ymax_ = 200;
      logScaleY = true;
      xl_=0.6;
      yl_=0.55;
      markerSize = 0.8;
    }
  else if( mode == Fig_7_b )
    {      
      cname += "Fig_7_b";
      ctitle = "Delta R of photon with pT>1 GeV, mu-mu analysis";
      xtitle = "#DeltaR(#mu-#gamma), #gamma with p_{T} > 1 ";	        
      xtitle += momUnit; 
      ytitle = "entries / 0.04"; 
      xmin_ = 0;
      xmax_ = 1;
      ymin_ = 0.;
      ymax_ = 100;
      logScaleY = false;
      xl_=0.60;
      yl_=0.55;
      markerSize = 1;
    }
  else
    return 0;

  data->SetMarkerSize(markerSize);
  data->SetMarkerStyle(kFullCircle);
  data->SetMarkerColor(kBlack);
  data->SetLineColor(kBlack);

  //  size_t nChan=channels.size();
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,
			  xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin(   60./dxcanv_ );
  c_->SetRightMargin(  30./dxcanv_ );
  c_->SetTopMargin(    30./dxcanv_ );
  c_->SetBottomMargin( 60./dxcanv_ ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);
   
  float titleOffsetY(1.2);
  float titleSizeY(xcanv_*0.060/ycanv_);
  float labelOffsetY(0.01);
  float titleOffsetX(1.0);
  float titleSizeX(0.060);
  float labelOffsetX(0.01);
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon4", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  ax_->SetTitleOffset(titleOffsetX);
  ax_->SetTitleSize(titleSizeX);
  //  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(labelOffsetX);

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetLabelOffset(labelOffsetY);

  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

  if( logScaleY ) c_->SetLogy(false);
  h_->GetYaxis()->SetRangeUser(ymin_,ymax_);

  h_->Draw("hist");

  // legend position and scale;
  int legendTextFont=42;
  float legendTextSize=0.040;

  TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
  legend->SetTextFont(legendTextFont);
  legend->SetTextSize(legendTextSize);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  
  TGraph* dummyGraph = new TGraph();
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);
  legend->AddEntry(dummyGraph," data","PL");
  legend->AddEntry(dummyGraph,"      ","0"); // skip a line

  TString leg_str[4] = 
    {
      " FSR photons",
      " ambiguous",
      " other photons",
      " pile-up and fakes"
    };

  bool fsrFirst = true;
  THStack* hstack = new THStack("StackedHisto","XXXX");
  for( size_t ii=0; ii<4; ii++ )
    {
      size_t jj = ii;
      if( fsrFirst ) jj = 3-ii;
      hstack->Add( histos[jj] );
      //      legend->AddEntry(histos[3-jj],leg_str[3-jj],"F"); 
      legend->AddEntry(histos[ii],leg_str[ii],"F"); 
    }
  hstack->Draw("SameHist");
  data->Draw("ESame");


  legend->Draw("Same");

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  c_->RedrawAxis();
  c_->GetFrame()->Draw();

  c_->cd();
  return c_;
}

//#include "UnfoldingMatrix.C"
TCanvas* UnfoldingMatrix( int mode )
{

  //  cout << "Hello-1" << endl;
  //setGrayScalePalette( 100, 0 );
  //void setGrayScalePalette( int ncol, int nwhite );
  //  setBlueToRedPalette( 40, 1 );
  //setHessPalette();
  //  gStyle->SetOptStat(0);
  if( grayScale )
    {
      setGrayScalePalette( 20, 1 );
    }
  else
    {
      gStyle->SetPalette(1);
    }
  // the canvas
  // text2
  //  size_t ntxt = 2;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "at #sqrt{s} = 8 TeV, CMS Simulation";
  txtSize[0] = 0.04;
  txtX[0] = 0.65;
  txtY[0] = 0.89;
  txtAlign[0] = 31;
  txtFont[0] = 42;
  
  txt[1] = "#gamma*/Z #rightarrow";
  if( mode==Fig_6_a )
    txt[1] += "ee";
  else   if( mode==Fig_6_b )
    txt[1] += "ee";
  else return 0;
  txtSize[1] = 0.07;
  txtX[1] = 0.18;
  txtY[1] = 0.85;
  txtAlign[1] = 11;
  txtFont[1] = 42;

  // canvas
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=350;

  // histogram limits, in linear and logarithmic
  int nbin_(100);
  float xmin_(15), xmax_(1500); 
  float ymin_(15), ymax_(1500); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(0);
  int ndivy(0);
  int ndivz(506);
  float markerSize(1.2);

  // canvas name
  TString ctitle;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString cname;
  if( mode == Fig_6_a )
    {    
      cname += "Fig_6_a";
      ctitle = "Unfolding matrix, mu-mu analysis";
      //xtitle = "pre-FSR M(ee) ["; //generated M(#mu#mu) (post-FSR)  [";	        
      ytitle = "reconstructed M(ee) ["; //"post-FSR M(#mu#mu) [";//reconstructed M(#mu#mu) [";	        
      xtitle = "generated M(ee) (post-FSR)  [";
      //ytitle = "post-FSR M(ee) [";

    }
  else if( mode == Fig_6_b )
    {      
      cname += "Fig_6_b";
      ctitle = "Unfolding matrix, m-e analysis";
      xtitle = "generated M(ee) (post-FSR)  [";	        
      ytitle = "reconstructed M(ee) [";	        
    }
  else
    return 0;

  xtitle += massUnit;       
  xtitle += "]";
  ytitle += massUnit;       
  ytitle += "]";

  markerSize = 1.2;

  //  size_t nChan=channels.size();
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,
			  xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin(   60./dxcanv_ );
  c_->SetRightMargin(  60./dxcanv_ );
  c_->SetTopMargin(    20./dxcanv_ );
  c_->SetBottomMargin( 60./dxcanv_ ); 
  c_->SetFillColor(0);
  c_->SetTickx(0);
  c_->SetTicky(0);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);
  c_->SetLogx();
  c_->SetLogy();
  if( logScaleZ )
    {
      c_->SetLogz();
    }
   
  float titleOffsetY(1.3);
  float titleSizeY(0.045*dxcanv_/dycanv_);
  //  float labelOffsetY(99);
  float titleOffsetX(1.3);
  float titleSizeX(0.045);
  //  float labelOffsetX(99);
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon5", "bidon", 
		      nbin_, xmin_, xmax_);
    //		      nbin_, ymin_, ymax_ );
  h_->SetMinimum(ymin_);
  h_->SetMaximum(ymax_);
  
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  ax_->SetTitleOffset(titleOffsetX);
  ax_->SetTitleSize(titleSizeX);
  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(99);
  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetLabelSize(0);
  ay_->SetLabelOffset(99);
  ay_->SetNoExponent(kTRUE);
  ay_->SetMoreLogLabels(kTRUE);

  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();
  h_->Draw("COLZ");

  TH1* resMatrix(0);
  if( mode==Fig_6_a )
    {

      //modify this
      //TFile* fr = new TFile("../Inputs/FSR_EE/EEFSRresMatrixProd_tmp_41.root");
      TFile* fr = new TFile("../Inputs/unfoldEE/resMatrixProdEE.root");
      fr->cd();
      resMatrix = (TH2D*)gDirectory->Get("DetResponse");
      //TFile* fr = new TFile("../Inputs/FSR/FSRresMatrixProd_tmp_41.root");
      //fr->cd();
      //TH2F* hmig = (TH2F*)gDirectory->Get("hden");
      //TH1F* _inTrue = (TH1F*)gDirectory->Get("htrue");
      //TH1F* _inMeas = (TH1F*)gDirectory->Get("hmeas");
      //resMatrix = getMigMatrix(_inTrue, hmig);
      //TH2F* hmig = (TH2F*)gDirectory->Get("hden");
      //TH1F* _inTrue = (TH1F*)gDirectory->Get("hpreFSR");
      //resMatrix = getMigMatrix(_inTrue, hmig);

    }
  else if( mode==Fig_6_b )
    {
      Double_t xAxis1[41] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
                  96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
		  200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};
      Double_t yAxis1[41] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
                  96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
		  200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

      resMatrix = new TH2F("hResponse__1","",40, xAxis1,40, yAxis1);
      resMatrix->SetBinContent(43,0.8467079);
      resMatrix->SetBinContent(44,0.08080643);
      resMatrix->SetBinContent(45,0.00142174);
      resMatrix->SetBinContent(46,0.0001512173);
      resMatrix->SetBinContent(48,3.667974e-05);
      resMatrix->SetBinContent(50,3.452443e-05);
      resMatrix->SetBinContent(51,3.277291e-05);
      resMatrix->SetBinContent(52,3.70439e-05);
      resMatrix->SetBinContent(53,3.14693e-05);
      resMatrix->SetBinContent(54,5.074081e-05);
      resMatrix->SetBinContent(55,3.782434e-05);
      resMatrix->SetBinContent(56,3.82453e-05);
      resMatrix->SetBinContent(58,1.786957e-06);
      resMatrix->SetBinContent(59,1.970142e-06);
      resMatrix->SetBinContent(85,0.1373563);
      resMatrix->SetBinContent(86,0.7547943);
      resMatrix->SetBinContent(87,0.07509371);
      resMatrix->SetBinContent(88,0.001738999);
      resMatrix->SetBinContent(89,0.0001842554);
      resMatrix->SetBinContent(90,7.335947e-05);
      resMatrix->SetBinContent(92,6.904885e-05);
      resMatrix->SetBinContent(93,3.277291e-05);
      resMatrix->SetBinContent(95,3.14693e-05);
      resMatrix->SetBinContent(98,9.561326e-06);
      resMatrix->SetBinContent(100,8.934786e-07);
      resMatrix->SetBinContent(101,1.970142e-06);
      resMatrix->SetBinContent(127,0.009922056);
      resMatrix->SetBinContent(128,0.1327651);
      resMatrix->SetBinContent(129,0.7033734);
      resMatrix->SetBinContent(130,0.06759413);
      resMatrix->SetBinContent(131,0.002257128);
      resMatrix->SetBinContent(132,0.0001833987);
      resMatrix->SetBinContent(133,3.540199e-05);
      resMatrix->SetBinContent(137,9.440791e-05);
      resMatrix->SetBinContent(138,2.537041e-05);
      resMatrix->SetBinContent(141,9.030079e-06);
      resMatrix->SetBinContent(142,3.573914e-06);
      resMatrix->SetBinContent(143,9.850712e-07);
      resMatrix->SetBinContent(145,2.883422e-05);
      resMatrix->SetBinContent(169,0.002186801);
      resMatrix->SetBinContent(170,0.01884937);
      resMatrix->SetBinContent(171,0.1680238);
      resMatrix->SetBinContent(172,0.6940118);
      resMatrix->SetBinContent(173,0.07757151);
      resMatrix->SetBinContent(174,0.003227817);
      resMatrix->SetBinContent(175,0.0002832159);
      resMatrix->SetBinContent(176,3.452443e-05);
      resMatrix->SetBinContent(178,7.408779e-05);
      resMatrix->SetBinContent(179,9.440791e-05);
      resMatrix->SetBinContent(180,7.611122e-05);
      resMatrix->SetBinContent(182,9.561326e-06);
      resMatrix->SetBinContent(184,8.934786e-07);
      resMatrix->SetBinContent(185,2.955214e-06);
      resMatrix->SetBinContent(187,2.883422e-05);
      resMatrix->SetBinContent(190,0.0001169178);
      resMatrix->SetBinContent(211,0.0007289337);
      resMatrix->SetBinContent(212,0.006392395);
      resMatrix->SetBinContent(213,0.02455732);
      resMatrix->SetBinContent(214,0.1819144);
      resMatrix->SetBinContent(215,0.6895297);
      resMatrix->SetBinContent(216,0.0937534);
      resMatrix->SetBinContent(217,0.003611003);
      resMatrix->SetBinContent(218,0.0002071466);
      resMatrix->SetBinContent(219,0.0001310916);
      resMatrix->SetBinContent(223,1.891217e-05);
      resMatrix->SetBinContent(224,3.82453e-05);
      resMatrix->SetBinContent(225,1.806016e-05);
      resMatrix->SetBinContent(226,1.786957e-06);
      resMatrix->SetBinContent(227,1.970142e-06);
      resMatrix->SetBinContent(242,8.693093e-07);
      resMatrix->SetBinContent(254,0.003442059);
      resMatrix->SetBinContent(255,0.01305416);
      resMatrix->SetBinContent(256,0.02661425);
      resMatrix->SetBinContent(257,0.1786356);
      resMatrix->SetBinContent(258,0.6608222);
      resMatrix->SetBinContent(259,0.1005062);
      resMatrix->SetBinContent(260,0.004039358);
      resMatrix->SetBinContent(261,0.0003277291);
      resMatrix->SetBinContent(262,0.0001111317);
      resMatrix->SetBinContent(264,0.0001014816);
      resMatrix->SetBinContent(265,3.782434e-05);
      resMatrix->SetBinContent(266,2.868398e-05);
      resMatrix->SetBinContent(267,9.030079e-06);
      resMatrix->SetBinContent(268,5.360871e-06);
      resMatrix->SetBinContent(269,2.955214e-06);
      resMatrix->SetBinContent(272,6.442884e-05);
      resMatrix->SetBinContent(276,0.0001653439);
      resMatrix->SetBinContent(296,0.0001639075);
      resMatrix->SetBinContent(297,0.004523717);
      resMatrix->SetBinContent(298,0.01096325);
      resMatrix->SetBinContent(299,0.02247916);
      resMatrix->SetBinContent(300,0.164692);
      resMatrix->SetBinContent(301,0.6057634);
      resMatrix->SetBinContent(302,0.09708269);
      resMatrix->SetBinContent(303,0.004031068);
      resMatrix->SetBinContent(304,0.0005556585);
      resMatrix->SetBinContent(305,0.0001573465);
      resMatrix->SetBinContent(306,0.0001014816);
      resMatrix->SetBinContent(307,7.564868e-05);
      resMatrix->SetBinContent(308,2.868398e-05);
      resMatrix->SetBinContent(309,9.030079e-06);
      resMatrix->SetBinContent(310,9.828264e-06);
      resMatrix->SetBinContent(311,8.865642e-06);
      resMatrix->SetBinContent(312,1.030949e-05);
      resMatrix->SetBinContent(313,2.883422e-05);
      resMatrix->SetBinContent(338,0.0001639075);
      resMatrix->SetBinContent(339,0.002455732);
      resMatrix->SetBinContent(340,0.005973083);
      resMatrix->SetBinContent(341,0.007646598);
      resMatrix->SetBinContent(342,0.01841323);
      resMatrix->SetBinContent(343,0.1478033);
      resMatrix->SetBinContent(344,0.5334714);
      resMatrix->SetBinContent(345,0.08786416);
      resMatrix->SetBinContent(346,0.005852936);
      resMatrix->SetBinContent(347,0.001007018);
      resMatrix->SetBinContent(348,0.0003805561);
      resMatrix->SetBinContent(349,5.673652e-05);
      resMatrix->SetBinContent(350,6.692929e-05);
      resMatrix->SetBinContent(351,4.51504e-06);
      resMatrix->SetBinContent(352,1.340218e-05);
      resMatrix->SetBinContent(353,9.850713e-06);
      resMatrix->SetBinContent(355,5.766843e-05);
      resMatrix->SetBinContent(381,0.001292491);
      resMatrix->SetBinContent(382,0.00166339);
      resMatrix->SetBinContent(383,0.00419181);
      resMatrix->SetBinContent(384,0.007666065);
      resMatrix->SetBinContent(385,0.01819662);
      resMatrix->SetBinContent(386,0.1453824);
      resMatrix->SetBinContent(387,0.4657361);
      resMatrix->SetBinContent(388,0.09538803);
      resMatrix->SetBinContent(389,0.009629606);
      resMatrix->SetBinContent(390,0.001420743);
      resMatrix->SetBinContent(391,0.0003782435);
      resMatrix->SetBinContent(392,0.0001338586);
      resMatrix->SetBinContent(393,4.063536e-05);
      resMatrix->SetBinContent(394,3.037827e-05);
      resMatrix->SetBinContent(395,2.856707e-05);
      resMatrix->SetBinContent(396,1.030949e-05);
      resMatrix->SetBinContent(423,0.0006462453);
      resMatrix->SetBinContent(424,0.001058521);
      resMatrix->SetBinContent(425,0.002533511);
      resMatrix->SetBinContent(426,0.003594614);
      resMatrix->SetBinContent(427,0.006266152);
      resMatrix->SetBinContent(428,0.01598481);
      resMatrix->SetBinContent(429,0.1271589);
      resMatrix->SetBinContent(430,0.3714021);
      resMatrix->SetBinContent(431,0.08295308);
      resMatrix->SetBinContent(432,0.009209458);
      resMatrix->SetBinContent(433,0.001153642);
      resMatrix->SetBinContent(434,0.0001529812);
      resMatrix->SetBinContent(435,0.0001038459);
      resMatrix->SetBinContent(436,5.271524e-05);
      resMatrix->SetBinContent(437,3.644764e-05);
      resMatrix->SetBinContent(438,2.061898e-05);
      resMatrix->SetBinContent(456,3.417432e-05);
      resMatrix->SetBinContent(463,0.0001822334);
      resMatrix->SetBinContent(465,0.0001292491);
      resMatrix->SetBinContent(466,0.0007560865);
      resMatrix->SetBinContent(467,0.001566171);
      resMatrix->SetBinContent(468,0.002640941);
      resMatrix->SetBinContent(469,0.003965023);
      resMatrix->SetBinContent(470,0.008562057);
      resMatrix->SetBinContent(471,0.02264608);
      resMatrix->SetBinContent(472,0.1422486);
      resMatrix->SetBinContent(473,0.3375712);
      resMatrix->SetBinContent(474,0.07900345);
      resMatrix->SetBinContent(475,0.008945458);
      resMatrix->SetBinContent(476,0.001472444);
      resMatrix->SetBinContent(477,0.0002573573);
      resMatrix->SetBinContent(478,0.0001125783);
      resMatrix->SetBinContent(479,6.304456e-05);
      resMatrix->SetBinContent(480,2.061898e-05);
      resMatrix->SetBinContent(481,5.766843e-05);
      resMatrix->SetBinContent(506,0.0001639075);
      resMatrix->SetBinContent(507,0.0005169963);
      resMatrix->SetBinContent(508,0.0008316952);
      resMatrix->SetBinContent(509,0.0007370215);
      resMatrix->SetBinContent(510,0.002090745);
      resMatrix->SetBinContent(511,0.004177435);
      resMatrix->SetBinContent(512,0.006145348);
      resMatrix->SetBinContent(513,0.01202766);
      resMatrix->SetBinContent(514,0.02556029);
      resMatrix->SetBinContent(515,0.1331151);
      resMatrix->SetBinContent(516,0.3005886);
      resMatrix->SetBinContent(517,0.07428701);
      resMatrix->SetBinContent(518,0.007629938);
      resMatrix->SetBinContent(519,0.001210031);
      resMatrix->SetBinContent(520,0.0003055697);
      resMatrix->SetBinContent(521,0.0001773128);
      resMatrix->SetBinContent(522,0.0001134044);
      resMatrix->SetBinContent(523,5.766843e-05);
      resMatrix->SetBinContent(524,6.442884e-05);
      resMatrix->SetBinContent(547,0.0003644669);
      resMatrix->SetBinContent(548,0.0001639075);
      resMatrix->SetBinContent(549,0.0001292491);
      resMatrix->SetBinContent(550,0.0003024346);
      resMatrix->SetBinContent(551,0.001105532);
      resMatrix->SetBinContent(552,0.001613908);
      resMatrix->SetBinContent(553,0.004071229);
      resMatrix->SetBinContent(554,0.006248921);
      resMatrix->SetBinContent(555,0.01002851);
      resMatrix->SetBinContent(556,0.01570661);
      resMatrix->SetBinContent(557,0.02999024);
      resMatrix->SetBinContent(558,0.1362898);
      resMatrix->SetBinContent(559,0.2869733);
      resMatrix->SetBinContent(560,0.0606953);
      resMatrix->SetBinContent(561,0.006032093);
      resMatrix->SetBinContent(562,0.001056092);
      resMatrix->SetBinContent(563,0.0004846551);
      resMatrix->SetBinContent(564,0.0001237139);
      resMatrix->SetBinContent(565,2.883422e-05);
      resMatrix->SetBinContent(566,6.442884e-05);
      resMatrix->SetBinContent(567,8.01796e-05);
      resMatrix->SetBinContent(589,0.0003644669);
      resMatrix->SetBinContent(590,0.0001639075);
      resMatrix->SetBinContent(591,0.0002584981);
      resMatrix->SetBinContent(592,0.0008316952);
      resMatrix->SetBinContent(593,0.0009673407);
      resMatrix->SetBinContent(594,0.002934379);
      resMatrix->SetBinContent(595,0.00569972);
      resMatrix->SetBinContent(596,0.00811324);
      resMatrix->SetBinContent(597,0.0125848);
      resMatrix->SetBinContent(598,0.01970735);
      resMatrix->SetBinContent(599,0.02825943);
      resMatrix->SetBinContent(600,0.04107469);
      resMatrix->SetBinContent(601,0.1494251);
      resMatrix->SetBinContent(602,0.3252763);
      resMatrix->SetBinContent(603,0.07005084);
      resMatrix->SetBinContent(604,0.008165501);
      resMatrix->SetBinContent(605,0.00277199);
      resMatrix->SetBinContent(606,0.0004536175);
      resMatrix->SetBinContent(607,0.0002306737);
      resMatrix->SetBinContent(608,0.0001932865);
      resMatrix->SetBinContent(627,1.289713e-05);
      resMatrix->SetBinContent(631,0.0009111672);
      resMatrix->SetBinContent(632,0.0003278151);
      resMatrix->SetBinContent(633,0.0007754944);
      resMatrix->SetBinContent(634,0.00113413);
      resMatrix->SetBinContent(635,0.001289788);
      resMatrix->SetBinContent(636,0.004841725);
      resMatrix->SetBinContent(637,0.009877156);
      resMatrix->SetBinContent(638,0.01688244);
      resMatrix->SetBinContent(639,0.02795529);
      resMatrix->SetBinContent(640,0.03708094);
      resMatrix->SetBinContent(641,0.04578783);
      resMatrix->SetBinContent(642,0.05500304);
      resMatrix->SetBinContent(643,0.07284968);
      resMatrix->SetBinContent(644,0.1822293);
      resMatrix->SetBinContent(645,0.3900723);
      resMatrix->SetBinContent(646,0.08236711);
      resMatrix->SetBinContent(647,0.02222124);
      resMatrix->SetBinContent(648,0.002597991);
      resMatrix->SetBinContent(649,0.0005190159);
      resMatrix->SetBinContent(650,6.442884e-05);
      resMatrix->SetBinContent(651,8.01796e-05);
      resMatrix->SetBinContent(652,0.0003507534);
      resMatrix->SetBinContent(673,0.0007289337);
      resMatrix->SetBinContent(674,0.0009834453);
      resMatrix->SetBinContent(675,0.001809487);
      resMatrix->SetBinContent(676,0.002192651);
      resMatrix->SetBinContent(677,0.004007555);
      resMatrix->SetBinContent(678,0.01327806);
      resMatrix->SetBinContent(679,0.03391511);
      resMatrix->SetBinContent(680,0.05903677);
      resMatrix->SetBinContent(681,0.09071541);
      resMatrix->SetBinContent(682,0.1185405);
      resMatrix->SetBinContent(683,0.1380558);
      resMatrix->SetBinContent(684,0.1627004);
      resMatrix->SetBinContent(685,0.1848854);
      resMatrix->SetBinContent(686,0.2024515);
      resMatrix->SetBinContent(687,0.339075);
      resMatrix->SetBinContent(688,0.5558098);
      resMatrix->SetBinContent(689,0.2382818);
      resMatrix->SetBinContent(690,0.01906225);
      resMatrix->SetBinContent(691,0.002739251);
      resMatrix->SetBinContent(692,0.0009020037);
      resMatrix->SetBinContent(693,0.0005612572);
      resMatrix->SetBinContent(694,0.0001169178);
      resMatrix->SetBinContent(695,0.000270416);
      resMatrix->SetBinContent(696,0.0001653439);
      resMatrix->SetBinContent(709,7.129493e-06);
      resMatrix->SetBinContent(715,0.0003644669);
      resMatrix->SetBinContent(716,0.0006556302);
      resMatrix->SetBinContent(717,0.001809487);
      resMatrix->SetBinContent(718,0.001814608);
      resMatrix->SetBinContent(719,0.004283938);
      resMatrix->SetBinContent(720,0.01705608);
      resMatrix->SetBinContent(721,0.04591638);
      resMatrix->SetBinContent(722,0.08220266);
      resMatrix->SetBinContent(723,0.1139514);
      resMatrix->SetBinContent(724,0.1378403);
      resMatrix->SetBinContent(725,0.1588256);
      resMatrix->SetBinContent(726,0.1764512);
      resMatrix->SetBinContent(727,0.1837317);
      resMatrix->SetBinContent(728,0.1843997);
      resMatrix->SetBinContent(729,0.1671332);
      resMatrix->SetBinContent(730,0.3289627);
      resMatrix->SetBinContent(731,0.628898);
      resMatrix->SetBinContent(732,0.185509);
      resMatrix->SetBinContent(733,0.01741587);
      resMatrix->SetBinContent(734,0.003221442);
      resMatrix->SetBinContent(735,0.001282874);
      resMatrix->SetBinContent(736,0.0001169178);
      resMatrix->SetBinContent(737,0.000135208);
      resMatrix->SetBinContent(738,0.0001653439);
      resMatrix->SetBinContent(739,0.0002053368);
      resMatrix->SetBinContent(747,8.950829e-06);
      resMatrix->SetBinContent(757,0.0001822334);
      resMatrix->SetBinContent(758,0.0001639075);
      resMatrix->SetBinContent(759,0.0001292491);
      resMatrix->SetBinContent(760,0.0003024346);
      resMatrix->SetBinContent(761,0.0007830854);
      resMatrix->SetBinContent(762,0.002677621);
      resMatrix->SetBinContent(763,0.007859242);
      resMatrix->SetBinContent(764,0.01273951);
      resMatrix->SetBinContent(765,0.01835283);
      resMatrix->SetBinContent(766,0.02263382);
      resMatrix->SetBinContent(767,0.02627687);
      resMatrix->SetBinContent(768,0.02909986);
      resMatrix->SetBinContent(769,0.02916257);
      resMatrix->SetBinContent(770,0.02843538);
      resMatrix->SetBinContent(771,0.02112587);
      resMatrix->SetBinContent(772,0.02070458);
      resMatrix->SetBinContent(773,0.09974536);
      resMatrix->SetBinContent(774,0.6095178);
      resMatrix->SetBinContent(775,0.1680747);
      resMatrix->SetBinContent(776,0.01939308);
      resMatrix->SetBinContent(777,0.004650417);
      resMatrix->SetBinContent(778,0.0008184245);
      resMatrix->SetBinContent(779,0.000405624);
      resMatrix->SetBinContent(783,0.0003067447);
      resMatrix->SetBinContent(802,7.560865e-05);
      resMatrix->SetBinContent(803,0.0001842554);
      resMatrix->SetBinContent(804,0.0002200784);
      resMatrix->SetBinContent(805,0.00141608);
      resMatrix->SetBinContent(806,0.002278612);
      resMatrix->SetBinContent(807,0.003801657);
      resMatrix->SetBinContent(808,0.004297092);
      resMatrix->SetBinContent(809,0.00453158);
      resMatrix->SetBinContent(810,0.004515932);
      resMatrix->SetBinContent(811,0.004746955);
      resMatrix->SetBinContent(812,0.004025318);
      resMatrix->SetBinContent(813,0.003142467);
      resMatrix->SetBinContent(814,0.001714585);
      resMatrix->SetBinContent(815,0.006035531);
      resMatrix->SetBinContent(816,0.1618487);
      resMatrix->SetBinContent(817,0.5924281);
      resMatrix->SetBinContent(818,0.1785967);
      resMatrix->SetBinContent(819,0.0275016);
      resMatrix->SetBinContent(820,0.004325958);
      resMatrix->SetBinContent(821,0.001487288);
      resMatrix->SetBinContent(822,0.0003306878);
      resMatrix->SetBinContent(825,0.0003067447);
      resMatrix->SetBinContent(844,7.560865e-05);
      resMatrix->SetBinContent(845,4.606384e-05);
      resMatrix->SetBinContent(846,0.0001100392);
      resMatrix->SetBinContent(847,0.0003186179);
      resMatrix->SetBinContent(848,0.0006904885);
      resMatrix->SetBinContent(849,0.001048733);
      resMatrix->SetBinContent(850,0.0009260974);
      resMatrix->SetBinContent(851,0.001479057);
      resMatrix->SetBinContent(852,0.001370002);
      resMatrix->SetBinContent(853,0.001456237);
      resMatrix->SetBinContent(854,0.001319463);
      resMatrix->SetBinContent(855,0.0006998311);
      resMatrix->SetBinContent(856,0.0002868066);
      resMatrix->SetBinContent(857,0.0006491619);
      resMatrix->SetBinContent(858,0.013691);
      resMatrix->SetBinContent(859,0.1759755);
      resMatrix->SetBinContent(860,0.5085368);
      resMatrix->SetBinContent(861,0.1467287);
      resMatrix->SetBinContent(862,0.01800534);
      resMatrix->SetBinContent(863,0.00270416);
      resMatrix->SetBinContent(864,0.0003306878);
      resMatrix->SetBinContent(865,0.0006160105);
      resMatrix->SetBinContent(889,0.000106206);
      resMatrix->SetBinContent(890,0.0003797687);
      resMatrix->SetBinContent(891,0.000688231);
      resMatrix->SetBinContent(892,0.0008890535);
      resMatrix->SetBinContent(893,0.0009440791);
      resMatrix->SetBinContent(894,0.001065557);
      resMatrix->SetBinContent(895,0.0007375747);
      resMatrix->SetBinContent(896,0.0006406089);
      resMatrix->SetBinContent(897,0.0004740792);
      resMatrix->SetBinContent(898,0.0001375957);
      resMatrix->SetBinContent(899,0.0002383872);
      resMatrix->SetBinContent(900,0.003804202);
      resMatrix->SetBinContent(901,0.02920906);
      resMatrix->SetBinContent(902,0.2441209);
      resMatrix->SetBinContent(903,0.5576491);
      resMatrix->SetBinContent(904,0.1552668);
      resMatrix->SetBinContent(905,0.01825308);
      resMatrix->SetBinContent(906,0.001984127);
      resMatrix->SetBinContent(907,0.0002053368);
      resMatrix->SetBinContent(930,3.667974e-05);
      resMatrix->SetBinContent(931,3.540199e-05);
      resMatrix->SetBinContent(932,0.0002071466);
      resMatrix->SetBinContent(933,0.0002949561);
      resMatrix->SetBinContent(934,0.0004445268);
      resMatrix->SetBinContent(935,0.0002832237);
      resMatrix->SetBinContent(936,0.000558149);
      resMatrix->SetBinContent(937,0.0003593313);
      resMatrix->SetBinContent(938,0.0002581558);
      resMatrix->SetBinContent(939,0.0001715715);
      resMatrix->SetBinContent(940,8.220003e-05);
      resMatrix->SetBinContent(941,0.0001201787);
      resMatrix->SetBinContent(942,0.0014124);
      resMatrix->SetBinContent(943,0.006170522);
      resMatrix->SetBinContent(944,0.02757554);
      resMatrix->SetBinContent(945,0.2117543);
      resMatrix->SetBinContent(946,0.5488142);
      resMatrix->SetBinContent(947,0.1404811);
      resMatrix->SetBinContent(948,0.01025132);
      resMatrix->SetBinContent(949,0.002258705);
      resMatrix->SetBinContent(950,0.0007468226);
      resMatrix->SetBinContent(951,0.0003067447);
      resMatrix->SetBinContent(973,3.540199e-05);
      resMatrix->SetBinContent(974,6.904885e-05);
      resMatrix->SetBinContent(975,0.0003277291);
      resMatrix->SetBinContent(976,0.0002963512);
      resMatrix->SetBinContent(977,0.0003461623);
      resMatrix->SetBinContent(978,0.0003298153);
      resMatrix->SetBinContent(979,0.0002080339);
      resMatrix->SetBinContent(980,0.0002199105);
      resMatrix->SetBinContent(981,0.0001219061);
      resMatrix->SetBinContent(982,5.718263e-05);
      resMatrix->SetBinContent(983,7.683556e-05);
      resMatrix->SetBinContent(984,0.0006701169);
      resMatrix->SetBinContent(985,0.00294109);
      resMatrix->SetBinContent(986,0.008053605);
      resMatrix->SetBinContent(987,0.02990699);
      resMatrix->SetBinContent(988,0.2257683);
      resMatrix->SetBinContent(989,0.594511);
      resMatrix->SetBinContent(990,0.1273148);
      resMatrix->SetBinContent(991,0.007597462);
      resMatrix->SetBinContent(992,0.0009957635);
      resMatrix->SetBinContent(993,0.0003067447);
      resMatrix->SetBinContent(1015,7.080398e-05);
      resMatrix->SetBinContent(1016,0.0001380977);
      resMatrix->SetBinContent(1017,0.0001638645);
      resMatrix->SetBinContent(1018,0.0002222634);
      resMatrix->SetBinContent(1019,0.000314693);
      resMatrix->SetBinContent(1020,0.0003551857);
      resMatrix->SetBinContent(1021,0.000113473);
      resMatrix->SetBinContent(1022,0.0001051746);
      resMatrix->SetBinContent(1023,4.063536e-05);
      resMatrix->SetBinContent(1024,4.110001e-05);
      resMatrix->SetBinContent(1025,4.235806e-05);
      resMatrix->SetBinContent(1026,0.000463927);
      resMatrix->SetBinContent(1027,0.001960727);
      resMatrix->SetBinContent(1028,0.004510019);
      resMatrix->SetBinContent(1029,0.01090443);
      resMatrix->SetBinContent(1030,0.02899561);
      resMatrix->SetBinContent(1031,0.2053811);
      resMatrix->SetBinContent(1032,0.642857);
      resMatrix->SetBinContent(1033,0.1217666);
      resMatrix->SetBinContent(1034,0.006721403);
      resMatrix->SetBinContent(1036,0.0003924443);
      resMatrix->SetBinContent(1057,3.540199e-05);
      resMatrix->SetBinContent(1059,3.277291e-05);
      resMatrix->SetBinContent(1060,3.70439e-05);
      resMatrix->SetBinContent(1061,0.0001888158);
      resMatrix->SetBinContent(1062,5.074081e-05);
      resMatrix->SetBinContent(1063,0.000113473);
      resMatrix->SetBinContent(1064,0.0001242972);
      resMatrix->SetBinContent(1065,7.675567e-05);
      resMatrix->SetBinContent(1066,2.591088e-05);
      resMatrix->SetBinContent(1067,3.743271e-05);
      resMatrix->SetBinContent(1068,0.0002371183);
      resMatrix->SetBinContent(1069,0.0008938607);
      resMatrix->SetBinContent(1070,0.001804007);
      resMatrix->SetBinContent(1071,0.003688262);
      resMatrix->SetBinContent(1072,0.00841808);
      resMatrix->SetBinContent(1073,0.01974037);
      resMatrix->SetBinContent(1074,0.17791);
      resMatrix->SetBinContent(1075,0.6468167);
      resMatrix->SetBinContent(1076,0.1062977);
      resMatrix->SetBinContent(1077,0.004294426);
      resMatrix->SetBinContent(1078,0.001962221);
      resMatrix->SetBinContent(1079,0.0004237144);
      resMatrix->SetBinContent(1080,0.0005461054);
      resMatrix->SetBinContent(1087,3.564747e-06);
      resMatrix->SetBinContent(1099,3.540199e-05);
      resMatrix->SetBinContent(1101,3.277291e-05);
      resMatrix->SetBinContent(1102,3.70439e-05);
      resMatrix->SetBinContent(1104,5.074081e-05);
      resMatrix->SetBinContent(1105,7.564868e-05);
      resMatrix->SetBinContent(1106,9.561326e-05);
      resMatrix->SetBinContent(1107,1.354512e-05);
      resMatrix->SetBinContent(1108,1.876305e-05);
      resMatrix->SetBinContent(1109,2.167157e-05);
      resMatrix->SetBinContent(1110,0.0001752613);
      resMatrix->SetBinContent(1111,0.0005190159);
      resMatrix->SetBinContent(1112,0.001030861);
      resMatrix->SetBinContent(1113,0.002164849);
      resMatrix->SetBinContent(1114,0.003858287);
      resMatrix->SetBinContent(1115,0.00878852);
      resMatrix->SetBinContent(1116,0.02066799);
      resMatrix->SetBinContent(1117,0.1829551);
      resMatrix->SetBinContent(1118,0.6883215);
      resMatrix->SetBinContent(1119,0.1030666);
      resMatrix->SetBinContent(1120,0.005101776);
      resMatrix->SetBinContent(1121,0.0004237144);
      resMatrix->SetBinContent(1129,3.564747e-06);
      resMatrix->SetBinContent(1143,3.277291e-05);
      resMatrix->SetBinContent(1144,7.408779e-05);
      resMatrix->SetBinContent(1145,3.14693e-05);
      resMatrix->SetBinContent(1146,7.611122e-05);
      resMatrix->SetBinContent(1147,9.456086e-05);
      resMatrix->SetBinContent(1148,6.692929e-05);
      resMatrix->SetBinContent(1149,4.515039e-05);
      resMatrix->SetBinContent(1150,1.161522e-05);
      resMatrix->SetBinContent(1151,1.280593e-05);
      resMatrix->SetBinContent(1152,0.0001134044);
      resMatrix->SetBinContent(1153,0.0002018395);
      resMatrix->SetBinContent(1154,0.0006442884);
      resMatrix->SetBinContent(1155,0.001042335);
      resMatrix->SetBinContent(1156,0.001636849);
      resMatrix->SetBinContent(1157,0.003650616);
      resMatrix->SetBinContent(1158,0.01008598);
      resMatrix->SetBinContent(1159,0.02238171);
      resMatrix->SetBinContent(1160,0.15982);
      resMatrix->SetBinContent(1161,0.6950834);
      resMatrix->SetBinContent(1162,0.1004694);
      resMatrix->SetBinContent(1163,0.004237144);
      resMatrix->SetBinContent(1171,5.953587e-05);
      resMatrix->SetBinContent(1189,3.782434e-05);
      resMatrix->SetBinContent(1191,1.806016e-05);
      resMatrix->SetBinContent(1192,7.147829e-06);
      resMatrix->SetBinContent(1193,4.925356e-06);
      resMatrix->SetBinContent(1194,6.185694e-05);
      resMatrix->SetBinContent(1195,0.0002306737);
      resMatrix->SetBinContent(1196,0.0005798595);
      resMatrix->SetBinContent(1197,0.0009621552);
      resMatrix->SetBinContent(1198,0.001636849);
      resMatrix->SetBinContent(1199,0.001081664);
      resMatrix->SetBinContent(1200,0.003141534);
      resMatrix->SetBinContent(1201,0.006160105);
      resMatrix->SetBinContent(1202,0.02190679);
      resMatrix->SetBinContent(1203,0.1625747);
      resMatrix->SetBinContent(1204,0.7154372);
      resMatrix->SetBinContent(1205,0.08050574);
      resMatrix->SetBinContent(1206,0.003822738);
      resMatrix->SetBinContent(1230,2.537041e-05);
      resMatrix->SetBinContent(1232,1.912265e-05);
      resMatrix->SetBinContent(1233,2.709024e-05);
      resMatrix->SetBinContent(1234,6.25435e-06);
      resMatrix->SetBinContent(1235,6.895499e-06);
      resMatrix->SetBinContent(1237,0.0001441711);
      resMatrix->SetBinContent(1238,0.0003221442);
      resMatrix->SetBinContent(1239,0.0003207184);
      resMatrix->SetBinContent(1240,0.0008184245);
      resMatrix->SetBinContent(1241,0.001487288);
      resMatrix->SetBinContent(1242,0.002810846);
      resMatrix->SetBinContent(1243,0.003696063);
      resMatrix->SetBinContent(1244,0.006472462);
      resMatrix->SetBinContent(1245,0.01963166);
      resMatrix->SetBinContent(1246,0.1542342);
      resMatrix->SetBinContent(1247,0.7482879);
      resMatrix->SetBinContent(1248,0.08519245);
      resMatrix->SetBinContent(1249,0.002959819);
      resMatrix->SetBinContent(1250,0.0007877786);
      resMatrix->SetBinContent(1257,2.579426e-05);
      resMatrix->SetBinContent(1270,3.70439e-05);
      resMatrix->SetBinContent(1274,9.561326e-06);
      resMatrix->SetBinContent(1275,4.51504e-06);
      resMatrix->SetBinContent(1276,3.573914e-06);
      resMatrix->SetBinContent(1277,3.940285e-06);
      resMatrix->SetBinContent(1278,2.061898e-05);
      resMatrix->SetBinContent(1279,2.883422e-05);
      resMatrix->SetBinContent(1280,6.442884e-05);
      resMatrix->SetBinContent(1281,0.0002405388);
      resMatrix->SetBinContent(1282,0.0004676711);
      resMatrix->SetBinContent(1283,0.0006760401);
      resMatrix->SetBinContent(1284,0.0006613755);
      resMatrix->SetBinContent(1285,0.002669605);
      resMatrix->SetBinContent(1286,0.003485172);
      resMatrix->SetBinContent(1287,0.006748383);
      resMatrix->SetBinContent(1288,0.010596);
      resMatrix->SetBinContent(1289,0.1296566);
      resMatrix->SetBinContent(1290,0.7367081);
      resMatrix->SetBinContent(1291,0.06629995);
      resMatrix->SetBinContent(1292,0.0007877786);
      resMatrix->SetBinContent(1294,0.001125895);
      resMatrix->SetBinContent(1300,0.0001594388);
      resMatrix->SetBinContent(1308,3.667974e-05);
      resMatrix->SetBinContent(1317,1.354512e-05);
      resMatrix->SetBinContent(1318,8.934786e-07);
      resMatrix->SetBinContent(1319,2.955214e-06);
      resMatrix->SetBinContent(1320,1.030949e-05);
      resMatrix->SetBinContent(1322,0.0001288577);
      resMatrix->SetBinContent(1323,0.0001603592);
      resMatrix->SetBinContent(1324,0.0001169178);
      resMatrix->SetBinContent(1325,0.000405624);
      resMatrix->SetBinContent(1326,0.0004960317);
      resMatrix->SetBinContent(1327,0.001848031);
      resMatrix->SetBinContent(1328,0.003236231);
      resMatrix->SetBinContent(1329,0.004294426);
      resMatrix->SetBinContent(1330,0.008633775);
      resMatrix->SetBinContent(1331,0.02076201);
      resMatrix->SetBinContent(1332,0.1479996);
      resMatrix->SetBinContent(1333,0.8027255);
      resMatrix->SetBinContent(1334,0.07405119);
      resMatrix->SetBinContent(1356,2.537041e-05);
      resMatrix->SetBinContent(1357,3.782434e-05);
      resMatrix->SetBinContent(1358,1.912265e-05);
      resMatrix->SetBinContent(1360,8.934786e-07);
      resMatrix->SetBinContent(1361,9.850712e-07);
      resMatrix->SetBinContent(1362,2.061898e-05);
      resMatrix->SetBinContent(1363,2.883422e-05);
      resMatrix->SetBinContent(1364,6.442884e-05);
      resMatrix->SetBinContent(1365,8.01796e-05);
      resMatrix->SetBinContent(1366,0.0001169178);
      resMatrix->SetBinContent(1367,0.000270416);
      resMatrix->SetBinContent(1368,0.0003306878);
      resMatrix->SetBinContent(1369,0.0002053368);
      resMatrix->SetBinContent(1370,0.0009957635);
      resMatrix->SetBinContent(1371,0.002453957);
      resMatrix->SetBinContent(1372,0.001177333);
      resMatrix->SetBinContent(1373,0.009321717);
      resMatrix->SetBinContent(1374,0.01256043);
      resMatrix->SetBinContent(1375,0.1071509);
      resMatrix->SetBinContent(1376,0.7736512);
      resMatrix->SetBinContent(1377,0.0573093);
      resMatrix->SetBinContent(1381,5.953587e-05);
      resMatrix->SetBinContent(1383,1.289713e-05);
      resMatrix->SetBinContent(1404,1.030949e-05);
      resMatrix->SetBinContent(1408,0.0001169178);
      resMatrix->SetBinContent(1409,0.000270416);
      resMatrix->SetBinContent(1410,0.0001653439);
      resMatrix->SetBinContent(1411,0.0006160105);
      resMatrix->SetBinContent(1412,0.0007468226);
      resMatrix->SetBinContent(1413,0.0003067447);
      resMatrix->SetBinContent(1414,0.001177333);
      resMatrix->SetBinContent(1415,0.004660858);
      resMatrix->SetBinContent(1416,0.007099371);
      resMatrix->SetBinContent(1417,0.0147991);
      resMatrix->SetBinContent(1418,0.1307866);
      resMatrix->SetBinContent(1419,0.8209133);
      resMatrix->SetBinContent(1420,0.05291708);
      resMatrix->SetBinContent(1423,5.953587e-05);
      resMatrix->SetBinContent(1445,3.940285e-06);
      resMatrix->SetBinContent(1446,2.061898e-05);
      resMatrix->SetBinContent(1449,8.01796e-05);
      resMatrix->SetBinContent(1450,0.0001169178);
      resMatrix->SetBinContent(1452,0.0001653439);
      resMatrix->SetBinContent(1454,0.0002489409);
      resMatrix->SetBinContent(1456,0.0007848886);
      resMatrix->SetBinContent(1457,0.0008474288);
      resMatrix->SetBinContent(1458,0.004914949);
      resMatrix->SetBinContent(1459,0.004735711);
      resMatrix->SetBinContent(1460,0.01811891);
      resMatrix->SetBinContent(1461,0.1058944);
      resMatrix->SetBinContent(1462,0.8604497);
      resMatrix->SetBinContent(1463,0.02872003);
      resMatrix->SetBinContent(1465,6.666536e-05);
      resMatrix->SetBinContent(1467,1.289713e-05);
      resMatrix->SetBinContent(1486,8.934786e-07);
      resMatrix->SetBinContent(1491,8.01796e-05);
      resMatrix->SetBinContent(1497,0.0003067447);
      resMatrix->SetBinContent(1499,0.0008474288);
      resMatrix->SetBinContent(1500,0.0005461054);
      resMatrix->SetBinContent(1501,0.001183928);
      resMatrix->SetBinContent(1502,0.0007877786);
      resMatrix->SetBinContent(1503,0.01069588);
      resMatrix->SetBinContent(1504,0.07770877);
      resMatrix->SetBinContent(1505,0.8908988);
      resMatrix->SetBinContent(1506,0.04086497);
      resMatrix->SetBinContent(1508,9.779369e-05);
      resMatrix->SetBinContent(1509,0.0001205965);
      resMatrix->SetBinContent(1510,0.0001594388);
      resMatrix->SetBinContent(1529,9.850712e-07);
      resMatrix->SetBinContent(1533,8.01796e-05);
      resMatrix->SetBinContent(1542,0.0005461054);
      resMatrix->SetBinContent(1544,0.0007886479);
      resMatrix->SetBinContent(1545,0.003885376);
      resMatrix->SetBinContent(1546,0.006796872);
      resMatrix->SetBinContent(1547,0.07484786);
      resMatrix->SetBinContent(1548,0.8853192);
      resMatrix->SetBinContent(1549,0.0783635);
      resMatrix->SetBinContent(1550,0.0005006794);
      resMatrix->SetBinContent(1551,1.289713e-05);
      resMatrix->SetBinContent(1581,2.826619e-06);
      resMatrix->SetBinContent(1583,7.808963e-06);
      resMatrix->SetBinContent(1584,1.00646e-05);
      resMatrix->SetBinContent(1585,2.181953e-05);
      resMatrix->SetBinContent(1586,2.177788e-05);
      resMatrix->SetBinContent(1587,0.001042951);
      resMatrix->SetBinContent(1588,0.000114125);
      resMatrix->SetBinContent(1589,0.002726167);
      resMatrix->SetBinContent(1590,0.06262239);
      resMatrix->SetBinContent(1591,0.844636);
      resMatrix->SetBinContent(1592,0.04157403);
      resMatrix->SetBinContent(1593,0.0001205965);
      resMatrix->SetBinContent(1594,0.0001594388);
      resMatrix->SetBinContent(1602,3.379998e-07);
      resMatrix->SetBinContent(1621,1.892156e-06);
      resMatrix->SetBinContent(1623,8.479856e-06);
      resMatrix->SetBinContent(1624,1.446532e-05);
      resMatrix->SetBinContent(1625,1.171345e-05);
      resMatrix->SetBinContent(1626,4.025842e-05);
      resMatrix->SetBinContent(1627,8.182323e-05);
      resMatrix->SetBinContent(1628,0.0001597045);
      resMatrix->SetBinContent(1629,0.0001790166);
      resMatrix->SetBinContent(1630,0.0006432502);
      resMatrix->SetBinContent(1631,0.001960465);
      resMatrix->SetBinContent(1632,0.008406883);
      resMatrix->SetBinContent(1633,0.06762668);
      resMatrix->SetBinContent(1634,0.8908074);
      resMatrix->SetBinContent(1635,0.02452514);
      resMatrix->SetBinContent(1655,9.850712e-07);
      resMatrix->SetBinContent(1662,1.82456e-07);
      resMatrix->SetBinContent(1664,4.587926e-06);
      resMatrix->SetBinContent(1665,6.769821e-07);
      resMatrix->SetBinContent(1666,1.851472e-05);
      resMatrix->SetBinContent(1667,5.774747e-06);
      resMatrix->SetBinContent(1668,1.307773e-05);
      resMatrix->SetBinContent(1669,4.145032e-05);
      resMatrix->SetBinContent(1670,5.690022e-05);
      resMatrix->SetBinContent(1671,6.442371e-05);
      resMatrix->SetBinContent(1672,0.0002368825);
      resMatrix->SetBinContent(1673,0.0008265644);
      resMatrix->SetBinContent(1674,0.002690949);
      resMatrix->SetBinContent(1675,0.008957417);
      resMatrix->SetBinContent(1676,0.06665705);
      resMatrix->SetBinContent(1677,0.9661477);
      resMatrix->SetBinContent(1678,0.0365912);
      resMatrix->SetBinContent(1708,4.330601e-07);
      resMatrix->SetBinContent(1709,4.675665e-07);
      resMatrix->SetBinContent(1710,6.026243e-07);
      resMatrix->SetBinContent(1712,8.693093e-07);
      resMatrix->SetBinContent(1713,6.431237e-06);
      resMatrix->SetBinContent(1714,7.454516e-06);
      resMatrix->SetBinContent(1715,2.007111e-05);
      resMatrix->SetBinContent(1716,6.138626e-05);
      resMatrix->SetBinContent(1717,0.0001568489);
      resMatrix->SetBinContent(1718,0.0003630384);
      resMatrix->SetBinContent(1719,0.009008647);
      resMatrix->SetBinContent(1720,0.9629305);
    }
  
  resMatrix->SetMaximum(1.000);
  if( logScaleZ ) resMatrix->SetMinimum(0.001);
  else            resMatrix->SetMinimum(0);
  resMatrix->GetZaxis()->SetNdivisions(ndivz);

  resMatrix->Draw("COLZSame");
      c_->Modified();
      if( drawGrid )
	{
	  draw_bin_grid( 15, 1500, 15, 1500, 0, kGray );
	  draw_bin_grid( 15, 1500, 15, 1500, 1, kGray );
	}
      draw_axis_labels( 14, 0.045,0 );
      draw_axis_labels( 14, 0.045,1 );

  {
    
    TLatex latex;
    latex.SetNDC();    
    latex.SetTextFont(txtFont[0]);
    latex.SetTextSize(txtSize[0]);    
    latex.SetTextAlign(txtAlign[0]); 
    latex.DrawLatex(txtX[0], txtY[0], txt[0]);
  }
  // redraw axis
  c_->RedrawAxis();
  c_->GetFrame()->Draw();

  return c_;
}

/*
//#include "Theory.C"
TCanvas* Theory()
{
  const int norders = 3;
  const int nbins  = 14;

  //per pdf within order
  TH1D* h_mstw[norders];
  TH1D* h_mstw_band[norders];

  //per pdf and per order
  double xsec_mass_mstw[nbins-1][norders];
  double xsec_err_mass_mstw[nbins-1][norders];
  
  //default masses
  double mass_xlow[nbins] = {
    15,20,30,40,50,60,76,86,96,106,120,150,200,600};

  TH1D* data1 = new TH1D("data1","data1",nbins-1,mass_xlow);
  TH1D* h = new TH1D("h","h",nbins-1,mass_xlow);

  //shape r
  double data_vals1[nbins-1] = 
    {
      1.239,
      0.552,
      0.144,
      0.0621,
      0.0305,
      0.0329,
      0.0580,
      0.8551,
      0.0422,
      0.0117,
      0.00883,
      0.00287,
      0.00146
    };
  double data_err1[nbins-1] = 
    {
      0.104,
      0.034,
      0.008,
      0.0035,
      0.0019,
      0.0017,
      0.0027,
      0.0243,
      0.0038,
      0.0012,
      0.00049,
      0.00051,
      0.00016
    };
  double th_err[nbins] = 
    {
      4.4,
      4.4,
      4.2,
      4.1,
      3.9,
      3.8,
      3.6,
      3.6,
      3.5,
      3.3,
      3.1,
      2.6
    };

  double rdata_vals1[nbins-1];
  double rdata_err1[nbins-1];

  double peak_bin_width = 60.;
  double peak_val_theory = 1009.0;

  //stream to get x-sec values and errors
  //LO
  ifstream in_xsec_massLO_mstw("root/Plain/LO/out_MSTW2008LO.txt",ios::in);

  //NLO
  ifstream in_xsec_massNLO_mstw("root/Plain/NLO/out_MSTW2008NLO.txt",ios::in);

  //NNLO
  ifstream in_xsec_massNNLO_mstw("root/Plain/NNLO/out_NNLO_MSTW2008NNLO.txt",ios::in);

  ostringstream pprint_ord;
  pprint_ord.str("");
  for( int iord = 0; iord < norders; iord++ ) 
    {
      pprint_ord << iord;
      string ordLabel = pprint_ord.str();

      h_mstw[iord]  = new TH1D("Cross section1"+TString(ordLabel),
			       "Cross section1"+TString(ordLabel),
			       nbins-1,mass_xlow);
      h_mstw_band[iord]  = new TH1D("Cross section1 band"+TString(ordLabel),
				    "Cross section1 band"+TString(ordLabel),
				    nbins-1,mass_xlow);

      //      cout << "order " << iord << endl;
    
      for( int i = 0; i < nbins-1; i++ ) 
	{
	  double dm_ = mass_xlow[i+1]-mass_xlow[i];

	  if (iord == 0) 
	    {
	      //LO  
	      in_xsec_massLO_mstw >> xsec_mass_mstw[i][0] >>  xsec_err_mass_mstw[i][0] ;
	      
	      //fill data1 histo 
	      rdata_vals1[i] = data_vals1[i]*peak_bin_width/dm_;
	      rdata_err1[i]  = data_err1[i]*peak_bin_width/dm_;
	      data1->SetBinContent(i+1,rdata_vals1[i]);
	      //	      data1->SetBinError(i+1,rdata_err1[i]);
	      data1->SetBinError(i+1,0);
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      in_xsec_massNLO_mstw >> xsec_mass_mstw[i][1] >>  xsec_err_mass_mstw[i][1] ;
	    } 
	  else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw >> xsec_mass_mstw[i][2] 
				    >>  xsec_err_mass_mstw[i][2] ;
	      h->SetBinContent(i+1,
			       xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow[i+1]-mass_xlow[i]));  // devide to the Z peak
	    }
	  
	  //check ok
	  //	  cout <<"bin " << i << xsec_mass_mstw[i][iord] << " " << xsec_err_mass_mstw[i][iord] << endl; 
	  //	  cout << "mass bin " << mass_xlow[i+1]-mass_xlow[i] << " " << i<< endl;

	  h_mstw[iord]->SetBinContent(i+1,xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band[iord]->SetBinContent(i+1,xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band[iord]->SetBinError(i+1,0.01*th_err[i]*h_mstw_band[iord]->GetBinContent(i+1));	  
	}
    }

  double fake_par[nbins-1] = {
    17.5,
    25.0,
    35.0,
    45.0,
    55.0,
    68.0,
    81.0,
    91.0,
    101.0,
    113.0,
    132.5,
    175.0,
    400.0
    //    17.0,
    //    24.0,
    //    34.3,
    //    44.3,
    //    54.4,
    //    68.0,
    //    81.2,
    //    91.0,
    //    100.4,
    //    113.3,
    //    132.4,
    //    173.5,
    //    392.2
  };

  TGraphAsymmErrors* grae = new TGraphAsymmErrors(nbins-1);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      double x_ = fake_par[ii];
      double y_ = rdata_vals1[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = rdata_err1[ii];
      double eyh_ = rdata_err1[ii];
      grae->SetPoint(ii,x_,y_);
      grae->SetPointError(ii,exl_,exh_,eyl_,eyh_);

    }

  // Voila! at this stage, on has
  // 1) one TGraph for the data
  TGraph* dataGraph   = grae;
  // 2) one histogram for the theory
  TH1* theoryHist     = h_mstw[2];
  // 2) one histogram for the theory error
  TH1* theoryBandHist = h_mstw_band[2];

  theoryHist->SetLineColor(kRed);
  theoryHist->SetLineWidth(2);
  theoryHist->SetMarkerSize(0);
  theoryHist->SetFillColor(kRed);
  
  theoryBandHist->SetLineColor(kGreen);
  theoryBandHist->SetLineWidth(2);
  theoryBandHist->SetMarkerSize(0);
  theoryBandHist->SetFillColor(kGreen);

  TH1* hfit = theoryHist;

  // calculate the ratio data/theory
  TGraphAsymmErrors* ratioGraph = 
    (TGraphAsymmErrors*) dataGraph->Clone("ratio");
  for( int ii=0; ii<dataGraph->GetN(); ii++ )
    {
      double x_, y_;
      dataGraph->GetPoint(ii,x_,y_ );
      //      ratioGraph->SetPointEYlow(ii,0);
      //      ratioGraph->SetPointEYhigh(ii,0);
      ratioGraph->SetPoint(ii,x_,0 );
      double eyl_ = dataGraph->GetErrorYlow(ii);
      double eyh_ = dataGraph->GetErrorYhigh(ii);
      int jj = hfit->FindBin(x_);
      float fit_ = hfit->GetBinContent( jj );
      //      float binWidth_ = hfit->GetBinWidth(jj);
      //      ratioGraph->SetPointEXlow(ii,binWidth_/2.);
      //      ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
      if( fit_>0 )
	{
	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
	}
      //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
      //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
      //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
    }

  // calculate the ratio band/fit
  TH1* theoryBandRatio = 
    (TH1*) theoryBandHist->Clone("ratioBand");
  for( int ii=1; ii<=theoryBandHist->GetNbinsX(); ii++ )
    {
      double y_(0), dy_(0);
      float fit_ =           hfit->GetBinContent(ii);  
      cout << ii << "/fit=" << fit_ << endl; 
      if( fit_>0 )
	{
	  y_  = theoryBandHist->GetBinContent(ii);
	  dy_ = theoryBandHist->GetBinError(ii);
	  theoryBandRatio->SetBinContent(ii,y_/fit_);
	  // theoryBandRatio->SetBinContent(ii,1.);
	  theoryBandRatio->SetBinError(ii,dy_/fit_);
	}
    }


  TGaxis::SetMaxDigits(3);

  float r0_ = 1.;
  float dr_ = 0.75;

//   // channels, ordered as in the legend
//   vector<TString> channels;  
//   vector<TString> hnames;
//   vector<TString> type;
//   vector<size_t>  order;  

//   map<TString,int> fillColor_;
//   map<TString,int> lineColor_;
//   int lineWidth1(1);
//   int lineWidth2(1);

//   if( salamanderColors )
//     {
//       lineWidth1 = 1;
//       lineWidth2 = 1;

//       fillColor_["Signal"] = kOrange-2;
//       lineColor_["Signal"] = kOrange+3;
      
//       fillColor_["Ztautau"] = kOrange+7;
//       lineColor_["Ztautau"] = kOrange+3;

//       fillColor_["VV"]     = kOrange+7;
//       lineColor_["VV"]     = kOrange+3;

//       fillColor_["EWK"] = kOrange+10;
//       lineColor_["EWK"] = kOrange+3;

//       fillColor_["QCD"] = kViolet-5;
//       lineColor_["QCD"] = kViolet+3;
      
//       fillColor_["ttbar"] = kRed+2;
//       lineColor_["ttbar"] = kRed+4;      

//       fillColor_["Y"] = kViolet-4;
//       lineColor_["Y"] = kViolet+3;
//     }
//   else
//     {
//       lineWidth1 = 1;
//       lineWidth2 = 1;

//       fillColor_["Signal"] = kSpring-3;
//       lineColor_["Signal"] = kSpring-3;
      
//       fillColor_["Ztautau"] = kOrange;
//       lineColor_["Ztautau"] = kOrange;
      
//       fillColor_["EWK"] = kCyan-4;
//       lineColor_["EWK"] = kCyan-4;
      
//       fillColor_["VV"] = kPink-1;
//       lineColor_["VV"] = kPink-1;
      
//       fillColor_["QCD"] =  kMagenta-3;
//       lineColor_["QCD"] =  kMagenta-3;
      
//       fillColor_["Y"]   =  kBlue+2;
//       lineColor_["Y"]   =  kBlue+2;
      
//       fillColor_["ttbar"] = kAzure;
//       lineColor_["ttbar"] = kAzure;
//     }

  // text2
  size_t ntxt = 3;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS preliminary";
  txtSize[0] = 0.055;
  txtX[0] = 0.915;
  txtY[0] = 0.935;
  txtAlign[0] = 31;
  txtFont[0] = 42;

  txt[1] = "36 pb^{-1}  at  #sqrt{s} = 8 TeV";
  txtSize[1] = 0.048;
  txtX[1] = 0.70;
  txtY[1] = 0.83;
  txtAlign[1] = 21;
  txtFont[1] = 42;
  
  txt[2] = "Z/#gamma* #rightarrow #mu#mu";
  txtSize[2] = 0.06;
  txtX[2] = 0.70;
  txtY[2] = 0.70;
  txtAlign[2] = 21;
  txtFont[2] = 42;
  
  // histogram limits, in linear and logarithmic
  int nbin_(0);
  float xmin_(0.), xmax_(0.); 
  float yminl_(0.), ymaxl_(0.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(510);
  int ndivy(510);
  float markerSize(1.2);
  float titleOffsetY(1.15);
  float titleSizeY(0.06);
  if( dataMCRatio )
    titleOffsetY=1.00;
  //  float titleOffsetX(1.00);

  // canvas name
  TString ctitle;

  // legend position and scale;
  float xl_(0);
  float yl_(0);
  float dxl_(0);
  float dyl_(0);
  int legendTextFont=42;
  float legendTextSize=0.040;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString cname;

  cname += "Fig_8";
  ctitle = "DY to mu-mu analysis - comparison with theory";
  xtitle = "M(#mu#mu)  [";
  xtitle += massUnit;       
  xtitle += "]";
  ytitle = "(#Delta M^{Z}_{#mu#mu}/#Delta M^{i}_{#mu#mu}) 1/#sigma_{Z} d#sigma/dM_{#mu#mu}"; 

  nbin_ = 100;
  xmin_ = 15.;
  xmax_ = 600.;
  ndivx = 0;
  ndivy = 506;
	  
  markerSize = 1.1;
    
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=400;

  yminl_ = 0.00005;
  ymaxl_ = 50.;

  xl_  = 0.65;
  yl_  = 0.42;
  dxl_ = 3.;
  dyl_ = 5.0;
  
  //  legendTextSize *= 1.1;
       
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin( 110./600 );
  c_->SetRightMargin( 42./600 );
  c_->SetTopMargin(  40./600 );
  c_->SetBottomMargin( 80./600 ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);

  Double_t scale = 4;
  Double_t wbin = 42*scale;
  Double_t left  = 8*scale;
  Double_t right = 5*scale;
  Double_t h1 = 135*scale;
  Double_t h2 = 45*scale;
  Double_t top1 = 15*scale;
  Double_t bot1 = 3*scale;
  Double_t top2 = 3*scale;
  //  Double_t bot1 = 0*scale;
  //  Double_t top2 = 0*scale;
  Double_t bot2 = 80*scale;
  Double_t W = left + wbin + right;
  Double_t H = h1 + h2;
  Double_t s[2] = {1, h1/h2 };

  TPad* pad[2];
  pad[0] = new TPad( "top", "top", 
		     0, h2/H, 1, 1,
		     kWhite,0,0);
  pad[0]->SetLeftMargin(  left/W );
  pad[0]->SetRightMargin( right/W );
  pad[0]->SetTopMargin(  top1/H );
  pad[0]->SetBottomMargin( bot1/H );

  pad[1] = new TPad( "bottom", "bottom", 
		     0, 0, 1, h2/H,
		     kWhite,0,0);
  pad[1]->SetLeftMargin(  left/W );
  pad[1]->SetRightMargin( right/W );
  pad[1]->SetTopMargin(  top2/H );
  pad[1]->SetBottomMargin( bot2/H );
  //  pad[1]->SetGridy();

  for( int ii=0; ii<2; ii++ )
    {
      pad[ii]->SetFillColor(0);
      pad[ii]->SetTickx(1);
      pad[ii]->SetTicky(1);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
    }


//   //  return c_;
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon6", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  //  ax_->SetTitleOffset(titleOffsetX);

  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(99);
  

  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);


  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetLabelOffset(0.008);

  dataGraph->SetName("dataGraph");
  dataGraph->SetMarkerStyle(kFullCircle);
  dataGraph->SetMarkerColor(kBlack);
  dataGraph->SetMarkerSize(markerSize);
  
  TGraph* dummyGraph = (TGraph*) dataGraph->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);
    
//   TH1* hfit = totalHisto;
//   for( int ii=0; ii<dataGraph->GetN(); ii++ )
//     {
//       dataGraph->GetPoint(ii,x_,y_ );
//       ratioGraph->SetPointEYlow(ii,0);
//       ratioGraph->SetPointEYhigh(ii,0);
//       ratioGraph->SetPoint(ii,x_,0 );
//       double eyl_ = dataGraph->GetErrorYlow(ii);
//       double eyh_ = dataGraph->GetErrorYhigh(ii);
//       int jj = hfit->FindBin(x_);
//       float fit_ = hfit->GetBinContent( jj );
//       float binWidth_ = hfit->GetBinWidth(jj);
//       ratioGraph->SetPointEXlow(ii,binWidth_/2.);
//       ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
//       if( fit_>0 )
// 	{
// 	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
// 	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
// 	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
// 	}
//       //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
//       //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
//       //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
//     }
  
  TH1* hratio_ = (TH1*) h_->Clone("hratio");
  ax_ = hratio_->GetXaxis();
  ay_ = hratio_->GetYaxis();
  ax_->SetLabelOffset(99);
  ax_->SetTitleOffset(99);
  //  ay_->SetTitleSize(0.13);
  //  ay_->SetTitleOffset(0.45);
  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

 
  TPad* p_ = pad[0];
  p_->Draw();
  p_->cd();

  p_->SetLogy(true);
  p_->SetLogx(true);

  h_->GetYaxis()->SetRangeUser(yminl_,ymaxl_);
  h_->Draw("hist");
  //  draw_axis_labels(0.9*yminl_,0.050);

//   dxl_ *= scalel_;
//   dyl_ *= scalel_;
//   TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
//   legend->SetTextFont(legendTextFont);
//   legend->SetTextSize(legendTextSize);
//   legend->SetLineColor(0);
//   legend->SetFillColor(0);
  
//   legend->AddEntry(dummyGraph,"  data","PL");
//   legend->AddEntry(dummyGraph,"      ","0"); // skip a line
//   for( size_t jj=0; jj<nChan; jj++ ) 
//     {
//       size_t ii = order[jj];
//       if( !hnames[jj].Contains("NOT_IN_LEGEND") )
// 	legend->AddEntry(histos[ii],hnames[ii],"F");
//     }
//   legend->Draw("same");

//   if( stackHists )
//     {
//       //      stackedHisto->Draw("samehist");
//       //      for( size_t jj=0; jj<nChan; jj++ )
//       for( size_t jj=0; jj<nChan; jj++ )
// 	{
// 	  size_t kk = nChan-jj-1;
// 	  size_t ii = order[jj];
// 	  if( hnames[ii].Contains("NOT_IN_LEGEND") ) continue;
// 	  TH1* hh_= listOfStackedHists[kk];
// 	  //	  cout << jj << " " << hh_->GetName() << endl;
// 	  hh_->SetLineWidth(1);
// 	  hh_->Draw("Samehist");
// 	  if( GHMStyle ) draw_mass_bins( hh_, yminl_ );
// 	}
//     }
//   else
//     {      
//       //       for( size_t jj=0; jj<nChan; jj++ ) 
//       // 	{
//       // 	  size_t ii = order[jj];
//       // 	  if( !hnames[ii].Contains("NOT_IN_LEGEND") )
//       // 	    histos[ii]->Draw("samehist");
//       // 	}
//     }

//   if( GHMStyle )
//     totalHisto->Draw("samehist");
  draw_bin_grid( yminl_, 1.3*yminl_ );
  draw_bin_grid( ymaxl_/1.3, ymaxl_ );

  bool drawHGrid = true;
  if( drawHGrid )
    {
      TLine grid_;
      grid_.SetLineColor(kGray+2);
      grid_.SetLineWidth(1);
      float y_;
      //      grid_.SetLineStyle(kSolid);
      grid_.SetLineStyle(kDotted);
      y_=0.0001;  grid_.DrawLine(120,y_,xmax_,y_);
      y_=0.001;   grid_.DrawLine(120,y_,xmax_,y_);
      y_=0.01;    grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=0.1;     grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=1;       grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=10;      grid_.DrawLine(xmin_,y_,60,y_);
      //      y_=0.9;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      y_=0.8;  grid_.DrawLine(xmin_,y_,xmax_,y_);
    }

    //    theoryHist->Draw("E3Same");
    theoryBandHist->Draw("E3Same");
    h->SetLineColor(kGreen+1);
    h->Draw("Lsame");
  //  h->SetLineColor(kGreen+1);
  //data1->SetMarkerSize(0);
  //data1->Draw("ESame");
    gStyle->SetEndErrorSize(3);
    
    dataGraph->Draw("PE");

//   //  h_->Draw("Same");

    xl_ = 0.2;
    yl_ = 0.12;
    dxl_ = 0.25;
    dyl_ = 0.13;

    // legend position and scale;
    TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
    legend->SetTextFont(legendTextFont);
    legend->SetTextSize(legendTextSize);
    legend->SetLineColor(0);
    legend->SetFillColor(0);

    legend->AddEntry(dummyGraph," data","PL");
    legend->AddEntry(theoryBandHist," NLO, FEWZ+CT10","F");
    
    legend->Draw("Same");

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  p_->RedrawAxis();
  p_->GetFrame()->Draw();
  c_->cd();
  
  p_ = pad[1];
  p_->SetLogx();
  p_->Draw();
  p_->cd();

  TAxis* xratio_ = hratio_->GetXaxis();
  TAxis* yratio_ = hratio_->GetYaxis();

  yratio_->SetRangeUser(r0_-0.9999*dr_,r0_+0.9999*dr_);
  yratio_->SetLabelSize( s[1]*yratio_->GetLabelSize() );
  yratio_->SetTitleSize( 0.13 );
  //yratio_->SetTitleSize( s[1]*yratio_->GetTitleSize() );
  //  yratio_->SetLabelOffset( yratio_->GetLabelOffset() );
  yratio_->SetLabelOffset( 0.012 );
  //  yratio_->SetTitleOffset( yratio_->GetTitleOffset()/s[1] );
  yratio_->SetTitleOffset( 0.45 );
  yratio_->SetTitle("data/theory");
  yratio_->SetNdivisions(3);
  
  xratio_->SetLabelSize( s[1]*xratio_->GetLabelSize() );
  xratio_->SetTitleSize( s[1]*xratio_->GetTitleSize() );
  xratio_->SetTitleOffset( 1.0 );
  //   //  xratio_->CenterTitle();
  xratio_->SetLabelOffset(99);
  xratio_->SetTickLength( xratio_->GetTickLength()*s[1] );
  
  hratio_->Draw();
  //theoryBandRatio->Draw("E3Same");
  theoryBandRatio->Draw("E2Same"); //!!!
  draw_axis_labels(r0_-1.15*dr_,0.15);
  draw_bin_grid( r0_-dr_, r0_+dr_ );
  {  
    TLine grid_;
    grid_.SetLineColor(kGray+2);
    grid_.SetLineWidth(1);
    float y_;
    grid_.SetLineStyle(kDashed);
    y_=1;  grid_.DrawLine(xmin_,y_,xmax_,y_);
  }

  
  ratioGraph->SetMarkerSize( ratioGraph->GetMarkerSize()*1. );
  ratioGraph->SetLineColor( kBlack );
  ratioGraph->SetLineWidth( 1 );
  
  ratioGraph->SetMarkerColor( kViolet );
  ratioGraph->SetMarkerStyle( kFullCircle );
  ratioGraph->DrawClone("PE");
  ratioGraph->SetMarkerColor( kBlack );
  ratioGraph->SetMarkerStyle( kOpenCircle );
  ratioGraph->DrawClone("PE");


   p_->RedrawAxis();
   p_->GetFrame()->Draw();
   
   c_->cd();
   return c_;
}
*/

void draw_bin_grid( float ymin, float ymax, float xmin, float xmax, int iV, int iCol )
{
  TLine grid_;
  grid_.SetLineColor(iCol);
  grid_.SetLineStyle(kSolid);
  float lim_[39] = {
    20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
    96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
    200, 220, 243, 273, 320, 380, 440, 510, 600, 1000};
  if( iV==1 )
    {
      for( size_t ii=0; ii<39; ii++ )
	{
	  if( lim_[ii]>xmin&&lim_[ii]<xmax )
	    grid_.DrawLine(lim_[ii],ymin,lim_[ii],ymax);
	}
    }
  else
    {
      for( size_t ii=0; ii<39; ii++ )
	{
	  if( lim_[ii]>ymin&&lim_[ii]<ymax )
	    grid_.DrawLine(xmin, lim_[ii],xmax,lim_[ii]);
	}
    }
}


void draw_mass_bins( TH1* data, float ymin ) 
{
  //  return;
  for (int bin=1; bin <= data->GetNbinsX(); bin++) 
    {
      float x1 = data -> GetBinLowEdge(bin);
      float x2 = data -> GetBinLowEdge(bin+1);
      float y = data -> GetBinContent(bin);
      if( y<ymin ) continue;
      //     if (y < data -> GetBinContent(bin+1)) y = data -> GetBinContent(bin+1);
      //    float ymin = 0.17; // pow(10., c1->GetY1());
      //    if (large_first_bin) ymin = 1.8;
      int color = data->GetLineColor();
      int width = data->GetLineWidth();
      width = 1;
      TLine* line;
      if( bin!=1 )
	{
	  line = new TLine(x1, ymin, x1, y);
	  line -> SetLineStyle(kSolid);
	  line -> SetLineColor(color);
	  line -> SetLineWidth(width);
	  line -> DrawClone();
	}
      if( bin!=data->GetNbinsX() )
	{
	  line = new TLine(x2, ymin, x2, y);
	  line -> SetLineStyle(kSolid);
	  line -> SetLineColor(color);
	  line -> SetLineWidth(width);
	  line -> DrawClone();
	}
      line = new TLine(x1, y, x2, y);
      line -> SetLineStyle(kSolid);
      line -> SetLineColor(color);
      line -> SetLineWidth(width);
      line -> DrawClone();      
    }
}

void draw_axis_labels( float pos, float size, int iV )
{
  TLatex* txt_ = new TLatex();
  // draw axis labels
  //  float xlab_ = 65;
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
  float mass[7] = {15,30,60,120,240,600,1500};
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

void setGrayScalePalette( int ncol, int nwhite )
{
  // my grayscale...
  TColor* myCol;
  //  const int ncol    =20;
  //  const int nwhite  = 1;
  const int ncoltot = ncol+nwhite;
  Int_t palette[ncoltot];
  int colOff7 = 12345;
  double dcol = 1./ncol;
  double gray = 1;

  for (int j = 0; j <ncoltot ; j++) {

    TString myColName("myCol_");
    int i = colOff7+j;
    myColName += i;
    myCol = new TColor(i,gray,gray,gray,myColName);
    cout << i << endl;
    myCol->Print();
    palette[j] = i;
    if( j>=nwhite ) gray -= dcol;
  }
  gStyle->SetPalette(ncol,palette);
}

void setBlueToRedPalette( const int ncoltot, int nwhite )
{

  // my color scale from blue to red
  TColor* myCol;
  //   const int ncol    = 10;
  //   const int nwhite  = 2;
  //   //  const int ncoltot = 2*(ncol+nwhite)+1;
  //   const int ncoltot = 25;
  Int_t palette[ncoltot];

  int ncol = ( ncoltot-1 ) /2 - nwhite; 

  //  int colOff7 = 67890;
  int colOff7 = 8500;
  double dcol = 1./ncol;
  double red   = 0;
  double green = 0;
  double blue  = 1;

  int jj = 0;
  for (int j = -ncol-nwhite; j <=ncol+nwhite ; j++) {

    TString myColName("myBtoRCol_");
    int i = colOff7+j;
    myColName += i;
    myCol = new TColor(i,red,green,blue,myColName);
    cout << i << endl;
    myCol->Print();
    palette[jj] = i;
    if( j<-nwhite )
      {
	red    += dcol;
	green  += dcol;
	//	blue   -= dcol;
      }
    else if( j>=nwhite )
      {
	//	red    += dcol;
	green  -= dcol;
	blue   -= dcol;
      }
    jj++;
  }
  gStyle->SetPalette(ncoltot,palette);
}

void setHessPalette()
{

  const int nfix    = 5;
  
  const int ninter  = 10;
  int nstep = ninter+1;
  double step = acos(-1.)/nstep;

  const int ncoltot = (nfix-1)*ninter + nfix;

  TColor* myCol;
  Int_t palette[ncoltot];
  for( int i=0; i<ncoltot; i++ ) palette[i] = 1;

  // 1:black, 4:blue, 2:red, 5:yellow, 10:white
  int colfix[nfix] = { 1, 4, 2, 5, 10 };
  
  int colOff7 = 4300;
  int icol=colOff7; // new color number

  float red, green, blue;

  int ifix=0;
  for( int ii=0; ii<nfix; ii++ )
    {
      TString myColName("myHessCol_");
      myColName += icol;
      TColor* theCol = (TColor*) gROOT->GetColor( colfix[ii] );
      theCol->GetRGB( red, green, blue );
      myCol = new TColor(icol,red,green,blue,myColName);
      cout << "ifix " << ifix << " r/g/b  " << red << "/" << green << "/" << blue << endl;
      palette[ifix] = icol++;
      ifix += nstep;
    }

  float r1, g1, b1;
  float r2, g2, b2;
  int ifix1 = 0;
  int ifix2 = 0;
  for( int ii=0; ii<nfix-1; ii++ )
    {
      ifix2 = ifix1 + nstep;
      
      int icol1 = palette[ifix1];
      int icol2 = palette[ifix2];
      TColor* col1 = gROOT->GetColor( icol1 );
      col1->Print();
      col1->GetRGB( r1, g1, b1 );
      TColor* col2 = gROOT->GetColor( icol2 );
      col2->Print();
      col2->GetRGB( r2, g2, b2 );

      ifix = ifix1;
      double x = -acos(-1.)/2.;
      for( int istep=0; istep<ninter; istep++ )
	{
	  x += step;
	  ifix++;

	  double sinx = TMath::Sin(x);	  
	  red   = 0.5*( (r2-r1)*sinx + (r1+r2) );
	  green = 0.5*( (g2-g1)*sinx + (g1+g2) );
	  blue  = 0.5*( (b2-b1)*sinx + (b1+b2) );

	  TString myColName("myHessCol_");
	  myColName += icol;
	  myCol = new TColor(icol,red,green,blue,myColName);
	  cout << "ifix " << ifix << " r/g/b  " << red << "/" << green << "/" << blue << endl;
	  palette[ifix] = icol++;

	}

      ifix1 = ifix2;
    }  

  gStyle->SetPalette(ncoltot,palette);
}

//#include "Theory.C"
TCanvas* Theory_Hist()
{
  const int norders = 3;
  const int nbins  = 14;
  const int nbins2  = 29;
  const int nbins3  = 84;
  const int nbins4  = 339;
  //const int nbins4= 305;
  //const int nbins4  = 200;

  //per pdf within order
  TH1D* h_mstw[norders];
  TH1D* h_mstw2[norders];
  TH1D* h_mstw3[norders];
  TH1D* h_mstw4[norders];
  TH1D* h_mstw_band[norders];
  TH1D* h_mstw_band2[norders];
  TH1D* h_mstw_band3[norders];
  TH1D* h_mstw_band4[norders];

  //per pdf and per order
  double xsec_mass_mstw[nbins-1][norders];
  double xsec_err_mass_mstw[nbins-1][norders];

  double xsec_mass_mstw2[nbins2-1][norders];
  double xsec_err_mass_mstw2[nbins2-1][norders];
  
  double xsec_mass_mstw3[nbins3-1][norders];
  double xsec_err_mass_mstw3[nbins3-1][norders];
  
  double xsec_mass_mstw4[nbins4-1][norders];
  double xsec_err_mass_mstw4[nbins4-1][norders];
  
  //default masses
  double mass_xlow[nbins] = {
    15,20,30,40,50,60,76,86,96,106,120,150,200,600};

  double mass_xlow2[nbins2] = {15.0,20.0,25.,30.0,35.0,40.0,45.0,50.0,55.0,60.0,76.0,86.0,96.0,106.0,120.0,125.0,130.0,135.0,140.0,145.0,150.0,155.0,160.0,165.0,170.0,180.0,190.0,200.0,600.};

  double mass_xlow3[nbins3] = {15.0,20.0,25.,30.0,35.0,40.0,45.0,50.0,55.0,
         60,61,62,63,64,65,66,67,68,69,
	 70,71,72,73,74,75,76,77,78,79,
	 80,81,82,83,84,85,86,87,88,89,
	 90,91,92,93,94,95,96,97,98,99,
	 100,101,102,103,104,105,106,107,108,109,
	 110,111,112,113,114,115,116,117,118,119,
	 120.0,125.0,130.0,135.0,140.0,145.0,150.0,155.0,160.0,165.0,170.0,180.0,190.0,200.0,600.};
/*
  double mass_xlow4[nbins4] = {15.0, 
17.0, 
19.0, 
21.0, 
22.0, 
24.0, 
26.0, 
28.0, 
30.0, 
33.0, 
36.0, 
39.0, 
42.0, 
45.0, 
48.0, 
51.0, 
54.0, 
57.0, 
60.0, 
63.0, 
66.0, 
69.0, 
72.0, 
75.0, 
78.0, 
80.0, 
82.0, 
84.0, 
86.0, 
88.0, 
90.0, 
91.0, 
92.0, 
94.0, 
96.0, 
98.0, 
100.0, 
103.0, 
106.0, 
109.0, 
112.0, 
115.0, 
118.0, 
119, 
120, 
121, 
122, 
123, 
124, 
125, 
126, 
127, 
128, 
129, 
130, 
131, 
132, 
133, 
134, 
135, 
136, 
137, 
138, 
139, 
140, 
141, 
142, 
143, 
144, 
145, 
146, 
147, 
148, 
149, 
150, 
151, 
152, 
153, 
154, 
155, 
156, 
157, 
158, 
159, 
160, 
162, 
164, 
166, 
168, 
170, 
172, 
174, 
176, 
178, 
180, 
182, 
184, 
186, 
188, 
190, 
192, 
194, 
196, 
198, 
200, 
202, 
204, 
206, 
208, 
210, 
212, 
214, 
216, 
218, 
220, 
222, 
224, 
226, 
228, 
230, 
232, 
234, 
236, 
238, 
240, 
242, 
244, 
246, 
248, 
250, 
252, 
254, 
256, 
258, 
260, 
262, 
264, 
266, 
268, 
270, 
272, 
274, 
276, 
278, 
280, 
282, 
284, 
286, 
288, 
290, 
292, 
294, 
296, 
298, 
300, 
302, 
304, 
306, 
308, 
310, 
312, 
314, 
316, 
318, 
320, 
322, 
324, 
326, 
328, 
330, 
332, 
334, 
336, 
338, 
340, 
342, 
344, 
346, 
348, 
350, 
352, 
354, 
356, 
358, 
360, 
362, 
364, 
366, 
368, 
370, 
372, 
374, 
376, 
378, 
380, 
382, 
384, 
386, 
388, 
390, 
392, 
394, 
396, 
398, 
400, 
402, 
404, 
406, 
408, 
410, 
412, 
414, 
416, 
418, 
420, 
422, 
424, 
426, 
428, 
430, 
432, 
434, 
436, 
438, 
440, 
442, 
444, 
446, 
448, 
450, 
452, 
454, 
456, 
458, 
460, 
462, 
464, 
466, 
468, 
470, 
472, 
474, 
476, 
478, 
480, 
482, 
484, 
486, 
488, 
490, 
492, 
494, 
496, 
498, 
500, 
502, 
504, 
506, 
508, 
510, 
512, 
514, 
516, 
518, 
520, 
522, 
524, 
526, 
528, 
530, 
532, 
534, 
536, 
538, 
540, 
542, 
544, 
546, 
548, 
550, 
552, 
554, 
556, 
558, 
560, 
562, 
564, 
566, 
568, 
570, 
572, 
574, 
576, 
578, 
580, 
582, 
584, 
586, 
588, 
590, 
592, 
594, 
596, 
598, 
600};
*/

  double initial = 14.0;
  double mass_xlow4[nbins4] = {0};
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
    else {
      initial += 2.0;
    }
    mass_xlow4[i] = initial;
    //cout << "mass_xlow4[" << i << "] = " << mass_xlow4[i] << endl;
  }

  /*
  double th_err4[nbins4-1] = {
     4.4,4.4,4.3,4.3,4.2,4.2,4.1,4.0,3.9,
     3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,
     3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3, 
     3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2, // 120
     3.1,3.1,3.1,3.1,3.1,3.0,3.0,3.0,3.0,3.0,
     3.0,3.0,3.0,3.0,3.0,2.9,2.9,2.9,2.9,2.9,
     2.9,2.9,2.9,2.9,2.9,2.8,2.8,2.8,2.8,2.8,
     2.8,2.8,2.8,2.8,2.8,2.7,2.7,2.7,2.7,2.7, // 160
     2.7,2.7,2.7,2.7,2.7,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6
     };
     */

  double th_uncert1p[13] = {4.3, 3.6, 2.7, 2.1, 1.6, 0.9, 0.2, 0.1, 0.2, 0.5, 0.9, 1.5, 2.8};
  double th_uncert1m[13] = {3.3, 2.8, 2.3, 1.9, 1.5, 0.9, 0.2, 0.1, 0.2, 0.5, 0.9, 1.6, 2.9};
  double th_uncert2p[13] = {2.5, 1.9, 3.1, 2.4, 2.6, 2.0, 2.1, 1.8, 2.8, 2.6, 2.5, 2.0, 1.8};
  double th_uncert2m[13] = {2.7, 3.6, 2.1, 2.5, 2.0, 2.4, 2.5, 2.2, 2.0, 2.2, 1.7, 1.8, 2.1};
  double th_err4[nbins4-1] = {0.0};
  double add_mass = 15.0;
  int inbins = 0;
  for( int i = 0; i < nbins4-1; i++ ) {
    if( i >= 0 && i < 11 ) {
      add_mass += 1.0;
    }
    else if( i >= 11 && i < 18 ) {
      add_mass += 5.0;
    }
    else if( i >= 18 && i < 118 ) {
      add_mass += 1.0;
    }
    else {
      add_mass += 2.0;
    }

    // inbins
    if( add_mass >=  mass_xlow[inbins] && add_mass <  mass_xlow[inbins+1] ) {
    }
    else inbins++;

    double errp = sqrt(th_uncert1p[inbins]*th_uncert1p[inbins] + th_uncert2p[inbins]*th_uncert2p[inbins]);
    double errm = sqrt(th_uncert1m[inbins]*th_uncert1m[inbins] + th_uncert2m[inbins]*th_uncert2m[inbins]);
    if( errp > errm ) th_err4[i] = errp;
    else th_err4[i] = errm;
//    cout << "err[" << i << "] = " << add_mass << " "  << " " << inbins << " " << mass_xlow[inbins] << " " << th_err4[i] << endl;
  }


  TH1D* data1 = new TH1D("data1","data1",nbins-1,mass_xlow);
  TH1D* data2 = new TH1D("data2","data2",nbins-1,mass_xlow);
  TH1D* h = new TH1D("h","h",nbins-1,mass_xlow);
  TH1D* h_2 = new TH1D("h_2","h_2",nbins2-1,mass_xlow2);
  TH1D* h_3 = new TH1D("h_3","h_3",nbins3-1,mass_xlow3);
  TH1D* h_4 = new TH1D("h_4","h_4",nbins4-1,mass_xlow4);

  //shape r
  // old (5/10/2011)
  //double data_vals1[nbins-1] = {0.183, 0.056, 0.0148, 0.0058, 0.0030, 0.0021, 0.0058, 0.0851, 0.0044, 0.00089, 0.00025, 0.000052, 0.0000042};
  //double data_err1[nbins-1] = {0.016, 0.003, 0.0007, 0.0003, 0.0002, 0.0001, 0.0003, 0.0019, 0.0003, 0.00008, 0.00002, 0.000008, 0.0000007};
  double data_vals1[nbins-1] = {0.1526, 0.0539, 0.0146, 0.00591, 0.00298, 0.00206, 0.00580, 0.08555, 0.00415, 0.000864, 0.000254, 0.0000509, 0.00000392};
  double data_err1[nbins-1] = {0.0133, 0.0032, 0.0008, 0.00033, 0.00019, 0.00010, 0.00030, 0.00235, 0.00033, 0.000083, 0.000024, 0.0000091, 0.00000078};

  double data_vals2[nbins-1] = {0.5055, 0.5560, 0.1299, 0.0469, 0.0271, 0.0354, 0.0639, 0.8292, 0.0527, 0.0134, 0.0054, 0.0023, 0.0017};
  double data_err2[nbins-1] = {0.1912, 0.0741, 0.0141, 0.0050, 0.0031, 0.0036, 0.0158, 0.0270, 0.0075, 0.0018, 0.0010, 0.0006, 0.0004};
  double th_err[nbins] = 
    {
      4.4,
      4.4,
      4.2,
      4.1,
      3.9,
      3.8,
      3.6,
      3.6,
      3.5,
      3.3,
      3.1,
      2.6
    };
  for( int i = 0; i < nbins; i++ ) {
    double errp = sqrt(th_uncert1p[i]*th_uncert1p[i] + th_uncert2p[i]*th_uncert2p[i]);
    double errm = sqrt(th_uncert1m[i]*th_uncert1m[i] + th_uncert2m[i]*th_uncert2m[i]);
    if( errp > errm ) th_err[i] = errp;
    else th_err[i] = errm;
    cout << "th_err = " << i << " " << th_err[i] << endl;
  }

  double th_err2[nbins2] = {4.4,4.4,4.3,4.3,4.2,4.2,4.1,4.0,3.9,3.8,3.6,3.6,3.5,3.3,3.1,3.0,3.0,2.9,2.9,2.8,2.8,2.7,2.7,2.7,2.6,2.6,2.6,2.6,2.8};

  double th_err3[nbins3] = {4.4,4.4,4.3,4.3,4.2,4.2,4.1,4.0,3.9,
     3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,
     3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,
     3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,
     3.1,3.0,3.0,2.9,2.9,2.8,2.8,2.7,2.7,2.7,2.6,2.6,2.6,2.6,2.8};

  // old (5/10/2011)
  //double err_model[nbins-1] = {0.166, 0.038, 0.054, 0.026, 0.037, 0.044, 0.038, 0.028, 0.058, 0.042, 0.027, 0.079, 0.146};
  double err_model[nbins-1] = {0.070, 0.057, 0.021, 0.0052, 0.021, 0.030, 0.041, 0.026, 0.019, 0.023, 0.0004, 0.011, 0.025};

  double rdata_vals1[nbins-1];
  double rdata_err1[nbins-1];
  double rdata_vals2[nbins-1];
  double rdata_err2[nbins-1];

  double peak_bin_width = 1.;
  double peak_val_theory = 1.; //unnormalized //1009.0*1.05;

  //stream to get x-sec values and errors
  //LO
  ifstream in_xsec_massLO_mstw("root/Plain/LO/out_MSTW2008LO.txt",ios::in);

  //NLO
  ifstream in_xsec_massNLO_mstw("root/Plain/NLO/out_MSTW2008NLO.txt",ios::in);

  //NNLO
  ifstream in_xsec_massNNLO_mstw("root/Plain/NNLO/out_NNLO_MSTW2008NNLO.txt",ios::in);

  //NNLO 2
  ifstream in_xsec_massNNLO_mstw2("root/Plain/NNLO/semifinaloutNNLO_PlainMSTW2008NNLO.txt",ios::in);

  //NNLO 3
  ifstream in_xsec_massNNLO_mstw3("root/Plain/NNLO/NNLO_MSTW_1GeV.txt",ios::in);
  ifstream in_xsec_massNNLO_mstw5("root/Plain/NNLO/nnlo.txt",ios::in);

  //NNLO 4
  ifstream in_xsec_massNNLO_mstw4("root/Plain/NNLO/8TeV_CTEQ12NNLO.txt",ios::in);

  ostringstream pprint_ord;
  pprint_ord.str("");
  for( int iord = 0; iord < norders; iord++ ) 
    {
      pprint_ord << iord;
      string ordLabel = pprint_ord.str();

      h_mstw[iord]  = new TH1D("Cross section1"+TString(ordLabel),
			       "Cross section1"+TString(ordLabel),
			       nbins-1,mass_xlow);
      h_mstw2[iord]  = new TH1D("Cross section1 2"+TString(ordLabel),
			       "Cross section1 2"+TString(ordLabel),
			       nbins2-1,mass_xlow2);
      h_mstw3[iord]  = new TH1D("Cross section1 3"+TString(ordLabel),
			       "Cross section1 3"+TString(ordLabel),
			       nbins3-1,mass_xlow3);
      h_mstw4[iord]  = new TH1D("Cross section1 4"+TString(ordLabel),
			       "Cross section1 4"+TString(ordLabel),
			       nbins4-1,mass_xlow4);
      
      h_mstw_band[iord]  = new TH1D("Cross section1 band"+TString(ordLabel),
				    "Cross section1 band"+TString(ordLabel),
				    nbins-1,mass_xlow);
      h_mstw_band2[iord]  = new TH1D("Cross section1 band2"+TString(ordLabel),
				    "Cross section1 band2"+TString(ordLabel),
				    nbins2-1,mass_xlow2);
      h_mstw_band3[iord]  = new TH1D("Cross section1 band3"+TString(ordLabel),
				    "Cross section1 band3"+TString(ordLabel),
				    nbins3-1,mass_xlow3);
      h_mstw_band4[iord]  = new TH1D("Cross section1 band4"+TString(ordLabel),
				    "Cross section1 band4"+TString(ordLabel),
				    nbins4-1,mass_xlow4);


      //      cout << "order " << iord << endl;
    
      for( int i = 0; i < nbins-1; i++ ) 
	{
	  double dm_ = mass_xlow[i+1]-mass_xlow[i];

	  if (iord == 0) 
	    {
	      //LO  
	      in_xsec_massLO_mstw >> xsec_mass_mstw[i][0] >>  xsec_err_mass_mstw[i][0] ;
	      
	      //fill data1 histo 
	      rdata_vals1[i] = data_vals1[i]*peak_bin_width;
	      rdata_err1[i]  = data_err1[i]*peak_bin_width;
	      data1->SetBinContent(i+1,rdata_vals1[i]);
	      //	      data1->SetBinError(i+1,rdata_err1[i]);
	      data1->SetBinError(i+1,0);

	      rdata_vals2[i] = data_vals2[i]*peak_bin_width;
	      rdata_err2[i]  = data_err2[i]*peak_bin_width;
	      data2->SetBinContent(i+1,rdata_vals2[i]);
	      //	      data2->SetBinError(i+1,rdata_err2[i]);
	      data2->SetBinError(i+1,0);
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      in_xsec_massNLO_mstw >> xsec_mass_mstw[i][1] >>  xsec_err_mass_mstw[i][1] ;
	    } 
	  else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw >> xsec_mass_mstw[i][2] 
				    >>  xsec_err_mass_mstw[i][2] ;
	      h->SetBinContent(i+1,
			       xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow[i+1]-mass_xlow[i]));  // devide to the Z peak
	    }
	  
	  //check ok
	  //	  cout <<"bin " << i << xsec_mass_mstw[i][iord] << " " << xsec_err_mass_mstw[i][iord] << endl; 
	  //	  cout << "mass bin " << mass_xlow[i+1]-mass_xlow[i] << " " << i<< endl;

	  h_mstw[iord]->SetBinContent(i+1,xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band[iord]->SetBinContent(i+1,xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band[iord]->SetBinError(i+1,0.01*th_err[i]*h_mstw_band[iord]->GetBinContent(i+1));	  
	  //cout << "band = " << xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_ << " " << 0.01*th_err[i]*h_mstw_band[iord]->GetBinContent(i+1) << " " << th_err[i] << endl;
	}

      for( int i = 0; i < nbins2-1; i++ ) 
	{
	  double dm_ = mass_xlow2[i+1]-mass_xlow2[i];

	  if (iord == 0) 
	    {
	      //LO  
//	      in_xsec_massNNLO_mstw2 >> xsec_mass_mstw2[i][0] 
//				    >>  xsec_err_mass_mstw2[i][0] ;
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      //in_xsec_massNNLO_mstw2 >> xsec_mass_mstw2[i][1] 
//				    >>  xsec_err_mass_mstw2[i][1] ;
	    } 
	    else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw2 >> xsec_mass_mstw2[i][2] 
				    >>  xsec_err_mass_mstw2[i][2] ;
	      h_2->SetBinContent(i+1,
			       xsec_mass_mstw2[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow2[i+1]-mass_xlow2[i]));  // devide to the Z peak
	    }
	  
	  	  //cout <<"bin2 " << i << " " << xsec_mass_mstw2[i][iord] << " " << xsec_err_mass_mstw2[i][iord] << endl; 
	  	  //cout << "mass bin2 " << mass_xlow2[i+1]-mass_xlow2[i] << " " << i<< endl;
	  h_mstw2[iord]->SetBinContent(i+1,xsec_mass_mstw2[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw2[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw2[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw2[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band2[iord]->SetBinContent(i+1,xsec_mass_mstw2[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band2[iord]->SetBinError(i+1,0.01*th_err2[i]*h_mstw_band2[iord]->GetBinContent(i+1));	  
	}

      for( int i = 0; i < nbins3-1; i++ ) 
	{
	  double dm_ = mass_xlow3[i+1]-mass_xlow3[i];

	  if (iord == 0) 
	    {
	      //LO  
//	      in_xsec_massNNLO_mstw3 >> xsec_mass_mstw3[i][0] 
//				    >>  xsec_err_mass_mstw3[i][0] ;
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      //in_xsec_massNNLO_mstw3 >> xsec_mass_mstw3[i][1] 
//				    >>  xsec_err_mass_mstw3[i][1] ;
	    } 
	    else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw3 >> xsec_mass_mstw3[i][2] 
				    >>  xsec_err_mass_mstw3[i][2] ;
	      h_3->SetBinContent(i+1,
			       xsec_mass_mstw3[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow3[i+1]-mass_xlow3[i]));  // devide to the Z peak
	    }
	  
	  	  //cout <<"bin3 " << i << " " << xsec_mass_mstw3[i][iord] << " " << xsec_err_mass_mstw3[i][iord] << endl; 
	  	  //cout << "mass bin3 " << mass_xlow3[i+1]-mass_xlow3[i] << " " << i<< endl;
	  h_mstw3[iord]->SetBinContent(i+1,xsec_mass_mstw3[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw3[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw3[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw3[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band3[iord]->SetBinContent(i+1,xsec_mass_mstw3[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band3[iord]->SetBinError(i+1,0.01*th_err3[i]*h_mstw_band3[iord]->GetBinContent(i+1));	  
	}

      for( int i = 0; i < nbins4-1; i++ ) 
	{
	  double dm_ = mass_xlow4[i+1]-mass_xlow4[i];

	  if (iord == 0) 
	    {
	      //LO  
//	      in_xsec_massNNLO_mstw4 >> xsec_mass_mstw4[i][0] 
//				    >>  xsec_err_mass_mstw4[i][0] ;
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      //in_xsec_massNNLO_mstw4 >> xsec_mass_mstw4[i][1] 
//				    >>  xsec_err_mass_mstw4[i][1] ;
	    } 
	    else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw4 >> xsec_mass_mstw4[i][2] 
				    >>  xsec_err_mass_mstw4[i][2] ;
	      h_4->SetBinContent(i+1,
			       xsec_mass_mstw4[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow4[i+1]-mass_xlow4[i]));  // devide to the Z peak
	    }
	  
	  	  //cout <<"bin4 " << i << " " << xsec_mass_mstw4[i][iord] << " " << xsec_err_mass_mstw4[i][iord] << endl; 
	  	  //cout << "mass bin4 " << mass_xlow4[i+1]-mass_xlow4[i] << " " << i<< endl;
	  h_mstw4[iord]->SetBinContent(i+1,xsec_mass_mstw4[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw4[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw4[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw4[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band4[iord]->SetBinContent(i+1,xsec_mass_mstw4[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band4[iord]->SetBinError(i+1,0.01*th_err4[i]*h_mstw_band4[iord]->GetBinContent(i+1));	  
	}
    }

    TH1D* herr1 = new TH1D("herr1","herr1",nbins-1,mass_xlow);
    for( int i = 0; i < nbins-1; i++ ) {
      double val1 = data1->GetBinContent(i+1)*err_model[i];
      double val2 = data1->GetBinContent(i+1)*(rdata_err1[i]/rdata_vals1[i]);
      double val = sqrt(val1*val1 + val2*val2);
      herr1->SetBinContent(i+1, data1->GetBinContent(i+1));
      herr1->SetBinError(i+1, val);
    }
    TH1D* herr2 = new TH1D("herr2","herr2",nbins-1,mass_xlow);
    for( int i = 0; i < nbins-1; i++ ) {
      double val1 = data2->GetBinContent(i+1)*err_model[i];
      double val2 = data2->GetBinContent(i+1)*(rdata_err2[i]/rdata_vals2[i]);
      double val = sqrt(val1*val1 + val2*val2);
      herr2->SetBinContent(i+1, data2->GetBinContent(i+1));
      herr2->SetBinError(i+1, val);
    }

  double fake_par[nbins-1] = {
    /*
    17.5,
    25.0,
    35.0,
    45.0,
    55.0,
    68.0,
    81.0,
    91.0,
    101.0,
    113.0,
    132.5,
    175.0,
    400.0
    */
        17.0,
        24.0,
        34.3,
        44.3,
        54.4,
        68.0,
        81.2,
        91.0,
        100.4,
        113.3,
        132.4,
        173.5,
        312.2
  };

  TGraphAsymmErrors* grae1 = new TGraphAsymmErrors(nbins-1);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      double x_ = fake_par[ii];
      double y_ = rdata_vals1[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = rdata_err1[ii];
      double eyh_ = rdata_err1[ii];
      grae1->SetPoint(ii,x_,y_);
      grae1->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }
  TGraphAsymmErrors* grae2 = new TGraphAsymmErrors(nbins-1);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      double x_ = fake_par[ii];
      double y_ = rdata_vals2[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = rdata_err2[ii];
      double eyh_ = rdata_err2[ii];
      grae2->SetPoint(ii,x_,y_);
      grae2->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }


  TGraphAsymmErrors* gerr = new TGraphAsymmErrors(nbins-1);
  TGraphAsymmErrors* gerr1 = new TGraphAsymmErrors(3);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      double val1 = data1->GetBinContent(ii+1)*err_model[ii];
      double val2 = data1->GetBinContent(ii+1)*(rdata_err1[ii]/rdata_vals1[ii]);
      double val = sqrt(val1*val1 + val2*val2);

      double x_ = fake_par[ii];
      double y_ = rdata_vals1[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = val;
      double eyh_ = val;
      if( ii >= 10 ) {
        gerr1->SetPoint(ii-10,x_,y_);
        gerr1->SetPointError(ii-10,exl_,exh_,eyl_,eyh_);
      }
      //if( ii == 12 ) val = 0.0;
      eyl_ = val;
      eyh_ = val;
      gerr->SetPoint(ii,x_,y_);
      gerr->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }

  TGraphAsymmErrors* gerr2 = new TGraphAsymmErrors(nbins-1);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      double val1 = data2->GetBinContent(ii+1)*err_model[ii];
      double val2 = data2->GetBinContent(ii+1)*(rdata_err2[ii]/rdata_vals2[ii]);
      double val = sqrt(val1*val1 + val2*val2);

      double x_ = fake_par[ii];
      double y_ = rdata_vals2[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = val;
      double eyh_ = val;
      //if( ii == 12 ) val = 0.0;
      eyl_ = val;
      eyh_ = val;
      gerr2->SetPoint(ii,x_,y_);
      gerr2->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }

  // Voila! at this stage, on has
  // 1) one TGraph for the data
  TGraph* dataGraph1   = grae1;
  TGraph* dataGraph2   = grae2;
  TGraph* errGraph   = gerr;
  TGraph* errGraph1   = gerr1;
  TGraph* errGraph2   = gerr2;
  // 2) one histogram for the theory
  TH1* theoryHist     = h_mstw[2];
  TH1* theoryHist2     = h_mstw2[2];
  TH1* theoryHist3     = h_mstw3[2];
  TH1* theoryHist4     = h_mstw4[2];
  // 2) one histogram for the theory error
  TH1* theoryBandHist = h_mstw_band[2];
  TH1* theoryBandHist2 = h_mstw_band2[2];
  TH1* theoryBandHist3 = h_mstw_band3[2];
  TH1* theoryBandHist4 = h_mstw_band4[2];

  theoryHist->SetLineColor(kRed);
  theoryHist->SetLineWidth(2);
  theoryHist->SetMarkerSize(0);
  theoryHist->SetFillColor(kRed);
  
  theoryHist2->SetLineColor(kRed);
  theoryHist2->SetLineWidth(2);
  theoryHist2->SetMarkerSize(0);
  theoryHist2->SetFillColor(kRed);
  
  theoryHist3->SetLineColor(kRed);
  theoryHist3->SetLineWidth(2);
  theoryHist3->SetMarkerSize(0);
  theoryHist3->SetFillColor(kRed);
  
  theoryHist4->SetLineColor(kRed);
  theoryHist4->SetLineWidth(2);
  theoryHist4->SetMarkerSize(0);
  theoryHist4->SetFillColor(kRed);
  
  theoryBandHist->SetLineColor(kBlue);
  theoryBandHist->SetLineWidth(2);
  theoryBandHist->SetMarkerSize(0);
  theoryBandHist->SetFillColor(kBlue);

  theoryBandHist2->SetLineColor(kBlue);
  theoryBandHist2->SetLineWidth(2);
  theoryBandHist2->SetMarkerSize(0);
  theoryBandHist2->SetFillColor(kBlue);

  theoryBandHist3->SetLineColor(kBlue);
  theoryBandHist3->SetLineWidth(2);
  theoryBandHist3->SetMarkerSize(0);
  theoryBandHist3->SetFillColor(kBlue);

  theoryBandHist4->SetLineColor(kBlue);
  theoryBandHist4->SetLineWidth(2);
  theoryBandHist4->SetMarkerSize(0);
  theoryBandHist4->SetFillColor(kBlue);

  herr1->SetLineColor(kOrange+7);
  herr1->SetLineWidth(2);
  herr1->SetMarkerSize(0);
  herr1->SetFillColor(kOrange+7);
  herr1->SetFillStyle(1001);

  herr2->SetLineColor(kRed+7);
  herr2->SetLineWidth(2);
  herr2->SetMarkerSize(0);
  herr2->SetFillColor(kRed+7);
  herr2->SetFillStyle(1001);

  errGraph->SetLineColor(kOrange+7);
  errGraph->SetLineWidth(0.0);
  errGraph->SetMarkerSize(0);
  errGraph->SetFillColor(kOrange+7);
  errGraph->SetFillStyle(1001);

  errGraph1->SetLineColor(kOrange+7);
  errGraph1->SetLineWidth(0.0);
  errGraph1->SetMarkerSize(0);
  errGraph1->SetFillColor(kOrange+7);
  errGraph1->SetFillStyle(1001);

  errGraph2->SetLineColor(kRed+7);
  errGraph2->SetLineWidth(0.0);
  errGraph2->SetMarkerSize(0);
  errGraph2->SetFillColor(kRed+7);
  errGraph2->SetFillStyle(1001);

  TH1* hfit = theoryHist;

  // calculate the ratio data/theory
  TGraphAsymmErrors* ratioGraph = 
    (TGraphAsymmErrors*) dataGraph1->Clone("ratio");
  for( int ii=0; ii<dataGraph1->GetN(); ii++ )
    {
      double x_, y_;
      dataGraph1->GetPoint(ii,x_,y_ );
      //      ratioGraph->SetPointEYlow(ii,0);
      //      ratioGraph->SetPointEYhigh(ii,0);
      ratioGraph->SetPoint(ii,x_,0 );
      double eyl_ = dataGraph1->GetErrorYlow(ii);
      double eyh_ = dataGraph1->GetErrorYhigh(ii);
      int jj = hfit->FindBin(x_);
      float fit_ = hfit->GetBinContent( jj );
      //      float binWidth_ = hfit->GetBinWidth(jj);
      //      ratioGraph->SetPointEXlow(ii,binWidth_/2.);
      //      ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
      if( fit_>0 )
	{
	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
	}
      //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
      //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
      //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
    }

  // calculate the ratio band/fit
  TH1* theoryBandRatio = 
    (TH1*) theoryBandHist->Clone("ratioBand");
  for( int ii=1; ii<=theoryBandHist->GetNbinsX(); ii++ )
    {
      double y_(0), dy_(0);
      float fit_ =           hfit->GetBinContent(ii);  
      cout << ii << "/fit=" << fit_ << endl; 
      if( fit_>0 )
	{
	  y_  = theoryBandHist->GetBinContent(ii);
	  dy_ = theoryBandHist->GetBinError(ii);
	  cout << "error band = " << y_ << " " << dy_ << endl;
	  theoryBandRatio->SetBinContent(ii,y_/fit_);
	  // theoryBandRatio->SetBinContent(ii,1.);
	  theoryBandRatio->SetBinError(ii,dy_/fit_);
	}
    }


  TGaxis::SetMaxDigits(3);

  float r0_ = 1.;
  float dr_ = 0.75;

//   // channels, ordered as in the legend
//   vector<TString> channels;  
//   vector<TString> hnames;
//   vector<TString> type;
//   vector<size_t>  order;  

//   map<TString,int> fillColor_;
//   map<TString,int> lineColor_;
//   int lineWidth1(1);
//   int lineWidth2(1);

//   if( salamanderColors )
//     {
//       lineWidth1 = 1;
//       lineWidth2 = 1;

//       fillColor_["Signal"] = kOrange-2;
//       lineColor_["Signal"] = kOrange+3;
      
//       fillColor_["Ztautau"] = kOrange+7;
//       lineColor_["Ztautau"] = kOrange+3;

//       fillColor_["VV"]     = kOrange+7;
//       lineColor_["VV"]     = kOrange+3;

//       fillColor_["EWK"] = kOrange+10;
//       lineColor_["EWK"] = kOrange+3;

//       fillColor_["QCD"] = kViolet-5;
//       lineColor_["QCD"] = kViolet+3;
      
//       fillColor_["ttbar"] = kRed+2;
//       lineColor_["ttbar"] = kRed+4;      

//       fillColor_["Y"] = kViolet-4;
//       lineColor_["Y"] = kViolet+3;
//     }
//   else
//     {
//       lineWidth1 = 1;
//       lineWidth2 = 1;

//       fillColor_["Signal"] = kSpring-3;
//       lineColor_["Signal"] = kSpring-3;
      
//       fillColor_["Ztautau"] = kOrange;
//       lineColor_["Ztautau"] = kOrange;
      
//       fillColor_["EWK"] = kCyan-4;
//       lineColor_["EWK"] = kCyan-4;
      
//       fillColor_["VV"] = kPink-1;
//       lineColor_["VV"] = kPink-1;
      
//       fillColor_["QCD"] =  kMagenta-3;
//       lineColor_["QCD"] =  kMagenta-3;
      
//       fillColor_["Y"]   =  kBlue+2;
//       lineColor_["Y"]   =  kBlue+2;
      
//       fillColor_["ttbar"] = kAzure;
//       lineColor_["ttbar"] = kAzure;
//     }

  // text2
  size_t ntxt = 3;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS";
  txtSize[0] = 0.055;
  txtX[0] = 0.88;
  txtY[0] = 0.84;
  txtAlign[0] = 31;
  txtFont[0] = 42;

  //txt[1] = "36 pb^{-1}  at  #sqrt{s} = 8 TeV";
  txt[1] = "36 pb^{-1} at #sqrt{s} = 8 TeV";
  //txt[1] = "#int#font[12]{L}dt = 36 pb^{-1} at #sqrt{s} = 8 TeV";
  txtSize[1] = 0.042;
  txtX[1] = 0.40;
  txtY[1] = 0.83;
  txtAlign[1] = 21;
  txtFont[1] = 42;
  
  txt[2] = "#gamma*/Z #rightarrow ll";
  txtSize[2] = 0.06;
  txtX[2] = 0.70;
  txtY[2] = 0.70;
  txtAlign[2] = 21;
  txtFont[2] = 42;
  
  // histogram limits, in linear and logarithmic
  int nbin_(0);
  float xmin_(0.), xmax_(0.); 
  float yminl_(0.), ymaxl_(0.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(510);
  int ndivy(510);
  float markerSize(1.2);
  float titleOffsetY(1.15);
  float titleSizeY(0.06);
  if( dataMCRatio )
    titleOffsetY=1.00;
    float titleOffsetX(1.00);

  // canvas name
  TString ctitle;

  // legend position and scale;
  float xl_(0);
  float yl_(0);
  float dxl_(0);
  float dyl_(0);
  int legendTextFont=42;
  float legendTextSize=0.040;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString cname;

  cname += "Fig_11";
  ctitle = "DY analysis - comparison with theory";
  xtitle = "M(ll)  [";
  xtitle += massUnit;       
  xtitle += "]";
  ytitle = "1/#sigma_{ll} d#sigma/dM(ll) [GeV^{-1} ]"; 

  nbin_ = 100;
  xmin_ = 15.;
  xmax_ = 600.;
  ndivx = 0;
  ndivy = 506;
	  
  markerSize = 1.1;
    
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=400;

  yminl_ = 0.0000005;
  ymaxl_ = 1.5;

  xl_  = 0.65;
  yl_  = 0.42;
  dxl_ = 3.;
  dyl_ = 5.0;
  
  //  legendTextSize *= 1.1;

       
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin( 110./600 );
  c_->SetRightMargin( 42./600 );
  c_->SetTopMargin(  40./600 );
  c_->SetBottomMargin( 80./600 ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);

  Double_t scale = 4;
  Double_t wbin = 42*scale;
  Double_t left  = 8*scale;
  Double_t right = 5*scale;
  Double_t h1 = 135*scale;
  Double_t h2 = 45*scale;
  Double_t top1 = 15*scale;
  Double_t bot1 = 3*scale;
  Double_t top2 = 3*scale;
  //  Double_t bot1 = 0*scale;
  //  Double_t top2 = 0*scale;
  Double_t bot2 = 80*scale;
  Double_t W = left + wbin + right;
  Double_t H = h1 + h2;
  Double_t s[2] = {1, h1/h2 };

  TPad* pad[2];
  pad[0] = new TPad( "top", "top", 
		     0, h2/H, 1, 1,
		     kWhite,0,0);
  pad[0]->SetLeftMargin(  left/W );
  pad[0]->SetRightMargin( right/W );
  pad[0]->SetTopMargin(  top1/H );
  pad[0]->SetBottomMargin( bot1/H );

  pad[1] = new TPad( "bottom", "bottom", 
		     0, 0, 1, h2/H,
		     kWhite,0,0);
  pad[1]->SetLeftMargin(  left/W );
  pad[1]->SetRightMargin( right/W );
  pad[1]->SetTopMargin(  top2/H );
  pad[1]->SetBottomMargin( bot2/H );
  //  pad[1]->SetGridy();

  for( int ii=0; ii<2; ii++ )
    {
      pad[ii]->SetFillColor(0);
      pad[ii]->SetTickx(1);
      pad[ii]->SetTicky(1);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
    }


//   //  return c_;
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon6", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  //ax_->SetTitleOffset(titleOffsetX);
  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(99);
  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetLabelOffset(0.008);

  dataGraph1->SetName("dataGraph1");
  dataGraph1->SetMarkerStyle(kFullCircle);
  dataGraph1->SetMarkerColor(kBlack);
  dataGraph1->SetMarkerSize(markerSize);
  
  dataGraph2->SetName("dataGraph2");
  dataGraph2->SetMarkerStyle(21);
  dataGraph2->SetMarkerColor(kRed);
  dataGraph2->SetLineColor(kRed);
  dataGraph2->SetMarkerSize(markerSize);
  
  TGraph* dummyGraph = (TGraph*) dataGraph1->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);
    
//   TH1* hfit = totalHisto;
//   for( int ii=0; ii<dataGraph->GetN(); ii++ )
//     {
//       dataGraph->GetPoint(ii,x_,y_ );
//       ratioGraph->SetPointEYlow(ii,0);
//       ratioGraph->SetPointEYhigh(ii,0);
//       ratioGraph->SetPoint(ii,x_,0 );
//       double eyl_ = dataGraph->GetErrorYlow(ii);
//       double eyh_ = dataGraph->GetErrorYhigh(ii);
//       int jj = hfit->FindBin(x_);
//       float fit_ = hfit->GetBinContent( jj );
//       float binWidth_ = hfit->GetBinWidth(jj);
//       ratioGraph->SetPointEXlow(ii,binWidth_/2.);
//       ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
//       if( fit_>0 )
// 	{
// 	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
// 	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
// 	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
// 	}
//       //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
//       //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
//       //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
//     }
  
  TH1* hratio_ = (TH1*) h_->Clone("hratio");
  ax_ = hratio_->GetXaxis();
  ay_ = hratio_->GetYaxis();
  ax_->SetLabelOffset(99);
  ax_->SetTitleOffset(99);
  //  ay_->SetTitleSize(0.13);
  //  ay_->SetTitleOffset(0.45);
  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

 
  TPad* p_ = pad[0];
  p_->Draw();
  p_->cd();

  p_->SetLogy(true);
  p_->SetLogx(true);

  h_->GetYaxis()->SetRangeUser(yminl_,ymaxl_);
  h_->Draw("hist");
  //  draw_axis_labels(0.9*yminl_,0.050);

  draw_bin_grid( yminl_, 1.3*yminl_ );
  draw_bin_grid( ymaxl_/1.3, ymaxl_ );

  //draw_axis_labels(r0_-1.15*dr_,0.15);

  bool drawHGrid = true;
  if( drawHGrid )
    {
      TLine grid_;
      grid_.SetLineColor(kGray+2);
      grid_.SetLineWidth(1);
      float y_;
      //      grid_.SetLineStyle(kSolid);
      grid_.SetLineStyle(kDotted);
      y_=0.0001;  grid_.DrawLine(120,y_,xmax_,y_);
      y_=0.001;   grid_.DrawLine(120,y_,xmax_,y_);
      y_=0.01;    grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=0.1;     grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=1;       grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=10;      grid_.DrawLine(xmin_,y_,60,y_);
      //      y_=0.9;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      y_=0.8;  grid_.DrawLine(xmin_,y_,xmax_,y_);
    }

    //    theoryHist->Draw("E3Same");
    //h->SetLineColor(kGreen+1);
    //h->Draw("Lsame");
    //herr1->Draw("E2same");
    //herr2->Draw("E2same");
    errGraph->SetLineWidth(2);
    errGraph->SetFillColor(kOrange+7); // NEW
    errGraph->SetLineColor(kOrange+7); // NEW
    errGraph->Draw("E2"); // Rectangular
    errGraph2->SetLineWidth(2);
    errGraph2->SetFillColor(kRed+7); // NEW
    errGraph2->SetLineColor(kRed+7); // NEW
    //errGraph2->Draw("E2same"); // Rectangular
    //errGraph->Draw("E4C"); // continuous plot
    //errGraph1->SetLineWidth(2);
    //errGraph1->SetFillColor(kOrange+7); // NEW
    //errGraph1->SetLineColor(kOrange+7); // NEW
    //errGraph1->Draw("E3Csame"); // continuous plot
    //theoryBandHist4->Smooth();
    //theoryBandHist4->Draw("E3same");
    //h_4->Smooth();
    h_4->SetLineColor(kBlue);
    h->SetLineColor(kBlue);
    h->Draw("same");
    theoryBandHist->Draw("e2same");
    //h_4->Draw("Lsame");

  //  h->SetLineColor(kGreen+1);
  //data1->SetMarkerSize(0);
  //data1->Draw("ESame");
    gStyle->SetEndErrorSize(3);
    
    dataGraph1->Draw("PE");
    //dataGraph2->Draw("PEsame");

//   //  h_->Draw("Same");

    xl_ = 0.2;
    yl_ = 0.2;
    dxl_ = 0.25;
    dyl_ = 0.20;

    // legend position and scale;
    TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
    legend->SetTextFont(legendTextFont);
    legend->SetTextSize(legendTextSize);
    legend->SetLineColor(0);
    legend->SetFillColor(0);

    legend->AddEntry(dataGraph1," Data (e+#mu channels)","PL");
    legend->AddEntry(errGraph," Uncertainty on modeling included","F");
    legend->AddEntry(theoryBandHist," NLO, FEWZ+CT10","F");
    
    legend->Draw("Same");

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  p_->RedrawAxis();
  p_->GetFrame()->Draw();
  c_->cd();
  
  p_ = pad[1];
  p_->SetLogx();
  p_->Draw();
  p_->cd();

  TAxis* xratio_ = hratio_->GetXaxis();
  TAxis* yratio_ = hratio_->GetYaxis();

  yratio_->SetRangeUser(r0_-0.9999*dr_,r0_+0.9999*dr_);
  yratio_->SetLabelSize( s[1]*yratio_->GetLabelSize() );
  yratio_->SetTitleSize( 0.13 );
  //yratio_->SetTitleSize( s[1]*yratio_->GetTitleSize() );
  //  yratio_->SetLabelOffset( yratio_->GetLabelOffset() );
  yratio_->SetLabelOffset( 0.012 );
  //  yratio_->SetTitleOffset( yratio_->GetTitleOffset()/s[1] );
  yratio_->SetTitleOffset( 0.45 );
  yratio_->SetTitle("data/theory");
  yratio_->SetNdivisions(3);
  
  xratio_->SetLabelSize( s[1]*xratio_->GetLabelSize() );
  xratio_->SetTitleSize( s[1]*xratio_->GetTitleSize() );
  xratio_->SetTitleOffset( 1.0 );
  //   //  xratio_->CenterTitle();
  xratio_->SetLabelOffset(99);
  xratio_->SetTickLength( xratio_->GetTickLength()*s[1] );
  
  hratio_->Draw();
  theoryBandRatio->Draw("E2Same");
  //theoryBandRatio->Draw("E2Same"); //!!!
  draw_axis_labels(r0_-1.15*dr_,0.15);
  draw_bin_grid( r0_-dr_, r0_+dr_ );
  {  
    TLine grid_;
    grid_.SetLineColor(kGray+2);
    grid_.SetLineWidth(1);
    float y_;
    grid_.SetLineStyle(kDashed);
    y_=1;  grid_.DrawLine(xmin_,y_,xmax_,y_);
  }

  
  ratioGraph->SetMarkerSize( ratioGraph->GetMarkerSize()*1. );
  ratioGraph->SetLineColor( kBlack );
  ratioGraph->SetLineWidth( 1 );
  
  ratioGraph->SetMarkerColor( kViolet );
  ratioGraph->SetMarkerStyle( kFullCircle );
  ratioGraph->DrawClone("PE");
  ratioGraph->SetMarkerColor( kBlack );
  ratioGraph->SetMarkerStyle( kOpenCircle );
  ratioGraph->DrawClone("PE");


   p_->RedrawAxis();
   p_->GetFrame()->Draw();
   
   c_->cd();
   return c_;
}
//#include "Theory.C"
TCanvas* Theory( bool isCombined = false )
{
  bool isElectron = false;
  bool isTheory = true;
  bool is2010 = false;
  bool is2DProj = false;

  double mass_2Dbin[7] = {20, 30, 45, 60, 120, 200, 1500};
  double result_2Dproj[6] = {0.0274095, 0.00673847, 0.00215625, 0.012402, 9.99694e-05, 1.03917e-06};
  double result_2Derrs[6] = {0.000160322, 3.42496e-05, 1.75895e-05, 1.93475e-05, 1.79167e-06, 4.62293e-08};
  TH1D* h2DProj = new TH1D("h2DProj", "h2Dproj", 6, mass_2Dbin);
  for( int i = 0; i < 6; i++ ) {
    h2DProj->SetBinContent(i+1, result_2Dproj[i]);
    h2DProj->SetBinError(i+1, result_2Derrs[i]);
  }

  const int norders = 3;
  const int nbins  = 14;
  const int nbins1 = 41;
  const int nbins2  = 29;
  const int nbins3  = 84;
  const int nbins4  = 339+179; // up to 600 => 339
  //const int nbins4  = 305;
  //const int nbins4  = 200;

  //per pdf within order
  TH1D* h_mstw[norders];
  TH1D* h_mstw1[norders];
  TH1D* h_mstw2[norders];
  TH1D* h_mstw3[norders];
  TH1D* h_mstw4[norders];
  TH1D* h_mstw_band[norders];
  TH1D* h_mstw_band1[norders];
  TH1D* h_mstw_band2[norders];
  TH1D* h_mstw_band3[norders];
  TH1D* h_mstw_band4[norders];

  //per pdf and per order
  double xsec_mass_mstw[nbins-1][norders];
  double xsec_err_mass_mstw[nbins-1][norders];

  double xsec_mass_mstw1[nbins1-1][norders];
  double xsec_err_mass_mstw1[nbins1-1][norders];

  double xsec_mass_mstw2[nbins2-1][norders];
  double xsec_err_mass_mstw2[nbins2-1][norders];
  
  double xsec_mass_mstw3[nbins3-1][norders];
  double xsec_err_mass_mstw3[nbins3-1][norders];
  
  double xsec_mass_mstw4[nbins4-1][norders];
  double xsec_err_mass_mstw4[nbins4-1][norders];
  
  //default masses
  double mass_xlow[nbins] = {
    15,20,30,40,50,60,76,86,96,106,120,150,200,600};

  double mass_xlow1[nbins1] = {
    15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
    96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
    200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

  double mass_xlow2[nbins2] = {15.0,20.0,25.,30.0,35.0,40.0,45.0,50.0,55.0,60.0,76.0,86.0,96.0,106.0,120.0,125.0,130.0,135.0,140.0,145.0,150.0,155.0,160.0,165.0,170.0,180.0,190.0,200.0,600.};

  double mass_xlow3[nbins3] = {15.0,20.0,25.,30.0,35.0,40.0,45.0,50.0,55.0,
         60,61,62,63,64,65,66,67,68,69,
	 70,71,72,73,74,75,76,77,78,79,
	 80,81,82,83,84,85,86,87,88,89,
	 90,91,92,93,94,95,96,97,98,99,
	 100,101,102,103,104,105,106,107,108,109,
	 110,111,112,113,114,115,116,117,118,119,
	 120.0,125.0,130.0,135.0,140.0,145.0,150.0,155.0,160.0,165.0,170.0,180.0,190.0,200.0,600.};
/*
  double mass_xlow4[nbins4] = {15.0, 
17.0, 
19.0, 
21.0, 
22.0, 
24.0, 
26.0, 
28.0, 
30.0, 
33.0, 
36.0, 
39.0, 
42.0, 
45.0, 
48.0, 
51.0, 
54.0, 
57.0, 
60.0, 
63.0, 
66.0, 
69.0, 
72.0, 
75.0, 
78.0, 
80.0, 
82.0, 
84.0, 
86.0, 
88.0, 
90.0, 
91.0, 
92.0, 
94.0, 
96.0, 
98.0, 
100.0, 
103.0, 
106.0, 
109.0, 
112.0, 
115.0, 
118.0, 
119, 
120, 
121, 
122, 
123, 
124, 
125, 
126, 
127, 
128, 
129, 
130, 
131, 
132, 
133, 
134, 
135, 
136, 
137, 
138, 
139, 
140, 
141, 
142, 
143, 
144, 
145, 
146, 
147, 
148, 
149, 
150, 
151, 
152, 
153, 
154, 
155, 
156, 
157, 
158, 
159, 
160, 
162, 
164, 
166, 
168, 
170, 
172, 
174, 
176, 
178, 
180, 
182, 
184, 
186, 
188, 
190, 
192, 
194, 
196, 
198, 
200, 
202, 
204, 
206, 
208, 
210, 
212, 
214, 
216, 
218, 
220, 
222, 
224, 
226, 
228, 
230, 
232, 
234, 
236, 
238, 
240, 
242, 
244, 
246, 
248, 
250, 
252, 
254, 
256, 
258, 
260, 
262, 
264, 
266, 
268, 
270, 
272, 
274, 
276, 
278, 
280, 
282, 
284, 
286, 
288, 
290, 
292, 
294, 
296, 
298, 
300, 
302, 
304, 
306, 
308, 
310, 
312, 
314, 
316, 
318, 
320, 
322, 
324, 
326, 
328, 
330, 
332, 
334, 
336, 
338, 
340, 
342, 
344, 
346, 
348, 
350, 
352, 
354, 
356, 
358, 
360, 
362, 
364, 
366, 
368, 
370, 
372, 
374, 
376, 
378, 
380, 
382, 
384, 
386, 
388, 
390, 
392, 
394, 
396, 
398, 
400, 
402, 
404, 
406, 
408, 
410, 
412, 
414, 
416, 
418, 
420, 
422, 
424, 
426, 
428, 
430, 
432, 
434, 
436, 
438, 
440, 
442, 
444, 
446, 
448, 
450, 
452, 
454, 
456, 
458, 
460, 
462, 
464, 
466, 
468, 
470, 
472, 
474, 
476, 
478, 
480, 
482, 
484, 
486, 
488, 
490, 
492, 
494, 
496, 
498, 
500, 
502, 
504, 
506, 
508, 
510, 
512, 
514, 
516, 
518, 
520, 
522, 
524, 
526, 
528, 
530, 
532, 
534, 
536, 
538, 
540, 
542, 
544, 
546, 
548, 
550, 
552, 
554, 
556, 
558, 
560, 
562, 
564, 
566, 
568, 
570, 
572, 
574, 
576, 
578, 
580, 
582, 
584, 
586, 
588, 
590, 
592, 
594, 
596, 
598, 
600};
*/
  double mass_xlow4[nbins4] = {0.};
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
    else {
      initial += 5.0;
    }
    mass_xlow4[i] = initial;
    //cout << "mass_xlow4[" << i << "] = " << mass_xlow4[i] << endl;
  }

  double th_err1[nbins1-1];
  double th_err1_s1_up[nbins1-1] = {
      4.3, 3.7, 3.2, 2.9, 2.5, 2.2, 1.9, 1.7, 1.4, 1.2,
      1.0, 0.8, 0.6, 0.4, 0.2, 0.03, 0.07, 0.2, 0.3, 0.4, 
      0.5, 0.6, 0.7, 0.8, 1.0, 1.1, 1.3, 1.5, 1.7, 1.9, 
      2.1, 2.4, 2.8, 3.2, 3.8, 4.4, 5.0, 5.7, 6.9, 10.5
  };
  double th_err1_s1_do[nbins1-1] = {
      3.3, 2.9, 2.6, 2.4, 2.2, 2.0, 1.8, 1.6, 1.4, 1.2,
      1.0, 0.8, 0.6, 0.4, 0.2, 0.02, 0.06, 0.2, 0.3, 0.4, 
      0.5, 0.7, 0.8, 0.9, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0,
      2.2, 2.5, 2.9, 3.3, 3.9, 4.5, 5.0, 5.6, 6.5, 8.6
  };
  double th_err1_s2_up[nbins1-1] = {
    2.5, 1.9, 1.9, 3.1, 3.1, 2.4, 2.4, 2.6, 2.6, 2.0, 
    2.0, 2.0, 2.0, 2.1, 2.1, 1.8, 1.8, 2.8, 2.8, 2.6,
    2.6, 2.6, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0, 2.0, 2.0,
    1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8
  };
  double th_err1_s2_do[nbins1-1] = {
    2.7, 3.6, 3.6, 2.1, 2.1, 2.5, 2.5, 2.0, 2.0, 2.4,
    2.4, 2.4, 2.4, 2.5, 2.5, 2.2, 2.2, 2.0, 2.0, 2.2, 
    2.2, 2.2, 1.7, 1.7, 1.7, 1.7, 1.8, 1.8, 1.8, 1.8,
    2.1, 2.1, 2.1, 2.1, 2.1, 2.1, 2.1, 2.1, 2.1, 2.1
  };
  for( int ii = 0; ii < nbins1-1; ii++ ) {
    double err2_up = th_err1_s1_up[ii]*th_err1_s1_up[ii] + th_err1_s2_up[ii]*th_err1_s2_up[ii];
    double err2_do = th_err1_s1_do[ii]*th_err1_s1_do[ii] + th_err1_s2_do[ii]*th_err1_s2_do[ii];
    if( err2_up > err2_do ) th_err1[ii] = sqrt(err2_up);
    else th_err1[ii] = sqrt(err2_do);
    //cout << "th_err1 = " << ii << " " << th_err1[ii] << " " << sqrt(err2_up) << " " << sqrt(err2_do) << endl;
  }

  /*
  double th_err4[nbins4-1] = {
     4.4,4.4,4.3,4.3,4.2,4.2,4.1,4.0,3.9,
     3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,
     3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3, 
     3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2, // 120
     3.1,3.1,3.1,3.1,3.1,3.0,3.0,3.0,3.0,3.0,
     3.0,3.0,3.0,3.0,3.0,2.9,2.9,2.9,2.9,2.9,
     2.9,2.9,2.9,2.9,2.9,2.8,2.8,2.8,2.8,2.8,
     2.8,2.8,2.8,2.8,2.8,2.7,2.7,2.7,2.7,2.7, // 160
     2.7,2.7,2.7,2.7,2.7,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,2.6,
     2.6,2.6,2.6,2.6,2.6
     };
     */

  double th_uncert1p[13] = {4.3, 3.6, 2.7, 2.1, 1.6, 0.9, 0.2, 0.1, 0.2, 0.5, 0.9, 1.5, 2.8};
  double th_uncert1m[13] = {3.3, 2.8, 2.3, 1.9, 1.5, 0.9, 0.2, 0.1, 0.2, 0.5, 0.9, 1.6, 2.9};
  double th_uncert2p[13] = {2.5, 1.9, 3.1, 2.4, 2.6, 2.0, 2.1, 1.8, 2.8, 2.6, 2.5, 2.0, 1.8};
  double th_uncert2m[13] = {2.7, 3.6, 2.1, 2.5, 2.0, 2.4, 2.5, 2.2, 2.0, 2.2, 1.7, 1.8, 2.1};
  double th_err4[nbins4-1] = {0.0};
  double add_mass = 15.0;
  int inbins = 0;
  for( int i = 0; i < nbins4-1; i++ ) {
    if( i >= 0 && i < 11 ) {
      add_mass += 1.0;
    }
    else if( i >= 11 && i < 18 ) {
      add_mass += 5.0;
    }
    else if( i >= 18 && i < 118 ) {
      add_mass += 1.0;
    }
    else if( i >= 118 && i < 338 ) {
      add_mass += 2.0;
    }
    else {
      add_mass += 5.0;
    }

    // inbins
    if( add_mass >=  mass_xlow[inbins] && add_mass <  mass_xlow[inbins+1] ) {
    }
    else inbins++;
    if( inbins > 12  ) inbins = 12;

    double errp = sqrt(th_uncert1p[inbins]*th_uncert1p[inbins] + th_uncert2p[inbins]*th_uncert2p[inbins]);
    double errm = sqrt(th_uncert1m[inbins]*th_uncert1m[inbins] + th_uncert2m[inbins]*th_uncert2m[inbins]);
    if( errp > errm ) th_err4[i] = errp;
    else th_err4[i] = errm;
    //cout << "err[" << i << "] = " << add_mass << " "  << " " << inbins << " " << mass_xlow[inbins] << " " << th_err4[i] << endl;
  }


  TH1D* data1 = new TH1D("data1","data1",nbins-1,mass_xlow);
  TH1D* data2 = new TH1D("data2","data2",nbins1-1,mass_xlow1);
  TH1D* h = new TH1D("h","h",nbins-1,mass_xlow);
  TH1D* h_2 = new TH1D("h_2","h_2",nbins2-1,mass_xlow2);
  TH1D* h_3 = new TH1D("h_3","h_3",nbins3-1,mass_xlow3);

  TFile* fth = new TFile("theory.root","recreate");
  TH1D* h_4 = new TH1D("h_4","h_4",nbins4-1,mass_xlow4);

  //shape r
  // old (5/10/2011)
  //double data_vals1[nbins-1] = {0.183, 0.056, 0.0148, 0.0058, 0.0030, 0.0021, 0.0058, 0.0851, 0.0044, 0.00089, 0.00025, 0.000052, 0.0000042};
  //double data_err1[nbins-1] = {0.016, 0.003, 0.0007, 0.0003, 0.0002, 0.0001, 0.0003, 0.0019, 0.0003, 0.00008, 0.00002, 0.000008, 0.0000007};

  // combined in 2010
  //double data_vals1[nbins-1] = {0.1526, 0.0539, 0.0146, 0.00591, 0.00298, 0.00206, 0.00580, 0.08555, 0.00415, 0.000864, 0.000254, 0.0000509, 0.00000392};
  //double data_err1[nbins-1] = {0.0133, 0.0032, 0.0008, 0.00033, 0.00019, 0.00010, 0.00030, 0.00235, 0.00033, 0.000083, 0.000024, 0.0000091, 0.00000078};

  // muon channel in 2010
  double data_vals1[nbins-1] = {0.156, 0.053, 0.015, 0.0062, 0.0030, 0.0020, 0.0058, 0.0857, 0.0041, 0.00084, 0.00029, 0.000054, 0.0000037};
  double data_err1[nbins-1] = {0.014, 0.003, 0.001, 0.0004, 0.0002, 0.00010, 0.00030, 0.0026, 0.0003, 0.00009, 0.00003, 0.000012, 0.0000010};

  // electron channel in 2010
  //double data_vals1[nbins-1] = {0.102, 0.056, 0.013, 0.0047, 0.0027, 0.0022, 0.0064, 0.0834, 0.0053, 0.00096, 0.00018, 0.000046, 0.0000043};
  //double data_err1[nbins-1] = {0.048, 0.010, 0.002, 0.0007, 0.0004, 0.0003, 0.0026, 0.0060, 0.0025, 0.00019, 0.00004, 0.000014, 0.0000013};

  double data_vals2[nbins1-1] = {1.};
  double data_err2[nbins1-1] = {1.};
  TFile* f = new TFile("../../Outputs/rshape_full.root");
  f->cd();
  TH1D* hxsec = (TH1D*)gDirectory->Get("hxsec");
  for (int i = 0; i < nbins1-1; i++) {
     data_vals2[i] = hxsec->GetBinContent(i+1);
     data_err2[i]  = hxsec->GetBinError(i+1);
     cout << data_vals2[i] << " " << data_err2[i] << endl; 
  }    

/*
  // 02052012 for 4.14 fb-1 for muon channel only
  double data_vals2[nbins1-1] = {
    0.162424 
    , 0.0690803 
    , 0.0326762 
    , 0.018096 
    , 0.010861 
    , 0.00691175 
    , 0.0046713 
    , 0.00337878 
    , 0.0025918 
    , 0.00216039 
    , 0.00192844 
    , 0.00204694 
    , 0.00234728 
    , 0.00337749 
    , 0.00801226 
    , 0.0763461 
    , 0.0927782 
    , 0.00753216 
    , 0.00262804 
    , 0.00140285 
    , 0.000846803 
    , 0.000570223 
    , 0.000396167 
    , 0.000284052 
    , 0.000186039 
    , 0.000136252 
    , 0.000101094 
    , 6.36749e-05 
    , 4.86828e-05 
    , 3.26977e-05 
    , 2.29489e-05 
    , 1.40906e-05 
    , 7.6816e-06 
    , 4.70827e-06 
    , 1.82499e-06 
    , 9.16794e-07 
    , 3.7713e-07 
    , 1.95385e-07 
    , 4.01899e-08 
    , 1.38422e-09 
  };
  double data_err2[nbins1-1] = {
    0.00700501
    , 0.00259042
    , 0.00115881
    , 0.000610307
    , 0.000359502
    , 0.000227665
    , 0.000153532
    , 0.000111472
    , 8.30082e-05
    , 7.57245e-05
    , 6.78746e-05
    , 6.92159e-05
    , 7.75857e-05
    , 0.000115328
    , 0.000260769
    , 0.00192759
    , 0.00234271
    , 0.000200345
    , 7.87188e-05
    , 5.60315e-05
    , 3.54273e-05
    , 2.75838e-05
    , 1.93291e-05
    , 1.39729e-05
    , 1.01227e-05
    , 8.31624e-06
    , 6.6229e-06
    , 4.88391e-06
    , 3.58642e-06
    , 2.7544e-06
    , 2.05636e-06
    , 1.33211e-06
    , 9.90919e-07
    , 5.50218e-07
    , 3.6811e-07
    , 1.98526e-07
    , 1.15641e-07
    , 6.83309e-08
    , 1.3409e-08
    , 2.02671e-09
  };
*/

  // theory r // 11232011 from Stoyan (from fit of theory shape r distribution on the plot)
  TFile* fewz = new TFile("../Inputs/Theory/1Dabsxsec_NNLO_CTEQ12NNLO.root");
  fewz->cd();
  Double_t yAxis1[41] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
                  96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
                  200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};
  TH1D* hf = (TH1D*)gDirectory->Get("invm_FEWZ");
  double th_r[nbins1-1] = {0};

  for (int i = 0; i < 40; i++) {
    hf->SetBinContent(i+1,hf->GetBinContent(i+1)/(yAxis1[i+1]-yAxis1[i]));
    hf->SetBinError(i+1,hf->GetBinError(i+1)/(yAxis1[i+1]-yAxis1[i]));
    th_r[i] = hf->GetBinContent(i+1);
  }

/*
  {
    0.161196
    , 0.0652464
    , 0.0324762
    , 0.017822
    , 0.0104631
    , 0.00665895
    , 0.00453091
    , 0.00328228
    , 0.0024752
    , 0.00205569
    , 0.00191615
    , 0.00198721
    , 0.00227068
    , 0.00335782
    , 0.00802862
    , 0.0760769
    , 0.0894752
    , 0.00782657
    , 0.00260345
    , 0.00136427
    , 0.000849739
    , 0.000555006
    , 0.000389536
    , 0.00026674
    , 0.000183322
    , 0.000127482
    , 9.00876e-05
    , 6.40219e-05
    , 4.38833e-05
    , 2.95683e-05
    , 1.96126e-05
    , 1.25999e-05
    , 7.43531e-06
    , 3.8182e-06
    , 1.71576e-06
    , 7.9912e-07
    , 4.13632e-07
    , 1.91377e-07
    , 3.67528e-08
    , 2.41571e-09
  };
*/
  double th_err[nbins] = 
    {
      4.4,
      4.4,
      4.2,
      4.1,
      3.9,
      3.8,
      3.6,
      3.6,
      3.5,
      3.3,
      3.1,
      2.6
    };

  double th_err2[nbins2] = {4.4,4.4,4.3,4.3,4.2,4.2,4.1,4.0,3.9,3.8,3.6,3.6,3.5,3.3,3.1,3.0,3.0,2.9,2.9,2.8,2.8,2.7,2.7,2.7,2.6,2.6,2.6,2.6,2.8};

  double th_err3[nbins3] = {4.4,4.4,4.3,4.3,4.2,4.2,4.1,4.0,3.9,
     3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,3.6,
     3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,3.5,
     3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,3.3,
     3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,3.2,
     3.1,3.0,3.0,2.9,2.9,2.8,2.8,2.7,2.7,2.7,2.6,2.6,2.6,2.6,2.8};

  // old (5/10/2011)
  //double err_model[nbins-1] = {0.166, 0.038, 0.054, 0.026, 0.037, 0.044, 0.038, 0.028, 0.058, 0.042, 0.027, 0.079, 0.146};
  // new (2010 paper)
  double err_model[nbins-1] = {0.070, 0.057, 0.021, 0.0052, 0.021, 0.030, 0.041, 0.026, 0.019, 0.023, 0.0004, 0.011, 0.025};

  double err_model_40bins[nbins1-1] = {
    0.097, 0.031, 0.019, 0.007, 0.005, 0.003, 0.001, 0.001, 0.002,
    0.002, 0.003, 0.002, 0.002, 0.002, 0.001, 0.002, 0.002, 0.003,
    0.003, 0.005, 0.006, 0.006, 0.005, 0.006, 0.007, 0.010, 0.011,
    0.011, 0.011, 0.011, 0.010, 0.015, 0.014, 0.013, 0.015, 0.012,
    0.013, 0.021, 0.024, 0.031
  };
  /*
  double err_model_40bins[nbins1-1] = {
    0.07, 0.057, 0.057, 0.021, 0.021, 0.0052, 0.0052, 0.021, 0.021, 0.030, 
    //0.22, 0.18, 0.12, 0.09, 0.02, 0.21, 0.34,
    //0.19, 0.056, 0.052, 0.043, 0.0062, 0.0080, 0.0012,
    //0.021, 0.021, 0.030,          
    0.030, 0.030, 0.030, 0.041, 0.041, 0.026, 0.026, 0.019, 0.019, 0.023,
    0.023, 0.023, 0.0004, 0.0004, 0.0004, 0.0004, 0.011, 0.011, 0.011, 0.011,
    0.025, 0.025, 0.025, 0.025, 0.025, 0.025, 0.025, 0.025, 0.025, 0.025};
    */

  for( int i = 0; i < 40; i++ ) {
    //cout << "mass(" << mass_xlow1[i] << ":" << mass_xlow1[i+1] << ") = " << err_model_40bins[i] << endl;
  }

  double rdata_vals1[nbins-1];
  double rdata_err1[nbins-1];
  double rdata_vals2[nbins1-1];
  double rdata_err2[nbins1-1];

  double peak_bin_width = 1.;
  double peak_val_theory = 1009.0;

  //stream to get x-sec values and errors
  //LO
  ifstream in_xsec_massLO_mstw("root/Plain/LO/out_MSTW2008LO.txt",ios::in);

  //NLO
  ifstream in_xsec_massNLO_mstw("root/Plain/NLO/out_MSTW2008NLO.txt",ios::in);

  //NNLO
  ifstream in_xsec_massNNLO_mstw("root/Plain/NNLO/out_NNLO_MSTW2008NNLO.txt",ios::in);

  ifstream in_xsec_massNNLO_mstw1("root/Plain/NNLO/fullnnlo_fewz_40bins.txt",ios::in);

  //NNLO 2
  ifstream in_xsec_massNNLO_mstw2("root/Plain/NNLO/semifinaloutNNLO_PlainMSTW2008NNLO.txt",ios::in);

  //NNLO 3
  ifstream in_xsec_massNNLO_mstw3("root/Plain/NNLO/NNLO_MSTW_1GeV.txt",ios::in);
  ifstream in_xsec_massNNLO_mstw5("root/Plain/NNLO/nnlo.txt",ios::in);

  //NNLO 4
  //ifstream in_xsec_massNNLO_mstw4("root/Plain/NNLO/finaloutNNLO_PlainMSTW2008NNLO_4_gautier1.txt",ios::in);
  ifstream in_xsec_massNNLO_mstw4("root/Plain/NLO/8TeV_CTEQ12NNLO.txt",ios::in);

  ostringstream pprint_ord;
  pprint_ord.str("");
  for( int iord = 0; iord < norders; iord++ ) 
    {
      pprint_ord << iord;
      string ordLabel = pprint_ord.str();

      h_mstw[iord]  = new TH1D("Cross section1"+TString(ordLabel),
			       "Cross section1"+TString(ordLabel),
			       nbins-1,mass_xlow);
      h_mstw1[iord]  = new TH1D("Cross section1 1"+TString(ordLabel),
			       "Cross section1 1"+TString(ordLabel),
			       nbins1-1,mass_xlow1);
      h_mstw2[iord]  = new TH1D("Cross section1 2"+TString(ordLabel),
			       "Cross section1 2"+TString(ordLabel),
			       nbins2-1,mass_xlow2);
      h_mstw3[iord]  = new TH1D("Cross section1 3"+TString(ordLabel),
			       "Cross section1 3"+TString(ordLabel),
			       nbins3-1,mass_xlow3);
      h_mstw4[iord]  = new TH1D("Cross section1 4"+TString(ordLabel),
			       "Cross section1 4"+TString(ordLabel),
			       nbins4-1,mass_xlow4);
      
      h_mstw_band[iord]  = new TH1D("Cross section1 band"+TString(ordLabel),
				    "Cross section1 band"+TString(ordLabel),
				    nbins-1,mass_xlow);
      h_mstw_band1[iord]  = new TH1D("Cross section1 band1"+TString(ordLabel),
				    "Cross section1 band1"+TString(ordLabel),
				    nbins1-1,mass_xlow1);
      h_mstw_band2[iord]  = new TH1D("Cross section1 band2"+TString(ordLabel),
				    "Cross section1 band2"+TString(ordLabel),
				    nbins2-1,mass_xlow2);
      h_mstw_band3[iord]  = new TH1D("Cross section1 band3"+TString(ordLabel),
				    "Cross section1 band3"+TString(ordLabel),
				    nbins3-1,mass_xlow3);
      h_mstw_band4[iord]  = new TH1D("Cross section1 band4"+TString(ordLabel),
				    "Cross section1 band4"+TString(ordLabel),
				    nbins4-1,mass_xlow4);


      //      cout << "order " << iord << endl;
    
      for( int i = 0; i < nbins-1; i++ ) 
	{
	  double dm_ = mass_xlow[i+1]-mass_xlow[i];

	  if (iord == 0) 
	    {
	      //LO  
	      in_xsec_massLO_mstw >> xsec_mass_mstw[i][0] >>  xsec_err_mass_mstw[i][0] ;
	      
	      //fill data1 histo 
	      rdata_vals1[i] = data_vals1[i]*peak_bin_width;
	      rdata_err1[i]  = data_err1[i]*peak_bin_width;
	      data1->SetBinContent(i+1,rdata_vals1[i]);
	      //	      data1->SetBinError(i+1,rdata_err1[i]);
	      data1->SetBinError(i+1,0);

	      /*
	      rdata_vals2[i] = data_vals2[i]*peak_bin_width;
	      rdata_err2[i]  = data_err2[i]*peak_bin_width;
	      data2->SetBinContent(i+1,rdata_vals2[i]);
	      //	      data2->SetBinError(i+1,rdata_err2[i]);
	      data2->SetBinError(i+1,0);
	      */
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      in_xsec_massNLO_mstw >> xsec_mass_mstw[i][1] >>  xsec_err_mass_mstw[i][1] ;
	    } 
	  else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw >> xsec_mass_mstw[i][2] 
				    >>  xsec_err_mass_mstw[i][2] ;
	      h->SetBinContent(i+1,
			       xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow[i+1]-mass_xlow[i]));  // devide to the Z peak
	    }
	  
	  //check ok
	  //	  cout <<"bin " << i << xsec_mass_mstw[i][iord] << " " << xsec_err_mass_mstw[i][iord] << endl; 
	  //	  cout << "mass bin " << mass_xlow[i+1]-mass_xlow[i] << " " << i<< endl;

	  h_mstw[iord]->SetBinContent(i+1,xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band[iord]->SetBinContent(i+1,xsec_mass_mstw[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band[iord]->SetBinError(i+1,0.01*th_err[i]*h_mstw_band[iord]->GetBinContent(i+1));	  
	}

      for( int i = 0; i < nbins1-1; i++ ) 
	{
	  double dm_ = (mass_xlow1[i+1]-mass_xlow1[i]);

	  rdata_vals2[i] = data_vals2[i]*peak_bin_width;
	  rdata_err2[i]  = data_err2[i]*peak_bin_width;
	  data2->SetBinContent(i+1,rdata_vals2[i]);
	  data2->SetBinError(i+1,0);

	  //NNLO  
	  if (iord == 2) {
	    in_xsec_massNNLO_mstw1 >> xsec_mass_mstw1[i][2] 
				>>  xsec_err_mass_mstw1[i][2] ;
	    h_mstw1[2]->SetBinContent(i+1,xsec_mass_mstw1[i][2]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	    //h_mstw1[2]->SetBinContent(i+1,xsec_mass_mstw1[i][2]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	    h_mstw1[2]->SetBinContent(i+1, th_r[i]*1.05);
	    h_mstw1[2]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw1[i][2]/peak_val_theory),2)+pow(xsec_mass_mstw1[i][2]*10./peak_val_theory/peak_val_theory,2)));
	    //cout << "h_mstw1 = " << mass_xlow1[i] << " " <<  xsec_mass_mstw1[i][2] << " " << h_mstw1[2]->GetBinContent(i+1) << " " <<  h_mstw1[2]->GetBinError(i+1) << endl;
	    //h_mstw_band1[2]->SetBinContent(i+1,xsec_mass_mstw1[i][2]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	    h_mstw_band1[2]->SetBinContent(i+1, th_r[i]*1.05);
	    h_mstw_band1[2]->SetBinError(i+1,0.01*th_err1[i]*1.05*h_mstw_band1[2]->GetBinContent(i+1));	  
	  }
	}

      for( int i = 0; i < nbins2-1; i++ ) 
	{
	  double dm_ = mass_xlow2[i+1]-mass_xlow2[i];

	  if (iord == 0) 
	    {
	      //LO  
//	      in_xsec_massNNLO_mstw2 >> xsec_mass_mstw2[i][0] 
//				    >>  xsec_err_mass_mstw2[i][0] ;
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      //in_xsec_massNNLO_mstw2 >> xsec_mass_mstw2[i][1] 
//				    >>  xsec_err_mass_mstw2[i][1] ;
	    } 
	    else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw2 >> xsec_mass_mstw2[i][2] 
				    >>  xsec_err_mass_mstw2[i][2] ;
	      h_2->SetBinContent(i+1,
			       xsec_mass_mstw2[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow2[i+1]-mass_xlow2[i]));  // devide to the Z peak
	    }
	  
	  	  //cout <<"bin2 " << i << " " << xsec_mass_mstw2[i][iord] << " " << xsec_err_mass_mstw2[i][iord] << endl; 
	  	  //cout << "mass bin2 " << mass_xlow2[i+1]-mass_xlow2[i] << " " << i<< endl;
	  h_mstw2[iord]->SetBinContent(i+1,xsec_mass_mstw2[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw2[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw2[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw2[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band2[iord]->SetBinContent(i+1,xsec_mass_mstw2[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band2[iord]->SetBinError(i+1,0.01*th_err2[i]*h_mstw_band2[iord]->GetBinContent(i+1));	  
	}

      for( int i = 0; i < nbins3-1; i++ ) 
	{
	  double dm_ = mass_xlow3[i+1]-mass_xlow3[i];

	  if (iord == 0) 
	    {
	      //LO  
//	      in_xsec_massNNLO_mstw3 >> xsec_mass_mstw3[i][0] 
//				    >>  xsec_err_mass_mstw3[i][0] ;
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      //in_xsec_massNNLO_mstw3 >> xsec_mass_mstw3[i][1] 
//				    >>  xsec_err_mass_mstw3[i][1] ;
	    } 
	    else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw3 >> xsec_mass_mstw3[i][2] 
				    >>  xsec_err_mass_mstw3[i][2] ;
	      h_3->SetBinContent(i+1,
			       xsec_mass_mstw3[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow3[i+1]-mass_xlow3[i]));  // devide to the Z peak
	    }
	  
	  	  //cout <<"bin3 " << i << " " << xsec_mass_mstw3[i][iord] << " " << xsec_err_mass_mstw3[i][iord] << endl; 
	  	  //cout << "mass bin3 " << mass_xlow3[i+1]-mass_xlow3[i] << " " << i<< endl;
	  h_mstw3[iord]->SetBinContent(i+1,xsec_mass_mstw3[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw3[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw3[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw3[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band3[iord]->SetBinContent(i+1,xsec_mass_mstw3[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band3[iord]->SetBinError(i+1,0.01*th_err3[i]*h_mstw_band3[iord]->GetBinContent(i+1));	  
	}

      for( int i = 0; i < nbins4-1; i++ ) 
	{
	  double dm_ = mass_xlow4[i+1]-mass_xlow4[i];

	  if (iord == 0) 
	    {
	      //LO  
//	      in_xsec_massNNLO_mstw4 >> xsec_mass_mstw4[i][0] 
//				    >>  xsec_err_mass_mstw4[i][0] ;
	    } 
	  else if (iord == 1) 
	    {
	      //NLO  
	      //in_xsec_massNNLO_mstw4 >> xsec_mass_mstw4[i][1] 
//				    >>  xsec_err_mass_mstw4[i][1] ;
	    } 
	    else 
	    {
	      //NNLO  
	      in_xsec_massNNLO_mstw4 >> xsec_mass_mstw4[i][2] 
				    >>  xsec_err_mass_mstw4[i][2] ;
	      h_4->SetBinContent(i+1,
			       xsec_mass_mstw4[i][iord]/peak_val_theory*peak_bin_width/(mass_xlow4[i+1]-mass_xlow4[i]));  // devide to the Z peak
	  //cout <<"bin4: " << mass_xlow4[i]  << " " << xsec_mass_mstw4[i][2] << " " << xsec_err_mass_mstw4[i][2] << endl; 
	    }
	  
	  	  //cout << "mass bin4 " << mass_xlow4[i+1]-mass_xlow4[i] << " " << i<< endl;
	  h_mstw4[iord]->SetBinContent(i+1,xsec_mass_mstw4[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw4[iord]->SetBinError(i+1,sqrt(pow((xsec_err_mass_mstw4[i][iord]/peak_val_theory),2)+pow(xsec_mass_mstw4[i][iord]*10./peak_val_theory/peak_val_theory,2)));
	  
	  h_mstw_band4[iord]->SetBinContent(i+1,xsec_mass_mstw4[i][iord]/peak_val_theory*peak_bin_width/dm_);  //devide to the Z peak
	  h_mstw_band4[iord]->SetBinError(i+1,0.01*th_err4[i]*h_mstw_band4[iord]->GetBinContent(i+1));	  
	}
    }
    fth->Write();

    TH1D* herr1 = new TH1D("herr1","herr1",nbins-1,mass_xlow);
    for( int i = 0; i < nbins-1; i++ ) {
      double val1 = data1->GetBinContent(i+1)*err_model[i];
      double val2 = data1->GetBinContent(i+1)*(rdata_err1[i]/rdata_vals1[i]);
      double val = sqrt(val1*val1 + val2*val2);
      herr1->SetBinContent(i+1, data1->GetBinContent(i+1));
      herr1->SetBinError(i+1, val);
    }
    TH1D* herr2 = new TH1D("herr2","herr2",nbins1-1,mass_xlow1);
    for( int i = 0; i < nbins1-1; i++ ) {
      double val1 = data2->GetBinContent(i+1)*err_model_40bins[i];
      double val2 = data2->GetBinContent(i+1)*(rdata_err2[i]/rdata_vals2[i]);
      double val = sqrt(val1*val1 + val2*val2);
      herr2->SetBinContent(i+1, data2->GetBinContent(i+1));
      herr2->SetBinError(i+1, val);
    }

  double fake_par[nbins-1] = {
    /*
    17.5,
    25.0,
    35.0,
    45.0,
    55.0,
    68.0,
    81.0,
    91.0,
    101.0,
    113.0,
    132.5,
    175.0,
    400.0
    */
        17.27,
        24.0,
        34.3,
        44.3,
        54.4,
        68.0,
        81.2,
        91.0,
        100.4,
        113.3,
        132.4,
        173.5,
        312.2
  };
  double fake_par1[nbins1-1] = {
    17.2655
    ,22.281
    ,27.3237
    ,32.3512
    ,37.364
    ,42.3807
    ,47.3957
    ,52.4083
    ,57.4193
    ,61.8588
    ,67.9756
    ,70.1934
    ,74.2101
    ,78.6364
    ,84.0163
    ,89.0294
    ,92.892
    ,98.0758
    ,103.247
    ,107.881
    ,112.353
    ,117.381
    ,122.87
    ,129.344
    ,136.824
    ,145.308
    ,154.795
    ,165.308
    ,177.695
    ,192.178
    ,209.494
    ,231.091
    ,257.117
    ,294.533
    ,348.766
    ,408.592
    ,473.467
    ,551.11
    ,754.288
    ,1196.21
  };

  TGraphAsymmErrors* grae1 = new TGraphAsymmErrors(nbins-1);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      /*
      double x_ = fake_par[ii];
      double y_ = rdata_vals1[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = rdata_err1[ii];
      double eyh_ = rdata_err1[ii];
      */
      double val1 = data1->GetBinContent(ii+1)*err_model[ii];
      double val2 = data1->GetBinContent(ii+1)*(rdata_err1[ii]/rdata_vals1[ii]);
      double val = sqrt(val1*val1 + val2*val2);

      double x_ = fake_par[ii];
      double y_ = rdata_vals1[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = val;
      double eyh_ = val;
      eyl_ = val;
      eyh_ = val;
      grae1->SetPoint(ii,x_,y_);
      grae1->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }
  TGraphAsymmErrors* grae2 = new TGraphAsymmErrors(nbins1-1);
  for( size_t ii=0; ii<nbins1-1; ii++ )
    {
      double val1 = data2->GetBinContent(ii+1)*err_model_40bins[ii];
      double val2 = data2->GetBinContent(ii+1)*(rdata_err2[ii]/rdata_vals2[ii]);
      double val = sqrt(val1*val1 + val2*val2);

      // point shift
      double x_ = fake_par1[ii];
      double y_ = rdata_vals2[ii];
      double exl_ = fake_par1[ii]-mass_xlow1[ii];
      double exh_ = mass_xlow1[ii+1]-fake_par1[ii];
      double eyl_ = val;
      double eyh_ = val;
      /*
      // center of bin
      double x_ = mass_xlow1[ii]+0.5*(mass_xlow1[ii+1]-mass_xlow1[ii]);
      double y_ = rdata_vals2[ii];
      double exl_ = x_-mass_xlow1[ii];
      double exh_ = mass_xlow1[ii+1]-x_;
      //double eyl_ = rdata_err2[ii];
      //double eyh_ = rdata_err2[ii];
      double eyl_ = val;
      double eyh_ = val;
      //cout << "ii = " << ii << " " << x_ << " " << y_ << " " << exl_ << " " << exh_ << " " << eyl_<< " " << eyh_ << endl;
      */
      grae2->SetPoint(ii,x_,y_);
      grae2->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }


  TGraphAsymmErrors* gerr = new TGraphAsymmErrors(nbins-1);
  TGraphAsymmErrors* gerr1 = new TGraphAsymmErrors(3);
  for( size_t ii=0; ii<nbins-1; ii++ )
    {
      double val1 = data1->GetBinContent(ii+1)*err_model[ii];
      double val2 = data1->GetBinContent(ii+1)*(rdata_err1[ii]/rdata_vals1[ii]);
      double val = sqrt(val1*val1 + val2*val2);

      double x_ = fake_par[ii];
      double y_ = rdata_vals1[ii];
      double exl_ = fake_par[ii]-mass_xlow[ii];
      double exh_ = mass_xlow[ii+1]-fake_par[ii];
      double eyl_ = val;
      double eyh_ = val;
      if( ii >= 10 ) {
        gerr1->SetPoint(ii-10,x_,y_);
        gerr1->SetPointError(ii-10,exl_,exh_,eyl_,eyh_);
      }
      //if( ii == 12 ) val = 0.0;
      eyl_ = val;
      eyh_ = val;
      gerr->SetPoint(ii,x_,y_);
      gerr->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }

  TGraphAsymmErrors* gerr2 = new TGraphAsymmErrors(nbins1-1);
  for( size_t ii=0; ii<nbins1-1; ii++ )
    {
      double val1 = data2->GetBinContent(ii+1);//*err_model[ii];
      double val2 = data2->GetBinContent(ii+1)*(rdata_err2[ii]/rdata_vals2[ii]);
      double val = sqrt(val1*val1 + val2*val2);

      // shift point
      double x_ = fake_par1[ii];
      double y_ = rdata_vals2[ii];
      double exl_ = fake_par1[ii]-mass_xlow1[ii];
      double exh_ = mass_xlow1[ii+1]-fake_par1[ii];
      double eyl_ = val;
      double eyh_ = val;
      /*
      // center of bin
      double x_ = mass_xlow1[ii]+0.5*(mass_xlow1[ii+1]-mass_xlow1[ii]);
      double y_ = rdata_vals2[ii];
      double exl_ = x_-mass_xlow1[ii];
      double exh_ = mass_xlow1[ii+1]-x_;
      double eyl_ = val;
      double eyh_ = val;
      //if( ii == 12 ) val = 0.0;
      eyl_ = val;
      eyh_ = val;
      */
      gerr2->SetPoint(ii,x_,y_);
      gerr2->SetPointError(ii,exl_,exh_,eyl_,eyh_);
    }

  // Voila! at this stage, on has
  // 1) one TGraph for the data
  TGraph* dataGraph1   = grae1;
  TGraph* dataGraph2   = grae2;
  TGraph* errGraph   = gerr;
  TGraph* errGraph1   = gerr1;
  TGraph* errGraph2   = gerr2;
  // 2) one histogram for the theory
  TH1* theoryHist     = h_mstw[2];
  TH1* theoryHist1     = h_mstw1[2];
  TH1* theoryHist2     = h_mstw2[2];
  TH1* theoryHist3     = h_mstw3[2];
  TH1* theoryHist4     = h_mstw4[2];
  // 2) one histogram for the theory error
  TH1* theoryBandHist = h_mstw_band[2];
  TH1* theoryBandHist1 = h_mstw_band1[2];
  TH1* theoryBandHist2 = h_mstw_band2[2];
  TH1* theoryBandHist3 = h_mstw_band3[2];
  TH1* theoryBandHist4 = h_mstw_band4[2];

  theoryHist->SetLineColor(kRed);
  theoryHist->SetLineWidth(2);
  theoryHist->SetMarkerSize(0);
  theoryHist->SetFillColor(kRed);
  
  theoryHist1->SetLineColor(kRed);
  theoryHist1->SetLineWidth(2);
  theoryHist1->SetMarkerSize(0);
  theoryHist1->SetFillColor(kRed);
  
  theoryHist2->SetLineColor(kRed);
  theoryHist2->SetLineWidth(2);
  theoryHist2->SetMarkerSize(0);
  theoryHist2->SetFillColor(kRed);
  
  theoryHist3->SetLineColor(kRed);
  theoryHist3->SetLineWidth(2);
  theoryHist3->SetMarkerSize(0);
  theoryHist3->SetFillColor(kRed);
  
  theoryHist4->SetLineColor(kRed);
  theoryHist4->SetLineWidth(2);
  theoryHist4->SetMarkerSize(0);
  theoryHist4->SetFillColor(kRed);
  
  theoryBandHist->SetLineColor(kBlue);
  theoryBandHist->SetLineWidth(2);
  theoryBandHist->SetMarkerSize(0);
  theoryBandHist->SetFillColor(kBlue);

  theoryBandHist1->SetLineColor(kBlue);
  theoryBandHist1->SetLineWidth(2);
  theoryBandHist1->SetMarkerSize(0);
  theoryBandHist1->SetFillColor(kBlue);

  theoryBandHist2->SetLineColor(kBlue);
  theoryBandHist2->SetLineWidth(2);
  theoryBandHist2->SetMarkerSize(0);
  theoryBandHist2->SetFillColor(kBlue);

  theoryBandHist3->SetLineColor(kBlue);
  theoryBandHist3->SetLineWidth(2);
  theoryBandHist3->SetMarkerSize(0);
  theoryBandHist3->SetFillColor(kBlue);

  theoryBandHist4->SetLineColor(kBlue);
  theoryBandHist4->SetLineWidth(2);
  theoryBandHist4->SetMarkerSize(0);
  theoryBandHist4->SetFillColor(kBlue);

  herr1->SetLineColor(kOrange+7);
  herr1->SetLineWidth(2);
  herr1->SetMarkerSize(0);
  herr1->SetFillColor(kOrange+7);
  herr1->SetFillStyle(1001);

  herr2->SetLineColor(kRed+7);
  herr2->SetLineWidth(2);
  herr2->SetMarkerSize(0);
  herr2->SetFillColor(kRed+7);
  herr2->SetFillStyle(1001);

  errGraph->SetLineColor(kOrange+7);
  errGraph->SetLineWidth(0.0);
  errGraph->SetMarkerSize(0);
  errGraph->SetFillColor(kOrange+7);
  errGraph->SetFillStyle(1001);

  errGraph1->SetLineColor(kOrange+7);
  errGraph1->SetLineWidth(0.0);
  errGraph1->SetMarkerSize(0);
  errGraph1->SetFillColor(kOrange+7);
  errGraph1->SetFillStyle(1001);

  errGraph2->SetLineColor(kRed+7);
  errGraph2->SetLineWidth(0.0);
  errGraph2->SetMarkerSize(0);
  errGraph2->SetFillColor(kRed+7);
  errGraph2->SetFillStyle(1001);

  TH1* hfit = theoryHist1;

  // calculate the ratio data/theory
  TGraphAsymmErrors* ratioGraph = 
    (TGraphAsymmErrors*) dataGraph2->Clone("ratio");
  for( int ii=0; ii<dataGraph2->GetN(); ii++ )
    {
      double x_, y_;
      dataGraph2->GetPoint(ii,x_,y_ );
      //      ratioGraph->SetPointEYlow(ii,0);
      //      ratioGraph->SetPointEYhigh(ii,0);
      ratioGraph->SetPoint(ii,x_,0 );
      double eyl_ = dataGraph2->GetErrorYlow(ii);
      double eyh_ = dataGraph2->GetErrorYhigh(ii);
      int jj = hfit->FindBin(x_);
      float fit_ = hfit->GetBinContent( jj );
      //      float binWidth_ = hfit->GetBinWidth(jj);
      //      ratioGraph->SetPointEXlow(ii,binWidth_/2.);
      //      ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
      if( fit_>0 )
	{
	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
	}
      //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
      //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
      //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
    }

  // calculate the ratio band/fit
  TH1* theoryBandRatio = 
    (TH1*) theoryBandHist1->Clone("ratioBand");
  for( int ii=1; ii<=theoryBandHist1->GetNbinsX(); ii++ )
    {
      double y_(0), dy_(0);
      float fit_ =           hfit->GetBinContent(ii);  
      cout << ii << "/fit=" << fit_ << endl; 
      if( fit_>0 )
	{
	  y_  = theoryBandHist1->GetBinContent(ii);
	  dy_ = theoryBandHist1->GetBinError(ii);
	  theoryBandRatio->SetBinContent(ii,y_/fit_);
	  // theoryBandRatio->SetBinContent(ii,1.);
	  theoryBandRatio->SetBinError(ii,dy_/fit_);
	}
    }


  TGaxis::SetMaxDigits(3);

  float r0_ = 1.;
  float dr_ = 0.75;

//   // channels, ordered as in the legend
//   vector<TString> channels;  
//   vector<TString> hnames;
//   vector<TString> type;
//   vector<size_t>  order;  

//   map<TString,int> fillColor_;
//   map<TString,int> lineColor_;
//   int lineWidth1(1);
//   int lineWidth2(1);

//   if( salamanderColors )
//     {
//       lineWidth1 = 1;
//       lineWidth2 = 1;

//       fillColor_["Signal"] = kOrange-2;
//       lineColor_["Signal"] = kOrange+3;
      
//       fillColor_["Ztautau"] = kOrange+7;
//       lineColor_["Ztautau"] = kOrange+3;

//       fillColor_["VV"]     = kOrange+7;
//       lineColor_["VV"]     = kOrange+3;

//       fillColor_["EWK"] = kOrange+10;
//       lineColor_["EWK"] = kOrange+3;

//       fillColor_["QCD"] = kViolet-5;
//       lineColor_["QCD"] = kViolet+3;
      
//       fillColor_["ttbar"] = kRed+2;
//       lineColor_["ttbar"] = kRed+4;      

//       fillColor_["Y"] = kViolet-4;
//       lineColor_["Y"] = kViolet+3;
//     }
//   else
//     {
//       lineWidth1 = 1;
//       lineWidth2 = 1;

//       fillColor_["Signal"] = kSpring-3;
//       lineColor_["Signal"] = kSpring-3;
      
//       fillColor_["Ztautau"] = kOrange;
//       lineColor_["Ztautau"] = kOrange;
      
//       fillColor_["EWK"] = kCyan-4;
//       lineColor_["EWK"] = kCyan-4;
      
//       fillColor_["VV"] = kPink-1;
//       lineColor_["VV"] = kPink-1;
      
//       fillColor_["QCD"] =  kMagenta-3;
//       lineColor_["QCD"] =  kMagenta-3;
      
//       fillColor_["Y"]   =  kBlue+2;
//       lineColor_["Y"]   =  kBlue+2;
      
//       fillColor_["ttbar"] = kAzure;
//       lineColor_["ttbar"] = kAzure;
//     }

  // text2
  size_t ntxt = 3;
  TString txt[3];
  float txtSize[3];
  float txtX[3];
  float txtY[3];
  int   txtAlign[3];
  int   txtFont[3];
  
  txt[0] = "CMS Preliminary";
  txtSize[0] = 0.055;
  txtX[0] = 0.915;
  txtY[0] = 0.935;
  //txtX[0] = 0.88;
  //txtY[0] = 0.84;
  txtAlign[0] = 31;
  txtFont[0] = 42;

  txt[1] = "19.7 fb^{-1} at #sqrt{s} = 8 TeV";
  if( isElectron ) {
    txt[1] = "19.7 fb^{-1} at #sqrt{s} = 8 TeV";
  }
  if( isCombined ) {
    txt[1] = "4.6 fb^{-1} at #sqrt{s} = 8 TeV";
  }
  //txt[1] = "#int#font[12]{L}dt = 36 pb^{-1} at #sqrt{s} = 8 TeV";
  txtSize[1] = 0.042;
  txtX[1] = 0.40;
  txtY[1] = 0.83;
  txtAlign[1] = 21;
  txtFont[1] = 42;
  
  //txt[2] = "#gamma*/Z #rightarrow #mu#mu";
  txt[2] = "#gamma*/Z #rightarrow ee";
  if( isElectron ) txt[2] = "#gamma*/Z #rightarrow ee";
  if( isCombined ) txt[2] = "#gamma*/Z #rightarrow ee, #mu#mu";
  txtSize[2] = 0.06;
  txtX[2] = 0.70;
  txtY[2] = 0.70;
  txtAlign[2] = 21;
  txtFont[2] = 42;
  
  // histogram limits, in linear and logarithmic
  int nbin_(0);
  float xmin_(0.), xmax_(0.); 
  float yminl_(0.), ymaxl_(0.); 

  // titles and axis, marker size
  TString xtitle;
  TString ytitle;
  int ndivx(510);
  int ndivy(510);
  float markerSize(1.2);
  float titleOffsetY(1.15);
  float titleSizeY(0.06);
  if( dataMCRatio )
    titleOffsetY=1.00;
  //  float titleOffsetX(1.00);

  // canvas name
  TString ctitle;

  // legend position and scale;
  float xl_(0);
  float yl_(0);
  float dxl_(0);
  float dyl_(0);
  int legendTextFont=42;
  float legendTextSize=0.040;

  TString massUnit = "GeV";
  TString momUnit = "GeV";
  if( units_with_c )
    {
      massUnit +=  "/#font[52]{c}^{ 2 }";    
      momUnit +=  "/#font[52]{c}";    
    }

  // root file, where the data is
  TString cname;

  cname += "Fig_8";
  ctitle = "DY analysis - comparison with theory";
  xtitle = "M(#mu#mu)  [";
  if( isElectron ) xtitle = "M(ee)  [";
  if( isCombined ) xtitle = "M(ee,#mu#mu)  [";
  xtitle += massUnit;       
  xtitle += "]";
  ytitle = "1/#sigma_{Z} d#sigma/dM(#mu#mu) [GeV^{-1} ]"; 
  if( isElectron ) ytitle = "1/#sigma_{Z} d#sigma/dM(ee) [GeV^{-1} ]"; 
  if( isCombined ) ytitle = "1/#sigma_{Z} d#sigma/dM(ee,#mu#mu) [GeV^{-1} ]"; 

  nbin_ = 100;
  xmin_ = 15.;
  xmax_ = 1500.;
  ndivx = 0;
  ndivy = 506;
	  
  markerSize = 0.7;
    
  int xcanv_ =300;
  int ycanv_ =300;
  int dxcanv_=400;
  int dycanv_=400;

  yminl_ = 0.0000000005;
  ymaxl_ = 5.0;

  xl_  = 0.65;
  yl_  = 0.42;
  dxl_ = 3.;
  dyl_ = 5.0;
  
  //  legendTextSize *= 1.1;

       
  // the canvas
  TCanvas* c_=new TCanvas(cname,ctitle,
			  xcanv_,ycanv_,xcanv_+dxcanv_,ycanv_+dycanv_);
  c_->SetLeftMargin( 110./600 );
  c_->SetRightMargin( 42./600 );
  c_->SetTopMargin(  40./600 );
  c_->SetBottomMargin( 80./600 ); 
  c_->SetFillColor(0);
  c_->SetTickx(1);
  c_->SetTicky(1);
  c_->SetFrameFillStyle(0);
  c_->SetFrameLineWidth(2);
  c_->SetFrameBorderMode(0);

  Double_t scale = 4;
  Double_t wbin = 42*scale;
  Double_t left  = 8*scale;
  Double_t right = 5*scale;
  Double_t h1 = 135*scale;
  //Double_t h2 = 15*scale;
  Double_t h2 = 45*scale;
  Double_t top1 = 15*scale;
  Double_t bot1 = 3*scale;
  Double_t top2 = 3*scale;
  //  Double_t bot1 = 0*scale;
  //  Double_t top2 = 0*scale;
  Double_t bot2 = 80*scale;
  Double_t W = left + wbin + right;
  Double_t H = h1 + h2;
  Double_t s[2] = {1, h1/h2 };

  TPad* pad[2];
  pad[0] = new TPad( "top", "top", 
		     0, h2/H, 1, 1,
		     kWhite,0,0);
  pad[0]->SetLeftMargin(  left/W );
  pad[0]->SetRightMargin( right/W );
  pad[0]->SetTopMargin(  top1/H );
  pad[0]->SetBottomMargin( bot1/H );

  pad[1] = new TPad( "bottom", "bottom", 
		     0, 0, 1, h2/H,
		     kWhite,0,0);
  pad[1]->SetLeftMargin(  left/W );
  pad[1]->SetRightMargin( right/W );
  pad[1]->SetTopMargin(  top2/H );
  pad[1]->SetBottomMargin( bot2/H );
  //  pad[1]->SetGridy();

  for( int ii=0; ii<2; ii++ )
    {
      pad[ii]->SetFillColor(0);
      pad[ii]->SetTickx(1);
      pad[ii]->SetTicky(1);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
      pad[ii]->SetFrameFillStyle(0);
      pad[ii]->SetFrameLineWidth(2);
      pad[ii]->SetFrameBorderMode(0);
    }


//   //  return c_;
  
  // histogram with the correct x axis
  // Warning: setTDRstyle() must be called before
  TH1F* h_= new TH1F( "bidon6", "bidon", nbin_, xmin_, xmax_ );
  TAxis* ax_ = h_->GetXaxis();
  TAxis* ay_ = h_->GetYaxis();
  
  ax_->SetTitle(xtitle);
  //  ax_->CenterTitle();
  ax_->SetNdivisions(ndivx);
  //ax_->SetTitleOffset(titleOffsetX);
  ax_->SetLabelSize(0);
  ax_->SetLabelOffset(99);
  ////ax_->SetLabelOffset(0.008);
  ax_->SetNoExponent(kTRUE);
  ax_->SetMoreLogLabels(kTRUE);

  ay_->SetTitle(ytitle);
  //  ay_->CenterTitle();
  ay_->SetNdivisions(ndivy);
  ay_->SetTitleOffset(titleOffsetY);
  ay_->SetTitleSize(titleSizeY);
  ay_->SetLabelSize(0.04);
  ay_->SetLabelOffset(0.008);

  dataGraph1->SetName("dataGraph1");
  dataGraph1->SetMarkerStyle(24);
  dataGraph1->SetMarkerColor(kRed);
  dataGraph1->SetLineColor(kRed);
  dataGraph1->SetMarkerSize(markerSize);
  
  dataGraph2->SetName("dataGraph2");
  dataGraph2->SetMarkerStyle(kFullCircle);
  dataGraph2->SetMarkerColor(kBlack);
  dataGraph2->SetLineColor(kBlack);
  dataGraph2->SetMarkerSize(markerSize);
  
  TGraph* dummyGraph = (TGraph*) dataGraph1->Clone("dummyGraph");
  dummyGraph->SetLineColor(0);
  dummyGraph->SetMarkerSize(1.2*markerSize);
    
//   TH1* hfit = totalHisto;
//   for( int ii=0; ii<dataGraph->GetN(); ii++ )
//     {
//       dataGraph->GetPoint(ii,x_,y_ );
//       ratioGraph->SetPointEYlow(ii,0);
//       ratioGraph->SetPointEYhigh(ii,0);
//       ratioGraph->SetPoint(ii,x_,0 );
//       double eyl_ = dataGraph->GetErrorYlow(ii);
//       double eyh_ = dataGraph->GetErrorYhigh(ii);
//       int jj = hfit->FindBin(x_);
//       float fit_ = hfit->GetBinContent( jj );
//       float binWidth_ = hfit->GetBinWidth(jj);
//       ratioGraph->SetPointEXlow(ii,binWidth_/2.);
//       ratioGraph->SetPointEXhigh(ii,binWidth_/2.);
//       if( fit_>0 )
// 	{
// 	  ratioGraph->SetPointEYlow(ii,eyl_/fit_);
// 	  ratioGraph->SetPointEYhigh(ii,eyh_/fit_);
// 	  ratioGraph->SetPoint(ii,x_,y_/fit_ );
// 	}
//       //      cout << ii << " ratio=" << ratioGraph->GetY()[ii] 
//       //       	   << "+" << ratioGraph->GetEYhigh()[ii] 
//       //	   << "-" << ratioGraph->GetEYlow()[ii] << endl;
//     }
  
  TH1* hratio_ = (TH1*) h_->Clone("hratio");
  ax_ = hratio_->GetXaxis();
  ay_ = hratio_->GetYaxis();
  ax_->SetLabelOffset(99);
  ax_->SetTitleOffset(99);
  //  ay_->SetTitleSize(0.13);
  //  ay_->SetTitleOffset(0.45);
  //
  // now plotting
  //  
  c_->Draw();
  c_->cd();

 
  TPad* p_ = pad[0];
  p_->Draw();
  p_->cd();

  p_->SetLogy(true);
  p_->SetLogx(true);

  h_->GetYaxis()->SetRangeUser(yminl_,ymaxl_);
  h_->Draw("hist");
    //draw_axis_labels(0.9*yminl_,0.050);

  draw_bin_grid( yminl_, 1.3*yminl_ );
  draw_bin_grid( ymaxl_/1.3, ymaxl_ );

  //draw_axis_labels(r0_-1.15*dr_,0.15);

  bool drawHGrid = false;
  if( drawHGrid )
    {
      TLine grid_;
      grid_.SetLineColor(kGray+2);
      grid_.SetLineWidth(1);
      float y_;
      //      grid_.SetLineStyle(kSolid);
      grid_.SetLineStyle(kDotted);
      y_=0.0001;  grid_.DrawLine(120,y_,xmax_,y_);
      y_=0.001;   grid_.DrawLine(120,y_,xmax_,y_);
      y_=0.01;    grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=0.1;     grid_.DrawLine(xmin_,y_,xmax_,y_);
      y_=1;       grid_.DrawLine(xmin_,y_,xmax_,y_);
      //y_=10;      grid_.DrawLine(xmin_,y_,60,y_);
      //      y_=0.9;  grid_.DrawLine(xmin_,y_,xmax_,y_);
      //      y_=0.8;  grid_.DrawLine(xmin_,y_,xmax_,y_);
    }

    //    theoryHist->Draw("E3Same");
    //theoryBandHist->Draw("E3Same");
    //h->SetLineColor(kGreen+1);
    //h->Draw("Lsame");
    //herr1->Draw("E2same");
    //herr2->Draw("E2same");
    errGraph->SetLineWidth(2);
    errGraph->SetFillColor(kOrange+7); // NEW
    errGraph->SetLineColor(kOrange+7); // NEW
    //errGraph->Draw("E2"); // Rectangular
    errGraph2->SetLineWidth(2);
    errGraph2->SetFillColor(kRed+7); // NEW
    errGraph2->SetLineColor(kRed+7); // NEW
    //errGraph2->Draw("E2same"); // Rectangular
    //errGraph->Draw("E4C"); // continuous plot
    //errGraph1->SetLineWidth(2);
    //errGraph1->SetFillColor(kOrange+7); // NEW
    //errGraph1->SetLineColor(kOrange+7); // NEW
    //errGraph1->Draw("E3Csame"); // continuous plot
    //theoryBandHist4->Smooth();
    if( isTheory ) theoryBandHist4->Draw("E3same");
    //h_4->Smooth();
    h_4->SetLineColor(kBlue);
    h->SetLineColor(kBlue);
    //h->Draw("same");
    if( isTheory ) h_4->Draw("Lsame");

  //  h->SetLineColor(kGreen+1);
  //data1->SetMarkerSize(0);
  //data1->Draw("ESame");
    gStyle->SetEndErrorSize(3);
    
    dataGraph2->Draw("P");
    if( is2010 ) dataGraph1->Draw("PEsame");
    if( is2DProj ) {
      h2DProj->SetLineColor(kRed);
      h2DProj->SetMarkerColor(kRed);
      h2DProj->SetMarkerStyle(26);
      h2DProj->Draw("PEsame");
    }

//   //  h_->Draw("Same");

    xl_ = 0.2;
    yl_ = 0.15;
    dxl_ = 0.22;
    dyl_ = 0.20;

    // legend position and scale;
    TLegend* legend=new TLegend(xl_,yl_,xl_+dxl_,yl_+dyl_);
    legend->SetTextFont(legendTextFont);
    legend->SetTextSize(legendTextSize);
    legend->SetLineColor(0);
    legend->SetFillColor(0);

    if( isCombined ) {
      legend->AddEntry(dataGraph2," Data (e+#mu), 4.6 fb^{-1} in 2012","PLE");
      if( is2010 ) legend->AddEntry(dataGraph1," Data (e+#mu), 36 pb^{-1} in 2010","PLE");
    }
    else if( isElectron ) {
      legend->AddEntry(dataGraph2," Data (e, 19.7 fb^{-1} in 2012)","PLE");
      if( is2010 ) legend->AddEntry(dataGraph1," Data (e, 36 pb^{-1} in 2010)","PLE");
    }
    else {
      legend->AddEntry(dataGraph2," Data (#mu, 19.7 fb^{-1} in 2012)","PLE");
      if( is2010 ) legend->AddEntry(dataGraph1," Data (#mu, 36 pb^{-1} in 2010)","PLE");
    }
    //legend->AddEntry(errGraph," Uncertainty on modeling included","F");
    if( isTheory ) legend->AddEntry(theoryBandHist4," NLO, FEWZ+CT10","F");
    
    legend->Draw("Same");

  //lumi pad, cms prelim pad etc..
  {
    
    TLatex latex;
    latex.SetNDC();    
    for( size_t ii=0; ii<ntxt; ii++ )
      { 
	latex.SetTextFont(txtFont[ii]);
	latex.SetTextSize(txtSize[ii]);    
	latex.SetTextAlign(txtAlign[ii]); 
	latex.DrawLatex(txtX[ii],txtY[ii],txt[ii]);
      }
  }

  // redraw axis
  p_->RedrawAxis();
  p_->GetFrame()->Draw();
  c_->cd();
  
  p_ = pad[1];
  p_->SetLogx();
  p_->Draw();
  p_->cd();

  TAxis* xratio_ = hratio_->GetXaxis();
  TAxis* yratio_ = hratio_->GetYaxis();

  yratio_->SetRangeUser(r0_-0.9999*dr_,r0_+0.9999*dr_);
  //yratio_->SetRangeUser(r0_-0.9999*dr_,r0_+0.9999*dr_);
  yratio_->SetLabelSize( s[1]*yratio_->GetLabelSize() );
  yratio_->SetTitleSize( 0.13 );
  //yratio_->SetTitleSize( s[1]*yratio_->GetTitleSize() );
  //  yratio_->SetLabelOffset( yratio_->GetLabelOffset() );
  yratio_->SetLabelOffset( 0.012 );
  //  yratio_->SetTitleOffset( yratio_->GetTitleOffset()/s[1] );
  yratio_->SetTitleOffset( 0.45 );
  yratio_->SetTitle("data/theory");
  yratio_->SetNdivisions(3);
  
  xratio_->SetLabelSize( s[1]*xratio_->GetLabelSize() );
  xratio_->SetTitleSize( s[1]*xratio_->GetTitleSize() );
  xratio_->SetTitleOffset( 1.0 );
  //   //  xratio_->CenterTitle();
  xratio_->SetLabelOffset(99);
  xratio_->SetTickLength( xratio_->GetTickLength()*s[1] );
  
  hratio_->Draw();
  //theoryBandRatio->Draw("E3Same");
  theoryBandRatio->Draw("E2Same"); //!!!
  draw_axis_labels(r0_-1.15*dr_,0.15);
  //draw_axis_labels(r0_-1.15*dr_,0.15);
  draw_bin_grid( r0_-0.9999*dr_,r0_+1.4999*dr_);
  //draw_bin_grid( r0_-dr_, r0_+dr_ );
  {  
    TLine grid_;
    grid_.SetLineColor(kGray+2);
    grid_.SetLineWidth(1);
    float y_;
    grid_.SetLineStyle(kDashed);
    y_=1;  grid_.DrawLine(xmin_,y_,xmax_,y_);
  }

  
  ratioGraph->SetMarkerSize( ratioGraph->GetMarkerSize()*1. );
  ratioGraph->SetLineColor( kBlack );
  ratioGraph->SetLineWidth( 1 );
  
  ratioGraph->SetMarkerColor( kViolet );
  ratioGraph->SetMarkerStyle( kFullCircle );
  ratioGraph->DrawClone("PE");
  ratioGraph->SetMarkerColor( kBlack );
  ratioGraph->SetMarkerStyle( kOpenCircle );
  ratioGraph->DrawClone("PE");


   p_->RedrawAxis();
   p_->GetFrame()->Draw();
   
   c_->cd();
   return c_;
}

TH2F* getMigMatrix(TH1F* htrue, TH2F* hden) {
  
    TH2F* migMatrix = (TH2F*)hden->Clone();
    const int nbin = htrue->GetXaxis()->GetNbins();
  
    for( int i = 0; i < nbin+2; i++ ) {
        for( int j = 0; j < nbin+2; j++ ) {
          double den = htrue->GetBinContent(i);
          migMatrix->SetBinContent(i, j, hden->GetBinContent(i,j)/den);
          //if (i==j)  cout << "XXX " << i << " " << migMatrix->GetBinContent(i,j)<< endl;
        }
     }

   //printHisto2D(migMatrix); 

   return migMatrix;
}
