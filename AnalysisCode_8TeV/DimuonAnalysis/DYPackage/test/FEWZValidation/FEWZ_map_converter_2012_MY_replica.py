#!/usr/bin/env python
import numpy as np
import array
from subprocess import Popen
import sys

#Number of Y bins, as much as you put in your FEWZ histo
nYbins = 24
nreplicas = 100
thisreplica = int(sys.argv[2]) # pass the number of eigenvectors from the command line

mass_cuts = open('input_masses2D.txt','r') # same binning, but not yopu should remove ".0d0" in the end of each number
masses = mass_cuts.readlines()
mass_cuts.close()

for imas in range(len(masses)-1):

    #load this file
    x_onemass_allreplicas = np.zeros(nYbins*nreplicas)
    tmp2 = open('tmp2'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat','r')
    x_onemass_allreplicas = np.loadtxt(tmp2) 
    row1,col1 = x_onemass_allreplicas.shape
    #print row1,col1

    #array holding columns per Pt bin only positive Ys as spit out by FEWZ
    x_onemass_thisreplica = np.zeros((nYbins,2)) #2 because holds also errors
    xfinal = np.zeros((1,4*nYbins))

    icount = -1
    print x_onemass_allreplicas
    for j in range(col1-1):
        for i in range(thisreplica,row1,nreplicas):    
            icount += 1
            #print i," ",j
            #print i-icount*(nreplicas-1)," ",j
            #values 
            x_onemass_thisreplica[i-icount*(nreplicas-1)-thisreplica,j] = x_onemass_allreplicas[i,j]
            #errors
            x_onemass_thisreplica[i-icount*(nreplicas-1)-thisreplica,j+1] = x_onemass_allreplicas[i,j+1]

    #print x_onemass_thisreplica,'\n\n'
    outmp = open('outmp'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_rep_'+str(thisreplica)+'.dat','w')
    np.savetxt(outmp,x_onemass_thisreplica)
    outmp.close()

    #print "x_onemass_thisreplica shape ", x_onemass_thisreplica.shape

    #transpose the x_onemass_thisreplica
    row2,col2 = x_onemass_thisreplica.shape
    x3 = np.zeros((col2/2,2*row2))
    #print x3,'\n\n'
    #print "x3 shape ", x3.shape
    for i in range(row2):
        for j in range(col2):
             #even numbers (values)
            if j%2 == 0:
                #print "Initial indices: ", i," ", j, " final indices ", j/2, " ", 2*i
                x3[j/2,2*i] = x_onemass_thisreplica[i,j]
            else:
                x3[(j-1)/2,2*i+1] = x_onemass_thisreplica[i,j]
            #print "x_onemass_thisreplica ", x_onemass_thisreplica[0][0]," ",x_onemass_thisreplica[0][1],"    ", "x3 ", x3[0][0]," ",x3[0][1]
 
    #print x3,'\n\n'
    #print x3[5,6], " ", x_onemass_thisreplica[6,5]

    #build the matrix symmetric wrt two central columns (which are equal also)
    row3,col3 = x3.shape     
    #print x3.shape
    #print xfinal.shape
    for i in range(row3):
        for j in range(col3):
            #even numbers
            #print i," ",j," " 
            if j%2 == 0:
                xfinal[i,4+j] = x3[i,j]
                xfinal[i,4-j-2] = x3[i,j]
                #print "x3 ", x3[i,j]," xfinal ", xfinal[i,j]
            #odd numbers (errors)
            else:
                xfinal[i,4+j] = x3[i,j]
                xfinal[i,4-j] = x3[i,j]
                #print j, " efirst ", i, " ", j, "  other ", i,2*len(files)*2-j
                #print j, " err ", x3[i,2*len(files)-j]
    tmp2.close()
    #print xfinal

    oufinal = open('map'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_rep_'+str(thisreplica)+'.dat','w')
    np.savetxt(oufinal,xfinal[:1,:]) #save at the very end
    oufinal.close()
