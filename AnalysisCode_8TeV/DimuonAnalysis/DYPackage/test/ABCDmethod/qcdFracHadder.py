#!/usr/bin/env python
import sys,os
from subprocess import Popen
import glob
from ROOT import *

print "WARNING: Step1 files are output in the rootfiles1 folder, so give a folder"
folder = './'+str(sys.argv[1])+'/'

#merge files
def stringer(inp_list):
    out_string = ''
    for item in inp_list:
        out_string += item+' '
    return out_string

#print stringer(EWKs)
isos = ['mu1iso','mu2iso']
regions = ['B','C','D']

#make stringers to hadd
for iso in isos:
    for reg in regions:
        DYMs  = glob.glob(folder+'DYM*'+iso+"_"+reg+'.root')
        DATAs = glob.glob(folder+'DATA*'+iso+"_"+reg+'.root')
        TTs   = glob.glob(folder+'t*'+iso+"_"+reg+'.root')
        TAUs  = glob.glob(folder+'DYtautau*'+iso+"_"+reg+'.root')
        EWKs  = glob.glob(folder+'WJ*'+iso+"_"+reg+'.root')
        QCDs  = glob.glob(folder+'QCD*'+iso+"_"+reg+'.root')
        ZZs   = glob.glob(folder+'ZZ*'+iso+"_"+reg+'.root')
        WZs   = glob.glob(folder+'WZ*'+iso+"_"+reg+'.root')
        WWs   = glob.glob(folder+'WW*'+iso+"_"+reg+'.root')

        if stringer(DYMs) == "": print "No DYM output to process"
        else: os.system("hadd -f "+folder+"DYM"+iso+"_"+reg+"_CP.root "+stringer(DYMs))
        if stringer(DATAs) == "": print "No DATA output to process"
        else: os.system("hadd -f "+folder+"DATA"+iso+"_"+reg+"_CP.root "+stringer(DATAs))
        if stringer(TTs) == "": print "No TT output to process"
        else: os.system("hadd -f "+folder+"TT"+iso+"_"+reg+"_CP.root "+stringer(TTs))
        if stringer(TAUs) == "": print "No TAU output to process"
        else: os.system("hadd -f "+folder+"TAU"+iso+"_"+reg+"_CP.root "+stringer(TAUs))
        if stringer(EWKs) == "": print "No EWK output to process"
        else: os.system("hadd -f "+folder+"EWK"+iso+"_"+reg+"_CP.root "+stringer(EWKs))
        if stringer(QCDs) == "": print "No QCD output to process"
        else: os.system("hadd -f "+folder+"QCD"+iso+"_"+reg+"_CP.root "+stringer(QCDs))
        if stringer(WWs) == "" or stringer(WZs) == "" or stringer(ZZs) == "": print "No DIBOSON output to process"
        else: os.system("hadd -f "+folder+"DIBOSON"+iso+"_"+reg+"_CP.root "+stringer(WWs)+' '+stringer(WZs)+' '+stringer(ZZs))


def histAccessor(hname, tfile_in, tfile_out):
    h = tfile_in.Get('template')
    h2030 = tfile_in.Get('template2030')
    h3045 = tfile_in.Get('template3045')
    h4560 = tfile_in.Get('template4560')
    h60120 = tfile_in.Get('template60120')
    h120200 = tfile_in.Get('template120200')
    h2001500 = tfile_in.Get('template2001500')
    h.SetName(hname)
    h2030.SetName(hname+"2030")
    h3045.SetName(hname+"3045")
    h4560.SetName(hname+"4560")
    h60120.SetName(hname+"60120")
    h120200.SetName(hname+"120200")
    h2001500.SetName(hname+"2001500")
    tfile_out.cd()
    h.Write()
    h2030.Write()
    h3045.Write()
    h4560.Write()
    h60120.Write()
    h120200.Write()
    h2001500.Write()

#get the histograms from files
#if a histo is missing it will give a crash
for iso in isos:
    for reg in regions:
        #FIXME not exactly sure about this
        fout = TFile("QCDFrac_"+iso+"_"+reg+".root","recreate")

        if os.path.exists(folder+"DATA"+iso+"_"+reg+"_CP.root"):
            fDATA = TFile(folder+"DATA"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("data",fDATA, fout)
        else: print folder+"DATA"+iso+"_"+reg+"_CP.root", " doesn't exist"

        if os.path.exists(folder+"DYM"+iso+"_"+reg+"_CP.root"):
            fDYM = TFile(folder+"DYM"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("sig",fDYM, fout)
        else: print folder+"DYM"+iso+"_"+reg+"_CP.root", " doesn't exist"

        if os.path.exists(folder+"TT"+iso+"_"+reg+"_CP.root"):
            fTT = TFile(folder+"TT"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("ttbar",fTT, fout)
        else: print folder+"TT"+iso+"_"+reg+"_CP.root", " doesn't exist"

        if os.path.exists(folder+"TAU"+iso+"_"+reg+"_CP.root"):
            print folder+"TAU"+iso+"_"+reg+"_CP.root"
            fTAU = TFile(folder+"TAU"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("tau",fTAU, fout)
        else: print folder+"TAU"+iso+"_"+reg+"_CP.root", " doesn't exist"

        if os.path.exists(folder+"EWK"+iso+"_"+reg+"_CP.root"):
            fEWK = TFile(folder+"EWK"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("ewk",fEWK, fout)
        else: print folder+"EWK"+iso+"_"+reg+"_CP.root", " doesn't exist"

        if os.path.exists(folder+"QCD"+iso+"_"+reg+"_CP.root"):
            fQCD = TFile(folder+"QCD"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("qcd",fQCD, fout)
        else: print folder+"QCD"+iso+"_"+reg+"_CP.root", " doesn't exist"

        if os.path.exists(folder+"DIBOSON"+iso+"_"+reg+"_CP.root"):
            fDIBOSON = TFile(folder+"DIBOSON"+iso+"_"+reg+"_CP.root") #at that point you have all the necessary files to access and rename and remerge
            histAccessor("diboson",fDIBOSON, fout)
        else: print folder+"DIBOSON"+iso+"_"+reg+"_CP.root", " doesn't exist"
