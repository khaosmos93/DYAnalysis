#!/usr/bin/env python
from subprocess import Popen

varTypes = ['pt3','eta3'] #['pt_eta'] #['pt2','eta','vtx'] #,'eta','pt2']  #['eta','eta2','pt','pt2','vtx']

for var in varTypes:
#1D basic distributions
    Popen('cmsRun fitMuonID_mc_all_2012_1_53X.py '+var+' cbelVPlus tnpZ_MC_M20_part1_V5homebrew.root',shell=True).wait()
    Popen('cmsRun fitMuonID_mc_all_2012_2_53X.py '+var+' cbelVPlus tnpZ_MC_M20_part1_V5homebrew.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_mc_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17 tnpZ_MC_M20_part1_ABCD_PU.root',shell=True).wait()
    Popen('cmsRun fitMuonID_mc_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17leg tnpZ_MC_M20_part1_V5homebrew.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_mc_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8 tnpZ_MC_M20_part1_ABCD_PU.root',shell=True).wait()
    Popen('cmsRun fitMuonID_mc_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8leg tnpZ_MC_M20_part1_V5homebrew.root',shell=True).wait()
    #next two skipped
    ##Popen('cmsRun fitMuonID_mc_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17 tnpZ_MC_M20_part1_ABCD_PU.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_mc_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17leg tnpZ_MC_M20_part1_ABCD_PU.root',shell=True).wait()
    #split by run range
    #Popen('cmsRun fitMuonID_data_all_2012_1_53X.py '+var+' cbelVPlus tnpZ_dataA.root tnpZ_dataB.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_1_53X.py '+var+' cbelVPlus tnpZ_dataC.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_1_53X.py '+var+' cbelVPlus tnpZ_dataD.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_2_53X.py '+var+' cbelVPlus tnpZ_dataA.root tnpZ_dataB.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_2_53X.py '+var+' cbelVPlus tnpZ_dataC.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_2_53X.py '+var+' cbelVPlus tnpZ_dataD.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17 tnpZ_dataA.root tnpZ_dataB.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17 tnpZ_dataC.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17 tnpZ_dataD.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17leg tnpZ_dataA_V5homebrew.root tnpZ_dataB_V5homebrew.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17leg tnpZ_dataC_V5homebrew.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17Mu8_Mu17leg tnpZ_dataD_V5homebrew.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8 tnpZ_dataA.root tnpZ_dataB.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8 tnpZ_dataC.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8 tnpZ_dataD.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8leg tnpZ_dataA_V5homebrew.root tnpZ_dataB_V5homebrew.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8leg tnpZ_dataC_V5homebrew.root',shell=True).wait()
    #Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus DoubleMu17TkMu8_TkMu8leg tnpZ_dataD_V5homebrew.root',shell=True).wait()
    #FIXME next six are new
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17 tnpZ_dataA.root tnpZ_dataB.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17 tnpZ_dataC.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17 tnpZ_dataD.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17leg tnpZ_dataA.root tnpZ_dataB.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17leg tnpZ_dataC.root',shell=True).wait()
    ##Popen('cmsRun fitMuonID_data_all_2012_3_53X.py '+var+' cbelVPlus tag_DoubleMu17Mu8_Mu17leg tnpZ_dataD.root',shell=True).wait()
