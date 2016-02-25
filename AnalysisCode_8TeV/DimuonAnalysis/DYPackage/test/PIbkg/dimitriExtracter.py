#!/usr/bin/env python


#format of the input file for parsing
#                  with PI                        with no PI
#|Y|      Sigma [pb]    +-    Error         Sigma [pb]    +-    Error
#0.05   0.82003913E-01   0.58387919E-04   0.79038447E-01   0.76023313E-04

import numpy as np
from ROOT import *
from array import array
import sys
from math import sqrt, pow

xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81,
86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141,
150, 160, 171, 185, 200, 220, 243, 273, 320,
380, 440, 510, 600, 1000, 1500,2000]

#load table to matrix using numpy
PDFset = '_NNPDF23'
file2D = 'pi_background2D_CMS'+PDFset+'.dat'  #open('pi_background2D.dat').readlines()
#allocate an array
mat2D = np.zeros((5,108))
mat2D = np.loadtxt(file2D)

hist_final2D_DY = TH1D('dy_2d','dy_2d',132,0,132)

for jx in range(132):
    if jx < 24:
        hist_final2D_DY.SetBinContent(jx+1,1.000000001)
        hist_final2D_DY.SetBinError(jx+1,0.0000000001)
    else:
        hist_final2D_DY.SetBinContent(jx+1,mat2D[jx-24,3]) #-24 because we do not have first bin
        hist_final2D_DY.SetBinError(jx+1,sqrt(pow(mat2D[jx-24,4],2)))


fout2 = TFile("theory2D"+PDFset+".root","recreate")
fout2.cd()
hist_final2D_DY.Write()
fout2.Close()
