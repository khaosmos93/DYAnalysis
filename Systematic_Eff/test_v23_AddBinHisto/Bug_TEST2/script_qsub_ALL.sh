#!bin/bash
cwd2=$(pwd)

cd /home/msoh/Zprime/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170927_125041_ProdHist_Systematic_Eff_TrkIso_v1
cd DYLL_Pt50to100; source qsub_all.sh
cd ../
cd DYLL_Pt100to250; source qsub_all.sh
cd ../
cd DYLL_Pt250to400; source qsub_all.sh
cd ../
cd DYLL_Pt400to650; source qsub_all.sh
cd ../
cd DYLL_Pt650toInf; source qsub_all.sh
cd ../


cd ${cwd2}
echo "full submission is finished"
