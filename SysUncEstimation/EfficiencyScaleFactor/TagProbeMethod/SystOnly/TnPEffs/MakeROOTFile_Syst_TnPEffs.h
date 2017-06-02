#include <Include/PlotTools.h>

class CalcSystTool
{
public:
	TString Type;
	TString ROOTFileName_CV;
	TString ROOTFileName_Syst;
	TFile *f_output;

	CalcSystTool() {} // -- default constructor -- //
	CalcSystTool( TString _Type ) : CalcSystTool()
	{
		this->Type = _Type;
		TString IncludePath = gSystem->Getenv("KP_INCLUDE_PATH");
		this->ROOTFileName_CV = IncludePath+"/ROOTFile_TagProbeEfficiency_76X_v20160502.root";
	}

	void InputROOTFileName( TString _ROOTFileName_Syst )
	{
		this->ROOTFileName_Syst = _ROOTFileName_Syst;
	}

	void OutputROOTFile( TFile *_f_output )
	{
		this->f_output = _f_output;
	}

	void Save_Syst_2DPlot()
	{
		cout << "Type: " << this->Type << endl;
		TH2D* h_2D_RecoID_Data = this->Hist2D_Alt_minus_CV( "h_2D_Eff_RecoID_Data" );
		TH2D* h_2D_RecoID_MC = this->Hist2D_Alt_minus_CV( "h_2D_Eff_RecoID_MC" );
		TH2D* h_2D_Iso_Data = this->Hist2D_Alt_minus_CV( "h_2D_Eff_Iso_Data" );
		TH2D* h_2D_Iso_MC = this->Hist2D_Alt_minus_CV( "h_2D_Eff_Iso_MC" );
		TH2D* h_2D_HLTv4p2_Data = this->Hist2D_Alt_minus_CV( "h_2D_Eff_HLTv4p2_Data" );
		TH2D* h_2D_HLTv4p2_MC = this->Hist2D_Alt_minus_CV( "h_2D_Eff_HLTv4p2_MC" );
		TH2D* h_2D_HLTv4p3_Data = this->Hist2D_Alt_minus_CV( "h_2D_Eff_HLTv4p3_Data" );
		TH2D* h_2D_HLTv4p3_MC = this->Hist2D_Alt_minus_CV( "h_2D_Eff_HLTv4p3_MC" );

		// -- make sub-directory -- //
		this->f_output->cd();
		this->f_output->mkdir(this->Type);
		f_output->cd(this->Type);

		h_2D_RecoID_Data->Write();
		h_2D_RecoID_MC->Write();
		h_2D_Iso_Data->Write();
		h_2D_Iso_MC->Write();
		h_2D_HLTv4p2_Data->Write();
		h_2D_HLTv4p2_MC->Write();
		h_2D_HLTv4p3_Data->Write();
		h_2D_HLTv4p3_MC->Write();

		cout << "Calc. and Save is finished for " << this->Type << endl;
	}
protected:
	TH2D* Hist2D_Alt_minus_CV( TString HistName )
	{
		TString OutputHistName = HistName;
		OutputHistName.ReplaceAll("Eff", "EffDiff");

		TH2D* h_CV = Get_Hist_2D( this->ROOTFileName_CV, HistName );
		TH2D* h_Alt = Get_Hist_2D( this->ROOTFileName_Syst, HistName );

		TH2D* h_Diff = (TH2D*)h_CV->Clone(OutputHistName);
		h_Diff->Reset("ICES");

		Int_t nEtaBin = h_CV->GetNbinsX();
		Int_t nPtBin = h_CV->GetNbinsY();

		for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
		{
			for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
			{
				Int_t i_etabin = i_eta+1;
				Int_t i_ptbin = i_pt+1;

				Double_t Eff_CV = h_CV->GetBinContent(i_etabin, i_ptbin);
				Double_t Eff_Alt = h_Alt->GetBinContent(i_etabin, i_ptbin);

				Double_t Eff_Diff = Eff_Alt - Eff_CV; // -- do not use absolute value! -- //
				h_Diff->SetBinContent(i_etabin, i_ptbin, Eff_Diff);

				printf("[(eta, pt) = (%d, %d)] (alt, CV, alt-CV) = (%5.3lf, %5.3lf, %5.3lf (%6.3lf%%))\n", 
					i_etabin, i_ptbin, Eff_Alt, Eff_CV, Eff_Diff, (Eff_Diff/Eff_CV)*100 );
			}
		}

		return h_Diff;
	}
};
