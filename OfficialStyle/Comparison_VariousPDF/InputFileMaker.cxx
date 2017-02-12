#include "Comparison_VariousPDF.cxx"

void FillROOTFile_Data( TFile *f_output );
void FillROOTFile_NNPDF( TFile *f_output );
void FillROOTFile_OtherPDFs( TFile *f_output );
TH1D* Extract_RelUnc( TH1D* h );
TH1D* MergeLastTwoBins( TH1D* h );
TH1D* Rebin_MassBinEdges_Above200( TH1D* h );
Double_t QuadratureSum( Double_t a, Double_t b );

void InputFileMaker()
{
	TFile *f_output = TFile::Open(FileName, "RECREATE");

	FillROOTFile_Data( f_output );
	FillROOTFile_NNPDF( f_output );
	FillROOTFile_OtherPDFs( f_output );

	f_output->Close();
}

void FillROOTFile_Data( TFile *f_output )
{
	// -- uses dimuon channel -- //
	TH1D* h_DXSec = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr", HistName_Data );

	TH1D* h_RelStatUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent", HistName_RelStatUnc );
	h_RelStatUnc->Scale( 0.01 );
	
	TH1D* h_RelExpSystUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Tot_Percent" );
	TH1D* h_RelAccUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Acc._Percent" );
	TH1D* h_RelSystUnc = QuadSum_NoError( h_RelExpSystUnc, h_RelAccUnc );
	h_RelSystUnc->Scale( 0.01 );

	TH1D* h_RelTotUnc = QuadSum_NoError( h_RelSystUnc, h_RelStatUnc );
	h_RelTotUnc->SetName( HistName_RelTotUnc );

	h_DXSec = Rebin_MassBinEdges_Above200( h_DXSec );
	h_RelStatUnc = Rebin_MassBinEdges_Above200( h_RelStatUnc );
	h_RelTotUnc = Rebin_MassBinEdges_Above200( h_RelTotUnc );

	f_output->cd();

	h_DXSec->Write();
	h_RelStatUnc->Write();
	h_RelTotUnc->Write();
}

void FillROOTFile_NNPDF( TFile *f_output )
{
	TH1D* h_DXSec = Get_Hist("ROOTFile_xSec_Theory.root", "h_DiffXsec_FEWZ_NNPDF_NNLO", HistName_NNPDF);
	TH1D* h_RelTotUnc = Extract_RelUnc( h_DXSec );
	h_RelTotUnc->SetName( HistName_NNPDF_RelUnc );

	h_DXSec = Rebin_MassBinEdges_Above200( h_DXSec );
	h_RelTotUnc = Rebin_MassBinEdges_Above200( h_RelTotUnc );

	f_output->cd();

	h_DXSec->Write();
	h_RelTotUnc->Write();
}

void FillROOTFile_OtherPDFs( TFile *f_output )
{
	TString BasePath = "/Users/KyeongPil_Lee/Research/ntupleMaking/13TeV/TheoryValues/v20161222_1st_NNLO_VariousPDF_HighMass_DYBin";
	TH1D* h_DXSec_CT = Get_Hist(BasePath+"/CT14/ROOTFile_DY_FEWZ_NNLO_CT14nnlo.root", "h_DiffXsec", HistName_CT);
	h_DXSec_CT = MergeLastTwoBins( h_DXSec_CT );
	TH1D* h_RelTotUnc_CT = Extract_RelUnc( h_DXSec_CT );
	h_RelTotUnc_CT->SetName( HistName_CT_RelUnc );

	TH1D* h_DXSec_MMHT = Get_Hist(BasePath+"/MMHT2014/ROOTFile_DY_FEWZ_NNLO_MMHT2014nnlo68cl.root", "h_DiffXsec", HistName_MMHT);
	h_DXSec_MMHT = MergeLastTwoBins( h_DXSec_MMHT );
	TH1D* h_RelTotUnc_MMHT = Extract_RelUnc( h_DXSec_MMHT );
	h_RelTotUnc_MMHT->SetName( HistName_MMHT_RelUnc );

	TH1D* h_DXSec_HERAPDF = Get_Hist(BasePath+"/HERAPDF/ROOTFile_DY_FEWZ_NNLO_HERAPDF15NNLO_EIG.root", "h_DiffXsec", HistName_HERAPDF);
	h_DXSec_HERAPDF = MergeLastTwoBins( h_DXSec_HERAPDF );
	TH1D* h_RelTotUnc_HERAPDF = Extract_RelUnc( h_DXSec_HERAPDF );
	h_RelTotUnc_HERAPDF->SetName( HistName_HERAPDF_RelUnc );

	TH1D* h_DXSec_ABM = Get_Hist(BasePath+"/ABM12LHC/ROOTFile_DY_FEWZ_NNLO_abm12lhc_5_nnlo.root", "h_DiffXsec", HistName_ABM);
	h_DXSec_ABM = MergeLastTwoBins( h_DXSec_ABM );
	TH1D* h_RelTotUnc_ABM = Extract_RelUnc( h_DXSec_ABM );
	h_RelTotUnc_ABM->SetName( HistName_ABM_RelUnc );

	f_output->cd();

	h_DXSec_CT->Write();
	h_RelTotUnc_CT->Write();

	h_DXSec_MMHT->Write();
	h_RelTotUnc_MMHT->Write();

	h_DXSec_HERAPDF->Write();
	h_RelTotUnc_HERAPDF->Write();

	h_DXSec_ABM->Write();
	h_RelTotUnc_ABM->Write();
}

TH1D* Extract_RelUnc( TH1D* h )
{
	TH1D* h_RelUnc = (TH1D*)h->Clone();
	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t value = h->GetBinContent(i_bin);
		Double_t AbsUnc = h->GetBinError(i_bin);
		Double_t RelUnc = AbsUnc / value;

		h_RelUnc->SetBinContent(i_bin, RelUnc ); // -- just ratio, not percentage -- //
		h_RelUnc->SetBinError(i_bin, 0);
	}

	return h_RelUnc;
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
	h_Merge = Rebin_MassBinEdges_Above200( h_Merge );

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

TH1D* Rebin_MassBinEdges_Above200( TH1D* h )
{
	const Int_t nMassBin = 13;
	Double_t MassBinEdges[nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

	return (TH1D*)h->Rebin(nMassBin, h->GetName(), MassBinEdges);
}

Double_t QuadratureSum( Double_t a, Double_t b )
{
	return sqrt( a*a + b*b );
}