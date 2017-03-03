#include "Systematic_Eff.h"
#include <utility>

void Systematic_Eff()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );

	TString InputFileName = "../ROOTFile_Systematic_Eff.root";

	vector< TString	> vec_Region;
	vec_Region.push_back( "All" );
	vec_Region.push_back( "BB" );
	vec_Region.push_back( "BEEE" );

	vector< pair<TString, TString> > vec_pair_DENNUM;
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM") );
	vec_pair_DENNUM.push_back( make_pair("DEN_RunBtoF", "NUM_RunBtoF") );
	vec_pair_DENNUM.push_back( make_pair("DEN_RunGtoH", "NUM_RunGtoH") );

	// for( const auto &pair_DENNUM : vec_pair_DENNUM )
	// {
	// 	printf( "DEN, NUM = %s, %s\n", pair_DENNUM.first.Data(), pair_DENNUM.second.Data() );
	// }

	TFile *f_output = TFile::Open( "ROOTFile_Output_Systematic_Eff.root", "RECREATE" );

	Int_t i_canvas = 0;

	ofstream *outFile;
	outFile = new ofstream("log.txt", ios::out);

	for( const auto &Region : vec_Region )
	{
		Tool_Systematic_Eff *tool = new Tool_Systematic_Eff();
		tool->Set_OutFile( outFile );
		tool->Set_iCanvas( &i_canvas );
		tool->Set_InputFileName( InputFileName );
		tool->Set_Output( f_output );


		for( const auto &pair_DENNUM : vec_pair_DENNUM )
		{
			tool->Set_DEN_NUM( pair_DENNUM.first, pair_DENNUM.second );
			tool->Set_Region( Region );

			if( Region == "All" )
			{
				tool->DrawCanvas_Data_vs_MC( "h_RatioPt_M400to2500", pair_DENNUM.first );
				tool->DrawCanvas_Data_vs_MC( "h_RatioPt_M400to2500", pair_DENNUM.second );

				tool->DrawCanvas_Data_vs_MC( "h_dPtOverPt", pair_DENNUM.first );
				tool->DrawCanvas_Data_vs_MC( "h_dPtOverPt", pair_DENNUM.second );

				tool->DrawCanvas_Data_vs_MC( "h_dPtOverPt_M400to2500", pair_DENNUM.first );
				tool->DrawCanvas_Data_vs_MC( "h_dPtOverPt_M400to2500", pair_DENNUM.second );
			}

			tool->DrawCanvas_Mass();
			tool->DrawCanvas_Eff_Data_vs_MC();

			f_output->cd();
			tool->g_data->Write();
			tool->g_MC->Write();
		}


	}

	outFile->close();
}
