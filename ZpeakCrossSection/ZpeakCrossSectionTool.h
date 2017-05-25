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
#include <TVector.h>

#include <vector>

#include <Include/DYAnalyzer.h>

// -- for Rochester Muon momentum correction -- //
#include <Include/RochesterMomCorr_76X/RoccoR.cc>
#include <Include/RochesterMomCorr_76X/rochcor2015.cc>


class ZpeakCrossSectionTool
{
public:
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
	Double_t RelSystUnc_BkgEst;

	// -- for the syst. from background estimation -- //
	TH1D *h_ttbar;
	TH1D *h_DYtautau;
	TH1D *h_tW;
	TH1D *h_WJets;
	TH1D *h_QCD;
	// TH1D *h_diboson;
	TH1D *h_WW;

	// -- MC backgrounds -- //
	TH1D *h_ZZ;
	TH1D *h_WZ;
	

	ZpeakCrossSectionTool(TString _HLTname)
	{
		cout << "===================================================================================" << endl;
		cout << "HLT = " << _HLTname << endl;
		cout << "===================================================================================\n" << endl;

		this->HLTname = _HLTname;
		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");

		this->analyzer = new DYAnalyzer( HLTname );
		this->analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X samples -- //

		this->f_data = NULL;
		this->f_yield = NULL;

		this->Acc = 0;
		this->Eff = 0;
		this->EffSF_HLTv4p2 = 0;
		this->EffSF_HLTv4p3 = 0;

		this->RelStatError = -1;
		this->RelLumiError = -1;
		this->RelSystUnc_BkgEst = -1;

		this->isDataDriven = kFALSE;

		this->h_ttbar = NULL;
		this->h_DYtautau = NULL;
		this->h_tW = NULL;
		this->h_WJets = NULL;
		this->h_QCD = NULL;
		// this->h_diboson = NULL;
		this->h_WW = NULL;

		this->h_WZ = NULL;
		this->h_ZZ = NULL;

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
		
		this->nEvents_Observed = this->nEvents_Zpeak(h_data); // -- for statistical error -- //

		f_yield = TFile::Open(FileLocation + "/ROOTFile_YieldHistogram.root"), f_yield->cd();
		TH1D *h_yield_HLTv4p2 = (TH1D*)f_yield->Get("h_yield_OS_HLTv4p2"+Type)->Clone();
		TH1D *h_yield_HLTv4p3 = (TH1D*)f_yield->Get("h_yield_OS_HLTv4p3"+Type)->Clone();

		this->yield_HLTv4p2 = this->nEvents_Zpeak( h_yield_HLTv4p2 );
		this->yield_HLTv4p3 = this->nEvents_Zpeak( h_yield_HLTv4p3 );
		this->yield_total = this->yield_HLTv4p2 + this->yield_HLTv4p3;

		printf("[Yield] HLTv4p2: %.3lf, HLTv4p3: %.3lf, Total: %.3lf\n\n", this->yield_HLTv4p2, this->yield_HLTv4p3, this->yield_total);
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
			ntuple->TurnOnBranches_HLT();
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
		this->Acc = _Acc;
		this->Eff = _Eff;
		cout << "Input Acceptance: " << this->Acc << ", Efficiency: " << this->Eff << endl; 

		this->yield_HLTv4p2_AccEff = this->yield_HLTv4p2 / (this->Acc*this->Eff);
		this->yield_HLTv4p3_AccEff = this->yield_HLTv4p3 / (this->Acc*this->Eff);
		printf("[Acc*Eff Correction] (HLTv4.2, HLTv4.3, Total) = (%.1lf, %.1lf, %.1lf)\n", yield_HLTv4p2_AccEff, yield_HLTv4p3_AccEff, yield_HLTv4p2_AccEff+yield_HLTv4p3_AccEff);
	}

	void EfficiencyScaleFactor()
	{
		this->yield_HLTv4p2_EffSF = this->yield_HLTv4p2_AccEff / this->EffSF_HLTv4p2;
		this->yield_HLTv4p3_EffSF = this->yield_HLTv4p3_AccEff / this->EffSF_HLTv4p3;
		this->yield_total_EffSF = this->yield_HLTv4p2_EffSF + this->yield_HLTv4p3_EffSF;
		printf("[Eff.SF-Corrected Yield] HLTv4p2: %.1lf, HLTv4p3: %.1lf, Total: %.1lf\n", yield_HLTv4p2_EffSF, yield_HLTv4p3_EffSF, yield_total_EffSF);
	}

	void EfficiencyScaleFactor(Double_t _EffSF_HLTv4p2, Double_t _EffSF_HLTv4p3)
	{
		this->EffSF_HLTv4p2 = _EffSF_HLTv4p2;
		this->EffSF_HLTv4p3 = _EffSF_HLTv4p3;
		cout << "Input Eff.SF (HLTv4.2): " << this->EffSF_HLTv4p2 << ", Eff.SF (HLTv4.3): " << this->EffSF_HLTv4p3 << endl;

		this->yield_HLTv4p2_EffSF = this->yield_HLTv4p2_AccEff / this->EffSF_HLTv4p2;
		this->yield_HLTv4p3_EffSF = this->yield_HLTv4p3_AccEff / this->EffSF_HLTv4p3;
		this->yield_total_EffSF = this->yield_HLTv4p2_EffSF + this->yield_HLTv4p3_EffSF;
		printf("[Eff.SF-Corrected Yield] HLTv4p2: %.1lf, HLTv4p3: %.1lf, Total: %.1lf\n", this->yield_HLTv4p2_EffSF, this->yield_HLTv4p3_EffSF, this->yield_total_EffSF);
	}

	void CalcXSec()
	{
		this->xSec = this->yield_total_EffSF / Lumi;

		// -- Statistical Error -- //
		this->RelStatError = sqrt(nEvents_Observed)/this->yield_total;
		printf("[Stat.Error] (sqrt(# observed events) / yield) = (%.1lf / %.1lf) = %12.7lf\n\n", sqrt(this->nEvents_Observed), this->yield_total, this->RelStatError);
		Double_t xSec_StatError = this->xSec * this->RelStatError;

		this->RelLumiError = 0.023;
		Double_t xSec_LumiError = this->xSec * this->RelLumiError;

		cout << "===============================================================================" << endl;
		printf("[Z-peak cross section] %.3lf +- %.3lf(stat. %.3lf%%) +- %.3lf(lumi. %.3lf%%)\n", this->xSec, xSec_StatError, this->RelStatError*100, xSec_LumiError, this->RelLumiError*100);
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
		printf("<Fiducial, post-FSR results> [Z-peak cross section] %.3lf +- %.3lf(stat. %.3lf%%) +- %.3lf(lumi. %.3lf%%)\n", this->FpoF_xSec, FpoF_xSec_StatError, this->RelStatError*100, FpoF_xSec_LumiError, this->RelLumiError*100);
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
		this->SetupHistgram_MCBkg_All();

		vector< Double_t > nBkgEvent;
		vector< Double_t > AbsSystUnc;

		// -- data-driven backgrounds -- //
		vector< TH1D* > Histo_DataDrivnBkg;
		Histo_DataDrivnBkg.push_back( this->h_ttbar ); 
		Histo_DataDrivnBkg.push_back( this->h_DYtautau ); 
		Histo_DataDrivnBkg.push_back( this->h_tW ); 
		Histo_DataDrivnBkg.push_back( this->h_QCD ); 
		Histo_DataDrivnBkg.push_back( this->h_WJets );
		Histo_DataDrivnBkg.push_back( this->h_WW );

		Int_t nDataDrivenBkg = (Int_t)Histo_DataDrivnBkg.size();

		// -- Data-driven backgrounds -- //
		for(Int_t i=0; i<nDataDrivenBkg; i++)
		{
			nBkgEvent.push_back( nEvents_Zpeak( Histo_DataDrivnBkg[i] ) );
			AbsSystUnc.push_back( SumError_Zpeak( Histo_DataDrivnBkg[i] ) );

			// printf("[%d bkg] (nEvent, AbsSystUnc) = (%.3lf, %.3lf)\n", i, nEvents_Zpeak( Histo_DataDrivnBkg[i] ), SumError_Zpeak( Histo_DataDrivnBkg[i] ) );
		}

		// -- MC-based backgrounds -- //
		Double_t RelUnc_xSec_WZ = 0.4052; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-011/index.html -- //
		Double_t RelUnc_xSec_ZZ = 0.1087; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-005/index.html -- //

		Double_t nEvent_WZ = this->nEvents_Zpeak( this->h_WZ );
		Double_t AbsStatUnc_WZ = this->SumError_Zpeak( this->h_WZ );
		Double_t AbsSystUnc_WZ = nEvent_WZ * RelUnc_xSec_WZ;
		Double_t AbsTotUnc_WZ = sqrt( AbsStatUnc_WZ*AbsStatUnc_WZ + AbsSystUnc_WZ*AbsSystUnc_WZ );

		nBkgEvent.push_back( nEvent_WZ );
		AbsSystUnc.push_back( AbsTotUnc_WZ );

		Double_t nEvent_ZZ = this->nEvents_Zpeak( this->h_ZZ );
		Double_t AbsStatUnc_ZZ = this->SumError_Zpeak( this->h_ZZ );
		Double_t AbsSystUnc_ZZ = nEvent_ZZ * RelUnc_xSec_ZZ;
		Double_t AbsTotUnc_ZZ = sqrt( AbsStatUnc_ZZ*AbsStatUnc_ZZ + AbsSystUnc_ZZ*AbsSystUnc_ZZ );

		nBkgEvent.push_back( nEvent_ZZ );
		AbsSystUnc.push_back( AbsTotUnc_ZZ );

		Int_t nBkg = (Int_t)nBkgEvent.size();
		Double_t Sum_nBkg = 0;
		Double_t QuadSum_AbsUnc = 0;
		for(Int_t i=0; i<nBkg; i++)
		{
			Sum_nBkg += nBkgEvent[i];
			QuadSum_AbsUnc += AbsSystUnc[i] * AbsSystUnc[i];
		}

		QuadSum_AbsUnc = sqrt(QuadSum_AbsUnc);

		// printf("\tttbar\tDYtautau\ttW\tDiJet\tWJet\tWW\tWZ\tZZ\n");
		printf("[%15s] %10s, %10s, %10s, %10s, %10s, %10s, %10s, %10s\n", "Category", "ttbar", "DYtautau", "tW", "DiJet", "WJet", "WW", "WZ", "ZZ");
		printf("[%15s] %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf = %10.2lf\n", 
				"# events", nBkgEvent[0], nBkgEvent[1], nBkgEvent[2], nBkgEvent[3], nBkgEvent[4], nBkgEvent[5], nBkgEvent[6], nBkgEvent[7], Sum_nBkg);
		printf("[%15s] %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf, %10.2lf = %10.2lf\n", 
				"AbsUnc", AbsSystUnc[0], AbsSystUnc[1], AbsSystUnc[2], AbsSystUnc[3], AbsSystUnc[4], AbsSystUnc[5], AbsSystUnc[6], AbsSystUnc[7], QuadSum_AbsUnc);

		this->RelSystUnc_BkgEst = QuadSum_AbsUnc / yield_total;
		cout << "===========================================================================================" << endl;
		printf("[Relative Uncertainty on the cross section from Bkg.Est. (Percent): %6.4lf]\n", RelSystUnc_BkgEst*100 );
		cout << "===========================================================================================\n\n" << endl;
	}

	void SetupHistgram_MCBkg_All()
	{
		this->h_WZ = this->SetupHistgram_MCBkg( "WZ" );
		this->h_ZZ = this->SetupHistgram_MCBkg( "ZZ" );
	}

	TH1D* SetupHistgram_MCBkg(TString Type )
	{
		Int_t nTag = (Int_t)Tag.size();
		Double_t normFactor = 0;

		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag[i_tag] == Type )
			{
				normFactor = (Xsec[i_tag] * Lumi) / nEvents[i_tag];
				printf( "[Normalization factor for %s = %lf]\n", Type.Data(), normFactor );
				break;
			}
		}

		TFile *f_MC = TFile::Open(this->FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root"); f_MC->cd();
		TH1D* h_Bkg = (TH1D*)f_MC->Get("h_mass_OS_"+Type)->Clone();
		// h_Bkg = (TH1D*)h_Bkg->Rebin(nMassBin, h_Bkg->GetName(), MassBinEdges); // -- doesn't need to Rebin -- //
		h_Bkg->Scale( normFactor );

		f_MC->Close();

		return h_Bkg;
	}

	void SetupHistogram_DataDrivenBkg()
	{
		TFile *f_DataDrivenBkg = TFile::Open(this->FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
		h_QCD = (TH1D*)f_DataDrivenBkg->Get("dijet")->Clone();
		h_WJets = (TH1D*)f_DataDrivenBkg->Get("wjets")->Clone();
		h_ttbar = (TH1D*)f_DataDrivenBkg->Get("ttbar")->Clone();
		h_tW = (TH1D*)f_DataDrivenBkg->Get("tW")->Clone();
		h_DYtautau = (TH1D*)f_DataDrivenBkg->Get("DYtautau")->Clone();
		h_WW = (TH1D*)f_DataDrivenBkg->Get("WW")->Clone();
		// h_diboson = (TH1D*)f_DataDrivenBkg->Get("diboson")->Clone();
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
		TFile* f_result = TFile::Open(this->FileLocation+"/ROOTFile_Results_DYAnalysis_76X.root");
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

	void Save_Results( TFile *f_output )
	{
		// Double_t nEvents_Observed; // -- for the estimation of stat. uncertainty -- //
		// Double_t yield_HLTv4p2;
		// Double_t yield_HLTv4p3;
		// Double_t yield_total;

		// Double_t Acc;
		// Double_t Eff;
		// Double_t Eff_HLTv4p2;
		// Double_t Eff_HLTv4p3;
		// Double_t EffSF_HLTv4p2;
		// Double_t EffSF_HLTv4p3;

		// Double_t yield_HLTv4p2_AccEff;
		// Double_t yield_HLTv4p3_AccEff;
		// Double_t yield_total_AccEff;

		// Double_t yield_HLTv4p2_EffSF;
		// Double_t yield_HLTv4p3_EffSF;
		// Double_t yield_total_EffSF;

		// Double_t xSec;

		// // -- for fiducial, post-FSR cross section -- //
		// Double_t FpoF_yield_HLTv4p2_Eff;
		// Double_t FpoF_yield_HLTv4p3_Eff;
		// Double_t FpoF_yield_Eff;

		// Double_t FpoF_yield_HLTv4p2_EffSF;
		// Double_t FpoF_yield_HLTv4p3_EffSF;
		// Double_t FpoF_yield_EffSF;

		// Double_t FpoF_xSec;

		f_output->cd();

		this->SaveAsTVector( this->nEvents_Observed, "nEvents_Observed", f_output );
		this->SaveAsTVector( this->yield_HLTv4p2, "yield_HLTv4p2", f_output );
		this->SaveAsTVector( this->yield_HLTv4p3, "yield_HLTv4p3", f_output );
		this->SaveAsTVector( this->yield_total, "yield_total", f_output );

		this->SaveAsTVector( this->Acc, "Acc", f_output );
		this->SaveAsTVector( this->Eff, "Eff", f_output );
		this->SaveAsTVector( this->Eff_HLTv4p2, "Eff_HLTv4p2", f_output );
		this->SaveAsTVector( this->Eff_HLTv4p3, "Eff_HLTv4p3", f_output );
		this->SaveAsTVector( this->EffSF_HLTv4p2, "EffSF_HLTv4p2", f_output );
		this->SaveAsTVector( this->EffSF_HLTv4p3, "EffSF_HLTv4p3", f_output );

		this->SaveAsTVector( this->yield_HLTv4p2_AccEff, "yield_HLTv4p2_AccEff", f_output );
		this->SaveAsTVector( this->yield_HLTv4p3_AccEff, "yield_HLTv4p3_AccEff", f_output );
		this->SaveAsTVector( this->yield_total_AccEff, "yield_total_AccEff", f_output );

		this->SaveAsTVector( this->yield_HLTv4p2_EffSF, "yield_HLTv4p2_EffSF", f_output );
		this->SaveAsTVector( this->yield_HLTv4p3_EffSF, "yield_HLTv4p3_EffSF", f_output );
		this->SaveAsTVector( this->yield_total_EffSF, "yield_total_EffSF", f_output );

		this->SaveAsTVector( this->xSec, "XSec", f_output );

		this->SaveAsTVector( this->RelStatError, "RelStatError", f_output );
		this->SaveAsTVector( this->RelLumiError, "RelLumiError", f_output );
		this->SaveAsTVector( this->RelSystUnc_BkgEst, "RelSystUnc_BkgEst", f_output );

		// -- for fiducial, post-FSR cross section -- //
		this->SaveAsTVector( this->FpoF_yield_HLTv4p2_Eff, "FpoF_yield_HLTv4p2_Eff", f_output );
		this->SaveAsTVector( this->FpoF_yield_HLTv4p3_Eff, "FpoF_yield_HLTv4p3_Eff", f_output );
		this->SaveAsTVector( this->FpoF_yield_Eff, "FpoF_yield_Eff", f_output );

		this->SaveAsTVector( this->FpoF_yield_HLTv4p2_EffSF, "FpoF_yield_HLTv4p2_EffSF", f_output );
		this->SaveAsTVector( this->FpoF_yield_HLTv4p3_EffSF, "FpoF_yield_HLTv4p3_EffSF", f_output );
		this->SaveAsTVector( this->FpoF_yield_EffSF, "FpoF_yield_EffSF", f_output );

		this->SaveAsTVector( this->FpoF_xSec, "FpoF_XSec", f_output );
	}

	void SaveAsTVector( Double_t var, TString Name, TFile *f_output )
	{
		TVectorD *Vec = new TVectorD(1);
		Vec[0] = var;

		f_output->cd();
		Vec->Write( Name );
	}
};









