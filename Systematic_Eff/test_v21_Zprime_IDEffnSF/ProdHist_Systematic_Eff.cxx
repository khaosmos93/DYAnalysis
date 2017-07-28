#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>

#include <vector>

// -- for Rochester Muon momentum correction -- //
// #include <Include/rochcor80x_070616v2/RoccoR.cc>
// #include <Include/rochcor80x_070616v2/rochcor2016.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

class HistContainer
{
public:
  TString Type;

  vector< TH1D* > vec_Hist;

  /* KP's
    TH1D* h_mass;
    TH1D* h_mass_BB;
    TH1D* h_mass_BEEE;

    TH1D* h_RatioPt;
    TH1D* h_RatioPt_M400to2500;

    TH1D* h_dPtOverPt;
    TH1D* h_dPtOverPt_M400to2500;

    TH1D* h_mass_PtRatio3toInf;
    TH1D* h_mass_PtRatio4toInf;
    TH1D* h_mass_PtRatio5toInf;

    TH1D* h_mass_PtRatio3toInf_M400to2500;
    TH1D* h_mass_PtRatio4toInf_M400to2500;
    TH1D* h_mass_PtRatio5toInf_M400to2500;
  */

  TH1D* h_Pt;
  TH1D* h_Pt_B;  // |eta| <= 1.2
  TH1D* h_Pt_E;  // |eta| > 1.2

  TH1D* h_Eta;
  TH1D* h_Eta_B;  // |eta| <= 1.2
  TH1D* h_Eta_E;  // |eta| > 1.2

  TH1D* h_Phi;
  TH1D* h_Phi_B;  // |eta| <= 1.2
  TH1D* h_Phi_E;  // |eta| > 1.2

  TH1D* h_mass; // mass of muon pair passing selection, fill only if both muon passe

  HistContainer( TString _Type )
  {
    Type = _Type;
    this->Set_Histograms();
    TH1::AddDirectory(kFALSE);
  }

  void Set_Histograms()
  {
    /* KP's
      this->h_mass = new TH1D("h_mass_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass );
      this->h_mass_BB = new TH1D("h_mass_BB_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_BB );
      this->h_mass_BEEE = new TH1D("h_mass_BEEE_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_BEEE );
      this->h_RatioPt = new TH1D("h_RatioPt_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_RatioPt );
      this->h_RatioPt_M400to2500 = new TH1D("h_RatioPt_M400to2500_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_RatioPt_M400to2500 );

      this->h_dPtOverPt = new TH1D("h_dPtOverPt_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_dPtOverPt );
      this->h_dPtOverPt_M400to2500 = new TH1D("h_dPtOverPt_M400to2500_"+this->Type, "", 10000, 0, 100); vec_Hist.push_back( this->h_dPtOverPt_M400to2500 );

      this->h_mass_PtRatio3toInf = new TH1D("h_mass_PtRatio3toInf_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_PtRatio3toInf );
      this->h_mass_PtRatio4toInf = new TH1D("h_mass_PtRatio4toInf_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_PtRatio4toInf );
      this->h_mass_PtRatio5toInf = new TH1D("h_mass_PtRatio5toInf_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_PtRatio5toInf );

      this->h_mass_PtRatio3toInf_M400to2500 = new TH1D("h_mass_PtRatio3toInf_M400to2500_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_PtRatio3toInf_M400to2500 );
      this->h_mass_PtRatio4toInf_M400to2500 = new TH1D("h_mass_PtRatio4toInf_M400to2500_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_PtRatio4toInf_M400to2500 );
      this->h_mass_PtRatio5toInf_M400to2500 = new TH1D("h_mass_PtRatio5toInf_M400to2500_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass_PtRatio5toInf_M400to2500 );
    */

    this->h_Pt = new TH1D("h_Pt_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt );
    this->h_Pt_B = new TH1D("h_Pt_B_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_B );
    this->h_Pt_E = new TH1D("h_Pt_E_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_E );

    this->h_Eta = new TH1D("h_Eta_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta );
    this->h_Eta_B = new TH1D("h_Eta_B_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta_B );
    this->h_Eta_E = new TH1D("h_Eta_E_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta_E );

    this->h_Phi = new TH1D("h_Phi_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi );
    this->h_Phi_B = new TH1D("h_Phi_B_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi_B );
    this->h_Phi_E = new TH1D("h_Phi_E_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi_E );

    this->h_mass = new TH1D("h_mass_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass );

    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++)
      vec_Hist[i]->Sumw2();
  }

  void Fill( Muon mu1, Bool_t pass1, Muon mu2, Bool_t pass2, Double_t weight )
  {
    Double_t M = (mu1.Momentum + mu2.Momentum).M();

    // -- Mass -- //
    if (pass1 && pass2) h_mass->Fill( M, weight );

    // -- pT, Eta, Phi -- //
    if (pass1) {      // Fill muon1
      h_Pt->Fill( mu1.Pt, weight );
      h_Eta->Fill( mu1.eta, weight );
      h_Phi->Fill( mu1.phi, weight );
      if (fabs(mu1.eta) <= 1.2) {
        h_Pt_B->Fill( mu1.Pt, weight );
        h_Eta_B->Fill( mu1.eta, weight );
        h_Phi_B->Fill( mu1.phi, weight );
      }
      else if (fabs(mu1.eta) > 1.2) {
        h_Pt_E->Fill( mu1.Pt, weight );
        h_Eta_E->Fill( mu1.eta, weight );
        h_Phi_E->Fill( mu1.phi, weight );
      }
    }

    if (pass2) {      // Fill muon2
      h_Pt->Fill( mu2.Pt, weight );
      h_Eta->Fill( mu2.eta, weight );
      h_Phi->Fill( mu2.phi, weight );
      if (fabs(mu2.eta) <= 1.2) {
        h_Pt_B->Fill( mu2.Pt, weight );
        h_Eta_B->Fill( mu2.eta, weight );
        h_Phi_B->Fill( mu2.phi, weight );
      }
      else if (fabs(mu2.eta) > 1.2) {
        h_Pt_E->Fill( mu2.Pt, weight );
        h_Eta_E->Fill( mu2.eta, weight );
        h_Phi_E->Fill( mu2.phi, weight );
      }
    }

    /* KP's
      if( fabs(mu1.eta) < 0.9 && fabs(mu2.eta) < 0.9 ) // -- BB event -- //
        h_mass_BB->Fill( M, weight );
      else // -- BE + EE event -- //
        h_mass_BEEE->Fill( M, weight );

      // -- ratio = leading pT / sub-leading pT -- //
      Double_t Pt_lead = 0;
      Double_t Pt_sub = 0;
      if( mu1.Pt > mu2.Pt )
      {
        Pt_lead = mu1.Pt;
        Pt_sub = mu2.Pt;
      }
      else
      {
        Pt_lead = mu2.Pt;
        Pt_sub = mu1.Pt;
      }

      Double_t RatioPt = Pt_lead / Pt_sub;
      Double_t dPtOverPt = fabs( Pt_lead - Pt_sub ) / Pt_sub;

      h_RatioPt->Fill( RatioPt, weight );
      h_dPtOverPt->Fill( dPtOverPt, weight );

      if( RatioPt > 3 )
        h_mass_PtRatio3toInf->Fill( M, weight );

      if( RatioPt > 4 )
        h_mass_PtRatio4toInf->Fill( M, weight );

      if( RatioPt > 5 )
        h_mass_PtRatio5toInf->Fill( M, weight );

      if( M > 400 && M < 2500 )
      {
        h_RatioPt_M400to2500->Fill( RatioPt, weight );
        h_dPtOverPt_M400to2500->Fill( dPtOverPt, weight );

        if( RatioPt > 3 )
          h_mass_PtRatio3toInf_M400to2500->Fill( M, weight );

        if( RatioPt > 4 )
          h_mass_PtRatio4toInf_M400to2500->Fill( M, weight );

        if( RatioPt > 5 )
          h_mass_PtRatio5toInf_M400to2500->Fill( M, weight );
      }
    */
  }

  void Save( TFile *f_output )
  {
    f_output->cd();

    TString FileName = f_output->GetName();
    printf( "[Save histograms for %s in %s]\n", Type.Data(), FileName.Data() );
    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++)
    {
      TString HistName = vec_Hist[i]->GetName();
      vec_Hist[i]->Write();
      printf("\t[%s is saved]\n", HistName.Data() );
    }
    printf("\n");
  }

protected:
};

class HistogramProducer
{
public:
  Int_t isMC;
  Double_t NormFactor;
  TString FileName_ROOTFileList;
  TString Tag;

  HistogramProducer( Int_t _isMC, TString _FileName_ROOTFileList, Double_t _NormFactor)
  {
    this->isMC = _isMC;
    this->FileName_ROOTFileList = _FileName_ROOTFileList;
    this->NormFactor = _NormFactor;
    this->Tag = this->GetTag( this->FileName_ROOTFileList );

    if( this->Tag.Contains("DYPowheg") ) this->Tag = "DYPowheg";

    printf("===============[HistogramProducer]===============\n");
    printf("[isMC = %d] Tag = %s, NormFactor = %lf\n", this->isMC, this->Tag.Data(), this->NormFactor);
    printf("=================================================\n");
  }

  void Producer()
  {
    TStopwatch totaltime;
    totaltime.Start();

    DYAnalyzer *analyzer = new DYAnalyzer( "Mu50_OR_TkMu50_Zprime" );

    // -- make chain -- //
    TChain *chain = new TChain("recoTree/DYTree");
    analyzer->MakeTChain_fromTextFile( chain, FileName_ROOTFileList );

    // -- turn-on ntuple -- //
    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_Muon();
    if( this->isMC )
      ntuple->TurnOnBranches_GenLepton();
    ntuple->Ready();

    Bool_t Flag_noBadMuons = kTRUE;
    if( !this->isMC ) // -- data, reminiAOD -- //
    {
      chain->SetBranchStatus("Flag_noBadMuons", 1);
      chain->SetBranchAddress("Flag_noBadMuons", &Flag_noBadMuons);
    }
    else
      Flag_noBadMuons = kTRUE; // -- MC: always true -- //

    HistContainer *Hist_DEN = new HistContainer( "DEN" );
    HistContainer *Hist_NUM = new HistContainer( "NUM" );

    // -- only for the data -- //
    HistContainer *Hist_DEN_RunBtoF = new HistContainer( "DEN_RunBtoF" );
    HistContainer *Hist_NUM_RunBtoF = new HistContainer( "NUM_RunBtoF" );

    HistContainer *Hist_DEN_RunGtoH = new HistContainer( "DEN_RunGtoH" );
    HistContainer *Hist_NUM_RunGtoH = new HistContainer( "NUM_RunGtoH" );

    Int_t nTotEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nTotEvent << "]" << endl;

    // for(Int_t i=0; i<10000; i++)
    for(Int_t i=0; i<nTotEvent; i++)
    {
      this->loadBar(i+1, nTotEvent, 100, 100);

      ntuple->GetEvent(i);

      // cout << i << "th event, Flag_noBadMuons: " << Flag_noBadMuons << endl;

      //Bring weights for NLO MC events
      Double_t GenWeight;
      ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

      Double_t TotWeight = this->NormFactor*GenWeight;

      Bool_t GenFlag = kFALSE;
      GenFlag = analyzer->SeparateDYLLSample_isHardProcess(this->Tag, ntuple); // -- for DY->tautau process -- //

      if( GenFlag == kTRUE )
      {
        ////////////////////////////////
        // -- reco-level selection -- //
        ////////////////////////////////
        if( ntuple->isTriggered( analyzer->HLT ) && Flag_noBadMuons )
        {
          vector< Muon > MuonCollection;
          Int_t NLeptons = ntuple->nMuon;
          for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
          {
            Muon mu;
            mu.FillFromNtuple(ntuple, i_reco);
            mu.ConvertMomentum_TuneP();

            // // -- Apply Rochester momentum scale correction -- //
            // // if( isCorrected == kTRUE )
            // {
            //  float qter = 1.0;

            //  if( this->SampleName == "Data" )
            //    rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
            //  else
            //    rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

            //  // -- Change Muon pT, eta and phi with updated(corrected) one -- //
            //  mu.Pt = mu.Momentum.Pt();
            //  mu.eta = mu.Momentum.Eta();
            //  mu.phi = mu.Momentum.Phi();
            // }

            MuonCollection.push_back( mu );
          }

          MuPair SelectedPair_DEN;
          pair<Bool_t,Bool_t> SelectedPairPass_DEN;
          Bool_t IsSelected_DEN = this->EventSelection_Zprime_DEN( ntuple, analyzer, MuonCollection, SelectedPair_DEN, SelectedPairPass_DEN );

          if( IsSelected_DEN )
          {
            Muon mu1 = SelectedPair_DEN.First;
            Muon mu2 = SelectedPair_DEN.Second;
            Bool_t pass1 = SelectedPairPass_DEN.first;
            Bool_t pass2 = SelectedPairPass_DEN.second;
            Hist_DEN->Fill( mu1, pass1, mu2, pass2, TotWeight);

            if( !isMC ) // -- data -- //
            {
              if( ntuple->runNum <= 278808 ) // -- RunBtoF -- //
                Hist_DEN_RunBtoF->Fill( mu1, pass1, mu2, pass2, TotWeight );
              else
                Hist_DEN_RunGtoH->Fill( mu1, pass1, mu2, pass2, TotWeight );
            }
          }

          MuPair SelectedPair_NUM;
          pair<Bool_t,Bool_t> SelectedPairPass_NUM;
          Bool_t IsSelected_NUM = this->EventSelection_Zprime_NUM( ntuple, analyzer, MuonCollection, SelectedPair_NUM, SelectedPairPass_NUM );

          if( IsSelected_NUM )
          {
            Muon mu1 = SelectedPair_NUM.First;
            Muon mu2 = SelectedPair_NUM.Second;
            Bool_t pass1 = SelectedPairPass_NUM.first;
            Bool_t pass2 = SelectedPairPass_NUM.second;
            Hist_NUM->Fill( mu1, pass1, mu2, pass2, TotWeight);

            if( !isMC ) // -- data -- //
            {
              if( ntuple->runNum <= 278808 ) // -- RunBtoF -- //
                Hist_NUM_RunBtoF->Fill( mu1, pass1, mu2, pass2, TotWeight);
              else
                Hist_NUM_RunGtoH->Fill( mu1, pass1, mu2, pass2, TotWeight);
            }
          }

        } // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

      } // -- end of if( GenFlag == kTRUE ) -- //

    } // -- end of event iteration -- //

    TFile *f_output = TFile::Open("ROOTFile_Output_Systematic_Eff_"+this->Tag+".root", "RECREATE");

    Hist_DEN->Save( f_output );
    Hist_NUM->Save( f_output );

    if( !isMC )
    {
      Hist_DEN_RunBtoF->Save( f_output );
      Hist_NUM_RunBtoF->Save( f_output );

      Hist_DEN_RunGtoH->Save( f_output );
      Hist_NUM_RunGtoH->Save( f_output );
    }

    Double_t TotalRunTime = totaltime.CpuTime();
    cout << "\tTotal RunTime(" << this->Tag << "): " << TotalRunTime << " seconds\n" << endl;

    printf("============================\nProducer() is finished\n============================\n\n");
  }

protected:
  TString GetTag( TString _FileName_ROOTFileList )
  {
    TObjArray *split = _FileName_ROOTFileList.Tokenize("-");
    return ((TObjString*)(split->At(1)))->String();
  }

  Bool_t isMuon_DEN( Muon mu )
  {
    if(    mu.isGLB == 1
        && mu.isTRK == 1
        && mu.TuneP_pT > 53
        && fabs(mu.eta) < 2.4
        //&& (mu.AbsTrkIso / mu.Inner_pT) < 0.1
        //&& fabs(mu.dB) < 0.2
      )
    {
      return 1;
    }
    return 0;
  }

  Bool_t isMuon_NUM( Muon mu )
  {
    if(    mu.isGLB == 1
        && mu.isTRK == 1
        && mu.TuneP_pT > 53
        && fabs(mu.eta) < 2.4

        && fabs(mu.dB) < 0.2
        && mu.trackerLayersGLB > 5
        && mu.pixelHitsGLB > 0
        && mu.muonHits > 0
        && (  mu.nMatches > 1
          || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
          || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
        && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3
        //&& (mu.AbsTrkIso / mu.Inner_pT) < 0.1
      )
    {
      return 1;
    }
    return 0;
  }

  Bool_t EventSelection_Zprime_DEN( NtupleHandle* ntuple, DYAnalyzer* analyzer, vector<Muon> &MuonCollection, MuPair &SelectedPair, pair<Bool_t,Bool_t> SelectedPairPass )
  {
    Bool_t isPassEventSelection = kFALSE;

    vector< Muon > QMuonCollection;
    for(Int_t j=0; j<(int)MuonCollection.size(); j++)
    {
      if( this->isMuon_DEN( MuonCollection[j] ) )
        QMuonCollection.push_back( MuonCollection[j] );
    }

    Int_t nQMuons = (Int_t)QMuonCollection.size();
    if( nQMuons >= 2 ) // -- at least 2 muons passing ID -- //
    {
      vector< MuPair > vec_GoodPair;
      for(Int_t i_mu=0; i_mu<nQMuons-1; i_mu++)
      {
        for(Int_t j_mu=i_mu+1; j_mu<nQMuons; j_mu++)
        {
          Muon mu1 = QMuonCollection[i_mu];
          Muon mu2 = QMuonCollection[j_mu];

          MuPair pair_temp( mu1, mu2 );

          Bool_t Flag_Mass = pair_temp.M > 50 ? kTRUE : kFALSE;

          // Bool_t Flag_TrigMatched = kFALSE;
          // if( this->TriggerMatching( analyzer->HLT, ntuple, pair_temp.First) ||
          //   this->TriggerMatching( analyzer->HLT, ntuple, pair_temp.Second) )
          //   Flag_TrigMatched = kTRUE;

          Bool_t Flag_Acc = pair_temp.isWithinAcc( 53, 53, 2.4, 2.4 );

          Bool_t Flag_3DAngle = pair_temp.Angle3D < TMath::Pi()-0.02 ? kTRUE : kFALSE;

          pair_temp.Calc_CommonVertexVariable_TuneP( ntuple );
          Bool_t Flag_Vtx = pair_temp.NormVtxChi2 < 20 ? kTRUE : kFALSE;

          Bool_t Flag_OS = pair_temp.isOS;

          Bool_t Flag_PtRatio = kFALSE;
          if( pair_temp.First.Pt / pair_temp.Second.Pt < 3 ) Flag_PtRatio = kTRUE;

          Bool_t Flag_B2B = kFALSE;
          Double_t pi = 3.141592; Double_t dPhim = 0.7*pi; Double_t dPhiM = 1.3*pi;
          Double_t dPhi = fabs(pair_temp.First.phi - pair_temp.Second.phi);
          if ( (dPhi < dPhiM) && (dPhi > dPhim) ) Flag_B2B = kTRUE;

          if( Flag_Mass &&
            // Flag_TrigMatched &&
            Flag_Acc &&
            Flag_3DAngle &&
            Flag_Vtx &&
            Flag_OS &&
            Flag_PtRatio &&
            Flag_B2B )
            vec_GoodPair.push_back( pair_temp );

        } // -- end of second muon iteration -- //
      } // -- end of first muon iteration -- //

      Int_t nPair = (Int_t)vec_GoodPair.size();
      if( nPair >= 1 ) // -- at least one pair pass all good pair condition -- //
      {
        // -- the pair with "largest" dimuon pT will be the first element -- //
        std::sort( vec_GoodPair.begin(), vec_GoodPair.end(), ComparePair_DimuonPt );
        isPassEventSelection = kTRUE;
        SelectedPair = vec_GoodPair[0];
        SelectedPairPass = make_pair(kTRUE,kTRUE);
      }
    } // -- end of if( nQMuons >= 2 ) -- //

    return isPassEventSelection;
  }


  Bool_t EventSelection_Zprime_NUM( NtupleHandle* ntuple, DYAnalyzer* analyzer, vector<Muon> &MuonCollection, MuPair &SelectedPair, pair<Bool_t,Bool_t> SelectedPairPass )
  {
    Bool_t isPassEventSelection = kFALSE;

    vector< Muon > QMuonCollection;
    for(Int_t j=0; j<(int)MuonCollection.size(); j++)
    {
      //if( this->isMuon_NUM( MuonCollection[j] ) )
      if( this->isMuon_DEN( MuonCollection[j] ) )
        QMuonCollection.push_back( MuonCollection[j] );
    }

    Int_t nQMuons = (Int_t)QMuonCollection.size();
    if( nQMuons >= 2 ) // -- at least 2 muons passing ID -- //
    {
      vector< MuPair > vec_GoodPair;
      for(Int_t i_mu=0; i_mu<nQMuons-1; i_mu++)
      {
        for(Int_t j_mu=i_mu+1; j_mu<nQMuons; j_mu++)
        {
          Muon mu1 = QMuonCollection[i_mu];
          Muon mu2 = QMuonCollection[j_mu];

          MuPair pair_temp( mu1, mu2 );

          Bool_t Flag_Mass = pair_temp.M > 50 ? kTRUE : kFALSE;

          // Bool_t Flag_TrigMatched = kFALSE;
          // if( this->TriggerMatching( analyzer->HLT, ntuple, pair_temp.First) ||
          //   this->TriggerMatching( analyzer->HLT, ntuple, pair_temp.Second) )
          //   Flag_TrigMatched = kTRUE;

          Bool_t Flag_Acc = pair_temp.isWithinAcc( 53, 53, 2.4, 2.4 );

          Bool_t Flag_3DAngle = pair_temp.Angle3D < TMath::Pi()-0.02 ? kTRUE : kFALSE;

          pair_temp.Calc_CommonVertexVariable_TuneP( ntuple );
          Bool_t Flag_Vtx = pair_temp.NormVtxChi2 < 20 ? kTRUE : kFALSE;

          Bool_t Flag_OS = pair_temp.isOS;

          Bool_t Flag_PtRatio = kFALSE;
          if( pair_temp.First.Pt / pair_temp.Second.Pt < 3 ) Flag_PtRatio = kTRUE;

          Bool_t Flag_B2B = kFALSE;
          Double_t pi = 3.141592; Double_t dPhim = 0.7*pi; Double_t dPhiM = 1.3*pi;
          Double_t dPhi = fabs(pair_temp.First.phi - pair_temp.Second.phi);
          if ( (dPhi < dPhiM) && (dPhi > dPhim) ) Flag_B2B = kTRUE;

          if( Flag_Mass &&
            // Flag_TrigMatched &&
            Flag_Acc &&
            Flag_3DAngle &&
            Flag_Vtx &&
            Flag_OS &&
            Flag_PtRatio &&
            Flag_B2B )
            vec_GoodPair.push_back( pair_temp );

        } // -- end of second muon iteration -- //
      } // -- end of first muon iteration -- //

      Int_t nPair = (Int_t)vec_GoodPair.size();
      if( nPair >= 1 ) // -- at least one pair pass all good pair condition -- //
      {
        // -- the pair with "largest" dimuon pT will be the first element -- //
        std::sort( vec_GoodPair.begin(), vec_GoodPair.end(), ComparePair_DimuonPt );
        isPassEventSelection = kTRUE;
        SelectedPair = vec_GoodPair[0];

        // -- final numerator selection -- //
        SelectedPairPass = make_pair( isMuon_NUM(SelectedPair.First), isMuon_NUM(SelectedPair.Second));
      }
    } // -- end of if( nQMuons >= 2 ) -- //

    return isPassEventSelection;
  }

  Bool_t TriggerMatching( TString HLT, NtupleHandle *ntuple, Muon &mu )
  {
    if( HLT == "HLT_Mu50_v* || HLT_TkMu50_v*" )
      return (mu.isTrigMatched_dR0p2(ntuple, "HLT_Mu50_v*") || mu.isTrigMatched_dR0p2(ntuple, "HLT_TkMu50_v*"));

    else
      return mu.isTrigMatched_dR0p2(ntuple, HLT);
  }

  void PrintRecoMuonInfo( Muon mu )
  {
    printf("\t\t(pT = %.3lf, eta = %.3lf, phi = %.3lf, charge = %.d)\n",
            mu.Pt, mu.eta, mu.phi, mu.charge);
  }

  static inline void loadBar(int x, int n, int r, int w)
  {
      // Only update r times.
      if( x == n )
        cout << endl;

      if ( x % (n/r +1) != 0 ) return;


      // Calculuate the ratio of complete-to-incomplete.
      float ratio = x/(float)n;
      int   c     = ratio * w;

      // Show the percentage complete.
      printf("%3d%% [", (int)(ratio*100) );

      // Show the load bar.
      for (int x=0; x<c; x++) cout << "=";

      for (int x=c; x<w; x++) cout << " ";

      // ANSI Control codes to go back to the
      // previous line and clear it.
    cout << "]\r" << flush;

  }
};

// -- SampleName: QCD, Diboson, WJets, DYTauTau, SingleTop, ttbar, DYPowheg -- //
void ProdHist_Systematic_Eff(Int_t _isMC, TString _FileName_ROOTFileList, Double_t _NormFactor)
{
  HistogramProducer *producer = new HistogramProducer( _isMC, _FileName_ROOTFileList, _NormFactor );
  producer->Producer();
}

