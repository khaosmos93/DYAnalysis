#!/usr/bin/env python
import numpy as np
from ROOT import *
from array import array

xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81,
86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141,
150, 160, 171, 185, 200, 220, 243, 273, 320,
380, 440, 510, 600, 1000, 1500]


def get2DSlice(hist2D, hist, inputIsEE, useSetter):
    if hist == "invm":
        return hist2D
    elif hist != "absrap200" and hist != "invm":
        last_index = 0
        if hist == "absrap3045": last_index = 24
        if hist == "absrap4560": last_index = 48
        if hist == "absrap60120": last_index = 72
        if hist == "absrap120200": last_index = 96
        if inputIsEE: last_index += 1
        retHist = TH1F("retHist"+hist,"retHist"+hist,24,0,2.4)
        for ibin in range(24):
            if not useSetter:
                retHist.Fill((ibin+0.5)*0.1,hist2D.GetBinContent(ibin+1+last_index))
            else:
                retHist.SetBinContent(ibin+1,hist2D.GetBinContent(ibin+1+last_index))
                retHist.SetBinError(ibin+1,hist2D.GetBinError(ibin+1+last_index))
        return retHist
    else:
        print "Last slice"
        last_index = 120
        if inputIsEE: last_index += 1
        retHist = TH1F("retHist"+hist,"retHist"+hist,12,0,2.4)
        for ibin in range(12):
            if not useSetter:
               retHist.Fill((ibin+0.5)*0.2,hist2D.GetBinContent(ibin+1+last_index))
            else:
                retHist.SetBinContent(ibin+1,hist2D.GetBinContent(ibin+1+last_index))
                retHist.SetBinError(ibin+1,hist2D.GetBinError(ibin+1+last_index))
        return retHist



#combination begin
ifile_data = open('output_data.txt','r') 

r_vals = np.zeros(1000)
r_vals = np.loadtxt(ifile_data)
#print "Hello ", r_vals.shape
#print r_vals[0,3]

fout = TFile('absex_DET2D_comb.root','recreate')

#FIXME
peak = 1. #(567.5+570.4)/2.
binw = 1

hxsec = TH1D('hxsec','hxsec',132,0,132)
for ibin in range(hxsec.GetXaxis().GetNbins()):
    hxsec.SetBinContent(ibin+1,binw*r_vals[ibin,3]*peak)
    hxsec.SetBinError(ibin+1,binw*peak*r_vals[ibin,3]*sqrt(pow(r_vals[ibin,4]/r_vals[ibin,3],2))) #+pow(hacc_syst.GetBinContent(ibin+1),2)/1.4)) #FIXME control acceptance error

inputIsEE = False
useSetter = True

h1 = get2DSlice(hxsec, "absrap2030", inputIsEE, useSetter)  
h2 = get2DSlice(hxsec, "absrap3045", inputIsEE, useSetter)
h3 = get2DSlice(hxsec, "absrap4560", inputIsEE, useSetter)
h4 = get2DSlice(hxsec, "absrap60120", inputIsEE, useSetter)
h5 = get2DSlice(hxsec, "absrap120200", inputIsEE, useSetter)
h6 = get2DSlice(hxsec, "absrap200", inputIsEE, useSetter)

#covariance matrix
ifile_matrix = open('output_covMat.txt','r')
r_covs = np.zeros(1000)
r_covs = np.loadtxt(ifile_matrix)

hcov = TH2D('hcov','hcov',132,0,132,132,0,132)

#print r_covs.shape
#print r_covs[0,2]

linear_index = 0
for ixbin in range(hcov.GetXaxis().GetNbins()):
    for iybin in range(hcov.GetYaxis().GetNbins()):
        hcov.SetBinContent(ixbin+1,iybin+1,r_covs[linear_index,2])
        hcov.SetBinError(ixbin+1,iybin+1,0.0)
        linear_index += 1

#Draw the cova matrix
gStyle.SetPalette(1)
c = TCanvas()
c.cd()
c.SetLogx()
c.SetLogy()
hcov.GetXaxis().SetTitle('M_{ll} (GeV)')
hcov.SetMinimum(0.00000005)
hcov.Draw('COLZ')
c.SaveAs('fullCov_1D_ee.pdf')
c.Update()
c.SaveAs('fullCov_1D_ee.png')

fout.cd()
hxsec.Write()
h1.Write('20to30')
h2.Write('30to45')
h3.Write('45to60')
h4.Write('60to120')
h5.Write('120to200')
h6.Write('200to1500')
hcov.Write()
fout.Close()
