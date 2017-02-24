from ROOT import TFile, TH1D, TGraphAsymmErrors, TString, TLatex, TCanvas, TPad, TLegend, TColor, TF1, Double, kRed, kBlack, kWhite
from math import sqrt
from array import array

def Get_Hist( FileName, HistName, HistName_New = "" ):
	TH1.AddDirectory(False)

	f_input = TFile( FileName )
	h_temp = f_input.Get( HistName ).Clone()
	if HistName_New != "":
		h_temp.SetName( HistName_New )

	f_input.Close()
	return h_temp

def Get_Graph( FileName, GraphName, GraphName_New = "" ):
	f_input = TFile( FileName )
	g_temp = f_input.Get( GraphName ).Clone()
	if GraphName_New != "":
		g_temp.SetName( GraphName_New )

	f_input.Close()
	return g_temp


class BaseInfo:
	def __init__(self, _color, _LegendName):
		self.Color = _color
		self.LegendName = _LegendName

class HistInfo(BaseInfo):

	h = None
	h_ratio = None

	def __init__(self, FileName, HistName, _color, _LegendName):
		self.Color = _color
		self.LegendName = _LegendName
		self.Get_Histogram( FileName, HistName )

	def Get_Histogram(self, FileName, HistName):
		self.h = Get_Hist( FileName, HistName )
		self._Set_Attributes()

	def Set_Histogram( self, _h ):
		self.h = _h.Clone()
		self._Set_Attributes()

	def CalcRatio_DEN( self, h_DEN ):
		self.h.Sumw2()
		h_DEN.Sumw2()
		self.h_ratio = self.h.Clone()
		self.h_ratio.Divide( h, h_DEN )

	def CalcRatio_NUM( self, h_NUM ):
		self.h.Sumw2()
		h_NUM.Sumw2()
		self.h_ratio = self.h.Clone()
		self.h_ratio.Divide( h_NUM, h )

	def Draw(self, DrawOp):
		self.h.Draw( DrawOp )

	def _Set_Attributes(self):
		self.h.SetTitle("")
		self.h.SetStats(False)
		self.h.SetLineColor(self.Color)
		self.h.SetFillColorAlpha(kWhite,0)
		self.h.SetMarkerStyle(20)
		self.h.SetMarkerColor(self.Color)

class GraphInfo(BaseInfo):
	g = None
	g_ratio = None

	def __init__(self, FileName, GraphName, _color, _LegendName):
		self.Color = _color
		self.LegendName = _LegendName
		self.Get_Graph( FileName, GraphName )

	def Get_Graph( self, FileName, GraphName ):
		self.g = Get_Graph( FileName, GraphName )

	def DrawGraph( self, DrawOp ):
		self.g.Draw( DrawOp )
		self._Set_Attributes()

	def CalcRatio_DEN( self, g_DEN ):
		self.g_ratio = self._MakeRatioGraph( self.g, g_DEN )

	def CalcRatio_NUM( self, g_NUM ):
		self.g_ratio = self._MakeRatioGraph( g_NUM, self.g )

	def _Set_Attributes(self):
		self.g.SetTitle("")
		self.g.SetLineColor( self.Color )
		self.g.SetMarkerStyle( 20 )
		self.g.SetMarkerColor( self.Color )
		self.g.SetMarkerSize(1.5)
		self.g.SetFillColorAlpha(kWhite, 0)

		if self.g_ratio is not None:
			self.g_ratio.SetTitle("")
			self.g_ratio.SetLineColor( self.Color )
			self.g_ratio.SetMarkerStyle( 20 )
			self.g_ratio.SetMarkerColor( self.Color )
			self.g_ratio.SetMarkerSize(1.5)
			self.g_ratio.SetFillColorAlpha(kWhite, 0)

	def _MakeRatioGraph( self, g_NUM, g_DEN ):
		gRatio = g_NUM.Clone()
		gRatio.Set(0) # -- remove all points -- #

		nPoint1 = g_NUM.GetN()
		nPoint2 = g_DEN.GetN()

		if nPoint1 != nPoint2:
			print "# points in DEN and NUM is different! ... (DEN, NUM) = (%d, %d)\n" % (nPoint2, nPoint1)
			return

		for i in range(0, nPoint1):
			x_DEN = Double(0)
			y_DEN = Double(0)

			x_NUM = Double(0)
			y_NUM = Double(0)

			g_DEN.GetPoint(i, x_DEN, y_DEN)
			yErrLow_DEN = g_DEN.GetErrorYlow(i)
			yErrHigh_DEN = g_DEN.GetErrorYhigh(i)
			yErr_DEN =  yErrLow_DEN if yErrLow_DEN > yErrHigh_DEN else yErrHigh_DEN

			g_NUM.GetPoint(i, x_NUM, y_NUM)
			yErrLow_NUM = g_NUM.GetErrorYlow(i)
			yErrHigh_NUM = g_NUM.GetErrorYhigh(i)
			yErr_NUM =  yErrLow_NUM if yErrLow_NUM > yErrHigh_NUM else yErrHigh_NUM

			ratio = 0
			ratio_err = 0

			if y_DEN == 0:
				print "[%02d point: y_DEN = 0 ... ratio is set as 0]\n" % (i)
				ratio = 0
				ratio_err = 0
			
			else:
				ratio = y_NUM / y_DEN
				ratio_err = self._ErrorPropagation_AoverB( y_DEN, yErr_DEN, y_NUM, yErr_NUM )

			gRatio.SetPoint(i, x_DEN, ratio)

			xErrLow_DEN = g_DEN.GetErrorXlow(i)
			xErrHigh_DEN = g_DEN.GetErrorXhigh(i)
			gRatio.SetPointError( i, xErrLow_DEN, xErrHigh_DEN, ratio_err, ratio_err )

		return gRatio;


	def _ErrorPropagation_AoverB( self, A, A_Err, B, B_Err ):
		ratio_A = A_Err / A
		ratio_B = B_Err / B

		Err2 = ratio_A * ratio_A + ratio_B*ratio_B

		return (A/B) * sqrt(Err2)


def Canvas_Square(CanvasName, isLogx = False, isLogy = False):
	c = TCanvas(CanvasName, "", 700, 700)

	c.SetTopMargin(0.05)
	c.SetLeftMargin(0.13)
	c.SetRightMargin(0.045)
	c.SetBottomMargin(0.13)

	if isLogx: c.SetLogx()
	if isLogy: c.SetLogy()

	return c


def Canvas_Ratio(CanvasName, isLogx = False, isLogy = False):

	c = TCanvas(CanvasName, "", 700, 700)
	c.cd()

	TopPad = TPad("TopPad", "TopPad", 0.01, 0.01, 0.99, 0.99)
	TopPad.Draw()
	TopPad.cd()

	TopPad.SetTopMargin(0.05)
	TopPad.SetLeftMargin(0.13)
	TopPad.SetRightMargin(0.045)
	TopPad.SetBottomMargin(0.3)

	if isLogx: TopPad.SetLogx()
	if isLogy: TopPad.SetLogy()

	c.cd()
	BottomPad = TPad("BottomPad", "BottomPad", 0.01, 0.01, 0.99, 0.29)
	BottomPad.Draw()
	BottomPad.cd()

	BottomPad.SetGridx()
	BottomPad.SetGridy()
	BottomPad.SetTopMargin(0.05)
	BottomPad.SetLeftMargin(0.13)
	BottomPad.SetRightMargin(0.045)
	BottomPad.SetBottomMargin(0.4)

	if isLogx: BottomPad.SetLogx()

	return c, TopPad, BottomPad


def DrawLine( f_line, Color = kRed ):
	f_line = TF1("f_line", "1", -10000, 10000)
	f_line.SetLineColor( Color )
	f_line.SetLineWidth(1)
	f_line.Draw("PSAME")

def GetLine( Color = kRed ):
	f_line = TF1("f_line", "1", -10000, 10000)
	f_line.SetLineColor( Color )
	f_line.SetLineWidth(1)

	return f_line


def Latex_Preliminary( latex, lumi,  E_CM ):
	Info = "%.1lf fb^{-1} (%d TeV)" % (lumi, E_CM)
	latex.DrawLatexNDC(0.69, 0.96, "#font[42]{#scale[0.8]{%s}}" % Info)
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}")
	latex.DrawLatexNDC(0.24, 0.96, "#font[42]{#it{#scale[0.8]{Preliminary}}}")


def Latex_Simulation( latex ):
	latex.DrawLatexNDC(0.82, 0.96, "#font[42]{#scale[0.8]{13 TeV}}")
	latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS}")
	latex.DrawLatexNDC(0.25, 0.96, "#font[42]{#it{#scale[0.8]{Simulation}}}")


def SetAxis_SinglePad( X_axis, Y_axis, XTitle, YTitle ):
	X_axis.SetTitle( XTitle )
	X_axis.SetLabelSize(0.04)
	X_axis.SetTitleOffset(1.1)
	X_axis.SetTitleSize(0.05)

	Y_axis.SetTitle( YTitle )
	Y_axis.SetTitleSize(0.05)
	Y_axis.SetTitleOffset(1.2)
	Y_axis.SetLabelSize(0.045)


def SetAxis_TopPad( X_axis, Y_axis, YTitle ):
	X_axis.SetLabelFont(42)
	X_axis.SetLabelSize(0.000)
	X_axis.SetTitleSize(0.000)

	Y_axis.SetTitleFont(42)
	Y_axis.SetTitle( YTitle )
	Y_axis.SetTitleSize(0.05)
	Y_axis.SetTitleFont(42)
	Y_axis.SetTitleOffset(1.25)
	Y_axis.SetLabelFont(42)
	Y_axis.SetLabelSize(0.04)


def SetAxis_BottomPad( X_axis, Y_axis, XTitle, YTitle, yMin = 0.51, yMax = 1.49 ):
	X_axis.SetMoreLogLabels()
	X_axis.SetNoExponent()
	X_axis.SetTitle( XTitle )
	X_axis.SetTitleOffset( 0.85 )
	X_axis.SetTitleSize( 0.2 )
	X_axis.SetLabelColor(1)
	X_axis.SetLabelFont(42)
	X_axis.SetLabelOffset(0.01)
	X_axis.SetLabelSize(0.13)

	Y_axis.CenterTitle()
	Y_axis.SetTitle( YTitle )
	Y_axis.SetTitleOffset( 0.55 )
	Y_axis.SetTitleSize( 0.12)
	Y_axis.SetRangeUser( yMin, yMax )
	Y_axis.SetLabelSize( 0.10 )

def SetFormat_TopPad( obj, YTitle ):
	SetAxis_TopPad( obj.GetXaxis(), obj.GetYaxis(), YTitle )

def SetFormat_BottomPad( obj, XTitle, YTitle, yMin = 0.51, yMax = 1.49 ):
	SetAxis_BottomPad( obj.GetXaxis(), obj.GetYaxis(), XTitle, YTitle, yMin, yMax )

def SetFormat_SinglePad( obj, YTitle ):
	SetAxis_SinglePad( obj.GetXaxis(), obj.GetYaxis(), XTitle, YTitle )

def GetLegend( xMin = 0.75, yMin = 0.75, xMax = 0.95, yMax = 0.95 ):
	legend = TLegend( xMin, yMin, xMax, yMax )
	legend.SetFillStyle(0)
	legend.SetBorderSize(0)
	legend.SetTextFont(62)

	return legend