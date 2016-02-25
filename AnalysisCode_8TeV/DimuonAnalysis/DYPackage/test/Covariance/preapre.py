#!/usr/bin/env python

from ROOT import *

fin = TFile('OUTcovariance_1D_mu_pre_inAcc_absolute.root')
totalCov_TM = fin.Get('totalCov_TM')


for ibinx in range(totalCov_TM.GetNrows()):
    for ibiny in range(totalCov_TM.GetNcols()):
        print ibinx+1, ' ', ibiny+1, ' ', totalCov_TM(ibinx,ibiny)
