#include <vector>


void rootInit(void );
TFile * openFile(bool isDcache, char *file_name);
//void printTheOutput(TH2D * getThisHisto, bool printContent_orErr, string name_str,  ofstream &outputFile);
//void testMe(void);

void print1D_current(TString caseAna, TString channel, TString fsrFlag, TString accFlag, TString norm_type){

  //TString norm_type = "shape"; //"shape";//"abs","dmdm"
  //FIXME theory numbers! FEWZ
  double sigma_Z = 1137.2; //989.499999999695774;
  //double sigma_Z = 989.499999999695774; //FIXME for validation tests
  //this switches printouts on and off
  double studyMode = true; //HepDatamode

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
  //FIXME efficiency corr mat input
  //MyFileName = "effCorr_1D_case_01.11.12.root"; //effCorr_12.09.13.root";
  //MyFileName = "covMatrices_store_1D_mu.root";
  //if (channel == "el") MyFileName =  "covMatrices_store_1D_el.root";

  MyFullPath =  MyDirectory +  MySubDirectory + MyFileName;
  file_name = MyFullPath.c_str();
  TFile *f1 = openFile(isDcache, file_name);
  string f1_output = "EvtEff_Fill2621";


  char *file_name;
  cout << "Reading " << "covariance_finalResults_1D_"+TString(channel)+"_"+TString(fsrFlag)+"FSR_"+TString(accFlag)+"_normalized.root" << endl;
  //FIXME input
  //MyFileName = "20137TeVPaper/covariance_finalResults_1D_"+TString(channel)+"_"+TString(fsrFlag)+"FSR_"+TString(accFlag)+"_normalized.root";
  MyFileName = "20148TeVPaper/covariance_finalResults_1D_mu_preFSR_fullAcc_unnormalized.root";
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

  //string  histo_1 = "effCorrelations";
  //string  histo_2 = "effCorrelations_bins";
  //string  histo_3 = "effCorrelations_NORM";
  //string  histo_4 = "effCorrelations_bins_NORM";
  //string  histo_5 = "effCorrelations_2D_bins";
  //string  histo_6 = "effCorrelations_2D_bins_NORM";

  //FIXME this is for absolute cross section now
  string  str_tm_4 = "totalCov_TM";

  //TH2D * h2_p1 = (TH2D*)(DataFiles[0])->Get(histo_1.c_str());
  //TH2D * h2_p2 = (TH2D*)(DataFiles[0])->Get(histo_2.c_str());  
  //TH2D * h2_p3 = (TH2D*)(DataFiles[0])->Get(histo_3.c_str());  
  //TH2D * h2_p4 = (TH2D*)(DataFiles[0])->Get(histo_4.c_str());  
  //TH2D * h2_p5 = (TH2D*)(DataFiles[0])->Get(histo_5.c_str());  
  //TH2D * h2_p6 = (TH2D*)(DataFiles[0])->Get(histo_6.c_str()); 

  std::string nameHist = "Efficiency correlations between mass bins - 1D; mass (GeV); mass (GeV)";
  std::string binStr = "mass bin; mass bin";
  std::string binStr_single = "mass bin";
  if("2D" == caseAna){
     binStr = "mass-rapidity bin; mass-rapidity bin";
     binStr_single = "mass-rapidity bin";
  }

  //FIXME insert an array with mass binning
  const int nbin = 41;
  double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

  if (!studyMode) {
    FILE * pFile;
    pFile = fopen ("new_final/cov_"+TString(channel)+"_"+fsrFlag+"FSR_"+accFlag+"_"+norm_type+".out","w");
  }
    TString accFlag1 = accFlag; 
    if (accFlag == "fullAcc") accFlag1= "fullSpace";
  if (!studyMode) {
    fprintf(pFile, "Covariances of the Drell-Yan differential cross section in the muon channel as a function of the dilepton mass\n");
    if (norm_type == "rshape") {
     string thiss =  "        # "+string(fsrFlag)+"FSR "+string(accFlag1)+" r-shape\n";
     fprintf(pFile,"%s", thiss.c_str());
    } else if (norm_type == "absolute") {
     string thiss =  "        # "+string(fsrFlag)+"FSR "+string(accFlag1)+" absolute\n";
     fprintf(pFile,"%s", thiss.c_str());
    } else {
     string thiss =  "        # "+string(fsrFlag)+"FSR "+string(accFlag1)+" (sigma/sigma_Z)\n";
     fprintf(pFile,"%s", thiss.c_str());
    }
    fprintf(pFile, "M : 15 - 1500 GeV\n");
    if (accFlag != "fullAcc") {
    fprintf(pFile, "ABS(ETA(MU)) <= 2.4\n");
    }
    fprintf(pFile, "PT(MU1) : >=9 GEV\n");
    fprintf(pFile, "PT(MU2) : >=14 GEV\n");
    fprintf(pFile, "RE : P P --> DYmumu X\n");
    fprintf(pFile, "SQRT(S) : 7000.0 GEV\n");
    fprintf(pFile, "x: M in GEV\n");
    fprintf(pFile, "y: M in GEV\n");
    if (norm_type == "rshape") {  
     fprintf(pFile, "z: cov(r_x,r_y) [GeV^{-2}]\n");
    } else if (norm_type == "absolute") {
     fprintf(pFile, "z: cov(sigma_x,sigma_y) [pb^2]\n");
    } else {
     fprintf(pFile, "z: cov(sigma_x/sigmaZ,sigma_y/sigmaZ)\n");
    }
    fprintf(pFile, "   xlow-xhigh\n");
  }
  //fprintf(pFile, "       15-20 20-25 25-30 30-35 35-40 40-45 45-50 50-55 55-60 60-64 64-68 68-72 72-76 76-81 81-86 86-91 91-96 96-101 101-106 106-110 110-115 115-120 120-126 126-133 133-141 141-150 150-160 160-171 171-185 185-200 200-220 220-243 243-273 273-320 320-380 380-440 440-510 510-600 600-1000 1000-1500\n");

  //FIXME below all values are for absolute cross section
  //FIXME no lumi!
  //errors
  double diagonals_preFSR_fullAcc[41] = {8.80412, 3.29234, 1.46575, 0.751465, 0.413033, 0.240538, 0.151241, 0.101233, 0.073219, 0.061808, 0.054303, 0.0531846, 0.0590174, 0.0841852, 0.222185, 2.37495, 3.11411, 0.255464, 0.0756085, 0.0358074, 0.0218415, 0.0155575, 0.0111998, 0.00836715, 0.00638793, 0.00478181, 0.00374254, 0.00277333, 0.00217444, 0.00155305, 0.00110093, 0.000791508, 0.000489534, 0.000278817, 0.000145276, 9.0259e-05, 4.89139e-05, 2.59619e-05, 5.33459e-06, 1.00711e-06, 1.51107e-07};

  //values
  double rdiagonals_preFSR_fullAcc[41] = {185.415, 74.4594, 35.5085, 19.5878, 11.714, 7.41756, 5.00932, 3.60601, 2.75472, 2.33912, 2.1616, 2.22005, 2.6031, 3.86065, 9.35679, 86.6177, 104.974, 8.95049, 2.98679, 1.52349, 0.
953199, 0.645626, 0.439095, 0.310173, 0.216115, 0.148822, 0.107822, 0.0708904, 0.0541272, 0.0343885, 0.023559, 0.0154579, 0.00898076, 0.00488887, 0.00204174, 0.00100656, 0.0004496
51, 0.000207937, 4.925e-05, 3.35757e-06, 2.93322e-07};

  //FIXME this you would need yo go from normalized to absolute (or back)
  double sigmaZerrNoLumi = 0.;
  if (fsrFlag == "pre" && accFlag == "fullAcc")  sigmaZerrNoLumi = pow(0.4,2) + pow(11.2,2) + pow(25.1,2);

  TMatrixT <double> *tm_4  = (TMatrixT <double> *)(DataFiles[1])->Get(str_tm_4.c_str());;  
  TMatrixT <double> *M(tm_4);
  TH2D *h2_tm_4 = new TH2D (*tm_4);
  for (int ix = h2_tm_4->GetNbinsX()-1; ix > -1; ix--) {
     if (!studyMode) fprintf(pFile, "%2.0lf-%2.0lf ", mass_xbin[ix],mass_xbin[ix+1]);
     for (int iy = 0; iy < h2_tm_4->GetNbinsY(); iy++) {
        //set the matrix element to the original
        double val = h2_tm_4->GetBinContent(ix+1,iy+1);
        //calculate correlation coeff
        double corr_ij = h2_tm_4->GetBinContent(ix+1,iy+1)/sqrt(h2_tm_4->GetBinContent(ix+1,ix+1)*h2_tm_4->GetBinContent(iy+1,iy+1));
        //caclulate modified matrix element based on diagonals 
        double prod = 0; 
        //FIXME diagonal should match. Forcing to match
        if (fsrFlag == "pre" && accFlag == "fullAcc") prod = diagonals_preFSR_fullAcc[ix]*diagonals_preFSR_fullAcc[iy]; 
        //rescale with correlation       
        if (ix!=iy) val = prod*corr_ij;
        else val = prod;
        //FIXME multiply by bin width for normalized only
        if (norm_type == "normalized") { // || norm_type == "absolute") {
             val *= (mass_xbin[iy+1]-mass_xbin[iy])*(mass_xbin[ix+1]-mass_xbin[ix]);
        }
        //FIXME
        M(ix,iy) = val;
        if (!studyMode) fprintf(pFile, "   % 6.2e", val);
      }
     if (!studyMode) fprintf(pFile, "\n");
  }  
  if (!studyMode) fclose (pFile);


  //FIXME debug 
  TFile * fFile = new TFile("OUTcovariance_1D_"+TString(channel)+"_"+fsrFlag+"_"+accFlag+"_"+norm_type+".root","RECREATE");
  M->Write("totalCov_TM");
  if (studyMode) { 
    for(int i=0;i<M->GetNrows();++i){
     for(int j=0;j<M->GetNcols();++j){
      cout << i+1 << " " << j+1 << " " << M(i,j) << endl;
     }
    }
  }
  fFile->Close();

//if (!studyMode) {
   cout << "Validation " << fsrFlag << " " << accFlag << " " << norm_type << endl;
   for (int i = 0; i < 41; i++) {
    for (int j = 0; j < 41; j++) {
     if (fabs(M(i,j)-M(j,i)) > fabs(M(i,j))/10000.) cout << "Asymmetry warning " << i << " " << j << " " << M(i,j) << " " <<  M(j,i) << endl;
    }
   }

  for (int i = 0; i < 41; i++) {
    for (int j = 0; j < 41; j++) {
     if (i!=j) continue;
     double val = 0;
     if (fsrFlag == "pre" && accFlag == "fullAcc") val =diagonals_preFSR_fullAcc[j];
     //FIXME it used to be like that for 7 TeV, but for 8 TeV we changed the input covariance to be for absolute already
     //if (norm_type == "absolute") {
     //     if (fsrFlag == "pre" && accFlag == "fullAcc") val = sqrt(sigma_Z*sigma_Z*diagonals_preFSR_fullAcc[i]*diagonals_preFSR_fullAcc[j] + rdiagonals_preFSR_fullAcc[i]*rdiagonals_preFSR_fullAcc[j]*sigmaZerrNoLumi);
     //}
     if (norm_type == "normalized") { // || norm_type == "absolute") {
             val *= sqrt((mass_xbin[i+1]-mass_xbin[i])*(mass_xbin[j+1]-mass_xbin[j]));
     }
     if (fabs(sqrt(M(j,j)) - val) > fabs(sqrt(M(j,j)))/10000.) cout << "Warning " << sqrt(M(j,j)) << " " << val << " " << fabs(sqrt(M(j,j)) - val) << endl;
    }
   }

   cout << "Check positive definite" << endl;
   const int dim = 41;
   TMatrixDSym MSym(dim); 
   for (int ir=0; ir<dim; ++ir) {
    for (int ic=0; ic<dim; ++ic) {
      MSym(ir,ic) = M(ir,ic);
    }
   }
   //MSym.Print();
  
   TMatrixDSymEigen ME(MSym);
   TVectorD eig= ME.GetEigenValues();
   TMatrixD eigVec= ME.GetEigenVectors();  
   eig.Print();
//}//end validation

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
