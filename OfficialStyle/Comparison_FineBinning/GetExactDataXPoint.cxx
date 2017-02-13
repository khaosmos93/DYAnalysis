#define nMassBin 43

#include <TMath.h>
#include <DYAnalysis_76X/Include/DYAnalyzer.h>

Double_t BreitWigner(Double_t* x, Double_t* par)
{
	Double_t arg1 = 14.0/TMath::Pi(); // 2 over pi
	Double_t arg2 = par[1]*par[1]*par[2]*par[2]; //Gamma=par[1]  M=par[2]
	Double_t arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]));
	Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]));
	return par[0]*arg1*arg2/(arg3 + arg4);
}

Double_t CustomizedBkg(Double_t *x, Double_t* par)
{
	Double_t logx = TMath::Log(x[0]);
	Double_t arg1 = par[1];
	Double_t arg2 = par[2]*logx;
	Double_t arg3 = par[3]*logx*logx;
	Double_t arg4 = par[4]*logx*logx*logx;

	return par[0]*TMath::Power(x[0], arg1 + arg2 + arg3 + arg4);
}

class FittingTool
{
public:
	FittingTool() { TH1::AddDirectory(kFALSE); }

	void Set_File_HistName( TFile *f, TString _HistName )
	{
		this->f_input = f;
		this->HistName = _HistName;
	}

	void FindDataPoint_All()
	{
		this->Set_MassBins();
		this->GetHistogram();

		for(Int_t i=0; i<nMassBin; i++)
		{
			this->FindDataPoint_GivenMassRange( MassBinEdges[i], MassBinEdges[i+1] );
		}

		this->PrintSummary();
	}

	void Save_XShiftedGraph( TH1D *h_data, TFile *f_output )
	{
		TGraphAsymmErrors *g_data = new TGraphAsymmErrors( nMassBin );
		g_data->SetName("g_DiffXSec_xShifted");
		for(Int_t i=0; i<g_data->GetN(); i++)
		{
			Int_t i_bin = i+1;
			Double_t DiffXSec = h_data->GetBinContent(i_bin);
			Double_t DiffXSecErr = h_data->GetBinError(i_bin);

			Double_t LowerEdge = h_data->GetBinLowEdge(i_bin);
			Double_t UpperEdge = h_data->GetBinLowEdge(i_bin+1);
			
			Double_t x_shifted = vec_xDataPoint[i];
			Double_t xErr_low = x_shifted - LowerEdge;
			Double_t xErr_high = UpperEdge - x_shifted;
			Double_t yErr_low = DiffXSecErr;
			Double_t yErr_high = yErr_low;

			g_data->SetPoint(i, x_shifted, DiffXSec);
			g_data->SetPointError(i, xErr_low, xErr_high, yErr_low, yErr_high);
		}
		f_output->cd();
		h_data->Write();
		g_data->Write();
	}

	void Save_NormChi2( TFile *f_output )
	{
		this->h_Chi2nDOF = new TH1D("h_Chi2nDOF", "", nMassBin, MassBinEdges);
		Int_t nChi2 = (Int_t)vec_Chi2.size();

		if( nChi2 != nMassBin )
		{
			printf("(nChi2, nMassBin) = (%d, %d) -> need to check\n", nChi2, nMassBin);
			return;
		}
		else
		{
			for(Int_t i=0; i<nMassBin; i++)
			{
				Int_t i_bin = i+1;
				h_Chi2nDOF->SetBinContent( i_bin, vec_Chi2[i] );
				h_Chi2nDOF->SetBinError( i_bin, 0 );
			}

			f_output->cd();
			h_Chi2nDOF->Write();
		}
	}

protected:
	TFile *f_input;
	TString HistName;
	TH1D* h_DiffXsec;

	Double_t MassBinEdges[nMassBin+1];

	vector< Double_t > vec_Chi2;
	vector< Double_t > vec_Average;
	vector< Double_t > vec_xDataPoint;
	vector< Double_t > vec_CrossCheck_yValue;

	TH1D* h_Chi2nDOF;

	void Set_MassBins()
	{
		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000};
		for(Int_t i=0; i<nMassBin+1; i++)
		{
			MassBinEdges[i] = MassBinEdges_temp[i];
		}
	}

	void GetHistogram()
	{
		this->h_DiffXsec = (TH1D*)f_input->Get( HistName )->Clone();
	}

	void FindDataPoint_GivenMassRange( Double_t LowerEdge, Double_t UpperEdge )
	{
		TString TStr_FuncName = this->Find_FitFunctionName( LowerEdge, UpperEdge );
		TF1 *func = NULL;
		if( TStr_FuncName == "BreitWigner" )
		{
			func = new TF1(TStr_FuncName, BreitWigner, LowerEdge, UpperEdge, 3);
			func->SetParameter(0, 1.0); func->SetParName(0, "const");
			func->SetParameter(1, 5.0); func->SetParName(1, "width");
			func->SetParameter(2, 95.0); func->SetParName(2, "mean");
		}
		else if( TStr_FuncName == "CustomizedBkg" )
		{
			func = new TF1(TStr_FuncName, CustomizedBkg, LowerEdge, UpperEdge, 5);
			func->SetParameter(0, 0.001); func->SetParName(0, "p0");
			func->SetParameter(1, 0.001); func->SetParName(1, "p1");
			func->SetParameter(2, -0.001); func->SetParName(2, "p2");
			func->SetParameter(3, -0.001); func->SetParName(3, "p3");
			func->SetParameter(4, -0.001); func->SetParName(4, "p4");
		}

		printf("\n======\n[%.0lf < M < %.0lf]: Fit function = %s\n======\n", LowerEdge, UpperEdge, TStr_FuncName.Data() );

		TH1D* h_Clone = (TH1D*)h_DiffXsec->Clone();

		h_Clone->GetXaxis()->SetRangeUser(LowerEdge, UpperEdge);
		h_Clone->Fit( TStr_FuncName, "R", "", LowerEdge, UpperEdge );
		this->PrintFitCanvas( h_Clone, LowerEdge, UpperEdge);

		TF1 *FitFunc = h_Clone->GetFunction( TStr_FuncName );

		Double_t chi2 = FitFunc->GetChisquare();
		Double_t nDOF = FitFunc->GetNDF();
		Double_t Chi2nDOF = chi2 / nDOF;
		vec_Chi2.push_back( Chi2nDOF );

		Double_t Average = FitFunc->Integral( LowerEdge, UpperEdge ) / (UpperEdge - LowerEdge);
		Double_t x_shifted = FitFunc->GetX( Average );
		Double_t CrossCheck_y = FitFunc->Eval( x_shifted );

		vec_Average.push_back( Average );
		vec_xDataPoint.push_back( x_shifted );
		vec_CrossCheck_yValue.push_back( CrossCheck_y );

	}

	void PrintFitCanvas(TH1D* h_DiffXsec, Double_t xMin, Double_t xMax)
	{
		// h_DiffXsec->SetStats(kTRUE);
		// gStyle->SetOptFit(1111);

		TCanvas *canvas = new TCanvas(TString::Format("FitResult_M%.0fto%.0f", xMin, xMax), "", 800, 800);
		canvas->cd();

		// gPad->SetLogy();
		gPad->SetTopMargin(0.03);
		gPad->SetRightMargin(0.03);
		gPad->SetLeftMargin(0.13);
		gPad->SetBottomMargin(0.13);


		h_DiffXsec->Draw("EP");


		h_DiffXsec->SetTitle("");
		h_DiffXsec->SetLineColor(kBlue);
		h_DiffXsec->SetMarkerColor(kBlue);
		h_DiffXsec->SetMarkerStyle(20);

		h_DiffXsec->GetXaxis()->SetTitle("m (#mu#mu) [GeV]");
		h_DiffXsec->GetXaxis()->SetNoExponent();
		h_DiffXsec->GetXaxis()->SetMoreLogLabels();
		h_DiffXsec->GetXaxis()->SetRangeUser(xMin, xMax);

		h_DiffXsec->GetYaxis()->SetTitle("d#sigma/dm [pb/GeV]");
		h_DiffXsec->GetYaxis()->SetTitleOffset(1.4);

		// TPaveStats *StatBox = (TPaveStats*)h_DiffXsec->GetListOfFunctions()->FindObject("stats");
		// StatBox->SetX1NDC(0.75);
		// StatBox->SetX2NDC(0.95);
		// StatBox->SetY1NDC(0.75);
		// StatBox->SetY2NDC(0.95);

		canvas->SaveAs(".pdf");
	}

	TString Find_FitFunctionName( Double_t LowerEdge, Double_t UpperEdge )
	{
		if( LowerEdge >= 76 && UpperEdge <= 101 )
			return "BreitWigner";
		else if( LowerEdge >= 1000 )
			return "CustomizedBkg";
		else
			return "expo";
	}

	void PrintSummary()
	{
		Int_t nChi2 = vec_Chi2.size();
		for(Int_t i=0; i<nChi2; i++)
		{
			printf( "[%.0lf < M < %.0lf: chi2/ndof = %.2lf] x = %.2lf -> average = %lf, f(x) = %lf\n", 
				MassBinEdges[i], MassBinEdges[i+1], vec_Chi2[i], vec_xDataPoint[i], vec_Average[i], vec_CrossCheck_yValue[i] );

			if( fabs(vec_Average[i] - vec_CrossCheck_yValue[i]) / vec_Average[i] > 0.001 )
				printf("\t[WARNNING] average and cross check value have large deviation\n\n");
		}
	}
};

void GetExactDataXPoint()
{
	TString FileName_NNLO = GetBasePath() + "TheoryValues/NNLO_NNPDF30_FineBinning/ROOTFile_DY_FEWZ_NNLO.root";
	TFile *f_input = TFile::Open(FileName_NNLO);

	FittingTool *tool = new FittingTool();

	tool->Set_File_HistName( f_input, "h_DiffXsec" );
	tool->FindDataPoint_All();

	TString FileName_Data = GetBasePath() + "Include/Results_ROOTFiles_76X/ROOTFile_DiffXSec_FullUnc.root";
	TFile *f_data = TFile::Open(FileName_Data); f_data->cd();
	TH1D* h_data = (TH1D*)f_data->Get("h_DiffXsec_FSRCorr_woLumi")->Clone("h_DiffXSec");

	TFile *f_output = TFile::Open("ROOTFile_Histograms_XDataPoint.root", "RECREATE");

	tool->Save_XShiftedGraph(h_data, f_output);
	tool->Save_NormChi2( f_output );
}