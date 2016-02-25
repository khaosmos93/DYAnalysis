#!/usr/bin/env python

from subprocess import Popen

histos = ['invm'] #,'relPFisoNoEGamma', 'chi2dof','trackerHits','pixelHits','CosAngle','muonHits','nMatches','dxyBS','relPFisoNoEGamma','vtxTrkProb','trigMatches','pT','eta'] #['invm']

eras = ['RunAB']
#analyzers
for run in eras:
    for hist in histos:
        Popen('root -b -l -q \'analyseYield_p3.C(\"'+hist+'\",\"False\",\"'+run+'\")\'',shell=True).wait()
