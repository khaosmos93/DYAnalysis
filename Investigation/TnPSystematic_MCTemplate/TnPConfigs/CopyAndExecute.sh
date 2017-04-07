#!/bin/bash

cd /share_home/kplee/TagProbe/v20160131_1st_76X/TagAndProbe/DYAnalysis_76X/v20170407_1st_MCTemplate

cp -r $KP_ANALYZER_PATH/Investigation/TnPSystematic_MCTemplate/TnPConfigs ./

source Run_TnP_MCTemplate_All.sh

echo "finished"