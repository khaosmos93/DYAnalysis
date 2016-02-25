#!/usr/bin/env python

from subprocess import Popen
import sys
import os

scenarios = ['trig','recoid','iso']
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
    print './outputs/'+scenario+'/DY_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '
    if os.path.isfile('./outputs/'+scenario+'/DYM1020_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM20_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM500_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM800_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM200_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM400_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM700_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root') and os.path.isfile('./outputs/'+scenario+'/DYM1000_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root'):
    #    print "Hadding good"
    #    #if RUNSPLIT != "runAB": print "WARNING: We should not have new MC outputs for this run! Check garbage collection procedure"
        Popen('hadd -f '+'./outputs/'+scenario+'/DY_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM1020_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM20_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM200_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM400_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM500_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM800_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM1000_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root '+'./outputs/'+scenario+'/DYM700_'+scenario+PUmarker+RUNSPLIT+SYST+REPLICA+'.root ',shell=True).wait()
    #else: "Something does not exist"
