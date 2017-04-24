#include "TagProbeEffTools.h"

void TagProbe_ExtractEfficiency_CalcSysUnc_2D()
{
	// gROOT->SetBatch(kTRUE);
	
	TFile *f_output = new TFile("ROOTFile_ExtractEfficiency_CalcSysUnc_2D_sgnChange.root", "RECREATE");

	const Int_t nPtBin = 4;
	Double_t PtBinEdges[nPtBin+1] = { 10, 22, 40, 70, 250 };

	const Int_t nEtaBin = 5;
	Double_t EtaBinEdges[nEtaBin+1] = { -2.4, -1.2, -0.3, 0.3, 1.2, 2.4 };

	TagProbeTools *tools = new TagProbeTools();
	tools->TreeName = "muonEffs";
	tools->SetBinEdges_2D( nEtaBin, EtaBinEdges, "Eta", "Muon #eta", nPtBin, PtBinEdges, "Pt", "Muon P_{T} [GeV]" );

	const Int_t nFiles = 8;
	TString FileName[nFiles] = {"../TnPConfigs/RecoID_Iso/TnP_MuonTrigger_data_25ns_HighPtMuon_pteta.root",
								"../TnPConfigs/RecoID_Iso/TnP_MuonTrigger_mc_weight_HighPtMuon_pteta.root",
								"../TnPConfigs/RecoID_Iso/TnP_MuonTrigger_data_25ns_TrkIso_010_from_HighPtMuon_pteta.root",
								"../TnPConfigs/RecoID_Iso/TnP_MuonTrigger_mc_weight_TrkIso_010_from_HighPtMuon_pteta.root",
								"../TnPConfigs/HLTv4p2/TnP_MuonTrigger_data_25ns_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010_pteta.root",
								"../TnPConfigs/HLTv4p2/TnP_MuonTrigger_mc_weight_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010_pteta.root",
								"../TnPConfigs/HLTv4p3/TnP_MuonTrigger_data_25ns_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010_pteta.root",
								"../TnPConfigs/HLTv4p3/TnP_MuonTrigger_mc_weight_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010_pteta.root" };

	for(Int_t i_file=0; i_file<nFiles; i_file++)
	{
		TFile *f_input = new TFile( FileName[i_file] );

		TString Type = "";
		if( FileName[i_file].Contains("_mc_weight_") )
			Type = "MC";
		else
			Type = "Data";

		TString EffName = "";
		if( i_file < 2 )
		{
			EffName = "RecoID";
			tools->zMin = 0.8; tools->zMax = 1.01;
		}
		else if( i_file < 4 )
		{
			EffName = "Iso";
			tools->zMin = 0.8; tools->zMax = 1.01;
		}
		else if( i_file < 6 )
		{
			EffName = "HLTv4p2";
			tools->zMin = -0.01; tools->zMax = 1.01;
		}
		else if( i_file < 8 )
		{
			EffName = "HLTv4p3";
			tools->zMin = -0.01; tools->zMax = 1.01;
		}

		tools->SaveFitCanvases_2D( f_input, EffName, Type);

		cout << "\tFileName: " << FileName[i_file] << ", EffName: " << EffName << ", Type: " << Type << endl;

		tools->SaveEffGraphs_2D( f_input, EffName, Type, f_output );

	} // -- end of for(Int_t i_file=0; i_file<nFiles; i_file++) -- //

	tools->zMin = 0.8; tools->zMax = 1.2;

	tools->SaveComparisonPlots_2D( f_output, "RecoID", "Data", "MC", f_output );
	tools->SaveComparisonPlots_2D( f_output, "Iso", "Data", "MC", f_output );
	tools->SaveComparisonPlots_2D( f_output, "HLTv4p2", "Data", "MC", f_output );
	tools->SaveComparisonPlots_2D( f_output, "HLTv4p3", "Data", "MC", f_output );

	/////////////////////////////////////////////////////////////////////////////
	// -- Calculate systematic uncertainty (compare with the central value) -- //
	/////////////////////////////////////////////////////////////////////////////
	TFile *f_cv = new TFile("/Users/KyeongPil_Lee/Research/TagProbe/DYAnalysis_76X/v20160501_1st_CentralValue_UpdateFitCanvases/TnPResults/ROOTFile_TagProbeEfficiency_76X_v20160502.root");

	vector<TString> EffNames;
	EffNames.push_back("RecoID"); EffNames.push_back("Iso"); EffNames.push_back("HLTv4p2"); EffNames.push_back("HLTv4p3");
	vector<TString> Types;
	Types.push_back("Data"); Types.push_back("MC");

	tools->zMin = 0; tools->zMax = 0.1;

	const Int_t nSource = 8;
	TString Sources[nSource] = {"sgnChange", "bkgChange", "nBin30", "nBin50", "M60to130", "M70to120", "TagPt20", "TagPt24"};
	
	tools->Calc_Difference_2D("sgnChange", EffNames, Types, f_cv, f_output, f_output);
}