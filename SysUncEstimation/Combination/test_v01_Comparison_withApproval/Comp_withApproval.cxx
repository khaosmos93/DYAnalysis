#include <Include/PlotTools.h>

TH1D* h_RelStatUnc_Approval();
TH1D* h_RelSysUnc_Tot_Approval();
TH1D* h_RelSystUnc_Bkg_Approval();
TH1D* h_RelSystUnc_FSR_Approval();

void DrawCanvas( TString Type, TH1D* h_old, TH1D* h_new );

void Comp_withApproval()
{
	TH1D* h_RelStatUnc_old = h_RelStatUnc_Approval();
	TH1D* h_RelSysUnc_Tot_old = h_RelSysUnc_Tot_Approval();
	TH1D* h_RelSystUnc_Bkg_old = h_RelSystUnc_Bkg_Approval();
	TH1D* h_RelSystUnc_FSR_old = h_RelSystUnc_FSR_Approval();

	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");

	TH1D* h_RelStatUnc_new = Get_Hist( ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelStatUnc_Percent");
	TH1D* h_RelSysUnc_Tot_new = Get_Hist( ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Tot_Percent" );
	TH1D* h_RelSystUnc_Bkg_new = Get_Hist( ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_Bkg.Est._Percent" );
	TH1D* h_RelSystUnc_FSR_new = Get_Hist( ROOTFilePath+"/ROOTFile_DiffXSec_FullUnc.root", "h_RelSysUnc_FSR_Percent" );

	Print_Histogram( h_RelStatUnc_old );
	Print_Histogram( h_RelStatUnc_new );
	DrawCanvas( "Stat", h_RelStatUnc_old, h_RelStatUnc_new );
	DrawCanvas( "Syst", h_RelSysUnc_Tot_old, h_RelSysUnc_Tot_new );
	DrawCanvas( "Bkg", h_RelSystUnc_Bkg_old, h_RelSystUnc_Bkg_new );
	DrawCanvas( "FSR", h_RelSystUnc_FSR_old, h_RelSystUnc_FSR_new );
}

void DrawCanvas( TString Type, TH1D* h_old, TH1D* h_new )
{
	HistInfo *Hist_old = new HistInfo( kBlack, "SMP-16-009" );
	Hist_old->Set_Histogram( h_old );
	Hist_old->Set();

	HistInfo *Hist_new = new HistInfo( kRed, "Updated" );
	Hist_new->Set_Histogram( h_new );
	Hist_new->Set();
	Hist_new->CalcRatio_DEN( h_old );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "c_Comp_"+Type, TopPad, BottomPad, 1, 1);

	c->cd();
	TopPad->cd();
	Hist_old->Draw("HISTLPSAME");
	Hist_new->Draw("HISTLPSAME");

	SetHistFormat_TopPad( Hist_old->h, TString::Format("Uncertainty (%s.)", Type.Data()) );

	TLegend *legend;
	SetLegend( legend );
	Hist_old->AddToLegend( legend );
	Hist_new->AddToLegend( legend );
	legend->Draw();

	c->cd();
	BottomPad->cd();
	Hist_new->DrawRatio( "HISTLPSAME" );
	SetHistFormat_BottomPad( Hist_new->h_ratio, "m [GeV]", "New/Old", 0.9, 1.1 );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");
}

TH1D* h_RelStatUnc_Approval()
{
	Double_t xAxis1[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000}; 
	
	TH1D *h_RelUnc_Stat1 = new TH1D("h_RelUnc_Stat1","",43, xAxis1);
	h_RelUnc_Stat1->SetBinContent(1,0.9292741);
	h_RelUnc_Stat1->SetBinContent(2,1.066643);
	h_RelUnc_Stat1->SetBinContent(3,1.07945);
	h_RelUnc_Stat1->SetBinContent(4,0.9630962);
	h_RelUnc_Stat1->SetBinContent(5,0.8227239);
	h_RelUnc_Stat1->SetBinContent(6,0.7295136);
	h_RelUnc_Stat1->SetBinContent(7,0.668092);
	h_RelUnc_Stat1->SetBinContent(8,0.690299);
	h_RelUnc_Stat1->SetBinContent(9,0.688372);
	h_RelUnc_Stat1->SetBinContent(10,0.7455992);
	h_RelUnc_Stat1->SetBinContent(11,0.7054089);
	h_RelUnc_Stat1->SetBinContent(12,0.6411995);
	h_RelUnc_Stat1->SetBinContent(13,0.5678028);
	h_RelUnc_Stat1->SetBinContent(14,0.4086377);
	h_RelUnc_Stat1->SetBinContent(15,0.2881502);
	h_RelUnc_Stat1->SetBinContent(16,0.1096569);
	h_RelUnc_Stat1->SetBinContent(17,0.1070512);
	h_RelUnc_Stat1->SetBinContent(18,0.4119376);
	h_RelUnc_Stat1->SetBinContent(19,0.6386702);
	h_RelUnc_Stat1->SetBinContent(20,0.9855344);
	h_RelUnc_Stat1->SetBinContent(21,1.078291);
	h_RelUnc_Stat1->SetBinContent(22,1.359013);
	h_RelUnc_Stat1->SetBinContent(23,1.444598);
	h_RelUnc_Stat1->SetBinContent(24,1.617161);
	h_RelUnc_Stat1->SetBinContent(25,1.834349);
	h_RelUnc_Stat1->SetBinContent(26,1.982299);
	h_RelUnc_Stat1->SetBinContent(27,2.299428);
	h_RelUnc_Stat1->SetBinContent(28,2.618774);
	h_RelUnc_Stat1->SetBinContent(29,2.892231);
	h_RelUnc_Stat1->SetBinContent(30,3.410538);
	h_RelUnc_Stat1->SetBinContent(31,3.389806);
	h_RelUnc_Stat1->SetBinContent(32,3.854085);
	h_RelUnc_Stat1->SetBinContent(33,4.438569);
	h_RelUnc_Stat1->SetBinContent(34,4.562812);
	h_RelUnc_Stat1->SetBinContent(35,5.361308);
	h_RelUnc_Stat1->SetBinContent(36,10.39859);
	h_RelUnc_Stat1->SetBinContent(37,10.46755);
	h_RelUnc_Stat1->SetBinContent(38,11.69718);
	h_RelUnc_Stat1->SetBinContent(39,14.64011);
	h_RelUnc_Stat1->SetBinContent(40,27.85197);
	h_RelUnc_Stat1->SetBinContent(41,25.96013);
	h_RelUnc_Stat1->SetBinContent(42,33.70234);
	h_RelUnc_Stat1->SetBinContent(43,156.2494);
	h_RelUnc_Stat1->SetMinimum(0.05);
	h_RelUnc_Stat1->SetMaximum(200);
	h_RelUnc_Stat1->SetEntries(43);
	h_RelUnc_Stat1->SetDirectory(0);
	h_RelUnc_Stat1->SetStats(0);

	return h_RelUnc_Stat1;
}

TH1D* h_RelSysUnc_Tot_Approval()
{
	Double_t xAxis8[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000}; 
	
	TH1D *h_RelSysUnc_Tot_Percent8 = new TH1D("h_RelSysUnc_Tot_Percent8","",43, xAxis8);
	h_RelSysUnc_Tot_Percent8->SetBinContent(1,3.224451);
	h_RelSysUnc_Tot_Percent8->SetBinContent(2,3.072625);
	h_RelSysUnc_Tot_Percent8->SetBinContent(3,3.069516);
	h_RelSysUnc_Tot_Percent8->SetBinContent(4,3.471512);
	h_RelSysUnc_Tot_Percent8->SetBinContent(5,3.558465);
	h_RelSysUnc_Tot_Percent8->SetBinContent(6,3.27274);
	h_RelSysUnc_Tot_Percent8->SetBinContent(7,2.802166);
	h_RelSysUnc_Tot_Percent8->SetBinContent(8,2.42309);
	h_RelSysUnc_Tot_Percent8->SetBinContent(9,2.242432);
	h_RelSysUnc_Tot_Percent8->SetBinContent(10,2.04532);
	h_RelSysUnc_Tot_Percent8->SetBinContent(11,1.914024);
	h_RelSysUnc_Tot_Percent8->SetBinContent(12,1.919396);
	h_RelSysUnc_Tot_Percent8->SetBinContent(13,1.913824);
	h_RelSysUnc_Tot_Percent8->SetBinContent(14,1.954709);
	h_RelSysUnc_Tot_Percent8->SetBinContent(15,2.042235);
	h_RelSysUnc_Tot_Percent8->SetBinContent(16,2.235193);
	h_RelSysUnc_Tot_Percent8->SetBinContent(17,2.485082);
	h_RelSysUnc_Tot_Percent8->SetBinContent(18,2.75133);
	h_RelSysUnc_Tot_Percent8->SetBinContent(19,2.964565);
	h_RelSysUnc_Tot_Percent8->SetBinContent(20,3.133715);
	h_RelSysUnc_Tot_Percent8->SetBinContent(21,3.31449);
	h_RelSysUnc_Tot_Percent8->SetBinContent(22,3.539982);
	h_RelSysUnc_Tot_Percent8->SetBinContent(23,3.868392);
	h_RelSysUnc_Tot_Percent8->SetBinContent(24,4.248714);
	h_RelSysUnc_Tot_Percent8->SetBinContent(25,4.560511);
	h_RelSysUnc_Tot_Percent8->SetBinContent(26,4.926964);
	h_RelSysUnc_Tot_Percent8->SetBinContent(27,5.227268);
	h_RelSysUnc_Tot_Percent8->SetBinContent(28,5.623098);
	h_RelSysUnc_Tot_Percent8->SetBinContent(29,6.012916);
	h_RelSysUnc_Tot_Percent8->SetBinContent(30,6.506325);
	h_RelSysUnc_Tot_Percent8->SetBinContent(31,7.207104);
	h_RelSysUnc_Tot_Percent8->SetBinContent(32,8.108294);
	h_RelSysUnc_Tot_Percent8->SetBinContent(33,8.868253);
	h_RelSysUnc_Tot_Percent8->SetBinContent(34,9.274403);
	h_RelSysUnc_Tot_Percent8->SetBinContent(35,9.487618);
	h_RelSysUnc_Tot_Percent8->SetBinContent(36,9.80504);
	h_RelSysUnc_Tot_Percent8->SetBinContent(37,10.21479);
	h_RelSysUnc_Tot_Percent8->SetBinContent(38,10.73466);
	h_RelSysUnc_Tot_Percent8->SetBinContent(39,11.45183);
	h_RelSysUnc_Tot_Percent8->SetBinContent(40,21.84955);
	h_RelSysUnc_Tot_Percent8->SetBinContent(41,20.8704);
	h_RelSysUnc_Tot_Percent8->SetBinContent(42,20.8769);
	h_RelSysUnc_Tot_Percent8->SetBinContent(43,155.3213);
	h_RelSysUnc_Tot_Percent8->SetEntries(43);
	h_RelSysUnc_Tot_Percent8->SetStats(0);

	return h_RelSysUnc_Tot_Percent8;
}

TH1D* h_RelSystUnc_Bkg_Approval()
{
	Double_t xAxis6[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000}; 
	
	TH1D *h_SysUnc_BkgEst_Percent6 = new TH1D("h_SysUnc_BkgEst_Percent6","",43, xAxis6);
	h_SysUnc_BkgEst_Percent6->SetBinContent(1,0.2658695);
	h_SysUnc_BkgEst_Percent6->SetBinContent(2,0.5643172);
	h_SysUnc_BkgEst_Percent6->SetBinContent(3,0.4459663);
	h_SysUnc_BkgEst_Percent6->SetBinContent(4,0.8129181);
	h_SysUnc_BkgEst_Percent6->SetBinContent(5,0.9495295);
	h_SysUnc_BkgEst_Percent6->SetBinContent(6,1.286626);
	h_SysUnc_BkgEst_Percent6->SetBinContent(7,1.148725);
	h_SysUnc_BkgEst_Percent6->SetBinContent(8,1.076786);
	h_SysUnc_BkgEst_Percent6->SetBinContent(9,1.044924);
	h_SysUnc_BkgEst_Percent6->SetBinContent(10,0.7150794);
	h_SysUnc_BkgEst_Percent6->SetBinContent(11,0.5830048);
	h_SysUnc_BkgEst_Percent6->SetBinContent(12,0.467985);
	h_SysUnc_BkgEst_Percent6->SetBinContent(13,0.2405282);
	h_SysUnc_BkgEst_Percent6->SetBinContent(14,0.1444775);
	h_SysUnc_BkgEst_Percent6->SetBinContent(15,0.08442813);
	h_SysUnc_BkgEst_Percent6->SetBinContent(16,0.0617226);
	h_SysUnc_BkgEst_Percent6->SetBinContent(17,0.05978974);
	h_SysUnc_BkgEst_Percent6->SetBinContent(18,0.09884099);
	h_SysUnc_BkgEst_Percent6->SetBinContent(19,0.1763976);
	h_SysUnc_BkgEst_Percent6->SetBinContent(20,0.3897832);
	h_SysUnc_BkgEst_Percent6->SetBinContent(21,0.6213543);
	h_SysUnc_BkgEst_Percent6->SetBinContent(22,0.8893258);
	h_SysUnc_BkgEst_Percent6->SetBinContent(23,1.207879);
	h_SysUnc_BkgEst_Percent6->SetBinContent(24,1.575807);
	h_SysUnc_BkgEst_Percent6->SetBinContent(25,1.963388);
	h_SysUnc_BkgEst_Percent6->SetBinContent(26,2.327328);
	h_SysUnc_BkgEst_Percent6->SetBinContent(27,2.653608);
	h_SysUnc_BkgEst_Percent6->SetBinContent(28,3.015115);
	h_SysUnc_BkgEst_Percent6->SetBinContent(29,3.565207);
	h_SysUnc_BkgEst_Percent6->SetBinContent(30,4.420739);
	h_SysUnc_BkgEst_Percent6->SetBinContent(31,5.561632);
	h_SysUnc_BkgEst_Percent6->SetBinContent(32,6.745092);
	h_SysUnc_BkgEst_Percent6->SetBinContent(33,7.576121);
	h_SysUnc_BkgEst_Percent6->SetBinContent(34,7.886474);
	h_SysUnc_BkgEst_Percent6->SetBinContent(35,7.89326);
	h_SysUnc_BkgEst_Percent6->SetBinContent(36,7.850716);
	h_SysUnc_BkgEst_Percent6->SetBinContent(37,7.838234);
	h_SysUnc_BkgEst_Percent6->SetBinContent(38,7.97514);
	h_SysUnc_BkgEst_Percent6->SetBinContent(39,8.40143);
	h_SysUnc_BkgEst_Percent6->SetBinContent(40,8.990943);
	h_SysUnc_BkgEst_Percent6->SetBinContent(41,9.484063);
	h_SysUnc_BkgEst_Percent6->SetBinContent(42,9.788539);
	h_SysUnc_BkgEst_Percent6->SetBinContent(43,9.970763);
	h_SysUnc_BkgEst_Percent6->SetEntries(43);
	h_SysUnc_BkgEst_Percent6->SetDirectory(0);
	h_SysUnc_BkgEst_Percent6->SetStats(0);

	return h_SysUnc_BkgEst_Percent6;
}

TH1D* h_RelSystUnc_FSR_Approval()
{
	Double_t xAxis7[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000}; 
	
	TH1D *h_SysUnc_Tot_Percent7 = new TH1D("h_SysUnc_Tot_Percent7","",43, xAxis7);
	h_SysUnc_Tot_Percent7->SetBinContent(1,0.09232847);
	h_SysUnc_Tot_Percent7->SetBinContent(2,0.2340192);
	h_SysUnc_Tot_Percent7->SetBinContent(3,0.3425519);
	h_SysUnc_Tot_Percent7->SetBinContent(4,0.399062);
	h_SysUnc_Tot_Percent7->SetBinContent(5,0.41123);
	h_SysUnc_Tot_Percent7->SetBinContent(6,0.404618);
	h_SysUnc_Tot_Percent7->SetBinContent(7,0.4071649);
	h_SysUnc_Tot_Percent7->SetBinContent(8,0.4575625);
	h_SysUnc_Tot_Percent7->SetBinContent(9,0.5912842);
	h_SysUnc_Tot_Percent7->SetBinContent(10,0.8108223);
	h_SysUnc_Tot_Percent7->SetBinContent(11,1.077461);
	h_SysUnc_Tot_Percent7->SetBinContent(12,1.329345);
	h_SysUnc_Tot_Percent7->SetBinContent(13,1.51865);
	h_SysUnc_Tot_Percent7->SetBinContent(14,1.631775);
	h_SysUnc_Tot_Percent7->SetBinContent(15,1.687822);
	h_SysUnc_Tot_Percent7->SetBinContent(16,1.717953);
	h_SysUnc_Tot_Percent7->SetBinContent(17,1.737469);
	h_SysUnc_Tot_Percent7->SetBinContent(18,1.746098);
	h_SysUnc_Tot_Percent7->SetBinContent(19,1.740862);
	h_SysUnc_Tot_Percent7->SetBinContent(20,1.725565);
	h_SysUnc_Tot_Percent7->SetBinContent(21,1.71366);
	h_SysUnc_Tot_Percent7->SetBinContent(22,1.714312);
	h_SysUnc_Tot_Percent7->SetBinContent(23,1.744198);
	h_SysUnc_Tot_Percent7->SetBinContent(24,1.823223);
	h_SysUnc_Tot_Percent7->SetBinContent(25,1.93022);
	h_SysUnc_Tot_Percent7->SetBinContent(26,2.020611);
	h_SysUnc_Tot_Percent7->SetBinContent(27,2.0723);
	h_SysUnc_Tot_Percent7->SetBinContent(28,2.093973);
	h_SysUnc_Tot_Percent7->SetBinContent(29,2.124898);
	h_SysUnc_Tot_Percent7->SetBinContent(30,2.220379);
	h_SysUnc_Tot_Percent7->SetBinContent(31,2.416391);
	h_SysUnc_Tot_Percent7->SetBinContent(32,2.703542);
	h_SysUnc_Tot_Percent7->SetBinContent(33,3.015272);
	h_SysUnc_Tot_Percent7->SetBinContent(34,3.258457);
	h_SysUnc_Tot_Percent7->SetBinContent(35,3.392335);
	h_SysUnc_Tot_Percent7->SetBinContent(36,3.473064);
	h_SysUnc_Tot_Percent7->SetBinContent(37,3.611637);
	h_SysUnc_Tot_Percent7->SetBinContent(38,3.886487);
	h_SysUnc_Tot_Percent7->SetBinContent(39,4.296934);
	h_SysUnc_Tot_Percent7->SetBinContent(40,4.766461);
	h_SysUnc_Tot_Percent7->SetBinContent(41,5.199903);
	h_SysUnc_Tot_Percent7->SetBinContent(42,5.560824);
	h_SysUnc_Tot_Percent7->SetBinContent(43,5.873456);
	h_SysUnc_Tot_Percent7->SetEntries(86);
	h_SysUnc_Tot_Percent7->SetDirectory(0);
	h_SysUnc_Tot_Percent7->SetStats(0);

	return h_SysUnc_Tot_Percent7;
}

