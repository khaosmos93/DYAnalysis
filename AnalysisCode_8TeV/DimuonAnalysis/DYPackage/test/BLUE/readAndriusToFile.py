#!/usr/bin/env python

from ROOT import *
import numpy as np

input = open("covariance_ee_NewAndrius.dat","r").readlines()
mat2D = np.zeros((3,1600))
mat2D = np.loadtxt("covariance_ee_NewAndrius.dat")
#i j 40x40

fout = TFile("covariance_ee_NewAndrius.root","recreate")
hout = TH1D("","",,)
