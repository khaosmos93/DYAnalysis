#include "Generate_MCTemplate.h"

void Generate_MCTemplate()
{
	vector<TString> vec_Type;
	vec_Type.push_back("ID");
	vec_Type.push_back("Iso");
	vec_Type.push_back("HLTv4p2");
	vec_Type.push_back("HLTv4p3");

	for( auto &Type : vec_Type )
	{
		MCTemplateTool *tool = new MCTemplateTool( Type );
		tool->Produce( -1 ); // -- negative value: all events -- //

		delete tool;
	}
}

