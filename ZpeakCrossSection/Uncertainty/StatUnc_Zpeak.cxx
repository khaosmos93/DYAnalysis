#include <SysUncEstimation/StatUnc/StatUnc_RandomizedYield.h>
#include <ZpeakCrossSection/ZpeakCrossSectionTool.h>

class MyZpeakXsecTool : public ZpeakCrossSectionTool
{
public:
	MyZpeakXsecTool(TString _HLTname) : ZpeakCrossSectionTool( _HLTname )
	{

	}

	void ObtainYield( TH1D* h_yield_HLTv4p2, TH1D* h_yield_HLTv4p3 )
	{
		this->isDataDriven = 1;

		f_data = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MuonPhys_MomCorr.root"); f_data->cd();
		TH1D *h_data = (TH1D*)f_data->Get("h_mass_OS_Data")->Clone();
		f_data->Close();
		
		this->nEvents_Observed = this->nEvents_Zpeak(h_data); // -- for statistical error -- //

		this->yield_HLTv4p2 = this->nEvents_Zpeak( h_yield_HLTv4p2 );
		this->yield_HLTv4p3 = this->nEvents_Zpeak( h_yield_HLTv4p3 );
		this->yield_total = this->yield_HLTv4p2 + this->yield_HLTv4p3;

		printf("[Yield] HLTv4p2: %.3lf, HLTv4p3: %.3lf, Total: %.3lf\n\n", this->yield_HLTv4p2, this->yield_HLTv4p3, this->yield_total);
	}
};

class StatUncTool_Zpeak
{
public:
	StatUncTool *tool;

	Double_t XSec_CV;
	Double_t XSec_Smeared[nMap];

	Double_t FpoF_XSec_CV;
	Double_t FpoF_XSec_Smeared[nMap];


	StatUncTool_Zpeak()
	{
		this->tool = new StatUncTool();

		// -- initialization -- //
		this->XSec_CV = 0;
		this->FpoF_XSec_CV = 0;
		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			this->XSec_Smeared[i_map] = 0;
			this->FpoF_XSec_Smeared[i_map] = 0;
		}
	}

	void Estimate()
	{
		tool->Hists_CV->Make_Histogram_BkgSub();
		this->CalcXSec_EachYield( tool->Hists_CV->h_BkgSub_4p2, tool->Hists_CV->h_BkgSub_4p3, this->XSec_CV, this->FpoF_XSec_CV );

		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			tool->Hists_Smeared[i_map]->Make_Histogram_BkgSub();
			this->CalcXSec_EachYield( tool->Hists_Smeared[i_map]->h_BkgSub_4p2, tool->Hists_Smeared[i_map]->h_BkgSub_4p3, this->XSec_Smeared[i_map], this->FpoF_XSec_Smeared[i_map] );
		}

		this->FitGaussian_GetSigma("All");
		this->FitGaussian_GetSigma("FpoF");
	}

protected:
	void FitGaussian_GetSigma( TString Type )
	{
		TTree *tree = this->makeTTree(Type);

		Double_t RangeMax = 0.01;
		Double_t Sigma_Init = 0.001;
		// Double_t BinCenter = ( MassBinEdges[i] + MassBinEdges[i+1] ) / 2.0;
		// this->Set_FitInitValues( BinCenter, RangeMax, Sigma_Init );

		RooRealVar RelDiff("RelDiff","(#sigma_{Smeared} - #sigma_{CV}) / #sigma_{CV}", (-1)*RangeMax, RangeMax);
		RooDataSet data("data","data", tree, RelDiff) ; // -- Name of the variable should be same with the branch name in the tree -- //

		// --- Make plot of binned dataset showing Poisson error bars (RooFit default)
		RooPlot* frame = RelDiff.frame( Title("60 < M < 120 GeV") );
		
		// -- Fit a Gaussian p.d.f to the data
		RooRealVar mean("mean", "mean", 0, -2, 2) ;
		RooRealVar sigma("sigma", "sigma", Sigma_Init, 0.0001, 2);
		RooGaussian gauss("gauss", "gauss", RelDiff, mean, sigma);
		gauss.fitTo(data);

		data.plotOn(frame, Binning(50));
		gauss.plotOn(frame);
		gauss.paramOn(frame,Layout(0.6, 0.9, 0.9));
		frame->getAttText()->SetTextSize(0.02);

		TString CanvasName = "c_RelDiff_Zpeak";
		if( Type == "FpoF" )
			CanvasName.ReplaceAll("c_", "c_FpoF_");

		TCanvas *c = new TCanvas(CanvasName, "", 700, 700); c->cd();
		frame->Draw();

		c->SaveAs(CanvasName+".pdf");
		
		Double_t RMS = sigma.getVal();
		// Double_t RMSErr = sigma.getError();

		Double_t value_CV = 0;
		if( Type == "All" )
			value_CV = this->XSec_CV;
		else if( Type == "FpoF" )
			value_CV = this->FpoF_XSec_CV;

		Double_t AbsStatUnc = value_CV * RMS;
		printf("[Statistical uncertainty (Type = %s) = %.3lf (%.3lf%%)]\n", Type.Data(), AbsStatUnc, RMS*100);
	}

	TTree* makeTTree( TString Type )
	{
		TTree* tree = new TTree("tree","tree");

		Double_t* RelDiff = new Double_t;

		tree->Branch("RelDiff", RelDiff, "RelDiff/D");

		////////////////////////////////////////////////

		Double_t value_CV = 0;
		if( Type == "All" )
			value_CV = this->XSec_CV;
		else if( Type == "FpoF" )
			value_CV = this->FpoF_XSec_CV;

		for(Int_t i_map=0; i_map<nMap; i_map++)
		{
			Double_t value_Smeared = 0;
			if( Type == "All" )
				value_Smeared = this->XSec_Smeared[i_map];
			else if( Type == "FpoF" )
				value_Smeared = this->FpoF_XSec_Smeared[i_map];

			*RelDiff = ( value_Smeared - value_CV ) / value_CV;

			// printf("[\t%d th RelDiff = %lf]\n", i, *RelDiff);

			tree->Fill();
		}

		return tree;
	}

	void CalcXSec_EachYield( TH1D* h_yield_HLTv4p2, TH1D* h_yield_HLTv4p3, Double_t &XSec, Double_t &FpoF_XSec )
	{
		MyZpeakXsecTool *XSecTool = new MyZpeakXsecTool( "IsoMu20_OR_IsoTkMu20" );
		XSecTool->ObtainYield( h_yield_HLTv4p2, h_yield_HLTv4p3 );

		// -- insert acceptance & efficinecy -- // 
		Double_t Acc = 0.453734;
		Double_t Eff = 0.914200;
		Double_t EffSF_HLTv4p2 = 0.958053;
		Double_t EffSF_HLTv4p3 = 0.964876;

		XSecTool->ApplyAccEffCorrection(Acc, Eff);
		XSecTool->EfficiencyScaleFactor(EffSF_HLTv4p2, EffSF_HLTv4p3);
		XSecTool->CalcXSec();
		XSecTool->FpoF_CalcXSec();

		XSec = XSecTool->xSec;
		FpoF_XSec = XSecTool->FpoF_xSec;
	}


};

void StatUnc_Zpeak()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");
	gSystem->cd( "./Local" );

	StatUncTool_Zpeak *tool = new StatUncTool_Zpeak();
	tool->Estimate();
}
