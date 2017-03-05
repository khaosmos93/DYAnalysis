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
#include <TTreePerfStats.h>

#include <vector>

// -- for Rochester Muon momentum correction -- //
// #include <Include/rochcor80x_070616v2/RoccoR.cc>
// #include <Include/rochcor80x_070616v2/rochcor2016.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

class HistContainer
{
public:
	TString Type;

	vector< TH1D* > vec_Hist;

	TH1D* h_mass;
	TH1D* h_mass_BB;
	TH1D* h_mass_BEEE;

	TH1D* h_RatioPt;
	TH1D* h_RatioPt_M400to2500;

	TH1D* h_dPtOverPt;
	TH1D* h_dPtOverPt_M400to2500;

	HistContainer( TString _Type )
	{
		Type = _Type;
		this->Set_Histograms();
		TH1::AddDirectory(kFALSE);
	}

	void Set_Histograms()
	{		
		this->h_mass = new TH1D("h_mass_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass );
		this->h_mass_BB = new TH1D("h_mass_BB_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_BB );
		this->h_mass_BEEE = new TH1D("h_mass_BEEE_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_BEEE );
		this->h_RatioPt = new TH1D("h_RatioPt_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_RatioPt );
		this->h_RatioPt_M400to2500 = new TH1D("h_RatioPt_M400to2500_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_RatioPt_M400to2500 );

		this->h_dPtOverPt = new TH1D("h_dPtOverPt_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_dPtOverPt );
		this->h_dPtOverPt_M400to2500 = new TH1D("h_dPtOverPt_M400to2500_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_dPtOverPt_M400to2500 );

		Int_t nHist = (Int_t)vec_Hist.size();
		for(Int_t i=0; i<nHist; i++)
			vec_Hist[i]->Sumw2();
	}

	void Fill( Muon mu1, Muon mu2, Double_t weight )
	{
		Double_t M = (mu1.Momentum + mu2.Momentum).M();

		// -- total category -- //
		h_mass->Fill( M, weight );

		if( fabs(mu1.eta) < 0.9 && fabs(mu2.eta) < 0.9 ) // -- BB event -- //
			h_mass_BB->Fill( M, weight );
		else // -- BE + EE event -- //
			h_mass_BEEE->Fill( M, weight );

		// -- ratio = leading pT / sub-leading pT -- //
		Double_t Pt_lead = 0;
		Double_t Pt_sub = 0;
		if( mu1.Pt > mu2.Pt )
		{
			Pt_lead = mu1.Pt;
			Pt_sub = mu2.Pt;
		}
		else
		{
			Pt_lead = mu2.Pt;
			Pt_sub = mu1.Pt;
		}

		Double_t RatioPt = Pt_lead / Pt_sub;
		Double_t dPtOverPt = fabs( Pt_lead - Pt_sub ) / Pt_sub;

		h_RatioPt->Fill( RatioPt, weight );
		h_dPtOverPt->Fill( dPtOverPt, weight );

		if( M > 400 && M < 2500 )
		{
			h_RatioPt_M400to2500->Fill( RatioPt, weight );
			h_dPtOverPt_M400to2500->Fill( dPtOverPt, weight );
		}
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		TString FileName = f_output->GetName();
		printf( "[Save histograms for %s in %s]\n", Type.Data(), FileName.Data() );
		Int_t nHist = (Int_t)vec_Hist.size();
		for(Int_t i=0; i<nHist; i++)
		{
			TString HistName = vec_Hist[i]->GetName();
			vec_Hist[i]->Write();
			printf("\t[%s is saved]\n", HistName.Data() );
		}
		printf("\n");
	}

protected:
};

class HistogramProducer
{
public:
	Int_t isMC;
	Double_t NormFactor;
	TString FileName_ROOTFileList;
	TString Tag;

	HistogramProducer( Int_t _isMC, TString _FileName_ROOTFileList, Double_t _NormFactor)
	{
		this->isMC = _isMC;
		this->FileName_ROOTFileList = _FileName_ROOTFileList;
		this->NormFactor = _NormFactor;
		this->Tag = this->GetTag( this->FileName_ROOTFileList );

		if( this->Tag.Contains("DYPowheg") ) this->Tag = "DYPowheg";

		printf("===============[HistogramProducer]===============\n");
		printf("[isMC = %d] Tag = %s, NormFactor = %lf\n", this->isMC, this->Tag.Data(), this->NormFactor);
		printf("=================================================\n");
	}

	void Producer()
	{
		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( "Mu50_OR_TkMu50_Zprime" );

		// -- make chain -- //
		TChain *chain = new TChain("recoTree/DYTree");
		analyzer->MakeTChain_fromTextFile( chain, FileName_ROOTFileList );

		// -- turn-on ntuple -- //		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_Muon();
		if( this->isMC )
			ntuple->TurnOnBranches_GenLepton();
		ntuple->Ready();

		TTreePerfStats *ps= new TTreePerfStats("PerfStats", chain);

		HistContainer *Hist_DEN = new HistContainer( "DEN" );
		HistContainer *Hist_NUM = new HistContainer( "NUM" );

		// -- only for the data -- //
		HistContainer *Hist_DEN_RunBtoF = new HistContainer( "DEN_RunBtoF" );
		HistContainer *Hist_NUM_RunBtoF = new HistContainer( "NUM_RunBtoF" );

		HistContainer *Hist_DEN_RunGtoH = new HistContainer( "DEN_RunGtoH" );
		HistContainer *Hist_NUM_RunGtoH = new HistContainer( "NUM_RunGtoH" );

		Int_t nTotEvent = chain->GetEntries();
		cout << "\t[Total Events: " << nTotEvent << "]" << endl;

		for(Int_t i=0; i<nTotEvent; i++)
		{
			this->loadBar(i+1, nTotEvent, 100, 100);
			
			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

			Double_t TotWeight = this->NormFactor*GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(this->Tag, ntuple); // -- for DY->tautau process -- //

			if( GenFlag == kTRUE )
			{
				////////////////////////////////
				// -- reco-level selection -- //
				////////////////////////////////
				if( ntuple->isTriggered( analyzer->HLT ) )
				{
					vector< Muon > MuonCollection;
					Int_t NLeptons = ntuple->nMuon;
					for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
					{
						Muon mu;
						mu.FillFromNtuple(ntuple, i_reco);
						mu.ConvertMomentum_TuneP();

						// // -- Apply Rochester momentum scale correction -- //
						// // if( isCorrected == kTRUE )
						// {
						// 	float qter = 1.0;
							
						// 	if( this->SampleName == "Data" )
						// 		rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
						// 	else
						// 		rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

						// 	// -- Change Muon pT, eta and phi with updated(corrected) one -- //
						// 	mu.Pt = mu.Momentum.Pt();
						// 	mu.eta = mu.Momentum.Eta();
						// 	mu.phi = mu.Momentum.Phi();
						// }
						
						MuonCollection.push_back( mu );
					}

					// -- single muon selection: Denominator, numerator -- //
					vector< Muon > QMuonCollection_DEN;
					vector< Muon > QMuonCollection_NUM;
					for(Int_t j=0; j<(int)MuonCollection.size(); j++)
					{
						if( this->isMuon_DEN( MuonCollection[j] ) && fabs(MuonCollection[j].eta) < 2.4 )
						{
							QMuonCollection_DEN.push_back( MuonCollection[j] );

							if( this->isMuon_NUM( MuonCollection[j] ) )
								QMuonCollection_NUM.push_back( MuonCollection[j] );
						}
					}

					vector< Muon > SelectedMuonCollection_DEN;
					Bool_t IsSelected_DEN = this->DimuonSelection_Zprime_DEN( ntuple, analyzer, QMuonCollection_DEN, SelectedMuonCollection_DEN );
					if( IsSelected_DEN )
					{
						Muon mu1 = SelectedMuonCollection_DEN[0];
						Muon mu2 = SelectedMuonCollection_DEN[1];
						Hist_DEN->Fill( mu1, mu2, TotWeight);

						if( !isMC ) // -- data -- //
						{
							if( ntuple->runNum <= 278808 ) // -- RunBtoF -- //
								Hist_DEN_RunBtoF->Fill( mu1, mu2, TotWeight );
							else
								Hist_DEN_RunGtoH->Fill( mu1, mu2, TotWeight );
						}
					}

					vector< Muon > SelectedMuonCollection_NUM;
					Bool_t IsSelected_NUM = this->DimuonSelection_Zprime( ntuple, analyzer, QMuonCollection_NUM, SelectedMuonCollection_NUM );
					if( IsSelected_NUM )
					{
						Muon mu1 = SelectedMuonCollection_NUM[0];
						Muon mu2 = SelectedMuonCollection_NUM[1];
						Hist_NUM->Fill( mu1, mu2, TotWeight);

						if( !isMC ) // -- data -- //
						{
							if( ntuple->runNum <= 278808 ) // -- RunBtoF -- //
								Hist_NUM_RunBtoF->Fill( mu1, mu2, TotWeight );
							else
								Hist_NUM_RunGtoH->Fill( mu1, mu2, TotWeight );
						}
					}

				} // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

			} // -- end of if( GenFlag == kTRUE ) -- //

		} // -- end of event iteration -- //

		TFile *f_output = TFile::Open("ROOTFile_Output_Systematic_Eff_"+this->Tag+".root", "RECREATE");

		Hist_DEN->Save( f_output );
		Hist_NUM->Save( f_output );

		if( !isMC )
		{
			Hist_DEN_RunBtoF->Save( f_output );
			Hist_NUM_RunBtoF->Save( f_output );

			Hist_DEN_RunGtoH->Save( f_output );
			Hist_NUM_RunGtoH->Save( f_output );
		}

		ps->SaveAs("PerformanceStats.root");

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "\tTotal RunTime(" << this->Tag << "): " << TotalRunTime << " seconds\n" << endl;

		printf("============================\nProducer() is finished\n============================\n\n");
	}

protected:
	TString GetTag( TString _FileName_ROOTFileList )
	{
		TObjArray *split = _FileName_ROOTFileList.Tokenize("-");
		return ((TObjString*)(split->At(1)))->String();
	}

	Bool_t isMuon_DEN( Muon mu )
	{
		if( mu.isGLB == 1
			&& mu.isTRK == 1
			&& (mu.AbsTrkIso / mu.Inner_pT) < 0.1
			&& mu.TuneP_pT > 53
			&& fabs(mu.dB) < 0.2
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_NUM( Muon mu )
	{
		if( mu.isGLB == 1
			&& mu.isTRK == 1
			&& fabs(mu.dB) < 0.2
			&& mu.trackerLayersGLB > 5
			&& mu.pixelHitsGLB > 0
			&& mu.muonHits > 0
			&& (  mu.nMatches > 1 
				|| ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
				|| ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
			&& (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3
			&& (mu.AbsTrkIso / mu.Inner_pT) < 0.1
			&& mu.TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t DimuonSelection_Zprime( NtupleHandle *ntuple, DYAnalyzer *analyzer, 
						vector<Muon>& QMuonCollection, vector<Muon>& SelectedMuonCollection )
	{
		Bool_t IsSelected = kFALSE;

		// -- Check the existence of at least one muon matched with HLT-object among all muons -- //
		Bool_t isExistHLTMatchedMuon = kFALSE;

		Int_t nMuon = (Int_t)QMuonCollection.size();
		for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
		{
			Muon mu = QMuonCollection[i_mu];

			if( this->TriggerMatching( analyzer->HLT, ntuple, mu) )
			{
				isExistHLTMatchedMuon = kTRUE;
				break;
			}
		}

		if( isExistHLTMatchedMuon == kTRUE )
		{
			Int_t nQMuons = (Int_t)QMuonCollection.size();
			if( nQMuons == 2)
			{
				Muon recolep1 = QMuonCollection[0];
				Muon recolep2 = QMuonCollection[1];

				if( this->isGoodMuonPair(ntuple, recolep1, recolep2) )
				{
					IsSelected = kTRUE;
					SelectedMuonCollection.push_back( recolep1 );
					SelectedMuonCollection.push_back( recolep2 );
				}
			}
			else if( nQMuons > 2 )
			{
				// Double_t M_BestPair = -1;
				Double_t Pt_BestPair = -1;
				Muon mu1_BestPair;
				Muon mu2_BestPair;

				for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
				{
					Muon Mu = QMuonCollection[i_mu];

					// -- at least 1 muon should be matched with HLT objects in best pair -- //
					if( this->TriggerMatching( analyzer->HLT, ntuple, Mu) )
					{
						// -- Mu in this loop: QMuon Matched with HLT object -- //

						// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
						for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
						{
							Muon Mu_jth = QMuonCollection[j_mu];

							if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
							{
								if( this->isGoodMuonPair(ntuple, Mu, Mu_jth) )
								{
									Double_t Pt_temp = (Mu.Momentum + Mu_jth.Momentum).Pt();

									// -- find the pair with the highest dimuon pt -- //
									if( Pt_BestPair < Pt_temp )
									{
										Pt_BestPair = Pt_temp;
										mu1_BestPair = Mu;
										mu2_BestPair = Mu_jth;
									}
								}
							}
						} // -- end of the loop for j_mu (finding for second muon)

					} // -- end of trigger matching -- //

				} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

				if( Pt_BestPair > 0 ) // -- If at least one pair pass above conditions -- //
				{
					IsSelected = kTRUE;
					SelectedMuonCollection.push_back( mu1_BestPair );
					SelectedMuonCollection.push_back( mu2_BestPair );
				}

			} // -- End of else if( nQMuons > 2 ) -- //

		} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

		return IsSelected;
	}

	Bool_t DimuonSelection_Zprime_DEN( NtupleHandle *ntuple, DYAnalyzer *analyzer, 
						vector<Muon>& QMuonCollection, vector<Muon>& SelectedMuonCollection )
	{
		Bool_t IsSelected = kFALSE;

		// -- Check the existence of at least one muon matched with HLT-object among all muons -- //
		Bool_t isExistHLTMatchedMuon = kFALSE;

		Int_t nMuon = (Int_t)QMuonCollection.size();
		for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
		{
			Muon mu = QMuonCollection[i_mu];

			if( this->TriggerMatching( analyzer->HLT, ntuple, mu) )
			{
				isExistHLTMatchedMuon = kTRUE;
				break;
			}
		}

		if( isExistHLTMatchedMuon == kTRUE )
		{
			Int_t nQMuons = (Int_t)QMuonCollection.size();
			if( nQMuons == 2)
			{
				Muon recolep1 = QMuonCollection[0];
				Muon recolep2 = QMuonCollection[1];

				if( this->isGoodMuonPair_DEN(ntuple, recolep1, recolep2) )
				{
					IsSelected = kTRUE;
					SelectedMuonCollection.push_back( recolep1 );
					SelectedMuonCollection.push_back( recolep2 );
				}
			}
			else if( nQMuons > 2 )
			{
				// Double_t M_BestPair = -1;
				Double_t Pt_BestPair = -1;
				Muon mu1_BestPair;
				Muon mu2_BestPair;

				for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
				{
					Muon Mu = QMuonCollection[i_mu];

					// -- at least 1 muon should be matched with HLT objects in best pair -- //
					if( this->TriggerMatching( analyzer->HLT, ntuple, Mu) )
					{
						// -- Mu in this loop: QMuon Matched with HLT object -- //

						// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
						for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
						{
							Muon Mu_jth = QMuonCollection[j_mu];

							if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
							{
								if( this->isGoodMuonPair_DEN(ntuple, Mu, Mu_jth) )
								{
									Double_t Pt_temp = (Mu.Momentum + Mu_jth.Momentum).Pt();

									// -- find the pair with the highest dimuon pt -- //
									if( Pt_BestPair < Pt_temp )
									{
										Pt_BestPair = Pt_temp;
										mu1_BestPair = Mu;
										mu2_BestPair = Mu_jth;
									}
								}
							}
						} // -- end of the loop for j_mu (finding for second muon)

					} // -- end of trigger matching -- //

				} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

				if( Pt_BestPair > 0 ) // -- If at least one pair pass above conditions -- //
				{
					IsSelected = kTRUE;
					SelectedMuonCollection.push_back( mu1_BestPair );
					SelectedMuonCollection.push_back( mu2_BestPair );
				}

			} // -- End of else if( nQMuons > 2 ) -- //

		} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

		return IsSelected;
	}

	Bool_t TriggerMatching( TString HLT, NtupleHandle *ntuple, Muon &mu )
	{
		if( HLT == "HLT_Mu50_v* || HLT_TkMu50_v*" )
			return (mu.isTrigMatched_dR0p2(ntuple, "HLT_Mu50_v*") || mu.isTrigMatched_dR0p2(ntuple, "HLT_TkMu50_v*"));

		else
			return mu.isTrigMatched_dR0p2(ntuple, HLT);
	}

	Bool_t isGoodMuonPair( NtupleHandle *ntuple, Muon mu1, Muon mu2 )
	{
		Bool_t GoodPair = kFALSE;

		// -- Check the Accpetance - Actually, acceptance cut is already checked in the muon selection... -- //
		// Bool_t isPassAcc = kFALSE;
		// isPassAcc = analyzer->isPassAccCondition_Muon(mu1, mu2);
		Bool_t isPassAcc = kTRUE; // -- it is already checked in previous step -- //

		Double_t reco_M = (mu1.Momentum + mu2.Momentum).M();

		Double_t VtxProb = -999;
		Double_t VtxNormChi2 = 999;
		DimuonVertexProbNormChi2_TuneP(ntuple, mu1.TuneP_pT, mu2.TuneP_pT, &VtxProb, &VtxNormChi2);

		TLorentzVector inner_v1 = mu1.Momentum_Inner;
		TLorentzVector inner_v2 = mu2.Momentum_Inner;

		// -- 3D open angle -- //
		Double_t Angle = mu1.Momentum.Angle( mu2.Momentum.Vect() );

		Bool_t isOS = kFALSE;
		if( mu1.charge != mu2.charge ) isOS = kTRUE;

		// -- pT ratio -- //
		Bool_t Flag_PtRatio = kFALSE;
		Double_t Pt_lead = 0;
		Double_t Pt_sub = 0;
		if( mu1.Pt > mu2.Pt )
		{
			Pt_lead = mu1.Pt;
			Pt_sub = mu2.Pt;
		}
		else
		{
			Pt_lead = mu2.Pt;
			Pt_sub = mu1.Pt;
		}

		if( reco_M < 400 )
			Flag_PtRatio = kTRUE;
		else // -- if over 400 GeV -- //
		{
			if( Pt_lead / Pt_sub < 3 ) 
				Flag_PtRatio = kTRUE;
		}

		// if( reco_M > 50 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi()-0.02 && isOS == kTRUE )
		if( reco_M > 50 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi()-0.02 && isOS == kTRUE && Flag_PtRatio == kTRUE)
			GoodPair = kTRUE;

		return GoodPair;
	}

	Bool_t isGoodMuonPair_DEN( NtupleHandle *ntuple, Muon mu1, Muon mu2 )
	{
		Bool_t GoodPair = kFALSE;

		// -- Check the Accpetance - Actually, acceptance cut is already checked in the muon selection... -- //
		// Bool_t isPassAcc = kFALSE;
		// isPassAcc = analyzer->isPassAccCondition_Muon(mu1, mu2);
		Bool_t isPassAcc = kTRUE; // -- it is already checked in previous step -- //

		Double_t reco_M = (mu1.Momentum + mu2.Momentum).M();

		Double_t VtxProb = -999;
		Double_t VtxNormChi2 = 999;
		DimuonVertexProbNormChi2_TuneP(ntuple, mu1.TuneP_pT, mu2.TuneP_pT, &VtxProb, &VtxNormChi2);

		TLorentzVector inner_v1 = mu1.Momentum_Inner;
		TLorentzVector inner_v2 = mu2.Momentum_Inner;

		// -- 3D open angle -- //
		Double_t Angle = mu1.Momentum.Angle( mu2.Momentum.Vect() );

		Bool_t Flag_PtRatio = kFALSE;
		Double_t Pt_lead = 0;
		Double_t Pt_sub = 0;
		if( mu1.Pt > mu2.Pt )
		{
			Pt_lead = mu1.Pt;
			Pt_sub = mu2.Pt;
		}
		else
		{
			Pt_lead = mu2.Pt;
			Pt_sub = mu1.Pt;
		}

		if( reco_M < 400 )
			Flag_PtRatio = kTRUE;
		else // -- if over 400 GeV -- //
		{
			if( Pt_lead / Pt_sub < 3 ) 
				Flag_PtRatio = kTRUE;
		}



		// Bool_t isOS = kFALSE;
		// if( mu1.charge != mu2.charge ) isOS = kTRUE;

		// if( reco_M > 50 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi()-0.02 && isOS == kTRUE )
		// if( reco_M > 50 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi()-0.02 )
		// if( reco_M > 50 && isPassAcc == kTRUE && Angle < TMath::Pi()-0.02 )
		if( reco_M > 50 && isPassAcc == kTRUE && Angle < TMath::Pi()-0.02 && Flag_PtRatio == kTRUE )
			GoodPair = kTRUE;

		return GoodPair;
	}

	void PrintRecoMuonInfo( Muon mu )
	{
		printf("\t\t(pT = %.3lf, eta = %.3lf, phi = %.3lf, charge = %.d)\n", 
						mu.Pt, mu.eta, mu.phi, mu.charge);
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

// -- SampleName: QCD, Diboson, WJets, DYTauTau, SingleTop, ttbar, DYPowheg -- //
void ProdHist_Systematic_Eff(Int_t _isMC, TString _FileName_ROOTFileList, Double_t _NormFactor)
{
	HistogramProducer *producer = new HistogramProducer( _isMC, _FileName_ROOTFileList, _NormFactor );
	producer->Producer();
}

