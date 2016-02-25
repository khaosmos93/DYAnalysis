TString getFolder(TString, TString);
TString getCanv(TString);

RooCurve* getFitCurve(TString INPUTDIR_PREFIX, TString SCENARIO, TString dataMC, TString vartype, TString SCEN_TRIG,TString thisShape,TString whichCan,TString thisBin, TString dataSigFitBkgFit) { 

    cout << "Parameters inside the getFitCurve "<<  INPUTDIR_PREFIX << " " << SCENARIO << " " <<  dataMC << " " <<  vartype << " " <<  SCEN_TRIG << " " <<  thisShape << " " <<  whichCan << " " <<  thisBin << " " << dataSigFitBkgFit << endl; 

    //debug flag
    bool debugVerify = true;

    TString DENOM = "_&_Glb_pass_&_VBTF_pass";
    TString DENOM2 = "__Glb_pass__VBTF_pass";
    if (SCENARIO == "Glb_VBTF") {
       DENOM = "";
       DENOM2 = "";
   } 
   else if (SCENARIO == "Glb_VBTF_IsolPFRelCombNoEGammaR03_"+SCEN_TRIG) { 
       DENOM = "_&_Glb_pass_&_IsolPFRelCombNoEGammaR03_pass_&_VBTF_pass";
       DENOM2 = "__Glb_pass__IsolPFRelCombNoEGammaR03_pass__VBTF_pass";
   } 

   TString POSTFIX = "";
   if (vartype == "vtx") POSTFIX = "_vtx";
   else if (vartype == "run") POSTFIX = ""; //"_rrr";
   else if (vartype == "rrr2") POSTFIX = "_rrr2";

   TFile *thisf = new TFile(INPUTDIR_PREFIX+"TnP_2011_MuonID_item_"+dataMC+"_"+SCENARIO+POSTFIX+".root");
  //cout << INPUTDIR_PREFIX+"TnP_2011_MuonID_item_"+dataMC+"_"+SCENARIO+POSTFIX+".root" << endl;
   thisf->cd();
   gDirectory->cd("tpTree/"+SCENARIO+"_"+vartype+"/"+getFolder(vartype,thisBin)+DENOM2+"__"+thisShape); //split based on varType, shape, for each bin num, SCENARIO 
  //cout << "tpTree/"+SCENARIO+"_"+vartype+"/fit_eff_plots" << endl;

   TCanvas* c = (TCanvas*)gDirectory->Get("fit_canvas"); //always
  //cout << getFolder(vartype)+DENOM << endl;
   c->GetListOfPrimitives();

//
//DEBUG
//
   TIter next(c->GetListOfPrimitives());
   obj=next();
   if (debugVerify) {
     while ((obj=next())) {
       cout << "Reading object2: " << obj->GetName() << endl;
         if (obj->InheritsFrom("TH1")) {
             cout << "Histo2: " << obj->GetName() << endl;
         } else if (obj->InheritsFrom("TCanvas") || obj->InheritsFrom("TPad")) {
            cout << "Canvas2: " << obj->GetName() << endl;
         } else if (obj->InheritsFrom("TF1")) {
            cout << "Function2: " << obj->GetName() << endl;
         } else {
            cout << "Something else idk2: " << obj->GetName() << endl;
         }
      }
   }
//
//
//

   TPad* pad = (TPad*)c->GetPrimitive("fit_canvas_"+whichCan);  // "fit_canvas_"+getCanv(whichCan)); //which pad - can be 1, 2, 3
   pad->GetListOfPrimitives();

   //cout << "FOR " << "fit_canvas_"+whichCan << endl;
//
//DEBUG
//
   TIter next(pad->GetListOfPrimitives());
   obj=next();
   if (debugVerify) { 
     while ((obj=next())) {
       cout << "Reading object: " << obj->GetName() << endl;
         if (obj->InheritsFrom("TH1")) {
             cout << "Histo: " << obj->GetName() << endl;
         } else if (obj->InheritsFrom("TCanvas") || obj->InheritsFrom("TPad")) {
            cout << "Canvas: " << obj->GetName() << endl;
         } else if (obj->InheritsFrom("TF1")) {
            cout << "Function: " << obj->GetName() << endl;
         } else {
            cout << "Something else idk: " << obj->GetName() << endl;
         }
      }
   }
//
//
//

   //get signal func
   cout << "get signal func" << endl;
   RooCurve* hsigfit = (RooCurve*)pad->GetPrimitive(getCanv(whichCan, dataSigFitBkgFit));
   //get background func
   cout << "get background func" << endl;
   RooCurve* hbkgfit = (RooCurve*)pad->GetPrimitive(getCanv(whichCan, dataSigFitBkgFit));

   if (dataSigFitBkgFit == "sig") return hsigfit;
   else if (dataSigFitBkgFit == "bkg") return hbkgfit;

}

TString getFolder(TString what, TString whichBin) {

   TString return_string = "";
   if (what == "pt") return_string = "eta_bin0__pt_bin"+whichBin;
   if (what == "eta") return_string = "eta_bin"+whichBin+"__pt_bin0";
   if (what == "vtx") return_string = "eta_bin0__pt_bin0__tag_nVertices_bin"+whichBin; 
   if (what == "run") return_string = "eta_bin0__pt_bin0__run_bin"+whichBin; //var integer __ + shape 
   if (what == "rrr2") return_string = "eta_bin0__pt_bin0__run2_bin"+whichBin; //var integer __ + shape 

   return return_string;
}

TString getCanv(TString which, TString what) {
   TString return_string = "";
   if (which == "1") {
      if (what == "sig") return_string = "pdfPass_Norm\[mass\]"; 
      else if (what == "bkg") return_string = "pdfPass_Norm\[mass\]_Comp\[backgroundPass\]"; 
   }
   else if (which == "2") {
      if (what == "sig") return_string = "pdfFail_Norm\[mass\]";
      else if (what == "bkg") return_string = "pdfFail_Norm\[mass\]_Comp\[backgroundFail\]";
   }
   else if (which == "3") {
      if (what == "sig") return_string = "simPdf_Norm\[mass\]";
      else if (what == "bkg") return_string = "simPdf_Norm\[mass\]_Comp\[backgroundPass\,backgroundFail\]";
   }

   return return_string;
}
