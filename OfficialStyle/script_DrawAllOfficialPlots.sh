#!bin/bash

root -b -q dimuonMass.C
root -b -q DrawRelUnc_Summary_LogY.cxx

# -- comparison with various PDF above 200 GeV -- #
root -b -q Comparison_VariousPDF/InputFileMaker.cxx
root -b -q 'Comparison_VariousPDF/Comparison_VariousPDF.cxx("MM")'
root -b -q 'Comparison_VariousPDF/Comparison_VariousPDF.cxx("EE")'
root -b -q 'Comparison_VariousPDF/Comparison_VariousPDF.cxx("LL")'

# -- comparison with DY+PI -- #
root -b -q Comparison_DYPI_PI/Ratio_TheoryToData.cxx

echo "finished"
