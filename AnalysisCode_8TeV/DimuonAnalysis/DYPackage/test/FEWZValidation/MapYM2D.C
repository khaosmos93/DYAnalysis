void MapYM2D(void) {

   const int nbinsY = 24;
   const int nfiles = 6;
   TString filenames[nfiles] = {"outmptmp20to30.dat","outmptmp30to45.dat","outmptmp45to60.dat","outmptmp60to120.dat","outmptmp120to200.dat","outmptmp200to1500.dat"};

   //2D sliced
   // 20-30: 24 bins
   // 30-45: 24 bins
   // 45-60: 24 bins
   // 60-120: 24 bins
   // 120-200: 24 bins
   // 200-1500: 12 bins
   // total 132 bins
   const int nbin2 = 132;
   double mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};

   TH1D* fAbsRap = new TH1D("rap_FEWZ", "rap_FEWZ", nbin2, 0, 132);

   int shift  = 0;
   for (int ifile = 0; ifile < nfiles; ifile++) {
    ifstream in_xsec("./"+filenames[ifile],ios::in);

    double xsec[nbinsY];
    double xsec_err[nbinsY];
    //initialize    
     for( int j = 0; j < nbinsY; j++ ) {
	xsec[j] = xsec_err[j] = 0.;
      }

    TH1D* hxsec = new TH1D("xsec","xsec",nbinsY,0,2.4);

    //fill from file
    for( int j = 0; j < nbinsY; j++ ) {
        in_xsec >> xsec[j] >>  xsec_err[j] ;

        //fill histo
        hxsec->SetBinContent(j+1,xsec[j]); 
        hxsec->SetBinError(j+1,xsec_err[j]);

   
        fAbsRap->SetBinContent(j+1+shift,xsec[j]);
        fAbsRap->SetBinError(j+1+shift,xsec_err[j]);
    }
    if (ifile != nfiles-1) { shift += 24;
    } else {
      shift += 12;
    }  

    TFile* f = new TFile(filenames[ifile]+"_NNLO.root","RECREATE");
    f->cd();
    hxsec->Write();
    f->Close();
  } //nfiles loop

  TFile* g = new TFile("combined.root","RECREATE");
  g->cd();
  fAbsRap->Write();
  g->Close();

}
