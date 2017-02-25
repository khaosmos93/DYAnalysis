from Systematic_Eff_Tool import *

List_Region = ["All", "BB", "BEEE"]
List_DENNUM = [ ("DEN", "NUM"), ("DEN_RunBtoF", "NUM_RunBtoF"), ("DEN_RunGtoH", "NUM_RunGtoH") ]

FileName = "ROOTFile_Output_Systematic_Eff.root"
f_output = TFile(FileName, "RECREATE")

for DEN, NUM in List_DENNUM:

	for Region in List_Region:
		tool = Tool_Systematic_Eff()

		tool.Set_InputFileName( "ROOTFile_Systematic_Eff.root" )
		tool.Set_Output(f_output)
		tool.Set_DEN_NUM(DEN, NUM)
		tool.Set_Region(Region)

		tool.DrawCanvas_Mass_All()












# Graph_Data = GraphInfo(FileName, "g_Data_All", kBlack, "Data (Bkg.Sub.)")
# Graph_MC = GraphInfo(FileName, "g_MC_All", kRed, "MC (DY)")


# c, TopPad, BottomPad = Canvas_Ratio( "c_test" )
# c.cd()
# TopPad.cd()
# Graph_MC.DrawGraph( "APSAME" )
# Graph_Data.DrawGraph( "PSAME" )
# Graph_Data.CalcRatio_DEN( Graph_MC.g )

# SetFormat_TopPad( Graph_MC.g, "Efficiency" )
# Graph_MC.g.GetYaxis().SetRangeUser(0.5, 1.1)

# legend = GetLegend()
# legend.AddEntry( Graph_Data.g, Graph_Data.LegendName )
# legend.AddEntry( Graph_MC.g, Graph_MC.LegendName )
# legend.Draw()

# latex = TLatex()
# Latex_Preliminary( latex, 35.9, 13 )

# c.cd()
# BottomPad.cd()

# Graph_Data.g_ratio.Draw("APSAME")
# SetFormat_BottomPad( Graph_Data.g_ratio, "m [GeV]", "Data/MC", 0.5, 1.05 )

# f_line = GetLine()
# f_line.Draw("PSAME")

# c.SaveAs(".pdf")