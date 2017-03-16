#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooGlobalFunc.h>
#include <RooPlot.h>

// #include "StatUnc_StepByStep.h"
#define nTest 100

using namespace RooFit;

class HistogramContainer
{
public:
	// Double_t MassBinEdges[nMassBin+1];
	TString FileLocation;

	TString Type;

	TH1D* h_data_4p2;
	TH1D* h_data_4p3;

	TH1D* h_BkgSub_4p2;
	TH1D* h_BkgSub_4p3;

	TH1D* h_Unfolded_4p2;
	TH1D* h_Unfolded_4p3;

	TH1D* h_AccEffCorr_4p2;
	TH1D* h_AccEffCorr_4p3;

	TH1D* h_EffSFCorr_4p2;
	TH1D* h_EffSFCorr_4p3;

	TH1D* h_FSRCorr;

	TH1D* h_DiffXSec;

	HistogramContainer()
	{
		// Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
		// 									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
		// 									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
		// 									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
		// 									 830, 1000, 1500, 3000};

		// for(Int_t i=0; i<nMassBin+1; i++)
		// 	this->MassBinEdges[i] = MassBinEdges_temp[i];

		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");

	}

	void Set_Type( TString _Type )
	{
		this->Type = _Type;
	}

	void Load_Histogram( TFile *f_input, TString _Type )
	{
		TString HistName = "";

		HistName = "h_data_4p2_"+_Type;
		this->h_data_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_data_4p3_"+_Type;
		this->h_data_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_BkgSub_4p2_"+_Type;
		this->h_BkgSub_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_BkgSub_4p3_"+_Type;
		this->h_BkgSub_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_Unfolded_4p2_"+_Type;
		this->h_Unfolded_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_Unfolded_4p3_"+_Type;
		this->h_Unfolded_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_AccEffCorr_4p2_"+_Type;
		this->h_AccEffCorr_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_AccEffCorr_4p3_"+_Type;
		this->h_AccEffCorr_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_EffSFCorr_4p2_"+_Type;
		this->h_EffSFCorr_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_EffSFCorr_4p3_"+_Type;
		this->h_EffSFCorr_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_FSRCorr_"+_Type;
		this->h_FSRCorr = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_DiffXSec_"+_Type;
		this->h_DiffXSec = (TH1D*)f_input->Get(HistName)->Clone();
	}
};

TTree* makeTTree_RelDiff_DEN_BkgSub_NUM_Unfolding(Int_t i_bin, HistogramContainer *Hists_CV, HistogramContainer *Hists_Smeared[nTest]);
void RelUnc_DEN_BkgSub_NUM_Unfolding(Int_t i_bin, HistogramContainer *Hists_CV, HistogramContainer *Hists_Smeared[nTest] );

void Check_nEvents_Replicas()
{
	TFile *f_input = TFile::Open("./Local/ROOTFile_CentralValue_and_Replicas.root");

	HistogramContainer *Hists_CV = new HistogramContainer();
	Hists_CV->Set_Type( "CV" );
	Hists_CV->Load_Histogram( f_input, Hists_CV->Type );

	HistogramContainer *Hists_Smeared[nTest];
	for(Int_t i=0; i<nTest; i++)
	{
		TString Type = TString::Format("Smeared_%d", i);

		Hists_Smeared[i] = new HistogramContainer();
		Hists_Smeared[i]->Load_Histogram( f_input, Type );
	}

	Int_t i_bin = 15; // -- 81 < M < 86 GeV -- //

	Int_t nEvent_largerRelDiff = 0;
	Double_t Sum_RelDiff_Observed = 0;
	Double_t Sum_RelDiff_Observed_LargerRelDiff = 0;
	for(Int_t i=0; i<nTest; i++)
	{
		printf("[%02d-th test]\n", i);
		Double_t Observed_CV = Hists_CV->h_data_4p3->GetBinContent(i_bin) + Hists_CV->h_data_4p2->GetBinContent(i_bin);
		Double_t Observed_Smeared = Hists_Smeared[i]->h_data_4p3->GetBinContent(i_bin) + Hists_Smeared[i]->h_data_4p2->GetBinContent(i_bin);
		Double_t AbsDiff_Observed = (Observed_Smeared - Observed_CV);
		Double_t RelDiff_Observed =  AbsDiff_Observed / Observed_CV;

		printf( "[Observed] Rel.Diff = (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf %%\n", Observed_Smeared, Observed_CV, AbsDiff_Observed, Observed_CV, RelDiff_Observed*100 );

		Double_t BkgSub_CV = Hists_CV->h_BkgSub_4p3->GetBinContent(i_bin) + Hists_CV->h_BkgSub_4p2->GetBinContent(i_bin);
		Double_t BkgSub_Smeared = Hists_Smeared[i]->h_BkgSub_4p3->GetBinContent(i_bin) + Hists_Smeared[i]->h_BkgSub_4p2->GetBinContent(i_bin);
		Double_t AbsDiff_BkgSub = BkgSub_Smeared - BkgSub_CV; 
		Double_t RelDiff_BkgSub = AbsDiff_Observed / BkgSub_CV;

		printf( "[BkgSub] Rel.Diff = (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf %%\n", BkgSub_Smeared, BkgSub_CV, AbsDiff_BkgSub, BkgSub_CV, RelDiff_BkgSub*100 );

		Double_t Unfolded_CV = Hists_CV->h_Unfolded_4p3->GetBinContent(i_bin) + Hists_CV->h_Unfolded_4p2->GetBinContent(i_bin);
		Double_t Unfolded_Smeared = Hists_Smeared[i]->h_Unfolded_4p3->GetBinContent(i_bin) + Hists_Smeared[i]->h_Unfolded_4p2->GetBinContent(i_bin);
		Double_t AbsDiff_Unfolded = Unfolded_Smeared - Unfolded_CV;
		Double_t RelDiff_Unfolded = AbsDiff_Unfolded / Unfolded_CV;

		printf( "[Unfolded] Rel.Diff = (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf %%\n", Unfolded_Smeared, Unfolded_CV, AbsDiff_Unfolded, Unfolded_CV, RelDiff_Unfolded*100 );

		Sum_RelDiff_Observed += RelDiff_Observed;
		if( fabs(RelDiff_Unfolded) > fabs(RelDiff_BkgSub) )
		{
			nEvent_largerRelDiff++;
			Sum_RelDiff_Observed_LargerRelDiff += RelDiff_Observed;
		}

		// if( fabs(RelDiff_Unfolded) > fabs(RelDiff_BkgSub) )
		// {
		// 	printf( "[Observed] Rel.Diff = (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf\n", Observed_Smeared, Observed_CV, AbsDiff_Observed, Observed_CV, RelDiff_Observed*100 );
		// 	printf( "[BkgSub] Rel.Diff = (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf\n", BkgSub_Smeared, BkgSub_CV, AbsDiff_BkgSub, BkgSub_CV, RelDiff_BkgSub*100 );
		// 	printf( "[Unfolded] Rel.Diff = (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf\n", Unfolded_Smeared, Unfolded_CV, AbsDiff_Unfolded, Unfolded_CV, RelDiff_Unfolded*100 );
		// 	printf( "\n" );
		// }

		printf("[CV] (Observed, BkgSub, Unfolded) = (%.1lf, %.1lf, %.1lf)\n", Observed_CV, BkgSub_CV, Unfolded_CV);
		Double_t RelDiff_Observed_BkgSub_CV = (BkgSub_CV - Observed_CV) / Observed_CV;
		Double_t RelDiff_BkgSub_Unfolded_CV = (Unfolded_CV - BkgSub_CV) / BkgSub_CV;
		printf("[Diff. in each step] (Observed->BkgSub, BkgSub->Unfolded) = (%.3lf %%, %.3lf %%)\n", RelDiff_Observed_BkgSub_CV*100, RelDiff_BkgSub_Unfolded_CV*100 );

		printf("[Smeared] (Observed, BkgSub, Unfolded) = (%.1lf, %.1lf, %.1lf)\n", Observed_Smeared, BkgSub_Smeared, Unfolded_Smeared);
		Double_t RelDiff_Observed_BkgSub_Smeared = (BkgSub_Smeared - Observed_Smeared) / Observed_Smeared;
		Double_t RelDiff_BkgSub_Unfolded_Smeared = (Unfolded_Smeared - BkgSub_Smeared) / BkgSub_Smeared;
		printf("[Diff. in each step] (Observed->BkgSub, BkgSub->Unfolded) = (%.3lf %%, %.3lf %%)\n", RelDiff_Observed_BkgSub_Smeared*100, RelDiff_BkgSub_Unfolded_Smeared*100 ); 

		printf( "\n" );
	}

	printf("nEvent_largerRelDiff = %d (%.3lf%%)\n", nEvent_largerRelDiff, (Double_t)nEvent_largerRelDiff/nTest);

	Double_t Average_RelDiff_Observed_LargerRelDiff = (Sum_RelDiff_Observed_LargerRelDiff/nEvent_largerRelDiff);
	printf("Average rel. diff at observed yield of the events with larger deviation in unfolding step: %.3lf\n",Average_RelDiff_Observed_LargerRelDiff*100);

	Double_t Average_RelDiff_Observed_SmallerRelDiff = (Sum_RelDiff_Observed - Sum_RelDiff_Observed_LargerRelDiff) / (nTest - nEvent_largerRelDiff );
	printf("Average rel. diff at observed yield of the events with smaller deviation in unfolding step: %.3lf\n",Average_RelDiff_Observed_SmallerRelDiff*100);
}