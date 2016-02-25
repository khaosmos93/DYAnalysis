#!/usr/bin/env python

from subprocess import Popen
import os

histos = ['invm'] #,'relPFisoNoEGamma', 'chi2dof','trackerHits','pixelHits','CosAngle','muonHits','nMatches','dxyBS','relPFisoNoEGamma','vtxTrkProb','trigMatches','pT','eta'] #['invm']

#analyzers
for hist in histos:
    Popen('root -b -l -q \'analyseYield_mc.C(\"'+hist+'\",\"False\",\"RunAB\")\'',shell=True).wait()
