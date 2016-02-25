#!/usr/bin/env python

from subprocess import Popen
from ROOT import *
import os

#def getMigMatrix(htrue, hden):
#    migMatrix = hden.Clone();
#    nbin = htrue.GetXaxis().GetNbins();
#
#    for i in range(nbin+2):
#        for j in range(nbin+2):
#            den = htrue.GetBinContent(i)
#            migMatrix.GetBinContent(i, j, hden.GetBinContent(i,j)/den)
#
#    return migMatrix

#note the FSR response matrix is obtained using the GEN level info so it is unique whatever run range
Popen('root -b -q -l \'FSRResMatrix.C()\'',shell=True).wait() 
Popen('root -b -q -l \'FSRResMatrixExtra.C()\'',shell=True).wait()
Popen('root -b -q -l \'FSRBinByBin.C()\'',shell=True).wait()

#postprocessing
#merge files
#os.system("hadd -f FSRresMatrixProd_tmp.root DYM1020.root DYM20.root DYM200.root DYM400.root DYM500.root DYM700.root DYM800.root DYM1000.root")
#os.system("hadd -f inputCorrinMC_tmp.root DYM1020_2.root DYM20_2.root DYM200_2.root DYM400_2.root DYM500_2.root DYM700_2.root DYM800_2.root DYM1000_2.root")
#os.system("hadd -f BinByBin.root DYM1020_BBB.root DYM20_BBB.root DYM200_BBB.root DYM400_BBB.root DYM500_BBB.root DYM700_BBB.root DYM800_BBB.root DYM1000_BBB.root")

#produce additional corrections
#Popen('root -b -q -l \'fracEff.C()\'',shell=True).wait()
#Popen('root -b -q -l \'fracEff_corr.C()\'',shell=True).wait()

#copy files to main location
#os.system("cp FSRresMatrixProd_tmp.root ../Inputs/FSR/")
#os.system("cp addMoreCorrinMC_tmp.root ../Inputs/FSR/")
#os.system("cp BinByBin.root ../Inputs/FSR/")
