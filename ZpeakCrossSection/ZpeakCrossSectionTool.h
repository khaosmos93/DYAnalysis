#pragma once

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

#include "/home/kplee/CommonCodes/DrellYanAnalysis/DYAnalyzer.h"

// -- for Rochester Muon momentum correction -- //
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/RoccoR.cc"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/rochcor2015.cc"


class ZpeakCrossSectionTool
{
public:
	TString version;
	TString HLTname;
	TString FileLocation;

	Bool_t isDataDriven;

	DYAnalyzer *analyzer;

	vector< TString > ntupleDirectory; 
	vector< TString > Tag; 
	vector< Double_t > Xsec; 
	vector< Double_t > nEvents;

	TFile *f_data;
	TFile *f_yield;

	Double_t nEvents_Observed; // -- for the estimation of stat. uncertainty -- //
	Double_t yield_HLTv4p2;
	Double_t yield_HLTv4p3;
	Double_t yield_total;

	Double_t Acc;
	Double_t Eff;
	Double_t Eff_HLTv4p2;
	Double_t Eff_HLTv4p3;
	Double_t EffSF_HLTv4p2;
	Double_t EffSF_HLTv4p3;

	Double_t yield_HLTv4p2_AccEff;
	Double_t yield_HLTv4p3_AccEff;
	Double_t yield_total_AccEff;

	Double_t yield_HLTv4p2_EffSF;
	Double_t yield_HLTv4p3_EffSF;
	Double_t yield_total_EffSF;

	Double_t xSec;

	// -- for fiducial, post-FSR cross section -- //
	Double_t FpoF_yield_HLTv4p2_Eff;
	Double_t FpoF_yield_HLTv4p3_Eff;
	Double_t FpoF_yield_Eff;

	Double_t FpoF_yield_HLTv4p2_EffSF;
	Double_t FpoF_yield_HLTv4p3_EffSF;
	Double_t FpoF_yield_EffSF;

	Double_t FpoF_xSec;

	// -- Uncertainties -- //
	Double_t RelStatError;
	Double_t RelLumiError;

	// -- for the syst. from background estimation -- //
	TH1D *h_ttbar;
	TH1D *h_DYtautau;
	TH1D *h_tW;
	TH1D *h_WJets;
	TH1D *h_QCD;
	TH1D *h_diboson;
	// TH1D *h_ZZ;
	// TH1D *h_WZ;
	// TH1D *h_WW;

	ZpeakCrossSectionTool(TString _version, TString _HLTname )
	{
		cout << "===================================================================================" << endl;
		cout << "version = " << _version << ", HLT = " << _HLTname << endl;
		cout << "===================================================================================\n" << endl;

		HLTname = _HLTname;
		version = _version;
		FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/" + version; // -- 76X -- //

		analyzer = new DYAnalyzer( HLTname );
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X samples -- //

		f_data = NULL;
		f_yield = NULL;

		Acc = 0;
		Eff = 0;
		EffSF_HLTv4p2 = 0;
		EffSF_HLTv4p3 = 0;

		isDataDriven = kFALSE;

		h_ttbar = NULL;
		h_DYtautau = NULL;
		h_tW = NULL;
		h_WJets = NULL;
		h_QCD = NULL;
		h_diboson = NULL;
		// h_ZZ = NULL;
		// h_WZ = NULL;
		// h_WW = NULL;

		TH1::AddDirectory(kFALSE);
	}

	void ObtainYield(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;

		TString Type = "";
		
		if( isDataDriven ) 
			Type = "_DataDrivenBkg";
		else
			Type = "_MCBasedBkg";

		cout << "=================================" << endl;
		cout << "Background Type: " << Type << endl;
		cout << "=================================\n" << endl;

		f_data = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root"); f_data->cd();
		TH1D *h_data = (TH1D*)f_data->Get("h_mass_OS_Data")->Clone();
		
		nEvents_Observed = nEvents_Zpeak(h_data); // -- for statistical error -- //

		f_yield = TFile::Open(FileLocation + "/ROOTFile_YieldHistogram.root"), f_yield->cd();
		TH1D *h_yield_HLTv4p2 = (TH1D*)f_yield->Get("h_yield_OS_HLTv4p2"+Type)->Clone();
		TH1D *h_yield_HLTv4p3 = (TH1D*)f_yield->Get("h_yield_OS_HLTv4p3"+Type)->Clone();

		yield_HLTv4p2 = nEvents_Zpeak( h_yield_HLTv4p2 );
		yield_HLTv4p3 = nEvents_Zpeak( h_yield_HLTv4p3 );
		yield_total = yield_HLTv4p2 + yield_HLTv4p3;

		printf("[Yield] HLTv4p2: %.3lf, HLTv4p3: %.3lf, Total: %.3lf\n\n", yield_HLTv4p2, yield_HLTv4p3, yield_total);
	}

	Double_t nEvents_Zpeak(TH1D *h)
	{
		Double_t _nEvents_Zpeak = 0;
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinCenter = h->GetBinCenter(i_bin);

			if( BinCenter > 60 && BinCenter < 120 )
				_nEvents_Zpeak += h->GetBinContent(i_bin);
		}

		return _nEvents_Zpeak;
	}

	void Calc_AccEff(Bool_t isCorrected = kTRUE)
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TString isApplyMomCorr = "";
		if( isCorrected == kTRUE )
		{
			cout << "Apply Roochester Muon Momentum Correction..." << endl;
			isApplyMomCorr = "MomCorr";
		}
		else
		{
			cout << "DO *NOT* Apply Roochester Muon Momentum Correction..." << endl;
			isApplyMomCorr = "MomUnCorr";
		}

		TStopwatch totaltime;
		totaltime.Start();

		analyzer->SetupEfficiencyScaleFactor( "ROOTFile_TagProbeEfficiency_76X_v20160502.root" );

		Double_t Count_Acc_Total = 0;
		Double_t Count_Acc_Pass = 0;
		Double_t Count_Eff_Total = 0;
		Double_t Count_Eff_Pass = 0;
		Double_t Count_Eff_Pass_Corr_HLTv4p2 = 0;
		Double_t Count_Eff_Pass_Corr_HLTv4p3 = 0;

		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			if( !(Tag[i_tup] == "DYMuMu_M50to100" || Tag[i_tup] == "DYMuMu_M100to200") ) continue;

			TStopwatch looptime;
			looptime.Start();

			TChain *chain = new TChain("recoTree/DYTree");
			TString BaseLocation = "/data4/Users/kplee/DYntuple";
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_Muon();
			ntuple->TurnOnBranches_GenLepton();

			rochcor2015 *rmcor = new rochcor2015();

			Bool_t isMC;
			Tag[i_tup] == "Data" ? isMC = kFALSE : isMC = kTRUE;
			analyzer->SetupPileUpReWeighting_76X( isMC );

			Bool_t isNLO = 0;
			if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
			{
				isNLO = 1;
				cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
			}

			Double_t SumWeights = 0;
			Double_t SumWeights_Separated = 0;

			Int_t NEvents = chain->GetEntries();
			cout << "\t[Total Events: " << NEvents << "]" << endl; 

			Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << norm << "]" << endl;

			// NEvents = 1000000;
			for(Int_t i=0; i<NEvents; i++)
			{
				loadBar(i+1, NEvents, 100, 100);

				ntuple->GetEvent(i);

				//Bring weights for NLO MC events
				Double_t GenWeight;
				if( isNLO == 1 )
					ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
				else
					GenWeight = 1;

				SumWeights += GenWeight;

				Int_t PU = ntuple->nPileUp;
				Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

				Double_t TotWeight = norm * GenWeight * PUWeight;

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

					// -- Mass cut flag -- //
					Bool_t isPassMassCut = kFALSE;
					if( gen_M > 60 && gen_M < 120 )
						isPassMassCut = kTRUE;

					if( isPassMassCut == kTRUE )
					{
						// -- Flags -- //
						Bool_t Flag_PassAcc = kFALSE;
						Bool_t Flag_PassAccEff = kFALSE;
						
						Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

						// -- Acceptance Calculation -- //
						if( Flag_PassAcc == kTRUE ) 
						{
							Count_Acc_Total += norm * GenWeight;
							Count_Acc_Pass += norm * GenWeight;
						}
						else
							Count_Acc_Total += norm * GenWeight;

						Double_t Eff_SF_HLTv4p2 = -999; // -- Efficiency correction factor -- //
						Double_t Eff_SF_HLTv4p3 = -999; // -- Efficiency correction factor -- //

						// -- Calculate the efficiency among the events passing acceptance condition -- //
						if( Flag_PassAcc == kTRUE )
						{
							Bool_t Flag_PassEff = kFALSE;
							if( ntuple->isTriggered( analyzer->HLT ) )
							{
								// -- Collect Reconstruction level information -- //
								vector< Muon > MuonCollection;
								Int_t NLeptons = ntuple->nMuon;
								for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
								{
									Muon mu;
									mu.FillFromNtuple(ntuple, i_reco);
									// -- Apply Rochester momentum scale correction -- //
									if( isCorrected == kTRUE )
									{
										float qter = 1.0;
										
										if( Tag[i_tup] == "Data" )
											rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
										else
											rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

										// -- Change Muon pT, eta and phi with updated(corrected) one -- //
										mu.Pt = mu.Momentum.Pt();
										mu.eta = mu.Momentum.Eta();
										mu.phi = mu.Momentum.Phi();
									}
									
									MuonCollection.push_back( mu );
								} // -- end of muon iteration in a event -- //

								// -- Event Selection -- //
								vector< Muon > SelectedMuonCollection;
								Bool_t isPassEventSelection = kFALSE;
								isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

								if( isPassEventSelection == kTRUE )
								{
									Eff_SF_HLTv4p2 = analyzer->EfficiencySF_EventWeight_HLTv4p2( SelectedMuonCollection[0], SelectedMuonCollection[1] );
									Eff_SF_HLTv4p3 = analyzer->EfficiencySF_EventWeight_HLTv4p3( SelectedMuonCollection[0], SelectedMuonCollection[1] );

									// printf("( SF_HLTv4p2, SF_HLTv4p3 ) = (%.5lf, %.5lf)\n", Eff_SF_HLTv4p2, Eff_SF_HLTv4p3);

									Flag_PassEff = kTRUE;
									Flag_PassAccEff = kTRUE;
								}

							} // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

							// -- Efficiency Calculation -- //
							if( Flag_PassEff == kTRUE)
							{
								Count_Eff_Total += TotWeight;
								Count_Eff_Pass += TotWeight;
								Count_Eff_Pass_Corr_HLTv4p2 += TotWeight * Eff_SF_HLTv4p2;
								Count_Eff_Pass_Corr_HLTv4p3 += TotWeight * Eff_SF_HLTv4p3;
							} 
							else 
								Count_Eff_Total += TotWeight;

						} // -- End of if( Flag_PassAcc )  -- //

					} // -- end of if( isPassMassCut == kTRUE ) -- // 

				} // -- End of if( GenFlag == kTRUE ) -- //

			} // -- end of event iteration -- //

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;

		} // -- end of sample iteration -- //


		Acc = (Double_t)Count_Acc_Pass / Count_Acc_Total;
		Eff = (Double_t)Count_Eff_Pass / Count_Eff_Total;
		printf("Acceptance = %.3lf / %.3lf = %.6lf\n", Count_Acc_Pass, Count_Acc_Total, Acc);
		printf("Efficiency = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass, Count_Eff_Total, Eff);
		printf("Acceptance * Efficiency = %.6lf\n", Acc*Eff );

		Eff_HLTv4p2 = (Double_t)Count_Eff_Pass_Corr_HLTv4p2 / Count_Eff_Total;
		Eff_HLTv4p3 = (Double_t)Count_Eff_Pass_Corr_HLTv4p3 / Count_Eff_Total;
		printf("Efficiency (SF_weighted, HLTv4.2) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p2, Count_Eff_Total, Eff_HLTv4p2);
		printf("Efficiency (SF_weighted, HLTv4.3) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p3, Count_Eff_Total, Eff_HLTv4p3);

		EffSF_HLTv4p2 = Eff_HLTv4p2 / Eff;
		EffSF_HLTv4p3 = Eff_HLTv4p3 / Eff;
		printf("Efficiency scale factor (HLTv4.2) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p2, Eff, EffSF_HLTv4p2);
		printf("Efficiency scale factor (HLTv4.3) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p3, Eff, EffSF_HLTv4p3);

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

	void ApplyAccEffCorrection()
	{
		yield_HLTv4p2_AccEff = yield_HLTv4p2 / (Acc*Eff);
		yield_HLTv4p3_AccEff = yield_HLTv4p3 / (Acc*Eff);
		yield_total_AccEff = yield_HLTv4p2_AccEff + yield_HLTv4p3_AccEff;
		printf("[Acc*Eff Correction] (HLTv4.2, HLTv4.3, Total) = (%.1lf, %.1lf, %.1lf)\n", yield_HLTv4p2_AccEff, yield_HLTv4p3_AccEff, yield_total_AccEff);
	}

 	// -- in case that Acceptance and efficiency are already calculated -- //
	void ApplyAccEffCorrection(Double_t _Acc, Double_t _Eff)
	{
		Acc = _Acc;
		Eff = _Eff;
		cout << "Input Acceptance: " << Acc << ", Efficiency: " << Eff << endl; 

		yield_HLTv4p2_AccEff = yield_HLTv4p2 / (Acc*Eff);
		yield_HLTv4p3_AccEff = yield_HLTv4p3 / (Acc*Eff);
		printf("[Acc*Eff Correction] (HLTv4.2, HLTv4.3, Total) = (%.1lf, %.1lf, %.1lf)\n", yield_HLTv4p2_AccEff, yield_HLTv4p3_AccEff, yield_HLTv4p2_AccEff+yield_HLTv4p3_AccEff);
	}

	void EfficiencyScaleFactor()
	{
		yield_HLTv4p2_EffSF = yield_HLTv4p2_AccEff / EffSF_HLTv4p2;
		yield_HLTv4p3_EffSF = yield_HLTv4p3_AccEff / EffSF_HLTv4p3;
		yield_total_EffSF = yield_HLTv4p2_EffSF + yield_HLTv4p3_EffSF;
		printf("[Eff.SF-Corrected Yield] HLTv4p2: %.1lf, HLTv4p3: %.1lf, Total: %.1lf\n", yield_HLTv4p2_EffSF, yield_HLTv4p3_EffSF, yield_total_EffSF);
	}

	void EfficiencyScaleFactor(Double_t _EffSF_HLTv4p2, Double_t _EffSF_HLTv4p3)
	{
		EffSF_HLTv4p2 = _EffSF_HLTv4p2;
		EffSF_HLTv4p3 = _EffSF_HLTv4p3;
		cout << "Input Eff.SF (HLTv4.2): " << EffSF_HLTv4p2 << ", Eff.SF (HLTv4.3): " << EffSF_HLTv4p3 << endl;

		yield_HLTv4p2_EffSF = yield_HLTv4p2_AccEff / EffSF_HLTv4p2;
		yield_HLTv4p3_EffSF = yield_HLTv4p3_AccEff / EffSF_HLTv4p3;
		yield_total_EffSF = yield_HLTv4p2_EffSF + yield_HLTv4p3_EffSF;
		printf("[Eff.SF-Corrected Yield] HLTv4p2: %.1lf, HLTv4p3: %.1lf, Total: %.1lf\n", yield_HLTv4p2_EffSF, yield_HLTv4p3_EffSF, yield_total_EffSF);
	}

	void CalcXSec()
	{
		xSec = yield_total_EffSF / Lumi;

		// -- Statistical Error -- //
		this->RelStatError = sqrt(nEvents_Observed)/yield_total;
		printf("[Stat.Error] (sqrt(# observed events) / yield) = (%.1lf / %.1lf) = %12.7lf\n\n", sqrt(nEvents_Observed), yield_total, RelStatError);
		Double_t xSec_StatError = xSec * RelStatError;

		this->RelLumiError = 0.027;
		Double_t xSec_LumiError = xSec * RelLumiError;


		cout << "===============================================================================" << endl;
		printf("[Z-peak cross section] %.3lf +- %.3lf(stat. %.3lf%%) +- %.3lf(lumi. %.3lf%%)\n", xSec, xSec_StatError, RelStatError*100, xSec_LumiError, RelLumiError*100);
		cout << "===============================================================================\n\n" << endl;
	}

	void FpoF_CalcXSec()
	{
		this->FpoF_yield_HLTv4p2_Eff = this->yield_HLTv4p2 / this->Eff;
		this->FpoF_yield_HLTv4p3_Eff = this->yield_HLTv4p3 / this->Eff;
		this->FpoF_yield_Eff = this->FpoF_yield_HLTv4p2_Eff + this->FpoF_yield_HLTv4p3_Eff;
		printf("<Fiducial, post-FSR results> [Eff Correction] (HLTv4.2, HLTv4.3, Total) = (%.1lf, %.1lf, %.1lf)\n", FpoF_yield_HLTv4p2_Eff, FpoF_yield_HLTv4p3_Eff, FpoF_yield_Eff);


		this->FpoF_yield_HLTv4p2_EffSF = this->FpoF_yield_HLTv4p2_Eff / this->EffSF_HLTv4p2;
		this->FpoF_yield_HLTv4p3_EffSF = this->FpoF_yield_HLTv4p3_Eff / this->EffSF_HLTv4p3;
		this->FpoF_yield_EffSF = this->FpoF_yield_HLTv4p2_EffSF + this->FpoF_yield_HLTv4p3_EffSF;
		printf("<Fiducial, post-FSR results> [Eff.SF-Corrected Yield] HLTv4p2: %.1lf, HLTv4p3: %.1lf, Total: %.1lf\n", FpoF_yield_HLTv4p2_EffSF, FpoF_yield_HLTv4p3_EffSF, FpoF_yield_EffSF);


		this->FpoF_xSec = FpoF_yield_EffSF / Lumi;

		Double_t FpoF_xSec_StatError = this->FpoF_xSec * this->RelStatError;
		Double_t FpoF_xSec_LumiError = this->FpoF_xSec * this->RelLumiError;

		cout << "===============================================================================" << endl;
		printf("<Fiducial, post-FSR results> [Z-peak cross section] %.3lf +- %.3lf(stat. %.3lf%%) +- %.3lf(lumi. %.3lf%%)\n", FpoF_xSec, FpoF_xSec_StatError, RelStatError*100, FpoF_xSec_LumiError, RelLumiError*100);
		cout << "===============================================================================\n\n" << endl;
	}

	void Calc_SysUnc_BkgEst()
	{
		if( isDataDriven == kFALSE )
		{
			cout << "This result is not based on the data-driven background! ... systematic uncertaity estimation for MC-based background is not provided.";
			return;
		}

		// -- data-driven backgrounds -- //
		this->SetupHistogram_DataDrivenBkg();

		// // -- MC-based background -- //
		// this->SetupHistgram_MCBkg();


		vector< TH1D* > Histo_Bkg;
		Histo_Bkg.push_back( h_ttbar ); 
		Histo_Bkg.push_back( h_DYtautau ); 
		Histo_Bkg.push_back( h_tW ); 
		Histo_Bkg.push_back( h_QCD ); 
		Histo_Bkg.push_back( h_WJets );
		Histo_Bkg.push_back( h_diboson );

		// Histo_Bkg.push_back( h_WW ); 
		// Histo_Bkg.push_back( h_WZ ); 
		// Histo_Bkg.push_back( h_ZZ );

		const Int_t nBkg = 6;
		Double_t nBkgEvent[nBkg] = {0};
		Double_t AbsSysUnc[nBkg] = {0};

		// -- Data-driven backgrounds -- //
		for(Int_t i=0; i<nBkg; i++)
		{
			nBkgEvent[i] = nEvents_Zpeak( Histo_Bkg[i] ); 
			AbsSysUnc[i] = SumError_Zpeak( Histo_Bkg[i] );

			// printf("[%d bkg] (nEvent, AbsSysUnc) = (%.3lf, %.3lf)\n", i, nEvents_Zpeak( Histo_Bkg[i] ), SumError_Zpeak( Histo_Bkg[i] ) );
		}

		Double_t Sum_nBkg = 0;
		Double_t QuadSum_AbsUnc = 0;
		for(Int_t i=0; i<nBkg; i++)
		{
			Sum_nBkg += nBkgEvent[i];
			QuadSum_AbsUnc += AbsSysUnc[i] * AbsSysUnc[i];
		}

		QuadSum_AbsUnc = sqrt(QuadSum_AbsUnc);

		// printf("\tttbar\tDYtautau\ttW\tDiJet\tWJet\tWW\tWZ\tZZ\n");
		printf("[ Category] %10s, %10s, %10s, %10s, %10s, %10s\n", "ttbar", "DYtautau", "tW", "DiJet", "WJet", "diboson");
		printf("[ # events] %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf = %10.2lf\n", 
				nBkgEvent[0], nBkgEvent[1], nBkgEvent[2], nBkgEvent[3], nBkgEvent[4], nBkgEvent[5], Sum_nBkg);
		printf("[AbsSysUnc] %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf = %10.2lf\n", 
				AbsSysUnc[0], AbsSysUnc[1], AbsSysUnc[2], AbsSysUnc[3], AbsSysUnc[4], AbsSysUnc[5], QuadSum_AbsUnc);

		Double_t RelUnc_BkgEst = QuadSum_AbsUnc / yield_total;
		cout << "===========================================================================================" << endl;
		printf("[Relative Uncertainty on the cross section from Bkg.Est. (Percent): %6.4lf]\n", RelUnc_BkgEst*100 );
		cout << "===========================================================================================\n\n" << endl;
	}

	// void SetupHistgram_MCBkg()
	// {
	// 	Int_t nTag = (Int_t)Tag.size();

	// 	Double_t norm_ZZ = 0;
	// 	Double_t norm_WZ = 0;
	// 	Double_t norm_WW = 0;

	// 	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
	// 	{
	// 		Double_t norm = (Xsec[i_tag] * Lumi) / nEvents[i_tag];
	// 		if( Tag[i_tag] == "ZZ" )
	// 		{
	// 			norm_ZZ = norm;
	// 			printf("[Normalization factor for ZZ = %lf]\n", norm_ZZ);
	// 		}
	// 		else if( Tag[i_tag] == "WZ" )
	// 		{
	// 			norm_WZ = norm;
	// 			printf("[Normalization factor for WZ = %lf]\n", norm_WZ);
	// 		}
	// 		else if( Tag[i_tag] == "WW" )
	// 		{
	// 			norm_WW = norm;
	// 			printf("[Normalization factor for WW = %lf]\n", norm_WW);
	// 		}
	// 	}
	// 	TFile *f_MC = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root"); f_MC->cd();
	// 	h_ZZ = (TH1D*)f_MC->Get("h_mass_OS_ZZ")->Clone();
	// 	// h_ZZ = (TH1D*)h_ZZ->Rebin(nMassBin, h_ZZ->GetName(), MassBinEdges);
	// 	h_WZ = (TH1D*)f_MC->Get("h_mass_OS_WZ")->Clone();
	// 	// h_WZ = (TH1D*)h_WZ->Rebin(nMassBin, h_WZ->GetName(), MassBinEdges);
	// 	h_WW = (TH1D*)f_MC->Get("h_mass_OS_WW")->Clone();
	// 	// h_WW = (TH1D*)h_WW->Rebin(nMassBin, h_WW->GetName(), MassBinEdges);

	// 	if( h_ZZ == NULL || h_WZ == NULL || h_WW == NULL )
	// 	{
	// 		printf("Histogram for MC_based background is not loaded properly! ... please check");
	// 		return;
	// 	}

	// 	h_ZZ->Scale( norm_ZZ );
	// 	h_WZ->Scale( norm_WZ );
	// 	h_WW->Scale( norm_WW );

	// 	TH1::AddDirectory(kFALSE);
	// }

	void SetupHistogram_DataDrivenBkg()
	{
		TFile *f_DataDrivenBkg = TFile::Open(FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
		h_QCD = (TH1D*)f_DataDrivenBkg->Get("dijet")->Clone();
		h_WJets = (TH1D*)f_DataDrivenBkg->Get("wjets")->Clone();
		h_ttbar = (TH1D*)f_DataDrivenBkg->Get("ttbar")->Clone();
		h_tW = (TH1D*)f_DataDrivenBkg->Get("tW")->Clone();
		h_DYtautau = (TH1D*)f_DataDrivenBkg->Get("DYtautau")->Clone();
		h_diboson = (TH1D*)f_DataDrivenBkg->Get("diboson")->Clone();

		if( h_QCD == NULL || h_WJets == NULL || h_ttbar == NULL || h_tW == NULL || h_DYtautau == NULL || h_diboson == NULL )
		{
			printf("Histogram for Data-driven background is not loaded properly! ... please check");
		}
	}

	Double_t SumError_Zpeak(TH1D *h)
	{
		Double_t _SumError_Zpeak = 0;

		Double_t SquareSum = 0;
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinCenter = h->GetBinCenter(i_bin);

			if( BinCenter > 60 && BinCenter < 120 )
			{
				Double_t BinError = h->GetBinError(i_bin);
				SquareSum += BinError * BinError;
			}
		}
		_SumError_Zpeak = sqrt( SquareSum );

		return _SumError_Zpeak;
	}

	void FpoF_CalcXsec_aMCNLO()
	{
		TFile* f_result = TFile::Open(FileLocation+"/ROOTFile_Results_DYAnalysis_76X.root");
		TH1D* h_FpoF_yield_aMCNLO = (TH1D*)f_result->Get("h_FpoF_yield_aMCNLO")->Clone();

		Double_t FpoF_yield_aMCNLO = 0;
		Double_t FpoF_yieldError_aMCNLO = 0;
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinCenter = h_FpoF_yield_aMCNLO->GetBinCenter(i_bin);

			if( BinCenter > 60 && BinCenter < 120 )
			{
				FpoF_yield_aMCNLO += h_FpoF_yield_aMCNLO->GetBinContent(i_bin);
				FpoF_yieldError_aMCNLO += h_FpoF_yield_aMCNLO->GetBinError(i_bin) * h_FpoF_yield_aMCNLO->GetBinError(i_bin);
			}
		}
		FpoF_yieldError_aMCNLO = sqrt( FpoF_yieldError_aMCNLO );

		Double_t FpoF_xSec_aMCNLO = FpoF_yield_aMCNLO / Lumi;
		Double_t FpoF_xSecError_aMCNLO = FpoF_yieldError_aMCNLO / Lumi;

		printf("< [aMC@NLO] fiducial, post-FSR > (yield, cross section) = (%lf +- %lf, %lf +- %lf)\n\n", FpoF_yield_aMCNLO, FpoF_yieldError_aMCNLO, FpoF_xSec_aMCNLO, FpoF_xSecError_aMCNLO);
	}
};









