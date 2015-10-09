from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'DYntuple_v20151003_Run2015D_SingleMuon_Run256870toRun257599'
config.General.workArea = 'DYntuple'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'data_cfg_v2_MuonElectron.py'

config.Data.inputDataset = '/SingleMuon/Run2015D-PromptReco-v3/AOD'

config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False

config.Data.lumiMask = '/cms/home/kplee/JSON/Cert_246908-257599_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
config.Data.runRange = '256870-257599'

config.Site.storageSite = 'T3_KR_KISTI'
