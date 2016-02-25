#!/usr/bin/env python

import ROOT
from math import sqrt, pow

#each histogram comes with total uncertainty here
def getXsection(huy,hacceff,hacc,heff,heffcorr_dd,hstat,xsectype, uncert_mode=0, isBBBFSR=False):
    #descriptions: xsectype = 1 full correction, type 2 no acceptance correcton
    #type3 no acceptance and FSR correction (pre-FSR) type 4 no FSR correction
    syst_frac = 1. 
    stat_frac = 1.
    if uncert_mode == 2: syst_frac = 0.
    elif uncert_mode == 1: stat_frac = 0.
    crossSection = huy.Clone()
    for ibin in range(huy.GetNbinsX()):
        if xsectype == 1:
            #print "preFSR-full acceptance" #FIXME use pre FSR acceptance
            if not isBBBFSR: crossSection.SetBinContent(ibin+1,huy.GetBinContent(ibin+1)/hacc.GetBinContent(ibin+1)) #/heffcorr_dd.GetBinContent(ibin+1))
            else: crossSection.SetBinContent(ibin+1,huy.GetBinContent(ibin+1)/hacceff.GetBinContent(ibin+1)/heffcorr_dd.GetBinContent(ibin+1))
            crossSection.SetBinError(ibin+1,crossSection.GetBinContent(ibin+1)*sqrt((pow(huy.GetBinError(ibin+1)/huy.GetBinContent(ibin+1),2)+pow(heffcorr_dd.GetBinError(ibin+1)/heffcorr_dd.GetBinContent(ibin+1),2)+pow(hacceff.GetBinError(ibin+1)/hacceff.GetBinContent(ibin+1),2))*syst_frac+stat_frac*(pow(hstat.GetBinContent(ibin+1),2))))
        elif xsectype == 4:
            #print "postFSR - detector acceptance"
            crossSection.SetBinContent(ibin+1,huy.GetBinContent(ibin+1)/heff.GetBinContent(ibin+1)/heffcorr_dd.GetBinContent(ibin+1))
            crossSection.SetBinError(ibin+1,crossSection.GetBinContent(ibin+1)*sqrt((pow(huy.GetBinError(ibin+1)/huy.GetBinContent(ibin+1),2)+pow(heffcorr_dd.GetBinError(ibin+1)/heffcorr_dd.GetBinContent(ibin+1),2)+pow(heff.GetBinError(ibin+1)/heff.GetBinContent(ibin+1),2))*syst_frac+stat_frac*(pow(hstat.GetBinContent(ibin+1),2))))
        elif xsectype == 3:
            #print "preFSR-full acceptance" #FIXME use pre FSR acceptance
            if not isBBBFSR: crossSection.SetBinContent(ibin+1,huy.GetBinContent(ibin+1)) #/heffcorr_dd.GetBinContent(ibin+1))
            else: crossSection.SetBinContent(ibin+1,huy.GetBinContent(ibin+1)/heff.GetBinContent(ibin+1)/heffcorr_dd.GetBinContent(ibin+1))
            crossSection.SetBinError(ibin+1,crossSection.GetBinContent(ibin+1)*sqrt((pow(huy.GetBinError(ibin+1)/huy.GetBinContent(ibin+1),2)+pow(heffcorr_dd.GetBinError(ibin+1)/heffcorr_dd.GetBinContent(ibin+1),2)+pow(heff.GetBinError(ibin+1)/heff.GetBinContent(ibin+1),2))*syst_frac+stat_frac*(pow(hstat.GetBinContent(ibin+1),2))))
        elif xsectype == 2:
            #print "postFSR - full acceptance"
            crossSection.SetBinContent(ibin+1,huy.GetBinContent(ibin+1)/hacceff.GetBinContent(ibin+1)/heffcorr_dd.GetBinContent(ibin+1))
            crossSection.SetBinError(ibin+1,crossSection.GetBinContent(ibin+1)*sqrt((pow(huy.GetBinError(ibin+1)/huy.GetBinContent(ibin+1),2)+pow(heffcorr_dd.GetBinError(ibin+1)/heffcorr_dd.GetBinContent(ibin+1),2)+pow(hacceff.GetBinError(ibin+1)/hacceff.GetBinContent(ibin+1),2))*syst_frac+stat_frac*(pow(hstat.GetBinContent(ibin+1),2))))
        else: print "Wrong correction type!!!"
    return crossSection

#this extracts the peak region for normalization and associated erroor
def getNormalizationXsection(thisH):
    #before it goes here, histogram should carry all the statistical and systematic uncertainty on itself. if we want to print out seprately    #stat and syst than we should store it in a separate histo
    print "WARNING: relies on the fixed binning 2011!"
    norm_constants = []
    thisNorm = 0.
    ithisNorm_err = 0.
    thisHNorm_err = 0.
    for ibin in range(9,22):
        thisNorm+=thisH.GetBinContent(ibin+1)
        ithisNorm_err+=pow(thisH.GetBinError(ibin+1),2)
    thisHNorm_err = sqrt(ithisNorm_err)
    norm_constants.append(thisNorm)
    norm_constants.append(thisHNorm_err)
    return norm_constants

def printHisto(hist, mode):
    #description: mode = 0 - simple print out for reading
    #mode = 1 - print out for tex table
    if mode == 0:
        for ibin in range(hist.GetNbinsX()):
            print "shape["+str(ibin)+"]", ibin, hist.GetBinContent(ibin+1), " ", hist.GetBinError(ibin+1)
            #print 'corr[%2.0f] = %1.5f; corr_err[%2.0f] = %1.5f;' %(ibin,hist.GetBinContent(ibin+1),ibin,hist.GetBinError(ibin+1))
    elif mode == 1:
        print "\\begin{table} [htpb]"
        print "\\begin{center}"
        print "\\begin{tabular}{|c|c|c|}"
        print "\\hline"
        print "Mass bin [GeV] & R shape ($x10^{5}$) & R shape error ($x10^{5}$) \\\\"
        print "\\hline"
        for ibin in range(hist.GetNbinsX()):
            if (ibin < 33): print '%d-%d & $ %5.2f $ & $ %5.2f $ \\\\' %(hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2.,hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.,100000.*hist.GetBinContent(ibin+1),100000.*hist.GetBinError(ibin+1))
            else: print '%d-%d & $ %5.2e $ & $ %5.2e $ \\\\' %(hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2.,hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.,100000.*
hist.GetBinContent(ibin+1),100000.*hist.GetBinError(ibin+1))
            print "\\hline"
        print "\\end{tabular}"
        print "\\caption{"
        print "The PDF and modeling uncertainty on acceptance per dimuon invariant mass bin.}"
        print "\\label{tab:}"
        print "\\end{center}"
        print "\\end{table}"
    elif mode == 2:
        for ibin in range(hist.GetNbinsX()):
            print ibin+1, " ", hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2., " ", hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.," ",hist.GetBinContent(ibin+1), " ", hist.GetBinError(ibin+1)
            #print 'corr[%2.0f] = %1.5f; corr_err[%2.0f] = %1.5f;' %(ibin,hist.GetBinContent(ibin+1),ibin,hist.GetBinError(ibin+1))
                    

def printUncertainty(hist, shaper, toShaper):
    for ibin in range(hist.GetNbinsX()):
        if toShaper:
            print ibin, " Errors*dr: ", shaper.GetBinContent(ibin+1)*hist.GetBinContent(ibin+1)
        else:
            #print ibin, " Errors [%]: ", 100.*hist.GetBinContent(ibin+1) 
            #print "%3.0d-%3.0d & $ %2.2f $ \\\\ \n" %((hist.GetBinCenter(ibin+1) - hist.GetBinWidth(ibin+1)/2.), (hist.GetBinCenter(ibin+1) + hist.GetBinWidth(ibin+1)/2.), hist.GetBinContent(ibin+1)*100.),
            print hist.GetBinContent(ibin+1)*100.,", ",
            #print "\\hline"


def printTotalUncertainty(shaper, hsyst_unf, hstat_unf, hsyst_acc, hsyst_fsr, hstat_effcorr, hsyst_effcorr, toShaper, isSystematic):
    for ibin in range(hist.GetNbinsX()):
        if toShaper:
            if isSystematic:
                print ibin, " Errors*dr: ", shaper.GetBinContent(ibin+1)*sqrt(pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(hsyst_acc.GetBinContent(ibin+1),2)+pow(hsyst_fsr.GetBinContent(ibin+1),2)+pow(hsyst_effcorr.GetBinContent(ibin+1),2))
            else:
                print ibin, " Errors*dr: ", shaper.GetBinContent(ibin+1)*sqrt(pow(hstat_unf.GetBinContent(ibin+1),2)+pow(hstat_effcorr.GetBinContent(ibin+1),2))
        else:
            if isSystematic:
                print ibin, " Errors [%]: ", 100.*sqrt(pow(hsyst_unf.GetBinContent(ibin+1),2)+pow(hsyst_acc.GetBinContent(ibin+1),2)+pow(hsyst_fsr.GetBinContent(ibin+1),2)+pow(hsyst_effcorr.GetBinContent(ibin+1),2))
            else:
                print ibin, " Errors [%]: ", 100.*sqrt(pow(hstat_unf.GetBinContent(ibin+1),2)+pow(hstat_effcorr.GetBinContent(ibin+1),2))

def saveHisto(hist):
    file = ROOT.TFile(str(hist.GetName())+'.root','recreate')
    hist.Write()
    file.Close()

def printHistoCombined(hist1,hist2,hist3,hist4, mode):
    #description: mode = 0 - simple print out for reading
    #mode = 1 - print out for tex table    print "Combined printout "
    if mode is 0:
        for ibin in range(hist1.GetNbinsX()):
            print ibin, " ", hist1.GetBinContent(ibin+1), " +/- ", hist1.GetBinError(ibin+1)," ", hist2.GetBinContent(ibin+1), " +/- ", hist2.GetBinError(ibin+1)," ", hist3.GetBinContent(ibin+1), " +/- ", hist3.GetBinError(ibin+1)," ", hist4.GetBinContent(ibin+1), " +/- ", hist4.GetBinError(ibin+1)
    elif mode is 1:
        for ibin in range(hist1.GetNbinsX()):
            if (ibin < 30): print '%d-%d & $ %5.1f \\pm %5.1f $ & $ %5.1f \\pm %5.1f $ & $ %5.1f \\pm %5.1f $ & $ %5.1f \\pm %5.1f $\\\\' %(hist1.GetBinCenter(ibin+1)-hist1.GetBinWidth(ibin+1)/2.,hist1.GetBinCenter(ibin+1)+hist1.GetBinWidth(ibin+1)/2.,100000.*hist1.GetBinContent(ibin+1),100000.*hist1.GetBinError(ibin+1),100000.*hist2.GetBinContent(ibin+1),100000.*hist2.GetBinError(ibin+1),100000.*hist3.GetBinContent(ibin+1),100000.*hist3.GetBinError(ibin+1),100000.*hist4.GetBinContent(ibin+1),100000.*hist4.GetBinError(ibin+1))
            else: print '%d-%d & $ %5.4f \\pm %5.5f $ & $ %5.4f \\pm %5.5f $ & $ %5.4f \\pm %5.5f $ & $ %5.4f \\pm %5.5f $\\\\' %(hist1.GetBinCenter(ibin+1)-hist1.GetBinWidth(ibin+1)/2.,hist1.GetBinCenter(ibin+1)+hist1.GetBinWidth(ibin+1)/2.,100000.*hist1.GetBinContent(ibin+1),100000.*hist1.GetBinError(ibin+1),100000.*hist2.GetBinContent(ibin+1),100000.*hist2.GetBinError(ibin+1),100000.*hist3.GetBinContent(ibin+1),100000.*hist3.GetBinError(ibin+1),100000.*hist4.GetBinContent(ibin+1),100000.*hist4.GetBinError(ibin+1))
            print "\\hline"


def printCombination(name):
    f = ROOT.TFile(name) #out1_1025_w1_corr.root') ##../Inputs/acc/table_2011AB.root') #'out.root') #../Inputs/acc/table_2011AB.root')
    hist = f.Get('hxsec')

    #description: mode = 0 - simple print out for reading
    #mode = 1 - print out for tex table    print "Combined printout "
    for ibin in range(hist.GetNbinsX()):
        if (ibin < 30): 
            print '%d-%d & $ %5.1f \\pm %5.1f $ \\\\' %(hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2.,hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.,100000.*hist.GetBinContent(ibin+1),100000.*hist.GetBinError(ibin+1))
            print '\\hline'
        else: 
            print '%d-%d & $ %5.4f \\pm %5.5f $ \\\\' %(hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2.,hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.,100000.*hist.GetBinContent(ibin+1),100000.*hist.GetBinError(ibin+1))
            print '\\hline'

def printCombination2(name):
    f = ROOT.TFile(name) #out1_1025_w1_corr.root') ##../Inputs/acc/table_2011AB.root') #'out.root') #../Inputs/acc/table_2011AB.root')
    hist = f.Get('hxsec')

    #description: mode = 0 - simple print out for reading
    #mode = 1 - print out for tex table    print "Combined printout "
    for ibin in range(hist.GetNbinsX()):
        if (ibin < 10):
            print '%d-%d & $ %5.1f \\pm %5.1f $ & %d-%d & $ %5.1f \\pm %5.1f $  \\\\' %(hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2.,hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.,100000.*hist.GetBinContent(ibin+1),100000.*hist.GetBinError(ibin+1),hist.GetBinCenter(ibin+1+20)-hist.GetBinWidth(ibin+1+20)/2.,hist.GetBinCenter(ibin+1+20)+hist.GetBinWidth(ibin+1+20)/2.,100000.*hist.GetBinContent(ibin+1+20),100000.*hist.GetBinError(ibin+1+20))
            print '\\hline'
        else:
            print '%d-%d & $ %5.4f \\pm %5.5f $ & %d-%d & $ %5.4f \\pm %5.5f $ \\\\' %(hist.GetBinCenter(ibin+1)-hist.GetBinWidth(ibin+1)/2.,hist.GetBinCenter(ibin+1)+hist.GetBinWidth(ibin+1)/2.,100000.*hist.GetBinContent(ibin+1),100000.*hist.GetBinError(ibin+1),hist.GetBinCenter(ibin+1+20)-hist.GetBinWidth(ibin+1+20)/2.,hist.GetBinCenter(ibin+1+20)+hist.GetBinWidth(ibin+1+20)/2.,100000.*hist.GetBinContent(ibin+1+20),100000.*hist.GetBinError(ibin+1+20))
            print '\\hline'


#def printHistoStoyan(hstat, heffcorr, hunfy, hbkg, hFSR, hxsec, hpdfu, mode):
#    print "Stoyan print out"
#    if mode is 0:
#        for ibin in range(hxsec.GetNbinsX()):
#            print '%d-%d & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $\\\\' %(hstat.GetBinCenter(ibin+1)-hstat.GetBinWidth(ibin+1)/2., hstat.GetBinCenter(ibin+1)+hstat.GetBinWidth(ibin+1)/2, hstat.GetBinContent(ibin+1)*100., 100.*heffcorr.GetBinError(ibin+1)/heffcorr.GetBinContent(ibin+1), 100.*hunfy.GetBinError(ibin+1)/hunfy.GetBinContent(ibin+1), 100.*hbkg.GetBinContent(ibin+1), 100.*hFSR.GetBinError(ibin+1)/hFSR.GetBinContent(ibin+1), hpdfu.GetBinContent(ibin+1),hpdfu.GetBinContent(ibin+1)+100.*hxsec.GetBinError(ibin+1)/hxsec.GetBinContent(ibin+1))
#            print "\\hline"
#    if mode is 1:
#        for ibin in range(hxsec.GetNbinsX()):
#            print "Hello"
#            #print ibin, " & ", hstat.GetBinContent(ibin+1)*100.," & ", 100.*heffcorr.GetBinError(ibin+1)/heffcorr.GetBinContent(ibin+1), " & ", 100.*hunfy.GetBinError(ibin+1)/hunfy.GetBinContent(ibin+1), " & ", 100.*hbkg.GetBinContent(ibin+1), " & ", 100.*hFSR.GetBinError(ibin+1)/hFSR.GetBinContent(ibin+1)," & ", 100.*hpdfu.GetBinContent(ibin+1)," & ", 100.*(hpdfu.GetBinContent(ibin+1)+hxsec.GetBinError(ibin+1)/hxsec.GetBinContent(ibin+1)))

#FIXME this version is from shapeDY_stoyan_twiki_syst.py
def printHistoStoyan(hstat, heffcorr, hunfy, hbkg, hFSR, hxsec, mode):
    print "Stoyan print out in percent"
    FSRfrac = 1. 
    if mode == 2 or mode == 4: FSRfrac = 0.
    for ibin in range(hxsec.GetNbinsX()):
        print ibin, " & ", hstat.GetBinContent(ibin+1)*100.," & ", 100.*heffcorr.GetBinError(ibin+1)/heffcorr.GetBinContent(ibin+1), " & ", 100.*hunfy.GetBinContent(ibin+1), " & ", 100.*hbkg.GetBinContent(ibin+1), " & ", hFSR.GetBinContent(ibin+1)*FSRfrac, " & ", 100.*hxsec.GetBinError(ibin+1)/hxsec.GetBinContent(ibin+1)
        #rap. bin & stat. error & efficiency correction & unfolding & background & FSR & total sys. err

def printHistoIlya(hstat, heffcorr, hunfy, hbkg, hFSR, hxsec, hsyst_acc, hpdfu, hmod_acc, mode):
    #> | eff | resol | backgrounds | FSR | total || acceptance (+PDF) | modeling |
#15-20 & $  2.94 $ & $  0.03 $ & $  0.48 $ & $  0.54 $ & $ 10.43 $ & $ 10.19 $ & $  9.70 $\\
#\hline
#20-25 & $  3.45 $ & $  0.40 $ & $  4.07 $ & $  0.47 $ & $  6.95 $ & $  5.42 $ & $  3.10 $\\
#\hline
#25-30 & $  3.36 $ & $  0.27 $ & $  3.08 $ & $  0.40 $ & $  5.66 $ & $  4.31 $ & $  1.90 $\\

    print "Ilya print out"
    if mode is 0:
        for ibin in range(hxsec.GetNbinsX()):
            #print "Mass efficiency resolution bkg FSR others total acc modeling" 
            print '%d-%d & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $ & $ %5.2f $\\\\' %(hstat.GetBinCenter(ibin+1)-hstat.GetBinWidth(ibin+1)/2., hstat.GetBinCenter(ibin+1)+hstat.GetBinWidth(ibin+1)/2, 100.*heffcorr.GetBinError(ibin+1)/heffcorr.GetBinContent(ibin+1), 100.*hunfy.GetBinContent(ibin+1), 100.*hbkg.GetBinContent(ibin+1), hFSR.GetBinContent(ibin+1), sqrt(pow(hxsec.GetBinError(ibin+1)/hxsec.GetBinContent(ibin+1),2)-pow(hstat.GetBinContent(ibin+1),2)-pow(hsyst_acc.GetBinContent(ibin+1),2))*100., 100.*sqrt(pow(hsyst_acc.GetBinContent(ibin+1),2)-pow(hmod_acc.GetBinContent(ibin+1),2)), hmod_acc.GetBinContent(ibin+1)*100.)
            print "\\hline"
    if mode is 1:
        for ibin in range(hxsec.GetNbinsX()):
            print "Hello"
            #print ibin, " & ", hstat.GetBinContent(ibin+1)*100.," & ", 100.*heffcorr.GetBinError(ibin+1)/heffcorr.GetBinContent(ibin+1), " & ", 100.*hunfy.GetBinError(ibin+1)/hunfy.GetBinContent(ibin+1), " & ", 100.*hbkg.GetBinContent(ibin+1), " & ", 100.*hFSR.GetBinError(ibin+1)/hFSR.GetBinContent(ibin+1)," & ", 100.*hpdfu.GetBinContent(ibin+1)," & ", 100.*(hpdfu.GetBinContent(ibin+1)+hxsec.GetBinError(ibin+1)/hxsec.GetBinContent(ibin+1)))


def noZerosInHist(thish):
    for ibin in range(thish.GetNbinsX()):
        if thish.GetBinContent(ibin+1) == 0.: thish.SetBinContent(ibin+1,0.000001)
