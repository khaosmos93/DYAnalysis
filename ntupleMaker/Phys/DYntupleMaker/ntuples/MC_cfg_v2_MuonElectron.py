import FWCore.ParameterSet.Config as cms

isMC = True

process = cms.Process("DYSkim")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( 
  wantSummary = cms.untracked.bool(True) 
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
                                    # "file:/data1/kplee/ROOTFiles_Test/SingleMu/Run2015B/1A12AAE9-C425-E511-87A6-02163E0118E8.root"
                                    # "file:/data1/kplee/ROOTFiles_Test/Express/Run2015B/FEVT/B89E47FC-DF25-E511-A825-02163E0118F0.root"
                                    # "file:/data1/kplee/ROOTFiles_Test/Spring15DR_DYMuMu_50ns/0033A97B-8707-E511-9D3B-008CFA1980B8.root"
                                    # "file:/data3/User/kplee/ROOTFiles_Test/Express/Run2015B/FEVT/00D1B530-2D25-E511-AD9D-02163E0133BB.root"
                                    # "file:/home/kplee/ntupleMaker/CMSSW_7_4_7/src/Phys/DYntupleMaker/ntuples/Data/SingleMuon_AOD_Run251252.root"
                                    "file:/home/kplee/ntupleMaker/CMSSW_7_4_7/src/Phys/DYntupleMaker/ntuples/50ns/MC/DYLL_M50/ExampleAOD_Spring15_DYLL_M50_50ns.root"
    #"root://cms-xrd-global.cern.ch//store/mc/Phys14DR/ZprimeToMuMu_M-5000_Tune4C_13TeV-pythia8/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/64A3B071-C66F-E411-9876-002481E0D9BC.root"
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
# process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MCRUN2_74_V9A::All')
# process.GlobalTag.globaltag = cms.string('MCRUN2_74_V9A::All')

# from Configuration.AlCa.autoCond import autoCond
# if isMC:
#    process.GlobalTag.globaltag = autoCond['startup']
# else:
#    # process.GlobalTag.globaltag = autoCond['com10']
#    # process.GlobalTag.globaltag = cms.string('GR_P_V56::All') #prompt-reco global tag
#    process.GlobalTag.globaltag = cms.string('GR_E_V49::All') #express-reco global tag
#    # process.GlobalTag.globaltag = cms.string('74X_dataRun2_Express_v0')

process.load("Configuration.StandardSequences.MagneticField_cff")

## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('patTuple_skim.root'),
    splitLevel = cms.untracked.int32(0),
    # save only events passing the full path
    #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring('keep *_*_*_*')
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*","HLT_DoubleMu*","HLT_IsoMu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim.root')
)

##FastFilters
process.goodOfflinePrimaryVertices = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"), # tracksSize() > 3 for the older cut
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)

process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)

process.FastFilters = cms.Sequence( process.goodOfflinePrimaryVertices + process.noscraping )

from Phys.DYntupleMaker.DYntupleMaker_cfi import *
from Phys.DYntupleMaker.PUreweight2012_cff import *

process.recoTree = DYntupleMaker.clone()
process.recoTree.isMC = isMC
process.recoTree.Muon = "patMuons"
process.recoTree.Electron = "patElectrons"
process.recoTree.PileUpRD = PileUpRun2012
process.recoTree.PileUpRDMuonPhys = PileUpRun2012_MuonPhys
process.recoTree.PileUpMC = Summer12S10
process.recoTree.ApplyFilter = False
process.recoTree.StoreGENFlag = True

# load the PAT config
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
# from PhysicsTools.PatAlgos.tools.coreTools import *
# if isMC==False:
#    removeMCMatching(process, ['All'])

from PhysicsTools.PatAlgos.patEventContent_cff import * 
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patEventContentNoCleaning
process.out.outputCommands.extend(cms.untracked.vstring(
  'keep *_*_*_*',
))

# Let it run
process.p = cms.Path(
  process.FastFilters *
  process.patCandidates *
    # process.patDefaultSequence
    process.recoTree
)
process.p.remove(process.makePatPhotons)
process.p.remove(process.makePatJets)
process.p.remove(process.makePatTaus)
process.p.remove(process.makePatMETs)
process.p.remove(process.patCandidateSummary)

# process.p.remove(process.electronMatch)
# process.p.remove(process.muonMatch)
# process.p.remove(process.photonMatch)
# process.p.remove(process.tauMatch)
# process.p.remove(process.patJetPartonMatch)
# process.p.remove(process.patJetGenJetMatch)
# process.p.remove(process.patJetFlavourId)
# process.p.remove(process.tauMatch)
# process.p.remove(process.tauGenJets)
# process.p.remove(process.tauGenJetMatch)



