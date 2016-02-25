#!/usr/bin/env python

from subprocess import Popen

Popen('root -b -q -l \'ABCD2vari_p4.C(\"'+'mu1iso'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'ABCD2vari_p4.C(\"'+'mu2iso'+'\")\'',shell=True).wait()
