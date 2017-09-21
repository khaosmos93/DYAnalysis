#!bin/bash
cwd2=$(pwd)

cd /share_home/msoh/ZPrime/KPNtuple/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170828_230616_ProdHist_Systematic_Eff_PhiDip
cd Data; source hadd_all.sh
cp *.root ${cwd2}
cd ../



cd ${cwd2}
echo "finished"
