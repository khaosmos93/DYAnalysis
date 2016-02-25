#!/usr/bin/env python

from subprocess import Popen
import sys
import os

scenarios = ['trig','recoid','iso']
BINNING = str(sys.argv[1])
RUNSPLIT = str(sys.argv[2])

REPLICA = str(sys.argv[3])
SYST = ''
if float(REPLICA) > 0: 
    SYST = '_syst' 
else:
    REPLICA = ''

PUmarker = ''
if len(sys.argv[4:]) != 0:
    PUmarker = str(sys.argv[4:][0])

print "PUmarker ", PUmarker, " ", len(sys.argv[4:]), " ", sys.argv[4:]

for scenario in scenarios:
    print 'Source files'
    print './outputs/'+scenario+'/DYM1020_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'
    print './outputs/'+scenario+'/DYM20_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'
    print './outputs/'+scenario+'/DYM1020_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root'
    print './outputs/'+scenario+'/DYM20_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root' 
    print './outputs/'+scenario+'/DYM500_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'
    print './outputs/'+scenario+'/DYM500_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root'
    print './outputs/'+scenario+'/DYM800_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'
    print './outputs/'+scenario+'/DYM800_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root'
    print 'Target files'
    print './outputs/'+scenario+'/DY_data_'+scenario+RUNSPLIT+SYST+REPLICA+'.root '
    print './outputs/'+scenario+'/DY_mc_'+scenario+PUmarker+SYST+REPLICA+'.root '
    if os.path.isfile('./outputs/'+scenario+'/DYM1020_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM20_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM500_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM800_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root'):
        Popen('hadd -f '+'./outputs/'+scenario+'/DY_data_'+scenario+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM1020_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root '+'./outputs/'+scenario+'/DYM20_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root'+'./outputs/'+scenario+'/DYM500_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root '+'./outputs/'+scenario+'/DYM800_data_'+scenario+RUNSPLIT+SYST+REPLICA+'_'+BINNING+'.root',shell=True).wait()
    else: print "WARNING: one of the raw inoput files was not produced! Check the log files."
    if os.path.isfile('./outputs/'+scenario+'/DYM1020_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM20_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM500_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM800_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'):
        #if RUNSPLIT != "runAB": print "WARNING: We should not have new MC outputs for this run! Check garbage collection procedure"
        Popen('hadd -f '+'./outputs/'+scenario+'/DY_mc_'+scenario+PUmarker+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM1020_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root '+'./outputs/'+scenario+'/DYM20_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'+'./outputs/'+scenario+'/DYM500_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root'+'./outputs/'+scenario+'/DYM800_mc_'+scenario+PUmarker+SYST+REPLICA+'_'+BINNING+'.root',shell=True).wait()
