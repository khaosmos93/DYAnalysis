#!/usr/bin/env python

from subprocess import Popen

rshapeTypes = ['_inAcc'] #['_inAccPostFSR','_inAcc','_fullAcc','_fullAccPostFSR'] #['_inAcc'] #['_fullAcc'] #['_inAcc'] #['','_PostFSR','_inAcc','_inAccPostFSR']

eras = ['_'] #['RunAB','RunC','RunD','_'] #'POWHEG']#,'_']
#analyzers
for run in eras:
    for rshape in rshapeTypes:
        if rshape == '_inAcc' or rshape == '_inAccPostFSR': Popen('root -b -q -l \'shapeR2D.C(\"'+rshape+'",\"'+'False'+'",\"'+'False'+'\",\"'+run+'\",\"'+''+'\")\'',shell=True).wait()
        else: Popen('root -b -q -l \'shapeR2D.C(\"'+rshape+'",\"'+'False'+'",\"'+'False'+'\",\"'+run+'\",\"'+'corr'+'\")\'',shell=True).wait() 
 
