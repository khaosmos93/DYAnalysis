#!bin/bash
cwd2=$(pwd)

cd /home/kplee/Physics/ZprimeAnalysis_80X/Outputs/v20170301_192240_ProdHist_Systematic_Eff
cd Data; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M50to120; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M120to200; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M200to400; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M400to800; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M800to1400; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M1400to2300; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M2300to3500; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M3500to4500; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M4500to6000; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd DYPowheg_M6000toInf; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd WWTo2L2Nu; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd WZ; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd ZZ; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd ttbarTo2L2Nu; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd tW; source hadd_all.sh
cp *.root ${cwd2}
cd ../

cd tbarW; source hadd_all.sh
cp *.root ${cwd2}
cd ../



cd ${cwd2}
echo "finished"
