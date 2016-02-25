#!/usr/bin/env python

from subprocess import Popen
import os

#void analyseCorrSyst(const char* SCENARIO, const char* BINNING, const char* EFFTYPE, const char* RUNSPLIT, const char* DOSMEAR) {

RUNSPLIT = ['_RunAB'] #,'_RunC','_RunD'] #'_RunAB' #FXIME right now it is: 'runAB','_RunA','_RunB'
PUmarker = '' # empty because they are ALL with PU by default '_PU'
chunk = [-1,5]

print "Bin choices are hard coded in cfg!"
print "WARNING: expect funny results, if runAB is not postfixed"


#this parameter is being deprecated
bin = '1415'
'''
Popen('rm -r iso/ trig/ recoid', shell=True).wait()
os.system('mkdir iso')
os.system('mkdir trig')
os.system('mkdir recoid')
for run in RUNSPLIT:
    print "Cleaning trash..."
    Popen('rm correctionWeightToBin.C',shell=True).wait()
    print "No new maps will be created in this mode"
    Popen('cp correctionWeightToBin'+run+'.C correctionWeightToBin.C',shell=True).wait()
    for irep in range(chunk[0],chunk[1]):
        print "Starting calculation for replica ", irep+1
        #this is when I run MC
        print "Starting calculation."
        #Popen('root -b -q -l \'analyseCorrSyst.C(\"'+'trig'+'\",\"'+run+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'analyseCorrSyst.C(\"'+'recoid'+'\",\"'+run+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'analyseCorrSyst.C(\"'+'iso'+'\",\"'+run+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
print "Copy outputs some out of tmp..."
#Popen("cp -r iso/ recoid/ trig/ outputs",shell=True).wait
'''

print "Hadding..."
for run in RUNSPLIT:
    for irep in range(chunk[0],chunk[1]):
        #FIXME hadder will be something different
        print "Hadding..."
        Popen('python corrHadder.py '+bin+' '+run+' '+str(irep+1)+' '+PUmarker,shell=True).wait()
