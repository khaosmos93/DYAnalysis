import sys
import os
from ROOT import TFile, TDirectory, TCanvas, TGraphAsymmErrors

###################
# -- functions -- #
###################
def SaveGraph_1D(FileName, outputFile):

	myfile = TFile( FileName )
	print "\nnow loading [%s]" % FileName

	#########################################
	# -- check the scenario (data or MC) -- #
	#########################################
	Scenario = ""
	GraphName = ""
	if "data_25ns" in FileName:
		Scenario = "Data"
		GraphName = Scenario + "_" + FileName.split("_data_25ns_")[1:][0].split(".root")[:1][0]
		
	elif "mc" in FileName:
		Scenario = "MC"
		GraphName = Scenario + "_" + FileName.split("_mc_")[1:][0].split(".root")[:1][0]

	#########################
	# -- Make graph name -- #
	#########################
	
	# print "GraphName will be [%s]" % GraphName

	TreeName = "muonEffs"

	#########################
	# -- sub-directory 1 -- #
	#########################
	Path1 = "%s" % TreeName
	Dir1 = myfile.GetDirectory( Path1 );
	print "now in %s" % Path1

	if Dir1.GetListOfKeys().GetEntries() > 2:
		print "More than 2 directories under %s" % Path1
		sys.exit()

	#########################
	# -- sub-directory 2 -- #
	#########################
	Path2 = "%s/%s/fit_eff_plots" % ( TreeName, Dir1.GetListOfKeys().At(0).GetName() )
	Dir2 = myfile.GetDirectory( Path2 )
	print "now in %s" % Path2

	if Dir2.GetListOfKeys().GetEntries() > 1:
		print "More than 1 directory under %s" % Path2
		sys.exit()

	############################
	# -- Get canvas & graph -- #
	############################
	canvas = Dir2.GetListOfKeys().At(0).ReadObj()
	graph = canvas.GetPrimitive("hxy_fit_eff").Clone();
	name_before = canvas.GetName()
	graph.SetName( GraphName )
	print "\tgraph: %s -> %s" % (name_before, GraphName)

	##############
	# -- Save -- #
	##############
	outputFile.cd()
	graph.Write()

	myfile.Close()


def SaveGraph_2D(FileName, outputFile):
	myfile = TFile( FileName )
	print "\nnow loading [%s]" % FileName

	#########################################
	# -- check the scenario (data or MC) -- #
	#########################################
	Scenario = ""
	GraphName_Base = ""
	if "data_25ns" in FileName:
		Scenario = "Data"
		GraphName_Base = Scenario + "_" + FileName.split("_data_25ns_")[1:][0].split(".root")[:1][0]
		
	elif "mc" in FileName:
		Scenario = "MC"
		GraphName_Base = Scenario + "_" + FileName.split("_mc_")[1:][0].split(".root")[:1][0]

	TreeName = "muonEffs"

	#########################
	# -- sub-directory 1 -- #
	#########################
	Path1 = "%s" % TreeName
	Dir1 = myfile.GetDirectory( Path1 )
	print "now in %s" % Path1

	if Dir1.GetListOfKeys().GetEntries() > 2:
		print "More than 2 directories under %s" % Path1
		sys.exit()

	#########################
	# -- sub-directory 2 -- #
	#########################
	Path2 = "%s/%s/fit_eff_plots" % ( TreeName, Dir1.GetListOfKeys().At(0).GetName() )
	Dir2 = myfile.GetDirectory( Path2 )
	print "now in %s" % Path2

	i_absetabin = 0
	List_Graphs = []
	for key in Dir2.GetListOfKeys():
		if "pt_PLOT_eta_bin" in key.GetName():
			# print key.GetName()

			canvas = key.ReadObj()
			graph = canvas.GetPrimitive("hxy_fit_eff").Clone()

			name_before = canvas.GetName()
			name_after = GraphName_Base + "_abseta%s" % (i_absetabin)
			graph.SetName( name_after )
			i_absetabin += 1

			print "\tgraph: %s -> %s" % (name_before, name_after)
			List_Graphs += [graph]

	outputFile.cd()
	for graph in List_Graphs:
		graph.Write()

	myfile.Close()

def PrintFitCanvases(FileName):
	myfile = TFile( FileName )
	print "\nnow loading [%s]" % FileName

	#########################################
	# -- check the scenario (data or MC) -- #
	#########################################
	Scenario = ""
	DirName = ""
	if "data_25ns" in FileName:
		Scenario = "Data"
		DirName = Scenario + "_" + FileName.split("_data_25ns_")[1:][0].split(".root")[:1][0]
		
	elif "mc" in FileName:
		Scenario = "MC"
		DirName = Scenario + "_" + FileName.split("_mc_")[1:][0].split(".root")[:1][0]

	if DirName not in os.listdir("./"):
		os.mkdir( DirName )

	TreeName = "muonEffs"

	#########################
	# -- sub-directory 1 -- #
	#########################
	Path1 = "%s" % TreeName
	Dir1 = myfile.GetDirectory( Path1 );
	print "now in %s" % Path1

	if Dir1.GetListOfKeys().GetEntries() > 2:
		print "More than 2 directories under %s" % Path1
		sys.exit()

	#########################
	# -- sub-directory 2 -- #
	#########################
	Path2 = "%s/%s" % ( TreeName, Dir1.GetListOfKeys().At(0).GetName() )
	Dir2 = myfile.GetDirectory( Path2 )
	print "now in %s" % Path2

	for key in Dir2.GetListOfKeys():
		FolderName = key.GetName()
		
		if "_eff" not in FolderName: 
			Path3 = Path2 + "/%s" % key.GetName()
			Dir3 = myfile.GetDirectory( Path3 )

			for key2 in Dir3.GetListOfKeys():
				if( key2.GetName() == "fit_canvas" ):
					canvas = key2.ReadObj()
					canvas.SaveAs( "./%s/%s.pdf" % (DirName, FolderName ) )

	myfile.Close()


# -- main part -- #
outputFile = TFile("ROOTFile_EfficiencyGraphs.root", "RECREATE")

List_ROOTFile_1D = []
List_ROOTFile_2D = []
for file in os.listdir("./"):
	ext = os.path.splitext(file)[-1]
	if ext == '.root' and file != "ROOTFile_EfficiencyGraphs.root": # -- find root file -- #
		if "_pteta.root" in file:
			List_ROOTFile_2D += [file]
		else:
			List_ROOTFile_1D += [file]

# -- 1D graphs -- #
for rootfile in List_ROOTFile_1D:
	SaveGraph_1D( rootfile, outputFile )
	PrintFitCanvases( rootfile )
	# if "mc" in rootfile:
		# PrintFitCanvases( rootfile )

# -- 2D graphs -- #
for rootfile in List_ROOTFile_2D:
	SaveGraph_2D( rootfile, outputFile )
	PrintFitCanvases( rootfile )


outputFile.Close()



