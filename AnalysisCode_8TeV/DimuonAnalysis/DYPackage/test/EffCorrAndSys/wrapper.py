#!/usr/bin/env python
from subprocess import Popen

#merge files
def stringer(inp_list):
    out_string = ''
    for item in inp_list:
        out_string += item+' '
    return out_string

#step1 produce base denominator
python analyse_Base.py
#step2 produce the reweightied distributions (includes also hadder)
python analyseCorrSyst.py
#analyseEffSyst - map variations mode
#analyseEffSyst - hadd mode
#reset the errors with mapRMSParser.py - touches only the variances 
#might need to rerun each time you manipulate the variances
Popen('python mapRMSParser.py',shell=True).wait()
#effCorrMass.py - calculate the efficiency correction factors
Popen('python effCorrMass.py',shell=True).wait()
Popen('python effCorrMass_reps.py',shell=True).wait()
#estimate correlations in two steps, step1 effCorrelEstimator.py
#outfolder already on scratrch
Popen('python effCorrelEstimator.py',shell=True).wait()
import os,glob
#####merge
folder = "/scratch/scratch95/a/asvyatko/correlReps/"
input = glob.glob(folder+'*.root')
os.system("hadd -f "+folder+"1.root "+stringer(input))
#####run below macro
Popen('root -b -q -l \'effCorrelEstimator_step2.C()\'',shell=True).wait()
###next continues in CovMat folder - reestimate cov matrix and chi2 tests
