#!/usr/bin/env python
import numpy as np
from ROOT import *
from array import array

xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81,
86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141,
150, 160, 171, 185, 200, 220, 243, 273, 320,
380, 440, 510, 600, 1000, 1500, 2000]

#combination begin
ifile_data = open('output_data.txt','r') 

r_vals = np.zeros(1000)
r_vals = np.loadtxt(ifile_data)
#print "Hello ", r_vals.shape
#print r_vals[0,3]

#FIXME add the acceptance related uncertainties here
#fin_systacc = TFile('../Outputs/syst_acc.root')
#hacc_syst = fin_systacc.Get('modeling_2012')

fin_systacc = TFile('../Inputs/sys/syst_acc.root')
hacc_syst = fin_systacc.Get('invm_FEWZ41')

fout = TFile('rshape_comb_41bin.root','recreate')

hxsec = TH1D('hxsec','hxsec',41,array('d',xAxis))
for ibin in range(hxsec.GetXaxis().GetNbins()):
    print ibin+1, r_vals[ibin,3]
    hxsec.SetBinContent(ibin+1,r_vals[ibin,3])
    hxsec.SetBinError(ibin+1,r_vals[ibin,3]*sqrt(pow(r_vals[ibin,4]/r_vals[ibin,3],2)+pow(hacc_syst.GetBinContent(ibin+1),2)/1.4)) #FIXME control acceptance error

#covariance matrix
ifile_matrix = open('output_covMat.txt','r')
r_covs = np.zeros(1000)
r_covs = np.loadtxt(ifile_matrix)


hcov = TH2D('hcov','hcov',41,array('d',xAxis),41,array('d',xAxis))

#print r_covs.shape
#print r_covs[0,2]

linear_index = 0
for ixbin in range(hcov.GetXaxis().GetNbins()):
    for iybin in range(hcov.GetYaxis().GetNbins()):
        hcov.SetBinContent(ixbin+1,iybin+1,r_covs[linear_index,2])
        hcov.SetBinError(ixbin+1,iybin+1,0.0)
        linear_index += 1

fout.cd()
hxsec.Write()
hcov.Write()
fout.Close()
