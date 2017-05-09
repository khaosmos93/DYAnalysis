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

#include <vector>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

static inline void loadBar(int x, int n, int r, int w);
void Acc_Above50GeV(TString Sample = "Powheg" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );

	TFile *f = new TFile("ROOTFile_Histogram_Acc_Above50GeV_" + Sample + ".root", "RECREATE");

	const Int_t nMassBin2 = 36;
	Double_t MassBinEdges[nMassBin2+1] = {50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

 	TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", nMassBin2, MassBinEdges);
	TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", nMassBin2, MassBinEdges);

	TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
	// -- Each ntuple directory & corresponding Tags -- //
	// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

	if( Sample == "aMCNLO" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}
	if( Sample == "Powheg" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;
		if( Sample == "aMCNLO" && Tag[i_tup].Contains("M10to50") ) continue;

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		// ntuple->TurnOnBranches_HLT();
		// ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_GenLepton();

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeights = 0;
		Double_t SumWeights_Separated = 0;

		Int_t nTotEvent = chain->GetEntries();
		cout << "\t[Total Events: " << nTotEvent << "]" << endl; 

		Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
		cout << "\t[Normalization factor: " << norm << "]" << endl;

		// nTotEvent = 1000;
		// -- Event loop starts -- //
		for(Int_t i=0; i<nTotEvent; i++)
		{
			loadBar(i+1, nTotEvent, 100, 100);

			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			if( isNLO == 1 )
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
			else
				GenWeight = 1;

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
					if( genlep.isMuon() && genlep.fromHardProcessFinalState )
						GenLeptonCollection.push_back( genlep );
				}
				GenLepton genlep1 = GenLeptonCollection[0];
				GenLepton genlep2 = GenLeptonCollection[1];
				Double_t gen_M = (genlep1.Momentum + genlep2.Momentum).M();

				// -- Flags -- //
				Bool_t Flag_PassAcc = kFALSE;
				Bool_t Flag_PassAccEff = kFALSE;

				Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

				// -- Acceptance Calculation -- //
				if( Flag_PassAcc == kTRUE ) 
				{
					h_mass_AccTotal->Fill( gen_M, TotWeight );
					h_mass_AccPass->Fill( gen_M, TotWeight );
				}
				else
				{
					h_mass_AccTotal->Fill( gen_M, TotWeight );
				} 	

			} // -- End of if( GenFlag == kTRUE )

		} //End of event iteration

		f->cd();
		h_mass->Write();

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();

	h_mass_AccTotal->Write();
	h_mass_AccPass->Write();

	TEfficiency *Acc_Mass = new TEfficiency(*h_mass_AccPass, *h_mass_AccTotal);
	Acc_Mass->SetName("TEff_Acc_Mass");
	Acc_Mass->Write();

	Double_t TotalRunTime = totaltime.CpuTime();
	cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

	TTimeStamp ts_end;
	cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
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


