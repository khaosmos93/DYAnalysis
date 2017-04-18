import os
import FWCore.ParameterSet.Config as cms
### USAGE:
###    cmsRun TriggerEff.py <scenario> [ <id> [ <binning1> ... <binningN> ] ]
###    ex> cmsRun TriggerEff.py mc IsoMu20_from_Tight2012
### scenarios:
###   - data_all (default)  
###   - signal_mc

#######################
# -- Setup by hand -- #
#######################
Sample = "" # -- Run2015C, Run2015D, HLTv4p2, HLTv4p3 : will be automatically set -- #
nBin = 40
LowerEdge = "70"
UpperEdge = "130"
Tag_pT = 22
########################
########################

DirName = os.getcwd().split("/")[-1]
if DirName == "RecoID_Iso": Sample = "Run2015D"
elif DirName == "HLTv4p2": Sample = "HLTv4p2"
elif DirName == "HLTv4p3": Sample = "HLTv4p3"
print "Sample: %s" % (Sample)

# -- Dependence on the machine -- #
Machine = ""
BaseLocation = ""
if os.environ["HOSTNAME"] == "ui10.sdfarm.kr":
    Machine = "KISTI"
    BaseLocation = "/cms/home/kplee/scratch/TagProbeTrees/76X/"

elif "tamsa2" in os.environ["HOSTNAME"] or "compute-" in os.environ["HOSTNAME"]:
    Machine = "SNU"
    BaseLocation = "/share_home/kplee/data/TagProbe/76X/"

elif "knu" in os.environ["HOSTNAME"]:
    Machine = "KNU"
    BaseLocation = "/u/user/kplee/TagProbeTrees/"

# -- Dependence on the kind of sample to be measured -- #
NtupleName_Data = ""
NtupleName_MC = ""

if Sample == "Run2015C":
    NtupleName_Data = "v20160425_First76XTrees/NotCompletedYet.root"
    NtupleName_MC   = "v20160425_First76XTrees/NotCompletedYet.root"
elif Sample == "Run2015D":
    NtupleName_Data = "v20160425_First76XTrees/ROOTFile_TPTree_Run2015Dv1_76X_SingleMuon.root"
    NtupleName_MC   = "v20160425_First76XTrees/ROOTFile_TPTree_DYLL_M50_76X_WithWeights.root"
elif Sample == "HLTv4p2":
    NtupleName_Data = "v20160425_First76XTrees/HLTv4p2/ROOTFile_TPTree_Run2015Dv1_76X_SingleMuon_Split_HLTv4p2.root"
    NtupleName_MC   = "v20160425_First76XTrees/HLTv4p2/ROOTFile_TPTree_DYLL_M50_76X_WithWeights.root"
elif Sample == "HLTv4p3":
    NtupleName_Data = "v20160425_First76XTrees/HLTv4p3/ROOTFile_TPTree_Run2015Dv1_76X_SingleMuon_Split_HLTv4p3.root"
    NtupleName_MC   = "v20160425_First76XTrees/HLTv4p3/ROOTFile_TPTree_DYLL_M50_76X_WithWeights.root"

FullLocation_Data = BaseLocation + NtupleName_Data
FullLocation_MC = BaseLocation + NtupleName_MC

print "=" * 50
print "Machine: " + Machine + ", Sample: " + Sample
print "Location of the data ntuple: " + FullLocation_Data
print "Location of the MC ntuple: " + FullLocation_MC
print "=" * 50

import sys
args = sys.argv[1:]
if (sys.argv[0] == "cmsRun"): args =sys.argv[2:]
scenario = "data_all"
if len(args) > 0: scenario = args[0]
print "Will run scenario ", scenario 

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    Variables = cms.PSet(
        mass = cms.vstring("Tag-muon Mass", LowerEdge, UpperEdge, "GeV/c^{2}"),
        pt = cms.vstring("muon p_{T}", "0", "1000", "GeV/c"),
        phi = cms.vstring("muon #phi", "-3.14", "3.14", ""),
        tag_pt = cms.vstring("tag muon p_{T}", "0", "1000", "GeV/c"),
        eta    = cms.vstring("muon #eta", "-2.5", "2.5", ""),
        tag_eta    = cms.vstring("tag muon #eta", "-2.5", "2.5", ""),
        tag_nVertices = cms.vstring("Number of vertices", "0", "999", ""),
        RelPFIso = cms.vstring("Relatvie PF Isolation (Uncorrected)", "0", "999", ""),
        RelPFIso_dBeta = cms.vstring("Relatvie PF Isolation (dBeta)", "0", "999", ""),
        RelPFIso_PFWeighted = cms.vstring("Relatvie PF Isolation (PFWeighted)", "0", "999", ""),
        RelPFIso_PUPPI = cms.vstring("Relatvie PF Isolation (PUPPI)", "0", "999", ""),
        RelTrkIso = cms.vstring("Relatvie Tracker Isolation", "0", "999", ""),
        # run = cms.vstring("Number of vertices", "-999", "999999", "")
    ),

    Categories = cms.PSet(
        mcTrue = cms.vstring("MC true", "dummy[true=1,false=0]"),

        # -- ID Flags
        IDFlag_Tight = cms.vstring("isTightMuon", "dummy[pass=1,fail=0]"),
        IDFlag_HighPtMuon = cms.vstring("isHighPtMuon", "dummy[pass=1,fail=0]"),
        IDFlag_GLB = cms.vstring("isGlobalMuon", "dummy[pass=1,fail=0]"),

        # -- Trigger Flags
        IsoMu20 = cms.vstring("IsoMu20", "dummy[pass=1,fail=0]"),
        IsoTkMu20 = cms.vstring("IsoTkMu20", "dummy[pass=1,fail=0]"),
        Mu45_eta2p1 = cms.vstring("Mu45_eta2p1", "dummy[pass=1,fail=0]"),
        Mu50 = cms.vstring("Mu50", "dummy[pass=1,fail=0]"),

        # -- Tag Flags
        tag_IsoMu20 = cms.vstring("tag_IsoMu20", "dummy[pass=1,fail=0]"),
        tag_IsoMu24_eta2p1 = cms.vstring("tag_IsoMu24_eta2p1", "dummy[pass=1,fail=0]"),
    ),

    Expressions = cms.PSet(
        Exp_IsoMu20_OR_IsoTkMu20 = cms.vstring("Exp_IsoMu20_OR_IsoTkMu20", "IsoMu20==1 || IsoTkMu20==1", "IsoMu20", "IsoTkMu20")
    ),

    Cuts = cms.PSet(
                IsoMu20_OR_IsoTkMu20 = cms.vstring("IsoMu20_OR_IsoTkMu20", "Exp_IsoMu20_OR_IsoTkMu20", "0.5"),
                PFIso_015 = cms.vstring("RelPFIso(Uncorrected) < 0.15", "RelPFIso", "0.15"),
                dBeta_015 = cms.vstring("RelPFIso(dBeta) < 0.15", "RelPFIso_dBeta", "0.15"),
                PFWeighted_015 = cms.vstring("RelPFIso(PFWeighted) < 0.15", "RelPFIso_PFWeighted", "0.15"),
                PUPPI_015 = cms.vstring("RelPFIso(PUPPI) < 0.15", "RelPFIso_PUPPI", "0.15"),
                TrkIso_010 = cms.vstring("RelTrkIso < 0.10", "RelTrkIso", "0.10"),
    ),

    PDFs = cms.PSet(
        voigtPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[90,80,100], width[2.495], sigma[3,1,20])",
            "Exponential::backgroundPass(mass, lp[0,-5,5])",
            "Exponential::backgroundFail(mass, lf[0,-5,5])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpo = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        # -- PDF Set for re-calculating abnormal errors
        vpvPlusExpo2 = cms.vstring(
            "Voigtian::signal1(mass, mean1[91,84,98], width[2.495], sigma1[2.5,1,6])",
            "Voigtian::signal2(mass, mean2[91,81,101], width,        sigma2[5,1,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        # -- Separate signal model of passing probe and failing probe -- #
        vpvPlusExpo3 = cms.vstring(
            "Voigtian::signalPass1(mass, meanPass1[91,84,98], width[2.495], sigmaPass1[2.5,1,6])",
            "Voigtian::signalPass2(mass, meanPass2[91,81,101], width,        sigmaPass2[5,1,10])",
            "SUM::signalPass(vFracPass[0.8,0,1]*signalPass1, signalPass2)",
            "Voigtian::signalFail1(mass, meanFail1[91,84,98], width[2.495], sigmaFail1[2.5,1,6])",
            "Voigtian::signalFail2(mass, meanFail2[91,81,101], width,        sigmaFail2[5,1,10])",
            "SUM::signalFail(vFracFail[0.8,0,1]*signalFail1, signalFail2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        # -- Refit to correct abnormal error -- #
        vpvPlusExpo4 = cms.vstring(
            "Voigtian::signalPass1(mass, meanPass1[91.1,84.1,98.1], width[2.496], sigmaPass1[2.6,1.1,6.1])",
            "Voigtian::signalPass2(mass, meanPass2[91.1,81.1,101.1], width,        sigmaPass2[5.1,1.1,10.1])",
            "SUM::signalPass(vFracPass[0.81,0.1,1.1]*signalPass1, signalPass2)",
            "Voigtian::signalFail1(mass, meanFail1[91.1,84.1,98.1], width[2.496], sigmaFail1[2.6,1,7.1.1])",
            "Voigtian::signalFail2(mass, meanFail2[91.1,81.1,101.1], width,        sigmaFail2[5.1,1.1,10.1])",
            "SUM::signalFail(vFracFail[0.81,0.1,1.1]*signalFail1, signalFail2)",
            "Exponential::backgroundPass(mass, lp[-0.11,-1.1,0.11])",
            "Exponential::backgroundFail(mass, lf[-0.11,-1.1,0.11])",
            "efficiency[0.91,0,1]",
            "signalFractionInPassing[0.91]"
        ),
        # -- Refit to correct abnormal error -- #
        vpvPlusExpo5 = cms.vstring(
            "Voigtian::signalPass1(mass, meanPass1[91.2,84.1,98.1], width[2.496], sigmaPass1[2.5,1.1,6.1])",
            "Voigtian::signalPass2(mass, meanPass2[91.1,81.3,101.2], width,        sigmaPass2[5.2,1.1,10.1])",
            "SUM::signalPass(vFracPass[0.81,0.02,1.1]*signalPass1, signalPass2)",
            "Voigtian::signalFail1(mass, meanFail1[91,84.1,98.1], width[2.496], sigmaFail1[2.6,1,7.1.1])",
            "Voigtian::signalFail2(mass, meanFail2[91.1,81.1,111.1], width,        sigmaFail2[5.3,1.2,10.3])",
            "SUM::signalFail(vFracFail[0.81,0.1,1.1]*signalFail1, signalFail2)",
            "Exponential::backgroundPass(mass, lp[-0.11,-1.1,0.11])",
            "Exponential::backgroundFail(mass, lf[-0.11,-1.1,0.11])",
            "efficiency[0.91,0,1]",
            "signalFractionInPassing[0.91]"
        ),
        vpvPlusExpoMin70 = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,3,10])",
            "SUM::signal(vFrac[0.8,0.5,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0.7,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusQuad = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Chebychev::backgroundPass(mass, {cPass1[0,-1,1], cPass2[0,-1,1]})",
            "Chebychev::backgroundFail(mass, {cFail1[0,-1,1], cFail2[0,-1,1]})",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusQuad2 = cms.vstring(
            "Voigtian::signalPass1(mass, meanPass1[91,84,98], width[2.495], sigmaPass1[2.5,1,6])",
            "Voigtian::signalPass2(mass, meanPass2[91,81,101], width,        sigmaPass2[5,1,10])",
            "SUM::signalPass(vFracPass[0.8,0,1]*signalPass1, signalPass2)",
            "Voigtian::signalFail1(mass, meanFail1[91,84,98], width[2.495], sigmaFail1[2.5,1,6])",
            "Voigtian::signalFail2(mass, meanFail2[91,81,101], width,        sigmaFail2[5,1,10])",
            "SUM::signalFail(vFracFail[0.8,0,1]*signalFail1, signalFail2)",
            "Chebychev::backgroundPass(mass, {cPass1[0,-3,3], cPass2[0,-3,3]})",
            "Chebychev::backgroundFail(mass, {cFail1[0,-3,3], cFail2[0,-3,3]})",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        # -- from 8TeV analysis -- #
        cbelVPlusExpo = cms.vstring(
            "CBShape::signal1(mass, m01[90,89,92], sigma1[2,1,3], alpha1[1,0,5], n1[1,0,10])",
            #"CBShape::signal2(mass, m02[90,89,92], sigma2[2,1,3], alpha2[1,0,5], n2[1,0,10])",
            "Voigtian::signal2(mass, mean2[90,89,92], width2[2.495], sigma2[2,1,3])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        cbelVPlusExpo2 = cms.vstring(
            "CBShape::signal1(mass, m01[90,89,92], sigma1[2,1,3], alpha1[1,0,5], n1[1,0,10])",
            "Voigtian::signal2(mass, mean2[90,89,92], width2[2.495], sigma2[2,1,3])",
            "SUM::signalPass(vFrac[0.8,0,1]*signal1, signal2)",
            "CBShape::signal3(mass, m03[90,89,92], sigma3[2,1,3], alpha3[1,0,5], n3[1,0,10])",
            "Voigtian::signal4(mass, mean4[90,89,92], width4[2.495], sigma4[2,1,3])",
            "SUM::signalFail(vFrac[0.8,0,1]*signal3, signal4)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),      
        cbelVPlusQuadratic = cms.vstring(
            "CBShape::signal1(mass, m01[90,89,92], sigma1[2,1,3], alpha1[1,0,5], n1[1,0,10])",
            #"CBShape::signal2(mass, m02[90,89,92], sigma2[2,1,3], alpha2[1,0,5], n2[1,0,10])",
            "Voigtian::signal2(mass, mean2[90,89,92], width2[2.495], sigma2[2,1,3])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Chebychev::backgroundPass(mass, {cPass1[0,-1,1], cPass2[0,-1,1]})",
            "Chebychev::backgroundFail(mass, {cFail1[0,-1,1], cFail2[0,-1,1]})",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        cbelVPlusQuadratic2 = cms.vstring(
            "CBShape::signal1(mass, m01[90,89,92], sigma1[2,1,3], alpha1[1,0,5], n1[1,0,10])",
            "Voigtian::signal2(mass, mean2[90,89,92], width2[2.495], sigma2[2,1,3])",
            "SUM::signalPass(vFrac[0.8,0,1]*signal1, signal2)",
            "CBShape::signal3(mass, m03[90,89,92], sigma3[2,1,3], alpha3[1,0,5], n3[1,0,10])",
            "Voigtian::signal4(mass, mean4[90,89,92], width4[2.495], sigma4[2,1,3])",
            "SUM::signalFail(vFrac[0.8,0,1]*signal3, signal4)",
            "Chebychev::backgroundPass(mass, {cPass1[0,-1,1], cPass2[0,-1,1]})",
            "Chebychev::backgroundFail(mass, {cFail1[0,-1,1], cFail2[0,-1,1]})",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvGausPlusExpo = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            # -- Signal3: Double Voigtian -- #
            "SUM::signal3(vFrac[0.8,0,1]*signal1, signal2)",

            # -- Signal: C*Gauss + (1-C)*Double Voigtian -- #
            "Gaussian::signal4(mass, mean3[81, 76, 86], sigma3[2,1,3])",
            "SUM::signal(vFrac2[0.2,0,1]*signal4, signal3)",

            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvGausPlusExpo2 = cms.vstring(
            # -- for Passing Probe -- #
            "Voigtian::signalPass1(mass, meanPass1[90,80,100], widthPass[2.495], sigmaPass1[2,1,3])",
            "Voigtian::signalPass2(mass, meanPass2[90,80,100], widthPass,        sigmaPass2[4,2,10])",
            # -- Signal3: Double Voigtian -- #
            "SUM::signalPass3(vFracPass1[0.8,0,1]*signalPass1, signalPass2)",

            # -- Signal: C*Gauss + (1-C)*Double Voigtian -- #
            "Gaussian::signalPass4(mass, meanPass3[81, 76, 86], sigmaPass3[2,1,3])",
            "SUM::signalPass(vFracPass2[0.2,0,1]*signalPass4, signalPass3)",

            # -- for Failing Probe -- #
            "Voigtian::signalFail1(mass, meanFail1[90,80,100], widthFail[2.495], sigmaFail1[2,1,3])",
            "Voigtian::signalFail2(mass, meanFail2[90,80,100], widthFail,        sigmaFail2[4,2,10])",
            # -- Signal3: Double Voigtian -- #
            "SUM::signalFail3(vFracFail1[0.8,0,1]*signalFail1, signalFail2)",

            # -- Signal: C*Gauss + (1-C)*Double Voigtian -- #
            "Gaussian::signalFail4(mass, meanFail3[81, 76, 86], sigmaFail3[2,1,3])",
            "SUM::signalFail(vFracFail2[0.2,0,1]*signalFail4, signalFail3)",

            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),


    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(nBin),
    saveDistributionsPlot = cms.bool(False),

    Efficiencies = cms.PSet(), # will be filled later
)

PassingProbe = ""
if "_from_" in args[1]:
    PassingProbe = args[1].split("_from_")[0]
else:
    PassingProbe = args[1]
print "PassingProbe: " + PassingProbe

import Def_AllTemplatePDF as TemplatePDFs

if PassingProbe == "HighPtMuon":
    Template.PDFs = TemplatePDFs.all_pdfs_ID
elif PassingProbe == "TrkIso_010":
    Template.PDFs = TemplatePDFs.all_pdfs_Iso
elif PassingProbe == "IsoMu20_OR_IsoTkMu20":
    if Sample == "HLTv4p2":
        Template.PDFs = TemplatePDFs.all_pdfs_HLTv4p2
    elif Sample == "HLTv4p3":
        Template.PDFs = TemplatePDFs.all_pdfs_HLTv4p3
else:
    Template.PDFs = cms.PSet()

print "[Available PDF list]"
print Template.PDFs
print "#"*100
print "\n\n"

PT_ETA_BINS = cms.PSet(
                        pt     = cms.vdouble( 10, 22, 40, 70, 250 ),
                        # pt     = cms.vdouble( 10, 22 ),
                        eta = cms.vdouble( -2.4, -1.2, -0.3, 0.3, 1.2, 2.4 ),

                        # Tight2012 = cms.vstring("pass"),
                        tag_IsoMu20 = cms.vstring("pass"),
                        tag_pt =  cms.vdouble(Tag_pT,9999)
)

PT_ETA_BINS_Coarse = cms.PSet(
                        pt     = cms.vdouble( 10, 22, 40, 70, 250 ),
                        # pt     = cms.vdouble( 10, 22 ),
                        eta = cms.vdouble( -2.4, -0.9, 0.9, 2.4 ),

                        # Tight2012 = cms.vstring("pass"),
                        tag_IsoMu20 = cms.vstring("pass"),
                        tag_pt =  cms.vdouble(22,9999)
)

PT_ETA_BINS_Fine = cms.PSet(
						pt     = cms.vdouble( 10, 16, 22, 31, 40, 55, 70, 85, 250 ),
                        # pt     = cms.vdouble( 10, 16, 22, 31, 40, 55, 70, 250 ),
                        # pt     = cms.vdouble( 10, 22 ),
                        eta = cms.vdouble( -2.4, -2.1, -1.2, -0.9, -0.3, 0, 0.3, 0.9, 1.2, 2.1, 2.4 ),

                        # Tight2012 = cms.vstring("pass"),
                        tag_IsoMu20 = cms.vstring("pass"),
                        tag_pt =  cms.vdouble(22,9999)
)

PT_BINS = cms.PSet(
                        pt     = cms.vdouble( 0, 9999 ), # -- will be set later -- #
                        
                        eta = cms.vdouble( -2.4, 2.4 ),

                        tag_IsoMu20 = cms.vstring("pass"),
                        tag_pt =  cms.vdouble(22,9999),
                        tag_eta =  cms.vdouble(-2.4, 2.4),
)
# -- pT binning for Reco + ID efficiency
if "TightMuon" in PassingProbe or "HighPtMuon" in PassingProbe:
    PT_BINS.pt = cms.vdouble( 10, 22, 30, 40, 50, 60, 70, 80, 90, 100, 250, 1000 )

# -- pT binning for Isolation efficiency
elif "TrkIso" in PassingProbe: 
    PT_BINS.pt = cms.vdouble( 10, 22, 30, 40, 50, 60, 70, 80, 90, 100, 250, 1000 ) 

# -- pT binning for trigger efficiency
elif "Mu20" in PassingProbe: 
    PT_BINS.pt = cms.vdouble( 10, 22, 30, 40, 50, 60, 70, 80, 90, 100, 250, 1000 ) 



ETA_BINS = cms.PSet(
                       # -- eta binning for Isolation & Trigger Efficiency Measurement
                       eta = cms.vdouble(-2.4, 2.4), # -- will be set later -- #

                       pt     = cms.vdouble( 10, 250 ),

                       tag_IsoMu20 = cms.vstring("pass"),
                       tag_pt =  cms.vdouble(22,9999),
                       tag_eta =  cms.vdouble(-2.4, 2.4),
                       )    
# -- eta binning for Reco+ID Efficiency -- #
if "TightMuon" in PassingProbe or "HighPtMuon" in PassingProbe:
    # ETA_BINS.eta = cms.vdouble(-2.4, -2.1, -1.9, -1.5, -1.1, -0.9, -0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.5, 1.9, 2.1, 2.4) # -- Used for 8TeV Analysis -- #
    ETA_BINS.eta = cms.vdouble(-2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4)

# -- eta binning for Isolation Efficiency -- #
elif "TrkIso" in PassingProbe:
    # ETA_BINS.eta = cms.vdouble(-2.4, -2.1, -1.9, -1.7, -1.5, -1.1, -0.9, -0.7, -0.5, 0.5, 0.7, 0.9, 1.1, 1.5, 1.7, 1.9, 2.1, 2.4) # -- Used for 8TeV Analysis -- #
    ETA_BINS.eta = cms.vdouble(-2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4)

# -- eta binning for Trigger Efficiency -- #
elif "Mu20" in PassingProbe:
    # ETA_BINS.eta = cms.vdouble(-2.4, -2.1, -1.9, -1.7, -1.5, -1.1, -0.9, -0.7, -0.5, 0.5, 0.7, 0.9, 1.1, 1.5, 1.7, 1.9, 2.1, 2.4) # -- Used for 8TeV Analysis -- #
    ETA_BINS.eta = cms.vdouble(-2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4)
    ETA_BINS.pt = cms.vdouble( 22, 250 )


PHI_BINS = cms.PSet(
                    phi     = cms.vdouble(-3.1,-2.7,-2.2,-1.8,-1.4,-1.0,-0.6,-0.2,0.2,0.6,1.0,1.4,1.8,2.2,2.7,3.1),

                    pt     = cms.vdouble( 10, 250 ),
                    eta = cms.vdouble( -2.4, 2.4 ),

                    tag_IsoMu20 = cms.vstring("pass"),
                    tag_pt =  cms.vdouble(22,9999),
                    tag_eta =  cms.vdouble(-2.4, 2.4),
                    )
if "Mu20" in PassingProbe:
    PHI_BINS.pt = cms.vdouble( 22, 250 )



VTX_BINS  = cms.PSet(
    tag_nVertices = cms.vdouble(2.5, 4.5, 6.5, 8.5, 10.5, 12.5, 14.5, 16.5, 18.5, 20.5, 22.5, 24.5, 26.5),

    pt     = cms.vdouble(  10, 250 ),
    eta = cms.vdouble( -2.4, 2.4 ),
    
    tag_IsoMu20 = cms.vstring("pass"),
    tag_pt =  cms.vdouble(22,9999),
    tag_eta =  cms.vdouble(-2.4, 2.4),
)
if "Mu20" in PassingProbe:
    VTX_BINS.pt = cms.vdouble( 22, 250 )



process.TnP_MuonID = Template.clone(
    InputFileNames = cms.vstring(),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("muonEffs"),
    OutputFileName = cms.string("TnP_MuonEffs_%s.root" % scenario),
    Efficiencies = cms.PSet(),
)

#Add the variables for PU reweighting
if "_weight" in scenario:
    process.TnP_MuonID.WeightVariable = cms.string("weight")
    process.TnP_MuonID.Variables.weight = cms.vstring("weight","-10","10","")

if scenario=="data_25ns":
    process.TnP_MuonID.InputFileNames = cms.vstring(
                                                    # put here the trees corresponding to data
                                                    "file:"+FullLocation_Data
                                                    )

if "mc" in scenario:
    process.TnP_MuonID.InputFileNames = cms.vstring(
                                                    # put here the trees corresponding to mc
                                                    "file:"+FullLocation_MC
                                                    # "file:/home/kplee/data/TagProbe/25ns/v20151105_MuonPhys_Run2015CDv3v4_SingleMuon_Run246908to259891/ROOTFile_TPTree_ZMuMu_MC_All_WithWeights.root"
                                                    )

#IDS = [ "IsoMu20","Mu20","L2fL1sMu16L1f0L2Filtered10Q","IsoTkMu20","L1sMu16"]
IDS = [args[1]] #here the id is taken from the arguments provided to cmsRun 
# ALLBINS = [("pt",PT_BINS),("phi",PHI_BINS),("eta",ETA_BINS),("vtx",VTX_BINS)]
# ALLBINS = [("pt",PT_BINS),("eta",ETA_BINS),("vtx",VTX_BINS)]
# ALLBINS = [("eta",ETA_BINS)]
ALLBINS = [("pteta", PT_ETA_BINS)]

if len(args) > 1 and args[1] not in IDS: IDS += [ args[1] ]
for ID in IDS:
    print "now doing ",ID
    if len(args) > 1 and ID != args[1]: continue
    for X,B in ALLBINS:
        if len(args) > 2 and X not in args[2:]: continue
        #Add the information about ID and binning into the outputFileName
        module = process.TnP_MuonID.clone(OutputFileName = cms.string("TnP_MuonTrigger_%s_%s_%s.root" % (scenario, ID, X)))
        
        shape = cms.vstring("TmpPlusExpo_PtBin0_EtaBin0")
        shape.extend( ["*eta_bin0*pt_bin0*tag_pt_bin0*", "TmpPlusExpo_PtBin0_EtaBin0"] )
        shape.extend( ["*eta_bin0*pt_bin1*", "TmpPlusExpo_PtBin1_EtaBin0"] )
        shape.extend( ["*eta_bin0*pt_bin2*", "TmpPlusExpo_PtBin2_EtaBin0"] )
        shape.extend( ["*eta_bin0*pt_bin3*", "TmpPlusExpo_PtBin3_EtaBin0"] )

        shape.extend( ["*eta_bin1*pt_bin0*tag_pt_bin0*", "TmpPlusExpo_PtBin0_EtaBin1"] )
        shape.extend( ["*eta_bin1*pt_bin1*", "TmpPlusExpo_PtBin1_EtaBin1"] )
        shape.extend( ["*eta_bin1*pt_bin2*", "TmpPlusExpo_PtBin2_EtaBin1"] )
        shape.extend( ["*eta_bin1*pt_bin3*", "TmpPlusExpo_PtBin3_EtaBin1"] )

        shape.extend( ["*eta_bin2*pt_bin0*tag_pt_bin0*", "TmpPlusExpo_PtBin0_EtaBin2"] )
        shape.extend( ["*eta_bin2*pt_bin1*", "TmpPlusExpo_PtBin1_EtaBin2"] )
        shape.extend( ["*eta_bin2*pt_bin2*", "TmpPlusExpo_PtBin2_EtaBin2"] )
        shape.extend( ["*eta_bin2*pt_bin3*", "TmpPlusExpo_PtBin3_EtaBin2"] )

        shape.extend( ["*eta_bin3*pt_bin0*tag_pt_bin0*", "TmpPlusExpo_PtBin0_EtaBin3"] )
        shape.extend( ["*eta_bin3*pt_bin1*", "TmpPlusExpo_PtBin1_EtaBin3"] )
        shape.extend( ["*eta_bin3*pt_bin2*", "TmpPlusExpo_PtBin2_EtaBin3"] )
        shape.extend( ["*eta_bin3*pt_bin3*", "TmpPlusExpo_PtBin3_EtaBin3"] )

        shape.extend( ["*eta_bin4*pt_bin0*tag_pt_bin0*", "TmpPlusExpo_PtBin0_EtaBin4"] )
        shape.extend( ["*eta_bin4*pt_bin1*", "TmpPlusExpo_PtBin1_EtaBin4"] )
        shape.extend( ["*eta_bin4*pt_bin2*", "TmpPlusExpo_PtBin2_EtaBin4"] )
        shape.extend( ["*eta_bin4*pt_bin3*", "TmpPlusExpo_PtBin3_EtaBin4"] )
        
        #DEN: Binning
        DEN = B.clone(); num = ID; numstate = "pass"

        if "_from_" in ID:
            parts = ID.split("_from_")
            num = parts[0]
            # add Additional ID conditions to the binning ... 
            # ex> cmsRun TriggerEff.py mc IsoMu20_and_Tight2012_from_SIP4_and_PFIso25 => SIP4 and PFIso25 info. is added to the binning definition
            for D in parts[1].split("_and_"):
                if D == "PFIso_015": DEN.RelPFIso = cms.vdouble(0, 0.15)
                elif D == "dBeta_015": DEN.RelPFIso_dBeta = cms.vdouble(0, 0.15)
                elif D == "PFWeighted_015": DEN.RelPFIso_PFWeighted = cms.vdouble(0, 0.15)
                elif D == "PUPPI_015": DEN.RelPFIso_PUPPI = cms.vdouble(0, 0.15)
                elif D == "TrkIso_010": DEN.RelTrkIso = cms.vdouble(0, 0.10)
                elif D == "TightMuon": DEN.IDFlag_Tight = cms.vstring("pass")
                elif D == "HighPtMuon": DEN.IDFlag_HighPtMuon = cms.vstring("pass")
                # Set D as the variable of DEN ... DEN.D = cms.vstring("pass")
                else: setattr(DEN, D, cms.vstring("pass"))

        print "#" * 100
        print "Binning variable: ", X
        print "Binning: ", DEN
        print "PDF: ", shape

        # numString: EfficiencyCategoryState variable. 
        # ex> cmsRun TriggerEff.py mc IsoMu20_and_Tight2012_from_SIP4_and_PFIso25 => numString = cms.vstring("IsoMu20", "pass", "Tight2012", "pass")
        numString = cms.vstring()
        for N in num.split("_and_"):
            if N == "IsoMu20_OR_IsoTkMu20":
                numString += [N, "above"]
            elif N == "PFIso_015" or N == "dBeta_015" or N == "PFWeighted_015" or N == "PUPPI_015" or N == "TrkIso_010":
                numString += [N, "below"]
            elif N == "TightMuon":
                numString += ["IDFlag_Tight", "pass"]
            elif N == "HighPtMuon":
                numString += ["IDFlag_HighPtMuon", "pass"]
            else:
                numString += [N, "pass"]

        print "Passing probe condition: ", numString
        print "#" * 100
        print "\n"
        
        #Set Efficiency
        setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
            EfficiencyCategoryAndState = numString,
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = DEN,
            BinToPDFmap = shape
        ))

        #Add mcTure Efficinecy when mc fitting
        if scenario.find("mc") != -1:
            setattr(module.Efficiencies, ID+"_"+X+"_mcTrue", cms.PSet(
                EfficiencyCategoryAndState = numString,
                UnbinnedVariables = cms.vstring("mass"),
                BinnedVariables = DEN.clone(mcTrue = cms.vstring("true"))
            ))
            #When mc is PU-weighted, "weight" variable is added to UnbinnedVariables
            if "_weight" in scenario:
                getattr(module.Efficiencies, ID+"_"+X          ).UnbinnedVariables.append("weight")
                getattr(module.Efficiencies, ID+"_"+X+"_mcTrue").UnbinnedVariables.append("weight")

        #Add module to the process
        setattr(process, "TnP_MuonID_"+ID+"_"+X, module)        
        #Add a path of module to the process
        setattr(process, "run_"+ID+"_"+X, cms.Path(module))
