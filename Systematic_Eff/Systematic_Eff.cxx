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
}
