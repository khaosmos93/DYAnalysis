#include <Include/PlotTools.h>

void MakeHistogram_Acc(TH1D *h, Double_t *value, Double_t *error);
void PDFUnc_PDF4LHC15()
{
	const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};
	Double_t Acc_NLO[nMassBin+2];
	Double_t error_high_NLO[nMassBin+2];
	Double_t error_low_NLO[nMassBin+2];
	Double_t error_NLO[nMassBin+2];

	Double_t Acc_NNLO[nMassBin+2];
	Double_t error_high_NNLO[nMassBin+2];
	Double_t error_low_NNLO[nMassBin+2];
	Double_t error_NNLO[nMassBin+2];

	FILE *fl;
	Double_t LowerEdge;
	Double_t UpperEdge;
	Double_t Acc_NLO_temp;
	Double_t error_high_NLO_temp;
	Double_t error_low_NLO_temp;
	Double_t Acc_NNLO_temp;
	Double_t error_high_NNLO_temp;
	Double_t error_low_NNLO_temp;


	// -- https://twiki.cern.ch/twiki/pub/CMS/EWKDrellYan2015/dy_mm_13tev_noPI_NLO_WNLO_acc_sys_twiki.txt -- //
	if ((fl = fopen("AccErrors.txt", "r")) != NULL)
	{
		Int_t i=0;

		while(fscanf(fl, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", &LowerEdge, &UpperEdge, &Acc_NLO_temp, &error_high_NLO_temp, &error_low_NLO_temp, &Acc_NNLO_temp, &error_high_NNLO_temp, &error_low_NNLO_temp) != EOF )
		{
			Acc_NLO[i] = Acc_NLO_temp;
			error_high_NLO[i] = error_high_NLO_temp;
			error_low_NLO[i] = error_low_NLO_temp;

			if( error_high_NLO_temp > error_low_NLO_temp ) 
				error_NLO[i] = error_high_NLO_temp;
			else 
				error_NLO[i] = error_low_NLO_temp;
			
			Acc_NNLO[i] = Acc_NNLO_temp;
			error_high_NNLO[i] = error_high_NNLO_temp;
			error_low_NNLO[i] = error_low_NNLO_temp;

			if( error_high_NNLO_temp > error_low_NNLO_temp )
				error_NNLO[i] = error_high_NNLO_temp;
			else
				error_NNLO[i] = error_low_NNLO_temp;

			printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", LowerEdge, UpperEdge, Acc_NLO[i], error_high_NLO[i], error_low_NLO[i], Acc_NNLO[i], error_high_NNLO[i], error_low_NNLO[i]);
			i++;

			if( i == 45 )
				break;
		}
	}
	fclose(fl);

	TH1D *h_Acc_NLO = new TH1D("h_Acc_NLO", "", nMassBin, MassBinEdges);
	MakeHistogram_Acc( h_Acc_NLO, Acc_NLO, error_NLO);
	TH1D* h_RelSysUnc_NLO_Percent = Extract_RelUnc( h_Acc_NLO, "h_RelSysUnc_NLO_Percent", kTRUE );

	TH1D *h_Acc_NNLO = new TH1D("h_Acc_NNLO", "", nMassBin, MassBinEdges);
	MakeHistogram_Acc( h_Acc_NNLO, Acc_NNLO, error_NNLO);
	TH1D* h_RelSysUnc_NNLO_Percent = Extract_RelUnc( h_Acc_NNLO, "h_RelSysUnc_NNLO_Percent", kTRUE );

	// -- comparison of acceptance -- //
	HistInfo *Hist_Acc_NLO = new HistInfo( kGreen+1, "NLO" );
	Hist_Acc_NLO->Set_Histogram( h_Acc_NLO );
	Hist_Acc_NLO->Set();
	HistInfo *Hist_Acc_NNLO = new HistInfo( kBlue, "NNLO" );
	Hist_Acc_NNLO->Set_Histogram( h_Acc_NNLO );
	Hist_Acc_NNLO->Set();

	DrawCanvas_TwoHistRatio *canvas1 = new DrawCanvas_TwoHistRatio( "c_Acc_NLO_NNLO", Hist_Acc_NLO, Hist_Acc_NNLO );
	canvas1->SetTitle( "m [GeV]", "Acceptance", "NLO/NNLO" );
	canvas1->SetRatioRange(0.4, 1.2);
	canvas1->SetLegendPosition( 0.75, 0.32, 0.95, 0.45 );
	canvas1->SetLatex("Simulation");
	canvas1->Draw(1, 0);
	canvas1->c->cd();
	canvas1->TopPad->cd();
	canvas1->latex.DrawLatexNDC(0.17, 0.90, "#font[42]{#scale[0.8]{FEWZ (PDF4LHC15)}}");
	canvas1->latex.DrawLatexNDC(0.17, 0.86, "#font[42]{#scale[0.6]{Dressed level}}");
	canvas1->c->SaveAs(".pdf");

	// -- comparison of PDF uncertainty -- //
	HistInfo *Hist_Unc_NLO = new HistInfo( kGreen+1, "NLO" );
	Hist_Unc_NLO->Set_Histogram( h_RelSysUnc_NLO_Percent );
	Hist_Unc_NLO->Set();
	HistInfo *Hist_Unc_NNLO = new HistInfo( kBlue, "NNLO" );
	Hist_Unc_NNLO->Set_Histogram( h_RelSysUnc_NNLO_Percent );
	Hist_Unc_NNLO->Set();

	DrawCanvas_TwoHistRatio *canvas2 = new DrawCanvas_TwoHistRatio( "c_PDFUnc_NLO_NNLO", Hist_Unc_NLO, Hist_Unc_NNLO );
	canvas2->SetTitle( "m [GeV]", "PDF Uncertainty (%)", "NLO/NNLO" );
	canvas2->SetRatioRange(0.6, 1.4);
	canvas2->SetYRange(0, 7);
	canvas2->SetLegendPosition( 0.75, 0.32, 0.95, 0.45 );
	canvas2->SetLatex("Simulation");
	canvas2->SetDrawOption("HISTLPSAME");
	canvas2->Draw(1, 0);
	canvas2->c->cd();
	canvas2->TopPad->cd();
	canvas2->latex.DrawLatexNDC(0.17, 0.90, "#font[42]{#scale[0.8]{FEWZ (PDF4LHC15)}}");
	canvas2->latex.DrawLatexNDC(0.17, 0.86, "#font[42]{#scale[0.6]{Dressed level}}");
	canvas2->c->SaveAs(".pdf");

	TFile *f_output = TFile::Open("ROOTFile_AccWithUnc_FEWZ_PDF4LHC15.root", "RECREATE");
	f_output->cd();

	h_Acc_NLO->Write();
	h_Acc_NNLO->Write();
	h_RelSysUnc_NLO_Percent->Write();
	h_RelSysUnc_NNLO_Percent->Write();
}

void MakeHistogram_Acc(TH1D *h, Double_t *value, Double_t *error)
{
	Int_t nBin = h->GetNbinsX(); // -- nbin = 43 -- //
	for(Int_t i=0; i<nBin-2; i++)
	{
		Int_t i_bin = i+1;
		h->SetBinContent( i_bin, value[i] );
		h->SetBinError( i_bin, error[i] );
	}

	// -- [1000, 1200], [1200, 1500] -> [1000, 1500] -- //
	Double_t value_Bin42 = ( value[41]*200 + value[42]*300 ) / 500.0;
	Double_t error_Bin42 = sqrt( error[41]*200*error[41]*200  + error[42]*300*error[42]*300 ) / 500.0;

	h->SetBinContent( 42, value_Bin42 );
	h->SetBinError( 42, error_Bin42 );

	// -- [1500, 2000], [2000, 3000] -> [1500, 3000] -- //
	Double_t value_Bin43 = ( value[43]*500 + value[44]*1000 ) / 1500;
	Double_t error_Bin43 = sqrt( error[43]*500*error[43]*500 + error[44]*1000*error[44]*1000 ) / 1500.0;

	h->SetBinContent( 43, value_Bin43 );
	h->SetBinError( 43, error_Bin43 );
}