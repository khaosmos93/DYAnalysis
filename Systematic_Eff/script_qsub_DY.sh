#!bin/bash

cwd2=$(pwd)

# cp /home/kplee/Physics/script/Split_BatchJobs_80X.py ./

python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M50to120 --njob 5 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M50to120; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M120to200 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M120to200; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M200to400 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M200to400; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M400to800 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M400to800; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M800to1400 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M800to1400; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M1400to2300 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M1400to2300; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M2300to3500 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M2300to3500; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M3500to4500 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M3500to4500; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M4500to6000 --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M4500to6000; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample DYPowheg_M6000toInf --njob 1 --lumi 35867.060 --isMC 1
cd ./DYPowheg_M6000toInf; source qsub_all.sh

cd ${cwd2}