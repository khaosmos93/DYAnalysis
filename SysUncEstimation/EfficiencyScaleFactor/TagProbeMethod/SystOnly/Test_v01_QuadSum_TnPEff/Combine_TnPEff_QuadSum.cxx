#include <Include/PlotTools.h>

void Combine_TnPEff_QuadSum()
{
	TString AnalyzerPath = gSystem->Getenv("KP_ANALYZER_PATH");
	TString InputName = AnalyzerPath+"/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly/TnPEffs/ROOTFile_Syst_TnPEffs.root";

	vector< TString > vec_Type = {"sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"};
	vector< TString > vec_EffType = {"RecoID", "Iso", "HLTv4p2", "HLTv4p3"};
	vector< TH2D* > vec_TotalHist_Data; // -- output -- //
	vector< TH2D* > vec_TotalHist_MC; // -- output -- //

	for( const auto& EffType : vec_EffType )
	{
		TString HistName_Data = TString::Format( "h_2D_EffDiff_%s_Data", EffType.Data() );
		TString HistName_MC = TString::Format( "h_2D_EffDiff_%s_MC", EffType.Data() );

		vector< TH2D* > vec_Hist_Data;
		vector< TH2D* > vec_Hist_MC;
		for( const auto &Type : vec_Type )
		{
			vec_Hist_Data.push_back( Get_Hist_2D(InputName, Type + "/" + HistName_Data) );
			vec_Hist_MC.push_back( Get_Hist_2D(InputName, Type + "/" + HistName_MC) );
		}

		TH2D* h_Tot_Data = (TH2D*)vec_Hist_Data[0]->Clone(TString::Format( "h_2D_EffDiff_%s_Data", EffType.Data() ));
		h_Tot_Data->Reset("ICES");
		TH2D* h_Tot_MC = (TH2D*)vec_Hist_Data[0]->Clone(TString::Format( "h_2D_EffDiff_%s_MC", EffType.Data() ));
		h_Tot_MC->Reset("ICES");

		Int_t nBinX = vec_Hist_Data[0]->GetNbinsX();
		Int_t nBinY = vec_Hist_Data[0]->GetNbinsY();
		Int_t nHist = (Int_t)vec_Hist_Data.size();

		for(Int_t iter_x=0; iter_x<nBinX; iter_x++)
		{
			for(Int_t iter_y=0; iter_y<nBinY; iter_y++)
			{
				Int_t i_binX = iter_x+1;
				Int_t i_binY = iter_y+1;

				Double_t Tot_Data = 0;
				Double_t Tot_MC = 0;

				for(Int_t i_hist=0; i_hist<nHist; i_hist++)
				{
					Double_t Eff_Data = vec_Hist_Data[i_hist]->GetBinContent(i_binX, i_binY);
					Double_t Eff_MC = vec_Hist_MC[i_hist]->GetBinContent(i_binX, i_binY);

					Tot_Data = Tot_Data + Eff_Data*Eff_Data;
					Tot_MC = Tot_MC + Eff_MC*Eff_MC;
				}	

				Tot_Data = sqrt(Tot_Data);
				Tot_MC = sqrt(Tot_MC);

				h_Tot_Data->SetBinContent(i_binX, i_binY, Tot_Data);
				h_Tot_Data->SetBinError(i_binX, i_binY, 0);
				h_Tot_MC->SetBinContent(i_binX, i_binY, Tot_MC);
				h_Tot_MC->SetBinError(i_binX, i_binY, 0);
			}
		}

		vec_TotalHist_Data.push_back( h_Tot_Data );
		vec_TotalHist_MC.push_back( h_Tot_MC );
	}

	TFile *f_output = TFile::Open("ROOTFile_Syst_TnPEffs_QuadSumAll.root", "RECREATE");
	f_output->cd();

	Int_t nHist2 = (Int_t)vec_TotalHist_Data.size();
	for(Int_t i_hist=0; i_hist<nHist2; i_hist++)
	{
		vec_TotalHist_Data[i_hist]->Write();
		vec_TotalHist_MC[i_hist]->Write();
	}

	f_output->Close();
}