#!/usr/bin/env python
import numpy as np
import array
from subprocess import Popen
import sys

#Number of Y bins, as much as you put in your FEWZ histo
nYbins = 24
nreplicas = 100
nsects = 133

mass_cuts = open('input_masses2D.txt','r') 
masses = mass_cuts.readlines()
mass_cuts.close()

for imas in range(len(masses)-1):

    #but first we simply parser all that into an extra file
    file0 = open('tmp1_NNLO'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat','w')

    #parse a file for each section and output to the above array 
    for isect in range(nsects):

        isecFile = open('NNLO.sect'+str(isect+1)+'.pdf.NNLO'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat','r')
        io = isecFile.readlines()
        isecFile.close()

        #load all the necessary shit: keep the 24*nreplicas last lines
        iline = 0
        for line in io:
            iline += 1
            if iline > (len(io)-nYbins*nreplicas): file0.write(line)
    
    file0.close()
    file0 = open('tmp1_NNLO'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat','r')

    #holds the contents of file0  
    xtmp = np.zeros(5000000)
    xtmp = np.loadtxt('tmp1_NNLO'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat')

    #output this here after summing over all 133 sections
    outmp = open('tmp2'+masses[imas][:-5]+'to'+masses[imas+1][:-5]+'_'+sys.argv[1]+'.dat','w')

    xtmp_thismass_bin = xtmp[:,1:3]
    x_thismass_bin_nosects = np.zeros((nYbins*nreplicas,2))

    #now you need to sum every 2400th entry and save it to another array which would be created for each mass bin
    row1,col1 = xtmp_thismass_bin.shape

    icount = -1
    #print row1,col1
    for i in range(row1):
        if (i%(nreplicas*nYbins) == 0): icount += 1
        #print icount," ",j," ",i
        x_thismass_bin_nosects[i-icount*nYbins*nreplicas,0] += xtmp_thismass_bin[i,0]
        x_thismass_bin_nosects[i-icount*nYbins*nreplicas,1] += xtmp_thismass_bin[i,1]

    np.savetxt(outmp,x_thismass_bin_nosects)
    outmp.close()
