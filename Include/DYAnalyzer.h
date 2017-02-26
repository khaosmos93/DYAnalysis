// -- Class for common functions used in DY differential cross section measurement analysis @ 13 TeV -- //
// -- Author: KyoengPil Lee, 05 Dec. 2015 -- //
#pragma once

#include <Include/Object.h>
#include <Include/NtupleHandle.h>
#include <fstream>
#include <iostream>
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
	TString BasePath = gSystem->Getenv("KP_ANALYZER_PATH");
	return BasePath + "/";
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

	////////////////////////////
	// -- Setup MC samples -- //
	////////////////////////////
	void SetupMCsamples_v20160808_80X_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	void SetupMCsamples_v20160809_80X_MoreTrigger_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	void SetupMCsamples_v20160909_80X_reHLT_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
	void SetupMCsamples_v20161017_80X_addMuonStationVar_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
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
	Double_t EfficiencySF_EventWeight_HLTv4p2(Muon mu1, Muon mu2);
	Double_t EfficiencySF_EventWeight_HLTv4p3(Muon mu1, Muon mu2);
	Int_t FindPtBin(Double_t Pt);
	Int_t FindEtaBin(Double_t eta);

	// -- outdated -- //
	Double_t EfficiencySF_EventWeight(Muon mu1, Muon mu2, NtupleHandle *ntuple);
	Double_t EfficiencySF_EventWeight_RecoIdIso(Muon mu1, Muon mu2, NtupleHandle *ntuple);
	
	////////////////////////////
	// -- Event Selections -- //
	////////////////////////////
	Bool_t EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Mu50(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_minusDimuonVtxCut(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Zdiff_13TeV(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Dijet(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Wjet(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_CheckMoreThanOneDimuonCand(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection, Bool_t& isMoreThanOneCand); // -- output: 2 muons passing event selection conditions -- //


	Bool_t isPassAccCondition_Muon(Muon Mu1, Muon Mu2);
	Bool_t isPassAccCondition_GenLepton(GenLepton genlep1, GenLepton genlep2);
	void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu);
	void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep);
	void DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2);
	Bool_t CheckTriggerMatching( Muon mu, NtupleHandle *ntuple );

	// -- for electron channel - //
	Bool_t EventSelection_Electron(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t isPassAccCondition_Electron(Electron Elec1, Electron Elec2);
	Bool_t isPassAccCondition_GenLepton_ECALGAP(GenLepton genlep1, GenLepton genlep2);
	void CompareElectron(Electron *Elec1, Electron *Elec2, Electron *leadElec, Electron *subElec);

	// -- pre-FSR functions -- //
	void PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	void PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	TString DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2);
	Double_t Calc_dR_GenLeptons( GenLepton genlep1, GenLepton genlep2 );
	Double_t Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 );

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

void DYAnalyzer::SetupMCsamples_v20161017_80X_addMuonStationVar_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "Full" )
	{
		cout << "Please add event number information" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt15to20" ); Tag->push_back( "QCD_Pt15to20" ); Xsec->push_back( 1273190000 * 0.003 ); nEvents->push_back( 4617612.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt20to30" ); Tag->push_back( "QCD_Pt20to30" ); Xsec->push_back( 558528000 * 0.0053 ); nEvents->push_back( 4335727.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt30to50" ); Tag->push_back( "QCD_Pt30to50" ); Xsec->push_back( 139803000 * 0.01182 ); nEvents->push_back( 28343191.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt50to80" ); Tag->push_back( "QCD_Pt50to80" ); Xsec->push_back( 19222500 * 0.02276 ); nEvents->push_back( 20383912.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt80to120" ); Tag->push_back( "QCD_Pt80to120" ); Xsec->push_back( 2758420 * 0.03844 ); nEvents->push_back( 13856728.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt120to170" ); Tag->push_back( "QCD_Pt120to170" ); Xsec->push_back( 469797 * 0.05362 ); nEvents->push_back( 7925911.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt170to300" ); Tag->push_back( "QCD_Pt170to300" ); Xsec->push_back( 117989 * 0.07335 ); nEvents->push_back( 8693103.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt300to470" ); Tag->push_back( "QCD_Pt300to470" ); Xsec->push_back( 7820.25 * 0.10196 ); nEvents->push_back( 16174384.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt470to600" ); Tag->push_back( "QCD_Pt470to600" ); Xsec->push_back( 645.528 * 0.12242 ); nEvents->push_back( 5927854.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt600to800" ); Tag->push_back( "QCD_Pt600to800" ); Xsec->push_back( 187.109 * 0.13412 ); nEvents->push_back( 5867994.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt800to1000" ); Tag->push_back( "QCD_Pt800to1000" ); Xsec->push_back( 32.3486 * 0.14552 ); nEvents->push_back( 5571386.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt1000toInf" ); Tag->push_back( "QCD_Pt1000toInf" ); Xsec->push_back( 10.4305 * 0.15544 ); nEvents->push_back( 3884654.0 );

		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_WWTo2L2Nu" ); Tag->push_back( "WW" ); Xsec->push_back( 12.178 ); nEvents->push_back( 1996559.0 );

		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 32491104.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6025.2/3.0 ); nEvents->push_back( 23260465.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 35.6 ); nEvents->push_back( 984995.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 35.6 ); nEvents->push_back( 998397.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ttbarTo2L2Nu" ); Tag->push_back( "ttbar" ); Xsec->push_back( 87.31 ); nEvents->push_back( 104606887.0 );
		
		// -- signal -- //
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M50to120" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back( 2976526.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M120to200" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back( 99998.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M200to400" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back( 99999.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M400to800" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back( 98999.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M800to1400" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back( 96398.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M1400to2300" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back( 99998.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M2300to3500" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back( 100000.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M3500to4500" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back( 99000.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M4500to6000" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back( 100000.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M6000toInf" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back( 100000.0 );
	}
	else if(Type == "QCD" )
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt15to20" ); Tag->push_back( "QCD_Pt15to20" ); Xsec->push_back( 1273190000 * 0.003 ); nEvents->push_back( 4617612.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt20to30" ); Tag->push_back( "QCD_Pt20to30" ); Xsec->push_back( 558528000 * 0.0053 ); nEvents->push_back( 4335727.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt30to50" ); Tag->push_back( "QCD_Pt30to50" ); Xsec->push_back( 139803000 * 0.01182 ); nEvents->push_back( 28343191.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt50to80" ); Tag->push_back( "QCD_Pt50to80" ); Xsec->push_back( 19222500 * 0.02276 ); nEvents->push_back( 20383912.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt80to120" ); Tag->push_back( "QCD_Pt80to120" ); Xsec->push_back( 2758420 * 0.03844 ); nEvents->push_back( 13856728.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt120to170" ); Tag->push_back( "QCD_Pt120to170" ); Xsec->push_back( 469797 * 0.05362 ); nEvents->push_back( 7925911.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt170to300" ); Tag->push_back( "QCD_Pt170to300" ); Xsec->push_back( 117989 * 0.07335 ); nEvents->push_back( 8693103.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt300to470" ); Tag->push_back( "QCD_Pt300to470" ); Xsec->push_back( 7820.25 * 0.10196 ); nEvents->push_back( 16174384.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt470to600" ); Tag->push_back( "QCD_Pt470to600" ); Xsec->push_back( 645.528 * 0.12242 ); nEvents->push_back( 5927854.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt600to800" ); Tag->push_back( "QCD_Pt600to800" ); Xsec->push_back( 187.109 * 0.13412 ); nEvents->push_back( 5867994.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt800to1000" ); Tag->push_back( "QCD_Pt800to1000" ); Xsec->push_back( 32.3486 * 0.14552 ); nEvents->push_back( 5571386.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_QCD_Pt1000toInf" ); Tag->push_back( "QCD_Pt1000toInf" ); Xsec->push_back( 10.4305 * 0.15544 ); nEvents->push_back( 3884654.0 );
	}
	else if( Type == "Diboson" )
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_WWTo2L2Nu" ); Tag->push_back( "WW" ); Xsec->push_back( 12.178 ); nEvents->push_back( 1996559.0 );
	}
	else if( Type == "WJets")
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 32491104.0 ); //nEvents: sum of weights
	}
	else if( Type == "DYTauTau" )
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6025.2/3.0 ); nEvents->push_back( 23260465.0 ); //nEvents: sum of DYTauTau weights
	}
	else if( Type == "SingleTop" )
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 35.6 ); nEvents->push_back( 984995.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 35.6 ); nEvents->push_back( 998397.0 );
	}
	else if( Type == "ttbar" )
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_ttbarTo2L2Nu" ); Tag->push_back( "ttbar" ); Xsec->push_back( 87.31 ); nEvents->push_back( 104606887.0 );
	}
	else if( Type == "DYPowheg" )
	{
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M50to120" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back( 2976526.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M120to200" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back( 99998.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M200to400" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back( 99999.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M400to800" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back( 98999.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M800to1400" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back( 96398.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M1400to2300" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back( 99998.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M2300to3500" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back( 100000.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M3500to4500" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back( 99000.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M4500to6000" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back( 100000.0 );
		ntupleDirectory->push_back( "DYntuple_v20161017_80X_MINIAODv2_addMuonStationVar_reHLT_ZMuMuPowheg_M6000toInf" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back( 100000.0 );
	}
}

void DYAnalyzer::SetupMCsamples_v20160909_80X_reHLT_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "Full_M50" )
	{
		// cout << "Please add event number information" << endl;
		// -- Background Samples -- //
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304 );
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994 );
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WW" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993203 );
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 32491104.0 ); //nEvents: sum of weights
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7471235.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 23260465.0 ); //nEvents: sum of DYTauTau weights
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 984995.0 );
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 998397.0 );
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ttbar" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 92925663.0 );
		
		// -- signal -- //
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7509978.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_DYLL_M50toInf" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 23243292.0 );
	}

	if( Type == "Full_Powheg" )
	{
		cout << "Please add event number information in SetupMCsamples_v20160809_80X_MoreTrigger_MiniAODv2" << endl;
		// -- Background Samples -- //
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304 );
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994 );
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WW" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993203 );
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 32491104.0 ); //nEvents: sum of weights
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7471235.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 23260465.0 ); //nEvents: sum of DYTauTau weights
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 984995.0 );
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 998397.0 );
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ttbar" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 92925663.0 );
		
		// -- signal -- //
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M50to120" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(2976526.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M120to200" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(99998.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M200to400" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(99999.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M400to800" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(98999.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M800to1400" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(96398.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M1400to2300" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(99998.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M2300to3500" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M3500to4500" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(99000.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M4500to6000" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(100000.0);
		ntupleDirectory->push_back( "DYntuple_v20160909_80X_MINIAODv2_reHLT_ZMuMuPowheg_M6000toInf" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(100000.0);
	}
}

void DYAnalyzer::SetupMCsamples_v20160809_80X_MoreTrigger_MiniAODv2( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents )
{
	if( Type == "Full" )
	{
		// cout << "Please add event number information" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WW" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993203 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 6776626.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7471235.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6407748.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 984995.0 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 998397.0 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ttbar" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 182122678.0 );
		
		// -- signal -- //
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7509978.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M50toInf" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6409849.0 );
	}

	if( Type == "Full_M50" )
	{
		// cout << "Please add event number information" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WW" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993203 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 6776626.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7471235.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6407748.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 984995.0 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 998397.0 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ttbar" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 182122678.0 );
		
		// -- signal -- //
		// ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYMuMu_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7509978.0 ); //nEvents: sum of weights within 10<M<50
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M50toInf" ); Tag->push_back( "DYMuMu_M50toInf" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6409849.0 );
	}

	if( Type == "Full_Powheg" )
	{
		cout << "Please add event number information in SetupMCsamples_v20160809_80X_MoreTrigger_MiniAODv2" << endl;
		// -- Background Samples -- //
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ZZ" ); Tag->push_back( "ZZ" ); Xsec->push_back( 15.4 ); nEvents->push_back( 989304 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WZ" ); Tag->push_back( "WZ" ); Xsec->push_back( 66.1 ); nEvents->push_back( 999994 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WW" ); Tag->push_back( "WW" ); Xsec->push_back( 118.7 ); nEvents->push_back( 993203 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_WJets" ); Tag->push_back( "WJets" ); Xsec->push_back( 6.15e4 ); nEvents->push_back( 6776626.0 ); //nEvents: sum of weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M10to50" ); Tag->push_back( "DYTauTau_M10to50" ); Xsec->push_back( 18610.0/3.0 ); nEvents->push_back( 7471235.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_DYLL_M50toInf" ); Tag->push_back( "DYTauTau" ); Xsec->push_back( 6104/3.0 ); nEvents->push_back( 6407748.0 ); //nEvents: sum of DYTauTau weights
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tbarW" ); Tag->push_back( "tbarW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 984995.0 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ST_tW" ); Tag->push_back( "tW" ); Xsec->push_back( 38.09 ); nEvents->push_back( 998397.0 );
		ntupleDirectory->push_back( "DYntuple_v20160809_80X_MINIAODv2_MoreTrigger_ttbar" ); Tag->push_back( "ttbar" ); Xsec->push_back( 831.76 ); nEvents->push_back( 182122678.0 );
		
		// -- signal -- //
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M50to120" ); Tag->push_back( "ZMuMu_M50to120" );  Xsec->push_back(1975);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M120to200" ); Tag->push_back( "ZMuMu_M120to200" );  Xsec->push_back(19.32);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M200to400" ); Tag->push_back( "ZMuMu_M200to400" );  Xsec->push_back(2.731);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M400to800" ); Tag->push_back( "ZMuMu_M400to800" );  Xsec->push_back(0.241);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M800to1400" );  Tag->push_back( "ZMuMu_M800to1400" );  Xsec->push_back(0.01678);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M1400to2300" );  Tag->push_back( "ZMuMu_M1400to2300" );  Xsec->push_back(0.00139);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M2300to3500" );  Tag->push_back( "ZMuMu_M2300to3500" );  Xsec->push_back(0.00008948);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M3500to4500" );  Tag->push_back( "ZMuMu_M3500to4500" );  Xsec->push_back(0.000004135);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M4500to6000" );  Tag->push_back( "ZMuMu_M4500to6000" );  Xsec->push_back(4.56E-07);  nEvents->push_back(0.0);
		ntupleDirectory->push_back( "DYntuple_v20160908_80X_MINIAODv2_MoreTrigger_ZMuMuPowheg_M6000toInf" );  Tag->push_back( "ZMuMu_M6000toInf" );  Xsec->push_back(2.066E-08);  nEvents->push_back(0.0);
	}
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
	TString FilePath = "~/Physics/ZprimeAnalysis_80X/CommonCodes/Pileup/" + FileName;
	printf( "[Setup the pileup reweighting values from: %s]\n", FilePath.Data() );

	TFile *f = new TFile("~/Physics/ZprimeAnalysis_80X/CommonCodes/Pileup/" + FileName);
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

Double_t DYAnalyzer::EfficiencySF_EventWeight_RecoIdIso(Muon mu1, Muon mu2, NtupleHandle *ntuple)
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

	Double_t Eff_data_all = Eff_muon1_data * Eff_muon2_data;
	Double_t Eff_MC_all = Eff_muon1_MC * Eff_muon2_MC;

	weight = Eff_data_all / Eff_MC_all;

	return weight;
}

Bool_t DYAnalyzer::CheckTriggerMatching( Muon mu, NtupleHandle *ntuple )
{
	Bool_t isTriggerMatched = kFALSE;

	if( HLT == "HLT_IsoMu24_v* || HLT_IsoTkMu24_v*" )
	{
		if( mu.isTrigMatched(ntuple, "HLT_IsoMu24_v*") || mu.isTrigMatched(ntuple, "HLT_IsoTkMu24_v*") )
			isTriggerMatched = kTRUE;
	}
	else if( HLT == "HLT_IsoMu27_v* || HLT_IsoTkMu27_v*" )
	{
		if( mu.isTrigMatched(ntuple, "HLT_IsoMu27_v*") || mu.isTrigMatched(ntuple, "HLT_IsoTkMu27_v*") )
			isTriggerMatched = kTRUE;
	}
	else if( HLT == "HLT_Mu50_v* || HLT_TkMu50_v*" )
	{
		if( mu.isTrigMatched(ntuple, "HLT_Mu50_v*") || mu.isTrigMatched(ntuple, "HLT_TkMu50_v*") )
			isTriggerMatched = kTRUE;
	}
	else if( !HLT.Contains("||") )
	{
		if( mu.isTrigMatched(ntuple, HLT) )
			isTriggerMatched = kTRUE;
	}

	return isTriggerMatched;
}

Bool_t DYAnalyzer::EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

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
		if( this->CheckTriggerMatching(mu, ntuple) )
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
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

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
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
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
				if( this->CheckTriggerMatching(Mu, ntuple) )
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
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

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
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

// -- Test using the trigger without isolation condition: HLT_Mu50_v* -- //
Bool_t DYAnalyzer::EventSelection_Mu50(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

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
		if( mu.isTrigMatched(ntuple, HLT) )
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
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
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
				if( Mu.isTrigMatched(ntuple, HLT) )
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
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

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

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}


Bool_t DYAnalyzer::EventSelection_minusDimuonVtxCut(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

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
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			// if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && Angle < TMath::Pi() - 0.005 )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
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
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

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

				// if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 )
				if( reco_M > 10 && Angle < TMath::Pi() - 0.005 )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

// -- Event selection used for differential Z cross section measurement @ 13TeV -- // 
Bool_t DYAnalyzer::EventSelection_Zdiff_13TeV(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isTightMuon() && MuonCollection[j].trkiso < 0.10) // -- Iso should be changed with PFIso(dBeta) in order to be same selection with Z-diff x-section measurement
	        QMuonCollection.push_back( MuonCollection[j] );
	}

	Int_t nQMuons = (Int_t)QMuonCollection.size();
	if( nQMuons == 2)
	{
		Muon recolep1 = QMuonCollection[0];
		Muon recolep2 = QMuonCollection[1];

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

		// -- Opposite sign condition -- //
		Bool_t isOppositeSign = kFALSE;
		if( recolep1.charge != recolep2.charge )
			isOppositeSign = kTRUE;

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 60 && reco_M < 120 && isPassAcc == kTRUE && isOppositeSign == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedMuonCollection->push_back( recolep1 );
			SelectedMuonCollection->push_back( recolep2 );
		}
	}
	else if( nQMuons > 2 )
	{
		// -- More then 2 Qualified Muon: Select the muons with highest pT -- // 
		Double_t Pt_leading = 0;
		Muon LeadingMuon;
		Double_t i_leading = 0;
		for(Int_t i_mu1 = 0; i_mu1 < nQMuons; i_mu1++)
		{
			Muon Mu = QMuonCollection[i_mu1];

			// printf("%dth Muon: Pt = %.3lf\n", i_mu1, Mu.Pt);

			if( Mu.Pt > Pt_leading )
			{
				Pt_leading = Mu.Pt;
				LeadingMuon	= Mu;
				i_leading = i_mu1;
			}
		}

		Double_t Pt_sub = 0;
		Muon SubMuon;
		for(Int_t i_mu2=0; i_mu2 < nQMuons; i_mu2++)
		{
			if( i_mu2 == i_leading ) continue;

			Muon Mu = QMuonCollection[i_mu2];

			if( Mu.Pt > Pt_sub )
			{
				Pt_sub = Mu.Pt;
				SubMuon	= Mu;
			}
		}

		// printf("\t(Pt_leading, Pt_sub) = (%.3lf, %.3lf)\n", LeadingMuon.Pt, SubMuon.Pt);

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(LeadingMuon, SubMuon);

		// -- Opposite sign condition -- //
		Bool_t isOppositeSign = kFALSE;
		if( LeadingMuon.charge != SubMuon.charge )
			isOppositeSign = kTRUE;

		Double_t reco_M = (LeadingMuon.Momentum + SubMuon.Momentum).M();

		if( reco_M > 60 && reco_M < 120 && isPassAcc == kTRUE && isOppositeSign == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedMuonCollection->push_back( LeadingMuon );
			SelectedMuonCollection->push_back( SubMuon );
		}

	} // -- End of else if( nQMuons > 2 ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::isPassAccCondition_Muon(Muon Mu1, Muon Mu2)
{
	Bool_t isPassAcc = kFALSE;
	Muon leadMu, subMu;
	CompareMuon(&Mu1, &Mu2, &leadMu, &subMu);
	if( leadMu.Pt > LeadPtCut && fabs(leadMu.eta) < LeadEtaCut && 
		subMu.Pt  > SubPtCut  && fabs(subMu.eta)  < SubEtaCut )
		isPassAcc = kTRUE;

	return isPassAcc;
}

Bool_t DYAnalyzer::isPassAccCondition_GenLepton(GenLepton genlep1, GenLepton genlep2)
{
	Bool_t isPassAcc = kFALSE;

	GenLepton leadGenLep, subGenLep;
	CompareGenLepton(&genlep1, &genlep2, &leadGenLep, &subGenLep);
	
	if( leadGenLep.Pt > LeadPtCut && fabs(leadGenLep.eta) < LeadEtaCut &&
		subGenLep.Pt  > SubPtCut  && fabs(subGenLep.eta) < SubEtaCut )
		isPassAcc = 1;

	return isPassAcc;
}

void DYAnalyzer::CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu)
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

void DYAnalyzer::CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep)
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

void DYAnalyzer::DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2)
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

	for(Int_t i=0; i<NProb; i++)
	{
		// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
		if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
		{
			*VtxProb = VtxProbCollection->at(i);
			*VtxNormChi2 = ntuple->vtxTrkChi2->at(i) / ntuple->vtxTrkNdof->at(i);
			break;
		}
	}

	return;
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
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

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
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

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
Bool_t DYAnalyzer::EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, NtupleHandle *ntuple, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns_minus_PFIso() && elec.ecalDriven == 1 
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
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

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

Bool_t DYAnalyzer::isPassAccCondition_Electron(Electron Elec1, Electron Elec2)
{
	Bool_t isPassAcc = kFALSE;
	Electron leadElec, subElec;
	CompareElectron(&Elec1, &Elec2, &leadElec, &subElec);
	if( leadElec.Pt > LeadPtCut && fabs(leadElec.etaSC) < LeadEtaCut && !( fabs(leadElec.etaSC) > 1.4442 && fabs(leadElec.etaSC) < 1.566 ) &&
		subElec.Pt  > SubPtCut  && fabs(subElec.etaSC)  < SubEtaCut && !( fabs(subElec.etaSC) > 1.4442 && fabs(subElec.etaSC) < 1.566 ) )
		isPassAcc = kTRUE;

	return isPassAcc;
}


Bool_t DYAnalyzer::isPassAccCondition_GenLepton_ECALGAP(GenLepton genlep1, GenLepton genlep2)
{
	Bool_t isPassAcc = kFALSE;

	GenLepton leadGenLep, subGenLep;
	CompareGenLepton(&genlep1, &genlep2, &leadGenLep, &subGenLep);
	
	if( leadGenLep.Pt > LeadPtCut && fabs(leadGenLep.eta) < LeadEtaCut && !( fabs(leadGenLep.eta) > 1.4442 && fabs(leadGenLep.eta) < 1.566 ) &&
		subGenLep.Pt  > SubPtCut  && fabs(subGenLep.eta) < SubEtaCut && !( fabs(subGenLep.eta) > 1.4442 && fabs(subGenLep.eta) < 1.566 ) )
		isPassAcc = 1;

	return isPassAcc;
}

void DYAnalyzer::CompareElectron(Electron *Elec1, Electron *Elec2, Electron *leadElec, Electron *subElec)
{
    if( Elec1->Pt > Elec2->Pt )
    {
        *leadElec = *Elec1;
        *subElec = *Elec2;
    }
    else
    {
        *leadElec = *Elec2;
        *subElec = *Elec1;
    }
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
			
			Double_t dR = Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);

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
			
			Double_t dR = Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);

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
	isPassAcc_preFSREvent = isPassAccCondition_GenLepton(preFSR1, preFSR2);

	Bool_t isPassAcc_postFSREvent = kFALSE;
	isPassAcc_postFSREvent = isPassAccCondition_GenLepton(postFSR1, postFSR2);


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

Double_t DYAnalyzer::Calc_dR_GenLeptons( GenLepton genlep1, GenLepton genlep2 )
{
	Double_t eta1 = genlep1.eta;
	Double_t phi1 = genlep1.phi;

	Double_t eta2 = genlep2.eta;
	Double_t phi2 = genlep2.phi;

	Double_t diff_eta = eta1 - eta2;
	Double_t diff_phi = phi1 - phi2;

	Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
	return dR;
}

Double_t DYAnalyzer::Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 )
{
	Double_t eta1 = genlep1.eta;
	Double_t phi1 = genlep1.phi;

	Double_t eta2 = genlep2.eta;
	Double_t phi2 = genlep2.phi;

	Double_t diff_eta = eta1 - eta2;
	Double_t diff_phi = phi1 - phi2;

	Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
	return dR;
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

void DYAnalyzer::ConvertToTunePInfo( Muon &mu )
{
	// -- Use TuneP information -- //
	mu.Pt = mu.TuneP_pT;
	mu.eta = mu.TuneP_eta;
	mu.phi = mu.TuneP_phi;

	Double_t Px = mu.TuneP_Px;
	Double_t Py = mu.TuneP_Py;
	Double_t Pz = mu.TuneP_Pz;
	Double_t E = sqrt( Px*Px + Py*Py + Pz*Pz + M_Mu*M_Mu );
	mu.Momentum.SetPxPyPzE( Px, Py, Pz, E );
}

void DYAnalyzer::PrintOutDoubleMuInfo( Muon mu1, Muon mu2 )
{
	printf("\t[Muon1] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu1.Pt, mu1.eta, mu1.phi, mu1.charge);
	printf("\t[Muon2] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu2.Pt, mu2.eta, mu2.phi, mu2.charge);
	Double_t reco_M = ( mu1.Momentum + mu2.Momentum ).M();
	printf("\t\tDilepton Mass: %10.5lf\n", reco_M);

}

Bool_t DYAnalyzer::EventSelection_Dijet(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > PassingMuonCollection;
	vector< Muon > FailingMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() )
	    {
	    	if( MuonCollection[j].trkiso < 0.10 )
	    		PassingMuonCollection.push_back( MuonCollection[j] );
	    	else
	    		FailingMuonCollection.push_back( MuonCollection[j] );
	    }
	}

	Int_t nFailMuon = (Int_t)FailingMuonCollection.size();

	if( nFailMuon >= 2 ) // -- Dijet events: contains more than 2 failing muons regardless of # passing muons -- // 
	{
		if( nFailMuon == 2 )
		{
			Muon recolep1 = FailingMuonCollection[0];
			Muon recolep2 = FailingMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

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
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}

		} // -- end of if( nFailMuon == 2 ) -- //
		else // -- # failing muons > 2 -- // 
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nFailMuon; i_mu++)
			{
				Muon Mu = FailingMuonCollection[i_mu];

				// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
				for(Int_t j_mu=0; j_mu<nFailMuon; j_mu++)
				{
					Muon Mu_jth = FailingMuonCollection[j_mu];

					if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
					{
						// -- Check that this pair is within acceptance -- //
						Bool_t isPassAcc = kFALSE;
						isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

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
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- end of (# failing muons > 2) case -- //

	} // -- end of if( nFailMuon >= 2 ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_Wjet(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > PassingMuonCollection;
	vector< Muon > FailingMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isHighPtMuon_minus_dzVTX() )
	    {
	    	if( MuonCollection[j].trkiso < 0.10 )
	    		PassingMuonCollection.push_back( MuonCollection[j] );
	    	else
	    		FailingMuonCollection.push_back( MuonCollection[j] );
	    }
	}

	Int_t nFailMuon = (Int_t)FailingMuonCollection.size();
	Int_t nPassMuon = (Int_t)PassingMuonCollection.size();

	if( nFailMuon == 1 && nPassMuon == 1) // -- W+Jets events: exactly (# pass muon , # fail muon ) = (1, 1) -- //
	{
		Muon recolep1 = PassingMuonCollection[0]; // -- first one: passing muon -- //
		Muon recolep2 = FailingMuonCollection[1]; // -- second one: failing muon -- //

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

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
			SelectedMuonCollection->push_back( recolep1 ); // -- first one: passing muon -- //
			SelectedMuonCollection->push_back( recolep2 ); // -- second one: failing muon -- //
		}
	}

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_CheckMoreThanOneDimuonCand(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection, Bool_t& isMoreThanOneCand) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;
	isMoreThanOneCand = kFALSE;

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
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

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
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			isMoreThanOneCand = kTRUE;
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
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

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
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
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




