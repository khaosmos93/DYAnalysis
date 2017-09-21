#cd /share_home/msoh/ZPrime/KPNtuple/CMSSW_8_0_26/src/DYAnalysis/Systematic_Eff/test_v22_Zprime_PhiDip/Data_0
#cp ProdHist_Systematic_Eff_PhiDip.cxx ../Data_1
#cp Run_0.py ../Data_1/Run_1.py

#cp ProdHist_Systematic_Eff_PhiDip.cxx ../Data_2
#cp Run_0.py ../Data_2/Run_2.py

#cp ProdHist_Systematic_Eff_PhiDip.cxx ../Data_3
#cp Run_0.py ../Data_3/Run_3.py

#cp ProdHist_Systematic_Eff_PhiDip.cxx ../Data_4
#cp Run_0.py ../Data_4/Run_4.py

#cp ProdHist_Systematic_Eff_PhiDip.cxx ../Data_5
#cp Run_0.py ../Data_5/Run_5.py

#cp ProdHist_Systematic_Eff_PhiDip.cxx ../Data_6
#cp Run_0.py ../Data_6/Run_6.py

cd /share_home/msoh/ZPrime/KPNtuple/CMSSW_8_0_26/src/DYAnalysis/Systematic_Eff/test_v22_Zprime_PhiDip/Nminus1/Data_0
source script_qsub_ALL.sh

cd ../Data_3
source script_qsub_ALL.sh
