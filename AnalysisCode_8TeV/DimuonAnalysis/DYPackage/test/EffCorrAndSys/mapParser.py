#!/usr/bin/env/python

from subprocess import Popen
import sys

#Main full analysis definitions
inputConfs = {1:'Glb_Tight2012',2:'Glb_Tight2012_IsolPFRelCombNoEGammaR03',3:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17Mu8_Mu17leg',4:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17TkMu8_TkMu8leg',5:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_tag_DoubleMu17Mu8_Mu17',6:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_tag_DoubleMu17Mu8_Mu17leg',7:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17TkMu8_TkMu8',8:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17Mu8_Mu17'} 

#inputConfs = {3:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17Mu8_Mu17leg',4:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17TkMu8_TkMu8leg'}

#inputConfs = {1:'Glb_Tight2012',2:'Glb_Tight2012_IsolPFRelCombNoEGammaR03',3:'Glb_Tight2012_IsolPFRelCombNoEGammaR03_DoubleMu17TkMu8_TkMu8'}
#inputConfs = {2:'Glb_Tight2012_IsolPFRelCombNoEGammaR03PU'} #,2:'Glb_Tight2012_IsolPFRelCombNoEGammaR03PU'}
#inputConfs = {2:'Glb_Tight2012_IsolPFRelCombNoEGammaR03PU'}

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
        if confs > 2: confLabel = '3'
        Popen('root -b -q -l \'mapParser.C(\"'+INPUT_PREFIX+'outputFiles_'+confLabel+'_53X_cbelVPlus/'+'\",\"'+inputConfs[confs]+'\",\"'+BINNING+'\",\"'+RUN
+'\")\'',shell=True).wait()
        #FIXME below is deprecated
        #if RUN == 'runABCD':  Popen('root -b -q -l \'mapParser.C(\"'+INPUT_PREFIX+'outputFiles_'+confLabel+'_53X_vpvPlus/'+'\",\"'+inputConfs[confs]+'\",\"'+BINNING+'\",\"'+RUN+'\")\'',shell=True).wait()
        #else : Popen('root -b -q -l \'mapParser_SplitRuns.C(\"'+INPUT_PREFIX+'outputFiles_'+confLabel+'_53X_vpvPlus/'+'\",\"'+inputConfs[confs]+'\",\"'+BINNING+'\",\"'+RUN+'\")\'',shell=True).wait()
