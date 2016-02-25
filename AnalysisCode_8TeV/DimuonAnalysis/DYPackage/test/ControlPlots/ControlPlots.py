#!/usr/bin/env python

from subprocess import Popen
import sys

histos = ['rap_corr'] #['angle3D','chi2dof','trackerHits','pixelHits','muonHits','nMatches','dxyBS','RelCombIsoNoEGamma','vtxTrkProb','trigMatches','pt','eta']

#histos = ['invm_fine_bb','invm_fineRoch_bb','invm_fine_ee','invm_fineRoch_ee','invm_fine_eb','invm_fineRoch_eb']  #rap'] 
folder = './runfolder_TRM/'

for hist in histos:
    Popen('root -b -l -q \'ControlPlots_2D.C(\"'+folder+'\",\"'+hist+'\",\"True\")\'',shell=True).wait()
    #Popen('root -b -l -q \'ControlPlots.C(\"'+folder+'\",\"'+hist+'\",\"True\")\'',shell=True).wait()
    #Popen('root -b -l -q \'rochStudy.C(\"'+folder+'\",\"'+hist+'\",\"True\")\'',shell=True).wait()
