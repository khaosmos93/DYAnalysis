#!/usr/bin/env python

from subprocess import Popen
from ROOT import *

eras = ['RunAB'] #['RunAB','RunC','RunD','POWHEG']
#analyzers
for run in eras:
    Popen('root -b -q -l \'unfold.C(\"'+run+'\")\'',shell=True).wait() 
    #Popen('cp rootfiles/unfoldData_HLT_Double_'+run+'.root ../Inputs/unfold/unfoldData_HLT_Double_'+run+'_tmp.root',shell=True).wait()

#FIXME no need in this really!
#Popen('hadd -f rootfiles/unfoldData_HLT_Double.root rootfiles/unfoldData_HLT_Double_Run*.root',shell=True).wait()
#Popen('cp rootfiles/unfoldData_HLT_Double.root ../Inputs/unfold/unfoldData_HLT_Double_tmp.root',shell=True).wait()
