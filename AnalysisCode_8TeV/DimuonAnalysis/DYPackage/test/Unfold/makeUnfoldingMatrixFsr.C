#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TClonesArray.h>           // ROOT array class
#include "TMatrixD.h"
#include "TVectorD.h"
#include <TCanvas.h>                // class for drawing
#include <TH1F.h>                   // 1D histograms
#include <TCanvas.h>                // class for drawing
#include <TH1F.h>                   // 1D histograms
#include <TGraphErrors.h>           // graphs
#include <TProfile.h>               // profile histograms
#include <TBenchmark.h>             // class to track macro running statistics
#include <TLorentzVector.h>         // class for Lorentz vector computations
#include <TVectorD.h>
#include <TStyle.h>
#include <TRandom.h>
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <vector>                   // STL vector class
#include <algorithm>
#include <string>                   // C++ string class
#include <sstream>                  // class for parsing strings

#include "../Include/CPlot.hh"          // helper class for plots
#include "../Include/MitStyleRemix.hh"  // style settings for drawing
#include "../Include/MyTools.hh"        // miscellaneous helper functions
#include "../Include/DYTools.hh"
#include "../Include/plotFunctions.hh"

// define classes and constants to read in ntuple
#include "../Include/EWKAnaDefs.hh"
#include "../Include/TEventInfo.hh"
#include "../Include/TGenInfo.hh"
#include "../Include/TDielectron.hh"   
#include "../Include/TMuon.hh"

// define classes and constants to read in ntuple
//#include "../Include/EWKAnaModsLinkDef.h"
#include "../Include/TGenPhoton.hh"

// Helper functions for Electron ID selection
#include "../Include/EleIDCuts.hh"

#include "../Include/ElectronEnergyScale.hh" //extra smearing
#include "../Include/UnfoldingTools.hh"

  // Trigger info
#include "../Include/TriggerSelection.hh"

#include "../Include/EventSelector.hh"
#include "../Include/FEWZ.hh"
#include "../Include/InputFileMgr.hh"
#include "../Include/PUReweight.hh"
#include "../Include/eventCounter.h"

//For post Andrius efficiency scale factor tests
#include "../Include/efficiencyWeightToBin2012.C"

//for getting matrix condition number
#include <TDecompLU.h>

#endif

using namespace std;

//=== FUNCTION DECLARATIONS ======================================================================================

int nUnfoldingBins = DYTools::getTotalNumberOfBins();

void computeNormalizedBinContent(double subset, double subsetErr,
				 double total, double totalErr,
				 double& ratio, double& ratioErr);
void calculateInvertedMatrixErrors(const TMatrixD &T, 
          const TMatrixD &TErrPos, const TMatrixD &TErrNeg,
				   TMatrixD &TinvErr);

// ---------------------------------------------------------------------

inline bool validFlatIndex(int idx) {
  return ( idx != -1 && idx < nUnfoldingBins ) ? true : false;
}

inline bool validFlatIndices(int idx1, int idx2) {
  return (validFlatIndex(idx1) && validFlatIndex(idx2)) ? true : false;
}


//=== Class DECLARATIONS =================================================================================================

class UnfoldingMatrix_t {
public:
  typedef enum { _cDET_Response, _cFSR, _cFSR_DET, _cFSR_DETcorrFactors } TUnfoldingMatrixType_t;
  // DET_Response: ini --> PostFsrGen, fin --> PostFsrReco
  // FSR, FSR_DET: ini --> PreFsrGen,  fin -->PostFsrGen
  // FSR_DETcorrFactors: factors, correcting preFSR and postFSR acceptance
  // yieldsIni: gen, yieldsFin: reco
public:
  TUnfoldingMatrixType_t kind;
  TString name, iniYieldsName, finYieldsName;
  TMatrixD *yieldsIni; //(DYTools::nMassBins,DYTools::nYBinsMax);
  TMatrixD *yieldsFin; //(DYTools::nMassBins,DYTools::nYBinsMax);

  // Matrices for unfolding
  TMatrixD *DetMigration; //(nUnfoldingBins, nUnfoldingBins);
  TMatrixD *DetMigrationErr; //(nUnfoldingBins, nUnfoldingBins);
  TMatrixD *DetResponse; //(nUnfoldingBins, nUnfoldingBins);
  TMatrixD *DetResponseErrPos; //(nUnfoldingBins, nUnfoldingBins);
  TMatrixD *DetResponseErrNeg; //(nUnfoldingBins, nUnfoldingBins);
  TMatrixD *DetInvertedResponse; //(nUnfoldingBins, nUnfoldingBins);
  TMatrixD *DetInvertedResponseErr; //(nUnfoldingBins, nUnfoldingBins);

  TVectorD *DetResponseArr; //(nUnfoldingBins);
  TVectorD *DetInvertedResponseArr; //(nUnfoldingBins);
  TVectorD *DetInvertedResponseErrArr; //(nUnfoldingBins);
  TVectorD *yieldsIniArr; //(nUnfoldingBins)
  TVectorD *yieldsFinArr; //(nUnfoldingBins);

public:
  static TString kindName(TUnfoldingMatrixType_t aKind) {
    TString s="unknown";
    switch(aKind) {
    case _cDET_Response: s="DetResponse"; break;
    case _cFSR: s="FSR"; break;
    case _cFSR_DET: s="FSR_DET"; break;
    case _cFSR_DETcorrFactors: s="FSR_DETcorrFactors"; break;
    }
    return s;
  }

  TString ourKindName() const { return UnfoldingMatrix_t::kindName(this->kind); }

public:
  UnfoldingMatrix_t(TUnfoldingMatrixType_t set_kind, const TString &set_name) : 
    kind(set_kind),
    name(set_name), iniYieldsName(), finYieldsName(),
    yieldsIni(0), yieldsFin(0),
    DetMigration(0), DetMigrationErr(0),
    DetResponse(0), DetResponseErrPos(0), DetResponseErrNeg(0),
    DetResponseArr(0), DetInvertedResponseArr(0), DetInvertedResponseErrArr(0),
    yieldsIniArr(0), yieldsFinArr(0)
  {
    TMatrixD my(DYTools::nMassBins,DYTools::nYBinsMax);
    TMatrixD unf(nUnfoldingBins, nUnfoldingBins);
    TVectorD arr(nUnfoldingBins);
    my=0; unf=0; arr=0;
    yieldsIni=new TMatrixD(my); yieldsFin=new TMatrixD(my);
    DetMigration=new TMatrixD(unf); DetMigrationErr=new TMatrixD(unf);
    DetResponse=new TMatrixD(unf); 
    DetResponseErrPos=new TMatrixD(unf); DetResponseErrNeg=new TMatrixD(unf);
    DetInvertedResponse=new TMatrixD(unf); DetInvertedResponseErr=new TMatrixD(unf);
    DetResponseArr=new TVectorD(arr);
    DetInvertedResponseArr=new TVectorD(arr); DetInvertedResponseErrArr=new TVectorD(arr);
    yieldsIniArr=new TVectorD(arr); yieldsFinArr=new TVectorD(arr);
    UnfoldingMatrix_t::getYieldNames(set_kind, iniYieldsName, finYieldsName);
  }

  void fillIni(int iMassBinGen, int iYBinGen, double fullWeight) {
    using namespace DYTools;
    if ((iMassBinGen==-1) || (iYBinGen==-1)) {
      return;
    }
    if ((iMassBinGen >= nMassBins) ||
	(iYBinGen >= nYBins[iMassBinGen])) {
      std::cout << "UnfoldingMatrix_t::fillIni(" << iMassBinGen << "," << iYBinGen << "): incorrect indices. Max values (" << nMassBins << "," << (((iMassBinGen>=0) && (iMassBinGen<nMassBins)) ? nYBins[iMassBinGen] : nYBinsMax) << "; matrixName=<" << name << ">\n";
      assert(0);
    }
    (*yieldsIni)(iMassBinGen,iYBinGen) += fullWeight;
  }

  void fillFin(int iMassBinReco, int iYBinReco, double fullWeight) {
    using namespace DYTools;
    if ((iMassBinReco==-1) || (iYBinReco==-1)) {
      return;
    }
    if ((iMassBinReco >= nMassBins) ||
	(iYBinReco >= nYBins[iMassBinReco])) {
      std::cout << "UnfoldingMatrix_t::fillPostFin(" << iMassBinReco << "," << iYBinReco << "): incorrect indices. Max values (" << nMassBins << "," << (((iMassBinReco>=0) && (iMassBinReco<nMassBins)) ? nYBins[iMassBinReco] : nYBinsMax) << "; matrixName=<" << name << ">\n";
      assert(0);
    }
    (*yieldsFin)(iMassBinReco,iYBinReco) += fullWeight;
  }

  void fillMigration(int idx1, int idx2, double weight) {
    if ( !validFlatIndices(idx1,idx2) ) {
      std::cout << "UnfoldingMatrix_t::fillMigration: idx1=" << idx1 << ", idx2=" << idx2 << ". Max allowed values: " << nUnfoldingBins << "; matrixName=<" << name << ">\n";
      return;
    }
    (*DetMigration)(idx1,idx2) += weight;
    (*DetMigrationErr)(idx1,idx2) += weight * weight;
  }

  void finalizeDetMigrationErr() {
    for(int i=0; i < (*DetMigration).GetNrows(); i++)
      for(int j=0; j < (*DetMigration).GetNcols(); j++)
	if( (*DetMigrationErr)(i,j) >=0 )
	  (*DetMigrationErr)(i,j) = sqrt( (*DetMigrationErr)(i,j) );
	else {
	  printf("UnfoldingMatrix_t::finalizeDetMigrationErr Error: negative weights in DetMigrationErr\n");
	  std::cout << "matrixName=<" << name << ">\n";
	  assert(0);
	}
  }

  void computeResponseMatrix() {
    double tCentral, tErr;
    for(int igen = 0; igen < (*DetMigration).GetNrows(); igen++){
      // First find the normalization for the given generator level slice
      double nEventsInGenBin = 0;
      double nEventsInGenBinErr = 0;
      for(int ireco = 0; ireco < (*DetMigration).GetNcols(); ireco++){
	nEventsInGenBin += (*DetMigration)(igen,ireco);
	nEventsInGenBinErr += ((*DetMigrationErr)(igen,ireco)*
			       (*DetMigrationErr)(igen,ireco));
      }
      nEventsInGenBinErr = sqrt(nEventsInGenBinErr);
      
      // Now normalize each element and find errors
      for(int ireco = 0; ireco < (*DetMigration).GetNcols(); ireco++){
	tCentral = 0;
	tErr     = 0;
	computeNormalizedBinContent((*DetMigration)(igen,ireco),
				    (*DetMigrationErr)(igen,ireco),
				    nEventsInGenBin,
				    nEventsInGenBinErr,
			    tCentral, tErr);
	(*DetResponse)      (igen,ireco) = tCentral;
	(*DetResponseErrPos)(igen,ireco) = tErr;
	(*DetResponseErrNeg)(igen,ireco) = tErr;
      }
    }
  }

  // This is Ilya's suggestion: modify the response matrix
  void computeResponseMatrix_Mdf(const UnfoldingMatrix_t &exact) {
    //this->computeResponseMatrix();
    std::cout << "computeResponseMatrix_Mdf\n";
    std::cout << "nUnfoldingBins " << nUnfoldingBins << std::endl;
    double tCentral, tErr;
    TVectorD iniV(nUnfoldingBins), finV(nUnfoldingBins);
    TVectorD iniVexact(nUnfoldingBins), finVexact(nUnfoldingBins);
    int resFlatten= 
      (
       (unfolding::flattenMatrix(*yieldsIni, iniV) == 1 ) &&
       (unfolding::flattenMatrix(*yieldsFin, finV) == 1 ) &&
       (unfolding::flattenMatrix(*exact.yieldsIni, iniVexact) == 1 ) &&
       (unfolding::flattenMatrix(*exact.yieldsFin, finVexact) == 1 ) 
       ) ? 1:0;
    assert(resFlatten);

    for(int igen = 0; igen < (*DetMigration).GetNrows(); igen++){
      // First find the normalization for the given generator level slice
      double nEventsInGenBin = 0;
      double nEventsInGenBinErr = 0;
      for(int ireco = 0; ireco < (*DetMigration).GetNcols(); ireco++){
	nEventsInGenBin += (*DetMigration)(igen,ireco);
	nEventsInGenBinErr += ((*DetMigrationErr)(igen,ireco)*
			       (*DetMigrationErr)(igen,ireco));
      }
      nEventsInGenBinErr = sqrt(nEventsInGenBinErr);
      
      //printf("igen=%d, nEventsInGenBin=%9.4lf, ini/fin=%9.4lf/%9.4lf, exact ini/fin=%9.4lf/%9.4lf\n",igen,nEventsInGenBin,iniV[igen],finV[igen],iniVexact[igen],finVexact[igen]);
      double scaleIniInv=
	( iniV[igen] == 0 ) ? 0. : (iniVexact[igen]/iniV[igen]);

      // Now normalize each element and find errors
      for(int ireco = 0; ireco < (*DetMigration).GetNcols(); ireco++){
      // prepare scale in the reconstructed bin
	double scaleFin=
	  ( finVexact[ireco] == 0 ) ? 0. : (finV[ireco]/finVexact[ireco] );

	tCentral = 0;
	tErr     = 0;
	computeNormalizedBinContent((*DetMigration)(igen,ireco),
				    (*DetMigrationErr)(igen,ireco),
				    nEventsInGenBin,
				    nEventsInGenBinErr,
				    tCentral, tErr);
	if ((igen<2) && (ireco<2)) {
	  printf("igen=%d, ireco=%d, tCentral=%8.4lf, tErr=%8.4lf, scaleIniInv=%6.4lf, scaleFin=%6.4lf\n",igen,ireco,tCentral,tErr,scaleIniInv,scaleFin);
	}
	(*DetResponse)      (igen,ireco) = tCentral * scaleIniInv*scaleFin;
	(*DetResponseErrPos)(igen,ireco) = tErr  *scaleIniInv*scaleFin;
	(*DetResponseErrNeg)(igen,ireco) = tErr *scaleIniInv*scaleFin;
      }
    }
  }


  // DET correction factors defined as all.yields/restricted.yields, where
  // all.yields contain events in the acceptance which pass only the relevant
  // pre-FSR or post-FSR cuts, while restricted.yields contain events in the
  // acceptance that passed both pre-FSR and post-FSR cuts (but their masses
  // are either pre-FSR /ini/ or post-FSR /fin/)
  void prepareFsrDETcorrFactors(const UnfoldingMatrix_t &all, const UnfoldingMatrix_t &restricted) {
    for (int iVec=0; iVec<2; ++iVec) {
      TMatrixD *nomV=(iVec==0) ? all.yieldsIni : all.yieldsFin;
      TMatrixD *denomV=(iVec==0) ? restricted.yieldsIni : restricted.yieldsFin;
      TMatrixD *ratioV=(iVec==0) ? yieldsIni : yieldsFin;
      for (int iM=0; iM<nomV->GetNrows(); ++iM) {
	for (int iY=0; iY<nomV->GetNcols(); ++iY) {
	  double d=(*denomV)[iM][iY];
	  double r=(d==0.) ? 0. : ((*nomV)[iM][iY] / d);
	  //std::cout << "divide " << (*nomV)[iM][iY] << " by " << d << " = " << r << "\n";
	  (*ratioV)[iM][iY] = r;
	}
      }
    }
    if (0) {
      std::cout << std::string(80,'-') << "\n";
      std::cout << "prepareFsrDETcorrFactors\n";
      std::cout << std::string(80,'-') << "\n";
      all.printYields();
      restricted.printYields();
      this->printYields();
      std::cout << std::string(80,'-') << "\n";
    }
  }

  void invertResponseMatrix() {
  // Find inverted response matrix
    (*DetInvertedResponse) = (*DetResponse);
    Double_t det;
    (*DetInvertedResponse).Invert(&det);
    calculateInvertedMatrixErrors(*DetResponse, *DetResponseErrPos, *DetResponseErrNeg, *DetInvertedResponseErr);
  }


  // apply factors to the response and inv.response matrices to account
  // for the event loss in DET
  void modifyDETResponseMatrices(const UnfoldingMatrix_t &exact) {
    HERE("modifyDETResponseMatrices");
    TVectorD iniV(nUnfoldingBins), finV(nUnfoldingBins);
    TVectorD iniVexact(nUnfoldingBins), finVexact(nUnfoldingBins);
    int resFlatten= 
      (
       (unfolding::flattenMatrix(*yieldsIni, iniV) == 1 ) &&
       (unfolding::flattenMatrix(*yieldsFin, finV) == 1 ) &&
       (unfolding::flattenMatrix(*exact.yieldsIni, iniVexact) == 1 ) &&
       (unfolding::flattenMatrix(*exact.yieldsFin, finVexact) == 1 ) 
       ) ? 1:0;
    assert(resFlatten);

    for(int igen = 0; igen < (*DetMigration).GetNrows(); igen++){
      for(int ireco = 0; ireco < (*DetMigration).GetNcols(); ireco++){
	double scaleIni=
	  ( iniVexact[igen] == 0 ) ? 0. : (iniV[igen]/iniVexact[igen]);
	double scaleIniInv=
	  ( iniV[igen] == 0 ) ? 0. : (iniVexact[igen]/iniV[igen]);
	double scaleFin=
	  ( finVexact[ireco] == 0 ) ? 0. : (finV[ireco]/finVexact[ireco] );
	double scaleFinInv=
	  ( finV[ireco] == 0 ) ? 0. : (finVexact[ireco]/finV[ireco] );

	(*DetResponse)      (igen,ireco) *= scaleIniInv*scaleFin;
	(*DetResponseErrPos)(igen,ireco) *= scaleIniInv*scaleFin;
	(*DetResponseErrNeg)(igen,ireco) *= scaleIniInv*scaleFin;
	(*DetInvertedResponse)   (ireco,igen) *= scaleIni*scaleFinInv;
	(*DetInvertedResponseErr)(ireco,igen) *= scaleIni*scaleFinInv;
      }
    }
  }


  void prepareFIArrays() {
    int resFlatten=
      (unfolding::flattenMatrix(*DetResponse, *DetResponseArr) == 1) &&
      (unfolding::flattenMatrix(*DetInvertedResponse, *DetInvertedResponseArr) == 1) &&
      (unfolding::flattenMatrix(*DetInvertedResponseErr, *DetInvertedResponseErrArr) == 1) &&
      (unfolding::flattenMatrix(*yieldsIni, *yieldsIniArr) == 1) &&
      (unfolding::flattenMatrix(*yieldsFin, *yieldsFinArr) == 1);
    if (!resFlatten) {
      std::cout << "Error : failed to flatten the arrays\n";
      assert(0);
    }
  }


  static void getYieldNames(TUnfoldingMatrixType_t theKind, TString &iniName, TString &finName) {
    switch(theKind) {
    case _cDET_Response: 
      iniName="yieldsMcPostFsrGen";
      finName="yieldsMcPostFsrRec";
      break;
    case _cFSR:
      iniName="yieldsMcPreFsrGen";
      finName="yieldsMcPostFsrGen";
      break;
    case _cFSR_DET:
      iniName="yieldsMcPreFsrGenDET";
      finName="yieldsMcPostFsrGenDET";
      break;
    case _cFSR_DETcorrFactors:
      iniName="fsrDETcorrFactorsGen";
      finName="fsrDETcorrFactorsReco";
      break;
    default:
      std::cout << "getYieldNames cannot handle this 'kind'=" << theKind << "\n";
      assert(0);
    }
  }

  void getFileNames(const TString &outputDir,
		    const TString &fileTag,
		    TString &matrixFName, TString &yieldsFName) const {
    matrixFName=outputDir + this->name + TString("_unfolding_constants") + 
      fileTag + TString(".root");
    yieldsFName=outputDir + TString("yields_") + this->name + 
      fileTag + TString(".root");
  }

  void autoSaveToFile(const TString &outputDir, const TString &fileTag) const {
    TString matrixFName,yieldsFName;
    this->getFileNames(outputDir,fileTag, matrixFName,yieldsFName);
    std::cout << "saving to files <" << matrixFName << "> and <" << yieldsFName << ">\n";
    this->saveToFile(matrixFName,yieldsFName);
  }
  
  int autoLoadFromFile(const TString &outputDir, const TString &fileTag) {
    TString matrixFName,yieldsFName;
    this->getFileNames(outputDir,fileTag, matrixFName,yieldsFName);
    std::cout << "loading from files <" << matrixFName << "> and <" << yieldsFName << ">\n";
    return this->loadFromFile(matrixFName,yieldsFName);
  }
  

  void saveToFile(const TString &fileName, const TString &refFileName) const {
    std::cout << "UnfoldingMatrix_t::saveToFile(\n  <" << fileName << ">\n  <" << refFileName << ">) for name=" << this->name << "\n";
    if (kind!=_cFSR_DETcorrFactors) {
      TFile fConst(fileName, "recreate" );
      //name.Write("matrixName");
      (*DetMigration)            .Write("DetMigration");
      (*DetMigrationErr)         .Write("DetMigrationErr");
      (*DetResponse)             .Write("DetResponse");
      (*DetResponseErrPos)       .Write("DetResponseErrPos");
      (*DetResponseErrNeg)       .Write("DetResponseErrNeg");
      (*DetInvertedResponse)     .Write("DetInvertedResponse");
      (*DetInvertedResponseErr)  .Write("DetInvertedResponseErr");
      (*DetResponseArr)          .Write("DetResponseFIArray");
      (*DetInvertedResponseArr)  .Write("DetInvertedResponseFIArray");
      (*DetInvertedResponseErrArr).Write("DetInvertedResponseErrFIArray");
      (*yieldsIni).Write(iniYieldsName);
      (*yieldsFin).Write(finYieldsName);
      (*yieldsIniArr).Write(iniYieldsName + TString("FIArray"));
      (*yieldsFinArr).Write(finYieldsName + TString("FIArray"));
      unfolding::writeBinningArrays(fConst);
      fConst.Close();
    }

    // Store reference MC arrays in a file
    TFile fRef(refFileName, "recreate" );
    (*yieldsIni).Write(iniYieldsName);
    (*yieldsFin).Write(finYieldsName);
    (*yieldsIniArr).Write(iniYieldsName + TString("FIArray"));
    (*yieldsFinArr).Write(finYieldsName + TString("FIArray"));
    unfolding::writeBinningArrays(fRef);
    fRef.Close();
  }

  // ------------------------------------------

  int loadFromFile(const TString &fileName, const TString &refFileName) {
    std::cout << "UnfoldingMatrix_t::loadFromFile(\n  <" << fileName << ">\n  <" << refFileName << ">) for name=" << this->name << "\n";
    if (kind!=_cFSR_DETcorrFactors) {
      TFile fConst(fileName);
      if (!fConst.IsOpen()) {
	std::cout << "failed to open the file <" << fileName << ">\n";
	return 0;
      }
      if (!unfolding::checkBinningArrays(fConst)) {
	fConst.Close();
	return 0;
      }
      (*DetMigration)            .Read("DetMigration");
      (*DetMigrationErr)         .Read("DetMigrationErr");
      (*DetResponse)             .Read("DetResponse");
      (*DetResponseErrPos)       .Read("DetResponseErrPos");
      (*DetResponseErrNeg)       .Read("DetResponseErrNeg");
      (*DetInvertedResponse)     .Read("DetInvertedResponse");
      (*DetInvertedResponseErr)  .Read("DetInvertedResponseErr");
      (*DetResponseArr)          .Read("DetResponseFIArray");
      (*DetInvertedResponseArr)  .Read("DetInvertedResponseFIArray");
      (*DetInvertedResponseErrArr).Read("DetInvertedResponseErrFIArray");
      (*yieldsIni).Read(iniYieldsName);
      (*yieldsFin).Read(finYieldsName);
      (*yieldsIniArr).Read(iniYieldsName + TString("FIArray"));
      (*yieldsFinArr).Read(finYieldsName + TString("FIArray"));
      fConst.Close();
    }

    if (kind==_cFSR_DETcorrFactors) {
      // Retrieve reference MC arrays in a file
      TFile fRef(refFileName);
      if (!fRef.IsOpen()) {
	std::cout << "failed to open the file <" << refFileName << ">\n";
	return 0;
      }
      if (!unfolding::checkBinningArrays(fRef)) {
	fRef.Close();
	return 0;
      }
      (*yieldsIni).Read(iniYieldsName);
      (*yieldsFin).Read(finYieldsName);
      (*yieldsIniArr).Read(iniYieldsName + TString("FIArray"));
      (*yieldsFinArr).Read(finYieldsName + TString("FIArray"));
      fRef.Close();
    }
    return 1;
  }

  // ------------------------------------------

  void printYields() const {
    std::cout << "Yields of matrix=" << name << " (" 
	      << iniYieldsName << " and " << finYieldsName << ")\n";
    for (int ir=0; ir<(*yieldsIni).GetNrows(); ++ir) {
      std::cout << "ir=" << ir << "\n";
      for (int ic=0; ic<(*yieldsIni).GetNcols(); ++ic) {
	printf(" % 9.6lf  % 9.6lf\n",(*yieldsIni)[ir][ic],(*yieldsFin)[ir][ic]);
      }
      printf("\n");
    }
  }

  void printMigration() const {
    std::cout << "DetMigration of <" << name << ">:\n";
    int printSystErr=0;
    TMatrixD zeroErr=*DetMigrationErr;
    zeroErr=0;
    printCSMatrixValues("DetMigration",*DetMigration,*DetMigrationErr,zeroErr,printSystErr);
  }

  void printResponse() const {
    std::cout << "DetResponse,ErrPos,ErrNeg of <" << name << ">:\n";
    int printSystErr=1;
    printCSMatrixValues("DetResponse",*DetResponse,*DetResponseErrPos,*DetResponseErrNeg,printSystErr);
  }

  void printInvResponse() const {
    std::cout << "DetInvertedResponse of <" << name << ">:\n";
    int printSystErr=0;
    TMatrixD zeroErr=*DetInvertedResponseErr;
    zeroErr=0;
    printCSMatrixValues("DetInvertedResponse",*DetInvertedResponse,*DetInvertedResponseErr,zeroErr,printSystErr);
  }

  void printMatrices() const {
    std::string line(80,'-');
    std::cout << "\n" << line << "\n";
    printMigration();
    printResponse();
    printInvResponse();
    std::cout << line << "\n";
  }

  void printConditionNumber() const {
    //matrix condition number
    TDecompLU lu(*DetResponse);
    double condLU=lu.Condition();
    std::cout << "Matrix=" << name << "\n";
    std::cout << " condition number from TDecompLU condLU= " << condLU << std::endl;
    std::cout << " condition number ||DetResponse||*||DetResponseInv||=" << DetResponse->Norm1()*DetInvertedResponse->Norm1() << std::endl;
    std::cout << " chk ROOT bug: -condLU*||DetResponse||=" << (-condLU*DetResponse->Norm1()) << "\n" << std::endl;
  }

  void prepareHResponse(TH2F **hResponse_out=NULL,
			TH2F **hInvResponse_out=NULL,
			TCanvas **canv=NULL,
			CPlot **plotResponse_out=NULL,
			CPlot **plotInvResponse_out=NULL
			) {
    // Plot response and inverted response matrices
    TString kName=this->ourKindName();
    TH2F *hResponse = new TH2F(TString("hResponse_") + kName,"",
			       nUnfoldingBins, -0.5, nUnfoldingBins-0.5,
			       nUnfoldingBins, -0.5, nUnfoldingBins-0.5);
    TH2F *hInvResponse = new TH2F(TString("hInvResponse") + kName,"",
				  nUnfoldingBins, -0.5, nUnfoldingBins-0.5,
				  nUnfoldingBins, -0.5, nUnfoldingBins-0.5);
    hResponse->SetDirectory(0);
    hInvResponse->SetDirectory(0);
    for(int i=0; i<(*DetResponse).GetNrows(); i++){
      for(int j=0; j<(*DetResponse).GetNcols(); j++){
	hResponse->SetBinContent(i,j, (*DetResponse)(i,j));
	hInvResponse->SetBinContent(i,j, (*DetInvertedResponse)(i,j));
      }
    }
    hResponse->GetYaxis()->SetTitleOffset(1.1);
    hInvResponse->GetYaxis()->SetTitleOffset(1.1);


    TString canvName=TString("canvResponse") + kName;
    TCanvas *e1 = MakeCanvas(canvName,canvName,1200,600);
    e1->Divide(2,1);
    AdjustFor2DplotWithHeight(e1);
    CPlot *plotResponse=
      new CPlot(TString("response") + kName,"",
		"flat index gen",
		"flat index reco");
    plotResponse->AddHist2D(hResponse,"COLZ");
    plotResponse->Draw(e1,false,"png",1);

    CPlot *plotInvResponse=
      new CPlot(TString("invResponse") + kName,"",
		"flat index reco",
		"flat index gen");
    plotInvResponse->AddHist2D(hInvResponse,"COLZ");
    plotInvResponse->Draw(e1,false,"png",2);
    e1->Update();
    SaveCanvas(e1,Form("hResponse_%s_",DYTools::analysisTag.Data()) + kName);
  
    if (hResponse_out) *hResponse_out=hResponse;
    if (hInvResponse_out) *hInvResponse_out=hInvResponse;
    if (canv) *canv=e1;
    if (plotResponse_out) *plotResponse_out=plotResponse;
    if (plotInvResponse_out) *plotInvResponse_out=plotInvResponse;
  }

  // ------------------------------------------------------

  TMatrixD* getReconstructionEffect(const UnfoldingMatrix_t &inexact) const {
    TMatrixD *res=new TMatrixD(*yieldsIni);
    *res=0;
    for(int i=0; i < res->GetNrows(); i++){
      for(int j=0; j < res->GetNcols(); j++){
	double nexact = (*yieldsIni)(i,j);
	double nactual = (*inexact.yieldsIni)(i,j);
	if( nexact != 0 )
	  (*res)(i,j) = (nexact-nactual)/nexact;
      }
    }
    return res;
  }

  // ------------------------------------------------------

  // ------------------------------------------------------

};

//=== MAIN MACRO =================================================================================================

void makeUnfoldingMatrixFsr(const TString input, 
			 const TString triggerSetString="Full2011DatasetTriggers",
			 int systematicsMode = DYTools::NORMAL, 
			 int randomSeed = 1, double reweightFsr = 1.0, 
			 double massLimit = -1.0, int performPUReweight=0,
			 int debugMode=0)
//systematicsMode 0 (NORMAL) - no systematic calc
//1 (RESOLUTION_STUDY) - systematic due to smearing, 2 (FSR_STUDY) - systematics due to FSR, reweighting
//check mass spectra with reweightFsr = 0.95; 1.00; 1.05  
//mass value until which do reweighting
{

  // check whether it is a calculation
  if (input.Contains("_DebugRun_")) {
    std::cout << "plotDYUnfoldingMatrix: _DebugRun_ detected. Terminating the script\n";
    return;
  }

  // normal calculation
  gBenchmark->Start("makeUnfoldingMatrix");

  if (systematicsMode==DYTools::NORMAL)
    std::cout<<"Running script in the NORMAL mode"<<std::endl;
  else if (systematicsMode==DYTools::RESOLUTION_STUDY)
    std::cout<<"Running script in the RESOLUTION_STUDY mode"<<std::endl;
  else if (systematicsMode==DYTools::FSR_STUDY)
    std::cout<<"Running script in the FSR_STUDY mode"<<std::endl;
  else if (systematicsMode==DYTools::ESCALE_RESIDUAL)
    std::cout << "Running script in the ESCALE_RESIDUAL mode\n";
  else { 
    std::cout<<"requested mode not recognized"<<std::endl;
    assert(0);
  }

  if (debugMode==1) std::cout << "\n\n\tDEBUG MODE is ON\n\n";
  else if (debugMode==-1) std::cout << "\n\n\tLOADING MODE is ON\n\n";
  
  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //==============================================================================================================
  
//   Bool_t doSave  = false;    // save plots?
  TString format = "png";   // output file format

  vector<TString> fnamev;   // file names   
  vector<TString> labelv;   // legend label
  vector<Int_t>   colorv;   // color in plots
  vector<Int_t>   linev;    // line style
  vector<Double_t> xsecv;
  vector<Double_t> lumiv;
  TString          dirTag;
  TString          escaleTag; // Energy scale calibrations tag

  if (1) {
    MCInputFileMgr_t mcInp; // avoid errors from empty lines
    if (!mcInp.Load(input)) {
      std::cout << "Failed to load mc input file <" << input << ">\n";
      return;
    }
    fnamev=mcInp.fileNames();
    labelv=mcInp.labels();
    colorv=mcInp.colors();
    linev=mcInp.lineStyles();
    xsecv=mcInp.xsecs();
    lumiv=mcInp.lumis();
    dirTag=mcInp.dirTag();
    escaleTag=mcInp.escaleTag();
  }
  else {
  ifstream ifs;
  ifs.open(input.Data());
  assert(ifs.is_open());
  string line;
  Int_t state=0;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    if(state == 0){
      dirTag = TString(line);
      getline(ifs,line);
      stringstream ss3(line); ss3 >> escaleTag;
      state++;
      continue;
    }else{
      string fname;
      Int_t color, linesty;
      stringstream ss(line);
      Double_t xsec;
      ss >> fname >> xsec >> color >> linesty;
      string label = line.substr(line.find('@')+1);
      fnamev.push_back(fname);
      labelv.push_back(label);
      colorv.push_back(color);
      linev.push_back(linesty);
      xsecv.push_back(xsec);
      lumiv.push_back(0);
    }
  }
  ifs.close();
  }
  
  // 
  // Set up energy scale corrections
  //
  ElectronEnergyScale escale(escaleTag);
  escale.print();

  if( !escale.isInitialized()) {
    printf("Failed to match escale calibration. Tag: >>%s<<\n", escaleTag.Data());
    assert(0);
  }

  TriggerConstantSet constantsSet=DetermineTriggerSet(triggerSetString);  
  assert ( constantsSet != TrigSet_UNDEFINED );

  // For MC the trigger does not depend on run number
  const bool isData=kFALSE;
  TriggerSelection requiredTriggers(constantsSet, isData, 0);

  PUReweight_t puWeight;
  // For Hildreth method of PU reweighting, the lines below are not needed
//   if (performPUReweight) {
//     assert(puWeight.setDefaultFile(dirTag,DYTools::analysisTag_USER, 0));
//     assert(puWeight.setReference("hNGoodPV_data"));
//     assert(puWeight.setActiveSample("hNGoodPV_zee"));
//   }

  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //==============================================================================================================

  //for the FSR case
  const bool useFewzWeights = true;
  const bool cutZPT100 = true;
  FEWZ_t fewz(useFewzWeights,cutZPT100);
  if (useFewzWeights && !fewz.isInitialized()) {
    std::cout << "failed to prepare FEWZ correction\n";
    throw 2;
  }


  TRandom random;
  // The random seeds are needed only if we are running this script in systematics mode
  int seed = randomSeed;
  random.SetSeed(seed);
  gRandom->SetSeed(seed);
  if(systematicsMode==DYTools::RESOLUTION_STUDY) {
    escale.randomizeSmearingWidth(seed);
  }

  // prepare tools for ESCALE_RESIDUAL
  TMatrixD *shapeWeights=NULL;
  if (systematicsMode==DYTools::ESCALE_RESIDUAL) {
    TString shapeFName=TString("../root_files/yields/") + dirTag + 
      TString("/yields_bg-subtracted") + DYTools::analysisTag + TString(".root");
    std::cout << "Obtaining shape weights from <" << shapeFName << ">\n";
    TFile fshape(shapeFName);
    if (!fshape.IsOpen()) {
      std::cout << "failed to open a file <" << shapeFName << ">\n";
      throw 2;
    }
    shapeWeights = (TMatrixD*)fshape.Get("ZeeMCShapeReweight");
    if (!shapeWeights) {
      std::cout << "failed to find object \"ZeeMCShapeReweight\"\n";
      throw 2;
    }
    dirTag += TString("_escale_residual");
    std::cout << "changing dirTag to <" << dirTag << ">\n";
    (*shapeWeights)(0,0)=1; (*shapeWeights)(1,0)=1; (*shapeWeights)(2,0)=1;
    std::cout << "shapeWeights:\n"; shapeWeights->Print(); // return;
  }

  //  
  // Set up histograms
  //
  vector<TH1F*> hZMassv;//, hZMass2v, hZPtv, hZPt2v, hZyv, hZPhiv;  
  
  char hname[100];
  eventCounter_t totEC;

  for(UInt_t ifile = 0; ifile<fnamev.size(); ifile++) {
    sprintf(hname,"hZMass_%i",ifile); hZMassv.push_back(new TH1F(hname,"",500,0,2000)); hZMassv[ifile]->Sumw2();
  }

  //1D set up 
  const int nbin = 41;
  double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
            96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000};
 
  double Mass_xbin2[7] = {20, 30, 45, 60, 120, 200, 1500};
 
  //pT distributions for pre-FSR post-FSR and dressed leptons
  TH1F *hpT_preFSR = new TH1F("hpT_preFSR","",700,0,700);
  TH1F *hpT_postFSR = new TH1F("hpT_postFSR","",700,0,700);
  TH1F *hpT_dressed = new TH1F("hpT_dressed","",700,0,700);

  //energy distributions
  TH1F *hGENphoton_energy = new TH1F("GENphoton_energy","",700,0,700);
  TH1F *hRECOphoton_energy = new TH1F("RECOphoton_energy","",700,0,700);  

  TH1F *hFSRdeltaR   = new TH1F("hFSRdeltaR","", 240, 0, 4.*3.142);
  TH1F *hFSRmultiplicity   = new TH1F("hFSRmultiplicity","", 50, 0, 50);

  TH1F *hnonFSRdeltaR   = new TH1F("hnonFSRdeltaR","", 240, 0, 4.*3.142);
  TH1F *hnonFSRmultiplicity   = new TH1F("hnonFSRmultiplicity","", 50, 0, 50);


  UnfoldingMatrix_t detResponse(UnfoldingMatrix_t::_cDET_Response,"detResponse");
  UnfoldingMatrix_t detResponseExact(UnfoldingMatrix_t::_cDET_Response,"detResponseExact");
  UnfoldingMatrix_t fsrGood(UnfoldingMatrix_t::_cFSR, "fsrGood");
  UnfoldingMatrix_t fsrExact(UnfoldingMatrix_t::_cFSR, "fsrExact");
  UnfoldingMatrix_t fsrDET(UnfoldingMatrix_t::_cFSR_DET,"fsrDET"); // only relevant indices are checked for ini,fin
  UnfoldingMatrix_t fsrDETexact(UnfoldingMatrix_t::_cFSR_DET,"fsrDETexact"); // all indices are checked

  // if computeResponseMatrix_MdfBeforeNormalization is called, then the modification is done to the migration matrix
  // if computeResponseMatrix_Mdf is called, then the modification is done to the response matrix, and invResponse is obtained from simple inversion
  UnfoldingMatrix_t fsrDET_Mdf(UnfoldingMatrix_t::_cFSR_DET,"fsrDET_Mdf"); 
  
  // a good working version: response matrix and invResponse are modified after the inversion
  UnfoldingMatrix_t fsrDET_good(UnfoldingMatrix_t::_cFSR_DET,"fsrDET_good"); 

  //
  // Access samples and fill histograms
  //  
  TFile *infile=0;
  TTree *eventTree=0;  
    
  // Data structures to store info from TTrees
  mithep::TEventInfo    *info = new mithep::TEventInfo();
  TClonesArray *dielectronArr = new TClonesArray("mithep::TDielectron");
  TClonesArray *photonArr = new TClonesArray("mithep::TGenPhoton"); 
 
  // loop over samples  
  if (debugMode!=-1) {
  for(UInt_t ifile=0; ifile<fnamev.size(); ifile++) {
    eventCounter_t ec;
  
    // Read input file
    cout << "Processing " << fnamev[ifile] << "..." << endl;
    infile = new TFile(fnamev[ifile]); 
    assert(infile);

    // Get the TTrees
    eventTree = (TTree*)infile->Get("Events"); assert(eventTree);

    // Find weight for events for this file
    // The first file in the list comes with weight 1,
    // all subsequent ones are normalized to xsection and luminosity
    double xsec=xsecv[ifile];
    AdjustXSectionForSkim(infile,xsec,eventTree->GetEntries(),1);
    lumiv[ifile] = eventTree->GetEntries()/xsec;
    double extraScale=1.; // 4839*1666/27166257.; // MC Zee scale in selectEvents
    double scale = extraScale*lumiv[0]/lumiv[ifile];
    cout << "       -> sample weight is " << scale << endl;

    // Set branch address to structures that will store the info  
    eventTree->SetBranchAddress("Info",&info);                TBranch *infoBr       = eventTree->GetBranch("Info");
    eventTree->SetBranchAddress("Dielectron",&dielectronArr); TBranch *dielectronBr = eventTree->GetBranch("Dielectron");
    eventTree->SetBranchAddress("GenPhoton",&photonArr); TBranch *photonBr = eventTree->GetBranch("GenPhoton");  

    // loop over events    
    for(UInt_t ientry=0; ientry<eventTree->GetEntries(); ientry++) {
      if (debugMode && (ientry>1000000)) break;
      if (ientry%1000000==0) { printProgress("ientry=",ientry,eventTree->GetEntriesFast()); }
      if (ientry%100000==0) { printProgress("ientry=",ientry,eventTree->GetEntriesFast()); }
      ec.numEvents++;

      infoBr->GetEntry(ientry);

      //only for RECO
      std::vector<int> scIDlist;
      //cout << "Number of entries " << photonArr->GetEntriesFast() << " " << dielectronArr->GetEntriesFast() << endl;
      for(Int_t i = 0; i < dielectronArr->GetEntriesFast(); i++) {
        const mithep::TDielectron *dielectron = (mithep::TDielectron*)((*dielectronArr)[i]);
        scIDlist.push_back(dielectron->scID_1);
        scIDlist.push_back(dielectron->scID_2);
      }

      dielectronArr->Clear();
      dielectronBr->GetEntry(ientry);

      //cout << "Number of entries " << photonArr->GetEntriesFast() << " " << dielectronArr->GetEntriesFast() << endl;
      for(Int_t i = 0; i < dielectronArr->GetEntriesFast(); i++) {
        const mithep::TDielectron *dielectron = (mithep::TDielectron*)((*dielectronArr)[i]);

        if (dielectron->pt_1 < 20) continue;
        if (dielectron->pt_2 < 20) continue;

        // Apply selection
        // Eta cuts
        if( ! DYTools::goodEtaPair(dielectron->scEta_1, dielectron->scEta_2 ) ) continue;
        if( ! requiredTriggers.matchTwoTriggerObjectsAnyOrder( dielectron->hltMatchBits_1,
                                                               dielectron->hltMatchBits_2,
                                                               info->runNum) ) continue;
        if(!passEGMID2012(dielectron, WP_MEDIUM, info->rhoLowEta)) continue;


        //FIXME dressed photon study
        TLorentzVector coneSumPhoton1;
        coneSumPhoton1.SetPtEtaPhiM(0,0,0,0);
        TLorentzVector coneSumPhoton2;
        coneSumPhoton2.SetPtEtaPhiM(0,0,0,0);
  
        //get two GEN post FSR 4 vectors 
        TLorentzVector dressedEle1; 
        dressedEle1.SetPtEtaPhiM(dielectron->pt_1,dielectron->eta_1,dielectron->phi_1,0.000511);
        TLorentzVector dressedEle2;
        dressedEle2.SetPtEtaPhiM(dielectron->pt_2,dielectron->eta_2,dielectron->phi_2,0.000511);

        photonArr->Clear();
        photonBr->GetEntry(ientry);

        int nFSRPhotonPerLepton = 0;
        int nPhotonPerLepton = 0;

        //FIXME this is for the case of MC
        for(Int_t i = 0; i < photonArr->GetEntriesFast(); i++) {

          const mithep::TGenPhoton *photon = (mithep::TGenPhoton*)((*photonArr)[i]);

          TLorentzVector thisConePhoton;
          thisConePhoton.SetPtEtaPhiM(photon->pt,photon->eta,photon->phi,0);

          //select muon mothers only
          //if (find(scIDlist.begin(),scIDlist.end(),photon->scID) != scIDlist.end()) continue;
          ////FIXME new photon ID requirements
          //if (photon->passConvId == 1) continue; 
          //if (photon->trkIso04+photon->emIso04+photon->hadIso04>6) continue; 
          //if (fabs(photon->parentId) != 13) continue; 
          //if (fabs(photon->parentId) != 11) continue;

          //if (photon->pt > 10) continue;
          //if (photon->pt < 10 || photon->pt > 50) continue;
          //if (photon->pt < 50) continue;      

          //cout << "Photon " << i+1 << " of " << photonArr->GetEntriesFast() << " having pT " << photon->pt << " reference GEN photon " << gen->phopt << endl; 
      
          //check the deltaR
          if (sqrt( pow(dressedEle1.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle1.Phi() - thisConePhoton.Phi(), 2) ) < 0.1) { //0.1) { 
            //cout << "Case1 adding photon " << thisConePhoton.X()  << " " << thisConePhoton.Y()  << " " << thisConePhoton.Z()  << " " << endl; 
            coneSumPhoton1 += thisConePhoton;
          //if (fabs(photon->parentId) == 11) {
            if (fabs(photon->parentId) == 11) nFSRPhotonPerLepton++;
            else nPhotonPerLepton++;

          //} else {
          //  nPhotonPerLepton++;
          //}

            hGENphoton_energy->Fill(photon->pt);
            //hRECOphoton_energy->Fille();
          } else if (sqrt( pow(dressedEle2.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle2.Phi() - thisConePhoton.Phi(), 2) ) < 0.1) { //0.1) {
            //cout << "Case2 adding photon " << thisConePhoton.X()  << " " << thisConePhoton.Y()  << " " << thisConePhoton.Z()  << " " << endl;
            coneSumPhoton2 += thisConePhoton;
          //if (fabs(photon->parentId) == 11) {
            if (fabs(photon->parentId) == 11) nFSRPhotonPerLepton++;
            else nPhotonPerLepton++;
          //} else {
          //  nPhotonPerLepton++;
          //}

            hGENphoton_energy->Fill(photon->pt);
            //hRECOphoton_energy->Fille();
          }

          //calculate rapidity
          //if (fabs(photon->parentId) == 11) {
            hFSRdeltaR->Fill(sqrt( pow(dressedEle1.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle1.Phi() - thisConePhoton.Phi(), 2) ));
            hFSRdeltaR->Fill(sqrt( pow(dressedEle2.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle2.Phi() - thisConePhoton.Phi(), 2) ));
          //} else {
          //  hnonFSRdeltaR->Fill(sqrt( pow(dressedEle1.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle1.Phi() - thisConePhoton.Phi(), 2) ));
          //  hnonFSRdeltaR->Fill(sqrt( pow(dressedEle2.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle2.Phi() - thisConePhoton.Phi(), 2) ));
          //}
        } 
/*
        int nPhotonPerLepton = 0;
        for(Int_t i = 0; i < photonArr->GetEntriesFast(); i++) {
          const mithep::TPhoton *photon = (mithep::TPhoton*)((*photonArr)[i]);

          TLorentzVector thisConePhoton;
          thisConePhoton.SetPtEtaPhiM(photon->pt,photon->eta,photon->phi,0);

          //select muon mothers only
          //if (photon->scID == dielectron->scID) continue; //this is dielectron
          if (find(scIDlist.begin(),scIDlist.end(),photon->scID) != scIDlist.end()) continue;
          //if (photon->pt > 5) continue;

          //cout << "XX supercluster ID " << photon->scID << " " << dielectron->scID << endl;
          //cout << "Photon " << i+1 << " of " << photonArr->GetEntriesFast() << " having pT " << photon->pt << " scID " << photon->scID << " reference Electron  " << dielectron->pt << " scID " << dielectron->scID << endl; 

          //check the deltaR
          if (sqrt( pow(dressedEle1.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle1.Phi() - thisConePhoton.Phi(), 2) ) < 0.1) { //0.1) { 
              coneSumPhoton1 += thisConePhoton;  
              nPhotonPerLepton++;
          } else if (sqrt( pow(dressedEle2.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle2.Phi() - thisConePhoton.Phi(), 2) ) < 0.1) {
              coneSumPhoton2 += thisConePhoton;
              nPhotonPerLepton++;
          }
          //calculate rapidity
          hFSRdeltaR->Fill(sqrt( pow(dressedEle1.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle1.Phi() - thisConePhoton.Phi(), 2) ));
          hFSRdeltaR->Fill(sqrt( pow(dressedEle2.PseudoRapidity() - thisConePhoton.PseudoRapidity(), 2) + pow(dressedEle2.Phi() - thisConePhoton.Phi(), 2) ));
        }
*/
        hFSRmultiplicity->Fill(nFSRPhotonPerLepton/2);
        hnonFSRmultiplicity->Fill(nPhotonPerLepton/2);
        //My new leptons
        dressedEle1 += coneSumPhoton1;
        dressedEle2 += coneSumPhoton2;
        hpT_dressed->Fill(dressedEle1.Pt());
        hpT_dressed->Fill(dressedEle2.Pt());

      }//end dielectron loop for event
      //hpT_postFSR->Fill(gen->pt_1);
      //hpT_postFSR->Fill(gen->pt_2);
    } // end loop over events 
    delete infile;
    infile=0, eventTree=0;
    std::cout << ec << "\n";
    totEC.add(ec);
  } // end loop over files
  std::cout << "total counts : " << totEC << "\n";

  TFile* ff = new TFile("testFSR.root","recreate"); 
  hGENphoton_energy->Write("GENphoton_energy");
  hFSRdeltaR->Write("hFSRdeltaR");
  hFSRmultiplicity->Write("hFSRmultiplicity");
  hnonFSRdeltaR->Write("hdeltaR");
  hnonFSRmultiplicity->Write("hmultiplicity");
  //hRECOphoton_energy->Fille();
  //hpT_dressed->Write("hpT_dressed");
  //hpT_postFSR->Write("hpT_postFSR");
  ff->Close();

  } 

}


//=== FUNCTION DEFINITIONS ======================================================================================
void computeNormalizedBinContent(double subset, double subsetErr,
				 double total, double totalErr,
				 double& ratio, double& ratioErr){
  
  if(total == 0) {
    printf("makeUnfoldingMatrix::Possible problem\n");
    printf("     empty column in the response matrix\n");
    return;
  }
  
  ratio = subset/total;

  // The formula for the ratio = subset/total is obtained by error
  // propagation. The subsetErr and totalErr are NOT assumed ot be
  // the sqrt(subset) and sqrt(total). (If one assume that, the formula
  // below reduces to the familiar sqrt(ratio*(1-ratio)/total) ).
  // The subset and subsetErr are part of the total and totalErr.
  // The formula is easiest to derive if we take "A +- dA" and
  // "B +- dB" as independent numbers, with total = A+B and
  // totalErr^2 = dA^2 + dB^2. One then does error propagation of
  // the expression ratio = A/(A+B).
  // The outcome of it is found below (the absolute error on the ratio)
  ratioErr = (1/total)*sqrt( subsetErr*subsetErr*(1-2*ratio)
			     + totalErr*totalErr*ratio*ratio );

  return;
}

void calculateInvertedMatrixErrors(const TMatrixD &T, 
	  const TMatrixD &TErrPos, const TMatrixD &TErrNeg,
				   TMatrixD &TinvErr){

  // Calculate errors on the inverted matrix by the Monte Carlo
  // method

  Double_t det;
  int nRow = T.GetNrows();
  int nCol = T.GetNcols();
  TMatrixD TinvSum(nRow,nCol);
  TMatrixD TinvSumSquares(nRow,nCol);

  // Reset Matrix where we will be accumulating RMS/sigma:
  TinvSum        = 0;
  TinvSumSquares = 0;
  TinvErr        = 0;

  // Do many tries, accumulate RMS
  int N = 10000;
  for(int iTry = 0; iTry<N; iTry++){
    // Find the smeared matrix
    TMatrixD Tsmeared = T;
    for(int i = 0; i<nRow; i++){
      for(int j = 0; j<nCol; j++){
	double central = T(i,j);
	double sigPos = TErrPos(i,j);
	double sigNeg = TErrNeg(i,j);
 	// Switch to symmetric errors: approximation, but much simpler
	double sig = (sigPos+sigNeg)/2.0;
	Tsmeared(i,j) = gRandom->Gaus(central,sig);
      }
    }
    // Find the inverted to smeared matrix
    TMatrixD TinvSmeared = Tsmeared;
    TinvSmeared.Invert(&det);
    // Accumulate sum and sum of squares for each element
    for(int i2 = 0; i2<nRow; i2++){
      for(int j2 = 0; j2<nCol; j2++){
	TinvSum       (i2,j2) += TinvSmeared(i2,j2);
	TinvSumSquares(i2,j2) += TinvSmeared(i2,j2)*TinvSmeared(i2,j2);
      }
    }
  }

  // Calculate the error matrix
  TMatrixD TinvAverage = TinvSum;
  for(int i = 0; i<nRow; i++){
    for(int j = 0; j<nCol; j++){
      TinvErr(i,j) = sqrt( TinvSumSquares(i,j)/double(N) 
			   - (TinvSum(i,j)/double(N))*(TinvSum(i,j)/double(N)) );
      TinvAverage(i,j) = TinvSum(i,j)/double(N);
    }
  }

  return;
}
