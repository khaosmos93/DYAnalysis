#!bin/bash
cwd2=$(pwd)

cd /share_home/msoh/ZPrime/KPNtuple/CMSSW_8_0_26/src/DYAnalysis/Outputs/v20170930_025700_ProdHist_Systematic_Eff_DeltaR_v1
cd Data; source qsub_all.sh
cd ../
cd DYPowheg_M50to120; source qsub_all.sh
cd ../
cd DYPowheg_M120to200; source qsub_all.sh
cd ../
cd DYPowheg_M200to400; source qsub_all.sh
cd ../
cd DYPowheg_M400to800; source qsub_all.sh
cd ../
cd DYPowheg_M800to1400; source qsub_all.sh
cd ../
cd DYPowheg_M1400to2300; source qsub_all.sh
cd ../
cd DYPowheg_M2300to3500; source qsub_all.sh
cd ../
cd DYPowheg_M3500to4500; source qsub_all.sh
cd ../
cd DYPowheg_M4500to6000; source qsub_all.sh
cd ../
cd DYPowheg_M6000toInf; source qsub_all.sh
cd ../
cd WJets_HT100to200; source qsub_all.sh
cd ../
cd WJets_HT200to400; source qsub_all.sh
cd ../
cd WJets_HT400to600; source qsub_all.sh
cd ../
cd WJets_HT600to800; source qsub_all.sh
cd ../
cd WJets_HT800to1200; source qsub_all.sh
cd ../
cd WJets_HT1200to2500; source qsub_all.sh
cd ../
cd WJets_HT2500toInf; source qsub_all.sh
cd ../
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
cd WWTo2L2Nu; source qsub_all.sh
cd ../
cd WZ; source qsub_all.sh
cd ../
cd ZZ; source qsub_all.sh
cd ../
cd ttbarTo2L2Nu; source qsub_all.sh
cd ../
cd tW; source qsub_all.sh
cd ../
cd tbarW; source qsub_all.sh
cd ../
cd DYTauTau; source qsub_all.sh
cd ../


cd ${cwd2}
echo "full submission is finished"
