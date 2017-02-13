#!bin/bash

cwd2=$(pwd)

cd ./DYPowheg_M50to120; source hadd_all.sh; cp ROOTFile_DYPowheg_M50to120.root ../

cd ${cwd2}
cd ./DYPowheg_M120to200; source hadd_all.sh; cp ROOTFile_DYPowheg_M120to200.root ../

cd ${cwd2}
cd ./DYPowheg_M200to400; source hadd_all.sh; cp ROOTFile_DYPowheg_M200to400.root ../

cd ${cwd2}
cd ./DYPowheg_M400to800; source hadd_all.sh; cp ROOTFile_DYPowheg_M400to800.root ../

cd ${cwd2}
cd ./DYPowheg_M800to1400; source hadd_all.sh; cp ROOTFile_DYPowheg_M800to1400.root ../

cd ${cwd2}
cd ./DYPowheg_M1400to2300; source hadd_all.sh; cp ROOTFile_DYPowheg_M1400to2300.root ../

cd ${cwd2}
cd ./DYPowheg_M2300to3500; source hadd_all.sh; cp ROOTFile_DYPowheg_M2300to3500.root ../

cd ${cwd2}
cd ./DYPowheg_M3500to4500; source hadd_all.sh; cp ROOTFile_DYPowheg_M3500to4500.root ../

cd ${cwd2}
cd ./DYPowheg_M4500to6000; source hadd_all.sh; cp ROOTFile_DYPowheg_M4500to6000.root ../

cd ${cwd2}
cd ./DYPowheg_M6000toInf; source hadd_all.sh; cp ROOTFile_DYPowheg_M6000toInf.root ../

cd ${cwd2}

hadd ROOTFile_DYPowheg.root *.root

echo "finished"