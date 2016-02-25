#ifndef __Unfolder_h_
#define __Unfolder_h_
#include <iostream>
#include <vector>
#include <cmath>

#include "TH1.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"

class Unfolder{
 
  private:
	TString _sname;


	TMatrixD* _mMigrationMatrix;
	TMatrixD* _mMigrationMatrixInverse;

	TH2F* _hMigrationMatrix;

	TH1F* _htrue;
	TH1F* _hmeas;


	TH1F* _hunfold;
	TH1F* _hforefold;


  public:
	Unfolder() {}
	Unfolder(TString const sname);
	~Unfolder() {};

	void setMigrationMatrix(TH2F* hmigmat) { 
		_hMigrationMatrix = hmigmat;
		if (_mMigrationMatrix !=0 ) _mMigrationMatrix->Delete(); 
                cout << "hmigmat " << hmigmat->GetXaxis()->GetNbins() << endl;
		_mMigrationMatrix= histToMatrix(hmigmat);
//		_mMigrationMatrix->Transpose(*_mMigrationMatrix);
		_mMigrationMatrixInverse = (TMatrixD*)_mMigrationMatrix->Clone(TString::Format("%s_migMatInverse",_sname.Data())); 

                //TDecompSVD svd(*_mMigrationMatrixInverse);
                //TMatrixD htmp = svd.Invert();
                //for (int i = 0; i < 40; i++) {
                //   for (int j = 0; j < 40; j++) {
                //      _mMigrationMatrixInverse(i,j) = htmp(i,j);
                //    }
                // }

		_mMigrationMatrixInverse->Invert();
	}

	void setTrue(TH1F* hist){ _htrue = hist;}		
	void setMeas(TH1F* hist){ _hmeas = hist;}		
	
//	void unfold();//{};

	TH1F* getTrue() {return _htrue;}
	TH1F* getMeas() {return _hmeas;}
	TH2F* getMigMatHist() { return _hMigrationMatrix;}
	TMatrixD* getMigMat() { return _mMigrationMatrix;}
	TMatrixD* getMigMatInverse() { return _mMigrationMatrixInverse;}

	TMatrixD* histToMatrix(TH2 const* hist);	
        //TH2F* matrixToHist(TMatrixD* mat, TH2F* hprototype);

	void CRMultiply(TH1F* hout, TH1F const* hist, TMatrixD const* mat);

	void unfold();
	void forefold();

	void doUnfolding();
	TH1F* getUnfolded() {return _hunfold;}
	TH1F* getForefold() {return _hforefold;}

	void printUnfoldStats();



};


//
// constructors
// 
Unfolder::Unfolder(TString const sname){
		_sname=sname;
		_htrue = 0;
		_hmeas = 0;
		_hMigrationMatrix = 0;
		_mMigrationMatrix = 0;
		_mMigrationMatrixInverse = 0;
		_hunfold = 0;
		_hforefold = 0;
}	

//
// convert 2-d histogram to matrix
//
TMatrixD* Unfolder::histToMatrix(TH2 const* hist) {

	int n = hist -> GetNbinsX();

  	TMatrixD *retVal = new TMatrixD(n,n);

  	int theBin;

	for (int i = 0; i < n; i++) 
    	for (int j = 0; j < n; j++) {
			theBin = hist -> GetBin ( i+1, j+1 );
			double val = hist->GetBinContent(theBin);
      		(*retVal)(i,j) = val; 
	}
	return retVal;
}

//
// convert 2-d matrix to histogram
//
/*
TH2F* Unfolder::matrixToHist(TMatrixD* mat, TH2F* hprototype) {

        TH2F* retVal = (TH2F*)hprototype->Clone();

        for (int i = 0; i < retVal->GetXaxis()->GetNbins(); i++) {
           for (int j = 0; j < retVal->GetYaxis()->GetNbins(); j++) {
                retVal->SetBinContent(i+1, j+1,mat(i,j)); 
                retVal->SetBinError(i+1, j+1,0.0); //just to visualize, never used for calculation
           }
        }
        return retVal;
}
*/

//
//
//
void Unfolder::CRMultiply(TH1F* hout, TH1F const* hist, TMatrixD const* mat){

	int ncols= mat->GetNcols(); 
	int nrows = mat->GetNrows();

	for (int col = 0; col < ncols; ++col) {
		double sumVal =0.0;
		double sumErr =0.0;
		for (int row = 0; row < nrows; ++row){
			double matval = (*mat)(row,col);
			double matval1 = (*mat)(row,col);
			double vecval = hist->GetBinContent(row+1);
			double vecerr = hist->GetBinError(row+1);
			sumVal+=matval*vecval;
			sumErr+=matval1*matval1*vecerr*vecerr;
		}

		hout->SetBinContent(col+1,sumVal);
		hout->SetBinError(col+1,sqrt(sumErr));
	}
}
//
//
//
void Unfolder::doUnfolding() {

	this->unfold();
	this->forefold();

}

void Unfolder::unfold(){
	if (_hunfold ==0)	_hunfold = (TH1F*)	_htrue->Clone(TString::Format("%s_unfold",_sname.Data()));
	_hunfold->Clear();
	CRMultiply(_hunfold,_hmeas,_mMigrationMatrixInverse);

}
void Unfolder::forefold(){
	if (_hforefold==0)	_hforefold = (TH1F*)_htrue->Clone(TString::Format("%s_forefold",_sname.Data()));
	_hforefold->Clear();

	CRMultiply(_hforefold,_htrue,_mMigrationMatrix);
}

void Unfolder::printUnfoldStats() {


	printf ("\n----------------------------------------------\n");
	printf ("printing the unfolding statistics\n");
	printf ("----------------------------------------------\n");

	printf ("%-11s"
		,"bin width"
	);

	printf("%10s", "true");
	printf("\t%-20s", "unfolded");
	printf("\t%-20s", "measured");
	printf("\t%-20s", "forefolded");


	printf("\n");

	int nbinsx =_hmeas->GetNbinsX();

	for (int ibin = 1; ibin <= nbinsx; ++ibin){

		double masslow = _hmeas->GetBinLowEdge(ibin);
		double binwidth = _hmeas->GetBinWidth(ibin);


		printf (
			"[%04.0f,%04.0f]"
			,masslow,masslow+binwidth
		);
			
		printf("%10.0f",this->_htrue->GetBinContent(ibin));

		
		printf("\t%9.1f +/- %-7.1f"
		""
			,this->_hunfold->GetBinContent(ibin),
			this->_hunfold->GetBinError(ibin)
		);

		printf("\t%9.1f +/- %-7.1f"
		""
			,this->_hmeas->GetBinContent(ibin),
			this->_hmeas->GetBinError(ibin)
		);


		printf("\t%9.1f +/- %-7.1f"
		""
			,this->	_hforefold->GetBinContent(ibin),
			this->	_hforefold->GetBinError(ibin)
		);

		printf("\n");
	}
}
#endif

