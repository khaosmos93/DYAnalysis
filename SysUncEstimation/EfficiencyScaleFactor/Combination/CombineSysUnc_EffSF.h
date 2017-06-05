#include <Include/PlotTools.h>

class CombineTool
{
public:
	TH1D* h_RelUnc_TnP;
	TH1D* h_RelUnc_Bin;
	TH1D* h_RelUnc_Syst;
	TH1D* h_RelUnc_Stat;
	TH1D* h_RelUnc_Tot;

	CombineTool()
	{
		this->Get_Histogram();

	}

	void Draw_SummaryPlot()
	{
		HistInfo* Hist_TnP = new HistInfo( kGreen+2, "Syst. (Tag & Probe)", this->h_RelUnc_TnP );
		HistInfo* Hist_Bin = new HistInfo( kOrange+2, "Syst. (Binning choice)", this->h_RelUnc_Bin );
		HistInfo* Hist_Stat = new HistInfo( kBlack, "Stat.", this->h_RelUnc_Stat );
		HistInfo* Hist_Syst = new HistInfo( kBlue, "Total Syst.", this->h_RelUnc_Syst );
		HistInfo* Hist_Tot = new HistInfo( kRed, "Total (Stat. + Syst.)", this->h_RelUnc_Tot );

		TCanvas *c;
		TString CanvasName = "./Local/c_RelUnc_TnP";
		SetCanvas_Square( c, CanvasName, 1, 0 );

		c->cd();
		Hist_TnP->Draw("HISTLPSAME");
		Hist_Bin->Draw("HISTLPSAME");
		Hist_Stat->Draw("HISTLPSAME");
		Hist_Syst->Draw("HISTLPSAME");
		Hist_Tot->Draw("HISTLPSAME");

		SetHistFormat_SinglePad( Hist_TnP->h, "m [GeV]", "Unc. (%)" );
		Hist_TnP->h->GetYaxis()->SetRangeUser(0, 7);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.70, 0.50, 0.95 );
		Hist_Stat->AddToLegend( legend );
		Hist_TnP->AddToLegend( legend );
		Hist_Bin->AddToLegend( legend );
		Hist_Syst->AddToLegend( legend );
		Hist_Tot->AddToLegend( legend );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );
		latex.DrawLatexNDC( 0.64, 0.915, "#font[42]{#scale[0.8]{Unc. from Eff. SF}}");

		c->SaveAs(".pdf");
	}

	void Save( TFile *f_output )
	{
		this->h_RelUnc_TnP->SetName("h_RelSysUnc_TagProbe");
		this->h_RelUnc_Bin->SetName("h_RelSysUnc_Binning");
		this->h_RelUnc_Stat->SetName("h_RelUnc_Stat");
		this->h_RelUnc_Syst->SetName("h_RelUnc_Syst");
		this->h_RelUnc_Tot->SetName("h_RelSysUnc_Tot");

		this->h_RelUnc_TnP->Write();
		this->h_RelUnc_Bin->Write();
		this->h_RelUnc_Stat->Write();
		this->h_RelUnc_Syst->Write();
		this->h_RelUnc_Tot->Write();
	}

protected:
	void Get_Histogram()
	{
		TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
		TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");

		TString FileName_TnP = AnalyzerPath + "/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly/ROOTFile_RelUnc_TnP_FromSystSource.root";
		TString FileName_Bin = ROOTFilePath + "/ROOTFile_SysUnc_EffSF_BinningChoice.root";
		TString FileName_Stat = AnalyzerPath + "/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/StatOnly/ROOTFile_SysUnc_EffSF_TagProbe_UnbinnedFit.root";

		TString HistName_TnP = "h_RelUnc_Total";
		TString HistName_Bin = "h_RelUnc_BinningChoice_Percent";
		TString HistName_Stat = "h_RMS";

		this->h_RelUnc_TnP = Get_Hist( FileName_TnP, HistName_TnP ); this->h_RelUnc_TnP->Scale( 100 );
		this->h_RelUnc_Bin = Get_Hist( FileName_Bin, HistName_Bin );
		this->h_RelUnc_Stat = Get_Hist( FileName_Stat, HistName_Stat ); this->h_RelUnc_Stat->Scale( 100 );
		this->h_RelUnc_Syst = QuadSum_NoError( this->h_RelUnc_TnP, this->h_RelUnc_Bin );
		this->h_RelUnc_Tot = QuadSum_NoError( this->h_RelUnc_Stat, this->h_RelUnc_Syst );
	}
};