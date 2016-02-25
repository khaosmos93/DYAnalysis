TString getFolder(TString, TString);

RooHist* getFitHist(TString INPUTDIR_PREFIX, TString SCENARIO, TString dataMC, TString vartype, TString SCEN_TRIG,TString thisShape,TString whichCan,TString thisBin, TString dataSigBkg) { 

    cout << "Parameters inside getFitHist " << INPUTDIR_PREFIX << " " <<  SCENARIO << " " <<  dataMC << " " <<  vartype << " " <<  SCEN_TRIG << " " <<  thisShape << " " <<  whichCan << " " << thisBin << " " <<  dataSigBkg << endl;

    //debug flag
    bool debugVerify = false;

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
   thisf->cd();
   gDirectory->cd("tpTree/"+SCENARIO+"_"+vartype+"/"+getFolder(vartype,thisBin)+DENOM2+"__"+thisShape); //split based on varType, shape, for each bin num, SCENARIO 

   TCanvas* c = (TCanvas*)gDirectory->Get("fit_canvas"); //always
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

   //get data
   cout << "get data " << vartype  << SCENARIO  << endl;
   RooHist* hdata = (RooHist*)pad->GetPrimitive("h_data_binned");

   return hdata;

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
