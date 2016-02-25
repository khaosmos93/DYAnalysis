#!/usr/bin/env python

from subprocess import Popen
from ROOT import *
import glob
import os 

def populator(folder):
    print glob.glob(folder+'resMatrixProd_RunAB*.root')
    for file in glob.glob(folder+'resMatrixProd_RunAB*.root'):
        print 'cp '+file+' '+file[:-11]+'C_tmp.root'
        Popen('cp '+file+' '+file[:-11]+'C_tmp.root',shell=True).wait()
        Popen('cp '+file+' '+file[:-11]+'D_tmp.root',shell=True).wait()
        Popen('cp '+file+' '+file[:-8]+'POWHEG_tmp.root',shell=True).wait()

#FIXME they are all the same
eras = ['RunAB'] #,'RunC','RunD']
regimes = ['MuMu'] #['MuMu'] #,'EE','EMu']
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
os.environ["LD_LIBRARY_PATH"] = os.environ["LD_LIBRARY_PATH"] + ":/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/Unfold_Purdue/proofbox/cache/"
#set
#setenv LD_LIBRARY_PATH

#analyzers
for run in eras:
    for reg in regimes:
        Popen('root -b -q -l \'ResMatrix.C(\"'+run+'\",\"'+reg+'\")\'',shell=True).wait() 

#postprocessing
#Popen('hadd -f resMatrixProd_RunAB_tmp.root DY*RunAB.root',shell=True).wait()
#populate
#FIXME they are all the same
#populator("./")
#copy to main
#for run in ['RunAB','RunC','RunD']:
#    Popen('cp resMatrixProd_'+run+'_tmp.root ../Inputs/unfold/',shell=True).wait()
