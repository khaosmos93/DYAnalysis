#!/usr/bin/env python

from subprocess import Popen
#from ROOT import *
#import datetime

#now = datetime.datetime.now()

Popen('root -b -l -q \'analyseMCtruth.C()\'',shell=True).wait()
#Popen('hadd -f DYMCtruth'+str(now)[:11]+'_w1.root DYM*MCtruth.root',shell=True).wait()
