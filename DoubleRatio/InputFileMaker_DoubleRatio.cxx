// #include <Include/PlotTools.h>
// #include <Include/PlotTools.h>
#include </Users/KyeongPil_Lee/Physics/Include/PlotTools.h>

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

	void InputMaker()
	{
		TH1::AddDirectory(kFALSE);
	}

	void Save( TFile *f_output )
	{
		this->PutIntoVector();

		Int_t nHist = (TH1D*)vec_Hist.size();
		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			vec_Hist[i_hist]->Write();
	}

	void Get_Histograms()
	{
		this->Get_Histograms_8TeV();
		this->Get_Histograms_13TeV();
	}

protected:

	void Get_Histograms_13TeV()
	{
		// -- 13 TeV, dimuon -- //
		// -- Z-peak -- //
		this->h_ZpeakXSec_13 = new TH1D("h_ZpeakXSec_13", "", 1, 0, 1 );
		h_ZpeakXSec_13->SetBinContent( 1, 1857 );
		h_ZpeakXSec_13->SetBinError( 1, sqrt(14*14 + 25*25) );

		// -- differential cross section -- //
		// TString Path_13 = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval";
		// TFile *f_input_13 = TFile::Open(Path_13+"/ROOTFile_DiffXSec_FullUnc.root");
		TFile *f_input_13 = TFile::Open("./ROOTFile_DiffXSec_FullUnc.root");

		TH1D* h_DiffXSec_13_beforeBin = (TH1D*)f_input_13->Get("h_DiffXsec_FSRCorr")->Clone("h_DiffXSec_13");
		this->h_DiffXSec_13 = this->Rebin_DiffXSec( h_DiffXSec_13_beforeBin );
		this->h_NormXSec_13 = this->NormXSec_CentralValue( this->h_DiffXSec_13, this->h_ZpeakXSec_13, "h_NormXSec_13" );

		// -- experimental uncertainties -- //
		vector<TH1D*> vec_UncHist;

		TH1D* h_Stat = (TH1D*)f_input_13->Get("h_RelStatUnc_Percent")->Clone("h_Stat"); h_Stat->Scale(0.01);
		h_Stat = this->Rebin_MassBin( h_Stat ); vec_UncHist.push_back( h_Stat );

		TH1D* h_EffSF = (TH1D*)f_input_13->Get("h_RelSysUnc_Eff.SF._Percent")->Clone("h_EffSF"); h_EffSF->Scale(0.01);
		h_EffSF = this->Rebin_MassBin( h_EffSF ); vec_UncHist.push_back( h_EffSF );
		
		TH1D* h_DetRes = (TH1D*)f_input_13->Get("h_RelSysUnc_Det.Res_Percent")->Clone("h_DetRes"); h_DetRes->Scale(0.01);
		h_DetRes = this->Rebin_MassBin( h_DetRes ); vec_UncHist.push_back( h_DetRes );
		
		TH1D* h_Bkg = (TH1D*)f_input_13->Get("h_RelSysUnc_Bkg.Est._Percent")->Clone("h_Bkg"); h_Bkg->Scale(0.01);
		h_Bkg = this->Rebin_MassBin( h_Bkg ); vec_UncHist.push_back( h_Bkg );
		
		TH1D* h_FSR = (TH1D*)f_input_13->Get("h_RelSysUnc_FSR_Percent")->Clone("h_FSR"); h_FSR->Scale(0.01);
		h_FSR = this->Rebin_MassBin( h_FSR ); vec_UncHist.push_back( h_FSR );

		this->h_ExpUnc_13 = this->MakeHistogram_QuadSum( vec_UncHist, "h_ExpUnc_13" );

		this->DrawCanvas_ExpUnc_13TeV( h_Stat, h_DetRes, h_Bkg, h_EffSF, h_FSR, this->h_ExpUnc_13 );

		// -- theoretical uncertainties -- //
		this->h_TheoUnc_13 = (TH1D*)f_input_13->Get("h_RelSysUnc_Acc._Percent")->Clone("h_TheoUnc_13");
		this->h_TheoUnc_13->Scale(0.01);
		this->h_TheoUnc_13 = this->Rebin_DiffXSec( this->h_TheoUnc_13 );

		// -- draw canvas: Exp.Unc vs. Theo. unc  -- //
		this->DrawCanvas_AllUnc( this->h_ExpUnc_13, this->h_TheoUnc_13 );

		this->NormXSec_AssignError( this->h_NormXSec_13, this->h_ExpUnc_13, this->h_TheoUnc_13 );

		// -- theory value -- //
		this->h_DiffXSec_NNLO_13 = (TH1D*)f_input_13->Get("h_DiffXsec_FEWZ_NNPDF_NNLO")->Clone("h_DiffXSec_NNLO_13");
		this->h_DiffXSec_NNLO_13 = this->Rebin_DiffXSec( this->h_DiffXSec_NNLO_13 );

		Double_t ZpeakXSec_NNLO_13 = this->Calc_ZpeakXSec( this->h_DiffXSec_NNLO_13 ); // -- from 60 to 120 GeV -- //
		Double_t ZpeakXSecErr_NNLO_13 = 0; // -- it is not used yet -- //

		this->h_ZpeakXSec_NNLO_13 = new TH1D("h_ZpeakXSec_NNLO_13", "", 1, 0, 1);
		this->h_ZpeakXSec_NNLO_13->SetBinContent( 1, ZpeakXSec_NNLO_13 );
		this->h_ZpeakXSec_NNLO_13->SetBinError( 1, ZpeakXSecErr_NNLO_13 );

		this->h_NormXSec_NNLO_13 = this->NormXSec_CentralValue_Error( this->h_DiffXSec_NNLO_13, this->h_ZpeakXSec_NNLO_13, "h_NormXSec_NNLO_13" );
		this->h_NormXSec_NNLO_13->SetName("h_NormXSec_NNLO_13");

		// -- comparison: data vs. theory -- //
		this->DrawCanvas_NormXSec_Data_vs_Theory( this->h_NormXSec_13, this->h_NormXSec_NNLO_13 );
	}

	void Get_Histograms_8TeV()
	{
		// -- Z-peak -- //
		this->h_ZpeakXSec_8 = new TH1D("h_ZpeakXSec_8", "", 1, 0, 1 );
		h_ZpeakXSec_8->SetBinContent( 1, 1135.4 );
		h_ZpeakXSec_8->SetBinError( 1, sqrt(11.2*11.2 + 25.1*25.1) );

		// -- differential & normalized cross section -- //
		TFile *f_input_8 = TFile::Open("absex_full_Bayesian.root"); f_input_8->cd();
		TH1D* h_DiffXSec_8_beforeBin = (TH1D*)f_input_8->Get("hxsec")->Clone("h_DiffXSec_8");
		this->h_DiffXSec_8 = this->Rebin_DiffXSec( h_DiffXSec_8_beforeBin );
		this->h_NormXSec_8 = this->NormXSec_CentralValue( this->h_DiffXSec_8, this->h_ZpeakXSec_8, "h_NormXSec_8" );

		// -- experimental uncertainties -- //
		vector<TH1D*> vec_UncHist;
		TFile *f_input_unc_8 = TFile::Open("muonSystAll_1D.root"); f_input_unc_8->cd();
		
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

		this->DrawCanvas_AllUnc( this->h_ExpUnc_8, this->h_TheoUnc_8);

		// -- assign the error on normalized cross section: quad.sum of exp. and theo -- //
		this->NormXSec_AssignError( this->h_NormXSec_8, this->h_ExpUnc_8, this->h_TheoUnc_8 );

		//////////////////////////////////
		// -- set theoretical values -- //
		//////////////////////////////////
		TFile *f_input_theo_8 = TFile::Open("1Dabsxsec_NNLO_CTEQ12NNLO.root"); f_input_theo_8->cd();
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
		TFile *f_input_theoUnc_8 = TFile::Open("pdfu.root");
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

		// -- comparison: data vs. theory -- //
		this->DrawCanvas_NormXSec_Data_vs_Theory( this->h_NormXSec_8, this->h_NormXSec_NNLO_8 );
	}

	Double_t Calc_ZpeakXSec( TH1D* h_DiffXSec )
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
		SetCanvas_Square( c, "c_AllUnc_"+Type );
		c->cd();
		c->SetLogx();
		c->SetLogy();

		SetHist_Color( h_ExpUnc, kRed );
		SetHist_Color( h_TheoUnc, kBlue+1 );

		h_ExpUnc->Draw("HISTLPSAME");
		h_TheoUnc->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( h_ExpUnc, "m (#mu#mu) [GeV]", "Uncertainty" );
		h_ExpUnc->GetXaxis()->SetNoExponent();
		h_ExpUnc->GetXaxis()->SetMoreLogLabels();

		h_ExpUnc->GetYaxis()->SetRangeUser(1e-3, 3);

		TLegend *legend = new TLegend(0.15, 0.70, 0.50, 0.95);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		legend->AddEntry( h_ExpUnc, "Stat. + (Exp.) Syst." );
		legend->AddEntry( h_TheoUnc, "Theory" );
		legend->Draw();

		TLatex latex;
		if( Type == "8TeV" )
			Latex_Preliminary( latex, 19.7, 8 );
		else if( Type == "13TeV" )
			Latex_Preliminary( latex, 2.8, 13 );

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
		SetCanvas_Ratio( c, "c_NormXSec_Data_vs_Theory_"+Type, TopPad, BottomPad, kTRUE, kTRUE );
		c->cd();

		SetHist_Color( h_data, kBlack );
		SetHist_Color( h_theory, kRed );

		TopPad->cd();
		h_data->Draw("EPSAME");
		h_theory->Draw("EPSAME");

		// SetHistFormat_TopPad( h_data, "#frac{1}{#sigma_{Z}} #frac{d#sigma}{dm}" );
		SetHistFormat_TopPad( h_data, "(1/#sigma_{Z}) d#sigma/dm" );
		h_data->GetYaxis()->SetRangeUser( 1e-10, 1 );

		TLegend *legend = new TLegend(0.15, 0.35, 0.50, 0.50);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		TString TheoType = "";
		if( Type == "8TeV" ) TheoType = "CT10";
		else if( Type == "13TeV" ) TheoType = "NNPDF 3.0";
		legend->AddEntry( h_data, "Data" );
		legend->AddEntry( h_theory, "Theory (NNLO, "+TheoType+")" );
		legend->Draw();

		TLatex latex;
		if( Type == "8TeV" )
			Latex_Preliminary( latex, 19.7, 8 );
		else if( Type == "13TeV" )
			Latex_Preliminary( latex, 2.8, 13 );

		BottomPad->cd();
		TH1D* h_ratio = (TH1D*)h_data->Clone("h_ratio");
		h_data->Sumw2(); h_theory->Sumw2();
		h_ratio->Divide( h_data, h_theory );
		h_ratio->Draw("EPSAME");

		SetHistFormat_BottomPad( h_ratio, "m (#mu#mu) [GeV]", "Data/Theory", 0.41, 1.59 );
		h_ratio->GetXaxis()->SetNoExponent();
		h_ratio->GetXaxis()->SetMoreLogLabels();

		TF1 *line;
		DrawLine( line );

		c->SaveAs(".pdf");
	}

	void DrawCanvas_ExpUnc_13TeV( TH1D* h_Stat, TH1D* h_DetRes, TH1D* h_Bkg, TH1D* h_EffSF, TH1D* h_FSR, TH1D* h_ExpUnc )
	{
		TCanvas *c;
		SetCanvas_Square( c, "c_ExpUnc_13TeV" );
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

		SetHistFormat_SinglePad( h_Stat, "m (#mu#mu) [GeV]", "Uncertainty" );
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
		Latex_Preliminary( latex, 2.8, 13 );

		c->SaveAs(".pdf");
	}

	void DrawCanvas_ExpUnc_8TeV( TH1D* h_Stat, TH1D* h_Unfold, TH1D* h_Bkg, TH1D* h_EffSF, TH1D* h_FSR, TH1D* h_ExpUnc )
	{
		TCanvas *c;
		SetCanvas_Square( c, "c_ExpUnc_8TeV" );
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

		SetHistFormat_SinglePad( h_Stat, "m (#mu#mu) [GeV]", "Uncertainty" );
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
		Latex_Preliminary( latex, 19.7, 8 );

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
			h_ratio->SetBinError(i_bin, h_DiffXSec->GetBinError(i_bin) / XSec_Zpeak); // -- will be set later -- //
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

};

void InputFileMaker_DoubleRatio()
{
	InputMaker *maker = new InputMaker();
	maker->Get_Histograms();

	TFile *f_ouptut = TFile::Open("ROOTFile_Input_DoubleRatio.root", "RECREATE");
	maker->Save( f_ouptut );
}