#include <vector>


void rootInit(void );
TFile * openFile(bool isDcache, char *file_name);
//void printTheOutput(TH2D * getThisHisto, bool printContent_orErr, string name_str,  ofstream &outputFile);
//void testMe(void);

void print2D_current(TString caseAna, TString channel, TString fsrFlag, TString accFlag, TString norm_type){

  //TString norm_type = "shape"; //"shape";//"abs","dmdm"
  //FIXME theory numbers! FEWZ
  double sigma_Z = 570.5;
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
  cout << "Reading " << "covariance_finalResults_2D_"+TString(channel)+"_"+TString(fsrFlag)+"FSR_"+TString(accFlag)+"_normalized.root" << endl;
  //FIXME input
  //MyFileName = "20137TeVPaper/covariance_finalResults_1D_"+TString(channel)+"_"+TString(fsrFlag)+"FSR_"+TString(accFlag)+"_normalized.root";
  MyFileName = "20148TeVPaper/covariance_finalResults_2D_mu_preFSR_inAcc_unnormalized.root";
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
  //const int nbin = 41;
  //double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
  //          96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
  //          200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};

  if (!studyMode) {
    FILE * pFile;
    pFile = fopen ("results/cov_"+TString(channel)+"_"+fsrFlag+"FSR_"+accFlag+"_"+norm_type+".out","w");
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
  double diagonals_preFSR_fullAcc[132] = {0.103165, 0.111709, 0.114268, 0.109006, 0.115496, 0.109547, 0.119788, 0.121407, 0.100394, 0.115624, 0.112131, 0.109158, 0.095902, 0.109587, 0.112382, 0.124734, 0.116801, 0.104963, 0.103837, 0.0950279, 0.134272, 0.10116, 0.0829061, 0.0335066, 0.203535, 0.195448, 0.206683, 0.222815, 0.221615, 0.202785, 0.202641, 0.213531, 0.195455, 0.197509, 0.197003, 0.181909, 0.184953, 0.174214, 0.173537, 0.177475, 0.181342, 0.170374, 0.178467, 0.159278, 0.159825, 0.158789, 0.135148, 0.0849642, 0.192479, 0.20443, 0.23476, 0.220033, 0.351283, 0.159534, 0.177235, 0.225877, 0.160891, 0.163037, 0.176801, 0.235607, 0.191899, 0.160942, 0.157267, 0.240316, 0.171125, 0.155988, 0.223125, 0.231156, 0.232559, 0.148853, 0.179379, 0.0923561, 2.52945, 2.38246, 2.22768, 2.07267, 1.91234, 1.7503, 1.60484, 1.47998, 1.37616, 1.41637, 1.28104, 1.24305, 1.26802, 1.29113, 1.30296, 1.29402, 1.36586, 1.26662, 1.15713, 0.942885, 0.894076, 0.682108, 0.468569, 0.283223, 0.0757472, 0.0855444, 0.0875303, 0.0854155, 0.0818136, 0.0797266, 0.0787246, 0.076746, 0.0728788, 0.0713692, 0.0696939, 0.0679149, 0.0649319, 0.0628104, 0.0599067, 0.0546335, 0.0529948, 0.0503427, 0.0451434, 0.0416635, 0.0315926, 0.0270185, 0.0204349, 0.0114651, 0.0317153, 0.0329654, 0.0291295, 0.0252041, 0.0218815, 0.0206282, 0.0172706, 0.0156289, 0.0141186, 0.0108849, 0.00789579, 0.00461221};

  //values
  double rdiagonals_preFSR_fullAcc[132] = {3.55373, 3.5594, 3.45245, 3.48084, 3.68256, 3.53281, 3.56681, 3.5891, 3.43185, 3.54109, 3.54885, 3.49892, 3.26409, 3.43919, 3.43361, 3.19662, 3.21297, 2.99306, 3.01269, 2.55518, 2.11612, 1.66128, 1.02513, 0.372608, 11.9813, 11.5141, 11.4116, 11.6332, 11.7458, 11.4972, 11.5833, 11.6189, 11.5481, 11.4628, 11.5819, 11.3866, 11.1839, 11.0457, 10.653, 10.5892, 10.0504, 9.52408, 8.95429, 7.87824, 6.45235, 4.87846, 3.08395, 1.09249, 10.2983, 10.2246, 10.1027, 9.90815, 10.195, 9.91584, 9.8503, 9.96064, 10.0249, 9.77496, 9.58784, 9.62868, 9.46939, 9.19605, 8.83427, 8.51204, 7.8739, 7.02286, 6.2486, 5.03853, 4.04577, 2.82609, 1.70848, 0.574816, 339.121, 337.69, 336.26, 335.243, 334.578, 331.795, 328.89, 325.864, 320.297, 313.54, 300.684, 287.802, 273.007, 255.628, 237.552, 215.695, 193.267, 167.52, 142.453, 116.211, 89.3665, 62.692, 36.4617, 11.8243, 3.77637, 3.9034, 3.72668, 3.81243, 3.74452, 3.78965, 3.6723, 3.5956, 3.36242, 3.31542, 3.19853, 3.08707, 2.91441, 2.84043, 2.43395, 2.29197, 2.06818, 1.70859, 1.46995, 1.26884, 0.891042, 0.62485, 0.348128, 0.117425, 0.618935, 0.690771, 0.652939, 0.612284, 0.556868, 0.531523, 0.433762, 0.370245, 0.296559, 0.183375, 0.101512, 0.0324654};

  //FIXME this you would need yo go from normalized to absolute (or back)
  //double sigmaZerrNoLumi = 0.;
  //if (fsrFlag == "pre" && accFlag == "fullAcc")  sigmaZerrNoLumi = pow(0.4,2) + pow(11.2,2) + pow(25.1,2);

  TMatrixT <double> *tm_4  = (TMatrixT <double> *)(DataFiles[1])->Get(str_tm_4.c_str());;  
  TMatrixT <double> *M(tm_4);
  TH2D *h2_tm_4 = new TH2D (*tm_4);
  for (int ix = h2_tm_4->GetNbinsX()-1; ix > -1; ix--) {
     //if (!studyMode) fprintf(pFile, "%2.0lf-%2.0lf ", mass_xbin[ix],mass_xbin[ix+1]);
     for (int iy = 0; iy < h2_tm_4->GetNbinsY(); iy++) {
        //set the matrix element to the original
        double val = h2_tm_4->GetBinContent(ix+1,iy+1);
        //calculate correlation coeff
        double corr_ij = h2_tm_4->GetBinContent(ix+1,iy+1)/sqrt(h2_tm_4->GetBinContent(ix+1,ix+1)*h2_tm_4->GetBinContent(iy+1,iy+1));
        //caclulate modified matrix element based on diagonals 
        double prod = 0; 
        //FIXME diagonal should match. Forcing to match
        if (fsrFlag == "pre" && accFlag == "inAcc") prod = diagonals_preFSR_fullAcc[ix]*diagonals_preFSR_fullAcc[iy]; 
        //rescale with correlation       
        if (ix!=iy) val = prod*corr_ij;
        else val = prod;
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
/*
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
*/
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
