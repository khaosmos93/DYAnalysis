#include <Include/DYAnalyzer.h>

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

class HistContainer
{
public:
	TString Type;

	vector< TH1D* > vec_Hist;
	TH1D* h_mass;
	TH1D *h_absdiRap_M20to30;
	TH1D *h_absdiRap_M30to45;
	TH1D *h_absdiRap_M45to60;
	TH1D *h_absdiRap_M60to120;
	TH1D *h_absdiRap_M120to200;
	TH1D *h_absdiRap_M200to1500;

	HistContainer(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( Muon mu1, Muon mu2, Double_t weight )
	{
		Double_t reco_M = (mu1.Momentum + mu2.Momentum).M();
		Double_t reco_Rap = (mu1.Momentum + mu2.Momentum).Rapidity();

		this->h_mass->Fill( reco_M, weight );

		// -- |y(ll)| distributions -- //
		if( reco_M >= 20 && reco_M < 30 ) this->h_absdiRap_M20to30->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 30 && reco_M < 45 ) this->h_absdiRap_M30to45->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 45 && reco_M < 60 ) this->h_absdiRap_M45to60->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 60 && reco_M < 120 ) this->h_absdiRap_M60to120->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 120 && reco_M < 200 ) this->h_absdiRap_M120to200->Fill( fabs(reco_Rap), weight);
		else if( reco_M >= 200 && reco_M < 1500 ) this->h_absdiRap_M200to1500->Fill( fabs(reco_Rap), weight);
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
		for( const auto& h : vec_Hist )
			h->Write();

		TString FileName = f_output->GetName();
		printf("[HistContainer] All histograms are saved in %s\n", FileName);
	}

	void Init_Hist()
	{
		Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		this->h_mass = new TH1D("h_mass_"+this->Type, nMassBin, MassBinEdges); vec_Hist.push_back( h_mass );
		this->h_absdiRap_M20to30 = new TH1D("h_absdiRap_M20to30_"+this->Type, "", 24, 0, 2.4); vec_Hist.push_back( h_absdiRap_M20to30 );
		this->h_absdiRap_M30to45 = new TH1D("h_absdiRap_M30to45_"+this->Type, "", 24, 0, 2.4); vec_Hist.push_back( h_absdiRap_M30to45 );
		this->h_absdiRap_M45to60 = new TH1D("h_absdiRap_M45to60_"+this->Type, "", 24, 0, 2.4); vec_Hist.push_back( h_absdiRap_M45to60 );
		this->h_absdiRap_M60to120 = new TH1D("h_absdiRap_M60to120_"+this->Type, "", 24, 0, 2.4); vec_Hist.push_back( h_absdiRap_M60to120 );
		this->h_absdiRap_M120to200 = new TH1D("h_absdiRap_M120to200_"+this->Type, "", 24, 0, 2.4); vec_Hist.push_back( h_absdiRap_M120to200 );
		this->h_absdiRap_M200to1500 = new TH1D("h_absdiRap_M200to1500_"+this->Type, "", 12, 0, 2.4); vec_Hist.push_back( h_absdiRap_M200to1500 );
	}

};

class HistProducer
{
public:
	Double_t Lumi_2016;

	HistProducer()
	{
		this->Lumi_2016 = 35867.060; // -- /pb -- //
	}

	void Produce()
	{
		TString HLTname = "IsoMu20_OR_IsoTkMu20";
		DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

		HistContainer *Hists = new HistContainer();

		TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
		//Each ntuple directory & corresponding Tags
		vector<TString> ntupleDirectory; vector<TString> Tag; vector<Double_t> Xsec; vector<Double_t> nEvents;
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- only DY events -- //

		const Int_t nSample = ntupleDirectory.size();
		for(Int_t i_sample = 0; i_sample<nSample; i_sample++)
		{
			cout << "\t<" << Tag[i_sample] << ">" << endl;

			if( !(Tag[i_sample] == "DYMuMu_M100to200") ) continue;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation + "/" + ntupleDirectory[i_sample]+"/ntuple_*.root");

			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_HLT();
			ntuple->TurnOnBranches_Muon();
			ntuple->TurnOnBranches_GenLepton();
			
			rochcor2015 *rmcor = new rochcor2015();

			analyzer->SetupPileUpReWeighting_76X( isMC );

			Int_t NEvents = chain->GetEntries();
			cout << "\t[Total Events: " << NEvents << "]" << endl;

			Double_t norm_2016 = ( Xsec[i_sample] * Lumi_2016 ) / nEvents[i_sample];
			printf("[norm_2016: %.lf]\n", norm_2016);

			for(Int_t i=0; i<NEvents; i++)
			{
				loadBar(i+1, NEvents, 100, 100);
				
				ntuple->GetEvent(i);

				/////////////////////////////
				// -- Bring the weights -- //
				/////////////////////////////
				// -- Positive/Negative Gen-weights -- //
				Double_t GenWeight;
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

				// -- Pileup-Reweighting -- //
				Double_t PUWeight = analyzer->PileUpWeightValue_76X( ntuple->nPileUp );

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_sample], ntuple);

				if( ntuple->isTriggered( analyzer->HLT ) && GenFlag )
				{
					//Collect Reconstruction level information
					vector< Muon > MuonCollection;
					Int_t NLeptons = ntuple->nMuon;
					for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
					{
						Muon mu;
						mu.FillFromNtuple(ntuple, i_reco);
						// -- Apply Rochester momentum scale correction -- //
						// if( isCorrected == kTRUE )
						{
							float qter = 1.0;
							
							if( Tag[i_sample] == "Data" )
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
					vector< Muon > SelectedMuonCollection = this->EventSelection( MuonCollection, ntuple );

					if( SelectedMuonCollection.size() == 2 )
					{
						Muon mu1 = SelectedMuonCollection[0];
						Muon mu2 = SelectedMuonCollection[1];
						Hists->Fill( mu1, mu2, GenWeight*PUWeight );
					}
					
				} //End of if( isTriggered )

			} //End of event iteration
		}

		TFile *f_output = TFile::Open("ROOTFile_Histogram_1D2D.root", "RECREATE");
		Hists->Save( f_output );
	}

	vector< Muon > EventSelection( vector<Muon>& MuonCollection, NtupleHandle *ntuple )
	{
		vector< Muon > vec_MuonSelected;

		//Collect qualified muons among muons
		vector< Muon > QMuonCollection;
		for(Int_t j=0; j<(int)MuonCollection.size(); j++)
		{
		    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() && MuonCollection[j].trkiso < 0.10)
		        QMuonCollection.push_back( MuonCollection[j] );
		}

		// -- Check the existence of at least one muon matched with HLT-object -- //
		Bool_t isExistHLTMatchedMuon = kFALSE;
		for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
		{
			Muon mu = QMuonCollection[i_mu];
			if( mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
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

				// -- Check the Accpetance -- //
				Bool_t isPassAcc = kFALSE;
				isPassAcc = PassAcc(recolep1, recolep2);

				Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

				Double_t VtxProb = -999;
				Double_t VtxNormChi2 = 999;
				DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

				TLorentzVector inner_v1 = recolep1.Momentum_Inner;
				TLorentzVector inner_v2 = recolep2.Momentum_Inner;

				// -- 3D open angle -- //
				Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

				Bool_t isOS = kFALSE;
				if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

				// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
				if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
				{
					isPassEventSelection = kTRUE;
					vec_MuonSelected.push_back( recolep1 );
					vec_MuonSelected.push_back( recolep2 );
				}
			}
			else if( nQMuons > 2 )
			{
				Double_t VtxProb_BestPair = -1;
				Double_t VtxNormChi2_BestPair = 999;
				Muon mu1_BestPair;
				Muon mu2_BestPair;

				for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
				{
					Muon Mu = QMuonCollection[i_mu];

					// -- at least 1 muon should be matched with HLT objects in best pair -- //
					if( Mu.isTrigMatched(ntuple, "HLT_IsoMu20_v*") || Mu.isTrigMatched(ntuple, "HLT_IsoTkMu20_v*") )
					{
						// -- Mu in this loop: QMuon Matched with HLT object -- //

						// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
						for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
						{
							Muon Mu_jth = QMuonCollection[j_mu];

							if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
							{
								// -- Check that this pair is within acceptance -- //
								Bool_t isPassAcc = kFALSE;
								isPassAcc = PassAcc(Mu, Mu_jth);

								if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
								{
									Double_t VtxProb_temp = -999;
									Double_t VtxNormChi2_temp = 999;
									DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

									// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
									if( VtxNormChi2_temp < VtxNormChi2_BestPair )
									{
										VtxNormChi2_BestPair = VtxNormChi2_temp;
										mu1_BestPair = Mu;
										mu2_BestPair = Mu_jth;
									}
								}
							}
						} // -- end of the loop for j_mu (finding for second muon)
					}
				} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

				if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
				{
					TLorentzVector reco_v1 = mu1_BestPair.Momentum;
					TLorentzVector reco_v2 = mu2_BestPair.Momentum;
					Double_t reco_M = (reco_v1 + reco_v2).M();

					// -- 3D open angle is calculated using inner track information -- //
					// -- 3D open angle -- //
					Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

					Bool_t isOS = kFALSE;
					if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

					if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
					{
						isPassEventSelection = kTRUE;
						vec_MuonSelected.push_back( mu1_BestPair );
						vec_MuonSelected.push_back( mu2_BestPair );
					}
				}

			} // -- End of else if( nQMuons > 2 ) -- //

		} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

		return vec_MuonSelected;
	}

	Bool_t PassAcc( Muon mu1, Muon mu2 )
	{
		Bool_t Flag = kFALSE;

		Double_t Pt_Lead;
		Double_t Pt_Sub;
		if( mu1.Pt > mu2.Pt )
		{
			Pt_Lead = mu1.Pt;
			Pt_Sub = mu2.Pt;
		}
		else
		{
			Pt_Lead = mu2.Pt;
			Pt_Sub = mu1.Pt;
		}

		if( Pt_Lead > 26 && Pt_Sub > 10 && fabs(mu1.eta) < 2.4 && fabs(mu2.eta) < 2.4 ) // -- 2016: IsoMu24 || IsoTkMu24 -- //
			Flag = kTRUE;

		return Flag;
	}

};