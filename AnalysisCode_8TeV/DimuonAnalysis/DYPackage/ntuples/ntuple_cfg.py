import FWCore.ParameterSet.Config as cms

process = cms.Process("DYSkim")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/mc/Summer12_DR53X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/AODSIM/PU_S10_START53_V7A-v1/0000/00037C53-AAD1-E111-B1BE-003048D45F38.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_RD1_START53_V7N-v1/10000/00BF53FB-BBCE-E211-9D43-485B39800C1C.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4CA35596-4E15-E211-A47F-E0CB4E29C4F5.root'
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/6AA7A192-AD14-E211-B5E3-001EC9D7F667.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4CDF7732-C814-E211-B303-00259073E316.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/6A7D697F-5915-E211-8E95-20CF3027A630.root',
       #'/store/mc/Summer12_DR53X/TTJets_FullLeptMGDecays_8TeV-madgraph/AODSIM/PU_S10_START53_V7A-v1/00000/06ED9A35-2A10-E211-94E4-485B398971FE.root',
       #'/store/mc/Summer12_DR53X/GluGlu_HToMM_M-125_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7C-v1/10000/0499577F-A692-E211-8582-001E67397D5A.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(30) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START53_V10::All')
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
    outputCommands = cms.untracked.vstring('drop *')
)

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")
#process.patJets.addTagInfos     = cms.bool(False)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim.root')
)
#FIXME
#process.Tracer = cms.Service("Tracer") 

from DimuonAnalysis.DYPackage.DiMuonIntoNtuples_cfi import *
from DimuonAnalysis.DYPackage.PUreweight2012_cff import *

process.recoTree = DiMuonIntoNtuples.clone()
process.recoTree.isMC = True
process.recoTree.StoreGENFlag = False
process.recoTree.Muon = "selectedPatMuons"
process.recoTree.PileUpRD = PileUpRun2012
process.recoTree.PileUpMC = Summer12S10

# Let it run
process.p = cms.Path(
    #process.dimuonsHLTFilter*
    process.patDefaultSequence*
    process.recoTree
)

#process.outpath = cms.EndPath(process.out)

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import * 
#process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patEventContentNoCleaning
process.out.outputCommands.extend(cms.untracked.vstring(
    #'keep *_particleFlow_*_*',
    'keep recoMuons_muons_*_*',
))
