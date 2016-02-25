#!/usr/bin/env python 
##bin      Mmin    Mmax      value       error
#1     15     20     0.996566   0.01
#2     20     25     0.994612   0.01
#3     25     30     0.980345   0.01
#4     30     35     0.966097   0.01

from ROOT import *

fin = TFile("../Outputs/absex_full_comb_PI_Bayesian.root")
fin.cd()
hxsec = fin.Get('hxsec')
peak = 1. #1135.4 #1134.6

for ibin in range(hxsec.GetNbinsX()):
    print '%i   %d   %d   %.16f   %.16f' %(ibin+1, hxsec.GetBinCenter(ibin+1)-hxsec.GetBinWidth(ibin+1)/2.,hxsec.GetBinCenter(ibin+1)+hxsec.GetBinWidth(ibin+1)/2.,hxsec.GetBinContent(ibin+1)/peak,hxsec.GetBinError(ibin+1)/peak)
