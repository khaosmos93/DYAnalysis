#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "TTree.h"

#include "../ZpeakCrossSectionTool.h"
#include <SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SysUncTool_EffCorr.h>

using namespace RooFit;

class MyZpeakXsecTool : public ZpeakCrossSectionTool
{
public:
	MyZpeakXsecTool(TString _HLTname) : ZpeakCrossSectionTool( _HLTname )
	{

	}
};

class ZpeakSysUncTool_TagProbe
{
public:
	SysUncTool_EffCorr *SysTool;

	TString HLTname;
	TString FileLocation;

	DYAnalyzer *analyzer;

	vector< TString > ntupleDirectory; 
	vector< TString > Tag; 
	vector< Double_t > Xsec; 
	vector< Double_t > nEvents;

	Bool_t isDataDriven;

	TString ROOTFile_TnPEff_CV;
	TString ROOTFile_TnPEff_SysUnc;

	Double_t Acc;
	Double_t Eff;

	Double_t EffSF_HLTv4p2_CV;
	Double_t EffSF_HLTv4p3_CV;

	Double_t EffSF_HLTv4p2[nEffMap]; // -- nEffMap: defined in SysUncTool_EffCorr.h -- //
	Double_t EffSF_HLTv4p3[nEffMap];

	Double_t xSec_CV;
	Double_t xSec[nEffMap];

	TH1D *h_RelDiff;

	TFile *f_output;

	Double_t RelSysUnc;

	ZpeakSysUncTool_TagProbe(TString _HLTname)
	{
		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");

		this->HLTname = _HLTname;
		this->analyzer = new DYAnalyzer( HLTname );
		this->analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X samples -- //

		// -- Initialization -- //

		this->EffSF_HLTv4p2_CV = 0;
		this->EffSF_HLTv4p3_CV = 0;
		this->xSec_CV = 0;

		for(Int_t i=0; i<nEffMap; i++)
		{
			this->EffSF_HLTv4p2[i] = 0;
			this->EffSF_HLTv4p3[i] = 0;
			this->xSec[i] = 0;
		}

		this->ROOTFile_TnPEff_CV = "";
		this->ROOTFile_TnPEff_SysUnc = "";	

		this->isDataDriven = kFALSE;

		this->h_RelDiff = new TH1D("h_RelDiff", "", 5000, -1, 1);
		this->f_output = new TFile("ROOTFile_Zpeak_SysUnc_TagProbe.root", "RECREATE");

		this->RelSysUnc = 0;
	}

	void SetIsDataDriven(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;
		cout << "\n=================================" << endl;
		cout << "isDataDriven = " << isDataDriven << endl;
		cout << "=================================\n\n" << endl;

	}

	void SetupROOTFile_TagProbeEff(TString _ROOTFile_TnPEff_CV, TString _ROOTFile_TnPEff_SysUnc)
	{
		ROOTFile_TnPEff_CV = _ROOTFile_TnPEff_CV;
		ROOTFile_TnPEff_SysUnc = _ROOTFile_TnPEff_SysUnc;

		SysTool = new SysUncTool_EffCorr();
		SysTool->SetupCentralValueStatError( ROOTFile_TnPEff_CV );
		SysTool->SetUpSysUnc( ROOTFile_TnPEff_SysUnc );
		SysTool->CalcTotalUnc();
		SysTool->MakeSmearedEffMap();
	}

	void Calc_EffSF_AllMap(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO")
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

		analyzer->SetupEfficiencyScaleFactor( ROOTFile_TnPEff_CV );

		Double_t Count_Acc_Total = 0;
		Double_t Count_Acc_Pass = 0;

		Double_t Count_Eff_Total = 0;
		Double_t Count_Eff_Pass = 0;

		Double_t Count_Eff_Pass_Corr_HLTv4p2_CV = 0;
		Double_t Count_Eff_Pass_Corr_HLTv4p3_CV = 0;

		Double_t Count_Eff_Pass_Corr_HLTv4p2[nEffMap] = {0};
		Double_t Count_Eff_Pass_Corr_HLTv4p3[nEffMap] = {0};

		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			if( !(Tag[i_tup] == "DYMuMu_M50to100" || Tag[i_tup] == "DYMuMu_M100to200") ) continue;

			TStopwatch looptime;
			looptime.Start();

			TChain *chain = new TChain("recoTree/DYTree");
			TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_HLT():
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

						Double_t Eff_SF_HLTv4p2[nEffMap] = {-999}; // -- Efficiency correction factor for each smeared map -- //
						Double_t Eff_SF_HLTv4p3[nEffMap] = {-999}; // -- Efficiency correction factor for each smeared map -- //

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

									for(Int_t i_map=0; i_map<nEffMap; i_map++)
									{
										Eff_SF_HLTv4p2[i_map] = SysTool->EfficiencySF_Smeared_EventWeight_HLTv4p2( i_map, DYmuon1, DYmuon2 );
										Eff_SF_HLTv4p3[i_map] = SysTool->EfficiencySF_Smeared_EventWeight_HLTv4p3( i_map, DYmuon1, DYmuon2 );
									}

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

								Count_Eff_Pass_Corr_HLTv4p2_CV += TotWeight * Eff_SF_HLTv4p2_CV;
								Count_Eff_Pass_Corr_HLTv4p3_CV += TotWeight * Eff_SF_HLTv4p3_CV;

								for(Int_t i_map=0; i_map<nEffMap; i_map++)
								{
									Count_Eff_Pass_Corr_HLTv4p2[i_map] += TotWeight * Eff_SF_HLTv4p2[i_map];
									Count_Eff_Pass_Corr_HLTv4p3[i_map] += TotWeight * Eff_SF_HLTv4p3[i_map];
								}
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


		cout << "[Central values]" << endl;

		Acc = (Double_t)Count_Acc_Pass / Count_Acc_Total;
		Eff = (Double_t)Count_Eff_Pass / Count_Eff_Total;
		printf("Acceptance = %.3lf / %.3lf = %.6lf\n", Count_Acc_Pass, Count_Acc_Total, Acc);
		printf("Efficiency = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass, Count_Eff_Total, Eff);
		printf("Acceptance * Efficiency = %.6lf\n", Acc*Eff );

		Double_t Eff_HLTv4p2_CV = (Double_t)Count_Eff_Pass_Corr_HLTv4p2_CV / Count_Eff_Total;
		Double_t Eff_HLTv4p3_CV = (Double_t)Count_Eff_Pass_Corr_HLTv4p3_CV / Count_Eff_Total;
		printf("Efficiency (SF_weighted, HLTv4.2) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p2_CV, Count_Eff_Total, Eff_HLTv4p2_CV);
		printf("Efficiency (SF_weighted, HLTv4.3) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p3_CV, Count_Eff_Total, Eff_HLTv4p3_CV);

		EffSF_HLTv4p2_CV = Eff_HLTv4p2_CV / Eff;
		EffSF_HLTv4p3_CV = Eff_HLTv4p3_CV / Eff;
		printf("Efficiency scale factor (HLTv4.2) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p2_CV, Eff, EffSF_HLTv4p2_CV);
		printf("Efficiency scale factor (HLTv4.3) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p3_CV, Eff, EffSF_HLTv4p3_CV);

		cout << endl;

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			printf("[%d smeared map]\n", i_map);
			Double_t Eff_HLTv4p2 = (Double_t)Count_Eff_Pass_Corr_HLTv4p2[i_map] / Count_Eff_Total;
			Double_t Eff_HLTv4p3 = (Double_t)Count_Eff_Pass_Corr_HLTv4p3[i_map] / Count_Eff_Total;
			printf("Efficiency (SF_weighted, HLTv4.2) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p2[i_map], Count_Eff_Total, Eff_HLTv4p2);
			printf("Efficiency (SF_weighted, HLTv4.3) = %.3lf / %.3lf = %.6lf\n", Count_Eff_Pass_Corr_HLTv4p3[i_map], Count_Eff_Total, Eff_HLTv4p3);

			EffSF_HLTv4p2[i_map] = Eff_HLTv4p2 / Eff;
			EffSF_HLTv4p3[i_map] = Eff_HLTv4p3 / Eff;
			printf("Efficiency scale factor (HLTv4.2) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p2, Eff, EffSF_HLTv4p2[i_map]);
			printf("Efficiency scale factor (HLTv4.3) = %.6lf / %.6lf = %.6lf\n", Eff_HLTv4p3, Eff, EffSF_HLTv4p3[i_map]);

			cout << endl;
		}

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

	void Calc_ZpeakXsec_AllMap()
	{
		cout << "=================================================" <<endl;
		cout << "< Start of the calculation of the cross section >" <<endl;
		cout << "=================================================" <<endl;

		cout << "[Central value]" << endl;
		xSec_CV = this->Calc_ZpeakXsec_GivenEffSF(Acc, Eff, EffSF_HLTv4p2_CV, EffSF_HLTv4p3_CV);
		cout << endl;

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			cout << Form("[%d map]", i_map) << endl;
			xSec[i_map] = this->Calc_ZpeakXsec_GivenEffSF(Acc, Eff, EffSF_HLTv4p2[i_map], EffSF_HLTv4p3[i_map]);
		}
	}

	void Calc_SysUnc()
	{
		TTree *tree = makeTTree();

		RooRealVar RelDiff("RelDiff","(#sigma_{Central} - #sigma_{Smeared}) / #sigma_{Central}", -0.1, 0.1);
		RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

		// Make plot of binned dataset showing Poisson error bars (RooFit default)
		RooPlot* frame = RelDiff.frame(Title("Relative difference between central and smeared value (Z-peak x-section)") );
		
		// Fit a Gaussian p.d.f to the data
		RooRealVar mean("mean", "mean", 0, -0.5, 0.5) ;
		RooRealVar sigma("sigma", "sigma", 0.01, 0.0001, 1);
		RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
		gauss.fitTo(data);

		data.plotOn(frame, Binning(50));
		gauss.plotOn(frame);

		TCanvas *c = new TCanvas("c_RelDiff_ZpeakXsec", "", 800, 800); c->cd();
		frame->Draw();

		TString CanvasName = c->GetName();
		c->SaveAs(CanvasName+".pdf");
		
		f_output->cd();

		tree->Write();
		c->Write();

		RelSysUnc = sigma.getVal();

		cout << "====================================================================" <<endl;
		printf("[Relative Systematic uncertainty from Tag&Probe method: %.5lf (%%)]\n", RelSysUnc*100);
		cout << "====================================================================" <<endl;
	}

	TTree* makeTTree()
	{
		TTree* tree = new TTree("tree","tree");

		Double_t* RelDiff = new Double_t;

		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

		for(Int_t i_map=0; i_map<nEffMap; i_map++)
		{
			*RelDiff = ( xSec_CV - xSec[i_map] ) / xSec_CV;
			tree->Fill();
		}

		return tree;
	}

	Double_t Calc_ZpeakXsec_GivenEffSF(Double_t _Acc, Double_t _Eff, Double_t EffSF_HLTv4p2, Double_t EffSF_HLTv4p3)
	{
		MyZpeakXsecTool *XsecTool = new MyZpeakXsecTool();
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

	void Save_Results( TFile *f_output )
	{
		this->SaveAsTVector( this->xSec_CV, xSec_CV, f_output );

		f_output->cd();
		TVectorD *XSec_Smeared = new TVectorD(nEffMap);
		for(Int_t i=0; i<nEffMap; i++)
			XSec_Smeared[i] = this->xSec[i];
		XSec_Smeared->Write( "XSec_Smeared" );

		this->h_RelDiff->Write("h_RelDiff");

		this->SaveAsTVector( this->RelSysUnc, "RelSystUnc", f_output );
	}

	void SaveAsTVector( Double_t var, TString Name, TFile *f_output )
	{
		TVectorD *Vec = new TVectorD(1);
		Vec[0] = var;

		f_output->cd();
		Vec->Write( Name );
	}
};

