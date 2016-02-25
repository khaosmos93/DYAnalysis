#!/usr/bin/env python

from subprocess import Popen

Popen('root -b -q -l \'ResMatrix.C(\"''\")\'',shell=True).wait() 
