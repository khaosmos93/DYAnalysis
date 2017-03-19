#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooGlobalFunc.h>
#include <RooPlot.h>

#include <src/RooUnfold.cxx>
#include <src/RooUnfoldResponse.cxx>
#include <src/RooUnfoldBayes.cxx>
#include <src/RooUnfoldInvert.cxx>
#include <TStyle.h>
#include <Include/PlotTools.h>
#include <Include/DYAnalyzer.h> // -- nMassBin, Lumi, Lumi_HLTv4p2 is defined -- //

#define nMap 1000

using namespace RooFit;

class HistogramContainer
{
public:
	Double_t MassBinEdges[nMassBin+1];
	TString FileLocation;

	TString Type;

	TH1D* h_data;
	TH1D* h_BkgSub;
	TH1D* h_Unfolded;

	// TH1D* h_data_4p2;
	// TH1D* h_data_4p3;

	// TH1D* h_BkgSub_4p2;
	// TH1D* h_BkgSub_4p3;

	// TH1D* h_Unfolded_4p2;
	// TH1D* h_Unfolded_4p3;

	// TH1D* h_AccEffCorr_4p2;
	// TH1D* h_AccEffCorr_4p3;

	// TH1D* h_EffSFCorr_4p2;
	// TH1D* h_EffSFCorr_4p3;

	// TH1D* h_FSRCorr;

	// TH1D* h_DiffXSec;

	HistogramContainer()
	{
		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for(Int_t i=0; i<nMassBin+1; i++)
			this->MassBinEdges[i] = MassBinEdges_temp[i];

		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");

	}

	void Set_Type( TString _Type )
	{
		this->Type = _Type;
	}

	HistogramContainer( TH1D* _h_data ) : HistogramContainer()
	{
		this->Set_DataHistogram( _h_data );
	}

	void Set_DataHistogram( TH1D* _h_data )
	{
		this->h_data = (TH1D*)_h_data->Clone();
	}

	TH1D* Get_Histogram( TString Type )
	{
		TH1D* h_return = NULL;
		if( Type == "Observed" )
		{
			h_return = (TH1D*)this->h_data->Clone();
		}
		else if( Type == "BkgSub" )
		{
			h_return = (TH1D*)this->h_BkgSub->Clone();
		}
		else if( Type == "Unfolded" )
		{
			h_return = (TH1D*)this->h_Unfolded->Clone();
		}
		// else if( Type == "AccEffCorr" )
		// {
		// 	h_return = (TH1D*)this->h_AccEffCorr_4p2->Clone();
		// 	h_return->Add( this->h_AccEffCorr_4p3 );
		// }
		// else if( Type == "EffSFCorr" )
		// {
		// 	h_return = (TH1D*)this->h_EffSFCorr_4p2->Clone();
		// 	h_return->Add( this->h_EffSFCorr_4p3 );
		// }
		// else if( Type == "FSRCorr" )
		// {
		// 	h_return = (TH1D*)h_FSRCorr->Clone();
		// }
		// else if( Type == "DiffXSec" )
		// {
		// 	h_return = (TH1D*)h_DiffXSec->Clone();
		// }
		return h_return;
	}

	void Make_Histogram_All()
	{
		this->Make_Histogram_BkgSub();
		this->Make_Histogram_Unfolded();
		// this->Make_Histogram_AccEffCorr();
		// this->Make_Histogram_EffSFCorr();
		// this->Make_Histogram_FSRCorr();
		// this->Make_Histogram_DiffXSec();
	}

	void Save_Histogram( TFile *f_output )
	{
		this->Set_HistogramName();

		f_output->cd();
		this->h_data->Write("h_data_"+this->Type);
		this->h_BkgSub->Write("h_BkgSub_"+this->Type);
		this->h_Unfolded->Write("h_Unfolded_"+this->Type);

		// this->h_data_4p2->Write();
		// this->h_data_4p3->Write();

		// this->h_BkgSub_4p2->Write();
		// this->h_BkgSub_4p3->Write();

		// this->h_Unfolded_4p2->Write();
		// this->h_Unfolded_4p3->Write();

		// this->h_AccEffCorr_4p2->Write();
		// this->h_AccEffCorr_4p3->Write();

		// this->h_EffSFCorr_4p2->Write();
		// this->h_EffSFCorr_4p3->Write();

		// this->h_FSRCorr->Write();

		// this->h_DiffXSec->Write();
	}

	void Load_Histogram( TFile *f_input, TString _Type )
	{
		TString HistName = "";

		HistName = "h_data_"+_Type;
		this->h_data = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_BkgSub_"+_Type;
		this->h_BkgSub = (TH1D*)f_input->Get(HistName)->Clone();

		HistName = "h_Unfolded_"+_Type;
		this->h_Unfolded = (TH1D*)f_input->Get(HistName)->Clone();


		// HistName = "h_data_4p2_"+_Type;
		// this->h_data_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_data_4p3_"+_Type;
		// this->h_data_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_BkgSub_4p2_"+_Type;
		// this->h_BkgSub_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_BkgSub_4p3_"+_Type;
		// this->h_BkgSub_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_Unfolded_4p2_"+_Type;
		// this->h_Unfolded_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_Unfolded_4p3_"+_Type;
		// this->h_Unfolded_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_AccEffCorr_4p2_"+_Type;
		// this->h_AccEffCorr_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_AccEffCorr_4p3_"+_Type;
		// this->h_AccEffCorr_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_EffSFCorr_4p2_"+_Type;
		// this->h_EffSFCorr_4p2 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_EffSFCorr_4p3_"+_Type;
		// this->h_EffSFCorr_4p3 = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_FSRCorr_"+_Type;
		// this->h_FSRCorr = (TH1D*)f_input->Get(HistName)->Clone();

		// HistName = "h_DiffXSec_"+_Type;
		// this->h_DiffXSec = (TH1D*)f_input->Get(HistName)->Clone();
	}

protected:
	void Set_HistogramName()
	{
		TString HistName = "";

		HistName = "h_data_"+this->Type;
		this->h_data->SetName(HistName);

		HistName = "h_BkgSub_"+this->Type;
		this->h_BkgSub->SetName(HistName);

		HistName = "h_Unfolded_"+this->Type;
		this->h_Unfolded->SetName(HistName);

		// HistName = "h_AccEffCorr_4p2_"+this->Type;
		// this->h_AccEffCorr_4p2->SetName(HistName);

		// HistName = "h_AccEffCorr_4p3_"+this->Type;
		// this->h_AccEffCorr_4p3->SetName(HistName);

		// HistName = "h_EffSFCorr_4p2_"+this->Type;
		// this->h_EffSFCorr_4p2->SetName(HistName);

		// HistName = "h_EffSFCorr_4p3_"+this->Type;
		// this->h_EffSFCorr_4p3->SetName(HistName);

		// HistName = "h_FSRCorr_"+this->Type;
		// this->h_FSRCorr->SetName(HistName);

		// HistName = "h_DiffXSec_"+this->Type;
		// this->h_DiffXSec->SetName(HistName);
	}
	void Make_Histogram_BkgSub()
	{
		// -- MC bkg. after applying efficiency SF -- //
		TH1D* h_WZ = this->Get_Histogram_MCBkg("WZ");
		TH1D* h_ZZ = this->Get_Histogram_MCBkg("ZZ");

		// --- data-driven backgrounds -- //
		TFile* f_input_bkg_dataDriven = TFile::Open(this->FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
		TH1D *h_diJet_FR = (TH1D*)f_input_bkg_dataDriven->Get("dijet")->Clone();
		TH1D *h_WJets_FR = (TH1D*)f_input_bkg_dataDriven->Get("wjets")->Clone();
		TH1D *h_ttbar_emu = (TH1D*)f_input_bkg_dataDriven->Get("ttbar")->Clone();
		TH1D *h_tW_emu = (TH1D*)f_input_bkg_dataDriven->Get("tW")->Clone();
		TH1D *h_DYTauTau_emu = (TH1D*)f_input_bkg_dataDriven->Get("DYtautau")->Clone();
		TH1D *h_WW_emu = (TH1D*)f_input_bkg_dataDriven->Get("WW")->Clone();

		f_input_bkg_dataDriven->Close();

		TH1D* h_bkg_noMCBkg = (TH1D*)h_diJet_FR->Clone();
		h_bkg_noMCBkg->Add( h_WJets_FR );
		h_bkg_noMCBkg->Add( h_ttbar_emu );
		h_bkg_noMCBkg->Add( h_tW_emu );
		h_bkg_noMCBkg->Add( h_DYTauTau_emu );
		h_bkg_noMCBkg->Add( h_WW_emu );

		TH1D* h_bkg = (TH1D*)h_bkg_noMCBkg->Clone();
		h_bkg->Add( h_WZ );
		h_bkg->Add( h_ZZ );

		this->h_BkgSub = (TH1D*)this->h_data->Clone(); h_BkgSub->Sumw2();
		this->h_BkgSub->Add( h_bkg, -1 );

		// -- Remove negative bins -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t nEvent = this->h_BkgSub->GetBinContent(i_bin);
			if( nEvent < 0 )
			{
				this->h_BkgSub->SetBinContent(i_bin, 0);
				this->h_BkgSub->SetBinError(i_bin, 0);
			}
		}
	}

	void Make_Histogram_Unfolded()
	{
		TString FileName = this->FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root";
		TFile *f_Unfold = TFile::Open( FileName );

		// -- HLTv4.2 -- //
		RooUnfoldResponse *UnfoldRes = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		RooUnfoldBayes *UnfoldBayes = new RooUnfoldBayes(UnfoldRes, this->h_BkgSub, 17); // -- iteration until convergence -- //
		this->h_Unfolded = (TH1D*)UnfoldBayes->Hreco();

		f_Unfold->Close();
	}

	// void Make_Histogram_AccEffCorr()
	// {
	// 	TFile *f_AccEff = TFile::Open(this->FileLocation + "/ROOTFile_AccEff.root"); f_AccEff->cd();
	// 	TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)f_AccEff->Get("g_AccEff");
	// 	// f_AccEff->Close();

	// 	// -- HLTv4p2 -- //
	// 	this->h_AccEffCorr_4p2 = (TH1D*)this->h_Unfolded_4p2->Clone();
	// 	this->Correction_AccEff( this->h_AccEffCorr_4p2, this->h_Unfolded_4p2, g_AccEff );

	// 	// -- HLTv4p3 -- //
	// 	this->h_AccEffCorr_4p3 = (TH1D*)this->h_Unfolded_4p3->Clone();
	// 	this->Correction_AccEff( this->h_AccEffCorr_4p3, this->h_Unfolded_4p3, g_AccEff );

	// 	f_AccEff->Close();
	// }

	// void Make_Histogram_EffSFCorr()
	// {
	// 	///////////////////////////////////////////////////////
	// 	// -- Calculation of efficiency correction factor -- //
	// 	///////////////////////////////////////////////////////
	// 	TFile *f_AccEff = TFile::Open(this->FileLocation + "/ROOTFile_AccEff.root"); f_AccEff->cd();

	// 	TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff")->Clone();
	// 	TGraphAsymmErrors *g_Eff_Corr_HLTv4p2 = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff_Corr_HLTv4p2")->Clone();
	// 	TGraphAsymmErrors *g_Eff_Corr_HLTv4p3 = (TGraphAsymmErrors*)f_AccEff->Get("g_Eff_Corr_HLTv4p3")->Clone();
		
	// 	f_AccEff->Close();

	// 	// -- Calculate efficiency scale factor for each mass bin: SF = Corrected Eff / Un-corrected Eff -- //
	// 	GraphInfo *Graph_CorrEff_4p2 = new GraphInfo( kBlack, "temp" );
	// 	Graph_CorrEff_4p2->Set_Graph( g_Eff_Corr_HLTv4p2 );
	// 	Graph_CorrEff_4p2->Calc_RatioGraph_Denominator( g_Eff );
	// 	TGraphAsymmErrors* g_EffCorr_4p2 = (TGraphAsymmErrors*)Graph_CorrEff_4p2->g_ratio->Clone();

	// 	GraphInfo *Graph_CorrEff_4p3 = new GraphInfo( kBlack, "temp" );
	// 	Graph_CorrEff_4p3->Set_Graph( g_Eff_Corr_HLTv4p3 );
	// 	Graph_CorrEff_4p3->Calc_RatioGraph_Denominator( g_Eff );
	// 	TGraphAsymmErrors* g_EffCorr_4p3 = (TGraphAsymmErrors*)Graph_CorrEff_4p3->g_ratio->Clone();

	// 	this->h_EffSFCorr_4p2 = (TH1D*)this->h_AccEffCorr_4p2->Clone();
	// 	this->ApplyEffCorr_Yield( this->h_EffSFCorr_4p2, this->h_AccEffCorr_4p2, g_EffCorr_4p2 );

	// 	this->h_EffSFCorr_4p3 = (TH1D*)this->h_AccEffCorr_4p3->Clone();
	// 	this->ApplyEffCorr_Yield( this->h_EffSFCorr_4p3, this->h_AccEffCorr_4p3, g_EffCorr_4p3 );
	// }

	// void Make_Histogram_FSRCorr()
	// {
	// 	//////////////////////////
	// 	// -- FSR Correction -- //
	// 	//////////////////////////
	// 	TFile *f_FSR = TFile::Open(this->FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
	// 	RooUnfoldResponse *UnfoldRes_FSR = (RooUnfoldResponse*)f_FSR->Get("UnfoldRes")->Clone();
	// 	f_FSR->Close();

	// 	TH1D* h_EffSFCorr = (TH1D*)this->h_EffSFCorr_4p2->Clone();
	// 	h_EffSFCorr->Add( this->h_EffSFCorr_4p3 );

	// 	RooUnfoldInvert *UnfoldInvert_FSR = new RooUnfoldInvert(UnfoldRes_FSR, h_EffSFCorr); // -- use inverting method after following recommendation by stat.committee -- //
	// 	this->h_FSRCorr = (TH1D*)UnfoldInvert_FSR->Hreco();
	// }

	// void Make_Histogram_DiffXSec()
	// {
	// 	TH1D* h_XSec = (TH1D*)this->h_FSRCorr->Clone();
	// 	h_XSec->Scale( 1.0 / Lumi );
	// 	this->h_DiffXSec = this->Convert_XSecToDiffXSec( h_XSec );
	// }

	TH1D* Convert_XSecToDiffXSec( TH1D* h_XSec )
	{
		TH1D* h_DiffXSec = (TH1D*)h_XSec->Clone();

		Int_t nBins = h_XSec->GetNbinsX();
		for(Int_t i=0; i<nBins; i++)
		{
			Int_t i_bin = i+1;

			Double_t BinWidth = h_XSec->GetBinWidth(i_bin);

			Double_t xSec = h_XSec->GetBinContent(i_bin);
			Double_t DiffXSec = xSec / BinWidth;

			Double_t error_before = h_XSec->GetBinError(i_bin);
			Double_t error_after = error_before / BinWidth;

			h_DiffXSec->SetBinContent(i_bin, DiffXSec);
			h_DiffXSec->SetBinError(i_bin, error_after);
		}

		return h_DiffXSec;
	}

	void ApplyEffCorr_Yield(TH1D *h_yield_EffCorr, TH1D *h_yield, TGraphAsymmErrors* g_EffCorr)
	{
		Int_t NPoints = g_EffCorr->GetN();
		Int_t nBins = h_yield->GetNbinsX();

		if( NPoints != nBins )
		{
			cout << "# Points in EffCorr != # Bins of yield histogram!" << endl;
			return;
		}

		for(Int_t i_p=0; i_p<NPoints; i_p++)
		{
			// -- Get g_EffCorr point -- //
			Double_t mass, EffCorr;
			g_EffCorr->GetPoint(i_p, mass, EffCorr);

			Int_t i_bin = i_p + 1;
			Double_t yield = h_yield->GetBinContent(i_bin);
			Double_t error_yield = h_yield->GetBinError(i_bin);

			Double_t yield_EffCorr = yield / EffCorr;
			Double_t error_yield_EffCorr = error_yield / EffCorr;

			h_yield_EffCorr->SetBinContent(i_bin, yield_EffCorr);
			h_yield_EffCorr->SetBinError(i_bin, error_yield_EffCorr);
		}
	}

	void Correction_AccEff(TH1D *h_yield_AccEff, TH1D *h_yield, TGraphAsymmErrors *g_AccEff)
	{
		Int_t nBins = h_yield->GetNbinsX();
		for(Int_t i=0; i<nBins; i++)
		{
			Int_t i_bin = i+1;

			Double_t x_AccEff, y_AccEff;
			g_AccEff->GetPoint(i, x_AccEff, y_AccEff);

			Double_t Yield = h_yield->GetBinContent(i_bin);
			Double_t Yield_AfterAccEff = Yield / y_AccEff;

			Double_t Error = h_yield->GetBinError(i_bin);
			Double_t Error_AfterAccEff = Error / y_AccEff;

			// -- Set the central value -- //
			h_yield_AccEff->SetBinContent( i_bin, Yield_AfterAccEff );
			h_yield_AccEff->SetBinError( i_bin, Error_AfterAccEff );
		}
	}

	TH1D* Get_Histogram_MCBkg( TString Type )
	{
		TString FileName = this->FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";
		TFile *f_MCBkg = TFile::Open( FileName ); f_MCBkg->cd();

		TString HistName = "h_mass_OS_Norm_"+Type;

		TH1D* h_mass = (TH1D*)f_MCBkg->Get( HistName )->Clone();
		h_mass = (TH1D*)h_mass->Rebin(nMassBin, h_mass->GetName(), MassBinEdges);

		f_MCBkg->Close();

		return h_mass;
	}
};

class Tool_StatUnc_StepByStep
{
public:
	Double_t MassBinEdges[nMassBin+1];
	TString BasePath;

	HistogramContainer *Hists_CV;
	HistogramContainer *Hists_Smeared[nMap];

	TH1D* h_1OverSqrtN;

	TH1D* h_RelStatUnc_Observed;
	TH1D* h_RelStatUnc_BkgSub;
	TH1D* h_RelStatUnc_Unfolded;
	TH1D* h_RelStatUnc_AccEffCorr;
	TH1D* h_RelStatUnc_EffSFCorr;
	TH1D* h_RelStatUnc_FSRCorr;
	TH1D* h_RelStatUnc_DiffXSec;

	Tool_StatUnc_StepByStep()
	{
		TH1::AddDirectory(kFALSE);
		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for(Int_t i=0; i<nMassBin+1; i++)
			this->MassBinEdges[i] = MassBinEdges_temp[i];

		this->BasePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString FileName =  this->BasePath + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root";

		TH1D* h_data = Get_Hist( FileName, "h_mass_OS_Data" ); h_data->Sumw2();
		h_data = this->Rebin_DYBin( h_data );

		this->Hists_CV = new HistogramContainer( h_data );
		this->Smearing();

		this->Make_Histogram_SqrtN();
	}

	void MakeAndSave_Histogram( TFile *f_output )
	{
		f_output->cd();
		Hists_CV->Set_Type( "CV" );
		Hists_CV->Make_Histogram_All();
		Hists_CV->Save_Histogram( f_output );

		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			TString Type = TString::Format("Smeared_%d", i_map);
			Hists_Smeared[i_map]->Set_Type( Type );
			Hists_Smeared[i_map]->Make_Histogram_All();
			Hists_Smeared[i_map]->Save_Histogram( f_output );
		}
	}

	void Save_RelStatUnc( TFile *f_output )
	{
		f_output->cd();
		this->h_1OverSqrtN->Write("h_1OverSqrtN");

		this->h_RelStatUnc_Observed->Write("h_RelStatUnc_Observed");
		this->h_RelStatUnc_BkgSub->Write("h_RelStatUnc_BkgSub");
		this->h_RelStatUnc_Unfolded->Write("h_RelStatUnc_Unfolded");
		// this->h_RelStatUnc_AccEffCorr->Write("h_RelStatUnc_AccEffCorr");
		// this->h_RelStatUnc_EffSFCorr->Write("h_RelStatUnc_EffSFCorr");
		// this->h_RelStatUnc_FSRCorr->Write("h_RelStatUnc_FSRCorr");
		// this->h_RelStatUnc_DiffXSec->Write("h_RelStatUnc_DiffXSec");

		f_output->Close();
	}

	void Load_Histogram_StepByStep( TFile *f_input )
	{
		this->Hists_CV->Load_Histogram( f_input, "CV" );
		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			TString Type = TString::Format("Smeared_%d", i_map);
			this->Hists_Smeared[i_map]->Load_Histogram( f_input, Type );
		}
	}

	void Make_Histogram_StepByStep()
	{
		this->Hists_CV->Make_Histogram_All();
		for(Int_t i_map=0; i_map<nMap; i_map++)
			this->Hists_Smeared[i_map]->Make_Histogram_All();
	}

	void Calc_RelStatUnc_StepByStep()
	{
		// -- just validation: statistical uncertainty of observed yield -- //
		this->h_RelStatUnc_Observed = this->Init_DYBin( "h_RelStatUnc_Observed" );
		this->Calc_RelStatUnc( "Observed", this->h_RelStatUnc_Observed );

		// -- statistical uncertainty after background subtraction -- //
		this->h_RelStatUnc_BkgSub = this->Init_DYBin( "h_RelStatUnc_BkgSub" );
		this->Calc_RelStatUnc( "BkgSub", this->h_RelStatUnc_BkgSub );

		// -- statistical uncertainty after unfolding correction -- //
		this->h_RelStatUnc_Unfolded = this->Init_DYBin( "h_RelStatUnc_Unfolded" );
		this->Calc_RelStatUnc( "Unfolded", this->h_RelStatUnc_Unfolded );

		// // -- statistical uncertainty after acc*eff correction -- //
		// this->h_RelStatUnc_AccEffCorr = this->Init_DYBin( "h_RelStatUnc_AccEffCorr" );
		// this->Calc_RelStatUnc( "AccEffCorr", this->h_RelStatUnc_AccEffCorr );

		// // -- statistical uncertainty after efficiency SF correction -- //
		// this->h_RelStatUnc_EffSFCorr = this->Init_DYBin( "h_RelStatUnc_EffSFCorr" );
		// this->Calc_RelStatUnc( "EffSFCorr", this->h_RelStatUnc_EffSFCorr );

		// // -- statistical uncertainty after FSR correction -- //
		// this->h_RelStatUnc_FSRCorr = this->Init_DYBin( "h_RelStatUnc_FSRCorr" );
		// this->Calc_RelStatUnc( "FSRCorr", this->h_RelStatUnc_FSRCorr );

		// // -- statistical uncertainty of differential cross section -- //
		// this->h_RelStatUnc_DiffXSec = this->Init_DYBin( "h_RelStatUnc_DiffXSec" );
		// this->Calc_RelStatUnc( "DiffXSec", this->h_RelStatUnc_DiffXSec );

		this->ComparisonPlot_RelStatUnc();
	}

	void Calc_RelStatUnc( TString Type, TH1D* h_RelStatUnc)
	{
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			TTree *tree = this->makeTTree(Type, i_bin);

			Double_t BinCenter = ( MassBinEdges[i] + MassBinEdges[i+1] ) / 2.0;
			Double_t RangeMax = 0.1;
			Double_t Sigma_Init = 0.01;
			this->Set_FitInitValues( BinCenter, RangeMax, Sigma_Init );

			RooRealVar RelDiff("RelDiff","(N_{Smeared} - N_{CV}) / N_{CV}", (-1)*RangeMax, RangeMax);
			RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

			// --- Make plot of binned dataset showing Poisson error bars (RooFit default)
			RooPlot* frame = RelDiff.frame( Title(TString::Format("%.0lf < M < %.0lf (%02d bin)", MassBinEdges[i], MassBinEdges[i+1], i_bin)) );
			
			// -- Fit a Gaussian p.d.f to the data
			RooRealVar mean("mean", "mean", 0, -2, 2) ;
			RooRealVar sigma("sigma", "sigma", Sigma_Init, 0.0001, 2);
			RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
			gauss.fitTo(data);

			data.plotOn(frame, Binning(50));
			gauss.plotOn(frame);
			gauss.paramOn(frame,Layout(0.6, 0.9, 0.9));
			frame->getAttText()->SetTextSize(0.02);

			TString CanvasName = TString::Format("c_RelDiff_%s_Bin%02d", Type.Data(), i_bin);

			TCanvas *c = new TCanvas(CanvasName, "", 700, 700); c->cd();
			frame->Draw();

			c->SaveAs(CanvasName+".pdf");
			
			Double_t RMS = sigma.getVal();
			Double_t RMSErr = sigma.getError();

			h_RelStatUnc->SetBinContent(i_bin, RMS);
			h_RelStatUnc->SetBinError(i_bin, RMSErr);

			// Double_t Mean = mean.getVal();
			// h_mean->SetBinContent(i_bin, Mean);
			// h_mean->SetBinError(i_bin, 0);
			
			// this->Print_FitCanvas( CanvasName, h_RelDiff );
		}
	}

	void Make_XCheckPlot()
	{
		this->Make_XCheckPlot_BkgSub();
		this->Make_XCheckPlot_Unfolded();
		// this->Make_XCheckPlot_AccEffCorr();
		// this->Make_XCheckPlot_EffSFCorr();
		// this->Make_XCheckPlot_FSRCorr();
		// this->Make_XCheckPlot_DiffXSec();
	}

	void Set_FitInitValues( Double_t BinCenter, Double_t &RangeMax, Double_t &Sigma_Init )
	{
		if( BinCenter > 1500 )
		{
			RangeMax = 4;
			Sigma_Init = 2;
		}
		else if( BinCenter > 600 )
		{
			RangeMax = 4;
			Sigma_Init = 0.5;
		}
		else if( BinCenter > 380 )
		{
			RangeMax = 0.5;
			Sigma_Init = 0.02;
		}
		else if( BinCenter > 141 )
		{
			RangeMax = 0.3;
			Sigma_Init = 0.01;
		}
		else
		{
			RangeMax = 0.1;
			Sigma_Init = 0.01;
		}

	}

	TTree* makeTTree(TString Type, Int_t i_bin) 
	{
		TTree* tree = new TTree("tree","tree");

		Double_t* RelDiff = new Double_t;

		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

		TH1D *h_CV = this->Hists_CV->Get_Histogram( Type );
		Double_t value_CV = h_CV->GetBinContent(i_bin);

		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			TH1D *h_Smeared = this->Hists_Smeared[i_map]->Get_Histogram( Type );

			Double_t value_Smeared = h_Smeared->GetBinContent(i_bin);

			*RelDiff = ( value_Smeared - value_CV ) / value_CV;

			// printf("[\t%d th RelDiff = %lf]\n", i, *RelDiff);

			tree->Fill();
		}

		return tree;
	}

	void ComparisonPlot_RelStatUnc()
	{
		HistInfo *Hist_sqrtN = new HistInfo( kBlack, "1 / #sqrt{N}");
		Hist_sqrtN->Set_Histogram( this->h_1OverSqrtN );
		Hist_sqrtN->Set();
		Hist_sqrtN->h->Scale( 100 );

		HistInfo *Hist_Observed = new HistInfo( kGreen+2, "Observed yield" );
		Hist_Observed->Set_Histogram( this->h_RelStatUnc_Observed );
		Hist_Observed->Set();
		Hist_Observed->h->Scale( 100 );
		Hist_Observed->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		HistInfo *Hist_BkgSub = new HistInfo( kBlue+2, "Yield after Bkg.Sub." );
		Hist_BkgSub->Set_Histogram( this->h_RelStatUnc_BkgSub );
		Hist_BkgSub->Set();
		Hist_BkgSub->h->Scale( 100 );
		Hist_BkgSub->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		HistInfo *Hist_Unfolded = new HistInfo( kViolet, "Yield after unfolding" );
		Hist_Unfolded->Set_Histogram( this->h_RelStatUnc_Unfolded );
		Hist_Unfolded->Set();
		Hist_Unfolded->h->Scale( 100 );
		Hist_Unfolded->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		// HistInfo *Hist_AccEffCorr = new HistInfo( kCyan, "Yield after Acc*Eff Corr." );
		// Hist_AccEffCorr->Set_Histogram( this->h_RelStatUnc_AccEffCorr );
		// Hist_AccEffCorr->Set();
		// Hist_AccEffCorr->h->Scale( 100 );
		// Hist_AccEffCorr->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		// HistInfo *Hist_EffSFCorr = new HistInfo( kOrange+2, "Yield after Eff.SF. Corr." );
		// Hist_EffSFCorr->Set_Histogram( this->h_RelStatUnc_EffSFCorr );
		// Hist_EffSFCorr->Set();
		// Hist_EffSFCorr->h->Scale( 100 );
		// Hist_EffSFCorr->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		// HistInfo *Hist_FSRCorr = new HistInfo( kBlue-9, "Yield after FSR Corr." );
		// Hist_FSRCorr->Set_Histogram( this->h_RelStatUnc_FSRCorr );
		// Hist_FSRCorr->Set();
		// Hist_FSRCorr->h->Scale( 100 );
		// Hist_FSRCorr->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		// HistInfo *Hist_DiffXSec = new HistInfo( kRed, "d#sigma/dm" );
		// Hist_DiffXSec->Set_Histogram( this->h_RelStatUnc_DiffXSec );
		// Hist_DiffXSec->Set();
		// Hist_DiffXSec->h->Scale( 100 );
		// Hist_DiffXSec->Calc_RatioHist_Denominator( Hist_sqrtN->h );

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = "c_Comparison_RelStatUnc";
		SetCanvas_Ratio(c, CanvasName, TopPad, BottomPad, 1, 1);

		c->cd();
		TopPad->cd();

		Hist_sqrtN->h->Draw("HISTLPSAME");
		Hist_Observed->h->Draw("HISTLPSAME");
		Hist_BkgSub->h->Draw("HISTLPSAME");
		Hist_Unfolded->h->Draw("HISTLPSAME");
		// Hist_AccEffCorr->h->Draw("HISTLPSAME");
		// Hist_EffSFCorr->h->Draw("HISTLPSAME");
		// Hist_FSRCorr->h->Draw("HISTLPSAME");
		// Hist_DiffXSec->h->Draw("HISTLPSAME");

		SetHistFormat_TopPad( Hist_sqrtN->h, "Rel. Uncertainty (%)");
		Hist_sqrtN->h->GetYaxis()->SetRangeUser( 5e-2, 300 );

		TLegend *legend;
		SetLegend( legend, 0.15, 0.70, 0.50, 0.93 );
		legend->AddEntry( Hist_sqrtN->h, Hist_sqrtN->LegendName );
		legend->AddEntry( Hist_Observed->h, Hist_Observed->LegendName );
		legend->AddEntry( Hist_BkgSub->h, Hist_BkgSub->LegendName );
		legend->AddEntry( Hist_Unfolded->h, Hist_Unfolded->LegendName );
		// legend->AddEntry( Hist_AccEffCorr->h, Hist_AccEffCorr->LegendName );
		// legend->AddEntry( Hist_EffSFCorr->h, Hist_EffSFCorr->LegendName );
		// legend->AddEntry( Hist_FSRCorr->h, Hist_FSRCorr->LegendName );
		// legend->AddEntry( Hist_DiffXSec->h, Hist_DiffXSec->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );

		c->cd();
		BottomPad->cd();

		Hist_Observed->h_ratio->Draw("HISTLPSAME");
		Hist_BkgSub->h_ratio->Draw("HISTLPSAME");
		Hist_Unfolded->h_ratio->Draw("HISTLPSAME");
		// Hist_AccEffCorr->h_ratio->Draw("HISTLPSAME");
		// Hist_EffSFCorr->h_ratio->Draw("HISTLPSAME");
		// Hist_FSRCorr->h_ratio->Draw("HISTLPSAME");
		// Hist_DiffXSec->h_ratio->Draw("HISTLPSAME");
		SetHistFormat_BottomPad( Hist_Observed->h_ratio, "m [GeV]", "Ratio to 1/#sqrt{N}", 0.9, 3.0 );

		c->SaveAs(".pdf");
	}

protected:
	void Make_Histogram_SqrtN()
	{
		TH1D* h_data = (TH1D*)this->Hists_CV->h_data->Clone();

		this->h_1OverSqrtN = (TH1D*)h_data->Clone("h_1OverSqrtN");
		Int_t nBin = h_1OverSqrtN->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t nEvent = h_data->GetBinContent(i_bin);
			Double_t sqrtN = sqrt( nEvent );

			this->h_1OverSqrtN->SetBinContent( i_bin, 1.0 / sqrtN );
			this->h_1OverSqrtN->SetBinError(i_bin, 0);
		}
	}

	void Smearing()
	{
		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			TH1D* h_data_smeared = (TH1D*)this->Hists_CV->h_data->Clone();

			Int_t nBin = h_data_smeared->GetNbinsX();
			for(Int_t i=0; i<nBin; i++) // -- make smeared histogram for i_map-th map -- //
			{
				Int_t i_bin = i+1;
				Double_t CV = this->Hists_CV->h_data->GetBinContent(i_bin);

				Double_t Smeared = (Double_t)gRandom->Poisson( CV );

				h_data_smeared->SetBinContent(i_bin, Smeared );
				h_data_smeared->SetBinError(i_bin, sqrt(Smeared) );
			}

			this->Hists_Smeared[i_map] = new HistogramContainer( h_data_smeared );
		}
	}

	TH1D* Rebin_DYBin( TH1D* h_before )
	{
		return (TH1D*)h_before->Rebin(nMassBin, h_before->GetName(), MassBinEdges);
	}

	TH1D* Init_DYBin(TString HistName)
	{
		TH1D* h_temp = new TH1D(HistName, "", nMassBin, MassBinEdges);
		return h_temp;
	}

	// void Print_FitCanvas( TString CanvasName, TH1D* h )
	// {
	// 	TCanvas *c;
	// 	SetCanvas_Square( c, CanvasName );
	// 	gStyle->SetOptFit(111);

	// 	c->cd();
	// 	h->Draw("HISTEP");

	// 	c->SaveAs(".pdf");
	// }

	void Make_XCheckPlot_BkgSub()
	{
		TH1D* h_Ref = Get_Hist( this->BasePath+"/ROOTFile_YieldHistogram.root", "h_yield_OS_DataDrivenBkg" );

		TString CanvasName = "c_this_vs_ref_BkgSub";
		this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_BkgSub, h_Ref);
	}

	void Make_XCheckPlot_Unfolded()
	{
		TH1D* h_Ref = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_yield_Unfolded" );

		TString CanvasName = "c_this_vs_ref_Unfolded";
		this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_Unfolded, h_Ref);
	}

	// void Make_XCheckPlot_AccEffCorr()
	// {
	// 	TH1D* h_Ref_4p2 = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_yield_HLTv4p2_Unfolded_AccEff" );
	// 	TH1D* h_Ref_4p3 = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_yield_HLTv4p3_Unfolded_AccEff" );

	// 	TString CanvasName = "c_this_vs_ref_AccEffCorr_4p2";
	// 	this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_AccEffCorr_4p2, h_Ref_4p2);

	// 	CanvasName = "c_this_vs_ref_AccEffCorr_4p3";
	// 	this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_AccEffCorr_4p3, h_Ref_4p3);
	// }

	// void Make_XCheckPlot_EffSFCorr()
	// {
	// 	TH1D* h_Ref_4p2 = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_yield_HLTv4p2_EffCorr" );
	// 	TH1D* h_Ref_4p3 = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_yield_HLTv4p3_EffCorr" );

	// 	TString CanvasName = "c_this_vs_ref_EffSFCorr_4p2";
	// 	this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_EffSFCorr_4p2, h_Ref_4p2);

	// 	CanvasName = "c_this_vs_ref_EffSFCorr_4p3";
	// 	this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_EffSFCorr_4p3, h_Ref_4p3);
	// }

	// void Make_XCheckPlot_FSRCorr()
	// {
	// 	TH1D* h_Ref = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_yield_FSRCorr" );

	// 	TString CanvasName = "c_this_vs_ref_FSRCorr";
	// 	this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_FSRCorr, h_Ref);
	// }

	// void Make_XCheckPlot_DiffXSec()
	// {
	// 	TH1D* h_Ref = Get_Hist( this->BasePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_DiffXsec_FSRCorr" );

	// 	TString CanvasName = "c_this_vs_ref_DiffXSec";
	// 	this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_DiffXSec, h_Ref);
	// }

	void Comparison_this_vs_ref( TString CanvasName, TH1D* h_this, TH1D* h_ref )
	{
		HistInfo *Hist_this = new HistInfo( kRed, "This" );
		Hist_this->Set_Histogram( h_this );
		Hist_this->Set();

		HistInfo *Hist_ref = new HistInfo( kBlack, "Reference");
		Hist_ref->Set_Histogram( h_ref );
		Hist_ref->Set();

		Hist_this->Calc_RatioHist_Denominator( Hist_ref->h );

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, 1, 1 );
		c->cd();
		TopPad->cd();

		Hist_ref->h->Draw("EPSAME");
		Hist_this->h->Draw("EPSAME");
		SetHistFormat_TopPad( Hist_ref->h, "Entries per bin" );

		TLegend *legend;
		SetLegend( legend );
		legend->AddEntry( Hist_this->h, Hist_this->LegendName );
		legend->AddEntry( Hist_ref->h, Hist_ref->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );

		c->cd();
		BottomPad->cd();

		Hist_this->h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( Hist_this->h_ratio, "m [GeV]", "this/ref", 0.99, 1.01 );

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}
};

// class TestTool_StatUnc_StepByStep: public Tool_StatUnc_StepByStep
// {
// public:

// 	TH1D* h_RelStatUnc_DEN_BkgSub_NUM_Unfolded;
// 	TH1D* h_RelStatUnc_DEN_Unfolded_NUM_BkgSub;

// 	TestTool_StatUnc_StepByStep(): Tool_StatUnc_StepByStep()
// 	{

// 	}

// 	void Load_Histograms_RelStatUnc( TFile *f_input )
// 	{
// 		f_input->cd();

// 		this->h_1OverSqrtN = (TH1D*)f_input->Get("h_1OverSqrtN")->Clone();

// 		this->h_RelStatUnc_Observed = (TH1D*)f_input->Get("h_RelStatUnc_Observed")->Clone();
// 		this->h_RelStatUnc_BkgSub = (TH1D*)f_input->Get("h_RelStatUnc_BkgSub")->Clone();
// 		this->h_RelStatUnc_Unfolded = (TH1D*)f_input->Get("h_RelStatUnc_Unfolded")->Clone();
// 		this->h_RelStatUnc_AccEffCorr = (TH1D*)f_input->Get("h_RelStatUnc_AccEffCorr")->Clone();
// 		this->h_RelStatUnc_EffSFCorr = (TH1D*)f_input->Get("h_RelStatUnc_EffSFCorr")->Clone();
// 		this->h_RelStatUnc_FSRCorr = (TH1D*)f_input->Get("h_RelStatUnc_FSRCorr")->Clone();
// 		this->h_RelStatUnc_DiffXSec = (TH1D*)f_input->Get("h_RelStatUnc_DiffXSec")->Clone();

// 	}

// 	void Test_SaveResults( TFile *f_output )
// 	{
// 		this->h_RelStatUnc_DEN_BkgSub_NUM_Unfolded = this->Test_Make_RelStatUnc_DEN_BkgSub_NUM_Unfolded();
// 		this->h_RelStatUnc_DEN_Unfolded_NUM_BkgSub = this->Test_Make_RelStatUnc_DEN_Unfolded_NUM_BkgSub();

// 		this->Test_Comparison_BkgSub_Unfold();

// 		f_output->cd();
// 		this->h_RelStatUnc_DEN_BkgSub_NUM_Unfolded->Write("h_RelStatUnc_DEN_BkgSub_NUM_Unfolded");
// 		this->h_RelStatUnc_DEN_Unfolded_NUM_BkgSub->Write("h_RelStatUnc_DEN_Unfolded_NUM_BkgSub");

// 		f_output->Close();
// 	}

// 	TH1D* Test_Make_RelStatUnc_DEN_BkgSub_NUM_Unfolded()
// 	{
// 		TString TStr_Type = "DEN_BkgSub_NUM_Unfolded";

// 		TH1D* h_RelStatUnc = new TH1D("h_RelStatUnc_"+TStr_Type, "", nMassBin, MassBinEdges);
// 		for(Int_t i=0; i<nMassBin; i++)
// 		{
// 			Int_t i_bin = i+1;

// 			TTree *tree = this->Test_MakeTree_DEN_BkgSub_NUM_Unfolded(i_bin);
// 			this->Calc_RelStatUnc_GivenTree( TStr_Type, i_bin, tree, h_RelStatUnc );
// 		}

// 		return h_RelStatUnc;
// 	}

// 	TH1D* Test_Make_RelStatUnc_DEN_Unfolded_NUM_BkgSub()
// 	{
// 		TString TStr_Type = "DEN_Unfolded_NUM_BkgSub";

// 		TH1D* h_RelStatUnc = new TH1D("h_RelStatUnc_"+TStr_Type, "", nMassBin, MassBinEdges);
// 		for(Int_t i=0; i<nMassBin; i++)
// 		{
// 			Int_t i_bin = i+1;

// 			TTree *tree = this->Test_MakeTree_DEN_Unfolded_NUM_BkgSub(i_bin);
// 			this->Calc_RelStatUnc_GivenTree( TStr_Type, i_bin, tree, h_RelStatUnc );
// 		}

// 		return h_RelStatUnc;
// 	}


// 	TTree* Test_MakeTree_DEN_BkgSub_NUM_Unfolded(Int_t i_bin) 
// 	{
// 		TTree* tree = new TTree("tree","tree");

// 		Double_t* RelDiff = new Double_t;

// 		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

// 		TH1D *h_BkgSub_CV = this->Hists_CV->Get_Histogram("BkgSub");
// 		TH1D *h_Unfolded_CV = this->Hists_CV->Get_Histogram("Unfolded");
		
// 		Double_t BkgSub_CV = h_BkgSub_CV->GetBinContent(i_bin);
// 		Double_t Unfolded_CV = h_Unfolded_CV->GetBinContent(i_bin);


// 		for(Int_t i_map=0; i_map<nMap; i_map++)
// 		{
// 			TH1D* h_BkgSub_Smeared = this->Hists_Smeared[i_map]->Get_Histogram("BkgSub");
// 			TH1D* h_Unfolded_Smeared = this->Hists_Smeared[i_map]->Get_Histogram("Unfolded");

// 			Double_t BkgSub_Smeared = h_BkgSub_Smeared->GetBinContent(i_bin);
// 			Double_t Unfolded_Smeared = h_Unfolded_Smeared->GetBinContent(i_bin);

// 			*RelDiff = ( Unfolded_Smeared - Unfolded_CV ) / BkgSub_CV;

// 			tree->Fill();
// 		}

// 		return tree;
// 	}

// 	TTree* Test_MakeTree_DEN_Unfolded_NUM_BkgSub(Int_t i_bin) 
// 	{
// 		TTree* tree = new TTree("tree","tree");

// 		Double_t* RelDiff = new Double_t;

// 		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

// 		TH1D *h_BkgSub_CV = this->Hists_CV->Get_Histogram("BkgSub");
// 		Double_t BkgSub_CV = h_BkgSub_CV->GetBinContent(i_bin);

// 		for(Int_t i_map=0; i_map<nMap; i_map++)
// 		{
// 			TH1D* h_BkgSub_Smeared = this->Hists_Smeared[i_map]->Get_Histogram("BkgSub");
// 			TH1D* h_Unfolded_Smeared = this->Hists_Smeared[i_map]->Get_Histogram("Unfolded");

// 			Double_t BkgSub_Smeared = h_BkgSub_Smeared->GetBinContent(i_bin);
// 			Double_t Unfolded_Smeared = h_Unfolded_Smeared->GetBinContent(i_bin);

// 			*RelDiff = ( BkgSub_Smeared - BkgSub_CV ) / Unfolded_Smeared;

// 			tree->Fill();
// 		}

// 		return tree;
// 	}

// 	void Calc_RelStatUnc_GivenTree( TString TypeForTitle, Int_t i_bin, TTree* tree, TH1D* h_RelStatUnc )
// 	{
// 		Int_t i = i_bin-1;

// 		Double_t BinCenter = ( MassBinEdges[i] + MassBinEdges[i+1] ) / 2.0;
// 		Double_t RangeMax = 0.1;
// 		Double_t Sigma_Init = 0.01;
// 		this->Set_FitInitValues( BinCenter, RangeMax, Sigma_Init );

// 		RooRealVar RelDiff("RelDiff","(N_{Smeared} - N_{CV}) / N_{CV}", (-1)*RangeMax, RangeMax);
// 		RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

// 		// --- Make plot of binned dataset showing Poisson error bars (RooFit default)
// 		RooPlot* frame = RelDiff.frame( Title(TString::Format("%.0lf < M < %.0lf (%02d bin)", MassBinEdges[i], MassBinEdges[i+1], i_bin)) );
		
// 		// -- Fit a Gaussian p.d.f to the data
// 		RooRealVar mean("mean", "mean", 0, -2, 2) ;
// 		RooRealVar sigma("sigma", "sigma", Sigma_Init, 0.0001, 2);
// 		RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
// 		gauss.fitTo(data);

// 		data.plotOn(frame, Binning(50));
// 		gauss.plotOn(frame);
// 		gauss.paramOn(frame,Layout(0.6, 0.9, 0.9));
// 		frame->getAttText()->SetTextSize(0.02);

// 		TString CanvasName = TString::Format("c_RelDiff_%s_Bin%02d", TypeForTitle.Data(), i_bin);

// 		TCanvas *c = new TCanvas(CanvasName, "", 700, 700); c->cd();
// 		frame->Draw();

// 		c->SaveAs(CanvasName+".pdf");
		
// 		Double_t RMS = sigma.getVal();
// 		Double_t RMSErr = sigma.getError();

// 		h_RelStatUnc->SetBinContent(i_bin, RMS);
// 		h_RelStatUnc->SetBinError(i_bin, RMSErr);
// 	}

// 	void Test_Comparison_BkgSub_Unfold()
// 	{
// 		HistInfo *Hist_sqrtN = new HistInfo( kBlack, "1 / #sqrt{N}");
// 		Hist_sqrtN->Set_Histogram( this->h_1OverSqrtN );
// 		Hist_sqrtN->Set();
// 		Hist_sqrtN->h->Scale( 100 );

// 		HistInfo *Hist_BkgSub = new HistInfo( kBlue+2, "Yield after Bkg.Sub." );
// 		Hist_BkgSub->Set_Histogram( this->h_RelStatUnc_BkgSub );
// 		Hist_BkgSub->Set();
// 		Hist_BkgSub->h->Scale( 100 );
// 		Hist_BkgSub->Calc_RatioHist_Denominator( Hist_sqrtN->h );

// 		HistInfo *Hist_Unfolded = new HistInfo( kViolet, "Yield after unfolding" );
// 		Hist_Unfolded->Set_Histogram( this->h_RelStatUnc_Unfolded );
// 		Hist_Unfolded->Set();
// 		Hist_Unfolded->h->Scale( 100 );
// 		Hist_Unfolded->Calc_RatioHist_Denominator( Hist_sqrtN->h );

// 		HistInfo *Hist_DEN_BkgSub_NUM_Unfolded = new HistInfo( kRed, "DEN: BkgSub, NUM: Unfolded" );
// 		Hist_DEN_BkgSub_NUM_Unfolded->Set_Histogram( this->h_RelStatUnc_DEN_BkgSub_NUM_Unfolded );
// 		Hist_DEN_BkgSub_NUM_Unfolded->Set();
// 		Hist_DEN_BkgSub_NUM_Unfolded->h->Scale( 100 );
// 		Hist_DEN_BkgSub_NUM_Unfolded->CalcRatio_DEN( Hist_sqrtN->h );

// 		HistInfo *Hist_DEN_Unfolded_NUM_BkgSub = new HistInfo( kGreen+2, "DEN: Unfolded, NUM:BkgSub" );
// 		Hist_DEN_Unfolded_NUM_BkgSub->Set_Histogram( this->h_RelStatUnc_DEN_Unfolded_NUM_BkgSub );
// 		Hist_DEN_Unfolded_NUM_BkgSub->Set();
// 		Hist_DEN_Unfolded_NUM_BkgSub->h->Scale( 100 );
// 		Hist_DEN_Unfolded_NUM_BkgSub->CalcRatio_DEN( Hist_sqrtN->h );

// 		// -- draw canvas -- //
// 		TCanvas *c; TPad *TopPad; TPad *BottomPad;
// 		TString CanvasName = "c_Test_Comparison_RelStatUnc";
// 		SetCanvas_Ratio(c, CanvasName, TopPad, BottomPad, 1, 1);

// 		c->cd();
// 		TopPad->cd();

// 		Hist_sqrtN->Draw("HISTLPSAME");
// 		Hist_BkgSub->Draw("HISTLPSAME");
// 		Hist_Unfolded->Draw("HISTLPSAME");
// 		Hist_DEN_BkgSub_NUM_Unfolded->Draw("HISTLPSAME");
// 		Hist_DEN_Unfolded_NUM_BkgSub->Draw("HISTLPSAME");

// 		SetHistFormat_TopPad( Hist_sqrtN->h, "Rel. Uncertainty (%)");
// 		Hist_sqrtN->h->GetYaxis()->SetRangeUser( 5e-2, 300 );

// 		TLegend *legend;
// 		SetLegend( legend, 0.15, 0.70, 0.50, 0.93 );
// 		Hist_sqrtN->AddToLegend( legend );
// 		Hist_BkgSub->AddToLegend( legend );
// 		Hist_Unfolded->AddToLegend( legend );
// 		Hist_DEN_BkgSub_NUM_Unfolded->AddToLegend( legend );
// 		Hist_DEN_Unfolded_NUM_BkgSub->AddToLegend( legend );
// 		legend->Draw();

// 		TLatex latex;
// 		Latex_Preliminary( latex, 2.8, 13 );

// 		c->cd();
// 		BottomPad->cd();

// 		Hist_BkgSub->DrawRatio("HISTLPSAME");
// 		Hist_Unfolded->DrawRatio("HISTLPSAME");
// 		Hist_DEN_BkgSub_NUM_Unfolded->DrawRatio("HISTLPSAME");
// 		Hist_DEN_Unfolded_NUM_BkgSub->DrawRatio("HISTLPSAME");
// 		SetHistFormat_BottomPad( Hist_BkgSub->h_ratio, "m [GeV]", "Ratio to 1/#sqrt{N}", 0.9, 3.0 );

// 		c->SaveAs(".pdf");
// 	}

// 	void DrawCanvas_CV_vs_Smeared( TString Type, Int_t i_smeared )
// 	{
// 		HistInfo *Hist_CV = new HistInfo( kBlack, "Central value" );
// 		Hist_CV->Set_Histogram( this->Hists_CV->Get_Histogram( Type ) );
// 		Hist_CV->Set();

// 		HistInfo *Hist_Smeared = new HistInfo( kRed, TString::Format("Smeared (%dth)", i_smeared));
// 		Hist_Smeared->Set_Histogram( this->Hists_Smeared[i_smeared]->Get_Histogram( Type ) );
// 		Hist_Smeared->Set();
// 		Hist_Smeared->CalcRatio_DEN( Hist_CV->h );

// 		// -- draw canvas -- //
// 		TCanvas *c; TPad *TopPad; TPad *BottomPad;
// 		TString CanvasName = TString::Format("c_%s_CV_vs_Smeared_%03d", Type.Data(), i_smeared);
// 		SetCanvas_Ratio(c, CanvasName, TopPad, BottomPad, 1, 1);

// 		c->cd();
// 		TopPad->cd();

// 		Hist_CV->Draw("EPSAME");
// 		Hist_Smeared->Draw("EPSAME");

// 		SetHistFormat_TopPad( Hist_CV->h, "Entries per bin");
// 		Hist_CV->h->GetYaxis()->SetRangeUser( 0.5, 1e8 );

// 		TLegend *legend;
// 		SetLegend( legend, 0.55, 0.80, 0.95, 0.95 );
// 		Hist_CV->AddToLegend( legend );
// 		Hist_Smeared->AddToLegend( legend );
// 		legend->Draw();

// 		TLatex latex;
// 		Latex_Preliminary( latex, 2.8, 13 );
// 		latex.DrawLatexNDC( 0.15, 0.91, TString::Format("#font[42]{#scale[0.8]{%s}}", Type.Data()) );

// 		c->cd();
// 		BottomPad->cd();

// 		Hist_Smeared->DrawRatio("EPSAME");
// 		SetHistFormat_BottomPad( Hist_Smeared->h_ratio, "m [GeV]", "Smeared/CV", 0.4, 1.6 );

// 		// -- only Z-peak -- //
// 		Hist_CV->h->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Smeared->h->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Smeared->h_ratio->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Smeared->h_ratio->GetYaxis()->SetRangeUser( 0.995, 1.005 );
// 		Hist_Smeared->h_ratio->GetYaxis()->SetNdivisions( 505 );

// 		c->cd();
// 		TopPad->cd();
// 		TopPad->SetLogx(kFALSE);

// 		c->cd();
// 		BottomPad->cd();
// 		BottomPad->SetLogx(kFALSE);
// 		////////////////////////////

// 		c->SaveAs(".pdf");

// 		// Print_Histogram( Hist_CV->h );
// 		// Print_Histogram( Hist_Smeared->h );
// 	}

// 	void DrawCanvas_BkgSub_vs_Unfolded_CV()
// 	{
// 		HistInfo *Hist_BkgSub = new HistInfo( kBlack, "Bkg.Sub." );
// 		Hist_BkgSub->Set_Histogram( this->Hists_CV->Get_Histogram( "BkgSub" ) );
// 		Hist_BkgSub->Set();

// 		HistInfo *Hist_Unfolded = new HistInfo( kRed, "Unfolded");
// 		Hist_Unfolded->Set_Histogram( this->Hists_CV->Get_Histogram( "Unfolded" ) );
// 		Hist_Unfolded->Set();
// 		Hist_Unfolded->CalcRatio_DEN( Hist_BkgSub->h );

// 		// -- draw canvas -- //
// 		TCanvas *c; TPad *TopPad; TPad *BottomPad;
// 		TString CanvasName = "c_BkgSub_vs_Unfolded_CV";
// 		SetCanvas_Ratio(c, CanvasName, TopPad, BottomPad, 1, 1);

// 		c->cd();
// 		TopPad->cd();

// 		Hist_BkgSub->Draw("EPSAME");
// 		Hist_Unfolded->Draw("EPSAME");

// 		SetHistFormat_TopPad( Hist_BkgSub->h, "Entries per bin");
// 		Hist_BkgSub->h->GetYaxis()->SetRangeUser( 0.5, 1e8 );

// 		TLegend *legend;
// 		SetLegend( legend, 0.75, 0.85, 0.95, 0.95 );
// 		Hist_BkgSub->AddToLegend( legend );
// 		Hist_Unfolded->AddToLegend( legend );
// 		legend->Draw();

// 		TLatex latex;
// 		Latex_Preliminary( latex, 2.8, 13 );
// 		latex.DrawLatexNDC( 0.15, 0.91, TString::Format("#font[42]{#scale[0.8]{%s}}", "Central value") );

// 		c->cd();
// 		BottomPad->cd();

// 		Hist_Unfolded->DrawRatio("EPSAME");
// 		SetHistFormat_BottomPad( Hist_Unfolded->h_ratio, "m [GeV]", "Unfolded/BkgSub", 0.4, 1.6 );

// 		// -- only Z-peak -- //
// 		Hist_BkgSub->h->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Unfolded->h->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Unfolded->h_ratio->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Unfolded->h_ratio->GetYaxis()->SetRangeUser( 0.7, 1.3 );
// 		Hist_Unfolded->h_ratio->GetYaxis()->SetNdivisions( 506 );

// 		c->cd();
// 		TopPad->cd();
// 		TopPad->SetLogx(kFALSE);

// 		c->cd();
// 		BottomPad->cd();
// 		BottomPad->SetLogx(kFALSE);
// 		////////////////////////////

// 		c->SaveAs(".pdf");
// 	}

// 	void DrawCanvas_BkgSub_vs_Unfolded_Smeared( Int_t i_smeared )
// 	{
// 		HistInfo *Hist_BkgSub = new HistInfo( kBlack, "Bkg.Sub." );
// 		Hist_BkgSub->Set_Histogram( this->Hists_Smeared[i_smeared]->Get_Histogram( "BkgSub" ) );
// 		Hist_BkgSub->Set();

// 		HistInfo *Hist_Unfolded = new HistInfo( kRed, "Unfolded");
// 		Hist_Unfolded->Set_Histogram( this->Hists_Smeared[i_smeared]->Get_Histogram( "Unfolded" ) );
// 		Hist_Unfolded->Set();
// 		Hist_Unfolded->CalcRatio_DEN( Hist_BkgSub->h );

// 		// -- draw canvas -- //
// 		TCanvas *c; TPad *TopPad; TPad *BottomPad;
// 		TString CanvasName = TString::Format("c_BkgSub_vs_Unfolded_Smeared_%03d", i_smeared);
// 		SetCanvas_Ratio(c, CanvasName, TopPad, BottomPad, 1, 1);

// 		c->cd();
// 		TopPad->cd();

// 		Hist_BkgSub->Draw("EPSAME");
// 		Hist_Unfolded->Draw("EPSAME");

// 		SetHistFormat_TopPad( Hist_BkgSub->h, "Entries per bin");
// 		Hist_BkgSub->h->GetYaxis()->SetRangeUser( 0.5, 1e8 );

// 		TLegend *legend;
// 		SetLegend( legend, 0.75, 0.85, 0.95, 0.95 );
// 		Hist_BkgSub->AddToLegend( legend );
// 		Hist_Unfolded->AddToLegend( legend );
// 		legend->Draw();

// 		TLatex latex;
// 		Latex_Preliminary( latex, 2.8, 13 );
// 		latex.DrawLatexNDC( 0.15, 0.91, TString::Format("#font[42]{#scale[0.8]{Smeared_%d}}", i_smeared) );

// 		c->cd();
// 		BottomPad->cd();

// 		Hist_Unfolded->DrawRatio("EPSAME");
// 		SetHistFormat_BottomPad( Hist_Unfolded->h_ratio, "m [GeV]", "Unfolded/BkgSub", 0.4, 1.6 );

// 		// -- only Z-peak -- //
// 		Hist_BkgSub->h->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Unfolded->h->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Unfolded->h_ratio->GetXaxis()->SetRangeUser( 60, 120 );
// 		Hist_Unfolded->h_ratio->GetYaxis()->SetRangeUser( 0.7, 1.3 );
// 		Hist_Unfolded->h_ratio->GetYaxis()->SetNdivisions( 506 );

// 		c->cd();
// 		TopPad->cd();
// 		TopPad->SetLogx(kFALSE);

// 		c->cd();
// 		BottomPad->cd();
// 		BottomPad->SetLogx(kFALSE);
// 		////////////////////////////

// 		c->SaveAs(".pdf");
// 	}

// 	void Print_BkgSub_Unfolded_CV_Smeared( Int_t i_bin, Int_t i_smeared )
// 	{
// 		printf("[Central value]\n");
// 		TH1D* h_BkgSub_CV = this->Hists_CV->Get_Histogram( "BkgSub" );
// 		TH1D* h_Unfolded_CV = this->Hists_CV->Get_Histogram( "Unfolded" );
// 		this->Print_BkgSub_Unfolded( h_BkgSub_CV, h_Unfolded_CV, i_bin );

// 		printf("[Smeared value: %03d-th]\n", i_smeared);
// 		TH1D* h_BkgSub_Smeared = this->Hists_Smeared[i_smeared]->Get_Histogram( "BkgSub" );
// 		TH1D* h_Unfolded_Smeared = this->Hists_Smeared[i_smeared]->Get_Histogram( "Unfolded" );
// 		this->Print_BkgSub_Unfolded( h_BkgSub_Smeared, h_Unfolded_Smeared, i_bin );

// 		printf("[Bkg.sub. step]\n");
// 		this->Print_CV_Smeared( h_BkgSub_CV, h_BkgSub_Smeared, i_bin );
// 		printf("[Unfolded step]\n");
// 		this->Print_CV_Smeared( h_Unfolded_CV, h_Unfolded_Smeared, i_bin );
// 		printf("\n");
// 	}

// 	void Print_CV_Smeared( TH1D* h_CV, TH1D* h_Smeared, Int_t i_bin )
// 	{
// 		Double_t CV = h_CV->GetBinContent(i_bin);
// 		Double_t Smeared = h_Smeared->GetBinContent(i_bin);
// 		Double_t AbsDiff = Smeared - CV;
// 		Double_t RelDiff = AbsDiff / CV;

// 		printf( "\t[RelDiff] (%.1lf - %.1lf = %.1lf) / %.1lf = %.3lf %%\n", Smeared, CV, AbsDiff, CV, RelDiff*100 );
// 	}

// 	void Print_BkgSub_Unfolded( TH1D* h_BkgSub, TH1D* h_Unfolded, Int_t i_bin )
// 	{
// 		Double_t BkgSub = h_BkgSub->GetBinContent(i_bin);
// 		Double_t Unfolded = h_Unfolded->GetBinContent(i_bin);
// 		Double_t AbsDiff = Unfolded - BkgSub;
// 		Double_t RelDiff = AbsDiff / BkgSub;

// 		Double_t BkgSub_PreBin = h_BkgSub->GetBinContent(i_bin-1);
// 		Double_t Unfolded_PreBin = h_Unfolded->GetBinContent(i_bin-1);
// 		Double_t AbsDiff_PreBin = Unfolded_PreBin - BkgSub_PreBin;
// 		Double_t RelDiff_PreBin = AbsDiff_PreBin / BkgSub_PreBin;

// 		Double_t BkgSub_NextBin = h_BkgSub->GetBinContent(i_bin+1);
// 		Double_t Unfolded_NextBin = h_Unfolded->GetBinContent(i_bin+1);
// 		Double_t AbsDiff_NextBin = Unfolded_NextBin - BkgSub_NextBin;
// 		Double_t RelDiff_NextBin = AbsDiff_NextBin / BkgSub_NextBin;

// 		printf("\t[BkgSub -> Unfolded]: %.1lf -> %.1lf (%.1lf, %.3lf %%)\n", BkgSub, Unfolded, AbsDiff, RelDiff*100 );
// 		printf("\t[Previous bin]: %.1lf -> %.1lf (%.1lf, %.3lf %%)\n", BkgSub_PreBin, Unfolded_PreBin, AbsDiff_PreBin, RelDiff_PreBin*100 );
// 		printf("\t[Next bin]: %.1lf -> %.1lf (%.1lf, %.3lf %%)\n", BkgSub_NextBin, Unfolded_NextBin, AbsDiff_NextBin, RelDiff_NextBin*100 );
// 	}
// };
