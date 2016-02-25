#!/usr/bin/env python

from subprocess import Popen
import sys

histos = ['invm_fine_bb','invm_fineRoch_bb','invm_fine_ee','invm_fineRoch_ee','invm_fine_eb','invm_fineRoch_eb']  #rap'] 
folder = '../runfolder/'

for hist in histos:
    Popen('root -b -l -q \'rochStudy.C(\"'+folder+'\",\"'+hist+'\")\'',shell=True).wait()
