///
/*
INPUTS NEEDED (in the proper format):
effCorrelations -> root file obtained from the T&P efficiencies 
and toy MC samples 

r_results - txt (.dat) file with the results

r_syst - txt (.dat) file with the proper systematics of the results 
("proper" - for instance the backgrond uncertainty should be in %
just after the background subtraction and before any other data 
manipulation like unfolding; the cov matrix is estimated as 
unfolding * efficiency * FSR)

ACTION NEEDED:
writeEffCorrToRootFile(case)  -  stores efficiency covariance in covMatrices_case_store.root
writeShapeToRootFile(case) - stores data in shape_r_case_store.root
writeSystToRootFile(case) - stores data in shape_rSyst_case_store.root
writeTheoryToRootFile(case) - stores predictions in shape_r_Theory_case_store.root
(needs specific .dat inputs like the others but is used only for chi2 below)

The above files are used by  estimateCovMatrix(case)   /if you call it/
which produces covariance_finalResults_case.root. The last file contains the 
total covariance matrix (the final result) and also the three 
"intermediate" covariance matrices. It also contans the original 
data and uncertainties vectors used (from the dat files). 

Finally estimateChi2(case) will estimate chi2 between two vectors 
(typically between a channel and the prediction but could be made between two channels) 
with or without correlations. Be aware when comparing the two channels - 
the functionality exists but is not fully automated (keep track on what is loaded, which path, etc.). "useTheoryErrors" option is also not fully functional 
(not implemented for "notNormalized" for instance - be careful).
 
The arguments (with default values) are "2D"/"1D", "mu"/"el","inAcc"/"fullAcc", "preFSR"/"postFSR". Only relevant combinations are covered ("2D" with "el" will crash).

estimate* functions require to specify if you want the "normalized" (default) or "notNormalized" case to be estimated. Generally the efficiency covariance is different and this is why this is needed.

Execute in ROOT as ".L covariantMatrix.C" and then call functions.
You may want to first "cmsenv" (CMSSW) so your environment is properly set. 

*/
///
//#ifndef VECTOR
//#define VECTOR
#include <vector>
//#endif

//#ifndef IOSTREAM
//#define IOSTREAM
#include <iostream>
//#endif

//#ifndef FSTREAM
//#define FSTREAM
#include <fstream>
//#endif

void rootInit(void );

void readMatrix ( TH2D * h2_d1,  TMatrixT <double> & responseMatrix_read);

void singleUnfoldingCovariance( TMatrixT <double> &  responseMatrix, 
				TMatrixT <double>  & dataErr_TM,  
				TMatrixT <double> & unfoldCov_TM
				TMatrixT <double> & unfoldCorr_TM);
const int theNBins = 40;
Double_t mBins[theNBins+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

std::string directoryName_mu =  "./"; //"/data/stoyan/DY/muCovInputs_14.03.13/";
std::string directoryName_el =  "./";

double largeNumber = 1E9;
double smallNumber = 1E-9;

double normalization_data_2D_mu = 563.9;  //524.7;
double normalization_Theory_2D_mu = 569.7;//534.29;
double normalization_data_1D = 1150.2;     //989.5;//mu
double normalization_Theory_1D = 1137.2;   //970.0;


void writeEffCorrToRootFile(std::string resultType  = "1D", std::string channel = "el"){
  std::string directoryName = directoryName_mu;
  if("el" == channel){
    directoryName = directoryName_el;
  }
  std::string file_name =  "effCorr_12.09.13.root";
  if("mu"==channel){
      file_name =  "2_split.root"; //"effCorr_1D_case_01.11.12.root"; //"effCorr_12.09.13.root";
  } else{
      if("2D"!=resultType) file_name = "covRhoFile_el1D_100exps_nMB41_egamma_asymHLT_Unregressed_energy-allSyst.root";
      else file_name = "covRhoFile_el2D_100exps_nMB7_egamma_asymHLT_Unregressed_energy-allSyst.root";
  }
  file_name = directoryName + file_name;
  cout<<" file_name = "<<file_name<<std::endl; 
  TFile *f1=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f1 = new TFile(file_name.c_str());
  std::vector< TFile * > DataFiles;
  DataFiles.push_back(f1);

  int const nBins = 40;
  std::string effHisto_name = "effCorrelations_bins_NORM";
  if("mu"!=channel){
    effHisto_name = "corrRhoRho";
  }
  TH2D  * effCorr_read_NORM =  ( TH2D*)(*(DataFiles[0])).Get(effHisto_name.c_str());
  TMatrixT<double> effCorr_TM_NORM(nBins,nBins);
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      effCorr_TM_NORM[i][j] = effCorr_read_NORM->GetBinContent(i+1,j+1);
    }
  } 
 
  effHisto_name = "effCorrelations_bins";
  if("mu"!=channel){
    effHisto_name = "corrRhoRho";
  }
  TH2D  * effCorr_read =  ( TH2D*)(*(DataFiles[0])).Get(effHisto_name.c_str());
  TMatrixT<double> effCorr_TM(nBins,nBins);
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      effCorr_TM[i][j] = effCorr_read->GetBinContent(i+1,j+1);
    }
  }

  int const nBins = 132;
  //if("mu"!=channel){
  //  int const nBins = 40;
  //}
  effHisto_name = "effCorrelations_2D_bins_NORM";
  if("mu"!=channel){
    effHisto_name = "corrRhoRho";
  }
  TH2D  * effCorr_read_2D_NORM =  ( TH2D*)(*(DataFiles[0])).Get(effHisto_name.c_str());
  TMatrixT<double> effCorr_TM_2D_NORM(nBins,nBins);
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      effCorr_TM_2D_NORM[i][j] = effCorr_read_2D_NORM->GetBinContent(i+1,j+1);
    }
  }

  effHisto_name = "effCorrelations_2D_bins";
  if("mu"!=channel){
    effHisto_name = "corrRhoRho";
  }
  TH2D  * effCorr_read_2D =  ( TH2D*)(*(DataFiles[0])).Get(effHisto_name.c_str());
  TMatrixT<double> effCorr_TM_2D(nBins,nBins);
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){ 
      effCorr_TM_2D[i][j] = effCorr_read_2D->GetBinContent(i+1,j+1);
    }
  }
  std::string fileName = "covMatrices_store_" + resultType + "_" + channel + ".root";
  TFile * fFile = new TFile(fileName.c_str(),"RECREATE");
  effCorr_TM_NORM.Write("effCorrelations_bins_NORM");
  effCorr_TM.Write("effCorrelations_bins");
  effCorr_TM_2D_NORM.Write("effCorrelations_2D_bins_NORM");
  effCorr_TM_2D.Write("effCorrelations_2D_bins");
  fFile->Close();
  cout<<"Output written to "<<fileName<<std::endl;
}
//
void writeShapeToRootFile(std::string resultType  = "1D", std::string channel = "el", 
			  std::string acc = "fullAcc", std::string fsr = "preFSR" ){
  std::cout<<" Writing shape for the case: "<<resultType<<" "<<channel<<" "<<acc <<" "<< fsr <<std::endl;
  std::string directoryName = directoryName_mu;
  //if("el" == channel){
  //  directoryName = directoryName_el;
  //}

  if (resultType == "2D") acc = "inAcc";

  std::string inputFileName = "r2D_preFSR_inAcc_mu_03.04.14.dat";
  if ("el"==channel) inputFileName = "r2D_preFSR_inAcc_mu_26.03.13.dat";
  if("1D"==resultType){
    if("mu"==channel){
      if("fullAcc" == acc){
	if("preFSR" == fsr){
	  inputFileName = "r1D_preFSR_fullPhS_mu_03.04.14.dat";
	}
	else{
	  inputFileName = "r1D_postFSR_fullPhS_mu_14.03.13.dat";
	}
      }
      else{
	if("preFSR" == fsr){
	  inputFileName = "r1D_preFSR_inAcc_mu_14.03.13.dat";
	}
	else{
	  inputFileName = "r1D_postFSR_inAcc_mu_14.03.13.dat";
	}
      }
    }
    else{
      if("fullAcc" == acc){
	if("preFSR" == fsr){
	  inputFileName = "r1D_preFSR_fullPhS_el_03.04.14.dat";
	}
	else{
	  inputFileName = "r1D_postFSR_fullPhS_el_20121122.dat";
	}
      }
      else{
	if("preFSR" == fsr){
	  inputFileName = "r1D_preFSR_inAcc_el_20121122.dat";
	}
	else{
	  inputFileName = "r1D_preFSR_inAcc_el_20121122.dat";
	}
      }
    }
  }
  inputFileName = directoryName + inputFileName;
  char line[100];
  int iLine = 0;
  FILE* file=fopen(inputFileName.c_str(),"rt");
  std::cout<<" Open file : "<<inputFileName.c_str()<<std::endl;
  char tmp_char[100];
  double tmp_binBoundary;
  double tmp_result;
  double tmp_error;
  double tmp_double[2];

  int allBins = 132;
  if("1D"==resultType){
    allBins = 40;
  }
  int const nBins = allBins;

  TVectorT<double> shape_r_TV_2D(nBins);
  TVectorT<double> shape_rErr_TV_2D(nBins);

  int iBin = 0;
  int jBin = 0;
  int kBin = 0;
  while (fgets(line,100,file)!=NULL){
    istringstream iss( line );
    iss>>tmp_char;
    std::string myString(1,*tmp_char);
    std::string findString = "#";
    size_t found=myString.find(findString);
    if(found!=string::npos){
      continue;
    }
    iss>>tmp_binBoundary;
    iss>>tmp_result;
    iss>>tmp_error;
    /*
    if(!(jBin%24) ){
      jBin = 0;
      if(kBin){
	++iBin;
      }
      jBin = 0;
    }
    */
    std::cout<<" i = "<<iBin<<" j = "<<jBin<<" k = "<<kBin<<" ch1 = "<<tmp_char<<" ch[0] = "<<tmp_char[0]<<" bin b = "<<tmp_binBoundary<<" res = "<<tmp_result<<" tmp_error = "<<tmp_error<<std::endl;
    ++jBin;
    //shape_r_TM_2D[i][j] = tmp_result;
    //shape_rErr_TM_2D = tmp_error;
    shape_r_TV_2D[kBin] = tmp_result;
    shape_rErr_TV_2D[kBin] = tmp_error;
    ++kBin;
  }
  //std::string rootFile_name = "shape_r_2D_store.root";
  std::string rootFile_name = "shape_r_" + resultType +"_" + channel + "_" + fsr + "_" + acc + "_store.root";
  std::string shape_name = "shape_r_TV_" + resultType;
  std::string shapeErr_name = "shape_rErr_TV_" + resultType;
  /*
  if("1D"==resultType){
    rootFile_name = "shape_r_1D_store.root";
    shape_name = "shape_r_TV_1D";
    shapeErr_name = "shape_rErr_TV_1D";
  }
  */
  TFile * fFile = new TFile(rootFile_name.c_str(),"RECREATE");
  shape_r_TV_2D.Write(shape_name.c_str());
  shape_rErr_TV_2D.Write(shapeErr_name.c_str());
  fFile->Close();
  cout<<"Output written to "<<rootFile_name<<std::endl;
  return;
}
///


void writeTheoryToRootFile(std::string resultType  = "1D", std::string channel = "mu", 
			  std::string acc = "inAcc", std::string fsr = "preFSR", std::string pdf = "HERA"){
  std::cout<<" Writing theory predictions for the case: "<<resultType<<" "<<channel<<" "<<acc <<" "<< fsr <<std::endl;
  std::string directoryName = directoryName_mu;
  if("el" == channel){
    directoryName = directoryName_el;
  }
  directoryName = "../Theory/";
  std::string endString = "_14.03.13.dat";
  std::string inputFileName = "r2D_preFSR_inAcc_mu_Theory_HERA_14.03.13.dat";
  if("1D"==resultType){
    if("mu"==channel){
      if("fullAcc" == acc){
	if("preFSR" == fsr){
	  inputFileName = "r1D_preFSR_fullPhS_mu_Theory_HERA_14.03.13.dat";
	}
	else{
	  inputFileName = "r1D_preFSR_fullPhS_mu_Theory_HERA_14.03.13.dat";
	}
      }
      else{
	if("preFSR" == fsr){
	  inputFileName = "r1D_preFSR_fullPhS_mu_Theory_HERA_14.03.13.dat";
	}
	else{
	  inputFileName = "dummy.dat";
	}
      }
    }
    else{
      //inputFileName = "r1D_preFSR_fullPhS_el_13.11.12.dat";
      // inputFileName = "r1D_preFSR_fullPhS_el_20121122.dat";
      if("fullAcc" == acc){
	if("preFSR" == fsr){
	  inputFileName = "dummy.dat";
	}
	else{
	  inputFileName = "dummy.dat";
	}
      }
      else{
	if("preFSR" == fsr){
	  inputFileName = "dummy.dat";
	}
	else{
	  inputFileName = "dummy.dat";
	}
      }
    }
  }
  else{
    if("mu"==channel){
      if("inAcc" == acc){
	if("preFSR" == fsr){
	}
      }
    }
  }
  inputFileName = "r2D_"+ fsr + "_" + acc + "_" + channel + "_Theory_" + pdf + endString;

  inputFileName = directoryName + inputFileName;
  char line[100];
  int iLine = 0;
  FILE* file=fopen(inputFileName.c_str(),"rt");
  std::cout<<" Open file : "<<inputFileName.c_str()<<std::endl;
  char tmp_char[100];
  char tmp_char2[100];
  double tmp_binBoundary;
  double tmp_result;
  double tmp_error;
  double tmp_double[2];

  int allBins = 132;
  if("1D"==resultType){
    allBins = 40;
  }
  int const nBins = allBins;

  //int const nBins = 132;
  //TMatrixT<double> shape_r_TM_2D(nBins,nBins);
  //TMatrixT<double> shape_rErr_TM_2D(nBins,nBins);

  TVectorT<double> shape_r_TV_2D(nBins);
  TVectorT<double> shape_rErr_TV_2D(nBins);

  int iBin = 0;
  int jBin = 0;
  int kBin = 0;
  while (fgets(line,100,file)!=NULL){
    istringstream iss( line );
    iss>>tmp_char;
    std::string myString(1,*tmp_char);
    std::string findString = "#";
    size_t found=myString.find(findString);
    if(found!=string::npos){
      continue;
    }
    iss>>tmp_result;
    iss>>tmp_error;
    std::cout<<" i = "<<iBin<<" j = "<<jBin<<" ch = "<<tmp_char<<" res = "<<tmp_result<<" tmp_error = "<<tmp_error<<std::endl;
    ++jBin;
    //shape_r_TM_2D[i][j] = tmp_result;
    //shape_rErr_TM_2D = tmp_error;
    shape_r_TV_2D[kBin] = tmp_result;
    shape_rErr_TV_2D[kBin] = tmp_error;
    ++kBin;
  }
  //std::string rootFile_name = "shape_r_2D_store.root";
  std::string rootFile_name = "shape_r_Theory_" +pdf + "_" + resultType + "_" + channel + "_" + fsr + "_" + acc + "_store.root";
  std::string shape_name = "shape_r_TV_" + resultType;
  std::string shapeErr_name = "shape_rErr_TV_" + resultType;
  /*
  if("1D"==resultType){
    rootFile_name = "shape_r_1D_store.root";
    shape_name = "shape_r_TV_1D";
    shapeErr_name = "shape_rErr_TV_1D";
  }
  */
  TFile * fFile = new TFile(rootFile_name.c_str(),"RECREATE");
  shape_r_TV_2D.Write(shape_name.c_str());
  shape_rErr_TV_2D.Write(shapeErr_name.c_str());
  fFile->Close();
  cout<<"Output written to "<<rootFile_name<<std::endl;
  return;
}


///

void writeSystToRootFile(std::string resultType  = "1D", std::string channel = "el",
			 std::string acc = "fullAcc", std::string fsr = "preFSR"){
  std::cout<<" Writing Syst. for the case: "<<resultType<<" "<<channel<<" "<<acc <<" "<< fsr <<std::endl;
  std::string directoryName = directoryName_mu;
  if("el" == channel){
    directoryName = directoryName_el;
  }

  if (resultType == "2D") acc = "inAcc";

  std::string inputFileName = "r2D_syst_preFSR_inAcc_mu_03.04.14.dat";
  directoryName = "./";
  //FIXME put electron case 
  if (channel != "mu") inputFileName = "r2D_syst_preFSR_inAcc_mu_26.03.13.dat";
  if("1D"==resultType){
    if("mu"==channel){
      if("fullAcc" == acc){
	if("preFSR" == fsr){
	  inputFileName = "r1D_syst_preFSR_fullPhS_mu_03.04.14.dat";
          //inputFileName = "r1D_syst_preFSR_fullPhS_mu_14.03.13.dat";
	  directoryName = "./";
	}
	else{
	  inputFileName = "r1D_syst_postFSR_fullPhS_mu_14.03.13.dat";
	}
      }
      else{
	if("preFSR" == fsr){
	  inputFileName = "r1D_syst_preFSR_inAcc_mu_14.03.13.dat";
	}
	else{
	  inputFileName = "r1D_syst_postFSR_inAcc_mu_14.03.13.dat";
	}
      }
    }
    else{
      //inputFileName = "r1D_syst_preFSR_fullPhS_el_13.11.12.dat";
      //inputFileName = "r1D_syst_preFSR_fullPhS_el_20121122.dat";
      if("fullAcc" == acc){
	if("preFSR" == fsr){
	  inputFileName = "r1D_syst_preFSR_fullPhS_mu_03.04.14.dat";
	}
	else{
	  inputFileName = "dummy.dat";
	}
      }
      else{
	if("preFSR" == fsr){
	  inputFileName = "dummy.dat";
	}
	else{
	  inputFileName = "dummy.dat";
	}
      }
    }
  }
  inputFileName = directoryName + inputFileName;
  char line[150];
  int iLine = 0;
  FILE* file=fopen(inputFileName.c_str(),"rt");
  std::cout<<" Open file : "<<inputFileName.c_str()<<std::endl;
  char tmp_char1[20];
  char tmp_char2[20];
  char tmp_char3[20];
  char tmp_char4[20];
  char tmp_char5[20];
  char tmp_char6[20];
  char tmp_char7[20];

  double tmp_stat;
  double tmp_systUnfold;
  double tmp_systBG;
  double tmp_systEff;
  double tmp_systFSR;
  double tmp_total;
  double tmp_systEScale;
  double tmp_i1;
  double tmp_i2;

 int allBins = 132;
  if("1D"==resultType){
    allBins = 40;
  }
  int const nBins = allBins;

  //int const nBins = 132;
  //TMatrixT<double> shape_r_TM_2D(nBins,nBins);
  //TMatrixT<double> shape_rErr_TM_2D(nBins,nBins);

  TVectorT<double> shape_r_stat_TV_2D(nBins);
  TVectorT<double> shape_r_systUnfold_TV_2D(nBins);
  TVectorT<double> shape_r_systBG_TV_2D(nBins);
  TVectorT<double> shape_r_systEff_TV_2D(nBins);
  TVectorT<double> shape_r_systFSR_TV_2D(nBins);
  TVectorT<double> shape_r_totalErr_TV_2D(nBins);
  TVectorT<double> shape_r_systEScale_TV_2D(nBins);


  int kBin = 0;
  while (fgets(line,150,file)!=NULL){
    istringstream iss( line );
    /*
    sscanf(line,"%s %s %f %s %f %s %f %s %f %s %f", &tmp_char1, &tmp_char2,
	   &tmp_stat, &tmp_char3, &tmp_systEff, &tmp_char4,
	   &tmp_systUnfold, &tmp_char5, &tmp_systBG, &tmp_char6, 
	   &tmp_systFSR);
    */
    tmp_systEScale = 0;
    iss>>tmp_char1;
    /*
    if("mu"==channel){
      sscanf(line,"%s  %f  %f  %f  %f %f ", &tmp_char1,
	     &tmp_stat, &tmp_systEff,
	     &tmp_systUnfold, &tmp_systBG, &tmp_systFSR,  &tmp_total);
    }
    else{
      sscanf(line,"%s  %f  %f  %f  %f %f ", &tmp_char1,
	     &tmp_stat,  &tmp_systEScale, &tmp_systEff,
	     &tmp_systUnfold, &tmp_systBG, &tmp_systFSR,  &tmp_total);
    }
    */
    std::string myString(1,*tmp_char1);
    std::string findString = "#";
    size_t found=myString.find(findString);
    if(found!=string::npos){
      //      std::cout<<"ffffff"<<std::endl;
      continue;
    }
    iss>>tmp_stat;
    //FIXME: ok... it seems like we have an extra column for electron case! with the values of escale systematics. Everything else is unchanged 
    if("mu"!=channel){
      iss>>tmp_systEScale;
    }
    iss>>tmp_systEff;
    iss>>tmp_systUnfold;
    iss>>tmp_systBG;
    iss>>tmp_systFSR;
    iss>>tmp_total;
    //std::cout<<" k = "<<kBin<<" ch1 = "<<tmp_char1<<" tmp_systEff = "<<tmp_systEff<<std::endl;
    //++kBin; 
    //continue;
    //shape_r_TM_2D[i][j] = tmp_result;
    //shape_rErr_TM_2D = tmp_error;
    shape_r_stat_TV_2D[kBin] = tmp_stat;
    shape_r_systUnfold_TV_2D[kBin] = tmp_systUnfold;
    shape_r_systBG_TV_2D[kBin] = tmp_systBG;
    shape_r_systEff_TV_2D[kBin] = tmp_systEff;
    cout << "Bin kbin " << shape_r_systEff_TV_2D[kBin] << endl;
    shape_r_systFSR_TV_2D[kBin] = tmp_systFSR;
    shape_r_totalErr_TV_2D[kBin] = tmp_total;
    shape_r_systEScale_TV_2D[kBin] = tmp_systEScale;
    ++kBin;
  }
  std::string rootFile_name = "shape_rSyst_" +  resultType + "_" + channel + "_" + fsr + "_"+ acc + "_store.root";
  std::string statErr_name = "shape_r_stat_TV_" + channel;
  std::string unfErr_name = "shape_r_systUnfold_TV_"+ channel;
  std::string bgErr_name = "shape_r_systBG_TV_"+ channel;
  std::string effErr_name = "shape_r_systEff_TV_"+ channel;
  std::string fsrErr_name = "shape_r_systFSR_TV_"+ channel;
  std::string eScaleErr_name = "shape_r_systEScale_TV_"+ channel;
  std::string totalErr_name = "shape_r_totalErr_TV_"+ channel;
  
  TFile * fFile = new TFile(rootFile_name.c_str(),"RECREATE");
  shape_r_stat_TV_2D.Write(statErr_name.c_str());
  shape_r_systUnfold_TV_2D.Write(unfErr_name.c_str());
  shape_r_systBG_TV_2D.Write(bgErr_name.c_str());
  shape_r_systEff_TV_2D.Write(effErr_name.c_str());
  shape_r_systFSR_TV_2D.Write(fsrErr_name.c_str());
  shape_r_systEScale_TV_2D.Write(eScaleErr_name.c_str());
  shape_r_totalErr_TV_2D.Write(totalErr_name.c_str());
  fFile->Close();
  cout<<"Output written to "<<rootFile_name<<std::endl;
  return;
}
//
void estimateCovMatrix(std::string resultType  = "1D", std::string channel = "el",
		       std::string acc = "fullAcc", std::string fsr = "preFSR", std::string resultR = "unnormalized"){// "2D", "1D"

  if (resultType == "2D") acc = "inAcc";

  std::cout<<" Building covariance matrix for the case: "<<resultType<<" "<<channel<<" "<<acc <<" "<< fsr <<std::endl;
  std::string directoryName = directoryName_mu;
  //if("el" == channel){
  //  directoryName = directorymName_el;
 // }
  TCanvas *c2 = new TCanvas("c2", "canvas 2",16,30,800,600);
  rootInit();
  //std::string file_name = "covMatrix.root";
  std::string file_name = "covMatrices_store_" + resultType + "_" + channel + ".root";
  //if("1D"==resultType){
  //file_name = "covMatrices_1D_store.root";
  //}
  std::cout<<" file_name = "<<file_name<<std::endl;
  TFile *f1=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f1 = new TFile(file_name.c_str());
  file_name = "massResolUnfold_2014.root";
  if("1D"==resultType){
    if("mu"==channel){
      file_name = "massResolUnfold_2014.root";
    }
    else{
      file_name = "resMatrixProdEE_1D_2014.root";
      //file_name = "dyee_detResponse_1D-20121122.root";
    }
  } else {
     if ("mu"!=channel) file_name = "resMatrixProdEE_2D.root";
  }
  file_name = directoryName + file_name;
  TFile *f2=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f2 = new TFile(file_name.c_str());
  
  //file_name = "~/work/DY/input2D_18.07.12/resFSRMatrixProd_HLT_Mu13_Mu8_40bins_fullpowheg_InAcc.root";
  file_name = "resFSRMatrixProd_2Dunfold_InAcc.root";
  if("1D"==resultType){
    if("mu"==channel){
      file_name = "FSRresMatrixProd_tmp2014.root";
    }
    else{
      //file_name = "dyee_fsrResponse_1D-20121113.root";
      file_name = "EEFSRresMatrixProd_tmp2014.root";
    }
  } else {
     if ("mu"!=channel) file_name = "EEFSRresMatrixProd_2D_tmp.root";
  }

  file_name = directoryName + file_name;
  TFile *f3=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f3 = new TFile(file_name.c_str());
  
  file_name = "shape_r_" + resultType + "_" + channel + "_" + fsr + "_" + acc + "_store.root";
  TFile *f4=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f4 = new TFile(file_name.c_str());
  
  file_name = "shape_rSyst_" + resultType + "_" + channel + "_" + fsr + "_" + acc + "_store.root";
  TFile *f5=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f5 = new TFile(file_name.c_str());
  
  std::vector< TFile * > DataFiles;
  DataFiles.push_back(f1);
  DataFiles.push_back(f2);
  DataFiles.push_back(f3);
  DataFiles.push_back(f4);
  DataFiles.push_back(f5);
  double normalization_data = normalization_data_2D_mu;
  int allBins = 132;
  if("1D"==resultType){
    normalization_data = normalization_data_1D;
    allBins = 40;
  }
  int const nBins = allBins;
 
  TMatrixT <double> diagMatrix_TM(nBins,nBins);
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      if(i!=j){
        diagMatrix_TM[i][j] = 0;
      }
      else{
        diagMatrix_TM[i][i]=1;
      }
    }
  }
  
  std::string vectorName = "shape_r_TV_" +resultType ;
  TVectorT<double> *data_TV_tmp = (TVectorT<double> *)(*(DataFiles[3])).Get(vectorName.c_str());

  std::string vectorName = "shape_rErr_TV_" + resultType;
  TVectorT<double> *dataErr_TV_tmp = (TVectorT<double> *)(*(DataFiles[3])).Get(vectorName.c_str());
  
  if("normalized" !=resultR){
    (*data_TV_tmp)*= normalization_data;
    (*dataErr_TV_tmp)*= normalization_data;
  }
	    

  vectorName = "shape_r_stat_TV_" + channel;
  TVectorT<double> *shape_r_stat_TV_2D_tmp = (TVectorT<double> *)(*(DataFiles[4])).Get(vectorName.c_str());
  
  vectorName = "shape_r_systUnfold_TV_" + channel;
  TVectorT<double> *shape_r_systUnfold_TV_2D_tmp = (TVectorT<double> *)(*(DataFiles[4])).Get(vectorName.c_str());
  
  vectorName = "shape_r_systBG_TV_" + channel;
  TVectorT<double> *shape_r_systBG_TV_2D_tmp = (TVectorT<double> *)(*(DataFiles[4])).Get(vectorName.c_str());
  
  vectorName = "shape_r_systEff_TV_" + channel;
  TVectorT<double> *shape_r_systEff_TV_2D_tmp = (TVectorT<double> *)(*(DataFiles[4])).Get(vectorName.c_str());
  
  vectorName = "shape_r_systFSR_TV_" + channel;
  TVectorT<double> *shape_r_systFSR_TV_2D_tmp = (TVectorT<double> *)(*(DataFiles[4])).Get(vectorName.c_str());
  
  vectorName = "shape_r_systEScale_TV_" + channel;
  TVectorT<double> *shape_r_systEScale_TV_2D_tmp = (TVectorT<double> *)(*(DataFiles[4])).Get(vectorName.c_str());
  
  
  dataErr_TV_tmp->Draw();
  //return;
  //std::string matrixName = "hcovMatrix";
  //std::string matrixName = "responseMatrix";
  //TMatrixT <double> * responseMatrix_read =  (TMatrixT <double> *)(*(DataFiles[0])).Get(matrixName.c_str());
  //responseMatrix_read->Draw("colz");
  std::string matrixName = "hden"; //"migMatrix";
  if (resultType == "2D") matrixName = "hden2";
  if("el"==channel){
    matrixName = "DetResponse";
  }
  TH2D * h2_responceMatrix;
  TH2D* hmig;
  TH1D* _inTrue;

  if ("el"!=channel) {
     if (resultType != "2D") {
       hmig = (TH2D*)(*(DataFiles[1])).Get("hden");
       _inTrue = (TH1D*)(*(DataFiles[1])).Get("htrue");
     } else {
       hmig = (TH2D*)(*(DataFiles[1])).Get("hden2");
       _inTrue = (TH1D*)(*(DataFiles[1])).Get("htrue2");
     }
     h2_responceMatrix  = getMigMatrix(_inTrue, hmig);
 
  } else {
     h2_responceMatrix = (TH2D*)(*(DataFiles[1])).Get(matrixName.c_str());
  }

  //TMatrixT <double>  responseMatrix_read(nBins,nBins);
  //readMatrix ( h2_responceMatrix, responseMatrix_read);
  TMatrixT <double>  responseMatrix(nBins,nBins);
  cout << "1before readmatrix"<< endl; 
  readMatrix ( h2_responceMatrix, responseMatrix);
  cout << "1after readmatrix"<< endl;
  //h2_responceMatrix->Draw("colz");
  responseMatrix.Draw("colz");
  //return;
  TMatrixT <double> tM(responseMatrix);
  tM.Invert();
  tM.Print();  

  TMatrixT <double> unfoldingMatrix(responseMatrix);
  unfoldingMatrix.Print();
  unfoldingMatrix*=largeNumber;
  //unfoldingMatrix.Print();
  unfoldingMatrix.Invert();
  //unfoldingMatrix.Print();
  unfoldingMatrix*=largeNumber;
  //unfoldingMatrix*=smallNumber;
  unfoldingMatrix.Print();
  unfoldingMatrix.Draw("colz");
  //return;
  
  /*
    double dataErr[nBins];
    for(int i =0;i<nBins;++i){
    //dataErr[i] = 1.;
    dataErr[i] = 0.01;
    }
  */
  
  //TVectorT<double> dataErr_TV (nBins,dataErr);
  TVectorT<double> data_TV (*data_TV_tmp);
  TVectorT<double> dataErr_TV (*dataErr_TV_tmp);
  TVectorT<double> dataRelErr_TV (nBins);
  data_TV_tmp->Draw("hist");
  //return;
  
  
  TVectorT<double> shape_r_stat_TV_2D (*shape_r_stat_TV_2D_tmp);
  TVectorT<double> shape_r_systUnfold_TV_2D (*shape_r_systUnfold_TV_2D_tmp);
  TVectorT<double> shape_r_systBG_TV_2D (*shape_r_systBG_TV_2D_tmp);
  TVectorT<double> shape_r_systEff_TV_2D (*shape_r_systEff_TV_2D_tmp);
  TVectorT<double> shape_r_systFSR_TV_2D (*shape_r_systFSR_TV_2D_tmp);
  TVectorT<double> shape_r_systEscale_TV_2D (*shape_r_systEScale_TV_2D_tmp);
  
  dataErr_TV.Draw();
  //return;
  //TMatrixT <double>  responseMatrix(nBins,nBins);
  
  TMatrixT<double> dataErr_TM(nBins,nBins);
  TMatrixT<double> dataErrUnfold_TM(nBins,nBins);
  TMatrixT<double> dataErrBG_TM(nBins,nBins);
  TMatrixT<double> dataErrEff_TM(nBins,nBins);
  TMatrixT<double> dataErrFSR_TM(nBins,nBins);
  TMatrixT<double> dataErrEscale_TM(nBins,nBins);
  bool setRespToDiag = false;
  for(int i =0;i<nBins;++i){
    //cout<<" i = "<<i<<endl;
    for(int j =0;j<nBins;++j){
      //cout<<" j = "<<j<<endl;
      if(i!=j){
	dataErr_TM[i][j] = dataErrUnfold_TM[i][j]=dataErrEff_TM[i][j]=dataErrBG_TM[i][j]=0;
	if(setRespToDiag){
	  responseMatrix[i][j] = 0;
	}
      }
      else{
	dataErr_TM[i][j] = dataErr_TV[i];
	dataErrUnfold_TM[i][j]= (pow(shape_r_stat_TV_2D[i]/100.,2) + 
				 pow(shape_r_systUnfold_TV_2D[i]/100.,2) + 
				 pow(shape_r_systBG_TV_2D[i]/100.,2) + 
				 pow(shape_r_systEscale_TV_2D[i]/100.,2))*pow(data_TV[i],2); 
        dataErrEff_TM[i][j] = pow(shape_r_systEff_TV_2D[i]*data_TV[i]/100.,2);
	dataErrFSR_TM[i][j] = pow(shape_r_systFSR_TV_2D[i]*data_TV[i]/100.,2);
	if(setRespToDiag){
	  responseMatrix[i][j] = 1;
	}
      }
    }
    
    dataRelErr_TV[i] = dataErr_TV[i]/data_TV[i];
  }
  // dataErr_TM.Draw("colz");
  dataErrEff_TM.Draw("colz");
  //dataErrFSR_TM.Draw("colz");
  //dataErrUnfold_TM.Draw("colz");
  
  //std::cout<<std::endl;
  
  //return;
  TMatrixT <double> unfoldCov_TM(nBins,nBins);
  TMatrixT <double> unfoldCorr_TM(unfoldCov_TM);
  //
  // the errors dataErr_TM should correspond to the case before unfolding;
  // this means that you take the uncertainties in % before unfolding multiplied 
  // by the quantity to which these uncertainties apply (shape r for instance);
  // this is how you get dataErr_TV (or dataErr) and from it dataErr_TM;
  // the uncertainties will include statistics, background, and all the ones 
  // associated to unfolding (resolution and scales, unfolding systematics)
  //
  cout<<" Unfolding cov..."<<endl; 
  singleUnfoldingCovariance( responseMatrix, dataErrUnfold_TM, unfoldCov_TM, unfoldCorr_TM);
  //return;
  //unfoldCov_TM.Draw("colz");
  responseMatrix.Draw("colz");
  //return;
  cout<<" FSR unfolding cov..."<<endl; 
  //FIXME SAV 
  matrixName = "hden"; //"migMatrix";
  if ("el"==channel) {
    matrixName = "DetResponse";
  }

  TH2D * h2_responceMatrix_FSR;
  TH2D* hmig;
  TH1D* _inTrue;

  if ("el"!=channel) {
     if (resultType != "2D") {
       hmig = (TH2D*)(*(DataFiles[2])).Get("hden");
       _inTrue = (TH1D*)(*(DataFiles[2])).Get("hpreFSR");
     } else {
       hmig = (TH2D*)(*(DataFiles[2])).Get("hden");
       _inTrue = (TH1D*)(*(DataFiles[2])).Get("hpreFSR");
     } 
     h2_responceMatrix_FSR  = getMigMatrix(_inTrue, hmig);
  } else {
     h2_responceMatrix_FSR = (TH2D*)(*(DataFiles[2])).Get(matrixName.c_str());
  }
  TMatrixT <double>  responseMatrix_FSR(nBins,nBins);
  cout << "1before readmatrix2"<< endl;
  readMatrix ( h2_responceMatrix_FSR, responseMatrix_FSR);
  cout << "1after readmatrix2"<< endl;
  //responseMatrix_FSR = diagMatrix_TM;
/*
  FIXME special processing
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      if(i!=j && i>20 && fabs(i-j)>15){
        responseMatrix_FSR[i][j] = 0;
      }
      else{
      }
    }
  }
*/
  responseMatrix_FSR.Draw("colz");
  //responseMatrix_FSR.Print("all");
  //cout<<"hist el(100,122) = "<<h2_responceMatrix_FSR->GetBinContent(100+1,122+1)<<" el(122,100) = "<<h2_responceMatrix_FSR->GetBinContent(122+1,100+1)<<endl;
  //cout<<"matr el(100,122) = "<<responseMatrix_FSR[100][122]<<" el(122,100) = "<<responseMatrix_FSR[122][100]<<endl;
  //return;
  dataErrFSR_TM.Draw("colz");
  //return;
  TMatrixT <double> fsrCov_TM(nBins,nBins);
  TMatrixT <double> fsrCorr_TM(fsrCov_TM);
  //
  // dataErr_TM would be the FSR uncertainty alone
  // it would likely be sligthly different between normalized an not normalized cases
  //
  singleUnfoldingCovariance( responseMatrix_FSR, dataErrFSR_TM, fsrCov_TM, fsrCorr_TM);
  fsrCov_TM.Draw("colz");
  //dataErrFSR_TM.Draw("colz");
  //responseMatrix_FSR.Draw("colz");
  //return;
  
  cout<<" Eff cov..."<<endl; 
  std::string effHisto_name = "effCorrelations_2D_bins_NORM";
  if("normalized" !=resultR){
    effHisto_name = "effCorrelations_2D_bins";
  }
  if("1D"==resultType){
    effHisto_name = "effCorrelations_bins_NORM";
    if("normalized" !=resultR){
      effHisto_name = "effCorrelations_bins";
    }   
  }
  /*
    TH2D  * effCorr_read =  ( TH2D*)(*(DataFiles[1])).Get(effHisto_name.c_str());
    TMatrixT<double> effCorr_TM(nBins,nBins);
    for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
    effCorr_TM[i][j] = effCorr_read->GetBinContent(i+1,j+1);
    //(*effCorr_TM)[i][j] = 1;
    }
    }
  */
  TMatrixT<double> * effCorr_TM =  (TMatrixT <double> *)(*(DataFiles[0])).Get(effHisto_name.c_str());
  cout << "Name " << effHisto_name.c_str() << " " << DataFiles[0]->GetName()<< endl; 

  effCorr_TM->Draw("colz");
  //
  // The correlations for normalized vs not normalized case are very different
  // one should estimate them having input from T&P (similar to the error propagation
  // with MC toys)
  //
  TMatrixT<double> effCov_TM(nBins,nBins);
  //TVectorT<double> effErr_TV (nBins,effErr);
  //
  // Having the correlation matrix and the (diagonal) errors you get the covariance
  // In fact the covariance can be directly obtained instead of the correlations
  // with the studies above; in any case the diagonal errors come from the same type
  // of studies (the RMS of the efficiency correction in a bin) 
  //
  cout << "So far so good" << endl;
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      //effCov_TM[i][j] = sqrt(dataErrEff_TM[i][i])*sqrt(dataErrEff_TM[j][j])*(*effCorr_TM)[i][j];
    }
  }
  cout << "Wouldnt go through" << endl;
  //effErr_TV.Draw();
  //dataErrEff_TM.Draw("colz");
  //effCov_TM.Draw("colz");
  //fsrCov_TM.Draw("colz");
  //unfoldCov_TM.Draw("colz");
  //effCov_TM.Draw("colz");
  //unfoldCorr_TM.Draw("colz");
  //fsrCorr_TM.Draw("colz");
  //responseMatrix_FSR.Draw("colz");
  //return;
  cout<<" Total cov..."<<endl; 
  TMatrixT<double> totalCov_TM(nBins,nBins);
  TMatrixT <double> totalCorr_TM(totalCov_TM);
  //
  // This is teh full covariance matrix
  // All the systematics (and statistic) errors should be included
  // If something is missing then the job is not complete
  // To add more sources like theory error on acceptance you just make a diagonal matrix 
  // with the (absolute) errors squared and add it - this of course assumes
  // there are no correlations between bins (you can doubt it is true for theory errors) 
  //
  totalCov_TM = unfoldCov_TM + effCov_TM + fsrCov_TM;
  totalCov_TM.Draw("colz");
  for(int i=0;i<totalCorr_TM.GetNrows();++i){
    for(int j=0;j<totalCorr_TM.GetNcols();++j){
      totalCorr_TM[i][j] = totalCov_TM[i][j]/sqrt(totalCov_TM[i][i]*totalCov_TM[j][j]);
      cout << i+1 << " " << j+1 << " " << totalCov_TM[i][j] << endl;
    }
  }
  cout << "Not to be used out of the box!" << endl;

  /*
    TFile * fFile = new TFile("covMatrices_store.root","UPDATE");
    effCorr_TM->Write("effCorrelations_2D_bins_NORM");
    fFile->Close();
  */
  std::string rootFile_name = "covariance_finalResults_"+ resultType + "_" + channel + "_" + fsr + "_" + acc + "_" + resultR + ".root";
  //TFile * fFile = new TFile("covariance_finalResults.root","RECREATE");
  TFile * fFile = new TFile(rootFile_name.c_str(),"RECREATE");
  unfoldCov_TM.Write("unfoldCov_TM");
  effCov_TM.Write("effCov_TM");
  fsrCov_TM.Write("fsrCov_TM");
  totalCov_TM.Write("totalCov_TM");
  unfoldCorr_TM.Write("unfoldCorr_TM");
  effCorr_TM->Write("effCorr_TM");
  fsrCorr_TM.Write("fsrCorr_TM");
  totalCorr_TM.Write("totalCorr_TM");
  data_TV.Write("data_r_TV_original");
  dataErr_TV.Write("data_rAbsErr_TV_original");
  dataRelErr_TV.Write("data_rRelErr_TV_original");
  fFile->Close();
  cout<<" Results are in "<<rootFile_name.c_str()<<endl;
  
}
//
void estimateChi2(int binsRegion_min, int binsRegion_max, std::string resultType  = "2D", std::string channel = "mu",
		       std::string acc = "inAcc", std::string fsr = "preFSR", 
		       std::string resultR = "unnormalized", bool useTheoryErrors =true){// "2D", "1D"
  std::cout<<" Estimating chi2 for the case: "<<resultType<<" "<<channel<<" "<<acc <<" "<< fsr <<std::endl;
  std::string directoryName = directoryName_mu;
  directoryName = "./";
  if("el" == channel){
    directoryName = directoryName_el;
  }
  TCanvas *c2 = new TCanvas("c2", "canvas 2",16,30,800,600);
  rootInit();
  //std::string file_name = "covMatrix.root";
  //std::string file_name = "covariance_finalResults_"+ resultType + "_" + channel + "_" + fsr + "_" + acc + ".root";;
  std::string file_name = "covariance_finalResults_"+ resultType + "_" + channel + "_" + fsr + "_" + acc + "_" + resultR + ".root";
  if("mu-el"==channel){
    file_name = "covariance_finalResults_"+ resultType + "_" + "mu" + "_" + fsr + "_" + acc + "_" + resultR + ".root";
  }
  //if("1D"==resultType){
  //file_name = "covMatrices_1D_store.root";
  //}
  file_name = directoryName + file_name;
  std::cout<<" file_name = "<<file_name<<std::endl;
  TFile *f1=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f1 = new TFile(file_name.c_str());
  /*
    file_name = "effCorrelations_2M.root";
    TFile *f2=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
    TFile *f2 = new TFile(file_name.c_str());
  */
  //file_name = "~/work/DY/input2D_18.07.12/resMatrixProd_2Dunfold_corrRun2011AB.root";
  file_name = "shape_r_2D_mu_preFSR_inAcc_store.root";
  if("1D"==resultType){
    if("mu"==channel || "mu-el"==channel){
      file_name = "shape_r_1D_mu_preFSR_fullAcc_store.root";
    }
    else{
      //file_name = "dyee_detResponse_1D-20121113.root";
      file_name = "covariance_finalResults_"+ resultType + "_" + channel + "_" + fsr + "_" + acc + "_" + resultR + ".root";
    }
  }
  file_name = directoryName + file_name;
  TFile *f2=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f2 = new TFile(file_name.c_str());
  std::cout<<" file_name = "<<file_name<<std::endl;
  //file_name = "~/work/DY/input2D_18.07.12/resFSRMatrixProd_HLT_Mu13_Mu8_40bins_fullpowheg_InAcc.root";
  file_name = "shape_r_Theory_HERA_2D_mu_preFSR_inAcc_store.root";
  //file_name = "shape_r_Theory_HERA_2D_mu_preFSR_inAcc_store.root";
  //file_name = "shape_r_Theory_HERA_2D_mu_preFSR_inAcc_store.root";
  //file_name = "shape_r_Theory_HERA_2D_mu_preFSR_inAcc_store.root";
  if("1D"==resultType){
    if("mu"==channel){
      file_name = "shape_r_Theory_HERA_1D_mu_preFSR_fullAcc_store.root";
    }
    else{
      //file_name = "dyee_fsrResponse_1D-20121113.root";
      file_name = "shape_r_Theory_HERA_1D_mu_preFSR_fullAcc_store.root";
    }
  }
  directoryName = "../Theory/";
  file_name = directoryName + file_name;
  TFile *f3=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f3 = new TFile(file_name.c_str());
  std::cout<<" file_name = "<<file_name<<std::endl;

  file_name = "covariance_finalResults_"+ resultType + "_" + "mu" + "_" + fsr + "_" + acc + "_" + resultR + ".root";
  directoryName = directoryName_el;
  file_name = directoryName + file_name;
  TFile *f4=
    (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
  TFile *f4 = new TFile(file_name.c_str());


  std::vector< TFile * > DataFiles;
  DataFiles.push_back(f1);
  DataFiles.push_back(f2);
  DataFiles.push_back(f3);
  DataFiles.push_back(f4);

  //int binsRegion_min = 0;
  //int binsRegion_max = 23;

  //int binsRegion_min = 24;
  //int binsRegion_max = 47;

  //int binsRegion_min = 48;
  //int binsRegion_max = 71;

//Z peak
  //int binsRegion_min = 72;
  //int binsRegion_max = 95;

  //int binsRegion_min = 96;
  //int binsRegion_max = 119;

  //int binsRegion_min = 120;
  //int binsRegion_max = 131;

  int binsZPeak_min = 72;
  int binsZPeak_max = 95;

  double normalization_data = normalization_data_2D_mu;
  double normalization_Theory = normalization_Theory_2D_mu;
  int allBins = 132;
  if("1D"==resultType){
    normalization_data = normalization_data_1D;
    normalization_Theory = normalization_Theory_1D;
    allBins = 40;
    //binsRegion_min = 1;
    //binsRegion_max = 8;

    //binsRegion_min = 0;
    //binsRegion_max = 8;

// Z peak
    binsRegion_min = 9;
    binsRegion_max = 21;

    //binsRegion_min = 22;
    //binsRegion_max = 39;
    
    //binsRegion_min = 27;
    //binsRegion_max = 34;

    binsZPeak_min = 9;
    binsZPeak_max = 21;

  }
  int const nBins = allBins;

  TMatrixT <double> diagMatrix_TM(nBins,nBins);
  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      if(i!=j){
        diagMatrix_TM[i][j] = 0;
      }
      else{
        diagMatrix_TM[i][j]=1;
      }
    }
  }

  std::string matrixName =  "unfoldCov_TM";
  TMatrixT<double> *unfoldCov_TM_tmp = (TMatrixT<double> *)(*(DataFiles[0])).Get(matrixName.c_str());
  std::string matrixName =  "effCov_TM";
  TMatrixT<double> *effCov_TM_tmp = (TMatrixT<double> *)(*(DataFiles[0])).Get(matrixName.c_str());
  std::string matrixName =  "fsrCov_TM";
  TMatrixT<double> *fsrCov_TM_tmp = (TMatrixT<double> *)(*(DataFiles[0])).Get(matrixName.c_str());
  std::string matrixName =  "totalCov_TM";
  TMatrixT<double> *totalCov_TM_tmp = (TMatrixT<double> *)(*(DataFiles[0])).Get(matrixName.c_str());

  TMatrixT<double> *totalCov_TM_tmp2;
  if("mu-el"==channel){
    totalCov_TM_tmp2 = (TMatrixT<double> *)(*(DataFiles[3])).Get(matrixName.c_str());
  }

  std::string matrixName =  "totalCorr_TM";
  TMatrixT<double> *totalCorr_TM_tmp = (TMatrixT<double> *)(*(DataFiles[0])).Get(matrixName.c_str());

  std::string vectorName = "shape_r_TV_" +resultType ;
  TVectorT<double> *data1_TV_tmp = (TVectorT<double> *)(*(DataFiles[1])).Get(vectorName.c_str());

  vectorName = "shape_rErr_TV_" + resultType;
  TVectorT<double> *dataErr1_TV_tmp = (TVectorT<double> *)(*(DataFiles[1])).Get(vectorName.c_str());
  
  vectorName = "shape_r_TV_" +resultType ;
  TVectorT<double> *data2_TV_tmp = (TVectorT<double> *)(*(DataFiles[2])).Get(vectorName.c_str());

  if("mu-el"==channel){
    data2_TV_tmp = (TVectorT<double> *)(*(DataFiles[3])).Get(vectorName.c_str());
  }

  vectorName = "shape_rErr_TV_" + resultType;
  TVectorT<double> *dataErr2_TV_tmp = (TVectorT<double> *)(*(DataFiles[2])).Get(vectorName.c_str());

  if("normalized" !=resultR){
    (*data1_TV_tmp)*= normalization_data;
    (*data2_TV_tmp)*= normalization_Theory;
    (*dataErr2_TV_tmp)*= normalization_Theory;
  }

  double deviation = 0.;
  double chi2_step = 0.;
  TVectorT<double> diagErr(allBins);

  TMatrixT<double> cov_theory(*totalCov_TM_tmp);
  TMatrixT<double> cov_theoryRegion(*totalCov_TM_tmp);

  TMatrixT<double> cov_invert(*totalCov_TM_tmp);

  TMatrixT<double> cov_invert_Z(*totalCov_TM_tmp);
  if("mu-el"==channel){
    cov_invert_Z+=(*totalCov_TM_tmp2);
  }
  cov_invert_Z.ResizeTo(binsRegion_min,binsRegion_max,binsRegion_min,binsRegion_max);
  cov_theoryRegion.ResizeTo(binsRegion_min,binsRegion_max,binsRegion_min,binsRegion_max);


  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      //cout << i << " " << j << " " << cov_invert[i][j] << endl;
      if(i!=j){

        cov_theory[i][j] = 0;

	//cov_invert[i][j] = 0;
	//cov_invert_Z[i][j] = 0;
      }
      else{
        //FIXME acceptance
        cov_theory[i][j] = 0.; //pow((*dataErr2_TV_tmp)[i],2);

	//cout<<"i = "<<i<<" err = "<<(*dataErr2_TV_tmp)[i]<<" cov = "<<cov_theory[i][j]<<endl;
	//if(i<=binsZPeak_max && i>=binsZPeak_min && j<=binsZPeak_max && j>=binsZPeak_min){
	//if(i<=binsRegion_max && i>=binsRegion_min && j<=binsRegion_max && j>=binsRegion_min){
	  //cov_invert[i][j] *=2 ;// sqrt(2) ~ 40% on the error
	  //cov_invert[i][j] *=4 ;// sqrt(2) ~ 100% on the error
	  //cov_invert_Z[i][j]=cov_invert[i][j];
	  //cov_invert_Z[i][j] *=2 ;// sqrt(2) ~ 40% on the error
	  //cov_invert[i][j] *=1.21 ;
	  //cov_invert_Z[i][j] *=1.21 ;
	//}

	diagErr[i] = sqrt(cov_invert[i][j]);
	deviation = (*data1_TV_tmp)[i]-(*data2_TV_tmp)[i];
	chi2_step = (pow(deviation,2)/pow(diagErr[i],2));
	//if(chi2_step>1.5){
	  //cov_invert[i][j] = 4*chi2_step*cov_invert[i][j];
	  //diagErr[i] = sqrt(cov_invert[i][j]);
	//}
        //diagMatrix_TM[i][j]=1;
      }
    }
  }


  for(int i =0;i<nBins;++i){
    for(int j =0;j<nBins;++j){
      if(i!=j){
	// this does not change anything if cov is not changed; this is the definition of cov
	cov_invert[i][j] = sqrt(cov_invert[i][i])*sqrt(cov_invert[j][j])*(*totalCorr_TM_tmp)[i][j];
	//cout<<" i = "<<i <<" j = "<<j<<" cov = "<<cov_invert[i][j] <<endl;
	//if(i<=binsZPeak_max && i>=binsZPeak_min && j<=binsZPeak_max && j>=binsZPeak_min){// switch off the Z peak correlations
	//if((i>96 || i>72) && (j>96 || j<72)){// switch off the rest keeping the Z
	  //cov_invert[i][j] = 0;
          //cov_invert_Z[i][j] = 0.;
	//}
	//cov_invert[i][j] = 0;
      }
    }
  }
  if("mu-el"==channel){
    cov_invert+=(*totalCov_TM_tmp2);
    for(int i =0;i<nBins;++i){
      for(int j =0;j<nBins;++j){
	if(i==j){
	  diagErr[i] = sqrt(cov_invert[i][j]);
	}
      }
    }
  }
  //cov_theory.Print("all");
  if(useTheoryErrors){
    cov_invert+=cov_theory;
    for(int i =0;i<nBins;++i){
      for(int j =0;j<nBins;++j){
	if(i==j){
	  diagErr[i] = sqrt(cov_invert[i][j]);
	}
      }
    }
    cov_invert_Z+=cov_theoryRegion;
  }

  //cov_invert*=1.8;
  cov_invert.Draw("colz");
  //return;
  cov_invert*=largeNumber;
  cov_invert.Invert();
  cov_invert*=largeNumber;
  //cov_invert.Draw("colz");
  TVectorT<double> data_TV_tmp(*data1_TV_tmp - *data2_TV_tmp);
  data_TV_tmp *= cov_invert;

  TH1D *h1_chi2Steps = new TH1D("chi2Steps", "chi2Steps",500,0,100);
  double chi2 = data_TV_tmp*(*data1_TV_tmp - *data2_TV_tmp);
  //FIXME
  //cout<<" chi2 (with correlations)/ndf = "<<chi2/double(allBins)<<" ndf = "<<allBins<<std::endl;

  int bins_Region = binsRegion_max-binsRegion_min+1;
  //cov_invert_Z.Print("all");
  //cout<<" bins_Region = "<<bins_Region<<" row = "<<cov_invert_Z.GetNrows()<<" col "<<cov_invert_Z.GetNcols() <<endl;
  for(int i =cov_invert_Z.GetRowLwb();i<bins_Region+cov_invert_Z.GetRowLwb();++i){
    //cout<<" i = "<<i<<endl;
    for(int j =cov_invert_Z.GetColLwb();j<bins_Region+cov_invert_Z.GetColLwb();++j){
      //cout<<" i = "<<i<<" j = "<<j<<endl;
      if(i!=j){
        //cov_invert_Z[i][j] = 0.;
      }
    }
  }
  TVectorT<double> data_TV_tmp_Z(*data1_TV_tmp - *data2_TV_tmp);
  cov_invert_Z*=largeNumber;
  cov_invert_Z.Invert();
  cov_invert_Z*=largeNumber;
  data_TV_tmp_Z.ResizeTo(binsRegion_min,binsRegion_max);
  data_TV_tmp_Z *= cov_invert_Z;
  TVectorT<double> data_TV_tmp_Z2(*data1_TV_tmp - *data2_TV_tmp);
  data_TV_tmp_Z2.ResizeTo(binsRegion_min,binsRegion_max);
  //double chi2_Z = data_TV_tmp_Z*(*data1_TV_tmp - *data2_TV_tmp);
  double chi2_Region = data_TV_tmp_Z*data_TV_tmp_Z2;
  cout<<" chi2 (with correlations)/ndf region only= "<<chi2_Region/double(bins_Region)<<" ndf = "<<bins_Region<<std::endl;
//
  double chi2Simple = 0.;
  double chi2Simple_Region = 0.;
  double deviation_m = 0.;
  double deviation_p = 0.;
  double deviation_m_Region = 0.;
  double deviation_p_Region = 0.;
  bins_Region = 0;
  deviation = 0.;
  chi2_step = 0;
  for(int i =0;i<nBins;++i){
    //cout<<" i = "<<i<<endl;
    deviation = (*data1_TV_tmp)[i]-(*data2_TV_tmp)[i];
    chi2_step = (pow(deviation,2)/pow(diagErr[i],2));
    chi2Simple+=chi2_step;
    if(i<=binsRegion_max && i>=binsRegion_min ){//chosen region only
      chi2Simple_Region+=chi2_step;
      ++bins_Region;
      if(deviation>0){
        deviation_p_Region+=deviation;
      }
      else{
        deviation_m_Region+=deviation;
      }

    }
    h1_chi2Steps->Fill(chi2_step);
    if(deviation>0){
      deviation_p+=deviation;
    }
    else{
      deviation_m+=deviation;
    }
    //std::cout<<" i = "<<i<<" data1 = "<<(*data1_TV_tmp)[i]<<" data2 = "<<(*data2_TV_tmp)[i]<<" (d1 -d2)/d1 = "<< deviation/(*data1_TV_tmp)[i]<<" err = "<<diagErr[i]<<" errData (%) = "<<100*diagErr[i]/(*data1_TV_tmp)[i]<<" chi2_step = "<<chi2_step<<" chi2Sum = "<<chi2Simple<<std::endl;
  }
  //cout<<" simpleChi2/ndf = "<<chi2Simple/double(allBins)<<" pos/neg variations = "<<deviation_p <<"/"<<deviation_m<<endl;
  //cout<<" bins_Region = "<<bins_Region<<endl;
  cout<<" simpleChi2/ndf (region)= "<<chi2Simple_Region/double(bins_Region)<<" ndf = "<<bins_Region<<" pos/neg variations = "<<deviation_p_Region <<"/"<<deviation_m_Region<<endl;
  h1_chi2Steps->Draw();
  return;  
}


//
void rootInit(void ){
  gStyle->SetOptStat(0);
  //gStyle->SetOptStat(111111111);
  gStyle->SetTitleFillColor(0);
  gStyle->SetHistFillStyle(1001);


  gStyle->SetHistFillStyle(1001);
  gStyle->SetHistFillColor(51);
  gStyle->SetHistLineWidth(2);
  //gStyle->SetHistLineColor(4);
  gStyle->SetFrameFillColor(0);
  gPad->SetFrameFillColor(0);
  gStyle->SetTitleW(0.65);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.25);
  gStyle->SetStatH(0.2);
  gStyle->SetStatColor(0);
  gPad->SetFillColor(0);
  gPad->SetBorderMode(0);
  gStyle->SetHistFillStyle(5101);
  //gStyle->SetErrorX(0);
  gStyle->SetEndErrorSize(0);
  //c2->Divide(2,2);
  c2->SetFillStyle(0);
  c2->SetGrid();
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

}

//FIXME Alexey binning requested
void readMatrix ( TH2D * h2_d1, TMatrixT <double> & responseMatrix_read){
  int nXbins = h2_d1->GetNbinsX();
  int nYbins = h2_d1->GetNbinsY();
  std::cout<<" nXbins = "<<nXbins<<" nYbins = "<<nYbins<<std::endl;
  //TMatrixT<double> myMatrix(nXbins,nYbins);//should be the same as nBins
  for(int iX=1;iX<nXbins+1;++iX){
    //if (iX==nXbins-1) std::cout<<" iX = "<<iX<<" "<<std::endl;
    for(int iY=1;iY<nYbins+1;++iY){
      //if(iX==nXbins && iY==nYbins) std::cout<<"   iY = "<<iY<<" cont = "<<h2_d1->GetBinContent(iX,iY)<<std::endl;
      responseMatrix_read[iX-1][iY-1] = h2_d1->GetBinContent(iX,iY);
    }
  }

  //for(int i=)
  //TMatrixT<double> * myMatrix_ref = & myMatrix;
  //return myMatrix_ref;
  return;
}


void singleUnfoldingCovariance( TMatrixT <double> &  responseMatrix, 
				TMatrixT <double>  & dataErr_TM,  
				TMatrixT <double> & unfoldCov_TM,
				TMatrixT <double> & unfoldCorr_TM){
  //responseMatrix.Invert();
  TMatrixT <double> unfoldingMatrix(responseMatrix);
  unfoldingMatrix*=largeNumber;
  unfoldingMatrix.Invert();
  unfoldingMatrix*=largeNumber;
  //unfoldingMatrix*=smallNumber;
  //responseMatrix.Draw("colz");
  //unfoldingMatrix.Draw("colz");
  TMatrixT <double> unfoldingMatrix_transpose(unfoldingMatrix);
  unfoldingMatrix_transpose.Transpose(unfoldingMatrix);
  //unfoldingMatrix_transpose.Draw("colz");
  unfoldCov_TM = unfoldingMatrix*dataErr_TM;
  //unfoldCov_TM.Draw("colz");
  dataErr_TM.Draw("colz");
  unfoldCov_TM = unfoldCov_TM*unfoldingMatrix_transpose;
  //unfoldCov_TM.Draw("colz");
  //TMatrixT <double> unfoldCorr_TM(unfoldCov_TM);
  for(int i=0;i<unfoldCorr_TM.GetNrows();++i){
    for(int j=0;j<unfoldCorr_TM.GetNcols();++j){
      unfoldCorr_TM[i][j] = unfoldCov_TM[i][j]/sqrt(unfoldCov_TM[i][i]*unfoldCov_TM[j][j]);
    }
  }
  return;
}

TH2D* getMigMatrix(TH1D* htrue, TH2D* hden) {
          
    TH2D* migMatrix = (TH2D*)hden->Clone();
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
