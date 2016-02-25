#!/usr/bin/env python

from subprocess import Popen
import sys

use_specific_hist = "" #"_invm_"

#merge files
def stringer(inp_list):
    out_string = ''
    for item in inp_list:
        out_string += item+' '
    return out_string

import os
import glob
folder = './runfolder/'

print "Hadd the main root files" 
#print stringer(EWKs)
included_eras = ['RunAB','RunC','RunD']
for run in included_eras:
    DATAs = glob.glob(folder+'DATA*'+use_specific_hist+run+'.root')
    DYMs = glob.glob(folder+'DYM*'+use_specific_hist+run+'.root')
    TTs = glob.glob(folder+'t*'+use_specific_hist+run+'.root')
    TAUs = glob.glob(folder+'DYtautau*'+use_specific_hist+run+'.root')
    EWKs = glob.glob(folder+'WJ*'+use_specific_hist+run+'.root')
    #QCDs = glob.glob(folder+'QCD*'+use_specific_hist+run+'.root')
    ZZs = glob.glob(folder+'ZZ*'+use_specific_hist+run+'.root')
    WZs = glob.glob(folder+'WZ*'+use_specific_hist+run+'.root')
    WWs = glob.glob(folder+'WW*'+use_specific_hist+run+'.root')

    #print "DATA ", stringer(DATAs)
    #print "DYM ", stringer(DYMs)
    #print "TT ",stringer(TTs)
    #print "TAU ", stringer(TAUs)
    #print "EWK ",stringer(EWKs)
    #print "QCD ",stringer(QCDs)
    #print "WW ",stringer(WWs)
    #print "WZ ",stringer(WZs)
    #print "ZZ ",stringer(ZZs)

    os.system("hadd -f "+folder+"DATA"+use_specific_hist+run+"CP.root "+stringer(DATAs))
    os.system("hadd -f "+folder+"DYM"+use_specific_hist+run+"CP.root "+stringer(DYMs))
    os.system("hadd -f "+folder+"TT"+use_specific_hist+run+"CP.root "+stringer(TTs))
    os.system("hadd -f "+folder+"TAU"+use_specific_hist+run+"CP.root "+stringer(TAUs))
    os.system("hadd -f "+folder+"EWK"+use_specific_hist+run+"CP.root "+stringer(EWKs))
    #os.system("hadd -f "+folder+"QCD"+use_specific_hist+run+"CP.root "+stringer(QCDs))
    os.system("hadd -f "+folder+"DIBOSON"+use_specific_hist+run+"CP.root "+stringer(WWs)+' '+stringer(WZs)+' '+stringer(ZZs))

os.system("hadd -f "+folder+"DATA_CP.root "+folder+"DATA"+use_specific_hist+"Run*CP.root")
os.system("hadd -f "+folder+"DYM_CP.root "+folder+"DYM"+use_specific_hist+"Run*CP.root")
os.system("hadd -f "+folder+"DIBOSON_CP.root "+folder+"DIBOSON"+use_specific_hist+"Run*CP.root")
os.system("hadd -f "+folder+"EWK_CP.root "+folder+"EWK"+use_specific_hist+"Run*CP.root")
os.system("hadd -f "+folder+"TT_CP.root "+folder+"TT"+use_specific_hist+"Run*CP.root")
os.system("hadd -f "+folder+"TAU_CP.root "+folder+"TAU"+use_specific_hist+"Run*CP.root")
#os.system("hadd -f "+folder+"QCD_CP.root "+folder+"QCD"+use_specific_hist+"Run*CP.root")


print "Parse all into one root file"
Popen('root -b -l -q \'parser.C(\"'+'invm_corr'+'\")\'',shell=True).wait()
Popen('root -b -l -q \'parser_2D.C(\"'+'rap_corr'+'\")\'',shell=True).wait()
