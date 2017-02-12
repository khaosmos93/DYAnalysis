#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TRandom3.h>

#include <vector>

// -- for Rochester Muon momentum correction -- //
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/RoccoR.cc"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/rochcor2015.cc"

#include </home/kplee/Unfolding/src/RooUnfoldResponse.h>
#include </home/kplee/Unfolding/src/RooUnfoldBayes.h>
#include </home/kplee/Unfolding/src/RooUnfoldInvert.h>

#include "/home/kplee/CommonCodes/DrellYanAnalysis/MyCanvas.C"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/DYAnalyzer.h"
#include "/home/kplee/CommonCodes/DrellYanAnalysis/DiffXsecTools.h"


#define nMomCorrMap 500

class MyDiffXsecTool : public DiffXsecTools
{
public:
	MyDiffXsecTool(TString version, TString Ver_CMSSW) : DiffXsecTools(version, Ver_CMSSW)
	{

	}

	void GetYieldHistograms(TH1D* _h_yield_HLTv4p2, TH1D* _h_yield_HLTv4p3)
	{
		h_yield_HLTv4p2 = (TH1D*)_h_yield_HLTv4p2->Clone();
		h_yield_HLTv4p3 = (TH1D*)_h_yield_HLTv4p3->Clone();

		h_yield_Raw = (TH1D*)h_yield_HLTv4p2->Clone();
		h_yield_Raw->Add( h_yield_HLTv4p3 );
	}
};

class SysUncTool_DataMomCorr
{
public:
	TString version;
	TString Ver_CMSSW;
	TString FileLocation;
	Double_t MassBinEdges[nMassBin+1];

	TFile *f_output;

	Bool_t isDataDriven;

	TH1D *h_mass_TotalBkg_HLTv4p2;
	TH1D *h_mass_TotalBkg_HLTv4p3;

	TH1D *h_mass_HLTv4p2_CV;
	TH1D *h_mass_HLTv4p3_CV;

	TH1D *h_mass_HLTv4p2_Smeared[nMomCorrMap];
	TH1D *h_mass_HLTv4p3_Smeared[nMomCorrMap];

	TH1D *h_yield_HLTv4p2_CV;
	TH1D *h_yield_HLTv4p3_CV;

	TH1D *h_yield_HLTv4p2_Smeared[nMomCorrMap];
	TH1D *h_yield_HLTv4p3_Smeared[nMomCorrMap];

	TH1D *h_DiffXsec_CV;

	TH1D *h_DiffXsec_Smeared[nMomCorrMap];

	TH1D *h_RelDiff_massBin[nMassBin];

	// -- fiducial, post-fSR -- //
	TFile *f_FpoF_ouptut;
	TH1D *h_FpoF_DiffXsec_CV;
	TH1D *h_FpoF_DiffXsec_Smeared[nMomCorrMap];

	SysUncTool_DataMomCorr(TString _version, TString _Ver_CMSSW)
	{
		cout << "============================================" << endl;
		cout << "[Number of iteration: " << nMomCorrMap << "]" << endl;
		cout << "============================================\n" << endl;

		version = _version;
		Ver_CMSSW = _Ver_CMSSW;

		if( Ver_CMSSW == "74X" )
			FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles/"+version;
		else if( Ver_CMSSW == "76X" )
			FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/"+version;

		Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
											 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
											 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
											 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
											 830, 1000, 1500, 3000}; // -- Merging high-mass bins -- //

		for(Int_t i=0; i<nMassBin+1; i++)
			MassBinEdges[i] = MassBinEdges_temp[i];

		isDataDriven = kTRUE;
		cout << "Default value for isDataDriven: " << isDataDriven << endl;

		h_mass_TotalBkg_HLTv4p2 = new TH1D("h_mass_TotalBkg_HLTv4p2", "", nMassBin, MassBinEdges );
		h_mass_TotalBkg_HLTv4p3 = new TH1D("h_mass_TotalBkg_HLTv4p3", "", nMassBin, MassBinEdges );

		h_mass_HLTv4p2_CV = new TH1D("h_mass_HLTv4p2_CV", "", nMassBin, MassBinEdges);
		h_mass_HLTv4p3_CV = new TH1D("h_mass_HLTv4p3_CV", "", nMassBin, MassBinEdges);

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			h_mass_HLTv4p2_Smeared[i_corrMap] = new TH1D("h_mass_HLTv4p2_Smeared_"+TString::Format("%d", i_corrMap), "", nMassBin, MassBinEdges);
			h_mass_HLTv4p3_Smeared[i_corrMap] = new TH1D("h_mass_HLTv4p3_Smeared_"+TString::Format("%d", i_corrMap), "", nMassBin, MassBinEdges);
		}



		h_DiffXsec_CV = new TH1D("h_DiffXsec_CV", "", nMassBin, MassBinEdges);
		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			h_DiffXsec_Smeared[i_corrMap] = new TH1D("h_DiffXsec_Smeared_"+TString::Format("%d", i_corrMap), "", nMassBin, MassBinEdges);

		h_FpoF_DiffXsec_CV = new TH1D("h_FpoF_DiffXsec_CV", "", nMassBin, MassBinEdges);
		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			h_FpoF_DiffXsec_Smeared[i_corrMap] = new TH1D("h_FpoF_DiffXsec_Smeared_"+TString::Format("%d", i_corrMap), "", nMassBin, MassBinEdges);

		// -- Setting the h_RelDiff_massBin -- //
		for(Int_t i=0; i<nMassBin; i++)
			h_RelDiff_massBin[i] = new TH1D("h_RelDiff_massBin_"+TString::Format("%d", i), "", 10000, -5, 5);

		f_output = NULL;
		f_FpoF_ouptut = NULL;

	}

	void GetHistograms_fromROOTFile( TFile *file )
	{
		file->cd();

		h_mass_HLTv4p2_CV = (TH1D*)file->Get("h_mass_HLTv4p2_CV")->Clone();
		h_mass_HLTv4p3_CV = (TH1D*)file->Get("h_mass_HLTv4p3_CV")->Clone();

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			h_mass_HLTv4p2_Smeared[i_corrMap] = (TH1D*)file->Get( "h_mass_HLTv4p2_Smeared_"+TString::Format("%d", i_corrMap) )->Clone();
			h_mass_HLTv4p3_Smeared[i_corrMap] = (TH1D*)file->Get( "h_mass_HLTv4p3_Smeared_"+TString::Format("%d", i_corrMap) )->Clone();
		}

		cout << "Load the dimuon mass distribution from the input file" << endl;


		// -- Subtracting the backgrounds -- //
		DYAnalyzer *analyzer = new DYAnalyzer( "IsoMu20_OR_IsoTkMu20" );
		this->DYMassSpectrum_TotalBkg( analyzer, isDataDriven );

		h_yield_HLTv4p2_CV = GetHistogram_BkgSubtracted( "HLTv4p2", h_mass_HLTv4p2_CV );	
		h_yield_HLTv4p3_CV = GetHistogram_BkgSubtracted( "HLTv4p3", h_mass_HLTv4p3_CV );

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			h_yield_HLTv4p2_Smeared[i_corrMap] = GetHistogram_BkgSubtracted( "HLTv4p2", h_mass_HLTv4p2_Smeared[i_corrMap] );
			h_yield_HLTv4p3_Smeared[i_corrMap] = GetHistogram_BkgSubtracted( "HLTv4p3", h_mass_HLTv4p3_Smeared[i_corrMap] );
		}

		cout << "Subtracting the backgrounds is completed" << endl;
	}

	void SetIsDataDriven(Bool_t _isDataDriven)
	{
		isDataDriven = _isDataDriven;
		cout << "\n=================================" << endl;
		cout << "isDataDriven = " << isDataDriven << endl;
		cout << "=================================\n\n" << endl;

	}

	void Calc_DYMassSpectrum(TString HLTname = "IsoMu20_OR_IsoTkMu20")
	{
		cout << "===============================================================================" << endl;
		cout << "[Start the calculation of DY mass spectrum for each Smeared momentum correction]" << endl;
		cout << "===============================================================================" << endl;

		TTimeStamp ts_start;
		cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

		TStopwatch totaltime;
		totaltime.Start();

		DYAnalyzer *analyzer = new DYAnalyzer( HLTname );
		this->DYMassSpectrum_TotalBkg( analyzer, isDataDriven );

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add("/home/kplee/data/DYntuple/76X/v20160205_SingleMuon_RunC_Rereco_MuonPhys/*.root"); // -- Run2015C -- //
		chain->Add("/home/kplee/data/DYntuple/76X/v20160303_SingleMuon_RunD_Rereco_MuonPhys/*.root"); // -- Run2015D -- //

		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_Muon();

		rochcor2015 *rmcor_CV = new rochcor2015(); // -- for central value -- //

		rochcor2015 *rmcor_Smeared[nMomCorrMap];
		for(Int_t i_corrMap=0; i_corrMap < nMomCorrMap; i_corrMap++)
			rmcor_Smeared[i_corrMap] = new rochcor2015(i_corrMap); // -- Assign different random seed for each rochcor2015 object -- //

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
		// NEvents = 100000;
		for(Int_t i=0; i<NEvents; i++)
		{
			loadBar(i+1, NEvents, 100, 100);
			
			ntuple->GetEvent(i);

			// printf("[%d event]\n", i);

			if( ntuple->isTriggered( analyzer->HLT ))
			{
				// printf("\tPass the trigger condition\n");

				FillMassHistogram_GivenCorr(rmcor_CV, ntuple, analyzer, h_mass_HLTv4p2_CV, h_mass_HLTv4p3_CV);

				for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
					FillMassHistogram_GivenCorr(rmcor_Smeared[i_corrMap], ntuple, analyzer, h_mass_HLTv4p2_Smeared[i_corrMap], h_mass_HLTv4p3_Smeared[i_corrMap]);
			}

		} // -- end of event iteration -- //

		// // -- Subtracting the backgrounds -- //
		// h_yield_HLTv4p2_CV = (TH1D*)h_mass_HLTv4p2_CV->Clone(); 
		// h_yield_HLTv4p2_CV->SetName("h_yield_HLTv4p2_CV"); h_yield_HLTv4p2_CV->Sumw2();
		// h_yield_HLTv4p2_CV->Add( h_mass_TotalBkg_HLTv4p2, -1 );	
		// RemoveNegativeBins( h_yield_HLTv4p2_CV );

		// h_yield_HLTv4p3_CV = (TH1D*)h_mass_HLTv4p3_CV->Clone(); 
		// h_yield_HLTv4p3_CV->SetName("h_yield_HLTv4p3_CV"); h_yield_HLTv4p3_CV->Sumw2();
		// h_yield_HLTv4p3_CV->Add( h_mass_TotalBkg_HLTv4p3, -1 );	
		// RemoveNegativeBins( h_yield_HLTv4p3_CV );

		// for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		// {
		// 	h_yield_HLTv4p2_Smeared[i_corrMap] = (TH1D*)h_mass_HLTv4p2_Smeared[i_corrMap]->Clone(); 
		// 	h_yield_HLTv4p2_Smeared[i_corrMap]->SetName("h_yield_HLTv4p2_Smeared_"+TString::Format("%d", i_corrMap)); h_yield_HLTv4p2_Smeared[i_corrMap]->Sumw2();
		// 	h_yield_HLTv4p2_Smeared[i_corrMap]->Add( h_mass_TotalBkg_HLTv4p2, -1 );	
		// 	RemoveNegativeBins( h_yield_HLTv4p2_Smeared[i_corrMap] );

		// 	h_yield_HLTv4p3_Smeared[i_corrMap] = (TH1D*)h_mass_HLTv4p3_Smeared[i_corrMap]->Clone(); 
		// 	h_yield_HLTv4p3_Smeared[i_corrMap]->SetName("h_yield_HLTv4p3_Smeared_"+TString::Format("%d", i_corrMap)); h_yield_HLTv4p3_Smeared[i_corrMap]->Sumw2();
		// 	h_yield_HLTv4p3_Smeared[i_corrMap]->Add( h_mass_TotalBkg_HLTv4p3, -1 );	
		// 	RemoveNegativeBins( h_yield_HLTv4p3_Smeared[i_corrMap] );
		// }

		// -- Subtracting the backgrounds -- //
		h_yield_HLTv4p2_CV = GetHistogram_BkgSubtracted( "HLTv4p2", h_mass_HLTv4p2_CV );	
		h_yield_HLTv4p3_CV = GetHistogram_BkgSubtracted( "HLTv4p3", h_mass_HLTv4p3_CV );

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			h_yield_HLTv4p2_Smeared[i_corrMap] = GetHistogram_BkgSubtracted( "HLTv4p2", h_mass_HLTv4p2_Smeared[i_corrMap] );
			h_yield_HLTv4p3_Smeared[i_corrMap] = GetHistogram_BkgSubtracted( "HLTv4p3", h_mass_HLTv4p3_Smeared[i_corrMap] );
		}

		Double_t TotalRunTime = totaltime.CpuTime();
		cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

		TTimeStamp ts_end;
		cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
	}

	TH1D* GetHistogram_BkgSubtracted( TString Type, TH1D* h_mass )
	{
		TString HistName = h_mass->GetName();
		HistName.ReplaceAll("h_mass", "h_yield");

		TH1D* h_bkgSubtracted = (TH1D*)h_mass->Clone();
		h_bkgSubtracted->SetName( HistName ); h_bkgSubtracted->Sumw2();

		if( Type == "HLTv4p2" )
			h_bkgSubtracted->Add( h_mass_TotalBkg_HLTv4p2, -1 );
		else if( Type == "HLTv4p3" )
			h_bkgSubtracted->Add( h_mass_TotalBkg_HLTv4p3, -1 );
		else
		{
			cout << "Wrong Type! ... HLTv4p2 or HLTv4p3" << endl;
			return NULL;
		}

		RemoveNegativeBins( h_bkgSubtracted );

		return h_bkgSubtracted;
	}

	void Calc_DiffXsec_All()
	{
		CalcDiffXsec_GivenYield(h_yield_HLTv4p2_CV, h_yield_HLTv4p3_CV, h_DiffXsec_CV);

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			CalcDiffXsec_GivenYield(h_yield_HLTv4p2_Smeared[i_corrMap], h_yield_HLTv4p3_Smeared[i_corrMap], h_DiffXsec_Smeared[i_corrMap]);
	}

	void Calc_SysUnc()
	{
		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			TH1D *h_diff_CV_Smeared = (TH1D*)h_DiffXsec_CV->Clone();
			h_diff_CV_Smeared->Add( h_DiffXsec_Smeared[i_corrMap], -1 ); // -- (Central value - Smeared value) -- //

			for(Int_t i=0; i<nMassBin; i++)
			{
				Int_t i_bin = i+1;
				Double_t diff = h_diff_CV_Smeared->GetBinContent(i_bin);
				Double_t CentralValue = h_DiffXsec_CV->GetBinContent(i_bin);
				h_RelDiff_massBin[i]->Fill( diff / CentralValue );

				printf("\t[%d mass bin] RelDiff = %9.6e\n", i_bin, diff / CentralValue);
			}
		}
	}

	void ValidationPlots()
	{
		TString BkgType = "";
		
		if( isDataDriven ) 
			BkgType = "_DataDrivenBkg";
		else
			BkgType = "_MCBasedBkg";

		TFile *f_cv_yield = TFile::Open(FileLocation+"/ROOTFile_YieldHistogram.root"); f_cv_yield->cd();
		TH1D *h_yield_HLTv4p2_ref = (TH1D*)f_cv_yield->Get("h_yield_OS_HLTv4p2"+BkgType)->Clone();
		TH1D *h_yield_HLTv4p3_ref = (TH1D*)f_cv_yield->Get("h_yield_OS_HLTv4p3"+BkgType)->Clone();

		MyCanvas *myc_yield_HLTv4p2 = new MyCanvas("c_yield_HLTv4p2_Ref_vs_CV", "Dimuon Mass (HLT v4.2) [GeV]", "Number of events");
		myc_yield_HLTv4p2->SetLogx(1);
		myc_yield_HLTv4p2->SetLogy(0);
		myc_yield_HLTv4p2->SetRatioRange(0.9, 1.1);
		myc_yield_HLTv4p2->CanvasWithHistogramsRatioPlot(h_yield_HLTv4p2_CV, h_yield_HLTv4p2_ref, "from SysUncTool", "Reference", "SysUncTool/Ref");
		myc_yield_HLTv4p2->PrintCanvas();

		MyCanvas *myc_yield_HLTv4p3 = new MyCanvas("c_yield_HLTv4p3_Ref_vs_CV", "Dimuon Mass (HLT v4.3) [GeV]", "Number of events");
		myc_yield_HLTv4p3->SetLogx(1);
		myc_yield_HLTv4p3->SetLogy(0);
		myc_yield_HLTv4p3->SetRatioRange(0.9, 1.1);
		myc_yield_HLTv4p3->CanvasWithHistogramsRatioPlot(h_yield_HLTv4p3_CV, h_yield_HLTv4p3_ref, "from SysUncTool", "Reference", "SysUncTool/Ref");
		myc_yield_HLTv4p3->PrintCanvas();

		TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_cv->cd();
		TH1D *h_DiffXsec_Ref = (TH1D*)f_cv->Get("h_DiffXsec_FSRCorr")->Clone();

		MyCanvas *myc_DiffXsec = new MyCanvas("c_DiffXsec_Ref_vs_CV", "Dimuon Mass [GeV]", "d#sigma/dM [GeV]");
		myc_DiffXsec->SetLogx(1);
		myc_DiffXsec->SetLogy(0);
		myc_DiffXsec->SetRatioRange(0.9, 1.1);
		myc_DiffXsec->CanvasWithHistogramsRatioPlot(h_DiffXsec_Ref, h_DiffXsec_CV, "from SysUncTool", "Reference", "SysUncTool/Ref");
		myc_DiffXsec->PrintCanvas();

		if( f_output == NULL )
			f_output = new TFile("ROOTFile_Histograms_SysUnc_DataMomCorr.root", "RECREATE");
		f_output->cd();
		myc_yield_HLTv4p2->c->Write();
		myc_yield_HLTv4p3->c->Write();
		myc_DiffXsec->c->Write();
	}

	void SaveResults()
	{
		if( f_output == NULL )
			f_output = new TFile("ROOTFile_Histograms_SysUnc_DataMomCorr.root", "RECREATE");
		
		f_output->cd();

		h_mass_TotalBkg_HLTv4p2->Write();
		h_mass_TotalBkg_HLTv4p3->Write();

		h_mass_HLTv4p2_CV->Write();
		h_mass_HLTv4p3_CV->Write();

		h_yield_HLTv4p2_CV->Write();
		h_yield_HLTv4p3_CV->Write();

		h_DiffXsec_CV->Write();

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			h_mass_HLTv4p2_Smeared[i_corrMap]->Write();
			h_mass_HLTv4p3_Smeared[i_corrMap]->Write();

			h_yield_HLTv4p2_Smeared[i_corrMap]->Write();
			h_yield_HLTv4p3_Smeared[i_corrMap]->Write();

			h_DiffXsec_Smeared[i_corrMap]->Write();
		}

		for(Int_t i=0; i<nMassBin; i++)
			h_RelDiff_massBin[i]->Write();
	}

	void CalcDiffXsec_GivenYield(TH1D *h_yield_HLTv4p2, TH1D *h_yield_HLTv4p3, TH1D* h_DiffXsec)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
		XsecTool->MakeSignalMCHistograms();

		XsecTool->GetYieldHistograms(h_yield_HLTv4p2, h_yield_HLTv4p3);

		XsecTool->UnfoldingCorrection();
		XsecTool->AccEffCorrection();
		XsecTool->EfficiencyScaleFactor();
		XsecTool->FSRCorrection();
		XsecTool->CalcXsec();

		CopyHistogram( XsecTool->h_DiffXsec_FSRCorr, h_DiffXsec );
	}

	void CopyHistogram( TH1D* h_Original, TH1D* h_Empty )
	{
		Int_t nBin = h_Original->GetNbinsX();
		Int_t nBin2 = h_Empty->GetNbinsX();

		if( nBin != nBin2 )
		{
			printf( "[ERROR] # bins of DiffXsec histogram between two histograms! ... (nBin1, nBin2) = (%d, %d)\n", nBin, nBin2 );
			return;
		}

		for(Int_t i=-1; i<nBin+1; i++) // -- including under/overflow -- //
		{
			Int_t i_bin = i+1;
			Double_t value = h_Original->GetBinContent(i_bin);
			Double_t error = h_Original->GetBinError(i_bin);

			h_Empty->SetBinContent(i_bin, value);
			h_Empty->SetBinError(i_bin, error);
		}
	}

	void RemoveNegativeBins( TH1D* h_yield )
	{
		// -- Remove negative bins -- //
		for(Int_t i=0; i<h_yield->GetNbinsX(); i++)
		{
			Double_t content = h_yield->GetBinContent(i+1);

			if( content < 0 )
			{
				h_yield->SetBinContent(i+1, 0);
				h_yield->SetBinError(i+1, 0);
			}
		}
	}

	void FillMassHistogram_GivenCorr(rochcor2015* rmcor, NtupleHandle *ntuple, DYAnalyzer *analyzer, TH1D *h_mass_HLTv4p2, TH1D *h_mass_HLTv4p3)
	{
		//Collect Reconstruction level information
		vector< Muon > MuonCollection;
		Int_t NLeptons = ntuple->nMuon;
		for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
		{
			Muon mu;
			mu.FillFromNtuple(ntuple, i_reco);
			// -- Apply Rochester momentum scale correction -- //
			float qter = 1.0;
			rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);

			// -- Change Muon pT, eta and phi with updated(corrected) one -- //
			mu.Pt = mu.Momentum.Pt();
			mu.eta = mu.Momentum.Eta();
			mu.phi = mu.Momentum.Phi();
			
			MuonCollection.push_back( mu );
		}

		// -- Event Selection -- //
		vector< Muon > SelectedMuonCollection;
		Bool_t isPassEventSelection = kFALSE;
		isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

		if( isPassEventSelection == kTRUE )
		{
			Muon mu1 = SelectedMuonCollection[0];
			Muon mu2 = SelectedMuonCollection[1];

			Double_t reco_M = (mu1.Momentum + mu2.Momentum).M();
			if( ntuple->runNum < 257933 )
				h_mass_HLTv4p2->Fill( reco_M );
			else
				h_mass_HLTv4p3->Fill( reco_M );
		}
	}

	void DYMassSpectrum_TotalBkg( DYAnalyzer *analyzer, Bool_t isDataDriven)
	{
		vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // --76X -- //

		TFile *f_input = TFile::Open(FileLocation+"/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root");
		f_input->cd();

		if( isDataDriven == kFALSE ) // -- MC-based backgrounds -- //
		{
			Int_t nTag = Tag.size();
			for(Int_t i_tag=0; i_tag<nTag; i_tag++)
			{
				if( !Tag[i_tag].Contains("DYMuMu") ) // -- background events -- //
				{
					TH1D *h_mass_bkg = (TH1D*)f_input->Get( "h_mass_OS_"+Tag[i_tag] )->Clone();
					h_mass_bkg = (TH1D*)h_mass_bkg->Rebin(nMassBin, h_mass_bkg->GetName(), MassBinEdges); // -- Rebin -- //

					TH1D *h_mass_bkg_HLTv4p2 = (TH1D*)h_mass_bkg->Clone();
					TH1D *h_mass_bkg_HLTv4p3 = (TH1D*)h_mass_bkg->Clone();

					// -- Normalization -- //
					Double_t Norm_HLTv4p2 = (Lumi_HLTv4p2 * Xsec[i_tag]) / nEvents[i_tag];
					h_mass_bkg_HLTv4p2->Scale( Norm_HLTv4p2 );

					Double_t Norm_HLTv4p3 = ( (Lumi - Lumi_HLTv4p2) * Xsec[i_tag] ) / nEvents[i_tag];
					h_mass_bkg_HLTv4p3->Scale( Norm_HLTv4p3 );

					h_mass_TotalBkg_HLTv4p2->Add( h_mass_bkg_HLTv4p2 );
					h_mass_TotalBkg_HLTv4p3->Add( h_mass_bkg_HLTv4p3 );
				}
			}
		}
		else if( isDataDriven == kTRUE )
		{
			TFile *f_input_bkg_dataDriven = new TFile(FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
			TH1D *h_diJet_FR = (TH1D*)f_input_bkg_dataDriven->Get("dijet")->Clone();
			TH1D *h_WJets_FR = (TH1D*)f_input_bkg_dataDriven->Get("wjets")->Clone();
			TH1D *h_ttbar_emu = (TH1D*)f_input_bkg_dataDriven->Get("ttbar")->Clone();
			TH1D *h_tW_emu = (TH1D*)f_input_bkg_dataDriven->Get("tW")->Clone();
			TH1D *h_DYTauTau_emu = (TH1D*)f_input_bkg_dataDriven->Get("DYtautau")->Clone();
			TH1D *h_diboson_emu = (TH1D*)f_input_bkg_dataDriven->Get("diboson")->Clone();

			TH1D *h_mass_bkgs_dataDriven = (TH1D*)h_diJet_FR->Clone();
			h_mass_bkgs_dataDriven->Add( h_WJets_FR );
			h_mass_bkgs_dataDriven->Add( h_ttbar_emu );
			h_mass_bkgs_dataDriven->Add( h_tW_emu );
			h_mass_bkgs_dataDriven->Add( h_DYTauTau_emu );
			h_mass_bkgs_dataDriven->Add( h_diboson_emu );

			TH1D *h_mass_bkgs_dataDriven_HLTv4p2 = (TH1D*)h_mass_bkgs_dataDriven->Clone();
			Double_t NormFactor_HLTv4p2 = Lumi_HLTv4p2 / Lumi;
			h_mass_bkgs_dataDriven_HLTv4p2->Scale( NormFactor_HLTv4p2 );
			h_mass_TotalBkg_HLTv4p2->Add( h_mass_bkgs_dataDriven_HLTv4p2 ); // - Add the background (HLTv4.2) -- //

			TH1D *h_mass_bkgs_dataDriven_HLTv4p3 = (TH1D*)h_mass_bkgs_dataDriven->Clone();
			Double_t NormFactor_HLTv4p3 = (Lumi - Lumi_HLTv4p2) / Lumi;
			h_mass_bkgs_dataDriven_HLTv4p3->Scale( NormFactor_HLTv4p3 );
			h_mass_TotalBkg_HLTv4p3->Add( h_mass_bkgs_dataDriven_HLTv4p3 ); // - Add the background (HLTv4.3) -- //


		} // -- end of else if( isDataDriven == kTRUE ) -- //
	}

	static inline void loadBar(int x, int n, int r, int w)
	{
	    // Only update r times.
	    if( x == n )
	    	cout << endl;

	    if ( x % (n/r +1) != 0 ) return;

	 
	    // Calculuate the ratio of complete-to-incomplete.
	    float ratio = x/(float)n;
	    int   c     = ratio * w;
	 
	    // Show the percentage complete.
	    printf("%3d%% [", (int)(ratio*100) );
	 
	    // Show the load bar.
	    for (int x=0; x<c; x++) cout << "=";
	 
	    for (int x=c; x<w; x++) cout << " ";
	 
	    // ANSI Control codes to go back to the
	    // previous line and clear it.
		cout << "]\r" << flush;

	}

	void FpoF_Calc_DiffXsec_All()
	{
		printf("[Calculation for the central value]\n");
		this->FpoF_CalcDiffXsec_GivenYield(h_yield_HLTv4p2_CV, h_yield_HLTv4p3_CV, h_FpoF_DiffXsec_CV);

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
		{
			printf("[Calculation for the %d-th smeared value]\n", i_corrMap);
			this->FpoF_CalcDiffXsec_GivenYield(h_yield_HLTv4p2_Smeared[i_corrMap], h_yield_HLTv4p3_Smeared[i_corrMap], h_FpoF_DiffXsec_Smeared[i_corrMap]);
		}
	}

	void FpoF_CalcDiffXsec_GivenYield(TH1D *h_yield_HLTv4p2, TH1D *h_yield_HLTv4p3, TH1D* h_FpoF_DiffXsec)
	{
		MyDiffXsecTool *XsecTool = new MyDiffXsecTool(version, Ver_CMSSW);
		XsecTool->MakeSignalMCHistograms();
		XsecTool->GetYieldHistograms(h_yield_HLTv4p2, h_yield_HLTv4p3);
		XsecTool->UnfoldingCorrection();
		// -- just for calculating efficiency scale factor ... yield after acc*eff*effcorr results will not be used for fiducial,post-FSR result -- //
		// XsecTool->AccEffCorrection();
		// XsecTool->EfficiencyScaleFactor();
		// -- //

		XsecTool->FpoF_GetTheoryHist();
		XsecTool->FpoF_EffCorrection();
		XsecTool->FpoF_EfficiencyScaleFactor();
		XsecTool->FpoF_CalcXsec();

		CopyHistogram( XsecTool->h_FpoF_DiffXsec_Data, h_FpoF_DiffXsec );
	}

	void FpoF_SaveResults()
	{
		if( f_FpoF_ouptut == NULL )
			f_FpoF_ouptut = new TFile("ROOTFile_FpoF_Histograms_SysUnc_DataMomCorr.root", "RECREATE");
		
		f_FpoF_ouptut->cd();

		h_FpoF_DiffXsec_CV->Write();

		for(Int_t i_corrMap=0; i_corrMap<nMomCorrMap; i_corrMap++)
			h_FpoF_DiffXsec_Smeared[i_corrMap]->Write();
	}

	void FpoF_ValidationPlot()
	{
		TString BkgType = "";
		
		if( isDataDriven ) 
			BkgType = "_DataDrivenBkg";
		else
			BkgType = "_MCBasedBkg";

		TFile *f_cv = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root"); f_cv->cd();
		TH1D *h_FpoF_DiffXsec_Ref = (TH1D*)f_cv->Get("h_FpoF_DiffXsec_Data")->Clone();

		MyCanvas *myc_DiffXsec = new MyCanvas("c_FpoF_DiffXsec_Ref_vs_CV", "Dimuon Mass (fiducial, post-FSR)[GeV]", "d#sigma/dM [GeV]");
		myc_DiffXsec->SetLogx(1);
		myc_DiffXsec->SetLogy(0);
		myc_DiffXsec->SetRatioRange(0.9, 1.1);
		myc_DiffXsec->CanvasWithHistogramsRatioPlot(h_FpoF_DiffXsec_Ref, h_FpoF_DiffXsec_CV, "from SysUncTool", "Reference", "SysUncTool/Ref");
		myc_DiffXsec->PrintCanvas();

		if( f_FpoF_ouptut == NULL )
			f_FpoF_ouptut = new TFile("ROOTFile_FpoF_Histograms_SysUnc_DataMomCorr.root", "RECREATE");

		f_FpoF_ouptut->cd();
		myc_DiffXsec->c->Write();
	}

};


