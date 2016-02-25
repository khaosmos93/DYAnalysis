#!/usr/bin/env python

from subprocess import Popen

#const char * name_dv1, const char * name_cm1, const int length1, const char * name_dv2, const char * name_cm2, const int length2, int ftr = 1, const char * outputNameStub = "output"

#data vector 1
name_dv1 =  'ee_results_2014_abs_adhoc_PI_Bayes.dat'
#covariance matrix 1
#covariance is always for rhsape for now
name_cm1 = 'covForBlue-1D-absolute_EE_Bayes.dat'
#size
length1 = '41'
#data vector 2
name_dv2 = 'mumu_results_2014_abs_PI_Bayes.dat'
#covariance matrix 2
#covariance is always for rhsape for now
name_cm2 = 'covForBlue-1D-absolute_MuMu.dat'
#length2
length2 = '41' 

#analyzers
Popen('root -b -l -q \'resultCombiner.C(\"'+name_dv1+'\",\"'+name_cm1+'\",\"'+length1+'\",\"'+name_dv2+'\",\"'+name_cm2+'\",\"'+length2+'\")\'',shell=True).wait()
#Popen('root -b -l -q \'resultCombiner_2D.C(\"'+name_dv1+'\",\"'+name_cm1+'\",\"'+length1+'\",\"'+name_dv2+'\",\"'+name_cm2+'\",\"'+length2+'\")\'',shell=True).wait()
