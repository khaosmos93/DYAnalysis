#!/usr/bin/env python

from subprocess import Popen

#Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu1iso'+'\",\"'+'A'+'\")\'',shell=True).wait() 
Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu1iso'+'\",\"'+'B'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu1iso'+'\",\"'+'C'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu1iso'+'\",\"'+'D'+'\")\'',shell=True).wait()

#Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu2iso'+'\",\"'+'A'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu2iso'+'\",\"'+'B'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu2iso'+'\",\"'+'C'+'\")\'',shell=True).wait()
Popen('root -b -q -l \'QCDFrac_p5.C(\"'+'mu2iso'+'\",\"'+'D'+'\")\'',shell=True).wait()
