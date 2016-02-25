void EEmapParser(TString INPUTDIR_PREFIX, TString SCENARIO, TString BINNING, TString whichRun) { 

   //for split run maps only
   //44X IsolPFRelCombR03
   //42X IsolpfMyHadIso03
   
   cout << "WARNING: BINNING parameter is dummy " << endl;
    
   TString DENOM = ""; 
   TString funHEAD = "RECOID";
   if (SCENARIO == "Glb_Tight2012_WP80ToHLTEle17") {
       DENOM = "";
       funHEAD = "EETRIG_Mu17leg";
   } else if (SCENARIO == "Glb_Tight2012_WP80ToHLTEle8") {
       DENOM = "";
       funHEAD = "EETRIG_Mu8leg";
   }

   //FIXME this is solution only for this version
   if (funHEAD == "EETRIG_Mu8leg" || funHEAD == "EETRIG_Mu17leg") {
      BINNING = "EE";
   } 

   TString abseta = "eta";

   gStyle->SetOptTitle(0);
   gStyle->SetPaintTextFormat("4.3f");

   if (BINNING == "EE") {
     const int nptbins = 7;
     const int netabins = 11;
     Double_t xbins_eta[netabins] = {-2.5,-2.0,-1.566,-1.4442,-0.8,0, 0.8, 1.4442, 1.566, 2.0, 2.5};
     Double_t xbins_pt[nptbins] = {10, 15, 20, 30, 40, 50, 500};
   } 

   //MC
   TFile *fmc = new TFile(INPUTDIR_PREFIX+"TnP_2011_MuonID_item_datalike_mc_"+SCENARIO+"_pt_eta.root"); //FIXME
   //cout << "SCENARIO " << SCENARIO << endl;
   //cout << "DENOM " << DENOM << endl;
   //cout << INPUTDIR_PREFIX+"TnP_2011_MuonID_item_datalike_mc_"+SCENARIO+"_pt_eta.root" << endl;
   fmc->cd();
   //gDirectory->cd("tpTree");
   //trigger eff only
   if (SCENARIO=="Glb_Tight2012_WP80ToHLTEle17") SCENARIO = "WP80ToHLTEle17/Glb_Tight2012";  
   if (SCENARIO=="Glb_Tight2012_WP80ToHLTEle8") SCENARIO = "WP80ToHLTEle8/Glb_Tight2012";
   //cout << "2 SCENARIO " << SCENARIO << endl;

   gDirectory->cd(SCENARIO+"_pt_eta"); //FIXME
   //cout << SCENARIO+"_pt_"+abseta+"" << endl;
   gDirectory->cd("fit_eff_plots");

   //cout << "+++++MC-report++++" << endl;
   TH2D* hOverall_mc = new TH2D("overall_mc","overall_mc",nptbins-1,xbins_pt,netabins-1,xbins_eta);
   for (int iptbin = 0; iptbin < nptbins-1; iptbin++) {
       //cout << "this pt bin " << iptbin << endl;
       std::ostringstream pprint;
       pprint.str("");
       pprint<<iptbin;
       string bin = pprint.str();

       TCanvas* cmceta = (TCanvas*)gDirectory->Get("probe_sc_"+abseta+"_PLOT_probe_sc_et_bin"+TString(bin)+DENOM);
       //cout << abseta+"_PLOT_pt_bin"+TString(bin)+DENOM << endl;
       cmceta->GetListOfPrimitives();
       TObject *obj;
       TIter next(cmceta->GetListOfPrimitives());
       TString frame;
       obj=next();
       if (obj->InheritsFrom("TH1")) frame = obj->GetName();
       RooHist* hmc = (RooHist*)cmceta->GetPrimitive("hxy_fit_eff");
       TH1D* hmc_squared = Squared(hmc,abseta, BINNING);

        for (int ietabin = 0; ietabin < netabins-1; ietabin++) {
            //cout << "this eta bin " << xbins_eta[ietabin+1] << " to " << xbins_eta[ietabin] << " " << hmc_squared->GetBinContent(ietabin+1) << endl;
            //cout << "Hello0" << endl;
          //if (iptbin != nptbins-2 && !(ietabin == 0 && iptbin > 4) && !(ietabin == netabins-2 && iptbin > 4)) {
            hOverall_mc->SetBinContent(iptbin+1,ietabin+1,hmc_squared->GetBinContent(ietabin+1));
            hOverall_mc->SetBinError(iptbin+1,ietabin+1,hmc_squared->GetBinError(ietabin+1));
        }//eta loop 
     }//pt bin loop

   TCanvas *c = new TCanvas("2Dmap_"+SCENARIO,"2Dmap_"+SCENARIO,800,600);
   c->cd();
   c->SetLogx();
   c->SetRightMargin(0.13);
   gStyle->SetOptTitle(0);
   gStyle->SetPalette(1);
   gStyle->SetOptStat(kFALSE);
   hOverall_mc->GetXaxis()->SetTitle("Probe p_{T} [GeV]");
   hOverall_mc->GetYaxis()->SetTitle("Probe #eta");
   hOverall_mc->GetXaxis()->SetMoreLogLabels();
   hOverall_mc->Draw("COLZ TEXT");
   //c->SaveAs("2Dmap"+SCENARIO+"_MC.png");

   for (int ipt = 0; ipt < 6; ipt++) {
     for (int ieta = 0; ieta < 10; ieta++) {
         if (ieta != 10-1) {
           cout << hOverall_mc->GetBinContent(ipt+1,ieta+1)  << ", ";
         } else {
           cout << hOverall_mc->GetBinContent(ipt+1,ieta+1);
         }
        }
        if (ipt != 10-1) {
           cout << "}, {" << endl;
        } else {
           cout << "}};" << endl;
        }
    }//x bins loop



/*
   //Data
   if (SCENARIO=="WP80ToHLTEle17/Glb_Tight2012") SCENARIO = "Glb_Tight2012_WP80ToHLTEle17";
   if (SCENARIO=="WP80ToHLTEle8/Glb_Tight2012") SCENARIO = "Glb_Tight2012_WP80ToHLTEle8";
   TFile *fdata = new TFile(INPUTDIR_PREFIX+"TnP_2011_MuonID_item_data_all_"+SCENARIO+"_pt_eta"+whichRun+".root"); //FIXME
   fdata->cd();
   cout << "SCENARIO " << SCENARIO << endl;
   cout << "DENOM " << DENOM << endl;
   cout << INPUTDIR_PREFIX+"TnP_2011_MuonID_item_data_all_"+SCENARIO+"_pt_eta.root" << endl;
   //gDirectory->cd("tpTree");
   if (SCENARIO=="Glb_Tight2012_WP80ToHLTEle17") SCENARIO = "WP80ToHLTEle17/Glb_Tight2012";
   if (SCENARIO=="Glb_Tight2012_WP80ToHLTEle8") SCENARIO = "WP80ToHLTEle8/Glb_Tight2012";

   gDirectory->cd(SCENARIO+"_pt_eta"); //FIXME
   gDirectory->cd("fit_eff_plots");

   cout << "+++++Data-report++++" << endl;
   TH2D* hOverall_data = new TH2D("overall_data","overall_data",nptbins-1,xbins_pt,netabins-1,xbins_eta);
   for (int iptbin = 0; iptbin < nptbins-1; iptbin++) {
       std::ostringstream pprint;
       pprint.str("");
       pprint<<iptbin;
       string bin = pprint.str();

       TCanvas* cdataeta = (TCanvas*)gDirectory->Get("probe_sc_"+abseta+"_PLOT_probe_sc_et_bin"+TString(bin)+DENOM);
       cdataeta->GetListOfPrimitives();
       TObject *obj;
       TIter next(cdataeta->GetListOfPrimitives());
       TString frame;
       obj=next();       if (obj->InheritsFrom("TH1")) frame = obj->GetName();
       RooHist* hdata = (RooHist*)cdataeta->GetPrimitive("hxy_fit_eff");
       TH1D* hdata_squared = Squared(hdata,abseta, BINNING);

        for (int ietabin = 0; ietabin < netabins-1; ietabin++) {
            cout << "this dataeta bin " << xbins_eta[ietabin+1] << " to " << xbins_eta[ietabin] << " " << hdata_squared->GetBinContent(ietabin+1) << endl;
            //data_out << hdata_squared->GetBinContent(ietabin+1) << " ";
            //data_out << hdata_squared->GetBinError(ietabin+1) << " ";   
            //if (iptbin != nptbins-2 && !(ietabin == 0 && iptbin > 4) && !(ietabin == netabins-2 && iptbin > 4)) {
               hOverall_data->SetBinContent(iptbin+1,ietabin+1,hdata_squared->GetBinContent(ietabin+1));
               hOverall_data->SetBinError(iptbin+1,ietabin+1,hdata_squared->GetBinError(ietabin+1));
        }//eta bin loop
     }//pt bin loop

     TCanvas *c2 = new TCanvas("2Dmap_"+SCENARIO,"2Dmap_"+SCENARIO,800,600);
     c2->cd();
     c2->SetLogx();
     c2->SetRightMargin(0.13);
     gStyle->SetOptTitle(0);
     gStyle->SetPalette(1);
     gStyle->SetOptStat(kFALSE);
     hOverall_data->GetXaxis()->SetTitle("Probe p_{T} [GeV]");
     hOverall_data->GetYaxis()->SetTitle("Probe #eta");
     cout << "WARNING: saving where you are "<< endl;
     fstream data_out("./efficiencyWeightToBin.C", ios::out | fstream::app);

    //FIXME construction goes here
    if (funHEAD == "RECOID") { 
        data_out <<  "#include <TStyle.h>" << endl;
        data_out <<  "#include <TChain.h>" << endl;
        data_out <<  "#include <TFile.h>" << endl;
        data_out <<  "#include <TH1D.h>" << endl;
        data_out <<  "#include <TGraphAsymmErrors.h>" << endl;
        data_out <<  "#include <TMultiGraph.h>" << endl;
        data_out <<  "#include <TLegend.h>" << endl;
        data_out <<  "#include <TCanvas.h>" << endl;
        data_out <<  "#include <TROOT.h>" << endl;
        data_out <<  "#include <THStack.h>" << endl;
        data_out <<  "#include <TMath.h>" << endl;
        data_out <<  "#include <TText.h>" << endl;
        data_out <<  "#include <TPad.h>" << endl;
        data_out <<  "#include <TPaveText.h>" << endl;
        data_out <<  "#include <iostream>" << endl;
        data_out <<  "#include <string>" << endl;
        data_out <<  "#include <fstream>" << endl;
        data_out <<  "#include <sstream>" << endl;
        data_out <<  "#include <iomanip>" << endl;
        data_out <<  "#include <vector>" << endl;
        data_out <<  endl;
        data_out <<  "using namespace std;" << endl;
        data_out <<  endl;
        data_out <<  "int Find_IndexRECOID( double muon_input, bool isPt )" << endl;
        data_out <<  "{" << endl;
        data_out <<  " const int nptbins = "<<to_string(nptbins)<<";" << endl;
        data_out <<  " const int netabins = "<<to_string(netabins)<<";" << endl;
        data_out <<  "   Double_t eta_bin[netabins] = {";
     for (int i = 0; i < netabins-1; i++) {
           data_out <<  to_string(xbins_eta[i]) << ", ";
      } 
        data_out <<  to_string(xbins_eta[netabins-1]) << "};" << endl;  
        data_out <<  "   Double_t pt_bin[nptbins] = {";
     for (int i = 0; i < nptbins-1; i++) {
           data_out <<  to_string(xbins_pt[i]) <<", ";
      }
        data_out <<  to_string(xbins_pt[nptbins-1]) << "};" << endl;
        data_out <<  "   int Index = -1;" << endl;
        data_out <<  "" << endl;
        data_out <<  "  if( isPt ) {" << endl;
        data_out <<  "    for( int i = 0; i < nptbins-1; i++ ) {" << endl;
        data_out <<  "      if( muon_input > pt_bin[i] && muon_input < pt_bin[i+1] ) {" << endl;
        data_out <<  "             Index = i;" << endl;
        data_out <<  "             break;" << endl;
        data_out <<  "      }" << endl;
        data_out <<  "      else if( muon_input > pt_bin[nptbins-1] ) Index = nptbins-2; //out of bounds" << endl;
        data_out <<  "    }" << endl;
        data_out <<  "    return Index;" << endl;
        data_out <<  "  } else {" << endl;
        data_out <<  "    for( int i = 0; i < netabins-1; i++ ) {" << endl;
        data_out <<  "      if( muon_input > eta_bin[i] && muon_input < eta_bin[i+1] ) {" << endl;
        data_out <<  "          Index = i; " << endl;
        data_out <<  "          break;" << endl;
        data_out <<  "       }" << endl;
        data_out <<  "    }" << endl;
        data_out <<  "    return Index;" << endl;
        data_out <<  "  }" << endl;
        data_out <<  "}" << endl;
        data_out <<  endl;
        data_out <<  endl;
    } //end hat 1
    if (funHEAD == "ISO") {
        data_out <<  "int Find_Index( double muon_input, bool isPt )" << endl;
        data_out <<  "{" << endl;
        data_out <<  " const int nptbins = "<<to_string(nptbins)<<";" << endl;
        data_out <<  " const int netabins = "<<to_string(netabins)<<";" << endl;
        data_out <<  "   Double_t eta_bin[netabins] = {";
     for (int i = 0; i < netabins-1; i++) {
           data_out <<  to_string(xbins_eta[i]) << ", ";
      }
        data_out <<  to_string(xbins_eta[netabins-1]) << "};" << endl;
        data_out <<  "   Double_t pt_bin[nptbins] = {";
     for (int i = 0; i < nptbins-1; i++) {
           data_out <<  to_string(xbins_pt[i]) <<", ";
      }
        data_out <<  to_string(xbins_pt[nptbins-1]) << "};" << endl;
        data_out <<  "   int Index = -1;" << endl;
        data_out <<  "" << endl;
        data_out <<  "  if( isPt ) {" << endl;
        data_out <<  "    for( int i = 0; i < nptbins-1; i++ ) {" << endl;
        data_out <<  "      if( muon_input > pt_bin[i] && muon_input < pt_bin[i+1] ) {" << endl;
        data_out <<  "             Index = i;" << endl;
        data_out <<  "             break;" << endl;
        data_out <<  "      }" << endl;
        data_out <<  "      else if( muon_input > pt_bin[nptbins-1] ) Index = nptbins-2; //out of bounds" << endl;
        data_out <<  "    }" << endl;
        data_out <<  "    return Index;" << endl;
        data_out <<  "  } else {" << endl;
        data_out <<  "    for( int i = 0; i < netabins-1; i++ ) {" << endl;
        data_out <<  "      if( muon_input > eta_bin[i] && muon_input < eta_bin[i+1] ) {" << endl;
        data_out <<  "          Index = i; " << endl;
        data_out <<  "          break;" << endl;
        data_out <<  "       }" << endl;
        data_out <<  "    }" << endl;
        data_out <<  "    return Index;" << endl;
        data_out <<  "  }" << endl;
        data_out <<  "}" << endl;
        data_out <<  endl;
     }//end hat 2
     TString marker = "";
     if (funHEAD == "RECOID") marker = funHEAD;
     data_out <<  "double efficiency_"<<funHEAD<<"( double muon_pt, double muon_eta, bool isMC , TString runSplit) {" << endl;
     data_out <<  " int index_pt = -1;" <<  endl; 
     data_out <<  " int index_eta = -1;" << endl;
     data_out <<  endl;
     data_out <<  " const int nptbins = "<<to_string(nptbins)<<";" << endl;
     data_out <<  " const int netabins = "<<to_string(netabins)<<";" << endl;
     data_out <<  " index_pt = Find_Index"+marker+"(muon_pt, true);" << endl;
     data_out <<  " index_eta = Find_Index"+marker+"(muon_eta, false);" << endl;
     data_out <<  endl;
     data_out <<  " double recoid_eff_data[nptbins-1][netabins-1] = {{" << endl;
     for (int ixbin = 0; ixbin < hOverall_data->GetXaxis()->GetNbins(); ixbin++) {
        for (int iybin = 0; iybin < hOverall_data->GetYaxis()->GetNbins(); iybin++) {
          if (iybin != hOverall_data->GetYaxis()->GetNbins()-1) {
            data_out << hOverall_data->GetBinContent(ixbin+1,iybin+1) << ", ";
          } else {
            data_out << hOverall_data->GetBinContent(ixbin+1,iybin+1); 
          } 
        }      
         if (ixbin != hOverall_data->GetXaxis()->GetNbins()-1) {
            data_out << "}, {" << endl;
         } else {
            data_out << "}};" << endl;
         }
     }//x bins loop
     data_out <<  endl;
     data_out <<  " double recoid_eff_mc[nptbins-1][netabins-1] = {{" << endl;
     for (int ixbin = 0; ixbin < hOverall_mc->GetXaxis()->GetNbins(); ixbin++) {
        for (int iybin = 0; iybin < hOverall_mc->GetYaxis()->GetNbins(); iybin++) {
          if (iybin != hOverall_mc->GetYaxis()->GetNbins()-1) {
            data_out << hOverall_mc->GetBinContent(ixbin+1,iybin+1) << ", ";
          } else {
            data_out << hOverall_mc->GetBinContent(ixbin+1,iybin+1);
          }
        }
         if (ixbin != hOverall_mc->GetXaxis()->GetNbins()-1) {
            data_out << "}, {" << endl;
         } else {
            data_out << "}};" << endl;
         }
     }//x bins loop
     data_out <<  endl;
     data_out <<  "  if ( isMC ) return recoid_eff_mc[index_pt][index_eta];" << endl;
     data_out <<  "  else return recoid_eff_data[index_pt][index_eta];" << endl;
     data_out <<  "}" << endl;
     data_out << endl;
     data_out << endl;
     data_out <<  "double efficiencyError_"<<funHEAD<<"( double muon_pt, double muon_eta, bool isMC , TString runSplit) {" << endl;
     data_out <<  " int index_pt = -1;" <<  endl;
     data_out <<  " int index_eta = -1;" << endl;
     data_out <<  endl;
     data_out <<  " const int nptbins = "<<to_string(nptbins)<<";" << endl;
     data_out <<  " const int netabins = "<<to_string(netabins)<<";" << endl;
     data_out <<  " index_pt = Find_Index"+marker+"(muon_pt, true);" << endl;
     data_out <<  " index_eta = Find_Index"+marker+"(muon_eta, false);" << endl;
     data_out <<  endl;
     data_out <<  " double recoid_eff_data[nptbins-1][netabins-1] = {{" << endl;
     for (int ixbin = 0; ixbin < hOverall_data->GetXaxis()->GetNbins(); ixbin++) {
        for (int iybin = 0; iybin < hOverall_data->GetYaxis()->GetNbins(); iybin++) {
          if (iybin != hOverall_data->GetYaxis()->GetNbins()-1) {
            data_out << hOverall_data->GetBinError(ixbin+1,iybin+1) << ", ";
          } else {
            data_out << hOverall_data->GetBinError(ixbin+1,iybin+1);
          }
         }
         if (ixbin != hOverall_data->GetXaxis()->GetNbins()-1) {
            data_out << "}, {" << endl;
         } else {
            data_out << "}};" << endl;
         }
     }//x bins loop
     data_out <<  endl;
     data_out <<  endl;
     data_out <<  " double recoid_eff_mc[nptbins-1][netabins-1] = {{" << endl;
     for (int ixbin = 0; ixbin < hOverall_mc->GetXaxis()->GetNbins(); ixbin++) {
        for (int iybin = 0; iybin < hOverall_mc->GetYaxis()->GetNbins(); iybin++) {
          if (iybin != hOverall_mc->GetYaxis()->GetNbins()-1) {
            data_out << hOverall_mc->GetBinError(ixbin+1,iybin+1) << ", ";
          } else {
            data_out << hOverall_mc->GetBinError(ixbin+1,iybin+1);
          }
         }
         if (ixbin != hOverall_mc->GetXaxis()->GetNbins()-1) {
            data_out << "}, {" << endl;
         } else {
            data_out << "}};" << endl;
         }
     }//x bins loop
     data_out <<  endl;
     data_out <<  "   if ( isMC ) return recoid_eff_mc[index_pt][index_eta];" << endl;
     data_out <<  "   else return recoid_eff_data[index_pt][index_eta];" << endl;
     data_out <<  endl;
     data_out <<  "}" << endl;
     //correction map
     //TH1D* hOverall_correction = (TH1D*)hOverall_data->Clone();
     hOverall_data->GetXaxis()->SetMoreLogLabels();
     hOverall_data->Draw("COLZ TEXT");
     //hOverall_correction->Divide(hOverall_data,hOverall_mc);
     //hOverall_correction->GetXaxis()->SetMoreLogLabels();
     //hOverall_correction->Draw("COLZ ETEXT");
     c2->SaveAs("2Dmap"+SCENARIO+"_Data.png");
*/
}

TH1D* Squared(RooHist* h, TString type, TString BINNING) {

   Double_t bincenter,bincontent;
   Double_t Xerr, Yerr;
   Double_t squared;

   //book return histogram
   int nbins;

   if (BINNING == "14"){
     const int nptbins = 9;
     const int netabins = 18;
     Double_t xbins_eta[netabins] = {-2.4,-2.1,-1.9,-1.7,-1.5,-1.1,-0.9,-0.7,-0.5,0.5,0.7,0.9,1.1,1.5,1.7,1.9,2.1,2.4};
     Double_t xbins_pt[nptbins] = {8,17,22,30,40,50,65,250,500};
   } else if (BINNING == "15"){
     const int nptbins = 8;
     const int netabins = 20;
     Double_t xbins_eta[netabins] = {-2.4,-2.1,-1.9,-1.5,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.5,1.9,2.1,2.4};
     Double_t xbins_pt[nptbins] = {8,17,30,40,50,70,250,500};
   } else if (BINNING == "EE") {
     const int nptbins = 7;
     const int netabins = 11;
     Double_t xbins_eta[netabins] = {-2.5,-2.0,-1.566,-1.4442,-0.8,0, 0.8, 1.4442, 1.566, 2.0, 2.5};
     Double_t xbins_pt[nptbins] = {10, 15, 20, 30, 40, 50, 500};
   } 

   TH1D* _squared;

   if (type == "pt") {
       nbins = nptbins;
      _squared = new TH1D("hsquared","hsquared", nbins-1, xbins_pt);
   } else {
      nbins = netabins;
      _squared = new TH1D("hsquared","hsquared", nbins-1, xbins_eta);   
   }

   //cout << "Fill histo" << endl;
   for (int ibin = 0; ibin < nbins-1; ibin++) {
                h->GetPoint(ibin,bincenter,bincontent);
                Xerr = h->GetErrorX(ibin);
                Yerr = h->GetErrorY(ibin);
                //FIXME
                squared = bincontent;//*bincontent;
                //cout << "before " << bincenter << " "   << bincontent << endl;
                _squared->Fill(bincenter,squared); //SetBinContent(ibin+1,squared); //Fill(bincenter,squared);
                _squared->SetBinError(ibin+1,Yerr*bincontent); //SetBinError(ibin+1,2.*Yerr*bincontent);
                //cout << "after " << _squared->GetBinContent(ibin+1) << endl;
                //cout << "bin " << ibin << " from " << xbins_pt[ibin] << " to " << xbins_pt[ibin+1]<< endl;
   //             cout << "Checking bin content " << _squared->GetBinContent(ibin+1) << " but filled " << squared<< endl;
  }
  return _squared;
}

/*   while ((obj=next())) {
     cout << "Reading: " << obj->GetName() << endl;
       if (obj->InheritsFrom("TH1")) {
           cout << "histo: " << obj->GetName() << endl;
       }
  }  
*/ 


template <typename T>
std::string to_string(T const& value) {
    stringstream sstr;
    sstr << value;
    return sstr.str();
}
