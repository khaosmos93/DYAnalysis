#!/usr/bin/env python

from subprocess import Popen
import os

#make shared libraries
Popen('rm -r proofbox',shell=True).wait()
Popen('mkdir proofbox',shell=True).wait()
Popen('mkdir proofbox/cache',shell=True).wait()
Popen('cp ./purdue_lib/*.d ./purdue_lib/*_cc.so ./proofbox/cache/',shell=True).wait()
os.environ["LD_LIBRARY_PATH"] = os.environ["LD_LIBRARY_PATH"] + ":/home/asvyatko/DYStudy/CMSSW_5_3_3_patch2/src/Analysis/DYPackage/test/FSRunfold_Purdue/proofbox/cache"
#set
#setenv LD_LIBRARY_PATH

#edit this depending on where you run
def getAlt(dataset):
    return ''

regimes = [,'EE','EMu']

#FIXME chunk like processing from hadoop
datasetNames = ['EEDYM1000_Purdue_ES','EEDYM1020_Purdue_ES','EEDYM1500_Purdue_ES','EEDYM2000_Purdue_ES','EEDYM200_Purdue_ES','EEDYM20_Purdue_ES_samp2','EEDYM400_Purdue_ES','EEDYM500_Purdue_ES','EEDYM700_Purdue_ES','EEDYM800_Purdue_ES','DYM1000_Purdue_ES','DYM1020_Purdue_ES','DYM1500_Purdue_ES','DYM2000_Purdue_ES','DYM200_Purdue_ES','DYM20_Purdue_ES_samp2','DYM400_Purdue_ES','DYM500_Purdue_ES','DYM700_Purdue_ES','DYM800_Purdue_ES']
chunks = ['1_','10','11','12','13','14','15','16','17','18','19','2_','20','22','23','24','25','26','27','28','29','3_','30','31','32','33','34','35','36','37','38','39','4_','40','41','42','43','44','45','46','47','48','49','5','6','7','8','9']

#analyzers
for reg in regimes:
    for dataset in datasetNames:
        #skip mumu datasets for EE 
        if reg == 'EE' and 'EE' not in dataset: continue
        if reg == 'MuMu' and 'EE' in dataset: continue
        for chunk in chunks:
             Popen('root -b -l -q \'FSRResMatrix.C(\"'+reg+'\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait() 
             Popen('root -b -l -q \'FSRResMatrixExtra.C(\"'+reg+'\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait()
             Popen('root -b -l -q \'FSRBinByBin.C(\"'+reg+'\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait()


#postprocessing
#merge files
#os.system("hadd -f FSRresMatrixProd_tmp.root DYM1020.root DYM20.root DYM200.root DYM400.root DYM500.root DYM700.root DYM800.root DYM1000.root")
#os.system("hadd -f inputCorrinMC_tmp.root DYM1020_2.root DYM20_2.root DYM200_2.root DYM400_2.root DYM500_2.root DYM700_2.root DYM800_2.root DYM1000_2.root")
#os.system("hadd -f BinByBin.root DYM1020_BBB.root DYM20_BBB.root DYM200_BBB.root DYM400_BBB.root DYM500_BBB.root DYM700_BBB.root DYM800_BBB.root DYM1000_BBB.root")

#produce additional corrections
#Popen('root -b -q -l \'fracEff.C()\'',shell=True).wait()
#Popen('root -b -q -l \'fracEff_corr.C()\'',shell=True).wait()

#copy files to main location
#os.system("cp FSRresMatrixProd_tmp.root ../Inputs/FSR/")
#os.system("cp addMoreCorrinMC_tmp.root ../Inputs/FSR/")
#os.system("cp BinByBin.root ../Inputs/FSR/")
