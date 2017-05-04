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

#define nWeight 111

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

static inline void loadBar(int x, int n, int r, int w);
void ProdHist_Acc_PDFUnc(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );

	TFile *f = new TFile("ROOTFile_Hists_ForPDFUnc_aMCNLO.root", "RECREATE");

	// const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

 	TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", nMassBin, MassBinEdges);
	TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", nMassBin, MassBinEdges);
 	TH1D *h_mass_AccTotal_Weighted[nWeight];
	TH1D *h_mass_AccPass_Weighted[nWeight];
	for(Int_t i=0; i<nWeight; i++)
	{
		TString TStr_Number = TString::Format("%03d", i);
		h_mass_AccTotal_Weighted[i] = new TH1D("h_mass_AccTotal_"+TStr_Number, "", nMassBin, MassBinEdges);
		h_mass_AccPass_Weighted[i] = new TH1D("h_mass_AccPass_"+TStr_Number, "", nMassBin, MassBinEdges);
	}

	// -- Each ntuple directory & corresponding Tags -- //
	// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
	analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF_PDFUnc", &ntupleDirectory, &Tag, &Xsec, &nEvents);

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		if( Tag[i_tup] == "DYMuMu_M10to50" || Tag[i_tup] == "DYMuMu_M50to100" ) continue;

		TChain *chain = new TChain("recoTree/DYTree");
		TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_GenLepton();

		cout << "Turn on branches: GenLepton" << endl;

		std::vector<double> *PDFWeights;
		chain->SetBranchStatus("PDFWeights", 1);
		chain->SetBranchAddress("PDFWeights", &PDFWeights);

		cout << "Set branch address for PDF weights branch" << endl;

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

			cout << i << "th event" << endl;

			// Int_t _nWeight = PDFWeights->size();
			// for(Int_t i_weight=0; i_weight<_nWeight; i_weight++)
			// 	cout << i_weight << "th weight: " << PDFWeights->at(i_weight) << endl;

			Int_t _nWeight = PDFWeights.size();
			cout << "_nWeight: " << _nWeight << endl;
			for(Int_t i_weight=0; i_weight<_nWeight; i_weight++)
				cout << i_weight << "th weight: " << PDFWeights.at(i_weight) << endl;

			//Bring weights for NLO MC events
			Double_t GenWeight = 0;
			ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

			SumWeights += GenWeight;

			Double_t TotWeight = norm * GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				cout << "\tPass GenFlag" << endl;

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
					cout << "\tPass acceptance" << endl;
					h_mass_AccTotal->Fill( gen_M, TotWeight );
					h_mass_AccPass->Fill( gen_M, TotWeight );

					// for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
					// {
					// 	h_mass_AccTotal_Weighted[i_weight]->Fill( gen_M, TotWeight*PDFWeights->at(i_weight) );
					// 	h_mass_AccPass_Weighted[i_weight]->Fill( gen_M, TotWeight*PDFWeights->at(i_weight) );
					// }
				}
				else
				{
					cout << "\tFail acceptance" << endl;
					h_mass_AccTotal->Fill( gen_M, TotWeight );
					// for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
					// 	h_mass_AccTotal_Weighted[i_weight]->Fill( gen_M, TotWeight*PDFWeights->at(i_weight) );
				} 	

			} // -- End of if( GenFlag == kTRUE )

			cout << endl;

		} // -- End of event iteration

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();

	h_mass_AccTotal->Write();
	h_mass_AccPass->Write();
	for(Int_t i=0; i<nWeight; i++)
	{
		h_mass_AccTotal_Weighted[i]->Write();
		h_mass_AccPass_Weighted[i]->Write();
	}

	TEfficiency *Acc_Mass = new TEfficiency(*h_mass_AccPass, *h_mass_AccTotal);
	Acc_Mass->SetName("TEff_Acc_Mass");
	TGraphAsymmErrors *g_Acc = (TGraphAsymmErrors*)Acc_Mass->CreateGraph()->Clone("g_Acc");

	TEfficiency *Acc_Mass_Weighted[nWeight];
	TGraphAsymmErrors *g_Acc_Weighted[nWeight];
	for(Int_t i=0; i<nWeight; i++)
	{
		TString TStr_Number = TString::Format("%03d", i);
		Acc_Mass_Weighted[i] = new TEfficiency(*h_mass_AccPass_Weighted[i], *h_mass_AccTotal_Weighted[i]);
		Acc_Mass_Weighted[i]->SetName("TEff_Acc_Mass_"+TStr_Number);
		g_Acc_Weighted[i] = (TGraphAsymmErrors*)Acc_Mass_Weighted[i]->CreateGraph()->Clone("g_Acc_"+TStr_Number);
	}

	Acc_Mass->Write();
	g_Acc->Write();
	for(Int_t i=0; i<nWeight; i++)
	{
		Acc_Mass_Weighted[i]->Write();
		g_Acc_Weighted[i]->Write();
	}

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