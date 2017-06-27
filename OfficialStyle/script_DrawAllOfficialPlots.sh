#!bin/bash

root -b -q dimuonMass.C
root -b -q DrawRelUnc_Summary_LogY.cxx

# -- comparison with various PDF above 200 GeV -- #

cd Comparison_VariousPDF

root -b -q InputFileMaker.cxx
root -b -q 'Comparison_VariousPDF.cxx("MM")'
root -b -q 'Comparison_VariousPDF.cxx("EE")'
root -b -q 'Comparison_VariousPDF.cxx("LL")'

# -- comparison with DY+PI -- #
cd ../Comparison_DYPI_PI
root -b -q Ratio_TheoryToData.cxx

echo "finished"
