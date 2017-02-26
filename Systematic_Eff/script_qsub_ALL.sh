#!bin/bash
cwd2=$(pwd)

cd /home/kplee/Physics/ZprimeAnalysis_80X/Outputs/v20170227_024739_ProdHist_Systematic_Eff
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


cd ${cwd2}
echo "full submission is finished"
