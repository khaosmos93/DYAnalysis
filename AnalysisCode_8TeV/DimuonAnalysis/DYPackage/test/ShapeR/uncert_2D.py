#!/usr/bin/env python

from ROOT import *
from math import sqrt, pow
from array import array

import sys, os
from rshape_tools import *


#
#get efficiency correction systematics
#
print "Doing eff corr syst uncertainty"
feffcorr_dd = TFile("../Inputs/effcorr/effCorrAllsplit_2D.root")
heffcorr_err0 = feffcorr_dd.Get("rap")
heffcorr_err = TH1D('hsyst_effc','hsyst_effc',132,0,132)
#FIXME 7x24
for ibin in range(heffcorr_err.GetNbinsX()):
    heffcorr_err.SetBinContent(ibin+1,100.*heffcorr_err0.GetBinError(ibin+1)/heffcorr_err0.GetBinContent(ibin+1))
    heffcorr_err.SetBinError(ibin+1,0.0)
    if ibin < 24: heffcorr_err.SetBinContent(ibin+1,heffcorr_err.GetBinContent(ibin+1)*0.99);
    #print ibin, " ", 100.*heffcorr_err0.GetBinError(ibin+1)/heffcorr_err0.GetBinContent(ibin+1)

#
#get PDF uncertainty on acceptance (same as dimuons)
#
print "Doing PDF uncertainty"
f_acc_pdf = ROOT.TFile('../Inputs/sys/pdf_syst2D_7TeV.root')
hsys1 = f_acc_pdf.Get('hslice1')
hsys2 = f_acc_pdf.Get('hslice2')
hsys3 = f_acc_pdf.Get('hslice3')
hsys4 = f_acc_pdf.Get('hslice4')
hsys5 = f_acc_pdf.Get('hslice5')
hsys6 = f_acc_pdf.Get('hslice6')
syst_list = [hsys1,hsys2,hsys3,hsys4,hsys5,hsys6]
hacc_pdf = TH1D('hacc_pdf','hacc_pdf',132,0,132)
for ih in range(len(syst_list)):
    for ibin in range(syst_list[ih].GetNbinsX()):
         #print ih, " ", ibin, " ", 100.*syst_list[ih].GetBinError(ibin+1), " ", ibin+1+ih*syst_list[ih].GetNbinsX()
         hacc_pdf.SetBinContent(ibin+1+ih*24,100.*syst_list[ih].GetBinError(ibin+1))
         hacc_pdf.SetBinError(ibin+1+ih*24,0.0)

#for ibin in range(hacc_pdf.GetNbinsX()):
#    print hacc_pdf.GetBinContent(ibin+1)

#
#get unfolding systematics
#
print "Doing unfolding uncertainty"
fsyst_unf = TFile("../shapeR2D/sys/unfold_corrRun2011AB_sys_new.root")
hsyst_unf0 = fsyst_unf.Get("hunfold_sys")

fsyst_unf2 = TFile("../Inputs/sys/muon_unf_stat_2D.root")
hstat_unf = fsyst_unf2.Get("muon_unf_stat")
hsyst_unf = TH1D('hsyst_unf','hsyst_unf',132,0,132)
for ibin in range(hsyst_unf.GetNbinsX()):
    hsyst_unf.SetBinContent(ibin+1,sqrt(pow(4./5.*hsyst_unf0.GetBinContent(ibin+1)*100.,2)+pow(hstat_unf.GetBinContent(ibin+1),2)))
    hsyst_unf.SetBinError(ibin+1,0.)
    #print hsyst_unf.GetBinContent(ibin+1)

#
#get statistical uncertainty
#
print "Doing stat uncertainty"
fstat = TFile('../Inputs/unfold/unfoldData_HLT_Double_2D.root')
hstat_full0 = fstat.Get('hunfolded')
hstat_full = TH1D('hstat_full','hstat_full',132,0,132)
for ibin in range(hstat_full.GetNbinsX()):
    hstat_full.SetBinContent(ibin+1,100.*sqrt(1./hstat_full0.GetBinContent(ibin+1)))
    hstat_full.SetBinError(ibin+1,0.0)
    #print ibin, " ", 100.*sqrt(1./hstat_full0.GetBinContent(ibin+1))

#
#get background systematic uncertainty
#
print "Doing background syst uncertainty"
fsyst_bkg = TFile("../Inputs/sys/bkg_syst2013_2D_wQCD.root")
#v 132
hsyst_bkg0 = fsyst_bkg.Get("hsyst2D")
hsyst_bkg = TH1D('hsyst_bkg','hsyst_bkg',132,0,132)
for ibin in range(hsyst_bkg0.GetNbinsX()):
    hsyst_bkg.SetBinContent(ibin+1,100.*hsyst_bkg0.GetBinContent(ibin+1))
    hsyst_bkg.SetBinError(ibin+1,0.)
    #print ibin, " ", 100.*hsyst_bkg0.GetBinContent(ibin+1)


#
#Efficiency MC truth uncertainty
#
feffMC = TFile("../Inputs/acc/out2.root")
heffMC = feffMC.Get("eff_postFSRcorr")
hsyst_effMC = feffMC.Get("eff_postFSRcorr")
for ibin in range(heffMC.GetNbinsX()):
    hsyst_effMC.SetBinContent(ibin+1,100.*heffMC.GetBinError(ibin+1)/heffMC.GetBinContent(ibin+1))
    hsyst_effMC.SetBinError(ibin+1,0.)


#
#get FSR systematic uncertainty
#
print "Doing FSR syst uncertainty"
fsyst_FSR = TFile("../Inputs/sys/fsrSysts_SanG_132_8TeV.root")
#v 132
hsyst_FSR = fsyst_FSR.Get("rap")

#
#get total xsection systematics as they are filled !
#
print "Doing total uncertainty"
f = TFile("../Outputs/absex_DET2D_PI.root")
peak = 1. #570.5 
thisx = f.Get('hxsec')
this_err = f.Get('hxsec')
for ibin in range(thisx.GetNbinsX()):
    #thisx.SetBinContent(ibin+1,thisx.GetBinError(ibin+1)/thisx.GetBinContent(ibin+1)*100.)
    #alternative
    this_err.SetBinContent(ibin+1,sqrt(pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(heffcorr_err.GetBinContent(ibin+1),2)+pow(hsyst_bkg.GetBinContent(ibin+1),2)+pow(hstat_full.GetBinContent(ibin+1),2)+pow(hsyst_FSR.GetBinContent(ibin+1),2)))
    this_err.SetBinError(ibin+1,0.)
    thisx.SetBinContent(ibin+1,thisx.GetBinContent(ibin+1)/peak)
    thisx.SetBinError(ibin+1,this_err.GetBinContent(ibin+1)*thisx.GetBinContent(ibin+1))
    #print ibin," ",thisx.GetBinContent(ibin+1), " ", sqrt(pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(heffcorr_err.GetBinContent(ibin+1),2)+pow(hsyst_bkg.GetBinContent(ibin+1),2)+pow(hstat_full.GetBinContent(ibin+1),2)+pow(hsyst_FSR.GetBinContent(ibin+1),2))
fout = TFile("uncertainties_2D.root","recreate")
fout.cd()
this_err.Write("uncert2D")
fout.Close()

#printHistoIlya_2D(hstat_full, heffcorr_err, hsyst_unf, hsyst_bkg, this_err, hsyst_FSR, hacc_pdf,hsyst_effMC,hstat_unf)

funcert = TFile('../Inputs/sys/muonSystAll_2D.root','recreate')
funcert.cd()
hstat_full.Write('stat')
heffcorr_err.Write('effcorr_syst')
hsyst_unf.Write('unf_syst')
hsyst_bkg.Write('bkg_syst')
hsyst_FSR.Write('FSR_syst')
hacc_pdf.Write('pdf_syst')
funcert.Close()


printHistoStoyan_2D(hstat_full, heffcorr_err, hsyst_unf, hsyst_bkg, hsyst_FSR, this_err)
## rap. bin & stat. error & efficiency correction & unfolding & background & FSR & total sys. err
#0-0.1  1.2  6.2  0.29  0.57  0.76  6.3

#printHisto_2D(thisx)
# shape r normalized to Z peak: 
#shaper[0][0] 0 0.0339971 0.00220474
