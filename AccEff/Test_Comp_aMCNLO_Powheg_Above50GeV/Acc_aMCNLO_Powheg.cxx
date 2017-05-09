#include <Include/PlotTools.h>

void Acc_aMCNLO_Powheg()
{
	// -- aMC@NLO -- //
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_aMCNLO = ROOTFilePath + "/ROOTFile_AccEff.root";
	TGraphAsymmErrors* g_aMCNLO = Get_Graph(FileName_aMCNLO, "g_Acc");
	TH1D* h_aMCNLO = Convert_GraphToHist( g_aMCNLO );

	const Int_t nMassBin2 = 36;
	Double_t MassBinEdges[nMassBin2+1] = {50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

	h_aMCNLO = (TH1D*)h_aMCNLO->Rebin(nMassBin2, h_aMCNLO->GetName(), MassBinEdges); // -- above 50 GeV -- //

	// -- Powheg -- //
	TString FileName_Powheg = "ROOTFile_Histogram_Acc_Above50GeV_Powheg.root";
	TFile *f_Powheg = TFile::Open( FileName_Powheg );
	TEfficiency *TEff_Acc = (TEfficiency*)f_Powheg->Get("TEff_Acc_Mass")->Clone();
	TGraphAsymmErrors* g_Powheg = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();
	TH1D* h_Powheg = Convert_GraphToHist( g_Powheg );

	HistInfo *Hist_aMCNLO = new HistInfo( kRed, "aMC@NLO" );
	Hist_aMCNLO->Set_Histogram( h_aMCNLO );
	Hist_aMCNLO->Set();

	HistInfo *Hist_Powheg = new HistInfo( kBlue, "Powheg" );
	Hist_Powheg->Set_Histogram( h_Powheg );
	Hist_Powheg->Set();

	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio( "c_Acc_aMCNLO_Powheg", Hist_Powheg, Hist_aMCNLO );
	canvas->SetTitle( "m [GeV]", "Acceptance", "Powheg/aMC@NLO" );
	canvas->SetRatioRange(0.94, 1.06);
	canvas->SetLegendPosition( 0.55, 0.32, 0.95, 0.42 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw(1, 0);
}