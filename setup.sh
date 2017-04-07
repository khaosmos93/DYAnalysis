#!/bin/bash

if [ $KP_ANALYZER_PATH ]; then
    echo "KP_ANALYZER_PATH is already defined: use a clean shell!"
    return 1
fi

# -- analyzer path (do not insert "/" in the end of the path)-- #
export KP_ANALYZER_PATH=$(pwd)
export KP_INCLUDE_PATH=$KP_ANALYZER_PATH/Include
export KP_ROOTFILE_PATH=$KP_INCLUDE_PATH/Results_ROOTFiles_76X

# -- root setup -- #
export ROOT_INCLUDE_PATH=${KP_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}

# -- ntuple path -- #
export KP_DATA_PATH=""
if [ $HOSTNAME == "tamsa2.snu.ac.kr" ]; # -- 147.47.242.67 -- # 
then 
	KP_DATA_PATH="/data4/Users/kplee/DYntuple"
	
	# -- cmssw setup (for ROOT6) -- #
	export SCRAM_ARCH=slc6_amd64_gcc530
	export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
	source $VO_CMS_SW_DIR/cmsset_default.sh

	cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13/src
	eval `scramv1 runtime -sh` # -- equivalent to cmsenv -- #
	#cmsenv # -- this doesn't work. why?? -- #
	cd $KP_ANALYZER_PATH
fi

# -- RooUnfold package path -- #
export KP_ROOUNFOLD_PATH=""
if [ $HOME == "/Users/KyeongPil_Lee" ]; # -- macbook -- #
then
	KP_ROOUNFOLD_PATH=/Users/KyeongPil_Lee/ROOT5/Unfolding/RooUnfold
	export ROOT_INCLUDE_PATH=${KP_ROOUNFOLD_PATH}:${ROOT_INCLUDE_PATH}
fi

# -- TnP CMSSW path -- #
export KP_TNP_PATH=""
if [ $HOSTNAME == "tamsa2.snu.ac.kr" || $HOSTNAME =~ "compute-" ]; then # -- 147.47.242.67 -- #
	KP_TNP_PATH="/share_home/kplee/TagProbe/v20160131_1st_76X/TagAndProbe/DYAnalysis_76X/"
fi

echo "================ environment ================"
echo "KP_ANALYZER_PATH:" ${KP_ANALYZER_PATH}
echo "KP_INCLUDE_PATH:" ${KP_INCLUDE_PATH}
echo "KP_ROOTFILE_PATH:" ${KP_ROOTFILE_PATH}
echo "KP_TNP_PATH:" ${KP_TNP_PATH}

echo "KP_DATA_PATH:" ${KP_DATA_PATH}
if [ -z $KP_DATA_PATH ]; then
    echo "     [WARNING]: ntuples are not available in this machine"
fi

echo "KP_ROOUNFOLD_PATH:" ${KP_ROOUNFOLD_PATH}
if [ -z $KP_ROOUNFOLD_PATH ]; then
	echo "     [WARNING]: RooUnfold package is not available in this machine"
fi
echo "============================================="
echo "setup is finished. Welcome :)"
