#!bin/bash

cwd2=$(pwd)

# cp /home/kplee/Physics/script/Split_BatchJobs_80X.py ./

python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample WWTo2L2Nu --njob 1 --lumi 35867.060 --isMC 1
cd ./WWTo2L2Nu; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample WZ --njob 1 --lumi 35867.060 --isMC 1
cd ./WZ; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample ZZ --njob 1 --lumi 35867.060 --isMC 1
cd ./ZZ; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample ttbarTo2L2Nu --njob 10 --lumi 35867.060 --isMC 1
cd ./ttbarTo2L2Nu; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample tW --njob 2 --lumi 35867.060 --isMC 1
cd ./tW; source qsub_all.sh

cd ${cwd2}
python Split_BatchJobs_80X.py --code ProdHist_Systematic_Eff.cxx --sample tbarW --njob 2 --lumi 35867.060 --isMC 1
cd ./tbarW; source qsub_all.sh

cd ${cwd2}