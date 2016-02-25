#!/usr/bin/env python

from subprocess import Popen
import os

#edit this depending on where you run
def getAlt(dataset):
    return 'rootd'

def populator(folder):
    print glob.glob(folder+'resMatrixProd_RunAB*.root')
    for file in glob.glob(folder+'resMatrixProd_RunAB*.root'):
        print 'cp '+file+' '+file[:-11]+'C_tmp.root'
        Popen('cp '+file+' '+file[:-11]+'C_tmp.root',shell=True).wait()
        Popen('cp '+file+' '+file[:-11]+'D_tmp.root',shell=True).wait()
        Popen('cp '+file+' '+file[:-8]+'POWHEG_tmp.root',shell=True).wait()

#FIXME chunk like processing from hadoop
#datasetNames = ['EEDYM800_PUOct','EEDYM20_PUOct','EEDYM1000_PUOct','EEDYM1020_PUOct_FULL','EEDYM1500_PUOct','EEDYM2000_PUOct','EEDYM200_PUOct','EEDYM400_PUOct','EEDYM500_PUOct','EEDYM700_PUOct']
datasetNames = ['EEDYM20_PUOct']

#even chunks
#chunks = ['1_','10','11','12','13','14','15','16','17','18','19','2_','20','22','23','24','25','26','27','28','29','3_','30','31','32','33','34','35','36','37','38','39','4_','40','41','42','43','44','45','46','47','48','49','5','6','7','8','9']
#chunks = ['22','23','24','25','26','27','28','29','3_','30','31','32','33','34','35','36','37','38','39','4_','40','41','42','43','44','45','46','47','48','49','5','6','7','8','9']
chunks = ['0','1','2','3','4','5','6','7','8','9','10'] #,'11','12','13','14','15','16','17','18','19'] 

#RECOVERY MODE
#dataChunks = {'EEDYM1020_PUOct_FULL':['12','38','40','41','42','43','45','46','47','48','49']}
#'DYM1500_PUOct':['21'],'DYM2000_PUOct':['21'],'DYM1000_PUOct':['21'],'DYM200_PUOct':['21'],'DYM400_PUOct':['21'],'DYM500_PUOct':['21'],'DYM700_PUOct':['21']}

#analyzers
for dataset in datasetNames:
    #skip mumu datasets for EE 
    for chunk in chunks:
         Popen('root -b -l -q \'FSRResMatrixEE.C(\"''\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait()
         Popen('root -b -q -l \'FSRResMatrixExtraEE.C(\"''\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait()
         Popen('root -b -q -l \'FSRBinByBinEE.C(\"''\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait() 

#analyzers
#for dataset in dataChunks.keys():
#    for chunk in dataChunks[dataset]:
#        print "Processing dataset ", dataset, " ", chunk
#        Popen('root -b -l -q \'analyseYieldEE_p2.C(\"''\",\"'+dataset+'\",\"'+chunk+'\",\"'+getAlt(dataset)+'\")\'',shell=True).wait()



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

