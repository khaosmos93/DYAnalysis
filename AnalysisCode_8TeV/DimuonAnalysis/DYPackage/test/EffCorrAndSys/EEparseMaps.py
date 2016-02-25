#!/usr/bin/env python

from subprocess import Popen
import os

#binning 1415 is a variable binning RECOID/ISO-TRIG combined 
BINNINGs = ['EE']
#Possible regimes for 8 TeV study are '_RunAB','_RunC','_RunD' - split runs
# and '_Debug''_Debug'. 'runABCD' is deprecated  
#Currently RunAB is the default MC identifier
RUNSPLIT = ["_RunA"]
PUmarker = '' #,'_PU'

print "Bin choices are hard coded in cfg!"
print "WARNING: expect funny results, if runAB is not postfixed"

for run in RUNSPLIT:
    for bin in BINNINGs:
        print "Cleaning trash..."
        Popen('rm efficiencyWeightToBin.C',shell=True).wait()
        print "Parsing weight map for bin "+bin+"..."
        print "PU reweighted MC maps are in PU file. The data efficiencies should be identical there..."
        Popen('python EEmapParser.py '+bin+' '+run,shell=True).wait()
        #BINNING is obsolete
        Popen('cp efficiencyWeightToBin.C efficiencyWeightToBin_'+run+PUmarker+'.C',shell=True).wait()
