#!bin/bash

if [ $KP_ANALYZER_PATH ]; then
    echo "KP_ANALYZER_PATH is already defined: use a clean shell!"
    return 1
fi

# -- analyzer path -- #
export KP_ANALYZER_PATH=$(pwd)
export ROOT_INCLUDE_PATH=${KP_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}

# -- ntuple path -- #
export KP_DATA_PATH=""
if [ $HOSTNAME == "tamsa2.snu.ac.kr" ]; # -- 147.47.242.67 -- # 
then 
	KP_DATA_PATH="/data4/Users/kplee/DYntuple/"
	
	# -- cmssw setup (for ROOT6) -- #
	export SCRAM_ARCH=slc6_amd64_gcc530
	export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
	source $VO_CMS_SW_DIR/cmsset_default.sh

	cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13/src; cmsenv;
	cd $KP_ANALYZER_PATH
else
	echo "WARNING: ntuples are not available in this machine"
fi

echo "================ environment ================"
echo "KP_ANALYZER_PATH:" ${KP_ANALYZER_PATH}
echo "KP_DATA_PATH:" ${KP_DATA_PATH}
echo "============================================="
echo "setup is finished. Welcome :)"