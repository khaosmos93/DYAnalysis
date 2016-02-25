#!/usr/bin/env python

from subprocess import Popen

chunk = [0,100]
RUNSPLIT = '_split'

for irep in range(chunk[0],chunk[1]):
    Popen('root -b -q -l \'effCorrelEstimator_step1.C(\"'+str(irep+1)+'\",\"'+RUNSPLIT+'\")\'',shell=True).wait()
