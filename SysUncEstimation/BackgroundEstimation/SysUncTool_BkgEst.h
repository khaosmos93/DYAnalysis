#include <DYAnalysis_76X/CommonCodes/DYAnalyzer_v02.h>

Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
									 830, 1000, 1500, 3000};

class HistogramContainer
{
public:
	TString Type;

	TH1D* h_nEvent;

	TH1D* h_AbsUnc_Stat;
	TH1D* h_AbsUnc_Syst;
	TH1D* h_AbsUnc_Tot;

	TH1D* h_RelUnc_Stat;
	TH1D* h_RelUnc_Syst;
	TH1D* h_RelUnc_Tot;

	HistogramContainer(TString _Type)
	{
		this->Type = _Type;

		this->h_nEvent = new TH1D( "h_nEvent_"+Type, "", nMassBin, MassBinEdges );

		this->h_AbsUnc_Stat = new TH1D( "h_AbsUnc_Stat_"+Type, "", nMassBin, MassBinEdges );
		this->h_AbsUnc_Syst = new TH1D( "h_AbsUnc_Syst_"+Type, "", nMassBin, MassBinEdges );
		this->h_AbsUnc_Tot = new TH1D( "h_AbsUnc_Tot_"+Type, "", nMassBin, MassBinEdges );

		this->h_RelUnc_Stat = new TH1D( "h_RelUnc_Stat_"+Type, "", nMassBin, MassBinEdges );
		this->h_RelUnc_Syst = new TH1D( "h_RelUnc_Syst_"+Type, "", nMassBin, MassBinEdges );
		this->h_RelUnc_Tot = new TH1D( "h_RelUnc_Tot_"+Type, "", nMassBin, MassBinEdges );
	}

	void Fill_RelUncHist_wrt_nUnfolded( TH1D* h_unfolded )
	{
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t nEvent_unfolded = h_unfolded->GetBinContent(i_bin);

			Double_t AbsUnc_Stat = this->h_AbsUnc_Stat->GetBinContent(i_bin);
			Double_t RelUnc_Stat = AbsUnc_Stat / nEvent_unfolded;
			this->h_RelUnc_Stat->SetBinContent(i_bin, RelUnc_Stat*100); // -- convert to % -- //
			this->h_RelUnc_Stat->SetBinError(i_bin, 0);

			Double_t AbsUnc_Syst = this->h_AbsUnc_Syst->GetBinContent(i_bin);
			Double_t RelUnc_Syst = AbsUnc_Syst / nEvent_unfolded;
			this->h_RelUnc_Syst->SetBinContent(i_bin, RelUnc_Syst*100); // -- convert to % -- //
			this->h_RelUnc_Syst->SetBinError(i_bin, 0);

			Double_t AbsUnc_Tot = this->h_AbsUnc_Tot->GetBinContent(i_bin);
			Double_t RelUnc_Tot = AbsUnc_Tot / nEvent_unfolded;
			this->h_RelUnc_Tot->SetBinContent(i_bin, RelUnc_Tot*100); // -- convert to % -- //
			this->h_RelUnc_Tot->SetBinError(i_bin, 0);
		}

		this->PrintHistogram();
	}

	void PrintHistogram()
	{
		printf("======================\n[%s]\n======================\n", Type.Data() );

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t nEvent = this->h_nEvent->GetBinContent(i_bin);

			Double_t AbsUnc_Stat = this->h_AbsUnc_Stat->GetBinContent(i_bin);
			Double_t AbsUnc_Syst = this->h_AbsUnc_Syst->GetBinContent(i_bin);
			Double_t AbsUnc_Tot = this->h_AbsUnc_Tot->GetBinContent(i_bin);

			Double_t RelUnc_Stat = this->h_RelUnc_Stat->GetBinContent(i_bin);
			Double_t RelUnc_Syst = this->h_RelUnc_Syst->GetBinContent(i_bin);
			Double_t RelUnc_Tot = this->h_RelUnc_Tot->GetBinContent(i_bin);

			printf("[%2d bin: nBkgEvent=%.2lf] (stat, syst, tot) = (%.2lf (%.2lf%%), %.2lf (%.2lf%%), %.2lf (%.2lf%%))\n", i_bin, nEvent, AbsUnc_Stat, RelUnc_Stat, AbsUnc_Syst, RelUnc_Syst, AbsUnc_Tot, RelUnc_Tot);
		}

		printf("======================\n\n");
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_nEvent->Write();

		this->h_AbsUnc_Stat->Write();
		this->h_AbsUnc_Syst->Write();
		this->h_AbsUnc_Tot->Write();

		this->h_RelUnc_Stat->Write();
		this->h_RelUnc_Syst->Write();
		this->h_RelUnc_Tot->Write();
	}
};


class SysUncTool_BkgEst
{
public:
	TString version;
	TString FileLocation;

	// Double_t MassBinEdges[nMassBin+1];

	TFile *f_output;

	TH1D *h_unfolded;

	vector< TString > ntupleDirectory; 
	vector< TString > Tag; 
	vector< Double_t > Xsec; 
	vector< Double_t > nEvents;

	// -- backgrounds -- //
	HistogramContainer* Hists_Total;

	vector< HistogramContainer* > Hists_Bkg;	
	HistogramContainer* Hists_ttbar;
	HistogramContainer* Hists_tW;
	HistogramContainer* Hists_DYtautau;
	HistogramContainer* Hists_WJets;
	HistogramContainer* Hists_QCD;
	HistogramContainer* Hists_WW;
	HistogramContainer* Hists_WZ;
	HistogramContainer* Hists_ZZ;

	// -- combined backgrouds with same estimation method -- //
	HistogramContainer* Hists_emu;
	HistogramContainer* Hists_FR;
	HistogramContainer* Hists_MC;

	SysUncTool_BkgEst(TString _version)
	{
		// Double_t MassBinEdges_temp[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
		// 									 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
		// 									 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
		// 									 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
		// 									 830, 1000, 1500, 3000};

		// for(Int_t i=0; i<nMassBin+1; i++)
		// 	MassBinEdges[i] = MassBinEdges_temp[i];

		version = _version;
		FileLocation = "/home/kplee/Physics/DYAnalysis_76X/CommonCodes/Results_ROOTFiles_76X/" + version;

		DYAnalyzer *analyzer = new DYAnalyzer( "None" );
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents); // -- 76X -- //

		this->f_output = new TFile("ROOTFile_SysUnc_BkgEst.root", "RECREATE");

		TH1::AddDirectory(kFALSE);

		this->Initialize_SystHistograms();

		this->SetupHistogram_Unfolded();
		this->SetupHistgram_MCBkg_All();
		this->SetupHistogram_DataDrivenBkg_All();
	}

	void SysUncEstimation()
	{
		this->MakeCombinedHistogram( this->Hists_Bkg, this->Hists_Total );

		// -- print out the details -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t nEvent_Unfold = h_unfolded->GetBinContent(i_bin);
			printf("[%2d bin]: nEvent_Unfold = %.2lf\n", i_bin, nEvent_Unfold);


			Int_t nHist = (Int_t)Hists_Bkg.size();
			for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			{
				Double_t AbsUnc_Stat = Hists_Bkg[i_hist]->h_AbsUnc_Stat->GetBinContent(i_bin);
				Double_t AbsUnc_Syst = Hists_Bkg[i_hist]->h_AbsUnc_Syst->GetBinContent(i_bin);
				Double_t AbsUnc_Tot = Hists_Bkg[i_hist]->h_AbsUnc_Tot->GetBinContent(i_bin);

				Double_t RelUnc_Stat = Hists_Bkg[i_hist]->h_RelUnc_Stat->GetBinContent(i_bin);
				Double_t RelUnc_Syst = Hists_Bkg[i_hist]->h_RelUnc_Syst->GetBinContent(i_bin);
				Double_t RelUnc_Tot = Hists_Bkg[i_hist]->h_RelUnc_Tot->GetBinContent(i_bin);

				TString Type = Hists_Bkg[i_hist]->Type;
				printf("\t[%10s] Uncertainty: (Stat, Syst, total) = (%.2lf (%.2lf%%), %.2lf (%.2lf%%), %.2lf (%.2lf%%))\n", Type.Data(), AbsUnc_Stat, RelUnc_Stat, AbsUnc_Syst, RelUnc_Syst, AbsUnc_Tot, RelUnc_Tot);
			}

			Double_t AbsUnc_Stat_TotBkg = Hists_Total->h_AbsUnc_Stat->GetBinContent(i_bin);
			Double_t AbsUnc_Syst_TotBkg = Hists_Total->h_AbsUnc_Syst->GetBinContent(i_bin);
			Double_t AbsUnc_Tot_TotBkg = Hists_Total->h_AbsUnc_Tot->GetBinContent(i_bin);

			Double_t RelUnc_Stat_TotBkg = Hists_Total->h_RelUnc_Stat->GetBinContent(i_bin);
			Double_t RelUnc_Syst_TotBkg = Hists_Total->h_RelUnc_Syst->GetBinContent(i_bin);
			Double_t RelUnc_Tot_TotBkg = Hists_Total->h_RelUnc_Tot->GetBinContent(i_bin);

			printf("[Total] Uncertainty: (Stat, Syst, total) = (%.2lf (%.2lf%%), %.2lf (%.2lf%%), %.2lf (%.2lf%%))\n\n", 
				AbsUnc_Stat_TotBkg, RelUnc_Stat_TotBkg, AbsUnc_Syst_TotBkg, RelUnc_Syst_TotBkg, AbsUnc_Tot_TotBkg, RelUnc_Tot_TotBkg);

		}
	}

	void MakeCanvas_ForEachBkgMethod()
	{
		this->MakeCombinedHistogram_byEstimationMethod();

		vector< TH1D* > Histos; vector< TString > Names;
		Histos.push_back( this->Hists_Total->h_RelUnc_Tot ); Names.push_back("Total Bkg. Unc." );
		Histos.push_back( this->Hists_MC->h_RelUnc_Tot ); Names.push_back("Syst. from MC based bkg." );
		Histos.push_back( this->Hists_emu->h_RelUnc_Tot ); Names.push_back("Syst. from e#mu method" );
		Histos.push_back( this->Hists_FR->h_RelUnc_Tot ); Names.push_back("Syst. from fake rate method" );

		this->MakeCanvas_SysUnc( "total", Histos, Names );
	}

	void MakeCanvas_SysUnc_StatSystTotal()
	{
		vector< TH1D* > Histos; vector< TString > Names;
		Histos.push_back( this->Hists_Total->h_RelUnc_Tot ); Names.push_back("Total Bkg. Unc." );
		Histos.push_back( this->Hists_Total->h_RelUnc_Stat ); Names.push_back("Stat. Unc." );
		Histos.push_back( this->Hists_Total->h_RelUnc_Syst ); Names.push_back("Syst. Unc." );

		this->MakeCanvas_SysUnc( "StatSystTotal", Histos, Names );

		TFile *f_output2 = TFile::Open("ROOTFile_RelUnc_Stat_Syst_Tot_BkgEst.root", "RECREATE"); f_output2->cd();
		TH1D* h_tot = (TH1D*)this->Hists_Total->h_RelUnc_Tot->Clone("h_RelUnc_Tot"); h_tot->Write();
		TH1D* h_stat = (TH1D*)this->Hists_Total->h_RelUnc_Stat->Clone("h_RelUnc_Stat"); h_stat->Write();
		TH1D* h_syst = (TH1D*)this->Hists_Total->h_RelUnc_Syst->Clone("h_RelUnc_Syst"); h_syst->Write();
	}

	void Save_All()
	{
		Hists_Total->Save( this->f_output );

		Int_t nHist = (Int_t)Hists_Bkg.size();
		for(Int_t i=0; i<nHist; i++)
			Hists_Bkg[i]->Save( this->f_output );

		this->Hists_emu->Save( this->f_output );
		this->Hists_FR->Save( this->f_output );
		this->Hists_MC->Save( this->f_output );
	}

protected:
	void Initialize_SystHistograms()
	{
		this->Hists_Total = new HistogramContainer( "Total" );

		this->Hists_ttbar = new HistogramContainer( "ttbar" ); Hists_Bkg.push_back( Hists_ttbar );
		this->Hists_tW = new HistogramContainer( "tW" ); Hists_Bkg.push_back( Hists_tW );
		this->Hists_DYtautau = new HistogramContainer( "DYtautau" ); Hists_Bkg.push_back( Hists_DYtautau );
		this->Hists_WJets = new HistogramContainer( "WJets" ); Hists_Bkg.push_back( Hists_WJets );
		this->Hists_QCD = new HistogramContainer( "QCD" ); Hists_Bkg.push_back( Hists_QCD );
		this->Hists_WW = new HistogramContainer( "WW" ); Hists_Bkg.push_back( Hists_WW );
		this->Hists_WZ = new HistogramContainer( "WZ" ); Hists_Bkg.push_back( Hists_WZ );
		this->Hists_ZZ = new HistogramContainer( "ZZ" ); Hists_Bkg.push_back( Hists_ZZ );

		this->Hists_emu = new HistogramContainer( "emu" );
		this->Hists_FR = new HistogramContainer( "FR" );
		this->Hists_MC = new HistogramContainer( "MC" );
	}

	void MakeCombinedHistogram_byEstimationMethod()
	{
		//////////////////////
		// -- emu method -- //
		//////////////////////
		vector< HistogramContainer* > vec_Hists_emu;
		vec_Hists_emu.push_back( Hists_ttbar );
		vec_Hists_emu.push_back( Hists_tW );
		vec_Hists_emu.push_back( Hists_DYtautau );
		vec_Hists_emu.push_back( Hists_WW );
		this->MakeCombinedHistogram( vec_Hists_emu, this->Hists_emu );

		vector< TH1D* > vec_h_RelUnc_emu; vector< TString > Names_emu;
		vec_h_RelUnc_emu.push_back( this->Hists_emu->h_RelUnc_Tot ); Names_emu.push_back( "Total Unc. from e#mu method" );
		vec_h_RelUnc_emu.push_back( this->Hists_ttbar->h_RelUnc_Tot ); Names_emu.push_back( "t#bar{t}" );
		vec_h_RelUnc_emu.push_back( this->Hists_tW->h_RelUnc_Tot ); Names_emu.push_back( "tW + #bar{t}W" );
		vec_h_RelUnc_emu.push_back( this->Hists_DYtautau->h_RelUnc_Tot ); Names_emu.push_back( "Z/#gamma* #rightarrow #tau#tau" );
		vec_h_RelUnc_emu.push_back( this->Hists_WW->h_RelUnc_Tot ); Names_emu.push_back( "WW" );
		this->MakeCanvas_SysUnc( "emu", vec_h_RelUnc_emu, Names_emu );


		/////////////////////
		// -- FR method -- //
		/////////////////////
		vector< HistogramContainer* > vec_Hists_FR;
		vec_Hists_FR.push_back( Hists_WJets );
		vec_Hists_FR.push_back( Hists_QCD );
		this->MakeCombinedHistogram( vec_Hists_FR, this->Hists_FR );

		vector< TH1D* > vec_h_RelUnc_FR; vector< TString > Names_FR;
		vec_h_RelUnc_FR.push_back( this->Hists_FR->h_RelUnc_Tot ); Names_FR.push_back( "Total Unc. from FR method" );
		vec_h_RelUnc_FR.push_back( this->Hists_WJets->h_RelUnc_Tot ); Names_FR.push_back( "W+Jets" );
		vec_h_RelUnc_FR.push_back( this->Hists_QCD->h_RelUnc_Tot ); Names_FR.push_back( "QCD" );
		this->MakeCanvas_SysUnc( "FR", vec_h_RelUnc_FR, Names_FR );


		////////////////////
		// -- MC based -- //
		////////////////////
		vector< HistogramContainer* > vec_Hists_MC;
		vec_Hists_MC.push_back( Hists_WZ );
		vec_Hists_MC.push_back( Hists_ZZ );
		this->MakeCombinedHistogram( vec_Hists_MC, this->Hists_MC );

		vector< TH1D* > vec_h_RelUnc_MC; vector< TString > Names_MC;
		vec_h_RelUnc_MC.push_back( this->Hists_MC->h_RelUnc_Tot ); Names_MC.push_back( "Total Unc. from MC" );
		vec_h_RelUnc_MC.push_back( this->Hists_WZ->h_RelUnc_Tot ); Names_MC.push_back( "WZ" );
		vec_h_RelUnc_MC.push_back( this->Hists_ZZ->h_RelUnc_Tot ); Names_MC.push_back( "ZZ" );
		this->MakeCanvas_SysUnc( "MC", vec_h_RelUnc_MC, Names_MC );
	}

	void MakeCombinedHistogram( vector< HistogramContainer* > vec_Hists, HistogramContainer* Hists_Combined )
	{
		Int_t nHist = (Int_t)vec_Hists.size();

		// -- combine nEvent histogram -- //
		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			Hists_Combined->h_nEvent->Add( vec_Hists[i_hist]->h_nEvent );

		// -- combine uncertainty histograms: quadrature sum -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			Double_t AbsUnc_Stat_Sum2 = 0;
			Double_t AbsUnc_Syst_Sum2 = 0;
			Double_t AbsUnc_Tot_Sum2 = 0;
			for(Int_t i_hist=0; i_hist<nHist; i_hist++)
			{
				Double_t AbsUnc_Stat_temp = vec_Hists[i_hist]->h_AbsUnc_Stat->GetBinContent(i_bin);
				Double_t AbsUnc_Syst_temp = vec_Hists[i_hist]->h_AbsUnc_Syst->GetBinContent(i_bin);
				Double_t AbsUnc_Tot_temp = vec_Hists[i_hist]->h_AbsUnc_Tot->GetBinContent(i_bin);

				AbsUnc_Stat_Sum2 = AbsUnc_Stat_Sum2 + AbsUnc_Stat_temp*AbsUnc_Stat_temp;
				AbsUnc_Syst_Sum2 = AbsUnc_Syst_Sum2 + AbsUnc_Syst_temp*AbsUnc_Syst_temp;
				AbsUnc_Tot_Sum2 = AbsUnc_Tot_Sum2 + AbsUnc_Tot_temp*AbsUnc_Tot_temp;
			}

			Double_t AbsUnc_Stat = sqrt( AbsUnc_Stat_Sum2 );
			Double_t AbsUnc_Syst = sqrt( AbsUnc_Syst_Sum2 );
			Double_t AbsUnc_Tot = sqrt( AbsUnc_Tot_Sum2 );

			Hists_Combined->h_AbsUnc_Stat->SetBinContent( i_bin, AbsUnc_Stat );
			Hists_Combined->h_AbsUnc_Syst->SetBinContent( i_bin, AbsUnc_Syst );
			Hists_Combined->h_AbsUnc_Tot->SetBinContent( i_bin, AbsUnc_Tot );
		}

		Hists_Combined->Fill_RelUncHist_wrt_nUnfolded( this->h_unfolded );
	}

	void MakeCanvas_SysUnc( TString Type, vector< TH1D* > Histos, vector< TString > Names )
	{
		TCanvas *c_RelUnc = new TCanvas("c_RelSysUnc_"+Type, "", 800, 800);
		c_RelUnc->cd();

		gPad->SetTopMargin(0.05);
		gPad->SetBottomMargin(0.13);
		gPad->SetRightMargin(0.03);
		gPad->SetLeftMargin(0.13);

		gPad->SetLogx();
		gPad->SetGridx();
		gPad->SetGridy();

		Int_t Colors[6] = {kRed, kBlack, kBlue, kGreen+1, kOrange+1, kViolet};
		Int_t nHist = (Int_t)Histos.size();
		if( nHist > 6 )
		{
			cout << "The number of colors is smaller than the number of histograms!" << endl;
			return;
		}

		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
		{
			TString DrawOption = "HISTLPSAME";
			if( i_hist == 0 )
				DrawOption = "HISTLP";

			Histos[i_hist]->Draw( DrawOption );
		}
		Histos[0]->Draw("HISTLPSAME");

		Histos[0]->GetXaxis()->SetLabelSize(0.04);
		Histos[0]->GetXaxis()->SetTitleSize(0.04);
		Histos[0]->GetXaxis()->SetNoExponent();
		Histos[0]->GetXaxis()->SetMoreLogLabels();
		Histos[0]->GetYaxis()->SetTitleOffset(1.4);

		Double_t yMax = 40;
		if( Type == "emu") yMax = 16;
		else if( Type == "FR") yMax = 3;
		// else if( Type == "MCBased") yMax = 80;
		// else if( Type == "total" ) yMax = 80;

		Histos[0]->GetYaxis()->SetRangeUser(0, yMax);

		Histos[0]->SetXTitle( "m (#mu#mu) [GeV]");
		Histos[0]->SetYTitle( "Rel. Unc. (%)");

		TLegend *legend = new TLegend(0.15, 0.65, 0.55, 0.95);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);

		for(Int_t i_hist=0; i_hist<nHist; i_hist++)
		{
			Histos[i_hist]->SetStats(kFALSE);
			Histos[i_hist]->SetMarkerStyle(20);
			Histos[i_hist]->SetMarkerColor( Colors[i_hist] );
			Histos[i_hist]->SetLineColor( Colors[i_hist] );
			Histos[i_hist]->SetFillColorAlpha( kWhite, 0);

			legend->AddEntry( Histos[i_hist], Names[i_hist] );
		}
		legend->Draw();

		TLatex latex;
		latex.DrawLatexNDC(0.70, 0.96, TString::Format("#font[42]{#scale[0.8]{%.2lf fb^{-1} (13 TeV)}}", Lumi / 1000.0) );
		latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}");
		latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}");

		c_RelUnc->SaveAs( ".pdf" );

		f_output->cd();
		c_RelUnc->Write();
	}

	void SetupHistogram_Unfolded()
	{
		TFile *f_result = TFile::Open(FileLocation + "/ROOTFile_Results_DYAnalysis_76X.root");
		h_unfolded = (TH1D*)f_result->Get("h_yield_Unfolded")->Clone();
	}

	void SetupHistogram_DataDrivenBkg_All()
	{
		this->SetupHistogram_DataDrivenBkg( "dijet", this->Hists_QCD );
		this->SetupHistogram_DataDrivenBkg( "wjets", this->Hists_WJets );
		this->SetupHistogram_DataDrivenBkg( "ttbar", this->Hists_ttbar );
		this->SetupHistogram_DataDrivenBkg( "tW", this->Hists_tW );
		this->SetupHistogram_DataDrivenBkg( "DYtautau", this->Hists_DYtautau );
		this->SetupHistogram_DataDrivenBkg( "WW", this->Hists_WW );
	}

	void SetupHistogram_DataDrivenBkg( TString Type, HistogramContainer* Hists )
	{
		TFile *f_input = TFile::Open(FileLocation + "/ROOTFile_Bkg_DataDrivenMethod.root");
		Hists->h_nEvent = (TH1D*)f_input->Get(Type)->Clone();
		Hists->h_AbsUnc_Stat = (TH1D*)f_input->Get(Type+"_stat")->Clone();
		Hists->h_AbsUnc_Syst = (TH1D*)f_input->Get(Type+"_systematic")->Clone();

		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t AbsUnc_Stat = Hists->h_AbsUnc_Stat->GetBinContent(i_bin);
			Double_t AbsUnc_Syst = Hists->h_AbsUnc_Syst->GetBinContent(i_bin);
			Double_t AbsUnc_Tot = sqrt( AbsUnc_Stat*AbsUnc_Stat + AbsUnc_Syst*AbsUnc_Syst );

			Hists->h_AbsUnc_Tot->SetBinContent(i_bin, AbsUnc_Tot);
			Hists->h_AbsUnc_Tot->SetBinError(i_bin, 0);
		}

		Hists->Fill_RelUncHist_wrt_nUnfolded( this->h_unfolded );

		delete f_input;
	}

	void SetupHistgram_MCBkg_All()
	{
		// Double_t RelUnc_xSec_WW = 0.0799; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-14-016/index.html -- //
		Double_t RelUnc_xSec_WZ = 0.4052; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-011/index.html -- //
		Double_t RelUnc_xSec_ZZ = 0.1087; // -- https://cms-results.web.cern.ch/cms-results/public-results/publications/SMP-13-005/index.html -- //

		this->SetupHistgram_MCBkg( "WZ", RelUnc_xSec_WZ, this->Hists_WZ );
		this->SetupHistgram_MCBkg( "ZZ", RelUnc_xSec_ZZ, this->Hists_ZZ );
	}

	void SetupHistgram_MCBkg(TString Type, Double_t RelUnc_xSec, HistogramContainer* Hists )
	{
		Int_t nTag = (Int_t)Tag.size();
		Double_t normFactor = 0;

		for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		{
			if( Tag[i_tag] == Type )
			{
				normFactor = (Xsec[i_tag] * Lumi) / nEvents[i_tag];
				printf( "[Normalization factor for %s = %lf]\n", Type.Data(), normFactor );
				break;
			}
		}

		TFile *f_MC = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MC_MomCorr.root"); f_MC->cd();
		Hists->h_nEvent = (TH1D*)f_MC->Get("h_mass_OS_"+Type)->Clone();
		Hists->h_nEvent = (TH1D*)Hists->h_nEvent->Rebin(nMassBin, Hists->h_nEvent->GetName(), MassBinEdges);
		Hists->h_nEvent->Scale( normFactor );

		// TFile *f_MC = TFile::Open(FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root"); f_MC->cd();
		// h_bkg = (TH1D*)f_MC->Get("h_mass_OS_Norm_"+Type)->Clone();
		// h_bkg = (TH1D*)h_bkg->Rebin(nMassBin, h_bkg->GetName(), MassBinEdges);

		// -- make stat. & syst. only histogram -- //
		for(Int_t i=0; i<nMassBin; i++)
		{
			Int_t i_bin = i+1;

			// Double_t AbsUnc_Stat = sqrt( Hists->h_nEvent->GetBinContent(i_bin) );
			Double_t AbsUnc_Stat = Hists->h_nEvent->GetBinError(i_bin);
			Hists->h_AbsUnc_Stat->SetBinContent(i_bin, AbsUnc_Stat);
			Hists->h_AbsUnc_Stat->SetBinError(i_bin, 0);

			Double_t nEvent = Hists->h_nEvent->GetBinContent(i_bin);
			Double_t AbsUnc_xSec = RelUnc_xSec * nEvent;
			Hists->h_AbsUnc_Syst->SetBinContent(i_bin, AbsUnc_xSec);
			Hists->h_AbsUnc_Syst->SetBinError(i_bin, 0);

			Double_t AbsUnc_Tot = sqrt( AbsUnc_Stat*AbsUnc_Stat + AbsUnc_xSec*AbsUnc_xSec );
			Hists->h_AbsUnc_Tot->SetBinContent(i_bin, AbsUnc_Tot);
			Hists->h_AbsUnc_Tot->SetBinError(i_bin, 0);
		}

		Hists->Fill_RelUncHist_wrt_nUnfolded( this->h_unfolded );

		delete f_MC;
	}

};