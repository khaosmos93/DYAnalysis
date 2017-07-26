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
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>

#include <vector>
using namespace std;

#define nWeight 111

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

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


class HistProducer
{
public:
	TString Flavor;
	TString SampleType;
	TString Luminosity;

	Double_t PtCut_Lead;
	Double_t PtCut_Sub;
	Double_t EtaCut_Lead;
	Double_t EtaCut_Sub;
	
	HistProducer()
	{
		this->Flavor = "";
	}

	HistProducer( TString _Flavor ) : HistProducer()
	{
		this->Set_Flavor( _Flavor );
	}

	void Set_Flavor( TString _Flavor )
	{
		this->Flavor = _Flavor;
		if( Flavor != "Muon" && Flavor != "Electron" )
			cout << this->Flavor << " is WRONG! ... Muon or Electron are available" << endl;

		this->Setup();
	}
	void Produce()
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( "None" );

		TFile *f = new TFile("ROOTFile_Hists_ForPDFUnc_XSec_aMCNLO_"+this->Flavor+".root", "RECREATE");

		// const Int_t nMassBin = 43;
		Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};


	 	TH1D *h_mass_withinAcc = new TH1D("h_mass_withinAcc", "", nMassBin, MassBinEdges);		 
	 	TH1D *h_mass_postFSR = new TH1D("h_mass_postFSR", "", nMassBin, MassBinEdges);
		TH1D *h_mass_dressed = new TH1D("h_mass_dressed", "", nMassBin, MassBinEdges);

		TH1D *h_mass_withinAcc_Weighted[nWeight];
	 	TH1D *h_mass_postFSR_Weighted[nWeight];
		TH1D *h_mass_dressed_Weighted[nWeight];
		for(Int_t i=0; i<nWeight; i++)
		{
			TString TStr_Number = TString::Format("%03d", i);
			h_mass_withinAcc_Weighted[i] = new TH1D("h_mass_withinAcc_"+TStr_Number, "", nMassBin, MassBinEdges);
			h_mass_postFSR_Weighted[i] = new TH1D("h_mass_postFSR_"+TStr_Number, "", nMassBin, MassBinEdges);
			h_mass_dressed_Weighted[i] = new TH1D("h_mass_dressed_"+TStr_Number, "", nMassBin, MassBinEdges);
		}

		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2(this->SampleType, &ntupleDirectory, &Tag, &Xsec, &nEvents);

		// -- Loop for all samples -- //
		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			// if( Tag[i_tup] == "DYMuMu_M10to50" || Tag[i_tup] == "DYMuMu_M50to100" ) continue;

			TChain *chain = new TChain("recoTree/DYTree");
			TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");

			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_GenOthers();
			ntuple->TurnOnBranches_GenLepton();

			std::vector<double> *PDFWeights; PDFWeights = 0; // -- if it is not initialized, then seg. fault will occur! -- //
			chain->SetBranchStatus("PDFWeights", 1);
			chain->SetBranchAddress("PDFWeights", &PDFWeights);

			Double_t SumWeights = 0;
			Double_t SumWeights_Separated = 0;

			Int_t nTotEvent = chain->GetEntries();
			cout << "\t[Total Events: " << nTotEvent << "]" << endl; 

			Double_t norm = ( Xsec[i_tup] * this->Luminosity ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << norm << "]" << endl;

			// nTotEvent = 1000;
			// -- Event loop starts -- //
			for(Int_t i=0; i<nTotEvent; i++)
			{
				loadBar(i+1, nTotEvent, 100, 100);

				ntuple->GetEvent(i);

				//Bring weights for NLO MC events
				Double_t GenWeight = 0;
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

				SumWeights += GenWeight;

				Double_t TotWeight = norm * GenWeight;

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

				if( GenFlag == kTRUE )
				{
					SumWeights_Separated += GenWeight;

					// -- Collect gen-level information -- //
					vector<GenLepton> GenLeptonCollection;
					Int_t NGenLeptons = ntuple->gnpair; 
					for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
					{
						GenLepton genlep;
						genlep.FillFromNtuple(ntuple, i_gen);
						if( fabs(genlep.ID) == this->LeptonID && genlep.fromHardProcessFinalState )
							GenLeptonCollection.push_back( genlep );
					}
					GenLepton genlep1 = GenLeptonCollection[0];
					GenLepton genlep2 = GenLeptonCollection[1];
					Double_t M_postFSR = (genlep1.Momentum + genlep2.Momentum).M();

					// -- post-FSR mass distribution -- //
					h_mass_postFSR->Fill( M_postFSR, TotWeight );
					for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
						h_mass_postFSR_Weighted[i_weight]->Fill( M_postFSR, TotWeight*PDFWeights->at(i_weight) );

					// -- post-FSR distribution within acceptance -- //
					Bool_t Flag_PassAcc = this->Test_Acc( genlep1, genlep2 );
					if( Flag_PassAcc )
					{
						h_mass_withinAcc->Fill( M_postFSR, TotWeight );
						for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
							h_mass_withinAcc_Weighted[i_weight]->Fill( M_postFSR, TotWeight*PDFWeights->at(i_weight) );
					}

					// -- construct dressed leptons -- //
					GenLepton genlep_postFSR1 = GenLeptonCollection[0];
					GenLepton genlep_dressed1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection1;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_dressed1, &GenPhotonCollection1);

					GenLepton genlep_postFSR2 = GenLeptonCollection[1];
					GenLepton genlep_dressed2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection2;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_dressed2, &GenPhotonCollection2);

					// -- dressed level mass distribution -- //
					Double_t M_dressed = (genlep_dressed1.Momentum + genlep_dressed2.Momentum).M();
					h_mass_dressed->Fill( M_dressed, TotWeight );
					for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
						h_mass_dressed_Weighted[i_weight]->Fill( M_dressed, TotWeight*PDFWeights->at(i_weight) );

				} // -- End of if( GenFlag == kTRUE )

			} // -- End of event iteration

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
		} //end of i_tup iteration

		f->cd();

		h_mass_withinAcc->Write();
		h_mass_postFSR->Write();
		h_mass_dressed->Write();

		for(Int_t i=0; i<nWeight; i++)
		{
			h_mass_withinAcc_Weighted[i]->Write();
			h_mass_postFSR_Weighted[i]->Write();
			h_mass_dressed_Weighted[i]->Write();
		}

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

protected:
	void Setup()
	{
		if( this->Flavor == "Muon" )
		{
			this->SampleType = "aMCNLO_AdditionalSF_PDFUnc";
			this->LeptonID = 13;
			this->Luminosity = Lumi;

			this->PtCut_Lead = 22;
			this->PtCut_Sub = 10;
			this->EtaCut_Lead = 2.4;
			this->EtaCut_Sub = 2.4;
		}
		else if( this->Flavor == "Electron" )
		{
			this->SampleType = "aMCNLO_ee_AdditionalSF_PDFUnc";
			this->LeptonID = 11;
			this->Luminosity = 2257.998; // -- Golden JSON -- //

			this->PtCut_Lead = 30;
			this->PtCut_Sub = 10;
			this->EtaCut_Lead = 2.5;
			this->EtaCut_Sub = 2.5;
		}
	}

	Bool_t Test_Acc( GenLepton genlep1, GenLepton genlep2 )
	{
		Bool_t Flag = kFALSE;

		Double_t Pt_Lead;
		Double_t Pt_Sub;
		Double_t Eta_Lead;
		Double_t Eta_Sub;

		if( genlep1.Pt > genlep2.Pt )
		{
			Pt_Lead = genlep1.Pt;
			Pt_Sub = genlep2.Pt;			
			Eta_Lead = genlep1.eta;
			Eta_Sub = genlep2.eta;
		}
		else
		{
			Pt_Lead = genlep2.Pt;
			Pt_Sub = genlep1.Pt;			
			Eta_Lead = genlep2.eta;
			Eta_Sub = genlep1.eta;
		}

		if( Pt_Lead > this->PtCut_Lead && Pt_Sub > this->PtCut_Sub &&
			fabs(Eta_Lead) < this->EtaCut_Lead && fabs(Eta_Sub) < this->EtaCut_Sub )
		{
			if( this->Flavor == "Electron" )
			{
				if( !( fabs(genlep1.eta) > 1.4442 && fabs(genlep1.eta) < 1.566 )
				&& !( fabs(genlep2.eta) > 1.4442 && fabs(genlep2.eta) < 1.566 ) )
					Flag = kTRUE;
			}
			else
				Flag = kTRUE;
		}

		return Flag;
	}

};