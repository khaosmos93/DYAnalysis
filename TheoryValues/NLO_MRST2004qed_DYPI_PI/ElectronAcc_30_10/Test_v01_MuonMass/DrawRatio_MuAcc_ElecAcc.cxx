#include <Include/PlotTools.h>

TH1D* Rebin_Above200GeV( TH1D* h );
void DrawCanvas_Above200GeV( TH1D* h_MM, TH1D* h_EE );

void DrawRatio_MuAcc_ElecAcc()
{
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString FileName_MM = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ROOTFile_DYPI_DY_FEWZ.root";
	TString FileName_EE = AnalyzerPath + "/TheoryValues/NLO_MRST2004qed_DYPI_PI/ElectronAcc_30_10/Test_v01_MuonMass/Results/ROOTFile_DYPI_DY_FEWZ.root";

	TH1D *h_MM =  Get_Hist( FileName_MM, "h_ratio" );
	TH1D *h_EE =  Get_Hist( FileName_EE, "h_ratio" );

	HistInfo *Hist_MM = new HistInfo( kBlue, "Dimuon acceptance" );
	Hist_MM->Set_Histogram( h_MM );
	Hist_MM->Set();

	HistInfo *Hist_EE = new HistInfo( kGreen+2, "Dielectron acceptance" );
	Hist_EE->Set_Histogram( h_EE );
	Hist_EE->Set();

	TString CanvasName = "c_Ratio_DYPI_PI_MuAcc_ElecAcc_MuonMass";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_EE, Hist_MM);
	canvas->SetTitle("m [GeV]", "#sigma_{DY+PI}/#sigma_{DY}", "Acc(e)/Acc(#mu)");
	canvas->SetLegendPosition(0.17, 0.75, 0.60, 0.95);
	canvas->SetRatioRange( 0.97, 1.03 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw(1, 0);

	DrawCanvas_Above200GeV( h_MM, h_EE );
}

void DrawCanvas_Above200GeV( TH1D* h_MM, TH1D* h_EE )
{
	TH1D* h_MM_temp = Rebin_Above200GeV( h_MM );
	TH1D* h_EE_temp = Rebin_Above200GeV( h_EE );

	HistInfo *Hist_MM = new HistInfo( kBlue, "Dimuon acceptance" );
	Hist_MM->Set_Histogram( h_MM_temp );
	Hist_MM->Set();

	HistInfo *Hist_EE = new HistInfo( kGreen+2, "Dielectron acceptance" );
	Hist_EE->Set_Histogram( h_EE_temp );
	Hist_EE->Set();

	TString CanvasName = "c_Ratio_DYPI_PI_MuAcc_ElecAcc_MuonMass_Above200GeV";
	DrawCanvas_TwoHistRatio *canvas = new DrawCanvas_TwoHistRatio(CanvasName, Hist_EE, Hist_MM);
	canvas->SetTitle("m [GeV]", "#sigma_{DY+PI}/#sigma_{DY}", "Acc(e)/Acc(#mu)");
	canvas->SetLegendPosition(0.17, 0.75, 0.60, 0.95);
	canvas->SetRatioRange( 0.994, 1.006 );
	canvas->SetLatex( "Simulation" );
	canvas->Draw(1, 0);
}

TH1D* Rebin_Above200GeV( TH1D* h )
{
	const Int_t _nMassBin = 13;
	Double_t MassBinEdges[_nMassBin+1] = {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000};

	return (TH1D*)h->Rebin(_nMassBin, h->GetName(), MassBinEdges);
} 