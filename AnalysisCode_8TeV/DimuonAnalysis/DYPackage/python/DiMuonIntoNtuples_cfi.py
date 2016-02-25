import FWCore.ParameterSet.Config as cms

DiMuonIntoNtuples = cms.EDAnalyzer("DiMuonIntoNtuples",
        processName = cms.untracked.string("HLT"),
        isMC = cms.untracked.bool(False),
        doPF = cms.untracked.bool(True),
        Muon = cms.untracked.InputTag("selectedPatMuons"),
        Photon = cms.untracked.InputTag("selectedPatPhotons"),
        Electron = cms.untracked.InputTag("selectedPatElectrons"),
        Jet = cms.untracked.InputTag("selectedPatJets"),
        metLabel = cms.untracked.InputTag("patMETs"), 
        StoreGENFlag = cms.untracked.bool(False),
        StoreMCtruthFlag = cms.bool(False),
        StorePriVtxFla = cms.bool(False),
        theStorePriVtxFlag = cms.untracked.bool(False),
        theStoreJetMETFlag= cms.untracked.bool(False),
        isoValInputTags = cms.VInputTag(cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
                                            cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
                                            cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')),

)
