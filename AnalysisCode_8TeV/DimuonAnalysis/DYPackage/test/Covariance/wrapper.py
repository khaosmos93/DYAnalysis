#!/usr/bin/env python

from subprocess import Popen
import os


normtypes = ['absolute'] #['rshape','normalized','absolute']
accFlags = ['inAcc'] #['inAcc','fullAcc']
fsrFlags = ['pre'] #,'post']

if not os.path.exists('./results'):
    print "Output folder does not yet exist!"
    Popen('mkdir ./results',shell=True).wait()

#analyzers
for norm in normtypes:
    for acc in accFlags:
        for fsr in fsrFlags:
            Popen('root -b -l -q \'print2D_current.C(\"2D\",\"mu\",\"'+fsr+'\",\"'+acc+'\",\"'+norm+'\")\'',shell=True).wait()
            #Popen('root -b -l -q \'print1D_current_7TeVEleAlter.C(\"1D\",\"mu\",\"'+fsr+'\",\"'+acc+'\",\"'+norm+'\")\'',shell=True).wait()

#for norm in normtypes:
#    for acc in accFlags:
#        for fsr in fsrFlags:
#            if acc == 'fullAcc': continue
#            Popen('root -b -l -q \'print2D_current.C(\"2D\",\"mu\",\"'+fsr+'\",\"'+acc+'\",\"'+norm+'\")\'',shell=True).wait()
