#include <vector>


void rootInit(void );
TFile * openFile(bool isDcache, char *file_name);
//void printTheOutput(TH2D * getThisHisto, bool printContent_orErr, string name_str,  ofstream &outputFile);
//void testMe(void);

void print1D_current_7TeVEleAlter(TString caseAna, TString channel, TString fsrFlag, TString accFlag, TString norm_type){

  //TString norm_type = "shape"; //"shape";//"abs","dmdm"
  double sigma_Z = 989.499999999695774;

  if (fsrFlag == "post" && accFlag == "fullAcc") sigma_Z = 974.8;
  if (fsrFlag == "pre" && accFlag == "inAcc")    sigma_Z = 524.7;
  if (fsrFlag == "post" && accFlag == "inAcc")   sigma_Z = 516.5;

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
  cout << "Reading " << "covariance_finalResults_1D_mu_"+TString(fsrFlag)+"FSR_"+TString(accFlag)+"_normalized.root" << endl;
  MyFileName = "covariance_finalResults_1D_el_preFSR_fullAcc_normalized_7TeV.root";
  MyFullPath =  MyDirectory +  MySubDirectory + MyFileName;
  file_name = MyFullPath.c_str();
  TFile *f2 = openFile(isDcache, file_name);
  string f2_output = "EvtEff_Fill2621";


  // All files in a vector
  std::vector< TFile * > DataFiles;
  DataFiles.push_back(f2);

  std::vector< string > DataFiles_names;
  DataFiles_names.push_back(f2_output);

  string  histo_1 = "effCorrelations";
  string  histo_2 = "effCorrelations_bins";
  string  histo_3 = "effCorrelations_NORM";
  string  histo_4 = "effCorrelations_bins_NORM";
  string  histo_5 = "effCorrelations_2D_bins";
  string  histo_6 = "effCorrelations_2D_bins_NORM";

  string  str_tm_1 = "unfoldCov_TM";
  string  str_tm_2 = "effCov_TM";
  string  str_tm_3 = "fsrCov_TM";
  string  str_tm_4 = "totalCov_TM";

  std::string nameHist = "Efficiency correlations between mass bins - 1D; mass (GeV); mass (GeV)";
  std::string binStr = "mass bin; mass bin";
  std::string binStr_single = "mass bin";
  if("2D" == caseAna){
     binStr = "mass-rapidity bin; mass-rapidity bin";
     binStr_single = "mass-rapidity bin";
  }

  //FIXME insert an array with mass binning
  const int nbin = 40;
  double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

  FILE * pFile;
  pFile = fopen ("new_final/cov_mu_"+fsrFlag+"FSR_"+accFlag+"_"+norm_type+".out","w");
  TString accFlag1 = accFlag; 
  if (accFlag == "fullAcc") accFlag1= "fullSpace";
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
  //fprintf(pFile, "       15-20 20-25 25-30 30-35 35-40 40-45 45-50 50-55 55-60 60-64 64-68 68-72 72-76 76-81 81-86 86-91 91-96 96-101 101-106 106-110 110-115 115-120 120-126 126-133 133-141 141-150 150-160 160-171 171-185 185-200 200-220 220-243 243-273 273-320 320-380 380-440 440-510 510-600 600-1000 1000-1500\n");

  //FIXME change this
  double diagonals_preFSR_fullAcc[40] = {0.0170807, 0.00421696, 0.00200767, 0.00145369, 0.000906991, 0.000689151, 0.000625353, 0.000325452, 0.000273191, 0.000467838, 0.000422541, 0.000492187, 0.000585519, 0.000704213, 0.000617396, 0.000442354, 0.000320368, 0.000654366, 0.000641616, 0.000268314, 0.000137334, 8.1531e-05, 4.61389e-05, 2.7452e-05, 2.09809e-05, 1.27176e-05, 9.5525e-06, 9.36637e-06, 4.86488e-06, 3.54608e-06, 2.11571e-06, 1.44662e-06, 8.83622e-07, 4.82429e-07, 2.43374e-07, 1.60911e-07, 9.60952e-08, 5.14436e-08, 1.00902e-08, 2.29561e-09};
  //FIXME and change this
  double rdiagonals_preFSR_fullAcc[40] = {0.153627, 0.0594784, 0.0340423, 0.0197036, 0.0115329, 0.00689448, 0.00475059, 0.00344261, 0.00246686, 0.00236348, 0.00220558, 0.00225351, 0.00252033, 0.00365256, 0.0106616, 0.0647833, 0.100841, 0.00693679, 0.00310761, 0.00127635, 0.000950094, 0.00057128, 0.00038017, 0.000260748, 0.000209709, 0.000125134, 8.30669e-05, 8.26584e-05, 3.88195e-05, 3.13807e-05, 2.24676e-05, 1.29005e-05, 7.35752e-06, 4.08249e-06, 1.77498e-06, 7.24544e-07, 3.8208e-07, 2.25861e-07, 4.14817e-08, 1.26196e-09};

  double diagonals_postFSR_fullAcc[40] = {0.0162243105846 ,  0.00336593514476 ,  0.00157637391767 ,  0.000645773242289 ,  0.000346757852179 ,  0.000209808296218 ,  0.00014900552904 ,  0.000113672302458 ,  9.37365103812e-05 ,  8.8235802415e-05 ,  9.17548573757e-05 ,  0.000102840758859 ,  0.0001261839453 ,  0.000177386599657 ,  0.000398835369078 ,  0.00255275688479 ,  0.00275533977836 ,  0.000241967309243 ,  7.42965050033e-05 ,  4.00993291723e-05 ,  2.45164522233e-05 ,  1.81713581323e-05 ,  1.20061631754e-05 ,  8.99094051388e-06 ,  6.42323231793e-06 ,  5.12594949872e-06 ,  4.00687234374e-06 ,  3.05293519634e-06 ,  2.10552482062e-06 ,  1.67916617425e-06 ,  1.09074693498e-06 ,  8.08139109429e-07 ,  5.12621029616e-07 ,  2.61309165458e-07 ,  1.44998114388e-07 ,  9.22513151045e-08 ,  6.15645532985e-08 ,  3.53875260529e-08 ,  6.37228926447e-09 ,  1.42285737414e-09};
  double rdiagonals_postFSR_fullAcc[40] = {0.159392853575 ,  0.0665969458706 ,  0.0314870063666 ,  0.0177013466172 ,  0.0108102132239 ,  0.00708788182474 ,  0.00505096465292 ,  0.00395903754428 ,  0.0032798137694 ,  0.00308954726685 ,  0.00316018553752 ,  0.00360163058894 ,  0.00446998731724 ,  0.00641301651881 ,  0.0134377991737 ,  0.0783508139407 ,  0.0785678978129 ,  0.00700111051395 ,  0.00237533189675 ,  0.0012920866628 ,  0.000797791708532 ,  0.000565488535882 ,  0.0003568546219 ,  0.000265435860573 ,  0.000174201621627 ,  0.000130534348978 ,  9.17368050452e-05 ,  6.32449380353e-05 ,  4.13271897565e-05 ,  3.12300666589e-05 ,  2.05530777599e-05 ,  1.29458593966e-05 ,  7.8782421867e-06 ,  3.84871688004e-06 ,  1.69977116029e-06 ,  7.74773684019e-07 ,  3.32769903789e-07 ,  1.999664689e-07 ,  3.28002612238e-08 ,  2.87543794311e-09};

  double diagonals_preFSR_inAcc[40] = {7.67689e-05, 0.000112916, 0.000347217, 0.000219383, 0.000122413, 7.39715e-05, 5.44625e-05, 4.09087e-05, 3.73977e-05, 3.51426e-05, 3.48707e-05, 3.93799e-05, 4.67808e-05, 6.38676e-05, 0.00017137, 0.00188843, 0.00258849, 0.000219414, 6.38757e-05, 3.49398e-05, 2.15315e-05, 1.69656e-05, 1.17225e-05, 9.09188e-06, 6.72669e-06, 5.53006e-06, 4.43396e-06, 3.52961e-06, 2.55343e-06, 2.12106e-06, 1.35221e-06, 9.6696e-07, 6.7099e-07, 3.6243e-07, 2.12615e-07, 1.50711e-07, 9.07646e-08, 5.80094e-08, 9.91054e-09, 2.86182e-09};
  double rdiagonals_preFSR_inAcc[40] = {0.00325167, 0.00448242, 0.00921863, 0.00870422, 0.00635869, 0.00452442, 0.00330342, 0.00246004, 0.00205493, 0.00173921, 0.00159824, 0.00180502, 0.00217259, 0.00315283, 0.00803583, 0.0762232, 0.0932965, 0.00804937, 0.00269356, 0.00145212, 0.000895317, 0.000639606, 0.000403091, 0.000305934, 0.000200642, 0.000152962, 0.000107449, 7.55664e-05, 5.08876e-05, 3.91673e-05, 2.70351e-05, 1.64207e-05, 1.07974e-05, 5.53093e-06, 2.55414e-06, 1.2795e-06, 4.92819e-07, 3.30098e-07, 5.13995e-08, 5.79458e-09};

  double diagonals_postFSR_inAcc[40] = {6.969073405e-05 ,  0.000108995816741 ,  0.00034178269674 ,  0.000213502681129 ,  0.000115034486042 ,  7.51568886458e-05 ,  5.78850003993e-05 ,  4.7032569967e-05 ,  4.5429201226e-05 ,  4.84019893501e-05 ,  5.48665129122e-05 ,  6.26451326603e-05 ,  7.811105417e-05 ,  0.000109280019944 ,  0.000267763909185 ,  0.00193672614214 ,  0.00219816040679 ,  0.000194832619493 ,  5.80749002395e-05 ,  3.21873936752e-05 ,  2.019527172e-05 ,  1.57775515527e-05 ,  1.11096546305e-05 ,  8.49676302794e-06 ,  6.3977105681e-06 ,  5.26190807866e-06 ,  4.22185610939e-06 ,  3.38604620872e-06 ,  2.43254009469e-06 ,  2.02818986224e-06 ,  1.24825226009e-06 ,  9.45665930088e-07 ,  6.3544987495e-07 ,  3.39461850083e-07 ,  2.01271146555e-07 ,  1.34872608535e-07 ,  9.1860695783e-08 ,  5.48863692185e-08 ,  1.0304881688e-08 ,  2.44572310853e-09};
  double rdiagonals_postFSR_inAcc[40] = {0.00303224339034 ,  0.00440413988655 ,  0.00912596854359 ,  0.00861561584263 ,  0.00636088561858 ,  0.0046763214196 ,  0.00364315675465 ,  0.00301781097697 ,  0.00275499085208 ,  0.00270607139173 ,  0.00287708193314 ,  0.00335675125341 ,  0.00426162394463 ,  0.00622330875529 ,  0.0132696570058 ,  0.0785115109917 ,  0.0793020827579 ,  0.00715842930286 ,  0.00245684778355 ,  0.00134335910416 ,  0.000844329651121 ,  0.000597923650072 ,  0.000383930567535 ,  0.000287376645816 ,  0.000191704935006 ,  0.000146231781721 ,  0.000102769178233 ,  7.27468201559e-05 ,  4.86410290893e-05 ,  3.76195228802e-05 ,  2.50450032288e-05 ,  1.61040810116e-05 ,  1.02536189902e-05 ,  5.19556601308e-06 ,  2.42355302009e-06 ,  1.14637367256e-06 ,  4.99009497385e-07 ,  3.12490686455e-07 ,  5.34680797461e-08 ,  4.95240287352e-09};


  double sigmaZerrNoLumi = 0.;
  if (fsrFlag == "pre" && accFlag == "fullAcc")  sigmaZerrNoLumi = pow(0.8,2) + pow(9.8,2) + pow(21.9,2);
  if (fsrFlag == "post" && accFlag == "fullAcc")    sigmaZerrNoLumi = pow(0.7,2) +pow(9.2,2) +pow(21.6,2);
  if (fsrFlag == "pre" && accFlag == "inAcc") sigmaZerrNoLumi = pow(0.4,2) +pow(5.1,2) +pow(1.2,2);
  if (fsrFlag == "post" && accFlag == "inAcc")   sigmaZerrNoLumi =pow(0.4,2) +pow(4.9,2) +pow(1.1,2); 


  TMatrixT <double> *tm_4  = (TMatrixT <double> *)(DataFiles[0])->Get(str_tm_4.c_str());;  
  TMatrixT <double> *M(tm_4);
  TH2D *h2_tm_4 = new TH2D (*tm_4);
  for (int ix = h2_tm_4->GetNbinsX()-1; ix > -1; ix--) {
     fprintf(pFile, "%2.0lf-%2.0lf ", mass_xbin[ix],mass_xbin[ix+1]);
     for (int iy = 0; iy < h2_tm_4->GetNbinsY(); iy++) {
        //set the matrix element to the original
        double val = h2_tm_4->GetBinContent(ix+1,iy+1);
        //calculate correlation coeff
        double corr_ij = h2_tm_4->GetBinContent(ix+1,iy+1)/sqrt(h2_tm_4->GetBinContent(ix+1,ix+1)*h2_tm_4->GetBinContent(iy+1,iy+1));
        //caclulate modified matrix element based on diagonals 
        double prod = 0; 
        if (fsrFlag == "pre" && accFlag == "fullAcc") prod = diagonals_preFSR_fullAcc[ix]*diagonals_preFSR_fullAcc[iy]; 
        if (fsrFlag == "post" && accFlag == "fullAcc") prod = diagonals_postFSR_fullAcc[ix]*diagonals_postFSR_fullAcc[iy];
        if (fsrFlag == "pre" && accFlag == "inAcc")    prod = diagonals_preFSR_inAcc[ix]*diagonals_preFSR_inAcc[iy];
        if (fsrFlag == "post" && accFlag == "inAcc")   prod = diagonals_postFSR_inAcc[ix]*diagonals_postFSR_inAcc[iy];
        //rescale with correlation       
        if (ix!=iy) val = prod*corr_ij;
        else val = prod;
        //covariance updated 
        if (norm_type == "absolute") {
           if (fsrFlag == "pre" && accFlag == "fullAcc")   val = sigma_Z*sigma_Z*val + rdiagonals_preFSR_fullAcc[ix]*rdiagonals_preFSR_fullAcc[iy]*sigmaZerrNoLumi; 
           if (fsrFlag == "post" && accFlag == "fullAcc")  val = sigma_Z*sigma_Z*val + rdiagonals_postFSR_fullAcc[ix]*rdiagonals_postFSR_fullAcc[iy]*sigmaZerrNoLumi; 
           if (fsrFlag == "pre" && accFlag == "inAcc")     val = sigma_Z*sigma_Z*val + rdiagonals_preFSR_inAcc[ix]*rdiagonals_preFSR_inAcc[iy]*sigmaZerrNoLumi; 
           if (fsrFlag == "post" && accFlag == "inAcc")    val = sigma_Z*sigma_Z*val + rdiagonals_postFSR_inAcc[ix]*rdiagonals_postFSR_inAcc[iy]*sigmaZerrNoLumi; 
        } 
          //FIXME adjust the value based on the normalization type
          if (norm_type == "normalized" || norm_type == "absolute") {
             val *= (mass_xbin[iy+1]-mass_xbin[iy])*(mass_xbin[ix+1]-mass_xbin[ix]);
          }
        //FIXME
        //if (fabs(val/sqrt(h2_tm_4->GetBinContent(iy+1,iy+1)*h2_tm_4->GetBinContent(ix+1,ix+1)))<0.01) val = 0;
        //FIXME
        M(ix,iy) = val;
        fprintf(pFile, "   % 6.2e", val);
      }
     fprintf(pFile, "\n");
  }  
  fclose (pFile);

  //FIXME debug 
  TFile * fFile = new TFile("OUTcovariance_1D_mu_"+fsrFlag+"_"+accFlag+"_"+norm_type+".root","RECREATE");
  M->Write("totalCov_TM");
  for(int i=0;i<M->GetNrows();++i){
    for(int j=0;j<M->GetNcols();++j){
      cout << i+1 << " " << j+1 << " " << M(i,j) << endl;
    }
  }
  fFile->Close();

  cout << "Validation " << fsrFlag << " " << accFlag << " " << norm_type << endl;
  for (int i = 0; i < 40; i++) {
    for (int j = 0; j < 40; j++) {
     if (fabs(M(i,j)-M(j,i)) > fabs(M(i,j))/10000.) cout << "Asymmetry warning " << i << " " << j << " " << M(i,j) << " " <<  M(j,i) << endl;
   }
  }

 for (int i = 0; i < 40; i++) {
   for (int j = 0; j < 40; j++) {
     if (i!=j) continue;
     double val = 0;
     if (fsrFlag == "pre" && accFlag == "fullAcc") val =diagonals_preFSR_fullAcc[j];
     if (fsrFlag == "pre" && accFlag == "inAcc") val =diagonals_preFSR_inAcc[j];
     if (fsrFlag == "post" && accFlag == "fullAcc") val =diagonals_postFSR_fullAcc[j];
     if (fsrFlag == "post" && accFlag == "inAcc") val =diagonals_postFSR_inAcc[j];
     if (norm_type == "absolute") {
          if (fsrFlag == "pre" && accFlag == "fullAcc") val = sqrt(sigma_Z*sigma_Z*diagonals_preFSR_fullAcc[i]*diagonals_preFSR_fullAcc[j] + rdiagonals_preFSR_fullAcc[i]*rdiagonals_preFSR_fullAcc[j]*sigmaZerrNoLumi);
          if (fsrFlag == "post" && accFlag == "fullAcc")  val = sqrt(sigma_Z*sigma_Z*diagonals_postFSR_fullAcc[i]*diagonals_postFSR_fullAcc[j] + rdiagonals_postFSR_fullAcc[i]*rdiagonals_postFSR_fullAcc[j]*sigmaZerrNoLumi);
          if (fsrFlag == "pre" && accFlag == "inAcc") val = sqrt(sigma_Z*sigma_Z*diagonals_preFSR_inAcc[i]*diagonals_preFSR_inAcc[j] + rdiagonals_preFSR_inAcc[i]*rdiagonals_preFSR_inAcc[j]*sigmaZerrNoLumi);
          if (fsrFlag == "post" && accFlag == "inAcc")  val = sqrt(sigma_Z*sigma_Z*diagonals_postFSR_inAcc[i]*diagonals_postFSR_inAcc[j] + rdiagonals_postFSR_inAcc[i]*rdiagonals_postFSR_inAcc[j]*sigmaZerrNoLumi);
     }
     if (norm_type == "normalized" || norm_type == "absolute") {
             val *= sqrt((mass_xbin[i+1]-mass_xbin[i])*(mass_xbin[j+1]-mass_xbin[j]));
     }
     if (fabs(sqrt(M(j,j)) - val) > fabs(sqrt(M(j,j)))/10000.) cout << "Warning " << sqrt(M(j,j)) << " " << val << " " << fabs(sqrt(M(j,j)) - val) << endl;
    }
  }

  cout << "Check positive definite" << endl;
  const int dim = 40;
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
