#!/usr/bin/env python

from ROOT import *
from math import sqrt, pow
from array import array

import sys, os
from rshape_tools import *

xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500]

#get 8tev muon uncertainty inputs
fsyst_mu1D = TFile('../Inputs/sys/muonSystAll_1D.root')	
hstat8 = fsyst_mu1D.Get('stat')
effcorr_syst8= fsyst_mu1D.Get('effcorr_syst')
for i in range(effcorr_syst8.GetNbinsX()):
    effcorr_syst8.SetBinContent(i+1,effcorr_syst8.GetBinError(i+1)/effcorr_syst8.GetBinContent(i+1))
    effcorr_syst8.SetBinError(i+1,0.)

unf_syst8= fsyst_mu1D.Get('unf_syst')
bkg_syst8= fsyst_mu1D.Get('bkg_syst')
FSR_syst8= fsyst_mu1D.Get('FSR_syst')
#FIXME it is no longer in percent!!!
for i in range(FSR_syst8.GetNbinsX()):
    FSR_syst8.SetBinContent(i+1,FSR_syst8.GetBinContent(i+1)/100.)

acc_syst8= fsyst_mu1D.Get('acc_syst')
#pdf_syst8= fsyst_mu1D.Get('pdf_syst')
#for i in range(pdf_syst8.GetNbinsX()):
#    pdf_syst8.SetBinContent(i+1,pdf_syst8.GetBinContent(i+1)/100.)
#mod_syst8= fsyst_mu1D.Get('mod_syst')

#get muon rshape ? or absolute cross section
#we will get the absolute cross section
f_mu1D = TFile('../Outputs/absex_full_Bayesian.root')
mu_xsec41 = f_mu1D.Get('hxsec')
#remove 41st bin
mu_xsec = TH1D('hxsec','hxsec',40,array('d',xAxis))
for i in range(mu_xsec.GetNbinsX()):
    mu_xsec.SetBinContent(i+1,mu_xsec41.GetBinContent(i+1))
#peak cross section and associated error
mu_peak = 1135.4 
mu_peak_err = sqrt(pow(11.2/mu_peak,2)+pow(25.1/mu_peak,2))

#calculate shape
for i in range(mu_xsec.GetXaxis().GetNbins()):
      mu_xsec.SetBinContent(i+1,mu_xsec.GetBinContent(i+1)/mu_peak)
      mu_xsec.SetBinError(i+1,0)

#get 7tev muon uncertainty inputs
fsyst_mu1D_7TeV = TFile('../Inputs/sys/muonSystAll_1D_7TeV.root')
hstat7 = fsyst_mu1D_7TeV.Get('stat')
effcorr_syst7= fsyst_mu1D_7TeV.Get('effcorr_syst')
for i in range(effcorr_syst7.GetNbinsX()):
    effcorr_syst7.SetBinContent(i+1,effcorr_syst7.GetBinError(i+1)/effcorr_syst7.GetBinContent(i+1))
    effcorr_syst7.SetBinError(i+1,0.)

unf_syst7= fsyst_mu1D_7TeV.Get('unf_syst')
bkg_syst7= fsyst_mu1D_7TeV.Get('bkg_syst')
FSR_syst7= fsyst_mu1D_7TeV.Get('FSR_syst')
#FIXME no longer in percent
for i in range(FSR_syst7.GetNbinsX()):
    FSR_syst7.SetBinContent(i+1,FSR_syst7.GetBinContent(i+1)/100.)
acc_syst7= fsyst_mu1D_7TeV.Get('acc_syst')
#pdf_syst7= fsyst_mu1D_7TeV.Get('pdf_syst')
#for i in range(pdf_syst7.GetNbinsX()):
#    pdf_syst7.SetBinContent(i+1,pdf_syst7.GetBinContent(i+1)/100.)
#mod_syst7= fsyst_mu1D_7TeV.Get('mod_syst')

#for i in range(mod_syst7.GetNbinsX()):
#    print mod_syst7.GetBinContent(i+1)*100.

#get rshape at 7 TeV
fmu_7TeV = TFile("../Outputs/rshape_full_mumuCurrentMarch.root")
mu_xsec7 = fmu_7TeV.Get('hxsec')

#peak cross section and associated error
mu_peak7 = 989.5
mu_peak_err7 = sqrt(pow(9.8/mu_peak7,2)+pow(21.9/mu_peak7,2))

#calculate the ratio, set all errors to 0
hratio = mu_xsec.Clone()
hratio.Divide(mu_xsec,mu_xsec7)
#set zero errors
for i in range(hratio.GetXaxis().GetNbins()):
      hratio.SetBinError(i+1,0)

#double ratio uncertainties

#propagate uncorrelated errors
hstat78 =  hstat7.Clone()
for i in range(hstat78.GetNbinsX()):
    hstat78.SetBinContent(i+1,sqrt(pow(hstat7.GetBinContent(i+1),2)+pow(hstat8.GetBinContent(i+1),2)))

effcorr_syst78 = effcorr_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    effcorr_syst78.SetBinContent(i+1,sqrt(pow(effcorr_syst7.GetBinContent(i+1),2)+pow(effcorr_syst8.GetBinContent(i+1),2)))

unf_syst78= unf_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    unf_syst78.SetBinContent(i+1,sqrt(pow(unf_syst7.GetBinContent(i+1),2)+pow(unf_syst8.GetBinContent(i+1),2)))

bkg_syst78= bkg_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    bkg_syst78.SetBinContent(i+1,sqrt(pow(bkg_syst7.GetBinContent(i+1),2)+pow(bkg_syst8.GetBinContent(i+1),2)))

#propagate correlated errors
acc_syst78= acc_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    acc_syst78.SetBinContent(i+1,sqrt(pow(acc_syst7.GetBinContent(i+1),2)+pow(acc_syst8.GetBinContent(i+1),2)-2*acc_syst7.GetBinContent(i+1)*acc_syst8.GetBinContent(i+1)))

#pdf_syst78= pdf_syst7.Clone()
#for i in range(hstat78.GetNbinsX()):
#    pdf_syst78.SetBinContent(i+1,sqrt(pow(pdf_syst7.GetBinContent(i+1),2)+pow(pdf_syst8.GetBinContent(i+1),2)-2*0.95*pdf_syst7.GetBinContent(i+1)*pdf_syst8.GetBinContent(i+1)))

#mod_syst78= mod_syst7.Clone()
#for i in range(hstat78.GetNbinsX()):
#    mod_syst78.SetBinContent(i+1,sqrt(pow(mod_syst7.GetBinContent(i+1),2)+pow(mod_syst8.GetBinContent(i+1),2)-2*mod_syst7.GetBinContent(i+1)*mod_syst8.GetBinContent(i+1)))

FSR_syst78= FSR_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    FSR_syst78.SetBinContent(i+1,sqrt(pow(FSR_syst7.GetBinContent(i+1),2)+pow(FSR_syst8.GetBinContent(i+1),2)-2.*FSR_syst7.GetBinContent(i+1)*FSR_syst8.GetBinContent(i+1)))

#calculate the total uncertainty
total_unc = hstat7.Clone()
for i in range(hstat78.GetNbinsX()):
    #all the systematics
    #if fullDET == 'full' and not geoTest: 
    #total_unc.SetBinContent(i+1,sqrt(pow(acc_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    total_unc.SetBinContent(i+1,sqrt(pow(acc_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))

    #no acceptance and modeling, PDF
    #elif fullDET == 'DET': total_unc.SetBinContent(i+1,sqrt(pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #elif geoTest: total_unc.SetBinContent(i+1,sqrt(pow(acc_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #no efficiency correction
    #total_unc.SetBinContent(i+1,sqrt(pow(mod_syst78.GetBinContent(i+1),2)+pow(pdf_syst78.GetBinContent(i+1),2)+pow(acc_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #no statistical
    #total_unc.SetBinContent(i+1,sqrt(pow(mod_syst78.GetBinContent(i+1),2)+pow(pdf_syst78.GetBinContent(i+1),2)+pow(acc_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)))
    #no background estimation
    #total_unc.SetBinContent(i+1,sqrt(pow(mod_syst78.GetBinContent(i+1),2)+pow(pdf_syst78.GetBinContent(i+1),2)+pow(acc_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #stat only
    #total_unc.SetBinContent(i+1,hstat78.GetBinContent(i+1))
    total_unc.SetBinError(i+1,0.000)


#combine all and set to the result
for i in range(hratio.GetXaxis().GetNbins()):
      if i < 10 or i > 22: hratio.SetBinError(i+1, hratio.GetBinContent(i+1)*sqrt(pow(total_unc.GetBinContent(i+1),2)+pow(0.005,2)))
      else: hratio.SetBinError(i+1, hratio.GetBinContent(i+1)*total_unc.GetBinContent(i+1))

fout = TFile('doubleratio_1Dmu_full_Bayesian.root','recreate')
hratio.Write()
fout.Close()
