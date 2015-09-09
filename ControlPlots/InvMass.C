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

#include <vector>

//Customized header files
#include <Lepton.h>
#include <ControlPlots.h>

void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut);
void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu);
void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep);
void GenMatching(TString HLTname, TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection);
Bool_t isPassDimuonVertexCut(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2);
void InvMass(TString HLTname)
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

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


	TFile *f = new TFile("ROOTFile_Histogram_InvMass_"+HLTname+".root", "RECREATE");

	TString BaseLocation = "/data4/Users/kplee/DYntuple";
	//Each ntuple directory & corresponding Tags
	vector< TString > ntupleDirectory; vector< TString > Tag;
	//MC
	ntupleDirectory.push_back( "Spring15DR/DYLL_M10to50_v11_AddTauInfo" ); Tag.push_back( "DYMuMu_M20to50" );
	ntupleDirectory.push_back( "Spring15DR/DYLL_M50_v11_AddTauInfo" ); Tag.push_back( "DYMuMu" );
	ntupleDirectory.push_back( "Spring15DR/DYLL_M10to50_v11_AddTauInfo" ); Tag.push_back( "DYTauTau_M20to50" );
	ntupleDirectory.push_back( "Spring15DR/DYLL_M50_v11_AddTauInfo" ); Tag.push_back( "DYTauTau" );
	ntupleDirectory.push_back( "Spring15DR/ttbar_v10_FastFilters_No8TeVPUReweighting" );  Tag.push_back( "ttbar" );
	ntupleDirectory.push_back( "Spring15DR/WJets_v10_FastFilters_No8TeVPUReweighting" );  Tag.push_back( "WJets" );
	ntupleDirectory.push_back( "Spring15DR/WW_v10_FastFilters_No8TeVPUReweighting" );  Tag.push_back( "WW" );
	ntupleDirectory.push_back( "Spring15DR/WZ_v10_FastFilters_No8TeVPUReweighting" );  Tag.push_back( "WZ" );
	ntupleDirectory.push_back( "Spring15DR/ZZ_v10_FastFilters_No8TeVPUReweighting" );  Tag.push_back( "ZZ" );

	//Data
	ntupleDirectory.push_back( "SingleMuon/GoldenJSON/v5_FastFilters_1_Run251162to251562" ); Tag.push_back( "Data" );

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
		if( Tag[i_tup] == "Data" )
			chain->Add("/data4/Users/kplee/DYntuple/SingleMuon/GoldenJSON/v5_FastFilters_2_Run251563to251883/*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );

		Bool_t isNLO = 0;
		if( Tag[i_tup] == "DYMuMu" || Tag[i_tup] == "DYTauTau" || Tag[i_tup] == "WJets" || Tag[i_tup] == "DYMuMu_M20to50" || Tag[i_tup] == "DYTauTau_M20to50" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Int_t count_Zpeak = 0;
		Double_t SumWeight = 0;
		Double_t SumWeight_DYMuMu_M20to50 = 0;
		Double_t SumWeight_DYTauTau_M20to50 = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
		for(Int_t i=0; i<NEvents; i++)
		{
			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			if( isNLO == 1 )
				GenWeight = ntuple->GENEvt_weight;
			else
				GenWeight = 1;
			// cout << "Weight of " << i << "th Event: " << GenWeight << endl;
			SumWeight += GenWeight;

			Int_t GenMassFlag = -1;
			//Take the events within 20<M<50 in gen-level
			if( Tag[i_tup] == "DYMuMu_M20to50" )
			{
				GenMassFlag = 0;
				vector<GenLepton> GenLeptonCollection;
				Int_t NGenLeptons = ntuple->gnpair;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.fromHardProcessFinalState )
						GenLeptonCollection.push_back( genlep );
				}

				if( GenLeptonCollection.size() == 2 )
				{
					GenLepton genlep1 = GenLeptonCollection[0];
					GenLepton genlep2 = GenLeptonCollection[1];

					TLorentzVector gen_v1 = genlep1.Momentum;
					TLorentzVector gen_v2 = genlep2.Momentum;
					TLorentzVector gen_vtot = gen_v1 + gen_v2;
					Double_t gen_M = gen_vtot.M();

					if( gen_M > 20 && gen_M < 50 ) //Take the events within 20<M<50 in gen-level
					{
						GenMassFlag = 1;
						SumWeight_DYMuMu_M20to50 += GenWeight;
					}		
				}
			}
			else if( Tag[i_tup] == "DYTauTau_M20to50" )
			{
				GenMassFlag = 0;
				vector<GenLepton> GenLeptonCollection;
				Int_t NGenLeptons = ntuple->gnpair;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( abs(genlep.ID) == 15 && genlep.fromHardProcessDecayed )
						GenLeptonCollection.push_back( genlep );
				}

				if( GenLeptonCollection.size() == 2 )
				{
					GenLepton genlep1 = GenLeptonCollection[0];
					GenLepton genlep2 = GenLeptonCollection[1];

					TLorentzVector gen_v1 = genlep1.Momentum;
					TLorentzVector gen_v2 = genlep2.Momentum;
					TLorentzVector gen_vtot = gen_v1 + gen_v2;
					Double_t gen_M = gen_vtot.M();

					if( gen_M > 20 && gen_M < 50 ) //Take the events within 20<M<50 in gen-level
					{
						GenMassFlag = 1;
						SumWeight_DYTauTau_M20to50 += GenWeight;
					}	
				}
			}
			else
				GenMassFlag = 1; // -- other cases: pass


			
			if( ntuple->isTriggered( HLT ) && GenMassFlag)
			{
				//Collect Reconstruction level information
				vector< Muon > MuonCollection;
				Int_t NLeptons = ntuple->nMuon;
				for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
				{
					Muon mu;
					mu.FillFromNtuple(ntuple, i_reco);
						MuonCollection.push_back( mu );
				}

				//Select muons directly from Z/gamma by matching with gen-level final state muons from hard process
				if( Tag[i_tup] == "DYMuMu" )
					GenMatching(HLTname, "fromHardProcess", ntuple, &MuonCollection);
				//Select muons directly from tau by matching with gen-level final state muons from prompt tau
				else if( Tag[i_tup] == "DYTauTau" )
					GenMatching(HLTname, "fromTau", ntuple, &MuonCollection);

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

					if( reco_M > 20 && isPassDimuonVertexCut(ntuple, recolep1.cktpT, recolep2.cktpT) )
					{
						Plots->FillHistograms(ntuple, HLT, recolep1, recolep2, GenWeight);

						//Count # events in the Z-peak region for each sample
						if( reco_M > 60 && reco_M < 120 )
							count_Zpeak++;
					}
				}

			} //End of if( isTriggered )

		} //End of event iteration

		cout << "\tcount_Zpeak(" << Tag[i_tup] << "): " << count_Zpeak << endl;

		// if( Tag[i_tup] == "DYTauTau_M20to50" )
		// {
		// 	for(Int_t i_hist=0; i_hist < (Int_t)Plots->Histo.size(); i_hist++)
		// 		Plots->Histo[i_hist]->Scale( 2.49997e+10 / 4.48119e+11 );
		// }

		Plots->WriteHistograms( f );

		if(isNLO == 1)
			cout << "\tTotal sum of weights: " << SumWeight << endl;

		if( Tag[i_tup] == "DYMuMu_M20to50" )
			cout << "\tSum of weights in 20<M<50 for DYMuMu events: " << SumWeight_DYMuMu_M20to50 << endl;

		if( Tag[i_tup] == "DYTauTau_M20to50" )
			cout << "\tSum of weights in 20<M<50 for DYTauTau events: " << SumWeight_DYTauTau_M20to50 << endl;

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
		*LeadPtCut = 21;
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


