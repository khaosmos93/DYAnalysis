#!/usr/bin/env python

from subprocess import Popen

isos = ['mu1iso','mu2iso']
regions = ['B','C','D']
histos = ['','2030','3045','4560','60120','120200','2001500']

for histo in histos:
    for iso in isos:
        for reg in regions:
            Popen('root -b -q -l \'ABCD2vari_init.C(\"'+histo+'\",\"'+iso+'\",\"'+reg+'\")\'',shell=True).wait()

import sys,os
import glob
from ROOT import *

#merge files
def stringer(inp_list):
    out_string = ''
    for item in inp_list:
        out_string += item+' '
    return out_string


for iso in isos:
    for reg in regions:
        QFRACSs = glob.glob('factor_qcd_'+iso+'_'+reg+'_tmp_*.root')
        os.system("hadd -f factor_qcd_"+iso+"_"+reg+".root "+stringer(QFRACSs))
        os.system("rm "+stringer(QFRACSs))
