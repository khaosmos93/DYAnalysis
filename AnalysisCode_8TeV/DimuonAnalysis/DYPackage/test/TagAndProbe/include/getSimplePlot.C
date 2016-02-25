#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <TH1D.h>
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TROOT.h>

TString getFolder(TString);
TH1D* rooHist_to_TH1D(RooHist*, TString, TString);

TH1D* getSimplePlot(TString INPUTDIR_PREFIX, TString SCENARIO, TString dataMC, TString vartype, TString SCEN_TRIG, TString RR) {

   TString DENOM = "_Glb_pass_&_Tight2012_pass"; //"_&_Glb_pass_&_Tight2012_pass";
   if (SCENARIO == "Glb_Tight2012") DENOM = "";
   else if (SCENARIO == "Glb_Tight2012_IsolPFRelCombNoEGammaR03PU_"+SCEN_TRIG) DENOM = "_Glb_pass_&_IsolPFRelCombNoEGammaR03PU_pass_&_Tight2012_pass";
   else if (SCENARIO == "Glb_Tight2012_IsolPFRelCombNoEGammaR03_"+SCEN_TRIG) DENOM = "_Glb_pass_&_IsolPFRelCombNoEGammaR03_pass_&_Tight2012_pass";

   TString POSTFIX = "";
   if (vartype == "vtx") POSTFIX = ""; //"_vtx";
   else if (vartype == "run") POSTFIX = ""; //"_rrr";
   else if (vartype == "rrr2") POSTFIX = "_rrr2";
   else if (vartype == "rrr3") POSTFIX = "_rrr3";

   if (dataMC == "datalike_mc") {
     RR = "";
   } else {
     RR = "_"+RR;
   }
   TFile *thisf = new TFile(INPUTDIR_PREFIX+"TnP_2011_MuonID_item_"+dataMC+"_"+SCENARIO+POSTFIX+"_"+vartype+RR+".root");
   cout << "HERE " << INPUTDIR_PREFIX+"TnP_2011_MuonID_item_"+dataMC+"_"+SCENARIO+POSTFIX+"_"+vartype+RR+".root" << endl;
   thisf->cd();
   gDirectory->cd("tpTree/"+SCENARIO+"_"+vartype+"/fit_eff_plots");
   cout << "tpTree/"+SCENARIO+"_"+vartype+"/fit_eff_plots" << endl;

   TCanvas* c = (TCanvas*)gDirectory->Get(getFolder(vartype)+DENOM);
   cout << getFolder(vartype)+DENOM << endl;
   c->GetListOfPrimitives();
   RooHist* hpt = (RooHist*)c->GetPrimitive("hxy_fit_eff");

   TH1D* test = rooHist_to_TH1D(hpt,vartype,SCENARIO);
   for (int ibin = 0; ibin < test->GetXaxis()->GetNbins();ibin++) {
       cout << "AFTER " << test->GetBinContent(ibin+1) << endl;
    }
   return rooHist_to_TH1D(hpt,vartype,SCENARIO);
}

TString getFolder(TString what) {

   TString return_string = "";
   if (what == "pt" || what == "pt2" || what == "pt3") return_string = "pt_PLOT"; //_eta_bin0";
   if (what == "pt_eta") return_string = "eta_PLOT_pt_bin2"; //FIXME SAV
   if (what == "eta" || what == "eta2" || what == "eta3" || what == "eta4") return_string = "eta_PLOT";//_pt_bin0";  
   if (what == "vtx") return_string = "tag_nVertices_PLOT"; //_eta_bin0_&_pt_bin0";
   if (what == "run" || what == "rrr2" || what == "rrr3") return_string = "run_PLOT_eta_bin0_&_pt_bin0";
   if (what == "rrr2") return_string = "run2_PLOT_eta_bin0_&_pt_bin0";
   if (what == "rrr3") return_string = "run3_PLOT_eta_bin0_&_pt_bin0";

   return return_string;
}

TH1D* rooHist_to_TH1D(RooHist* h, TString type, TString scenario) { 

   Double_t bincenter,bincontent;
   Double_t Xerr, Yerr;
   Double_t content;

   //book return histogram
   int nbins;
   const int nvtxbins = 24; 
   Double_t xbins_vtx[nvtxbins] = {0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,13.5,14.5,15.5,16.5,17.5,20.5,22.5,25.0,27.5,30.0,35.0};
   const int nptbins = 19;
   const int nptbins2 = 10;
   const int nptbins3 = 3;
   const int netabins = 16;
   const int netabins2 = 22;
   const int netabins3 = 5; 
   const int netabins4 = 51;
   Double_t xbins_eta[netabins] = {-2.4,-1.9,-1.5,-1.1,-0.9,-0.7,-0.5,-0.1,0.1,0.5,0.7,0.9,1.1,1.5,1.9,2.4};
   Double_t xbins_eta2[netabins2] = {-2.4,-2.1,-1.9,-1.7,-1.5,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.5,1.7,1.9,2.1,2.4};
   Double_t xbins_eta3[netabins3] = {-2.4,-0.9,0.,0.9,2.4};
   Double_t xbins_eta4[netabins4] = {-2.4,-2.1,-1.9,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.85,-0.8,-0.75,-0.7,-0.65,-0.6,-0.5,-0.4,-0.3,-0.25,-0.2,-0.15,-0.1,0.0,0.1,0.15,0.2,0.25,0.3,0.4,0.5,0.6,0.65,0.7,0.75,0.8,0.85,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.9,2.1,2.4};

   //Double_t xbins_pt[nptbins]   = {7,10,13,16,20,24,26,28,30,32,36,40,44,48,55,70,100,220,400};
   Double_t xbins_pt[nptbins]   = {8,10,13,17,20,24,26,28,30,32,36,40,44,48,55,70,100,220,400};
   Double_t xbins_pt2[nptbins2]   = {8,17,26,32,44,55,70,100,220,400};
   Double_t xbins_pt3[nptbins3] = {8,20,400000};

   const int nrunbins = 154;
   Double_t xbins_run[nrunbins] = {
160431,  160871,  160874,  160942,  160956,  161008,  161106,  161116,  161156,  161222,  161310,  162762,  162808,  162825,  162909,  163071,  163233,  163237,  163255,  163286,  163297,  163302,  163334,  163339,  163369,  163372,  163376,  163387,  163476,  163480,  163483,  163584,  163587,  163596,  163657,  163660,  163663,  163738,  163759,  163763,  163869,  165088,  165102,  165121,  165364,  165467,  165487,  165548,  165570,  165633,  166011,  166049,  166161,  166346,  166408,  166462,  166512,  166554,  166699,  166781,  166787,  166842,  166861,  166889,  166895,  166923,  166960,  167039,  167078,  167103,  167282,  167673,  167676,  167754,  167807,  167913,  170854,  170899,  171091,  171106,  171178,  171282,  171446,  171812,  171895,  171926,  172033,  172252,  172268,  172399,  172411,  172620,  172778,  172799,  172819,  172847,  172949,  172992,  173236,  173243,  173389,  173438,  173658,  173661,  173692,  175865,  175873,  175881,  175888,  175921,  175975,  176023,  176165,  176201,  176207,  176304,  176467,  176545,  176697,  176765,  176796,  176801,  176841,  176850,  176885,  176928,  176959,  177074,  177096,  177139,  177183,  177222,  177319,  177718,  177776,  177788,  177791,  178098,  178101,  178116,  178162,  178380,  178424,  178708,  178731,  178803,  178866,  178970,  179434,  179497,  179563,  179977,  180093,  180252};

   const int nrunbins2 = 58;
   Double_t xbins_run2[nrunbins2] = {160431,  160940,  161016,  161156,  161312,  162826,  163233,  163270,  163332,  163369,  163385,  163481,  163587,  163659,  163757,  163869,  165120,  165472,  165570,  166034,  166374,  166512,  166763,  166859,  166895,  166967,  167151,  167676,  167898,  170901,  171178,  171578,  172014,  172268,  172619,  172801,  172949,  173241,  173439,  173692,  175877,  175973,  176165,  176289,  176547,  176796,  176848,  176929,  177096,  177201,  177719,  177791,  178110,  178380,  178731,  178920,  179547,  180093};

   const int nrunbins3 = 6;
   Double_t xbins_run3[nrunbins3] = {160431,  165088,170249,173236,178420,180252};

   TH1D* _content;
   Double_t bincenter_tmp = 0;
   Double_t bincontent_tmp = 0;
   int index = 0;
   if (type == "pt") {
       nbins = nptbins-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_pt);
       //sanity check
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_pt[j+1] && bincenter_tmp > xbins_pt[j]) index = j; //bin j+1
       }
   } else if (type == "pt2") {
       nbins = nptbins2-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_pt2);
       //sanity check
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_pt2[j+1] && bincenter_tmp > xbins_pt2[j]) index = j; //bin j+1
       }
   } else if (type == "pt3") {
       nbins = nptbins3-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_pt3);
       //sanity check
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_pt3[j+1] && bincenter_tmp > xbins_pt3[j]) index = j; //bin j+1
       }

   } else if (type == "eta2") { 
      nbins = netabins2-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_eta2);
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_eta2[j+1] && bincenter_tmp > xbins_eta2[j]) index = j; //bin j+1
       }
   } else if (type == "eta" || type == "pt_eta") {
      nbins = netabins-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_eta);   
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_eta[j+1] && bincenter_tmp > xbins_eta[j]) index = j; //bin j+1
       }
   } else if (type == "eta4") {
      nbins = netabins4-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_eta4);
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_eta4[j+1] && bincenter_tmp > xbins_eta4[j]) index = j; //bin j+1
       }
   } else if (type == "eta3") {
      nbins = netabins3-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_eta3);
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_eta3[j+1] && bincenter_tmp > xbins_eta3[j]) index = j; //bin j+1
       }
   } else if (type == "vtx") {
      cout << "Use this" << endl;
      nbins = nvtxbins-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_vtx); 
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_vtx[j+1] && bincenter_tmp > xbins_vtx[j]) index = j; //bin j+1
       }
   } else if (type == "run"){
       nbins = nrunbins-1;
       _content = new TH1D(scenario,scenario, nbins, xbins_run); 
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       cout << bincenter_tmp  << " bincontent_tmp " << bincontent_tmp << endl;
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_run[j+1] && bincenter_tmp > xbins_run[j]) index = j; //bin j+1
       }
   } else if (type == "rrr2") {
      nbins = nrunbins2-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_run2);
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_run2[j+1] && bincenter_tmp > xbins_run2[j]) index = j; //bin j+1
       }
   } else if (type == "rrr3") {
      nbins = nrunbins3-1;
      _content = new TH1D(scenario,scenario, nbins, xbins_run3);
       h->GetPoint(0,bincenter_tmp,bincontent_tmp);
       for (int j = 0; j < nbins+1; j++) {
           if (bincenter_tmp < xbins_run3[j+1] && bincenter_tmp > xbins_run3[j]) index = j; //bin j+1
       }
   }

   cout << "INDEX " << index << endl;

   for (int ibin = 0; ibin < nbins+1; ibin++) {
                h->GetPoint(ibin,bincenter,bincontent);
                Xerr = h->GetErrorX(ibin);
                Yerr = h->GetErrorY(ibin);
		content = bincontent;//*bincontent;
                //cout << ibin << " Checking the contents here: " << scenario << " " << content << endl;
                //if (ibin != nbins-1) _content->Fill(bincenter,content);
                //else _content->SetBinContent(ibin+1,content);
                _content->SetBinContent(index+ibin+1,content); //Fill(bincenter,content);
                _content->SetBinError(index+ibin+1,Yerr*bincontent);
                cout << "Set " << _content->GetBinContent(ibin+1) << endl; 
                content = 0;
                bincontent = 0;
                bincenter = 0; 
  }

  return _content;
}
