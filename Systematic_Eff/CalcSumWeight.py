from array import array
import TChain from ROOT

BaseDir = "/data1/kplee/DYntuple/80X/"
List_DirName = []
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ttbarTo2L2Nu" )
List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ZZ" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WW" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WZ" )

for DirName in Liset_DirName:
	print "Sample: ", DirName
	chain = TChain("recoTree/DYTree");
	chain.Add( BaseDir + DirName + "/*.root" )
	
	nTotEvent = chain.GetEntries()
	print "# Total event:  ", nTotEvent

	GENEvt_weight = array('d', [0] ) # -- 'double' type, # element = 1, initial number = 0
	chain.SetBranchStatus("*", 0);
	chain.SetBranchStatus("GENEvt_weight", 1);
	chain.SetBranchAddress("GENEvt_weight", GENEvt_weight);

	SumWeight = 0.0
	for i in range( nTotEvent ):
		chain.GetEntry(i)
		print GENEvt_weight[0]

		GenWeight = -1 if GENEvt_weight[0] < 0 else 1

		SumWeight = SumWeight + GenWeight

		if i > 100: break

	print "SumWeight: ", SumWeight
	print "\n"

print "All jobs are finished"

