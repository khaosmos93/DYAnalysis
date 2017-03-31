#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>
#include <TRandom.h>

// -- for Rochester Muon momentum correction -- //
#include "DYAnalysis_80X/Include/RochesterMomCorr_80X/RoccoR.cc"

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "DYAnalysis_80X/Include/DYAnalyzer.h"
#include "ControlPlots.h"

class HistogramProducer;

void ProdHist_nVtx(Int_t _isMC, TString _FileName_ROOTFileList, Double_t _NormFactor)
{
	HistogramProducer *producer = new HistogramProducer( _isMC, _FileName_ROOTFileList, _NormFactor );
	producer->Producer();
}


class HistogramProducer
{
public:
	Int_t isMC;
	Double_t NormFactor;
	TString FileName_ROOTFileList;
	TString Tag;

	HistogramProducer( Int_t _isMC, TString _FileName_ROOTFileList, Double_t _NormFactor)
	{
		this->isMC = _isMC;
		this->FileName_ROOTFileList = _FileName_ROOTFileList;
		this->NormFactor = _NormFactor;
		this->Tag = GetTag( this->FileName_ROOTFileList );

		// if( this->Tag.Contains("DYPowheg") ) this->Tag = "DYPowheg";
		// if( this->Tag.Contains("DYMuMu") ) this->Tag = "DYMuMu";
		// if( this->Tag.Contains("ttbar") ) this->Tag = "ttbar";

		printf("===============[HistogramProducer]===============\n");
		printf("[isMC = %d] Tag = %s, NormFactor = %lf\n", this->isMC, this->Tag.Data(), this->NormFactor);
		printf("=================================================\n");
	}

	void Producer()
	{
		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu24_OR_IsoTkMu24" );

		// -- make chain -- //
		TChain *chain = new TChain("recoTree/DYTree");
		analyzer->MakeTChain_fromTextFile( chain, FileName_ROOTFileList );

		// -- turn-on ntuple -- //		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_Muon();
		if( this->isMC )
			ntuple->TurnOnBranches_GenLepton();

		// -- plot container -- //
		ControlPlots *Plots = new ControlPlots( "", this->isMC );
		// TH1D *h_PU = new TH1D("h_PU_"+Tag[i_tup], "", 50, 0, 50);
		// TH1D *h_nVertices_before = new TH1D("h_nVertices_before_"+Tag[i_tup], "", 50, 0, 50);
		// TH1D *h_nVertices_after = new TH1D("h_nVertices_after_"+Tag[i_tup], "", 50, 0, 50);


		// -- Rochester momentum correction -- //
		TString DirPath_RoccoR = GetBasePath() + "Include/RochesterMomCorr_80X/rcdata.2016.v3";
		RoccoR rmcor(DirPath_RoccoR.Data());

		Int_t nTotEvent = chain->GetEntries();
		cout << "\t[Total Events: " << nTotEvent << "]" << endl;

		for(Int_t i=0; i<nTotEvent; i++)
		{
			this->loadBar(i+1, nTotEvent, 100, 100);
			
			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

			Double_t TotWeight = this->NormFactor*GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(this->Tag, ntuple); // -- for DY->tautau process -- //

			// // -- Fill the histogram for gen-level information (signal sample) -- //
			// if( GenFlag == 1 && Tag[i_tup].Contains("DYMuMu") )
			// {
			// 	vector<GenLepton> GenLeptonCollection;
			// 	Int_t NGenLeptons = ntuple->gnpair;
			// 	for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
			// 	{
			// 		GenLepton genlep;
			// 		genlep.FillFromNtuple(ntuple, i_gen);
			// 		if( genlep.isMuon() && genlep.isHardProcess )
			// 		{
			// 			GenLeptonCollection.push_back( genlep );

			// 			if( GenLeptonCollection.size() ==  2 )
			// 				break;
			// 		}
			// 	}
			// 	Plots->FillHistograms_GenDoubleMu(ntuple, GenLeptonCollection[0], GenLeptonCollection[1], GenWeight);
			// }

			if( GenFlag == kTRUE )
			{
				////////////////////////////////
				// -- reco-level selection -- //
				////////////////////////////////
				if( ntuple->isTriggered( analyzer->HLT ) )
				{
					vector< Muon > MuonCollection;
					Int_t nLepton = ntuple->nMuon;
					for(Int_t i_reco=0; i_reco<nLepton; i_reco++)
					{
						Muon mu(ntuple, i_reco);

						// -- Apply Rochester momentum scale correction -- //
						Double_t SF_rmcor = 0;
						if( isMC )
						{
							Double_t u1 = gRandom->Rndm();
							Double_t u2 = gRandom->Rndm();
							SF_rmcor = rmcor.kScaleAndSmearMC(mu.charge, mu.Pt, mu.eta, mu.phi, mu.trackerLayers, u1, u2);
						}
						else // -- data -- //
							SF_rmcor = rmcor.kScaleDT(mu.charge, mu.Pt, mu.eta, mu.phi);

						mu.Pt = mu.Pt * SF_rmcor;

						TLorentzVector momentum_rmcor;
						momentum_rmcor.SetPtEtaPhiM(mu.Pt, mu.eta, mu.phi, M_Mu);
						mu.Momentum = momentum_rmcor;
						// -- end of correction -- //

						MuonCollection.push_back( mu );
					}

					// -- Event Selection -- //
					MuPair SelectedPair;
					Bool_t isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, SelectedPair);

					if( isPassEventSelection == kTRUE )
					{
						// printf("\t[Pass event selection condition]\n");
						// PrintRecoMuonInfo( SelectedPair.First );
						// PrintRecoMuonInfo( SelectedPair.Second );

						Plots->FillHistograms_MuPair( SelectedPair, ntuple->runNum, TotWeight );

						// Int_t PU = ntuple->nPileUp;
						// h_PU->Fill( PU, PUWeight );

						// Int_t nVertices = ntuple->nVertices;
						// h_nVertices_before->Fill(nVertices, GenWeight);
						// h_nVertices_after->Fill(nVertices, GenWeight*PUWeight);
					}

				} // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

			} // -- end of if( GenFlag == kTRUE ) -- //

		} // -- end of event iteration -- //

		TFile *f_output = TFile::Open("ROOTFile_MuonPlots_"+this->Tag+".root", "RECREATE");
		Plots->WriteHistograms( f_output );
		// h_PU->Write();
		// h_nVertices_before->Write();
		// h_nVertices_after->Write();

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "\tTotal RunTime(" << this->Tag << "): " << TotalRunTime << " seconds\n" << endl;

		printf("============================\nProducer() is finished\n============================\n\n");
	}

protected:
	void PrintRecoMuonInfo( Muon mu )
	{
		printf("\t\t(pT = %.3lf, eta = %.3lf, phi = %.3lf, charge = %.d)\n", 
						mu.Pt, mu.eta, mu.phi, mu.charge);
	}

	static inline void loadBar(int x, int n, int r, int w)
	{
	    // Only update r times.
	    if( x == n )
	    	cout << endl;

	    if ( x % (n/r +1) != 0 ) return;

	 
	    // Calculuate the ratio of complete-to-incomplete.
	    float ratio = x/(float)n;
	    int   c     = ratio * w;
	 
	    // Show the percentage complete.
	    printf("%3d%% [", (int)(ratio*100) );
	 
	    // Show the load bar.
	    for (int x=0; x<c; x++) cout << "=";
	 
	    for (int x=c; x<w; x++) cout << " ";
	 
	    // ANSI Control codes to go back to the
	    // previous line and clear it.
		cout << "]\r" << flush;

	}
};