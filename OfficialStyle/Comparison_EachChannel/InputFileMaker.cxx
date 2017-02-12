// #include </Users/KyeongPil_Lee/Physics/Include/PlotTools.h>
#include <Comparison_EachChannel.cxx>

void FileMaker_MuonChannel( TFile *f_output )
{
	TH1D* h_DXSec = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr", HistName_DXSec_mm );
	TH1D* h_RelStatUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent", HistName_RelStatUnc_mm );
	h_RelStatUnc->Scale( 0.01 );

	f_output->cd();
	h_DXSec->Write();
	h_RelStatUnc->Write();
}

void FileMaker_ElectronChannel( TFile *f_output )
{
	TH1D* h_DXSec = Get_Hist( "diffXsec_Electron.root", "h_xSec_dM_FSRCorr", HistName_DXSec_ee );
	// -- uses dimuon channel -- //
	TH1D* h_RelStatUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent", HistName_RelStatUnc_ee ); 
	h_RelStatUnc->Scale( 0.01 );

	f_output->cd();
	h_DXSec->Write();
	h_RelStatUnc->Write();
}

void FileMaker_CombinedChannel( TFile *f_output )
{
	// -- uses dimuon channel -- //
	TH1D* h_DXSec = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_DiffXsec_FSRCorr", HistName_DXSec_ll );
	TH1D* h_RelStatUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent", HistName_RelStatUnc_ll );
	h_RelStatUnc->Scale( 0.01 );
	
	TH1D* h_RelExpSystUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Tot_Percent" );
	TH1D* h_RelAccUnc = Get_Hist( "ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Acc._Percent" );
	TH1D* h_RelSystUnc = QuadSum_NoError( h_RelExpSystUnc, h_RelAccUnc );
	h_RelSystUnc->SetName( HistName_RelSystUnc_ll );
	h_RelSystUnc->Scale( 0.01 );

	TH1D* h_RelTotUnc = QuadSum_NoError( h_RelSystUnc, h_RelStatUnc );
	h_RelTotUnc->SetName( HistName_RelTotUnc_ll );

	f_output->cd();
	h_DXSec->Write();
	h_RelStatUnc->Write();
	h_RelSystUnc->Write();
	h_RelTotUnc->Write();
}

void InputFileMaker()
{
	TFile *f_output = TFile::Open(FileName, "RECREATE");

	FileMaker_CombinedChannel( f_output );
	FileMaker_MuonChannel( f_output );
	FileMaker_ElectronChannel( f_output );
}