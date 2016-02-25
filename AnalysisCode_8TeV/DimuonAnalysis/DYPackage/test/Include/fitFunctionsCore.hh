#ifndef FITFUNCTIONSCORE_HH
#define FITFUNCTIONSCORE_HH

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TChain.h>
#include <TCanvas.h>                // class for drawing
#include <TH1F.h>                   // 1D histograms
#include <TGraphErrors.h>           // graphs
#include <TProfile.h>               // profile histograms
#include <TBenchmark.h>             // class to track macro running statistics
#include <TLorentzVector.h>         // class for Lorentz vector computations
#include <TGraphAsymmErrors.h>
#include <TClonesArray.h>
#include <TMatrixD.h>

#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <vector>                   // STL vector class
#include <string>                   // C++ string class
#include <sstream>                  // class for parsing strings

#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooRealVar.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooDataHist.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooBreitWigner.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooExponential.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooCBShape.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooGaussian.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooGenericPdf.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooHistPdf.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooAddPdf.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooExtendPdf.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooFFTConvPdf.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooCategory.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooSimultaneous.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooDataSet.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooFitResult.h"
#include "/group/cms/users/asvyatko/roofit/roofit/inc/RooCMSShape.h"
#include "/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/roofit/5.32.03/include/RooChebychev.h"


#include "CPlot.hh"          // helper class for plots
#include "DYTools.hh"

#endif

enum SignalFitPDFType {TEMPLATE_CONV_GAUSS,
		       TEMPLATE_CONV_CB};

enum BackgroundFitPDFType {EXPONENTIAL,
			   CMS_SHAPE, 
			   CHEBYCHEV};


void printCorrelations(ostream& os, RooFitResult *res);

void fitMass(TTree *passTree, TTree *failTree, TString cut, int mode, 
     double &efficiency, double &efficiencyErrHigh, double &efficiencyErrLow, 
	     TPad *passPad, TPad *failPad, TFile *plotsRootFile,
	     ofstream &fitLog, int NsetBins, 
	     bool isRECO, const char* setBinsType, TString dirTag);

void fitMassWithTemplates(TTree *passTree, TTree *failTree, TString cut, 
			  int mode, 
			  double &efficiency, 
			  double &efficiencyErrHigh, double &efficiencyErrLow, 
			  TPad *passPad, TPad *failPad, TFile *plotsRootFile,
			  ofstream &fitLog, 
			  TH1F *templatePass, TH1F *templateFail, 
			  bool isRECO, const char* setBinsType, 
			  TString dirTag, const TString &picFileExtraTag );

#endif
