#!bin/bash

#########################################################
# -- qsub commands: #$ + command (details: man qsub) -- #
#########################################################
# -- shell used for executing the script -- #
#$ -S /bin/sh

# -- combine standard output & error file -- #
#$ -j y

## -- send the mail when the job is aborted or ended -- #
##$ -m ae -M kplee@cern.ch

# -- stay in the directory where qsub command is executed -- #
#$ -cwd

root -l -b << EOF

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x SysUnc_EffCorr_SystSource.cxx++("sgnChange")

.q

EOF

echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- #

