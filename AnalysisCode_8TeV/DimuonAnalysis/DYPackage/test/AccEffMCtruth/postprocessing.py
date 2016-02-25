#!/usr/bin/env python

from subprocess import Popen
import datetime

now = datetime.datetime.now()

Popen('root -b -q -l \'plotMCtruth.C(\"'+str(now)[:11]+'\")\'',shell=True).wait()
Popen('root -b -q -l \'plotMCtruth_2D.C(\"'+str(now)[:11]+'\")\'',shell=True).wait()

#copy the right place but not override...
Popen('cp out1_'+NOW+'_w1.root ../Inputs/acc/out1_w1_tmp.root',shell=True).wait()
Popen('cp out2_'+NOW+'_w1.root ../Inputs/acc/out2_w1_tmp.root',shell=True).wait()
