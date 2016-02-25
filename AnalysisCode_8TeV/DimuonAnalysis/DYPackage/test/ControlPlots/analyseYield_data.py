#!/usr/bin/env python

from subprocess import Popen

histos = ['invm'] #,'relPFisoNoEGamma', 'chi2dof','trackerHits','pixelHits','CosAngle','muonHits','nMatches','dxyBS','relPFisoNoEGamma','vtxTrkProb','trigMatches','pT','eta'] #['invm']

eras = ['RunAB','RunC1','RunC2','RunD1','RunD2'] #RunAB
#analyzers
for run in eras:
    for hist in histos:
        Popen('root -b -l -q \'analyseYield_data.C(\"'+hist+'\",\"False\",\"'+run+'\")\'',shell=True).wait()
