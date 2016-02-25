#!/usr/bin/env python

from ROOT import *
from math import sqrt, pow
from array import array

import sys, os
from rshape_tools import *

xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500, 2000]

#FIXME accept new inouts from Andrius
fAndrius = TFile('../Inputs/sys/table_1D_frac_nBayes1.root')

#
#New source: MC efficiency
#
heffMC_syst1 = fAndrius.Get('eff_rnd_err')
heffMC_syst = fAndrius.Get('eff_rnd_err')
for ibin in range(heffMC_syst.GetNbinsX()):
    heffMC_syst.SetBinContent(ibin+1,100.*sqrt(pow(heffMC_syst1.GetBinContent(ibin+1),2)))
    heffMC_syst.SetBinError(ibin+1,0.)
  
#
#New source: pile up
#
hcollCS_syst = fAndrius.Get('pileup_err')
for ibin in range(hcollCS_syst.GetNbinsX()):
    hcollCS_syst.SetBinContent(ibin+1,100.*sqrt(pow(hcollCS_syst.GetBinContent(ibin+1),2)))
    hcollCS_syst.SetBinError(ibin+1,0.)


#get background systematic uncertainty
#
print "Doing background syst uncertainty"
hsyst_bkg = fAndrius.Get("bkgr_est_err")
for ibin in range(hsyst_bkg.GetNbinsX()):
    hsyst_bkg.SetBinContent(ibin+1,100.*hsyst_bkg.GetBinContent(ibin+1))
    hsyst_bkg.SetBinError(ibin+1,0.)
#fix for adjusted fake background
#hsyst_bkg.SetBinContent(41,80.1964)


#
#get statistical uncertainty
#
print "Doing stat uncertainty"
fstat = TFile('../Inputs/sys/xSec_results_1D_debug_unf.root')
mstat_full_den = fstat.Get('XSec')

fstat2 = TFile('../Inputs/sys/yields1D.root')
mstat_full_num = fstat2.Get('yields_data')

hstat_full = TH1D('hstat_full','hstat_full',41,array('d',xAxis))
for ibin in range(hstat_full.GetNbinsX()):
    hstat_full.SetBinContent(ibin+1,100.*sqrt(mstat_full_num(ibin,0))/(19712.*mstat_full_den(ibin,0)))
    #hstat_full.SetBinContent(ibin+1,100.*1./sqrt(19712.*mstat_full_den(ibin,0)))
    hstat_full.SetBinError(ibin+1,0.)   
    print hstat_full.GetBinContent(ibin+1)


#FSR systematics
hsyst_FSR1 = fAndrius.Get("fsr_rnd_err")
hsyst_FSR2 = fAndrius.Get("fsr_model_err")
hsyst_FSR = fAndrius.Get("fsr_rnd_err")
for ibin in range(hsyst_FSR.GetNbinsX()):
    hsyst_FSR.SetBinContent(ibin+1,100.*sqrt(pow(hsyst_FSR1.GetBinContent(ibin+1),2)+pow(hsyst_FSR2.GetBinContent(ibin+1),2)))
    hsyst_FSR.SetBinError(ibin+1,0.00)   

#
#get efficiency correction systematics
#
print "Doing eff corr syst uncertainty"
heffcorr_err = fAndrius.Get('rho_err')
for ibin in range(heffcorr_err.GetNbinsX()):
    heffcorr_err.SetBinContent(ibin+1,100.*heffcorr_err.GetBinContent(ibin+1))
    heffcorr_err.SetBinError(ibin+1,0.)

#print "Coll CS"
#hcollcs_err = fAndrius.Get('xsec')
#for ibin in range(hcollcs_err.GetNbinsX()):
#    hcollcs_err.SetBinContent(ibin+1,100.*hcollcs_err.GetBinContent(ibin+1))
#    hcollcs_err.SetBinError(ibin+1,0.)

#
#get PDF uncertainty on acceptance (same as dimuons)
#
print "Doing PDF uncertainty"
f_acc_pdf = ROOT.TFile('../Inputs/sys/pdfu_8TeV.root')
hpdf_acc = f_acc_pdf.Get('pdfu')

#for ibin in range(hpdf_acc.GetNbinsX()):
    #print hpdf_acc.GetBinContent(ibin+1)
#
#get modelinc uncertainty on acceptance
#
print "Doing modeling uncertainty"
f_acc_mod = ROOT.TFile('../Inputs/sys/modeling2013_1D_smoothed_41.root')
hmod_acc = f_acc_mod.Get('invm_FEWZ')
for ibin in range(hmod_acc.GetNbinsX()):
    hmod_acc.SetBinContent(ibin+1,hmod_acc.GetBinContent(ibin+1)*100.)
    #print hmod_acc.GetBinContent(ibin+1)

#
#get acceptance systematics
#
print "Doing acceptance uncertainty"
hsyst_acc = fAndrius.Get('acc_rnd_err')
hsyst_acc1 = fAndrius.Get('acc_rnd_err')
for ibin in range(hsyst_acc.GetNbinsX()):
    hsyst_acc.SetBinContent(ibin+1,sqrt(pow(100.*hsyst_acc1.GetBinContent(ibin+1),2)+pow(hmod_acc.GetBinContent(ibin+1),2)+pow(hpdf_acc.GetBinContent(ibin+1),2)))
    hsyst_acc.SetBinError(ibin+1,0.)

#
#get unfolding systematics
#
print "Doing unfolding uncertainty"
hsyst_unf = fAndrius.Get("det_resolution_err")
for ibin in range(hsyst_unf.GetNbinsX()):
    hsyst_unf.SetBinContent(ibin+1,hsyst_unf.GetBinContent(ibin+1)*100.)
#bump
hsyst_unf.SetBinContent(40,(hsyst_unf.GetBinContent(39)+hsyst_unf.GetBinContent(41))/2.)

#save stat uncertainty on unfolding for muons
#fout = ROOT.TFile("muon_unf_stat.root","recreate")
#hsyst_unf_mu = TH1D('muon_unf_stat','muon_unf_stat',41,array('d',xAxis))
#mele_yield = fstat2.Get('yields_data')
#fraw = ROOT.TFile("../Inputs/rawYield/DYspectrum_uncorr_2013_tmp_TRMNov_41.root")
#hmu_yield = fraw.Get("hdata")
#for ibin in range(hsyst_unf_mu.GetNbinsX()):
#    hsyst_unf_mu.SetBinContent(ibin+1,hsyst_unf.GetBinContent(ibin+1)/sqrt(hmu_yield.GetBinContent(ibin+1)/(19712.*mstat_full_den(ibin,0))))
#    hsyst_unf_mu.SetBinError(ibin+1,0.)   
#    print "XX ", hsyst_unf_mu.GetBinContent(ibin+1)
#
#fout.cd()
#hsyst_unf_mu.Write("muon_unf_stat")
#fout.Close()

#c = TCanvas()
#c.cd()
#hsyst_unf.Draw("hist")
#c.SetLogx()
#hsyst_unf.GetXaxis().SetMoreLogLabels()
#c.SaveAs("1_test.png")

#hsyst_unf.SetBinContent(14,hsyst_unf.GetBinContent(14)+5.)

#unfold = [1.9,2.2,1.9,1.4,1.1,1.1,1.3,1.7,2.4,3.8,4.5,5.0, 7.5,7.9,7.3, 3.7,1.7,3.7,4.6,6.5,7.2,7.2,7.1,5.0,4.7,4.7,4.9,4.9,4.7,3.8,2.9,3.3,3.4,3.0,3.7,3.8,3.3,2.3,2.2,1.3,1.3]
#for ibin in range(hsyst_unf.GetNbinsX()):
#    hsyst_unf.SetBinContent(ibin+1,unfold[ibin])
#    hsyst_unf.SetBinError(ibin+1,0.)

#
#get escale systematics (2011)
#
print "Doing escale uncertainty"
hsyst_escale = fAndrius.Get("escale_err")
#hsyst_escale2 = fAndrius.Get("unf_escale_res")
#hsyst_escale = fAndrius.Get("unf_escale_res")
for ibin in range(hsyst_escale.GetNbinsX()):
    hsyst_escale.SetBinContent(ibin+1,100.*sqrt(pow(hsyst_escale.GetBinContent(ibin+1),2))) #+pow(hsyst_escale2.GetBinContent(ibin+1),2)))

hsyst_escale.SetBinContent(38,1.14)
hsyst_escale.SetBinContent(39,2.01)
hsyst_escale.SetBinContent(40,3.23)
hsyst_escale.SetBinContent(41,4.78)

#
#get total xsection systematics as they are filled !
#
print "Doing total uncertainty"
f = TFile("../Inputs/sys/xSec_results_1D.root")
mxsec = f.Get("XSec")
mstat = f.Get("XSecErr")
msyst = f.Get("XSecSystErr")
thisx = TH1D('thisx','thisx',41,array('d',xAxis))
for ibin in range(thisx.GetNbinsX()):
    #this is the current way
    thisx.SetBinContent(ibin+1,sqrt(pow(hcollCS_syst.GetBinContent(ibin+1),2)+pow(heffMC_syst.GetBinContent(ibin+1),2)+pow(hsyst_escale.GetBinContent(ibin+1),2)+pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(heffcorr_err.GetBinContent(ibin+1),2)+pow(hsyst_bkg.GetBinContent(ibin+1),2)+pow(hstat_full.GetBinContent(ibin+1),2)+pow(hsyst_acc.GetBinContent(ibin+1),2)+pow(hsyst_FSR.GetBinContent(ibin+1),2)))
    #print heffMC_syst.GetBinContent(ibin+1)," ",hsyst_escale.GetBinContent(ibin+1)," ",hsyst_unf.GetBinContent(ibin+1)," ",heffcorr_err.GetBinContent(ibin+1)," ",hsyst_bkg.GetBinContent(ibin+1)," ",hstat_full.GetBinContent(ibin+1)," ",hsyst_acc.GetBinContent(ibin+1)," ",hsyst_FSR.GetBinContent(ibin+1)
    thisx.SetBinError(ibin+1,0.)
    #print thisx.GetBinContent(ibin+1)
fout = TFile("uncertaintiesEE_1D.root","recreate")
fout.cd()
thisx.Write()
fout.Close()

#
#produce a validation plot
#
'''
syst_stack = THStack("ustack","ustack")
hsyst_bkg.SetLineColor(2)
hsyst_bkg.SetFillColor(2)
syst_stack.Add(hsyst_bkg)
hsyst_acc.SetLineColor(3)
hsyst_acc.SetFillColor(3)
syst_stack.Add(hsyst_acc)
hmod_acc.SetLineColor(4)
hmod_acc.SetFillColor(4)
syst_stack.Add(hmod_acc)
hpdf_acc.SetLineColor(5)
hpdf_acc.SetFillColor(5)
syst_stack.Add(hpdf_acc) 
heffcorr_err.SetLineColor(6)
heffcorr_err.SetFillColor(6)
syst_stack.Add(heffcorr_err) 
hsyst_unf.SetLineColor(7)
hsyst_unf.SetFillColor(7)
syst_stack.Add(hsyst_unf)
hsyst_escale.SetLineColor(8)
hsyst_escale.SetFillColor(8)
syst_stack.Add(hsyst_escale)
c= TCanvas("ucanvas","ucanvas")
c.cd()
c.SetLogy()
#syst_stack.GetYaxis().SetRangeUser(0,100.)
#syst_stack.GetXaxis().SetTitle("m [GeV]")
#syst_stack.GetYaxis().SetTitle("Uncertainty [%]")
syst_stack.Draw()
leg= TLegend(0.17,0.20,0.35,0.40) 
leg.SetTextFont(72)
leg.SetTextSize(0.060)
leg.SetBorderSize(0)
leg.SetFillColor(0)
leg.AddEntry(hsyst_bkg,"Bkg","f")
leg.AddEntry(hsyst_acc,"Acc","f")
leg.AddEntry(hmod_acc,"Modeling","f")
leg.AddEntry(hpdf_acc,"PDF","f")
leg.AddEntry(heffcorr_err,"Eff. #rho","f")
leg.AddEntry(hsyst_unf,"Det.resol.","f")
leg.AddEntry(hsyst_escale,"E-scale","f")
leg.Draw("same")
c.SaveAs("uncertainty1D_EE.png")
'''

printHistoIlyaEE(hstat_full, heffcorr_err, hsyst_unf, hsyst_bkg, hsyst_escale, thisx, hsyst_acc, hpdf_acc, hmod_acc, hsyst_FSR, heffMC_syst, hcollCS_syst) #, hcollcs_err)

#print out the cross section
peak = 1142.6
f = TFile("../Outputs/absex_full_PI_Bayesian.root")
thisx = f.Get("hxsec")
for ibin in range(thisx.GetNbinsX()):
   thisx.SetBinContent(ibin+1,thisx.GetBinContent(ibin+1)/peak)
   thisx.SetBinError(ibin+1,thisx.GetBinError(ibin+1)/peak)
   #print thisx.GetBinError(ibin+1), ",",

#prints the absolute cross section in one column
#printHisto(thisx,0)

#####systematics table
#printHistoStoyanEE(hstat_full, hsyst_escale, heffcorr_err, hsyst_unf, hsyst_bkg, hsyst_FSR, thisx, 0)

#save uncertainties for the double ratio
funcert = ROOT.TFile('../Inputs/sys/electronSystAll_1D.root','recreate')
funcert.cd()
hstat_full.Write('stat')
hsyst_escale.Write('escale_syst')
heffcorr_err.Write('effcorr_syst')
heffMC_syst.Write('effMC_syst')
hsyst_unf.Write('unf_syst')
hsyst_bkg.Write('bkg_syst')
hsyst_FSR.Write('FSR_syst')
hsyst_acc.Write('acc_syst')
hpdf_acc.Write('pdf_syst')
hmod_acc.Write('mod_syst')
hcollCS_syst.Write('collCS')
funcert.Close()
