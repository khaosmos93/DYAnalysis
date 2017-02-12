#!/bin/bash

#########################################################
# -- qsub commands: #$ + command (details: man qsub) -- #
#########################################################
# -- shell used for executing the script -- #
#$ -S /bin/sh

# -- combine standard output & error file -- #
#$ -j y

# -- send the mail when the job is aborted or ended -- #
#$ -m ae -M kplee@cern.ch

# -- stay in the directory where qsub command is executed -- #
#$ -cwd

cwd=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

# -- CMSSW enviornment -- #
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13
cmsenv

cd ${cwd}

./local_run.sh z v20161222_M200to220_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M200to220_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M200to220.txt v20161222_M200to220_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
./finish.sh v20161222_M200to220_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M200to220_DY_NNLO_abm12lhc_5_nnlo.dat

./local_run.sh z v20161222_M220to243_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M220to243_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M220to243.txt v20161222_M220to243_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
./finish.sh v20161222_M220to243_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M220to243_DY_NNLO_abm12lhc_5_nnlo.dat

./local_run.sh z v20161222_M243to273_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M243to273_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M243to273.txt v20161222_M243to273_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
./finish.sh v20161222_M243to273_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M243to273_DY_NNLO_abm12lhc_5_nnlo.dat

./local_run.sh z v20161222_M273to320_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M273to320_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M273to320.txt v20161222_M273to320_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
./finish.sh v20161222_M273to320_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M273to320_DY_NNLO_abm12lhc_5_nnlo.dat

./local_run.sh z v20161222_M320to380_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M320to380_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M320to380.txt v20161222_M320to380_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
./finish.sh v20161222_M320to380_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M320to380_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M380to440_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M380to440_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M380to440.txt v20161222_M380to440_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M380to440_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M380to440_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M440to510_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M440to510_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M440to510.txt v20161222_M440to510_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M440to510_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M440to510_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M510to600_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M510to600_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M510to600.txt v20161222_M510to600_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M510to600_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M510to600_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M600to700_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M600to700_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M600to700.txt v20161222_M600to700_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M600to700_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M600to700_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M700to830_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M700to830_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M700to830.txt v20161222_M700to830_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M700to830_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M700to830_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M830to1000_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M830to1000_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M830to1000.txt v20161222_M830to1000_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M830to1000_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M830to1000_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M1000to1200_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M1000to1200_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M1000to1200.txt v20161222_M1000to1200_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M1000to1200_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M1000to1200_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M1200to1500_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M1200to1500_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M1200to1500.txt v20161222_M1200to1500_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M1200to1500_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M1200to1500_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M1500to2000_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M1500to2000_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M1500to2000.txt v20161222_M1500to2000_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M1500to2000_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M1500to2000_DY_NNLO_abm12lhc_5_nnlo.dat

# ./local_run.sh z v20161222_M2000to3000_DY_NNLO_abm12lhc_5_nnlo input_v20161222_M2000to3000_DY_NNLO_abm12lhc_5_nnlo.txt v20161222_histograms_M2000to3000.txt v20161222_M2000to3000_DY_NNLO_abm12lhc_5_nnlo.dat ../ 22
# ./finish.sh v20161222_M2000to3000_DY_NNLO_abm12lhc_5_nnlo NNLO.v20161222_M2000to3000_DY_NNLO_abm12lhc_5_nnlo.dat



echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- # 
