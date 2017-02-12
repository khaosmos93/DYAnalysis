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

#include <Include/PlotTools.h>

// TH1D* Rebin_MassBinEdges( TH1D* h );
TH1D* Rebin_MassBinEdges_AfterMerge( TH1D* h );
TH1D* MergeLastTwoBins( TH1D* h );
Double_t QuadratureSum( Double_t a, Double_t b );
void Comparison_Data_vs_VariousPDF()
{
	HistInfo *Hist_Data = new HistInfo();
	Hist_Data->Set_FileName_ObjectName("/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval/ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr"); // -- before merging -- //
	Hist_Data->Set_Color(kBlack);
	Hist_Data->Set_LegendName( "Data" );
	Hist_Data->Set();
	Hist_Data->h = Rebin_MassBinEdges_AfterMerge( Hist_Data->h );

	vector<HistInfo*> vec_HistInfo;

	HistInfo *Hist_NNPDF30 = new HistInfo();
	Hist_NNPDF30->Set_FileName_ObjectName("/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/v20170106_1st_UpdateSyst_fromApproval/ROOTFile_xSec_Theory.root", "h_DiffXsec_FEWZ_NNPDF_NNLO"); // -- before merging -- //
	Hist_NNPDF30->Set_Color(kRed);
	Hist_NNPDF30->Set_LegendName( "NNPDF 3.0" );
	Hist_NNPDF30->Set();
	Hist_NNPDF30->h = Rebin_MassBinEdges_AfterMerge( Hist_NNPDF30->h );
	Hist_NNPDF30->Calc_RatioHist_Numerator( Hist_Data->h );
	vec_HistInfo.push_back( Hist_NNPDF30 );

	HistInfo *Hist_CT14 = new HistInfo();
	Hist_CT14->Set_FileName_ObjectName("./CT14/ROOTFile_DY_FEWZ_NNLO_CT14nnlo.root", "h_DiffXsec");
	Hist_CT14->Set_Color(kViolet);
	Hist_CT14->Set_LegendName( "CT14" );
	Hist_CT14->Set();
	Hist_CT14->h = MergeLastTwoBins( Hist_CT14->h );
	Hist_CT14->Calc_RatioHist_Numerator( Hist_Data->h );
	vec_HistInfo.push_back( Hist_CT14 );

	HistInfo *Hist_MMHT2014 = new HistInfo();
	Hist_MMHT2014->Set_FileName_ObjectName("./MMHT2014/ROOTFile_DY_FEWZ_NNLO_MMHT2014nnlo68cl.root", "h_DiffXsec");
	Hist_MMHT2014->Set_Color(kBlue);
	Hist_MMHT2014->Set_LegendName( "MMHT2014" );
	Hist_MMHT2014->Set();
	Hist_MMHT2014->h = MergeLastTwoBins( Hist_MMHT2014->h );
	Hist_MMHT2014->Calc_RatioHist_Numerator( Hist_Data->h );
	vec_HistInfo.push_back( Hist_MMHT2014 );

	HistInfo *Hist_HERAPDF15 = new HistInfo();
	Hist_HERAPDF15->Set_FileName_ObjectName("./HERAPDF/ROOTFile_DY_FEWZ_NNLO_HERAPDF15NNLO_EIG.root", "h_DiffXsec");
	Hist_HERAPDF15->Set_Color(kGreen+1);
	Hist_HERAPDF15->Set_LegendName( "HERAPDF15" );
	Hist_HERAPDF15->Set();
	Hist_HERAPDF15->h = MergeLastTwoBins( Hist_HERAPDF15->h );
	Hist_HERAPDF15->Calc_RatioHist_Numerator( Hist_Data->h );
	vec_HistInfo.push_back( Hist_HERAPDF15 );

	HistInfo *Hist_ABM12LHC = new HistInfo();
	Hist_ABM12LHC->Set_FileName_ObjectName("./ABM12LHC/ROOTFile_DY_FEWZ_NNLO_abm12lhc_5_nnlo.root", "h_DiffXsec");
	Hist_ABM12LHC->Set_Color(kOrange+2);
	Hist_ABM12LHC->Set_LegendName( "ABM12LHC" );
	Hist_ABM12LHC->Set();
	Hist_ABM12LHC->h = MergeLastTwoBins( Hist_ABM12LHC->h );
	Hist_ABM12LHC->Calc_RatioHist_Numerator( Hist_Data->h );
	vec_HistInfo.push_back( Hist_ABM12LHC );

	Int_t nHist = (Int_t)vec_HistInfo.size();

	for(Int_t i=0; i<nHist; i++)
	{
		//////////////////////////
		// -- Canvas setting -- //
		//////////////////////////
		TCanvas *c = new TCanvas(TString::Format("Comparison_Data_vs_VariousPDF_%02d", i), "", 800, 600);
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

		TLegend *legend = new TLegend( 0.70, 0.83, 0.95, 0.95 );
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		vec_HistInfo[i]->h->Draw("EPSAME");
		Hist_Data->h->Draw("EPSAME");

		SetHistFormat_TopPad( vec_HistInfo[i]->h, "d#sigma/dm [pb/GeV]" );
		vec_HistInfo[i]->h->GetYaxis()->SetRangeUser(5e-8, 1e-1);
		legend->AddEntry( Hist_Data->h, Hist_Data->LegendName );
		legend->AddEntry( vec_HistInfo[i]->h, vec_HistInfo[i]->LegendName );
		legend->Draw();

		// -- latex should be drawn after drawing axis (even thouhgh it uses NDC) -- //
		TLatex latex;
		latex.DrawLatexNDC(0.76, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (13 TeV)}}");
		latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
		latex.DrawLatexNDC(0.21, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

		latex.DrawLatexNDC( 0.15, 0.40, "#font[42]{#scale[0.8]{Z/#gamma*#rightarrow#mu#mu}}" );
		latex.DrawLatexNDC( 0.15, 0.35, "#font[42]{#scale[0.6]{Theory: QCD@NNLO & EWK@NLO (FEWZ)}}" );

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

		vec_HistInfo[i]->h_ratio->Draw( "EPSAME" );
		SetHistFormat_BottomPad( vec_HistInfo[i]->h_ratio, "m [GeV]", "Data/Theory", 0.31, 1.69 );

		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kAzure+1);
		f_line->SetLineWidth(1);
		f_line->Draw("PSAME");

		c->SaveAs(".pdf");
	}

	// -- Canvas for ratios -- //
	TCanvas *c2 = new TCanvas("RatioWithData_VariousPDF", "", 800, 600);
	c2->cd();
	c2->SetTopMargin(0.05);
	c2->SetLeftMargin(0.13);
	c2->SetRightMargin(0.045);
	c2->SetBottomMargin(0.13);
	c2->SetLogx();

	TLegend *legend = new TLegend( 0.70, 0.70, 0.95, 0.95 );
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	vector< TH1D* > vec_ratio;
	for(Int_t i=0; i<nHist; i++)
	{
		vec_ratio.push_back( (TH1D*)vec_HistInfo[i]->h_ratio->Clone() );
		vec_ratio[i]->Draw("EPSAME");

		legend->AddEntry( vec_ratio[i], vec_HistInfo[i]->LegendName );
	}
	legend->Draw();

	SetHistFormat_SinglePad( vec_ratio[0], "m [GeV]", "Data/Theory" );

	// -- latex should be drawn after drawing axis (even thouhgh it uses NDC) -- //
	TLatex latex;
	latex.DrawLatexNDC(0.76, 0.96, "#font[42]{#scale[0.8]{2.8 fb^{-1} (13 TeV)}}");
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
	latex.DrawLatexNDC(0.21, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

	latex.DrawLatexNDC( 0.16, 0.20, "#font[42]{#scale[0.8]{Z/#gamma*#rightarrow#mu#mu}}" );
	latex.DrawLatexNDC( 0.16, 0.15, "#font[42]{#scale[0.6]{Theory: QCD@NNLO & EWK@NLO (FEWZ)}}" );

	TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	f_line->SetLineColor(kAzure+1);
	f_line->SetLineWidth(1);
	f_line->Draw("PSAME");

	c2->SaveAs(".pdf");
}

// TH1D* Rebin_MassBinEdges( TH1D* h )
// {
// 	const Int_t nMassBin = 15;
// 	Double_t MassBinEdges[nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1200, 1500, 2000, 3000};

// 	return (TH1D*)h->Rebin(nMassBin, h->GetName(), MassBinEdges);
// }

TH1D* Rebin_MassBinEdges_AfterMerge( TH1D* h )
{
	const Int_t nMassBin = 13;
	Double_t MassBinEdges[nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

	return (TH1D*)h->Rebin(nMassBin, h->GetName(), MassBinEdges);
}


TH1D* MergeLastTwoBins( TH1D* h )
{
	// const Int_t nMassBin = 15;
	// Double_t MassBinEdges[nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1200, 1500, 2000, 3000};

	const Int_t nMassBin = 13;
	Double_t MassBinEdges[nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

	TString HistName = h->GetName();
	// TH1D* h_Merge = new TH1D(HistName, "", nMassBin, MassBinEdges);
	TH1D* h_Merge = (TH1D*)h->Clone(); // -- in order to maintain attributes -- //
	h_Merge = Rebin_MassBinEdges_AfterMerge( h_Merge );

	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t BinCenter = h->GetBinCenter(i_bin);

		if( BinCenter < 1000 )
		{
			Double_t value = h->GetBinContent(i_bin);
			Double_t error = h->GetBinError(i_bin);

			h_Merge->SetBinContent(i_bin, value);
			h_Merge->SetBinError(i_bin, error);
		}
	}

	Double_t XSec_M1000to1200 = h->GetBinContent(12) * h->GetBinWidth(12);
	Double_t XSecErr_M1000to1200 = h->GetBinError(12) * h->GetBinWidth(12);

	Double_t XSec_M1200to1500 = h->GetBinContent(13) * h->GetBinWidth(13);
	Double_t XSecErr_M1200to1500 = h->GetBinError(13) * h->GetBinWidth(13);

	Double_t XSec_M1500to2000 = h->GetBinContent(14) * h->GetBinWidth(14);
	Double_t XSecErr_M1500to2000 = h->GetBinError(14) * h->GetBinWidth(14);

	Double_t XSec_M2000to3000 = h->GetBinContent(15) * h->GetBinWidth(15);
	Double_t XSecErr_M2000to3000 = h->GetBinError(15) * h->GetBinWidth(15);

	Double_t DiffXSec_M1000to1500 = (XSec_M1000to1200 + XSec_M1200to1500) / 500.0;
	Double_t DiffXSecErr_M1000to1500 = QuadratureSum( XSecErr_M1000to1200, XSecErr_M1200to1500 ) / 500.0; // -- assume no correlation -- //

	Double_t DiffXSec_M1500to3000 = (XSec_M1500to2000 + XSec_M2000to3000) / 1500.0;
	Double_t DiffXSecErr_M1500to3000 = QuadratureSum( XSecErr_M1500to2000, XSecErr_M2000to3000 ) / 1500.0;

	h_Merge->SetBinContent( 12, DiffXSec_M1000to1500 );
	h_Merge->SetBinError( 12, DiffXSecErr_M1000to1500 );

	h_Merge->SetBinContent( 13, DiffXSec_M1500to3000 );
	h_Merge->SetBinError( 13, DiffXSecErr_M1500to3000 );

	return h_Merge;
}

Double_t QuadratureSum( Double_t a, Double_t b )
{
	return sqrt( a*a + b*b );
}
