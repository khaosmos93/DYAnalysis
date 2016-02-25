#!/usr/bin/env python

from subprocess import Popen

corrTypes = ['trig','iso','recoid']
#no splits here! all individual
RUNSPLIT = ['_RunAB','_RunC','_RunD'] 
NREPS = '5'

for corrType in corrTypes:
    for run in RUNSPLIT:
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'invm'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap2030'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap3045'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap4560'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap60120'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap120200'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        Popen('root -b -q -l \'mapRMSParser.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap200'+'\",\"'+NREPS+'\")\'',shell=True).wait()
        ##collect garbage for each cat
        Popen('root -b -q -l \'garbageCollector.C(\"'+corrType+'\",\"'+run+'\")\'',shell=True).wait()
        Popen('mv ./outputs/'+corrType+'/DY_'+corrType+run+'_fin.root ./outputs/'+corrType+'/DY_'+corrType+run+'.root',shell=True).wait() 
        Popen('rm ./outputs/'+corrType+'/DY_'+corrType+run+'_err.root',shell=True).wait()
