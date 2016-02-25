#!/usr/bin/env python

from ROOT import *
from math import sqrt, pow
from array import array

import sys, os
from rshape_tools import *

xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500]

#get 8tev muon uncertainty inputs
fsyst_mu1D = TFile('../Inputs/sys/electronSystAll_1D.root')
hescale_syst8= fsyst_mu1D.Get('escale_syst')
for i in range(hescale_syst8.GetNbinsX()):
    hescale_syst8.SetBinContent(i+1,hescale_syst8.GetBinContent(i+1)/100.) # it was in percent

hstat8 = fsyst_mu1D.Get('stat')
for i in range(hstat8.GetNbinsX()):
    hstat8.SetBinContent(i+1,hstat8.GetBinContent(i+1)/100.) # it was in percent

#heffMC_syst = fsyst_mu1D.Get('effMC_syst')
#for i in range(heffMC_syst.GetNbinsX()):
#    heffMC_syst.SetBinContent(i+1,heffMC_syst.GetBinContent(i+1)/100.) # it was in percent

effcorr_syst8= fsyst_mu1D.Get('effcorr_syst')
for i in range(effcorr_syst8.GetNbinsX()):
    effcorr_syst8.SetBinContent(i+1,effcorr_syst8.GetBinError(i+1)/effcorr_syst8.GetBinContent(i+1))
    effcorr_syst8.SetBinError(i+1,0.)

unf_syst8= fsyst_mu1D.Get('unf_syst')
for i in range(hstat8.GetNbinsX()):
    unf_syst8.SetBinContent(i+1,unf_syst8.GetBinContent(i+1)/100.) # it was in percent

bkg_syst8= fsyst_mu1D.Get('bkg_syst')
for i in range(hstat8.GetNbinsX()):
    bkg_syst8.SetBinContent(i+1,bkg_syst8.GetBinContent(i+1)/100.) # it was in percent
FSR_syst8= fsyst_mu1D.Get('FSR_syst')
acc_stat8= fsyst_mu1D.Get('acc_stat')
acc_syst8= fsyst_mu1D.Get('acc_syst')
for i in range(hstat8.GetNbinsX()):
    acc_syst8.SetBinContent(i+1,acc_syst8.GetBinContent(i+1)/100.) # it was in percent

#pdf_syst8= fsyst_mu1D.Get('pdf_syst')
#for i in range(pdf_syst8.GetNbinsX()):
#    pdf_syst8.SetBinContent(i+1,pdf_syst8.GetBinContent(i+1)/100.) # it was in percent

#mod_syst8= fsyst_mu1D.Get('mod_syst')
#for i in range(mod_syst8.GetNbinsX()):
#    mod_syst8.SetBinContent(i+1,mod_syst8.GetBinContent(i+1)/100.) # it was in percent

#get muon rshape ? or absolute cross section
#we will get the absolute cross section
f_mu1D = TFile('../Outputs/absex_full_EE_Bayesian.root')
mu_xsec41 = f_mu1D.Get('hxsec')
#remove 41st bin
mu_xsec = TH1D('hxsec','hxsec',40,array('d',xAxis))
for i in range(mu_xsec.GetNbinsX()):
    mu_xsec.SetBinContent(i+1,mu_xsec41.GetBinContent(i+1))
#peak cross section and associated error
mu_peak = 1142.6
mu_peak_err = sqrt(pow(11.2/mu_peak,2)+pow(25.1/mu_peak,2))

#calculate shape
for i in range(mu_xsec.GetXaxis().GetNbins()):
      mu_xsec.SetBinContent(i+1,mu_xsec.GetBinContent(i+1)/mu_peak)
      mu_xsec.SetBinError(i+1,0)

#get 7tev muon uncertainty inputs
#fsyst_mu1D = TFile('../Inputs/sys/electronSystAll_1D.root')
fsyst_mu1D_7TeV = TFile('../Inputs/sys/muonSystAll_1D_7TeV.root')
hstat7 = fsyst_mu1D_7TeV.Get('stat')

hescale_syst7= fsyst_mu1D.Get('escale_syst')
for i in range(hescale_syst7.GetNbinsX()):
    hescale_syst7.SetBinContent(i+1,hescale_syst7.GetBinContent(i+1)/100.) # it was in percent

effcorr_syst7= fsyst_mu1D.Get('effcorr_syst')
for i in range(effcorr_syst7.GetNbinsX()):
    effcorr_syst7.SetBinContent(i+1,effcorr_syst7.GetBinError(i+1)/effcorr_syst7.GetBinContent(i+1))
    effcorr_syst7.SetBinError(i+1,0.)

unf_syst7= fsyst_mu1D.Get('unf_syst')
for i in range(hstat7.GetNbinsX()):
    unf_syst7.SetBinContent(i+1,unf_syst7.GetBinContent(i+1)/100.) # it was in percent

bkg_syst7= fsyst_mu1D.Get('bkg_syst')
for i in range(hstat7.GetNbinsX()):
    bkg_syst7.SetBinContent(i+1,bkg_syst7.GetBinContent(i+1)/100.) # it was in percent
FSR_syst7= fsyst_mu1D.Get('FSR_syst')
#acc_stat7= fsyst_mu1D.Get('acc_stat')
acc_syst7= fsyst_mu1D_7TeV.Get('acc_syst')
#pdf_syst7= fsyst_mu1D_7TeV.Get('pdf_syst')
#for i in range(pdf_syst7.GetNbinsX()):
#    pdf_syst7.SetBinContent(i+1,pdf_syst7.GetBinContent(i+1)/100.) # it was in percent

#mod_syst7= fsyst_mu1D_7TeV.Get('mod_syst')

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
    hstat78.SetBinContent(i+1,sqrt(pow(1.15*hstat7.GetBinContent(i+1),2)+pow(hstat8.GetBinContent(i+1),2)))

effcorr_syst78 = effcorr_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    effcorr_syst78.SetBinContent(i+1,sqrt(pow(effcorr_syst7.GetBinContent(i+1),2)+pow(effcorr_syst8.GetBinContent(i+1),2)))

unf_syst78= unf_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    unf_syst78.SetBinContent(i+1,sqrt(pow(unf_syst7.GetBinContent(i+1),2)+pow(unf_syst8.GetBinContent(i+1),2)))

#FIXME
hescale_syst78= hescale_syst7.Clone()
for i in range(hescale_syst7.GetNbinsX()):
    hescale_syst78.SetBinContent(i+1,sqrt(pow(hescale_syst7.GetBinContent(i+1),2)+pow(hescale_syst8.GetBinContent(i+1),2)))


bkg_syst78= bkg_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    bkg_syst78.SetBinContent(i+1,sqrt(pow(1.1*bkg_syst7.GetBinContent(i+1),2)+pow(bkg_syst8.GetBinContent(i+1),2)))

#propagate correlated errors
acc_syst78= acc_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    acc_syst78.SetBinContent(i+1,sqrt(pow(acc_syst7.GetBinContent(i+1),2)+pow(acc_syst8.GetBinContent(i+1),2)-2*acc_syst7.GetBinContent(i+1)*acc_syst8.GetBinContent(i+1)))

#pdf_syst78= pdf_syst7.Clone()
#for i in range(hstat78.GetNbinsX()):
#    pdf_syst78.SetBinContent(i+1,sqrt(pow(pdf_syst7.GetBinContent(i+1),2)+pow(pdf_syst8.GetBinContent(i+1),2)-2*pdf_syst7.GetBinContent(i+1)*pdf_syst8.GetBinContent(i+1)))

#mod_syst78= mod_syst7.Clone()
#for i in range(hstat78.GetNbinsX()):
#    mod_syst78.SetBinContent(i+1,sqrt(pow(mod_syst7.GetBinContent(i+1),2)+pow(mod_syst8.GetBinContent(i+1),2)-2*mod_syst7.GetBinContent(i+1)*mod_syst8.GetBinContent(i+1)))

FSR_syst78= FSR_syst7.Clone()
for i in range(hstat78.GetNbinsX()):
    FSR_syst78.SetBinContent(i+1,sqrt(pow(FSR_syst7.GetBinContent(i+1),2)+pow(FSR_syst8.GetBinContent(i+1),2)-2.*FSR_syst7.GetBinContent(i+1)*FSR_syst8.GetBinContent(i+1)))

#calculate the total uncertainty
total_unc = hstat7.Clone()
for i in range(hstat78.GetNbinsX()):
    total_unc.SetBinContent(i+1,sqrt(pow(hescale_syst78.GetBinContent(i+1),2)+pow(acc_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(hstat78.GetBinContent(i+1),2)))
    #total_unc.SetBinContent(i+1,sqrt(pow(hstat78.GetBinContent(i+1),2)+pow(effcorr_syst78.GetBinContent(i+1),2)+pow(unf_syst78.GetBinContent(i+1),2)+pow(bkg_syst78.GetBinContent(i+1),2)+pow(FSR_syst78.GetBinContent(i+1),2)+pow(acc_syst78.GetBinContent(i+1),2)+pow(pdf_syst78.GetBinContent(i+1),2)+pow(mod_syst78.GetBinContent(i+1),2)))
    total_unc.SetBinError(i+1,0.000)


#combine all and set to the result
for i in range(hratio.GetXaxis().GetNbins()):
      hratio.SetBinError(i+1, hratio.GetBinContent(i+1)*total_unc.GetBinContent(i+1))

fout = TFile('doubleratio_1Dee_full_Bayesian.root','recreate')
hratio.Write()
fout.Close()
