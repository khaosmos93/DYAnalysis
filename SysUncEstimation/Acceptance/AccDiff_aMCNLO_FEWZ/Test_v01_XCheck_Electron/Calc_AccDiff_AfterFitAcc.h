#include <TMath.h>
#include <Include/PlotTools.h>

Double_t FitFunc_Low(Double_t* x, Double_t* par)
{
	Double_t arg1 = par[0];

	Double_t ExpArg = (par[2] - x[0]) / par[3];
	Double_t arg2 = (-1)*par[1]*TMath::Exp( ExpArg );

	Double_t arg3 = par[4]*TMath::Power( x[0], par[5] );

	return arg1 + arg2 + arg3;
}

Double_t FitFunc_High(Double_t* x, Double_t* par)
{
	Double_t arg1 = par[0];

	Double_t arg2 = par[1] / TMath::Power( x[0]+par[2] , 3);

	Double_t arg3 = x[0]*x[0] / par[3];

	return arg1 + arg2 + arg3;
}

class FittingTool
{
public:
	TString TStr_Channel;

	TString AnalyzerPath;

	TH1D* h_Acc_aMCNLO;
	TH1D* h_Acc_FEWZ;

	TH1D* h_AccDiff;

	FittingTool( TString _TStr_Channel )
	{
		this->TStr_Channel = _TStr_Channel;
		this->Get_Histogram();

		this->AnalyzerPath = gSystem->Getenv("KP_ANALYER_PATH");
	}

	void Fit_All()
	{

	}

	void Save( TFile *f_output )
	{

	}

protected:
	void Fit_Acc( TH1D* h_Acc )
	{
		h_Acc->Fit

	}


	void Get_Histogram()
	{
		if( this->TStr_Channel == "Muon" )
		{
			TString FileName_aMCNLO = AnalyzerPath+"/SysUncEstimation/Acceptance/AccDiff_aMCNLO_FEWZ/ROOTFile_SysUnc_DiffWithFEWZ.root";
			this->h_Acc_aMCNLO = Get_Hist( FileName_aMCNLO, "h_Acc_aMCNLO");

			TString FileName_FEWZ = FileName_aMCNLO;
			this->h_Acc_FEWZ = Get_Hist( FileName_FEWZ, "h_Acc_FEWZ_NNLO_PDF4LHC15");
		}
		else if( this->TStr_Channel == "Electron" )
		{

		}
	}
};