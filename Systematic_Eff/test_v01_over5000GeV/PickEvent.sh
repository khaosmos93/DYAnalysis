#!bin/bash

# /SingleMuon/Run2016B-23Sep2016-v3/MINIAOD: 272007 - 275376
# /SingleMuon/Run2016C-23Sep2016-v1/MINIAOD: 275657 - 276283
# /SingleMuon/Run2016D-23Sep2016-v1/MINIAOD: 276315 - 276811
# /SingleMuon/Run2016E-23Sep2016-v1/MINIAOD: 276831 - 277420
# /SingleMuon/Run2016F-23Sep2016-v1/MINIAOD: 277772 - 278808

# /SingleMuon/Run2016B-PromptReco-v2/MINIAOD
# /SingleMuon/Run2016C-PromptReco-v2/MINIAOD
# /SingleMuon/Run2016D-PromptReco-v2/MINIAOD
# /SingleMuon/Run2016E-PromptReco-v2/MINIAOD
# /SingleMuon/Run2016F-PromptReco-v1/MINIAOD

cwd=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc600
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc600/cms/cmssw/CMSSW_8_1_0_pre11
cmsenv
echo "cmsenv(810_pre11) is done"
cd ${cwd}

edmPickEvents.py "/SingleMuon/Run2016C-23Sep2016-v1/MINIAOD" event_RunC.txt
edmPickEvents.py "/SingleMuon/Run2016C-PromptReco-v2/MINIAOD" event_RunC.txt

# edmPickEvents.py "/SingleMuon/Run2016D-23Sep2016-v1/MINIAOD" event_RunD.txt
# edmPickEvents.py "/SingleMuon/Run2016D-PromptReco-v2/MINIAOD" event_RunD.txt

edmPickEvents.py "/SingleMuon/Run2016E-23Sep2016-v1/MINIAOD" event_RunE.txt
edmPickEvents.py "/SingleMuon/Run2016E-PromptReco-v1/MINIAOD" event_RunE.txt

edmPickEvents.py "/SingleMuon/Run2016F-23Sep2016-v1/MINIAOD" event_RunF.txt
edmPickEvents.py "/SingleMuon/Run2016F-PromptReco-v1/MINIAOD" event_RunF.txt

edmPickEvents.py "/SingleMuon/Run2016G-23Sep2016-v1/MINIAOD" event_RunG.txt
edmPickEvents.py "/SingleMuon/Run2016G-PromptReco-v1/MINIAOD" event_RunG.txt

edmPickEvents.py "/SingleMuon/Run2016H-PromptReco-v2/MINIAOD" event_RunH.txt
edmPickEvents.py "/SingleMuon/Run2016H-PromptReco-v3/MINIAOD" event_RunH.txt

