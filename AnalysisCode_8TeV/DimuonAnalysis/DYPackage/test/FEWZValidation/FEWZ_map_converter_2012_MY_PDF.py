#!/usr/bin/env python
import numpy as np
import array
from subprocess import Popen
import sys

from optparse import OptionParser
parser = OptionParser(usage="usage: %prog [PDF set name]")

(options, args) = parser.parse_args()

#Number of Y bins, as much as you put in your FEWZ histo
nYbins = 24

mass_cuts = open('input_masses2D.txt','r') # same binning, but not yopu should remove ".0d0" in the end of each number
masses = mass_cuts.readlines()
mass_cuts.close()

for imas in range(len(masses)-1):

    inputFiles = [
      'NNLO'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat',
    ]
    #debug and verify
    #for file in inputFiles:
    #    print file

    #will read the files, keep last 10 lines and 
    #save it in the file with the name required below

    for file in inputFiles:
        
        ifile = open(file,'r')
        io = ifile.readlines()
        ifile.close()

        oufinal_tmpw = open('tmp'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'.dat','w')

        #just keep the 12 last lines
        iline = 0
        for line in io:
            iline += 1
            if iline > (len(io)-nYbins): oufinal_tmpw.write(line)
        oufinal_tmpw.close() 


for imas in range(len(masses)-1):

    files = ['tmp'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'.dat',
    ]

    #allocate an array
    xtmp = np.zeros(1000)
    #array holding columns per Pt bin only positive Ys as spit out by FEWZ
    x2 = np.zeros((nYbins,2*len(files))) #2 because holds also errors
    xfinal = np.zeros((len(files),4*nYbins))

    findex = 0
    oufinal = open('map'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'.dat','w')

    for file in files:
        x1 = np.zeros(1000)

        xtmp = np.loadtxt(file)
        outmp = open('outmp'+file,'w')
        #FIXME
        outmp_PDFu = open('fewz_'+file,'w')
        #keep only columns 2 and 3 the rest is meaningless 
        #FIXME here I would like to keep columns 4 and 5 - the PDF uncertainties
        np.savetxt(outmp,xtmp[:,1:3])
        outmp.close()    
        #FIXME this is tmp
        np.savetxt(outmp_PDFu,xtmp[:],'%10.9f')
        outmp_PDFu.close()
        outmp = open('outmp'+file,'r')
        x1 = np.loadtxt(outmp) 

        #print x1,'\n\n'

        #xfinal[] = x1[:4,:]
        row1,col1 = x1.shape
        #print row1,col1

        print findex

        for i in range(row1):    
            for j in range(col1-1): #binning in Pt is fixed and equal to number of files now
 
                #values 
                x2[i,2*findex+j] = x1[i,j]
                #errors
                x2[i,2*findex+j+1] = x1[i,j+1]

        #print x2,'\n\n'
        #print "x2 shape ", x2.shape

        #transpose the x2
        row2,col2 = x2.shape
        x3 = np.zeros((col2/2,2*row2))
        #print x3,'\n\n'
        #print "x3 shape ", x3.shape
        for i in range(row2):
            for j in range(col2):
                #even numbers (values)
                if j%2 == 0:
                    #print "Initial indices: ", i," ", j, " final indices ", j/2, " ", 2*i
                    x3[j/2,2*i] = x2[i,j]
                else:
                    x3[(j-1)/2,2*i+1] = x2[i,j]
                #print "x2 ", x2[0][0]," ",x2[0][1],"    ", "x3 ", x3[0][0]," ",x3[0][1]
 
        print x3,'\n\n'
        #print x3[5,6], " ", x2[6,5]

        #build the matrix symmetric wrt two central columns (which are equal also)
        row3,col3 = x3.shape     
        #print x3.shape
        #print xfinal.shape
        for i in range(row3):
            for j in range(col3):
                #even numbers
                #print i," ",j," " 
                if j%2 == 0:
                    xfinal[i,4*len(files)+j] = x3[i,j]
                    xfinal[i,4*len(files)-j-2] = x3[i,j]
                    #print "x3 ", x3[i,j]," xfinal ", xfinal[i,j]
                #odd numbers (errors)
                else:
                    xfinal[i,4*len(files)+j] = x3[i,j]
                    xfinal[i,4*len(files)-j] = x3[i,j]
                    #print j, " efirst ", i, " ", j, "  other ", i,2*len(files)*2-j
                    #print j, " err ", x3[i,2*len(files)-j]
        outmp.close()
        findex += 1
    #print xfinal

    np.savetxt(oufinal,xfinal[:len(files),:]) #save at the very end
    oufinal.close()
