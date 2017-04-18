import FWCore.ParameterSet.Config as cms

all_pdfs_ID = cms.PSet(
TmpPlusExpo_PtBin0_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin0_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin0_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin0_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin0_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin0_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin0_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin0_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin0_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin0_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin0_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin0_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin0_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin1_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin1_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin1_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin1_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin1_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin1_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin1_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin1_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin1_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin1_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin1_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin1_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin2_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin2_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin2_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin2_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin2_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin2_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin2_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin2_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin2_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin2_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin2_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin2_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin3_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin3_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin3_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin3_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin3_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin3_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin3_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin3_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin3_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin3_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin3_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin3_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin4_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin4_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin4_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin4_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin4_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin4_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin4_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin4_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin4_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin4_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Pass_PtBin4_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_ID.root\", \"h_Fail_PtBin4_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

)

all_pdfs_Iso = cms.PSet(
TmpPlusExpo_PtBin0_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin0_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin0_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin0_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin0_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin0_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin0_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin0_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin0_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin0_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin0_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin0_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin0_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin1_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin1_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin1_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin1_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin1_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin1_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin1_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin1_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin1_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin1_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin1_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin1_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin2_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin2_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin2_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin2_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin2_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin2_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin2_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin2_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin2_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin2_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin2_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin2_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin3_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin3_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin3_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin3_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin3_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin3_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin3_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin3_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin3_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin3_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin3_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin3_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin4_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin4_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin4_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin4_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin4_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin4_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin4_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin4_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin4_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin4_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Pass_PtBin4_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_Iso.root\", \"h_Fail_PtBin4_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

)

all_pdfs_HLTv4p2 = cms.PSet(
TmpPlusExpo_PtBin0_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin0_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin0_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin0_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin0_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin0_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin0_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin0_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin0_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin0_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin0_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin0_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin0_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin1_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin1_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin1_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin1_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin1_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin1_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin1_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin1_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin1_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin1_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin1_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin1_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin2_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin2_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin2_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin2_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin2_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin2_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin2_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin2_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin2_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin2_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin2_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin2_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin3_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin3_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin3_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin3_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin3_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin3_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin3_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin3_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin3_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin3_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin3_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin3_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin4_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin4_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin4_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin4_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin4_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin4_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin4_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin4_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin4_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin4_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Pass_PtBin4_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p2.root\", \"h_Fail_PtBin4_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

)

all_pdfs_HLTv4p3 = cms.PSet(
TmpPlusExpo_PtBin0_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin0_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin0_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin0_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin0_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin0_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin0_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin0_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin0_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin0_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin0_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin0_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin0_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin0_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin1_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin1_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin1_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin1_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin1_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin1_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin1_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin1_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin1_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin1_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin1_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin1_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin1_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin2_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin2_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin2_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin2_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin2_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin2_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin2_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin2_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin2_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin2_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin2_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin2_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin2_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin3_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin3_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin3_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin3_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin3_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin3_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin3_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin3_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin3_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin3_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin3_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin3_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin3_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin0 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin4_EtaBin0\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin4_EtaBin0\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin1 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin4_EtaBin1\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin4_EtaBin1\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin2 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin4_EtaBin2\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin4_EtaBin2\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin3 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin4_EtaBin3\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin4_EtaBin3\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin4 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin4_EtaBin4\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin4_EtaBin4\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

TmpPlusExpo_PtBin4_EtaBin5 = cms.vstring(
"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",
"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",
"ZGeneratorLineShape::signalPhyPass(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Pass_PtBin4_EtaBin5\")",
"ZGeneratorLineShape::signalPhyFail(mass, \"/cms/home/kplee/Physics/DYAnalysis_76X/Investigation/TnPSystematic_MCTemplate/ROOTFile_MCTemplates_HLTv4p3.root\", \"h_Fail_PtBin4_EtaBin5\")",
"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
"FCONV::signalPass(mass, signalPhyPass, signalResPass)",
"FCONV::signalFail(mass, signalPhyFail, signalResFail)",
"efficiency[0.9,0,1]",
"signalFractionInPassing[0.9]"
),

)

