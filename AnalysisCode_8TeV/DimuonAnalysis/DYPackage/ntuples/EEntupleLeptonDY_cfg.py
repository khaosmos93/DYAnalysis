import FWCore.ParameterSet.Config as cms

process = cms.Process("DYSkim")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       #'/store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00003/4CA35596-4E15-E211-A47F-E0CB4E29C4F5.root',
       '/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00000/025B348F-0118-E211-A25D-00259074AEE6.root', 
       #'/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00002/A08BD13D-3F1A-E211-8A34-485B39800B98.root',
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

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
process.recoTree.runOnEleInput = True
process.recoTree.Muon = "selectedPatMuons"
process.recoTree.Photon = "selectedPatPhotons"
process.recoTree.Electron = "calibratedElectrons" #FIXME selectedPatElectrons"
process.recoTree.Jet = "selectedPatJets"
process.recoTree.metLabel = "patMETs"
process.recoTree.PileUpRD = PileUpRun2012
process.recoTree.PileUpMC = Summer12S10


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    calibratedElectrons = cms.PSet(
        initialSeed = cms.untracked.uint32(1),
        engineName = cms.untracked.string('TRandom3')
    ),
)

process.load("EgammaAnalysis.ElectronTools.calibratedElectrons_cfi")

# dataset to correct
process.calibratedElectrons.isMC = cms.bool(True)
process.calibratedElectrons.inputDataset = cms.string("Summer12_LegacyPaper")
process.calibratedElectrons.updateEnergyError = cms.bool(True)
process.calibratedElectrons.correctionsType = cms.int32(2) 
process.calibratedElectrons.combinationType = cms.int32(3)
process.calibratedElectrons.verbose = cms.bool(False)
process.calibratedElectrons.synchronization = cms.bool(False) #True)

process.load('EgammaAnalysis.ElectronTools.electronRegressionEnergyProducer_cfi')
process.eleRegressionEnergy.energyRegressionType = cms.uint32(2)
process.eleRegressionEnergy.inputElectronsTag = cms.InputTag('gsfElectrons')
process.eleRegressionEnergy.inputCollectionType = cms.uint32(0)
process.eleRegressionEnergy.useRecHitCollections = cms.bool(True)
process.eleRegressionEnergy.produceValueMaps = cms.bool(True)


#adding eletron iso
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons') #selectedPatElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True)
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

# Let it run
process.p = cms.Path(
    process.kt6PFJetsForIsolation*
    process.patDefaultSequence*
    process.eleRegressionEnergy * process.calibratedElectrons* 
    process.pfiso*
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
