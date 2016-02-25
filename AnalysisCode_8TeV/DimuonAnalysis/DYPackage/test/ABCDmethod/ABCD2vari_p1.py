#!/usr/bin/env python

from subprocess import Popen

Popen('root -b -q -l \'ABCD2vari_p1.C(\"'+'mu1iso'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'ABCD2vari_p1.C(\"'+'mu2iso'+'\")\'',shell=True).wait()
