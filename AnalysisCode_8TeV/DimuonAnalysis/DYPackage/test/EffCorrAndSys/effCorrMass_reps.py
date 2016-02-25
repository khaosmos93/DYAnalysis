#!/usr/bin/env python

from subprocess import Popen

corrTypes = ['trig','iso','recoid']
RUNSPLIT = ['_split','_RunAB','_RunC','_RunD'] #,'_runA','_runB','_split'] #'_runA','_runB','_split','_runAB'] #,'_runB']
# use '_aux' for special comparisons per run range
split = '' #'_MCOnly' #'_aux'
effCorr = 'c' #effC
chunk = [0,5]

for run in RUNSPLIT:
    for corrType in corrTypes:
        for irep in range(chunk[0],chunk[1]):
            #Popen('root -b -q -l \''+effCorr+'orrMass'+split+'.C(\"'+corrType+'\",\"'+run+'\",\"'+'eta'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            #combined runs: if you type _runAB else it will do split by runs, else just type whatever   
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'invm'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap2030'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap3045'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap4560'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap60120'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap120200'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            Popen('root -b -q -l \''+effCorr+'orrMass'+split+'_reps.C(\"'+corrType+'\",\"'+run+'\",\"'+'absrap200'+'\",\"'+str(irep+1)+'\")\'',shell=True).wait()
            ##Popen('root -b -q -l \'correctionMass.C(\"'+corrType+'\")\'',shell=True).wait()

#
#AUXILIARY
#

#for corrType in corrTypes:
#    Popen('root -b -q -l \''+effCorr+'orrMass_aux.C(\"'+corrType+'\",\"'+'NOT_runAB'+'\",\"'+'invm'+'\")\'',shell=True).wait()
