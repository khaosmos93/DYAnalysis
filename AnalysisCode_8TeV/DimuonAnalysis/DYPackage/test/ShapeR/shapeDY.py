#!/usr/bin/env python

import ROOT
from math import sqrt, pow

import sys, os
from rshape_tools import *

from optparse import OptionParser
parser = OptionParser(usage="usage: %prog --x [r-shape type] --p [print out type] --u [what uncertainty] --a [absolute cross section]")

#dis is how you add options
parser.add_option("--x", dest="xsectype", help="r-shape type: 1. preFSR-full acceptance, 2. postFSR - full acceptance, 3. preFSR - detector acceptance, 4. postFSR - detector acceptance", default="1")
parser.add_option("--p", dest="pprint", help="Print out mode: 0-simple, 1-for latex tables", default="0")
parser.add_option("--u", dest="uncert_mode", help="Uncertainty mode: 0-total, 1-systematics only, 2-statistics only", default="0")
parser.add_option("--a", dest="absolute", help="Normalization mode: 1-absolute (2013 style), 0-r-shape", default="1")
    
(options, args) = parser.parse_args()

components = "xsectype","pprint","uncert_mode","absolute"
for component in components:
    exec("%s = int(options.%s)" % (component, component))

#FIXME do you need correction
USE_CORR = "corr" #"corr"
USE_BBBFSR = False

nBins = 40
mBins = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500]

#FILL NECESSARY HISTOGRAM
#for the cross section calculation, we need following quantities
#1) the unfolded background subtracted yield with associated errors
#2) MC truth efficiency
#3) DD efficiency correction factors
#4) acceptance-efficiency
#5) fsr correction factors

#we will have twoi histograms for each constituent (statistical and systematic)
#which will store the error in percent (but not multiplied by 100)

#GET UNFOLDED YIELD
f_unfolded_yield = ROOT.TFile('../Inputs/unfold/unfoldData_HLT_Double.root')
hyield = f_unfolded_yield.Get('hunfolded')
#statistical uncertainty from unfolding is sqrt of yield poissonian error (formula 20)
hstat_unf = hyield.Clone()
for ibin in range(hyield.GetNbinsX()):
    hstat_unf.SetBinContent(ibin+1,sqrt(hyield.GetBinContent(ibin+1)))
    hstat_unf.SetBinError(ibin+1,0.0)

#systematic uncertainty on unfolding is provided by Rochester (first term in formula 22)
#there are various components:
#1) momentum scale correction systematics, the error from the momentum scale correction which is determined as a difference
#between the central value of the unfolded distribution and the shifted value of the
#unfolded distribution (note: previously this uncertainty was large, right now it is just the uncertainty on correction estimation)
#2)the error on the momentum scale correction estimation method is assigned. The
#difference between the momentum scale correction as estimated using the Rochester
#technique described in the section 5 and muscle fit are taken.
f_unfold_syst = ROOT.TFile('../Inputs/sys/unfold_sys.root')
hsyst_unf = f_unfold_syst.Get('hunfold_sys')
noZerosInHist(hsyst_unf)
#systematic uncertainty on the background estimation is assigned separately#second term in formula (22), explained in formula (26)
#we have two cases: data driven estimation and MC based estimation
#For data driven estimation we have two following sources
#1) Poissonian statistical uncertainty of estimated QCD backgrounds,
#2) (uncertainty on the method essentially) the deviation of the data driven prediction from the arithmetic mean (Naverage) of the
#data driven prediction and corresponding MC expectation.
#for the case of MC based estimation we have two following sources
#1)Poissonian statistical uncertainty from the MC sample (which is treated as systematic
#2)systematic uncertainty due to the knowledge on the theoretical cross section
#this corresponds to the second term in (22) and more detailed explanation in formula (27)
#3)Systematic error on unfolding. Reflects the robustness with respect to statistical fluctuations, estimated using the ensemble test
#4)Unfolding error on the method, estimated by comparing the matrix inversion technique and SVD
f_unfold_met_syst = ROOT.TFile('../Inputs/sys/unfolding_met_sys.root')
hsyst_met_unf = f_unfold_met_syst.Get('hden')

#FSR UNFOLDING
f_FSRunfolded_yield = ROOT.TFile('../Inputs/FSR/FSRunfoldData_HLT_Double_tmp.root')
hFSRyield = f_FSRunfolded_yield.Get('hunfolded')

#Additional FSR corrections
f_FSR_extraCorrections = ROOT.TFile('../Inputs/FSR/addMoreCorrinMC_tmp.root')
hFSRextra2 = f_FSR_extraCorrections.Get('heff1D_2'+USE_CORR) #apply extra bin-by-bin corrections after unfolding 
#FSR systematics from San Geun
f_fsr_SanG_syst = ROOT.TFile('../Inputs/sys/fsrSysts_SanG.root')
hsyst_FSR = f_fsr_SanG_syst.Get('fsr_syst')

#apply extra bin-by-bin corrections after unfolding 
for ibin in range(hFSRyield.GetNbinsX()):
#    #just set the contents - gives correct stat error after all
    hFSRyield.SetBinContent(ibin+1,hFSRyield.GetBinContent(ibin+1)/hFSRextra2.GetBinContent(ibin+1)) #/hFSRextra1.GetBinContent(ibin+1))

### Debug case if use BBBFSR
f_FSRunfolded_yield_BBB = ROOT.TFile('../Inputs/unfold/unfoldData_HLT_Double.root')
hFSRyield_BBB = f_FSRunfolded_yield_BBB.Get('hunfolded')
f_BBB_FSR = ROOT.TFile('../Inputs/FSR/BinByBin_Saved.root')
BBB_FSR=f_BBB_FSR.Get("ratio_corr")
#if USE_CORR == "corr": USE_CORR = "_corr"
#num = f_BBB_FSR.Get("hpostFSR"+USE_CORR)
#den = f_BBB_FSR.Get("hpreFSR"+USE_CORR)
#if USE_CORR == "_corr": USE_CORR = "corr"
#BBB_FSR = num.Clone()
#BBB_FSR.Divide(num,den)

print BBB_FSR.GetBinError(1)/BBB_FSR.GetBinContent(1)
if USE_BBBFSR: 
    hFSRyield = hFSRyield_BBB
    #APPLY BBBFSR
    for ibin in range(hFSRyield.GetNbinsX()):
        hFSRyield.SetBinContent(ibin+1,hFSRyield.GetBinContent(ibin+1)/BBB_FSR.GetBinContent(ibin+1))

#statistical uncertainty from unfolding is sqrt of yield poissonian error (formula 20)
hstat_FSRunf = hFSRyield.Clone()
for ibin in range(hFSRyield.GetNbinsX()):
    hstat_FSRunf.SetBinContent(ibin+1,hFSRyield.GetBinError(ibin+1)/hFSRyield.GetBinContent(ibin+1))
    hstat_FSRunf.SetBinError(ibin+1,0.0)

#this will have full statistical uncertainty
hstat_full = hstat_unf.Clone()
#Add up to full stat uncertainty
for ibin in range(hstat_full.GetNbinsX()):
    if (xsectype is 2) or (xsectype is 4): 
        hstat_full.SetBinContent(ibin+1,sqrt(pow(hstat_unf.GetBinContent(ibin+1),2)))
        hstat_full.SetBinError(ibin+1,0.0)
    if (xsectype is 1) or (xsectype is 3):
        hstat_full.SetBinContent(ibin+1,sqrt(pow(hstat_FSRunf.GetBinContent(ibin+1),2)))
        hstat_full.SetBinError(ibin+1,0.0)
    
#BACKGROUND
f_bkg_err = ROOT.TFile('../Inputs/sys/bkg_syst2013_1D.root')
hsyst_bkg = f_bkg_err.Get('hsyst1D')
#FIXME temporary QCD uncertainty estimate: 1% in first 7 bins
for ibin in range(7):
    hsyst_bkg.SetBinError(ibin+1,sqrt(pow(hsyst_bkg.GetBinContent(ibin+1),2)+pow(0.01,2)))

for ibin in range(hyield.GetNbinsX()):
    hyield.SetBinError(ibin+1,hyield.GetBinContent(ibin+1)*sqrt(pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(hsyst_bkg.GetBinContent(ibin+1),2)))
    hFSRyield.SetBinError(ibin+1,hFSRyield.GetBinContent(ibin+1)*sqrt(pow(hsyst_FSR.GetBinContent(ibin+1)/100.,2)+pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(hsyst_bkg.GetBinContent(ibin+1),2)))

#GET ACCEPTANCE-EFFICIENCY, EFFICIENCY AND FSR
f_acceff = ROOT.TFile('../Inputs/acc/out1.root') 
acceff_postFSRcorr = f_acceff.Get('acceff_postFSR'+USE_CORR)
acc_preFSRcorr = f_acceff.Get('acc_preFSR'+USE_CORR)
acceff_postFSR = f_acceff.Get('acceff_postFSR')
acc_preFSR = f_acceff.Get('acc_preFSR')
eff_postFSR = f_acceff.Get('eff_postFSR'+USE_CORR) #FIXME 1% effect at low mass

#Acceptance uncertainties. These are purely systematic (term 3 in (22) for acc-eff, other sources of acceptance uncertainty: modeling 
#and PDF are treated specially)
#1)The first contribution (from limited MC statistics) is determined with Clopper-Pearson uncertainties for the ratios of Poisson means
#see: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/SUSYBSMAnalysis/Zprime2muAnalysis/python/roottools.py?revision=1.29&view=markup
#clopper_pearson function 
#2) PDF uncertainty is determined using the LHAGLUE, provided by Dimitri (Florida)
#3) Modeling uncertainty on acceptance (formula (25)). Tha acceptance uncertainty due to the effects of higher order QCD and EWK corrections (the
#modeling uncertainty) is calculated by comparing the pre-FSR FEWZ corrected acceptances
#calculated with FEWZ and POWHEG
f_acc_mod = ROOT.TFile('../Inputs/sys/modeling2013_1D_smoothed.root')
hmod_acc = f_acc_mod.Get('invm_FEWZ')
f_PUsys = ROOT.TFile('../Inputs/sys/pileup2013_forABS.root')
hPUsys = f_PUsys.Get('pileup_sys')
for ibin in range(hmod_acc.GetNbinsX()):
    #print "Before ", hmod_acc.GetBinContent(ibin+1), " After ", sqrt(pow(hmod_acc.GetBinContent(ibin+1),2)+pow(hPUsys.GetBinContent(ibin+1),2))
    hmod_acc.SetBinContent(ibin+1,sqrt(pow(hmod_acc.GetBinContent(ibin+1),2)+pow(hPUsys.GetBinContent(ibin+1),2)))

hsyst_acc = hmod_acc.Clone()
f_acc_pdf = ROOT.TFile('../Inputs/sys/pdfu.root')
hpdf_acc = f_acc_pdf.Get('pdfu')

#FSR uncertainty is purely statistical, FSR systematics is determined also using the Clopper-Pearson uncertainties for the ratios of Poisson means
#same reference as acceptance 1)
hsyst_fsr = hmod_acc.Clone()
for ibin in range(hyield.GetNbinsX()):
    acceff_postFSRcorr.SetBinError(ibin+1,acceff_postFSRcorr.GetBinContent(ibin+1)*sqrt(pow(acceff_postFSRcorr.GetBinError(ibin+1)/acceff_postFSRcorr.GetBinContent(ibin+1),2)+pow(hmod_acc.GetBinContent(ibin+1),2)+pow(hpdf_acc.GetBinContent(ibin+1)/100.,2)))
    acc_preFSRcorr.SetBinError(ibin+1,acc_preFSRcorr.GetBinContent(ibin+1)*sqrt(pow(acc_preFSRcorr.GetBinError(ibin+1)/acc_preFSRcorr.GetBinContent(ibin+1),2)+pow(hmod_acc.GetBinContent(ibin+1),2)+pow(hpdf_acc.GetBinContent(ibin+1)/100.,2)))
    hsyst_acc.SetBinContent(ibin+1,acc_preFSRcorr.GetBinError(ibin+1)/acc_preFSRcorr.GetBinContent(ibin+1))
    hsyst_acc.SetBinError(ibin+1,0.0)

#GET EFFICIENCY CORRECTIONS DD
#initially they are in three different files
f_isocorr_dd = ROOT.TFile('../Inputs/effcorr/1D/correction_isoinvm_split1D.root') #/out_iso_1.root')
isocorr_dd = f_isocorr_dd.Get('invm')
f_recoidcorr_dd = ROOT.TFile('../Inputs/effcorr/1D/correction_recoidinvm_split1D.root') #/out_recoid_1.root')
recoidcorr_dd = f_recoidcorr_dd.Get('invm')
f_trigcorr_dd = ROOT.TFile('../Inputs/effcorr/1D/correction_triginvm_split1D.root') #out_trigger_1.root')
trigcorr_dd = f_trigcorr_dd.Get('invm')
#FIXME add the DZ filter
fdz = ROOT.TFile("../Inputs/effcorr/DZ_SF.root")
hdz = fdz.Get("invm")
for ibin in range(hdz.GetNbinsX()):
    trigcorr_dd.SetBinContent(ibin+1,trigcorr_dd.GetBinContent(ibin+1)*hdz.GetBinContent(ibin+1))

#fill in one
#There are various sources of efficiency/efficiency correction uncertainties.
#MC truth efficiency has only systematic uncertainty, efficiency corrections have both stat and syst
#Systematic uncertainty on MC truth efficiency is calculated using the Clopper-Pearson uncertainties for the ratios of Poisson means
#and is a part of acceptance-efficiency systematics if the r shape is calculated in the full phase sapce (otherwise is independent)
#Uncertainty on the correction (currently we do not split statistics and systematics for this one), given by term 4 in (22), 
#explained in (22) partially, has the contribution from the systematic uncertainty related to the difference in efficiency corrections extracted
#using fitting (ML fit) and counting,
#the line shape modeling uncertainty, accounting for the imperfect fit models used,
#the uncertainty due to binning of the look-up tabel (the last two source a partially overlapped)
heffcorr_dd = trigcorr_dd.Clone()
hstat_effcorr = trigcorr_dd.Clone()
#f_eff_syst = ROOT.TFile('../Inputs/sys/eff_syst2012.root')
#hsyst_effcorr = f_eff_syst.Get('2012_eff')
hsyst_effcorr = trigcorr_dd.Clone()
for ibin in range (isocorr_dd.GetNbinsX()):
    heffcorr_dd.SetBinContent(ibin+1,isocorr_dd.GetBinContent(ibin+1)*recoidcorr_dd.GetBinContent(ibin+1)*trigcorr_dd.GetBinContent(ibin+1))
    #FIXME 0.01 
    heffcorr_dd.SetBinError(ibin+1,heffcorr_dd.GetBinContent(ibin+1)*sqrt(pow(isocorr_dd.GetBinError(ibin+1)/isocorr_dd.GetBinContent(ibin+1),2)+pow(recoidcorr_dd.GetBinError(ibin+1)/recoidcorr_dd.GetBinContent(ibin+1),2)+pow(trigcorr_dd.GetBinError(ibin+1)/trigcorr_dd.GetBinContent(ibin+1),2)))
    frac_stat = 0. #0.29
    hstat_effcorr.SetBinContent(ibin+1,frac_stat*heffcorr_dd.GetBinError(ibin+1))
    hstat_effcorr.SetBinError(ibin+1,0.)
    #keep only systematic uncertainty
    heffcorr_dd.SetBinError(ibin+1,sqrt((1-pow(frac_stat,2)))*heffcorr_dd.GetBinError(ibin+1))
    #hsyst_effcorr.SetBinError(ibin+1,0.)

#Add up to full uncertainty
#for ibin in range(hstat_full.GetNbinsX()):
#    hstat_full.SetBinContent(ibin+1,sqrt(pow(hstat_full.GetBinContent(ibin+1),2)))
#    hstat_full.SetBinError(ibin+1,0.0)

#saveHisto(heffcorr_dd)
#printHisto(heffcorr_dd)

#get the cross section and normalization
hxsec2 = getXsection(hyield,acceff_postFSRcorr,acc_preFSRcorr,eff_postFSR,heffcorr_dd,hstat_full,2,uncert_mode,USE_BBBFSR)
hxsec4 = getXsection(hyield,acceff_postFSRcorr,acc_preFSRcorr,eff_postFSR,heffcorr_dd,hstat_full,4,uncert_mode,USE_BBBFSR)
hxsec1 = getXsection(hFSRyield,acceff_postFSRcorr,acc_preFSRcorr,eff_postFSR,heffcorr_dd,hstat_full,1,uncert_mode,USE_BBBFSR)
hxsec3 = getXsection(hFSRyield,acceff_postFSRcorr,acc_preFSRcorr,eff_postFSR,heffcorr_dd,hstat_full,3,uncert_mode,USE_BBBFSR)

#FIXME currently, it only serves to get the central value
hnorm1 = getNormalizationXsection(hxsec1)
hnorm2 = getNormalizationXsection(hxsec2)
hnorm3 = getNormalizationXsection(hxsec3)
hnorm4 = getNormalizationXsection(hxsec4)

#FIXME reset the error from 1 bin, spli stat and syst
if uncert_mode is 0: 
    print "Total uncertainty"
    hnrom1_err = sqrt(pow(0.8,2)+pow(11.6,2))
    hnrom2_err = sqrt(pow(0.7,2)+pow(11.0,2))
    hnrom3_err = sqrt(pow(0.4,2)+pow(6.1,2))
    hnrom4_err = sqrt(pow(0.4,2)+pow(5.8,2))
elif uncert_mode is 1: 
    print "Systematic only uncertainty"
    hnrom1_err = 9.8 #11.6
    hnrom2_err = 9.2 #11.0
    hnrom3_err = 5.1 #6.1
    hnrom4_err = 4.9 #5.8 is 0.18% descrease 
elif uncert_mode is 2: 
    print "Statistical only uncertainty"
    hnrom1_err = 0.8
    hnrom2_err = 0.7
    hnrom3_err = 0.4
    hnrom4_err = 0.4
#FIXME from here macro will be sensitive depending on whether we want
#to calculate the absolute cross section
hnorm1 = [hnorm1[0],hnrom1_err]
hnorm2 = [hnorm2[0],hnrom2_err]
hnorm3 = [hnorm3[0],hnrom3_err]
hnorm4 = [hnorm4[0],hnrom4_err]
if absolute:
    #FIXME error is broken, because it has to include the lumi uncertaintyat least 
    lumi = 19710
    print "lumi ", lumi
    hnorm1 = [lumi,hnrom1_err]
    hnorm2 = [lumi,hnrom2_err]
    hnorm3 = [lumi,hnrom3_err]
    hnorm4 = [lumi,hnrom4_err]

#now extract the r shape by normalizing it to the peak cross section and the bin width
#FIXME setup for absolute cross section
#for ibin in range(hxsec1.GetNbinsX()):
#    err = sqrt(pow(hxsec1.GetBinError(ibin+1)/hxsec1.GetBinContent(ibin+1),2)+pow(hnorm1[1]/hnorm1[0],2))
#    hxsec1.SetBinContent(ibin+1,hxsec1.GetBinContent(ibin+1)/hnorm1[0])#/(mBins[ibin+1]-mBins[ibin]))
#    hxsec1.SetBinError(ibin+1,hxsec1.GetBinContent(ibin+1)*err)

for ibin in range(hxsec1.GetNbinsX()):
    if absolute:
        #FIXME I think the expression will be the same but the norm error will simply become the lumi error
        err = sqrt(pow(hxsec1.GetBinError(ibin+1)/hxsec1.GetBinContent(ibin+1),2)+pow(hnorm1[1]/hnorm1[0],2))
        hxsec1.SetBinContent(ibin+1,hxsec1.GetBinContent(ibin+1)/hnorm1[0]/(mBins[ibin+1]-mBins[ibin]))
    else:
        err = sqrt(pow(hxsec1.GetBinError(ibin+1)/hxsec1.GetBinContent(ibin+1),2)+pow(hnorm1[1]/hnorm1[0],2))
        hxsec1.SetBinContent(ibin+1,hxsec1.GetBinContent(ibin+1)/hnorm1[0]/(mBins[ibin+1]-mBins[ibin]))
    hxsec1.SetBinError(ibin+1,hxsec1.GetBinContent(ibin+1)*err)

for ibin in range(hxsec3.GetNbinsX()):
    if absolute:
        err = sqrt(pow(hxsec3.GetBinError(ibin+1)/hxsec3.GetBinContent(ibin+1),2)+pow(hnorm3[1]/hnorm3[0],2))
        hxsec3.SetBinContent(ibin+1,hxsec3.GetBinContent(ibin+1)/hnorm3[0]/(mBins[ibin+1]-mBins[ibin]))
    else:
        err = sqrt(pow(hxsec3.GetBinError(ibin+1)/hxsec3.GetBinContent(ibin+1),2)+pow(hnorm3[1]/hnorm3[0],2))
        hxsec3.SetBinContent(ibin+1,hxsec3.GetBinContent(ibin+1)/hnorm3[0]/(mBins[ibin+1]-mBins[ibin]))
    hxsec3.SetBinError(ibin+1,hxsec3.GetBinContent(ibin+1)*err)

for ibin in range(hxsec4.GetNbinsX()):
    if absolute:
        err = sqrt(pow(hxsec4.GetBinError(ibin+1)/hxsec4.GetBinContent(ibin+1),2)+pow(hnorm4[1]/hnorm4[0],2))
        hxsec4.SetBinContent(ibin+1,hxsec4.GetBinContent(ibin+1)/hnorm4[0]/(mBins[ibin+1]-mBins[ibin]))
    else:
        err = sqrt(pow(hxsec4.GetBinError(ibin+1)/hxsec4.GetBinContent(ibin+1),2)+pow(hnorm4[1]/hnorm4[0],2))
        hxsec4.SetBinContent(ibin+1,hxsec4.GetBinContent(ibin+1)/hnorm4[0]/(mBins[ibin+1]-mBins[ibin]))
    hxsec4.SetBinError(ibin+1,hxsec4.GetBinContent(ibin+1)*err)

for ibin in range(hxsec2.GetNbinsX()):
    if absolute:
        err = sqrt(pow(hxsec2.GetBinError(ibin+1)/hxsec2.GetBinContent(ibin+1),2)+pow(hnorm2[1]/hnorm2[0],2))
        hxsec2.SetBinContent(ibin+1,hxsec2.GetBinContent(ibin+1)/hnorm2[0]/(mBins[ibin+1]-mBins[ibin]))
    else:
        err = sqrt(pow(hxsec2.GetBinError(ibin+1)/hxsec2.GetBinContent(ibin+1),2)+pow(hnorm2[1]/hnorm2[0],2))
        hxsec2.SetBinContent(ibin+1,hxsec2.GetBinContent(ibin+1)/hnorm2[0]/(mBins[ibin+1]-mBins[ibin]))
    hxsec2.SetBinError(ibin+1,hxsec2.GetBinContent(ibin+1)*err)

#rshapes table as in paper and AN (all 4, full error)
#printHistoCombined(hxsec1,hxsec2,hxsec3,hxsec4,pprint)

hnorm = hnorm1
thisx = hxsec1.Clone()
if xsectype is 1:
    print "preFSR-full acceptance"
elif xsectype is 4:
    print "postFSR - detector acceptance"
    thisx = hxsec4.Clone()
    hnorm = hnorm4
elif xsectype is 3:
    print "preFSR - detector acceptance"
    thisx = hxsec3.Clone()
    hnorm = hnorm3
elif xsectype is 2:
    print "postFSR - full acceptance"
    thisx = hxsec2.Clone()
    hnorm = hnorm2
#print "Check the vtx correction each time..."
#print "hnorm[0] ", hnorm[0]/4481./9.82999672934744440e-01*0.9941489," +- ", hnorm[1]/4481./9.82999672934744440e-01*0.9941489 #*0.9874

#print '\n\n'
#simple r shape print out, as in twiki, the latex mode is not used
#printHisto(thisx,pprint)
#systematics table as in twiki
#printHistoStoyan(hstat_full, heffcorr_dd, hsyst_unf, hsyst_bkg, hsyst_FSR, thisx, xsectype)
#systematics table from the paper
#printHistoIlya(hstat_full, heffcorr_dd, hsyst_unf, hsyst_bkg, hsyst_FSR, thisx, hsyst_acc, hpdf_acc, hmod_acc, pprint)

#printCombination2('../Outputs/rshape_comb_MarchMain14Acc.root')

POSTFIX = "rshape"
if absolute: POSTFIX = "absex"

if xsectype is 1:
    thisx.SetName('hxsec')
    outf = ROOT.TFile('../Outputs/'+POSTFIX+'_full.root','recreate')
    thisx.Write()
    outf.Close()
    out_systacc = ROOT.TFile('../Outputs/syst_acc.root','recreate')
    hsyst_acc.Write()
    out_systacc.Close()
if xsectype is 2:
    thisx.SetName('hxsec')
    outf = ROOT.TFile('../Outputs/'+POSTFIX+'_full_postFSR.root','recreate')
    thisx.Write()
    outf.Close()

if xsectype is 3:
    thisx.SetName('hxsec')
    outf = ROOT.TFile('../Outputs/'+POSTFIX+'_DET.root','recreate')
    thisx.Write()
    outf.Close()

if xsectype is 4:
    thisx.SetName('hxsec')
    outf = ROOT.TFile('../Outputs/'+POSTFIX+'_DET_postFSR.root','recreate')
    thisx.Write()
    outf.Close()

