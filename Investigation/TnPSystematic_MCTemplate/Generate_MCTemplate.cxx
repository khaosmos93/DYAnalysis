#include "Generate_MCTemplate.h"

void Generate_MCTemplate()
{
	TString Arr_Type[3] = {"ID", "Iso", "HLTv4p2", "HLTv4p3"};

	for( auto &Type : Arr_Type )
	{
		MCTemplateTool *tool = new MCTemplateTool( Type );
		tool->Produce( 10000 ); // -- negative value: all events -- //
	}
}

