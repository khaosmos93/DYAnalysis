#!/usr/bin/env python

from ROOT import *
from math import sqrt, pow
from array import array

import sys, os
from rshape_tools import *

#get 8tev muon uncertainty inputs
fsyst_mu1D = TFile('../Inputs/sys/muonSystAll_2D.root')	
hstat8 = fsyst_mu1D.Get('stat')
effcorr_syst8= fsyst_mu1D.Get('effcorr_syst')
unf_syst8= fsyst_mu1D.Get('unf_syst')
bkg_syst8= fsyst_mu1D.Get('bkg_syst')
FSR_syst8= fsyst_mu1D.Get('FSR_syst')

fsyst_effMC = TFile('../Inputs/acc/out1.root')
effMC_syst8 = fsyst_effMC.Get('eff_postFSRcorr')
for i in range(effMC_syst8.GetNbinsX()):
    val = effMC_syst8.GetBinContent(i+1)
    effMC_syst8.SetBinContent(i+1,effMC_syst8.GetBinError(i+1)/val)

#get muon rshape ? or absolute cross section
#we will get the absolute cross section
f_mu1D = TFile('../Outputs/absex_DET2D_comb_Bayesian.root')
mu_xsec = f_mu1D.Get('hxsec')
combination_SF_err = 170
for i in range(mu_xsec.GetNbinsX()):
    mu_xsec.SetBinError(i+1,combination_SF_err*mu_xsec.GetBinError(i+1))

#peak cross section and associated error
mu_peak = 567.914 #FIXME
mu_peak_err = sqrt(pow(5.7/mu_peak,2)+pow(1.3/mu_peak,2))

#calculate shape
for i in range(mu_xsec.GetXaxis().GetNbins()):
      mu_xsec.SetBinContent(i+1,mu_xsec.GetBinContent(i+1)/mu_peak) #*mu_xsec.GetBinWidth(i+1))
      mu_xsec.SetBinError(i+1,0)

#get 7tev muon uncertainty inputs
fsyst_mu1D_7TeV = TFile('../Inputs/sys/muonSystAll_2D_7TeV.root')
hstat7 = fsyst_mu1D_7TeV.Get('stat')
effcorr_syst7= fsyst_mu1D_7TeV.Get('effcorr_syst')
unf_syst7= fsyst_mu1D_7TeV.Get('unf_syst')
bkg_syst7= fsyst_mu1D_7TeV.Get('bkg_syst')
FSR_syst7= fsyst_mu1D_7TeV.Get('FSR_syst')

fsyst_effMC7 = TFile('../Inputs/acc/out1_7TeV.root')
effMC_syst7 = fsyst_effMC7.Get('eff_postFSR')
for i in range(effMC_syst7.GetNbinsX()):
    val = effMC_syst7.GetBinContent(i+1)
    effMC_syst7.SetBinContent(i+1,effMC_syst7.GetBinError(i+1)/val)

#get rshape at 7 TeV
fmu_7TeV = TFile("../Outputs/rshape_full2011_2D.root")
#FIXME get all slices then combine
h1 = fmu_7TeV.Get('h20to30')
h2 = fmu_7TeV.Get('h30to45')
h3 = fmu_7TeV.Get('h45to60')
h4 = fmu_7TeV.Get('h60to120')
h5 = fmu_7TeV.Get('h120to200')
h6 = fmu_7TeV.Get('h200to1500')
#allocate a 132 bin one
#mu_peak_7TeV = h4.Integral()
#peak cross section and associated error
mu_peak_7TeV = 526.054 #524.7
#mu_peak_err7 = sqrt(pow(5.1/mu_peak7,2)+pow(1.2/mu_peak7,2))
mu_xsec7 = TH1D('hxsec7','hxsec7',132,0,132)
for i in range(132):
    if i < 24: mu_xsec7.SetBinContent(i+1,h1.GetBinContent(i+1)/h1.GetBinWidth(i+1)/mu_peak_7TeV)
    elif i >= 24 and i < 48:  mu_xsec7.SetBinContent(i+1,h2.GetBinContent(i-24+1)/h2.GetBinWidth(i+1)/mu_peak_7TeV)
    elif i >= 48 and i < 72: mu_xsec7.SetBinContent(i+1,h3.GetBinContent(i-48+1)/h3.GetBinWidth(i+1)/mu_peak_7TeV)
    elif i >= 72 and i < 96: mu_xsec7.SetBinContent(i+1,h4.GetBinContent(i-72+1)/h4.GetBinWidth(i+1)/mu_peak_7TeV)
    elif i >= 96 and i < 120: mu_xsec7.SetBinContent(i+1,h5.GetBinContent(i-96+1)/h5.GetBinWidth(i+1)/mu_peak_7TeV)
    elif i >= 120 :  mu_xsec7.SetBinContent(i+1,h6.GetBinContent(i-120+1)/h6.GetBinWidth(i+1)/mu_peak_7TeV)  

#calculate the ratio, set all errors to 0
hratio = mu_xsec.Clone()
hratio.Divide(mu_xsec,mu_xsec7)
#set zero errors
for i in range(hratio.GetXaxis().GetNbins()):
      hratio.SetBinError(i+1,0)

#double ratio uncertainties

#propagate uncorrelated errors
effMC_syst78 = effMC_syst8.Clone()
for i in range(effMC_syst78.GetNbinsX()):
    effMC_syst78.SetBinContent(i+1,sqrt(pow(effMC_syst7.GetBinContent(i+1),2)+pow(effMC_syst8.GetBinContent(i+1),2)))

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

FSR_syst78= FSR_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    FSR_syst78.SetBinContent(i+1,sqrt(pow(FSR_syst7.GetBinContent(i+1),2)+pow(FSR_syst8.GetBinContent(i+1),2)))


#calculate the total uncertainty
total_unc = hstat7.Clone()
for i in range(hstat78.GetNbinsX()):
    #total uncertainties
    total_unc.SetBinContent(i+1,sqrt(pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)+pow(effMC_syst78.GetBinContent(i+1),2)))
    #statistical only
    #total_unc.SetBinContent(i+1,hstat78.GetBinContent(i+1))
    #No bkg uncertainties
    #total_unc.SetBinContent(i+1,sqrt(pow(FSR_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #No efficiency correction uncertainties
    #total_unc.SetBinContent(i+1,sqrt(pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #efficiency correction only
    #total_unc.SetBinContent(i+1,effcorr_syst78.GetBinContent(i+1))
    total_unc.SetBinError(i+1,0.000)


#combine all and set to the result
for i in range(hratio.GetXaxis().GetNbins()):
      if i < 72 or i > 96: hratio.SetBinError(i+1, hratio.GetBinContent(i+1)*sqrt(pow(total_unc.GetBinContent(i+1)/100.,2)+pow(0.005,2)))
      else: hratio.SetBinError(i+1, hratio.GetBinContent(i+1)*total_unc.GetBinContent(i+1)/100.)

fout = TFile('doubleratio_2Dcomb_Bayesian.root','recreate')
hratio.Write('hxsec')
fout.Close()
