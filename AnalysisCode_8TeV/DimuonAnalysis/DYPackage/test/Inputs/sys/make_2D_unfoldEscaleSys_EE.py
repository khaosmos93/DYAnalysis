#!/usr/bin/env python

from ROOT import *

#take the rapidity dependency template: dimuon unfolding systematics
fsyst_unf = TFile("../../shapeR2D/sys/unfold_corrRun2011AB_sys_new.root")
hsyst_unf0 = fsyst_unf.Get("hunfold_sys")
hsyst_unf2D = TH1D('hsyst_unf','hsyst_unf',132,0,132)
for ibin in range(hsyst_unf2D.GetNbinsX()):
    hsyst_unf2D.SetBinContent(ibin+1,hsyst_unf0.GetBinContent(ibin+1)*100.)
    hsyst_unf2D.SetBinError(ibin+1,0.)
    #print hsyst_unf.GetBinContent(ibin+1)

#Escale
hsyst_escale2D = hsyst_unf2D.Clone()

#
#
#Unfolding systematics
#
#
#take the magnitude template: 1D unfolding systematics in the dimuon channel
#xAxis = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500]
#unfold_SF = [1.9,2.2,1.9,1.4,1.1,1.1,1.3,1.7,2.4,3.8,4.9,5.2,5.3,3.7,2.3,0.7,0.7,3.7,5.8,13.1,10.2,10.2,8.1,6.0,4.7,4.7,4.9,3.9,4.1,3.8,2.9,3.3,3.4,3.0,3.7,5.8,5.3,1.2,2.2,1.3]
unfold_SF = [1.8,1.2,2.3,5.3,5.0,3.2]    
#Produce the 2D unfolding systematics in EE channel using the averaged
corr = 1.
for ibin in range(hsyst_unf2D.GetNbinsX()):
    if ibin < 24: corr = unfold_SF[0]
    elif ibin < 48: corr = unfold_SF[1]
    elif ibin < 72: corr = unfold_SF[2]
    elif ibin < 96: corr = unfold_SF[3]
    elif ibin < 120: corr = unfold_SF[4]
    else: corr = unfold_SF[5]
    hsyst_unf2D.SetBinContent(ibin+1,hsyst_unf2D.GetBinContent(ibin+1)*corr)

#
#
#Escale systematics
#
#
#take the magnitude template: 1D unfolding systematics in the dimuon channel
#unfold_Escale = [1.4,2.5,1.5,1.4,0.6,0.7,0.7,3.3,2.8,6.4,2.4,2.1,1.5,2.0,5.9,8.8,8.4,15.6,17.6,10.4,5.5,2.5,2.0,2.9,4.9,3.3,3.5,6.7,5.6,4.1,2.6,1.8,1.6,1.1,1.8,3.3,3.2,3.4,1.5,7.8]
unfold_Escale = [1.8,0.7,3.3,6.9,4.1,2.8]

#Produce the 2D escale systematics in EE channel using the averaged
for ibin in range(hsyst_escale2D.GetNbinsX()):
    if ibin < 24: corr = unfold_Escale[0]
    elif ibin < 48: corr = unfold_Escale[1]
    elif ibin < 72: corr = unfold_Escale[2]
    elif ibin < 96: corr = unfold_Escale[3]
    elif ibin < 120: corr = unfold_Escale[4]
    else: corr = unfold_Escale[5]
    hsyst_escale2D.SetBinContent(ibin+1,hsyst_escale2D.GetBinContent(ibin+1)*corr)

funf_out = TFile("unfold2D_EE.root","recreate")
funf_out.cd()
hsyst_unf2D.Write()
funf_out.Close()

fescale_out = TFile("escale2D_EE.root","recreate")
fescale_out.cd()
hsyst_escale2D.Write()
fescale_out.Close()
