#!bin/bash
cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd sgnChange
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd bkgChange
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd M60to130
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd M70to120
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd nBin30
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd nBin50
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd TagPt20
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
cd TagPt24
qsub -V LoadLib_SysUnc_EffCorr.sh

cd /Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/SysUncEstimation/EfficiencyScaleFactor/TagProbeMethod/SystOnly
echo "job submission is finished"
