#include "DrawControlPlotTool.h"

void DrawControlPlot(TString version, Bool_t DrawDataDriven = kTRUE, TString NormType = "Lumi" )
{
	DrawControlPlotTool *tool = new DrawControlPlotTool(version, DrawDataDriven, NormType);
	// tool->GenLevelMassSpectrum();
	tool->SetupHistogramNames();
	tool->LoopForHistograms(3);
}

