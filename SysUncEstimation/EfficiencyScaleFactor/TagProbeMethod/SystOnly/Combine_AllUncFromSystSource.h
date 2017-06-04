#include <Include/PlotTools.h>

class CombineTool
{
public:
	vector< TString > vec_Type = {"sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"};

	TH1D* h_RelUnc_sgnChange;
	TH1D* h_RelUnc_bkgChange;
	TH1D* h_RelUnc_MassRange;
	TH1D* h_RelUnc_nBin;
	TH1D* h_RelUnc_TagPt;
	TH1D* h_RelUnc_Total;

	TFile *f_output;

	CombineTool()
	{

	}

	void OutputFile( TFile *_f_output )
	{
		this->f_output = _f_output;
	}

	void Set_Histograms()
	{
		this->h_RelUnc_sgnChange = this->Get_RelUncHist( "sgnChange" );
		this->h_RelUnc_bkgChange = this->Get_RelUncHist( "bkgChange" );
		this->h_RelUnc_MassRange = this->Take_LargerUnc( "M60to130", "60 < M < 130", "M70to120", "70 < M < 120" );
		this->h_RelUnc_TagPt = this->Take_LargerUnc( "TagPt20", "Tag P_{T} > 20", "TagPt24", "Tag P_{T} > 24" );
		this->h_RelUnc_nBin = this->Take_LargerUnc( "nBin30", "# bin = 30", "nBin50", "# bin = 50" );

		this->Make_TotalHist_QuadSum();
	}

	void Draw_SummaryPlot()
	{
		HistInfo *Hist_sgnChange = new HistInfo( kBlue, "Signal shape", this->h_RelUnc_sgnChange );
		HistInfo *Hist_bkgChange = new HistInfo( kGreen+2, "Bkg. shape", this->h_RelUnc_bkgChange );
		HistInfo *Hist_MassRange = new HistInfo( kOrange+1, "Mass range", this->h_RelUnc_MassRange );
		HistInfo *Hist_TagPt = new HistInfo( kViolet, "Tag P_{T}", this->h_RelUnc_TagPt );
		HistInfo *Hist_nBin = new HistInfo( kBlack, "# bins", this->h_RelUnc_nBin );
		HistInfo *Hist_Total = new HistInfo( kRed, "Total", this->h_RelUnc_Total );

		// -- convert to % -- //
		Hist_sgnChange->h->Scale( 100 );
		Hist_bkgChange->h->Scale( 100 );
		Hist_MassRange->h->Scale( 100 );
		Hist_TagPt->h->Scale( 100 );
		Hist_nBin->h->Scale( 100 );
		Hist_Total->h->Scale( 100 );

		TCanvas *c;
		TString CanvasName = "c_RelUnc_TnP_SystSource";
		SetCanvas_Square( c, CanvasName, 1, 0 );

		Hist_sgnChange->Draw("HISTLPSAME");
		Hist_bkgChange->Draw("HISTLPSAME");
		Hist_MassRange->Draw("HISTLPSAME");
		Hist_TagPt->Draw("HISTLPSAME");
		Hist_nBin->Draw("HISTLPSAME");
		Hist_Total->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( Hist_sgnChange->h, "m [GeV]", "Unc. (%)" );
		Hist_sgnChange->h->GetYaxis()->SetRangeUser(0, 1);

		TLegend *legend;
		SetLegend( legend, 0.70, 0.63, 0.95, 0.94 );
		Hist_sgnChange->AddToLegend( legend );
		Hist_bkgChange->AddToLegend( legend );
		Hist_MassRange->AddToLegend( legend );
		Hist_TagPt->AddToLegend( legend );
		Hist_nBin->AddToLegend( legend );
		Hist_Total->AddToLegend( legend );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );
		latex.DrawLatexNDC(0.15, 0.91, "#scale[0.8]{#font[42]{Unc. from TnP (syst. source)}}");

		c->SaveAs(".pdf");
	}

	void Save()
	{
		this->h_RelUnc_sgnChange->SetName("h_RelUnc_sgnChange");
		this->h_RelUnc_bkgChange->SetName("h_RelUnc_bkgChange");
		this->h_RelUnc_MassRange->SetName("h_RelUnc_MassRange");
		this->h_RelUnc_TagPt->SetName("h_RelUnc_TagPt");
		this->h_RelUnc_nBin->SetName("h_RelUnc_nBin");
		this->h_RelUnc_Total->SetName("h_RelUnc_Total");

		this->f_output->cd();
		this->h_RelUnc_sgnChange->Write();
		this->h_RelUnc_bkgChange->Write();
		this->h_RelUnc_MassRange->Write();
		this->h_RelUnc_TagPt->Write();
		this->h_RelUnc_nBin->Write();
		this->h_RelUnc_Total->Write();
	}

protected:
	void Make_TotalHist_QuadSum()
	{
		vector< TH1D* > vec_Hist = {
		this->h_RelUnc_sgnChange,
		this->h_RelUnc_bkgChange,
		this->h_RelUnc_MassRange,
		this->h_RelUnc_TagPt,
		this->h_RelUnc_nBin };

		this->h_RelUnc_Total = (TH1D*)vec_Hist[0]->Clone();
		Int_t nBin = this->h_RelUnc_Total->GetNbinsX();
		Int_t nHist = (Int_t)vec_Hist.size();

		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t TotalUnc = 0;
			for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			{
				Double_t Unc = vec_Hist[i_hist]->GetBinContent(i_bin);
				TotalUnc = TotalUnc + Unc*Unc;
			}

			TotalUnc = sqrt( TotalUnc );
			this->h_RelUnc_Total->SetBinContent(i_bin, TotalUnc);
			this->h_RelUnc_Total->SetBinError(i_bin, 0);
		}
	}

	TH1D* Get_RelUncHist( TString Type )
	{
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
		TString BasePath = AnalyzerPath + "/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly";
		TString ROOTFileName = TString::Format("%s/%s/ROOTFile_SysUnc_EffSF_TagProbe_UnbinnedFit.root", BasePath.Data(), Type.Data());
		TH1D* h_temp = Get_Hist( ROOTFileName, "h_RMS" );

		return h_temp;

	}

	TH1D* Take_LargerUnc( TString Type1, TString Legend1, TString Type2, TString Legend2 )
	{
		TH1D* h1 = this->Get_RelUncHist( Type1 );
		h1->SetName( "h_RelUnc_"+Type1 );
		TH1D* h2 = this->Get_RelUncHist( Type2 );
		h2->SetName( "h_RelUnc_"+Type2 );

		TH1D* h_RelUnc_Large = (TH1D*)h1->Clone();
		Int_t nBin = h1->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t unc1 = h1->GetBinContent(i_bin);
			Double_t unc2 = h2->GetBinContent(i_bin);
			Double_t unc = unc1 > unc2 ? unc1 : unc2; // -- take the larger one -- //

			h_RelUnc_Large->SetBinContent(i_bin, unc );
			h_RelUnc_Large->SetBinError(i_bin, 0 );
		}

		// -- Draw a canvas -- //
		HistInfo* Hist1 = new HistInfo( kBlue, Legend1 );
		Hist1->Set_Histogram( h1 );
		Hist1->Set();
		HistInfo *Hist2 = new HistInfo( kGreen+2, Legend2 );
		Hist2->Set_Histogram( h2 );
		Hist2->Set();
		HistInfo* Hist_total = new HistInfo( kRed, "Total" );
		Hist_total->Set_Histogram( h_RelUnc_Large );
		Hist_total->Set();

		// -- convert to % -- //
		Hist1->h->Scale( 100 );
		Hist2->h->Scale( 100 );
		Hist_total->h->Scale( 100 );

		TCanvas *c;
		TString CanvasName = TString::Format("c_RelUnc_%s_%s", Type1.Data(), Type2.Data());
		SetCanvas_Square( c, CanvasName, 1, 0 );

		Hist1->Draw("HISTLPSAME");
		Hist2->Draw("HISTLPSAME");
		Hist_total->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( Hist1->h, "m [GeV]", "Unc. (%)" );
		Hist1->h->GetYaxis()->SetRangeUser(0, 0.5);

		TLegend *legend;
		SetLegend( legend, 0.50, 0.75, 0.95, 0.95 );
		Hist1->AddToLegend( legend );
		Hist2->AddToLegend( legend );
		Hist_total->AddToLegend( legend );
		legend->Draw();


		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );

		c->SaveAs(".pdf");

		this->f_output->cd();
		h1->Write();
		h2->Write();

		return h_RelUnc_Large;
	}
};
