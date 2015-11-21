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
#include <TROOT.h>
#include <TApplication.h>
#include <vector>

//Customized header files
#include <Lepton.h>
#include <ControlPlots.h>

void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut);
Int_t SeparateMuMuTauTauEvent(TString Tag, NtupleHandle *ntuple, ControlPlots *Plots);
void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu);
void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep);
void GenMatching(TString HLTname, TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection);
Bool_t isPassDimuonVertexCut(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2);
static inline void loadBar(int x, int n, int r, int w);
void MuonPlots_AllBinnedSample(Bool_t isCorrected = kTRUE, TString Type = "MuonPhys", TString HLTname = "IsoMu20")
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Type: " << Type << endl;
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

	TString HLT;
	Double_t LeadPtCut = 9999;
	Double_t SubPtCut = 9999;
	Double_t LeadEtaCut = 9999;
	Double_t SubEtaCut = 9999;
	AssignAccThreshold(HLTname, &HLT, &LeadPtCut, &SubPtCut, &LeadEtaCut, &SubEtaCut);
	cout << "===========================================================" << endl;
	cout << "Trigger: " << HLT << endl;
	cout << "leading lepton pT Cut: " << LeadPtCut << endl;
	cout << "Sub-leading lepton pT Cut: " << SubPtCut << endl;
	cout << "leading lepton Eta Cut: " << LeadEtaCut << endl;
	cout << "sub-leading lepton Eta Cut: " << SubEtaCut << endl;
	cout << "===========================================================" << endl;


	TFile *f = new TFile("ROOTFile_Histogram_InvMass_" + HLTname + "_" + Type + "_" + isApplyMomCorr + ".root", "RECREATE");

	TString BaseLocation = "/data4/Users/kplee/DYntuple";
	//Each ntuple directory & corresponding Tags
	vector< TString > ntupleDirectory; vector< TString > Tag;

	if( Type == "MC" )
	{
		// -- Signal binned samples -- //
		ntupleDirectory.push_back( "Spring15DR/25ns/DYLL_M10to50_v2_AddTauInfo" ); Tag.push_back( "DYMuMu_M10to50" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v20151026_AddTauInfo_DYaMCNLO_M50_25ns" ); Tag.push_back( "DYMuMu_M50to200" );
		// ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M100to200_25ns" ); Tag.push_back( "DYMuMu_M100to200" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M200to400_25ns" ); Tag.push_back( "DYMuMu_M200to400" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M400to500_25ns" ); Tag.push_back( "DYMuMu_M400to500" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M500to700_25ns" ); Tag.push_back( "DYMuMu_M500to700" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M700to800_25ns" ); Tag.push_back( "DYMuMu_M700to800" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M800to1000_25ns" ); Tag.push_back( "DYMuMu_M800to1000" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M1000to1500_25ns" ); Tag.push_back( "DYMuMu_M1000to1500" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M1500to2000_25ns" ); Tag.push_back( "DYMuMu_M1500to2000" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v2_FastFilters_No8TeVPUReweighting_DYLL_M2000to3000_25ns" ); Tag.push_back( "DYMuMu_M2000to3000" );

		ntupleDirectory.push_back( "Spring15DR/25ns/DYLL_M10to50_v2_AddTauInfo" ); Tag.push_back( "DYTauTau_M10to50" );
		ntupleDirectory.push_back( "Spring15DR/25ns/v20151026_AddTauInfo_DYaMCNLO_M50_25ns" ); Tag.push_back( "DYTauTau" );
		ntupleDirectory.push_back( "Spring15DR/25ns/ttbar_v1" );  Tag.push_back( "ttbar" );
		ntupleDirectory.push_back( "Spring15DR/25ns/WJets_v1" );  Tag.push_back( "WJets" );
		ntupleDirectory.push_back( "Spring15DR/25ns/WW_v1" );  Tag.push_back( "WW" );
		ntupleDirectory.push_back( "Spring15DR/25ns/WZ_v1" );  Tag.push_back( "WZ" );
		ntupleDirectory.push_back( "Spring15DR/25ns/ZZ_v1" );  Tag.push_back( "ZZ" );

	}
	else if( Type == "Golden" )
	{
		ntupleDirectory.push_back( "Run2015C/GoldenJSON/SingleMuon_v3_Run246908to256869" ); Tag.push_back( "Data" ); // -- Run2015C -- //
	}
	else if( Type == "MuonPhys" )
	{
		ntupleDirectory.push_back( "/Run2015D/MuonPhysJSON/v20151017_MuonPhys_Run2015C_SingleMuon" ); Tag.push_back( "Data" ); // -- Run2015C -- //
	}
	else
	{
		cout << "ERROR: Possible Type: Golden, MuonPhys, and MC" << endl;
		return;
	}

	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		ControlPlots *Plots = new ControlPlots( Tag[i_tup] );

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		if( Tag[i_tup] == "Data" && Type == "Golden" )
		{
			// -- Run2015D -- // 
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151016_v3JSON_Run2015D_SingleMuon_Run246908to256869/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151010_v2JSON_SingleMuon_Run256870to257599/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151009_SingleMuon_Run257600toRun258159/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151022_GoldenJSON_Run2015Dv4_SingleMuon_Run246908to258750/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151103_GoldenJSON_Run2015Dv4_SingleMuon_Run258751to259891/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/GoldenJSON/v20151111_GoldenJSON_Run2015Dv4_SingleMuon_Run259892to260426/*.root");
		}
		else if( Tag[i_tup] == "Data" && Type == "MuonPhys" )
		{
			// -- Run2015D -- // 
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/MuonPhysJSON/v20151017_MuonPhys_Run2015D_SingleMuon_Run246908to258159/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/MuonPhysJSON/v20151022_MuonPhys_Run2015Dv4_SingleMuon_Run246908to258750/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/MuonPhysJSON/v20151103_MuonPhys_Run2015Dv4_SingleMuon_Run258751to259891/*.root");
			chain->Add("/data4/Users/kplee/DYntuple/Run2015D/MuonPhysJSON/v20151111_MuonPhys_Run2015Dv4_SingleMuon_Run259892to260426/*.root");
		}
		NtupleHandle *ntuple = new NtupleHandle( chain );
		
		rochcor2015 *rmcor = new rochcor2015();

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Int_t count_Zpeak = 0;
		Double_t SumWeight = 0;
		Double_t SumWeight_Separated = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
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
			// cout << "Weight of " << i << "th Event: " << GenWeight << endl;
			SumWeight += GenWeight;

			Int_t GenFlag = 0;
			GenFlag = SeparateMuMuTauTauEvent(Tag[i_tup], ntuple, Plots);

			if( GenFlag == 1 )
				SumWeight_Separated += GenWeight;
			
			if( ntuple->isTriggered( HLT ) && GenFlag)
			{
				//Collect Reconstruction level information
				vector< Muon > MuonCollection;
				Int_t NLeptons = ntuple->nMuon;
				for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
				{
					Muon mu;
					mu.FillFromNtuple(ntuple, i_reco);
					// -- Apply Rochester momentum scale correction -- //
					if( isCorrected == kTRUE )
					{
						if( Tag[i_tup] == "Data" )
							mu.ApplyRochCorr(rmcor, 1);
						else
							mu.ApplyRochCorr(rmcor, 0);
					}
					
					MuonCollection.push_back( mu );
				}
				
				//Collect qualified muons among muons
				vector< Muon > QMuonCollection;
				for(Int_t j=0; j<(int)MuonCollection.size(); j++)
				{
				    if( MuonCollection[j].isTightMuon() && MuonCollection[j].trkiso < 0.10)
				        QMuonCollection.push_back( MuonCollection[j] );
				}

				//Give Acceptance cuts
				if( QMuonCollection.size() >= 2)
				{
					Muon leadMu, subMu;
					CompareMuon(&QMuonCollection[0], &QMuonCollection[1], &leadMu, &subMu);
					if( !(leadMu.Pt > LeadPtCut && subMu.Pt > SubPtCut && abs(leadMu.eta) < LeadEtaCut && abs(subMu.eta) < SubEtaCut) )
						QMuonCollection.clear();
				}

				if( QMuonCollection.size() == 2)
				{
					Muon recolep1 = QMuonCollection[0];
					Muon recolep2 = QMuonCollection[1];
					TLorentzVector reco_v1 = recolep1.Momentum;
					TLorentzVector reco_v2 = recolep2.Momentum;
					Double_t reco_M = (reco_v1 + reco_v2).M();

					if( reco_M > 10 && isPassDimuonVertexCut(ntuple, recolep1.cktpT, recolep2.cktpT) )
					{
						Plots->FillHistograms_DoubleMu(ntuple, HLT, recolep1, recolep2);

						//Count # events in the Z-peak region for each sample
						if( reco_M > 60 && reco_M < 120 )
							count_Zpeak++;
					}
				}

			} //End of if( isTriggered )

		} //End of event iteration

		cout << "\tcount_Zpeak(" << Tag[i_tup] << "): " << count_Zpeak << endl;

		Plots->WriteHistograms( f );

		if(isNLO == 1)
		{
			cout << "\tTotal sum of weights: " << SumWeight << endl;
			cout << "\tSum of weights of Seperated events: " << SumWeight_Separated << endl;
		}

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	Double_t TotalRunTime = totaltime.CpuTime();
	cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

	TTimeStamp ts_end;
	cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
}

void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut)
{
	if( HLTname == "IsoMu20" )
	{
		*HLT = "HLT_IsoMu20_v*";
		*LeadPtCut = 22;
		*SubPtCut = 10;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu45_eta2p1" )
	{
		*HLT = "HLT_Mu45_eta2p1_v*";
		*LeadPtCut = 46;
		*SubPtCut = 10;
		*LeadEtaCut = 2.1;
		*SubEtaCut = 2.4;
	}
	else
	{ 
		cout << "Wrong HLT name!: " << HLTname << endl;
		return; 
	}

}

Int_t SeparateMuMuTauTauEvent(TString Tag, NtupleHandle *ntuple, ControlPlots *Plots)
{
	Int_t GenFlag = 0;

	// -- Seperate DYMuMu events from DYTauTau  -- //
	if( Tag.Contains("DYMuMu") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "DYMuMu_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = 1;
			}
			else
				GenFlag = 1;
		}

		if( GenFlag == 1 )
			Plots->FillHistograms_GenMu( ntuple, GenLeptonCollection[0], GenLeptonCollection[1] );
	}
	// -- Separate DYTauTau events from MuMu events -- //
	else if( Tag.Contains("DYTauTau") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( abs(genlep.ID) == 15 && genlep.fromHardProcessDecayed )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
		{
			GenFlag = 1;
		}
	}
	else
		GenFlag = 1; // -- other cases(e.g. ttbar, WJets, Diboson...): pass

	return GenFlag;
}

void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu)
{
    if( Mu1->Pt > Mu2->Pt )
    {
        *leadMu = *Mu1;
        *subMu = *Mu2;
    }
    else
    {
        *leadMu = *Mu2;
        *subMu = *Mu1;
    }
}

void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep)
{
	if( genlep1->Pt > genlep2->Pt )
	{
		*leadgenlep = *genlep1;
		*subgenlep = *genlep2;
	}
	else
	{
		*leadgenlep = *genlep2;
		*subgenlep = *genlep1;
	}
}

void GenMatching(TString HLTname, TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection)
{
	TString HLT;
	Double_t LeadPtCut = 9999;
	Double_t SubPtCut = 9999;
	Double_t LeadEtaCut = 9999;
	Double_t SubEtaCut = 9999;

	AssignAccThreshold(HLTname, &HLT, &LeadPtCut, &SubPtCut, &LeadEtaCut, &SubEtaCut);

	vector<GenLepton> GenLeptonCollection;
	Int_t NGenLeptons = ntuple->gnpair;

	if( MuonType == "PromptFinalState" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isPromptFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else if( MuonType == "fromTau")
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isDirectPromptTauDecayProductFinalState )
				GenLeptonCollection.push_back( genlep );
		}

	}
	else if( MuonType == "fromHardProcess" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else
	{
		cout << "Incorrect MuonType!" << endl;
		return;
	}

	//Give Acceptance Cuts
	if( GenLeptonCollection.size() >= 2 )
	{
		GenLepton leadGenLep, subGenLep;
		CompareGenLepton(&GenLeptonCollection[0], &GenLeptonCollection[1], &leadGenLep, &subGenLep);
		if( !(leadGenLep.Pt > LeadPtCut && subGenLep.Pt > SubPtCut && abs(leadGenLep.eta) < LeadEtaCut && abs(subGenLep.eta) < SubEtaCut) )
			GenLeptonCollection.clear();
	}


	
	Int_t NMuons = (Int_t)MuonCollection->size();
	vector<Muon> RecoMuonCollection;
	//Copy all muons in MuonCollection into RecoMuonCollection
	for(Int_t i_mu=0; i_mu<NMuons; i_mu++)
		RecoMuonCollection.push_back( MuonCollection->at(i_mu) );

	MuonCollection->clear();

	Int_t NGen = (Int_t)GenLeptonCollection.size();
	for(Int_t i_gen=0; i_gen<NGen; i_gen++)
	{
		GenLepton genlep = GenLeptonCollection[i_gen];
		Double_t gen_Pt = genlep.Pt;
		Double_t gen_eta = genlep.eta;
		Double_t gen_phi = genlep.phi;

		Int_t i_matched = -1;
		Double_t dPtMin = 1e10;
		for(Int_t i_reco=0; i_reco<NMuons; i_reco++)
		{
			Muon mu = RecoMuonCollection[i_reco];
			Double_t reco_Pt = mu.Pt;
			Double_t reco_eta = mu.eta;
			Double_t reco_phi = mu.phi;

			Double_t dR = sqrt( (gen_eta-reco_eta)*(gen_eta-reco_eta) + (gen_phi-reco_phi)*(gen_phi-reco_phi) );
			Double_t dPt = fabs(gen_Pt - reco_Pt);
			if( dR < 0.3 )
			{
				if( dPt < dPtMin )
				{
					i_matched = i_reco;
					dPtMin = dPt;
				}
			}
		}

		if( i_matched != -1 )
			MuonCollection->push_back( RecoMuonCollection[i_matched] );
	}

	return;
}

Bool_t isPassDimuonVertexCut(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2)
{
	vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
	vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
	vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

	Int_t NPt1 = (Int_t)PtCollection1->size();
	Int_t NPt2 = (Int_t)PtCollection2->size();
	Int_t NProb = (Int_t)VtxProbCollection->size();

	if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
		cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

	// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

	Double_t VtxProb = -1;
	for(Int_t i=0; i<NProb; i++)
	{
		// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
		if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
		{
			VtxProb = VtxProbCollection->at(i);
			break;
		}
	}

	// cout << "VtxProb: " << VtxProb << endl;
	if( VtxProb > 0.02 )
		return 1;
	else
		return 0;
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

