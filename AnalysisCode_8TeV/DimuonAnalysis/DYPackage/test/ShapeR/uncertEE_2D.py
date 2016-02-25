#!/usr/bin/env python

from ROOT import *
from math import sqrt, pow
from array import array

import sys, os
from rshape_tools import *

def bambuConverter7_24(h,m_num,m_den):
    for ibin_rows in range(1,7):
        for ibin_cols in range(24):
            if ibin_rows == 6 and ibin_cols > 11: continue
            #FIXME what about the last bin?
            h.SetBinContent(ibin_cols+1+(ibin_rows-1)*24,100.*sqrt(m_num(ibin_rows,ibin_cols))/m_den(ibin_rows,ibin_cols))
            h.SetBinError(ibin_cols+1+(ibin_rows-1)*24,0.)
            #print h.GetBinContent(ibin_cols+1+(ibin_rows-1)*24)

#FIXME accept new inouts from Andrius
fAndrius = TFile('../Inputs/sys/table_2D_frac_nBayes1.root')

#
#New source: MC efficiency and pile up 
#
heffMC_syst1 = fAndrius.Get('eff_rnd_err')
heffMC_syst = TH1D("effMC_syst","effMC_syst",132,0,132)
for ix in range(heffMC_syst1.GetNbinsX()):
    for iy in range(heffMC_syst1.GetNbinsY()):
        heffMC_syst.SetBinContent(iy+1+ix*24,100.*sqrt(pow(heffMC_syst1.GetBinContent(ix+1,iy+1),2)))
        heffMC_syst.SetBinError(iy+1+ix*24,0.)

#
#New source: pile up
#
hcollCS_syst1 = fAndrius.Get('pileup_err')
hcollCS_syst = TH1D("collCS_syst","collCS_syst",132,0,132)
for ix in range(hcollCS_syst1.GetNbinsX()):
    for iy in range(hcollCS_syst1.GetNbinsY()):
        hcollCS_syst.SetBinContent(iy+1+ix*24,100.*sqrt(pow(hcollCS_syst1.GetBinContent(ix+1,iy+1),2)))
        hcollCS_syst.SetBinError(iy+1+ix*24,0.)


#
#get statistical uncertainty
#
print "Doing stat uncertainty"
fstat1 = TFile('../Inputs/sys/yields_bg-subtracted2D.root')
mstat_full_den = fstat1.Get('YieldsSignal')
fstat2 = TFile('../Inputs/sys/yields2D.root')
mstat_full_num = fstat2.Get('yields_data')

hstat_full = TH1D('hstat_full','hstat_full',132,0,132)
bambuConverter7_24(hstat_full,mstat_full_num, mstat_full_den)

#
#get FSR systematic uncertainty
#
print "Doing FSR syst uncertainty"
#FSR systematics
hsyst_FSR1 = fAndrius.Get("fsr_rnd_err")
hsyst_FSR2 = fAndrius.Get("fsr_model_err")
hsyst_FSR = TH1D('syst_FSR','syst_FSR',132,0,132)
for ix in range(hsyst_FSR1.GetNbinsX()):
    for iy in range(hsyst_FSR1.GetNbinsY()):
        hsyst_FSR.SetBinContent(iy+1+ix*24,100.*sqrt(pow(hsyst_FSR1.GetBinContent(ix+1,iy+1),2)+pow(hsyst_FSR2.GetBinContent(ix+1,iy+1),2)))
        hsyst_FSR.SetBinError(iy+1+ix*24,0.)

#
#get background systematic uncertainty
#
print "Doing background syst uncertainty"
hsyst_bkg0 = fAndrius.Get("bkgr_est_err")
hsyst_bkg = TH1D("syst_bkg","syst_bkg",132,0,132)
for ix in range(hsyst_bkg0.GetNbinsX()):
    for iy in range(hsyst_bkg0.GetNbinsY()):
        hsyst_bkg.SetBinContent(iy+1+ix*24,100.*hsyst_bkg0.GetBinContent(ix+1,iy+1))
        hsyst_bkg.SetBinError(iy+1+ix*24,0.)

#
#get efficiency correction systematics
#
print "Doing eff corr syst uncertainty"
heffcorr_err0 = fAndrius.Get('rho_err')
heffcorr_err = TH1D("effcorr_err","effcorr_err",132,0,132)
for ix in range(heffcorr_err0.GetNbinsX()):
    for iy in range(heffcorr_err0.GetNbinsY()):
        heffcorr_err.SetBinContent(iy+1+ix*24,100.*heffcorr_err0.GetBinContent(ix+1,iy+1))
        heffcorr_err.SetBinError(iy+1+ix*24,0.)

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
hsyst_unf0 = fAndrius.Get("det_resolution_err")
hsyst_unf = TH1D("syst_unf","syst_unf",132,0,132)
for ix in range(hsyst_unf0.GetNbinsX()):
    for iy in range(hsyst_unf0.GetNbinsY()):
        hsyst_unf.SetBinContent(iy+1+ix*24,100.*hsyst_unf0.GetBinContent(ix+1,iy+1))
        hsyst_unf.SetBinError(iy+1+ix*24,0.)


#save stat uncertainty on unfolding for muons
#fout = ROOT.TFile("muon_unf_stat_2D.root","recreate")
##scaling numerator: ok
#hsyst_unf_mu = TH1D('muon_unf_stat','muon_unf_stat',132,0,132)
##scaling denominator ok
#mele_yield = fstat2.Get('yields_data')
#hsyst_unf_ele = TH1D('ele_unf_stat','ele_unf_stat',132,0,132)
#for ibin_rows in range(1,7):
#    for ibin_cols in range(24):
#        if ibin_rows == 6 and ibin_cols > 11: continue
#        #FIXME what about the last bin?
#        hsyst_unf_ele.SetBinContent(ibin_cols+1+(ibin_rows-1)*24,mele_yield(ibin_rows,ibin_cols))
#        hsyst_unf_ele.SetBinError(ibin_cols+1+(ibin_rows-1)*24,0.)
#        #print "XXX ", ibin_cols+1+(ibin_rows-1)*24, " ", hsyst_unf_ele.GetBinContent(ibin_cols+1+(ibin_rows-1)*24)
#
#fraw = ROOT.TFile("../Inputs/rawYield/DYspectrum_Rap_uncorr_2013_tmp_TRMNov.root")
#hmu_yield = fraw.Get("hdata")
#for ibin in range(hsyst_unf_mu.GetNbinsX()):
#    #print ibin, " ",  
#    hsyst_unf_mu.SetBinContent(ibin+1,hsyst_unf.GetBinContent(ibin+1)/sqrt(hmu_yield.GetBinContent(ibin+1)/hsyst_unf_ele.GetBinContent(ibin+1)))
#    hsyst_unf_mu.SetBinError(ibin+1,0.)
#    print ibin, " XX ", hsyst_unf_mu.GetBinContent(ibin+1)
#
#fout.cd()
#hsyst_unf_mu.Write("muon_unf_stat")
#fout.Close()


#
#get escale systematics (2011)
#
print "Doing escale uncertainty"
hsyst_escale1 = fAndrius.Get("escale_err")
#hsyst_escale2 = fAndrius.Get("unf_escale_res")
hsyst_escale = TH1D('syst_escale','syst_escale',132,0,132)
for ix in range(hsyst_escale1.GetNbinsX()):
    for iy in range(hsyst_escale1.GetNbinsY()):
        hsyst_escale.SetBinContent(iy+1+ix*24,100.*sqrt(pow(hsyst_escale1.GetBinContent(ibin+1),2))) #+pow(hsyst_escale2.GetBinContent(ibin+1),2)))
        hsyst_escale.SetBinError(iy+1+ix*24,0.)

#
#get total xsection systematics as they are filled !
#
print "Doing total uncertainty"
f = TFile("../Outputs/absex_DET2D_PI_Bayesian.root")
thisx = f.Get('hxsec')
this_err = thisx.Clone()
for ibin in range(thisx.GetNbinsX()):
    #alternative
    this_err.SetBinContent(ibin+1,sqrt(pow(hcollCS_syst.GetBinContent(ibin+1),2)+pow(heffMC_syst.GetBinContent(ibin+1),2)+pow(hsyst_escale.GetBinContent(ibin+1),2)+pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(heffcorr_err.GetBinContent(ibin+1),2)+pow(hsyst_bkg.GetBinContent(ibin+1),2)+pow(hstat_full.GetBinContent(ibin+1),2)+pow(hsyst_FSR.GetBinContent(ibin+1),2)))
    #print ibin+1," ",heffMC_syst.GetBinContent(ibin+1)," ",hsyst_escale.GetBinContent(ibin+1)," ",hsyst_unf.GetBinContent(ibin+1)," ",heffcorr_err.GetBinContent(ibin+1)," ",hsyst_bkg.GetBinContent(ibin+1)," ",hstat_full.GetBinContent(ibin+1)," ",hsyst_FSR.GetBinContent(ibin+1)
    this_err.SetBinError(ibin+1,0.)
    thisx.SetBinError(ibin+1,this_err.GetBinContent(ibin+1))
    #print ibin," ",thisx.GetBinContent(ibin+1)
fout = TFile("uncertaintiesEE_2D.root","recreate")
fout.cd()
this_err.Write()
fout.Close()

printHistoIlyaEE_2D(hstat_full, heffcorr_err, hsyst_unf, hsyst_bkg, hsyst_escale, thisx, hacc_pdf,hsyst_FSR, heffMC_syst,hcollCS_syst)

#for ibin in range(hstat_full.GetNbinsX()):
#    print ibin+1, ' ', hstat_full.GetBinContent(ibin+1)

#####systematics table
#printHistoStoyanEE_2D(hstat_full, hsyst_escale, heffcorr_err, hsyst_unf, hsyst_bkg, hsyst_FSR, thisx)

#printHisto_2D(thisx)
