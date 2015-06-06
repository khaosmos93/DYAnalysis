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
                                      #"file:/data1/kplee/AODTest/024378CE-4F9D-E411-9D76-001E673971CA.root"
                                    ""
    #"root://cms-xrd-global.cern.ch//store/mc/Phys14DR/ZprimeToMuMu_M-5000_Tune4C_13TeV-pythia8/AODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/64A3B071-C66F-E411-9876-002481E0D9BC.root"
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
if isMC:
   process.GlobalTag.globaltag = autoCond['startup']
else:
   process.GlobalTag.globaltag = autoCond['com10']

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

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*","HLT_DoubleMu*","HLT_IsoMu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim.root')
)

process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)

from Phys.DYntupleMaker.DYntupleMaker_cfi import *
from Phys.DYntupleMaker.PUreweight2012_cff import *

process.recoTree = DYntupleMaker.clone()
process.recoTree.isMC = isMC
process.recoTree.Muon = "selectedPatMuons"
process.recoTree.Electron = "selectedPatElectrons"
process.recoTree.PileUpRD = PileUpRun2012
process.recoTree.PileUpRDMuonPhys = PileUpRun2012_MuonPhys
process.recoTree.PileUpMC = Summer12S10
process.recoTree.ApplyFilter = False
process.recoTree.StoreGENFlag = True

if isMC==False:
   removeMCMatching(process, ['All'])

from PhysicsTools.PatAlgos.patEventContent_cff import * 
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patEventContentNoCleaning
process.out.outputCommands.extend(cms.untracked.vstring(
  'keep *_*_*_*',
))

# Let it run
process.p = cms.Path(
    process.patDefaultSequence*
    process.recoTree
)
