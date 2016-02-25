#include <iostream>
#include <iomanip>
using namespace std;

void plotter() 
{
  gROOT->ProcessLine(".L ../../tools/misctools.C");
  gROOT->ProcessLine(".L ../../tools/histotools.C");
  gROOT->ProcessLine(".L ../../tools/plottools.C");

  //
  //PREPARE THEORY
  // 
  TString thisPDF = "CTEQ12NNLO";
  TString thisPDF2 = "NNPDF21nnlo";
  bool normalizeToZ = false;
  TString subfolder = "_cuts_";
  bool compareToPOWHEG = false;
  TString WHICHTYPE = "_inAcc";
  TString RR = "_";


  const int nbinsM = 6;
  TString beauty_slices[nbinsM] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};

  TH1D* hfewz1 = acceptFEWZInputs(beauty_slices[0], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz2 = acceptFEWZInputs(beauty_slices[1], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz3 = acceptFEWZInputs(beauty_slices[2], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz4 = acceptFEWZInputs(beauty_slices[3], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz5 = acceptFEWZInputs(beauty_slices[4], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz6 = acceptFEWZInputs(beauty_slices[5], subfolder, thisPDF, normalizeToZ);

  TH1D* hfewz1b = acceptFEWZInputs(beauty_slices[0], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz2b = acceptFEWZInputs(beauty_slices[1], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz3b = acceptFEWZInputs(beauty_slices[2], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz4b = acceptFEWZInputs(beauty_slices[3], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz5b = acceptFEWZInputs(beauty_slices[4], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz6b = acceptFEWZInputs(beauty_slices[5], subfolder, thisPDF2, normalizeToZ);

  // 0: all corrections are applied, preFSR; 
  // 1: all corrections are applied, postFSR; 
  // 2: no acceptance correction, preFSR, 
  // 3: no acceptance correction, postFSR
  int shaperType = 2;

  //
  //GET PDF UNCERTAINTY BAND PER SLICE
  //
  TFile* fsys = new TFile("../../Inputs/sys/pdf_syst2D_7TeV.root");
  fsys->cd();
  TH1D* hsys1 = (TH1D*)gDirectory->Get("hslice1");
  TH1D* hsys2 = (TH1D*)gDirectory->Get("hslice2");
  TH1D* hsys3 = (TH1D*)gDirectory->Get("hslice3");
  TH1D* hsys4 = (TH1D*)gDirectory->Get("hslice4");
  TH1D* hsys5 = (TH1D*)gDirectory->Get("hslice5");
  TH1D* hsys6 = (TH1D*)gDirectory->Get("hslice6");

  //add PDF systematics to theory
  addPDFSystematics(hfewz1,hsys1);
  addPDFSystematics(hfewz1b,hsys1);
  addPDFSystematics(hfewz2,hsys2);
  addPDFSystematics(hfewz2b,hsys2);
  addPDFSystematics(hfewz3,hsys3);
  addPDFSystematics(hfewz3b,hsys3);
  addPDFSystematics(hfewz4,hsys4);
  addPDFSystematics(hfewz4b,hsys4);
  addPDFSystematics(hfewz5,hsys5);
  addPDFSystematics(hfewz5b,hsys5);
  addPDFSystematics(hfewz6,hsys6);
  addPDFSystematics(hfewz6b,hsys6);

  //
  //GET THE CROSS SECTION
  //
  TFile* fdata = new TFile("../absex_DET2D_EE.root");
  fdata->cd();
  TH1D* rshape1 = (TH1D*)gDirectory->Get("20to30");
  TH1D* rshape2 = (TH1D*)gDirectory->Get("30to45");
  TH1D* rshape3 = (TH1D*)gDirectory->Get("45to60");
  TH1D* rshape4 = (TH1D*)gDirectory->Get("60to120");
  TH1D* rshape5 = (TH1D*)gDirectory->Get("120to200");
  TH1D* rshape6 = (TH1D*)gDirectory->Get("200to1500");

  //
  //PLOT THE RESULTS
  //
  gStyle->SetOptStat(0);
  gROOT->ProcessLine(".L ../../ControlPlots/setTDRStyle.C");
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");
  gROOT->ForceStyle(true);
  tdrStyle->SetTitleX(0.14);
  tdrStyle->SetTitleY(0.973);
  gStyle->SetEndErrorSize(2);
  gStyle->SetErrorX(0.5);

  cout << "All done, drawing...." << endl;
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape1,hfewz1,hfewz1b,hsys1,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape2,hfewz2,hfewz2b,hsys2,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape3,hfewz3,hfewz3b,hsys3,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape4,hfewz4,hfewz4b,hsys4,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape5,hfewz5,hfewz5b,hsys5,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape6,hfewz6,hfewz6b,hsys6,RR);

}

TString getBinLabel(TString name) {
  if (name == "hslice1") return "20 < M(ee) < 30 GeV";
  if (name == "hslice2") return "30 < M(ee) < 45 GeV";
  if (name == "hslice3") return "45 < M(ee) < 60 GeV";
  if (name == "hslice4") return "60 < M(ee) < 120 GeV";
  if (name == "hslice5") return "120 < M(ee) < 200 GeV";
  if (name == "hslice6") return "200 < M(ee) < 1500 GeV";
} 

TH1D* getFSRSystsematics2D(TString onOff) {

  double fsr_sys1_m1[24] = {0.00647234, 0.00415486, 0.0108202, 0.00403136, 0.00574873, 0.00362989, 0.00505102, 0.00114965, 0.00389124, 0.00646128, 0.00447475, 0.00228061, 0.00227461, 0.00249416, 0.00829864, 0.0018035, 0.00283594, 0.00214582, 0.00574902, 0.00413263, 0.0118533, 0.0162197, 0.0334081, 0.0494639};
  double fsr_sys1_m2[24] = {0.00695026, 0.00616308, 0.00806352, 0.00985189, 0.00879987, 0.00502088, 0.00441509, 0.00713293, 0.00332536, 0.00510922, 0.00583788, 0.0020042, 0.00473196, 0.000193928, 0.00303252, 0.00372845, 0.00690953, 0.0059758, 0.00995221, 0.00939139, 0.0157836, 0.0245717, 0.0282447, 0.0627514};
  double fsr_sys1_m3[24] = {0.00678912, 0.0101303, 0.0162067, 0.0147403, 0.0305059, 0.00329938, 0.00901348, 0.0160075, 0.000727849, 0.00321051, 0.00881395, 0.018282, 0.0108828, 0.00375671, 0.00434735, 0.0220398, 0.0122086, 0.00991563, 0.0297635, 0.0411319, 0.0526711, 0.0436081, 0.099442, 0.150372};
  double fsr_sys1_m4[24] = {4.27893e-05, 0.000298569, 0.000471714, 0.000501392, 0.000142054, 5.03784e-05, 0.000369636, 7.02515e-05, 2.51258e-05, 0.00173303, 0.000761879, 4.7479e-05, 0.000873971, 0.00112472, 0.00114911, 0.00131473, 0.00301244, 0.00312481, 0.00343671, 0.00204168, 0.00540447, 0.00629831, 0.00848048, 0.0205518};
  double fsr_sys1_m5[24] = {0.00428379, 0.00621338, 0.0098965, 0.00695492, 0.00581896, 0.00176032, 0.00428015, 0.00163176, 0.00359524, 0.000123611, 0.00205836, 0.0026989, 0.00422844, 0.00111364, 0.00202451, 0.00187149, 0.000535225, 0.00944515, 0.0100203, 0.0112066, 0.00334101, 0.00116116, 0.0181624, 0.0111227};
  double fsr_sys1_m6[12] = {0.00295293, 0.00032152, 0.000433585, 0.00247265, 0.00420994, 0.00412127, 0.00287175, 0.00241768, 0.00799397, 0.00205394, 0.00574837, 0.0110666};

  TH1D* hfsr_sys1_m1 = new TH1D("hfsr_sys1_m1","hfsr_sys1_m1",24,0,2.4);
  TH1D* hfsr_sys1_m2 = new TH1D("hfsr_sys1_m2","hfsr_sys1_m2",24,0,2.4);
  TH1D* hfsr_sys1_m3 = new TH1D("hfsr_sys1_m3","hfsr_sys1_m3",24,0,2.4);
  TH1D* hfsr_sys1_m4 = new TH1D("hfsr_sys1_m4","hfsr_sys1_m4",24,0,2.4);
  TH1D* hfsr_sys1_m5 = new TH1D("hfsr_sys1_m5","hfsr_sys1_m5",24,0,2.4);
  TH1D* hfsr_sys1_m6 = new TH1D("hfsr_sys1_m6","hfsr_sys1_m6",12,0,2.4);
  for (int i = 0; i < 24; i++) {    
    if (onOff == "on") {
      hfsr_sys1_m1->SetBinContent(i+1,fsr_sys1_m1[i]);    hfsr_sys1_m1->SetBinError(i+1,0.00001);
      hfsr_sys1_m2->SetBinContent(i+1,fsr_sys1_m2[i]);    hfsr_sys1_m2->SetBinError(i+1,0.00001);
      hfsr_sys1_m3->SetBinContent(i+1,fsr_sys1_m3[i]);    hfsr_sys1_m3->SetBinError(i+1,0.00001);
      hfsr_sys1_m4->SetBinContent(i+1,fsr_sys1_m4[i]);    hfsr_sys1_m4->SetBinError(i+1,0.00001);
      hfsr_sys1_m5->SetBinContent(i+1,fsr_sys1_m5[i]);   hfsr_sys1_m5->SetBinError(i+1,0.00001);
    } else {
      hfsr_sys1_m1->SetBinContent(i+1,0.00001);    hfsr_sys1_m1->SetBinError(i+1,0.00001);
      hfsr_sys1_m2->SetBinContent(i+1,0.00001);    hfsr_sys1_m2->SetBinError(i+1,0.00001);
      hfsr_sys1_m3->SetBinContent(i+1,0.00001);    hfsr_sys1_m3->SetBinError(i+1,0.00001);
      hfsr_sys1_m4->SetBinContent(i+1,0.00001);    hfsr_sys1_m4->SetBinError(i+1,0.00001);
      hfsr_sys1_m5->SetBinContent(i+1,0.00001);   hfsr_sys1_m5->SetBinError(i+1,0.00001);
    }
  }
  for (int i = 0; i < 12; i++) {
    hfsr_sys1_m6->SetBinContent(i+1,fsr_sys1_m6[i]);
    hfsr_sys1_m6->SetBinError(i+1,0.00001); 
  }

  return sliceCombiner(hfsr_sys1_m1,hfsr_sys1_m2,hfsr_sys1_m3,hfsr_sys1_m4,hfsr_sys1_m5,hfsr_sys1_m6,true);

}

void addPDFSystematics(TH1D* hfewz, TH1D* hsys) {

   for (int i = 0; i < hfewz->GetNbinsX(); i++) {
        hfewz->SetBinError(i+1,hfewz->GetBinContent(i+1)*sqrt(pow(hfewz->GetBinError(i+1)/hfewz->GetBinContent(i+1),2)+pow(hsys->GetBinError(i+1),2)));
    }
}

TH1D* getBkgSystematics(TString onOff) {

  // QCD sys
  double qcd_sys_m1[24] = {0.0056532, 0.00550908, 0.00540882, 0.00474059, 0.00548575, 0.00560484, 0.00511055, 0.00545808, 0.00538611, 0.00525683, 0.00526199, 0.00570695, 0.00502583, 0.00617854, 0.00596419, 0.00584117, 0.00656915, 0.00650653, 0.0070545, 0.00692165, 0.00713966, 0.00888529, 0.0101327, 0.0155619};
  double qcd_sys_m2[24] = {0.00351196, 0.00367751, 0.00408944, 0.00370759, 0.0036681, 0.00329544, 0.0035952, 0.003873, 0.00361388, 0.0037399, 0.00379453, 0.00390213, 0.00434069, 0.00337894, 0.00366135, 0.00382634, 0.00352825, 0.00326635, 0.00446296, 0.00448979, 0.00526434, 0.00708578, 0.0230291, 0.0237456};
  double qcd_sys_m3[24] = {0.00440309, 0.00445827, 0.00506042, 0.00467334, 0.00498347, 0.00437084, 0.00459451, 0.00526771, 0.00456958, 0.00513797, 0.00448449, 0.0038474, 0.00442588, 0.00349733, 0.00345998, 0.00297294, 0.00358496, 0.00686592, 0.00272317, 0.00275562, 0.00531983, 0.0125791, 0.00597966, 7.35413e-07}; 
  double qcd_sys_m4[24] = {0.000241935, 0.000219588, 0.000209963, 0.000202262, 0.000219526, 0.0001979, 0.000183811, 0.000237073, 0.000196266, 0.000196642, 0.000322948, 0.000210226, 0.000180688, 0.000270308, 0.000443863, 0.000490265, 0.000799433, 0.00075193, 0.000423587, 0.000454509, 0.00134244, 0.000674608, 2.61718e-08, 7.0583e-08};
  double qcd_sys_m5[24] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double qcd_sys_m6[24] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  
  TH1D* hqcd_sys_m1 = new TH1D("hqcd_sys_m1","hqcd_sys_m1",24,0,2.4);
  TH1D* hqcd_sys_m2 = new TH1D("hqcd_sys_m2","hqcd_sys_m2",24,0,2.4);
  TH1D* hqcd_sys_m3 = new TH1D("hqcd_sys_m3","hqcd_sys_m3",24,0,2.4);
  TH1D* hqcd_sys_m4 = new TH1D("hqcd_sys_m4","hqcd_sys_m4",24,0,2.4);
  TH1D* hqcd_sys_m5 = new TH1D("hqcd_sys_m5","hqcd_sys_m5",24,0,2.4);
  TH1D* hqcd_sys_m6 = new TH1D("hqcd_sys_m6","hqcd_sys_m6",24,0,2.4);
  
  for (int i = 0; i < 24; i++) {
    hqcd_sys_m1->SetBinContent(i+1,qcd_sys_m1[i]);
    hqcd_sys_m1->SetBinError(i+1,0.00001);
    hqcd_sys_m2->SetBinContent(i+1,qcd_sys_m2[i]);
    hqcd_sys_m2->SetBinError(i+1,0.00001);
    hqcd_sys_m3->SetBinContent(i+1,qcd_sys_m3[i]);
    hqcd_sys_m3->SetBinError(i+1,0.00001);
    hqcd_sys_m4->SetBinContent(i+1,qcd_sys_m4[i]);
    hqcd_sys_m4->SetBinError(i+1,0.00001);
    hqcd_sys_m5->SetBinContent(i+1,qcd_sys_m5[i]);
    hqcd_sys_m5->SetBinError(i+1,0.00001);
    hqcd_sys_m6->SetBinContent(i+1,qcd_sys_m6[i]);
    hqcd_sys_m6->SetBinError(i+1,0.00001);
  }

  TH1D* qcd_sys = sliceCombiner(hqcd_sys_m1,hqcd_sys_m2,hqcd_sys_m3,hqcd_sys_m4,hqcd_sys_m5,hqcd_sys_m6,true);


  // ttbar sys
  TFile* f = new TFile("~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/Inputs/sys/bkg_syst2013_2D.root");
  f->cd();
  TH1D* ttbar_sys = (TH1D*)gDirectory->Get("hsyst2D");

  TH1D* bkg_sys = (TH1D*)ttbar_sys->Clone();
  for (int i = 0; i < bkg_sys->GetNbinsX(); i++) {
     if (onOff == "on") {
        bkg_sys->SetBinContent(i+1,sqrt(pow(ttbar_sys->GetBinContent(i+1),2)+pow(qcd_sys->GetBinContent(i+1),2)));
     } else {
        bkg_sys->SetBinContent(i+1,0.00001);
     }
  }

  return bkg_sys;

}

TH1D* acceptFEWZInputs(TString beauty_label, TString subfolder, TString thisPDF, bool normalizeToZ) {

  //TString beauty_slices[nbinsM] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};


  TFile *f_fewzio  = new TFile("../../Inputs/Theory/2Dabsxsec"+beauty_label+"_NNLO"+subfolder+thisPDF+".root","open");
  f_fewzio->cd();
  TH1D* hfewz = (TH1D*)xsec->Clone();
  hfewz->SetName(beauty_label);
  if (beauty_label == "200to1500") hfewz->Rebin(2);

  //Always access 4th slice for normalization
  TFile *f_norm  = new TFile("../../Inputs/Theory/2Dabsxsec60to120_NNLO"+subfolder+thisPDF+".root","open");
  f_norm->cd();
  TH1D* hfewz4 = (TH1D*)xsec->Clone();
  hfewz4->SetName("60to120");

  double totFEWZXsec = 0.;//970;
  double binSizeF = 0.1;
  if (normalizeToZ) {
     totFEWZXsec = 2.*hfewz4->Integral();
  } else {
     totFEWZXsec = 1.;
  }
  if (beauty_label == "200to1500") binSizeF = 0.2;

  for (int i = 0; i < hfewz->GetXaxis()->GetNbins(); i++) {
    hfewz->SetBinContent(i+1,2.*hfewz->GetBinContent(i+1)/totFEWZXsec/binSizeF);
    hfewz->SetBinError(i+1,hfewz->GetBinError(i+1)/totFEWZXsec/binSizeF);
  }

  return hfewz;

}
