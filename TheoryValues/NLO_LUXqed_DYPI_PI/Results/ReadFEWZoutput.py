Version = "20170628"

from ROOT import TFile, TH1D
from array import array

def GetXsec( FileName ):
	print FlieName
	xSec = 0.0;
	IntgErr = 0.0;
	PDFErr_plus = 0.0;
	PDFErr_minus = 0.0;
	PDFErr = 0.0;

	f = open(FileName, "r")
	lines = f.readlines()

	Next_PDFError = 0
	for line in lines:
		if "Sigma (pb)" in line:
			items = line.split(" ")
			for item in items:
				if isNumber( item ):
					xSec = float(item);
					break;

		elif "Integration Error (pb)" in line:
			items = line.split(" ")
			for item in items:
				if isNumber( item ):
					IntgErr = float(item);
					break;

		elif "PDF Error (pb)" in line:
			items = line.split(" ")
			for item in items:
				if isNumber( item ):
					PDFErr_plus = float(item)
					Next_PDFError = 1
					break;

		elif Next_PDFError == 1:
			items = line.split(" ")
			for item in items:
				if isNumber( item ):
					PDFErr_minus = float(item)
					Next_PDFError = 0
					break;


	print "x-section = %lf, integration error = %lf, PDFError(+,-) = (%lf, %lf)\n" % ( xSec, IntgErr, PDFErr_plus, PDFErr_minus )

	if PDFErr_plus > PDFErr_minus:
		PDFErr = PDFErr_plus
	else:
		PDFErr = PDFErr_minus

	return [xSec, IntgErr, PDFErr]

def isNumber(s):
	try:
		float(s)
		return True
	except ValueError:
		return False

def MakeHistogram( process, values ):
	nMassBin = 43
	MassBinEdges = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
				 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
				 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
				 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
				 830, 1000, 1500, 3000]

	h = TH1D("h_"+process, "", nMassBin, array("d", MassBinEdges) )

	i = 0
	for value in values:
		i_bin = i+1;

		h.SetBinContent(i_bin, value[0])
		h.SetBinError(i_bin, value[1]) # -- PDF error: will be cancelled when (DY+PI)/DY ratio calcalation -- #

		i = i+1;

	return h

# -- main part -- #
nMassBin = 43
MassBinEdges = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
			 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
			 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
			 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
			 830, 1000, 1500, 3000]

values_DY = []
values_DYPI = []

for i in range(0, nMassBin):
	LowerEdge = MassBinEdges[i]
	UpperEdge = MassBinEdges[i+1]

	MassRange = "M%.0fto%.0f" % (LowerEdge, UpperEdge)

	Process = "DY"
	FlieName = "NLO.v"+Version+"_"+MassRange+"_"+Process+"_NLO_LUXqed_plus_PDF4LHC15_nnlo_100.dat"
	value_DY = GetXsec( FlieName )
	values_DY.append( value_DY )

	Process = "DYPI"
	FlieName = "NLO.v"+Version+"_"+MassRange+"_"+Process+"_NLO_LUXqed_plus_PDF4LHC15_nnlo_100.dat"
	value_DYPI = GetXsec( FlieName )
	values_DYPI.append( value_DYPI )

h_DY = MakeHistogram( "DY", values_DY )
h_DYPI = MakeHistogram( "DYPI", values_DYPI )

h_DY.Sumw2()
h_DYPI.Sumw2()

h_ratio = h_DY.Clone()
h_ratio.SetName("h_ratio")
h_ratio.Divide( h_DYPI, h_DY )

f_output = TFile("ROOTFile_DYPI_DY_FEWZ.root", "RECREATE")

f_output.cd()
h_DY.Write()
h_DYPI.Write()
h_ratio.Write()

f_output.Close()