#define UTIL
#include <utility>
#endif

#ifndef MAP
#define MAP
#include <map>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef IOMANIP
#define IOMANIP
#include <iomanip>
#endif


void rootInit(void );
TFile * openFile(bool isDcache, char *file_name);
//void printTheOutput(TH2D * getThisHisto, bool printContent_orErr, string name_str,  ofstream &outputFile);
void testMe(void);

void drawNicePlots_DYAndrius(std::string caseAna = "1D", std::string channel = "mu"){
  string whichSignMu = "";// both
  //string whichSignMu = "pos";
  //string whichSignMu = "neg";
  //TCanvas *c2 = new TCanvas("c2", "canvas 2",16,30,800,700);
  TCanvas* c2 = new TCanvas("c2", "canvas 2", 16,30, 800,800);
  rootInit();
  //bool isDcache = true;
  //string MyDirectory = "root://eoscms.cern.ch//eos/cms/$EOSCAF/FNAL/DY_output/53X/";

  bool isDcache = false;
  string MyDirectory = "./";
  string MySubDirectory = "./";
  string MyFileName ;
  string MyFullPath;
  char *file_name;
  MyFileName = "effCorr_2D_case_26.10.12.root";

  MyFullPath =  MyDirectory +  MySubDirectory + MyFileName;
  file_name = MyFullPath.c_str();
  TFile *f1 = openFile(isDcache, file_name);
  string f1_output = "EvtEff_Fill2621";


  char *file_name;
  //MyFileName = "covariance_finalResults_1D.root"; //covariance2D_finalResults_26.10.12.root";
  MyFileName = "../covAndrius/Covariance/covariance_finalResults_1D_mu_preFSR_fullAcc_loadRhoRho100_noAccSyst_normalized-13binErr.root";
  MyFullPath =  MyDirectory +  MySubDirectory + MyFileName;
  file_name = MyFullPath.c_str();
  TFile *f2 = openFile(isDcache, file_name);
  string f2_output = "EvtEff_Fill2621";


  // All files in a vector
  std::vector< TFile * > DataFiles;
  DataFiles.push_back(f1);
  DataFiles.push_back(f2);

  std::vector< string > DataFiles_names;
  DataFiles_names.push_back(f1_output);
  DataFiles_names.push_back(f2_output);

  string  str_tm_8 = "totalCov_TM";

  std::string binStr = "mass bin; mass bin";
  std::string binStr_single = "mass bin";
  if("2D" == caseAna){
     binStr = "mass-rapidity bin; mass-rapidity bin";
     binStr_single = "mass-rapidity bin";
  }
  //
  gPad->SetRightMargin(0.12);
  TMatrixT <double> *tm_8  = (TMatrixT <double> *)(DataFiles[1])->Get(str_tm_8.c_str());;  
  TH2D *h2_tm_8 = new TH2D (*tm_8);
  string nameHist = "Total correlation matrix;";
  nameHist+=binStr;
 // h2_tm_8->SetTitle(nameHist.c_str()); 
  h2_tm_8->GetXaxis()->SetTitle(binStr_single.c_str());
  h2_tm_8->GetYaxis()->SetTitle(binStr_single.c_str());
  h2_tm_8->GetXaxis()->SetTitleOffset(1.1);
  h2_tm_8->GetYaxis()->SetTitleOffset(1.25);
  h2_tm_8->GetXaxis()->SetTitleSize(0.04);
  h2_tm_8->GetYaxis()->SetTitleSize(0.04);
  h2_tm_8->SetTitle("CMS Preliminary, 4.5 fb^{-1} at #sqrt{s} = 7 TeV");
  gStyle->SetTitleW(0.4);
  gStyle->SetTitleH(0.07);
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleY(0.97);
  h2_tm_8->Draw("colz"); 
  bluePrintOut(h2_tm_8);
  //return; 
  //
  TPaveText *pt = new TPaveText(0.75,0.77,0.78,0.80,"brNDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(0.040);
  pt->SetBorderSize(0);
  pt->SetTextAlign(31);
  pt->AddText("at #sqrt{s} = 7 TeV, CMS Preliminary");
  pt->Draw("same");

}



void rootInit(void ){
  gStyle->SetTitleBorderSize(0); 
  gStyle->SetOptStat(0);
  //gStyle->SetOptStat(111111111);
  gStyle->SetTitleFillColor(0);
  gStyle->SetHistFillStyle(1001);


  gStyle->SetHistFillStyle(1001);
  gStyle->SetHistFillColor(51);
  gStyle->SetHistLineWidth(2);
  //gStyle->SetHistLineColor(4);
  gStyle->SetFrameFillColor(0);
  gStyle->SetTitleW(0.65);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.25);
  gStyle->SetStatH(0.2);
  gStyle->SetStatColor(0);
  gStyle->SetHistFillStyle(5101);
  //gStyle->SetErrorX(0);
  gStyle->SetEndErrorSize(0);
  //c2->Divide(2,2);
  /*
    c2->Divide(2,2);
    for(int i=1;i<5;++i){
    c2->cd(i)->SetFillStyle(0);
    c2->cd(i)->SetGrid();
    c2->cd(i)->SetBorderMode(0);
    c2->SetBorderMode(0);
    }
  */
  gStyle->SetPalette(1);
  // gPad:
  gPad->SetFrameFillColor(0);
  gPad->SetFillColor(0);
  gPad->SetBorderMode(0);
  c2->SetFillStyle(0);
  c2->SetGrid();
  //c2->SetLogx();
}
//
TFile * openFile(bool isDcache, char *file_name){

  TFile *f1=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name);
  if(isDcache){
    f1=TFile::Open(file_name);
  }
  if (!f1){ 
    TFile * f1 = new TFile(file_name);
  }
  return f1;
}

void testMe(void) {

  string MyDirectory = "./";
  string MySubDirectory = "./";
  char *file_name;
  string MyFileName = "covariance_finalResults_2D_mu_preFSR_inAcc_normalized.root";
  MyFullPath =  MyDirectory +  MySubDirectory + MyFileName;
  file_name = MyFullPath.c_str();
  bool isDcache = false;
  TFile *f = openFile(isDcache, file_name);
  //string f2_output = "EvtEff_Fill2621";
  //TFile *f1 = openFile(isDcache, file_name);
  //TString _trig = "HLT_Mu13_Mu8";
  //TString _corr = "corrRun2011B";
  //TFile* f = new TFile("rootfiles/resMatrixProd_2Dunfold_"+_corr+".root","open");

  string  str_tm_8 = "totalCorr_TM";
  //
  //nameHist = "Total correlation matrix;";
  //nameHist+=binStr;
  //h2_tm_8->SetTitle(nameHist.c_str()); 
  // All files in a vector
  std::vector< TFile * > DataFiles;
  DataFiles.push_back(f);
  TMatrixT <double> *tm_8  = (TMatrixT <double> *)(DataFiles[0])->Get(str_tm_8.c_str());;  
  TH2D *h2_tm_8 = new TH2D (*tm_8);

  f->cd();

  TH1D* hmig = (TH1D*)h2_tm_8->Clone();
  TH1D* hres = (TH1D*)h2_tm_8->Clone();
  //TH1D* hmig = (TH1D*)hden->Clone();
  //TH1D* hres = (TH1D*)migMatrix->Clone();
  // rootInit();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  //gStyle->SetPaintTextFormat("4.2g");
  
  TCanvas* c2 = new TCanvas("c2", "c2", 800, 800);
  rootInit();
  c2->cd();
  c2->SetGridy();
  c2->SetGridx();
  //c2->SetMargin(0.1,0.6, 0.2,0.4);
  hmig->SetTitle(""); 
  //hmig->SetTitle("CMS at 7TeV, Lumi = 36.1 pb^{-1}");
  hmig->GetXaxis()->SetTitle("generated mass / rapidity bins");
  hmig->GetYaxis()->SetTitle("reconstructed mass / rapidity bins");
  hmig->Draw("COLZ");
  //c1->Print("eps/migMatrix_"+_trig+"_"+_corr+".png");
  //return;  

  //TCanvas* c2 = new TCanvas("c2", "c2", 900, 750);
  TCanvas* c2 = new TCanvas("c2", "c2", 800, 920);
  c2->cd();
  c2->SetBottomMargin(0.2);
  //c2->SetLeftMargin(0.1);
  //c2->SetRightMargin(0.7);
  gPad->SetRightMargin(0.12); 
  //gStyle->SetPadRightMargin(0.3);
  //c2->SetMargin(0.1,0.9, 0.2,0.4);
  //c2->SetCanvasSize(900,600);
  //c2->SetWindowSize(900,600);
  //c2->SetBottomMargin(0.1);
  
  //hres->SetTitle("Total correlation matrix");
  //hres->SetTitle("CMS at 7TeV, Lumi = 36.1 pb^{-1}");
  hres->GetXaxis()->SetTitleSize(0.03);
  hres->GetYaxis()->SetTitleSize(0.03);
  hres->GetXaxis()->SetLabelSize(0.03);
  hres->GetYaxis()->SetLabelSize(0.03);
  hres->GetZaxis()->SetLabelSize(0.04);
  hres->GetXaxis()->SetTitleOffset(1.2);
  hres->GetYaxis()->SetTitleOffset(1.2);
  //gStyle->SetNDivisions(10);
  //hres->GetXaxis()->SetNdivisions(2);
  //hres->GetYaxis()->SetNDivisions(10);
  //hres->SetTitle(";rapidity for each mass bin;rapidity for each mass bin");
  hres->GetXaxis()->SetTitle("rapidity for each mass bin");
  hres->GetYaxis()->SetTitle("rapidity for each mass bin");
  //hres->SetMinimum(0.0001);
  
  ostringstream tmpstrm;
  for( int i = 0; i < 6; i++ ) {
    int nbins;
    double bin_size = 0;
    if( i == 5 ) {
      nbins = 12;
      bin_size = 0.2; 
    }
    else {
      nbins = 24;
      bin_size = 0.1;
    }
  
    for( int j = 0; j < nbins; j++ ) {
      tmpstrm.str("");
      tmpstrm << j*bin_size;
      if( j % 4 == 3 ) {
      //if( j % 4 == 0  ) {
        hres->GetXaxis()->SetBinLabel(i*24+j, tmpstrm.str().c_str());
        hres->GetYaxis()->SetBinLabel(i*24+j, tmpstrm.str().c_str());
      }
      else {
        hres->GetXaxis()->SetBinLabel(i*24+j, "");
        hres->GetYaxis()->SetBinLabel(i*24+j, "");
      }
    }
  }
  //TAxis *yaxis2 = hres->GetXaxis(510); 
  hres->GetYaxis()->SetNdivisions(-2);
  //yaxis2->SetNDivisions();
  //gROOT->ProcessLine(".L ~/setTDRStyle.C");
  //setTDRStyle();
  //gROOT->SetStyle("tdrStyle");
  //gROOT->ForceStyle(true);
  //tdrStyle->SetTitleX(0.13);
  //tdrStyle->SetTitleY(0.975);
 //hres->SetTitleSize(0.1);
  hres->SetTitle("at #sqrt{s} = 7 TeV, CMS Preliminary");
  gStyle->SetTitleW(0.4);
  gStyle->SetTitleH(0.07);
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleY(0.97);

  hres->Draw("COLZ");
  hres->GetYaxis()->SetNdivisions(-2);
  //hres->Print("all");

  //TGaxis *axis1 = new TGaxis(0, -20.0, 23.5, -20.0, 0, 1, 510, "+L");
  TGaxis *axis1 = new TGaxis(0, -20.0, 23.5, -20.0, 0, 1, 1, "+L");
  axis1->SetName("axis1");
  axis1->SetLabelSize(0.0);
  axis1->SetTitle("20 < M < 30");
  axis1->SetTitleSize(0.023);
  axis1->SetTitleOffset(0.5);
  axis1->CenterTitle();
  axis1->Draw();

  TGaxis *axis2 = new TGaxis(24, -20.0, 47.5, -20.0, 0, 1, 1, "+L");
  axis2->SetName("axis2");
  axis2->SetLabelSize(0.0);
  axis2->SetTitle("30 < M < 45");
  axis2->SetTitleSize(0.023);
  axis2->SetTitleOffset(0.5);
  axis2->CenterTitle();
  axis2->Draw();

  TGaxis *axis3 = new TGaxis(48, -20.0, 71.5, -20.0, 0, 1, 1, "+L");
  axis3->SetName("axis3");
  axis3->SetLabelSize(0.0);
  axis3->SetTitle("45 < M < 60");
  axis3->SetTitleSize(0.023);
  axis3->SetTitleOffset(0.5);
  axis3->CenterTitle();
  axis3->Draw();

  TGaxis *axis4 = new TGaxis(72, -20.0, 95.5, -20.0, 0, 1, 1, "+L");
  axis4->SetName("axis4");
  axis4->SetLabelSize(0.0);
  axis4->SetTitle("60 < M < 120");
  axis4->SetTitleSize(0.023);
  axis4->SetTitleOffset(0.5);
  axis4->CenterTitle();
  axis4->Draw();

  TGaxis *axis5 = new TGaxis(96, -20.0, 119.5, -20.0, 0, 1, 1, "+L");
  axis5->SetName("axis5");
  axis5->SetLabelSize(0.0);
  axis5->SetTitle("120 < M < 200");
  axis5->SetTitleSize(0.023);
  axis5->SetTitleOffset(0.5);
  axis5->CenterTitle();
  axis5->Draw();

  TGaxis *axis6 = new TGaxis(120, -20.0, 132, -20.0, 0, 1, 1, "+L");
  axis6->SetName("axis6");
  axis6->SetLabelSize(0.0);
  axis6->SetTitle("M > 200");
  axis6->SetTitleSize(0.023);
  axis6->SetTitleOffset(0.5);
  axis6->CenterTitle();
  axis6->Draw();

  //c2->Print("eps/resMatrix_"+_trig+"_"+_corr+".eps");
}

void bluePrintOut(TH2D* hprototype) {

        for (int i = 0; i < hprototype->GetXaxis()->GetNbins(); i++) {
           for (int j = 0; j < hprototype->GetYaxis()->GetNbins(); j++) {
                //if (i==j) {
                //if (i > 37 && j > 37) {
                //   cout << i+1 << " " << j+1 << " " << 1./250.*hprototype->GetBinContent(i+1,j+1)  << endl;
                //} else {
                   cout << i+1 << " " << j+1 << " " << hprototype->GetBinContent(i+1,j+1)  << endl;
                //} 
                //} else {
                //   cout << i+1 << " " << j+1 << " 0.0" << endl;
                //}
           }
        }
}

