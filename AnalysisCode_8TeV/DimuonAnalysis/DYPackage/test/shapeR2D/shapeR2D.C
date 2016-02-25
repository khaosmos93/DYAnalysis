#include <iostream>
#include <iomanip>
using namespace std;

TH1D* getXsection(TH1D* huy, TH1D* hacceff, TH1D* hacc, TH1D* heff, TH1D* heffcorr_dd, int xsectype, TString RR, bool useFSR_BBB, TString systOn) {
    //cout << huy->GetName() << " " << hacceff->GetName() << " " << heffcorr_dd->GetName() << endl;
    TH1D* crossSection = huy->Clone();
    if (xsectype == 2) {
      cout << "preFSR - detector acceptance" << endl;
      for (int ibin = 0; ibin < huy->GetNbinsX(); ibin++) { 
         if (useFSR_BBB) crossSection->SetBinContent(ibin+1,huy->GetBinContent(ibin+1)/heff->GetBinContent(ibin+1)/heffcorr_dd->GetBinContent(ibin+1));
         else crossSection->SetBinContent(ibin+1,huy->GetBinContent(ibin+1)/heff->GetBinContent(ibin+1)); 
         if (RR == "POWHEG") { 
             crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
         } else {
             if (!useFSR_BBB) { 
                   crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)));
             } else { 
                   if (systOn == "on") crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
                   else crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)));
             }
         }
    }
  } else if (xsectype == 1) {
      cout << "preFSR - full acceptance" << endl;
      for (int ibin = 0; ibin < huy->GetNbinsX(); ibin++) {
         if (useFSR_BBB) crossSection->SetBinContent(ibin+1,huy->GetBinContent(ibin+1)/hacceff->GetBinContent(ibin+1)/heffcorr_dd->GetBinContent(ibin+1));
         else crossSection->SetBinContent(ibin+1,huy->GetBinContent(ibin+1)/hacceff->GetBinContent(ibin+1));
         if (RR == "POWHEG") {
             crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
         } else {
             if (!useFSR_BBB) {
                   crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)));
             } else {
                   if (systOn == "on") crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
                   else crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)));
            }
        }
     }  
  } else if (xsectype == 3) {
      cout << "postFSR - detector acceptance" << endl;
      for (int ibin = 0; ibin < huy->GetNbinsX(); ibin++) {
         crossSection->SetBinContent(ibin+1,huy->GetBinContent(ibin+1)/heff->GetBinContent(ibin+1)/heffcorr_dd->GetBinContent(ibin+1));
         if (RR == "POWHEG") {
             crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
         } else {
             if (systOn == "on") crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
             else crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(heff->GetBinError(ibin+1)/heff->GetBinContent(ibin+1),2)));
         }
     }
  } else if (xsectype == 4) {
      cout << "postFSR - full acceptance" << endl;
      for (int ibin = 0; ibin < huy->GetNbinsX(); ibin++) {
         crossSection->SetBinContent(ibin+1,huy->GetBinContent(ibin+1)/hacceff->GetBinContent(ibin+1)/heffcorr_dd->GetBinContent(ibin+1));
         if (RR == "POWHEG") {
             crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
         } else {
             if (systOn == "on") crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)+pow(heffcorr_dd->GetBinError(ibin+1)/heffcorr_dd->GetBinContent(ibin+1),2)));
             else crossSection->SetBinError(ibin+1,crossSection->GetBinContent(ibin+1)*sqrt(pow(huy->GetBinError(ibin+1)/huy->GetBinContent(ibin+1),2)+pow(hacceff->GetBinError(ibin+1)/hacceff->GetBinContent(ibin+1),2)));
        }
     }
  }

    return crossSection;
}

double* getNormalizationXsection(TH1D* thisH) { 
    //FIXME it is supposed to get the rshape and just get the value (integral in the mass bin4)
    double norm_constants[2] = {0};
    double thisNorm = 0.;
    double ithisNorm_err = 0.;
    double thisHNorm_err = 0.;
    for (int ibin = 72; ibin < 96; ibin++) { 
        thisNorm+=thisH->GetBinContent(ibin+1);
        ithisNorm_err+=pow(thisH->GetBinError(ibin+1),2);
    } 
    thisHNorm_err = sqrt(ithisNorm_err);
    norm_constants[0] = thisNorm;
    norm_constants[1] = thisHNorm_err;
    return norm_constants;
}

void shapeR2D(TString WHICHTYPE, TString gcompareToPOWHEG, TString gnormalizeToZ, TString RR, TString applyFEWZ) 
{

  TString systOn = "on";
  bool useFSR_BBB = true;
  TString thisPDF = "CTEQ12NNLO";
  TString thisPDF2 = "NNPDF21nnlo";
  bool compareToPOWHEG = false;
  if (gcompareToPOWHEG == "True") compareToPOWHEG = true;
  bool normalizeToZ = false;
  if (gnormalizeToZ == "True") normalizeToZ = true; 

  // 0: all corrections are applied, preFSR; 
  // 1: all corrections are applied, postFSR; 
  // 2: no acceptance correction, preFSR, 
  // 3: no acceptance correction, postFSR
  int shaperType = 2; //inAcc pre fsr
  if (WHICHTYPE == "_fullAcc") shaperType = 1; //ore fsr
  else if (WHICHTYPE == "_inAccPostFSR") shaperType = 3;
  else if (WHICHTYPE == "_fullAccPostFSR") shaperType = 4;

  const int nbinsM = 6;

  //
  // FEWZ NNLO x-section. Read all 6 histos into array of TH1D
  //
  TString subfolder = "_cuts_";
  if (shaperType == 1 || shaperType == 4) subfolder = "_plain_"; 

  TString beauty_slices[nbinsM] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};

  gROOT->ProcessLine(".L ../tools/misctools.C");
  gROOT->ProcessLine(".L ../tools/histotools.C");

  TH1D* hfewz1 = acceptFEWZInputs(beauty_slices[0], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz2 = acceptFEWZInputs(beauty_slices[1], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz3 = acceptFEWZInputs(beauty_slices[2], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz4 = acceptFEWZInputs(beauty_slices[3], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz5 = acceptFEWZInputs(beauty_slices[4], subfolder, thisPDF, normalizeToZ);
  TH1D* hfewz6 = acceptFEWZInputs(beauty_slices[5], subfolder, thisPDF, normalizeToZ);

  TH1D* hfewz1b = acceptFEWZInputs(beauty_slices[0], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz2b = acceptFEWZInputs(beauty_slices[1], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz3b = acceptFEWZInputs(beauty_slices[2], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz4b = acceptFEWZInputs(beauty_slices[3], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz5b = acceptFEWZInputs(beauty_slices[4], subfolder, thisPDF2, normalizeToZ);
  TH1D* hfewz6b = acceptFEWZInputs(beauty_slices[5], subfolder, thisPDF2, normalizeToZ);

  // unfolded yield without FSR correction
  TString RR2 = "";
  if (RR=="_") RR2 = "";
  else RR2 = "_"+RR; //+"_"; 
  TH1D* hyield;
  TH1D* hyieldFSR;
  TH1D* hFSRextra;

  if (useFSR_BBB) {
    TFile* f = new TFile("../Inputs/unfold/unfoldData_HLT_Double_2D.root");
    f->cd();
    hyield = (TH1D*)hunfolded->Clone();
    // unfold sys
    TFile* f21 = new TFile("./sys/unfold_corrRun2011AB_sys_new.root","open");
    f21->cd();
    TH1D* hunfoldSYSAB = (TH1D*) hunfold_sys->Clone();

    TFile* f21b = new TFile("../Inputs/sys/muon_unf_stat_2D.root");
    f21b->cd();
    TH1D* hstat_unf = (TH1D*)gDirectory->Get("muon_unf_stat");
    for (int i = 0; i < hunfoldSYSAB->GetNbinsX(); i++) {
        hunfoldSYSAB->SetBinContent(i+1,sqrt(pow(hunfoldSYSAB->GetBinContent(i+1)*4./5.,2)+pow(hstat_unf->GetBinContent(i+1)/100.,2)));
    }

    TFile* f2= new TFile("../Inputs/FSR/BinByBin_Saved_2D.root");
    f2->cd();
    TH1D* ratio = (TH1D*)gDirectory->Get("ratio2_corr");
    hyieldFSR = (TH1D*)hyield->Clone();
    if (shaperType == 1 || shaperType == 2) hyieldFSR->Divide(hyieldFSR,ratio);
    //Add FSR systematics
    TH1D* FSRsyst = getFSRSystsematics2D(systOn);
    //TFile* fee = new TFile("../Inputs/sys/fsrSysts_SanG_2D.root","recreate");
    //fee->cd();
    //FSRsyst->Write();
    //fee->Close();

    //Add background estimation errors
    TH1D* BkgSyst = getBkgSystematics(systOn);
    //TFile* fee = new TFile("../Inputs/sys/bkg_syst2013_2D_wQCD.root","recreate");
    //fee->cd();
    //BkgSyst->Write("hsyst2D");
    //fee->Close();

    for (int i = 0; i < hyieldFSR->GetXaxis()->GetNbins(); i++) {
      hyieldFSR->SetBinError(i+1,sqrt(hyieldFSR->GetBinContent(i+1)));
      if (systOn = "on") { 
           if (shaperType == 1 || shaperType == 2) { 
               hyieldFSR->SetBinError(i+1,hyieldFSR->GetBinContent(i+1)*sqrt(pow(hunfoldSYSAB->GetBinContent(i+1),2)+pow(hyieldFSR->GetBinError(i+1)/hyieldFSR->GetBinContent(i+1),2)+pow(FSRsyst->GetBinContent(i+1),2)+pow(BkgSyst->GetBinContent(i+1),2)));
           } else {
               hyieldFSR->SetBinError(i+1,hyieldFSR->GetBinContent(i+1)*sqrt(pow(hunfoldSYSAB->GetBinContent(i+1),2)+pow(hyieldFSR->GetBinError(i+1)/hyieldFSR->GetBinContent(i+1),2)+pow(BkgSyst->GetBinContent(i+1),2)));
           }
        }  
     }
  } else {
    cout << "WARNING: Currently not used and needs to be reviewed before using!" << endl;
    TFile* f = new TFile("../Inputs/FSR/FSRunfoldData_HLT_Double_2D_tmp.root");
    f->cd();
    hyieldFSR = (TH1D*)hunfolded->Clone();
    TFile* f2 = new TFile("../Inputs/FSR/addMoreCorrinMC_tmp.root");
    f2->cd();
    hFSRextra = (TH1D*)gDirectory->Get("heff2D_2"+applyFEWZ); 
    hyieldFSR->Divide(hyieldFSR,hFSRextra);
    //remove errors for now? FIXME
    removeErrors(hyieldFSR,1000.);
  }  

  //
  // Efficiency corrections
  //

  // Run2011
  TString _nameEffFile = "_"+RR;
  if (RR=="_") _nameEffFile = "_split"; 

  // reco+id
  // 20-30
  TFile* freco1 = new TFile("../Inputs/effcorr/2D/correction_recoidabsrap2030"+_nameEffFile+"2D.root","open");
  freco1->cd();
  TH1D* eff_recoid1 = (TH1D*)gDirectory->Get("absrap2030");
  // 30-45
  TFile* freco2 = new TFile("../Inputs/effcorr/2D/correction_recoidabsrap3045"+_nameEffFile+"2D.root","open");
  freco2->cd();
  TH1D* eff_recoid2 = (TH1D*)gDirectory->Get("absrap3045");
  // 45-60
  TFile* freco3 = new TFile("../Inputs/effcorr/2D/correction_recoidabsrap4560"+_nameEffFile+"2D.root","open");
  freco3->cd();
  TH1D* eff_recoid3 = (TH1D*)gDirectory->Get("absrap4560");
  // 60-120
  TFile* freco4 = new TFile("../Inputs/effcorr/2D/correction_recoidabsrap60120"+_nameEffFile+"2D.root","open");
  freco4->cd();
  TH1D* eff_recoid4 = (TH1D*)gDirectory->Get("absrap60120");
  // 120-200
  TFile* freco5 = new TFile("../Inputs/effcorr/2D/correction_recoidabsrap120200"+_nameEffFile+"2D.root","open");
  freco5->cd();
  TH1D* eff_recoid5 = (TH1D*)gDirectory->Get("absrap120200");
  // 200-1500
  TFile* freco6 = new TFile("../Inputs/effcorr/2D/correction_recoidabsrap200"+_nameEffFile+"2D.root","open");
  freco6->cd();
  TH1D* eff_recoid6 = (TH1D*)gDirectory->Get("absrap200");

  TH1D* hvector_recoid = sliceCombiner(eff_recoid1,eff_recoid2,eff_recoid3,eff_recoid4,eff_recoid5,eff_recoid6,true);

  // isolation
  // 20-30
  TFile* freco1 = new TFile("../Inputs/effcorr/2D/correction_isoabsrap2030"+_nameEffFile+"2D.root","open");
  freco1->cd();
  TH1D* eff_iso1 = (TH1D*)gDirectory->Get("absrap2030");
  // 30-45
  TFile* freco2 = new TFile("../Inputs/effcorr/2D/correction_isoabsrap3045"+_nameEffFile+"2D.root","open");
  freco2->cd();
  TH1D* eff_iso2 = (TH1D*)gDirectory->Get("absrap3045");
  // 45-60
  TFile* freco3 = new TFile("../Inputs/effcorr/2D/correction_isoabsrap4560"+_nameEffFile+"2D.root","open");
  freco3->cd();
  TH1D* eff_iso3 = (TH1D*)gDirectory->Get("absrap4560");
  // 60-120
  TFile* freco4 = new TFile("../Inputs/effcorr/2D/correction_isoabsrap60120"+_nameEffFile+"2D.root","open");
  freco4->cd();
  TH1D* eff_iso4 = (TH1D*)gDirectory->Get("absrap60120");
  // 120-200
  TFile* freco5 = new TFile("../Inputs/effcorr/2D/correction_isoabsrap120200"+_nameEffFile+"2D.root","open");
  freco5->cd();
  TH1D* eff_iso5 = (TH1D*)gDirectory->Get("absrap120200");
  // 200-1500
  TFile* freco6 = new TFile("../Inputs/effcorr/2D/correction_isoabsrap200"+_nameEffFile+"2D.root","open");
  freco6->cd();
  TH1D* eff_iso6 = (TH1D*)gDirectory->Get("absrap200");
     
  TH1D* hvector_iso = sliceCombiner(eff_iso1,eff_iso2,eff_iso3,eff_iso4,eff_iso5,eff_iso6,true);

  //trigger
  // 20-30
  TFile* freco1 = new TFile("../Inputs/effcorr/2D/correction_trigabsrap2030"+_nameEffFile+"2D.root","open");
  freco1->cd();
  TH1D* eff_trig1 = (TH1D*)gDirectory->Get("absrap2030");
  // 30-45
  TFile* freco2 = new TFile("../Inputs/effcorr/2D/correction_trigabsrap3045"+_nameEffFile+"2D.root","open");
  freco2->cd();
  TH1D* eff_trig2 = (TH1D*)gDirectory->Get("absrap3045");
  // 45-60
  TFile* freco3 = new TFile("../Inputs/effcorr/2D/correction_trigabsrap4560"+_nameEffFile+"2D.root","open");
  freco3->cd();
  TH1D* eff_trig3 = (TH1D*)gDirectory->Get("absrap4560");
  // 60-120
  TFile* freco4 = new TFile("../Inputs/effcorr/2D/correction_trigabsrap60120"+_nameEffFile+"2D.root","open");
  freco4->cd();
  TH1D* eff_trig4 = (TH1D*)gDirectory->Get("absrap60120");
  // 120-200
  TFile* freco5 = new TFile("../Inputs/effcorr/2D/correction_trigabsrap120200"+_nameEffFile+"2D.root","open");
  freco5->cd();
  TH1D* eff_trig5 = (TH1D*)gDirectory->Get("absrap120200");
  // 200-1500
  TFile* freco6 = new TFile("../Inputs/effcorr/2D/correction_trigabsrap200"+_nameEffFile+"2D.root","open");
  freco6->cd();
  TH1D* eff_trig6 = (TH1D*)gDirectory->Get("absrap200");
     
  TH1D* hvector_trig = sliceCombiner(eff_trig1,eff_trig2,eff_trig3,eff_trig4,eff_trig5,eff_trig6,true);
  //get the total efficiency correction
  TH1D* hvector_effcorr = histoMultiply3(hvector_recoid,hvector_iso,hvector_trig,true);
  //FIXME Add the DZ filter efficiency
  TFile* DZfile = new TFile("../Inputs/effcorr/DZ_SF_TRM_smoothed.root"); 
  DZfile->cd();
  TH1D* hDZ = (TH1D*)gDirectory->Get("rap");
  //remove errors for now? FIXME
  removeErrors(hDZ,1000.);
  hvector_effcorr = histoMultiply2(hvector_effcorr,hDZ,true);

  //
  // Acceptance and efficiency
  //
  TFile* facc = new TFile("../Inputs/acc/out2.root");  //out2_1121_w1.root");
  facc->cd();
  TH1D* heff_vector = (TH1D*)gDirectory->Get("eff_postFSR"+applyFEWZ);

  //for (int i = 0; i < hvector_effcorr->GetNbinsX(); i++) {
  //  cout << i+1 << " bin: " << 0.971*hvector_effcorr->GetBinContent(i+1)*heff_vector->GetBinContent(i+1) << " +/- " << sqrt(pow(heff_vector->GetBinError(i+1),2)+pow(hvector_effcorr->GetBinError(i+1),2)+pow(0.01,2)) << endl;
  //}

  TH1D* hacceff_vector = (TH1D*)gDirectory->Get("acceff_postFSR"+applyFEWZ);
  TH1D* hacc_vector = (TH1D*)gDirectory->Get("acc_postFSR"+applyFEWZ);
  //for (int i = 0; i < heff_vector->GetXaxis()->GetNbins(); i++) {
  //   cout << i+1 << " Error " << heff_vector->GetBinError(i+1)/heff_vector->GetBinContent(i+1) << endl;
  //}
  TFile* fmod = new TFile("../Inputs/sys/modeling2013_2D.root");
  fmod->cd();
  TH1D* hrap_FEWZ = (TH1D*)gDirectory->Get("rap_FEWZ");
  for (int i = 0; i < hacceff_vector->GetNbinsX(); i++) {
     if (systOn == "on") hacceff_vector->SetBinError(i+1,hacceff_vector->GetBinContent(i+1)*sqrt(pow(hrap_FEWZ->GetBinContent(i+1),2)+pow(hacceff_vector->GetBinError(i+1)/hacceff_vector->GetBinContent(i+1),2)));
  }


  //
  //SYSTEMATICS
  //

  //
  //GET PDF UNCERTAINTY BAND PER SLICE
  //
  TFile* fsys = new TFile("../Inputs/sys/pdf_syst2D_7TeV.root");
  fsys->cd();
  TH1D* hsys1 = (TH1D*)gDirectory->Get("hslice1");
  TH1D* hsys2 = (TH1D*)gDirectory->Get("hslice2");
  TH1D* hsys3 = (TH1D*)gDirectory->Get("hslice3");
  TH1D* hsys4 = (TH1D*)gDirectory->Get("hslice4");
  TH1D* hsys5 = (TH1D*)gDirectory->Get("hslice5");
  TH1D* hsys6 = (TH1D*)gDirectory->Get("hslice6");

  //
  //CALCULATE THE CROSS SECTION
  //
  TH1D* rshape_vector = getXsection(hyieldFSR, hacceff_vector, hacc_vector, heff_vector, hvector_effcorr, shaperType, RR,useFSR_BBB,systOn);
  //TH1D* rshape_vector2 = (TH1D*)rshape_vector->Clone();
  double norm = rshape_vector->Integral(72,96);
  double norm_err = 0;
  double binWid = 0.1;

  if (!normalizeToZ) {
     norm = 18600.; //18170 lumi
     //binWid = 1.;
  }

  for (int ibin = 0; ibin < rshape_vector->GetNbinsX(); ibin++) { 
     double err = sqrt(pow(rshape_vector->GetBinError(ibin+1)/rshape_vector->GetBinContent(ibin+1),2)); //+pow(norm_err/norm,2));
     if (ibin >= 120 && binWid != 0.2) binWid *= 2.;
     //cout << ibin << " " << binWid << " " << norm << endl;
     rshape_vector->SetBinContent(ibin+1,rshape_vector->GetBinContent(ibin+1)/norm/binWid);
     rshape_vector->SetBinError(ibin+1,rshape_vector->GetBinContent(ibin+1)*err);
  }

  //
  //PLOT THE RESULTS
  //
  gStyle->SetOptStat(0);
  gROOT->ProcessLine(".L ../ControlPlots/setTDRStyle.C");
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");
  gROOT->ForceStyle(true);
  tdrStyle->SetTitleX(0.14);
  tdrStyle->SetTitleY(0.973);

  gStyle->SetEndErrorSize(2);
  gStyle->SetErrorX(0.5);

  //
  // POWHEG
  //
  TFile* fpowheg = new TFile("./powheg/PowhegXsec.root","open");
  fpowheg->cd();
  //get slices back before plotting. FIXME what is hresult
  TH1D* hpowheg1 = (TH1D*)retHist20to30->Clone();
  TH1D* hpowheg2 = (TH1D*)retHist30to45->Clone();
  TH1D* hpowheg3 = (TH1D*)retHist45to60->Clone();
  TH1D* hpowheg4 = (TH1D*)retHist60to120->Clone();
  TH1D* hpowheg5 = (TH1D*)retHist120to200->Clone();
  TH1D* hpowheg6 = (TH1D*)retHist200->Clone();

  TH1D* rshape1 = get2DSlice(rshape_vector, "absrap2030",false,true);
  TH1D* rshape2 = get2DSlice(rshape_vector, "absrap3045",false,true);
  TH1D* rshape3 = get2DSlice(rshape_vector, "absrap4560",false,true);
  TH1D* rshape4 = get2DSlice(rshape_vector, "absrap60120",false,true);
  TH1D* rshape5 = get2DSlice(rshape_vector, "absrap120200",false,true);
  TH1D* rshape6 = get2DSlice(rshape_vector, "absrap200",false,true);

  //TH1D* rshape1_2 = get2DSlice(rshape_vector2, "absrap2030",false,true);
  //TH1D* rshape2_2 = get2DSlice(rshape_vector2, "absrap3045",false,true);
  //TH1D* rshape3_2 = get2DSlice(rshape_vector2, "absrap4560",false,true);
  //TH1D* rshape4_2 = get2DSlice(rshape_vector2, "absrap60120",false,true);
  //TH1D* rshape5_2 = get2DSlice(rshape_vector2, "absrap120200",false,true);
  //TH1D* rshape6_2 = get2DSlice(rshape_vector2, "absrap200",false,true);


  //cout << "result full " << rshape_vector->GetXaxis()->GetNbins() << " 1st " << rshape1->GetXaxis()->GetNbins() << " integral " << rshape_vector->Integral() << endl;
  //cout << "powheg full " << hpowheg_vector->GetXaxis()->GetNbins() << " 1st " << hpowheg1->GetXaxis()->GetNbins() << " integral " << hpowheg1->Integral() << endl;
  //cout << "fewz full " << hfewz_vector->GetXaxis()->GetNbins() <<" 1st " << hfewz1->GetXaxis()->GetNbins() << " integral " << hfewz1->Integral() << endl;
  //cout << "sys 1st " << hsys1->GetXaxis()->GetNbins() << " integral " << hsys1->Integral() << endl;

  //FIXME it was not vectorized 
  //TH1D* hsys1 = get2DSlice(hsys_vector, "20to30");
  //TH1D* hsys2 = get2DSlice(hsys_vector, "30to45");
  //TH1D* hsys3 = get2DSlice(hsys_vector, "45to60");
  //TH1D* hsys4 = get2DSlice(hsys_vector, "60to120");
  //TH1D* hsys5 = get2DSlice(hsys_vector, "120to200");
  //TH1D* hsys6 = get2DSlice(hsys_vector, "200to1500");

  //TFile* fres7tev = new TFile("result_combAB.root");
  //fres7tev->cd();
  //TH1D* hresult1 = (TH1D*)hresult1->Clone();
  //TH1D* hresult2 = (TH1D*)hresult2->Clone();
  //TH1D* hresult3 = (TH1D*)hresult3->Clone();
  //TH1D* hresult4 = (TH1D*)hresult4->Clone();
  //TH1D* hresult5 = (TH1D*)hresult5->Clone();
  //TH1D* hresult6 = (TH1D*)hresult6->Clone();

/*
  TFile* fout = new TFile("out_POWHEG.root","recreate");
  fout->cd();
  rshape1->Write();
  rshape2->Write();
  rshape3->Write();
  rshape4->Write();
  rshape5->Write();
  rshape6->Write();
  fout->Close(); 
*/

  if (!normalizeToZ && RR == "_") {
    TString filename = "DET2D_postFSR";
    if (shaperType == 2) filename = "DET2D"; 
    else if (shaperType == 1) filename = "full2D"; 
    else if (shaperType == 4) filename = "full2D_postFSR";

    //Apply PI background subtraction
    TFile* fpi = new TFile("../Inputs/PIbkg/pi_background2D_CMS_NNPDF23.root");
    fpi->cd();
    TH1D* hpi_vector = (TH1D*)gDirectory->Get("pi_back_2d"); 
    TH1D* hpi1 = get2DSlice(hpi_vector, "absrap2030",false,true);
    TH1D* hpi2 = get2DSlice(hpi_vector, "absrap3045",false,true);
    TH1D* hpi3 = get2DSlice(hpi_vector, "absrap4560",false,true);
    TH1D* hpi4 = get2DSlice(hpi_vector, "absrap60120",false,true);
    TH1D* hpi5 = get2DSlice(hpi_vector, "absrap120200",false,true);
    TH1D* hpi6 = get2DSlice(hpi_vector, "absrap200",false,true);
    rshape1->Add(hpi1,-1);
    rshape2->Add(hpi2,-1);
    rshape3->Add(hpi3,-1);
    rshape4->Add(hpi4,-1);
    rshape5->Add(hpi5,-1);
    rshape6->Add(hpi6,-1);


    //TH1D* gamma_contribution = (TH1D*)rshape_vector->Clone();
    //gamma_contribution->Divide(hpi_vector,rshape_vector);
    //TFile* fpiout = new TFile("../Inputs/PIbkg/pi_fraction.root","recreate");
    //fpiout->cd();
    //gamma_contribution->Write("pi_fraction");
    //fpiout->Close(); 

    TString FSRlabel = "FSRunfGood";
    if (useFSR_BBB) FSRlabel = "";

    TFile* fout = new TFile("../Outputs/absex_"+filename+FSRlabel+"_PI_Bayesian.root","recreate");
    fout->cd();
    rshape1->Write("20to30");
    rshape2->Write("30to45");
    rshape3->Write("45to60");
    rshape4->Write("60to120");
    rshape5->Write("120to200");
    rshape6->Write("200to1500");
    rshape_vector->Write("hxsec");
    fout->Close(); 
  }

    //TFile* fout2 = new TFile("../Outputs/rshape_full2D.root","recreate");
    //fout2->cd();
    //rshape1_2->Write("20to30");
    //rshape2_2->Write("30to45");
    //rshape3_2->Write("45to60");
    //rshape4_2->Write("60to120");
    //rshape5_2->Write("120to200");
    //rshape6_2->Write("200to1500");
    //fout2->Close();


  cout << "All done, drawing...." << endl;
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape1,hfewz1,hfewz1b,hsys1,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape2,hfewz2,hfewz2b,hsys2,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape3,hfewz3,hfewz3b,hsys3,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape4,hfewz4,hfewz4b,hsys4,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape5,hfewz5,hfewz5b,hsys5,RR);
  drawComparison(compareToPOWHEG,WHICHTYPE,shaperType,rshape6,hfewz6,hfewz6b,hsys6,RR);

/*
  for (int i = 0; i < 24; i++) {
      cout << i+1 << " " << rshape6->GetBinContent(i+1) << " " << rshape_vector->GetBinContent(i+1+120) << endl;
  //   cout << "Measurement " << i+1 << " " << rshape4->GetBinContent(i+1) << " FEWZ " << hfewz4->GetBinContent(i+1)<< " " << rshape_vector->GetBinContent(i+1+72) << endl;
  }
*/

}

void changeCentralValues(TH1D* hfewz, TH1D* hsys) {

   for (int i = 0; i < hfewz->GetNbinsX(); i++) {
        hsys->SetBinContent(i+1,hfewz->GetBinContent(i+1));
        double pdf_err = hfewz->GetBinContent(i+1)*hsys->GetBinError(i+1);
        hsys->SetBinError(i+1,hfewz->GetBinError(i+1)+pdf_err);
    }
}

TString getBinLabel(TString name) {
  if (name == "hslice1") return "20 < M(#mu#mu) < 30 GeV";
  if (name == "hslice2") return "30 < M(#mu#mu) < 45 GeV";
  if (name == "hslice3") return "45 < M(#mu#mu) < 60 GeV";
  if (name == "hslice4") return "60 < M(#mu#mu) < 120 GeV";
  if (name == "hslice5") return "120 < M(#mu#mu) < 200 GeV";
  if (name == "hslice6") return "200 < M(#mu#mu) < 1500 GeV";
} 

TLegend*  MakeLegend() {

   TLegend *_leg = new TLegend(0.55, 0.72, 0.85, 0.93);
   _leg->SetTextFont(72);
   _leg->SetTextSize(0.025);
   _leg->SetBorderSize(0);
   _leg->SetFillColor(0);
   return _leg;

}

void drawComparison(bool compareToPOWHEG, TString name, int shaperType, TH1D* hresult, TH1D* hfewz, TH1D* hfewzb, TH1D* hsys, TString RR) {

/*
  cout << "compareToPOWHEG " << compareToPOWHEG << endl;
  cout << " name " << name << endl;
  cout << " shaperType " << shaperType << endl;
  cout << " hresult " << hresult->Integral() << endl;
  cout << " hpowheg " << hpowheg->Integral() << endl;
  cout << " hfewz " << hfewz->Integral() << endl;
  cout << " hsys " << hsys->Integral() << endl;
*/

  TCanvas* c = new TCanvas("c"++TString(hsys->GetName()), "c"+TString(hsys->GetName()), 800, 800);
  c->cd();
  TPad *c_1 = new TPad("c_1"+TString(hsys->GetName()), "newpad"+TString(hsys->GetName()),0.01,0.05,0.99,0.99);
  c_1->Draw();
  c_1->cd();
  c_1->SetTopMargin(0.01);
  c_1->SetBottomMargin(0.3);
  c_1->SetRightMargin(0.1);
  c_1->SetFillStyle(0);

//  changeCentralValues(hresult,hsys);
  hfewz->GetXaxis()->SetTitle("");
  hfewz->GetYaxis()->SetTitleOffset(1.1);
  hfewz->GetXaxis()->SetTitleOffset(1.1);
  hfewz->SetTitle("CMS Preliminary, "+getBinLabel(hsys->GetName()));
  hfewz->GetYaxis()->SetTitleSize(0.05);
  hfewz->SetMaximum(hfewz->GetMaximum()*1.9);
  hfewz->SetMinimum(0.0);
  hfewz->GetYaxis()->SetTitleOffset(1.3);
  hfewz->GetYaxis()->SetTitle("d#sigma/d|y|");
  hfewz->SetLabelSize(0.0);
  hfewz->GetYaxis()->SetLabelSize(0.030);
  hfewz->SetLineColor(kRed);
  hfewz->SetFillColor(kRed);
  hfewz->SetLineWidth(1.2);
  hfewz->SetMarkerSize(0);
  hfewz->Draw("E2");

  hfewzb->SetLineColor(kBlue+1);
  hfewzb->SetFillColor(kBlue+1);
  hfewzb->SetLineWidth(1.2);
  hfewzb->SetMarkerSize(0);
  hfewzb->Draw("E2same");

  hresult->SetMarkerStyle(20);
  hresult->SetMarkerSize(1.0);
  hresult->Draw("epsame");
  //simplePrintOut(hresult);

  cout << hresult->GetName() << endl;
  for (int i = 0; i < hresult->GetNbinsX(); i++) {
     cout << hresult->GetBinContent(i+1) - hfewz->GetBinContent(i+1) << ", ";
  }

//  h2010comb->SetLineColor(kRed);
//  h2010comb->SetMarkerColor(kRed);
//  h2010comb->SetMarkerStyle(26);
//  if( shaperType == 0 ) h2010comb->Draw("epsame");

  ostringstream tmpstrm;
  TLegend *legend = MakeLegend();
  tmpstrm.str("");
  tmpstrm << "Data (#mu, 2012) " ;
  legend->AddEntry(hresult, tmpstrm.str().c_str(), "PL");

//  if( shaperType == 0 ) tmpstrm.str("");
//  if( shaperType == 0 ) tmpstrm << "Data (e+#mu, 36 pb^{-1} in 2010) " ;
//  if( shaperType == 0 )  legend->AddEntry(h2010comb, tmpstrm.str().c_str(), "PL");
  tmpstrm.str("");
  tmpstrm << "FEWZ+CT12 NNLO" ;
  legend->AddEntry(hfewz, tmpstrm.str().c_str(), "FL");
  tmpstrm.str("");
  tmpstrm << "FEWZ+NNPDF21 NNLO" ;
  legend->AddEntry(hfewzb, tmpstrm.str().c_str(), "FL");


  legend->SetFillColor(0);
  legend->Draw("0");

  c->cd();
  c_2 = new TPad("c_2"+TString(hsys->GetName()), "newpad"+TString(hsys->GetName()),0.01,0.05,0.99,0.32);
  c_2->Draw();
  c_2->cd();
  c_2->SetTopMargin(0.1);
  c_2->SetBottomMargin(0.30);
  c_2->SetRightMargin(0.1);
  c_2->SetFillStyle(0);
  c_2->SetGrid();

//points, no PDF
  TH1D* hdiff = hresult->Clone();
  hdiff->Divide(hresult, hfewz);
  TH1D* hdiffb = hresult->Clone();
  hdiffb->Divide(hresult, hfewzb);

//bands yes PDF
/*
  changeCentralValues(hresult,hsys);
  TH1D* hsys_tmp = (TH1D*)hsys->Clone();
  hsys_tmp->Divide(hsys,hfewz);

  TH1D* hsys_tmp2 = (TH1D*)hsys->Clone();
  hsys_tmp2->Divide(hsys,hfewzb);

  hsys_tmp2->SetTitle("");
  hsys_tmp2->GetXaxis()->SetTitle("Dimuon Rapidity, |y|");
  hsys_tmp2->GetYaxis()->SetTitle("data/theory");
  hsys_tmp2->GetXaxis()->SetTitleSize(0.15);
  hsys_tmp2->GetYaxis()->SetTitleSize(0.11);
  hsys_tmp2->GetYaxis()->SetTitleOffset(0.4);
  hsys_tmp2->GetYaxis()->SetLabelSize(0.07);
  hsys_tmp2->GetXaxis()->SetLabelSize(0.11);
  hsys_tmp2->SetMaximum(1.7);
  hsys_tmp2->SetMinimum(0.2);
  hsys_tmp2->SetLineColor(kBlue);
  hsys_tmp2->SetFillColor(kBlue);
  hsys_tmp2->SetFillStyle(3002);
  hsys_tmp2->SetMarkerSize(0);
  hsys_tmp2->SetLineWidth(1.2);
  hsys_tmp2->Draw("E2");

  hsys_tmp->SetLineColor(kRed);
  hsys_tmp->SetFillColor(kRed);
  hsys_tmp->SetFillStyle(3002);
  hsys_tmp->SetMarkerSize(0);
  hsys_tmp->SetLineWidth(1.2);
  hsys_tmp->Draw("E2same");
*/

  double max = hdiff->GetMaximum();
  double min = hdiff->GetMinimum();
  hdiff->SetMaximum(hdiff->GetMaximum()*1.3);
  hdiff->SetMinimum(hdiff->GetMinimum()*0.7);
  hdiff->SetLineColor(kRed);
  hdiff->SetMarkerColor(kRed);
  hdiff->SetTitle("");
  hdiff->GetXaxis()->SetTitle("|y|");
  hdiff->GetYaxis()->SetTitle("data/theory");
  hdiff->GetXaxis()->SetTitleSize(0.13);
  hdiff->GetYaxis()->SetTitleSize(0.09);
  hdiff->GetYaxis()->SetTitleOffset(0.4);
  hdiff->SetLineWidth(1.2);
  hdiff->GetXaxis()->SetLabelSize(0.11);
  hdiff->GetYaxis()->SetLabelSize(0.07);
  hdiff->Draw("PE");

  hdiffb->SetLineColor(kBlue+1);
  hdiffb->SetMarkerColor(kBlue+1);
  hdiffb->SetLineWidth(1.2);
  hdiffb->Draw("PEsame");

  c->Print("./shapeR_M"+TString(hsys->GetName())+name+".pdf");

}

/*
void simplePrintOut(TH1D* h) {

      cout << h->GetName() << endl;
 
  for (int i = 0; i < h->GetNbinsX(); i++) {
      cout << h->GetBinContent(i+1) << " " << h->GetBinError(i+1) << endl;
  }

  cout << endl;
}
*/


TH1D* getFSRSystsematics2D(TString onOff) {

  double fsr_sys1_m1[24] = {0.00647234, 0.00415486, 0.0108202, 0.00403136, 0.00574873, 0.00362989, 0.00505102, 0.00114965, 0.00389124, 0.00646128, 0.00447475, 0.00228061, 0.00227461, 0.00249416, 0.00829864, 0.0018035, 0.00283594, 0.00214582, 0.00574902, 0.00413263, 0.0118533, 0.0162197, 0.0334081, 0.0494639};
  double fsr_sys1_m2[24] = {0.00695026, 0.00616308, 0.00806352, 0.00985189, 0.00879987, 0.00502088, 0.00441509, 0.00713293, 0.00332536, 0.00510922, 0.00583788, 0.0020042, 0.00473196, 0.000193928, 0.00303252, 0.00372845, 0.00690953, 0.0059758, 0.00995221, 0.00939139, 0.0157836, 0.0245717, 0.0282447, 0.0627514};
  double fsr_sys1_m3[24] = {0.00678912, 0.0101303, 0.0162067, 0.0147403, 0.0305059, 0.00329938, 0.00901348, 0.0160075, 0.000727849, 0.00321051, 0.00881395, 0.018282, 0.0108828, 0.00375671, 0.00434735, 0.0220398, 0.0122086, 0.00991563, 0.0297635, 0.0411319, 0.0526711, 0.0436081, 0.099442, 0.150372};
  double fsr_sys1_m4[24] = {4.27893e-05, 0.000298569, 0.000471714, 0.000501392, 0.000142054, 5.03784e-05, 0.000369636, 7.02515e-05, 2.51258e-05, 0.00173303, 0.000761879, 4.7479e-05, 0.000873971, 0.00112472, 0.00114911, 0.00131473, 0.00301244, 0.00312481, 0.00343671, 0.00204168, 0.00540447, 0.00629831, 0.00848048, 0.0205518};
  double fsr_sys1_m5[24] = {0.00428379, 0.00621338, 0.0098965, 0.00695492, 0.00581896, 0.00176032, 0.00428015, 0.00163176, 0.00359524, 0.000123611, 0.00205836, 0.0026989, 0.00422844, 0.00111364, 0.00202451, 0.00187149, 0.000535225, 0.00944515, 0.0100203, 0.0112066, 0.00334101, 0.00116116, 0.0181624, 0.0111227};
  double fsr_sys1_m6[12] = {0.00295293, 0.00032152, 0.000433585, 0.00247265, 0.00420994, 0.00412127, 0.00287175, 0.00241768, 0.00799397, 0.00205394, 0.00574837, 0.0110666};

  TH1D* hfsr_sys1_m1 = new TH1D("hfsr_sys1_m1","hfsr_sys1_m1",24,0,2.4);
  TH1D* hfsr_sys1_m2 = new TH1D("hfsr_sys1_m2","hfsr_sys1_m2",24,0,2.4);
  TH1D* hfsr_sys1_m3 = new TH1D("hfsr_sys1_m3","hfsr_sys1_m3",24,0,2.4);
  TH1D* hfsr_sys1_m4 = new TH1D("hfsr_sys1_m4","hfsr_sys1_m4",24,0,2.4);
  TH1D* hfsr_sys1_m5 = new TH1D("hfsr_sys1_m5","hfsr_sys1_m5",24,0,2.4);
  TH1D* hfsr_sys1_m6 = new TH1D("hfsr_sys1_m6","hfsr_sys1_m6",12,0,2.4);
  for (int i = 0; i < 24; i++) {    
    if (onOff == "on") {
      hfsr_sys1_m1->SetBinContent(i+1,fsr_sys1_m1[i]);    hfsr_sys1_m1->SetBinError(i+1,0.00001);
      hfsr_sys1_m2->SetBinContent(i+1,fsr_sys1_m2[i]);    hfsr_sys1_m2->SetBinError(i+1,0.00001);
      hfsr_sys1_m3->SetBinContent(i+1,fsr_sys1_m3[i]);    hfsr_sys1_m3->SetBinError(i+1,0.00001);
      hfsr_sys1_m4->SetBinContent(i+1,fsr_sys1_m4[i]);    hfsr_sys1_m4->SetBinError(i+1,0.00001);
      hfsr_sys1_m5->SetBinContent(i+1,fsr_sys1_m5[i]);   hfsr_sys1_m5->SetBinError(i+1,0.00001);
    } else {
      hfsr_sys1_m1->SetBinContent(i+1,0.00001);    hfsr_sys1_m1->SetBinError(i+1,0.00001);
      hfsr_sys1_m2->SetBinContent(i+1,0.00001);    hfsr_sys1_m2->SetBinError(i+1,0.00001);
      hfsr_sys1_m3->SetBinContent(i+1,0.00001);    hfsr_sys1_m3->SetBinError(i+1,0.00001);
      hfsr_sys1_m4->SetBinContent(i+1,0.00001);    hfsr_sys1_m4->SetBinError(i+1,0.00001);
      hfsr_sys1_m5->SetBinContent(i+1,0.00001);   hfsr_sys1_m5->SetBinError(i+1,0.00001);
    }
  }
  for (int i = 0; i < 12; i++) {
    hfsr_sys1_m6->SetBinContent(i+1,fsr_sys1_m6[i]);
    hfsr_sys1_m6->SetBinError(i+1,0.00001); 
  }

  return sliceCombiner(hfsr_sys1_m1,hfsr_sys1_m2,hfsr_sys1_m3,hfsr_sys1_m4,hfsr_sys1_m5,hfsr_sys1_m6,true);

}

TH1D* getBkgSystematics(TString onOff) {

  // QCD sys
  double qcd_sys_m1[24] = {0.0056532, 0.00550908, 0.00540882, 0.00474059, 0.00548575, 0.00560484, 0.00511055, 0.00545808, 0.00538611, 0.00525683, 0.00526199, 0.00570695, 0.00502583, 0.00617854, 0.00596419, 0.00584117, 0.00656915, 0.00650653, 0.0070545, 0.00692165, 0.00713966, 0.00888529, 0.0101327, 0.0155619};
  double qcd_sys_m2[24] = {0.00351196, 0.00367751, 0.00408944, 0.00370759, 0.0036681, 0.00329544, 0.0035952, 0.003873, 0.00361388, 0.0037399, 0.00379453, 0.00390213, 0.00434069, 0.00337894, 0.00366135, 0.00382634, 0.00352825, 0.00326635, 0.00446296, 0.00448979, 0.00526434, 0.00708578, 0.0230291, 0.0237456};
  double qcd_sys_m3[24] = {0.00440309, 0.00445827, 0.00506042, 0.00467334, 0.00498347, 0.00437084, 0.00459451, 0.00526771, 0.00456958, 0.00513797, 0.00448449, 0.0038474, 0.00442588, 0.00349733, 0.00345998, 0.00297294, 0.00358496, 0.00686592, 0.00272317, 0.00275562, 0.00531983, 0.0125791, 0.00597966, 7.35413e-07}; 
  double qcd_sys_m4[24] = {0.000241935, 0.000219588, 0.000209963, 0.000202262, 0.000219526, 0.0001979, 0.000183811, 0.000237073, 0.000196266, 0.000196642, 0.000322948, 0.000210226, 0.000180688, 0.000270308, 0.000443863, 0.000490265, 0.000799433, 0.00075193, 0.000423587, 0.000454509, 0.00134244, 0.000674608, 2.61718e-08, 7.0583e-08};
  double qcd_sys_m5[24] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  double qcd_sys_m6[24] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  
  TH1D* hqcd_sys_m1 = new TH1D("hqcd_sys_m1","hqcd_sys_m1",24,0,2.4);
  TH1D* hqcd_sys_m2 = new TH1D("hqcd_sys_m2","hqcd_sys_m2",24,0,2.4);
  TH1D* hqcd_sys_m3 = new TH1D("hqcd_sys_m3","hqcd_sys_m3",24,0,2.4);
  TH1D* hqcd_sys_m4 = new TH1D("hqcd_sys_m4","hqcd_sys_m4",24,0,2.4);
  TH1D* hqcd_sys_m5 = new TH1D("hqcd_sys_m5","hqcd_sys_m5",24,0,2.4);
  TH1D* hqcd_sys_m6 = new TH1D("hqcd_sys_m6","hqcd_sys_m6",24,0,2.4);
  
  for (int i = 0; i < 24; i++) {
    hqcd_sys_m1->SetBinContent(i+1,qcd_sys_m1[i]);
    hqcd_sys_m1->SetBinError(i+1,0.00001);
    hqcd_sys_m2->SetBinContent(i+1,qcd_sys_m2[i]);
    hqcd_sys_m2->SetBinError(i+1,0.00001);
    hqcd_sys_m3->SetBinContent(i+1,qcd_sys_m3[i]);
    hqcd_sys_m3->SetBinError(i+1,0.00001);
    hqcd_sys_m4->SetBinContent(i+1,qcd_sys_m4[i]);
    hqcd_sys_m4->SetBinError(i+1,0.00001);
    hqcd_sys_m5->SetBinContent(i+1,qcd_sys_m5[i]);
    hqcd_sys_m5->SetBinError(i+1,0.00001);
    hqcd_sys_m6->SetBinContent(i+1,qcd_sys_m6[i]);
    hqcd_sys_m6->SetBinError(i+1,0.00001);
  }

  TH1D* qcd_sys = sliceCombiner(hqcd_sys_m1,hqcd_sys_m2,hqcd_sys_m3,hqcd_sys_m4,hqcd_sys_m5,hqcd_sys_m6,true);


  // ttbar sys
  TFile* f = new TFile("~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/Inputs/sys/bkg_syst2013_2D.root");
  f->cd();
  TH1D* ttbar_sys = (TH1D*)gDirectory->Get("hsyst2D");

  TH1D* bkg_sys = (TH1D*)ttbar_sys->Clone();
  for (int i = 0; i < bkg_sys->GetNbinsX(); i++) {
     if (onOff == "on") {
        bkg_sys->SetBinContent(i+1,sqrt(pow(ttbar_sys->GetBinContent(i+1),2)+pow(qcd_sys->GetBinContent(i+1),2)));
     } else {
        bkg_sys->SetBinContent(i+1,0.00001);
     }
  }

  return bkg_sys;

}

TH1D* acceptFEWZInputs(TString beauty_label, TString subfolder, TString thisPDF, bool normalizeToZ) {

  //TString beauty_slices[nbinsM] = {"20to30","30to45","45to60","60to120","120to200","200to1500"};


  TFile *f_fewzio  = new TFile("../Inputs/Theory/2Dabsxsec"+beauty_label+"_NNLO"+subfolder+thisPDF+".root","open");
  f_fewzio->cd();
  TH1D* hfewz = (TH1D*)xsec->Clone();
  hfewz->SetName(beauty_label);
  if (beauty_label == "200to1500") hfewz->Rebin(2);

  //Always access 4th slice for normalization
  TFile *f_norm  = new TFile("../Inputs/Theory/2Dabsxsec60to120_NNLO"+subfolder+thisPDF+".root","open");
  f_norm->cd();
  TH1D* hfewz4 = (TH1D*)xsec->Clone();
  hfewz4->SetName("60to120");

  double totFEWZXsec = 0.;//970;
  double binSizeF = 0.1;
  if (normalizeToZ) {
     totFEWZXsec = 2.*hfewz4->Integral();
  } else {
     totFEWZXsec = 1.;
  }
  if (beauty_label == "200to1500") binSizeF = 0.2;

  for (int i = 0; i < hfewz->GetXaxis()->GetNbins(); i++) {
    hfewz->SetBinContent(i+1,2.*hfewz->GetBinContent(i+1)/totFEWZXsec/binSizeF);
    hfewz->SetBinError(i+1,hfewz->GetBinError(i+1)/totFEWZXsec/binSizeF);
  }

  //FIXME temporary
  if (thisPDF == "NNPDF21") {
     if (beauty_label == "20to30") { 
        hfewz->SetBinError(23,4.042e-03);
        hfewz->SetBinError(24,3.572e-03);
     }
     if (beauty_label == "30to45") { 
        hfewz->SetBinError(23,3.584e-03);
        hfewz->SetBinError(24,5.100e-03);
     }
     if (beauty_label == "120to200") { 
        hfewz->SetBinError(23,4.743e-04);
        hfewz->SetBinError(24,6.859e-04);
     }
  } 

  return hfewz;

}
