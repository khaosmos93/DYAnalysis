#!bin/bash
cwd2=$(pwd)

cd /share_home/msoh/ZPrime/KPNtuple/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170828_230927_ProdHist_Systematic_Eff_PhiDip
cd Data; source qsub_all.sh
cd ../


cd ${cwd2}
echo "full submission is finished"
