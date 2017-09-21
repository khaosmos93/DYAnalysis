#!bin/bash
cwd2=$(pwd)

cd /share_home/msoh/ZPrime/KPNtuple/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170919_225819_ProdHist_Systematic_Eff_noB2B_v6
cd DYTauTau; source hadd_all.sh
cp *.root ${cwd2}
cd ../



cd ${cwd2}
echo "finished"
