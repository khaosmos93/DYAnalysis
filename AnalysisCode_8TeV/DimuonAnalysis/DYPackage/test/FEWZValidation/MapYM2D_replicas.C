void MapYM2D_replicas(void) {

//adjust binning
   const int nbinsY = 24;
   const int nreplicas = 100;

   const int nfiles = 6;
   TString filenames[nfiles] = {"outmp20to30","outmp30to45","outmp45to60","outmp60to120","outmp120to200","outmp200to1500"};

   for (int ifile = 0; ifile < nfiles; ifile++) {
       TFile* f = new TFile("replicas_"+filenames[ifile]+".root","RECREATE");
       f->cd();
 
       cout << "Create file " << filenames[ifile]+".root" << endl;
       for (int ireplica = 0; ireplica < nreplicas; ireplica++)  {

          std::ostringstream pprint;
          pprint.str("");
          pprint<<ireplica;
          string rep = pprint.str();

          ifstream in_xsec("./"+filenames[ifile]+"_rep_"+TString(rep)+".dat",ios::in);

          cout << "Open data file " << "./"+filenames[ifile]+"_rep_"+TString(rep)+".dat" << endl; 

          double xsec[nbinsY];
          double xsec_err[nbinsY];
          //initialize    
          for( int j = 0; j < nbinsY; j++ ) {
     	    xsec[j] = xsec_err[j] = 0.;
          }

         TH1D* hxsec = new TH1D("xsec_rep_"+TString(rep),"xsec_rep_"+TString(rep),nbinsY,0,2.4);

         //fill from file
         for( int j = 0; j < nbinsY; j++ ) {

            in_xsec >> xsec[j] >>  xsec_err[j] ;
            //check ok
            cout << "Fill: " << xsec[j] << " " << xsec_err[j] << endl; 

            //fill histo
            hxsec->SetBinContent(j+1,xsec[j]); 
            hxsec->SetBinError(j+1,xsec_err[j]);
            //cout << "his content " << hnum1->GetBinContent(i+1) << " bin error: " << hnum1->GetBinError(i+1) << " bin center " << hnum1->GetXaxis()->GetBinCenter(i+1) << endl;
         }

         //SMOOTHING
    for (int i =1; i < 23; i++) {
        simpleSmoother(hxsec,i+1);
    }
    for (int i =1; i < 23; i++) {
        simpleSmoother(hxsec,i+1);
    }
    for (int i =1; i < 23; i++) {
        simpleSmoother(hxsec,i+1);
    }
    fr (int i =1; i < 23; i++) {
        simpleSmoother(hxsec,i+1);
    }
    for (int i =1; i < 23; i++) {
        simpleSmoother(hxsec,i+1);
    }
      
         hxsec->Write();
        }//nreplicas loop
        f->Close();
    } //nfiles loop
}

void simpleSmoother(TH1D* h, int ibin) {
      
      h->SetBinContent(ibin,(h->GetBinContent(ibin-1)+h->GetBinContent(ibin+1))/2.);
      
} 
