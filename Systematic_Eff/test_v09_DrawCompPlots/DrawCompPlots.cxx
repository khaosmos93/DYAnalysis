#include "DrawCompPlots.h"

void DrawCompPlots()
{
	if( gSystem->mkdir( "./Local" ) == 0 )
		printf("Directory [Local] is created\n");

	gSystem->cd( "./Local" );



	vector< TString > vec_Region = {"All", "BB", "BEEE" };

	for( const auto& Region: vec_Region )
	{
		Tool_DrawCompPlots *tool = new Tool_DrawCompPlots();
		tool->Set_DataType( "Data" );
		tool->Set_Region( Region );

		tool->Set_DEN_NUM( "DEN", "NUM" );
		tool->DrawCanvas();

		tool->Set_DEN_NUM( "DEN_RunBtoF", "NUM_RunBtoF" );
		tool->DrawCanvas();

		tool->Set_DEN_NUM( "DEN_RunGtoH", "NUM_RunGtoH" );
		tool->DrawCanvas();

		tool->Set_DataType( "MC" );
		tool->Set_DEN_NUM( "DEN", "NUM" );
		tool->DrawCanvas();
	}



}