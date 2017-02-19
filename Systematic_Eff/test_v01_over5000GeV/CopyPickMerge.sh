#!bin/bash

#################################################################
export SCRAM_ARCH=slc6_amd64_gcc530
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_25
cmsenv
echo "cmsenv(8025) is done"
cd ${cwd}
#################################################################

# -- Run D -- #
# edmCopyPickMerge outputFile=pickevents_RunD_Rereco.root \
#   eventsToProcess=276495:193021195,276543:1587173701,276582:1393373573 \
#   inputFiles="/store/data/Run2016D/SingleMuon/MINIAOD/23Sep2016-v1/50000/C41E273C-B68E-E611-A6B2-0090FAA57C00.root","/store/data/Run2016D/SingleMuon/MINIAOD/23Sep2016-v1/60000/D8ECAD90-769D-E611-99A8-848F69FD4553.root","/store/data/Run2016D/SingleMuon/MINIAOD/23Sep2016-v1/70000/66E8F6B1-0B97-E611-A1DE-001E677924B2.root"


# edmCopyPickMerge outputFile=pickevents_RunD_Prompt.root \
#   eventsToProcess=276495:193021195,276543:1587173701,276582:1393373573 \
#   inputFiles="/store/data/Run2016D/SingleMuon/MINIAOD/PromptReco-v2/000/276/495/00000/EEBF9DE1-5B48-E611-9582-02163E01228A.root","/store/data/Run2016D/SingleMuon/MINIAOD/PromptReco-v2/000/276/543/00000/DC2C35A7-9C4A-E611-AB7A-FA163EBC6EBF.root","/store/data/Run2016D/SingleMuon/MINIAOD/PromptReco-v2/000/276/582/00000/6CB568FF-704A-E611-95F1-02163E013987.root"


edmCopyPickMerge outputFile=pickevents_RunC_Rereco.root \
  eventsToProcess=276242:551767827 \
  inputFiles="/store/data/Run2016C/SingleMuon/MINIAOD/23Sep2016-v1/70000/8AB5D43B-B88C-E611-8CF1-0CC47A57D066.root"


edmCopyPickMerge outputFile=pickevents_RunC_Prompt.root \
  eventsToProcess=276242:551767827 \
  inputFiles="/store/data/Run2016C/SingleMuon/MINIAOD/PromptReco-v2/000/276/242/00000/C28D2DF1-8442-E611-A5E2-02163E01356A.root"


edmCopyPickMerge outputFile=pickevents_RunE_Rereco.root \
  eventsToProcess=276870:1367635399,276948:517467886 \
  inputFiles="/store/data/Run2016E/SingleMuon/MINIAOD/23Sep2016-v1/70000/6E6F87B5-298D-E611-A67D-0025905B85FC.root","/store/data/Run2016E/SingleMuon/MINIAOD/23Sep2016-v1/80000/9ED0821A-8E8C-E611-B415-0CC47A7C34EE.root"

# -- no Prompt RunE event (was it not included in prompt JSON? ) -- #

edmCopyPickMerge outputFile=pickevents_RunF_Rereco.root \
  eventsToProcess=278017:96889195,278406:475843842 \
  inputFiles="/store/data/Run2016F/SingleMuon/MINIAOD/23Sep2016-v1/80000/7248BC2D-2990-E611-84E5-008CFA197A28.root","/store/data/Run2016F/SingleMuon/MINIAOD/23Sep2016-v1/100000/529915DA-7D8C-E611-B952-008CFAFBF0BA.root"


edmCopyPickMerge outputFile=pickevents_RunF_Prompt.root \
  eventsToProcess=278017:96889195,278406:475843842 \
  inputFiles="/store/data/Run2016F/SingleMuon/MINIAOD/PromptReco-v1/000/278/017/00000/9E09592C-275A-E611-B4EB-02163E0120FD.root","/store/data/Run2016F/SingleMuon/MINIAOD/PromptReco-v1/000/278/406/00000/2C9C367D-C35F-E611-90B0-02163E014633.root"


edmCopyPickMerge outputFile=pickevents_RunG_Rereco.root \
  eventsToProcess=279116:292079063 \
  inputFiles="/store/data/Run2016G/SingleMuon/MINIAOD/23Sep2016-v1/90000/48642510-3899-E611-B5CE-008CFA111268.root"


edmCopyPickMerge outputFile=pickevents_RunG_Prompt.root \
  eventsToProcess=279116:292079063 \
  inputFiles="/store/data/Run2016G/SingleMuon/MINIAOD/PromptReco-v1/000/279/116/00000/4C41A593-1069-E611-9543-02163E013910.root"


edmCopyPickMerge outputFile=pickevents_RunH_Prompt.root \
  eventsToProcess=281727:539783509,281727:694794554,283865:437080058 \
  inputFiles="/store/data/Run2016H/SingleMuon/MINIAOD/PromptReco-v2/000/281/727/00000/A41D5D98-1F87-E611-B15F-02163E01385F.root","/store/data/Run2016H/SingleMuon/MINIAOD/PromptReco-v2/000/281/727/00000/9A03337D-1F87-E611-8B04-FA163E6E5E9B.root","/store/data/Run2016H/SingleMuon/MINIAOD/PromptReco-v2/000/283/865/00000/805CE836-619C-E611-99BA-FA163EDF9ED3.root"