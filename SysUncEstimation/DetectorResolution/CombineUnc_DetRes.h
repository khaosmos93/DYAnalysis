#include <Include/PlotTools.h>

class CombineTool
{
public:
	// TString AnalyzerPath;
	TString ROOTFilePath;

	TH1D* h_MomCorr;
	TH1D* h_Unfold;
	TH1D* h_Unfold_MCSize;
	TH1D* h_Unfold_AltMC;
	TH1D* h_total;

	TH1D* h_Stat;
	TH1D* h_Syst;

	CombineTool()
	{
		this->ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		this->Get_Histogram();
	}

	void SummaryPlot()
	{
		HistInfo *Hist_MomCorr = new HistInfo( kGreen+2, "Momentum correction", this->h_MomCorr );
		HistInfo *Hist_Unfold = new HistInfo( kViolet , "Unfolding procedure", this->h_Unfold );
		HistInfo *Hist_total = new HistInfo( kRed, "Total", this->h_total );

		TString CanvasName = "c_RelUnc_DetRes";
		TCanvas *c;
		SetCanvas_Square( c, CanvasName, 1, 1 );

		c->cd();

		Hist_MomCorr->Draw("HISTLPSAME");
		Hist_Unfold->Draw("HISTLPSAME");
		Hist_total->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( Hist_MomCorr->h, "m [GeV]", "Unc. (%)");
		Hist_MomCorr->h->GetYaxis()->SetRangeUser( 1e-1, 200 );

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );
		latex.DrawLatexNDC( 0.16, 0.92, "#scale[0.6]{#font[42]{Unc. from detector resolution}}");

		TLegend *legend;
		SetLegend( legend, 0.16, 0.70, 0.50, 0.90 );
		Hist_MomCorr->AddToLegend( legend );
		Hist_Unfold->AddToLegend( legend );
		Hist_total->AddToLegend( legend );
		legend->Draw();

		c->SaveAs("./Local/"+CanvasName+".pdf");
	}

	void SummaryPlot_StatSyst()
	{
		HistInfo *Hist_Stat = new HistInfo( kBlack, "Statistical source(s)", this->h_Stat );
		HistInfo *Hist_Syst = new HistInfo( kBlue, "Systematic source(s)", this->h_Syst );
		HistInfo *Hist_total = new HistInfo( kRed, "Total", this->h_total );

		TString CanvasName = "c_RelUnc_DetRes_StatSyst";
		TCanvas *c;
		SetCanvas_Square( c, CanvasName, 1, 1 );

		c->cd();

		Hist_Stat->Draw("HISTLPSAME");
		Hist_Syst->Draw("HISTLPSAME");
		Hist_total->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( Hist_Stat->h, "m [GeV]", "Unc. (%)");
		Hist_Stat->h->GetYaxis()->SetRangeUser( 1e-1, 200 );

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );
		latex.DrawLatexNDC( 0.16, 0.92, "#scale[0.6]{#font[42]{Unc. from detector resolution}}");

		TLegend *legend;
		SetLegend( legend, 0.16, 0.70, 0.50, 0.90 );
		Hist_Stat->AddToLegend( legend );
		Hist_Syst->AddToLegend( legend );
		Hist_total->AddToLegend( legend );
		legend->Draw();

		c->SaveAs("./Local/"+CanvasName+".pdf");
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_MomCorr->SetName("h_RelSysUnc_MomCorr_Percent");
		this->h_MomCorr->Write();

		this->h_Unfold->SetName("h_RelSysUnc_Unfolding_Percent");
		this->h_Unfold->Write();

		this->h_total->SetName("h_RelSysUnc_DetRes_Percent");
		this->h_total->Write();

		this->h_Stat->SetName("h_RelUnc_Stat");
		this->h_Stat->Write();

		this->h_Syst->SetName("h_RelUnc_Syst");
		this->h_Syst->Write();
	}

protected:

	void Get_Histogram()
	{
		TString FileName_MomCorr = ROOTFilePath + "/ROOTFile_SysUnc_MomCorr.root";
		TString FileName_Unfold = ROOTFilePath + "/ROOTFile_SysUnc_Unfolding.root";
		
		this->h_MomCorr = Get_Hist( FileName_MomCorr, "h_SysUnc_Tot_Percent" );
		this->h_Unfold = Get_Hist( FileName_Unfold, "h_SysUnc_Tot_Percent" );
		this->h_Unfold_MCSize = Get_Hist( FileName_Unfold, "h_SysUnc_MCSize_Percent" );
		this->h_Unfold_AltMC = Get_Hist( FileName_Unfold, "h_RelSysUnc_AltMC_Percent" );

		this->h_total = QuadSum_NoError( this->h_MomCorr, this->h_Unfold );

		// -- stat. source: limiited MC statistics -- //
		this->h_Stat = (TH1D*)this->h_Unfold_MCSize->Clone();

		// -- syst. source: compare with the alternative MC & momentum correction -- //
		this->h_Syst = QuadSum_NoError( this->h_MomCorr, this->h_Unfold_AltMC );
	}

};
