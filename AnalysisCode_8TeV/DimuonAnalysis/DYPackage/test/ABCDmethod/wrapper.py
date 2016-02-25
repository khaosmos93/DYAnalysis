#!/usr/bin/env python

from subprocess import Popen

Popen("python QCDFrac_p1.py",shell=True).wait()
Popen("python QCDFrac_p2.py",shell=True).wait()
Popen("python QCDFrac_p3.py",shell=True).wait()
Popen("python QCDFrac_p4.py",shell=True).wait()
Popen("python qcdFracHadder.py rootfiles1",shell=True).wait()
Popen("python ABCD2vari_init.py",shell=True).wait()
Popen("python ABCD2vari_p1.py",shell=True).wait()
Popen("python ABCD2vari_p2.py",shell=True).wait()
Popen("python ABCD2vari_p3.py",shell=True).wait()
Popen("python ABCD2vari_p4.py",shell=True).wait()
