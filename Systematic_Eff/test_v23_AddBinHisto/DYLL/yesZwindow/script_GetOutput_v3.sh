#!bin/bash
cwd2=$(pwd)

cd /home/msoh/Zprime/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170927_191631_ProdHist_Systematic_Eff_BinHisto_v1
cd DYLL_Pt100to250; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYLL_Pt250to400; source hadd_all.sh
cp *.root ${cwd2}
cd ../



cd ${cwd2}
echo "finished"
