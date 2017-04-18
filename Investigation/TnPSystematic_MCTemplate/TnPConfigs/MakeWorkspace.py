import os
import sys

List_Dir = ['RecoID_Iso', 'HLTv4p2', 'HLTv4p3']
List_Configs = ['SaveEfficiencyGraphs.py', 'Def_AllTemplatePDF.py', 'MuonEffs_MCTemplate.py']
ScriptName = 'RunTnP_All.sh'
TnPDir = 'v20170407_1st_MCTemplate'

path_TnP = os.getenv('KP_TNP_PATH')
if not path_TnP:
	print "TnP path is not set!"
	sys.exit()
path_TnP = path_TnP + '/%s' % TnPDir

# -- create configuration with MC template definitions -- #
cmd_makeTemplateDef = 'python MakeConfigForTemplate.py'
print cmd_makeTemplateDef
os.system( cmd_makeTemplateDef )

# -- copy all configurations in each directory -- #
List_File = os.listdir('.')
for Dir in List_Dir:
	# -- make directories if they didn't exist -- #
	if Dir not in List_File:
		os.mkdir( Dir )

	for config in List_Configs:
		cmd = 'cp %s ./%s' % (config, Dir)
		print cmd
		os.system( cmd )

# -- copy to CMSSW area -- #
cmd_copy = 'cp -r * %s' % (path_TnP)
print cmd_copy
os.system( cmd_copy )

print "Workspace is created"

