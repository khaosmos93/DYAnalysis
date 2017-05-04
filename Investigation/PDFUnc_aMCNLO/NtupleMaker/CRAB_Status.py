#!/usr/bin/env python
import subprocess
import os

proxy = '"/tmp/x509up_u8130"'

FileList = os.listdir(".")
print "available crabDir list: "
for filename in FileList:
	if "crab_" in filename:
		print "'"+filename+"',"

CRABDirs = [
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M10to50_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M100to200_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M800to1000_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M1000to1500_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M400to500_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M50toInf_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M2000to3000_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M700to800_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M200to400_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M1500to2000_25ns',
'crab_DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M500to700_25ns',
]

CRABDirs.sort()

print "Selected file list: "
print CRABDirs

# for File in FileList:
# 	if "crab_" in File and os.path.isdir( File ):
# 		print "Recognized crab directory: " + File
# 		CRABDirs.append( File )

ResubmtCMD = []
CompletedList = []
UnknownList = []
OthersList = []

for crabDir in CRABDirs:
	# outputDir = "v" + crabDir.split("_v")[1]
	
	cmd = 'crab statusold "'+crabDir+'" --proxy='+proxy
	result = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
	(stdout, stderr) = result.communicate()
	print "#" * 100
	print cmd+'\n'
	print "[stdout]"
	print stdout
	print "[stderr]"
	print stderr
	print "#" * 100 +'\n\n'

	if "failed" in stdout:
		ResubmtCMD += ['crab resubmit '+crabDir+' --proxy='+proxy]

	elif "COMPLETED" in stdout:
		CompletedList.append( crabDir )

	elif "UNKNOWN" in stdout:
		UnknownList.append( crabDir )

	else:
		OthersList.append( crabDir )

print "[Completed list]"
for one in CompletedList:
	print "'"+one+"',"

print "[Unknown list]"
for one in UnknownList:
	print "'"+one+"',"

print "[Others]"
for one in OthersList:
	print "'"+one+"',"

print "\n[CRAB jobs which should be resubmitted]"
for CMD in ResubmtCMD:
	print CMD

