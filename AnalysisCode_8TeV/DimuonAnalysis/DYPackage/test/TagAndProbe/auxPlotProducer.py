#!/usr/bin/env/python

from subprocess import Popen
import os

#
#FOR GENERAL EFFICIENCY EXERCISE
#
inputVars = ['vtx'] #['pt2','eta'] 
whichHists = {'recoid':1,'iso':2,'trig':3}
scen_trigs = ['DoubleMu17Mu8_Mu17leg','DoubleMu17TkMu8_TkMu8leg'] #['DoubleMu17Mu8_Mu17leg','DoubleMu17TkMu8_TkMu8leg','DZ_filter']#,'DoubleMu17Mu8_Mu17','DoubleMu17TkMu8_TkMu8']
fitCombo = 'cbelVPlus'
eras = ['RunAB','RunC','RunD'] #POWHEG is called implicitely in include/getSimplePlot.C

for run in eras:
    for var in inputVars:
        for histo in whichHists.keys():
            if histo != 'trig':
                #dir = 'outputFiles_'+whichHists[histo]+'_53X_cbelVPlus/' 
                if histo == 'recoid': dir = 'outputFiles_1_53X_cbelVPlus/'
                if histo == 'iso': dir = 'outputFiles_2_53X_cbelVPlus/'
                Popen('root -b -q -l \'getComparisons_ABCD.C(\"'+dir+'\",\"'+var+'\",\"'+histo+'\",\"'+scen_trigs[0]+'\",\"'+fitCombo+'\",\"'+run+'\")\'',shell=True).wait()
            else:
               for trig in scen_trigs:
                   dir = 'outputFiles_3_53X_cbelVPlus/'
                   Popen('root -b -q -l \'getComparisons_ABCD.C(\"'+dir+'\",\"'+var+'\",\"'+histo+'\",\"'+trig+'\",\"'+fitCombo+'\",\"'+run+'\")\'',shell=True).wait()
