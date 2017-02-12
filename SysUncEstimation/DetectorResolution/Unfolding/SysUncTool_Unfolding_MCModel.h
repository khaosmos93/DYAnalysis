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
#include <TRandom3.h>

#include <vector>


#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/RoccoR.cc"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/rochcor2015.cc"
#include "/home/kplee/Unfolding/src/RooUnfoldResponse.h"
#include "/home/kplee/Unfolding/src/RooUnfoldBayes.h"
#include "/home/kplee/Unfolding/src/RooUnfoldInvert.h"

#include "/home/kplee/CommonCodes/DrellYanAnalysis/DYAnalyzer.h"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/DiffXsecTools.h"

class MyDiffXsecTool : public DiffXsecTools
{
public:
	MyDiffXsecTool(TString version, TString Ver_CMSSW) : DiffXsecTools(version, Ver_CMSSW)
	{

	}

	void UnfoldingCorrection(RooUnfoldResponse* RooUnfoldRes)
	{
		if( f_Unfold == NULL )
			// f_Unfold = new TFile(FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root");
			f_Unfold = TFile::Open(FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root");
		
		f_Unfold->cd();
		h_totSignalMC_GenLevel_WithinAcc = (TH1D*)f_Unfold->Get("h_Truth_RooUnfold")->Clone();

		// -- HLTv4.2 -- //
		// RooUnfoldResponse *UnfoldRes_HLTv4p2 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		RooUnfoldResponse *UnfoldRes_HLTv4p2 = (RooUnfoldResponse*)RooUnfoldRes->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p2 = new RooUnfoldBayes(UnfoldRes_HLTv4p2, h_yield_HLTv4p2, 17);
		h_yield_HLTv4p2_Unfolded = (TH1D*)UnfoldBayes_HLTv4p2->Hreco();
		h_yield_HLTv4p2_Unfolded->SetName("h_yield_HLTv4p2_Unfolded");
		
		h_totSignalMC_GenLevel_WithinAcc_HLTv4p2 = (TH1D*)h_totSignalMC_GenLevel_WithinAcc->Clone();
		h_totSignalMC_GenLevel_WithinAcc_HLTv4p2->Scale( Lumi_HLTv4p2 / Lumi );

		// -- HLTv4.3 -- //
		// RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)RooUnfoldRes->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p3 = new RooUnfoldBayes(UnfoldRes_HLTv4p3, h_yield_HLTv4p3, 17);
		h_yield_HLTv4p3_Unfolded = (TH1D*)UnfoldBayes_HLTv4p3->Hreco();
		h_yield_HLTv4p3_Unfolded->SetName("h_yield_HLTv4p3_Unfolded");

		h_totSignalMC_GenLevel_WithinAcc_HLTv4p3 = (TH1D*)h_totSignalMC_GenLevel_WithinAcc->Clone();
		h_totSignalMC_GenLevel_WithinAcc_HLTv4p3->Scale( (Lumi - Lumi_HLTv4p2) / Lumi );

		delete f_Unfold;

		// -- Combined -- //
		h_yield_HLTv4p2_Unfolded->Sumw2(); h_yield_HLTv4p3_Unfolded->Sumw2();
		h_yield_Unfolded = (TH1D*)h_yield_HLTv4p2_Unfolded->Clone();
		h_yield_Unfolded->Add( h_yield_HLTv4p3_Unfolded );
	}
};

class SysUncTool_Unfolding_MCModel
{
public:
	TString version;
	TString FileLocation;
	Double_t MassBinEdges[nMassBin+1];

	TFile *f_output;
	TFile *f_FpoF_output;

	Bool_t isDataDriven;

	TH1D* h_GenMass;
	TH1D* h_RecoMass;
	TH2D* h_2D_nEvents;
	TH2D* h_2D_nEvents_NotNorm;
	TH2D* h_RespM;
	RooUnfoldResponse *UnfoldRes;
	TH1D* h_unfoldedMC;

	TH1D* h_RelSysUnc;

	Int_t nEvent_Test; // -- for test -- //


	SysUncTool_Unfolding_MCModel(TString _version)
	{
		version = _version;

		FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/"+version;
		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		isDataDriven = kTRUE;
		cout << "Default value for isDataDriven: " << isDataDriven << endl;

		f_output = NULL;
		f_FpoF_output = NULL;

		h_RelSysUnc = new TH1D("h_RelSysUnc", "", nMassBin, MassBinEdges);
		h_2D_nEvents_NotNorm = new TH2D("h_2D_nEvents_NotNorm", "", nMassBin, MassBinEdges, nMassBin, MassBinEdges);

		nEvent_Test = 0;
	}

	void GetUnfoldRes_FromROOTFile( TFile *file )
	{
		file->cd();
		UnfoldRes = (RooUnfoldResponse*)file->Get("UnfoldRes_MadgraphPowheg")->Clone();
	}

	void Calc_RespM_AltMC(Bool_t isCorrected = kTRUE, TString Sample = "MadgraphPowheg", TString HLTname = "IsoMu20_OR_IsoTkMu20")
	{
		if( Sample != "aMCNLO" && Sample != "Powheg" && Sample != "Madgraph" && Sample != "MadgraphPowheg")
		{
			cout << "ERROR: " << Sample << " is wrong Sample. Possible sample: aMCNLO, Powheg, Madgraph and MadgraphPowheg" << endl;
			return;
		}

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

		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();
		
		DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

		TH1D *h_GenMass_temp = new TH1D("h_GenMass_temp", "", nMassBin, MassBinEdges); // -- just for binning of RooUnfoldResponse -- //
		TH1D *h_RecoMass_temp = new TH1D("h_RecoMass_temp", "", nMassBin, MassBinEdges); // -- just for binning of RooUnfoldResponse -- //
		UnfoldRes = new RooUnfoldResponse(h_RecoMass_temp, h_GenMass_temp);

		TString BaseLocation = "/data4/Users/kplee/DYntuple";
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

		if( Sample == "aMCNLO" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}
		else if( Sample == "Powheg" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}
		else if( Sample == "Madgraph" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Madgraph", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}
		else if( Sample == "MadgraphPowheg" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("MadgraphPowheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}

		cout << "======================================================================" << endl;
		cout << "Alternative MC Sample for response matrix calculation: " << Sample << endl;
		cout << "======================================================================" << endl;

		// -- Loop for each sample -- //
		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			// if( !Tag[i_tup].Contains("DYMuMu") ) continue;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");

			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_GenLepton();
			ntuple->TurnOnBranches_Muon();

			rochcor2015 *rmcor = new rochcor2015();

			analyzer->SetupPileUpReWeighting_76X( Tag[i_tup] );

			Bool_t isNLO = 0;
			if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
			{
				isNLO = 1;
				cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
			}

			Int_t NEvents = chain->GetEntries();
			cout << "\t[Total Events: " << NEvents << "]" << endl;

			Double_t NormFactor = (Lumi * Xsec[i_tup]) / nEvents[i_tup];
			cout << "\t[Normalization factor: " << NormFactor << "]" << endl;

			if( nEvent_Test > 0 )
				NEvents = nEvent_Test;
			
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

				Int_t PU = ntuple->nPileUp;
				Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

				Double_t TotWeight = NormFactor * GenWeight * PUWeight;

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

				if( GenFlag == kTRUE )
				{
					/////////////////////////////////////
					// -- Generator level selection -- //
					/////////////////////////////////////
					vector< GenLepton > GenLeptonCollection;

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

					Bool_t isPassAcc_GenLepton = kFALSE;
					isPassAcc_GenLepton = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

					///////////////////////////////////////////
					// -- Reconstruction level selection -- //
					///////////////////////////////////////////
					Bool_t isPassEventSelection = kFALSE;

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
						}

						// -- Event Selection -- //
						vector< Muon > SelectedMuonCollection;
						isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

						if( isPassEventSelection == kTRUE )
						{
							Muon Mu1 = SelectedMuonCollection[0];
							Muon Mu2 = SelectedMuonCollection[1];
							Double_t reco_M = ( Mu1.Momentum + Mu2.Momentum ).M();

							if( isPassAcc_GenLepton == kTRUE )
							{
								UnfoldRes->Fill( reco_M, gen_M, TotWeight );
								h_2D_nEvents_NotNorm->Fill( gen_M, reco_M );
							}
							else // -- No gen-level event within the acceptance, but reco event exists (= "Fake" events) -- //
								UnfoldRes->Fake( reco_M, TotWeight );
						}

					} // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

				} // -- end of if( GenFlag == kTRUE ) -- //

			} // -- end of event iteration -- //

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;

		} // -- end of sample iteration -- //

		if( f_output == NULL )
			f_output = new TFile("ROOTFile_SysUnc_Unfolding_MCModel.root", "RECREATE");

		f_output->cd();

		UnfoldRes->SetName("UnfoldRes_"+Sample);
		UnfoldRes->Write();

		h_RecoMass = (TH1D*)UnfoldRes->Hmeasured();
		h_RecoMass->SetName("h_RecoMass");
		h_RecoMass->Write();

		h_GenMass = (TH1D*)UnfoldRes->Htruth();
		h_GenMass->SetName("h_GenMass");
		h_GenMass->Write();

		h_2D_nEvents = (TH2D*)UnfoldRes->Hresponse();
		h_2D_nEvents->SetName("h_2D_nEvents");
		h_2D_nEvents->Write();
		h_RespM = (TH2D*)h_2D_nEvents->Clone();
		this->CalculateFractionPerBin(h_2D_nEvents, h_GenMass, h_RespM);

		h_RespM->SetName("h_RespM");
		h_RespM->Write();

		// -- for the closure test -- //
		RooUnfoldBayes *UnfoldBayes = new RooUnfoldBayes(UnfoldRes, h_RecoMass, 17);
		h_unfoldedMC = (TH1D*)UnfoldBayes->Hreco();
		h_unfoldedMC->SetName("h_unfoldedMC");
		h_unfoldedMC->Write();

		h_2D_nEvents_NotNorm->Write();
	}

	void ClosureTest()
	{
		// MyCanvas *myc2 = new MyCanvas("c_RespM_DetRes", "Reco-level Dimuon Mass [GeV]", "Gen-level Dimuon Mass [GeV]");
		MyCanvas *myc2 = new MyCanvas("c_RespM_DetRes", "Gen-level Dimuon Mass [GeV]", "Reco-level Dimuon Mass [GeV]");
		myc2->SetLogx();
		myc2->SetLogy();
		myc2->SetLogz();
		myc2->SetXRange(15, 3000);
		myc2->SetYRange(15, 3000);
		myc2->SetZRange(1e-3, 1);
		myc2->SetTranspose(kTRUE);
		myc2->CanvasWith2DHistogram(h_RespM, "COLZ");
		myc2->PrintCanvas();

		MyCanvas *myc = new MyCanvas("c_ClosureTest", "Dimuon Mass [GeV]", "Number of events");
		myc->SetLogx();
		myc->SetLogy(0);
		myc->SetRatioRange(0.7, 1.3);
		myc->CanvasWithThreeHistogramsRatioPlot( h_RecoMass, h_unfoldedMC, h_GenMass, 
												 "Measured (Reco-Level)", "Unfolded", "Truth (Gen-Level)", "Ratio to Truth",
												 kBlue, kGreen+1, kRed);
		myc->PrintCanvas();

		// -- Check the detail number -- //
		TH1D *h_ratio = (TH1D*)(myc->h_ratio2)->Clone();
		Int_t nBin = h_ratio->GetNbinsX();
		Bool_t isPassTest = kTRUE;
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t ratio = h_ratio->GetBinContent(i_bin);
			if( (ratio-1) > 1e-5 )
			{
				Double_t lowerEdge = h_ratio->GetBinLowEdge(i_bin);
				Double_t upperEdge = h_ratio->GetBinLowEdge(i_bin+1);
				printf("[%2dth bin] (lowerEdge, upperEdge) = (%.3lf, %.3lf) (Unfolded/Truth) = %.6lf\n", i_bin, lowerEdge, upperEdge, ratio);
				isPassTest = kFALSE;
			}
		}
		cout << "==============================================" << endl;
		if( isPassTest ) 
			cout << "[Pass the closure test]" << endl;
		else
			cout << "[Closure test is FAILED! ... Check the details!]" << endl;
		cout << "==============================================" << endl;
	}

	void Calc_SysUnc()
	{
		TH1D* h_DiffXsec_AltMC = this->CalcDiffXsec_GivenRespM(UnfoldRes);
		h_DiffXsec_AltMC->SetName("h_DiffXsec_AltMC");

		TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
		TH1D* h_cv = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr");

		MyCanvas *myc = new MyCanvas("c_DiffXsec_Unfolding_DetRes_CentralMC_vs_AltMC", "Dimuon Mass [GeV]", "d#sigma/dM [pb/GeV]");
		myc->SetLogx(1);
		myc->SetLogy(0);
		myc->SetRatioRange(0.9, 1.1);
		myc->CanvasWithHistogramsRatioPlot( h_DiffXsec_AltMC, h_cv, "Unfolding(Det.Res) from Madgraph+Powheg", "Central value", "Mad+Pow/C.V.");
		myc->PrintCanvas();

		cout << "===============================================================" << endl;
		cout << "Relative uncertainty from unfolding correction (for Det.Res.)" << endl;
		cout << "===============================================================" << endl;
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t CV = h_cv->GetBinContent(i_bin);
			Double_t altV = h_DiffXsec_AltMC->GetBinContent(i_bin);

			Double_t RelSysUnc = fabs(CV - altV) / CV;

			printf("\t[%.1lf, %.1lf] %.3lf (%%)\n", MassBinEdges[i], MassBinEdges[i+1], RelSysUnc * 100 );

			h_RelSysUnc->SetBinContent(i_bin, RelSysUnc);
			h_RelSysUnc->SetBinError(i_bin, 0);
		}
		h_RelSysUnc->Scale( 100 );
		h_RelSysUnc->SetName("h_RelSysUnc_Unfolding_DetRes_AltMC_Percent");

		if( f_output == NULL )
			f_output = new TFile("ROOTFile_SysUnc_Unfolding_MCModel.root", "RECREATE");

		f_output->cd();
		h_DiffXsec_AltMC->Write();
		h_RelSysUnc->Write();
	}



	void CalculateFractionPerBin(TH2D *h_nEvents, TH1D* h_Truth, TH2D *h_Response)
	{
		Int_t nBins = h_Truth->GetNbinsX();

		for(Int_t i_genbin=0; i_genbin <= nBins+1; i_genbin++) // -- Include under/overflow -- //
		{
			Double_t SumN_truth = h_Truth->GetBinContent(i_genbin);

			for(Int_t i_recobin=0; i_recobin <= nBins+1; i_recobin++) // -- Include under/overflow -- //
			{
				Double_t nEvent = h_nEvents->GetBinContent(i_recobin, i_genbin);
				Double_t fraction = nEvent / SumN_truth;

				// if( fraction < 0 && fabs(fraction) < 1e-3 )
				if( fraction < 0 )
				{
					printf("\t[(i_Recobin, i_Genbin) = (%2.d, %2.d): has nagative bin! ... value = %.3lf => Set as 0 in the plot]\n", i_recobin, i_genbin, fraction);
					fraction = 0;
				}

				h_Response->SetBinContent( i_recobin, i_genbin, fraction );
			}
		}
	}

	// TH1D* CalcDiffXsec_GivenRespM(RooUnfoldResponse *RooUnfoldRes, TH1D *h_DiffXsec)
	TH1D* CalcDiffXsec_GivenRespM(RooUnfoldResponse *RooUnfoldRes)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, "76X");
		XsecTool->MakeSignalMCHistograms();

		XsecTool->GetYieldHistograms( isDataDriven );

		XsecTool->UnfoldingCorrection(RooUnfoldRes);
		XsecTool->AccEffCorrection();
		XsecTool->EfficiencyScaleFactor();
		XsecTool->FSRCorrection();
		XsecTool->CalcXsec();

		// CopyHistogram( XsecTool->h_DiffXsec_FSRCorr, h_DiffXsec );
		return XsecTool->h_DiffXsec_FSRCorr;
	}

	void FpoF_Calc_SysUnc()
	{
		TH1D* h_FpoF_DiffXsec_AltMC = this->FpoF_CalcDiffXsec_GivenRespM(UnfoldRes);
		h_FpoF_DiffXsec_AltMC->SetName("h_FpoF_DiffXsec_AltMC");

		TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
		TH1D* h_cv = (TH1D*)f_cv->Get("h_FpoF_DiffXsec_Data");

		MyCanvas *myc = new MyCanvas("c_FpoF_DiffXsec_Unfolding_DetRes_CentralMC_vs_AltMC", "Dimuon Mass (fiducial, post-FSR) [GeV]", "d#sigma/dM [pb/GeV]");
		myc->SetLogx(1);
		myc->SetLogy(0);
		myc->SetRatioRange(0.9, 1.1);
		myc->CanvasWithHistogramsRatioPlot( h_FpoF_DiffXsec_AltMC, h_cv, "Unfolding(Det.Res) from Madgraph+Powheg", "Central value", "Mad+Pow/C.V.");
		myc->PrintCanvas();

		cout << "===============================================================" << endl;
		cout << "Relative uncertainty from unfolding correction (for Det.Res.)" << endl;
		cout << "===============================================================" << endl;
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t CV = h_cv->GetBinContent(i_bin);
			Double_t altV = h_FpoF_DiffXsec_AltMC->GetBinContent(i_bin);

			Double_t RelSysUnc = fabs(CV - altV) / CV;

			printf("\t[%.1lf, %.1lf] %.3lf (%%)\n", MassBinEdges[i], MassBinEdges[i+1], RelSysUnc * 100 );

			h_RelSysUnc->SetBinContent(i_bin, RelSysUnc);
			h_RelSysUnc->SetBinError(i_bin, 0);
		}
		h_RelSysUnc->Scale( 100 );
		h_RelSysUnc->SetName("h_RelSysUnc_Unfolding_DetRes_AltMC_Percent");

		if( f_FpoF_output == NULL )
			f_FpoF_output = new TFile("ROOTFile_FpoF_SysUnc_Unfolding_MCModel.root", "RECREATE");

		f_FpoF_output->cd();
		h_FpoF_DiffXsec_AltMC->Write();
		h_RelSysUnc->Write();
	}

	TH1D* FpoF_CalcDiffXsec_GivenRespM(RooUnfoldResponse *RooUnfoldRes)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, "76X");
		XsecTool->MakeSignalMCHistograms();
		XsecTool->GetYieldHistograms( isDataDriven );
		XsecTool->UnfoldingCorrection(RooUnfoldRes);

		XsecTool->FpoF_GetTheoryHist();
		XsecTool->FpoF_EffCorrection();
		XsecTool->FpoF_EfficiencyScaleFactor();
		XsecTool->FpoF_CalcXsec();

		return XsecTool->h_FpoF_DiffXsec_Data;
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

