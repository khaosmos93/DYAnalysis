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
		print "Include path: %s" % (IncludePath) 
		break

import sys
sys.path.append( IncludePath )
###############################################################
from PlotTools import *

FileName = "ROOTFile_Output_Systematic_Eff.root"

Graph_Data = GraphInfo(FileName, "g_Data_All", kBlack, "Data (Bkg.Sub.)")
Graph_MC = GraphInfo(FileName, "g_MC_All", kRed, "MC (DY)")


c, TopPad, BottomPad = Canvas_Ratio( "c_test" )
c.cd()
TopPad.cd()
Graph_MC.DrawGraph( "APSAME" )
Graph_Data.DrawGraph( "PSAME" )
Graph_Data.CalcRatio_DEN( Graph_MC.g )

SetFormat_TopPad( Graph_MC.g, "Efficiency" )
Graph_MC.g.GetYaxis().SetRangeUser(0.5, 1.1)

legend = GetLegend()
legend.AddEntry( Graph_Data.g, Graph_Data.LegendName )
legend.AddEntry( Graph_MC.g, Graph_MC.LegendName )
legend.Draw()

latex = TLatex()
Latex_Preliminary( latex, 35.9, 13 )

c.cd()
BottomPad.cd()

Graph_Data.g_ratio.Draw("APSAME")
SetFormat_BottomPad( Graph_Data.g_ratio, "m [GeV]", "Data/MC", 0.5, 1.05 )

f_line = GetLine()
f_line.Draw("PSAME")

c.SaveAs(".pdf")