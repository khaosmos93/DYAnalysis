#include "Systematic_Eff.h"

void Systematic_Eff()
{
	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	TFile *f_output = TFile::Open( "ROOTFile_Output_Systematic_Eff.root", "RECREATE" );

	for( const auto &Region : vec_Region )
	{
		Tool_Systematic_Eff *tool = new Tool_Systematic_Eff();
		tool->Set_Output( f_output );
		tool->Set_DEN_NUM( "DEN", "NUM" );
		tool->Set_Region( Region );
		
		tool->DrawCanvas_Mass();
		tool->DrawCanvas_Eff_Data_vs_MC();

		f_output->cd();
		tool->g_data->Write();
		tool->g_MC->Write();
	}
}
