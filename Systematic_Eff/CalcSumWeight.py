from array import array
from ROOT import TChain
import logging

logging.basicConfig(level=logging.DEBUG, filename="log_SumWeight.txt", filemode="a+",
                    format="%(asctime)-15s %(levelname)-8s %(message)s")
logging.info("start")

BaseDir = "/data1/kplee/DYntuple/80X/"
List_DirName = []

# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WWTo2L2Nu" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ZZ" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WW" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_WZ" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ttbarTo2L2Nu" )
# List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_DYLL_M50toInf" )
List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ST_tW" )
List_DirName.append( "DYntuple_v20170127_80XMoriond17_AddZprimeVar_ST_tbarW" )


for DirName in List_DirName:
	logging.info( "Sample: %s" % (DirName) )
	chain = TChain("recoTree/DYTree");
	chain.Add( BaseDir + DirName + "/*.root" )

	GENEvt_weight = array('d', [0] ) # -- 'double' type, # element = 1, initial number = 0 -- #
	chain.SetBranchStatus("*", 0);
	chain.SetBranchStatus("GENEvt_weight", 1);
	chain.SetBranchAddress("GENEvt_weight", GENEvt_weight);

	nTotEvent = chain.GetEntries()
	logging.info( "\tTotal event: %d" % (nTotEvent) )

	nTotEvent = 10000

	SumWeight = 0.0
	for i in range( nTotEvent ):
		chain.GetEntry(i)

		# print GENEvt_weight[0]

		GenWeight = -1 if GENEvt_weight[0] < 0 else 1
		SumWeight = SumWeight + GenWeight

	logging.info( "\tSumWeight: %.1lf\n" % (SumWeight) )

logging.info("All jobs are finished")
