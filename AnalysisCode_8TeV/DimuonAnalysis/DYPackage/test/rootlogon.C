// This is the file rootlogon.C
{
   //SET UP ROO
   gSystem->Load("/afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Unfold/RooUnfold-1.1.1/libRooUnfold.so");

   printf("Set include paths for rootfit\n"); 
   gSystem->Setenv("ROOT_INCLUDE","$ROOTSYS/include");
   gSystem->Setenv("ROOFIT_INCLUDE","$ROOFITSYS/include");
   gSystem->SetIncludePath("-I$ROOT_INCLUDE -I$ROOFIT_INCLUDE");

   gROOT->Macro("/afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/CPlot.cc+");
   gROOT->Macro("/afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/MitStyleRemix.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TDielectron.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TElectron.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/DYTools.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TMuon.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TEventInfo.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TGenInfo.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TPhoton.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TGenPhoton.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TJet.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TVertex.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/EleIDCuts.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/ZeeData.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/TriggerSelection.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/JsonParser.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/EtaEtaMass.hh+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/ElectronEnergyScale.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/FEWZ.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/EventSelector.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/InputFileMgr.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/PUReweight.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Unfold/UnfoldingTools.C+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/plotFunctions.cc+");
   gROOT->ProcessLine(".L /afs/cern.ch/work/a/asvyatko/NEWDY/CMSSW_5_3_11/src/DimuonAnalysis/DYPackage/test/Include/latexPrintouts.cc+");
}
