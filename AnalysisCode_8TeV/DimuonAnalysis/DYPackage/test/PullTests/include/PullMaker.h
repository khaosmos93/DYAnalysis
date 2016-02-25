#ifndef __PullMaker_h_
#define __PullMaker_h_

#include "TF1.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "TH2F.h"
#include <vector>
#include <algorithm>

#include "include/Unfolder.h"
#include "TStopwatch.h"

class PullMaker{

  private:


	TRandom3* _randgen;

	int _numEnsembles;
	int _numPseudoExp;

	
	TF1* _genFunction;
	TF1* _smearingFunction;

	TH1F* _genHistRef;
	TH1F* _measHistRef;

	TH1F* _genHist;
	TH1F* _measHist;

	TH2F* _migMatrixRef;
	TH2F* _inResponseMatrix;

	Unfolder* _unfolder;

	TH1F* _hPullSingle;
	TH2F* _hPullAllUnfold;


//	void initSmearTable();


	void killErrors(TH1F* hist);
  public:

	PullMaker();
//	PullMaker(int const numEnsembles, int const numPseudos, TH1F* inhist);
	PullMaker(int const numEnsembles, int const numPseudos, TH1F* inhist, TH2F* inMigMat);
	PullMaker(int const numEnsembles, int const numPseudos, TH1F* inhist, TH2F* inMigMat, TH2F* inResponseMatrix);




	void setSeed(int const rndmSeed) { _randgen->SetSeed(rndmSeed);}
	void setNumPseudoExp(int const val) { _numPseudoExp = val;}
	void setNumEnsembles(int const val) { _numEnsembles = val;}
	

	int getNumPseudoExp() const { return _numPseudoExp;}
	int getNumEnsembles() const { return _numEnsembles;} 


	void setGenerationFunction(TF1 const* val) {  _genFunction = (TF1*)val;}
	void setSmearingFunction(TF1 const* val) {  	_smearingFunction = (TF1*)val;}


	TH1F* getGenRefHist() const{ return _genHistRef;}

	TH1F* getTrueHist() const { return _genHist;}
	TH1F* getMeasHist() const { return _measHist;}


	void generateEnsemble();

	void unfoldEnsemble();

	void unfoldEnsembles();


};

PullMaker::PullMaker(){
		_randgen = new TRandom3(42);
		_numEnsembles = int(0);
		_numPseudoExp = int(0);

		_genHist 			= 0;
		_genHistRef			= 0;
		_smearingFunction  	= 0;
		_measHist 			= 0;
		_migMatrixRef = 0;
		_inResponseMatrix = 0;
	}

PullMaker::PullMaker(int const numEnsembles, int const numPseudos, TH1F* inhist, TH2F* inMigMat){
		_randgen = new TRandom3(42);
		_numEnsembles = int(numEnsembles);
		_numPseudoExp = int(numPseudos);

		_genHistRef = (TH1F*)inhist->Clone("genHistRef_pullMaker");
		_genHist	= (TH1F*)inhist->Clone("genHist_pullMaker");
		_measHist	= (TH1F*)inhist->Clone("measHist_pullMaker");
		
		_genHistRef->SetTitle("true reference histogram");
		_genHist->SetTitle("true generated");
		_measHist->SetTitle("measured");

		_genHist->SetLineColor(kRed);
		_measHist->SetLineColor(kBlue);


		_genHist->Reset();
		_measHist->Reset();
	
		_smearingFunction  	= 0;

		_migMatrixRef = (TH2F*)inMigMat->Clone("migMatrixRef_pullMaker");
		_inResponseMatrix	= 0;
//		initSmearTable();

}
//
// the great and mighty constructor
//
PullMaker::PullMaker(int const numEnsembles, int const numPseudos, TH1F* inhist, TH2F* inMigMat, TH2F* inResponseMatrix){
	
	_randgen = new TRandom3(42);
	_numEnsembles = int(numEnsembles);
	_numPseudoExp = int(numPseudos);

	_migMatrixRef = (TH2F*)inMigMat->Clone("migMatrixRef_pullMaker");
	_inResponseMatrix	= (TH2F*)inResponseMatrix->Clone("inResponseMatrix_pullMaker");

	_genHistRef = (TH1F*)inhist->Clone("genHistRef_pullMaker");
	_measHistRef	= (TH1F*)_migMatrixRef->ProjectionY()->Clone("measHistRef_pullMaker");
	_genHist	= (TH1F*)inhist->Clone("genHist_pullMaker");
	_measHist	= (TH1F*)inhist->Clone("measHist_pullMaker");
//	_genHist	= (TH1F*)_migMatrixRef->ProjectionX()->Clone("genHist_pullMaker");
//	_measHist	= (TH1F*)_migMatrixRef->ProjectionY()->Clone("measHist_pullMaker");
		
	_genHistRef->SetTitle("true reference histogram");
	_genHist->SetTitle("true generated");
	_measHist->SetTitle("measured");

	_genHist->SetLineColor(kRed);
	_measHist->SetLineColor(kBlue);

	_genHist->Reset();
	_measHist->Reset();
	
	_smearingFunction  	= 0;

//
// build the unfolder
//	
	_unfolder = new Unfolder("pullMakerUnfolder");
	_unfolder->setTrue(_genHistRef);
	_unfolder->setMeas(_measHistRef);
	_unfolder->setMigrationMatrix(_inResponseMatrix);

//	_unfolder->doUnfolding();
//	_unfolder->printUnfoldStats();

//
// initialize the pulls
//

	_hPullSingle = (TH1F*)inhist ->Clone("hPullSingle_pullMaker");
	_hPullSingle->Reset();


	_hPullAllUnfold = new TH2F("hPullAllUnfold_pullMaker","all pulls unfold",_genHist->GetNbinsX(),0,_genHist->GetNbinsX(),200, -10,10);
	_hPullAllUnfold->SetXTitle("bin of pull");
	_hPullAllUnfold->SetYTitle("(true - meas)/error");
		
	
	
	
}

//
//
//
void PullMaker::generateEnsemble(){



/*
//
// super slow 
// super slow 
// super slow 
//
//
	TStopwatch timer1;
	TStopwatch timer2;

	_genHist->Reset();
	_measHist->Reset();



	timer1.Start();
	for (int i = 0; i < _numPseudoExp; ++i) {

		double valtrue = _genHistRef->GetRandom();
		_genHist->Fill(valtrue);

		int refbin = _migMatrixRef->GetXaxis()->FindBin(valtrue);

		double valmeas = _migMatrixRef->ProjectionY("tmp",refbin,refbin)->GetRandom();
		while (valmeas < 0) valmeas = _migMatrixRef->ProjectionY("tmp",refbin,refbin)->GetRandom();


		_measHist->Fill(valmeas);
	}

	timer1.Stop();
*/


//
// 10x speed from above 
//
//
//	timer2.Start();

	_genHist->Reset();
	_measHist->Reset();
	std::vector<float> trueMasses; trueMasses.clear();
	for (int i = 0; i < _numPseudoExp; ++i) {

		double valtrue = _genHistRef->GetRandom();
		_genHist->Fill(valtrue);
		trueMasses.push_back(valtrue);
	}
	std::sort(trueMasses.begin(),trueMasses.end());


	int currBin = 0;
	TH1F* histMeas	= (TH1F*)_migMatrixRef->ProjectionY("tmp",currBin,currBin);//->GetRandom();
	for (std::vector<float>::const_iterator trueMass = trueMasses.begin(); trueMass !=trueMasses.end(); ++trueMass){

		int refbin = _migMatrixRef->GetXaxis()->FindBin(*trueMass);
		if (currBin != refbin || currBin ==0) {
			currBin = refbin;	
			histMeas->Delete();
			histMeas	= (TH1F*)_migMatrixRef->ProjectionY("tmp",currBin,currBin);//->GetRandom();
		}

		float valmeas = histMeas->GetRandom();
		while (valmeas < 0) valmeas = histMeas->GetRandom();

		_measHist->Fill(valmeas);
	}
	histMeas->Delete();

//	timer2.Stop();

//	timer1.Print();
//	timer2.Print();

}
//
// generate and unfold an ensemble assuming the migration matrix is the same
//
void PullMaker::unfoldEnsemble(){

	_hPullSingle->Reset();
	generateEnsemble();
	killErrors(_genHist);

	_unfolder->setTrue(_genHist);
	_unfolder->setMeas(_measHist);
	
	_unfolder->doUnfolding();
//	_unfolder->printUnfoldStats();

	_hPullSingle->Add(_genHist,1);		
	_hPullSingle->Add(_unfolder->getUnfolded(),-1);
//	_hPullSingle->Print("range");

	int nbinsx = _hPullSingle->GetNbinsX();
	for (int i = 1; i <= nbinsx; ++i){
		double val = _hPullSingle->GetBinContent(i);
		double err = _unfolder->getUnfolded()->GetBinError(i);
		_hPullSingle->SetBinContent(i,val/err);
		_hPullAllUnfold->Fill(_hPullAllUnfold->GetBinCenter(i),val/err);	
	}


//	_hPullSingle->Print("range");
}

//
// unfold many ensembles
//

void PullMaker::unfoldEnsembles(){

	for (int i = 1; i <= _numEnsembles; ++i) {	
//		if ((_numEnsembles%(0.1*i) ==0)) std::cout<<"ensemble: "<<i<<std::endl;
//		if (i%40 ==0) std::cout<<"ensemble: "<<i<<std::endl;
		if (i%100==0) std::cout<<i<<std::endl;
		unfoldEnsemble();
	}
}


//
// set all errors in a histogram to 0
//
void PullMaker::killErrors(TH1F* hist){

	int nbins = hist->GetNbinsX();
	
	for (int i = 0; i <= nbins+1; ++i){
		hist->SetBinError(i,0);
	}


}
#endif


//
//
//
/*
void PullMaker::initSmearTable(){
	
	_vecSmear.clear();

	int nbins = _migMatrixRef->GetNbinsX();


	for (int ibin = 1; ibin <= nbins; ++ibin){	
		
		TString sname = TString::Format("pullMaker_hrecoSmearHist%03d",ibin);	

		TH1F* htmp = (TH1F*)_migMatrixRef->ProjectionY(sname,ibin,ibin)->Clone(sname);
		std::cout<<htmp->GetEntries()<<std::endl;

//		_vecSmear.push_back(TH1F(*htmp));		
	}
	
}
*/



