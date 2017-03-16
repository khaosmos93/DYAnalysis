#include "InputFileMaker.h"

void InputFileMaker()
{
	TString FileName_Base = "ROOTFile_Input_Comparison_VariousPDF";

	TString FileName_LL = FileName_Base + "_LL.root";
	TFile *f_output_LL = TFile::Open(FileName_LL, "RECREATE");

	TString FileName_MM = FileName_Base + "_MM.root";
	TFile *f_output_MM = TFile::Open(FileName_MM, "RECREATE");

	Tool_InputFileMaker *tool = new Tool_InputFileMaker();
	tool->MakeInputFile_MM( f_output_MM );
	tool->MakeInputFile_LL( f_output_LL );

	f_output_MM->Close();
	f_output_LL->Close();
}