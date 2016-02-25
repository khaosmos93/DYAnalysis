#!/usr/bin/env python

from subprocess import Popen

corrTypes = ['trig','iso','recoid']
RUNSPLIT = ['_split','_RunC','_RunAB','_RunD'] #'_Debug'] #['_split','_RunAB','_RunC','_RunD']
split = '' #'_aux'
effCorr = 'corr' #'corr' #eff

for run in RUNSPLIT:
    for corrType in corrTypes:
        #combined runs: if you type _runAB else it will do split by runs, else just type whatever   
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'invm'+'\")\'',shell=True).wait()
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap2030'+'\")\'',shell=True).wait()
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap3045'+'\")\'',shell=True).wait()
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap4560'+'\")\'',shell=True).wait()
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap60120'+'\")\'',shell=True).wait()
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap120200'+'\")\'',shell=True).wait()
        Popen('root -b -q -l \''+effCorr+'Mass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap200'+'\")\'',shell=True).wait()
        ##Popen('root -b -q -l \'correctionMass.C(\"'+corrType+'\")\'',shell=True).wait()
