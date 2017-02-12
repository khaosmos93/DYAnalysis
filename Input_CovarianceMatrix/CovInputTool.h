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
#include <stdio.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>

#include <vector>

#include <DYAnalysis_76X/CommonCodes/RochesterMomCorr_76X/RoccoR.cc>
#include <DYAnalysis_76X/CommonCodes/RochesterMomCorr_76X/rochcor2015.cc>

#include <Unfolding/src/RooUnfoldResponse.h>
#include <Unfolding/src/RooUnfoldBayes.h>
#include <Unfolding/src/RooUnfoldInvert.h>

#include <DYAnalysis_76X/CommonCodes/MyCanvas.C>
#include <DYAnalysis_76X/CommonCodes/DYAnalyzer.h>
#include <DYAnalysis_76X/CommonCodes/DiffXsecTools.h>

class CovInputTool
{
public:
	TString FileLocation;
	Double_t MassBinEdges[nMassBin+1];

	DYAnalyzer *analyzer;

	vector< TString > ntupleDirectory; 
	vector< TString > Tag; 
	vector< Double_t > Xsec; 
	vector< Double_t > nEvents;

	CovInputTool(TString version)
	{
		FileLocation = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/" + version;

		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X samples -- //

		TH1::AddDirectory(kFALSE);
	}

	// -- data & yield distribution -- //
	void Input1()
	{
		TFile *f_input = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root");
		TFile *f_input2 = TFile::Open(FileLocation + "/ROOTFile_YieldHistogram.root");

		f_input->cd();
		TH1D *h_data = (TH1D*)f_input->Get("h_mass_OS_Data")->Clone();
		h_data->SetName("h_data");

		TH1D *h_data_HLTv4p2 = (TH1D*)f_input->Get("h_mass_OS_HLTv4p2_Data")->Clone();
		h_data_HLTv4p2->SetName("h_data_HLTv4p2");

		TH1D *h_data_HLTv4p3 = (TH1D*)f_input->Get("h_mass_OS_HLTv4p3_Data")->Clone();
		h_data_HLTv4p3->SetName("h_data_HLTv4p3");

		h_data = (TH1D*)h_data->Rebin(nMassBin, h_data->GetName(), MassBinEdges);
		h_data_HLTv4p2 = (TH1D*)h_data_HLTv4p2->Rebin(nMassBin, h_data_HLTv4p2->GetName(), MassBinEdges);
		h_data_HLTv4p3 = (TH1D*)h_data_HLTv4p3->Rebin(nMassBin, h_data_HLTv4p3->GetName(), MassBinEdges);

		f_input2->cd();
		TH1D *h_yield_HLTv4p2 = (TH1D*)f_input2->Get("h_yield_OS_HLTv4p2_DataDrivenBkg")->Clone();
		h_yield_HLTv4p2->SetName("h_yield_HLTv4p2");

		TH1D *h_yield_HLTv4p3 = (TH1D*)f_input2->Get("h_yield_OS_HLTv4p3_DataDrivenBkg")->Clone();
		h_yield_HLTv4p3->SetName("h_yield_HLTv4p3");

		TFile *f_output = new TFile("ROOTFile_Histograms_Data.root", "RECREATE");
		f_output->cd();

		h_data->Write();
		h_data_HLTv4p2->Write();
		h_data_HLTv4p3->Write();
		h_yield_HLTv4p2->Write();
		h_yield_HLTv4p3->Write();
	}

	// -- background histograms -- //
	void Input2()
	{
		///////////////////////////////////
		// -- data-driven backgrounds -- //
		///////////////////////////////////
		TH1D *h_ttbar = NULL;
		TH1D *h_DYtautau = NULL;
		TH1D *h_tW = NULL;
		TH1D *h_WJets = NULL;
		TH1D *h_QCD = NULL;
		TH1D *h_WW = NULL;

		TFile *f_DataDrivenBkg = TFile::Open(FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
		h_QCD = (TH1D*)f_DataDrivenBkg->Get("dijet")->Clone();
		h_WJets = (TH1D*)f_DataDrivenBkg->Get("wjets")->Clone();
		h_ttbar = (TH1D*)f_DataDrivenBkg->Get("ttbar")->Clone();
		h_tW = (TH1D*)f_DataDrivenBkg->Get("tW")->Clone();
		h_DYtautau = (TH1D*)f_DataDrivenBkg->Get("DYtautau")->Clone();
		h_WW = (TH1D*)f_DataDrivenBkg->Get("WW")->Clone();

		if( h_QCD == NULL || h_WJets == NULL || h_ttbar == NULL || h_tW == NULL || h_DYtautau == NULL || h_WW == NULL )
		{
			printf("Histogram for Data-driven background is not loaded properly! ... please check");
		}

		////////////////////////////////
		// -- MC-based backgrounds -- //
		////////////////////////////////
		TH1D* h_ZZ_HLTv4p2 = this->MakeMassHistogram( "HLTv4p2", "ZZ");
		TH1D *h_ZZ_HLTv4p3 = this->MakeMassHistogram( "HLTv4p3", "ZZ");
		TH1D *h_WZ_HLTv4p2 = this->MakeMassHistogram( "HLTv4p2", "WZ");
		TH1D *h_WZ_HLTv4p3 = this->MakeMassHistogram( "HLTv4p3", "WZ");

		if( h_ZZ_HLTv4p2 == NULL || h_ZZ_HLTv4p3 == NULL || h_WZ_HLTv4p2 == NULL || h_WZ_HLTv4p3 == NULL )
		{
			printf("Histogram for MC-based background is not loaded properly! ... please check");
		}

		this->SetErrors_MCBasedBkg( h_ZZ_HLTv4p2, h_ZZ_HLTv4p3, h_WZ_HLTv4p2, h_WZ_HLTv4p3 );

		////////////////
		// -- Save -- //
		////////////////
		TFile *f_output = new TFile("ROOTFile_Histograms_Bkg.root", "RECREATE");
		f_output->cd();

		h_ZZ_HLTv4p2->SetName("h_ZZ_HLTv4p2");
		h_ZZ_HLTv4p2->Write();

		h_ZZ_HLTv4p3->SetName("h_ZZ_HLTv4p3");
		h_ZZ_HLTv4p3->Write();

		h_WZ_HLTv4p2->SetName("h_WZ_HLTv4p2");
		h_WZ_HLTv4p2->Write();

		h_WZ_HLTv4p3->SetName("h_WZ_HLTv4p3");
		h_WZ_HLTv4p3->Write();

		h_WW->SetName("h_WW");
		h_WW->Write();

		h_ttbar->SetName("h_ttbar");
		h_ttbar->Write();

		h_DYtautau->SetName("h_DYtautau");
		h_DYtautau->Write();

		h_tW->SetName("h_tW");
		h_tW->Write();

		h_WJets->SetName("h_WJets");
		h_WJets->Write();

		h_QCD->SetName("h_QCD");
		h_QCD->Write();

	}

	TH1D* MakeMassHistogram( TString HLTType, TString Type )
	{
		TString FileName = FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";
		TFile *f_MCBkg = TFile::Open( FileName ); f_MCBkg->cd();

		TString HistName = "";
		if( (HLTType == "HLTv4p2") || (HLTType == "HLTv4p3") )
			HistName = "h_mass_OS_"+HLTType+"_Norm_"+Type;
		else
			HistName = "h_mass_OS_Norm_"+Type;
		
		TH1D* h_mass = (TH1D*)f_MCBkg->Get( HistName )->Clone();
		h_mass = (TH1D*)h_mass->Rebin(nMassBin, h_mass->GetName(), MassBinEdges);

		delete f_MCBkg;

		return h_mass;
	}

	void SetErrors_MCBasedBkg( TH1D* h_ZZ_HLTv4p2, TH1D* h_ZZ_HLTv4p3, TH1D* h_WZ_HLTv4p2, TH1D* h_WZ_HLTv4p3 )
	{
		TFile *f_BkgSys = TFile::Open( FileLocation + "/ROOTFile_SysUnc_BkgEst.root" );
		f_BkgSys->cd();
		TH1D* h_SysUnc_ZZ = (TH1D*)f_BkgSys->Get("h_SysUnc_ZZ")->Clone();
		TH1D* h_SysUnc_WZ = (TH1D*)f_BkgSys->Get("h_SysUnc_WZ")->Clone();

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t RelError_ZZ = h_SysUnc_ZZ->GetBinContent(i_bin) / 100.0;
			Double_t RelError_WZ = h_SysUnc_WZ->GetBinContent(i_bin) / 100.0;
			// printf("[Relative Errors: (WZ, ZZ) = (%lf, %lf)]\n", RelError_ZZ, RelError_WZ);

			Double_t nEvent_ZZ_HLTv4p2 = h_ZZ_HLTv4p2->GetBinContent(i_bin);
			Double_t nEvent_ZZ_HLTv4p3 = h_ZZ_HLTv4p3->GetBinContent(i_bin);

			Double_t nEvent_WZ_HLTv4p2 = h_WZ_HLTv4p2->GetBinContent(i_bin);
			Double_t nEvent_WZ_HLTv4p3 = h_WZ_HLTv4p3->GetBinContent(i_bin);

			// printf("[nEvents: (ZZ_HLTv4p2, ZZ_HLTv4p3, WZ_HLTv4p2, WZ_HLTv4p3) = (%.3lf, %.3lf, %.3lf, %.3lf)]\n", nEvent_ZZ_HLTv4p2, nEvent_ZZ_HLTv4p3, nEvent_WZ_HLTv4p2, nEvent_WZ_HLTv4p3);

			h_ZZ_HLTv4p2->SetBinError( i_bin, nEvent_ZZ_HLTv4p2*RelError_ZZ );
			h_ZZ_HLTv4p3->SetBinError( i_bin, nEvent_ZZ_HLTv4p3*RelError_ZZ );

			h_WZ_HLTv4p2->SetBinError( i_bin, nEvent_WZ_HLTv4p2*RelError_WZ );
			h_WZ_HLTv4p3->SetBinError( i_bin, nEvent_WZ_HLTv4p3*RelError_WZ );
		}
	}

	// -- signal MC tree -- //
	void Input3()
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();

		TString BaseLocation = "/data1/kplee/DYntuple";

		TFile *f = TFile::Open("./ROOTFile_Input_CovarianceMatrix.root", "RECREATE");

		TTree *tree = new TTree("DYTree","ntuple for signal MC");
		TLorentzVector Momentum_Reco_Lead_BeforeMomCorr;
		TLorentzVector Momentum_Reco_Sub_BeforeMomCorr;
		TLorentzVector Momentum_Reco_Lead;
		TLorentzVector Momentum_Reco_Sub;
		Int_t Charge_Reco_Lead;
		Int_t Charge_Reco_Sub;
		Int_t TrackerLayers_Reco_Lead;
		Int_t TrackerLayers_Reco_Sub;
		TLorentzVector Momentum_postFSR_Lead;
		TLorentzVector Momentum_postFSR_Sub;
		TLorentzVector Momentum_preFSR_Lead;
		TLorentzVector Momentum_preFSR_Sub;
		Bool_t Flag_EventSelection;
		Double_t Weight_Norm;
		Double_t Weight_PU;
		Double_t Weight_Gen;

		tree->Branch("Momentum_Reco_Lead_BeforeMomCorr", "TLorentzVector", &Momentum_Reco_Lead_BeforeMomCorr);
		tree->Branch("Momentum_Reco_Sub_BeforeMomCorr", "TLorentzVector", &Momentum_Reco_Sub_BeforeMomCorr);
		tree->Branch("Momentum_Reco_Lead", "TLorentzVector", &Momentum_Reco_Lead);
		tree->Branch("Momentum_Reco_Sub", "TLorentzVector", &Momentum_Reco_Sub);
		tree->Branch("Charge_Reco_Lead",&Charge_Reco_Lead,"Charge_Reco_Lead/I");
		tree->Branch("Charge_Reco_Sub",&Charge_Reco_Sub,"Charge_Reco_Sub/I");
		tree->Branch("TrackerLayers_Reco_Lead",&TrackerLayers_Reco_Lead,"TrackerLayers_Reco_Lead/I");
		tree->Branch("TrackerLayers_Reco_Sub",&TrackerLayers_Reco_Sub,"TrackerLayers_Reco_Sub/I");

		tree->Branch("Momentum_postFSR_Lead", "TLorentzVector", &Momentum_postFSR_Lead);
		tree->Branch("Momentum_postFSR_Sub", "TLorentzVector", &Momentum_postFSR_Sub);
		tree->Branch("Momentum_preFSR_Lead", "TLorentzVector", &Momentum_preFSR_Lead);
		tree->Branch("Momentum_preFSR_Sub", "TLorentzVector", &Momentum_preFSR_Sub);

		tree->Branch("Flag_EventSelection",&Flag_EventSelection,"Flag_EventSelection/O");
		tree->Branch("Weight_Norm",&Weight_Norm,"Weight_Norm/D");
		tree->Branch("Weight_PU",&Weight_PU,"Weight_PU/D");
		tree->Branch("Weight_Gen",&Weight_Gen,"Weight_Gen/D");

		// -- Loop for all samples -- //
		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			if( !Tag[i_tup].Contains("DYMuMu") ) continue;

			// -- ntuple Setting -- //
			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_GenLepton();
			ntuple->TurnOnBranches_GenOthers();
			ntuple->TurnOnBranches_Muon();

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
			Int_t SumEvents_Separated = 0;

			Int_t NEvents = chain->GetEntries();
			cout << "\t[Total Events: " << NEvents << "]" << endl; 

			Weight_Norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << Weight_Norm << "]" << endl;

			// NEvents = 100000;
			// -- Event loop starts -- //
			for(Int_t i=0; i<NEvents; i++)
			{
				this->loadBar(i+1, NEvents, 100, 100);

				//////////////////////////
				// -- Initialization -- //
				//////////////////////////
				Momentum_Reco_Lead_BeforeMomCorr.SetPxPyPzE(0, 0, 0, 0);
				Momentum_Reco_Sub_BeforeMomCorr.SetPxPyPzE(0, 0, 0, 0);
				Momentum_Reco_Lead.SetPxPyPzE(0, 0, 0, 0);
				Momentum_Reco_Sub.SetPxPyPzE(0, 0, 0, 0);
				Charge_Reco_Lead = -99;
				Charge_Reco_Sub = -99;
				TrackerLayers_Reco_Lead = -99;
				TrackerLayers_Reco_Sub = -99;
				Momentum_postFSR_Lead.SetPxPyPzE(0, 0, 0, 0);
				Momentum_postFSR_Sub.SetPxPyPzE(0, 0, 0, 0);
				Momentum_preFSR_Lead.SetPxPyPzE(0, 0, 0, 0);
				Momentum_preFSR_Sub.SetPxPyPzE(0, 0, 0, 0);
				Flag_EventSelection = kFALSE;
				// Weight_Norm = 0;
				Weight_PU = 0;
				Weight_Gen = 0;

				ntuple->GetEvent(i);

				////////////////////////////////////
				// -- Bring Weight Information -- //
				////////////////////////////////////

				// -- GenWeight -- //
				if( isNLO == 1 )
					ntuple->GENEvt_weight < 0 ? Weight_Gen = -1 : Weight_Gen = 1;
				else
					Weight_Gen = 1;

				SumWeights += Weight_Gen;

				// -- PU Weights -- //
				Int_t PU = ntuple->nPileUp;
				Weight_PU = analyzer->PileUpWeightValue_76X( PU );

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

				// -- Only for DY->MuMu events -- //
				if( GenFlag == kTRUE )
				{
					SumWeights_Separated += Weight_Gen;
					SumEvents_Separated++;

					///////////////////////////////////////////
					// -- Get generator level information -- //
					///////////////////////////////////////////
					vector<GenLepton> GenLeptonCollection_FinalState;
					Int_t NGenLeptons = ntuple->gnpair;
					for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
					{
						GenLepton genlep;
						genlep.FillFromNtuple(ntuple, i_gen);
						if( genlep.isMuon() && genlep.fromHardProcessFinalState )
						{
							GenLeptonCollection_FinalState.push_back( genlep );

							if( (Int_t)GenLeptonCollection_FinalState.size() == 2 )
								break;
						}
					}

					Double_t dRCut = 0.1;

					/////////////////////////////////////////////
					// -- postFSR to preFSR: dressed lepton -- //
					/////////////////////////////////////////////
					GenLepton genlep_postFSR1 = GenLeptonCollection_FinalState[0];
					GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection1;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

					GenLepton genlep_postFSR2 = GenLeptonCollection_FinalState[1];
					GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
					vector< GenOthers > GenPhotonCollection2;
					analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);

					////////////////////////////////////////////////////////////////
					// -- Set tree variables: Momentum of post & pre FSR muons -- //
					////////////////////////////////////////////////////////////////

					// -- post-FSR -- //
					if( genlep_postFSR1.Pt > genlep_postFSR2.Pt )
					{
						Momentum_postFSR_Lead = genlep_postFSR1.Momentum;
						Momentum_postFSR_Sub = genlep_postFSR2.Momentum;
					}
					else
					{
						Momentum_postFSR_Lead = genlep_postFSR2.Momentum;
						Momentum_postFSR_Sub = genlep_postFSR1.Momentum;
					}

					// -- pre-FSR -- //
					if( genlep_preFSR1.Pt > genlep_preFSR2.Pt )
					{
						Momentum_preFSR_Lead = genlep_preFSR1.Momentum;
						Momentum_preFSR_Sub = genlep_preFSR2.Momentum;
					}
					else
					{
						Momentum_preFSR_Lead = genlep_preFSR2.Momentum;
						Momentum_preFSR_Sub = genlep_preFSR1.Momentum;
					}

					////////////////////////////////////////////////////
					// -- Collect Reconstruction level information -- //
					////////////////////////////////////////////////////
					if( ntuple->isTriggered( analyzer->HLT ) )
					{
						vector< Muon > MuonCollection;
						vector< Muon > MuonCollection_beforeCorr;
						Int_t NLeptons = ntuple->nMuon;
						for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
						{
							Muon mu;
							mu.FillFromNtuple(ntuple, i_reco);

							MuonCollection_beforeCorr.push_back( mu );

							// -- Apply Rochester momentum scale correction -- //
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
						Bool_t isPassEventSelection = kFALSE;
						isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

						if( isPassEventSelection == kTRUE )
						{
							Muon mu1 = SelectedMuonCollection[0];
							Muon mu2 = SelectedMuonCollection[1];

							Int_t i_muon1 = -1;
							Int_t i_muon2 = -1;
							this->FindSelectedMuonInMuonCollection( MuonCollection, SelectedMuonCollection, i_muon1, i_muon2 );

							///////////////////////////////////////////////////////
							// -- Set tree variables: reco-level informations -- //
							///////////////////////////////////////////////////////

							Flag_EventSelection = kTRUE;

							if( mu1.Pt > mu2.Pt )
							{
								Momentum_Reco_Lead = mu1.Momentum;
								Momentum_Reco_Sub = mu2.Momentum;

								Momentum_Reco_Lead_BeforeMomCorr = MuonCollection_beforeCorr[i_muon1].Momentum;
								Momentum_Reco_Sub_BeforeMomCorr = MuonCollection_beforeCorr[i_muon2].Momentum;

								Charge_Reco_Lead = mu1.charge;
								Charge_Reco_Sub = mu2.charge;

								TrackerLayers_Reco_Lead = mu1.trackerLayers;
								TrackerLayers_Reco_Sub = mu2.trackerLayers;
							}
							else
							{
								Momentum_Reco_Lead = mu2.Momentum;
								Momentum_Reco_Sub = mu1.Momentum;

								Momentum_Reco_Lead_BeforeMomCorr = MuonCollection_beforeCorr[i_muon2].Momentum;
								Momentum_Reco_Sub_BeforeMomCorr = MuonCollection_beforeCorr[i_muon1].Momentum;

								Charge_Reco_Lead = mu2.charge;
								Charge_Reco_Sub = mu1.charge;

								TrackerLayers_Reco_Lead = mu2.trackerLayers;
								TrackerLayers_Reco_Sub = mu1.trackerLayers;
							}

						} // -- end of if( isPassEventSelection == kTRUE ) -- //

					} // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

					tree->Fill();

				} // -- end of if( GenFlag == kTRUE ) -- //

			} // -- end of event iteration -- //

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;
			cout << "\tSum of Events of Separated Events: " << SumEvents_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;

		} // -- end of sample iteration -- //

		f->cd();
		tree->AutoSave();
		f->Close();

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;

	}

	void Input3_CrossCheck()
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();

		TLorentzVector* Momentum_Reco_Lead_BeforeMomCorr = new TLorentzVector();
		TLorentzVector* Momentum_Reco_Sub_BeforeMomCorr = new TLorentzVector();
		TLorentzVector* Momentum_Reco_Lead = new TLorentzVector();
		TLorentzVector* Momentum_Reco_Sub = new TLorentzVector();


		Int_t Charge_Reco_Lead;
		Int_t Charge_Reco_Sub;
		Int_t TrackerLayers_Reco_Lead;
		Int_t TrackerLayers_Reco_Sub;
		TLorentzVector* Momentum_postFSR_Lead = new TLorentzVector();
		TLorentzVector* Momentum_postFSR_Sub = new TLorentzVector();
		TLorentzVector* Momentum_preFSR_Lead = new TLorentzVector();
		TLorentzVector* Momentum_preFSR_Sub = new TLorentzVector();
		Bool_t Flag_EventSelection;
		Double_t Weight_Norm;
		Double_t Weight_PU;
		Double_t Weight_Gen;

		// -- ntuple Setting -- //
		TChain *chain = new TChain("DYTree");
		chain->Add("./ROOTFile_Input_CovarianceMatrix.root");

		chain->SetBranchAddress("Momentum_Reco_Lead_BeforeMomCorr", &Momentum_Reco_Lead_BeforeMomCorr);
		chain->SetBranchAddress("Momentum_Reco_Sub_BeforeMomCorr", &Momentum_Reco_Sub_BeforeMomCorr);
		chain->SetBranchAddress("Momentum_Reco_Lead", &Momentum_Reco_Lead);
		chain->SetBranchAddress("Momentum_Reco_Sub", &Momentum_Reco_Sub);

		chain->SetBranchAddress("Charge_Reco_Lead", &Charge_Reco_Lead);
		chain->SetBranchAddress("Charge_Reco_Sub", &Charge_Reco_Sub);
		chain->SetBranchAddress("TrackerLayers_Reco_Lead", &TrackerLayers_Reco_Lead);
		chain->SetBranchAddress("TrackerLayers_Reco_Sub", &TrackerLayers_Reco_Sub);

		chain->SetBranchAddress("Momentum_postFSR_Lead", &Momentum_postFSR_Lead);
		chain->SetBranchAddress("Momentum_postFSR_Sub", &Momentum_postFSR_Sub);
		chain->SetBranchAddress("Momentum_preFSR_Lead", &Momentum_preFSR_Lead);
		chain->SetBranchAddress("Momentum_preFSR_Sub", &Momentum_preFSR_Sub);

		chain->SetBranchAddress("Flag_EventSelection", &Flag_EventSelection);
		chain->SetBranchAddress("Weight_Norm", &Weight_Norm);
		chain->SetBranchAddress("Weight_PU", &Weight_PU);
		chain->SetBranchAddress("Weight_Gen", &Weight_Gen);


		TH1D *h_GenMass_preFSR = new TH1D("h_GenMass_preFSR", "", nMassBin, MassBinEdges);
		TH1D *h_GenMass_postFSR = new TH1D("h_GenMass_postFSR", "", nMassBin, MassBinEdges);
		TH1D *h_RecoMass = new TH1D("h_RecoMass", "", nMassBin, MassBinEdges);

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;

		// NEvents = 1000;
		for(Int_t i=0; i<NEvents; i++)
		{
			loadBar(i+1, NEvents, 100, 100);

			// printf("[%d event]\n", i);

			chain->GetEntry(i);

			Double_t gen_M_preFSR = (*Momentum_preFSR_Lead + *Momentum_preFSR_Sub).M();
			Double_t gen_M_postFSR = (*Momentum_postFSR_Lead + *Momentum_postFSR_Sub).M();

			// printf("\t(gen_M_preFSR, gen_M_postFSR) = (%12.5lf, %12.5lf)\n", gen_M_preFSR, gen_M_postFSR);

			h_GenMass_preFSR->Fill( gen_M_preFSR, Weight_Gen * Weight_Norm );
			h_GenMass_postFSR->Fill( gen_M_postFSR, Weight_Gen * Weight_Norm );

			// printf("\t[Flag_EventSelection = %d]\n", (Int_t)Flag_EventSelection);
			if( Flag_EventSelection == kTRUE )
			{
				Double_t reco_M = (*Momentum_Reco_Lead + *Momentum_Reco_Sub).M();
				h_RecoMass->Fill( reco_M, Weight_Gen * Weight_Norm * Weight_PU );

				// printf("\t\treco_M = %12.5lf\n", reco_M);
			}

		} // -- end of event iteration -- //

		TFile *f_Ref_Gen = TFile::Open(FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root");
		TH1D* h_Ref_GenMass_preFSR = (TH1D*)f_Ref_Gen->Get("h_mass_preFSR")->Clone();
		TH1D* h_Ref_GenMass_postFSR = (TH1D*)f_Ref_Gen->Get("h_mass_postFSR")->Clone();

		TFile *f_Ref_Reco = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
		TH1D* h_Ref_RecoMass = (TH1D*)f_Ref_Reco->Get("h_DYMC_Reco")->Clone();

		TFile *f_output = new TFile("ROOTFile_TestInputTree_signalMC.root", "RECREATE"); f_output->cd();
		h_GenMass_preFSR->Write();
		h_GenMass_postFSR->Write();
		h_RecoMass->Write();

		h_Ref_GenMass_preFSR->SetName("h_Ref_GenMass_preFSR");
		h_Ref_GenMass_preFSR->Write();

		h_Ref_GenMass_postFSR->SetName("h_Ref_GenMass_postFSR");
		h_Ref_GenMass_postFSR->Write();

		h_Ref_RecoMass->SetName("h_Ref_RecoMass");
		h_Ref_RecoMass->Write();

		MyCanvas *myc_preFSR = new MyCanvas("myc_preFSR_Ref_vs_Tree", "Dimuon Mass (pre-FSR) [GeV]", "Number of events");
		myc_preFSR->SetLogx(1);
		myc_preFSR->SetLogy(0);
		myc_preFSR->SetRatioRange(0.9, 1.1);
		myc_preFSR->CanvasWithHistogramsRatioPlot(h_GenMass_preFSR, h_Ref_GenMass_preFSR, "from Input Tree", "Reference", "Black/Red");
		myc_preFSR->PrintCanvas();
		myc_preFSR->c->Write();

		MyCanvas *myc_postFSR = new MyCanvas("myc_postFSR_Ref_vs_Tree", "Dimuon Mass (post-FSR) [GeV]", "Number of events");
		myc_postFSR->SetLogx(1);
		myc_postFSR->SetLogy(0);
		myc_postFSR->SetRatioRange(0.9, 1.1);
		myc_postFSR->CanvasWithHistogramsRatioPlot(h_GenMass_postFSR, h_Ref_GenMass_postFSR, "from Input Tree", "Reference", "Black/Red");
		myc_postFSR->PrintCanvas();
		myc_postFSR->c->Write();

		MyCanvas *myc_reco = new MyCanvas("myc_Reco_Ref_vs_Tree", "Dimuon Mass (reco-level) [GeV]", "Number of events");
		myc_reco->SetLogx(1);
		myc_reco->SetLogy(0);
		myc_reco->SetRatioRange(0.9, 1.1);
		myc_reco->CanvasWithHistogramsRatioPlot(h_RecoMass, h_Ref_RecoMass, "from Input Tree", "Reference", "Black/Red");
		myc_reco->PrintCanvas();
		myc_reco->c->Write();

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;

	}

	// -- cross-check materials -- //
	void Input6()
	{
		TFile *f_RespM = TFile::Open( FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root" );
		TFile *f_FSR = TFile::Open( FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root" );
		TFile *f_AccEff = TFile::Open( FileLocation + "/ROOTFile_AccEff.root" );
		TFile *f_DiffXsec = TFile::Open( FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root" );

		f_RespM->cd();
		RooUnfoldResponse *UnfoldRes_DetectorResol = (RooUnfoldResponse*)f_RespM->Get("h_RecoMass_h_GenMass")->Clone();
		UnfoldRes_DetectorResol->SetName("UnfoldRes_DetectorResol");

		f_FSR->cd();
		RooUnfoldResponse *UnfoldRes_FSR = (RooUnfoldResponse*)f_FSR->Get("UnfoldRes")->Clone();
		UnfoldRes_FSR->SetName("UnfoldRes_FSR");

		f_AccEff->cd();
		TGraphAsymmErrors *g_Acc = (TGraphAsymmErrors*)f_AccEff->Get("g_Acc")->Clone();
		TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff")->Clone();
		TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)f_AccEff->Get("g_AccEff")->Clone();

		TGraphAsymmErrors *g_EffSF_HLTv4p2 = (TGraphAsymmErrors*)f_AccEff->Get("g_EffCorr_HLTv4p2")->Clone();
		g_EffSF_HLTv4p2->SetName("g_EffSF_HLTv4p2");

		TGraphAsymmErrors *g_EffSF_HLTv4p3 = (TGraphAsymmErrors*)f_AccEff->Get("g_EffCorr_HLTv4p3")->Clone();
		g_EffSF_HLTv4p3->SetName("g_EffSF_HLTv4p3");


		f_DiffXsec->cd();
		TH1D* h_DiffXsec_Data = (TH1D*)f_DiffXsec->Get("h_DiffXsec_FSRCorr")->Clone();
		h_DiffXsec_Data->SetName("h_DiffXsec_Data");

		TFile *f_output = new TFile("ROOTFile_Input6_CrossCheck.root", "RECREATE");
		f_output->cd();

		UnfoldRes_DetectorResol->Write();
		UnfoldRes_FSR->Write();
		g_Acc->Write();
		g_Eff->Write();
		g_AccEff->Write();
		g_EffSF_HLTv4p2->Write();
		g_EffSF_HLTv4p3->Write();
		h_DiffXsec_Data->Write();
	}

	void FindSelectedMuonInMuonCollection( vector< Muon > MuonCollection, vector< Muon > SelectedMuonCollection, Int_t &i_muon1, Int_t &i_muon2 )
	{
		Muon SelectedMu1 = SelectedMuonCollection[0];
		Muon SelectedMu2 = SelectedMuonCollection[1];

		Int_t nMuon = (Int_t)MuonCollection.size();
		for(Int_t i=0; i<nMuon; i++)
		{
			Muon mu = MuonCollection[i];

			if( mu.Pt == SelectedMu1.Pt && mu.eta == SelectedMu1.eta && mu.phi == SelectedMu1.phi )
				i_muon1 = i;
			else if( mu.Pt == SelectedMu2.Pt && mu.eta == SelectedMu2.eta && mu.phi == SelectedMu2.phi )
				i_muon2 = i;
		}

		if( i_muon1 == -1 || i_muon2 == -1 )
		{
			printf("CANNOT find selected muon in muon collection! ... (i_muon1, i_muon2) = (%d, %d)\n", i_muon1, i_muon2);
		}

		return;
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