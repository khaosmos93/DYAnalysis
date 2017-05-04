#include <Include/PlotTools.h>
#define nWeight 111

void CrossCheck_WithRef( TGraphAsymmErrors* g_Acc );
void makeTTree(TGraphAsymmErrors *g_Acc, TGraphAsymmErrors* g_Acc_Weighted[nWeight], Int_t i_point);

void Calc_PDFUnc()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
	printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	TString FileName = "../ROOTFile_Hists_ForPDFUnc_aMCNLO.root";
	TGraphAsymmErrors *g_Acc = Get_Graph(FileName, "g_Acc"); // -- central value -- //
	CrossCheck_WithRef( g_Acc );

	TGraphAsymmErrors *g_Acc_Weighted[nWeight];
	for(Int_t i=0; i<nWeight; i++)
	{
		TString TStr_Number = TString::Format("%03d", i);
		g_Acc_Weighted[i] = Get_Graph(FileName, "g_Acc_"+TStr_Number);
	}

	const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {
		15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
		64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
		110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
		200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
		830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //

	TFile *f_output = TFile::Open("ROOTFile_PDFUnc_aMCNLO.root", "RECREATE");
	TH1D *h_RMS = new TH1D("h_RMS", "", nMassBin, MassBinEdges);
	TH1D *h_mean = new TH1D("h_mean", "", nMassBin, MassBinEdges);

	// -- iteration for each mass bin -- //
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;

		TTree *tree = makeTTree(g_Acc, g_Acc_Weighted, i);

		RooRealVar RelDiff("RelDiff","(Acc_{Central} - Acc_{Weighted}) / Acc_{Central}", -0.1, 0.1);
		RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

		// Make plot of binned dataset showing Poisson error bars (RooFit default)
		RooPlot* frame = RelDiff.frame(Title("Relative difference between central and weighted value in "+TString::Format("%d", i_bin)+"th mass bin") );
		
		// Fit a Gaussian p.d.f to the data
		RooRealVar mean("mean", "mean", 0, -0.5, 0.5) ;
		RooRealVar sigma("sigma", "sigma", 0.01, 0.0001, 1);
		RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
		gauss.fitTo(data);

		data.plotOn(frame, Binning(50));
		gauss.plotOn(frame);

		TString CanvasName = "c_RelDiff_Acc" + TString::Format("_%d", i_bin) + "bin";

		TCanvas *c = new TCanvas(CanvasName, "", 800, 800); c->cd();
		frame->Draw();

		c->SaveAs(CanvasName+".pdf");
		
		f_output->cd();
		c->Write();

		Double_t RMS = sigma.getVal();

		h_RMS->SetBinContent(i_bin, RMS);
		h_RMS->SetBinError(i_bin, 0); // -- how to get the error of the sigma? -- //

		Double_t Mean = mean.getVal();
		h_mean->SetBinContent(i_bin, Mean);
		h_mean->SetBinError(i_bin, 0);
	} // -- end of bin iteration -- //

	f_output->cd();
	h_RMS->Write();
	h_mean->Write();
}

void makeTTree(TGraphAsymmErrors *g_Acc, TGraphAsymmErrors* g_Acc_Weighted[nWeight], Int_t i_point)
{
	TTree* tree = new TTree("tree","tree");

	Double_t* RelDiff = new Double_t;

	tree->Branch("RelDiff", RelDiff, "RelDiff/D");

	Double_t x_CV, y_CV;
	g_Acc->GetPoint(i_point, x_CV, y_CV);

	for(Int_t i_weight=0; i_weight<nWeight; i_weight++)
	{
		Double_t x_Weighted, y_Weighted;
		g_Acc_Weighted[i_weight]->GetPoint( i_point, x_Weighted, y_Weighted );

		*RelDiff = ( y_Weighted - y_CV ) / y_CV;

		// printf("[\t%d th RelDiff = %lf]\n", i, *RelDiff);

		tree->Fill();
	}

	return tree;
}

void CrossCheck_WithRef( TGraphAsymmErrors* g_Acc )
{
	TString ROOTFilePath = gSystem->Getenv("KP_ROOTFILE_PATH");
	TString FileName_Ref = ROOTFilePath + "/ROOTFile_AccEff.root";
	TGraphAsymmErrors* g_Acc_Ref = Get_Graph( FileName_Ref, "g_Acc" );

	GraphInfo *Graph_Ref = new GraphInfo( kRed, "Reference" );
	Graph_Ref->Set_Graph( g_Acc_Ref );

	GraphInfo *Graph_CV = new GraphInfo( kBlack, "Central value" );
	Graph_CV->Set_Graph( g_Acc );
	Graph_CV->CalcRatio_DEN( Graph_Ref->g );

	TCanvas *c; TPad *TopPad; TPad *BottomPad;
	SetCanvas_Ratio( c, "Comparison_WithRef", TopPad, BottomPad, 1, 0 );

	c->cd();
	TopPad->cd();
	Graph_CV->Draw("APSAME");
	Graph_Ref->Draw("PSAME");

	SetGraphFormat_TopPad( Graph_CV->g, "Acceptance" );

	TLegend *legend;
	SetLegend( legend );
	Graph_CV->AddToEntry( legend );
	Graph_Ref->AddToEntry( legend );

	c->cd();
	BottomPad->cd();

	Graph_CV->DrawGraph("APSAME");
	SetGraphFormat_BottomPad( Graph_CV->g_ratio, "m [GeV]", "CV/Ref" );

	TF1 *f_line;
	DrawLine( f_line );

	c->SaveAs(".pdf");

	cout << "Check the detail numbers in ratio: is it really 1 or not?" << endl;
	Bool_t isFound = kFALSE;
	for(Int_t i=0; Graph_CV->g_ratio->GetN(); i++)
	{
		Double_t x, y;
		Graph_CV->g_ratio->GetPoint( i, x, y );

		if( y != 1.0 )
		{
			isFound = kTRUE;
			printf("[x = %lf]: ratio = %lf != 1.0\n", x, y );
		}
	}

	if( !isFound )
		cout << "it Passes the test: consistent with the previous result" << endl;
}