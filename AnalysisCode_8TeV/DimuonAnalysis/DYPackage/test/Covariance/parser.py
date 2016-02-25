#!/usr/bin/env python

#0-0.1  1.2  6.2  0.29  0.57  0.76  6.3
#0.1-0.2  1.2  6  0.37  0.56  0.58  6.1
#0.2-0.3  1.2  6  0.33  0.55  1.2  6.2

import numpy as np
from ROOT import *
from array import array
import sys
from math import sqrt, pow

#load table to matrix using numpy
file2D = 'r2D_syst_preFSR_inAcc_mu_26.03.13_forparser.dat'
#allocate an array
mat2D = np.zeros((7,132))
mat2D = np.loadtxt(file2D)

# rap. bin & stat. error & efficiency correction & unfolding & background & FSR & total sys. err

hist_stat = TH1D('hist_stat','hist_stat',132,0,132)
hist_effcorr = TH1D('hist_effcorr','hist_effcorr',132,0,132)
hist_unfold  = TH1D('hist_unfold','hist_unfold',132,0,132)
hist_bkg = TH1D('hist_bkg','hist_bkg',132,0,132)
hist_FSR = TH1D('hist_FSR','hist_FSR',132,0,132)

for x in range(132):
    hist_stat.SetBinContent(x+1,mat2D[x,1])
    hist_effcorr.SetBinContent(x+1,mat2D[x,2])
    hist_unfold.SetBinContent(x+1,mat2D[x,3])
    hist_bkg.SetBinContent(x+1,mat2D[x,4])
    hist_FSR.SetBinContent(x+1,mat2D[x,5])

fsyst_mu1D = TFile('~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/Inputs/sys/muonSystAll_2D_7TeV.root','recreate')
fsyst_mu1D.cd()
hist_stat.Write('stat')
hist_effcorr.Write('effcorr_syst')
hist_unfold.Write('unf_syst')
hist_bkg.Write('bkg_syst')
hist_FSR.Write('FSR_syst')

