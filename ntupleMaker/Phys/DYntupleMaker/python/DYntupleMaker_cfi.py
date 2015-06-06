import FWCore.ParameterSet.Config as cms

#from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *

DYntupleMaker = cms.EDAnalyzer("DYntupleMaker",
        processName = cms.untracked.string("HLT"),
        isMC = cms.untracked.bool(True),
        ApplyFilter = cms.untracked.bool(True),
        FilterType = cms.untracked.int32(0),
        Muon = cms.untracked.InputTag("selectedPatMuons"),
        Electron = cms.untracked.InputTag("selectedPatElectrons"),
                                #ElectronCalib = cms.untracked.InputTag("calibratedPatElectrons"),
                                Jet = cms.untracked.InputTag("selectedPatJets"),
                                #MET = cms.untracked.InputTag("patMETs"), 
                                #pfMET = cms.untracked.InputTag("pfType1CorrectedMet"), 
                                
                                #barrelCuts = cms.PSet(heepBarrelCuts),
                                #endcapCuts = cms.PSet(heepEndcapCuts),
                                applyRhoCorrToEleIsol = cms.untracked.bool(False),
                                #eleIsolEffectiveAreas = cms.PSet(heepEffectiveAreas),
        IntLuminosity = cms.untracked.double(0.25),
        nHighQualLeptons = cms.untracked.int32(2),
        CrossSection = cms.untracked.double(1.0),
        FilterEfficiency = cms.untracked.double(1.0),
        TotalNevents = cms.untracked.double(1),
        StoreGENFlag = cms.untracked.bool(False),
                                StoreJetFlag = cms.untracked.bool(False),
        StoreTTFlag = cms.untracked.bool(True),
        StoreElectronFlag = cms.untracked.bool(True),
)
