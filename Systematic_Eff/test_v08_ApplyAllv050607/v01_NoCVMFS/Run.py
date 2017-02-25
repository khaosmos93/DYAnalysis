import sys
import os
import time
from os.path import expanduser
from getopt import gnu_getopt as getopt


def instruction():
    print sys.argv[0], " : split the jobs"
    print "  Mandatory options :"
    print "   --code CODE.cxx                  C++ Code file name"
    print "   --lumi LUMI                      Integrated lumionsity in pb"
    print "  Optional options :"
    print "   --queue queueName                      queue name (default: fastq)"
    sys.exit()

def Add_DYPowheg( _List_Samples ):
	_List_Samples.append( ["DYPowheg_M50to120", 1, 5] )
	_List_Samples.append( ["DYPowheg_M120to200", 1, 1] )
	_List_Samples.append( ["DYPowheg_M200to400", 1, 1] )
	_List_Samples.append( ["DYPowheg_M400to800", 1, 1] )
	_List_Samples.append( ["DYPowheg_M800to1400", 1, 1] )
	_List_Samples.append( ["DYPowheg_M1400to2300", 1, 1] )
	_List_Samples.append( ["DYPowheg_M2300to3500", 1, 1] )
	_List_Samples.append( ["DYPowheg_M3500to4500", 1, 1] )
	_List_Samples.append( ["DYPowheg_M4500to6000", 1, 1] )
	_List_Samples.append( ["DYPowheg_M6000toInf", 1, 1] )

def MakeScript_Sub(_OutDir, _List_Samples, _TIME):
	f = open("./script_qsub_ALL.sh", "w")
	f.write( "#!bin/bash\n" )
	f.write( "cwd2=$(pwd)\n\n" )
	f.write( "cd %s\n" % (_OutDir) )
	for Samples in _List_Samples:
		SampleName = Samples[0]
		f.write( "cd %s; source qsub_all.sh\n" % (SampleName) )
		f.write( "cd ../\n")

	f.write( "\n\n" )

	f.write( 'cd ${cwd2}\n' )
	f.write( 'echo "full submission is finished"\n' )
	f.close()

	print "source script_qsub_ALL.sh"

def MakeScript_GetOutput(_OutDir, _List_Samples, _TIME):
	f = open("./script_GetOutput.sh", "w")
	f.write( "#!bin/bash\n" )
	f.write( "cwd2=$(pwd)\n\n" )
	f.write( "cd %s\n" % (_OutDir) )

	for Samples in _List_Samples:
		SampleName = Samples[0]
		f.write( "cd %s; source hadd_all.sh\n" % (SampleName) )
		f.write( "cp *.root ${cwd2}\n" )
		f.write( "cd ../\n")
		f.write( "\n" )

	f.write( "\n\n" )

	f.write( 'cd ${cwd2}\n' )
	f.write( 'echo "finished"\n' )
	f.close()

# -- main -- #
# Parse arguments
if len(sys.argv) < 2: instruction()
try:
    MainOpts, MainArgs = getopt(sys.argv[1:], 'n', ["code=", "lumi=", "queue="])
    MainOpts = dict(MainOpts)
except:
    print "!!! Error parsing arguments"
    instruction()

# -- SampleName, isMC, nJobs -- #
List_Samples = []
List_Samples.append( ["Data", 0, 20] )
Add_DYPowheg( List_Samples )
List_Samples.append( ["WWTo2L2Nu", 1, 1] )
List_Samples.append( ["WZ", 1, 1] )
List_Samples.append( ["ZZ", 1, 1] )
List_Samples.append( ["ttbar", 1, 20] )
List_Samples.append( ["tW", 1, 2] )
List_Samples.append( ["tbarW", 1, 2] )

# -- output directory -- //
TIME = time.strftime('%Y%m%d_%H%M%S', time.localtime(time.time()))
OutDir = "~/Physics/ZprimeAnalysis_80X/Outputs/v%s_%s" % (TIME, MainOpts['--code'].split('.cxx')[0] )
OutDir = os.path.expanduser( OutDir )
os.mkdir( OutDir )

# -- convert path for code to absolute path -- #
CodeAbsPath = os.path.abspath( MainOpts['--code'] )

# -- make script -- #
MakeScript_Sub( OutDir, List_Samples, TIME )
MakeScript_GetOutput( OutDir, List_Samples, TIME )

# sys.path.append( "/home/kplee/Physics/ZprimeAnalysis_80X/Include" )
from Split_BatchJobs_LocalROOT import SplitJobs

for Samples in List_Samples:
	SampleName = Samples[0]
	isMC = (int)(Samples[1])
	nJob = (int)(Samples[2])

	opts = dict()
	opts['--code'] = CodeAbsPath
	opts['--sample'] = SampleName
	opts['--njob'] = nJob
	opts['--lumi'] = (float)(MainOpts['--lumi'])
	opts['--isMC'] = isMC
	opts['--outdir'] = OutDir
	if '--queue' in MainOpts:
		opts['--queue'] = MainOpts['--queue']

	split = SplitJobs( opts )
	split.CreateWorkSpace()