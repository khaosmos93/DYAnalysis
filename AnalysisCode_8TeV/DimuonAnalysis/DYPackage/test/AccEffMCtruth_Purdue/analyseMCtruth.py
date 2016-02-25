#!/usr/bin/env python

from subprocess import Popen
from ROOT import *
import datetime

now = datetime.datetime.now()

Popen('root -b -l -q \'analyseMCtruth.C()\'',shell=True).wait()
