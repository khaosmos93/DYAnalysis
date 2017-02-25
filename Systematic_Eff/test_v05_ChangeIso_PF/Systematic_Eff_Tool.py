import logging

# logging.basicConfig(level=logging.DEBUG, filename="log.txt", filemode="a+",
#                     format="%(asctime)-15s %(levelname)-8s %(message)s")
logging.basicConfig(level=logging.DEBUG, filename="log.txt", filemode="a",
                    format="%(message)s")
logging.info("start")

###########################
# -- find include path -- #
###########################
import os
Dirs = os.getcwd().split("/")
IncludePath = ""
for i in range(0, len(Dirs)):
	if Dirs[i] == "Physics":
		IncludePath = "~/Physics/%s/Include" % (Dirs[i+1])
		IncludePath = os.path.expanduser( IncludePath )
		logging.info( "Include path: %s" % (IncludePath) )
		break

import sys
sys.path.append( IncludePath )
###############################################################
from PlotTools import *
from ROOT import TEfficiency, THStack, kPink, kYellow, kBlue

def Latex_Info( latex, Type, region ):
	TStr_Type = ""
	if Type == "DEN": 
		TStr_Type = "Denominator"
	elif Type == "":
		TStr_Type = ""
	else:
		TStr_Type = "Numerator"

	TStr_Region = ""
	if region == "BB": TStr_Region = "BB"
	if region == "BEEE": TStr_Region = "BE+EE"
	if region == "All": TStr_Region = "All"

	latex.DrawLatexNDC(0.15, 0.91, "#font[42]{#scale[0.8]{"+TStr_Region+"}}")
	latex.DrawLatexNDC(0.15, 0.88, "#font[42]{#scale[0.6]{"+TStr_Type+"}}")


class Tool_Systematic_Eff:
	InputFileName = ""
	i_canvas = 0
	f_output = None

	Lumi = 0
	Lumi_Total = 35867.060
	Lumi_RunBtoF = 19720.882
	Lumi_RunGtoH = 16146.178

	Lumi_Scale = 1.0

	DENStr = ""
	NUMStr = ""
	Region = ""

	h_data_DEN_BkgSub = None
	h_data_NUM_BkgSub = None

	h_DY_DEN = None
	h_DY_NUM = None

	g_data = None
	g_MC = None

	# SubDirData = "Data"
	# Tuple_SubDirMC = ("WWTo2L2Nu", "WZ", "ZZ", "ttbarTo2L2Nu", "tW", "tbarW", "DYPowheg")

	# def __init__(self):

	def Set_InputFileName( self, _FileName ):
		self.InputFileName = _FileName

	def Set_Output( self, _f_output ):
		self.f_output = _f_output

	def Set_Region( self, _Region ):
		self.Region = _Region

	def Set_DEN_NUM( self, _DENStr, _NUMStr ):
		self._Init()

		self.DENStr = _DENStr
		self.NUMStr = _NUMStr

		if "RunBtoF" in self.DENStr:
			self.Lumi_Scale = self.Lumi_RunBtoF / self.Lumi_Total
			self.Lumi = self.Lumi_RunBtoF

		elif "RunGtoH" in self.DENStr:
			self.Lumi_Scale = self.Lumi_RunGtoH / self.Lumi_Total
			self.Lumi = self.Lumi_RunGtoH

		else:
			self.Lumi_Scale = 1.0
			self.Lumi = self.Lumi_Total

	def DrawCanvas_Mass_All( self ):
		self._DrawCanvas_Mass( self.DENStr, self.Region )
		self._DrawCanvas_Mass( self.NUMStr, self.Region )

	def DrawCanvas_Eff_Data_vs_MC( self ):
		self.g_data = _EfficiencyGraph( "Data" )
		self.g_MC = _EfficiencyGraph( "MC" )

		Graph_MC = GraphInfo( kRed, "MC (DY)" )
		Graph_MC.Set_Graph( self.g_MC )

		Graph_Data = GraphInfo( kBlack, "Data (Bkg.Sub.)")
		Graph_Data.Set_Graph( self.g_data )
		Graph_Data.CalcRatio_DEN( Graph_MC.g )

		CanvasName = "c_Eff_%s_%s_%s" % (self.DENStr, self.NUMStr, self.Region)

		c, TopPad, BottomPad = Canvas_Ratio( c, CanvasName, 1, 0 )
		c.cd()
		TopPad.cd()

		Graph_MC.DrawGraph("APSAME")
		Graph_Data.DrawGraph("PSAME")

		SetFormat_TopPad( Graph_MC.g, "Efficiency" )
		Graph_MC.g.SetXaxis().SetRangeUser(0.7, 1.05)

		legend = GetLegend()
		legend.AddEntry( Graph_Data.g, Graph_Data.LegendName )
		legend.AddEntry( Graph_MC.g, Graph_MC.LegendName )
		legend.Draw()

		latex = TLatex()
		Latex_Preliminary( latex, self.Lumi, 13 )
		Latex_Info( latex, "", self.Region )


		c.cd()
		BottomPad.cd()

		Graph_Data.g_ratio.Draw("APSAME")
		SetFormat_BottomPad( Graph_Data.g_ratio, "m [GeV}", "Data/MC", 0.6, 1.05 )

		f_line = GetLine()
		f_line.Draw("PSAME")

		c.SaveAs(".pdf")









	def _EfficiencyGraph( self, _DataType ):

		h_DEN = None
		h_NUM = None

		if _DataType == "Data":
			h_DEN = h_data_DEN_BkgSub.Clone()
			h_NUM = h_data_NUM_BkgSub.Clone()
		elif _DataType == "MC":
			h_DEN = h_DY_DEN.Clone()
			h_NUM = h_DY_NUM.Clone()

		h_DEN = self._Rebin_Mass( h_DEN )
		h_NUM = self._Rebin_Mass( h_NUM )

		self._RemoveUnderOverFlow( h_DEN )
		self._RemoveUnderOverFlow( h_NUM )

		TEff = TEfficiency(h_NUM, h_DEN);

		GraphName = "g_%s_%s_%s_%s" % (_DataType, self.DENStr, self.NUMStr, self.Region)

		g_Eff = TEff.CreateGraph().Clone(GraphName)

		return g_Eff

	def _RemoveUnderOverFlow( h ):
		h.SetBinContent(0, 1)
		h.SetBinError(0, 1)
		h.SetBinContent( h.GetNbinsX()+1, 1)
		h.SetBinError( h.GetNbinsX()+1, 1)

	def _Rebin_Mass( h_before ):
		_MassBinEdges = [60, 120, 200, 400, 600, 800, 1000, 2500]
		Arr_MassBinEdges = array("d", _MassBinEdges)

		return h_before.Rebin( len(_MassBinEdges)-1, h_before.GetName(), Arr_MassBinEdges)

	def _Init( self ):
		self.i_canvas = 0
		self.f_output = None

		self.Lumi = 0.0
		self.Lumi_Scale = 1.0

		self.DENStr = ""
		self.NUMStr = ""
		self.Region = ""

		self.h_data_DEN_BkgSub = None
		self.h_data_NUM_BkgSub = None

		self.h_DY_DEN = None
		self.h_DY_NUM = None

		self.g_data = None
		self.g_MC = None

	def _DrawCanvas_Mass( self, _DENNUMTYPE, _Region ):
		logging.info("\nDrawCanvas: Mass comparison between DATA and MC")
		logging.info("\t%s, %s" % (_DENNUMTYPE, _Region))

		HistName_Base = ""
		if _Region == "All":
			HistName_Base = "h_mass"
		else:
			HistName_Base = "h_mass_%s" % (_Region)

		HistName_Data = "%s_%s" % (HistName_Base, _DENNUMTYPE)
		HistName_MC = ""
		if "_Run" in _DENNUMTYPE:
			HistName_MC = "%s_%s" % (HistName_Base, _DENNUMTYPE.split("_Run")[0] )
		else:
			HistName_MC = HistName_Data
		logging.info( "[HistName: (Data, MC) = (%s, %s)]" % (HistName_Data, HistName_MC) )

		# -- Data -- #
		Legend_Data = "Data"
		if "RunBtoF" in _DENNUMTYPE:
			Legend_Data = Legend_Data + " (RunB-F)"
		elif "RunGtoH" in _DENNUMTYPE:
			Legend_Data = Legend_Data + " (RunG-H)"

		Hist_data = HistInfo( kBlack, Legend_Data )
		Hist_data.Get_Histogram( self.InputFileName, "Data/%s" % HistName_Data )

		# -- MC -- #
		Hist_DY = HistInfo( kPink, "Z/#gamma*#rightarrow#mu#mu" )
		Hist_DY.Get_Histogram( self.InputFileName, "DYPowheg/%s" % HistName_MC )
		Hist_DY.h.SetFillColorAlpha( Hist_DY.Color, 1 )

		# -- ttbar + tW + tbarW -- #
		h_ttbar = Get_Hist( self.InputFileName, "ttbarTo2L2Nu/%s" % HistName_MC)
		h_tW = Get_Hist( self.InputFileName, "tW/%s" % HistName_MC )
		h_tbarW = Get_Hist( self.InputFileName, "tbarW/%s" % HistName_MC )

		h_top = h_ttbar.Clone()
		h_top.Sumw2()
		h_top.Add( h_tW )
		h_top.Add( h_tbarW )

		Hist_top = HistInfo( kYellow, "t#bar{t}+tW+#bar{t}W" )
		Hist_top.Set_Histogram( h_top )
		Hist_top.h.SetFillColorAlpha( Hist_top.Color, 1 )

		# -- dibosons -- #
		h_WW = Get_Hist(self.InputFileName, "WWTo2L2Nu/%s" % HistName_MC )
		h_WZ = Get_Hist(self.InputFileName, "WZ/%s" % HistName_MC )
		h_ZZ = Get_Hist(self.InputFileName, "ZZ/%s" % HistName_MC )

		h_diboson = h_WW.Clone()
		h_diboson.Sumw2()
		h_diboson.Add( h_WZ )
		h_diboson.Add( h_ZZ )

		Hist_diboson = HistInfo( kBlue, "Diboson" )
		Hist_diboson.Set_Histogram( h_diboson )
		Hist_diboson.h.SetFillColorAlpha( Hist_diboson.Color, 1 )

		RebinSize = 50
		Hist_data.h.Rebin( RebinSize )
		Hist_DY.h.Rebin( RebinSize )
		Hist_top.h.Rebin( RebinSize )
		Hist_diboson.h.Rebin( RebinSize )
		xMin = 50

		# -- normalize to RunB-F or RunG-H luminosity -- #
		logging.info( "[Lumi_Scale = %lf]" % (self.Lumi_Scale) )
		Hist_DY.h.Scale( self.Lumi_Scale );
		Hist_top.h.Scale( self.Lumi_Scale );
		Hist_diboson.h.Scale( self.Lumi_Scale );

		# -- apply scale factor (data/MC @ Z-peak) -- #
		SF = self._SF_Zpeak_DirectlyExtracted( Hist_data.h, Hist_DY.h, Hist_top.h, Hist_diboson.h );
		Hist_DY.h.Scale( SF );
		Hist_top.h.Scale( SF );
		Hist_diboson.h.Scale( SF );

		# -- histogram stack -- #
		hs = THStack("hs", "")
		hs.Add( Hist_diboson.h )
		hs.Add( Hist_top.h )
		hs.Add( Hist_DY.h )

		# -- total MC for ratio plot -- #
		h_totMC = Hist_DY.h.Clone()
		h_totMC.Sumw2()
		h_totMC.Add( Hist_top.h )
		h_totMC.Add( Hist_diboson.h )
		Hist_data.CalcRatio_DEN( h_totMC )

		# -- draw canvas -- #
		CanvasName = "c%02d_Mass_%s_%s" % (self.i_canvas, _DENNUMTYPE, _Region)
		self.i_canvas = self.i_canvas + 1

		c, TopPad, BottomPad = Canvas_Ratio( CanvasName, 0, 1 )

		c.cd()
		TopPad.cd()

		h_format = Hist_data.h.Clone();
		h_format.Reset("ICES");
		h_format.Draw("");
		hs.Draw("histsame");
		Hist_data.h.Draw("EPsame");
		h_format.Draw("axissame");

		YTitle = "Events / %.0d GeV" % RebinSize
		SetFormat_TopPad(h_format, YTitle)
		h_format.GetXaxis().SetRangeUser(xMin, 2500);
		h_format.GetYaxis().SetRangeUser(1e-3, 1e7);

		legend = GetLegend(0.65, 0.80, 0.95, 0.95)
		legend.AddEntry( Hist_data.h, Hist_data.LegendName )
		legend.AddEntry( Hist_DY.h, Hist_DY.LegendName )
		legend.AddEntry( Hist_top.h, Hist_top.LegendName )
		legend.AddEntry( Hist_diboson.h, Hist_diboson.LegendName )
		legend.Draw()

		latex = TLatex()
		Latex_Preliminary( latex, self.Lumi / 1000.0, 13 )
		Latex_Info( latex, _DENNUMTYPE, _Region )

		c.cd()
		BottomPad.cd()

		Hist_data.h_ratio.Draw("EPSAME")
		SetFormat_BottomPad( Hist_data.h_ratio, "m [GeV]", "Data/MC", 0, 2.5)
		Hist_data.h_ratio.GetXaxis().SetRangeUser(xMin, 2500)

		f_line = GetLine()
		f_line.Draw("PSAME")

		c.SaveAs(".pdf")

		if _DENNUMTYPE == self.DENStr:
			self.h_DY_DEN = Hist_DY.h.Clone()
			self.h_data_DEN_BkgSub = self._BackgroundSubtraction( Hist_data.h, Hist_top.h, Hist_diboson.h )

		else _DENNUMTYPE == self.NUMStr:
			self.h_DY_NUM = Hist_DY.h.Clone()
			self.h_data_NUM_BkgSub = self._BackgroundSubtraction( Hist_data.h, Hist_top.h, Hist_diboson.h )

	def _BackgroundSubtraction( self, h_data, h_top, h_diboson ):
		HistName = h_data.GetName()
		HistName = HistName + "_BkgSub"

		h_BkgSub = h_data.Clone(HistName)

		h_BkgSub.Sumw2()
		h_top.Sumw2()
		h_diboson.Sumw2()

		h_BkgSub.Add( h_top, -1 )
		h_BkgSub.Add( h_diboson, -1 )

		return h_BkgSub

	def _SF_Zpeak_DirectlyExtracted( self, h_data, h_DY, h_top, h_diboson ):
		h_totMC = h_DY.Clone()
		h_totMC.Sumw2()
		h_totMC.Add( h_top )
		h_totMC.Add( h_diboson )

		nEvent_Data = self._nEvent_Zpeak( h_data )
		nEvent_MC = self._nEvent_Zpeak( h_totMC )
		SF = nEvent_Data / nEvent_MC

		logging.info( "[# events in Z-peak: (Data, MC) = (%.1lf, %.1lf) -> SF = %lf]" % (nEvent_Data, nEvent_MC, SF) )

		return SF

	def _nEvent_Zpeak( self, h ):
		nEvent = 0

		nBin = h.GetNbinsX()
		for i in range(0, nBin):
			i_bin = i+1
			BinCenter = h.GetBinCenter(i_bin)

			if BinCenter > 60 and BinCenter < 120:
				nEvent = nEvent + h.GetBinContent(i_bin)

		return nEvent





























