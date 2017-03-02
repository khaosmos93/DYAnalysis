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
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus0") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus1") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus2") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus3") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus4") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus5") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus6") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM_DENPlus7") );
	vec_pair_DENNUM.push_back( make_pair("DEN", "NUM") );

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
		for( const auto &pair_DENNUM : vec_pair_DENNUM )
		{
			Tool_Systematic_Eff *tool = new Tool_Systematic_Eff();
			tool->Set_OutFile( outFile );
			tool->Set_iCanvas( &i_canvas );
			tool->Set_InputFileName( InputFileName );
			tool->Set_Output( f_output );
			tool->Set_DEN_NUM( pair_DENNUM.first, pair_DENNUM.second );
			tool->Set_Region( Region );
			
			tool->DrawCanvas_Mass();
			tool->DrawCanvas_Eff_Data_vs_MC();

			f_output->cd();
			tool->g_data->Write();
			tool->g_MC->Write();
		}

		vector< pair<TString, TString> > vec_pair_NUM_Legend;
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus0", "1. NUM=DEN") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus1", "2. +pixelHits") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus2", "3. +trackerLayers") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus3", "4. +dB") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus4", "5. +vertex chi2") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus5", "6. +muonHits") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus6", "7. +matchedStations") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM_DENPlus7", "8. +RelPtError") );
		vec_pair_NUM_Legend.push_back( make_pair("NUM", "9. +OS (=Full Selection)") );

		DrawCanvas_Eff_VariousNUM( f_output, "Data", Region, vec_pair_NUM_Legend);
	}
	outFile->close();


}
