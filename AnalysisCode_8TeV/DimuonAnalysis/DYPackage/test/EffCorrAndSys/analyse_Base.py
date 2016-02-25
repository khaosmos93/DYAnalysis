#!/usr/bin/env python

from subprocess import Popen
import os

Popen('root -b -q -l \'analyse_Base.C()\'',shell=True).wait()
#Popen('hadd -f DY_base.root DYM1020.root DYM20.root ',shell=True).wait()
