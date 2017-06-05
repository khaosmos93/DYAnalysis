import os

BasePath = os.getcwd()
ScriptName = "LoadLib_SysUnc_EffCorr.sh"
CodeName = "SysUnc_Zpeak_TagProbe_SystSource.cxx"

def Make_Script( _Type ):
	f = open(ScriptName, "w")
	f.write( "#!bin/bash\n" )
	f.write("""
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

.x SysUnc_EffCorr_SystSource.cxx++("{_SystType}")

.q

EOF

echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- #

""".format(_SystType=_Type) )

	f.close()

def Make_ScriptForBkgJob( _List_SystType ):
	f = open("script_BkgJob.sh", "w")
	f.write( "#!bin/bash\n" )

	for SystType in _List_SystType:
		f.write("cd %s\n" % BasePath)
		f.write("cd %s\n" % SystType)
		f.write("qsub -V %s\n" % ScriptName )
		f.write("\n")

	f.write("cd %s\n" % BasePath)
	f.write('echo "job submission is finished"\n')
	f.close()

# -- main -- #
List_SystType = ["sgnChange", "bkgChange", "M60to130", "M70to120", "nBin30", "nBin50", "TagPt20", "TagPt24"]


List_FilesInCWD = os.listdir(".")
for SystType in List_SystType:
	os.chdir( BasePath )

	if SystType not in List_FilesInCWD:
		os.mkdir( SystType )

	cmd_cp = "cp %s ./%s" % (CodeName, SystType)
	print cmd_cp
	os.system( cmd_cp )

	os.chdir( SystType )
	Make_Script( SystType )

os.chdir( BasePath )
Make_ScriptForBkgJob( List_SystType )