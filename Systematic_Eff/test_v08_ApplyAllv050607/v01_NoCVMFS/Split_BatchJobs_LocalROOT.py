import sys
import os
from getopt import gnu_getopt as getopt

def usage():
    print sys.argv[0], " : split the jobs"
    print "  Mandatory options :"
    print "   --code CODE.cxx                  C++ Code file name: should be absolute path"
    print "   --sample SAMPLENAME    		   Sample Name"
    print "   --njob N                9         Total number of jobs"
    print "   --lumi LUMI                      Integrated lumionsity in pb"
    print "   --isMC isMC                      MC or data"
    print "   --outdir Dir                     Directory where outputs are stored"
    print "  Optional options :"
    print "   --queue queueName                      queue name (default: fastq)"
    sys.exit()

# Parse arguments
if len(sys.argv) < 2: usage()
try:
    opts, args = getopt(sys.argv[1:], 'n', ["code=", "sample=", "njob=", "lumi=", "isMC=", "queue=", "outdir="])
    opts = dict(opts)
except:
    print "!!! Error parsing arguments"
    usage()


class SplitJobs:
	def __init__(self, _opts):
		self.CodeFullPath = _opts['--code']
		self.Sample = _opts['--sample']
		self.nJob = int(_opts['--njob'])
		self.Lumi = float(_opts['--lumi'])
		self.isMC = int(_opts['--isMC'])
		self.OutDir = _opts['--outdir']

		self.CodeName = self.CodeFullPath.split('/')[-1]

		self.queue = "fastq"
		if os.environ["HOSTNAME"] == "tamsa2.snu.ac.kr":
			self.queue = "bigq"
		if '--queue' in _opts:
			self.queue = _opts['--queue']

		_TYPE = ""
		if self.isMC == 1: _TYPE = "MC"
		else: _TYPE = "Data"

		print "+" * 100
		print "Create %d jobs to run %s with lumi = %lf on %s -> queue name = %s" % (self.nJob, self.CodeName, self.Lumi, _TYPE, self.queue)
		print "Output directory: %s" % (self.OutDir)
		print "+" * 100

		print self.isMC
		
		self.List_ROOTFiles = self.GetListOfROOTFiles()
		if self.isMC == 1:
			self.NormFactor = self.GetNormFactor()
		else:
			self.NormFactor = 1

		os.chdir( self.OutDir )


	def CreateWorkSpace( self ):
		DirName = "%s" % (self.Sample)
		
		List_File_cwd = os.listdir( "." )
		if DirName in List_File_cwd:
			print "%s is already exists!" % (DirName)
			sys.exit()

		os.mkdir( DirName )

		nROOTFile = len(self.List_ROOTFiles)
		if self.nJob > nROOTFile:
			print "nJob > nROOTFile -> nJob is set as same with nROOTFile"
			self.nJob = nROOTFile

		nROOTFilePerJob = int( float(nROOTFile) / float(self.nJob) )
		print "nJob = %d, nROOTFile = %d -> nROOTFilePerJob = %d\n" % (self.nJob, nROOTFile, nROOTFilePerJob)

		List_cmd_qsub = []
		List_cmd_hadd = []
		for i in range(0, self.nJob):
			List_ROOTFilesPerJob = []
			if i == self.nJob-1:
				List_ROOTFilesPerJob = self.List_ROOTFiles[int(i*nROOTFilePerJob):]
			else:
				List_ROOTFilesPerJob = self.List_ROOTFiles[int(i*nROOTFilePerJob):int((i+1)*nROOTFilePerJob)]
			
			# print "List_ROOTFilesPerJob"
			# for rootfile in List_ROOTFilesPerJob:
			# 	print "%s" % (rootfile)
			# print "\n"
			
			self.CreateWorkSpace_PerJob( i, List_ROOTFilesPerJob, List_cmd_qsub, List_cmd_hadd )

		self.CreateScript_qsub( List_cmd_qsub )
		self.CreateScript_hadd( List_cmd_hadd )

	def CreateWorkSpace_PerJob( self, _iter, _List_ROOTFilesPerJob, List_cmd_qsub, List_cmd_hadd ):
		DirName = "Job_%d" % (_iter)
		os.mkdir( "./"+self.Sample+"/"+DirName )

		FileName = "ROOTFileList-%s-%d.txt" % (self.Sample, _iter)
		f_ROOTFileList = open("./"+self.Sample+"/"+DirName+"/"+FileName, "w")
		for rootfilepath in _List_ROOTFilesPerJob:
			f_ROOTFileList.write( rootfilepath )
			f_ROOTFileList.write( "\n" )
		f_ROOTFileList.close()

		cmd_cp = "cp %s ./%s/%s" % (self.CodeFullPath, self.Sample, DirName)
		# print cmd_cp
		os.system( cmd_cp )

		# cmd_execute = "root -l -b -q '"+ self.CodeName + '++(%d, "%s", %.15lf)' % (self.isMC, FileName, self.NormFactor) + "'"
		cmd_execute = ".x "+ self.CodeName + '++(%d, "%s", %.15lf)' % (self.isMC, FileName, self.NormFactor)
		print cmd_execute

		BatchFileName = self.CreateBatchJobScript( _iter, DirName, cmd_execute )

		cmd_cd = "cd ${cwd}/%s" % (DirName)
		cmd_sub = "qsub -V -q %s %s" % (self.queue, BatchFileName)
		List_cmd_qsub.append( [cmd_cd, cmd_sub] )

		cmd_hadd = "${cwd}/%s/*.root \\" % (DirName)
		List_cmd_hadd.append( cmd_hadd )

	def CreateScript_qsub( self, _List_cmd_qsub ):
		f = open("./"+self.Sample+"/qsub_all.sh", "w")
		f.write( "#!bin/bash\n" )
		f.write( "cwd=$(pwd)\n\n" )
		for cmd_qsub in _List_cmd_qsub:
			cmd_cd = cmd_qsub[0]
			cmd_sub = cmd_qsub[1]

			f.write( cmd_cd )
			f.write( "\n" )
			f.write( cmd_sub )
			f.write( "\n\n" )

		f.write( 'cd ${cwd}\n' )
		f.write( 'echo "finished"\n' )
		f.close()

		print "cd %s; source qsub_all.sh" % (self.Sample)

	def CreateScript_hadd( self, _List_cmd_hadd ):
		f = open("./"+self.Sample+"/hadd_all.sh", "w")
		f.write( "#!bin/bash\n" )
		f.write( "cwd=$(pwd)\n\n" )
		f.write( "hadd ROOTFile_%s.root \\\n" % (self.Sample) )
		for cmd_hadd in _List_cmd_hadd:
			f.write( cmd_hadd )
			f.write( "\n" )
		
		f.write( "\n" )
		f.write( 'echo "finished"\n' )
		f.close()

	def CreateBatchJobScript( self, iter, _DirName, cmd_execute ):
		BatchFileName = "%s_%d.sh" % (self.Sample, iter)
		f = open("./"+self.Sample+"/"+_DirName+"/"+BatchFileName, "w")
		f.write(
"""#!/bin/bash

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

cwd=$(pwd)

#export SCRAM_ARCH=slc6_amd64_gcc530
#export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
#source $VO_CMS_SW_DIR/cmsset_default.sh

# -- CMSSW enviornment -- #
#cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_13
#cmsenv

# -- local ROOT environment without CVMFS -- #
source /share/apps/root_v5-34-32/root/bin/thisroot.sh

cd ${{cwd}}

root -l -b << EOF

.include /share_home/kplee/Physics/ZprimeAnalysis_80X/

.include

{_cmd_execute}

.q

EOF

echo "job is completed"

# -- &>log: "Invalid null command" Error occurs. please use >&log. -- #

# -- PLEASE ENTER AFTER THE LAST LINE! ... IF YOU DON'T, LAST LINE WILL NOT BE EXECUTED BY BATCH JOB -- # 
""".format(_cmd_execute=cmd_execute)
		)
		print "[%s is created]" % BatchFileName
		f.close()

		return BatchFileName

	def GetListOfROOTFiles( self ):
		List_FullPath = []
		BasePath = os.environ['KP_DATA_PATH'] + "/"

		if "DYPowheg" in self.Sample:
			MassRange = self.Sample.split("_")[-1]
			List_FullPath.append( BasePath + "DYntuple_v20170207_80XMoriond17_AddZprimeVar_ZMuMuPowheg_"+MassRange )

		# elif "DYMuMu" in self.Sample:
		# 	MassRange = self.Sample.split("_")[-1]
		# 	if MassRange == "M50to100": MassRange = "M50toInf"
		# 	List_FullPath.append( BasePath + "v20160304_76X_MINIAODv2_DYLL_"+MassRange+"_25ns" )

		elif "ZZ" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ZZ" )

		elif "ZZTo4L" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ZZto4L" )

		elif "WW" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WW" )

		elif "WWTo2L2Nu" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WWTo2L2Nu" )

		elif "WZ" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WZ" )

		elif "WZTo3LNu" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WZTo3LNu" )

		elif "ttbar" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ttbar" )
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ttbarBackup" )

		elif "ttbarTo2L2Nu" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ttbarTo2L2Nu" )

		elif "tW" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ST_tW" )

		elif "tbarW" == self.Sample:
			List_FullPath.append( BasePath + "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ST_tbarW" )

		elif self.Sample == "Data":
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016B_v3_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016C_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016D_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016E_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016F_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016G_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80X_AddZprimeVar_SingleMuon_Run2016H_v2_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170118_80X_AddZprimeVar_SingleMuon_Run2016H_v3_GoldenJSON_271036_to_284044" )

		elif self.Sample == "DataRunBtoF":
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016B_v3_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016C_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016D_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016E_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016F_v1_GoldenJSON_271036_to_284044" )

		elif self.Sample == "DataRunGtoH":
			List_FullPath.append( BasePath + "DYntuple_v20170117_80XRereco_AddZprimeVar_SingleMuon_Run2016G_v1_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170117_80X_AddZprimeVar_SingleMuon_Run2016H_v2_GoldenJSON_271036_to_284044" )
			List_FullPath.append( BasePath + "DYntuple_v20170118_80X_AddZprimeVar_SingleMuon_Run2016H_v3_GoldenJSON_271036_to_284044" )

		List_ROOTFiles = []
		for fullpath in List_FullPath:
			FileList = os.listdir( fullpath )

			for file in FileList:
				if ".root" in file:
					List_ROOTFiles.append( fullpath+"/"+file )

		if len(List_ROOTFiles) == 0:
			print "There is no available root files ... check the directory name"
			print "Path: ", List_FullPath
			sys.exit()

		return List_ROOTFiles

	def GetNormFactor( self ):
		XSecSumW_Powheg = {}
		XSecSumW_Powheg["M50to120"] = [1975, 2976526.0]
		XSecSumW_Powheg["M120to200"] = [19.32, 99998]
		XSecSumW_Powheg["M200to400"] = [2.731, 99999]
		XSecSumW_Powheg["M400to800"] = [0.241, 98999]
		XSecSumW_Powheg["M800to1400"] = [0.01678, 96398]
		XSecSumW_Powheg["M1400to2300"] = [0.00139, 99998]
		XSecSumW_Powheg["M2300to3500"] = [0.00008948, 100000]
		XSecSumW_Powheg["M3500to4500"] = [0.000004135, 99000]
		XSecSumW_Powheg["M4500to6000"] = [4.56E-07, 100000]
		XSecSumW_Powheg["M6000toInf"] = [2.066E-08, 100000]

		# XSecSumW_DYMMaMCNLO = {}
		# XSecSumW_DYMMaMCNLO["M10to50"] = [18610.0/3.0, 7506956]
		# XSecSumW_DYMMaMCNLO["M50toInf"] = [6025.2/3.0, 6311695]
		# XSecSumW_DYMMaMCNLO["M50to100"] = [5869.58346/3.0, 6061181]
		# XSecSumW_DYMMaMCNLO["M100to200"] = [226/3.0, 227522]
		# XSecSumW_DYMMaMCNLO["M200to400"] = [7.67/3.0, 170955]
		# XSecSumW_DYMMaMCNLO["M400to500"] = [0.423/3.0, 50136]
		# XSecSumW_DYMMaMCNLO["M500to700"] = [0.24/3.0, 47833]
		# XSecSumW_DYMMaMCNLO["M700to800"] = [0.035/3.0, 44740]
		# XSecSumW_DYMMaMCNLO["M800to1000"] = [0.03/3.0, 43496]
		# XSecSumW_DYMMaMCNLO["M1000to1500"] = [0.016/3.0, 40783]
		# XSecSumW_DYMMaMCNLO["M1500to2000"] = [0.002/3.0, 37176]
		# XSecSumW_DYMMaMCNLO["M2000to3000"] = [0.00054/3.0, 23078]

		XSec = -1
		SumW = -1

		# -- need to be added: ttbar, diboson, ... -- #
		xSecSumW_Others = {}
		xSecSumW_Others["ZZ"] = [16.523, 998034]
		xSecSumW_Others["ZZTo4L"] = [1.256, -1.0]

		xSecSumW_Others["WW"] = [-1.0, 6987123]
		xSecSumW_Others["WWTo2L2Nu"] = [12.178, 1999000]

		xSecSumW_Others["WZ"] = [47.13, 2995828]
		xSecSumW_Others["WZTo3LNu"] = [-1.0, -1.0]

		xSecSumW_Others["ttbar"] = [831.76, 77229334+76175894]
		xSecSumW_Others["ttbarTo2L2Nu"] = [87.31, 79092391]

		xSecSumW_Others["tW"] = [35.6, 6952830]
		xSecSumW_Others["tbarW"] = [35.6, 6933093]


		if "DYPowheg" in self.Sample:
			MassRange = self.Sample.split("_")[1]
			if MassRange in XSecSumW_Powheg:
				XSec = XSecSumW_Powheg[ MassRange ][0]
				SumW = XSecSumW_Powheg[ MassRange ][1]
			else:
				XSec = -1
				SumW = -1

		# elif "DYMuMu" in self.Sample:
		# 	MassRange = self.Sample.split("_")[1]
		# 	if MassRange in XSecSumW_DYMMaMCNLO:
		# 		XSec = XSecSumW_DYMMaMCNLO[ MassRange ][0]
		# 		SumW = XSecSumW_DYMMaMCNLO[ MassRange ][1]
		# 	else:
		# 		XSec = -1
		# 		SumW = -1
		elif self.Sample in xSecSumW_Others:
			XSec = xSecSumW_Others[ self.Sample ][0]
			SumW = xSecSumW_Others[ self.Sample ][1]
		else:
			XSec = -1
			SumW = -1

		if XSec < 0:
			print "There is no cross section information for this sample: %s" % (self.Sample)
			sys.exit()

		if SumW < 0:
			print "There is no SumW information for this sample: %s" % (self.Sample)
			sys.exit()

		normfactor = (XSec * self.Lumi) / SumW
		print "[%s] Normalization factor = (%.10lf * %.10lf) / %.10lf = %.10lf" % (self.Sample, XSec, self.Lumi, SumW, normfactor)

		return normfactor


if __name__ == "__main__":
	split = SplitJobs( opts )
	split.CreateWorkSpace()