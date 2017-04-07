#!/bin/bash

KP_CWD=$(pwd)

# -- make config. containing all MC templates (based on env. variables) -- #
python MakeConfigForTemplate.py

# -- move to CMSSW area -- #
cd ${KP_TNP_PATH}/v20170407_1st_MCTemplate

# -- copy all related configuration in github -- #
cp -r ${KP_ANALYZER_PATH}/Investigation/TnPSystematic_MCTemplate/TnPConfigs/* ./

# -- setup CMSSSW -- #
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc491
cmsenv

# -- Run -- #
cmsRun MuonEffs_MCTemplate.py mc_weight HighPtMuon >&log_mc_weight_HighPtMuon

echo "RecoID(MC) is completed"

# cmsRun MuonEffs_MCTemplate.py data_25ns HighPtMuon >&log_data_25ns_HighPtMuon

# echo "RecoID(Data) is completed"

# cmsRun MuonEffs_MCTemplate.py mc_weight TrkIso_010_from_HighPtMuon >&log_mc_weight_TrkIso_010_from_HighPtMuon

# echo "Isolation(MC) is completed"

# cmsRun MuonEffs_MCTemplate.py data_25ns TrkIso_010_from_HighPtMuon >&log_data_25ns_TrkIso_010_from_HighPtMuon

# echo "Isolation(Data) is completed"

# cd HLTv4p2

# cmsRun MuonEffs_MCTemplate.py mc_weight IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010 >&log_mc_weight_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010

# echo "Trigger_HLTv4p2(MC) is completed"

# cmsRun MuonEffs_MCTemplate.py data_25ns IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010 >&log_data_25ns_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010

# echo "Trigger_HLTv4p2(Data) is completed"

# cd ../HLTv4p3

# cmsRun MuonEffs_MCTemplate.py mc_weight IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010 >&log_mc_weight_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010

# echo "Trigger_HLTv4p3(MC) is completed"

# cmsRun MuonEffs_MCTemplate.py data_25ns IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010 >&log_data_25ns_IsoMu20_OR_IsoTkMu20_from_HighPtMuon_and_TrkIso_010

# echo "Trigger_HLTv4p3(Data) is completed"


echo "[job is completed]"

cd $KP_CWD

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- # 