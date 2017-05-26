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

	// -- fiducial, post-FSR results -- //
	TH1D* h_FpoF_EffCorr_4p2;
	TH1D* h_FpoF_EffCorr_4p3;
	TH1D* h_FpoF_EffSFCorr_4p2;
	TH1D* h_FpoF_EffSFCorr_4p3;
	TH1D* h_FpoF_DiffXSec;

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

	HistogramContainer( TH1D* _h_4p2, TH1D* _h_4p3 ) : HistogramContainer()
	{
		this->Set_DataHistogram( _h_4p2, _h_4p3 );
	}

	void Set_Type( TString _Type )
	{
		this->Type = _Type;
	}

	void Set_DataHistogram( TH1D* _h_4p2, TH1D* _h_4p3 )
	{
		this->h_data_4p2 = (TH1D*)_h_4p2->Clone();
		this->h_data_4p3 = (TH1D*)_h_4p3->Clone();
	}

	TH1D* Get_Histogram( TString Type )
	{
		TH1D* h_return = NULL;
		if( Type == "Observed" )
		{
			h_return = (TH1D*)this->h_data_4p2->Clone();
			h_return->Add( this->h_data_4p3 );
		}
		else if( Type == "BkgSub" )
		{
			h_return = (TH1D*)this->h_BkgSub_4p2->Clone();
			h_return->Add( this->h_BkgSub_4p3 );
		}
		else if( Type == "Unfolded" )
		{
			h_return = (TH1D*)this->h_Unfolded_4p2->Clone();
			h_return->Add( this->h_Unfolded_4p3 );
		}
		else if( Type == "AccEffCorr" )
		{
			h_return = (TH1D*)this->h_AccEffCorr_4p2->Clone();
			h_return->Add( this->h_AccEffCorr_4p3 );
		}
		else if( Type == "EffSFCorr" )
		{
			h_return = (TH1D*)this->h_EffSFCorr_4p2->Clone();
			h_return->Add( this->h_EffSFCorr_4p3 );
		}
		else if( Type == "FSRCorr" )
		{
			h_return = (TH1D*)h_FSRCorr->Clone();
		}
		else if( Type == "DiffXSec" )
		{
			h_return = (TH1D*)h_DiffXSec->Clone();
		}
		else if( Type == "FpoF_DiffXSec" )
		{
			h_return = (TH1D*)h_FpoF_DiffXSec->Clone();
		}
		return h_return;
	}

	void Make_Histogram_All()
	{
		this->Make_Histogram_BkgSub();
		this->Make_Histogram_Unfolded();
		this->Make_Histogram_AccEffCorr();
		this->Make_Histogram_EffSFCorr();
		this->Make_Histogram_FSRCorr();
		this->Make_Histogram_DiffXSec();

		this->Make_Histogram_FpoF_EffCorr();
		this->Make_Histogram_FpoF_EffSFCorr();
		this->Make_Histogram_FpoF_DiffXSec();
	}

	void Save_Histogram( TFile *f_output )
	{
		this->Set_HistogramName();

		f_output->cd();
		this->h_data_4p2->Write();
		this->h_data_4p3->Write();

		this->h_BkgSub_4p2->Write();
		this->h_BkgSub_4p3->Write();

		this->h_Unfolded_4p2->Write();
		this->h_Unfolded_4p3->Write();

		this->h_AccEffCorr_4p2->Write();
		this->h_AccEffCorr_4p3->Write();

		this->h_EffSFCorr_4p2->Write();
		this->h_EffSFCorr_4p3->Write();

		this->h_FSRCorr->Write();

		this->h_DiffXSec->Write();
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

	void Set_HistogramName()
	{
		TString HistName = "";

		HistName = "h_data_4p2_"+this->Type;
		this->h_data_4p2->SetName(HistName);

		HistName = "h_data_4p3_"+this->Type;
		this->h_data_4p3->SetName(HistName);

		HistName = "h_BkgSub_4p2_"+this->Type;
		this->h_BkgSub_4p2->SetName(HistName);

		HistName = "h_BkgSub_4p3_"+this->Type;
		this->h_BkgSub_4p3->SetName(HistName);

		HistName = "h_Unfolded_4p2_"+this->Type;
		this->h_Unfolded_4p2->SetName(HistName);

		HistName = "h_Unfolded_4p3_"+this->Type;
		this->h_Unfolded_4p3->SetName(HistName);

		HistName = "h_AccEffCorr_4p2_"+this->Type;
		this->h_AccEffCorr_4p2->SetName(HistName);

		HistName = "h_AccEffCorr_4p3_"+this->Type;
		this->h_AccEffCorr_4p3->SetName(HistName);

		HistName = "h_EffSFCorr_4p2_"+this->Type;
		this->h_EffSFCorr_4p2->SetName(HistName);

		HistName = "h_EffSFCorr_4p3_"+this->Type;
		this->h_EffSFCorr_4p3->SetName(HistName);

		HistName = "h_FSRCorr_"+this->Type;
		this->h_FSRCorr->SetName(HistName);

		HistName = "h_DiffXSec_"+this->Type;
		this->h_DiffXSec->SetName(HistName);

		// -- fiducial, post-FSR -- //
		HistName = "h_FpoF_EffCorr_4p2_"+this->Type;
		this->h_FpoF_EffCorr_4p2->SetName( HistName );

		HistName = "h_FpoF_EffCorr_4p3_"+this->Type;
		this->h_FpoF_EffCorr_4p3->SetName( HistName );

		HistName = "h_FpoF_EffSFCorr_4p2_"+this->Type;
		this->h_FpoF_EffSFCorr_4p2->SetName( HistName );

		HistName = "h_FpoF_EffSFCorr_4p3_"+this->Type;
		this->h_FpoF_EffSFCorr_4p3->SetName( HistName );

		HistName = "h_FpoF_DiffXSec_"+this->Type;
		this->h_FpoF_DiffXSec->SetName( HistName );
	}

	void Make_Histogram_BkgSub()
	{
		// -- MC bkg. after applying efficiency SF -- //
		TH1D *h_WZ_4p2 = this->Get_Histogram_MCBkg("HLTv4p2", "WZ");
		TH1D *h_ZZ_4p2 = this->Get_Histogram_MCBkg("HLTv4p2", "ZZ");

		TH1D *h_WZ_4p3 = this->Get_Histogram_MCBkg("HLTv4p3", "WZ");
		TH1D *h_ZZ_4p3 = this->Get_Histogram_MCBkg("HLTv4p3", "ZZ");


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

		TH1D* h_bkg_4p2 = (TH1D*)h_bkg_noMCBkg->Clone();
		h_bkg_4p2->Scale( Lumi_HLTv4p2 / Lumi );
		h_bkg_4p2->Add( h_WZ_4p2 );
		h_bkg_4p2->Add( h_ZZ_4p2 );

		TH1D* h_bkg_4p3 = (TH1D*)h_bkg_noMCBkg->Clone();
		h_bkg_4p3->Scale( (Lumi - Lumi_HLTv4p2) / Lumi );
		h_bkg_4p3->Add( h_WZ_4p3 );
		h_bkg_4p3->Add( h_ZZ_4p3 );

		// -- background subtraction: HLT v4.2 -- //
		this->h_BkgSub_4p2 = (TH1D*)this->h_data_4p2->Clone(); h_BkgSub_4p2->Sumw2();
		this->h_BkgSub_4p2->Add( h_bkg_4p2, -1 );

		// -- background subtraction: HLT v4.3 -- //
		this->h_BkgSub_4p3 = (TH1D*)this->h_data_4p3->Clone(); h_BkgSub_4p3->Sumw2();
		this->h_BkgSub_4p3->Add( h_bkg_4p3, -1 );

		// -- Remove negative bins -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			// -- HLT v4.2 -- //
			Double_t nEvent_4p2 = this->h_BkgSub_4p2->GetBinContent(i_bin);
			if( nEvent_4p2 < 0 )
			{
				this->h_BkgSub_4p2->SetBinContent(i_bin, 0);
				this->h_BkgSub_4p2->SetBinError(i_bin, 0);
			}

			// -- HLT v4.3 -- //
			Double_t nEvent_4p3 = this->h_BkgSub_4p3->GetBinContent(i_bin);
			if( nEvent_4p3 < 0 )
			{
				this->h_BkgSub_4p3->SetBinContent(i_bin, 0);
				this->h_BkgSub_4p3->SetBinError(i_bin, 0);
			}
		}
	}

protected:

	void Make_Histogram_Unfolded()
	{
		TString FileName = this->FileLocation + "/ROOTFile_Histogram_ResponseM_1D_aMCNLO_IsoMu20_OR_IsoTkMu20.root";
		TFile *f_Unfold = TFile::Open( FileName );

		// -- HLTv4.2 -- //
		RooUnfoldResponse *UnfoldRes_HLTv4p2 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p2 = new RooUnfoldBayes(UnfoldRes_HLTv4p2, this->h_BkgSub_4p2, 17); // -- iteration until convergence -- //
		this->h_Unfolded_4p2 = (TH1D*)UnfoldBayes_HLTv4p2->Hreco();

		// -- HLTv4.3 -- //
		RooUnfoldResponse *UnfoldRes_HLTv4p3 = (RooUnfoldResponse*)f_Unfold->Get("h_RecoMass_h_GenMass")->Clone();
		RooUnfoldBayes *UnfoldBayes_HLTv4p3 = new RooUnfoldBayes(UnfoldRes_HLTv4p3, this->h_BkgSub_4p3, 17); // -- iteration until convergence -- //
		this->h_Unfolded_4p3 = (TH1D*)UnfoldBayes_HLTv4p3->Hreco();

		f_Unfold->Close();
	}

	void Make_Histogram_AccEffCorr()
	{
		TFile *f_AccEff = TFile::Open(this->FileLocation + "/ROOTFile_AccEff.root"); f_AccEff->cd();
		TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)f_AccEff->Get("g_AccEff");
		// f_AccEff->Close();

		// -- HLTv4p2 -- //
		this->h_AccEffCorr_4p2 = (TH1D*)this->h_Unfolded_4p2->Clone();
		this->Correction_AccEff( this->h_AccEffCorr_4p2, this->h_Unfolded_4p2, g_AccEff );

		// -- HLTv4p3 -- //
		this->h_AccEffCorr_4p3 = (TH1D*)this->h_Unfolded_4p3->Clone();
		this->Correction_AccEff( this->h_AccEffCorr_4p3, this->h_Unfolded_4p3, g_AccEff );

		f_AccEff->Close();
	}

	void Make_Histogram_EffSFCorr()
	{
		TGraphAsymmErrors* g_EffCorr_4p2 = this->CalcEffSF_Mass( "HLTv4p2" );
		TGraphAsymmErrors* g_EffCorr_4p3 = this->CalcEffSF_Mass( "HLTv4p3" );

		this->h_EffSFCorr_4p2 = (TH1D*)this->h_AccEffCorr_4p2->Clone();
		this->ApplyEffCorr_Yield( this->h_EffSFCorr_4p2, this->h_AccEffCorr_4p2, g_EffCorr_4p2 );

		this->h_EffSFCorr_4p3 = (TH1D*)this->h_AccEffCorr_4p3->Clone();
		this->ApplyEffCorr_Yield( this->h_EffSFCorr_4p3, this->h_AccEffCorr_4p3, g_EffCorr_4p3 );
	}

	///////////////////////////////////////////////////////
	// -- Calculation of efficiency correction factor -- //
	///////////////////////////////////////////////////////
	// -- Type: HLTv4p2, HLTv4p3 -- //
	TGraphAsymmErrors* CalcEffSF_Mass( TString Type )
	{
		TString FileName = this->FileLocation + "/ROOTFile_AccEff.root";

		TGraphAsymmErrors *g_Eff = Get_Graph( FileName, "g_Eff" );
		TGraphAsymmErrors *g_Eff_Corrected = Get_Graph( FileName, "g_Eff_Corr_"+Type );

		GraphInfo *Graph_CorrEff = new GraphInfo( kBlack, "temp" );
		Graph_CorrEff->Set_Graph( g_Eff_Corrected );
		Graph_CorrEff->CalcRatio_DEN( g_Eff );

		return (TGraphAsymmErrors*)Graph_CorrEff->g_ratio->Clone();
	}

	void Make_Histogram_FSRCorr()
	{
		//////////////////////////
		// -- FSR Correction -- //
		//////////////////////////
		TFile *f_FSR = TFile::Open(this->FileLocation + "/ROOTFile_FSRCorrections_DressedLepton_aMCNLO.root"); f_FSR->cd();
		RooUnfoldResponse *UnfoldRes_FSR = (RooUnfoldResponse*)f_FSR->Get("UnfoldRes")->Clone();
		f_FSR->Close();

		TH1D* h_EffSFCorr = (TH1D*)this->h_EffSFCorr_4p2->Clone();
		h_EffSFCorr->Add( this->h_EffSFCorr_4p3 );

		RooUnfoldInvert *UnfoldInvert_FSR = new RooUnfoldInvert(UnfoldRes_FSR, h_EffSFCorr); // -- use inverting method after following recommendation by stat.committee -- //
		this->h_FSRCorr = (TH1D*)UnfoldInvert_FSR->Hreco();
	}

	void Make_Histogram_DiffXSec()
	{
		TH1D* h_XSec = (TH1D*)this->h_FSRCorr->Clone();
		h_XSec->Scale( 1.0 / Lumi );
		this->h_DiffXSec = DivideEachBin_ByBinWidth( h_XSec );
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

	TH1D* Get_Histogram_MCBkg( TString HLTType, TString Type )
	{
		this->FileLocation = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString FileName = this->FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";

		TString HistName = "";
		if( (HLTType == "HLTv4p2") || (HLTType == "HLTv4p3") )
			HistName = "h_mass_OS_"+HLTType+"_Norm_"+Type;
		else
			HistName = "h_mass_OS_Norm_"+Type;

		TH1D* h_mass = Get_Hist( FileName, HistName );
		h_mass = (TH1D*)h_mass->Rebin(nMassBin, h_mass->GetName(), this->MassBinEdges);

		return h_mass;
	}

	void Make_Histogram_FpoF_EffCorr()
	{
		TString FileName = this->FileLocation + "/ROOTFile_AccEff.root";
		TGraphAsymmErrors *g_Eff = Get_Graph( FileName, "g_Eff" );

		// -- HLTv4p2 -- //
		this->h_FpoF_EffCorr_4p2 = (TH1D*)this->h_Unfolded_4p2->Clone();
		this->Correction_AccEff( this->h_FpoF_EffCorr_4p2, this->h_Unfolded_4p2, g_Eff );

		// -- HLTv4p3 -- //
		this->h_FpoF_EffCorr_4p3 = (TH1D*)this->h_Unfolded_4p3->Clone();
		this->Correction_AccEff( this->h_FpoF_EffCorr_4p3, this->h_Unfolded_4p3, g_Eff );
	}

	void Make_Histogram_FpoF_EffSFCorr()
	{
		TGraphAsymmErrors* g_EffCorr_4p2 = this->CalcEffSF_Mass( "HLTv4p2" );
		TGraphAsymmErrors* g_EffCorr_4p3 = this->CalcEffSF_Mass( "HLTv4p3" );

		this->h_FpoF_EffSFCorr_4p2 = (TH1D*)this->h_FpoF_EffCorr_4p2->Clone();
		this->ApplyEffCorr_Yield( this->h_FpoF_EffSFCorr_4p2, this->h_FpoF_EffCorr_4p2, g_EffCorr_4p2 );

		this->h_FpoF_EffSFCorr_4p3 = (TH1D*)this->h_FpoF_EffCorr_4p3->Clone();
		this->ApplyEffCorr_Yield( this->h_FpoF_EffSFCorr_4p3, this->h_FpoF_EffCorr_4p3, g_EffCorr_4p3 );
	}

	void Make_Histogram_FpoF_DiffXSec()
	{
		TH1D* h_FpoF_EffSFCorr = (TH1D*)this->h_FpoF_EffSFCorr_4p2->Clone("h_FpoF_EffSFCorr");
		h_FpoF_EffSFCorr->Add( this->h_FpoF_EffSFCorr_4p3 );

		TH1D* h_XSec = (TH1D*)h_FpoF_EffSFCorr->Clone();
		h_XSec->Scale( 1.0 / Lumi );
		this->h_FpoF_DiffXSec = DivideEachBin_ByBinWidth( h_XSec );
	}
};


class StatUncTool
{
public:
	HistogramContainer *Hists_CV;
	HistogramContainer *Hists_Smeared[nMap];

	TH1D* h_RelStatUnc;
	TH1D* h_FpoF_RelStatUnc;

	StatUncTool()
	{
		TH1::AddDirectory(kFALSE);
		
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString FileName =  ROOTFilePath + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root";

		TH1D* h_data_4p2 = Get_Hist( FileName, "h_mass_OS_HLTv4p2_Data" ); h_data_4p2->Sumw2();
		TH1D* h_data_4p3 = Get_Hist( FileName, "h_mass_OS_HLTv4p3_Data" ); h_data_4p3->Sumw2();

		h_data_4p2 = this->Rebin_DYBin( h_data_4p2 );
		h_data_4p3 = this->Rebin_DYBin( h_data_4p3 );

		this->Hists_CV = new HistogramContainer( h_data_4p2, h_data_4p3 );
		this->Hists_CV->Set_Type( "CV" );
		this->Set_HistContainers_Randomized();

		this->h_RelStatUnc = new TH1D("h_RelStatUnc", "", 3000, 0, 3000);
		this->h_RelStatUnc = this->Rebin_DYBin( this->h_RelStatUnc );

		this->h_FpoF_RelStatUnc = new TH1D("h_FpoF_RelStatUnc", "", 3000, 0, 3000);
		this->h_FpoF_RelStatUnc = this->Rebin_DYBin( this->h_FpoF_RelStatUnc );
	}

	void Estimate()
	{
		this->Hists_CV->Make_Histogram_All();
		for(Int_t i_map=0; i_map<nMap; i_map++)
			this->Hists_Smeared[i_map]->Make_Histogram_All();

		this->FitGaussian_GetSigma("All", this->h_RelStatUnc);
		this->FitGaussian_GetSigma("FpoF", this->h_FpoF_RelStatUnc);
	}

	void SetStatUnc_ByHand()
	{
		this->h_RelStatUnc->SetBinContent(43, 1); // -- 100% by hand: fit failed because of small statistics -- //
		this->h_FpoF_RelStatUnc->SetBinContent(43, 1);
	}

	void Save_Results( TFile *f_output )
	{
		f_output->cd();
		this->h_RelStatUnc->Write();
		this->h_FpoF_RelStatUnc->Write();

		TH1D* h_RelStatUnc_Percent = (TH1D*)h_RelStatUnc->Clone("h_RelStatUnc_Percent");
		h_RelStatUnc_Percent->Scale( 100 );
		h_RelStatUnc_Percent->Write();

		TH1D* h_FpoF_RelStatUnc_Percent = (TH1D*)h_FpoF_RelStatUnc->Clone("h_FpoF_RelStatUnc_Percent");
		h_FpoF_RelStatUnc_Percent->Scale( 100 );
		h_FpoF_RelStatUnc_Percent->Write();

		this->Hists_CV->Set_HistogramName();
		this->Hists_CV->h_DiffXSec->Write();
		this->Hists_CV->h_FpoF_DiffXSec->Write();
		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			this->Hists_Smeared[i_map]->Set_HistogramName();
			this->Hists_Smeared[i_map]->h_DiffXSec->Write();
			this->Hists_Smeared[i_map]->h_FpoF_DiffXSec->Write();
		}
	}

	void Make_XCheckPlot_DiffXSec()
	{
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TH1D* h_Ref = Get_Hist( ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_DiffXsec_FSRCorr" );

		TString CanvasName = "c_this_vs_ref_DiffXSec";
		this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_DiffXSec, h_Ref);

		CanvasName = "c_this_vs_ref_FpoF_DiffXSec";
		TH1D* h_Ref_FpoF = Get_Hist(ROOTFilePath+"/ROOTFile_Results_DYAnalysis_76X.root", "h_FpoF_DiffXsec_Data" );
		this->Comparison_this_vs_ref( CanvasName, this->Hists_CV->h_FpoF_DiffXSec, h_Ref_FpoF);
	}


protected:
	void Set_HistContainers_Randomized()
	{
		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			TH1D* h_data_4p2_smeared = (TH1D*)this->Hists_CV->h_data_4p2->Clone();
			TH1D* h_data_4p3_smeared = (TH1D*)this->Hists_CV->h_data_4p3->Clone();

			Int_t nBin = h_data_4p2_smeared->GetNbinsX();
			for(Int_t i=0; i<nBin; i++) // -- make smeared histogram for i_map-th map -- //
			{
				Int_t i_bin = i+1;
				Double_t CV_4p2 = this->Hists_CV->h_data_4p2->GetBinContent(i_bin);
				Double_t CV_4p3 = this->Hists_CV->h_data_4p3->GetBinContent(i_bin);

				Double_t Smeared_4p2 = (Double_t)gRandom->Poisson( CV_4p2 );
				Double_t Smeared_4p3 = (Double_t)gRandom->Poisson( CV_4p3 );

				h_data_4p2_smeared->SetBinContent(i_bin, Smeared_4p2 );
				h_data_4p2_smeared->SetBinError(i_bin, sqrt(Smeared_4p2) );

				h_data_4p3_smeared->SetBinContent(i_bin, Smeared_4p3 );
				h_data_4p3_smeared->SetBinError(i_bin, sqrt(Smeared_4p3) );
			}

			this->Hists_Smeared[i_map] = new HistogramContainer( h_data_4p2_smeared, h_data_4p3_smeared );
			this->Hists_Smeared[i_map]->Set_Type( TString::Format("Smeared_%03d", i_map) );
		}
	}

	void FitGaussian_GetSigma(TString Type, TH1D* h_RelStatUnc_this )
	{
		Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			TTree *tree = this->makeTTree(i_bin, Type);

			Double_t BinCenter = ( MassBinEdges[i] + MassBinEdges[i+1] ) / 2.0;
			Double_t RangeMax = 0.1;
			Double_t Sigma_Init = 0.01;
			this->Set_FitInitValues( BinCenter, RangeMax, Sigma_Init );

			RooRealVar RelDiff("RelDiff","(#sigma_{Smeared} - #sigma_{CV}) / #sigma_{CV}", (-1)*RangeMax, RangeMax);
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

			TString CanvasName = TString::Format("c_RelDiff_Bin%02d", i_bin);
			if( Type == "FpoF" )
				CanvasName.ReplaceAll("c_", "c_FpoF_");

			TCanvas *c = new TCanvas(CanvasName, "", 700, 700); c->cd();
			frame->Draw();

			c->SaveAs(CanvasName+".pdf");
			
			Double_t RMS = sigma.getVal();
			// Double_t RMSErr = sigma.getError();

			h_RelStatUnc_this->SetBinContent(i_bin, RMS);
			h_RelStatUnc_this->SetBinError(i_bin, 0);
		}
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

	TTree* makeTTree(Int_t i_bin, TString Type) 
	{
		TTree* tree = new TTree("tree","tree");

		Double_t* RelDiff = new Double_t;

		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

		TH1D *h_CV;
		if( Type == "All" )
			h_CV = this->Hists_CV->Get_Histogram( "DiffXSec" );
		else if( Type == "FpoF" )
			h_CV = this->Hists_CV->Get_Histogram( "FpoF_DiffXSec" );

		Double_t value_CV = h_CV->GetBinContent(i_bin);

		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			TH1D *h_Smeared;
			if( Type == "All" )
				h_Smeared = this->Hists_Smeared[i_map]->Get_Histogram( "DiffXSec" );
			else if( Type == "FpoF" )
				h_Smeared = this->Hists_Smeared[i_map]->Get_Histogram( "FpoF_DiffXSec" );

			Double_t value_Smeared = h_Smeared->GetBinContent(i_bin);

			*RelDiff = ( value_Smeared - value_CV ) / value_CV;

			// printf("[\t%d th RelDiff = %lf]\n", i, *RelDiff);

			tree->Fill();
		}

		return tree;
	}

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
		SetHistFormat_TopPad( Hist_ref->h, "d#sigma/dm [pb/GeV]" );

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

	TH1D* Rebin_DYBin( TH1D* h_before )
	{
		Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};

		return (TH1D*)h_before->Rebin(nMassBin, h_before->GetName(), MassBinEdges);
	}



};