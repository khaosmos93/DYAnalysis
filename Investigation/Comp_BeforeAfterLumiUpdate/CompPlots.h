#include <Include/PlotTools.h>

class Tool_CompPlots
{
public:
	TH1D* h_before;
	TH1D* h_after;

	Double_t xMin;
	Double_t xMax;
	Double_t yMin;
	Double_t yMax;
	Double_t ratio_Min;
	Double_t ratio_Max;
	Bool_t isLogx;
	Bool_t isLogy;
	TString XTitle;
	TString YTitle;

	Tool_CompPlots()
	{

	}

	void Setup( TH1D* _h_before, TH1D* _h_after )
	{
		this->h_before = (TH1D*)_h_before->Clone();
		this->h_after = (TH1D*)_h_after->Clone();

		TString HistName = this->h_after->GetName();
		this->SetHistStyle( HistName );
	}

	void DrawCanvas()
	{
		HistInfo *Hist_before = new HistInfo( kGreen+2, "Before" );
		Hist_before->Set_Histogram( this->h_before );
		Hist_before->Set();
		Hist_before->h->SetMarkerSize(1);

		HistInfo *Hist_after = new HistInfo( kBlue+1, "After" );
		Hist_after->Set_Histogram( this->h_after );
		Hist_after->Set();
		Hist_after->h->SetMarkerSize(1);
		Hist_after->Calc_RatioHist_Denominator( Hist_before->h );

		TCanvas *c; TPad *TopPad; TPad *BottomPad;
		TString CanvasName = this->h_after->GetName();
		CanvasName.ReplaceAll("h_", "c_");

		SetCanvas_Ratio( c, CanvasName, TopPad, BottomPad, this->isLogx, this->isLogy );
		c->cd();
		TopPad->cd();

		Hist_before->h->Draw("EPSAME");
		Hist_after->h->Draw("EPSAME");

		SetHistFormat_TopPad(Hist_before->h, this->YTitle);
		Hist_before->h->GetXaxis()->SetRangeUser( this->xMin, this->xMax );
		Hist_before->h->GetYaxis()->SetRangeUser( this->yMin, this->yMax );

		TLegend *legend;
		SetLegend( legend );
		legend->AddEntry( Hist_before->h, Hist_before->LegendName );
		legend->AddEntry( Hist_after->h, Hist_after->LegendName );
		legend->Draw();

		TLatex latex;
		Latex_Preliminary( latex, 2.8, 13 );

		c->cd();
		BottomPad->cd();
		Hist_after->h_ratio->Draw("EPSAME");
		SetHistFormat_BottomPad( Hist_after->h_ratio, this->XTitle, "after/before", this->ratio_Min, this->ratio_Max );
		Hist_after->h_ratio->GetXaxis()->SetRangeUser( this->xMin, this->xMax );
		Hist_after->h_ratio->GetYaxis()->SetNdivisions( 505 );

		TF1 *f_line;
		DrawLine( f_line );

		c->SaveAs(".pdf");
	}
protected:
	void SetHistStyle( TString HistName )
	{
		if( HistName == "h_yield_OS_DataDrivenBkg" )
		{
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
			this->xMin = 15;
			this->xMax = 3000;
			this->yMin = 0.5;
			this->yMax = 1e7;
			this->ratio_Min = 0.9;
			this->ratio_Max = 1.1;
			this->XTitle = "m [GeV]";
			this->YTitle = "Entries per bin";
		}

		else if( HistName == "h_DiffXsec_FSRCorr" )
		{
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
			this->xMin = 15;
			this->xMax = 3000;
			this->yMin = 5e-8;
			this->yMax = 1e+3;
			this->ratio_Min = 0.9;
			this->ratio_Max = 1.1;
			this->XTitle = "m [GeV]";
			this->YTitle = "d#sigma/dm [pb/GeV]";
		}

		else if( HistName == "ttbar" )
		{
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
			this->xMin = 15;
			this->xMax = 3000;
			this->yMin = 0.05;
			this->yMax = 1e4;
			this->ratio_Min = 0.8;
			this->ratio_Max = 1.2;
			this->XTitle = "m [GeV]";
			this->YTitle = "Entries per bin";
		}

		else if( HistName == "dijet" )
		{
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
			this->xMin = 15;
			this->xMax = 3000;
			this->yMin = 0.05;
			this->yMax = 1e4;
			this->ratio_Min = 0.9;
			this->ratio_Max = 1.1;
			this->XTitle = "m [GeV]";
			this->YTitle = "Entries per bin";
		}

		else if( HistName == "h_mass_OS_Norm_WZ" )
		{
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
			this->xMin = 15;
			this->xMax = 3000;
			this->yMin = 0.05;
			this->yMax = 1e4;
			this->ratio_Min = 0.5;
			this->ratio_Max = 1.5;
			this->XTitle = "m [GeV]";
			this->YTitle = "Entries per bin";
		}

		else if( HistName == "h_mass_OS_Norm_ZZ" )
		{
			this->isLogx = kTRUE;
			this->isLogy = kTRUE;
			this->xMin = 15;
			this->xMax = 3000;
			this->yMin = 0.05;
			this->yMax = 1e4;
			this->ratio_Min = 0.5;
			this->ratio_Max = 1.5;
			this->XTitle = "m [GeV]";
			this->YTitle = "Entries per bin";
		}
	}


};
