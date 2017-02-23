#include "Systematic_Eff.h"

void Systematic_Eff()
{
	// PlotTools_Mass *masstool = new PlotTools_Mass();
	// TFile *f_output = TFile::Open( "ROOTFile_Output_Systematic_Eff.root", "RECREATE" );
	// masstool->Set_Output( f_output );
	// masstool->DrawCanvas_EachTypeAndRegion( "DEN", "All" );
	// masstool->DrawCanvas_EachTypeAndRegion( "DEN", "BB" );
	// masstool->DrawCanvas_EachTypeAndRegion( "DEN", "BEEE" );
	// masstool->DrawCanvas_EachTypeAndRegion( "NUM", "All" );
	// masstool->DrawCanvas_EachTypeAndRegion( "NUM", "BB" );
	// masstool->DrawCanvas_EachTypeAndRegion( "NUM", "BEEE" );

	vector< TString > vec_NUMStr;
	vec_NUMStr.push_back( "NUM_DENPlus0");
	vec_NUMStr.push_back( "NUM_DENPlus1");
	vec_NUMStr.push_back( "NUM_DENPlus2");
	vec_NUMStr.push_back( "NUM_DENPlus3");
	vec_NUMStr.push_back( "NUM_DENPlus4");
	vec_NUMStr.push_back( "NUM_DENPlus5");
	vec_NUMStr.push_back( "NUM_DENPlus6");
	vec_NUMStr.push_back( "NUM");

	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	for( const auto &Region : vec_Region )
	{
		TFile *f_output = TFile::Open( "ROOTFile_Output_Systematic_Eff_"+Region+".root", "RECREATE" );

		for( const auto &NUMStr : vec_NUMStr )
		{
			Tool_Systematic_Eff *tool = new Tool_Systematic_Eff();
			tool->Set_Output( f_output );
			tool->Set_DEN_NUM( "DEN", NUMStr );
			tool->Set_Region( Region );
			
			tool->DrawCanvas_Mass();
			tool->DrawCanvas_Eff_Data_vs_MC();

			f_output->cd();
			tool->g_data->Write();
			tool->g_MC->Write();
		}

		DrawCanvas_Eff_VariousNUM( f_output, "Data", Region, vec_NUMStr );
		DrawCanvas_Eff_VariousNUM( f_output, "MC", Region, vec_NUMStr );
	}
}
