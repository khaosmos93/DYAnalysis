#include <TH1D.h>
#include <TColor.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLatex.h>
#include <TFile.h>
#include <TPad.h>
#include <TF1.h>

#include <vector>


class HistInfo
{
public:
	TH1D* h;
	TH1D* h_ratio;

	TString FileName; // -- including path -- //
	TString HistName;
	TString LegendName;
	Int_t Color;

	HistInfo()
	{
		TH1::AddDirectory( kFALSE );
		this->Init();
	}

	void Set_FileName_HistName( TString _FileName, TString _HistName )
	{
		this->FileName = _FileName;
		this->HistName = _HistName;

		this->Get_Histogram();
	}

	void Set_Color( Int_t _Color )
	{
		this->Color = _Color;

		this->Set_Attributes();
	}

	void Set_LegendName( TString _LegendName )
	{
		this->LegendName = _LegendName;
	}

	void Calc_RatioHist( TH1D* h_Denom )
	{
		h->Sumw2();
		h_Denom->Sumw2();

		this->h_ratio = (TH1D*)this->h->Clone();
		h_ratio->Divide( h, h_Denom );
	}

protected:
	void Init()
	{
		this->FileName = "";
		this->HistName = "";
		this->Color = 0;
	}

	void Get_Histogram()
	{
		if( this->FileName == "" || this->HistName == "" )
		{
			printf( "[FileName, HistName] = [%s, %s] ... at least one of them is not set yet", this->FileName.Data(), this->HistName.Data() );
			h = NULL;
			return;
		}

		TFile *f_input = TFile::Open( this->FileName );
		f_input->cd();

		this->h = (TH1D*)f_input->Get( this->HistName )->Clone();

		f_input->Close();
	}

	void Set_Attributes()
	{
		this->h->SetStats(kFALSE);
		this->h->SetMinimum(1e-7);

		this->h->SetLineColor( this->Color );
		this->h->SetFillColorAlpha( kWhite, 0 );
		this->h->SetMarkerStyle( 20 );
		this->h->SetMarkerColor( this->Color );
	}

};

TH1D* Rebin_MassBinEdges( TH1D* h );
void SetFormat_TopPad( TH1D* h_format );
void SetFormat_BottomPad( TH1D* h_format );

void Comparison_VariousPDF()
{
	vector<HistInfo*> vec_HistInfo;

	HistInfo *Hist_NNPDF30 = new HistInfo();
	Hist_NNPDF30->Set_FileName_HistName("/share_home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/v20160426_1st_UpdateEffSF/ROOTFile_xSec_Theory.root", "h_DiffXsec_FEWZ_NNPDF_NNLO"); // -- before merging -- //
	Hist_NNPDF30->Set_Color(kRed);
	Hist_NNPDF30->Set_LegendName( "NNPDF 3.0" );
	Hist_NNPDF30->h = Rebin_MassBinEdges( Hist_NNPDF30->h );
	vec_HistInfo.push_back( Hist_NNPDF30 );

	HistInfo *Hist_CT14 = new HistInfo();
	Hist_CT14->Set_FileName_HistName("./CT14/ROOTFile_DY_FEWZ_NNLO_CT14nnlo.root", "h_DiffXsec");
	Hist_CT14->Set_Color(kBlack);
	Hist_CT14->Set_LegendName( "CT14" );
	Hist_CT14->Calc_RatioHist( Hist_NNPDF30->h );
	vec_HistInfo.push_back( Hist_CT14 );

	HistInfo *Hist_MMHT2014 = new HistInfo();
	Hist_MMHT2014->Set_FileName_HistName("./MMHT2014/ROOTFile_DY_FEWZ_NNLO_MMHT2014nnlo68cl.root", "h_DiffXsec");
	Hist_MMHT2014->Set_Color(kBlue);
	Hist_MMHT2014->Set_LegendName( "MMHT2014" );
	Hist_MMHT2014->Calc_RatioHist( Hist_NNPDF30->h );
	vec_HistInfo.push_back( Hist_MMHT2014 );

	HistInfo *Hist_HERAPDF15 = new HistInfo();
	Hist_HERAPDF15->Set_FileName_HistName("./HERAPDF/ROOTFile_DY_FEWZ_NNLO_HERAPDF15NNLO_EIG.root", "h_DiffXsec");
	Hist_HERAPDF15->Set_Color(kGreen+1);
	Hist_HERAPDF15->Set_LegendName( "HERAPDF15" );
	Hist_HERAPDF15->Calc_RatioHist( Hist_NNPDF30->h );
	vec_HistInfo.push_back( Hist_HERAPDF15 );

	HistInfo *Hist_ABM12LHC = new HistInfo();
	Hist_ABM12LHC->Set_FileName_HistName("./ABM12LHC/ROOTFile_DY_FEWZ_NNLO_abm12lhc_5_nnlo.root", "h_DiffXsec");
	Hist_ABM12LHC->Set_Color(kOrange+2);
	Hist_ABM12LHC->Set_LegendName( "ABM12LHC" );
	Hist_ABM12LHC->Calc_RatioHist( Hist_NNPDF30->h );
	vec_HistInfo.push_back( Hist_ABM12LHC );

	Int_t nHist = (Int_t)vec_HistInfo.size();

	//////////////////////////
	// -- Canvas setting -- //
	//////////////////////////
	TCanvas *c = new TCanvas("Comparison_VariousPDF", "", 800, 600);
	c->cd();
	
	///////////////////
	// -- Top Pad -- //
	///////////////////
	TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
	c1_1->Draw();
	c1_1->cd();
	c1_1->SetTopMargin(0.05);
	c1_1->SetLeftMargin(0.13);
	c1_1->SetRightMargin(0.045);
	c1_1->SetBottomMargin(0.3);
	c1_1->SetLogx();
	c1_1->SetLogy();

	TLegend *legend = new TLegend( 0.70, 0.70, 0.95, 0.95 );
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	for(Int_t i=0; i<nHist; i++)
	{
		if( i == 0 )
			SetFormat_TopPad( vec_HistInfo[i]->h );

		vec_HistInfo[i]->h->Draw("EPSAME");
		legend->AddEntry( vec_HistInfo[i]->h, vec_HistInfo[i]->LegendName );
	}

	legend->Draw();
	// -- latex should be drawn after drawing axis (even thouhgh it uses NDC) -- //
	TLatex latex;
	latex.DrawLatexNDC(0.85, 0.96, "#font[42]{#scale[0.8]{13 TeV}}");
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.21, 0.96, "#font[42]{#it{#scale[0.8]{Simulation}}}");

	latex.DrawLatexNDC( 0.15, 0.35, "#font[42]{#scale[0.8]{Z/#gamma*#rightarrow#mu#mu, QCD@NNLO & EWK@NLO}}" );

	//////////////////////
	// -- Bottom Pad -- //
	//////////////////////
	TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.3);
	c1_2->Draw();
	c1_2->cd();
	c1_2->SetLogx();
	c1_2->SetGridx();
	c1_2->SetGridy();

	c1_2->SetBottomMargin(0.4);
	c1_2->SetRightMargin(0.035);
	c1_2->SetLeftMargin(0.12);

	for(Int_t i=1; i<nHist; i++) // -- i starts from 1: NNPDF is not included -- //
	{
		if( i == 1 )
			SetFormat_BottomPad( vec_HistInfo[i]->h_ratio );

		vec_HistInfo[i]->h_ratio->Draw( "EPSAME" );
	}

	TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	f_line->SetLineColor(kRed);
	f_line->SetLineWidth(1);
	f_line->Draw("PSAME");

	// TH1D* h_format_ratio = (TH1D*)Hist_MMHT2014->h_ratio->Clone();
	// Hist_MMHT2014->h_ratio->Draw("EPSAME");
	// h_format_ratio->Draw("AXIS");


	// // -- flat line = 1.00 -- //
	// TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	// f_line->SetLineColor(kRed);
	// f_line->SetLineWidth(1);
	// f_line->Draw("APSAME");




	// -- Draw histograms -- //
	// Bool_t isFirst_TopPad = kTRUE;
	// Bool_t isFirst_BottomPad = kTRUE;
	// for( const auto histinfo : vec_HistInfo )
	// {
	// 	c1_1->cd();
	// 	histinfo->h->Draw( "EPSAME" );
	// 	legend->AddEntry( histinfo->h, histinfo->LegendName );

	// 	if( isFirst_TopPad )
	// 	{
	// 		SetFormat_TopPad( histinfo->h );
	// 		isFirst_TopPad = kFALSE;
	// 	}

	// 	if( histinfo->LegendName != "NNPDF 3.0")
	// 	{
	// 		c1_2->cd();
	// 		histinfo->h_ratio->Draw( "EPSAME" );

	// 		if( isFirst_BottomPad )
	// 		{
	// 			SetFormat_BottomPad( histinfo->h_ratio );
	// 			isFirst_BottomPad = kFALSE;
	// 		}
	// 	}
	// }



	// c1_2->cd();
	// TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	// f_line->SetLineColor(kRed);
	// f_line->SetLineWidth(1);
	// f_line->Draw("APSAME");

	c->SaveAs(".pdf");

	// TCanvas *c2 = new TCanvas("test", "", 800, 800);
	// c2->cd();
	// c2->SetLogx();
	// c2->SetLogy();
	// Hist_ABM12LHC->h->Draw("EPSAME");
	// c2->SaveAs(".pdf");
}

TH1D* Rebin_MassBinEdges( TH1D* h )
{
	const Int_t nMassBin = 15;
	Double_t MassBinEdges[nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1200, 1500, 2000, 3000};

	return (TH1D*)h->Rebin(nMassBin, h->GetName(), MassBinEdges);
}

void SetFormat_TopPad( TH1D* h_format )
{
	h_format->SetYTitle( "d#sigma/dm [pb/GeV]" );

	h_format->GetXaxis()->SetLabelSize(0.000);
	h_format->GetXaxis()->SetTitleSize(0.000);

	h_format->GetYaxis()->SetTitleSize(0.06);
	h_format->GetYaxis()->SetTitleOffset(1.1);
	h_format->GetYaxis()->SetLabelSize(0.045);
	h_format->SetMinimum(5e-8);
}

void SetFormat_BottomPad( TH1D* h_format )
{
	h_format->GetXaxis()->SetMoreLogLabels();
	h_format->GetXaxis()->SetNoExponent();
	h_format->GetXaxis()->SetTitle( "m (#mu#mu) [GeV]" );
	h_format->GetXaxis()->SetTitleOffset( 0.85 );
	h_format->GetXaxis()->SetTitleSize( 0.2 );
	h_format->GetXaxis()->SetLabelColor(1);
	h_format->GetXaxis()->SetLabelFont(42);
	h_format->GetXaxis()->SetLabelOffset(0.01);
	h_format->GetXaxis()->SetLabelSize(0.13);

	h_format->GetYaxis()->SetTitle( "Other PDFs/NNPDF" );
	h_format->GetYaxis()->SetTitleOffset( 0.5 );
	h_format->GetYaxis()->SetTitleSize( 0.1);
	
	h_format->GetYaxis()->SetRangeUser( 0.86, 1.14 ); // -- default -- //
	h_format->GetYaxis()->SetLabelSize( 0.10 );
}