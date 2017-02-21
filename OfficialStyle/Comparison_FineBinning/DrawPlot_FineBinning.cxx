#include "DrawPlot_FineBinning.h"

void DrawPlot_FineBinning()
{
	DrawingTool *tool = new DrawingTool();
	tool->MakeInputFile_Dielectron();
	tool->MakeInputFile_Dimuon();
	tool->MakeInputFile_Combined();

	tool->Load_Histograms( "Dielectron" );
	tool->DrawCanvas();
	tool->DrawCanvas(15, 60);
	tool->DrawCanvas(60, 120);
	tool->DrawCanvas(120, 3000);

	tool->Load_Histograms( "Dimuon" );
	tool->DrawCanvas();
	tool->DrawCanvas(15, 60);
	tool->DrawCanvas(60, 120);
	tool->DrawCanvas(120, 3000);

	tool->Load_Histograms( "Combined" );
	tool->DrawCanvas();
	tool->DrawCanvas(15, 60);
	tool->DrawCanvas(60, 120);
	tool->DrawCanvas(120, 3000);
}