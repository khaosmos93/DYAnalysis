#include <fstream>
#include <iostream>
#include <stdio.h>

void AssignScaleUnc_3Percent( TH1D* h );
void MakeDiffXsecHisto_HistOnly(TH1D *h_Xsec, TH1D *h_DiffXsec);
void MakeDiffXsecHisto( TH1D *h, Double_t *xSec, Double_t *error );
void MakeROOTFile_xSec_Theory()
{
	// setTDRStyle();
	// tdrStyle->SetErrorX(0.5);

	// const Int_t nMassBin = 45;
	// Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
	// 									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
	// 									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
	// 									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
	// 									 830, 1000, 1200, 1500, 2000, 3000};
	
	const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};
	Double_t xSec_NNPDF[nMassBin+2];
	Double_t error_NNPDF[nMassBin+2];
	Double_t xSec_CT14[nMassBin+2];
	Double_t error_CT14[nMassBin+2];
	Double_t xSec_MMHT2014[nMassBin+2];
	Double_t error_MMHT2014[nMassBin+2];

	FILE *fl;
	Double_t LowerEdge;
	Double_t UpperEdge;
	Double_t xSec_NNPDF_temp;
	Double_t error_NNPDF_temp;
	Double_t xSec_CT14_temp;
	Double_t error_CT14_temp;
	Double_t xSec_MMHT2014_temp;
	Double_t error_MMHT2014_temp;

	// -- https://twiki.cern.ch/twiki/pub/CMS/EWKDrellYan2015/dy_mm_13tev_NNLO_WNLO_4pi_twiki.txt -- //
	if ((fl = fopen("DiffXsec_NNLO.txt", "r")) != NULL)
	{
		Int_t i=0;

		while(fscanf(fl, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", &LowerEdge, &UpperEdge, &xSec_NNPDF_temp, &error_NNPDF_temp, &xSec_CT14_temp, &error_CT14_temp, &xSec_MMHT2014_temp, &error_MMHT2014_temp) != EOF )
		{
			xSec_NNPDF[i] = xSec_NNPDF_temp;
			error_NNPDF[i] = error_NNPDF_temp;
			xSec_CT14[i] = xSec_CT14_temp;
			error_CT14[i] = error_CT14_temp;
			xSec_MMHT2014[i] = xSec_MMHT2014_temp;
			error_MMHT2014[i] = error_MMHT2014_temp;

			printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", LowerEdge, UpperEdge, xSec_NNPDF[i], error_NNPDF[i], xSec_CT14[i], error_CT14[i], xSec_MMHT2014[i], error_MMHT2014[i]);
			i++;

			if( i == 45 )
				break;
		}
	}
	fclose(fl);

	TH1D *h_NNPDF = new TH1D("h_NNPDF_NNLO", "", nMassBin, MassBinEdges);
	TH1D *h_CT14 = new TH1D("h_CT14_NNLO", "", nMassBin, MassBinEdges);
	TH1D *h_MMHT2014 = new TH1D("h_MMHT2014_NNLO", "", nMassBin, MassBinEdges);

	MakeDiffXsecHisto( h_NNPDF, xSec_NNPDF, error_NNPDF );
	MakeDiffXsecHisto( h_CT14, xSec_CT14, error_CT14 );
	MakeDiffXsecHisto( h_MMHT2014, xSec_MMHT2014, error_MMHT2014 );

	TFile *f_input = new TFile("ROOTFile_xSec_Theory_NLO.root");
	TH1D *h_FEWZ_NLO = (TH1D*)f_input->Get("h_NNPDF30_nlo_as_0118")->Clone();
	TH1D *h_DiffXsec_FEWZ_NLO = new TH1D("h_DiffXsec_FEWZ_NNPDF_NLO", "", nMassBin, MassBinEdges);
	MakeDiffXsecHisto_HistOnly( h_FEWZ_NLO, h_DiffXsec_FEWZ_NLO );

	AssignScaleUnc_3Percent( h_DiffXsec_FEWZ_NLO );
	AssignScaleUnc_3Percent( h_NNPDF );
	AssignScaleUnc_3Percent( h_CT14 );
	AssignScaleUnc_3Percent( h_MMHT2014 );

	// -- save -- //
	TFile *f_output = new TFile("ROOTFile_xSec_Theory.root", "RECREATE"); f_output->cd();

	h_DiffXsec_FEWZ_NLO->Write();

	h_NNPDF->SetName("h_DiffXsec_FEWZ_NNPDF_NNLO");
	h_NNPDF->Write();

	h_CT14->SetName("h_DiffXsec_FEWZ_CT14_NNLO");
	h_CT14->Write();

	h_MMHT2014->SetName("h_DiffXsec_FEWZ_MMHT2014_NNLO");
	h_MMHT2014->Write();
}

void AssignScaleUnc_3Percent( TH1D* h )
{
	Int_t nBin = h->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t DiffXSec = h->GetBinContent(i_bin);
		Double_t Err_before = h->GetBinError(i_bin);
		Double_t Err_ScaleVar = DiffXSec * 0.03; // -- 3% for all mass bins -- //

		Double_t Err_after = sqrt(Err_before*Err_before + Err_ScaleVar*Err_ScaleVar);
		h->SetBinError(i_bin, Err_after);
	}
}

void MakeDiffXsecHisto_HistOnly(TH1D *h_Xsec, TH1D *h_DiffXsec)
{
	Int_t nBins = h_Xsec->GetNbinsX();
	for(Int_t i=0; i<nBins-4; i++)
	{
		Int_t i_bin = i+1;
		Double_t BinWidth = h_Xsec->GetBinWidth(i_bin);

		Double_t xSec = h_Xsec->GetBinContent(i_bin);
		Double_t xSec_dM = xSec / BinWidth;

		Double_t error_before = h_Xsec->GetBinError(i_bin);
		Double_t error_after = error_before / BinWidth;

		h_DiffXsec->SetBinContent(i_bin, xSec_dM);
		h_DiffXsec->SetBinError(i_bin, error_after);

		// Double_t low = h_Xsec->GetBinLowEdge(i_bin);
		// Double_t high = h_Xsec->GetBinLowEdge(i_bin + 1);
		// printf("%2dth bin [%5.lf, %5.lf] (xSec, BinWidth, dSigma/dM) = (%15.9lf, %6.1lf, %15.9lf), (error_before, error_after) = (%8.5lf, %8.5lf)\n", 
			// i_bin, low, high, xSec, BinWidth, xSec_dM, error_before, error_after );
	}

	// -- Merging the high-mass bins -- //
	Double_t Xsec_Bin42 = h_Xsec->GetBinContent(42);
	Double_t Xsec_Bin43 = h_Xsec->GetBinContent(43);
	Double_t DiffXsec_M1000to1500 = (Xsec_Bin42 + Xsec_Bin43) / 500; // -- 500 = bin width -- //

	Double_t error_Bin42 = h_Xsec->GetBinError(42);
	Double_t error_Bin43 = h_Xsec->GetBinError(43);
	Double_t error_M1000to1500 = sqrt(error_Bin42*error_Bin42 + error_Bin43*error_Bin43) / 500;

	h_DiffXsec->SetBinContent(42, DiffXsec_M1000to1500);
	h_DiffXsec->SetBinError(42, error_M1000to1500);

	Double_t Xsec_Bin44 = h_Xsec->GetBinContent(44);
	Double_t Xsec_Bin45 = h_Xsec->GetBinContent(45);
	Double_t DiffXsec_M1500to3000 = (Xsec_Bin44 + Xsec_Bin45) / 1500; // -- 1500 = bin width -- //

	Double_t error_Bin44 = h_Xsec->GetBinError(44);
	Double_t error_Bin45 = h_Xsec->GetBinError(45);
	Double_t error_M1500to3000 = sqrt(error_Bin44*error_Bin44 + error_Bin45*error_Bin45) / 1500;

	h_DiffXsec->SetBinContent(43, DiffXsec_M1500to3000);
	h_DiffXsec->SetBinError(43, error_M1500to3000);
}

void MakeDiffXsecHisto( TH1D *h, Double_t *xSec, Double_t *error )
{
	Int_t nBins = 45;
	for(Int_t i=0; i<nBins-4; i++)
	{
		Int_t i_bin = i+1;
		Double_t BinWidth = h->GetBinWidth(i_bin);

		Double_t DiffxSec = xSec[i] / BinWidth;
		Double_t DiffxSec_error = error[i] / BinWidth;

		h->SetBinContent(i_bin, DiffxSec);
		h->SetBinError(i_bin, DiffxSec_error);
	}

	// -- Merging the high-mass bins -- //
	Double_t Xsec_Bin42 = xSec[41]; // -- i = i_bin - 1 -- //
	Double_t Xsec_Bin43 = xSec[42];
	Double_t DiffXsec_M1000to1500 = (Xsec_Bin42 + Xsec_Bin43) / 500; // -- 500 = bin width -- //

	Double_t error_Bin42 = error[41];
	Double_t error_Bin43 = error[42];
	Double_t error_M1000to1500 = sqrt(error_Bin42*error_Bin42 + error_Bin43*error_Bin43) / 500;

	h->SetBinContent(42, DiffXsec_M1000to1500);
	h->SetBinError(42, error_M1000to1500);


	Double_t Xsec_Bin44 = xSec[43];
	Double_t Xsec_Bin45 = xSec[44];
	Double_t DiffXsec_M1500to3000 = (Xsec_Bin44 + Xsec_Bin45) / 1500; // -- 1500 = bin width -- //

	Double_t error_Bin44 = error[43];
	Double_t error_Bin45 = error[44];
	Double_t error_M1500to3000 = sqrt(error_Bin44*error_Bin44 + error_Bin45*error_Bin45) / 1500;

	h->SetBinContent(43, DiffXsec_M1500to3000);
	h->SetBinError(43, error_M1500to3000);
}

