import FWCore.ParameterSet.Config as cms
### USAGE:
###    cmsRun fitMuonID_Paper2010.py <scenario> [ <id> [ <binning1> ... <binningN> ] ]
###
### scenarios:
###   - data_all (default)  
###   - signal_mc

import sys
scenario = "datalike_mc"
MODE=str(sys.argv[2])
PDF4FIT = str(sys.argv[3])
print "Will run scenario ", scenario
TRIGGER_PATH = str(sys.argv[4])

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(2),
    SaveWorkspace = cms.bool(False),
    WeightVariable = cms.string("weight"),

    Variables = cms.PSet(
        mass = cms.vstring("Tag-muon Mass", "70", "110", "GeV/c^{2}"),
        pt = cms.vstring("muon p_{T}", "0", "1000", "GeV/c"),
        eta    = cms.vstring("muon #eta", "-2.5", "2.5", ""),
        weight = cms.vstring("weight","0","10",""),
        abseta = cms.vstring("muon |#eta|", "0", "2.5", ""),
        phi    = cms.vstring("muon #phi at vertex", "-3.1416", "3.1416", ""),
        charge = cms.vstring("muon charge", "-2.5", "2.5", ""),
        run = cms.vstring("run range","160431","180252",""),
        run2 = cms.vstring("run range2","160431","180252",""),
        tag_pt = cms.vstring("Tag p_{T}", "0", "1000", "GeV/c"),
        tag_nVertices   = cms.vstring("Number of vertices", "0", "999", ""),
        tag_nVerticesDA = cms.vstring("Number of vertices", "0", "999", ""),
        isoTrk03Abs    = cms.vstring("Probe abs trk iso", "-2", "9999999", ""),
        tag_combRelIso = cms.vstring("Tag comb rel iso", "-2", "9999999", ""),
        l3pt = cms.vstring("L3 p_{T}", "-99999999", "999999999", ""),
        pair_nL1EG5 = cms.vstring("Number of L1 EG5", "-1", "9999", ""),
    ),      
    
    Categories = cms.PSet(
        Glb   = cms.vstring("Global", "dummy[pass=1,fail=0]"),
        VBTF     = cms.vstring("VBTFLike", "dummy[pass=1,fail=0]"),
        VBTFold  = cms.vstring("VBTFLike", "dummy[pass=1,fail=0]"),
        Tight2012  = cms.vstring("Tight2012", "dummy[pass=1,fail=0]"),
        TMOSL = cms.vstring("TMOneStationLoose", "dummy[pass=1,fail=0]"),
        TMOST = cms.vstring("TMOneStationTight", "dummy[pass=1,fail=0]"),
        PF    = cms.vstring("PF Muon", "dummy[pass=1,fail=0]"),
        TM    = cms.vstring("Tracker Muon", "dummy[pass=1,fail=0]"),
        TMA   = cms.vstring("Arbitrated Tracker Muon", "dummy[pass=1,fail=0]"),
        IsolRelCombNoECALR03 = cms.vstring("IsolRelCombNoECALR03", "dummy[pass=1,fail=0]"),
        #PF based
        IsolpfMyHadIso04 = cms.vstring("IsolpfMyHadIso04", "dummy[pass=1,fail=0]"),
        IsolPFRelCombNoEGammaR03 = cms.vstring("IsolPFRelCombNoEGammaR03", "dummy[pass=1,fail=0]"),
        IsolPFRelCombNoEGammaR03PU = cms.vstring("IsolPFRelCombNoEGammaR03PU", "dummy[pass=1,fail=0]"),
        Mu15  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu20  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu24  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu30  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu40  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu40_eta2p1  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        IsoMu15   = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        IsoMu17   = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        IsoMu24   = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu7 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu6 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu3 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu13Mu8_Mu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu13Mu8_Mu13 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu17Mu8_Mu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu17Mu8_Mu17 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu17Mu8_Mu8leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu17Mu8_Mu17leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu17TkMu8_TkMu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu17TkMu8_TkMu8leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22Mu8_Mu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22Mu8_Mu22 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22Mu8_Mu8leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22Mu8_Mu22leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22TkMu8_TkMu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22TkMu8_TkMu8leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu22TkMu8_TkMu8_DzFilter = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_DoubleMu17Mu8_Mu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_DoubleMu17Mu8_Mu17 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_DoubleMu17Mu8_Mu8leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_DoubleMu17Mu8_Mu17leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_DoubleMu17TkMu8_TkMu8 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_DoubleMu17TkMu8_TkMu8leg = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_Mu20 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_Mu24 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_Mu30 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        mcTrue = cms.vstring("MC true", "dummy[true=1,false=0]"),
    ),

    PDFs = cms.PSet(
        voigtPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[90,89,92], width[2.495], sigma[3,1,10])",
            "Exponential::backgroundPass(mass, lp[0,-1,1])",
            "Exponential::backgroundFail(mass, lf[0,-1,1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        gaussPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[90,89,92], width[2.495], sigma[3,1,10])",
            "Exponential::backgroundPass(mass, lp[0,-1,1])",
            "Exponential::backgroundFail(mass, lf[0,-1,1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpo = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,89,92], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,89,92], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),

        vpvPlusQuadratic = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,89,92], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,89,92], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Chebychev::backgroundPass(mass, {cPass1[0,-1,1], cPass2[0,-1,1]})",
            "Chebychev::backgroundFail(mass, {cFail1[0,-1,1], cFail2[0,-1,1]})",
            #"Exponential::backgroundPass(mass, lp[0,-5,5])",
            #"Exponential::backgroundFail(mass, lf[0,-5,5])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
       #http://root.cern.ch/root/html/src/RooCBShape.cxx.html#WRH3ZD
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
    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(40),

    Efficiencies = cms.PSet(), # will be filled later
)

ETA_BINS = cms.PSet(
     pt  = cms.vdouble(8,250),
     eta = cms.vdouble(-2.4,-1.9,-1.5,-1.1,-0.9,-0.7,-0.5,-0.1,0.1,0.5,0.7,0.9,1.1,1.5,1.9,2.4)
)
#finer granulariy in eta
ETA2_BINS = cms.PSet(
     pt  = cms.vdouble(8,250),
     eta = cms.vdouble(-2.4,-2.1,-1.9,-1.7,-1.5,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.5,1.7,1.9,2.1,2.4)
)
#Use pT cut as in POG
ETA3_BINS = cms.PSet(
     pt  = cms.vdouble(8,500),
     eta = cms.vdouble(-2.4,-2.1,-1.9,-1.7,-1.5,-1.1,-0.9,-0.7,-0.5,0.5,0.7,0.9,1.1,1.5,1.7,1.9,2.1,2.4),
)

ETA4_BINS = cms.PSet(
     pt  = cms.vdouble(8,250),
     eta = cms.vdouble(-2.4,-2.1,-1.9,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.85,-0.8,-0.75,-0.7,-0.65,-0.6,-0.5,-0.4,-0.3,-0.25,-0.2,-0.15,-0.1
,0.0,0.1,0.15,0.2,0.25,0.3,0.4,0.5,0.6,0.65,0.7,0.75,0.8,0.85,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.9,2.1,2.4)
)

PT_BINS = cms.PSet(
     pt = cms.vdouble(8,10,13,17,20,24,26,28,30,32,36,40,44,48,55,70,100,220,400),
     eta = cms.vdouble(-2.4,2.4)
)
#coarser granularity in pT: use the one as in POG
PT2_BINS = cms.PSet(
     pt = cms.vdouble(8,17,26,32,44,55,70,100,220,400),
     eta = cms.vdouble(-2.4,2.4)
)

PT3_BINS = cms.PSet(
     pt = cms.vdouble(8,17,22,30,40,50,65,250,500),
     eta = cms.vdouble(-2.4,2.4)
)   

VTX_BINS  = cms.PSet(
    pt     = cms.vdouble(8, 250),
    eta = cms.vdouble(-2.4, 2.4),
    tag_nVertices = cms.vdouble(0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,13.5,14.5,15.5,16.5,17.5,20.5,22.5,25.0,27.5,30.0,35.0)
)
PT_ETA_BINS = cms.PSet(
#bin11, special stoyan 1
    eta = cms.vdouble(-2.4,-2.1,-1.9,-1.7,-1.5,-1.1,-0.9,-0.7,-0.5,0.5,0.7,0.9,1.1,1.5,1.7,1.9,2.1,2.4),
    pt = cms.vdouble(8,17,22,30,40,50,65,250,500),
)

PT_ETA_BINS2 = cms.PSet(
 eta = cms.vdouble(-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4),
 pt = cms.vdouble(10, 20, 25, 30, 35, 40, 50, 60, 90, 140, 300, 500),
)

PREFIX="/scratch/scratch95/a/asvyatko/inputFiles_2013/single2012/cmssw533_parked/"

#Define the list of input root files
inString = []
fileNamePostfix = ''
iFile = ''
for ifile in sys.argv[5:]:
    inString.append(PREFIX+ifile)
    iFile = iFile+ifile
if 'ZWindow' in iFile: fileNamePostfix += '_ZWindow'
if 'PU' in iFile: fileNamePostfix += '_PU'
if '20_' in iFile: fileNamePostfix += '_M20'
elif '10_' in iFile: fileNamePostfix += '_M10'
elif '200_' in iFile: fileNamePostfix += '_M200'
if 'V5' in ifile: fileNamePostfix += 'V5'

process.TnP_MuonID = Template.clone(
    InputFileNames = cms.vstring(inString),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("TnP_%s_dummy.root" % scenario),
    Efficiencies = cms.PSet(),
)

#categories you needif 
if MODE == "vtx": bins = VTX_BINS
elif MODE == "eta": bins = ETA_BINS
elif MODE == "pt": bins = PT_BINS
elif MODE == "eta2": bins = ETA2_BINS
elif MODE == "eta3": bins = ETA3_BINS
elif MODE == "eta4":
    bins = ETA4_BINS
    fileNamePostfix += ('_'+MODE)
elif MODE == "pt2": bins = PT2_BINS
elif MODE == "pt3": bins = PT3_BINS
elif MODE == "pt_eta": bins = PT_ETA_BINS
elif MODE == "pt_eta2": bins = PT_ETA_BINS2
ALLBINS = [ (MODE,bins)]
    
import os
OUT_PREFIX = "./outputFiles_"+str(sys.argv[1][22:-3])+"_"+PDF4FIT+'/'
os.system('mkdir '+OUT_PREFIX)

#trigger
module = process.TnP_MuonID.clone(OutputFileName = cms.string(OUT_PREFIX+"TnP_2011_MuonID_item_%s_Glb_Tight2012_IsolPFRelCombNoEGammaR03_%s_%s%s.root" % (scenario,TRIGGER_PATH,MODE,fileNamePostfix))
)
for  X,B in ALLBINS:
    shape = PDF4FIT
    BINNEDVARS = B.clone()
    #relative to
    setattr(BINNEDVARS, "Glb", cms.vstring("pass"))
    #setattr(BINNEDVARS, "PF", cms.vstring("pass"))
    setattr(BINNEDVARS, "Tight2012", cms.vstring("pass"))
    setattr(BINNEDVARS, "IsolPFRelCombNoEGammaR03", cms.vstring("pass"))
    setattr(module.Efficiencies, "Glb_Tight2012_IsolPFRelCombNoEGammaR03_"+TRIGGER_PATH+"_"+X, cms.PSet(
        EfficiencyCategoryAndState = cms.vstring(TRIGGER_PATH,"pass"),
        UnbinnedVariables = cms.vstring("mass","weight"),
        BinnedVariables = BINNEDVARS,
        BinToPDFmap = cms.vstring(shape+"Expo",
                              "*pt_bin4*",shape+"Quadratic",
                              "*pt_bin5*",shape+"Quadratic",
                              "*pt_bin6*",shape+"Quadratic",
                              "*pt_bin7*",shape+"Expo"),
    ))
    setattr(process, "TnP_Trigger_Glb_Tight2012_IsolPFRelCombNoEGammaR03_"+TRIGGER_PATH+"_"+X, module)
    setattr(process, "run_Glb_Tight2012_IsolPFRelCombNoEGammaR03_"+TRIGGER_PATH+"_"+X, cms.Path(module))
