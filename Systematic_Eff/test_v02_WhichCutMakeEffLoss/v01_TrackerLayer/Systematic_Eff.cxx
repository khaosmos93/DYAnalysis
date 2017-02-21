#include "Systematic_Eff.h"

void Systematic_Eff()
{
	PlotTools_Mass *masstool = new PlotTools_Mass();
	TFile *f_output = TFile::Open( "ROOTFile_Output_Systematic_Eff.root", "RECREATE" );
	masstool->Set_Output( f_output );
	masstool->DrawCanvas_EachTypeAndRegion( "DEN", "All" );
	masstool->DrawCanvas_EachTypeAndRegion( "DEN", "BB" );
	masstool->DrawCanvas_EachTypeAndRegion( "DEN", "BEEE" );
	masstool->DrawCanvas_EachTypeAndRegion( "NUM", "All" );
	masstool->DrawCanvas_EachTypeAndRegion( "NUM", "BB" );
	masstool->DrawCanvas_EachTypeAndRegion( "NUM", "BEEE" );

	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	for( const auto &Region : vec_Region )
	{
		TGraphAsymmErrors* g_data = EfficiencyGraph( "Data", Region );
		TGraphAsymmErrors* g_MC = EfficiencyGraph( "MC", Region );

		DrawCanvas_Eff_Data_vs_MC( Region, g_data, g_MC	);

		f_output->cd();
		g_data->Write();
		g_MC->Write();
	}
}
