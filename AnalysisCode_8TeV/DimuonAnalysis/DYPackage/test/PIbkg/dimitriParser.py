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
PDFset = '' #'_NNPDF23'
file2D = 'pi_background2D_CMS'+PDFset+'.dat'  #open('pi_background2D.dat').readlines()
#allocate an array
mat2D = np.zeros((5,108))
mat2D = np.loadtxt(file2D)

hist_final2D = TH1D('pi_back_2d','pi_back_2d',132,0,132)
hist_final2D_DY = TH1D('dy_2d','dy_2d',132,0,132)

for jx in range(132):
    if jx < 24:
        #no PI background at low mass
        hist_final2D.SetBinContent(jx+1,0.000000001)
        hist_final2D.SetBinError(jx+1,0.0000000001)
        hist_final2D_DY.SetBinContent(jx+1,1.000000001)
        hist_final2D_DY.SetBinError(jx+1,0.0000000001)
    else:
        hist_final2D.SetBinContent(jx+1,mat2D[jx-24,1]-mat2D[jx-24,3]) #-24 because we do not have first bin
        hist_final2D.SetBinError(jx+1,sqrt(pow(mat2D[jx-24,2],2)+pow(mat2D[jx-24,4],2)))
        hist_final2D_DY.SetBinContent(jx+1,mat2D[jx-24,1]) #-24 because we do not have first bin
        hist_final2D_DY.SetBinError(jx+1,sqrt(pow(mat2D[jx-24,4],2)))


fout2 = TFile("pi_background2D_CMS"+PDFset+".root","recreate")
fout2.cd()
hist_final2D.Write()
hist_final2D_DY.Write()
fout2.Close()

'''
#load table to matrix using numpy
file1D = 'pi_background1D_CMS'+PDFset+'.dat'  #open('pi_background2D.dat').readlines()
#allocate an array
#mat1D = np.zeros((2,41))
mat1D = np.zeros((4,41))
mat1D = np.loadtxt(file1D)

hist_final1D = TH1D('pi_back_1d','pi_back_1d',41,array('d',xAxis))
hist_final1D_DY = TH1D('dy_1d','dy_1d',41,array('d',xAxis))

for jx in range(41):
    #hist_final1D.SetBinContent(jx+1,mat1D[jx,0]-mat1D[jx,1]) #-24 because we do not have first bin
    #hist_final1D.SetBinError(jx+1,hist_final1D.GetBinContent(jx+1)/100.);
    #hist_final1D.SetBinContent(jx+1,mat1D[jx,0]-mat1D[jx,1]) #-24 because we do not have first bin
    #hist_final1D.SetBinError(jx+1,hist_final1D.GetBinContent(jx+1)/100.);
    hist_final1D.SetBinContent(jx+1,mat1D[jx,0]-mat1D[jx,2]) #-24 because we do not have first bin
    hist_final1D.SetBinError(jx+1,0.5*sqrt(pow(mat1D[jx,1],2)+pow(mat1D[jx,3],2)))
    hist_final1D_DY.SetBinContent(jx+1,mat1D[jx,0]) #-24 because we do not have first bin
    hist_final1D_DY.SetBinError(jx+1,0.5*sqrt(pow(mat1D[jx,1],2)))
    #hist_final1D.SetBinError(jx+1,hist_final1D.GetBinContent(jx+1)/100.);

fout1 = TFile("pi_background1D_CMS"+PDFset+".root","recreate")
fout1.cd()
hist_final1D.Write()
hist_final1D_DY.Write()
fout1.Close()
'''
