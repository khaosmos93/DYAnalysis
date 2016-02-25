#!/usr/bin/env python

from subprocess import Popen
import os

#void analyseEff(const char* SCENARIO, const char* BINNING, const char* EFFTYPE, const char* RUNSPLIT, const char* DOSMEAR) {

RUNSPLIT = ['_RunAB','_RunC','_RunD'] #'_RunAB' #FXIME right now it is: 'runAB','_RunA','_RunB'
PUmarker = '' # empty because they are ALL with PU by default '_PU'

print "Bin choices are hard coded in cfg!"
print "WARNING: expect funny results, if runAB is not postfixed"

#this parameter is being deprecated
bin = '1415'

Popen('rm -r iso/ trig/ recoid', shell=True).wait()
os.system('mkdir iso')
os.system('mkdir trig')
os.system('mkdir recoid')
for run in RUNSPLIT:
    print "Cleaning trash..."
    Popen('rm efficiencyWeightToBin.C',shell=True).wait()
    print "No new maps will be created in this mode"
    Popen('cp efficiencyWeightToBin'+run+'.C efficiencyWeightToBin.C',shell=True).wait()
    #this is when I run MC
    print "Starting calculation."
    #Popen('root -b -q -l \'analyseEff.C(\"'+'mc'+'\",\"'+'trig'+'\",\"'+run+'\",\"'+'False'+'\")\'',shell=True).wait()
    #Popen('root -b -q -l \'analyseEff.C(\"'+'mc'+'\",\"'+'recoid'+'\",\"'+run+'\",\"'+'False'+'\")\'',shell=True).wait()
    #Popen('root -b -q -l \'analyseEff.C(\"'+'mc'+'\",\"'+'iso'+'\",\"'+run+'\",\"'+'False'+'\")\'',shell=True).wait()
    Popen('root -b -q -l \'analyseEff.C(\"'+'data'+'\",\"'+'trig'+'\",\"'+run+'\",\"'+'False'+'\")\'',shell=True).wait()
    Popen('root -b -q -l \'analyseEff.C(\"'+'data'+'\",\"'+'recoid'+'\",\"'+run+'\",\"'+'False'+'\")\'',shell=True).wait()
    Popen('root -b -q -l \'analyseEff.C(\"'+'data'+'\",\"'+'iso'+'\",\"'+run+'\",\"'+'False'+'\")\'',shell=True).wait()
    #void analyseEff(const char* SCENARIO, const char* EFFTYPE, const char* RUNSPLIT, const char* DOSMEAR)
print "Copy outputs some out of tmp..."
#Popen("cp -r iso/ recoid/ trig/ outputs",shell=True).wait

'''
print "Hadding..."
for run in RUNSPLIT:
    for irep in range(chunk[0],chunk[1]):
        #FIXME hadder will be something different
        print "Hadding..."
        Popen('python corrHadder.py '+bin+' '+run+' '+str(irep+1)+' '+PUmarker,shell=True).wait()
'''
