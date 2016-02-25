#!/usr/bin/env python

import glob
from subprocess import Popen

fsrFlags = ['pre','post']
accFlag = 'inAcc'
normTypes = ['rshape','absolute','normalized']

for fsrFlag in fsrFlags:
    for norm_type in normTypes:
        Popen('rm new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('touch new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_20_30_vs_20_30_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_20_30_vs_30_45_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_20_30_vs_45_60_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_20_30_vs_60_120_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_20_30_vs_120_200_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_20_30_vs_200_1500_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()

        Popen('cat cov_30_45_vs_20_30_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_30_45_vs_30_45_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_30_45_vs_45_60_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_30_45_vs_60_120_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_30_45_vs_120_200_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_30_45_vs_200_1500_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()

        Popen('cat cov_45_60_vs_20_30_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_45_60_vs_30_45_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_45_60_vs_45_60_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_45_60_vs_60_120_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_45_60_vs_120_200_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_45_60_vs_200_1500_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()

        Popen('cat cov_60_120_vs_20_30_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_60_120_vs_30_45_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_60_120_vs_45_60_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_60_120_vs_60_120_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_60_120_vs_120_200_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_60_120_vs_200_1500_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()

        Popen('cat cov_120_200_vs_20_30_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_120_200_vs_30_45_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_120_200_vs_45_60_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_120_200_vs_60_120_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_120_200_vs_120_200_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_120_200_vs_200_1500_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()

        Popen('cat cov_200_1500_vs_20_30_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_200_1500_vs_30_45_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_200_1500_vs_45_60_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_200_1500_vs_60_120_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_200_1500_vs_120_200_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()
        Popen('cat cov_200_1500_vs_200_1500_'+fsrFlag+'_'+accFlag+'_'+norm_type+'.out >> new_final/cov2D_mu_'+fsrFlag+'FSR_'+accFlag+'_'+norm_type+'.out',shell=True).wait()

