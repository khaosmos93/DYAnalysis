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
if [ $HOSTNAME == "cms.snu.ac.kr" ]; # -- 147.47.242.42 -- # 
then 
	KP_DATA_PATH="/data1/kplee/DYntuple/80X"
else
	echo "WARNING: ntuples are not available in this machine"
fi

echo "================ environment ================"
echo "KP_ANALYZER_PATH:" ${KP_ANALYZER_PATH}
echo "KP_DATA_PATH:" ${KP_DATA_PATH}
echo "============================================="
echo "setup is finished. Welcome :)"