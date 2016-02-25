#!/usr/bin/env python

from subprocess import Popen
from ROOT import *

eras = [''] #['RunAB','RunC','RunD','POWHEG']
#analyzers
for run in eras:
    Popen('root -b -q -l \'unfold_2D.C(\"'+run+'\")\'',shell=True).wait() 
    #Popen('cp rootfiles/unfoldData_HLT_Double_2D_'+run+'.root ../Inputs/unfold/unfoldData_HLT_Double_2D_'+run+'_tmp.root',shell=True).wait()

#Popen('hadd -f rootfiles/unfoldData_HLT_Double_2D.root rootfiles/unfoldData_HLT_Double_2D_Run*.root',shell=True).wait()
#Popen('cp rootfiles/unfoldData_HLT_Double_2D.root ../Inputs/unfold/unfoldData_HLT_Double_2D_tmp.root',shell=True).wait()
