void pdf_uncertainties(void) {

  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("4.2f");

  //ratio to the Z
  double slice1_syst[24] = {0.0488996, 0.0480503, 0.0468811, 0.0465003, 0.0458715, 0.045876, 0.0447526, 0.0430188, 0.0431146, 0.0413629, 0.0414581, 0.038216, 0.0376922, 0.0362282, 0.0366756, 0.0341411, 0.0340329, 0.035102, 0.0356915, 0.0338529, 0.0351627, 0.0353477, 0.0371606, 0.039907};
  double slice2_syst[24] = {0.0401328, 0.0393695, 0.0393287, 0.0389178, 0.0369393, 0.0363564, 0.0351071, 0.0335794, 0.0326966, 0.0314976, 0.0288921, 0.0284314, 0.0270263, 0.0269579, 0.0263543, 0.0268105, 0.0274705, 0.0282361, 0.0299226, 0.0304501, 0.0316045, 0.0332126, 0.0350832, 0.0364597};
  double slice3_syst[24] = {0.0295948, 0.0295461, 0.0296231, 0.0281969, 0.0267823, 0.0255306, 0.0235360, 0.0223563, 0.0210731, 0.0196623, 0.0179633, 0.0179035, 0.0180664, 0.0182034, 0.0194056, 0.0209625, 0.0225700, 0.0246211, 0.0269105, 0.0289781, 0.0310505, 0.0328032, 0.0336563, 0.0335694};
  double slice4_syst[24] = {0.0153656, 0.0149584, 0.0143041, 0.0132396, 0.0118986, 0.0102901, 0.0084255, 0.0063825, 0.0043575, 0.0028186, 0.0037872, 0.0055769, 0.0083710, 0.0112862, 0.0145205, 0.0175045, 0.020591, 0.0235691, 0.0263427, 0.029981, 0.0326192, 0.0350795, 0.0377262, 0.0409137};
  double slice5_syst[24] = {0.0240726, 0.0238486, 0.0230292, 0.021893, 0.0207896, 0.0193144, 0.0183151, 0.0168415, 0.0158686, 0.0153115, 0.0149445, 0.0156146, 0.0172919, 0.0190587, 0.0218642, 0.0247972, 0.0276917, 0.0307963, 0.0333197, 0.0358717, 0.0392671, 0.0405292, 0.0437361, 0.0437764};
  double slice6_syst[24] = {0.0488363, 0.0486393, 0.0473678, 0.0457903, 0.0441629, 0.0419467, 0.0389664, 0.0371913, 0.0350951, 0.0333258, 0.0322675, 0.031566, 0.0319891, 0.0322446, 0.0338141, 0.0354026, 0.0376102, 0.0394908, 0.042404, 0.0430432, 0.0454988, 0.0492076, 0.0525985, 0.0578158};

  TH1D* hslice1 = new TH1D("hslice1","hslice1",24,0,2.4);
  TH1D* hslice2 = new TH1D("hslice2","hslice2",24,0,2.4);
  TH1D* hslice3 = new TH1D("hslice3","hslice3",24,0,2.4);
  TH1D* hslice4 = new TH1D("hslice4","hslice4",24,0,2.4);
  TH1D* hslice5 = new TH1D("hslice5","hslice5",24,0,2.4);
  TH1D* hslice6 = new TH1D("hslice6","hslice6",12,0,2.4);

  const int nbins = 6;
  double mass[nbins+1] = {20,30,45,60,120,200,1500}; 
  TH2D* hsliceAll = new TH2D("hsliceAll","hsliceAll",nbins,mass,24,0,2.4);
  for (int i = 0; i < nbins; i++) {  
    for (int j = 0; j < 24; j++) {
      if (i==0) hsliceAll->SetBinContent(i+1,j+1,100.*slice1_syst[j]);
      if (i==1) hsliceAll->SetBinContent(i+1,j+1,100.*slice2_syst[j]);
      if (i==2) hsliceAll->SetBinContent(i+1,j+1,100.*slice3_syst[j]);
      if (i==3) hsliceAll->SetBinContent(i+1,j+1,100.*slice4_syst[j]);
      if (i==4) hsliceAll->SetBinContent(i+1,j+1,100.*slice5_syst[j]);
      if (i==5) hsliceAll->SetBinContent(i+1,j+1,100.*slice6_syst[j]);
      hsliceAll->SetBinError(i+1,j+1,0.0);
    }
  }

  for (int i = 0; i < 24; i++) {
    hslice1->SetBinError(i+1,slice1_syst[i]);
    hslice1->SetBinContent(i+1,0.0);
    hslice2->SetBinError(i+1,slice2_syst[i]);
    hslice2->SetBinContent(i+1,0.0);
    hslice3->SetBinError(i+1,slice3_syst[i]);
    hslice3->SetBinContent(i+1,0.0);
    hslice4->SetBinError(i+1,slice4_syst[i]);
    hslice4->SetBinContent(i+1,0.0);
    hslice5->SetBinError(i+1,slice5_syst[i]);
    hslice5->SetBinContent(i+1,0.0);
  }

   int index_shift = 0;
  for (int i = 0; i < 12; i++) {
    hslice6->SetBinError(i+1,(slice6_syst[i+index_shift]+slice6_syst[i+index_shift+1])/2.);
    hslice6->SetBinContent(i+1,0.0);
    index_shift++;  
  }


  TFile* pdf_syst2D = new TFile("pdf_syst2D.root","recreate");
  pdf_syst2D->cd();
  hslice1->Write();
  hslice2->Write();
  hslice3->Write();
  hslice4->Write();
  hslice5->Write();
  hslice6->Write();
  pdf_syst2D->Close(); 


//DRAW
  TCanvas* c1 = new TCanvas("2D_u","2D_u");
  c1->cd();
  c1->SetLogx();
  c1->SetRightMargin(0.11);
  hsliceAll->GetXaxis()->SetTitleOffset(1.5);
  hsliceAll->GetYaxis()->SetTitleOffset(1.3);
  hsliceAll->GetZaxis()->SetTitle("(#delta#epsilon/#epsilon)_{syst} [%]");
  hsliceAll->GetYaxis()->SetTitle("|Y(#mu#mu)|");
  hsliceAll->GetXaxis()->SetTitle("M_{#mu#mu}");
  hsliceAll->Draw("LEGO2Z");

//BACKGROUND UNCERTAINTIES
  double qcd_sys_m1[24] = {0.00555232, 0.00544458, 0.00515766, 0.00476991, 0.0052639, 0.00535482, 0.00497909, 0.00541881, 0.00526648, 0.00517364, 0.00511148, 0.0054655, 0.00488701, 0.00618889, 0.00582134, 0.0055841, 0.0064118, 0.00634195, 0.00634774, 0.00730442, 0.00673281, 0.00825394, 0.00973256, 0.0174525};
  double qcd_sys_m2[24] = {0.00345731, 0.00356044, 0.00401447, 0.00355822, 0.00350947, 0.00323543, 0.0034363, 0.00379751, 0.00348425, 0.00368308, 0.00363828, 0.00374097, 0.00413348, 0.00320857, 0.00355025, 0.00364248, 0.00353678, 0.00324369, 0.00405766, 0.00409114, 0.00517177, 0.0067538, 0.0145334, 0.019816};
  double qcd_sys_m3[24] = {0.00432952, 0.00433334, 0.00487352, 0.00435215, 0.0046475, 0.004205, 0.0045219, 0.00480428, 0.00438259, 0.00507252, 0.00410954, 0.0042161, 0.00407733, 0.00344005, 0.00351414, 0.00285376, 0.00309674, 0.00401298, 0.00365888, 0.00275306, 0.00307169, 0.0182915, 0.00828701, 0.00508648};
  double qcd_sys_m4[24] = {0.00023954, 0.000221809, 0.000203469, 0.000197556, 0.000220731, 0.000198575, 0.000181144, 0.000240572, 0.000201295, 0.000196397, 0.000253554, 0.000197084, 0.000145327, 0.000245209, 0.000433283, 0.000434458, 0.000327886, 0.00400911, 0.000237153, 0.00284565, 0.00133638, 0.000390907, 2.12736e-08, 5.00202e-08};


  TH2D* hsliceAll_bkg = new TH2D("hsliceAll_bkg","hsliceAll_bkg",nbins,mass,24,0,2.4);
  for (int i = 0; i < nbins; i++) {
    for (int j = 0; j < 24; j++) {
      if (i==0) hsliceAll_bkg->SetBinContent(i+1,j+1,100.*qcd_sys_m1[j]);
      if (i==1) hsliceAll_bkg->SetBinContent(i+1,j+1,100.*qcd_sys_m2[j]);
      if (i==2) hsliceAll_bkg->SetBinContent(i+1,j+1,100.*qcd_sys_m3[j]);
      if (i==3) hsliceAll_bkg->SetBinContent(i+1,j+1,100.*qcd_sys_m4[j]);
      if (i==4) hsliceAll_bkg->SetBinContent(i+1,j+1,0.0);
      if (i==5) hsliceAll_bkg->SetBinContent(i+1,j+1,0.0);
      hsliceAll_bkg->SetBinError(i+1,j+1,0.0);
    }
  }

  TCanvas* c2 = new TCanvas("2D_b","2D_b");
  c2->cd();
  c2->SetLogx();
  c2->SetRightMargin(0.11);
  hsliceAll_bkg->GetXaxis()->SetTitleOffset(1.5);
  hsliceAll_bkg->GetYaxis()->SetTitleOffset(1.3);
  hsliceAll_bkg->GetZaxis()->SetTitle("(#delta#epsilon/#epsilon)_{syst} [%]");
  hsliceAll_bkg->GetYaxis()->SetTitle("|Y(#mu#mu)|");
  hsliceAll_bkg->GetXaxis()->SetTitle("M_{#mu#mu}");
  hsliceAll_bkg->Draw("LEGO2Z");

}
