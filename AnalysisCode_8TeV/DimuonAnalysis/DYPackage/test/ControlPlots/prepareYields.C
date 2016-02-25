//================================================================================================
//
// Prepare binned histograms with signal and background events for further analysis.
//
//________________________________________________________________________________________________

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TCanvas.h>                // class for drawing
#include <TH1F.h>                   // 1D histograms
#include <THStack.h>
#include <TBenchmark.h>             // class to track macro running statistics
#include <TVector3.h>               // 3D vector class
#include <TArrayD.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TLorentzVector.h>         // 4-vector class
#include <TRandom.h>
#include <TDatime.h>                // time stamp
#include <TMath.h>

#include <TLatex.h> 

#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <string>                   // C++ string class
#include <sstream>                  // class for parsing strings

#include "../Include/CPlot.hh"          // helper class for plots
#include "../Include/MitStyleRemix.hh"  // style settings for drawing
#include "../Include/MyTools.hh"        // miscellaneous helper functions
#include "../Include/CSample.hh"        // helper class for organizing input ntuple files    

// define structures to read in ntuple
#include "../Include/ZeeData.hh"

#include "../Include/ElectronEnergyScale.hh"        // energy scale correction

#include "../Include/DYTools.hh"
#include "../Include/MyTools.hh"
#include "../Include/DYToolsUI.hh"
#include "../Include/plotFunctions.hh"
#include "../Include/PUReweight.hh"
#include "../Include/UnfoldingTools.hh"
#include "../Include/ComparisonPlot.hh"

//#include "../Include/TEventInfo.hh"

#include "../Include/efficiencyWeightToBin2012.C"

#endif

// Forward declarations
/*
void DrawMassPeak(vector<TH1F*> hMassv, vector<CSample*> samplev, vector<TString> snamev, TH1F* hMassDibosons, bool hasData, 
                   bool mergeDibosons, TString labelDibosons, Int_t colorDibosons, Double_t lumi, char* lumitext,  bool actualBinning);

void DrawFlattened(vector<TMatrixD*> yields, vector<TMatrixD*> yieldsSumw2, vector<CSample*> samplev, vector<TString> snamev, bool hasData, 
                   bool mergeDibosons, TString labelDibosons, Int_t colorDibosons, Double_t lumi, char* lumitext);

void Draw6Canvases(vector<TMatrixD*> yields, vector<TMatrixD*> yieldsSumw2,
                    vector<CSample*> samplev, vector<TString> snamev, 
                    bool hasData, double dataOverMc, double* dataOverMcEachBin, bool normEachBin=1, bool singleCanvas=0);
void SomeHistAttributes (TH1F* hist, TString samplename);
*/
//void SaveCanvas(TCanvas* canv, TString canvName);

Bool_t passEGMID(ZeeData_t *data, TString label);
void latexPrintoutBkgSources(  vector<TString>  snamev, vector<CSample*> samplev, vector<TH1F*>    hMassBinsv);

//=== MAIN MACRO =================================================================================================

void prepareYields(const TString conf  = "data_plot.conf",
		   DYTools::TSystematicsStudy_t runMode=DYTools::NORMAL,
		   const TString plotsDirExtraTag="",
		   int performPUReweight=1)
{  
  gBenchmark->Start("prepareYields");

  std::cout << "\n\nRun mode: " << SystematicsStudyName(runMode) << "\n";
  switch(runMode) {
  case DYTools::NORMAL:
  case DYTools::ESCALE_STUDY:
  case DYTools::ESCALE_STUDY_RND:
    break;
  default:
    std::cout << "prepareYields is not ready for runMode=" << SystematicsStudyName(runMode) << "\n";
    throw 2;
  }
 
  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //==============================================================================================================

  TString  outputDir;         // output directory
  Double_t lumi;              // luminosity (pb^-1)
  Bool_t   doWeight;          // weight events?
  TString  escaleTag;         // Energy scale calibrations tag
  TString  format;            // plot format

  vector<TString>  snamev;    // sample name (for output file)
  vector<CSample*> samplev;   // data/MC samples
  Bool_t hasData=false;
    
  //
  // parse .conf file
  //
  ifstream ifs;
  ifs.open(conf.Data());
  assert(ifs.is_open());
  string line;
  Int_t state=0;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    if(line[0]=='%') { 
      state++; 
      continue; 
    }
    if(line[0]=='$') {
      samplev.push_back(new CSample());
      stringstream ss(line);
      string chr;
      string sname;
      Int_t color;
      ss >> chr >> sname >> color;
      string label = line.substr(line.find('@')+1);
      snamev.push_back(sname);
      samplev.back()->label = label;
      samplev.back()->color = color;
      continue;
    }
    
    if(state==0) {  // general settings
      stringstream ss1(line); ss1 >> lumi;
      getline(ifs,line);
      stringstream ss2(line); ss2 >> doWeight;
      getline(ifs,line);
      outputDir = TString(line);
      getline(ifs,line);
      stringstream ss3(line); ss3 >> escaleTag;
      getline(ifs,line);
      format = TString(line);
      
    } else if(state==1) {  // define data sample
      string fname;
      Double_t xsec;
      string json;
      stringstream ss(line);
      ss >> fname >> xsec >> json;
      samplev.back()->fnamev.push_back(fname);
      samplev.back()->xsecv.push_back(xsec);
      samplev.back()->jsonv.push_back(json);
      hasData=true;
    
    } else if(state==2) {  // define MC samples
      string fname;
      Double_t xsec;
      stringstream ss(line);
      ss >> fname >> xsec;
      samplev.back()->fnamev.push_back(fname);
      samplev.back()->xsecv.push_back(xsec);
    }
  }
  ifs.close();
  
  // 
  // Set up energy scale corrections
  //
  ElectronEnergyScale escale(escaleTag);
  assert(escale.isInitialized());
  escale.print();

  // sOutDir is a static data member in the CPlot class.
  // There is a strange crash of the whole ROOT session well after
  // this script is executed when one attempts to exit ROOT, with 
  // a dump of memory map. This happens only on UNL Tier3, but
  // there is no sign of a problem on any other computer.
  //   The consequence of this variable is not set is that the plots
  // will be created in the local directory rather than the
  // one configured through sOutDir.
//   CPlot::sOutDir = outputDir + TString("/plots");

  if ((runMode==DYTools::ESCALE_STUDY) || (runMode==DYTools::ESCALE_STUDY_RND)) {
    CPlot::sOutDir = "plots_escale/";
  }
  else CPlot::sOutDir = TString("plots") + DYTools::analysisTag;
  if (!performPUReweight) CPlot::sOutDir.Append("_noPU");
  CPlot::sOutDir += plotsDirExtraTag;

  //
  // Canvas dimensions
  //
  //Int_t canw=800, canh=600;
  
  
  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //============================================================================================================== 

  vector<TMatrixD*> yields;
  vector<TMatrixD*> yieldsSumw2;
  int maxYBins = DYTools::findMaxYBins();
  vector<TString> sampleTags;

  vector<TH1F*>    hMassv;
  vector<TH1F*>    hMassv_weighted;
  vector<TH1F*>    hMassv_bb;
  vector<TH1F*>    hMassv_ee;
  vector<TH1F*>    hMassv_oo;
  //vector<TH1F*>    hMassv_bb_r92;
  //vector<TH1F*>    hMassv_ee_r92;
  //vector<TH1F*>    hMassv_oo_r92;

  //Control plots for AN
  vector<TH1F*>    hscEtv_b;
  vector<TH1F*>    hscEtav_b;
  vector<TH1F*>    hrelPFIso03_b;
  vector<TH1F*>    hsigiEtaiEta_b;
  vector<TH1F*>    hdeltaEtaIn_b;
  vector<TH1F*>    hdeltaPhiIn_b;
  vector<TH1F*>    hHoverE_b;
  vector<TH1F*>    hDXY_b;
  vector<TH1F*>    hDZ_b;
  vector<TH1F*>    hOOEOOP_b;
  vector<TH1F*>    hExpHitsInner_b;
  vector<TH1F*>    hNPV_b;

  vector<TH1F*>    hscEtv_e;
  vector<TH1F*>    hscEtav_e;
  vector<TH1F*>    hrelPFIso03_e;
  vector<TH1F*>    hsigiEtaiEta_e;
  vector<TH1F*>    hdeltaEtaIn_e;
  vector<TH1F*>    hdeltaPhiIn_e;
  vector<TH1F*>    hHoverE_e;
  vector<TH1F*>    hDXY_e;
  vector<TH1F*>    hDZ_e;
  vector<TH1F*>    hOOEOOP_e;
  vector<TH1F*>    hExpHitsInner_e;
  vector<TH1F*>    hNPV_e;

  vector<TH1F*>    hMassBinsv;
  vector<Double_t> nSelv;
  vector<Double_t> nSelVarv;  

  vector<TH1F*> hZpeakv;

  char hname[100];
  for(UInt_t isam=0; isam<samplev.size(); isam++) {
    TMatrixD *yieldsMatrix = new TMatrixD(DYTools::nMassBins, maxYBins);
    (*yieldsMatrix) = 0;
    yields.push_back(yieldsMatrix);
    //
    TMatrixD *yieldsSumw2Matrix = new TMatrixD(DYTools::nMassBins, maxYBins);
    (*yieldsSumw2Matrix) = 0;
    yieldsSumw2.push_back(yieldsSumw2Matrix);
    //
    sampleTags.push_back(snamev[isam]);

    sprintf(hname,"hMass_%i",isam);  
    if (DYTools::study2D==1) {
      hMassv.push_back(new TH1F(hname,"",156,0,156));
      hMassv_weighted.push_back(new TH1F(hname,"weighted",156,0,156));
    } else {
      hMassv.push_back(new TH1F(hname,"",DYTools::nMassBins,
                                  DYTools::massBinLimits));
      hMassv_weighted.push_back(new TH1F(hname,"weighted",DYTools::nMassBins,
                                  DYTools::massBinLimits));
    }
    hMassv_bb.push_back(new TH1F(hname,"",240,60,120));
    hMassv_ee.push_back(new TH1F(hname,"",240,60,120));
    hMassv_oo.push_back(new TH1F(hname,"",240,60,120));

    //hMassv_bb_r92.push_back(new TH1F(hname,"",60,60,120));
    //hMassv_ee_r92.push_back(new TH1F(hname,"",60,60,120));
    //hMassv_oo_r92.push_back(new TH1F(hname,"",60,60,120));

    hscEtv_b.push_back(new TH1F(hname,"scEt_b",120,0,700));
    hscEtav_b.push_back(new TH1F(hname,"scEta_b",48,-2.4,2.4));
    hrelPFIso03_b.push_back(new TH1F(hname,"relPFIso03_b",200,0,1));
    hdeltaEtaIn_b.push_back(new TH1F(hname,"deltaEtaIn_b",140,-0.007,0.007));
    hdeltaPhiIn_b.push_back(new TH1F(hname,"deltaPhiIn_b",120,-0.06,0.06));
    hHoverE_b.push_back(new TH1F(hname,"HoverE_b",100,0,0.1));
    //new control plots
    hsigiEtaiEta_b.push_back(new TH1F(hname,"sigiEtaiEta_b",100,0,0.1));
    hDXY_b.push_back(new TH1F(hname,"hdxy_b",100,-1.,1.)); //
    hDZ_b.push_back(new TH1F(hname,"hdz_b",1000,-10.,10.));
    hOOEOOP_b.push_back(new TH1F(hname,"hooeoop_b",500,0,0.5));
    hExpHitsInner_b.push_back(new TH1F(hname,"ExpHitsInner_b",10,0,10));
    hNPV_b.push_back(new TH1F(hname,"NPV_b",100,0,100));

    hscEtv_e.push_back(new TH1F(hname,"scEt_e",120,0,700));
    hscEtav_e.push_back(new TH1F(hname,"scEta_e",48,-2.4,2.4));
    hrelPFIso03_e.push_back(new TH1F(hname,"relPFIso03_e",200,0,1));
    hdeltaEtaIn_e.push_back(new TH1F(hname,"deltaEtaIn_e",140,-0.007,0.007));
    hdeltaPhiIn_e.push_back(new TH1F(hname,"deltaPhiIn_e",120,-0.06,0.06));
    hHoverE_e.push_back(new TH1F(hname,"HoverE_e",100,0,0.1));
    //new control plots
    hsigiEtaiEta_e.push_back(new TH1F(hname,"sigiEtaiEta_e",100,0,0.1));
    hDXY_e.push_back(new TH1F(hname,"hdxy_e",100,-1.,1.)); //
    hDZ_e.push_back(new TH1F(hname,"hdz_e",1000,-10.,10.));
    hOOEOOP_e.push_back(new TH1F(hname,"hooeoop_e",500,0,0.5));
    hExpHitsInner_e.push_back(new TH1F(hname,"ExpHitsInner_e",10,0,10));
    hNPV_e.push_back(new TH1F(hname,"NPV_e",100,0,100));


    //hMassv[isam]->Sumw2();
    sprintf(hname,"hMassBins_%i",isam);  
    hMassBinsv.push_back(new TH1F(hname,"",
				  DYTools::nMassBins,
				  DYTools::massBinLimits));
    hMassBinsv[isam]->Sumw2();
    hMassBinsv[isam]->SetDirectory(0);

    sprintf(hname,"hZpeak_%s",snamev[isam].Data());
    hZpeakv.push_back(new TH1F(hname,"",
                               //240,60.,120.));
			       60, 60., 120.));
    hZpeakv[isam]->Sumw2();
    hZpeakv[isam]->SetDirectory(0);
			 
    nSelv.push_back(0);
    nSelVarv.push_back(0);  
  }
  

#ifdef ZeeData_is_TObject
  ZeeData_t *data = new ZeeData_t();
#else
  ZeeData *data = new ZeeData();
#endif
  TRandom random;

  int puReweight_new_code=1;
  // Open file with number of PV distributions for pile-up reweighting
  // TString("/npv.root");
  PUReweight_t puWeight;
  const TString fnamePV = outputDir+TString("/npv") + DYTools::analysisTag_USER + TString(".root");
  std::cout << "fnamePV=<" << fnamePV << ">\n";
  TFile *pvfile = NULL;
  TH1F *hPVData = 0;
  if (puReweight_new_code) {
    // Do nothing: for the Hildreth method the code commented out below is not needed
    //     assert(puWeight.setFile(fnamePV));
    //     if (hasData && performPUReweight) {
    //       assert(puWeight.setReference("hNGoodPV_data"));
    //     }
  }
  else {
    pvfile=new TFile(fnamePV);
    assert(pvfile);
    if(hasData){ 
      hPVData = (TH1F*)pvfile->Get("hNGoodPV_data"); assert(hPVData); 
    }
  }

  //
  //  Diboson backgrounds need to be saved separately, but plotted
  //  together. Detect whether there are separate ww/wz/zz contribution,
  //  and whether merging is needed later. Of course, this relies on
  // the fact that the file data.conf has names ww, wz, zz for those
  // contributions.
  int nDibosonSamples = 0;
  bool mergeDibosons = false;
  for(UInt_t isam=0; isam<samplev.size(); isam++) {
    if( snamev[isam] == "ww" || snamev[isam] == "wz" || snamev[isam] == "zz")
      nDibosonSamples++;
  }
  if(nDibosonSamples==3)
    mergeDibosons = true;

  //
  // Access samples and fill histograms
  //  
  TFile *infile=0;
  TTree *eventTree=0; 
  
  for(UInt_t isam=0; isam<samplev.size(); isam++) {

    TString fname = outputDir + TString("/ntuples/") + snamev[isam] + DYTools::analysisTag_USER + TString("_select.root");
    if ((isam==0) && 
	((runMode==DYTools::ESCALE_STUDY) || (runMode==DYTools::ESCALE_STUDY_RND))) {
      // selectEvents corrects only data energies
      TString fnameTag=TString("_select_") + escale.calibrationSetShortName();
      fname.Replace(fname.Index("_select."),sizeof("_select.")-2,fnameTag);
    }
    cout << "Processing " << fname << "..." << endl;   
    infile = new TFile(fname);
    assert(infile); 

    // Prepare weights for pile-up reweighting for MC
    TH1F *puWeights=NULL;
    if (puReweight_new_code) {
      // Do nothing: for the Hildreth method the code commented out below is not needed
//       if (performPUReweight) {
// 	assert(puWeight.setActiveSample(TString("hNGoodPV_")+snamev[isam]));
	//puWeight.printActiveDistr_and_Weights(std::cout);
	//puWeight.printWeights(std::cout);
//       }
    }
    else {
      TH1F *hPVThis = (TH1F*) pvfile->Get(TString("hNGoodPV_")+snamev[isam]); assert(hPVThis);
      hPVThis->SetDirectory(0);
      // Normalize or not? Not clear
      hPVThis->Scale( hPVData->GetSumOfWeights()/hPVThis->GetSumOfWeights());
      puWeights = (TH1F*)hPVData->Clone("puWeights");
      puWeights->SetDirectory(0);
      puWeights->Divide(hPVThis);
      for(int i=1; i<=puWeights->GetNbinsX(); i++)
	printf(" %f    %f    %f\n",puWeights->GetBinCenter(i),puWeights->GetBinContent(i),puWeights->GetBinError(i));
    }

    // Get the TTree and set branch address
    eventTree = (TTree*)infile->Get("Events"); assert(eventTree); 
    eventTree->SetBranchAddress("Events",&data);
    TMatrixD *thisSampleYields = yields.at(isam);
    TMatrixD *thisSampleYieldsSumw2 = yieldsSumw2.at(isam);

    for(UInt_t ientry=0; ientry<eventTree->GetEntries(); ientry++) {
      eventTree->GetEntry(ientry);

      Double_t weight = data->weight;

      // Any extra weight factors:
      if (performPUReweight) {
	double weightPU=(puReweight_new_code) ?
	  puWeight.getWeightHildreth( data->nPV ) :
	  puWeights->GetBinContent( puWeights->FindBin( data->nPV ));
	// Make sure data are not reweighted.
	bool isData = ((isam == 0) && hasData);
	if( !isData )
	  weight *= weightPU;
      }

/*
      if(isam!=0) {            	    
	double smearingCorrection = escale.generateMCSmear(data->scEta_1, data->scEta_2);
	if ( escale.getCalibrationSet() == ElectronEnergyScale::Date20130529_2012_j22_adhoc ){
	  // These calibrtions are designed for multiplicative per-electron smearing correction.
	  // ElectronEnergyScale class is not set up to work with those, so the code
	  // below is a hack.
	  double corr1 = 1.0 + escale.generateMCSmearSingleEle( data->scEta_1 );
	  double corr2 = 1.0 + escale.generateMCSmearSingleEle( data->scEta_2 );
	  // Scale 4-momenta
	  TLorentzVector ele1; 
	  ele1.SetPtEtaPhiM(data->pt_1,data->eta_1,data->phi_1,0.000511);
	  ele1 *= corr1;
	  data->pt_1  = ele1.Pt();
	  data->eta_1 = ele1.Eta();
	  data->phi_1 = ele1.Phi();
	  //
	  TLorentzVector ele2; 
	  ele2.SetPtEtaPhiM(data->pt_2,data->eta_2,data->phi_2,0.000511);
	  ele2 *= corr2;
	  data->pt_2  = ele2.Pt();
	  data->eta_2 = ele2.Eta();
	  data->phi_2 = ele2.Phi();
	  //
	  data->mass = (ele1+ele2).M();
	  data->y = (ele1+ele2).Rapidity();
	}else{
	  data->mass = data->mass + smearingCorrection;
	}
      }
*/
      // Find the 2D bin for this event:
      int massBin = DYTools::findMassBin(data->mass);
      int yBin    = DYTools::findAbsYBin(massBin, data->y);

      if ((massBin==-1) || (yBin==-1)) // out of range
	continue;

      (*thisSampleYields)(massBin,yBin) += weight;
      (*thisSampleYieldsSumw2)(massBin,yBin) += weight*weight;
      //Alexey thats the way to go
      double extra_weight = 1.0;
      //FIXME configure extra weight
      double weight_lepton1 = -1;
      double weight_lepton2 = -1;

      weight_lepton1 = correction_lepton(data->pt_1,fabs(data->eta_1));
      weight_lepton2 = correction_lepton(data->pt_2,fabs(data->eta_2));
      //convert to per event weight
      extra_weight = weight_lepton1*weight_lepton2;

      if (DYTools::study2D==1) { 


        //2D sliced 
        double value = -1;
        double Mass_xbin2[8] = {0, 20, 30, 45, 60, 120, 200, 1500};

        for( int j = 0; j < 7; j++ ) {
          // preFSR
          if( data->mass > Mass_xbin2[j] && data->mass < Mass_xbin2[j+1] ) {
            int nbins;
            double bin_size;
            if( j == 6 ) {
              nbins = 12;
              bin_size = 0.2;
             }
            else {
              nbins = 24;
              bin_size = 0.1;
            }
            for( int k = 0; k < nbins; k++ ) {
              if( fabs(data->y) > k*bin_size && fabs(data->y) < (k+1)*bin_size ) value = k + j*24;
            }
          }
        }
        hMassv[isam]->Fill(value,weight);
        hMassv_weighted[isam]->Fill(value,weight*extra_weight);
      }

      if (DYTools::study2D==0) { 
          hMassv[isam]->Fill(data->mass,weight);
          hMassv_weighted[isam]->Fill(data->mass,weight*extra_weight);
      } 
      if (fabs(data->eta_2) < 1.479 && fabs(data->eta_1) < 1.479) { 
          hMassv_bb[isam]->Fill(data->mass,weight);
          //hMassv_bb_r92[isam]->Fill(data->mass,weight);
      } else if (fabs(data->eta_2) > 1.479 && fabs(data->eta_1) > 1.479) { 
          hMassv_ee[isam]->Fill(data->mass,weight);
          //hMassv_ee_r92[isam]->Fill(data->mass,weight);
      } else {
          hMassv_oo[isam]->Fill(data->mass,weight);
          //hMassv_oo_r92[isam]->Fill(data->mass,weight);
      } 
/*
      if (fabs(data->eta_1) < 1.479) {
        if (passEGMID(data, "pt")) hscEtv_b[isam]->Fill(data->scEt_1,weight);
        if (passEGMID(data, "eta")) hscEtav_b[isam]->Fill(data->scEta_1,weight);
        if (passEGMID(data, "iso")) hrelPFIso03_b[isam]->Fill(data->relPFIso03_1/data->scEt_1,weight);
        if (passEGMID(data, "sigietaeta")) hsigiEtaiEta_b[isam]->Fill(data->sigiEtaiEta_1,weight);
        if (passEGMID(data, "deta")) hdeltaEtaIn_b[isam]->Fill(data->deltaEtaIn_1,weight);
        if (passEGMID(data, "dphi")) hdeltaPhiIn_b[isam]->Fill(data->deltaPhiIn_1,weight);
        if (passEGMID(data, "hovere")) hHoverE_b[isam]->Fill(data->HoverE_1,weight);
        if (passEGMID(data, "dxy")) hDXY_b[isam]->Fill(data->d0_1,weight);
        if (passEGMID(data, "dz")) hDZ_b[isam]->Fill(data->dz_1,weight);
        if (passEGMID(data, "ooeoop")) hOOEOOP_b[isam]->Fill(data->ooeoop_1,weight);
        if (passEGMID(data, "nexpectedh")) hExpHitsInner_b[isam]->Fill(data->nExpHitsInner_1,weight);
        hNPV_b[isam]->Fill(data->nPV,weight);
      } else {
        if (passEGMID(data, "pt")) hscEtv_e[isam]->Fill(data->scEt_1,weight);
        if (passEGMID(data, "eta")) hscEtav_e[isam]->Fill(data->scEta_1,weight);
        if (passEGMID(data, "iso")) hrelPFIso03_e[isam]->Fill(data->relPFIso03_1/data->scEt_1,weight);
        if (passEGMID(data, "sigietaeta")) hsigiEtaiEta_e[isam]->Fill(data->sigiEtaiEta_1,weight);
        if (passEGMID(data, "deta")) hdeltaEtaIn_e[isam]->Fill(data->deltaEtaIn_1,weight);
        if (passEGMID(data, "dphi")) hdeltaPhiIn_e[isam]->Fill(data->deltaPhiIn_1,weight);
        if (passEGMID(data, "hovere")) hHoverE_e[isam]->Fill(data->HoverE_1,weight);
        if (passEGMID(data, "dxy")) hDXY_e[isam]->Fill(data->d0_1,weight);
        if (passEGMID(data, "dz")) hDZ_e[isam]->Fill(data->dz_1,weight);
        if (passEGMID(data, "ooeoop")) hOOEOOP_e[isam]->Fill(data->ooeoop_1,weight);
        if (passEGMID(data, "nexpectedh")) hExpHitsInner_e[isam]->Fill(data->nExpHitsInner_1,weight);
        hNPV_e[isam]->Fill(data->nPV,weight);
      }

      if (fabs(data->eta_2) < 1.479) {
        if (passEGMID(data, "pt")) hscEtv_b[isam]->Fill(data->scEt_2,weight);
        if (passEGMID(data, "eta")) hscEtav_b[isam]->Fill(data->scEta_2,weight);
        if (passEGMID(data, "iso")) hrelPFIso03_b[isam]->Fill(data->relPFIso03_2/data->scEt_2,weight);
        if (passEGMID(data, "sigietaeta")) hsigiEtaiEta_b[isam]->Fill(data->sigiEtaiEta_2,weight);
        if (passEGMID(data, "deta")) hdeltaEtaIn_b[isam]->Fill(data->deltaEtaIn_2,weight);
        if (passEGMID(data, "dphi")) hdeltaPhiIn_b[isam]->Fill(data->deltaPhiIn_2,weight);
        if (passEGMID(data, "hovere")) hHoverE_b[isam]->Fill(data->HoverE_2,weight);
        if (passEGMID(data, "dxy")) hDXY_b[isam]->Fill(data->d0_2,weight);
        if (passEGMID(data, "dz")) hDZ_b[isam]->Fill(data->dz_2,weight);
        if (passEGMID(data, "ooeoop")) hOOEOOP_b[isam]->Fill(data->ooeoop_2,weight);
        if (passEGMID(data, "nexpectedh")) hExpHitsInner_b[isam]->Fill(data->nExpHitsInner_2,weight);
        hNPV_b[isam]->Fill(data->nPV,weight);
      } else {
        if (passEGMID(data, "pt")) hscEtv_e[isam]->Fill(data->scEt_2,weight);
        if (passEGMID(data, "eta")) hscEtav_e[isam]->Fill(data->scEta_2,weight);
        if (passEGMID(data, "iso")) hrelPFIso03_e[isam]->Fill(data->relPFIso03_2/data->scEt_2,weight);
        if (passEGMID(data, "sigietaeta")) hsigiEtaiEta_e[isam]->Fill(data->sigiEtaiEta_2,weight);
        if (passEGMID(data, "deta")) hdeltaEtaIn_e[isam]->Fill(data->deltaEtaIn_2,weight);
        if (passEGMID(data, "dphi")) hdeltaPhiIn_e[isam]->Fill(data->deltaPhiIn_2,weight);
        if (passEGMID(data, "hovere")) hHoverE_e[isam]->Fill(data->HoverE_2,weight);
        if (passEGMID(data, "dxy")) hDXY_e[isam]->Fill(data->d0_2,weight);
        if (passEGMID(data, "dz")) hDZ_e[isam]->Fill(data->dz_2,weight);
        if (passEGMID(data, "ooeoop")) hOOEOOP_e[isam]->Fill(data->ooeoop_2,weight);
        if (passEGMID(data, "nexpectedh")) hExpHitsInner_e[isam]->Fill(data->nExpHitsInner_2,weight);
        hNPV_e[isam]->Fill(data->nPV,weight);
      }
*/

      hMassBinsv[isam]->Fill(data->mass,weight);
      hZpeakv[isam]->Fill(data->mass,weight);

      nSelv[isam] += weight;
      nSelVarv[isam] += weight*weight;
      
    }
    delete infile;
    infile=0, eventTree=0;
  }


  //--------------------------------------------------------------------------------------------------------------
  // Make plots
  //==============================================================================================================

  // Buffers for labels and comments
  //char ylabel[100];   // y-axis label
  char lumitext[50];
  if(lumi>0) {
    if(lumi<1) { sprintf(lumitext,"#int#font[12]{L}dt = %.0f nb^{-1}",1000.*lumi); }
    if(lumi<1000) { sprintf(lumitext,"#int#font[12]{L}dt = %.0f nb^{-1}",1000.*lumi); }
    else       { sprintf(lumitext,"#int#font[12]{L}dt = %.3g fb^{-1}",lumi/1000.0); }
  }

  // Merge diboson histograms if needed
  int clone_idx=(hasData) ? 1:0;
  TH1F *hMassBinsDibosons = (TH1F*)hMassBinsv[clone_idx]->Clone("hMassBinsDibosons");
  TH1F *hMassDibosons = (TH1F*)hMassv[clone_idx]->Clone("hMassDibosons");
  hMassBinsDibosons->Reset();
  hMassDibosons->Reset();
  Int_t colorDibosons = 1;
  TString labelDibosons = "WW/WZ/ZZ";
  for(UInt_t isam=0; isam<samplev.size(); isam++) {
    if( snamev[isam] == "ww" || snamev[isam] == "wz" || snamev[isam] == "zz"){
      hMassDibosons->Add(hMassv[isam]);
      hMassBinsDibosons->Add(hMassBinsv[isam]);
      // Use color of the last diboson entry
      colorDibosons = samplev[isam]->color;
    }
  }

  // Additional normalization for MC:
  //
  // Ideally, we would normalize all MC samples to data luminosity
  // In practice, however, it is not easy because of two reasons:
  //  - the luminosity is known with an error (systematic shift of 6% is
  //       for example suspected in mid-2011)
  //  - data/MC scale factors for efficiency to select events may
  //       move normalization off by another 5%
  // Therefore, we normalize total MC to the data Z peak. This gives us
  // the scale factor that is applied to all samples. 
  // In the following calculation it is assumed that the first histogram
  // is data and the last is signal MC.  
  TH1F *totalMCMass = (TH1F*)hMassv[0]->Clone("totalMCMass");
  totalMCMass->Reset();
  for(UInt_t isam=(hasData)?1:0; isam<samplev.size(); isam++) {
    totalMCMass->Add(hMassv[isam]);
  }
  double massNormMin = 60.0;
  double massNormMax = 120.0;
  double dataOverMc = hMassv[0]->Integral(hMassv[0]->FindBin(massNormMin+0.001),
					  hMassv[0]->FindBin(massNormMax-0.001)) /
    totalMCMass->Integral(totalMCMass->FindBin(massNormMin+0.001),
			  totalMCMass->FindBin(massNormMax-0.001));
  printf("data to MC extra correction from Z peak normalization: %f\n",dataOverMc);

  double dataOverMcEachBin[DYTools::nMassBins+1];
  for (int i=0; i<DYTools::nMassBins; i++)
    {
      dataOverMcEachBin[i] = hMassv[0]->Integral(hMassv[0]->FindBin(DYTools::massBinLimits[i]+0.001),hMassv[0]->FindBin(DYTools::massBinLimits[i+1]-0.001)) /
	totalMCMass->Integral(totalMCMass->FindBin(DYTools::massBinLimits[i]+0.001),totalMCMass->FindBin(DYTools::massBinLimits[i+1]-0.001));
      printf("data to MC %i bin norm: %f\n",i,dataOverMcEachBin[i]);
    }

  // Rescale all MC samples. This is not totally proper for fake lepton
  // backgrounds, but ok for backgrounds with true leptons, and those are dominant
  for(UInt_t isam=1; isam<samplev.size(); isam++) {
    hMassv[isam]->Scale(dataOverMc);
    hMassv_weighted[isam]->Scale(dataOverMc);
    //hMassv_bb[isam]->Scale(dataOverMc);
    //hMassv_ee[isam]->Scale(dataOverMc);
    //hMassv_oo[isam]->Scale(dataOverMc);
    hMassBinsv[isam]->Scale(dataOverMc);
    printf("  MC %s IS RESCALED for plotting\n", snamev[isam].Data());
  }


  hMassDibosons->Scale(dataOverMc); printf(" compound MC %s IS RESCALED for plotting\n", hMassDibosons->GetName());
  hMassBinsDibosons->Scale(dataOverMc); printf("  compound MC %s IS RESCALED for plotting\n", hMassBinsDibosons->GetName());

  //
  // Prepare outputDir and the plot file
  //

  TString outputDirYields(outputDir.Data());
  if (performPUReweight) outputDirYields.ReplaceAll("selected_events","yields");
  else outputDirYields.ReplaceAll("selected_events","yields_noPU");
  gSystem->mkdir(outputDirYields,kTRUE);
  TString fNameOutYieldPlots(outputDirYields+TString("/yield_plots") + DYTools::analysisTag);
  fNameOutYieldPlots += ".root";
  TFile *fYieldPlots = new TFile( fNameOutYieldPlots, "recreate" );
  if (!fYieldPlots) {
    std::cout << "Failed to create a file <" << fNameOutYieldPlots << ">\n";
    throw 2;
  }

  //
  // Draw mass spectrum without rapidity binning
  //


  // First, draw the mass histograms with fine mass binning
  DrawMassPeak(hMassv, samplev, snamev, hMassDibosons, hasData, mergeDibosons, labelDibosons, colorDibosons, lumi, lumitext, 0, fYieldPlots);

  // Second, draw the mass histograms with the mass binning used in the analysis
  DrawMassPeak(hMassBinsv, samplev, snamev, hMassBinsDibosons, hasData, mergeDibosons, labelDibosons, colorDibosons, lumi, lumitext, 1, fYieldPlots);

  // Draw the flattened figure (Y histograms for different mass regions)
  DrawFlattened(yields, yieldsSumw2, samplev, snamev, hasData, mergeDibosons, labelDibosons, colorDibosons, lumi, lumitext, fYieldPlots);

  // Draw rapidity in mass slices 
  if (DYTools::study2D==1)
    {
       Draw6Canvases(yields, yieldsSumw2, samplev, snamev, hasData, dataOverMc, dataOverMcEachBin, 1, 0, fYieldPlots);
       Draw6Canvases(yields, yieldsSumw2, samplev, snamev, hasData, dataOverMc, dataOverMcEachBin, 1, 1, fYieldPlots);
       Draw6Canvases(yields, yieldsSumw2, samplev, snamev, hasData, dataOverMc, dataOverMcEachBin, 1,-1, fYieldPlots);
       Draw6Canvases(yields, yieldsSumw2, samplev, snamev, hasData, dataOverMc, dataOverMcEachBin, 0, 0, fYieldPlots);
       Draw6Canvases(yields, yieldsSumw2, samplev, snamev, hasData, dataOverMc, dataOverMcEachBin, 0, 1, fYieldPlots);
       Draw6Canvases(yields, yieldsSumw2, samplev, snamev, hasData, dataOverMc, dataOverMcEachBin, 0,-1, fYieldPlots);
    }

  fYieldPlots->Close();

  //--------------------------------------------------------------------------------------------------------------
  // Save the results
  //==============================================================================================================

  // Pack info into writable objects
  //TVectorD massBinLimits(nMassBins+1);
  //TVectorD rapidityBinning(nMassBins+1);
  //for(int i=0; i <= nMassBins; i++){
  //  massBinLimits(i) = DYTools::massBinLimits[i];
  //  rapidityBinning(i) = DYTools::nYBins[i];
  //}

  // This dummy object is only needed to convey the number
  // of samples considered. The method below is rather convoluted,
  // but I do not know a better one. Ideally, we would just store
  // a list of strings, each string containing the sample name.
  TVectorD dummySampleCount(sampleTags.size());
  dummySampleCount = 0;

  gSystem->mkdir(outputDirYields,kTRUE);
  TString fNameOutYields(outputDirYields+TString("/yields") + DYTools::analysisTag);
  fNameOutYields += ".root";
  TFile fYields( fNameOutYields, "recreate" );
  //massBinLimits      .Write("massBinLimits");
  //rapidityBinning    .Write("rapidityBinning");
  unfolding::writeBinningArrays(fYields);
  dummySampleCount   .Write("dummySampleCount");
  unfolding::writeBinningArrays(fYields);
  char objName[100];
  for(UInt_t isam = 0; isam < yields.size(); isam++){
    sprintf(objName,"sample_name_%i",isam);
    TObjString *sampleNameStorable = new TObjString( sampleTags.at(isam) );
    sampleNameStorable->Write(objName);
    sprintf(objName,"yields_%s",sampleTags.at(isam).Data());
    yields[isam]       ->Write(objName);
    sprintf(objName,"yieldsSumw2_%s",sampleTags.at(isam).Data());
    yieldsSumw2[isam]  ->Write(objName);
    hMassv[isam]->Write("peak_"+snamev[isam]);
    hMassv_weighted[isam]->Write("weighted_peak_"+snamev[isam]);
    hMassv_bb[isam]->Write("peak_bb_"+snamev[isam]);
    hMassv_ee[isam]->Write("peak_ee_"+snamev[isam]);
    hMassv_oo[isam]->Write("peak_oo_"+snamev[isam]);

    hscEtv_b[isam]->Write("scEt_b_"+snamev[isam]);
    hscEtav_b[isam]->Write("scEta_b_"+snamev[isam]);
    hrelPFIso03_b[isam]->Write("relPFIso03_b_"+snamev[isam]);
    hsigiEtaiEta_b[isam]->Write("sigiEtaiEta_b_"+snamev[isam]);
    hdeltaEtaIn_b[isam]->Write("deltaEtaIn_b_"+snamev[isam]);
    hdeltaPhiIn_b[isam]->Write("deltaPhiIn_b_"+snamev[isam]);
    hHoverE_b[isam]->Write("HoverE_b_"+snamev[isam]);
    hDXY_b[isam]->Write("dxy_b_"+snamev[isam]);
    hDZ_b[isam]->Write("dz_b_"+snamev[isam]);
    hOOEOOP_b[isam]->Write("OOEOOP_b_"+snamev[isam]);
    hExpHitsInner_b[isam]->Write("ExpHitsInner_b_"+snamev[isam]);
    hNPV_b[isam]->Write("NPV_b_"+snamev[isam]);

    hscEtv_e[isam]->Write("scEt_e_"+snamev[isam]);
    hscEtav_e[isam]->Write("scEta_e_"+snamev[isam]);
    hrelPFIso03_e[isam]->Write("relPFIso03_e_"+snamev[isam]);
    hsigiEtaiEta_e[isam]->Write("sigiEtaiEta_e_"+snamev[isam]);
    hdeltaEtaIn_e[isam]->Write("deltaEtaIn_e_"+snamev[isam]);
    hdeltaPhiIn_e[isam]->Write("deltaPhiIn_e_"+snamev[isam]);
    hHoverE_e[isam]->Write("HoverE_e_"+snamev[isam]);
    hDXY_e[isam]->Write("dxy_e_"+snamev[isam]);
    hDZ_e[isam]->Write("dz_e_"+snamev[isam]);
    hOOEOOP_e[isam]->Write("OOEOOP_e_"+snamev[isam]);
    hExpHitsInner_e[isam]->Write("ExpHitsInner_e_"+snamev[isam]);
    hNPV_e[isam]->Write("NPV_e_"+snamev[isam]);


  }
  fYields.Close();
  
  /*
  // Save mass histograms into a separate file for a direct comparison 
  // with DrellYan(1D) package
  TString fNameOutHists(outputDirYields+"/massHist");
  fNameOutHists += DYTools::analysisTag + TString(".root");
  TFile fMassHists(fNameOutHists,"recreate");
  for(UInt_t isam=0; isam<samplev.size(); isam++) {
    hMassBinsv[isam]->Write(snamev[isam]);
  }
  std::cout << "file <" << fNameOutHists << "> created\n";
  fMassHists.Close();
  */



  //--------------------------------------------------------------------------------------------------------------
  // Summary print out
  //==============================================================================================================
  cout << endl;
  cout << "*" << endl;
  cout << "* SUMMARY" << endl;
  cout << "*--------------------------------------------------" << endl;
  cout << endl;
  
  if(hasData) {
    cout << "   Data: " << setprecision(1) << fixed << nSelv[0] << " Drell-Yan events!" << endl;
    for(UInt_t ifile=0; ifile<samplev[0]->fnamev.size(); ifile++)
      cout << "     " << samplev[0]->fnamev[ifile] << endl;
  }
  cout << endl;
  if(samplev.size()>1) {
    cout << "   MC:" << endl;
    for(UInt_t isam=1; isam<samplev.size(); isam++) {
      cout << "      " << snamev[isam] << setw(8) << setprecision(3) << fixed << nSelv[isam] << " +/- ";
      cout << setw(4) << setprecision(3) << fixed << sqrt(nSelVarv[isam]) << endl;
      for(UInt_t ifile=0; ifile<samplev[isam]->fnamev.size(); ifile++) {
        cout << "         " << samplev[isam]->fnamev[ifile] << endl;
      }
    }
    cout << endl;
  }
  cout << endl;

  //
  // Summary printout in mass bins, integrated over rapidity
  //
  // Add yields over rapidity bins
  double totalData            [DYTools::nMassBins];
  double totalSignalMC        [DYTools::nMassBins];
  double totalSignalMCError   [DYTools::nMassBins];
  double totalBg     [DYTools::nMassBins];
  double totalBgError[DYTools::nMassBins];

  for( int im=0; im<DYTools::nMassBins; im++){
    totalData         [im] = 0;
    totalSignalMC     [im] = 0;
    totalSignalMCError[im] = 0;
    totalBg           [im] = 0;
    totalBgError      [im] = 0;
    for(int iy = 0; iy < DYTools::nYBins[im]; iy++){
      for( UInt_t isam = 0; isam < yields.size(); isam++){
	if( sampleTags.at(isam) == TString("data") ){
	  totalData[im] += (*yields.at(isam))(im,iy);
	}else if ( sampleTags.at(isam) == TString("zee") ){
	  totalSignalMC[im] += (*yields.at(isam))(im,iy);
	  totalSignalMCError[im] += (*yieldsSumw2.at(isam))(im,iy);
	}else{
	  // what remains are background samples
	  totalBg[im] += (*yields.at(isam))(im,iy);
	  totalBgError[im] += (*yieldsSumw2.at(isam))(im,iy);
	}
      } // end loop over samples
    } // end loop over rapidity bins
    totalBgError[im] = sqrt( totalBgError[im] );
    totalSignalMCError[im] = sqrt( totalSignalMCError[im] );
  } // end loop over mass bins

  printf("Printout of the data, MC signal and MC backgrounds integrated over rapidity\n");
  printf("     mass bin        data      MC-signal     MC-backgr\n");
  for(int im = 0; im < DYTools::nMassBins; im++){
    printf("%5.0f-%5.0f GeV: ", DYTools::massBinLimits[im],
	   DYTools::massBinLimits[im+1]);
    printf(" %7.0f", totalData[im]);
    printf(" %9.1f+-%6.1f", totalSignalMC[im], totalSignalMCError[im]);
    printf(" %7.2f+-%5.2f", totalBg[im], totalBgError[im]);
    printf("\n");
  }
  printf("Note: these MC numbers are not rescaled!\n");

  if (1) {
  // A different view of background table
  printf("\n\nPrintout of the backgrounds for all mass bins, view II\n");

  printf("            ");
  for(UInt_t isam=0; isam<samplev.size(); isam++) {
    printf(" %14s ",snamev[isam].Data());
  }
  printf("           total          fraction\n");
  for(int ibin=0; ibin<DYTools::nMassBins; ibin++){
    printf("%5.1f-%5.1f GeV: ",
           hMassBinsv[0]->GetXaxis()->GetBinLowEdge(ibin+1),
           hMassBinsv[0]->GetXaxis()->GetBinUpEdge(ibin+1));
    // Data:
    printf(" %7.0f+-%5.0f ",hMassBinsv[0]->GetBinContent(ibin+1),hMassBinsv[0]->GetBinError(ibin+1) );
    // Individual MC samples
    double total=0., totalError=0.;
    for(UInt_t isam=1; isam<samplev.size(); isam++) {
      double thisContent = hMassBinsv[isam]->GetBinContent(ibin+1);
      double thisError = hMassBinsv[isam]->GetBinError(ibin+1);
      printf(" %7.2f+-%5.2f ",thisContent, thisError);
      if ( (isam!=0) && (snamev[isam]!=TString("zee"))) {
	total+= thisContent;
	totalError+=thisError*thisError;
      }
    }
    totalError = sqrt(totalError);
    // Total
    printf("  %8.2f+-%6.2f",total, totalError);
    printf("  %5.1f\n",100*total/hMassBinsv[0]->GetBinContent(ibin+1));
  }
  }


  latexPrintoutBkgSources(  snamev, samplev, hMassBinsv);
/*
  if (1 && hZpeakv.size()) {
    std::cout << "Zpeak region average\n";
    unsigned int idx=(hasData) ? 1 : 0;
    TH1F *hTotMC=  (TH1F*)hZpeakv[idx]->Clone("hZpeak_all");
    hTotMC->Reset();
    hTotMC->SetDirectory(0);
    TH1F *hZee=NULL;
    for (unsigned int i=0; i<hZpeakv.size(); ++i) {
      printf(" mean in <%s> is %6.4lf pm %6.4lf\n",hZpeakv[i]->GetName(),hZpeakv[i]->GetMean(),hZpeakv[i]->GetMeanError());
      if (i>=idx) hTotMC->Add(hZpeakv[i]);
      if (snamev[i] == "zee") {
	hZee=(TH1F*)hZpeakv[i]->Clone("hZpeak_zee_clone");
	hZee->SetDirectory(0);
      }
    }
    printf(" mean in <%s> is %6.4lf pm %6.4lf\n",hTotMC->GetName(),hTotMC->GetMean(),hTotMC->GetMeanError());



    if (!hZee) std::cout << "failed to identify Zee\n";
    else if (!hasData) std::cout << "there no experimental data points available\n";
    else {
      TH1F* hData=(TH1F*) hZpeakv[0]->Clone("hZpeak_data_clone");
      hData->SetDirectory(0);

      TCanvas *cZp=MakeCanvas("cZpeak","", 600,700);
      ComparisonPlot_t cp(ComparisonPlot_t::_ratioPlain,"compPlot","",
			  "mass [GeV]","counts", "MC/data");
      std::cout << "hZee normalization factor=" << (hData->Integral()/hZee->Integral()) << "\n";
      std::cout << "hTotMC normalization factor=" << (hData->Integral()/hTotMC->Integral()) << "\n";
      hZee->Scale( hData->Integral()/hZee->Integral() );
      hTotMC->Scale( hData->Integral()/hTotMC->Integral() );
      cp.Prepare2Pads(cZp);
      //cp.SetLogy();
      cp.SetRatioYRange(0.8,1.1);
      cp.AddHist1D(hData,"data", "LPE", kBlack, 1,0,1);
      cp.AddHist1D(hZee,"MC (norm)", "hist", kBlue, 1,0,1);
      cp.AddHist1D(hTotMC,"tot MC (norm)","hist",kRed, 1,0,1);
      cp.Draw(cZp,false,"png");
      SaveCanvas(cZp,cZp->GetName());
    }
  }

*/

  gBenchmark->Show("prepareYields");      
}


void latexPrintoutBkgSources(  vector<TString>  snamev, vector<CSample*> samplev, vector<TH1F*>    hMassBinsv)
{
   TString path;
   if (DYTools::study2D==0) path="tables1D/";
   else path="tables2D/";
   gSystem->mkdir(path,kTRUE);
   TString txtFileName=path + "bkgSources.txt";
  FILE* txtFile = fopen(txtFileName,"w");
  TString str=DayAndTimeTag();
  fprintf(txtFile,"%50s",str.Data());
  fprintf(txtFile,"\n\nPrintout of the backgrounds for all mass bins\n");
  fprintf(txtFile,"            ");
  for(UInt_t isam=0; isam<samplev.size(); isam++) {
    if ( (isam!=0) && (snamev[isam]!=TString("zee"))) fprintf(txtFile," %14s ",snamev[isam].Data());
  }
  fprintf(txtFile,"   total  \n");
  for(int ibin=0; ibin<DYTools::nMassBins; ibin++){
    fprintf(txtFile,"%5.0f-%5.0f & ",
           hMassBinsv[0]->GetXaxis()->GetBinLowEdge(ibin+1),
           hMassBinsv[0]->GetXaxis()->GetBinUpEdge(ibin+1));
     // Individual MC samples
    double total=0., totalError=0.;
    for(UInt_t isam=1; isam<samplev.size(); isam++) {
      double thisContent = hMassBinsv[isam]->GetBinContent(ibin+1);
      double thisError = hMassBinsv[isam]->GetBinError(ibin+1);
      if ( (isam!=0) && (snamev[isam]!=TString("zee"))) {
        fprintf(txtFile," $%7.0f\\pm%5.0f$ & ",thisContent, thisError);
	total+= thisContent;
	totalError+=thisError*thisError;
      }
    }
    totalError = sqrt(totalError);
    // Total
    fprintf(txtFile,"  $%8.0f\\pm%6.0f$ & \\\\\n",total, totalError);
  }

 fclose (txtFile);

}

//FIXME
// The primary implementation for dielectron object of the ID follows
Bool_t passEGMID(ZeeData_t *data, TString label)
{

  if (label != "pt") {
     if( data->scEt_1 < 20 || data->scEt_2 < 10 ) {
          if( data->scEt_2 < 20 || data->scEt_1 < 10 ) return kFALSE;
          }
    }

  if (label != "eta") {
       if( fabs(data->scEta_1) > 2.4 ) return kFALSE;
       if( fabs(data->scEta_2) > 2.4 ) return kFALSE;
     }

  if (label != "dxy") {
        if(fabs(data->d0_1) > 0.02 )  return kFALSE;
        if(fabs(data->d0_1) > 0.02 )  return kFALSE;
  }
  if (label != "dz") {
        if(fabs(data->dz_1) > 0.1 )  return kFALSE;
        if(fabs(data->dz_1) > 0.1 )  return kFALSE;
  }
  
  double invEMinusInvP_1 = data->ooeoop_1;
  double invEMinusInvP_2 = data->ooeoop_2;
  if (label != "ooeoop") {
        if (invEMinusInvP_1 > 0.05) return kFALSE;
        if (invEMinusInvP_2 > 0.05) return kFALSE;
  }

  // Compute PF isolation:
  // Add energy in the rings with dR 0.0-0.1, 0.1-0.2, 0.2-0.3 
  // to get the total deposits in the cone 0.3.
  double relPFIso03_1 = data->relPFIso03_1/data->pt_1;
  double relPFIso03_2 = data->relPFIso03_2/data->pt_2;
  
  // barrel/endcap dependent requirments      
  if(fabs(data->scEta_1)<1.479) {
    // barrel
    if( label != "iso" && relPFIso03_1 > 0.15) return kFALSE;
    
    if(label != "deta" && fabs(data->deltaEtaIn_1) >= 0.04 )  return kFALSE;
    if(label != "dphi" &&fabs(data->deltaPhiIn_1) >= 0.06 )  return kFALSE;
    if(label != "sigietaeta" && data->sigiEtaiEta_1	  > 0.01)  return kFALSE;
    if(label != "hovere" && data->HoverE_1	          > 0.12)  return kFALSE;
      
  } else {
    // endcap

    if(data->pt_1>20) {
      if( label != "iso" && relPFIso03_1 >  0.15) return kFALSE;
    } else {
      if( label != "iso" && relPFIso03_1 >  0.1) return kFALSE;
    }
    
    if(label != "deta" && fabs(data->deltaEtaIn_1) > 0.007)  return kFALSE;
    if(label != "dphi" &&fabs(data->deltaPhiIn_1) > 0.03)  return kFALSE;
    if(label != "sigietaeta" && data->sigiEtaiEta_1 > 0.03)  return kFALSE;
    if(label != "hovere" && data->HoverE_1	     > 0.1)  return kFALSE;

  }
  
  // barrel/endcap dependent requirments for the second electron
  if(fabs(data->scEta_2)<1.479) {
    // barrel
    if( label != "iso" && relPFIso03_2 > 0.15 ) return kFALSE;
    
    if(label != "deta" && fabs(data->deltaEtaIn_2) > 0.04)  return kFALSE;
    if(label != "dphi" &&fabs(data->deltaPhiIn_2) > 0.06)  return kFALSE;
    if(label != "sigietaeta" && data->sigiEtaiEta_2	  > 0.01)  return kFALSE;
    if(label != "hovere" && data->HoverE_2	          > 0.12)  return kFALSE;
      
  } else {
    // endcap

    if(data->pt_2>20) {
      if( label != "iso" && relPFIso03_2 > 0.15) return kFALSE;
    } else {
      if( label != "iso" && relPFIso03_2 >  0.1) return kFALSE;
    }
    
    if(label != "deta" && fabs(data->deltaEtaIn_2) > 0.007)  return kFALSE;
    if(label != "dphi" &&fabs(data->deltaPhiIn_2) > 0.03)  return kFALSE;
    if(label != "sigietaeta" && data->sigiEtaiEta_2	  > 0.03)  return kFALSE;
    if(label != "hovere" && data->HoverE_2	          > 0.1)  return kFALSE;

  }
 
  return kTRUE;
}
