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

class MyDiffXsecTool : public DiffXsecTools
{
public:
	MyDiffXsecTool(TString version, TString Ver_CMSSW) : DiffXsecTools(version, Ver_CMSSW)
	{

	}

	void EfficiencyScaleFactor(TGraphAsymmErrors *g_Eff, TGraphAsymmErrors *g_Eff_Corr_HLTv4p2, TGraphAsymmErrors *g_Eff_Corr_HLTv4p3)
	{
		// -- Calculate efficiency scale factor for each mass bin: SF = Corrected Eff / Un-corrected Eff -- //
		TGraphAsymmErrors *g_EffCorr_HLTv4p2 = (TGraphAsymmErrors*)g_Eff->Clone();
		this->MakeRatioGraph(g_EffCorr_HLTv4p2, g_Eff_Corr_HLTv4p2, g_Eff);

		TGraphAsymmErrors *g_EffCorr_HLTv4p3 = (TGraphAsymmErrors*)g_Eff->Clone();
		this->MakeRatioGraph(g_EffCorr_HLTv4p3, g_Eff_Corr_HLTv4p3, g_Eff);

		// this->SaveCanvas_EffCorr_HLTv4p2_vs_HLTv4p3( g_EffCorr_HLTv4p2, g_EffCorr_HLTv4p3 );

		/////////////////////////////////////////////////////////////
		// -- Apply Efficiency correction factors to each Yield -- //
		/////////////////////////////////////////////////////////////
		h_yield_HLTv4p2_EffCorr = (TH1D*)h_yield_HLTv4p2_Unfolded_AccEff->Clone();
		this->ApplyEffCorr_Yield(h_yield_HLTv4p2_EffCorr, h_yield_HLTv4p2_Unfolded_AccEff, g_EffCorr_HLTv4p2);

		h_yield_HLTv4p3_EffCorr = (TH1D*)h_yield_HLTv4p3_Unfolded_AccEff->Clone();
		this->ApplyEffCorr_Yield(h_yield_HLTv4p3_EffCorr, h_yield_HLTv4p3_Unfolded_AccEff, g_EffCorr_HLTv4p3);

		/////////////////////////////
		// -- Combine the yield -- //
		/////////////////////////////
		h_yield_HLTv4p2_EffCorr->Sumw2(); h_yield_HLTv4p3_EffCorr->Sumw2();
		h_yield_EffCorr = (TH1D*)h_yield_HLTv4p2_EffCorr->Clone();
		h_yield_EffCorr->Add( h_yield_HLTv4p3_EffCorr );
	}

	void FpoF_EfficiencyScaleFactor(TGraphAsymmErrors *g_EffSF_HLTv4p2, TGraphAsymmErrors *g_EffSF_HLTv4p3)
	{
		/////////////////////////////////////////////////////////////
		// -- Apply Efficiency correction factors to each Yield -- //
		/////////////////////////////////////////////////////////////
		this->h_FpoF_yield_HLTv4p2_EffCorr = (TH1D*)this->h_FpoF_yield_HLTv4p2_Unfolded_Eff->Clone( "h_FpoF_yield_HLTv4p2_EffCorr" );
		this->ApplyEffCorr_Yield(this->h_FpoF_yield_HLTv4p2_EffCorr, this->h_FpoF_yield_HLTv4p2_Unfolded_Eff, g_EffSF_HLTv4p2);

		this->h_FpoF_yield_HLTv4p3_EffCorr = (TH1D*)this->h_FpoF_yield_HLTv4p3_Unfolded_Eff->Clone( "h_FpoF_yield_HLTv4p3_EffCorr" );
		this->ApplyEffCorr_Yield(this->h_FpoF_yield_HLTv4p3_EffCorr, this->h_FpoF_yield_HLTv4p3_Unfolded_Eff, g_EffSF_HLTv4p3);

		/////////////////////////////
		// -- Combine the yield -- //
		/////////////////////////////
		this->h_FpoF_yield_HLTv4p2_EffCorr->Sumw2(); this->h_FpoF_yield_HLTv4p3_EffCorr->Sumw2();
		this->h_FpoF_yield_EffCorr = (TH1D*)this->h_FpoF_yield_HLTv4p2_EffCorr->Clone( "h_FpoF_yield_EffCorr" );
		this->h_FpoF_yield_EffCorr->Add( this->h_FpoF_yield_HLTv4p3_EffCorr );
	}
};

class SysUncTool_BinningChoice
{
public:
	TString Type;
	TString version;
	TString Ver_CMSSW;
	TString FileLocation;
	TFile *f_cv; // -- File containing central values -- //
	TFile *f_output;

	Double_t MassBinEdges[nMassBin+1];

	TString ROOTFileName_EffSF;

	Int_t nPtBin;
	Double_t *PtBinEdges;
	Int_t nEtaBin;
	Double_t *EtaBinEdges;

	Double_t **Eff_RecoID_data;
	Double_t **Eff_RecoID_MC;

	Double_t **Eff_Iso_data;
	Double_t **Eff_Iso_MC;

	Double_t **Eff_HLTv4p2_data;
	Double_t **Eff_HLTv4p2_MC;

	Double_t **Eff_HLTv4p3_data;
	Double_t **Eff_HLTv4p3_MC;

 	TH1D *h_mass_AccTotal;
	TH1D *h_mass_AccPass;
	TH1D *h_mass_EffTotal;
	TH1D *h_mass_EffPass;

	TEfficiency *Acc_Mass;
	TEfficiency *Eff_Mass;

	TGraphAsymmErrors *g_Acc;
	TGraphAsymmErrors *g_Eff;
	TGraphAsymmErrors *g_AccEff;

	// -- After applying efficiency scale factor -- //
	TH1D *h_mass_EffPass_EffSF_HLTv4p2;
	TH1D *h_mass_EffPass_EffSF_HLTv4p3;

	TEfficiency *Eff_Mass_EffSF_HLTv4p2;
	TEfficiency *Eff_Mass_EffSF_HLTv4p3;

	TGraphAsymmErrors *g_Eff_EffSF_HLTv4p2;
	TGraphAsymmErrors *g_Eff_EffSF_HLTv4p3;

	TGraphAsymmErrors *g_EffSF_HLTv4p2;
	TGraphAsymmErrors *g_EffSF_HLTv4p3;

	Bool_t isDataDriven;

	TH1D *h_DiffXsec;

	TH1D *h_RelSysUnc_Percent;

	// -- fiducial, post-FSR results -- //
	TH1D* h_FpoF_DiffXsec;
	TFile *f_FpoF_output;

	SysUncTool_BinningChoice(TString _Type, TString _version, TString _Ver_CMSSW)
	{
		Type = _Type;
		cout << "===================================" << endl;
		cout << "[Type] " << Type << endl; 
		cout << "===================================\n" << endl;

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

		ROOTFileName_EffSF = "";

		f_output = NULL;
		f_FpoF_output = NULL;
		f_cv = NULL;

		isDataDriven = kTRUE;
		cout << "Default value for isDataDriven: " << isDataDriven << endl;

		h_DiffXsec = new TH1D("h_DiffXsec_"+Type, "", nMassBin, MassBinEdges);
		h_RelSysUnc_Percent = new TH1D("h_RelSysUnc_Percent", "", nMassBin, MassBinEdges);
	}

	void GetEffSF_FromROOTFile( TFile *file )
	{
		file->cd();
		this->g_EffSF_HLTv4p2 = (TGraphAsymmErrors*)file->Get("g_EffSF_HLTv4p2")->Clone();
		this->g_EffSF_HLTv4p3 = (TGraphAsymmErrors*)file->Get("g_EffSF_HLTv4p3")->Clone();

		cout << g_EffSF_HLTv4p2 << ", " << g_EffSF_HLTv4p3 << endl;
	}

	void SetupBinning(Int_t _nPtBin, Double_t *_PtBinEdges, Int_t _nEtaBin, Double_t *_EtaBinEdges)
	{
		nPtBin = _nPtBin;
		PtBinEdges = _PtBinEdges;

		nEtaBin = _nEtaBin;
		EtaBinEdges = _EtaBinEdges;

		cout << "\n====================================" << endl;
		cout << "[pT binning (" << nPtBin << " bins)]" << endl;
		for(Int_t i=0; i<nPtBin+1; i++)
			printf("%.1lf\t", PtBinEdges[i]);
		cout << endl;
		cout << endl;

		cout << "[eta binning (" << nEtaBin << " bins)]" << endl;
		for(Int_t i=0; i<nEtaBin+1; i++)
			printf("%.1lf\t", EtaBinEdges[i]);
		cout << endl;
		cout << "====================================\n\n" << endl;

		// -- dynmaic memory allocation for 2D array of efficiencies -- //
		Eff_RecoID_data = new Double_t * [ nEtaBin ];
		Eff_RecoID_MC = new Double_t * [ nEtaBin ];
		Eff_Iso_data = new Double_t * [ nEtaBin ];
		Eff_Iso_MC = new Double_t * [ nEtaBin ];
		Eff_HLTv4p2_data = new Double_t * [ nEtaBin ];
		Eff_HLTv4p2_MC = new Double_t * [ nEtaBin ];
		Eff_HLTv4p3_data = new Double_t * [ nEtaBin ];
		Eff_HLTv4p3_MC = new Double_t * [ nEtaBin ];

		for( int i=0; i < nEtaBin; i++ )
		{
		        Eff_RecoID_data[i] = new Double_t [ nPtBin ];
		        Eff_RecoID_MC[i] = new Double_t [ nPtBin ];
		        Eff_Iso_data[i] = new Double_t [ nPtBin ];
		        Eff_Iso_MC[i] = new Double_t [ nPtBin ];
		        Eff_HLTv4p2_data[i] = new Double_t [ nPtBin ];
		        Eff_HLTv4p2_MC[i] = new Double_t [ nPtBin ];
		        Eff_HLTv4p3_data[i] = new Double_t [ nPtBin ];
		        Eff_HLTv4p3_MC[i] = new Double_t [ nPtBin ];
		}

	}

	void SetupEfficiencyScaleFactor(TString _ROOTFileName)
	{
		ROOTFileName_EffSF = _ROOTFileName;
		TString Location_TnP = "./" + ROOTFileName_EffSF;
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

		if( (nPtBins != nPtBin) || (nEtaBins != nEtaBin) )
		{
			printf("Wrong # bins! ... # pT and eta bin in histograms: (%d, %d)\n", nPtBins, nEtaBins);
			return;
		}

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

	Double_t EfficiencySF_EventWeight_HLTv4p2(Muon mu1, Muon mu2)
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

	Double_t EfficiencySF_EventWeight_HLTv4p3(Muon mu1, Muon mu2)
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

	Int_t FindPtBin(Double_t Pt)
	{
		Int_t ptbin = 9999;

		// -- if Pt is larger than the largest Pt bin edge, SF is same with the value for the last bin -- // 
		if( Pt > PtBinEdges[nPtBin] )
			ptbin = nPtBin-1;
		else
		{
			for(Int_t i=0; i<nPtBin; i++)
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

	Int_t FindEtaBin(Double_t eta)
	{
		Int_t etabin = 9999;

		for(Int_t i=0; i<nEtaBin; i++)
		{
			if( eta > EtaBinEdges[i] && eta < EtaBinEdges[i+1] )
			{
				etabin = i;
				break;
			}
		}

		return etabin;
	}

	void CalcEfficiencies(Bool_t isCorrected = kTRUE, TString Sample = "aMCNLO", TString HLTname = "IsoMu20_OR_IsoTkMu20")
	{
		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
		cout << "Sample: " << Sample << endl;

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

		DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

		TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_" + Sample + "_" + HLTname + ".root", "RECREATE");

	 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

	 	h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", nMassBin, MassBinEdges);
		h_mass_AccPass = new TH1D("h_mass_AccPass", "", nMassBin, MassBinEdges);
		h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", nMassBin, MassBinEdges);
		h_mass_EffPass = new TH1D("h_mass_EffPass", "", nMassBin, MassBinEdges);	 

		// -- After applying efficiency correction -- //
		h_mass_EffPass_EffSF_HLTv4p2 = new TH1D("h_mass_EffPass_EffSF_HLTv4p2", "", nMassBin, MassBinEdges);
		h_mass_EffPass_EffSF_HLTv4p3 = new TH1D("h_mass_EffPass_EffSF_HLTv4p3", "", nMassBin, MassBinEdges);

		TString BaseLocation = "/data4/Users/kplee/DYntuple";
		// -- Each ntuple directory & corresponding Tags -- //
			// -- GenWeights are already taken into account in nEvents -- //
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

		if( Sample == "aMCNLO" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("aMCNLO_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}
		if( Sample == "Powheg" )
		{
			analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
		}

		// -- Loop for all samples -- //
		const Int_t Ntup = ntupleDirectory.size();
		for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
		{
			TStopwatch looptime;
			looptime.Start();

			cout << "\t<" << Tag[i_tup] << ">" << endl;

			TChain *chain = new TChain("recoTree/DYTree");
			chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
			
			NtupleHandle *ntuple = new NtupleHandle( chain );
			ntuple->TurnOnBranches_Muon();
			ntuple->TurnOnBranches_GenLepton();

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

			Int_t NEvents = chain->GetEntries();
			cout << "\t[Total Events: " << NEvents << "]" << endl; 

			Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
			cout << "\t[Normalization factor: " << norm << "]" << endl;

			// if( NEvents > 1000000 ) NEvents = 1000000;

			// -- Event loop starts -- //
			for(Int_t i=0; i<NEvents; i++)
			{
				this->loadBar(i+1, NEvents, 100, 100);

				ntuple->GetEvent(i);

				//Bring weights for NLO MC events
				Double_t GenWeight;
				if( isNLO == 1 )
					ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
				else
					GenWeight = 1;

				SumWeights += GenWeight;

				Int_t PU = ntuple->nPileUp;
				Double_t PUWeight = analyzer->PileUpWeightValue_76X( PU );

				Double_t TotWeight = norm * GenWeight;

				Bool_t GenFlag = kFALSE;
				GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

				if( GenFlag == kTRUE )
				{
					SumWeights_Separated += GenWeight;

					// -- Collect gen-level information -- //
					vector<GenLepton> GenLeptonCollection;
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

					// -- Flags -- //
					Bool_t Flag_PassAcc = kFALSE;
					Bool_t Flag_PassAccEff = kFALSE;

					Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

					// -- Acceptance Calculation -- //
					if( Flag_PassAcc == kTRUE ) 
					{
						h_mass_AccTotal->Fill( gen_M, TotWeight );
						h_mass_AccPass->Fill( gen_M, TotWeight );
					}
					else
					{
						h_mass_AccTotal->Fill( gen_M, TotWeight );
					} 	

					Double_t Eff_SF_HLTv4p2 = -999; // -- Efficiency correction factor -- //
					Double_t Eff_SF_HLTv4p3 = -999; // -- Efficiency correction factor -- //

					// -- Calculate the efficiency among the events passing acceptance condition -- //
					if( Flag_PassAcc == kTRUE )
					{
						Bool_t Flag_PassEff = kFALSE;

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
							Bool_t isPassEventSelection = kFALSE;
							isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

							if( isPassEventSelection == kTRUE )
							{
								Eff_SF_HLTv4p2 = this->EfficiencySF_EventWeight_HLTv4p2( SelectedMuonCollection[0], SelectedMuonCollection[1] );
								Eff_SF_HLTv4p3 = this->EfficiencySF_EventWeight_HLTv4p3( SelectedMuonCollection[0], SelectedMuonCollection[1] );

								// printf("( SF_HLTv4p2, SF_HLTv4p3 ) = (%.5lf, %.5lf)\n", Eff_SF_HLTv4p2, Eff_SF_HLTv4p3);

								Flag_PassEff = kTRUE;
								Flag_PassAccEff = kTRUE;
							}

						} // -- End of if( ntuple->isTriggered( HLT ) ) -- //

						// -- Efficiency Calculation -- //
						if( Flag_PassEff == kTRUE)
						{
							h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );
							h_mass_EffPass->Fill( gen_M, TotWeight * PUWeight );
							h_mass_EffPass_EffSF_HLTv4p2->Fill( gen_M, TotWeight * PUWeight * Eff_SF_HLTv4p2 );
							h_mass_EffPass_EffSF_HLTv4p3->Fill( gen_M, TotWeight * PUWeight * Eff_SF_HLTv4p3 );
						}
						else
							h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );

					} // -- End of if( Flag_PassAcc )

				} // -- End of if( GenFlag == kTRUE )

			} //End of event iteration

			cout << "Total Sum of Weight: " << SumWeights << endl;
			cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

			Double_t LoopRunTime = looptime.CpuTime();
			cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
		} //end of i_tup iteration

		// -- Acceptance -- //
		Acc_Mass = new TEfficiency(*h_mass_AccPass, *h_mass_AccTotal);
		Acc_Mass->SetName("TEff_Acc_Mass");
		g_Acc = (TGraphAsymmErrors*)Acc_Mass->CreateGraph()->Clone();

		// -- Efficiency -- //
		Eff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_EffTotal);
		Eff_Mass->SetName("TEff_Eff_Mass");
		g_Eff = (TGraphAsymmErrors*)Eff_Mass->CreateGraph()->Clone();

		// -- Acceptance * Efficiency -- //
		g_AccEff = (TGraphAsymmErrors*)g_Eff->Clone();
		MakeAccEffGraph( g_AccEff, g_Acc, g_Eff );

		// -- Efficiency (after Eff.SF (HLTv4.2) is weighted) -- //
		Eff_Mass_EffSF_HLTv4p2 = new TEfficiency(*h_mass_EffPass_EffSF_HLTv4p2, *h_mass_EffTotal);
		Eff_Mass_EffSF_HLTv4p2->SetName("TEff_Eff_Mass_EffSF_HLTv4p2");
		g_Eff_EffSF_HLTv4p2 = (TGraphAsymmErrors*)Eff_Mass_EffSF_HLTv4p2->CreateGraph()->Clone();

		// -- Efficiency (after Eff.SF (HLTv4.3) is weighted) -- //
		Eff_Mass_EffSF_HLTv4p3 = new TEfficiency(*h_mass_EffPass_EffSF_HLTv4p3, *h_mass_EffTotal);
		Eff_Mass_EffSF_HLTv4p3->SetName("TEff_Eff_Mass_EffSF_HLTv4p3");
		g_Eff_EffSF_HLTv4p3 = (TGraphAsymmErrors*)Eff_Mass_EffSF_HLTv4p3->CreateGraph()->Clone();

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

	void Calc_EffCorr()
	{
		MyCanvas *myc_HLTv4p2 = new MyCanvas("c_UnCorr_vs_Corr_HLTv4p2", "Gen-Level Dimuon Mass [GeV]", "Efficiency");
		myc_HLTv4p2->SetLogx(1);
		myc_HLTv4p2->CanvasWithGraphRatioPlot( (TGraphAsymmErrors*)g_Eff_EffSF_HLTv4p2->Clone(), (TGraphAsymmErrors*)g_Eff->Clone(),
										"Efficiency (Eff.SF_HLTv4.2)", "Efficiency", "After/Before",
										kBlue+1, kGreen+1,
										"EP", "EPSAME");

		MyCanvas *myc_HLTv4p3 = new MyCanvas("c_UnCorr_vs_Corr_HLTv4p3", "Gen-Level Dimuon Mass [GeV]", "Efficiency");
		myc_HLTv4p3->SetLogx(1);
		myc_HLTv4p3->CanvasWithGraphRatioPlot( (TGraphAsymmErrors*)g_Eff_EffSF_HLTv4p3->Clone(), (TGraphAsymmErrors*)g_Eff->Clone(),
										"Efficiency (Eff.SF_HLTv4.3)", "Efficiency", "After/Before",
										kBlue+1, kGreen+1,
										"EP", "EPSAME");

		g_EffSF_HLTv4p2 = (TGraphAsymmErrors*)myc_HLTv4p2->g_ratio->Clone();
		g_EffSF_HLTv4p2->SetName("g_EffSF_HLTv4p2");

		g_EffSF_HLTv4p3 = (TGraphAsymmErrors*)myc_HLTv4p3->g_ratio->Clone();
		g_EffSF_HLTv4p3->SetName("g_EffSF_HLTv4p3");
	}

	void SetIsDataDriven(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;
		cout << "\n=================================" << endl;
		cout << "isDataDriven = " << isDataDriven << endl;
		cout << "=================================\n\n" << endl;

	}

	void CalcDiffXsec_GivenEffSF()
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
		XsecTool->MakeSignalMCHistograms();

		XsecTool->GetYieldHistograms(isDataDriven);
		XsecTool->UnfoldingCorrection();
		XsecTool->AccEffCorrection();

		XsecTool->EfficiencyScaleFactor(g_Eff, g_Eff_EffSF_HLTv4p2, g_Eff_EffSF_HLTv4p3);
		XsecTool->FSRCorrection();
		XsecTool->CalcXsec();

		CopyHistogram( XsecTool->h_DiffXsec_FSRCorr, h_DiffXsec );
	}

	void ComparisonPlots_WithCentralValue()
	{
		TFile *f_cv_EffSF = TFile::Open(FileLocation + "/ROOTFile_AccEff.root");
		TGraphAsymmErrors *g_EffSF_HLTv4p2_CV = (TGraphAsymmErrors*)f_cv_EffSF->Get("g_EffCorr_HLTv4p2")->Clone();
		TGraphAsymmErrors *g_EffSF_HLTv4p3_CV = (TGraphAsymmErrors*)f_cv_EffSF->Get("g_EffCorr_HLTv4p3")->Clone();

		MyCanvas *myc_EffSF_HLTv4p2_CV_vs_Type = new MyCanvas("c_EffSF_HLTv4p2_CV_vs_"+Type, "Dimuon Mass(HLT v4.2) [GeV]", "Efficiency SF");
		myc_EffSF_HLTv4p2_CV_vs_Type->SetLogx(1);
		myc_EffSF_HLTv4p2_CV_vs_Type->SetYRange(0.8, 1.15);
		myc_EffSF_HLTv4p2_CV_vs_Type->SetRatioRange(0.95, 1.05);
		myc_EffSF_HLTv4p2_CV_vs_Type->CanvasWithGraphRatioPlot( g_EffSF_HLTv4p2, g_EffSF_HLTv4p2_CV, Type, "Central value", Type+"/C.V.");
		myc_EffSF_HLTv4p2_CV_vs_Type->PrintCanvas();

		MyCanvas *myc_EffSF_HLTv4p3_CV_vs_Type = new MyCanvas("c_EffSF_HLTv4p3_CV_vs_"+Type, "Dimuon Mass(HLT v4.3) [GeV]", "Efficiency SF");
		myc_EffSF_HLTv4p3_CV_vs_Type->SetLogx(1);
		myc_EffSF_HLTv4p3_CV_vs_Type->SetYRange(0.8, 1.15);
		myc_EffSF_HLTv4p3_CV_vs_Type->SetRatioRange(0.95, 1.05);
		myc_EffSF_HLTv4p3_CV_vs_Type->CanvasWithGraphRatioPlot( g_EffSF_HLTv4p3, g_EffSF_HLTv4p3_CV, Type, "Central value", Type+"/C.V.");
		myc_EffSF_HLTv4p3_CV_vs_Type->PrintCanvas();

		f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
		TH1D *h_DiffXsec_CV = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr")->Clone();

		MyCanvas *myc_DiffXsec = new MyCanvas("c_DiffXsec_CV_vs_"+Type, "Dimuon Mass [GeV]", "d#sigma/dM [pb/GeV]");
		myc_DiffXsec->SetLogx(1);
		myc_DiffXsec->SetLogy(0);
		myc_DiffXsec->SetRatioRange(0.95, 1.05);
		myc_DiffXsec->CanvasWithHistogramsRatioPlot(h_DiffXsec, h_DiffXsec_CV, Type, "Central value", Type+"/C.V.");
		myc_DiffXsec->PrintCanvas();

		if( f_output == NULL )
			f_output = new TFile("ROOTFile_SysUnc_BinningChoice_"+Type+".root", "RECREATE");
		f_output->cd();
		myc_EffSF_HLTv4p2_CV_vs_Type->c->Write();
		myc_EffSF_HLTv4p3_CV_vs_Type->c->Write();
		myc_DiffXsec->c->Write();
	}

	void Calc_SysUnc()
	{
		if( f_cv == NULL )
			f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
		TH1D *h_DiffXsec_CV = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr")->Clone();

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			
			Double_t central_value = h_DiffXsec_CV->GetBinContent(i_bin);
			Double_t alter_value = h_DiffXsec->GetBinContent(i_bin);

			Double_t AbsUnc = fabs( central_value - alter_value );
			Double_t RelUnc = AbsUnc / central_value;

			h_RelSysUnc_Percent->SetBinContent(i_bin, RelUnc);
			h_RelSysUnc_Percent->SetBinError(i_bin, 0);
		}
		h_RelSysUnc_Percent->Scale( 100 ); // -- percent -- //

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

	void MakeAccEffGraph(TGraphAsymmErrors *g_AccEff, TGraphAsymmErrors *g_Acc, TGraphAsymmErrors *g_Eff)
	{
		g_AccEff->Set(0);
		Int_t nPoint = g_Acc->GetN();
		for(Int_t i=0; i<nPoint; i++)
		{
			Double_t x_Acc, y_Acc, error_Acc, x_Eff, y_Eff, error_Eff;
			g_Acc->GetPoint(i, x_Acc, y_Acc);
			error_Acc = g_Acc->GetErrorYhigh(i);

			if( error_Acc != g_Acc->GetErrorYlow(i) )
			{
				printf("Acceptance has asymmetric error! ... (Low, High) = (%.5lf, %.5lf) : use larger one.\n", g_Acc->GetErrorYlow(i), error_Acc);

				if( g_Acc->GetErrorYlow(i) > error_Acc )
					error_Acc = g_Acc->GetErrorYlow(i);
			}
				
				// cout << "Acceptance has asymmetric error! ... (Low, High) = " << (Double_t)g_Acc->GetErrorYlow(i) << ", " << error_Acc << endl;

			g_Eff->GetPoint(i, x_Eff, y_Eff);
			error_Eff = g_Eff->GetErrorYhigh(i);

			if( error_Eff != g_Eff->GetErrorYlow(i) )
				printf("Efficiency has asymmetric error! ... (Low, High) = (%.5lf, %.5lf)\n", g_Eff->GetErrorYlow(i), error_Acc);

			Double_t x_AccEff, y_AccEff, error_AccEff;

			x_AccEff = x_Acc;
			y_AccEff = y_Acc * y_Eff;
			error_AccEff = Error_PropagatedAoverB(y_Acc, error_Acc, y_Eff, error_Eff);

			g_AccEff->SetPoint(i, x_AccEff, y_AccEff);
			g_AccEff->SetPointError(i, g_Acc->GetErrorXlow(i), g_Acc->GetErrorXhigh(i), error_AccEff, error_AccEff);
		}

	}

	Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
	{
		Double_t ratio_A = (sigma_A) / A;
		Double_t ratio_B = (sigma_B) / B;

		Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

		return (A/B) * sqrt(errorSquare);
	}

	void SaveResults()
	{
		if( f_output == NULL )
			f_output = new TFile("ROOTFile_SysUnc_BinningChoice_"+Type+".root", "RECREATE");

		f_output->cd();

		h_mass_AccTotal->Write();
		h_mass_AccPass->Write();
		h_mass_EffTotal->Write();
		h_mass_EffPass->Write();
		h_mass_EffPass_EffSF_HLTv4p2->Write();
		h_mass_EffPass_EffSF_HLTv4p3->Write();

		Acc_Mass->Write();
		Eff_Mass->Write();
		Eff_Mass_EffSF_HLTv4p2->Write();
		Eff_Mass_EffSF_HLTv4p3->Write();

		g_Acc->Write();
		g_Eff->Write();
		g_AccEff->Write();

		g_Eff_EffSF_HLTv4p2->Write();
		g_Eff_EffSF_HLTv4p3->Write();

		g_EffSF_HLTv4p2->Write();
		g_EffSF_HLTv4p3->Write();

		h_DiffXsec->Write();

		h_RelSysUnc_Percent->Write();
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

	void FpoF_Calc_SysUnc()
	{
		if( f_cv == NULL )
			f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");

		f_cv->cd();
		TH1D *h_FpoF_DiffXsec_CV = (TH1D*)f_cv->Get("h_FpoF_DiffXsec_Data")->Clone();

		this->h_FpoF_DiffXsec = this->FpoF_CalcDiffXsec_GivenEffSF();

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			
			Double_t central_value = h_FpoF_DiffXsec_CV->GetBinContent(i_bin);
			Double_t alter_value = h_FpoF_DiffXsec->GetBinContent(i_bin);

			Double_t AbsUnc = fabs( central_value - alter_value );
			Double_t RelUnc = AbsUnc / central_value;

			h_RelSysUnc_Percent->SetBinContent(i_bin, RelUnc);
			h_RelSysUnc_Percent->SetBinError(i_bin, 0);
		}
		h_RelSysUnc_Percent->Scale( 100 ); // -- percent -- //

	}

	TH1D* FpoF_CalcDiffXsec_GivenEffSF()
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
		XsecTool->MakeSignalMCHistograms();
		XsecTool->GetYieldHistograms(isDataDriven);
		XsecTool->UnfoldingCorrection();

		XsecTool->FpoF_GetTheoryHist();
		XsecTool->FpoF_EffCorrection();
		
		cout << g_EffSF_HLTv4p2 << ", " << g_EffSF_HLTv4p3 << endl;

		XsecTool->FpoF_EfficiencyScaleFactor(this->g_EffSF_HLTv4p2, this->g_EffSF_HLTv4p3);
		XsecTool->FpoF_CalcXsec();

		return XsecTool->h_FpoF_DiffXsec_Data;
	}

	void FpoF_SaveResults()
	{
		if( f_FpoF_output == NULL )
			f_FpoF_output = new TFile("ROOTFile_FpoF_SysUnc_BinningChoice_"+Type+".root", "RECREATE");

		f_FpoF_output->cd();

		g_EffSF_HLTv4p2->Write();
		g_EffSF_HLTv4p3->Write();

		h_FpoF_DiffXsec->Write();

		h_RelSysUnc_Percent->Write();
	}
};