#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TMatrixT.h"
#include "TMatrixD.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "TCanvas.h"

#include "TStopwatch.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TF1.h"

TFile* _inFile;
TFile* _outFile;

#include "include/Unfolder.h"
Unfolder* _unfolder;

TRandom3* _rand3;

#include "include/PullMaker.h"
PullMaker* _pullMaker;


TH1F* _inTrue = 0;
TCanvas* can = 0;

TH2F* _inMigMat = 0;
TH2F* _inResMat = 0;
TH2F* _inEvMat  = 0;
void colNormalize(TH2F* hist);
TH2F* getMigMatrix(TH1F* htrue, TH2F* hden);

void testPulls(){

	can = new TCanvas("can","can",1700,40,600,600);
	_rand3 = new TRandom3(42);
	gStyle->SetPalette(1);
	_inFile = new TFile("resMatrixProd.root");
	_outFile = new TFile("outTestPulls.root","recreate");
	_outFile->cd();

	_inTrue = (TH1F*)_inFile->Get("htrue");
	_inEvMat = (TH2F*)_inFile->Get("hden");
        //same integral
        _inTrue->Scale(_inEvMat->Integral()/_inTrue->Integral()); 

        _inMigMat = getMigMatrix(_inTrue,_inEvMat);
        _inResMat = getMigMatrix(_inTrue,_inEvMat);
	_inResMat->SetName("resMatrix");
	colNormalize(_inResMat);

	_pullMaker = new PullMaker(1000, 1e6, _inTrue,_inMigMat,_inResMat);
//	_pullMaker = new PullMaker(1000, 1e5,_inTrue,_inMigMat,_inResMat);
	_pullMaker->unfoldEnsembles();
	_pullMaker->getGenRefHist()->Draw();
        _pullMaker->getTrueHist()->Draw("same");
	_pullMaker->getMeasHist()->Draw("same,hist");

	_outFile->Write();

}
//
// column normalize
//
void colNormalize(TH2F* hist){
	
	int nbinsx = hist->GetNbinsX();
	int nbinsy = hist->GetNbinsY();

	for (int icol = 1; icol <= nbinsx; ++icol){

		double sum = 0.;
		for (int irow = 1; irow <= nbinsy; ++irow){
			sum += hist->GetBinContent(icol,irow);
		}
		
		for (int irow = 1; irow <= nbinsy; ++irow){
			double val = hist->GetBinContent(icol,irow);
			val /= sum;
			hist->SetBinContent(icol,irow,val);
		}

	}

}


TH2F* getMigMatrix(TH1F* htrue, TH2F* hden) {

    TH2F* migMatrix = (TH2F*)hden->Clone();
    const int nbin = hden->GetXaxis()->GetNbins();

    for( int i = 0; i < nbin+1; i++ ) {
        for( int j = 0; j < nbin+1; j++ ) {
          double den = htrue->GetBinContent(i);
          migMatrix->SetBinContent(i, j, hden->GetBinContent(i,j)/den);
        }
    }
   //printHisto2D(migMatrix); 

   return migMatrix;
}
