#include "../ZpeakCrossSectionTool.h"
#include <SysUncEstimation/EfficiencyScaleFactor/BinningChoice/SysUncTool_BinningChoice.h>

class ZpeakSysUncTool_BinningChoice
{
public:
	TString FileLocation;
	TString HLTname;
	DYAnalyzer *analyzer;

	vector< TString > ntupleDirectory; 
	vector< TString > Tag; 
	vector< Double_t > Xsec; 
	vector< Double_t > nEvents;

	SysUncTool_BinningChoice *SysTool;

	Bool_t isDataDriven;

	TString FileName_CV;
	TString FileName_DB; // -- DB = different binning -- //

	Double_t Acc;
	Double_t Eff;
	Double_t EffSF_HLTv4p2_CV;
	Double_t EffSF_HLTv4p3_CV;

	Double_t EffSF_HLTv4p2_DB; 
	Double_t EffSF_HLTv4p3_DB;

	Double_t xSec_CV;
	Double_t xSec_DB;
	Double_t RelSysUnc;

	ZpeakSysUncTool_BinningChoice(TString _HLTname)
	{
		FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH"); // -- 76X -- //

		HLTname = _HLTname;
		analyzer = new DYAnalyzer( HLTname );
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X samples -- //

		// -- Initialization -- //
		isDataDriven = kFALSE;

		Acc = 0;
		Eff = 0;

		EffSF_HLTv4p2_CV = 0;
		EffSF_HLTv4p3_CV = 0;

		EffSF_HLTv4p2_DB = 0;
		EffSF_HLTv4p3_DB = 0;

		xSec_CV = 0;
		xSec_DB = 0;

		RelSysUnc = 0;
	}

	void SetIsDataDriven(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;
		cout << "\n=================================" << endl;
		cout << "isDataDriven = " << isDataDriven << endl;
		cout << "=================================\n\n" << endl;
	}

	void SetROOTFiles_TnPEff(TString _FileName_CV, TString _FileName_DB) // -- DB = Different binning -- //
	{
		FileName_CV = _FileName_CV;
		FileName_DB = _FileName_DB;

		cout << "\n===========================================================" << endl;
		cout << "T&P Efficiency for the central value: " << FileName_CV << endl;
		cout << "T&P Efficiency for the alternative binnnig: " << FileName_DB << endl;
		cout << "===========================================================\n" << endl;

	}

	void SetSysUncTool(TString Type, Double_t nPtBin, Double_t *PtBinEdges, Int_t nEtaBin, Double_t *EtaBinEdges)
	{
		SysTool = new SysUncTool_BinningChoice(Type, version, "76X");
		SysTool->SetupBinning(nPtBin, PtBinEdges, nEtaBin, EtaBinEdges);
		SysTool->SetupEfficiencyScaleFactor( "./" + FileName_DB );
	}


	void Calc_EffSF(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO")
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
		cout << "Sample: " << Sample << endl;

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

		analyzer->SetupEfficiencyScaleFactor( FileName_CV );

		Double_t Count_Acc_Total = 0;
		Double_t Count_Acc_Pass = 0;

		Double_t Count_Eff_Total = 0;
		Double_t Count_Eff_Pass = 0;

		Double_t Count_Eff_Pass_Corr_HLTv4p2_CV = 0;
		Double_t Count_Eff_Pass_Corr_HLTv4p3_CV = 0;

		// -- DB = Different Binning -- //
		Double_t Count_Eff_Pass_Corr_HLTv4p2_DB = 0;
		Double_t Count_Eff_Pass_Corr_HLTv4p3_DB = 0;

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

						Double_t Eff_SF_HLTv4p2_CV = -999; // -- Efficiency correction factor -- //
						Double_t Eff_SF_HLTv4p3_CV = -999; // -- Efficiency correction factor -- //

						Double_t Eff_SF_HLTv4p2_DB = -999;
						Double_t Eff_SF_HLTv4p3_DB = -999;

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
									Muon DYmuon1 = SelectedMuonCollection[0];
									Muon DYmuon2 = SelectedMuonCollection[1];
									Eff_SF_HLTv4p2_CV = analyzer->EfficiencySF_EventWeight_HLTv4p2( DYmuon1, DYmuon2 );
									Eff_SF_HLTv4p3_CV = analyzer->EfficiencySF_EventWeight_HLTv4p3( DYmuon1, DYmuon2 );

									Eff_SF_HLTv4p2_DB = SysTool->EfficiencySF_EventWeight_HLTv4p2( DYmuon1, DYmuon2 );
									Eff_SF_HLTv4p3_DB = SysTool->EfficiencySF_EventWeight_HLTv4p3( DYmuon1, DYmuon2 );

									Flag_PassEff = kTRUE;
									Flag_PassAccEff = kTRUE;
								}

							} // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

							// -- Efficiency Calculation -- //
							if( Flag_PassEff == kTRUE)
							{
								Count_Eff_Total += TotWeight;
								Count_Eff_Pass += TotWeight;

								Count_Eff_Pass_Corr_HLTv4p2_CV += TotWeight * Eff_SF_HLTv4p2_CV;
								Count_Eff_Pass_Corr_HLTv4p3_CV += TotWeight * Eff_SF_HLTv4p3_CV;

								Count_Eff_Pass_Corr_HLTv4p2_DB += TotWeight * Eff_SF_HLTv4p2_DB;
								Count_Eff_Pass_Corr_HLTv4p3_DB += TotWeight * Eff_SF_HLTv4p3_DB;

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

		cout << "[Central values]" << endl;

		Double_t Eff_HLTv4p2_CV = (Double_t)Count_Eff_Pass_Corr_HLTv4p2_CV / Count_Eff_Total;
		Double_t Eff_HLTv4p3_CV = (Double_t)Count_Eff_Pass_Corr_HLTv4p3_CV / Count_Eff_Total;
		printf("Efficiency (SF_weighted, HLTv4.2) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p2_CV, Count_Eff_Total, Eff_HLTv4p2_CV);
		printf("Efficiency (SF_weighted, HLTv4.3) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p3_CV, Count_Eff_Total, Eff_HLTv4p3_CV);

		EffSF_HLTv4p2_CV = Eff_HLTv4p2_CV / Eff;
		EffSF_HLTv4p3_CV = Eff_HLTv4p3_CV / Eff;
		printf("Efficiency scale factor (HLTv4.2) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p2_CV, Eff, EffSF_HLTv4p2_CV);
		printf("Efficiency scale factor (HLTv4.3) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p3_CV, Eff, EffSF_HLTv4p3_CV);

		cout << endl;

		cout << "[Altenative values from different binning choice for T&P efficiency]" << endl;

		Double_t Eff_HLTv4p2_DB = (Double_t)Count_Eff_Pass_Corr_HLTv4p2_DB / Count_Eff_Total;
		Double_t Eff_HLTv4p3_DB = (Double_t)Count_Eff_Pass_Corr_HLTv4p3_DB / Count_Eff_Total;
		printf("Efficiency (SF_weighted, HLTv4.2) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p2_DB, Count_Eff_Total, Eff_HLTv4p2_DB);
		printf("Efficiency (SF_weighted, HLTv4.3) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p3_DB, Count_Eff_Total, Eff_HLTv4p3_DB);

		EffSF_HLTv4p2_DB = Eff_HLTv4p2_DB / Eff;
		EffSF_HLTv4p3_DB = Eff_HLTv4p3_DB / Eff;
		printf("Efficiency scale factor (HLTv4.2) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p2_DB, Eff, EffSF_HLTv4p2_DB);
		printf("Efficiency scale factor (HLTv4.3) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p3_DB, Eff, EffSF_HLTv4p3_DB);

		cout << endl;

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

	void Calc_ZpeakXsec_All()
	{
		cout << "=================================================" <<endl;
		cout << "< Start of the calculation of the cross section >" <<endl;
		cout << "=================================================" <<endl;

		cout << "[Central value]" << endl;
		xSec_CV = Calc_ZpeakXsec_GivenEffSF(Acc, Eff, EffSF_HLTv4p2_CV, EffSF_HLTv4p3_CV);
		cout << endl;

		cout << "[Altenative values from different binning choice for T&P efficiency]" << endl;
		xSec_DB = Calc_ZpeakXsec_GivenEffSF(Acc, Eff, EffSF_HLTv4p2_DB, EffSF_HLTv4p3_DB);
		cout << endl;

		RelSysUnc = fabs(xSec_CV - xSec_DB) / xSec_CV;

		cout << "====================================================================" <<endl;
		printf("[Relative Systematic uncertainty from this binning choice: %.5lf (%%)]\n", RelSysUnc*100);
		cout << "====================================================================" <<endl;
	}

	Double_t Calc_ZpeakXsec_GivenEffSF(Double_t _Acc, Double_t _Eff, Double_t EffSF_HLTv4p2, Double_t EffSF_HLTv4p3)
	{
		ZpeakCrossSectionTool *XsecTool = new ZpeakCrossSectionTool(version, HLTname);
		XsecTool->ObtainYield( isDataDriven );
		XsecTool->ApplyAccEffCorrection(_Acc, _Eff);
		XsecTool->EfficiencyScaleFactor(EffSF_HLTv4p2, EffSF_HLTv4p3);
		XsecTool->CalcXSec();

		return XsecTool->xSec;
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