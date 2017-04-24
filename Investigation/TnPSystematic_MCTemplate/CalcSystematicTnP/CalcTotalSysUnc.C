#include "TagProbeEffTools.h"

void CalcTotalSysUnc()
{
	const Int_t nSource = 8;

	TString Sources[nSource] = {"sgnChange", "bkgChange", "nBin30", "nBin50", "M60to130", "M70to120", "TagPt20", "TagPt24"};

	TFile *f_inputs[nSource];
	for(Int_t i=0; i<nSource; i++)
	{
		TString FileName = "ROOTFile_ExtractEfficiency_CalcSysUnc_2D_"+Sources[i]+".root";
		cout << "FileName: " << FileName << endl;

		f_inputs[i] = new TFile(FileName);
	}

	TFile *f_output = new TFile("ROOTFile_SysUnc_TagProbeMethod.root", "RECREATE");

	const Int_t nEffName = 4;
	const Int_t nType = 2;
	TString EffNames[nEffName] = {"RecoID", "Iso", "HLTv4p2", "HLTv4p3"};
	TString Types[nType] = {"Data", "MC"};

	/////////////////////////////
	// -- Get the histogram -- //
	/////////////////////////////
	TH2D *h_RelDiff[nEffName][nType][nSource];
	for(Int_t i_eff=0; i_eff<nEffName; i_eff++)
	{
		for(Int_t i_type=0; i_type<nType; i_type++)
		{
			for(Int_t i_source=0; i_source<nSource; i_source++)
			{
				f_inputs[i_source]->cd();
				h_RelDiff[i_eff][i_type][i_source] = (TH2D*)f_inputs[i_source]->Get( "h_2D_RelDiff_"+EffNames[i_eff]+"_"+Types[i_type]+"_"+Sources[i_source] )->Clone();
			}
		}
	}

	const Int_t nPtBin = 4;
	Double_t PtBinEdges[nPtBin+1] = { 10, 22, 40, 70, 250 };

	const Int_t nEtaBin = 5;
	Double_t EtaBinEdges[nEtaBin+1] = { -2.4, -1.2, -0.3, 0.3, 1.2, 2.4 };

	TagProbeTools *tools = new TagProbeTools();
	tools->TreeName = "muonEffs";
	tools->SetBinEdges_2D( nEtaBin, EtaBinEdges, "Eta", "Muon #eta", nPtBin, PtBinEdges, "Pt", "Muon P_{T} [GeV]" );
	tools->zMin = 0; tools->zMax = 0.1;

	for(Int_t i_eff=0; i_eff<nEffName; i_eff++)
	{
		for(Int_t i_type=0; i_type<nType; i_type++)
		{
			cout << "[EffName = " << EffNames[i_eff] << ", Type = " << Types[i_type] << "]" << endl;

			TH2D *h_RelDiff_Tot = new TH2D("h_2D_RelDiff_"+EffNames[i_eff]+"_"+Types[i_type]+"_Tot", "", nEtaBin, EtaBinEdges, nPtBin, PtBinEdges);

			for(Int_t iter_x=0; iter_x<nEtaBin; iter_x++)
			{
				for(Int_t iter_y=0; iter_y<nPtBin; iter_y++)
				{
					Int_t i_etabin = iter_x+1;
					Int_t i_ptbin = iter_y+1;

					Double_t RelSysUnc_totSquare = 0;
					Double_t value[nSource] = {0};
					for(Int_t i_source=0; i_source<nSource; i_source++)
					{
						value[i_source] = h_RelDiff[i_eff][i_type][i_source]->GetBinContent(i_etabin, i_ptbin);

						RelSysUnc_totSquare += value[i_source] * value[i_source];
					}
					
					Double_t RelSysUnc_tot = sqrt(RelSysUnc_totSquare);

					h_RelDiff_Tot->SetBinContent( i_etabin, i_ptbin, RelSysUnc_tot );

					printf("\t(%d, %d): %6.3lf, %6.3lf, %6.3lf, %6.3lf, %6.3lf, %6.3lf, %6.3lf, %6.3lf -> %6.3lf\n", i_etabin, i_ptbin, value[0], value[1], value[2], value[3], value[4], value[5], value[6], value[7], RelSysUnc_tot);
				}
			}

			f_output->cd();
			h_RelDiff_Tot->Write();

			tools->Store_Canvas_2DHisto(h_RelDiff_Tot, f_output);
		}
	}
}