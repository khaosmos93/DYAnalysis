import os

# -- main -- #
BasePath = os.getcwd()
List_SystType = ["sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"]
FitCode = "GetRMS_UnbinnedLikelihoodFit.C"

for SystType in List_SystType:
	ROOTFileName = "ROOTFile_Outputs_SysUncTool_EffCorr_Syst_%s.root" % SystType
	cmd_fit = "root -b -q %s/Local/%s %s" % (SystType, ROOTFileName, FitCode)
	print cmd_fit
	os.system( cmd_fit )

	cmd_mv = "mv c_RelDiff* %s/Local" % (SystType)
	print cmd_mv
	os.system( cmd_mv )

	cmd_mv2 = "mv ROOTFile_SysUnc_EffSF_TagProbe_UnbinnedFit.root %s" % (SystType)
	print cmd_mv2
	os.system( cmd_mv2 )

	print "\n"