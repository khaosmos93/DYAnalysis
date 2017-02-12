from ROOT import gROOT, TColor, TROOT, TFile, TH1D, TCanvas, TPad, TLatex
from array import array
import math

# def GetXsec( FileName ):
# 	print FileName
# 	xSec = 0.0;
# 	IntgErr = 0.0;
# 	PDFErr_plus = 0.0;
# 	PDFErr_minus = 0.0;
# 	PDFErr = 0.0;

# 	f = open(FileName, "r")
# 	lines = f.readlines()

# 	Next_PDFError = 0
# 	for line in lines:
# 		if "Sigma (pb)" in line:
# 			items = line.split(" ")
# 			for item in items:
# 				if isNumber( item ):
# 					xSec = float(item);
# 					break;

# 		elif "Integration Error (pb)" in line:
# 			items = line.split(" ")
# 			for item in items:
# 				if isNumber( item ):
# 					IntgErr = float(item);
# 					break;

# 		elif "PDF Error (pb)" in line:
# 			items = line.split(" ")
# 			for item in items:
# 				if isNumber( item ):
# 					PDFErr_plus = float(item)
# 					Next_PDFError = 1
# 					break;

# 		elif Next_PDFError == 1:
# 			items = line.split(" ")
# 			for item in items:
# 				if isNumber( item ):
# 					PDFErr_minus = float(item)
# 					Next_PDFError = 0
# 					break;


# 	print "x-section = %lf, integration error = %lf, PDFError(+,-) = (%lf, %lf)\n" % ( xSec, IntgErr, PDFErr_plus, PDFErr_minus )

# 	if PDFErr_plus > PDFErr_minus:
# 		PDFErr = PDFErr_plus
# 	else:
# 		PDFErr = PDFErr_minus

# 	return [xSec, IntgErr, PDFErr]

def isNumber(s):
	try:
		float(s)
		return True
	except ValueError:
		return False

# def MakeHistogram( process, values ):
# 	nMassBin = 43
# 	MassBinEdges = [15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
# 				 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
# 				 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
# 				 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
# 				 830, 1000, 1500, 3000]

# 	h = TH1D("h_"+process, "", nMassBin, array("d", MassBinEdges) )

# 	i = 0
# 	for value in values:
# 		i_bin = i+1;

# 		h.SetBinContent(i_bin, value[0])
# 		h.SetBinError(i_bin, value[1]) # -- PDF error: will be cancelled when (DY+PI)/PI ratio calcalation -- #

# 		i = i+1;

# 	return h

def GetXsec( FileName ):
	print "GetXsec: process ", FileName
	binCenter_xsec = []

	f = open("./Results/"+FileName, "r")

	isFound = False
	lines = f.readlines()
	for line in lines:

		Numbers = []
		if isFound:
			if "----" in line: # -- if Inv.mass histogram is finished -- #
				break
			else:
				# print "\t", line
				for item in line.split():
					if isNumber(item):
						Numbers += [float(item)]

		# if( len(Numbers) >= 3 ):
		# 	print "\t(BinCenter, X-sec, Integration error) = (%lf, %lf, %lf)" % (Numbers[0], Numbers[1], Numbers[2])
		# 	binCenter_xsec.append( Numbers )

		if( len(Numbers) >= 4 ):
			print "\t(BinCenter, X-sec, Integration error, PDF error) = (%lf, %lf, %lf, %lf)" % (Numbers[0], Numbers[1], Numbers[2], Numbers[3])
			binCenter_xsec.append( Numbers )

		if "Q_ll Invaria" in line:
			isFound = True



	return binCenter_xsec

def GetXsec_SingleBin( FileName ):
	print "GetXsec_SingleBin: process ", FileName

	f = open("./Results/"+FileName, "r")

	MassRange = FileName.split("_")[1] # -- ex> M15to16 --#
	LowerEdge = float( MassRange[1:3] )
	UpperEdge = float( MassRange[5:7] )

	print "%.0f < M < %0.lf" % (LowerEdge, UpperEdge)

	BinCenter = (LowerEdge + UpperEdge) / 2.0;
	XSec = 0
	IntegErr = 0
	PDFErr = 0

	isFound = False
	lines = f.readlines()
	for line in lines:

		if isFound:
			if "Sigma (pb)" in line:
				for item in line.split():
					if isNumber(item):
						XSec = float(item)
						break

			elif "Integration Error (pb)" in line:
				for item in line.split():
					if isNumber(item):
						IntegErr = float(item)
						break

			elif "PDF Error (pb)" in line:
				for item in line.split():
					if isNumber(item):
						PDFErr = float(item)
						break
				break

		if "===========   RESULT" in line:
			isFound = True

	binCenter_xsec = []
	binCenter_xsec.append( [BinCenter, XSec, IntegErr, PDFErr] )
	print "[BinCenter = %.1lf] Xsec = %lf +- %lf (Integ.) +- %lf (PDF)" % (BinCenter, XSec, IntegErr, PDFErr )

	return binCenter_xsec


def MakeHistogram( MassBinEdges, FileNames ):
	h = TH1D("h_Xsec", "", len(MassBinEdges)-1, array("d", MassBinEdges) )

	BinCenter_Xsec = []
	for filename in FileNames:
		MassRange = filename.split("_")[1] # -- ex> M15to16 --#
		UpperEdge = float( MassRange.split("to")[1] )
		print "%s -> UpperEdge = %lf -> if UpperEdge <= 30: %d" % (filename, UpperEdge, UpperEdge <=30 )
		
		if UpperEdge <= 30:
			BinCenter_Xsec += GetXsec_SingleBin( filename )
		else:
			BinCenter_Xsec += GetXsec( filename )

	if len(BinCenter_Xsec) != len(MassBinEdges)-1:
		print "(# x-seciton, # mass bins) = (%d, %d) -> they are different! check the details" % ( len(BinCenter_Xsec), len(MassBinEdges)-1 )
		return h

	for i in range(0, len(MassBinEdges)-1):
		LowerEdge = MassBinEdges[i]
		UpperEdge = MassBinEdges[i+1]

		BinCenter_Xsec_ithBin = BinCenter_Xsec[i]

		BinCenter = BinCenter_Xsec_ithBin[0]
		if not(BinCenter > LowerEdge and BinCenter < UpperEdge):
			print "(LowerEdge, UpperEdge) = (%.0lf, %.0lf) ... BinCenter=%.0lf is not in this bin!" % (LowerEdge, UpperEdge, BinCenter)
			break

		i_bin = i+1
		CentralValue = BinCenter_Xsec_ithBin[1]
		h.SetBinContent( i_bin, CentralValue )

		IntgErr = BinCenter_Xsec_ithBin[2]
		PDFErr = BinCenter_Xsec_ithBin[3]

		# Error = BinCenter_Xsec_ithBin[2] # -- Integration error only -- #
		Error = math.sqrt( IntgErr*IntgErr + PDFErr*PDFErr )
		h.SetBinError( i_bin, Error )

	return h

def MakeDiffXsecHist( h_Xsec ):
	h_DiffXsec = h_Xsec.Clone("h_DiffXsec")

	nBin = h_Xsec.GetNbinsX()
	for i in range(0, nBin):
		i_bin = i+1

		Xsec = h_Xsec.GetBinContent(i_bin)
		XsecErr = h_Xsec.GetBinError(i_bin)

		LowerEdge = h_Xsec.GetBinLowEdge(i_bin)
		UpperEdge = h_Xsec.GetBinLowEdge(i_bin+1)
		BinSize = UpperEdge - LowerEdge

		DiffXsec = Xsec / BinSize
		DiffXsecErr = XsecErr / BinSize

		h_DiffXsec.SetBinContent(i_bin, DiffXsec)
		h_DiffXsec.SetBinError(i_bin, DiffXsecErr)

	return h_DiffXsec

def DrawHistogram( h, LowerEdge, UpperEdge ):
	CanvasName = h.GetName()
	CanvasName.replace("h_", "c_")
	c = TCanvas(CanvasName+"M%.0lfto%.0lf"%(LowerEdge, UpperEdge), "", 800, 800)
	c.cd()
	if (UpperEdge - LowerEdge) > 20:
		c.SetLogy()
	c.SetTopMargin(0.03)
	c.SetLeftMargin(0.13)
	c.SetRightMargin(0.05)
	c.SetBottomMargin(0.1)

	if UpperEdge==3000:
		c.SetLogx()

	h.Draw()
	h.SetStats(0)
	h.SetMarkerColor(2)
	h.SetLineColor(2) # -- red -- #
	h.SetFillColorAlpha(0, 0) # -- white with 100% transparency -- #
	h.GetXaxis().SetRangeUser( LowerEdge, UpperEdge )
	h.GetXaxis().SetMoreLogLabels()
	h.GetXaxis().SetNoExponent()
	h.GetYaxis().SetTitleOffset(1.3)
	h.SetXTitle("m(#mu#mu) [GeV]")
	h.SetYTitle("d#sigma/dm [pb/GeV]")

	latex = TLatex()
	latex.SetTextSize(0.028);
	if LowerEdge == 15 and UpperEdge==3000:
		latex.DrawLatexNDC(0.50, 0.85, "Z/#gamma* #rightarrow #mu#mu")
		latex.DrawLatexNDC(0.50, 0.80, "FEWZ (NNPDF30_nnlo_as_0118)")
		latex.DrawLatexNDC(0.50, 0.77, "QCD@NNLO, EWK@NLO")

	c.SaveAs(".pdf")

# -- main part -- #
gROOT.SetBatch(1)

Date = "20161110" # -- set by hand -- #

MassBinEdges = []

BinEdge = 15
while (BinEdge <= 3000):

	MassBinEdges.append( BinEdge )

	BinSize = 0
	if BinEdge < 320:
		BinSize	= 1
	else:
		BinSize = 2

	BinEdge = BinEdge + BinSize

print "MassBinEdges: \n", MassBinEdges



#-- files -- #
MassBinEdges_File = [15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 55, 80, 100, 120]

BinEdge_File = 145
while (BinEdge_File <= 3000):
	MassBinEdges_File += [BinEdge_File]

	BinSize = 0;
	if BinEdge_File < 320:
		BinSize = 25
	elif BinEdge_File > 320 and BinEdge_File < 2960:
		BinSize = 50
	else:
		BinSize = 40

	BinEdge_File = BinEdge_File + BinSize

# MassBinEdges_File = [15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30]

print "MassBinEdges_File: \n", MassBinEdges_File

FileNames = []

for i in range(0, len(MassBinEdges_File)-1):
	LowerEdge = MassBinEdges_File[i]
	UpperEdge = MassBinEdges_File[i+1]

	MassRange = "M%.0fto%.0f" % (LowerEdge, UpperEdge)

	Process = "DY"
	FileName = "NNLO.v%s_" % (Date) + MassRange + "_DY_NNLO_NNPDF30_nnlo_as_0118.dat"
	FileNames.append( FileName )


h_Xsec = MakeHistogram( MassBinEdges, FileNames )
h_Xsec.Sumw2()
h_DiffXsec = MakeDiffXsecHist( h_Xsec )
h_DiffXsec.Sumw2()

DrawHistogram(h_DiffXsec, 15, 3000)
DrawHistogram(h_DiffXsec, 15, 60)
DrawHistogram(h_DiffXsec, 50, 70)
DrawHistogram(h_DiffXsec, 60, 120)
DrawHistogram(h_DiffXsec, 110, 130)
DrawHistogram(h_DiffXsec, 120, 3000)

f_output = TFile("ROOTFile_DY_FEWZ_NNLO.root", "RECREATE")
f_output.cd()

h_Xsec.Write()
h_DiffXsec.Write()

f_output.Close()