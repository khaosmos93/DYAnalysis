"Phys" directory contains all codes used for generating ntuples

*ntuple generation
1. Setup the environment: cmsrel CMSSW_7_2_0; cd CMSSW_7_2_0/src; cmsenv
(I did not check that this ntuplemaker works at the version higher than 7_2_0)

2. put "Phys" directory into CMSSW_7_2_0/src, and compile it(scram b)

3. in Phys/DYntupleMaker/ntuples, you can find "mc_cfg.py". This file is the CMSSW configuration file for the ntuple generation.

4. If you want to submit the jobs via CRAB, first you should setup the environment.
(1) GRID: voms-proxy-init --voms cms
(2) CRAB3: source /cvmfs/cms.cern.ch/crab3/crab.sh 

After that, you can use "DYJetsToEEMuMu_M100to200_crab3cfg" file as CRAB3 configuration file. you may change some lines.
( ex> config.Site.storageSite = "...", config.Data.inputDataset = "...", etc)
After changing crab3cfg files appropriately, use this command to submit the job: crab submit -c <crab3 configuration file name>
