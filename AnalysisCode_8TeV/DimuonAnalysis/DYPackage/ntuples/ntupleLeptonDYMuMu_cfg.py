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
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4CA35596-4E15-E211-A47F-E0CB4E29C4F5.root',
       #Benjamin
       '/store/mc/Summer12_DR53X/GluGlu_HToMM_M-125_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7C-v1/10000/0499577F-A692-E211-8582-001E67397D5A.root'
       #'/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00002/A08BD13D-3F1A-E211-8A34-485B39800B98.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/6AA7A192-AD14-E211-B5E3-001EC9D7F667.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4CDF7732-C814-E211-B303-00259073E316.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/6A7D697F-5915-E211-8E95-20CF3027A630.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4E24127F-9014-E211-9057-E0CB4E55366B.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/68C94F75-2E15-E211-BDA4-90E6BAE8CC0C.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4E72B0E8-A314-E211-8BF2-E0CB4E29C4E9.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/68B1CC37-C114-E211-A49F-00259073E4C8.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4EB83A3F-EB14-E211-83DE-00259073E4F6.root',
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/686D7C58-9414-E211-AE63-001E4F3F355E.root',
#      '/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/06ABC806-29E1-E111-A1F0-00215E2221E4.root',
    )
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(300) )

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

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim.root')
)
#FIXME
#process.Tracer = cms.Service("Tracer") 

from DimuonAnalysis.DYPackage.DiLeptonIntoNtuples_cfi import *
from DimuonAnalysis.DYPackage.PUreweight2012_cff import *

process.recoTree = DiLeptonIntoNtuples.clone()
process.recoTree.isMC = True
process.recoTree.isSignal = True
process.recoTree.runOnMuInput = True
process.recoTree.Muon = "selectedPatMuons"
process.recoTree.Photon = "selectedPatPhotons"
process.recoTree.Electron = "selectedPatElectrons"
process.recoTree.Jet = "selectedPatJets"
process.recoTree.metLabel = "patMETs"
process.recoTree.PileUpRD = PileUpRun2012
process.recoTree.PileUpMC = Summer12S10

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True)
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

# Let it run
process.p = cms.Path(
    #process.filter*
    process.kt6PFJetsForIsolation*
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
    'keep *_*_*_*',
))
