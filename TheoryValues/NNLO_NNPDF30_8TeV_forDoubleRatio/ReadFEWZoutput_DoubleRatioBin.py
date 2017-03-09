from ROOT import gROOT, TColor, TROOT, TFile, TH1D, TCanvas, TPad, TLatex
from array import array
import sys
import math
from getopt import gnu_getopt as getopt

gROOT.SetBatch(1)

def usage():
    print sys.argv[0], " : Read FEWZ output file and make ROOT histogram with x-section"
    print "  Mandatory options :"
    print "   --PDF PDFname                   PDF name"
    print "   --version version               version (ex> 20161110)"
    sys.exit()


print "sys.argv: ", sys.argv
# Parse arguments
if len(sys.argv) < 2: usage()
try:
    opts, args = getopt(sys.argv[1:], 'n', ["PDF=", "version="])
    opts = dict(opts)
except:
    print "!!! Error parsing arguments"
    usage()

class GetFEWZResults:
	def __init__(self, _opts):
		self.PDF = _opts['--PDF']
		self.version = _opts['--version']

		# self.Init_MassBinEdges()
		# self.Init_MassBinEdges_File()

		self.MassBinEdges = [
		15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 
		64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 
		110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 
		200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500 ]
		self.MassBinEdges_File = [
		15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 
		64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 
		110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 
		200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500 ]

		print "Prcess version = %s with PDF = %s" % (self.version, self.PDF)
		print "\tMassBinEdges: \n\t", self.MassBinEdges
		print "\tMassBinEdges_File: \n\t", self.MassBinEdges_File

	def MakeFileName( self, _LowerEdge, _UpperEdge ):
		return "NNLO.v%s_M%sto%s_DY_NNLO_%s.dat" % (self.version, _LowerEdge, _UpperEdge, self.PDF)

	def MakeHistogram( self ):
		h_Xsec = TH1D("h_Xsec", "", len(self.MassBinEdges)-1, array("d", self.MassBinEdges) )

		###############################################################
		# -- collect Xsec information from FEWZ outputs (txt file) -- #
		###############################################################
		BinCenter_Xsec = []
		for i in range(0, len(self.MassBinEdges_File)-1):
			LowerEdge = self.MassBinEdges_File[i]
			UpperEdge = self.MassBinEdges_File[i+1]

			FileName = self.MakeFileName( LowerEdge, UpperEdge )

			BinCenter_Xsec += self.GetXsec_SingleBin( FileName )

		# -- check consistency -- #
		if len(BinCenter_Xsec) != len(self.MassBinEdges)-1:
			print "(# x-seciton, # mass bins) = (%d, %d) -> they are different! check the details" % ( len(BinCenter_Xsec), len(self.MassBinEdges)-1 )
			return

		############################
		# -- fill the histogram -- #
		############################
		for i in range(0, len(self.MassBinEdges)-1):
			LowerEdge = self.MassBinEdges[i]
			UpperEdge = self.MassBinEdges[i+1]

			BinCenter_Xsec_ithBin = BinCenter_Xsec[i]

			BinCenter = BinCenter_Xsec_ithBin[0]
			if not(BinCenter > LowerEdge and BinCenter < UpperEdge):
				print "(LowerEdge, UpperEdge) = (%.0lf, %.0lf) ... BinCenter=%.0lf is not in this bin!" % (LowerEdge, UpperEdge, BinCenter)
				break

			i_bin = i+1
			CentralValue = BinCenter_Xsec_ithBin[1]
			h_Xsec.SetBinContent( i_bin, CentralValue )

			IntgErr = BinCenter_Xsec_ithBin[2]
			PDFErr = BinCenter_Xsec_ithBin[3]

			# Error = BinCenter_Xsec_ithBin[2] # -- Integration error only -- #
			Error = math.sqrt( IntgErr*IntgErr + PDFErr*PDFErr )
			h_Xsec.SetBinError( i_bin, Error )


		h_Xsec.Sumw2()
		h_DiffXsec = self.MakeDiffXsecHist( h_Xsec )
		h_DiffXsec.Sumw2()

		# self.DrawHistogram(h_DiffXsec, 15, 3000)
		# self.DrawHistogram(h_DiffXsec, 15, 60)
		# self.DrawHistogram(h_DiffXsec, 50, 70)
		# self.DrawHistogram(h_DiffXsec, 60, 120)
		# self.DrawHistogram(h_DiffXsec, 110, 130)
		# self.DrawHistogram(h_DiffXsec, 120, 3000)
		self.DrawHistogram(h_DiffXsec, 15, 1500)

		f_output = TFile("ROOTFile_DY_FEWZ_NNLO_%s.root" % (self.PDF), "RECREATE")
		f_output.cd()

		h_Xsec.Write()
		h_DiffXsec.Write()

		f_output.Close()

	def GetXsec_SingleBin( self, FileName ):
		print "GetXsec: process ", FileName

		MassRange = FileName.split("_")[1] # -- ex> M15to16 --#
		LowerEdge = float( MassRange.split("to")[0].split("M")[1] )
		UpperEdge = float( MassRange.split("to")[1] )

		print "\t[%.0f < M < %0.lf]" % (LowerEdge, UpperEdge)
		BinCenter = (LowerEdge + UpperEdge) / 2.0;

		xSec = 0.0;
		IntgErr = 0.0;
		PDFErr_plus = 0.0;
		PDFErr_minus = -1.0; # -- if there's no minus error -- #
		PDFErr = 0.0;

		f = open("./Results/"+FileName, "r")
		lines = f.readlines()

		Next_PDFError = 0
		for line in lines:
			if "Sigma (pb)" in line:
				items = line.split(" ")
				for item in items:
					if self.isNumber( item ):
						xSec = float(item);
						break;

			elif "Integration Error (pb)" in line:
				items = line.split(" ")
				for item in items:
					if self.isNumber( item ):
						IntgErr = float(item);
						break;

			elif "PDF Error (pb)" in line:
				items = line.split(" ")
				for item in items:
					if self.isNumber( item ):
						PDFErr_plus = float(item)
						Next_PDFError = 1
						break;

			elif Next_PDFError == 1:
				items = line.split(" ")
				for item in items:
					if self.isNumber( item ):
						PDFErr_minus = float(item)
						break;
				Next_PDFError = 0


		print "x-section = %lf, integration error = %lf, PDFError(+,-) = (%lf, %lf)\n" % ( xSec, IntgErr, PDFErr_plus, PDFErr_minus )

		if PDFErr_plus > PDFErr_minus:
			PDFErr = PDFErr_plus
		else:
			PDFErr = PDFErr_minus

		binCenter_xsec = []
		binCenter_xsec.append( [BinCenter, xSec, IntgErr, PDFErr] )

		return binCenter_xsec

	def GetXsec( self, FileName ):
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
						if self.isNumber(item):
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

	def isNumber(self, s):
		try:
			float(s)
			return True
		except ValueError:
			return False


	def MakeDiffXsecHist( self, h_Xsec ):
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

	def DrawHistogram( self, h, _LowerEdge, _UpperEdge ):
		CanvasName = h.GetName()
		CanvasName.replace("h_", "c_")
		c = TCanvas(CanvasName+"M%.0lfto%.0lf"%(_LowerEdge, _UpperEdge), "", 800, 800)
		c.cd()
		if (_UpperEdge - _LowerEdge) > 20:
			c.SetLogy()
		c.SetTopMargin(0.03)
		c.SetLeftMargin(0.13)
		c.SetRightMargin(0.05)
		c.SetBottomMargin(0.1)

		if _UpperEdge > 1000:
			c.SetLogx()

		h.Draw()
		h.SetStats(0)
		h.SetMarkerColor(2)
		h.SetLineColor(2) # -- red -- #
		h.SetFillColorAlpha(0, 0) # -- white with 100% transparency -- #
		h.GetXaxis().SetRangeUser( _LowerEdge, _UpperEdge )
		h.GetXaxis().SetMoreLogLabels()
		h.GetXaxis().SetNoExponent()
		h.GetYaxis().SetTitleOffset(1.3)
		h.SetXTitle("m(#mu#mu) [GeV]")
		h.SetYTitle("d#sigma/dm [pb/GeV]")

		latex = TLatex()
		latex.SetTextSize(0.028);
		# if _LowerEdge == 15 and _UpperEdge==3000:
		latex.DrawLatexNDC(0.50, 0.85, "Z/#gamma* #rightarrow #mu#mu @ E_{CM} = 8 TeV")
		latex.DrawLatexNDC(0.50, 0.80, "FEWZ (%s)" % (self.PDF) )
		latex.DrawLatexNDC(0.50, 0.77, "QCD@NNLO, EWK@NLO")

		c.SaveAs(".pdf")

	def Init_MassBinEdges_File( self ):
		self.MassBinEdges_File = [15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 55, 80, 100, 120]

		BinEdge_File = 145
		while (BinEdge_File <= 3000):
			self.MassBinEdges_File += [BinEdge_File]

			BinSize = 0;
			if BinEdge_File < 320:
				BinSize = 25
			elif BinEdge_File > 320 and BinEdge_File < 2960:
				BinSize = 50
			else:
				BinSize = 40

			BinEdge_File = BinEdge_File + BinSize

		print "MassBinEdges_File: \n", self.MassBinEdges_File

	def Init_MassBinEdges( self ):
		BinEdge = 15
		while (BinEdge <= 3000):

			self.MassBinEdges.append( BinEdge )

			BinSize = 0
			if BinEdge < 320:
				BinSize	= 1
			else:
				BinSize = 2

			BinEdge = BinEdge + BinSize

		print "MassBinEdges: \n", self.MassBinEdges

###################
# -- main part -- #
###################
Result = GetFEWZResults( opts )
Result.MakeHistogram()