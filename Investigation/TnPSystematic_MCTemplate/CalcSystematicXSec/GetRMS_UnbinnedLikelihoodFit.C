#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"

using namespace RooFit;

#define nMassBin 43
#define nMap 500

TTree* makeTTree(TFile *f_input, Int_t i_bin);

void GetRMS_UnbinnedLikelihoodFit()
{
	TFile *f_input = (TFile*)gROOT->GetListOfFiles()->At(0);

	TString Type = "";
	TString FileName = f_input->GetName();
	if( FileName.Contains("DataMomCorr") )
		Type = "DataMomCorr";
	else if( FileName.Contains("MCMomCorr") )
		Type = "MCMomCorr";
	else if( FileName.Contains("EffCorr") )
		Type = "EffSF_TagProbe";

	TString OutputFileName = "ROOTFile_SysUnc_"+Type+"_UnbinnedFit.root";

	if( FileName.Contains("FpoF") )
		OutputFileName.ReplaceAll("SysUnc_", "FpoF_SysUnc_");

	TFile *f_output = new TFile(OutputFileName, "RECREATE");

	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //

	TH1D *h_RMS = new TH1D("h_RMS", "", nMassBin, MassBinEdges);
	TH1D *h_mean = new TH1D("h_mean", "", nMassBin, MassBinEdges);

	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;

		TTree *tree = makeTTree(f_input, i_bin);

		RooRealVar RelDiff("RelDiff","(#sigma_{Central} - #sigma_{Smeared}) / #sigma_{Central}", -0.1, 0.1);
		RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

		// Make plot of binned dataset showing Poisson error bars (RooFit default)
		RooPlot* frame = RelDiff.frame(Title("Relative difference between central and smeared value in "+TString::Format("%d", i_bin)+"th mass bin") );
		
		// Fit a Gaussian p.d.f to the data
		RooRealVar mean("mean", "mean", 0, -0.5, 0.5) ;
		RooRealVar sigma("sigma", "sigma", 0.01, 0.0001, 1);
		RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
		gauss.fitTo(data);

		data.plotOn(frame, Binning(50));
		gauss.plotOn(frame);

		TString CanvasName = "c_RelDiff_" + Type + TString::Format("_%d", i_bin) + "bin";
		if( FileName.Contains("FpoF") )
			CanvasName.ReplaceAll("RelDiff_", "FpoF_RelDiff_");

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
	}

	f_output->cd();
	h_RMS->Write();
	h_mean->Write();

}

TTree* makeTTree(TFile *f_input, Int_t i_bin) 
{
	TTree* tree = new TTree("tree","tree");

	Double_t* RelDiff = new Double_t;

	tree->Branch("RelDiff", RelDiff, "RelDiff/D");

	TString HistName = "h_DiffXsec";
	TString FileName = f_input->GetName();
	if( FileName.Contains("FpoF") )
		HistName = "h_FpoF_DiffXsec";

	f_input->cd();
	TH1D *h_DiffXsec_CV = (TH1D*)f_input->Get(HistName+"_CV")->Clone();
	Double_t value_CV = h_DiffXsec_CV->GetBinContent(i_bin);

	for(Int_t i=0; i<nMap; i++)
	{
		TH1D *h_DiffXsec_Smeared = (TH1D*)f_input->Get( HistName+"_Smeared_"+TString::Format("%d", i) );
		Double_t value_Smeared = h_DiffXsec_Smeared->GetBinContent(i_bin);

		*RelDiff = ( value_CV - value_Smeared ) / value_CV;

		// printf("[\t%d th RelDiff = %lf]\n", i, *RelDiff);

		tree->Fill();
	}

	return tree;
}