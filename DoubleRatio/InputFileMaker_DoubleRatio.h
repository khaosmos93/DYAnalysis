#include <Include/PlotTools.h>

const Int_t nMassBin = 40;
Double_t MassBinEdges[nMassBin+1] = {
	15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 
	64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 
	110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 
	200, 220, 243, 273, 320, 380, 440, 510, 600, 1000,
	1500};

class InputMaker
{
public:
	TString AnalyzerPath;
	TString ROOTFilePath;
	vector< TH1D* > vec_Hist;

	/////////////////
	// -- 8 TeV -- //
	/////////////////
	// -- experimental results -- //
	TH1D* h_ZpeakXSec_8;
	TH1D* h_DiffXSec_8;
	TH1D* h_NormXSec_8;
	TH1D* h_ExpUnc_8;
	TH1D* h_TheoUnc_8;

	// -- theory -- //
	TH1D* h_ZpeakXSec_NNLO_8;
	TH1D* h_DiffXSec_NNLO_8;
	TH1D* h_NormXSec_NNLO_8;

	/////////////////
	// -- 13 TeV -- //
	/////////////////
	// -- experimental results -- //
	TH1D* h_ZpeakXSec_13;
	TH1D* h_DiffXSec_13;
	TH1D* h_NormXSec_13;
	TH1D* h_ExpUnc_13;
	TH1D* h_TheoUnc_13;

	// -- theory -- //
	TH1D* h_ZpeakXSec_NNLO_13;
	TH1D* h_DiffXSec_NNLO_13;
	TH1D* h_NormXSec_NNLO_13;

	TString TStr_Channel;

	TString TheoType_8TeV;
	TString TheoType_13TeV;

	InputMaker()
	{
		TH1::AddDirectory(kFALSE);
		this->AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	}

	void Save( TFile *f_output )
	{
		this->PutIntoVector();

		Int_t nHist = (Int_t)vec_Hist.size();
		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			vec_Hist[i_hist]->Write();
	}

	void Get_Histograms_MM()
	{
		this->TStr_Channel = "MM";
		this->Init();

		this->Get_Histogram_Theory_8eV_CT10();
		// this->Get_Histogram_Theory_8eV_NNPDF();
		this->Get_Histogram_Theory_13TeV();
		this->Get_Histograms_MM_8TeV();
		this->Get_Histograms_MM_13TeV();
	}

	void Get_Histograms_LL()
	{
		this->TStr_Channel = "LL";
		this->Init();

		this->Get_Histogram_Theory_8eV_CT10();
		// this->Get_Histogram_Theory_8eV_NNPDF();
		this->Get_Histogram_Theory_13TeV();
		this->Get_Histograms_LL_8TeV();
		this->Get_Histograms_LL_13TeV();
	}

	void MakePlots_CrossCheck()
	{
		//////////////////
		// -- 13 TeV -- //
		//////////////////
		this->DrawCanvas_AllUnc( this->h_ExpUnc_13, this->h_TheoUnc_13 );
		this->DrawCanvas_NormXSec_Data_vs_Theory( this->h_NormXSec_13, this->h_NormXSec_NNLO_13 );

		/////////////////
		// -- 8 TeV -- //
		/////////////////
		this->DrawCanvas_AllUnc( this->h_ExpUnc_8, this->h_TheoUnc_8 );
		this->DrawCanvas_NormXSec_Data_vs_Theory( this->h_NormXSec_8, this->h_NormXSec_NNLO_8 );
	}

protected:
	void Init()
	{
		/////////////////
		// -- 8 TeV -- //
		/////////////////
		// -- experimental results -- //
		this->h_ZpeakXSec_8 = NULL;
		this->h_DiffXSec_8 = NULL;
		this->h_NormXSec_8 = NULL;
		this->h_ExpUnc_8 = NULL;
		this->h_TheoUnc_8 = NULL;

		// -- theory -- //
		this->h_ZpeakXSec_NNLO_8 = NULL;
		this->h_DiffXSec_NNLO_8 = NULL;
		this->h_NormXSec_NNLO_8 = NULL;

		/////////////////
		// -- 13 TeV -- //
		/////////////////
		// -- experimental results -- //
		this->h_ZpeakXSec_13 = NULL;
		this->h_DiffXSec_13 = NULL;
		this->h_NormXSec_13 = NULL;
		this->h_ExpUnc_13 = NULL;
		this->h_TheoUnc_13 = NULL;

		// -- theory -- //
		this->h_ZpeakXSec_NNLO_13 = NULL;
		this->h_DiffXSec_NNLO_13 = NULL;
		this->h_NormXSec_NNLO_13 = NULL;
	}
	void Get_Histograms_MM_13TeV()
	{
		// -- 13 TeV, dimuon -- //
		// -- Z-peak -- //
		this->h_ZpeakXSec_13 = new TH1D("h_ZpeakXSec_13", "", 1, 0, 1 );
		h_ZpeakXSec_13->SetBinContent( 1, 1857 );
		h_ZpeakXSec_13->SetBinError( 1, sqrt(14*14 + 25*25) );

		// -- differential cross section -- //
		// TString Path_13 = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval";
		// TFile *f_input_13 = TFile::Open(Path_13+"/ROOTFile_DiffXSec_FullUnc.root");
		TFile *f_input_13 = TFile::Open(this->ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root");

		TH1D* h_DiffXSec_13_beforeBin = (TH1D*)f_input_13->Get("h_DiffXsec_FSRCorr")->Clone("h_DiffXSec_13");
		this->h_DiffXSec_13 = this->Rebin_DiffXSec( h_DiffXSec_13_beforeBin );
		this->h_NormXSec_13 = this->NormXSec_CentralValue( this->h_DiffXSec_13, this->h_ZpeakXSec_13, "h_NormXSec_13" );

		// -- experimental uncertainties -- //
		vector<TH1D*> vec_UncHist;

		TH1D* h_Stat = (TH1D*)f_input_13->Get("h_RelStatUnc_Percent")->Clone("h_Stat"); h_Stat->Scale(0.01);
		h_Stat = this->Rebin_MassBin_UncHist( h_Stat, h_DiffXSec_13_beforeBin ); vec_UncHist.push_back( h_Stat );

		TH1D* h_EffSF = (TH1D*)f_input_13->Get("h_RelSysUnc_Eff.SF._Percent")->Clone("h_EffSF"); h_EffSF->Scale(0.01);
		h_EffSF = this->Rebin_MassBin_UncHist( h_EffSF, h_DiffXSec_13_beforeBin ); vec_UncHist.push_back( h_EffSF );
		
		TH1D* h_DetRes = (TH1D*)f_input_13->Get("h_RelSysUnc_Det.Res_Percent")->Clone("h_DetRes"); h_DetRes->Scale(0.01);
		h_DetRes = this->Rebin_MassBin_UncHist( h_DetRes, h_DiffXSec_13_beforeBin ); vec_UncHist.push_back( h_DetRes );
		
		TH1D* h_Bkg = (TH1D*)f_input_13->Get("h_RelSysUnc_Bkg.Est._Percent")->Clone("h_Bkg"); h_Bkg->Scale(0.01);
		h_Bkg = this->Rebin_MassBin_UncHist( h_Bkg, h_DiffXSec_13_beforeBin ); vec_UncHist.push_back( h_Bkg );
		
		TH1D* h_FSR = (TH1D*)f_input_13->Get("h_RelSysUnc_FSR_Percent")->Clone("h_FSR"); h_FSR->Scale(0.01);
		h_FSR = this->Rebin_MassBin_UncHist( h_FSR, h_DiffXSec_13_beforeBin ); vec_UncHist.push_back( h_FSR );

		this->h_ExpUnc_13 = this->MakeHistogram_QuadSum( vec_UncHist, "h_ExpUnc_13" );

		this->DrawCanvas_ExpUnc_13TeV( h_Stat, h_DetRes, h_Bkg, h_EffSF, h_FSR, this->h_ExpUnc_13 );

		// -- theoretical uncertainties -- //
		this->h_TheoUnc_13 = (TH1D*)f_input_13->Get("h_RelSysUnc_Acc._Percent")->Clone("h_TheoUnc_13");
		this->h_TheoUnc_13->Scale(0.01);
		this->h_TheoUnc_13 = this->Rebin_DiffXSec( this->h_TheoUnc_13 );

		this->NormXSec_AssignError( this->h_NormXSec_13, this->h_ExpUnc_13, this->h_TheoUnc_13 );
	}

	TH1D* Rebin_MassBin_UncHist( TH1D* h_RelUnc_beforeBin, TH1D* h_DiffXSec_beforeBin )
	{
		TH1D* h_updated = this->Rebin_MassBin( h_RelUnc_beforeBin );

		Double_t AbsXSec_M600to1000 = 0;
		Double_t QuadSum2_M600to1000 = 0;
		for(Int_t i=0; i<h_RelUnc_beforeBin->GetNbinsX(); i++)
		{
			Int_t i_bin = i+1;

			Double_t BinCenter = h_RelUnc_beforeBin->GetBinCenter(i_bin);
			if( BinCenter > 600 && BinCenter < 1000 )
			{
				Double_t AbsXSec = h_DiffXSec_beforeBin->GetBinContent(i_bin) * h_DiffXSec_beforeBin->GetBinWidth(i_bin);
				AbsXSec_M600to1000 = AbsXSec_M600to1000 + AbsXSec;

				Double_t AbsUnc = h_RelUnc_beforeBin->GetBinContent(i_bin) * AbsXSec;
				QuadSum2_M600to1000 = QuadSum2_M600to1000 + AbsUnc*AbsUnc;
			}
		}


		Double_t DiffXSec_M600to1000 = AbsXSec_M600to1000 / 400.0; // -- 400 = Bin width -- //
		Double_t AbsUnc_AbsXSec_M600to1000 = sqrt(QuadSum2_M600to1000);
		Double_t AbsUnc_DiffXSec_M600to1000 = AbsUnc_AbsXSec_M600to1000 / 400.0;
		Double_t RelUnc_M600to1000 = AbsUnc_DiffXSec_M600to1000 / DiffXSec_M600to1000;

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinCenter = h_updated->GetBinCenter(i_bin);
			if( BinCenter > 600 && BinCenter < 1000 )
			{
				h_updated->SetBinContent( i_bin, RelUnc_M600to1000 );
				break;
			}
		}

		return h_updated;
	}

	void Get_Histograms_LL_13TeV()
	{
		// -- Z-peak -- //
		this->h_ZpeakXSec_13 = new TH1D("h_ZpeakXSec_13", "", 1, 0, 1 );
		h_ZpeakXSec_13->SetBinContent( 1, 1860 );
		h_ZpeakXSec_13->SetBinError( 1, sqrt(14*14 + 25*25) );

		TString FileName_CenV = this->ROOTFilePath + "/foutCanvas_DYCSCombi_corr_plotChCov.root";
		// -- differential cross section: convert TH1F to TH1D -- //
		TFile *f_input = TFile::Open( FileName_CenV ); f_input->cd();
		TH1F* h_temp = (TH1F*)f_input->Get( "h1Combi" )->Clone();
		TH1D* h_DXSec = new TH1D();
		h_temp->Copy( *h_DXSec );
		h_DXSec->SetTitle(""); // -- before re-binning -- //
		///////////////////////////////////////////

		this->h_DiffXSec_13 = this->Rebin_DiffXSec( h_DXSec );
		this->h_NormXSec_13 = this->NormXSec_CentralValue( this->h_DiffXSec_13, this->h_ZpeakXSec_13, "h_NormXSec_13" );

		// -- uncertainties -- //
		this->h_ExpUnc_13 = Extract_RelUnc( this->h_DiffXSec_13, "h_ExpUnc_13" ); // -- total uncertianty, including theoretical uncertainty -- //
		this->h_ExpUnc_13 = this->Rebin_MassBin_UncHist( this->h_ExpUnc_13, h_DXSec );
		this->h_TheoUnc_13 = new TH1D("h_TheoUnc_13", "", nMassBin, MassBinEdges);
		// -- theory uncertainty is already included in h_ExpUnc_13 -- //
		// -- h_TheoUnc_13 is just filled by 0 -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			this->h_TheoUnc_13->SetBinContent(i_bin, 0);
			this->h_TheoUnc_13->SetBinError(i_bin, 0); 
		}

		this->NormXSec_AssignError( this->h_NormXSec_13, this->h_ExpUnc_13, this->h_TheoUnc_13 );
	}

	void Get_Histogram_Theory_13TeV()
	{
		this->TheoType_13TeV = "NNPDF 3.0";
		// -- theory value -- //
		TFile *f_input_13 = TFile::Open(this->ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root");
		this->h_DiffXSec_NNLO_13 = (TH1D*)f_input_13->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone("h_DiffXSec_NNLO_13");
		this->h_DiffXSec_NNLO_13 = this->Rebin_DiffXSec( this->h_DiffXSec_NNLO_13 );

		Double_t ZpeakXSec_NNLO_13 = this->Calc_ZpeakXSec_FromDiffXSec( this->h_DiffXSec_NNLO_13 ); // -- from 60 to 120 GeV -- //
		Double_t ZpeakXSecErr_NNLO_13 = 0; // -- it is not used yet -- //

		this->h_ZpeakXSec_NNLO_13 = new TH1D("h_ZpeakXSec_NNLO_13", "", 1, 0, 1);
		this->h_ZpeakXSec_NNLO_13->SetBinContent( 1, ZpeakXSec_NNLO_13 );
		this->h_ZpeakXSec_NNLO_13->SetBinError( 1, ZpeakXSecErr_NNLO_13 );

		this->h_NormXSec_NNLO_13 = this->NormXSec_CentralValue_Error( this->h_DiffXSec_NNLO_13, this->h_ZpeakXSec_NNLO_13, "h_NormXSec_NNLO_13" );
		this->h_NormXSec_NNLO_13->SetName("h_NormXSec_NNLO_13");
	}

	void Get_Histograms_MM_8TeV()
	{
		// -- Z-peak -- //
		this->h_ZpeakXSec_8 = new TH1D("h_ZpeakXSec_8", "", 1, 0, 1 );
		h_ZpeakXSec_8->SetBinContent( 1, 1135.4 );
		h_ZpeakXSec_8->SetBinError( 1, sqrt(11.2*11.2 + 25.1*25.1) );

		// -- differential & normalized cross section -- //
		TFile *f_input_8 = TFile::Open("./Input_8TeV/absex_full_Bayesian.root"); f_input_8->cd();
		TH1D* h_DiffXSec_8_beforeBin = (TH1D*)f_input_8->Get("hxsec")->Clone("h_DiffXSec_8");
		this->h_DiffXSec_8 = this->Rebin_DiffXSec( h_DiffXSec_8_beforeBin );
		this->h_NormXSec_8 = this->NormXSec_CentralValue( this->h_DiffXSec_8, this->h_ZpeakXSec_8, "h_NormXSec_8" );

		// -- experimental uncertainties -- //
		vector<TH1D*> vec_UncHist;
		TFile *f_input_unc_8 = TFile::Open("./Input_8TeV/muonSystAll_1D.root"); f_input_unc_8->cd();
		
		TH1D* h_Stat = (TH1D*)f_input_unc_8->Get("stat")->Clone();
		h_Stat = this->Rebin_MassBin(h_Stat);
		vec_UncHist.push_back( h_Stat );
		
		TH1D* h_Unfold = (TH1D*)f_input_unc_8->Get("unf_syst")->Clone();
		h_Unfold = this->Rebin_MassBin(h_Unfold);
		vec_UncHist.push_back( h_Unfold );

		TH1D* h_Bkg = (TH1D*)f_input_unc_8->Get("bkg_syst")->Clone();
		h_Bkg = this->Rebin_MassBin(h_Bkg);
		vec_UncHist.push_back( h_Bkg );

		TH1D* h_EffSF = (TH1D*)f_input_unc_8->Get("effcorr_syst")->Clone();
		for(Int_t i=0; i<h_EffSF->GetNbinsX(); i++)
		{
			Int_t i_bin = i+1;
			h_EffSF->SetBinContent( i_bin, h_EffSF->GetBinError(i_bin) / h_EffSF->GetBinContent(i_bin) );
			h_EffSF->SetBinError(i_bin, 0.0 );
		}
		h_EffSF = this->Rebin_MassBin(h_EffSF);
		vec_UncHist.push_back( h_EffSF );
		
		TH1D* h_FSR = (TH1D*)f_input_unc_8->Get("FSR_syst")->Clone();
		for(Int_t i=0; i<h_FSR->GetNbinsX(); i++)
		{
			Int_t i_bin = i+1;
			h_FSR->SetBinContent(i_bin, h_FSR->GetBinContent(i_bin) / 100 );
			h_FSR->SetBinError(i_bin, 0.0);
		}
		h_FSR = this->Rebin_MassBin(h_FSR);
		vec_UncHist.push_back( h_FSR );

		this->h_ExpUnc_8 = this->MakeHistogram_QuadSum( vec_UncHist, "h_ExpUnc_8" );
		this->DrawCanvas_ExpUnc_8TeV( h_Stat, h_Unfold, h_Bkg, h_EffSF, h_FSR, this->h_ExpUnc_8 );

		this->h_TheoUnc_8 = (TH1D*)f_input_unc_8->Get("acc_syst")->Clone("h_TheoUnc_8");
		this->h_TheoUnc_8 = this->Rebin_MassBin(this->h_TheoUnc_8);

		// -- assign the error on normalized cross section: quad.sum of exp. and theo -- //
		this->NormXSec_AssignError( this->h_NormXSec_8, this->h_ExpUnc_8, this->h_TheoUnc_8 );
	}

	
	void Get_Histograms_LL_8TeV()
	{
		// -- Z-peak -- //
		this->h_ZpeakXSec_8 = new TH1D("h_ZpeakXSec_8", "", 1, 0, 1 );
		h_ZpeakXSec_8->SetBinContent( 1, 1138 );
		h_ZpeakXSec_8->SetBinError( 1, sqrt(8*8 + 25*25) );

		// -- from HEP data of SMP-14-003: http://hepdata.cedar.ac.uk/view/ins1332509 -- //
		Double_t DiffXSec[nMassBin] = {
		184.5, 75.0, 35.9, 19.72, 11.81, 7.32, 4.97, 3.591, 2.750, 2.342,
		2.177, 2.243, 2.623, 3.889, 9.31, 85.7, 103.1, 8.71, 2.961, 1.552, 
		0.983, 0.649, 0.4453, 0.3101, 0.2135, 0.1501, 0.1061, 0.0738, 0.0536, 0.0351,
		0.02404, 0.01532, 0.00909, 0.00491, 0.00221, 0.001034, 0.000476, 0.000221, 0.0000471, 0.00000309 };

		// -- includes uncertainty from acceptance(= theoretical uncertianty ) -- //
		Double_t AbsUnc_DiffXSec[nMassBin] = {
		8.8, 3.2, 1.5, 0.81, 0.39, 0.22, 0.14, 0.094, 0.068, 0.056, 
		0.049, 0.049, 0.055, 0.075, 0.20, 1.4, 1.6, 0.17, 0.053, 0.029, 
		0.019, 0.014, 0.0100, 0.0075, 0.0056, 0.0043, 0.0033, 0.0025, 0.0019, 0.0014, 
		0.00098, 0.00067, 0.00043, 0.00025, 0.00012, 0.000071, 0.000038, 0.000021, 0.0000048, 0.00000084};

		Double_t RelTheoUnc_Percent[nMassBin] = {
		3.8, 3.5, 3.2, 3.0, 2.7, 2.5, 2.3, 2.1, 1.9, 1.8, 
		1.6, 1.5, 1.4, 1.4, 1.4, 1.4, 1.4, 1.4, 1.5, 1.5,
		1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.3, 2.5, 2.6, 2.8,
		3.0, 3.1, 3.3, 3.5, 3.7, 3.7, 3.9, 4.2, 4.5, 4.9 };

		// -- differential & normalized cross section -- //
		this->h_DiffXSec_8 = new TH1D("h_DiffXSec_8", "", nMassBin, MassBinEdges);
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			this->h_DiffXSec_8->SetBinContent( i_bin, DiffXSec[i] );
			this->h_DiffXSec_8->SetBinError( i_bin, AbsUnc_DiffXSec[i] );
		}
		
		this->h_NormXSec_8 = this->NormXSec_CentralValue( this->h_DiffXSec_8, this->h_ZpeakXSec_8, "h_NormXSec_8" );

		// -- uncertainties -- //
		this->h_ExpUnc_8 = new TH1D("h_ExpUnc_8", "", nMassBin, MassBinEdges);
		this->h_TheoUnc_8 = new TH1D("h_TheoUnc_8", "", nMassBin, MassBinEdges);
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t RelTotUnc = AbsUnc_DiffXSec[i] / DiffXSec[i];
			Double_t RelTheoUnc = RelTheoUnc_Percent[i] * 0.01;

			if( RelTotUnc < RelTheoUnc )
				printf("[%02d-th bin] total unc. < theo. unc ... %.3lf%% < %.3lf%%\n", i_bin, RelTotUnc*100, RelTheoUnc*100 );
			Double_t RelExpUnc = sqrt( RelTotUnc*RelTotUnc - RelTheoUnc*RelTheoUnc );

			this->h_ExpUnc_8->SetBinContent(i_bin, RelExpUnc );
			this->h_ExpUnc_8->SetBinError(i_bin, 0 );

			this->h_TheoUnc_8->SetBinContent(i_bin, RelTheoUnc );
			this->h_TheoUnc_8->SetBinError(i_bin, 0 );
		}

		Print_Histogram( this->h_ExpUnc_8 );

		// -- assign the error on normalized cross section: quad.sum of exp. and theo -- //
		this->NormXSec_AssignError( this->h_NormXSec_8, this->h_ExpUnc_8, this->h_TheoUnc_8 );
	}

	void Get_Histogram_Theory_8eV_NNPDF()
	{
		this->TheoType_8TeV = "NNPDF 3.0";
		//////////////////////////////////
		// -- set theoretical values -- //
		//////////////////////////////////
		TFile *f_input_theo_8 = TFile::Open(this->AnalyzerPath+"/TheoryValues/NNLO_NNPDF30_8TeV_forDoubleRatio/ROOTFile_DY_FEWZ_NNLO_NNPDF30_nnlo_as_0118.root");
		f_input_theo_8->cd();
		TH1D* h_AbsXSec =  (TH1D*)f_input_theo_8->Get("h_Xsec")->Clone(); // -- just sigma, not dsigma/dm -- //
		Double_t ZpeakXSec_NNLO_8 = h_AbsXSec->Integral(10, 22); // -- from 60 to 120 GeV -- //
		Double_t ZpeakXSecErr_NNLO_8 = 0; // -- not used yet -- //
		this->h_ZpeakXSec_NNLO_8 = new TH1D("h_ZpeakXSec_NNLO_8", "", 1, 0, 1);
		this->h_ZpeakXSec_NNLO_8->SetBinContent( 1, ZpeakXSec_NNLO_8 );
		this->h_ZpeakXSec_NNLO_8->SetBinError( 1, ZpeakXSecErr_NNLO_8 );

		this->h_DiffXSec_NNLO_8 = (TH1D*)f_input_theo_8->Get("h_DiffXsec")->Clone("h_DiffXSec_NNLO_8");

		// -- calculate ratio (normalized x-sec) -- //
		this->h_NormXSec_NNLO_8 = (TH1D*)this->h_DiffXSec_NNLO_8->Clone("h_NormXSec_NNLO_8");
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t DiffXSec = this->h_DiffXSec_NNLO_8->GetBinContent(i_bin);
			Double_t NormXSec = (DiffXSec) / ZpeakXSec_NNLO_8;

			this->h_NormXSec_NNLO_8->SetBinContent( i_bin, NormXSec );

			Double_t DiffXSecAbsError = this->h_DiffXSec_NNLO_8->GetBinError(i_bin);
			Double_t NormXSecAbsError = DiffXSecAbsError / ZpeakXSec_NNLO_8;

			this->h_NormXSec_NNLO_8->SetBinError( i_bin, NormXSecAbsError );
		}

		Print_Histogram( this->h_NormXSec_NNLO_8 );
	}

	void Get_Histogram_Theory_8eV_CT10()
	{
		this->TheoType_8TeV = "CT10";
		//////////////////////////////////
		// -- set theoretical values -- //
		//////////////////////////////////
		TFile *f_input_theo_8 = TFile::Open("./Input_8TeV/1Dabsxsec_NNLO_CTEQ12NNLO.root"); f_input_theo_8->cd();
		TH1D* h_AbsXSec =  (TH1D*)f_input_theo_8->Get("invm_FEWZ")->Clone(); // -- just sigma, not dsigma/dm -- //

		// -- Z-peak - //
		Double_t ZpeakXSec_NNLO_8 = h_AbsXSec->Integral(10, 22); // -- from 60 to 120 GeV -- //
		Double_t ZpeakXSecErr_NNLO_8 = 0; // -- not used yet -- //
		this->h_ZpeakXSec_NNLO_8 = new TH1D("h_ZpeakXSec_NNLO_8", "", 1, 0, 1);
		this->h_ZpeakXSec_NNLO_8->SetBinContent( 1, ZpeakXSec_NNLO_8 );
		this->h_ZpeakXSec_NNLO_8->SetBinError( 1, ZpeakXSecErr_NNLO_8 );

		// -- change from x-sec to diff.x-sec -- //
		h_AbsXSec = this->Rebin_MassBin( h_AbsXSec );
		this->h_DiffXSec_NNLO_8 = this->GetDiffXSec_FromAbsXSec( h_AbsXSec, "h_DiffXSec_NNLO_8" );

		// -- calculate ratio (normalized x-sec) -- //
		this->h_NormXSec_NNLO_8 = (TH1D*)this->h_DiffXSec_NNLO_8->Clone("h_NormXSec_NNLO_8");

		// -- assign error -- //
		TFile *f_input_theoUnc_8 = TFile::Open("./Input_8TeV/pdfu.root");
		TH1D* h_PDF = (TH1D*)f_input_theoUnc_8->Get("pdfu")->Clone(); h_PDF->Scale( 0.01 );

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t DiffXSec = this->h_DiffXSec_NNLO_8->GetBinContent(i_bin);
			Double_t NormXSec = (DiffXSec) / ZpeakXSec_NNLO_8;

			this->h_NormXSec_NNLO_8->SetBinContent( i_bin, NormXSec );

			Double_t DiffXSecAbsError = this->h_DiffXSec_NNLO_8->GetBinError(i_bin);
			Double_t NormXSecAbsError = DiffXSecAbsError / ZpeakXSec_NNLO_8;

			Double_t PDFError = h_PDF->GetBinContent(i_bin);
			Double_t PDFAbsError = PDFError * NormXSec;

			Double_t TotError = sqrt( NormXSecAbsError*NormXSecAbsError + 0.04*PDFAbsError*PDFAbsError); // -- what is 0.04? ... copy & paste it from 8TeV code (DYPackage/test/ShapeR/plot_dratio.C)

			this->h_NormXSec_NNLO_8->SetBinError( i_bin, TotError );
		}

		Print_Histogram( this->h_NormXSec_NNLO_8 );
	}

	Double_t Calc_ZpeakXSec_FromDiffXSec( TH1D* h_DiffXSec )
	{
		Double_t ZpeakXSec = 0;

		Int_t nBin = h_DiffXSec->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t BinCenter = h_DiffXSec->GetBinCenter(i_bin);

			if( BinCenter > 60 && BinCenter < 120 )
			{
				Double_t BinWidth = h_DiffXSec->GetBinWidth(i_bin);

				Double_t XSec = h_DiffXSec->GetBinContent(i_bin) * BinWidth;

				ZpeakXSec += XSec;
			}
		}

		return ZpeakXSec;
	}

	void NormXSec_AssignError( TH1D* h_NormXSec, TH1D* h_ExpUnc, TH1D* h_TheoUnc )
	{
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t NormXSec = h_NormXSec->GetBinContent(i_bin);

			Double_t ExpUncRel = h_ExpUnc->GetBinContent(i_bin);
			Double_t TheoUncRel = h_TheoUnc->GetBinContent(i_bin);
			Double_t TotUncRel = sqrt( ExpUncRel*ExpUncRel + TheoUncRel*TheoUncRel );

			h_NormXSec->SetBinError( i_bin, NormXSec * TotUncRel );
		}

		printf("Error of normalized cross section is assigned\n");
		Print_Histogram( h_NormXSec );
	}

	TH1D* GetDiffXSec_FromAbsXSec( TH1D* h_AbsXSec, TString Name )
	{
		TH1D* h_DiffXSec = (TH1D*)h_AbsXSec->Clone( Name );
		Int_t nBin = h_AbsXSec->GetNbinsX();
		for(Int_t i=0 ; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t BinWidth = h_AbsXSec->GetBinWidth(i_bin);
			Double_t AbsXSec = h_AbsXSec->GetBinContent(i_bin);
			Double_t AbsXSecErr = h_AbsXSec->GetBinError(i_bin);

			Double_t DiffXSec = AbsXSec / BinWidth;
			Double_t DiffXSecErr = 	AbsXSecErr / BinWidth;

			h_DiffXSec->SetBinContent(i_bin, DiffXSec );
			h_DiffXSec->SetBinError(i_bin, DiffXSecErr );
		}

		return h_DiffXSec;
	}

	void DrawCanvas_AllUnc( TH1D* h_ExpUnc, TH1D* h_TheoUnc )
	{
		TString HistName = h_ExpUnc->GetName();
		TString Type = "";
		if( HistName.Contains("_8") )
			Type = "8TeV";
		else if (HistName.Contains("_13") )
			Type = "13TeV";

		TCanvas *c = new TCanvas();
		SetCanvas_Square( c, "c_AllUnc_"+Type+"_"+this->TStr_Channel );
		c->cd();
		c->SetLogx();
		c->SetLogy();

		SetHist_Color( h_ExpUnc, kRed );
		SetHist_Color( h_TheoUnc, kBlue+1 );

		h_ExpUnc->Draw("HISTLPSAME");
		h_TheoUnc->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( h_ExpUnc, "m [GeV]", "Uncertainty" );
		h_ExpUnc->GetXaxis()->SetNoExponent();
		h_ExpUnc->GetXaxis()->SetMoreLogLabels();

		h_ExpUnc->GetYaxis()->SetRangeUser(1e-3, 3);
		h_ExpUnc->GetYaxis()->SetNoExponent( kFALSE );

		TLegend *legend = new TLegend(0.15, 0.70, 0.50, 0.95);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		legend->AddEntry( h_ExpUnc, "Stat. + (Exp.) Syst." );
		legend->AddEntry( h_TheoUnc, "Theory" );
		legend->Draw();

		TLatex latex;
		this->DrawLatex_EachChannel( latex, Type );

		c->SaveAs(".pdf");
	}

	void DrawCanvas_NormXSec_Data_vs_Theory( TH1D* h_data, TH1D* h_theory )
	{
		TString HistName = h_data->GetName();
		TString Type = "";
		if( HistName.Contains("_8") )
			Type = "8TeV";
		else if( HistName.Contains("_13") )
			Type = "13TeV";

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		SetCanvas_Ratio( c, "c_NormXSec_Data_vs_Theory_"+Type+"_"+this->TStr_Channel, TopPad, BottomPad, kTRUE, kTRUE );
		c->cd();

		SetHist_Color( h_data, kBlack );
		SetHist_Color( h_theory, kRed );

		TopPad->cd();
		h_data->Draw("EPSAME");
		h_theory->Draw("EPSAME");

		// SetHistFormat_TopPad( h_data, "#frac{1}{#sigma_{Z}} #frac{d#sigma}{dm}" );
		SetHistFormat_TopPad( h_data, "(1/#sigma_{Z}) d#sigma/dm" );
		h_data->GetYaxis()->SetRangeUser( 1e-10, 1 );
		h_data->GetYaxis()->SetNoExponent( kFALSE );

		TLegend *legend = new TLegend(0.15, 0.35, 0.50, 0.50);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		TString TheoType = "";
		if( Type == "8TeV" ) TheoType = this->TheoType_8TeV;
		else if( Type == "13TeV" ) TheoType = this->TheoType_13TeV;
		legend->AddEntry( h_data, "Data" );
		legend->AddEntry( h_theory, "Theory (NNLO, "+TheoType+")" );
		legend->Draw();

		TLatex latex;
		this->DrawLatex_EachChannel( latex, Type );

		BottomPad->cd();
		TH1D* h_ratio = (TH1D*)h_data->Clone("h_ratio");
		h_data->Sumw2(); h_theory->Sumw2();
		h_ratio->Divide( h_data, h_theory );
		h_ratio->Draw("EPSAME");

		SetHistFormat_BottomPad( h_ratio, "m [GeV]", "Data/Theory", 0.41, 1.59 );
		h_ratio->GetXaxis()->SetNoExponent();
		h_ratio->GetXaxis()->SetMoreLogLabels();

		TF1 *line;
		DrawLine( line );

		c->SaveAs(".pdf");
	}

	void DrawCanvas_ExpUnc_13TeV( TH1D* h_Stat, TH1D* h_DetRes, TH1D* h_Bkg, TH1D* h_EffSF, TH1D* h_FSR, TH1D* h_ExpUnc )
	{
		TCanvas *c;
		SetCanvas_Square( c, "c_ExpUnc_13TeV_"+this->TStr_Channel );
		c->cd();
		c->SetLogx();
		c->SetLogy();

		SetHist_Color( h_Stat, kBlack );
		SetHist_Color( h_DetRes, kBlue+1 );
		SetHist_Color( h_Bkg, kGreen+1 );
		SetHist_Color( h_EffSF, kOrange+1 );
		SetHist_Color( h_FSR, kViolet );
		SetHist_Color( h_ExpUnc, kRed );

		h_Stat->Draw("LPSAME");
		h_DetRes->Draw("LPSAME");
		h_Bkg->Draw("HISTLPSAME");
		h_EffSF->Draw("LPSAME");
		h_FSR->Draw("LPSAME");
		h_ExpUnc->Draw("LPSAME");

		SetHistFormat_SinglePad( h_Stat, "m [GeV]", "Uncertainty" );
		h_Stat->GetXaxis()->SetNoExponent();
		h_Stat->GetXaxis()->SetMoreLogLabels();

		h_Stat->GetYaxis()->SetRangeUser(1e-4, 2);

		TLegend *legend = new TLegend(0.15, 0.70, 0.50, 0.95);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		legend->AddEntry( h_ExpUnc, "Stat. + Syst." );
		legend->AddEntry( h_Stat, "Stat." );
		legend->AddEntry( h_DetRes, "Detector resolution" );
		legend->AddEntry( h_Bkg, "Background" );
		legend->AddEntry( h_EffSF, "Eff. SF" );
		legend->AddEntry( h_FSR, "FSR" );
		legend->Draw();

		TLatex latex;
		this->DrawLatex_EachChannel( latex, "13TeV" );

		c->SaveAs(".pdf");
	}

	void DrawCanvas_ExpUnc_8TeV( TH1D* h_Stat, TH1D* h_Unfold, TH1D* h_Bkg, TH1D* h_EffSF, TH1D* h_FSR, TH1D* h_ExpUnc )
	{
		TCanvas *c;
		SetCanvas_Square( c, "c_ExpUnc_8TeV_"+this->TStr_Channel );
		c->cd();
		c->SetLogx();
		c->SetLogy();

		SetHist_Color( h_Stat, kBlack );
		SetHist_Color( h_Unfold, kBlue+1 );
		SetHist_Color( h_Bkg, kGreen+1 );
		SetHist_Color( h_EffSF, kOrange+1 );
		SetHist_Color( h_FSR, kViolet );
		SetHist_Color( h_ExpUnc, kRed );

		h_Stat->Draw("LPSAME");
		h_Unfold->Draw("LPSAME");
		h_Bkg->Draw("HISTLPSAME");
		h_EffSF->Draw("LPSAME");
		h_FSR->Draw("LPSAME");
		h_ExpUnc->Draw("LPSAME");

		SetHistFormat_SinglePad( h_Stat, "m [GeV]", "Uncertainty" );
		h_Stat->GetXaxis()->SetNoExponent();
		h_Stat->GetXaxis()->SetMoreLogLabels();

		h_Stat->GetYaxis()->SetRangeUser(1e-4, 2);

		TLegend *legend = new TLegend(0.15, 0.70, 0.50, 0.95);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		legend->AddEntry( h_ExpUnc, "Stat. + Syst." );
		legend->AddEntry( h_Stat, "Stat." );
		legend->AddEntry( h_Unfold, "Unfolding" );
		legend->AddEntry( h_Bkg, "Background" );
		legend->AddEntry( h_EffSF, "Eff. SF" );
		legend->AddEntry( h_FSR, "FSR" );
		legend->Draw();

		TLatex latex;
		this->DrawLatex_EachChannel( latex, "8TeV" );

		c->SaveAs(".pdf");
	}

	TH1D* NormXSec_CentralValue( TH1D* h_DiffXSec, TH1D* h_ZpeakXSec, TString Name )
	{
		Double_t XSec_Zpeak = h_ZpeakXSec->GetBinContent(1);

		TH1D* h_ratio = (TH1D*)h_DiffXSec->Clone(Name);
		Int_t nBin = h_DiffXSec->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			h_ratio->SetBinContent(i_bin, h_DiffXSec->GetBinContent(i_bin) / XSec_Zpeak);
			h_ratio->SetBinError(i_bin, 0); // -- will be set later -- //
		}

		return h_ratio;
	}

	TH1D* NormXSec_CentralValue_Error( TH1D* h_DiffXSec, TH1D* h_ZpeakXSec, TString Name )
	{
		Double_t XSec_Zpeak = h_ZpeakXSec->GetBinContent(1);

		TH1D* h_ratio = (TH1D*)h_DiffXSec->Clone(Name);
		Int_t nBin = h_DiffXSec->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			h_ratio->SetBinContent(i_bin, h_DiffXSec->GetBinContent(i_bin) / XSec_Zpeak);
			h_ratio->SetBinError(i_bin, h_DiffXSec->GetBinError(i_bin) / XSec_Zpeak);
		}

		return h_ratio;
	}

	TH1D* Rebin_MassBin( TH1D* h )
	{
		return (TH1D*)h->Rebin(nMassBin, h->GetName(), MassBinEdges);
	}

	TH1D* Rebin_DiffXSec( TH1D* h_DiffXSec )
	{	
		// -- change from Diff.x-section to x-section -- //
		TH1D* h_XSec = (TH1D*)h_DiffXSec->Clone();
		Int_t nBin = h_DiffXSec->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t DiffXSec = h_DiffXSec->GetBinContent(i_bin);
			Double_t Error = h_DiffXSec->GetBinError(i_bin); // -- even though this error will not be used .. -- //
			Double_t BinWidth = h_DiffXSec->GetBinWidth(i_bin);

			h_XSec->SetBinContent( i_bin, DiffXSec*BinWidth );
			h_XSec->SetBinError( i_bin, Error*BinWidth );
		}

		// -- rebin -- //
		h_XSec = this->Rebin_MassBin( h_XSec );

		TH1D* h_DiffXSec_Rebin = (TH1D*)h_XSec->Clone();

		// -- change x-section to diff.x-section -- //
		Int_t nBin2 = h_DiffXSec_Rebin->GetNbinsX();
		for(Int_t i=0; i<nBin2; i++)
		{
			Int_t i_bin = i+1;
			Double_t BinWidth = h_XSec->GetBinWidth(i_bin);
			Double_t DiffXSec = h_XSec->GetBinContent(i_bin) / BinWidth;
			Double_t Error_DiffXSec = h_XSec->GetBinError(i_bin) / BinWidth;

			h_DiffXSec_Rebin->SetBinContent(i_bin, DiffXSec);
			h_DiffXSec_Rebin->SetBinError(i_bin, Error_DiffXSec);
		}

		return h_DiffXSec_Rebin;
	}

	TH1D* MakeHistogram_QuadSum( vector<TH1D*> vec_Hist, TString Name )
	{
		Int_t nHist = (Int_t)vec_Hist.size();
		TH1D* h_QuadSum = (TH1D*)vec_Hist[0]->Clone(Name);

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t QuadSum = 0;
			for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			{
				Double_t value = vec_Hist[i_hist]->GetBinContent(i_bin);
				QuadSum = QuadSum + value*value;
			}
			QuadSum = sqrt( QuadSum );
			h_QuadSum->SetBinContent( i_bin, QuadSum );
			h_QuadSum->SetBinError( i_bin, 0 );
		}
		return h_QuadSum;
	}

	void PutIntoVector()
	{
		/////////////////
		// -- 8 TeV -- //
		/////////////////
		// -- experimental results -- //
		this->vec_Hist.push_back(this->h_ZpeakXSec_8);
		this->vec_Hist.push_back(this->h_DiffXSec_8);
		this->vec_Hist.push_back(this->h_NormXSec_8);
		this->vec_Hist.push_back(this->h_ExpUnc_8);
		this->vec_Hist.push_back(this->h_TheoUnc_8);

		// -- theory -- //
		this->vec_Hist.push_back(this->h_ZpeakXSec_NNLO_8);
		this->vec_Hist.push_back(this->h_DiffXSec_NNLO_8);
		this->vec_Hist.push_back(this->h_NormXSec_NNLO_8);

		/////////////////
		// -- 13 TeV -- //
		/////////////////
		// -- experimental results -- //
		this->vec_Hist.push_back(this->h_ZpeakXSec_13);
		this->vec_Hist.push_back(this->h_DiffXSec_13);
		this->vec_Hist.push_back(this->h_NormXSec_13);
		this->vec_Hist.push_back(this->h_ExpUnc_13);
		this->vec_Hist.push_back(this->h_TheoUnc_13);

		// -- theory -- //
		this->vec_Hist.push_back(this->h_ZpeakXSec_NNLO_13);
		this->vec_Hist.push_back(this->h_DiffXSec_NNLO_13);
		this->vec_Hist.push_back(this->h_NormXSec_NNLO_13);
	}

	void DrawLatex_EachChannel( TLatex &latex, TString CMEnergy )
	{
		if( this->TStr_Channel == "LL" )
		{
			if( CMEnergy == "13TeV" )
			{
				latex.DrawLatexNDC( 0.55, 0.96, "#font[42]{#scale[0.8]{2.3 fb^{-1} (ee)}}");
				latex.DrawLatexNDC( 0.75, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (#mu#mu)}}");
				latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
				latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

			}
			else if( CMEnergy == "8TeV" )
			{
				latex.DrawLatexNDC( 0.68, 0.96, "#font[42]{#scale[0.8]{19.7 fb^{-1} (ee, #mu#mu)}}");
				latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
				latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
			}
		}
		else if( this->TStr_Channel == "MM" )
		{
			if( CMEnergy == "13TeV" )
				Latex_Preliminary( latex, 2.8, 13 );
			else if( CMEnergy == "8TeV" )
				Latex_Preliminary( latex, 19.7, 8 );
		}
	}

};