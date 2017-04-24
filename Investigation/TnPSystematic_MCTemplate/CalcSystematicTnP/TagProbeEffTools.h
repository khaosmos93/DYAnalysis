#pragma once

#include <TKey.h>
#include <TDirectory.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TH2.h>
#include <TLatex.h>
#include <TF1.h>
#include <TLegend.h>

#include <vector>
#include <iostream>

#include </Users/KyeongPil_Lee/Codes/DYAnalysisCodes/tdrstyle.C>

// #include </Users/KyeongPil_Lee/Codes/DYAnalysisCodes/MyCanvas.C>

class TagProbeTools
{
public:
	TString TreeName;
	
	// -- Used for canvas with 1D histogram -- //
	Int_t nBin_1D;
	Double_t *BinEdges_1D;
	TString VarName_1D;
	TString AxisName_1D;

	// -- Used for canvas with 2D histgoram -- //
	Int_t nBin_2D_X;
	Double_t *BinEdges_2D_X;
	TString VarName_2D_X;
	TString AxisName_2D_X;

	Int_t nBin_2D_Y;
	Double_t *BinEdges_2D_Y;
	TString VarName_2D_Y;
	TString AxisName_2D_Y;

	Double_t zMin;
	Double_t zMax;

	TagProbeTools();

	///////////////////////
	// -- Bin Setting -- //
	///////////////////////
	void SetBinEdges_1D( Int_t nBin, Double_t BinEdges[], TString VarName, TString AxisName);
	void SetBinEdges_2D( Int_t nBin_X, Double_t BinEdges_X[], TString VarName_X, TString AxisName_X,
						 Int_t nBin_Y, Double_t BinEdges_Y[], TString VarName_Y, TString AxisName_Y );

	void SaveFitCanvases_2D(TFile *f_input, TString EffName, TString Type);

	// -- (bin1, bin2) = (0, 0), (0, 1), (0, 2), ... (1, 0), (1, 1), (1, 2) ... -- //
	TString ConstructBinName_2D(Int_t i_bin, 
								TString BinName1, Int_t nBin1, Double_t BinEdges1[], 
								TString BinName2, Int_t nBin2, Double_t BinEdges2[] );

	void Store_Canvas_2DHisto( TH2D *h_2D, TFile *f_output );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// -- GraphName = "g_Eff_" + EffName + "_" + Type + "_etabin" + TString::Format("%d", i_graph) -- //
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SaveEffGraphs_2D(TFile *f_input, TString EffName, TString Type, TFile *f_output);

	/////////////////////////////////////////////////////////
	// -- f_input: should be output of SaveEffGraphs_2D -- //
	///////////////////////////////////////////////////////// 
	void SaveComparisonPlots_2D(TFile *f_input, TString EffName, TString Type1, TString Type2, TFile *f_output);
	void MakeRatioGraph(TGraphAsymmErrors *g_ratio, TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2);
	Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B);
	void PrintSF(TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2, TGraphAsymmErrors *g_ratio, 
				 TString EffName, TString Type1, TString Type2, Int_t i_graph);
	void InsertLatex(TLatex *latex, Int_t i_graph, TString EffName);

	// -- f_input1 will be the reference value when relative difference is calculated -- //
	void Calc_Difference_2D(TString Info, vector<TString> EffNames, vector<TString> Types, TFile *f_input1, TFile *f_input2, TFile *f_output);
	void Calc_Difference_2DHisto( TH2D* h1_2D, TH2D* h2_2D, TH2D* h_2D_Diff, TH2D* h_2D_RelDiff );

	
	Double_t ReturnLargerValue(Double_t a, Double_t b);

};

TagProbeTools::TagProbeTools()
{
	TreeName = "muonEffs";
	zMin = 0;
	zMax = 1;
}

void TagProbeTools::SetBinEdges_1D( Int_t nBin, Double_t BinEdges[], TString VarName, TString AxisName)
{
	nBin_1D = nBin;
	BinEdges_1D = BinEdges;
	VarName_1D = VarName;
	AxisName_1D = AxisName;
}

void TagProbeTools::SetBinEdges_2D( Int_t nBin_X, Double_t BinEdges_X[], TString VarName_X, TString AxisName_X,
									Int_t nBin_Y, Double_t BinEdges_Y[], TString VarName_Y, TString AxisName_Y )
{
	nBin_2D_X = nBin_X;
	BinEdges_2D_X = BinEdges_X;
	VarName_2D_X = VarName_X;
	AxisName_2D_X = AxisName_X;

	nBin_2D_Y = nBin_Y;
	BinEdges_2D_Y = BinEdges_Y;
	VarName_2D_Y = VarName_Y;
	AxisName_2D_Y = AxisName_Y;
}

void TagProbeTools::Store_Canvas_2DHisto( TH2D *h_2D, TFile *f_output )
{
	TString CanvasName = h_2D->GetName();
	CanvasName.ReplaceAll("h_", "c_");

	TCanvas *c = new TCanvas(CanvasName, "", 800, 800);
	c->cd();
	gPad->SetTopMargin(0.03);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.15);
	gPad->SetLogy();

	h_2D->Draw("COLZTEXT");

	h_2D->SetStats(kFALSE);
	h_2D->SetXTitle(AxisName_2D_X);
	h_2D->SetYTitle(AxisName_2D_Y);

	h_2D->GetYaxis()->SetNoExponent();
	h_2D->GetYaxis()->SetMoreLogLabels();
	h_2D->GetYaxis()->SetTitleOffset(1.5);
	h_2D->GetZaxis()->SetRangeUser(zMin, zMax);

	f_output->cd();
	c->Write();
	c->SaveAs(CanvasName+".pdf");
}



void TagProbeTools::SaveEffGraphs_2D(TFile *f_input, TString EffName, TString Type, TFile *f_output)
{
	// -- Why is "nPtBin" not recoginzed below while? -- // 
	// Int_t nPtBin_ = nPtBin;

	f_input->cd();

	cout << "\n=====================================================================================" << endl;
	cout << "Processing [" << f_input->GetName() << "]" << endl;
	cout << "=====================================================================================" << endl;

	vector< TGraphAsymmErrors* > Graphs;

	// -- Get the efficiency graphs -- //
	TDirectory *dir_muonEffs = f_input->GetDirectory( TreeName );
	TIter next_muonEffs( dir_muonEffs->GetListOfKeys() );
	TKey *key_muonEffs;
	
	while (  ( key_muonEffs = (TKey*)next_muonEffs() )  ) // -- Loop for subdirecties in muonEffs -- //
	{
		TString ClassName1 = key_muonEffs->GetClassName();
		TString ObjectName1 = key_muonEffs->GetName();

		if( ClassName1 == "TDirectoryFile" && !ObjectName1.Contains("_mcTrue") )
		{
			cout << "\tLook fit_eff_plots in " << ObjectName1 << " (Class = " << ClassName1 << ")" << endl;

			TDirectory *dir_plots = f_input->GetDirectory(TreeName + "/" + ObjectName1 + "/fit_eff_plots");
			TIter next_plots( dir_plots->GetListOfKeys() );
			TKey *key_plots;
			while (  ( key_plots = (TKey*)next_plots() )  ) // -- Loop for subdirectories in fit_eff_plots -- //
			{
				TString ClassName2 = key_plots->GetClassName();
				TString ObjectName2 = key_plots->GetName();

				if(  ClassName2 == "TCanvas" && ObjectName2.Contains("pt_PLOT_eta_bin") )
				{
					TCanvas	*c = (TCanvas*)f_input->Get( TreeName + "/" + ObjectName1 + "/fit_eff_plots/" + ObjectName2 );
					TGraphAsymmErrors *g = (TGraphAsymmErrors*)c->GetPrimitive("hxy_fit_eff")->Clone();
					Graphs.push_back( g );
				}

			} // -- end of while (  ( key_plots = (TKey*)next_plots() )  ) -- //

		}

	} // -- end of while (  ( key_muonEffs = (TKey*)next() )  ) -- //


	f_output->cd();

	TH2D *h_2D = new TH2D("h_2D_Eff_" + EffName + "_" + Type, "", nBin_2D_X, BinEdges_2D_X, nBin_2D_Y, BinEdges_2D_Y);
	Int_t nGraphs = (Int_t)Graphs.size();
	for(Int_t i_graph=0; i_graph<nGraphs; i_graph++)
	{
		TGraphAsymmErrors *g = Graphs[i_graph];
		TString GraphName = "g_Eff_" + EffName + "_" + Type + "_" + VarName_2D_X + "Bin" + TString::Format("%d", i_graph);
		g->SetName( GraphName );
		g->Write();

		Int_t nPoint = g->GetN();
		if( nPoint != nBin_2D_Y )
		{
			printf("(nPoint != nBin_2D_Y) ... (%d != %d)\n", nPoint, nBin_2D_Y);
			return;
		}

		// -- Fill the histogram -- //
		for(Int_t i_point=0; i_point<nPoint; i_point++)
		{
			Double_t x, y, y_error;

			g->GetPoint(i_point, x, y);
			y_error = ReturnLargerValue( g->GetErrorYhigh(i_point), g->GetErrorYlow(i_point) );

			Int_t i_binX = i_graph+1;
			Int_t i_binY = i_point+1;

			h_2D->SetBinContent(i_binX, i_binY, y);
			h_2D->SetBinError(i_binX, i_binY, y_error);
		}

	} // -- end of for(Int_t i_graph=0; i_graph<nGraphs; i_graph++) -- //

	h_2D->Write();
	Store_Canvas_2DHisto( h_2D, f_output );
}

Double_t TagProbeTools::ReturnLargerValue(Double_t a, Double_t b)
{
	if( a > b )
		return a;
	else
		return b;
}

void TagProbeTools::Calc_Difference_2D(TString Info, vector<TString> EffNames, vector<TString> Types, TFile *f_input1, TFile *f_input2, TFile *f_output)
{
	Int_t nEffName = (Int_t)EffNames.size();
	Int_t nType = (Int_t)Types.size();

	for(Int_t i=0; i<nEffName; i++)
	{
		for(Int_t j=0; j<nType; j++)
		{
			f_input1->cd();
			TH2D *h1_2D = (TH2D*)f_input1->Get( "h_2D_Eff_" + EffNames[i] + "_" + Types[j] );

			f_input2->cd();
			TH2D *h2_2D = (TH2D*)f_input2->Get( "h_2D_Eff_" + EffNames[i] + "_" + Types[j] );

			if( h1_2D == NULL || h2_2D == NULL )
			{
				cout << "ERROR! ... Can't find the histogram: h_2D_Eff_" << EffNames[i] << "_" << Types[j];
				return;
			}

			TH2D* h_2D_Diff = new TH2D("h_2D_Diff_" + EffNames[i] + "_" + Types[j] + "_" + Info, "", nBin_2D_X, BinEdges_2D_X, nBin_2D_Y, BinEdges_2D_Y);
			TH2D* h_2D_RelDiff = new TH2D("h_2D_RelDiff_" + EffNames[i] + "_" + Types[j] + "_" + Info, "", nBin_2D_X, BinEdges_2D_X, nBin_2D_Y, BinEdges_2D_Y);
			Calc_Difference_2DHisto( h1_2D, h2_2D, h_2D_Diff, h_2D_RelDiff );

			f_output->cd();
			h_2D_Diff->Write();
			h_2D_RelDiff->Write();
			// Store_Canvas_2DHisto( (TH2D*)h_2D_Diff->Clone(), f_output );
			Store_Canvas_2DHisto( (TH2D*)h_2D_RelDiff->Clone(), f_output );
		}
	}
}

void TagProbeTools::Calc_Difference_2DHisto( TH2D* h1_2D, TH2D* h2_2D, TH2D* h_2D_Diff, TH2D* h_2D_RelDiff )
{
	Int_t nBinsX = h1_2D->GetNbinsX();
	Int_t nBinsY = h1_2D->GetNbinsY();

	if( (nBinsX != nBin_2D_X) || (nBinsY != nBin_2D_Y) )
	{
		printf("ERROR: (nBinsX != nBin_2D_X) || (nBinsY != nBin_2D_Y)! ... (nBinsX, nBin_2D_X) = (%d, %d), (nBinsY, nBin_2D_Y) = (%d, %d)\n", nBinsX, nBin_2D_X, nBinsY, nBin_2D_Y);
		return;
	}
	
	for(Int_t iter_x=0; iter_x<nBin_2D_X; iter_x++)
	{
		for(Int_t iter_y=0; iter_y<nBin_2D_Y; iter_y++)
		{
			Int_t i_binX = iter_x + 1;
			Int_t i_binY = iter_y + 1;

			Double_t content_h1 = h1_2D->GetBinContent(i_binX, i_binY);
			Double_t content_h2 = h2_2D->GetBinContent(i_binX, i_binY);

			Double_t Diff = fabs( content_h1 - content_h2 );
			Double_t RelDiff = Diff / content_h1; // -- Ref. value: h1 -- //

			h_2D_Diff->SetBinContent(i_binX, i_binY, Diff);
			h_2D_RelDiff->SetBinContent(i_binX, i_binY, RelDiff);
		}
	}

}


void TagProbeTools::SaveFitCanvases_2D(TFile *f_input, TString EffName, TString Type)
{
	TString FileName = f_input->GetName();

	cout << "\n=====================================================================================" << endl;
	cout << "Processing [" << FileName << "]" << endl;
	cout << "=====================================================================================\n" << endl;

	TDirectory *dir1 = f_input->GetDirectory(TreeName);
	TIter next( dir1->GetListOfKeys() );
	TKey *key1;

	// -- Iteration for sub-directory in "TreeName"  -- //
	while (  ( key1 = (TKey*)next() )  )
	{
		TString ClassName = key1->GetClassName();
		TString ObjectName = key1->GetName();

		if( ClassName == "TDirectoryFile" && !ObjectName.Contains("_mcTrue") )
		{
			TDirectory *dir2 = f_input->GetDirectory(TreeName + "/" + ObjectName);
			TIter next2( dir2->GetListOfKeys() );
			TKey *key2;

			// -- Iteration for sub-directory in "TreeName/ObjectName"  -- //
			Int_t i_bin = 0;
			while (  ( key2 = (TKey*)next2() )  )
			{
				TString ClassName2 = key2->GetClassName();
				TString ObjectName2 = key2->GetName();
				// cout << "\t\tClassName2: " << ClassName2 << "\n\t\tObjectName2: " << ObjectName2 << endl;

				if( ClassName2 == "TDirectoryFile" && !(ObjectName2 == "fit_eff_plots") && !(ObjectName2 == "cnt_eff_plots") )
				{
					TDirectory *dir3 = f_input->GetDirectory(TreeName + "/" + ObjectName + "/" + ObjectName2);
					TIter next3( dir3->GetListOfKeys() );
					TKey *key3 = (TKey*)next3(); // -- Key => 'fitresults' - //
					key3 = (TKey*)next3(); // -- Key => 'fit_canvas' -- //

					TString ClassName3 = key3->GetClassName();
					TString ObjectName3 = key3->GetName();
					if( ClassName3 != "TCanvas" || ObjectName3 != "fit_canvas" )
					{
						cout << "Error: NOT APPROPRIATE CLASSNAME OR OBJECTNAME" << endl; 
						cout << "\tClassName3: " << ClassName3 << " ObjectName3: " << ObjectName3 << endl;
						return;
					}
					// TCanvas *c = (TCanvas*)key3->ReadObj();
					// c->Draw();

					TCanvas *c = (TCanvas*)f_input->Get( TreeName + "/" + ObjectName + "/" + ObjectName2 + "/" + ObjectName3 )->Clone();

					TString SaveName = "FitCanvas_" + Type + "_" + EffName + "_" + ConstructBinName_2D(i_bin, VarName_2D_Y, nBin_2D_Y, BinEdges_2D_Y, VarName_2D_X, nBin_2D_X, BinEdges_2D_X);
					// cout << "\tFileName: " << ObjectName2 << "\n\tSaveName: " << SaveName << endl;
					c->SaveAs("./" + SaveName + ".png");
					i_bin++;
				}
			}
		}
	}
}

// -- (bin1, bin2) = (0, 0), (0, 1), (0, 2), ... (1, 0), (1, 1), (1, 2) ... -- //
TString TagProbeTools::ConstructBinName_2D( Int_t i_bin, TString BinName1, Int_t nBin1, Double_t BinEdges1[], TString BinName2, Int_t nBin2, Double_t BinEdges2[] )
{
	Int_t temp = nBin1; // -- just for removing warning -- //
	
	Int_t i_bin2 = i_bin % nBin2;
	Int_t i_bin1 = (Int_t)( ( i_bin - i_bin2 ) / nBin2 );

	TString Name1 = BinName1+"Bin_"+TString::Format("%.1lf_to_%.1lf", BinEdges1[i_bin1], BinEdges1[i_bin1+1]);
	TString Name2 = BinName2+"Bin_"+TString::Format("%.1lf_to_%.1lf", BinEdges2[i_bin2], BinEdges2[i_bin2+1]);

	return Name1 + "_" + Name2;
}

void TagProbeTools::SaveComparisonPlots_2D(TFile *f_input, TString EffName, TString Type1, TString Type2, TFile *f_output)
{
	vector< TGraphAsymmErrors* > Graphs_SF;
	for(Int_t i_graph=0; i_graph< nBin_2D_X; i_graph++)
	{
		TString BinNumber = TString::Format("%d", i_graph);
		TString GraphName_Type1 = "g_Eff_" + EffName + "_" + Type1 + "_" + VarName_2D_X + "Bin" + BinNumber;
		TString GraphName_Type2 = "g_Eff_" + EffName + "_" + Type2 + "_" + VarName_2D_X + "Bin" + BinNumber;
	
		f_input->cd();	
		TGraphAsymmErrors *g_Type1 = (TGraphAsymmErrors*)f_input->Get( GraphName_Type1 )->Clone();
		TGraphAsymmErrors *g_Type2 = (TGraphAsymmErrors*)f_input->Get( GraphName_Type2 )->Clone();

		////////////////////////////////////
		// -- Make Type1 / Type2 Graph -- //
		////////////////////////////////////
		TGraphAsymmErrors *g_ratio = (TGraphAsymmErrors*)g_Type1->Clone();
		g_ratio->SetName( "g_SF_" + EffName + "_" + VarName_2D_X + "Bin" + BinNumber );
		MakeRatioGraph(g_ratio, g_Type1, g_Type2);

		Graphs_SF.push_back( g_ratio );

		PrintSF(g_Type1, g_Type2, g_ratio, EffName, Type1, Type2, i_graph);


		///////////////////////////
		// -- Make the Canvas -- //
		///////////////////////////
		// MyCanvas *myc = new MyCanvas("c_Eff_" + EffName + "_" + VarName_2D_X + "Bin" + BinNumber, AxisName_2D_Y, "Efficiency");
		// myc->Legend_x1 = 0.70; myc->Legend_y1 = 0.33;
		// myc->Legend_x2 = 0.99; myc->Legend_y2 = 0.53;
		// myc->LowerEdge_Ratio = 0.9; myc->UpperEdge_Ratio = 1.1;

		// myc->CanvasWithGraphRatioPlot( g_Type1, g_Type2, Type1, Type2, Type1+"/"+Type2 );
		// myc->PrintCanvas();

		// f_output->cd();
		// g_ratio->Write();
		// myc->c->Write();

		setTDRStyle();
		// gROOT->SetStyle( "tdrStyle" );

		TLegend *legend = new TLegend(0.70, 0.33, 0.99, 0.53);
		TString CanvasName = "c_Eff_" + EffName + "_" + VarName_2D_X + "Bin" + BinNumber;
		TCanvas *c = new TCanvas(CanvasName, "", 600, 600); c->cd();

		TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
		c1_1->Draw();
		c1_1->cd();

		//Draw Plots
		g_Type2->Draw("APSAME");
		g_Type1->Draw("PSAME");

		//Apply tdrStyle
		c1_1->UseCurrentStyle();

		//ensure additional space at the bottom side for ratio plot
		c1_1->SetBottomMargin(0.3);

		//Attributes on MC plot
		g_Type2->SetLineColor( kRed );
		g_Type2->SetMarkerColor( kRed );
		g_Type2->SetMarkerStyle( 20 );
		g_Type2->SetFillColorAlpha( kWhite, 0 );
		
		TString XTitle = AxisName_2D_Y;
		g_Type2->GetXaxis()->SetTitle( XTitle );
		g_Type2->GetXaxis()->SetLabelSize(0);
		g_Type2->GetXaxis()->SetNoExponent();
		g_Type2->GetXaxis()->SetMoreLogLabels();

		Double_t xMin = BinEdges_2D_Y[0], xMax = BinEdges_2D_Y[nBin_2D_Y] * 1.1;
		// if( PassingProbe == "IsoMu20" ) xMin = 22;
		// else xMin = 10;
		g_Type2->GetXaxis()->SetRangeUser(xMin, xMax);

		TString YTitle = "Efficiency";
		if( EffName == "RecoID" )
			YTitle = "Reco+ID Efficiency";
		else if( EffName == "Iso" )
			YTitle = "Isolation Efficiency";
		else if( EffName.Contains("HLTv4p") )
			YTitle = "Trigger Efficiency";
		
		g_Type2->GetYaxis()->SetTitle( YTitle );
		g_Type2->SetMinimum(0.84);
		g_Type2->SetMaximum(1.05);
		if( EffName.Contains("HLTv4p") )
		{
			g_Type2->SetMinimum(-0.03);
			g_Type2->SetMaximum(1.2);
		}

		g_Type2->SetTitle("");

		//Attributes on data plot
		g_Type1->SetLineColor( kBlack );
		g_Type1->SetMarkerColor( kBlack );
		g_Type1->SetMarkerStyle( kFullSquare );
		g_Type1->SetFillColorAlpha( kWhite, 0 );

		//Draw legend
		legend->AddEntry( g_Type2, Type2 );
		legend->AddEntry( g_Type1, Type1 );
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->Draw();

		//Draw latex
		TLatex latex;
		InsertLatex(&latex, i_graph, EffName);

		//////////////
		//Bottom Pad//
		//////////////
		TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.3);
		c1_2->Draw();
		c1_2->cd();

		g_ratio->Draw("APSAME");

		//Set the margins
		c1_2->SetBottomMargin(0.4);
		c1_2->SetRightMargin(0.01);
		c1_2->SetLeftMargin(0.15);		

		//Attribtues on ratio plot
		g_ratio->SetTitle("");
		g_ratio->GetXaxis()->SetTitle( XTitle );
		g_ratio->GetXaxis()->SetTitleOffset( 0.9 );
		g_ratio->GetXaxis()->SetTitleSize( 0.2 );

		g_ratio->GetYaxis()->SetTitle(Type1 + "/" + Type2);
		g_ratio->GetYaxis()->SetTitleOffset( 0.4 );
		g_ratio->GetYaxis()->SetTitleSize( 0.17);
		g_ratio->GetYaxis()->SetLabelSize(0.065);

		g_ratio->GetXaxis()->SetLabelColor(1);
		g_ratio->GetXaxis()->SetLabelFont(42);
		g_ratio->GetXaxis()->SetLabelOffset(0.007);
		g_ratio->GetXaxis()->SetLabelSize(0.15);
		g_ratio->GetXaxis()->SetRangeUser(xMin, xMax);

		g_ratio->SetMaximum( 1.1 );
		g_ratio->SetMinimum( 0.9 );
		g_ratio->SetMarkerSize(1);

		//ratio=1 line
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kAzure+1);
		f_line->SetLineWidth(1);
		f_line->Draw("SAME");

		g_ratio->Draw("PSAME");
		c->SaveAs(CanvasName+".pdf");

		f_output->cd();
		g_ratio->Write();
		c->Write();
	}

	///////////////////////////////////
	// -- Make 2D histogram of SF -- //
	///////////////////////////////////
	TString name_histo = "h_2D_SF_"+EffName;
	TH2D *h_SF = new TH2D(name_histo, "ScaleFactor_"+EffName, nBin_2D_X, BinEdges_2D_X, nBin_2D_Y, BinEdges_2D_Y);
	h_SF->SetStats(kFALSE);
	h_SF->SetXTitle( AxisName_2D_X );
	h_SF->SetYTitle( AxisName_2D_Y );

	Int_t nGraphs = Graphs_SF.size();
	if( nBin_2D_X != nGraphs )
	{
		printf( "ERROR! # Graphs != nBin_2D_X ... (# Graphs, nBin_2D_X) = (%d, %d)\n", nGraphs, nBin_2D_X );
		return;
	}

	//////////////////////////////////////////////////////////////////
	// -- Fill the histogram using the values from the SF graphs -- //
	//////////////////////////////////////////////////////////////////
	for(Int_t i_graph=0; i_graph < nGraphs; i_graph++)
	{
		TGraphAsymmErrors *g_SF = Graphs_SF[i_graph];

		Int_t nPoint = g_SF->GetN();
		if( nPoint != nBin_2D_Y )
		{
			printf( "ERROR! # Points != nBin_2D_Y ... (# Points, nBin_2D_Y) = (%d, %d)\n", nPoint, nBin_2D_Y );
			return ;
		}

		for(Int_t i_p=0; i_p < nPoint; i_p++)
		{
			Double_t x, y, error;
			g_SF->GetPoint(i_p, x, y);
			error = g_SF->GetErrorYhigh(i_p);

			Int_t i_binX = i_graph +1;
			Int_t i_binY = i_p + 1;
			
			h_SF->SetBinError(i_binX, i_binY, error);
			h_SF->SetBinContent(i_binX, i_binY, y);
		}
	}

	f_output->cd();
	h_SF->Write();

	Store_Canvas_2DHisto( h_SF, f_output );
}

void TagProbeTools::MakeRatioGraph(TGraphAsymmErrors *g_ratio, TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2)
{
	// g_ratio = (TGraphAsymmErrors*)g_Type2->Clone();
	g_ratio->Set(0); //Remove all points (reset)

	Int_t NPoints = g_Type2->GetN();
	for(Int_t i_p=0; i_p<NPoints; i_p++)
	{
		// cout << i_p << "th Point" << endl;
		//Get Type1 point
		Double_t x_Type1, y_Type1;
		g_Type1->GetPoint(i_p, x_Type1, y_Type1);
		Double_t error_Type1 = ReturnLargerValue( g_Type1->GetErrorYhigh(i_p), g_Type1->GetErrorYlow(i_p) );
		// cout << "x_Type1: " << x_Type1 << " y_Type1: " << y_Type1 << " error_Type1: " << error_Type1 << " g_Type1->GetErrorYhigh: " << g_Type1->GetErrorYhigh(i_p) << " g_Type1->GetErrorYlow: " << g_Type1->GetErrorYlow(i_p) << endl;

		//Get Type2 point
		Double_t x_Type2, y_Type2;
		g_Type2->GetPoint(i_p, x_Type2, y_Type2);
		Double_t error_Type2 = ReturnLargerValue( g_Type2->GetErrorYhigh(i_p), g_Type2->GetErrorYlow(i_p) );
		// cout << "x_Type2: " << x_Type2 << " y_Type2: " << y_Type2 << " error_Type2: " << error_Type2 << " g_Type2->GetErrorYhigh: " << g_Type2->GetErrorYhigh(i_p) << " g_Type2->GetErrorYlow: " << g_Type2->GetErrorYlow(i_p) << endl;

		Double_t ratio;
		Double_t ratio_error;
		if(y_Type1 != 0 && error_Type1 != 0 && y_Type2 != 0 && error_Type2 != 0)
		{
			ratio = y_Type1 / y_Type2;
			ratio_error = Error_PropagatedAoverB(y_Type1, error_Type1, y_Type2, error_Type2);
			//calculate Scale Factor(Type1/Type2) & error

			// cout << "ratio: " << ratio << " ratio_error: " << ratio_error << endl;
		}
		else
		{
			ratio = 0;
			ratio_error = 0;
		}

		//Set Central value
		g_ratio->SetPoint(i_p, x_Type2, ratio);

		//Set the error
		Double_t error_XLow = g_Type2->GetErrorXlow(i_p);
		Double_t error_Xhigh = g_Type2->GetErrorXhigh(i_p);
		g_ratio->SetPointError(i_p, error_XLow, error_Xhigh, ratio_error, ratio_error);

		// cout << endl;
	}
}

void TagProbeTools::PrintSF(TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2, TGraphAsymmErrors *g_ratio, TString EffName, TString Type1, TString Type2, Int_t i_graph)
{
	Double_t LowerEdge = BinEdges_2D_X[i_graph];
	Double_t UpperEdge = BinEdges_2D_X[i_graph+1];
	cout << "\n===============================================================================================================" << endl;
	cout << "[Scale Factor for " << EffName << ", " << LowerEdge << " < " << VarName_2D_X << " < " << UpperEdge << "]" <<endl;

	cout << "Eff(" << Type1 << ")\tError(" << Type1 << ")\tEff(" << Type2 << ")\t\tError(" << Type2 << ")\tSF\t\tError(SF)" << endl;

	Int_t NPoints = g_Type2->GetN();
	for(Int_t i_p=0; i_p<NPoints; i_p++)
	{
		Double_t x_Type1, y_Type1;
		g_Type1->GetPoint(i_p, x_Type1, y_Type1);
		Double_t error_Type1 = ReturnLargerValue( g_Type1->GetErrorYhigh(i_p), g_Type1->GetErrorYlow(i_p) );

		//Get Type2 point
		Double_t x_Type2, y_Type2;
		g_Type2->GetPoint(i_p, x_Type2, y_Type2);
		Double_t error_Type2 = ReturnLargerValue( g_Type2->GetErrorYhigh(i_p), g_Type2->GetErrorYlow(i_p) );

		//Get SF point
		Double_t x_SF, y_SF;
		g_ratio->GetPoint(i_p, x_SF, y_SF);
		Double_t error_SF = ReturnLargerValue( g_ratio->GetErrorYhigh(i_p), g_ratio->GetErrorYlow(i_p) );
		printf("%.4f\t\t%.4f\t\t%.4f\t\t%.4f\t\t%.4f\t\t%.4f\n", y_Type1, error_Type1, y_Type2, error_Type2, y_SF, error_SF);
	}
	cout << "===============================================================================================================\n" << endl;
	cout << endl;
}

Double_t TagProbeTools::Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
{
	Double_t ratio_A = (sigma_A) / A;
	Double_t ratio_B = (sigma_B) / B;

	Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

	return (A/B) * sqrt(errorSquare);
}

void TagProbeTools::InsertLatex(TLatex *latex, Int_t i_graph, TString EffName)
{
	Double_t y1 = 1.029; // -- for "CMS" & PassingProbe name -- //
	Double_t y2 = y1 - 0.014; // -- for "Preliminary" & Conditions -- //

	Double_t x1 = 17.5; // -- for "CMS" & "Preliminary" -- //
	Double_t x2 = 141; // -- for PassingProbe Name & Conditions -- //
	Double_t x_201513TeV = 184; // -- for "2015, 13TeV" -- //
	Double_t y_201513TeV = 1.054;
	
	if( EffName.Contains("HLTv4p") ) { y1 = 1.095; y2 = 1.015; x2 = 100; y_201513TeV = 1.22185; }
	// else if( PassingProbe == "Mu45_eta2p1" ) { x1 = x1 - 5; x2 = x2 - 5; }
	// else if( PassingProbe == "Mu50" ) { x1 = ; x2 = ; }

	latex->DrawLatex(x_201513TeV, y_201513TeV, "#font[42]{2015, 13TeV}");
	latex->DrawLatex(x1, y1, "#font[62]{CMS}");
	latex->DrawLatex(x1, y2, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
	latex->DrawLatex(x2, y1, "#font[42]{#scale[0.7]{"+EffName+"}}");

	if( VarName_2D_X == "Eta" )
	{
		TString EtaRange = TString::Format("%.1lf < #eta < %.1lf", BinEdges_2D_X[i_graph], BinEdges_2D_X[i_graph+1]);
		latex->DrawLatex(x2, y2, "#font[42]{#scale[0.7]{"+EtaRange+"}}");
	}
	// if( i_etabin == 0 )
	// 	latex->DrawLatex(x2, y2, "#font[42]{#scale[0.7]{-2.4 < #eta < -1.2}}");
	// else if( i_etabin == 1 )
	// 	latex->DrawLatex(x2, y2, "#font[42]{#scale[0.7]{-1.2 < #eta < -0.3}}");
	// else if( i_etabin == 2 )
	// 	latex->DrawLatex(x2, y2, "#font[42]{#scale[0.7]{-0.3 < #eta < 0.3}}");
	// else if( i_etabin == 3 )
	// 	latex->DrawLatex(x2, y2, "#font[42]{#scale[0.7]{0.3 < #eta < 1.2}}");
	// else if( i_etabin == 4 )
	// 	latex->DrawLatex(x2, y2, "#font[42]{#scale[0.7]{1.2 < #eta < 2.4}}");
}