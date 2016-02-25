#!/usr/bin/env python

from subprocess import Popen

Popen("python analyseYield_mc.py",shell=True).wait()
Popen("python analyseYield_data.py",shell=True).wait()
