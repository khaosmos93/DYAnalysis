#!/bin/bash

anTag="2D${anTagUser}"      # 1D or 2D plus analysisTag_USER, see DYTools.hh
filename_data="../config_files/test.conf"
filename_mc="../config_files/summer12mc.input"
filename_cs="../config_files/xsecCalc8TeV.conf"
triggerSet="Full2012_hltEffOld"        # for primary 7 TeV result change to Full2011_hltEffOld
fsrUnfSet="_fsrUnfGood"
fsrPUReweight=1   # makeUnfoldingMatrixFsr
debugMode=0

## controlling your work
do_all_steps=0
do_post_selection_steps=0
# specify whether you want to clean the old logs
clear_old_logs=0
# specify whether the support files need to be rebuilt
force_rebuild_include_files=0


# individual flags. 
do_selection=0
do_prepareYields=1
do_subtractBackground=0
do_unfolding=0
do_unfoldingFsr=0
do_unfoldingSyst=0 
do_acceptance=0
do_acceptanceSyst=0
do_efficiency=0
do_setupTheory=0
do_crossSectionFsr=0
do_plotXSec=0

# the variables below are more persistent
crossSectionTag="DY"
expectSelectedEventsFile="../root_files/selected_events/${crossSectionTag}/ntuples/zee${anTagUser}_select.root"
expectSelectedEventsFile2="../root_files/selected_events/${crossSectionTag}/npv${anTagUser}.root"
expectYieldsFile="../root_files/yields/${crossSectionTag}/yields${anTag}.root"
expectBkgSubtractedFile="../root_files/yields/${crossSectionTag}/yields_bg-subtracted${anTag}.root"
expectUnfoldingFile="../root_files/constants/${crossSectionTag}/unfolding_constants${anTag}.root"
expectUnfoldingSystematicsFile="../root_files/systematics/${crossSectionTag}/unfolding_systematics${anTag}.root"
expectEfficiencyFile="../root_files/constants/${crossSectionTag}/event_efficiency_constants${anTag}.root"
expectAcceptanceSystematicsFile="../root_files/systematics/${crossSectionTag}/acceptance_FSR_systematics${anTag}.root"
expectXSecFile=  # defined later
expectXSecThFile="../root_files/theory/xSectTheory1D_MSTW2008.root"
expectXSecPlotFile=  # defined later

tmp=${anTag/2D/}
if [ ${#tmp} -eq ${#anTag} ] ; then
  fname=
  expectXSecFile="../root_files/${crossSectionTag}${fsrUnfSet}/xSec_results_1D.root"
  expectXSecPlotFile="../ShapeR/plots_1D_${crossSectionTag}/png/cXsec_preFsrNorm_1D.png"
else
  expectXSecFile="../root_files/${crossSectionTag}${fsrUnfSet}/xSecDET_results_2D.root"
  expectXSecPlotFile="../ShapeR/plots_2D_${crossSectionTag}/png/cXsec_preFsrDetNorm_2D.png"
fi

# export some variables
export mainConfInputFile=${filename_data}
export mcConfInputFile=${filename_mc}
export xsecConfInputFile=${filename_cs}
export triggerSet="${triggerSet}"

# use logDir="./" if you want that the log files are placed in the directory
# where the producing script resides
logDir="./"
logDir="../logs-${anTag}"
if [ ! -d ${logDir} ] ; then mkdir ${logDir}; fi
if [ ${clear_old_logs} -eq 1 ] && [ "${logDir}"="../logs" ] ; then
    echo -e "\tclean old logs\n"
    rm -f ${logDir}/*log
fi

# if you do not want to have the time stamp, comment the line away 
# or set timeStamp=
timeStamp="-`date +%Y%m%d-%H%M`"
#timeStamp=

#
# no error flag
#

noError=1

# -------------  Change individual flags in agreement with superior flags

if [ ${do_all_steps} -eq 1 ] ; then
  do_selection=1
  do_post_selection_steps=1
fi

if [ ${do_post_selection_steps} -eq 1 ] ; then
    do_prepareYields=1
    do_subtractBackground=1
    do_unfolding=1
    do_unfoldingFsr=1
    do_unfoldingSyst=1  # long calculation
    do_acceptance=1
    do_acceptanceSyst=1
    do_efficiency=1
    do_setupTheory=1
    do_crossSection=0
    do_crossSectionFsr=1
#   do_escaleSystematics=0   # very long calculation
    do_plotXSec=1
fi


# ------------------------- Define a few functions

checkFile() { 
  if [ $? != 0 ] || [ ${noError} -eq 0 ] ; then 
      noError=0
      echo "error present before checking for the file(s) $@"
      return
  fi

  # double underscore not to mess a possible variable with the same name in the script
  for __fname in $@ ; do
# if "-f" does not work (check plain file), 
# one can use -e (name exists), but then directory will return 'true' as well
# 2. else... echo ".. ok" can be removed
  if [ ${#__fname} -gt 0 ] ; then 
      if [ ! -f ${__fname} ] ; then 
	  echo "! FullChain.sh: file <${__fname}> is missing"
	  noError=0
      else
	  echo "! FullChain.sh: file <${__fname}> checked ok"
      fi
  fi
  done
}

get_status() {
    checkFile $@
    RUN_STATUS="OK"
    if [ ${noError} -eq 0 ] ; then 
       RUN_STATUS="FAILED"
    fi
}

# -------------------- Preparatory checks

checkFile ${filename_data} ${filename_mc} ${filename_cs} 
checkFile ${tnpMCFile} ${tnpDataFile}

if [ ${debugMode} -eq 1 ] ; then
    echo -e "\n\n"
    echo -e "\t--------------------------------------------------------------"
    echo -e "\t\t (debugMode=1): scripts are called in DEBUG MODE"
    echo -e "\t--------------------------------------------------------------"
    echo -e "\n\n"
fi


if [ ${force_rebuild_include_files} -eq 1 ] ; then
    echo -e " All libraries in Include will be rebuilt\n"
    cd ../Include
    rm -f *.so
    cd ../FullChain
fi


# -------------------- Main work

# prepare support libraries
cd ../Include
root -b -q -l rootlogon.C+             # | tee ${logDir}/out${timeStamp}-00-include${anTag}.log


#Selection
if [ ${do_selection} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusSelection=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: selectEvents(\"${filename_data}\",\"${triggerSet}\",NORMAL,debug=${debugMode})"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../ControlPlots
rm -f *.so ${expectSelectedEventsFile} ${expectSelectedEventsFile2}
echo
checkFile selectEvents.C
root -b -q -l ${LXPLUS_CORRECTION} selectEvents.C+\(\"$filename_data\",\"$triggerSet\",DYTools::NORMAL,${debugMode}\)           | tee ${logDir}/out${timeStamp}-01-selectEvents${anTag}.log
get_status ${expectSelectedEventsFile} ${expectSelectedEventsFile2}
statusSelection=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: selectEvents(\"${filename_data}\",\"${triggerSet}\",NORMAL)"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
  statusSelection=skipped
fi

#prepareYields
if [ ${do_prepareYields} -eq 1 ] && [ ${noError} -eq 1 ]  ; then
statusPrepareYields=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: prepareYields.C(\"${filename_data}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../ControlPlots
rm -f *.so ${expectYieldsFile}
echo
checkFile prepareYields.C ${expectSelectedEventsFile} ${expectSelectedEventsFile2}
root -b -q -l ${LXPLUS_CORRECTION} prepareYields.C+\(\"$filename_data\"\)       | tee ${logDir}/out${timeStamp}-02-prepareYields${anTag}.log
get_status ${expectYieldsFile}
statusPrepareYields=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: prepareYields.C(\"${filename_data}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else 
    statusPrepareYields=skipped
fi

#SubtractBackground
if [ ${do_subtractBackground} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusSubtractBackground=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: subtractBackground.C(\"${filename_data}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../ControlPlots
rm -f *.so ${expectBkgSubtractedFile}
echo
checkFile subtractBackground.C ${expectYieldsFile}
root -b -q -l ${LXPLUS_CORRECTION} subtractBackground.C+\(\"$filename_data\"\)      | tee ${logDir}/out${timeStamp}-03-subtractBackground${anTag}.log
get_status ${expectBkgSubractedFile}
statusSubtractBackground=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: subtractBackground.C(\"${filename_data}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusSubtractBackground=skipped
fi

#Unfolding FSR
if [ ${do_unfolding} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusUnfolding=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: makeUnfoldingMatrix(\"${filename_mc}\",\"${triggerSet}\",debug=${debugMode})"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../Unfold
rm -f *.so ${expectUnfoldingFile}
echo
checkFile makeUnfoldingMatrix.C
root -b -q -l ${LXPLUS_CORRECTION} makeUnfoldingMatrix.C+\(\"$filename_mc\",\"${triggerSet}\",DYTools::NORMAL,1,1.0,-1.0,${fsrPUReweight},${debugMode}\)    | tee ${logDir}/out${timeStamp}-04-makeUnfoldingMatrix${anTag}.log
get_status ${expectUnfoldingFile}
statusUnfolding=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: makeUnfoldingMatrix(\"${filename_mc}\",\"${triggerSet}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusUnfolding=skipped
fi

#Unfolding
if [ ${do_unfoldingFsr} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusUnfoldingFsr=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: makeUnfoldingMatrixFsr(\"${filename_mc}\",\"${triggerSet}\",debug=${debugMode})"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../FSRUnfold
rm -f *.so ${expectUnfoldingFileFsr}
echo
checkFile makeUnfoldingMatrixFsr.C
root -b -q -l ${LXPLUS_CORRECTION} makeUnfoldingMatrixFsr.C+\(\"$filename_mc\",\"${triggerSet}\",DYTools::NORMAL,1,1.0,-1.0,${fsrPUReweight},${debugMode}\)    | tee ${logDir}/out${timeStamp}-04-makeUnfoldingMatrixFsr${anTag}.log
get_status ${expectUnfoldingFileFsr}
statusUnfoldingFsr=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: makeUnfoldingMatrixFsr(\"${filename_mc}\",\"${triggerSet}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusUnfoldingFsr=skipped
fi

#Unfolding Systematics
if [ ${do_unfoldingSyst} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusUnfoldingSyst=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: evaluateUnfoldingSyst.sh"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../Unfolding
rm -f *.so ${expectUnfoldingSystematicsFile}
echo
checkFile evaluateUnfoldingSyst.sh
source evaluateUnfoldingSyst.sh --debug${debugMode} ${filename_mc} ${filename_cs} ${triggerSet} 1 | tee ${logDir}/out${timeStamp}-05-evaluateUnfoldingSyst${anTag}.log
get_status ${expectUnfoldingSystematicsFile}
statusUnfoldingSyst=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: evaluateUnfoldingSyst.sh"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusUnfoldingSyst=skipped
fi

#Acceptance
if [ ${do_acceptance} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusAcceptance=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: plotDYAcceptance(\"${filename_mc}\",DYTools::NORMAL,1.,-1,debug=${debugMode}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../AccEffMCtruth
rm -f *.so
echo
checkFile plotDYAcceptance.C
root -b -q -l ${LXPLUS_CORRECTION} \
      plotDYAcceptance.C+\(\"$filename_mc\",DYTools::NORMAL,1.,-1,${debugMode}\) \
    | tee ${logDir}/out${timeStamp}-06-plotDYAcceptance${anTag}.log
get_status
statusAcceptance=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: plotDYAcceptance(\"${filename_mc}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusAcceptance=skipped
fi

#Acceptance Systematics
if [ ${do_acceptanceSyst} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusAcceptanceSyst=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: evaluateAcceptanceSyst.sh"
echo "in systematics mode"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../AccEffMCtruth
rm -f *.so ${expectAcceptanceSystematicsFile}
echo
checkFile evaluateAcceptanceSyst.sh
source evaluateAcceptanceSyst.sh ${filename_mc} ${debugMode} | tee ${logDir}/out${timeStamp}-07-evaluateAcceptanceSyst${anTag}.log
get_status ${expectAcceptanceSystematicsFile}
statusAcceptanceSyst=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: evaluateAcceptanceSyst.sh"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusAcceptanceSyst=skipped
fi

#Efficiency
if [ ${do_efficiency} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusEfficiency=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: plotDYEfficiency(\"${filename_mc},\"${triggerSet}\",debug=${debug}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../AccEffMCtruth
rm -f *.so ${expectEfficiencyFile}
echo
checkFile plotDYEfficiency.C
root -b -q -l ${LXPLUS_CORRECTION} plotDYEfficiency.C+\(\"$filename_mc\",\"$triggerSet\",${debugMode}\)       | tee ${logDir}/out${timeStamp}-08-plotDYEfficiency${anTag}.log
get_status ${expectEfficiencyFile}
statusEfficiency=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: plotDYEfficiency(\"${filename_mc},\"${triggerSet}\"}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusEfficiency=skipped
fi

#SetupTheory
if [ ${do_setupTheory} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusTheoryErrors=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: setupTheory(\"${filename_mc}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../Theory
rm -f *.so
echo
checkFile setupTheory.sh
source setupTheory.sh ${filename_mc}  -debug ${debugMode} | tee ${logDir}/out${timeStamp}-11-setupTheory${anTag}.out
get_status
statusSetupTheory=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: setupTheory(\"${filename_mc}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusSetupTheory=skipped
fi

#CrossSection
if [ ${do_crossSection} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusCrossSection=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: CrossSection(\"${filename_cs}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../ShapeR
rm -f *.so ${expectXSecFile} 
#  commented out removal of this file: ${expectXSecThFile}
echo
checkFile calcCrossSection.C
root -q -b -l ${LXPLUS_CORRECTION} calcCrossSection.C+\(\"$filename_cs\"\)     | tee ${logDir}/out${timeStamp}-14-CrossSection${anTag}.out
get_status ${expectXSecFile} ${expectXSecThFile}
statusCrossSection=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: CrossSection(\"${filename_cs}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusCrossSection=skipped
fi

#CrossSectionFsr
if [ ${do_crossSectionFsr} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusCrossSectionFsr=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: CrossSectionFsr(\"${filename_cs}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../ShapeR
rm -f *.so ${expectXSecFile}
#  commented out removal of this file: ${expectXSecThFile}
echo
checkFile calcCrossSectionFsr.C
root -q -b -l ${LXPLUS_CORRECTION} calcCrossSectionFsr.C+\(\"$filename_cs\"\)    \
 | tee ${logDir}/out${timeStamp}-15-CrossSectionFsr${anTag}.out
get_status ${expectXSecFile} ${expectXSecThFile}
statusCrossSectionFsr=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: CrossSectionFsr(\"${filename_cs}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusCrossSectionFsr=skipped
fi

#plotXSec
if [ ${do_plotXSec} -eq 1 ] && [ ${noError} -eq 1 ] ; then
statusPlotXSec=OK
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "WILL DO: plotXsec(\"${filename_cs}\",\"default\",\"${fsrUnfSet}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
cd ../ShapeR
rm -f plotXsec.so ${expectXSecPlotFile}
echo
checkFile plotXsec.C
root -q -b -l ${LXPLUS_CORRECTION} plotXsec.C+\(\"$filename_cs\",\"default\",\"${fsrUnfSet}\"\)  \
  \
 | tee ${logDir}/out${timeStamp}-16-plotXsec${anTag}.out
get_status ${expectXSecPlotFile}
statusPlotXSec=$RUN_STATUS
cd ../FullChain
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
echo "DONE: plotXsec(\"${filename_cs}\",\"default\",\"${fsrUnfSet}\")"
echo "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
else
    statusPlotXSec=skipped
fi



# ------------------------------ final summary

echo "Full chain summary:"
echo "              Selection:    " $statusSelection
echo "          PrepareYields:    " $statusPrepareYields
echo "     SubtractBackground:    " $statusSubtractBackground
echo "              Unfolding:    " $statusUnfolding
echo "           UnfoldingFsr:    " ${statusUnfoldingFsr}
echo "         Syst Unfolding:    " $statusUnfoldingSyst
echo "             Acceptance:    " $statusAcceptance
echo "        Syst Acceptance:    " $statusAcceptanceSyst
echo "             Efficiency:    " $statusEfficiency
echo "            SetupTheory:    " $statusSetupTheory
echo "           CrossSection:    " $statusCrossSection
echo "        CrossSectionFsr:    " $statusCrossSectionFsr
echo "               PlotXSec:    " $statusPlotXSec

if [ ${noError} -eq 0 ] ; then 
  echo
  echo " !! ERROR was detected !!"
  echo


fi
