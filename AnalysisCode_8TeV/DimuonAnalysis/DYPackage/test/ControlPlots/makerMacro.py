#!/usr/bin/env python

#masses = ['15','20','30','60','120','300','1500'] #["15", "20", "25", "30", "35", "40", "45", "50", "55", "60", "64", "68", "72", "76", "81", "86", "91",
            #"96", "101", "106", "110", "115", "120", "126", "133", "141", "150", "160", "171", "185",
            #"200", "220", "243", "273", "320", "380", "440", "510", "600", "1000", "1500"]

masses = ["15", "20", "25", "30", "35", "40", "45", "50", "55", "60", "64", "68", "72", "76", "81", "86", "91",
          "96", "101", "106", "110", "115", "120", "126", "133", "141", "150", "160", "171", "185",
          "200", "220", "243", "273", "320", "380", "440", "510", "600", "1000", "1500"]

#masses = ['15','20','30','45','60','72','106','120','133','150','171','200','400','1500']

#   xmap_POW3001500 = new TH2D("xsec_3001500bin","xsec_3001500bin",7, Ybins, 20, PTbins);

#for bin in range(len(masses)-1):
    #print " GetOutputList()->Add(xmap_POW"+masses[bin]+masses[bin+1]+");"
    #print "      if (mass > ", masses[bin]," && mass < ",masses[bin+1],") {"
    #print "          return weights_fewz_"+masses[bin]+masses[bin+1]+"[index_pt][index_rap];"
    #print "    xmap_POW"+masses[bin]+masses[bin+1]+ "= new TH2D(\"xsec_"+masses[bin]+masses[bin+1]+"bin\",\"xsec_"+masses[bin]+masses[bin+1]+"bin\",1, Ybins, 20, PTbins);" 
    #print "GetOutputList()->Add(xmap_POW"+masses[bin]+masses[bin+1]+");"
    #print "      if (genMass >= "+masses[bin]+" && genMass < "+masses[bin+1]+") {"
    #print "         xmap_POW"+masses[bin]+masses[bin+1]+"->Fill(fabs(genRapidity),genDiMuPt,WEIGHT);" 
    #print "         validFEWZ"+str(bin+1)+"->Fill(FEWZ_WEIGHT);"
    #print "       }"
    #print "    xmap_POW"+masses[bin]+masses[bin+1]+" = dynamic_cast<TH2D*>(fOutput->FindObject(Form(\"xsec_"+masses[bin]+masses[bin+1]+"bin\")));"
    #print "      xmap_POW"+masses[bin]+masses[bin+1]+"->Scale(1./Nntuple->Integral());"
    #print "   xmap_POW"+masses[bin]+masses[bin+1]+"->Write();"
    #print "   TH2D* xmap_POW"+masses[bin]+masses[bin+1]+";"

for i in range(1,41):
    #print  "   validFEWZ"+str(i)+" = new TH1D(\"validFEWZ"+str(i)+"\",\"validFEWZ"+str(i)+"\",300,0,3);"
    #print "   GetOutputList()->Add(validFEWZ"+str(i)+");"
    #print    "validFEWZ"+str(i)+" = dynamic_cast<TH1D*>(fOutput->FindObject(Form(\"validFEWZ"+str(i)+"\")));"
    print    "validFEWZ"+str(i)+"->Write();"
