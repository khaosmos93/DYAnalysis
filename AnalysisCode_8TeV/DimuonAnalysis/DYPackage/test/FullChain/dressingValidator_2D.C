void dressingValidator_2D() {

  TString channel = "";//"_MuMu";

  TFile* f = new TFile("../root_files_preFSR_emothers_2D"+channel+"/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants2D_PU.root");
  //TFile* f = new TFile("/group/cms/users/asvyatko/BACKUPCOPY_LATEST2/root_files_NOFEWZ_NEWILYA_2D/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants2D_PU.root");
  f->cd();
  TH1D* hpreFSR_pre = new TH1D(*yieldsMcPreFsrGenDETFIArray);
  TH1D* hpostFSR_pre = new TH1D(*yieldsMcPostFsrGenDETFIArray);
  TH1D* hpreFSR = new TH1D("hpreFSR","hpreFSR",132,0,132);
  TH1D* hpostFSR = new TH1D("hpostFSR","hpostFSR",132,0,132);

  TFile* ff = new TFile("../root_files_R01_emothers_2D"+channel+"/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants2D_PU.root");
  ff->cd();
  TH1D* hdressedR01_pre = new TH1D(*yieldsMcPreFsrGenDETFIArray);
  TH1D* hdressedR01 = new TH1D("hdressedR01","hdressedR01",132,0,132);

  //extrapolate in the first bin
/*
  TFile* f = new TFile("/group/cms/users/asvyatko/BACKUPCOPY_LATEST2/root_files_NOFEWZ_NEWILYA_2D/constants/DY_j22_19712pb/fsrDET_good_unfolding_constants2D_PU.root");
  f->cd();
  TH1D* hpreFSR_main_pre = new TH1D(*yieldsMcPreFsrGenDETFIArray);
  TH1D* hpostFSR_main_pre = new TH1D(*yieldsMcPostFsrGenDETFIArray);
  TH1D* hpreFSR_main = new TH1D("hpreFSR_main","hpreFSR_main",132,0,132);
  TH1D* hpostFSR_main = new TH1D("hpostFSR_main","hpostFSR_main",132,0,132);
*/

  for (int i = 0; i < 132; i++) {
    hpreFSR->SetBinContent(i+1,hpreFSR_pre->GetBinContent(i+1+24));
    hpreFSR->SetBinError(i+1,hpreFSR_pre->GetBinError(i+1+24));
    hpostFSR->SetBinContent(i+1,hpostFSR_pre->GetBinContent(i+1+24));
    hpostFSR->SetBinError(i+1,hpostFSR_pre->GetBinError(i+1+24));
    hdressedR01->SetBinContent(i+1,hdressedR01_pre->GetBinContent(i+1+24));
    hdressedR01->SetBinError(i+1,hdressedR01_pre->GetBinError(i+1+24));
  }


  //introduce smoothing
 gROOT->ProcessLine(".L ~/DYStudy/CMSSW_5_3_3_patch2/src/DimuonAnalysis/DYPackage/test/tools/histotools.C");
 TH1D* hpreFSR_slice1 = get2DSlice(hpreFSR, "absrap2030",false,true);
 TH1D* hpreFSR_slice2 = get2DSlice(hpreFSR, "absrap3045",false,true);
 TH1D* hpreFSR_slice3 = get2DSlice(hpreFSR, "absrap4560",false,true);
 TH1D* hpreFSR_slice4 = get2DSlice(hpreFSR, "absrap60120",false,true);
 TH1D* hpreFSR_slice5 = get2DSlice(hpreFSR, "absrap120200",false,true);
 TH1D* hpreFSR_slice6 = get2DSlice(hpreFSR, "absrap200",false,true);
 hpreFSR_slice1->Smooth(10,"R");
 hpreFSR_slice2->Smooth(10,"R");
 hpreFSR_slice3->Smooth(10,"R");
 hpreFSR_slice4->Smooth(10,"R");
 hpreFSR_slice5->Smooth(10,"R");
 hpreFSR_slice6->Smooth(10,"R");
 hpreFSR = sliceCombiner(hpreFSR_slice1,hpreFSR_slice2,hpreFSR_slice3,hpreFSR_slice4,hpreFSR_slice5,hpreFSR_slice6,true); 

 TH1D* hpostFSR_slice1 = get2DSlice(hpostFSR, "absrap2030",false,true);
 TH1D* hpostFSR_slice2 = get2DSlice(hpostFSR, "absrap3045",false,true);
 TH1D* hpostFSR_slice3 = get2DSlice(hpostFSR, "absrap4560",false,true);
 TH1D* hpostFSR_slice4 = get2DSlice(hpostFSR, "absrap60120",false,true);
 TH1D* hpostFSR_slice5 = get2DSlice(hpostFSR, "absrap120200",false,true);
 TH1D* hpostFSR_slice6 = get2DSlice(hpostFSR, "absrap200",false,true);
 hpostFSR_slice1->Smooth(10,"R");
 hpostFSR_slice2->Smooth(10,"R");
 hpostFSR_slice3->Smooth(10,"R");
 hpostFSR_slice4->Smooth(10,"R");
 hpostFSR_slice5->Smooth(10,"R");
 hpostFSR_slice6->Smooth(10,"R");
 hpostFSR = sliceCombiner(hpostFSR_slice1,hpostFSR_slice2,hpostFSR_slice3,hpostFSR_slice4,hpostFSR_slice5,hpostFSR_slice6,true);       

 TH1D* hdressedR01_slice1 = get2DSlice(hdressedR01, "absrap2030",false,true);
 TH1D* hdressedR01_slice2 = get2DSlice(hdressedR01, "absrap3045",false,true);
 TH1D* hdressedR01_slice3 = get2DSlice(hdressedR01, "absrap4560",false,true);
 TH1D* hdressedR01_slice4 = get2DSlice(hdressedR01, "absrap60120",false,true);
 TH1D* hdressedR01_slice5 = get2DSlice(hdressedR01, "absrap120200",false,true);
 TH1D* hdressedR01_slice6 = get2DSlice(hdressedR01, "absrap200",false,true);
 hdressedR01_slice1->Smooth(10,"R");
 hdressedR01_slice2->Smooth(10,"R");
 hdressedR01_slice3->Smooth(10,"R");
 hdressedR01_slice4->Smooth(10,"R");
 hdressedR01_slice5->Smooth(10,"R");
 hdressedR01_slice6->Smooth(10,"R");
 hdressedR01 = sliceCombiner(hdressedR01_slice1,hdressedR01_slice2,hdressedR01_slice3,hdressedR01_slice4,hdressedR01_slice5,hdressedR01_slice6,true);       
 //smoothing end

  TH1D* hpostpreFSR = (TH1D*)hpostFSR->Clone();
  hpostpreFSR->Divide(hpostFSR,hpreFSR);

  TH1D* hpostFSRdressed = (TH1D*)hpostFSR->Clone();
  hpostFSRdressed->Divide(hpostFSR,hdressedR01);

 TH1D* hpostFSRdressed_slice1 = get2DSlice(hpostFSRdressed, "absrap2030",false,true);
 TH1D* hpostFSRdressed_slice2 = get2DSlice(hpostFSRdressed, "absrap3045",false,true);
 TH1D* hpostFSRdressed_slice3 = get2DSlice(hpostFSRdressed, "absrap4560",false,true);
 TH1D* hpostFSRdressed_slice4 = get2DSlice(hpostFSRdressed, "absrap60120",false,true);
 TH1D* hpostFSRdressed_slice5 = get2DSlice(hpostFSRdressed, "absrap120200",false,true);
 TH1D* hpostFSRdressed_slice6 = get2DSlice(hpostFSRdressed, "absrap200",false,true);
 hpostFSRdressed_slice1->Smooth(5,"R");
 hpostFSRdressed_slice2->Smooth(100,"R");
 hpostFSRdressed_slice3->Smooth(100,"R");
 hpostFSRdressed_slice4->Smooth(100,"R");
 hpostFSRdressed_slice5->Smooth(100,"R");
 //hpostFSRdressed_slice6->Smooth(10,"R");
 hpostFSRdressed = sliceCombiner(hpostFSRdressed_slice1,hpostFSRdressed_slice2,hpostFSRdressed_slice3,hpostFSRdressed_slice4,hpostFSRdressed_slice5,hpostFSRdressed_slice6,true);

 TH1D* hpostpreFSR_slice1 = get2DSlice(hpostpreFSR, "absrap2030",false,true);
 TH1D* hpostpreFSR_slice2 = get2DSlice(hpostpreFSR, "absrap3045",false,true);
 TH1D* hpostpreFSR_slice3 = get2DSlice(hpostpreFSR, "absrap4560",false,true);
 TH1D* hpostpreFSR_slice4 = get2DSlice(hpostpreFSR, "absrap60120",false,true);
 TH1D* hpostpreFSR_slice5 = get2DSlice(hpostpreFSR, "absrap120200",false,true);
 TH1D* hpostpreFSR_slice6 = get2DSlice(hpostpreFSR, "absrap200",false,true);
 //hpostpreFSR_slice1->Smooth(5,"R");
 //hpostpreFSR_slice2->Smooth(100,"R");
 hpostpreFSR_slice3->Smooth(100,"R");
 //hpostpreFSR_slice4->Smooth(100,"R");
 //hpostpreFSR_slice5->Smooth(100,"R");
 hpostpreFSR_slice6->Smooth(10,"R");
 hpostpreFSR = sliceCombiner(hpostpreFSR_slice1,hpostpreFSR_slice2,hpostpreFSR_slice3,hpostpreFSR_slice4,hpostpreFSR_slice5,hpostpreFSR_slice6,true);

  //pre-FSR, post-FSR and dressed R01
  TCanvas* c = new TCanvas("val2");
  c->cd();
  hpostFSR->GetXaxis()->SetTitle("Bin number");
  if (channel == "") {
    hpostFSR->GetYaxis()->SetTitle("Dielectron events");
  } else {
    hpostFSR->GetYaxis()->SetTitle("Dimuon events");
  }
  hpostFSR->Draw("hist");
  hdressedR01->SetLineColor(kRed);
  hdressedR01->Draw("histsame");
  hpreFSR->SetLineColor(kBlue);
  hpreFSR->Draw("histsame");

  //add legend
  TLegend *leg2= new TLegend(0.20,0.15,0.35,0.30);
  leg2->SetTextFont(72);
  leg2->SetTextSize(0.060);
  leg2->SetBorderSize(0);
  leg2->SetFillColor(0);
  leg2->AddEntry(hpostFSR,"Standard post-FSR","l");
  leg2->AddEntry(hpreFSR,"Standard pre-FSR","l");
  leg2->AddEntry(hdressedR01,"Dressed in #DeltaR < 0.1 cone","l");
  leg2->Draw("same");

  //add the ratios
  TCanvas* c3 = new TCanvas("val3");
  c3->cd();
  hpostFSRdressed->GetXaxis()->SetTitle("Bin number");
  hpostFSRdressed->GetYaxis()->SetRangeUser(0.7,2.0);
  if (channel == "") {
   hpostFSRdressed->GetYaxis()->SetTitle("Dielectron event ratios");
  } else {
   hpostFSRdressed->GetYaxis()->SetTitle("Dimuon event ratios");
  }
  hpostFSRdressed->SetMarkerStyle(22);
  hpostFSRdressed->SetMarkerColor(kRed);
  hpostFSRdressed->SetLineColor(kRed);
  hpostFSRdressed->Draw("PE");
  hpostpreFSR->SetMarkerStyle(22);
  hpostpreFSR->Draw("PEsame");
  
  //add legend
  TLegend *leg3= new TLegend(0.20,0.15,0.35,0.30);
  leg3->SetTextFont(72);
  leg3->SetTextSize(0.060);
  leg3->SetBorderSize(0);
  leg3->SetFillColor(0);
  leg3->AddEntry(hpostFSRdressed,"N_{post-FSR}/N_{dressed, #DeltaR<0.1}","lp");
  leg3->AddEntry(hpostpreFSR,"N_{post-FSR}/N_{pre-FSR}","lp");
  leg3->Draw("same");

}
