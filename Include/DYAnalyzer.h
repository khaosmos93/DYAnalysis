// -- Class for common functions used in DY differential cross section measurement analysis @ 13 TeV -- //
// -- Author: KyoengPil Lee, 05 Dec. 2015 -- //
#pragma once

#include <DYAnalysis_80X/Include/Object.h>
#include <DYAnalysis_80X/Include/NtupleHandle.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <TSystem.h>

// #define Lumi 4423.577 // -- Up to Run 275125 (2016 Data), MuonPhysJSON. unit: /pb, Updated at 2016.08.08 -- //
// #define Lumi 16141.063 // -- Up to Run 277148 (2016 Data), MuonPhysJSON. unit: /pb, Updated at 2016.08.12 -- //
#define Lumi 36459.0 // -- Rereco B-G and prompt H(v2+v3) -- //
#define Lumi_afterTkMu50 33618.689 // -- Rereco B-G and prompt H(v2+v3) after TkMu50 is implemented (run >= 274954) -- //
#define Lumi_BeforeL2Fix 656.977 // -- integrated luminosity before Run 274094 -- //
#define Lumi_RunG 7554.454 // -- integrated luminosity for Run2016G -- //
#define nMassBin 43
#define nPUBin 50

TString GetBasePath()
{
	TString BasePath = gSystem->HomeDirectory();
	BasePath = BasePath + "/Physics/DYAnalysis_80X/";

	return BasePath;
}

TString GetTag( TString _FileName_ROOTFileList )
{
	TObjArray *split = _FileName_ROOTFileList.Tokenize("-");
	return ((TObjString*)(split->At(1)))->String();
}

class DYAnalyzer
{
public:
	TString HLT;
	Double_t LeadPtCut;
	Double_t SubPtCut;
	Double_t LeadEtaCut;
	Double_t SubEtaCut;

	Double_t PileUpWeight[nPUBin];

	Double_t Eff_RecoID_data[5][4];
	Double_t Eff_RecoID_MC[5][4];

	Double_t Eff_Iso_data[5][4];
	Double_t Eff_Iso_MC[5][4];

	Double_t Eff_HLTv4p2_data[5][4];
	Double_t Eff_HLTv4p2_MC[5][4];

	Double_t Eff_HLTv4p3_data[5][4];
	Double_t Eff_HLTv4p3_MC[5][4];

	// -- Constructor -- //
	DYAnalyzer(TString HLTname);

	// -- Setup accetpance cuts -- //
	void AssignAccThreshold(TString HLTname);

	Bool_t SeparateDYLLSample_isHardProcess(TString Tag, NtupleHandle *ntuple);

	// -- Read text file with root file list -- //
	void MakeTChain_fromTextFile( TChain *chain, TString FileName);

	//////////////////////////////////
	// -- Setup pileup weighting -- //
	//////////////////////////////////
	void SetupPileUpReWeighting( Bool_t isMC, TString FileName );
	Double_t PileUpWeightValue(Int_t PileUp_MC);


	/////////////////////////////////////////
	// -- Setup Efficiency scale factor -- //
	/////////////////////////////////////////
	void SetupEfficiencyScaleFactor();
	void SetupEfficiencyScaleFactor(TString ROOTFileName);
	Double_t EfficiencySF_EventWeight(Muon mu1, Muon mu2, NtupleHandle *ntuple);
	Double_t EfficiencySF_EventWeight_HLTv4p2(Muon mu1, Muon mu2);
	Double_t EfficiencySF_EventWeight_HLTv4p3(Muon mu1, Muon mu2);
	Int_t FindPtBin(Double_t Pt);
	Int_t FindEtaBin(Double_t eta);
	
	////////////////////////////
	// -- Event Selections -- //
	////////////////////////////
	Bool_t EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, MuonPair &SelectedPair);

	Bool_t Flag_Acc( Double_t pt1, Double_t eta1, Double_t pt2, Double_t eta2, Bool_t ExcludeECALGap );

	Bool_t isPassAccCondition_Muon(Muon Mu1, Muon Mu2);

	void DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2);
	Bool_t CheckTriggerMatching( Muon mu, NtupleHandle *ntuple );

	// -- for electron channel - //
	Bool_t EventSelection_Electron(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //

	// -- pre-FSR functions -- //
	void PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	void PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	TString DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2);

	// -- miscellaneous -- //
	void GenMatching(TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection);
	void ConvertToTunePInfo( Muon &mu );
	void PrintOutDoubleMuInfo( Muon mu1, Muon mu2 );
};

void DYAnalyzer::MakeTChain_fromTextFile( TChain *chain, TString FileName)
{
	ifstream openFile( FileName.Data() );
	cout << "===============================" << endl;
	cout << "Read " << FileName << endl;
	cout << "===============================" << endl;
	if( openFile.is_open() )
	{
		string line;
		while(getline(openFile, line))
		{
			cout << line << endl;
			chain->Add(line.data());
		}
		openFile.close();
	}

	cout << "==================================" << endl;
	cout << "All ROOT Files are put into TChain" << endl;
	cout << "==================================" << endl;
}

DYAnalyzer::DYAnalyzer(TString HLTname)
{
	if( HLTname == "None" )
	{
		cout << "===================================================" << endl;
		cout << "[No specific trigger setting ... basic constructor]" << endl;
		cout << "===================================================" << endl;
		
		this->HLT = "None";
		this->LeadPtCut = 9999;
		this->SubPtCut = 9999;
		this->LeadEtaCut = 9999;
		this->SubEtaCut = 9999;
	}
	else
	{
		this->AssignAccThreshold(HLTname);
		cout << "===========================================================" << endl;
		cout << "DYAnalyzer.h for Run2016 data" << endl;
		cout << "Trigger: " << HLT << endl;
		cout << "leading lepton pT Cut: " << LeadPtCut << endl;
		cout << "Sub-leading lepton pT Cut: " << SubPtCut << endl;
		cout << "leading lepton Eta Cut: " << LeadEtaCut << endl;
		cout << "sub-leading lepton Eta Cut: " << SubEtaCut << endl;
		cout << "===========================================================" << endl;
	}

}

void DYAnalyzer::AssignAccThreshold(TString HLTname)
{
	if( HLTname == "IsoMu20" )
	{
		this->HLT = "HLT_IsoMu20_v*";
		this->LeadPtCut = 22;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu20_OR_IsoTkMu20" )
	{
		this->HLT = "HLT_IsoMu20_v* || HLT_IsoTkMu20_v*";
		this->LeadPtCut = 22;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu27_OR_IsoTkMu27" )
	{
		this->HLT = "HLT_IsoMu27_v* || HLT_IsoTkMu27_v*";
		this->LeadPtCut = 29;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu24_OR_IsoTkMu24" )
	{
		this->HLT = "HLT_IsoMu24_v* || HLT_IsoTkMu24_v*";
		this->LeadPtCut = 26;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu45_eta2p1" )
	{
		this->HLT = "HLT_Mu45_eta2p1_v*";
		this->LeadPtCut = 46;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.1;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu50" )
	{
		this->HLT = "HLT_Mu50_v*";
		this->LeadPtCut = 53;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu50_Zprime" )
	{
		this->HLT = "HLT_Mu50_v*";
		this->LeadPtCut = 53;
		this->SubPtCut = 53;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "TkMu50_Zprime" )
	{
		this->HLT = "HLT_TkMu50_v*";
		this->LeadPtCut = 53;
		this->SubPtCut = 53;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "Mu50_OR_TkMu50_Zprime" )
	{
		this->HLT = "HLT_Mu50_v* || HLT_TkMu50_v*";
		this->LeadPtCut = 53;
		this->SubPtCut = 53;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "IsoMu20_SymmetricPt25" )
	{
		this->HLT = "HLT_IsoMu20_v*";
		this->LeadPtCut = 25;
		this->SubPtCut = 25;
		this->LeadEtaCut = 2.4;
		this->SubEtaCut = 2.4;
	}
	else if( HLTname == "Ele17Ele12" )
	{
		this->HLT = "HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*";
		this->LeadPtCut = 25;
		this->SubPtCut = 15;
		this->LeadEtaCut = 2.5;
		this->SubEtaCut = 2.5;
	}
	else if( HLTname == "Ele22_eta2p1" )
	{
		this->HLT = "HLT_Ele22_eta2p1_WPLoose_Gsf_v*"; // -- Exist only for the data; "HLT_Ele22_eta2p1_WP75_Gsf_v*" should be used for MC
		this->LeadPtCut = 25;
		this->SubPtCut = 15;
		this->LeadEtaCut = 2.1;
		this->SubEtaCut = 2.1;
	}
	else if( HLTname == "Ele22_eta2p1_NoEtaCut" )
	{
		this->HLT = "HLT_Ele22_eta2p1_WPLoose_Gsf_v*"; // -- Exist only for the data; "HLT_Ele22_eta2p1_WP75_Gsf_v*" should be used for MC
		this->LeadPtCut = 25;
		this->SubPtCut = 15;
		this->LeadEtaCut = 2.5;
		this->SubEtaCut = 2.5;
	}
	else if( HLTname == "Pt_30_10_eta_2p5" )
	{
		this->HLT = "None"; // -- just for acceptance test -- //
		this->LeadPtCut = 30;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.5;
		this->SubEtaCut = 2.5;
	}
	else if( HLTname == "Ele23_WPLoose" )
	{
		this->HLT = "HLT_Ele23_WPLoose_Gsf_v*"; // -- Exist only for the data; "HLT_Ele22_eta2p1_WP75_Gsf_v*" should be used for MC
		this->LeadPtCut = 30;
		this->SubPtCut = 10;
		this->LeadEtaCut = 2.5;
		this->SubEtaCut = 2.5;
	}
	else if( HLTname == "Ele23_Ele12" )
	{
		this->HLT = "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*";
		this->LeadPtCut = 28;
		this->SubPtCut = 17;
		this->LeadEtaCut = 2.5;
		this->SubEtaCut = 2.5;
	}
	else
	{ 
		cout << "Wrong HLT name!: " << HLTname << endl;
		return; 
	}

}

Bool_t DYAnalyzer::Flag_Acc( Double_t pt1, Double_t eta1, Double_t pt2, Double_t eta2, Bool_t ExcludeECALGap = kFALSE )
{
	Bool_t Flag = kFALSE;

	Double_t Pt_Lead = 0;
	Double_t eta_Lead = 0;
	Double_t Pt_Sub = 0;
	Double_t eta_Sub = 0;

	if( pt1 > pt2 )
	{
		Pt_Lead = pt1;
		eta_Lead = eta1;
		Pt_Sub = pt2;
		eta_Sub = eta2;
	}
	else
	{
		Pt_Lead = pt2;
		eta_Lead = eta2;
		Pt_Sub = pt1;
		eta_Sub = eta1;
	}

	if( Pt_Lead > this->LeadPtCut && fabs(eta_Lead) < this->LeadEtaCut &&
		Pt_Sub > this->SubPtCut && fabs(eta_Sub) < this->SubEtaCut )
	{
		if( ExcludeECALGap == kTRUE )
		{
			if( !(fabs(eta_Lead) > 1.4442 && fabs(eta_Sub) < 1.566) )
				Flag = kTRUE;
		}
		else
			Flag = kTRUE;
	}

	return Flag;
}


Bool_t DYAnalyzer::SeparateDYLLSample_isHardProcess(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events from DYTauTau  -- //
	if( Tag.Contains("DYMuMu") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isHardProcess )
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
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to400" ) // -- Select only evetns withtin 50 < M < 400 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 400 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to100" || Tag == "DYMuMu_Photos_M50to100" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 100 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M50to120" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 120 )
					GenFlag = kTRUE;
			}
			else if( Tag == "DYMuMu_M120to200" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M > 120 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	else if( Tag.Contains("DYEE") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isElectron() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 electrons from hard-process -- //
		{
			if( Tag == "DYEE_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
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
			if( abs(genlep.ID) == 15 && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
		{
			GenFlag = kTRUE;
		}
	}
	// -- Madgraph sample -- //
	else if( Tag.Contains("Madgraph") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "Madgraph_M50to150" ) // -- Select only evetns withtin 50 < M < 150 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 150 )
					GenFlag = kTRUE;
			}
			else if( Tag == "Madgraph_M10to50" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M > 10 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- other cases(e.g. ttbar, WJets, Diboson...): pass
	else
		GenFlag = kTRUE; 

	return GenFlag;
}

void DYAnalyzer::SetupPileUpReWeighting( Bool_t isMC, TString FileName )
{
	if( isMC == kFALSE ) // -- for data -- //
	{
		for(Int_t i=0; i<nPUBin; i++)
			PileUpWeight[i] = 1;

		return;
	}
	
	// -- Only for the MC -- //
	TString FilePath = GetBasePath()+"CommonCodes/Pileup/" + FileName;
	printf( "[Setup the pileup reweighting values from: %s]\n", FilePath.Data() );

	TFile *f = new TFile(GetBasePath()+"CommonCodes/Pileup/" + FileName);
	f->cd();
	TH1D *h_weight = (TH1D*)f->Get("h_PUReWeights");
	if( h_weight == NULL )
	{
		cout << "ERROR! ... No Weight histogram!"<< endl;
		return;
	}

	for(Int_t i=0; i<nPUBin; i++)
	{
		Int_t i_bin = i+1;
		PileUpWeight[i] = h_weight->GetBinContent(i_bin);
	}
}

Double_t DYAnalyzer::PileUpWeightValue(Int_t PileUp_MC)
{
	if( PileUp_MC < 0 || PileUp_MC > 50 )
	{
		cout << "[PileUp_MC = " << PileUp_MC << "]: NO CORRESPONDING PU Weight! ... it returns 0" << endl;
		return 0;
	}
	return PileUpWeight[PileUp_MC];
}

void DYAnalyzer::SetupEfficiencyScaleFactor()
{
	TString Location_TnP = "/home/kplee/CommonCodes/DrellYanAnalysis/ROOTFile_TagProbeEfficiency_v20160329.root";
	cout << "[Tag&Probe efficiency is from " << Location_TnP << " (Default, 74X)]" << endl;
	
	TFile *f = new TFile( Location_TnP );
	TH2D *h_RecoID_data = (TH2D*)f->Get("h_2D_Eff_RecoID_Data");
	TH2D *h_RecoID_MC = (TH2D*)f->Get("h_2D_Eff_RecoID_MC");

	TH2D *h_Iso_data = (TH2D*)f->Get("h_2D_Eff_Iso_Data");
	TH2D *h_Iso_MC = (TH2D*)f->Get("h_2D_Eff_Iso_MC");

	TH2D *h_HLTv4p2_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_Data");
	TH2D *h_HLTv4p2_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_MC");

	TH2D *h_HLTv4p3_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_Data");
	TH2D *h_HLTv4p3_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_MC");


	Int_t nEtaBins = h_RecoID_data->GetNbinsX();
	Int_t nPtBins = h_RecoID_data->GetNbinsY();

	for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
	{
		for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
		{
			Int_t i_etabin = iter_x + 1;
			Int_t i_ptbin = iter_y + 1;

			Double_t RecoID_data = h_RecoID_data->GetBinContent(i_etabin, i_ptbin);
			Double_t RecoID_MC = h_RecoID_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t Iso_data = h_Iso_data->GetBinContent(i_etabin, i_ptbin);
			Double_t Iso_MC = h_Iso_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p2_data = h_HLTv4p2_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p2_MC = h_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p3_data = h_HLTv4p3_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p3_MC = h_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);

			Eff_RecoID_data[iter_x][iter_y] = RecoID_data;
			Eff_RecoID_MC[iter_x][iter_y] = RecoID_MC;

			Eff_Iso_data[iter_x][iter_y] = Iso_data;
			Eff_Iso_MC[iter_x][iter_y] = Iso_MC;

			Eff_HLTv4p2_data[iter_x][iter_y] = HLTv4p2_data;
			Eff_HLTv4p2_MC[iter_x][iter_y] = HLTv4p2_MC;

			Eff_HLTv4p3_data[iter_x][iter_y] = HLTv4p3_data;
			Eff_HLTv4p3_MC[iter_x][iter_y] = HLTv4p3_MC;
		}
	}
	cout << "Setting for efficiency correction factors is completed" << endl;
}

void DYAnalyzer::SetupEfficiencyScaleFactor(TString ROOTFileName)
{
	TString Location_TnP = "/home/kplee/CommonCodes/DrellYanAnalysis/"+ROOTFileName;
	cout << "[Tag&Probe efficiency is from " << Location_TnP << "]" << endl; 

	TFile *f = new TFile( Location_TnP );

	TH2D *h_RecoID_data = (TH2D*)f->Get("h_2D_Eff_RecoID_Data");
	TH2D *h_RecoID_MC = (TH2D*)f->Get("h_2D_Eff_RecoID_MC");

	TH2D *h_Iso_data = (TH2D*)f->Get("h_2D_Eff_Iso_Data");
	TH2D *h_Iso_MC = (TH2D*)f->Get("h_2D_Eff_Iso_MC");

	TH2D *h_HLTv4p2_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_Data");
	TH2D *h_HLTv4p2_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p2_MC");

	TH2D *h_HLTv4p3_data = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_Data");
	TH2D *h_HLTv4p3_MC = (TH2D*)f->Get("h_2D_Eff_HLTv4p3_MC");


	Int_t nEtaBins = h_RecoID_data->GetNbinsX();
	Int_t nPtBins = h_RecoID_data->GetNbinsY();

	for(Int_t iter_x = 0; iter_x < nEtaBins; iter_x++)
	{
		for(Int_t iter_y = 0; iter_y < nPtBins; iter_y++)
		{
			Int_t i_etabin = iter_x + 1;
			Int_t i_ptbin = iter_y + 1;

			Double_t RecoID_data = h_RecoID_data->GetBinContent(i_etabin, i_ptbin);
			Double_t RecoID_MC = h_RecoID_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t Iso_data = h_Iso_data->GetBinContent(i_etabin, i_ptbin);
			Double_t Iso_MC = h_Iso_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p2_data = h_HLTv4p2_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p2_MC = h_HLTv4p2_MC->GetBinContent(i_etabin, i_ptbin);

			Double_t HLTv4p3_data = h_HLTv4p3_data->GetBinContent(i_etabin, i_ptbin);
			Double_t HLTv4p3_MC = h_HLTv4p3_MC->GetBinContent(i_etabin, i_ptbin);

			Eff_RecoID_data[iter_x][iter_y] = RecoID_data;
			Eff_RecoID_MC[iter_x][iter_y] = RecoID_MC;

			Eff_Iso_data[iter_x][iter_y] = Iso_data;
			Eff_Iso_MC[iter_x][iter_y] = Iso_MC;

			Eff_HLTv4p2_data[iter_x][iter_y] = HLTv4p2_data;
			Eff_HLTv4p2_MC[iter_x][iter_y] = HLTv4p2_MC;

			Eff_HLTv4p3_data[iter_x][iter_y] = HLTv4p3_data;
			Eff_HLTv4p3_MC[iter_x][iter_y] = HLTv4p3_MC;
		}
	}
	cout << "Setting for efficiency correction factors is completed" << endl;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight_HLTv4p2(Muon mu1, Muon mu2)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];


	Double_t Eff_EventTrig_data = 0;
	Double_t Eff_EventTrig_MC = 0;

	Double_t Eff_Trig_muon1_data = Eff_HLTv4p2_data[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_data = Eff_HLTv4p2_data[etabin2][ptbin2];
	Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

	Double_t Eff_Trig_muon1_MC = Eff_HLTv4p2_MC[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_MC = Eff_HLTv4p2_MC[etabin2][ptbin2];
	Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data * Eff_EventTrig_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

	// cout << "Eff_data_all: " << Eff_data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
	weight = Eff_data_all / Eff_MC_all;

	if( weight > 2 )
	{
		printf("[Data]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin1][ptbin1], Eff_Iso_data[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin2][ptbin2], Eff_Iso_data[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_data, Eff_data_all);

		printf("[MC]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin1][ptbin1], Eff_Iso_MC[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin2][ptbin2], Eff_Iso_MC[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

		printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
		
		printf("[SF] Weight = %.3lf\n", weight);
	}
	return weight;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight_HLTv4p3(Muon mu1, Muon mu2)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];


	Double_t Eff_EventTrig_data = 0;
	Double_t Eff_EventTrig_MC = 0;

	Double_t Eff_Trig_muon1_data = Eff_HLTv4p3_data[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_data = Eff_HLTv4p3_data[etabin2][ptbin2];
	Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

	Double_t Eff_Trig_muon1_MC = Eff_HLTv4p3_MC[etabin1][ptbin1];
	Double_t Eff_Trig_muon2_MC = Eff_HLTv4p3_MC[etabin2][ptbin2];
	Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data * Eff_EventTrig_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

	// cout << "Eff_data_all: " << Eff_data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
	weight = Eff_data_all / Eff_MC_all;

	if( weight > 2 )
	{
		printf("[Data]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin1][ptbin1], Eff_Iso_data[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin2][ptbin2], Eff_Iso_data[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_data, Eff_data_all);

		printf("[MC]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin1][ptbin1], Eff_Iso_MC[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin2][ptbin2], Eff_Iso_MC[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

		printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
		
		printf("[SF] Weight = %.3lf\n", weight);
	}
	return weight;
}

Int_t DYAnalyzer::FindPtBin(Double_t Pt)
{
	const Int_t nPtBins = 4;
	Double_t PtBinEdges[nPtBins+1] = {10, 22, 40, 70, 250};

	Int_t ptbin = 9999;

	// -- if Pt is larger than the largest Pt bin edge, SF is same with the value for the last bin -- // 
	if( Pt > PtBinEdges[nPtBins] )
		ptbin = nPtBins-1;
	else
	{
		for(Int_t i=0; i<nPtBins; i++)
		{
			if( Pt > PtBinEdges[i] && Pt < PtBinEdges[i+1] )
			{
				ptbin = i;
				break;
			}
		}
	}

	return ptbin;
}

Int_t DYAnalyzer::FindEtaBin(Double_t eta)
{
	const Int_t nEtaBins = 5;
	Double_t EtaBinEdges[nEtaBins+1] = {-2.4, -1.2, -0.3, 0.3, 1.2, 2.4};

	Int_t etabin = 9999;

	for(Int_t i=0; i<nEtaBins; i++)
	{
		if( eta > EtaBinEdges[i] && eta < EtaBinEdges[i+1] )
		{
			etabin = i;
			break;
		}
	}

	return etabin;
}

Double_t DYAnalyzer::EfficiencySF_EventWeight(Muon mu1, Muon mu2, NtupleHandle *ntuple)
{
	Double_t weight = -999;

	// -- Muon1 -- //
	Double_t Pt1 = mu1.Pt;
	Double_t eta1 = mu1.eta;

	Int_t ptbin1 = FindPtBin( Pt1 );
	Int_t etabin1 = FindEtaBin( eta1 );

	if( ptbin1 == 9999 || etabin1 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin1, etabin1);
		return -999;
	}

	Double_t Eff_muon1_data = Eff_RecoID_data[etabin1][ptbin1] * Eff_Iso_data[etabin1][ptbin1];
	Double_t Eff_muon1_MC = Eff_RecoID_MC[etabin1][ptbin1] * Eff_Iso_MC[etabin1][ptbin1];

	// -- Muon2 -- //
	Double_t Pt2 = mu2.Pt;
	Double_t eta2 = mu2.eta;

	Int_t ptbin2 = FindPtBin( Pt2 );
	Int_t etabin2 = FindEtaBin( eta2 );

	if( ptbin2 == 9999 || etabin2 == 9999 )
	{
		printf("ERROR! Wrong assigned bin number ... (ptbin, etabin) = (%d, %d)\n", ptbin2, etabin2);
		return -999;
	}
	Double_t Eff_muon2_data = Eff_RecoID_data[etabin2][ptbin2] * Eff_Iso_data[etabin2][ptbin2];
	Double_t Eff_muon2_MC = Eff_RecoID_MC[etabin2][ptbin2] * Eff_Iso_MC[etabin2][ptbin2];

	Bool_t isHLTv4p2 = kFALSE;
	if( ntuple->runNum < 257932.5 )
		isHLTv4p2 = kTRUE;

	Double_t Eff_EventTrig_data = 0;
	Double_t Eff_EventTrig_MC = 0;
	if( isHLTv4p2 )
	{
		Double_t Eff_Trig_muon1_data = Eff_HLTv4p2_data[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_data = Eff_HLTv4p2_data[etabin2][ptbin2];
		Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

		Double_t Eff_Trig_muon1_MC = Eff_HLTv4p2_MC[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_MC = Eff_HLTv4p2_MC[etabin2][ptbin2];
		Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;
	}
	else
	{
		Double_t Eff_Trig_muon1_data = Eff_HLTv4p3_data[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_data = Eff_HLTv4p3_data[etabin2][ptbin2];
		Eff_EventTrig_data = Eff_Trig_muon1_data + Eff_Trig_muon2_data - Eff_Trig_muon1_data * Eff_Trig_muon2_data;

		Double_t Eff_Trig_muon1_MC = Eff_HLTv4p3_MC[etabin1][ptbin1];
		Double_t Eff_Trig_muon2_MC = Eff_HLTv4p3_MC[etabin2][ptbin2];
		Eff_EventTrig_MC = Eff_Trig_muon1_MC + Eff_Trig_muon2_MC - Eff_Trig_muon1_MC * Eff_Trig_muon2_MC;
	}

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data * Eff_EventTrig_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC * Eff_EventTrig_MC;

	// cout << "Eff_data_all: " << Eff_data_all << ", Eff_MC_all: " << Eff_MC_all << endl;
	weight = Eff_data_all / Eff_MC_all;

	if( weight > 2 )
	{
		printf("[Data]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin1][ptbin1], Eff_Iso_data[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_data[etabin2][ptbin2], Eff_Iso_data[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_data, Eff_data_all);

		printf("[MC]\n");
		printf("\t[Muon1] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin1][ptbin1], Eff_Iso_MC[etabin1][ptbin1]);
		printf("\t[Muon2] (RecoID, Iso): (%.3lf, %.3lf)\n", Eff_RecoID_MC[etabin2][ptbin2], Eff_Iso_MC[etabin2][ptbin2]);
		printf("\t[Event] (TrigEvent, Total): (%.3lf, %.3lf)\n", Eff_EventTrig_MC, Eff_MC_all);

		printf("(ptbin1, etabin1, ptbin2, etabin2): (%d, %d, %d, %d)\n", ptbin1, etabin1, ptbin2, etabin2);
		
		printf("[SF] Weight = %.3lf\n", weight);
	}
	return weight;
}

Bool_t DYAnalyzer::EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						MuonPair &SelectedPair ) // -- output: selected muon pair passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Collect qualified muons among muons -- //
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() && MuonCollection[j].trkiso < 0.10)
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	Int_t nQMuons = (Int_t)QMuonCollection.size();
	if( nQMuons >= 2 ) // -- at least more than or equal to 2 qualified muons in a event -- // 
	{
		// -- find all qualified dimuon pairs -- //
		vector< MuonPair > vec_GoodPair;
		for(Int_t i_mu=0; i_mu<nQMuons-1; i_mu++)
		{
			for(Int_t j_mu=i_mu+1; j_mu<nQMuons; j_mu++)
			{
				Muon mu1 = QMuonCollection[i_mu];
				Muon mu2 = QMuonCollection[j_mu];

				MuonPair pair_temp( mu1, mu2 );

				if( pair_temp.isGoodMuonPair( ntuple, this->HLT, this->LeadPtCut, this->SubPtCut, this->LeadEtaCut, this->SubEtaCut) )
					vec_GoodPair.push_back( pair_temp );
			}

			Int_t nPair = (Int_t)vec_GoodPair.size();
			if( nPair >= 1 ) // -- at least one pair pass all good pair condition -- //
			{
				// -- the pair with "smallest" vertex chi2 will be the first element -- //
				std::sort( vec_GoodPair.begin(), vec_GoodPair.end(), ComparePair_VtxChi2 );

				if( nPair >= 2 )
				{
					for(Int_t i_pair=0; i_pair<nPair; i_pair++)
						printf("\t[%02d pair: vertex chi2 = %lf]\n", i_pair, vec_GoodPair[i_pair].NormVtxChi2);
				}

				isPassEventSelection = kTRUE;
				SelectedPair = vec_GoodPair[0]; // -- does it properly work? -- //
				// SelectedMuonCollection->push_back( vec_GoodPair[0].First );
				// SelectedMuonCollection->push_back( vec_GoodPair[0].Second );
			}
		}
	} // end of if( nQMuons >= 2 ) -- //

	return isPassEventSelection;
}

// -- Event selecton for the electron channel (test) -- //
Bool_t DYAnalyzer::EventSelection_Electron(vector< Electron > ElectronCollection, NtupleHandle *ntuple, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns() && elec.ecalDriven == 1 && elec.Pt > 15 )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = this->Flag_Acc( recolep1.Pt, recolep1.eta, recolep2.Pt, recolep2.eta, kTRUE );

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

// -- Event selecton for the electron channel (2016.02.11) -- //
Bool_t DYAnalyzer::EventSelection_ElectronChannel(vector< Electron > ElectronCollection, NtupleHandle *ntuple, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns() && elec.ecalDriven == 1 
			&& elec.Pt > SubPtCut && fabs(elec.etaSC) < SubEtaCut && !( fabs(elec.etaSC) > 1.4442 && fabs(elec.etaSC) < 1.566 ) )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = this->Flag_Acc( recolep1.Pt, recolep1.eta, recolep2.Pt, recolep2.eta, kTRUE );

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}


void DYAnalyzer::PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
{
	TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;

	TLorentzVector SumPhotonMom;
	SumPhotonMom.SetPxPyPzE(0,0,0,0);

	Int_t NGenOthers = ntuple->nGenOthers;
	for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
	{
		GenOthers genlep;
		genlep.FillFromNtuple(ntuple, i_gen);

		// -- Only for the photons whose mother is muon or anti-muon -- //
		if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
		{
			
			Double_t dR = genlep_postFSR->Momentum.DeltaR( genlep.Momentum );

			// -- Sum of all photon's momentum near the post-FSR muon -- //
			if( dR < dRCut )
			{
				SumPhotonMom  = SumPhotonMom + genlep.Momentum;
				GenPhotonCollection->push_back( genlep );
			}
		}
	}

	// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
	genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
	genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
	genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
	genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
	genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
	genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
	genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
	genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();
}

void DYAnalyzer::PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
{
	TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;

	TLorentzVector SumPhotonMom;
	SumPhotonMom.SetPxPyPzE(0,0,0,0);

	Int_t NGenOthers = ntuple->nGenOthers;
	for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
	{
		GenOthers genlep;
		genlep.FillFromNtuple(ntuple, i_gen);

		// -- all photons within dR < 0.1 -- //
		// if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
		if( fabs(genlep.ID) == 22 )
		{
			
			Double_t dR = genlep_postFSR->Momentum.DeltaR( genlep.Momentum );

			// -- Sum of all photon's momentum near the post-FSR muon -- //
			if( dR < dRCut )
			{
				SumPhotonMom  = SumPhotonMom + genlep.Momentum;
				GenPhotonCollection->push_back( genlep );
			}
		}
	}

	// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
	genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
	genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
	genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
	genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
	genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
	genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
	genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
	genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();
}

TString DYAnalyzer::DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2)
{
	TString FSRType = "";

	Bool_t isPassAcc_preFSREvent = kFALSE;
	isPassAcc_preFSREvent = Flag_Acc( preFSR1.Pt, preFSR1.eta, preFSR2.Pt, preFSR2.eta );

	Bool_t isPassAcc_postFSREvent = kFALSE;
	isPassAcc_postFSREvent = Flag_Acc( postFSR1.Pt, postFSR1.eta, postFSR2.Pt, postFSR2.eta );


	if( isPassAcc_preFSREvent == kFALSE && isPassAcc_postFSREvent == kTRUE )
		FSRType = "A";

	else if( isPassAcc_preFSREvent == kTRUE && isPassAcc_postFSREvent == kTRUE)
		FSRType = "B";
	
	else if( isPassAcc_preFSREvent == kTRUE && isPassAcc_postFSREvent == kFALSE)
		FSRType = "C";

	else if( isPassAcc_preFSREvent == kFALSE && isPassAcc_postFSREvent == kFALSE)
		FSRType = "D";
	else
	{
		cout << "ERROR: NO FSR TYPE CORRESPONDING TO THIS EVENT" << endl;
		FSRType = "NOTAssigned";
	}

	return FSRType;
}

void DYAnalyzer::GenMatching(TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection)
{
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
		if( !(this->Flag_Acc( GenLeptonCollection[0].Pt, GenLeptonCollection[0].eta, 
			GenLeptonCollection[1].Pt, GenLeptonCollection[1].eta )) )
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

void DYAnalyzer::PrintOutDoubleMuInfo( Muon mu1, Muon mu2 )
{
	printf("\t[Muon1] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu1.Pt, mu1.eta, mu1.phi, mu1.charge);
	printf("\t[Muon2] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu2.Pt, mu2.eta, mu2.phi, mu2.charge);
	Double_t reco_M = ( mu1.Momentum + mu2.Momentum ).M();
	printf("\t\tDilepton Mass: %10.5lf\n", reco_M);

}

////////////////////////////
// -- useful functions -- //
////////////////////////////
void DimuonVertexProbNormChi2_TuneP(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2)
{
	vector<double> *PtCollection1 = ntuple->vtxTrk1Pt_TuneP;
	vector<double> *PtCollection2 = ntuple->vtxTrk2Pt_TuneP;
	vector<double> *VtxProbCollection = ntuple->vtxTrkProb_TuneP;

	Int_t NPt1 = (Int_t)PtCollection1->size();
	Int_t NPt2 = (Int_t)PtCollection2->size();
	Int_t NProb = (Int_t)VtxProbCollection->size();

	if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
		cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

	// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

	for(Int_t i=0; i<NProb; i++)
	{
		// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
		if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
		{
			*VtxProb = VtxProbCollection->at(i);
			*VtxNormChi2 = ntuple->vtxTrkChi2_TuneP->at(i) / ntuple->vtxTrkNdof_TuneP->at(i);
			break;
		}
	}

	return;
}




