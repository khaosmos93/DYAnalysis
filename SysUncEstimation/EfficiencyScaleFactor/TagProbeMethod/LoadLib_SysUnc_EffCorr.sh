#!/bin/bash
echo "Change ROOT version: ROOT6 -> ROOT5"
# source ${KP_ROOT5_PATH}
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc493/cms/cmssw/CMSSW_7_6_3
eval `scramv1 runtime -sh` # -- equivalent to cmsenv -- #

root -l -b << EOF

TString IncludePath = gSystem->Getenv("KP_ANALYZER_PATH")
gInterpreter->AddIncludePath( TString::Format("%s", IncludePath.Data()) )

TString IncludePath2 = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gInterpreter->AddIncludePath( TString::Format("%s", IncludePath2.Data()) )

.include

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x SysUnc_EffCorr.C++

.q

EOF

echo "Return to latest ROOT version: ROOT5 -> ROOT6"
# source ${KP_ROOT6_PATH}
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13/src
eval `scramv1 runtime -sh` # -- equivalent to cmsenv -- #