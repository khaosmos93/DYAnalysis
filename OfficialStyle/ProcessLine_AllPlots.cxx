void ProcessLine_AllPlots()
{
	gROOT->ProcessLine(".x dimuonMass.C");
	gROOT->ProcessLine(".x DrawRelUnc_Summary_LogY.cxx");
}