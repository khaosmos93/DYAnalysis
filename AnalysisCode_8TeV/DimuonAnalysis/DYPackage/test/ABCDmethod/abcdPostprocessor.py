#!/usr/bin/env python

from subprocess import Popen
from ROOT import *
from math import sqrt
import os, sys

folder = './'

#merge files
def stringer(inp_list):
    out_string = ''
    for item in inp_list:
        out_string += item+' '
    return out_string

def err_calc(v1, v2, v3, e1, e2, e3):
    if v3 == 0: return 0.

    dv1 = (v2/v3)
    dv2 = (v1/v3)
    dv3 = -1.0*(v1*v2/v3/v3)
    dv_2 = dv1*dv1*e1*e1 + dv2*dv2*e2*e2 + dv3*dv3*e3*e3

    if dv_2 == 0: dv_2 = 0.0000001
    return sqrt(dv_2)

#function used to convert 132 bin rapidity sliced histos to just 1 slice
def get2DSlice(hist2D,whichhist):
    #no action for 1D
    if hist == '': return hist2D
    elif hist != '2001500' and hist != '': 
        last_index = 0
        if hist == '3045': last_index = 24
        if hist == '4560': last_index = 48
        if hist == '60120': last_index = 72
        if hist == '120200': last_index = 96
        retHist = TH1D('retHist'+hist,'retHist'+hist,24,0,2.4) 
        for ibin in range(24):
            retHist.SetBinContent(ibin+1,hist2D.GetBinContent(ibin+1+last_index))
            retHist.SetBinError(ibin+1,hist2D.GetBinError(ibin+1+last_index))
        return retHist
    else:
        last_index = 120
        retHist = TH1D('retHist'+hist,'retHist'+hist,12,0,2.4)
        for ibin in range(12):
            retHist.SetBinContent(ibin+1,hist2D.GetBinContent(ibin+1+last_index))
            retHist.SetBinError(ibin+1,hist2D.GetBinError(ibin+1+last_index))
        return retHist



isos = ['mu1iso','mu2iso']
histos = ['','2030','3045','4560','60120','120200','2001500']

import glob
#merge and prepare ABCD2vari_ files
for iso in isos:
    inputs = glob.glob(folder+'step2_'+iso+'_*.root')
    if stringer(inputs) == "": print "No input to process"
    else: os.system("hadd -f "+folder+"ABCD2vari_"+iso+".root "+stringer(inputs))

#parse
for hist in histos:
    print "Hist ", hist
    ftmp = TFile('tmpABCD_'+hist+'.root',"recreate")
    for iso in isos:
        #open the ABCD2vari_ file
        if os.path.exists(folder+"ABCD2vari_"+iso+".root"):
            f = TFile(folder+"ABCD2vari_"+iso+".root")
        else: print folder+"ABCD2vari_"+iso+".root", " does not exist" 
        is2D = '2'
        if hist == '': is2D = '' 
        totAtmp = f.Get('totA'+is2D)
        totBtmp = f.Get('totB'+is2D)
        totCtmp = f.Get('totC'+is2D)
        totDtmp = f.Get('totD'+is2D)
        totA = get2DSlice(totAtmp,hist)
        totB = get2DSlice(totBtmp,hist)
        totC = get2DSlice(totCtmp,hist)
        totD = get2DSlice(totDtmp,hist)

        #print "XXX val ", totA.Integral(), totB.Integral(), totC.Integral(), totD.Integral()  
        g = TFile(folder+"factor_qcd_"+iso+"_C.root")
        hlookup = g.Get('hfactor'+hist)
        #print "YYY val ", hlookup.Integral()
        val = -1.

        fB = TFile("QCDFrac_"+iso+"_B.root")
        #Stack up all the MC 
        htotB = fB.Get('sig'+hist) 
        ewk = fB.Get('ewk'+hist)
        tau = fB.Get('tau'+hist)
        diboson = fB.Get('diboson'+hist)
        ttbar = fB.Get('ttbar'+hist)
        #FIXME
        htotB.Add(ewk)
        htotB.Add(tau)
        htotB.Add(diboson)
        htotB.Add(ttbar)
        htotB.Sumw2()
        print "Check 1 ", htotB.Integral() 
 
        hDataB = fB.Get('data'+hist)
        hDataB.Sumw2()
  
        #Produce the histo of MC that is not in data
        hNumB = fB.Get('data'+hist)
        hNumB.Add(htotB, -1)

        print "Check 2 ", htotB.Integral()
  
        #Produce corresponding excess/data ratio
        hRatioB = hNumB.Clone()
        hRatioB.Divide(hNumB, hDataB)
        hRatioB.Fit("pol0")

        print "Check 3 ", htotB.Integral()
  
        #fit it
        myfitB = hRatioB.GetFunction("pol0")
        #factor_B = myfitB.GetParameter(0)
        factor_Berr = myfitB.GetParError(0)

        print "Check 4 ", htotB.Integral()

        fD = TFile("QCDFrac_"+iso+"_D.root")
        #Stack up all the MC  
        htotD = fD.Get('sig'+hist) #sig
        ewk = fD.Get('ewk'+hist)
        tau = fD.Get('tau'+hist)
        diboson = fD.Get('diboson'+hist)
        ttbar = fD.Get('ttbar'+hist)
        htotD.Add(ewk)
        htotD.Add(tau)
        htotD.Add(diboson)
        htotD.Add(ttbar)
        htotD.Sumw2()

        print "Check 5 ", htotB.Integral()
  
        hDataD = fD.Get('data'+hist)
        hDataD.Sumw2()

        print "Check 6 ", htotB.Integral()
  
        #Produce the histo of MC that is not in data
        hNumD = fD.Get('data'+hist)
        hNumD.Add(htotD, -1)

        print "Check 7 ", htotB.Integral()
  
        #Produce corresponding excess/data ratio
        hRatioD = hNumD.Clone()
        hRatioD.Divide(hNumD, hDataD)
        hRatioD.Fit("pol0")

        print "Check 8 ", htotB.Integral()
  
        #fit it
        myfitD = hRatioD.GetFunction("pol0")
        #factor_D = myfitD.GetParameter(0)
        factor_Derr = myfitD.GetParError(0)

        print "Check 9 ", htotB.Integral()

        #output histogram with ABCD estimate, one per iso configuration
        hout = hDataD.Clone()
        hout.SetName(iso)

        print "Check 10 ", htotB.Integral()

        for ibin in range(hlookup.GetXaxis().GetNbins()):
            print "Check 11 ", htotB.Integral()

            factor_C = hlookup.GetBinContent(ibin+1)
            factor_Cerr = hlookup.GetBinError(ibin+1)
            if totD.GetBinContent(ibin+1) != 0: 
                val = totB.GetBinContent(ibin+1)*totC.GetBinContent(ibin+1)/totD.GetBinContent(ibin+1)
                print ibin+1, " totB ", totB.Integral()
            ferr = sqrt(factor_Berr*factor_Berr + factor_Cerr*factor_Cerr + factor_Derr*factor_Derr)
            err = err_calc(totB.GetBinContent(ibin+1),totC.GetBinContent(ibin+1),totD.GetBinContent(ibin+1),(1+ferr)*sqrt(totB.GetBinContent(ibin+1)),(1+ferr)*sqrt(totC.GetBinContent(ibin+1)),(1+ferr)*sqrt(totD.GetBinContent(ibin+1)))
            if hist == '':
                #FIXME artificially stop QCD at high mass for now
                if ibin < 40: 
                    hout.SetBinContent(ibin+1,val)
                    hout.SetBinError(ibin+1,err)
                else:
                    hout.SetBinContent(ibin+1,0.)
                    hout.SetBinError(ibin+1,0.)
            else:
                hout.SetBinContent(ibin+1,val)
                hout.SetBinError(ibin+1,err)
        #print "QCD cont:(", totA.GetBinCenter(ibin+1)-totA.GetBinWidth(ibin+1)/2., ", ", totA.GetBinCenter(ibin+1)+totA.GetBinWidth(ibin+1)/2., ") = ", val, " +- ", err, " :: ", totA.GetBinContent(ibin+1)," " ,  totB.GetBinContent(ibin+1),  " ",  totC.GetBinContent(ibin+1),  " ",  totD.GetBinContent(ibin+1),  " ",  ferr
        ftmp.cd()
        hout.Write()
    ftmp.Close()
    fin = TFile('tmpABCD_'+hist+'.root')
    fin.cd()
    mu1iso = fin.Get("mu1iso")
    mu2iso = fin.Get("mu2iso")
    hQCD = hNumB.Clone()
    hQCD.SetName('hQCD'+hist)
    for  ibin in range(hQCD.GetXaxis().GetNbins()):
        hQCD.SetBinContent(ibin+1, (mu1iso.GetBinContent(ibin+1)+mu2iso.GetBinContent(ibin+1))/2.) 
        hQCD.SetBinError(ibin+1, sqrt(mu1iso.GetBinError(ibin+1)*mu1iso.GetBinError(ibin+1)+mu2iso.GetBinError(ibin+1)*mu2iso.GetBinError(ibin+1)))
    fout = TFile('outABCD_'+hist+'.root','recreate')
    fout.cd()
    hQCD.Write()
    fout.Close()


import sys,os
import glob
from ROOT import *

#merge files
def stringer(inp_list):
    out_string = ''
    for item in inp_list:
        out_string += item+' '
    return out_string


TMPs = glob.glob('tmpABCD_*.root')
os.system("rm "+stringer(TMPs))
OUTs = glob.glob('outABCD_*.root')
os.system("hadd -f outABCD.root "+stringer(OUTs))
os.system("rm "+stringer(OUTs))


#fout = TFile('outABCD.root',"recreate")
#hQCD.Write()
#fout.Close()
