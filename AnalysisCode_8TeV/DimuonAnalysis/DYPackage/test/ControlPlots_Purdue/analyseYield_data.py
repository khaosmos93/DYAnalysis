#!/usr/bin/env python

from subprocess import Popen
import os

histos = ['invm']
regimes = ['MuMu'] #,'EE','EMu']
#make shared libraries
#Popen('root -l Muon.cc+',shell=True).wait()
#Popen('root -l Electron.cc+',shell=True).wait()
#Popen('root -l Dimuon.cc+',shell=True).wait()
#Popen('root -l Dielectron.cc+',shell=True).wait()
#copy them over to proofbox. What if does not exist?
Popen('rm -r *.so *.d proofbox',shell=True).wait()
Popen('mkdir proofbox',shell=True).wait()
Popen('mkdir proofbox/cache',shell=True).wait()
Popen('cp ./purdue_lib/*.d ./purdue_lib/*_cc.so ./proofbox/cache/',shell=True).wait()
os.environ["LD_LIBRARY_PATH"] = os.environ["LD_LIBRARY_PATH"] + ":/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/ControlPlots_Purdue/proofbox/cache"
#set
#setenv LD_LIBRARY_PATH

#analyzers
for hist in histos:
    for reg in regimes:
        Popen('root -b -l -q \'analyseYield_data.C(\"'+hist+'\",\"False\",\"'+'RunA'+'\",\"'+reg+'\")\'',shell=True).wait()
