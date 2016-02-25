#!/usr/bin/env python

from subprocess import Popen
import sys

pdfset = sys.argv[1]
nreplicas = 100

Popen("python FEWZ_replica_preprocessor.py "+pdfset,shell=True).wait()
Popen("rm tmp1_*",shell=True).wait()

for i in range(nreplicas):
   Popen("python FEWZ_map_converter_2012_MY_replica.py "+pdfset+" "+str(i),shell=True).wait()

Popen("rm tmp2*",shell=True).wait()
