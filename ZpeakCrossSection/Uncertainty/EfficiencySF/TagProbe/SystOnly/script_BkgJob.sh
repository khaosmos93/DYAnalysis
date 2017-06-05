#!bin/bash
cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd sgnChange
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd bkgChange
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd M60to130
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd M70to120
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd nBin30
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd nBin50
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd TagPt20
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
cd TagPt24
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /share_home/kplee/Physics/DYAnalysis_76X_LumiUpdate/ZpeakCrossSection/Uncertainty/EfficiencySF/TagProbe/SystOnly
echo "job submission is finished"
