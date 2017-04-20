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
// #include <Include/RochesterMomCorr_80X/RoccoR.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>
#include <Include/PlotTools.h>

#define nXSec 9

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

	Double_t Arr_MinBiasXSec[nXSec];
	Double_t Arr_PUWeights[nXSec][nPUBin];

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

		this->Init();
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
		// ControlPlots *Plots = new ControlPlots( "", this->isMC );
		// TH1D *h_PU = new TH1D("h_PU_"+Tag[i_tup], "", 50, 0, 50);
		TH1D *h_nVertices_before = new TH1D("h_nVtx_before", "", 50, 0, 50);
		TH1D *h_nVertices_after[nXSec];
		for(Int_t i=0; i<nXSec; i++)
			h_nVertices_after[i] = new TH1D(TString::Format("h_nVtx_after_XSec_%.0lf", Arr_MinBiasXSec[i]), "", 50, 0, 50);

		Int_t nTotEvent = chain->GetEntries();
		cout << "\t[Total Events: " << nTotEvent << "]" << endl;

		for(Int_t i_event=0; i_event<nTotEvent; i_event++)
		{
			this->loadBar(i_event+1, nTotEvent, 100, 100);
			
			ntuple->GetEvent(i_event);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

			Double_t TotWeight = this->NormFactor*GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(this->Tag, ntuple); // -- for DY->tautau process -- //

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
						MuonCollection.push_back( mu );
					}

					// -- Event Selection -- //
					MuPair SelectedPair;
					Bool_t isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, SelectedPair);

					if( isPassEventSelection == kTRUE )
					{
						Int_t nVertices = ntuple->nVertices;
						h_nVertices_before->Fill(nVertices, GenWeight);

						Int_t PU = ntuple->nPileUp;
						for(Int_t i_XSec=0; i_XSec<nXSec; i_XSec++)
						{
							Double_t PUWeight = Arr_PUWeights[i_XSec][PU];
							h_nVertices_after->Fill( nVertices, GenWeight*PUWeight );
						}
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

	// -- Fill PU weight array -- //
	void Set_PUWeights( Bool_t isMC )
	{
		if( isMC == kFALSE ) // -- for data: no weights -- //
		{
			for(Int_t i_XSec=0; i_XSec<nXSec; i_XSec++)
				for(Int_t i=0; i<nPUBin; i++)
					PileUpWeight[i_XSec][i] = 1;

			return;
		}
		
		// -- Only for the MC -- //
		TString IncludePath = gSystem->Getenv("KP_INCLUDE_PATH");
		TString BaseFilePath = IncludePath+"/PUWeights";

		for(Int_t i_XSec=0; i_XSec<nXSec; i_XSec++)
		{
			TString FileName = BaseFilePath + TString::Format("/ROOTFile_PUDist_MinBiasXSec_%.0lf.root\n", Arr_MinBiasXSec[i_XSec]);
			printf("[Min-bias cross section = %.0lf] FileName = %s\n", Arr_MinBiasXSec[i_XSec], FileName);
			TH1D* h_PUWeight = Get_Hist(FileName, "h_PUWeight");

			printf("[PU weight values]\n");
			for(Int_t i=0; i<nPUBin; i++)
			{
				Int_t i_bin = i+1;
				Arr_PUWeights[i_XSec][i] = h_PUWeight->GetBinContent(i_bin);
				printf("%7.3lf\t", Arr_PUWeights[i_XSec][i]);
				if( i_bin % 10 == 0 && i_bin != 0 )
					printf("\n");
			}
		}

		printf("[Setup for PU weights are finished]\n");
	}

	void Init()
	{
		Arr_MinBiasXSec[0] = 65;
		Arr_MinBiasXSec[1] = 66;
		Arr_MinBiasXSec[2] = 67;
		Arr_MinBiasXSec[3] = 68;

		Arr_MinBiasXSec[4] = 69;

		Arr_MinBiasXSec[5] = 70;
		Arr_MinBiasXSec[6] = 71;
		Arr_MinBiasXSec[7] = 72;
		Arr_MinBiasXSec[8] = 73;
	}
};