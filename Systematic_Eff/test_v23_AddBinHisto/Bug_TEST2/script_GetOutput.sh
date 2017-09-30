#!bin/bash
cwd2=$(pwd)

cd /home/msoh/Zprime/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170927_125041_ProdHist_Systematic_Eff_TrkIso_v1
cd DYLL_Pt50to100; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYLL_Pt100to250; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYLL_Pt250to400; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYLL_Pt400to650; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYLL_Pt650toInf; source hadd_all.sh
cp *.root ${cwd2}
cd ../



cd ${cwd2}
echo "finished"
