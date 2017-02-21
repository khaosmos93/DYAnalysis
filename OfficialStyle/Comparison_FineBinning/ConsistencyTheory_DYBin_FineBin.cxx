#include "DYAnalysis_76X/Include/DYAnalyzer.h"
#include "DYAnalysis_76X/Include/PlotTools.h"

void ConsistencyTheory_DYBin_FineBin()
{
	TString FileName_NNLO = GetBasePath() + "TheoryValues/NNLO_NNPDF30_FineBinning/ROOTFile_DY_FEWZ_NNLO.root";
	TH1D* h_NNLO_FineBin = Get_Hist( FileName_NNLO, "h_DiffXsec" );

	TString FileName_Data = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_DiffXSec_FullUnc.root";
	TH1D* h_NNLO_DYBin = Get_Hist( FileName_Data, "h_DiffXsec_FEWZ_NNPDF_NNLO" );

	Int_t nBin = h_NNLO_DYBin->GetNbinsX();
	for(Int_t i=0; i<nBin; i++)
	{
		Int_t i_bin = i+1;

		Double_t BinWidth = h_NNLO_DYBin->GetBinWidth(i_bin);	
		Double_t LowerEdge = h_NNLO_DYBin->GetBinLowEdge(i_bin);
		Double_t UpperEdge = h_NNLO_DYBin->GetBinLowEdge(i_bin+1);

		Double_t XSec_SumUp = 0;
		Int_t nBinFine = h_NNLO_FineBin->GetNbinsX();
		for(Int_t i_fine=0; i_fine<nBinFine; i_fine++)
		{
			Int_t i_fineBin = i_fine + 1;
			Double_t BinCenter = h_NNLO_FineBin->GetBinCenter(i_fineBin);

			if( BinCenter > LowerEdge && BinCenter < UpperEdge )
				XSec_SumUp = XSec_SumUp + h_NNLO_FineBin->GetBinContent(i_fineBin) * h_NNLO_FineBin->GetBinWidth(i_fineBin);
		}

		Double_t DiffXSec_FineBin = XSec_SumUp / BinWidth;
		Double_t DiffXSec_DYBin = h_NNLO_DYBin->GetBinContent(i_bin);

		Double_t Diff = fabs(DiffXSec_DYBin - DiffXSec_FineBin) / DiffXSec_DYBin;

		printf("[%4.0lf< M <%4.0lf] (DiffXSec_DYBin, DiffXSec_FineBin) = (%lf, %lf) (%.3lf %% diff.)\n", 
			LowerEdge, UpperEdge, DiffXSec_DYBin, DiffXSec_FineBin, Diff*100 );
	}
}
