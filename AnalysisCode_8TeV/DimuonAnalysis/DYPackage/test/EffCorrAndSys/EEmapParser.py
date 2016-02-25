#!/usr/bin/env/python

from subprocess import Popen
import sys

#Main full analysis definitions
inputConfs = {3:'Glb_Tight2012_WP80ToHLTEle8',4:'Glb_Tight2012_WP80ToHLTEle17'}

vtxType = [''] #,'DA']
confLabel = ''

BINNING = str(sys.argv[1])
RUN = str(sys.argv[2])
if RUN == "runABCD": RUN = ""

INPUT_PREFIX = '/home/ba01/u115/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/TagAndProbe/'

print "Doing step1"
for confs in inputConfs.keys():
    #account for variable binning
    for type in vtxType:
        confLabel = str(confs)
        #if confs == 3: confLabel = '3_leg_tk1'
        if confs > 3: confLabel = '3'
        Popen('root -b -q -l \'EEmapParser.C(\"'+INPUT_PREFIX+'EEoutputFiles_'+confLabel+'_53X_cbelVPlus/'+'\",\"'+inputConfs[confs]+'\",\"'+BINNING+'\",\"'+RUN
+'\")\'',shell=True).wait()
        #FIXME below is deprecated
        #if RUN == 'runABCD':  Popen('root -b -q -l \'mapParser.C(\"'+INPUT_PREFIX+'outputFiles_'+confLabel+'_53X_vpvPlus/'+'\",\"'+inputConfs[confs]+'\",\"'+BINNING+'\",\"'+RUN+'\")\'',shell=True).wait()
        #else : Popen('root -b -q -l \'mapParser_SplitRuns.C(\"'+INPUT_PREFIX+'outputFiles_'+confLabel+'_53X_vpvPlus/'+'\",\"'+inputConfs[confs]+'\",\"'+BINNING+'\",\"'+RUN+'\")\'',shell=True).wait()
