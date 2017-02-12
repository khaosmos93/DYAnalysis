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

// -- for Rochester Muon momentum correction -- //
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/RoccoR.cc"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/rochcor2015.cc"

#include </home/kplee/Unfolding/src/RooUnfoldResponse.h>
#include </home/kplee/Unfolding/src/RooUnfoldBayes.h>
#include </home/kplee/Unfolding/src/RooUnfoldInvert.h>

#include "/home/kplee/CommonCodes/DrellYanAnalysis/MyCanvas.C"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/DYAnalyzer.h"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/DiffXsecTools.h"

#define nMomCorrMap 500

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
		RooUnfoldBayes *UnfoldBayes_HLTv4p2 = new RooUnfoldBayes(UnfoldRes_HLTv4p2, h_yield_HLTv4p2, 4);
		h_yield_HLTv4p2_Unfolded = (TH1D*)UnfoldBayes_HLTv4p2->Hreco();
		h_yield_HLTv4p2_Unfolded->SetName("h_yield_HLTv4p2_Unfolded");
		
		h_totSignalMC_GenLevel_WithinAcc_HLTv4p2 = (TH1D*)h_totSignalMC_GenLevel_WithinAcc->Clone();
		h_totSignalMC_GenLevel_WithinAcc_HLTv4p2->Scale( Lumi_HLTv4p2 / Lumi );

		// -- HLTv4.3 -- //
		// RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)RooUnfoldRes->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p3 = new RooUnfoldBayes(UnfoldRes_HLTv4p3, h_yield_HLTv4p3, 4);
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

class SysUncTool_MCMomCorr
{
public:
	TString version;
	TString Ver_CMSSW;
	TString FileLocation;
	Double_t MassBinEdges[nMassBin+1];

	TFile *f_output;

	Bool_t isDataDriven;

	// -- for central values -- //
	TH1D *h_GenMass; // -- just for the binning for RooUnfoldResponse Object -- //
	TH1D *h_RecoMass; // -- just for the binning for RooUnfoldResponse Object -- //
	RooUnfoldResponse* RooUnfoldRes_CV;

	// -- for smeared values -- //
	RooUnfoldResponse* RooUnfoldRes_Smeared[nMomCorrMap];

	TH1D *h_DiffXsec_CV;
	TH1D *h_DiffXsec_Smeared[nMomCorrMap];

	TH1D *h_RelDiff_massBin[nMassBin];

	// -- fiducial, post-FSR -- //
	TFile *f_FpoF_ouptut;
	TH1D *h_FpoF_DiffXsec_CV;
	TH1D *h_FpoF_DiffXsec_Smeared[nMomCorrMap];


	SysUncTool_MCMomCorr(TString _version, TString _Ver_CMSSW)
	{
		cout << "============================================" << endl;
		cout << "[Number of iteration: " << nMomCorrMap << "]" << endl;
		cout << "============================================\n" << endl;

		version = _version;
		Ver_CMSSW = _Ver_CMSSW;

		if( Ver_CMSSW == "74X" )
			FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles/"+version;
		else if( Ver_CMSSW == "76X" )
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

		// -- Setup the histogram & RooUnfoldResponse -- //
		h_GenMass = new TH1D("h_GenMass", "", nMassBin, MassBinEdges);
		h_RecoMass = new TH1D("h_RecoMass", "", nMassBin, MassBinEdges);
		RooUnfoldRes_CV = new RooUnfoldResponse(h_RecoMass, h_GenMass);
		RooUnfoldRes_CV->SetName("RooUnfoldRes_CV");

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			RooUnfoldRes_Smeared[i_corrMap] = new RooUnfoldResponse(h_RecoMass, h_GenMass);
			RooUnfoldRes_Smeared[i_corrMap]->SetName("RooUnfoldRes_Smeared_"+TString::Format("%d", i_corrMap));
		}

		h_DiffXsec_CV = new TH1D("h_DiffXsec_CV", "", nMassBin, MassBinEdges);
		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			h_DiffXsec_Smeared[i_corrMap] = new TH1D("h_DiffXsec_Smeared_"+TString::Format("%d", i_corrMap), "", nMassBin, MassBinEdges);
		
		// -- Setting the h_RelDiff_massBin -- //
		for(Int_t i=0; i<nMassBin; i++)
			h_RelDiff_massBin[i] = new TH1D("h_RelDiff_massBin_"+TString::Format("%d", i), "", 10000, -5, 5);

		h_FpoF_DiffXsec_CV = new TH1D("h_FpoF_DiffXsec_CV", "", nMassBin, MassBinEdges);
		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			h_FpoF_DiffXsec_Smeared[i_corrMap] = new TH1D("h_FpoF_DiffXsec_Smeared_"+TString::Format("%d", i_corrMap), "", nMassBin, MassBinEdges);

		f_output = NULL;
		f_FpoF_ouptut = NULL;
	}

	void GetRooUnfoldRes_fromROOTFile( TFile *file )
	{
		file->cd();

		RooUnfoldRes_CV = (RooUnfoldResponse*)file->Get("RooUnfoldRes_CV")->Clone();

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			RooUnfoldRes_Smeared[i_corrMap] = (RooUnfoldResponse*)file->Get( "RooUnfoldRes_Smeared_"+TString::Format("%d", i_corrMap) )->Clone();
		}

		cout << "Load the response matrices from the input file" << endl;
	}

	void SetIsDataDriven(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;
		cout << "\n=================================" << endl;
		cout << "isDataDriven = " << isDataDriven << endl;
		cout << "=================================\n\n" << endl;

	}

	void Calc_ResponseMatrix(TString Sample = "aMCNLO", TString HLTname = "IsoMu20_OR_IsoTkMu20")
	{
		if( Sample != "aMCNLO" && Sample != "Powheg" )
		{
			cout << "ERROR: " << Sample << " is wrong Sample. Possible sample: aMCNLO, Powheg" << endl;
			return;
		}

		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

		TString BaseLocation = "/data4/Users/kplee/DYntuple";
		//Each ntuple directory & corresponding Tags
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

		if( Sample == "aMCNLO" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}
		if( Sample == "Powheg" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}

		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");

			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_GenLepton();
			ntuple->TurnOnBranches_Muon();

			rochcor2015 *rmcor_CV = new rochcor2015(); // -- for central value -- //

			rochcor2015 *rmcor_Smeared[nMomCorrMap];
			for(Int_t i_corrMap=0; i_corrMap < nMomCorrMap; i_corrMap++)
				rmcor_Smeared[i_corrMap] = new rochcor2015(i_corrMap); // -- Assign different random seed for each rochcor2015 object -- //

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

			// if( NEvents > 1000000 ) NEvents = 1000000;

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
					if( ntuple->isTriggered( analyzer->HLT ) )
					{
						FillRooUnfoldResponse_GivenCorr(rmcor_CV, ntuple, analyzer, 
							isPassAcc_GenLepton, gen_M, TotWeight, RooUnfoldRes_CV);

						for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
							FillRooUnfoldResponse_GivenCorr(rmcor_Smeared[i_corrMap], ntuple, analyzer, 
								isPassAcc_GenLepton, gen_M, TotWeight, RooUnfoldRes_Smeared[i_corrMap]);
					}

				} // -- end of if( GenFlag == kTRUE ) -- //

			} // -- end of event iteration -- //

		} // -- end of sample iteration -- //
		
	}

	void Calc_DiffXsec_All()
	{
		CalcDiffXsec_GivenRespM(RooUnfoldRes_CV, h_DiffXsec_CV);

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			CalcDiffXsec_GivenRespM( RooUnfoldRes_Smeared[i_corrMap], h_DiffXsec_Smeared[i_corrMap] );
	}

	void Calc_SysUnc()
	{
		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			TH1D *h_diff_CV_Smeared = (TH1D*)h_DiffXsec_CV->Clone();
			h_diff_CV_Smeared->Add( h_DiffXsec_Smeared[i_corrMap], -1 ); // -- (Central value - Smeared value) -- //

			for(Int_t i=0; i<nMassBin; i++)
			{
				Int_t i_bin = i+1;
				Double_t diff = h_diff_CV_Smeared->GetBinContent(i_bin);
				Double_t CentralValue = h_DiffXsec_CV->GetBinContent(i_bin);
				h_RelDiff_massBin[i]->Fill( diff / CentralValue );

				printf("\t[%d mass bin] RelDiff = %9.6e\n", i_bin, diff / CentralValue);
			}
		}
	}
	void SaveResults()
	{
		if( f_output == NULL )
			f_output = new TFile("ROOTFile_Histograms_SysUnc_MCMomCorr.root", "RECREATE");

		f_output->cd();

		RooUnfoldRes_CV->Write();
		h_DiffXsec_CV->Write();

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			RooUnfoldRes_Smeared[i_corrMap]->Write();
			h_DiffXsec_Smeared[i_corrMap]->Write();
		}

		for(Int_t i=0; i<nMassBin; i++)
			h_RelDiff_massBin[i]->Write();
	}

	void ValidationPlots()
	{
		TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_cv->cd();
		TH1D *h_DiffXsec_Ref = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr")->Clone();

		MyCanvas *myc_DiffXsec = new MyCanvas("c_DiffXsec_Ref_vs_CV_SysUnTool_MCMomCorr", "Dimuon Mass [GeV]", "d#sigma/dM [GeV]");
		myc_DiffXsec->SetLogx(1);
		myc_DiffXsec->SetLogy(0);
		myc_DiffXsec->SetRatioRange(0.9, 1.1);
		myc_DiffXsec->CanvasWithHistogramsRatioPlot(h_DiffXsec_Ref, h_DiffXsec_CV, "from SysUncTool", "Reference", "SysUncTool/Ref");
		myc_DiffXsec->PrintCanvas();

		if( f_output == NULL )
			f_output = new TFile("ROOTFile_Histograms_SysUnc_MCMomCorr.root", "RECREATE");
		f_output->cd();
		myc_DiffXsec->c->Write();
	}


	void CalcDiffXsec_GivenRespM(RooUnfoldResponse *RooUnfoldRes, TH1D *h_DiffXsec)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
		XsecTool->MakeSignalMCHistograms();

		XsecTool->GetYieldHistograms( isDataDriven );

		XsecTool->UnfoldingCorrection(RooUnfoldRes);
		XsecTool->AccEffCorrection();
		XsecTool->EfficiencyScaleFactor();
		XsecTool->FSRCorrection();
		XsecTool->CalcXsec();

		CopyHistogram( XsecTool->h_DiffXsec_FSRCorr, h_DiffXsec );
	}

	void CopyHistogram( TH1D* h_Original, TH1D* h_Empty )
	{
		Int_t nBin = h_Original->GetNbinsX();
		Int_t nBin2 = h_Empty->GetNbinsX();

		if( nBin != nBin2 )
		{
			printf( "[ERROR] # bins of DiffXsec histogram between two histograms! ... (nBin1, nBin2) = (%d, %d)\n", nBin, nBin2 );
			return;
		}

		for(Int_t i=-1; i<nBin+1; i++) // -- including under/overflow -- //
		{
			Int_t i_bin = i+1;
			Double_t value = h_Original->GetBinContent(i_bin);
			Double_t error = h_Original->GetBinError(i_bin);

			h_Empty->SetBinContent(i_bin, value);
			h_Empty->SetBinError(i_bin, error);
		}
	}

	void FillRooUnfoldResponse_GivenCorr(rochcor2015* rmcor, NtupleHandle* ntuple, DYAnalyzer* analyzer, 
		Bool_t isPassAcc_GenLepton, Double_t gen_M, Double_t TotWeight, RooUnfoldResponse* RooUnfoldRes)
	{
		// -- Collect Reconstruction level information -- //
		vector< Muon > MuonCollection;
		Int_t NLeptons = ntuple->nMuon;
		for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
		{
			Muon mu;
			mu.FillFromNtuple(ntuple, i_reco);
			// -- Apply Rochester momentum scale correction -- //
			float qter = 1.0;
			rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);					

			// -- Change Muon pT, eta and phi with updated(corrected) one -- //
			mu.Pt = mu.Momentum.Pt();
			mu.eta = mu.Momentum.Eta();
			mu.phi = mu.Momentum.Phi();

			MuonCollection.push_back( mu );
		}

		// -- Event Selection -- //
		vector< Muon > SelectedMuonCollection;
		Bool_t isPassEventSelection = kFALSE;
		isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

		if( isPassEventSelection == kTRUE )
		{
			Muon Mu1 = SelectedMuonCollection[0];
			Muon Mu2 = SelectedMuonCollection[1];
			Double_t reco_M = ( Mu1.Momentum + Mu2.Momentum ).M();

			if( isPassAcc_GenLepton == kTRUE )
				RooUnfoldRes->Fill( reco_M, gen_M, TotWeight );
			else
				RooUnfoldRes->Fake( reco_M, TotWeight );
		}

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

	void FpoF_Calc_DiffXsec_All()
	{
		this->FpoF_CalcDiffXsec_GivenRespM(RooUnfoldRes_CV, h_FpoF_DiffXsec_CV);

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			this->FpoF_CalcDiffXsec_GivenRespM( RooUnfoldRes_Smeared[i_corrMap], h_FpoF_DiffXsec_Smeared[i_corrMap] );
	}

	void FpoF_CalcDiffXsec_GivenRespM(RooUnfoldResponse *RooUnfoldRes, TH1D *h_FpoF_DiffXsec)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
		XsecTool->MakeSignalMCHistograms();
		XsecTool->GetYieldHistograms( isDataDriven );
		XsecTool->UnfoldingCorrection(RooUnfoldRes);

		XsecTool->FpoF_GetTheoryHist();
		XsecTool->FpoF_EffCorrection();
		XsecTool->FpoF_EfficiencyScaleFactor();
		XsecTool->FpoF_CalcXsec();

		CopyHistogram( XsecTool->h_FpoF_DiffXsec_Data, h_FpoF_DiffXsec );
	}

	void FpoF_SaveResults()
	{
		if( f_FpoF_ouptut == NULL )
			f_FpoF_ouptut = new TFile("ROOTFile_FpoF_Histograms_SysUnc_MCMomCorr.root", "RECREATE");
		
		f_FpoF_ouptut->cd();

		h_FpoF_DiffXsec_CV->Write();

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			h_FpoF_DiffXsec_Smeared[i_corrMap]->Write();
	}

	void FpoF_ValidationPlot()
	{
		TString BkgType = "";
		
		if( isDataDriven ) 
			BkgType = "_DataDrivenBkg";
		else
			BkgType = "_MCBasedBkg";

		TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_cv->cd();
		TH1D *h_FpoF_DiffXsec_Ref = (TH1D*)f_cv->Get("h_FpoF_DiffXsec_Data")->Clone();

		MyCanvas *myc_DiffXsec = new MyCanvas("c_FpoF_DiffXsec_Ref_vs_CV_MCMomCorr", "Dimuon Mass (fiducial, post-FSR)[GeV]", "d#sigma/dM [GeV]");
		myc_DiffXsec->SetLogx(1);
		myc_DiffXsec->SetLogy(0);
		myc_DiffXsec->SetRatioRange(0.9, 1.1);
		myc_DiffXsec->CanvasWithHistogramsRatioPlot(h_FpoF_DiffXsec_Ref, h_FpoF_DiffXsec_CV, "from SysUncTool", "Reference", "SysUncTool/Ref");
		myc_DiffXsec->PrintCanvas();

		if( f_FpoF_ouptut == NULL )
			f_FpoF_ouptut = new TFile("ROOTFile_FpoF_Histograms_SysUnc_MCMomCorr.root", "RECREATE");

		f_FpoF_ouptut->cd();
		myc_DiffXsec->c->Write();
	}

};






