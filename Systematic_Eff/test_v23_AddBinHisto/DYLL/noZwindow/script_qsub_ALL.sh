#!bin/bash
cwd2=$(pwd)

cd /home/msoh/Zprime/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170927_181734_ProdHist_Systematic_Eff_BinHisto_v1
cd DYLL_Pt100to250; source qsub_all.sh
cd ../
cd DYLL_Pt250to400; source qsub_all.sh
cd ../


cd ${cwd2}
echo "full submission is finished"
