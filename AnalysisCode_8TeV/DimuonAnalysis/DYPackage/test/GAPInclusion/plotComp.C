void plotComp() {

   gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/histotools.C");


   TFile* f = new TFile("out2_EE_GAP.root");
   f->cd();
   TH1D* gap_eff = (TH1D*)eff_postFSRcorr->Clone();
   TH1D* gap_acc = (TH1D*)acc_postFSRcorr->Clone();

   TH1D* gap_acceff = histoMultiply2(gap_eff,gap_acc,true);

   TFile* g = new TFile("out2_EE_NOGAP.root");
   g->cd();
   TH1D* nogap_eff = (TH1D*)eff_postFSRcorr->Clone();
   TH1D* nogap_acc = (TH1D*)acc_postFSRcorr->Clone();

   TH1D* nogap_acceff = histoMultiply2(nogap_eff,nogap_acc,true);

   TH1D* h1_mca = get2DSlice(gap_eff, "absrap2030",true,true); //WHAT IS THIS FALSE?? adds +23 shift! 
   TH1D* h1_dda = get2DSlice(nogap_eff, "absrap2030",true,true);  
   TH1D* h2_mca = get2DSlice(gap_eff, "absrap3045",true,true);  
   TH1D* h2_dda = get2DSlice(nogap_eff, "absrap3045",true,true);  
   TH1D* h3_mca = get2DSlice(gap_eff, "absrap4560",true,true);  
   TH1D* h3_dda = get2DSlice(nogap_eff, "absrap4560",true,true);  
   TH1D* h4_mca = get2DSlice(gap_eff, "absrap60120",true,true);  
   TH1D* h4_dda = get2DSlice(nogap_eff, "absrap60120",true,true);  
   TH1D* h5_mca = get2DSlice(gap_eff, "absrap120200",true,true);  
   TH1D* h5_dda = get2DSlice(nogap_eff, "absrap120200",true,true);  
   TH1D* h6_mca = get2DSlice(gap_eff, "absrap200",true,true);  
   TH1D* h6_dda = get2DSlice(nogap_eff, "absrap200",true,true);  
 
   gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/plottools.C");

/*
   makeAPlot((TH1F*)h1_mca,(TH1F*)h1_dda,"ECAL gap not excluded", "ECAL gap excluded","#epsilon/#epsilon(No gap)"); 
   makeAPlot((TH1F*)h2_mca,(TH1F*)h2_dda,"ECAL gap not excluded", "ECAL gap excluded","#epsilon/#epsilon(No gap)");
   makeAPlot((TH1F*)h3_mca,(TH1F*)h3_dda,"ECAL gap not excluded", "ECAL gap excluded","#epsilon/#epsilon(No gap)");
   makeAPlot((TH1F*)h4_mca,(TH1F*)h4_dda,"ECAL gap not excluded", "ECAL gap excluded","#epsilon/#epsilon(No gap)");
   makeAPlot((TH1F*)h5_mca,(TH1F*)h5_dda,"ECAL gap not excluded", "ECAL gap excluded","#epsilon/#epsilon(No gap)");
   makeAPlot((TH1F*)h6_mca,(TH1F*)h6_dda,"ECAL gap not excluded", "ECAL gap excluded","#epsilon/#epsilon(No gap)");
*/

   TH1D* h1_mcb = get2DSlice(gap_acc, "absrap2030",true,true);  
   TH1D* h1_ddb = get2DSlice(nogap_acc, "absrap2030",true,true);  
   TH1D* h2_mcb = get2DSlice(gap_acc, "absrap3045",true,true);  
   TH1D* h2_ddb = get2DSlice(nogap_acc, "absrap3045",true,true);  
   TH1D* h3_mcb = get2DSlice(gap_acc, "absrap4560",true,true);  
   TH1D* h3_ddb = get2DSlice(nogap_acc, "absrap4560",true,true);  
   TH1D* h4_mcb = get2DSlice(gap_acc, "absrap60120",true,true);  
   TH1D* h4_ddb = get2DSlice(nogap_acc, "absrap60120",true,true);  
   TH1D* h5_mcb = get2DSlice(gap_acc, "absrap120200",true,true);  
   TH1D* h5_ddb = get2DSlice(nogap_acc, "absrap120200",true,true);  
   TH1D* h6_mcb = get2DSlice(gap_acc, "absrap200",true,true);  
   TH1D* h6_ddb = get2DSlice(nogap_acc, "absrap200",true,true);  

   makeAPlot((TH1F*)h1_mcb,(TH1F*)h1_ddb,"ECAL gap not excluded", "ECAL gap excluded","A/A(No gap)"); 
   makeAPlot((TH1F*)h2_mcb,(TH1F*)h2_ddb,"ECAL gap not excluded", "ECAL gap excluded","A/A(No gap)");
   makeAPlot((TH1F*)h3_mcb,(TH1F*)h3_ddb,"ECAL gap not excluded", "ECAL gap excluded","A/A(No gap)");
   makeAPlot((TH1F*)h4_mcb,(TH1F*)h4_ddb,"ECAL gap not excluded", "ECAL gap excluded","A/A(No gap)");
   makeAPlot((TH1F*)h5_mcb,(TH1F*)h5_ddb,"ECAL gap not excluded", "ECAL gap excluded","A/A(No gap)");
   makeAPlot((TH1F*)h6_mcb,(TH1F*)h6_ddb,"ECAL gap not excluded", "ECAL gap excluded","A/A(No gap)");

   TH1D* h1_mcab = get2DSlice(gap_acceff, "absrap2030",true,true);  
   TH1D* h1_ddab = get2DSlice(nogap_acceff, "absrap2030",true,true);  
   TH1D* h2_mcab = get2DSlice(gap_acceff, "absrap3045",true,true);  
   TH1D* h2_ddab = get2DSlice(nogap_acceff, "absrap3045",true,true);  
   TH1D* h3_mcab = get2DSlice(gap_acceff, "absrap4560",true,true);  
   TH1D* h3_ddab = get2DSlice(nogap_acceff, "absrap4560",true,true);  
   TH1D* h4_mcab = get2DSlice(gap_acceff, "absrap60120",true,true);  
   TH1D* h4_ddab = get2DSlice(nogap_acceff, "absrap60120",true,true);  
   TH1D* h5_mcab = get2DSlice(gap_acceff, "absrap120200",true,true);  
   TH1D* h5_ddab = get2DSlice(nogap_acceff, "absrap120200",true,true);  
   //TH1D* h6_mcab = get2DSlice(gap_acceff, "absrap200",true,true);  
   //TH1D* h6_ddab = get2DSlice(nogap_acceff, "absrap200",true,true);  

   TH1D* h6_mcab = histoMultiply2(h6_mca,h6_mcb,true);
   TH1D* h6_ddab = histoMultiply2(h6_dda,h6_ddb,true);
/*
   makeAPlot((TH1F*)h1_mcab,(TH1F*)h1_ddab,"ECAL gap not excluded", "ECAL gap excluded","A#epsilon/A#epsilon(No gap)"); 
   makeAPlot((TH1F*)h2_mcab,(TH1F*)h2_ddab,"ECAL gap not excluded", "ECAL gap excluded","A#epsilon/A#epsilon(No gap)");
   makeAPlot((TH1F*)h3_mcab,(TH1F*)h3_ddab,"ECAL gap not excluded", "ECAL gap excluded","A#epsilon/A#epsilon(No gap)");
   makeAPlot((TH1F*)h4_mcab,(TH1F*)h4_ddab,"ECAL gap not excluded", "ECAL gap excluded","A#epsilon/A#epsilon(No gap)");
   makeAPlot((TH1F*)h5_mcab,(TH1F*)h5_ddab,"ECAL gap not excluded", "ECAL gap excluded","A#epsilon/A#epsilon(No gap)");
   makeAPlot((TH1F*)h6_mcab,(TH1F*)h6_ddab,"ECAL gap not excluded", "ECAL gap excluded","A#epsilon/A#epsilon(No gap)");
*/
}
