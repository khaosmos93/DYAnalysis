from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'DYJetsToEEMuMu_M120to200'
config.General.workArea = 'PHYS14'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'mc_cfg.py'

config.Data.inputDataset = '/DYJetsToEEMuMu_M-120To200_13TeV-madgraph/Phys14DR-PU20bx25_PHYS14_25_V1-v2/AODSIM' 

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False

config.Site.storageSite = 'T3_KR_UOS'
