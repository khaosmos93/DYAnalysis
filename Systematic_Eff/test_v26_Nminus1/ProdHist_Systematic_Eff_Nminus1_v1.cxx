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

class Nminus1HistContainer
{
public:
  TString Type;

  Int_t theCut;
  TString theStrCut;

  vector< TH1D* > vec_Hist;
  vector< TH2D* > vec_Hist2;

  TH1D* h_Pt;
  TH1D* h_Pt_B;  // |eta| <= 1.2
  TH1D* h_Pt_E;  // |eta| > 1.2

  TH1D* h_Pt_bin;
  TH1D* h_Pt_bin_B;  // |eta| <= 1.2
  TH1D* h_Pt_bin_E;  // |eta| > 1.2

  TH1D* h_Eta;
  TH1D* h_Eta_B;  // |eta| <= 1.2
  TH1D* h_Eta_E;  // |eta| > 1.2

  TH1D* h_Eta_bin;
  TH1D* h_Eta_bin_B;
  TH1D* h_Eta_bin_E;

  TH1D* h_Phi;
  TH1D* h_Phi_B;  // |eta| <= 1.2
  TH1D* h_Phi_E;  // |eta| > 1.2

  TH1D* h_mass;   // mass of muon pair passing selection, fill only if both muon passe
  TH1D* h_mass_Z;
  TH1D* h_ZPt;

  TH2D *h_EtaPhi;
  TH2D *h_EtaPhi_B;
  TH2D *h_EtaPhi_E;
  TH2D *h_EtaPhi_bin;
  TH2D *h_EtaPhi_bin_B;
  TH2D *h_EtaPhi_bin_E;

  // -- N-1 -- //
  TH1D *h_dB;
  TH1D *h_nTrkLayers;
  TH1D *h_nPixelHits;
  TH1D *h_nPixelHits_1p5;
  TH1D *h_nMuonHits;
  TH1D *h_dPtOverPt;
  TH1D *h_ZprimeStation;

  Nminus1HistContainer( TString _Type, Int_t _theCut )
  {
    Type = _Type;

    TString StrCuts[7] = { "EveryCuts", "No-dB", "No-trackerLayersGLB", "No-pixelHitsGLB", "No-muonHits", "No-dPtOverPt", "No-ZprimeStation" };   //Min
    theCut = _theCut;
    theStrCut = StrCuts[theCut];

    this->Set_Histograms();
    TH1::AddDirectory(kFALSE);
  }

  void Set_Histograms()
  {

    const Double_t Pt_bins[9] = {0, 53, 55, 60, 80, 120, 200, 360, 1000};
    const Double_t Eta_bins[16] = {-2.4, -2.1, -1.6, -1.2, -1.04, -0.9, -0.3, -0.2,  0.2, 0.3, 0.9, 1.04, 1.2, 1.6, 2.1, 2.4};

    this->h_Pt = new TH1D("h_Pt_"+this->theStrCut+"_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt );
    this->h_Pt_B = new TH1D("h_Pt_B_"+this->theStrCut+"_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_B );
    this->h_Pt_E = new TH1D("h_Pt_E_"+this->theStrCut+"_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_E );

    this->h_Pt_bin = new TH1D("h_Pt_bin_"+this->theStrCut+"_"+this->Type, "", 8, Pt_bins); vec_Hist.push_back( this->h_Pt_bin );
    this->h_Pt_bin_B = new TH1D("h_Pt_bin_B_"+this->theStrCut+"_"+this->Type, "", 8, Pt_bins); vec_Hist.push_back( this->h_Pt_bin_B );
    this->h_Pt_bin_E = new TH1D("h_Pt_bin_E_"+this->theStrCut+"_"+this->Type, "", 8, Pt_bins); vec_Hist.push_back( this->h_Pt_bin_E );

    this->h_Eta = new TH1D("h_Eta_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta );
    this->h_Eta_B = new TH1D("h_Eta_B_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta_B );
    this->h_Eta_E = new TH1D("h_Eta_E_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta_E );

    this->h_Eta_bin = new TH1D("h_Eta_bin_"+this->Type, "", 15, Eta_bins); vec_Hist.push_back( this->h_Eta_bin );
    this->h_Eta_bin_B = new TH1D("h_Eta_bin_B_"+this->Type, "", 15, Eta_bins); vec_Hist.push_back( this->h_Eta_bin_B );
    this->h_Eta_bin_E = new TH1D("h_Eta_bin_E_"+this->Type, "", 15, Eta_bins); vec_Hist.push_back( this->h_Eta_bin_E );

    this->h_Phi = new TH1D("h_Phi_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi );
    this->h_Phi_B = new TH1D("h_Phi_B_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi_B );
    this->h_Phi_E = new TH1D("h_Phi_E_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi_E );

    this->h_mass = new TH1D("h_mass_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass );
    this->h_mass_Z = new TH1D("h_mass_Z_"+this->Type, "", 60, 70, 130); vec_Hist.push_back( this->h_mass_Z );
    this->h_ZPt = new TH1D("h_ZPt_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_ZPt );

    this->h_EtaPhi = new TH2D("h_EtaPhi_"+this->Type, "", 24, -2.4, 2.4, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi );
    this->h_EtaPhi_B = new TH2D("h_EtaPhi_B_"+this->Type, "", 24, -2.4, 2.4, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_B );
    this->h_EtaPhi_E = new TH2D("h_EtaPhi_E_"+this->Type, "", 24, -2.4, 2.4, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_E );

    this->h_EtaPhi_bin = new TH2D("h_EtaPhi_bin_"+this->Type, "", 15, Eta_bins, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_bin );
    this->h_EtaPhi_bin_B = new TH2D("h_EtaPhi_bin_B_"+this->Type, "", 15, Eta_bins, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_bin_B );
    this->h_EtaPhi_bin_E = new TH2D("h_EtaPhi_bin_E_"+this->Type, "", 15, Eta_bins, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_bin_E );

    // N-1
    this->h_dB = new TH1D("h_dB_"+this->theStrCut+"_"+this->Type, "", 2000, -1.0, 1.0); vec_Hist.push_back( this->h_dB );
    this->h_nTrkLayers = new TH1D("h_nTrkLayers_"+this->theStrCut+"_"+this->Type, "", 50, 0, 50); vec_Hist.push_back( this->h_nTrkLayers );
    this->h_nPixelHits = new TH1D("h_nPixelHits_"+this->theStrCut+"_"+this->Type, "", 20, 0, 20); vec_Hist.push_back( this->h_nPixelHits );
    this->h_nPixelHits_1p5 = new TH1D("h_nPixelHits_1p5_"+this->theStrCut+"_"+this->Type, "", 20, 0, 20); vec_Hist.push_back( this->h_nPixelHits_1p5 );
    this->h_nMuonHits = new TH1D("h_nMuonHits_"+this->theStrCut+"_"+this->Type, "", 60, 0, 60); vec_Hist.push_back( this->h_nMuonHits );
    this->h_dPtOverPt = new TH1D("h_dPtOverPt_"+this->theStrCut+"_"+this->Type, "", 2000, 0, 2); vec_Hist.push_back( this->h_dPtOverPt );
    this->h_ZprimeStation = new TH1D("h_ZprimeStation_"+this->theStrCut+"_"+this->Type, "", 10, 0, 10); vec_Hist.push_back( this->h_ZprimeStation );

    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++)
      vec_Hist[i]->Sumw2();
    Int_t nHist2 = (Int_t)vec_Hist2.size();
    for(Int_t i2=0; i2<nHist2; i2++)
      vec_Hist2[i2]->Sumw2();
  }

  void Fill_Den ( Muon mu1, Bool_t isTag1, Bool_t pass1, Muon mu2, Bool_t isTag2, Bool_t pass2, Double_t weight )
  {
    /*Verbose*/////cout << "pass1 : " << pass1 << endl;
    /*Verbose*/////cout << "pass2 : " << pass2 << endl;

    Muon TagMu;
    Muon ProbeMu;
    Double_t M = -1;
    Double_t ZPt = -1;

    // -- mu1 -> Tag? -- //
    if (isTag1) {
      TagMu = mu1;
      ProbeMu = mu2;

      M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      // -- Mass -- //
      h_mass->Fill( M, weight );
      if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
      h_ZPt->Fill( ZPt, weight );

      // -- pT, Eta, Phi -- //
      h_Pt     ->Fill( ProbeMu.Pt, weight );
      h_Pt_bin ->Fill( ProbeMu.Pt, weight );
      h_Eta    ->Fill( ProbeMu.eta, weight );
      h_Eta_bin    ->Fill( ProbeMu.eta, weight );
      h_Phi    ->Fill( ProbeMu.phi, weight );
      h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      if (fabs(ProbeMu.eta) <= 1.2) {
        h_Pt_B     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
        h_Eta_B    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
        h_Phi_B    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      else if (fabs(ProbeMu.eta) > 1.2) {
        h_Pt_E     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
        h_Eta_E    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
        h_Phi_E    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      // N-1
      h_dB            ->Fill( ProbeMu.dB, weight );
      h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
      h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
      if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
        h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
      h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
      h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
      h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
    }

    // -- mu2 -> Tag? -- //
    if (isTag2) {
      TagMu = mu2;
      ProbeMu = mu1;

      M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      // -- Mass -- //
      h_mass->Fill( M, weight );
      if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
      h_ZPt->Fill( ZPt, weight );

      // -- pT, Eta, Phi -- //
      h_Pt     ->Fill( ProbeMu.Pt, weight );
      h_Pt_bin ->Fill( ProbeMu.Pt, weight );
      h_Eta    ->Fill( ProbeMu.eta, weight );
      h_Eta_bin    ->Fill( ProbeMu.eta, weight );
      h_Phi    ->Fill( ProbeMu.phi, weight );
      h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      if (fabs(ProbeMu.eta) <= 1.2) {
        h_Pt_B     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
        h_Eta_B    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
        h_Phi_B    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      else if (fabs(ProbeMu.eta) > 1.2) {
        h_Pt_E     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
        h_Eta_E    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
        h_Phi_E    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      // N-1
      h_dB            ->Fill( ProbeMu.dB, weight );
      h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
      h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
      if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
        h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
      h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
      h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
      h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
    }
  }

  void Fill_Num ( Muon mu1, Bool_t isTag1, Bool_t pass1, Muon mu2, Bool_t isTag2, Bool_t pass2, Double_t weight )
  {
    /*Verbose*/////cout << "pass1 : " << pass1 << endl;
    /*Verbose*/////cout << "pass2 : " << pass2 << endl;

    Muon TagMu;
    Muon ProbeMu;
    Double_t M = -1;
    Double_t ZPt = -1;

    // -- mu1 -> Tag? -- //
    if (isTag1) {
      TagMu = mu1;
      ProbeMu = mu2;

      M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      if (pass2) {
        // -- Mass -- //
        h_mass->Fill( M, weight );
        if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
        h_ZPt->Fill( ZPt, weight );

        // -- pT, Eta, Phi -- //
        h_Pt     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin ->Fill( ProbeMu.Pt, weight );
        h_Eta    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin    ->Fill( ProbeMu.eta, weight );
        h_Phi    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        if (fabs(ProbeMu.eta) <= 1.2) {
          h_Pt_B     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
          h_Eta_B    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
          h_Phi_B    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        else if (fabs(ProbeMu.eta) > 1.2) {
          h_Pt_E     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
          h_Eta_E    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
          h_Phi_E    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        // N-1
        h_dB            ->Fill( ProbeMu.dB, weight );
        h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
        h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
        if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
          h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
        h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
        h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
        h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
      }
    }

    // -- mu2 -> Tag? -- //
    if (isTag2) {
      TagMu = mu2;
      ProbeMu = mu1;

      Double_t M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      if (pass1) {
        // -- Mass -- //
        h_mass->Fill( M, weight );
        if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
        h_ZPt->Fill( ZPt, weight );

        // -- pT, Eta, Phi -- //
        h_Pt     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin ->Fill( ProbeMu.Pt, weight );
        h_Eta    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin    ->Fill( ProbeMu.eta, weight );
        h_Phi    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        if (fabs(ProbeMu.eta) <= 1.2) {
          h_Pt_B     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
          h_Eta_B    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
          h_Phi_B    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        else if (fabs(ProbeMu.eta) > 1.2) {
          h_Pt_E     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
          h_Eta_E    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
          h_Phi_E    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        // N-1
        h_dB            ->Fill( ProbeMu.dB, weight );
        h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
        h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
        if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
          h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
        h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
        h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
        h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
      }
    }
  }

  void Save( TFile *f_output )
  {
    f_output->cd();

    TString FileName = f_output->GetName();
    printf( "[Save histograms for %s in %s, N-1 : %s]\n", Type.Data(), FileName.Data(), this->theStrCut.Data() );

    TString DirName = "Nminus1_" + this->theStrCut + "_" + this->Type;
    TDirectory *SubDir = f_output->mkdir(DirName);
    SubDir->cd();

    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++) {
      TString HistName = vec_Hist[i]->GetName();
      vec_Hist[i]->Write();
      printf("\t[%s is saved]\n", HistName.Data() );
    }

    f_output->cd();

    printf("\n");
  }

protected:
};

class BinHistContainer
{
public:
  TString Type;

  vector< TH1D* > vec_Hist;

  // Fill mass of TnP
  TH1D* h_Pt[8];
  TH1D* h_Pt_B[8];  // |eta| <= 1.2
  TH1D* h_Pt_E[8];  // |eta| > 1.2

  TH1D* h_Eta[15];

  TH1D* h_Phi[40];

  BinHistContainer( TString _Type ) {
    Type = _Type;
    this->Set_Histograms();
    TH1::AddDirectory(kFALSE);
  }

  void Set_Histograms() {

    const Double_t Pt_bins[9] = {0, 53, 55, 60, 80, 120, 200, 360, 1000};
    const Double_t Eta_bins[16] = {-2.4, -2.1, -1.6, -1.2, -1.04, -0.9, -0.3, -0.2,  0.2, 0.3, 0.9, 1.04, 1.2, 1.6, 2.1, 2.4};
    const Double_t Phi_bins[41] = { -3.2, -3.04, -2.88, -2.72, -2.56, -2.4, -2.24, -2.08, -1.92, -1.76,
                                    -1.6, -1.44, -1.28, -1.12, -0.96, -0.8, -0.64, -0.48, -0.32, -0.16,
                                       0, 0.16, 0.32, 0.48, 0.64, 0.8, 0.96, 1.12, 1.28, 1.44,
                                     1.6, 1.76, 1.92, 2.08, 2.24, 2.4, 2.56, 2.72, 2.88, 3.04,
                                     3.2};
    const Int_t nPtBin = 8;
    const Int_t nEtaBin = 15;
    const Int_t nPhiBin = 40;

    TString BinLow="";
    TString BinUp="";
    Int_t iPtBin, iEtaBin, iPhiBin;

    TString HistTitle_Pt="";
    TString HistTitle_Pt_B="";
    TString HistTitle_Pt_E="";
    for(iPtBin=0; iPtBin<nPtBin; ++iPtBin) {
      BinLow.Form("%.0f",Pt_bins[iPtBin]);
      BinUp.Form("%.0f",Pt_bins[iPtBin+1]);
      HistTitle_Pt = "pT [" + BinLow+", "+ BinUp +"]    0 < |#eta| < 2.4";
      HistTitle_Pt_B = "pT [" + BinLow+", "+ BinUp +"]    0 < |#eta| < 1.2";
      HistTitle_Pt_E = "pT [" + BinLow+", "+ BinUp +"]    1.2 < |#eta| < 2.4";
      this->h_Pt[iPtBin] = new TH1D("h_Pt~"+BinLow+"_"+BinUp+"~"+this->Type, HistTitle_Pt, 10000, 0, 10000);       vec_Hist.push_back( this->h_Pt[iPtBin] );
      this->h_Pt_B[iPtBin] = new TH1D("h_Pt_B~"+BinLow+"_"+BinUp+"~"+this->Type, HistTitle_Pt_B, 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_B[iPtBin] );
      this->h_Pt_E[iPtBin] = new TH1D("h_Pt_E~"+BinLow+"_"+BinUp+"~"+this->Type, HistTitle_Pt_E, 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_E[iPtBin] );
    }

    TString HistTitle_Eta="";
    for(iEtaBin=0; iEtaBin<nEtaBin; ++iEtaBin) {
      BinLow.Form("%.2f",Eta_bins[iEtaBin]);
      TObjArray *SpBinLow = BinLow.Tokenize(".");
      BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "p" + ((TObjString*)(SpBinLow->At(1)))->String();

      BinUp.Form("%.2f",Eta_bins[iEtaBin+1]);
      TObjArray *SpBinUp = BinUp.Tokenize(".");
      BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "p" + ((TObjString*)(SpBinUp->At(1)))->String();

      HistTitle_Eta = "#eta [" + BinLow+", "+ BinUp +"]";
      this->h_Eta[iEtaBin] = new TH1D("h_Eta~"+BinLow+"_"+BinUp+"~"+this->Type, HistTitle_Eta, 10000, 0, 10000);  vec_Hist.push_back( this->h_Eta[iEtaBin] );
    }

    TString HistTitle_Phi="";
    for(iPhiBin=0; iPhiBin<nPhiBin; ++iPhiBin) {
      BinLow.Form("%.2f",Phi_bins[iPhiBin]);
      TObjArray *SpBinLow = BinLow.Tokenize(".");
      BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "p" + ((TObjString*)(SpBinLow->At(1)))->String();

      BinUp.Form("%.2f",Phi_bins[iPhiBin+1]);
      TObjArray *SpBinUp = BinUp.Tokenize(".");
      BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "p" + ((TObjString*)(SpBinUp->At(1)))->String();

      HistTitle_Phi = "#phi [" + BinLow+", "+ BinUp +"]";
      this->h_Phi[iPhiBin] = new TH1D("h_Phi~"+BinLow+"_"+BinUp+"~"+this->Type, HistTitle_Phi, 10000, 0, 10000);  vec_Hist.push_back( this->h_Phi[iPhiBin] );
    }

    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++)
      vec_Hist[i]->Sumw2();
  }

  void Fill_Den ( Muon mu1, Bool_t isTag1, Bool_t pass1, Muon mu2, Bool_t isTag2, Bool_t pass2, Double_t weight ) {

    Muon TagMu;
    Muon ProbeMu;
    Double_t M = -1;

    // -- mu1 -> Tag? -- //
    if (isTag1) {
      TagMu = mu1;
      ProbeMu = mu2;
      M = (TagMu.Momentum + ProbeMu.Momentum).M();

      Int_t nHist = (Int_t)vec_Hist.size();
      for(Int_t i=0; i<nHist; i++) {

        TString HistName = vec_Hist[i]->GetName();

        TObjArray *SpName = HistName.Tokenize("~");
        TString Val   = ((TObjString*)(SpName->At(0)))->String();
        TString Range = ((TObjString*)(SpName->At(1)))->String();

        if ( Val.Contains("Pt") ) {
          TObjArray *SpRange = Range.Tokenize("_");
          TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
          TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
          Double_t DouBinLow = BinLow.Atof();
          Double_t DouBinUp  = BinUp.Atof();
          Double_t AbsEtaLow = 0;
          Double_t AbsEtaUp  = 2.4;
          if ( Val.Contains("B") ) {
            AbsEtaLow = 0;
            AbsEtaUp  = 1.2;
          }
          else if ( Val.Contains("E") ) {
            AbsEtaLow = 1.2;
            AbsEtaUp  = 2.4;
          }
          if ( fabs(ProbeMu.eta) > AbsEtaLow && fabs(ProbeMu.eta) < AbsEtaUp) {
            if ( ProbeMu.Pt > DouBinLow && ProbeMu.Pt < DouBinUp ) vec_Hist[i]->Fill(M, weight );
          }
        }
        else if ( Val.Contains("Eta") ) {
          TObjArray *SpRange = Range.Tokenize("_");
          TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
          TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
          TObjArray *SpBinLow = BinLow.Tokenize("p");
          BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
          TObjArray *SpBinUp = BinUp.Tokenize("p");
          BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
          Double_t DouBinLow = BinLow.Atof();
          Double_t DouBinUp  = BinUp.Atof();
          if ( ProbeMu.eta > DouBinLow && ProbeMu.eta < DouBinUp ) vec_Hist[i]->Fill(M, weight );
        }
        else if ( Val.Contains("Phi") ) {
          TObjArray *SpRange = Range.Tokenize("_");
          TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
          TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
          TObjArray *SpBinLow = BinLow.Tokenize("p");
          BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
          TObjArray *SpBinUp = BinUp.Tokenize("p");
          BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
          Double_t DouBinLow = BinLow.Atof();
          Double_t DouBinUp  = BinUp.Atof();
          if ( ProbeMu.phi > DouBinLow && ProbeMu.phi < DouBinUp ) vec_Hist[i]->Fill(M, weight );
        }
      }
    }

    // -- mu2 -> Tag? -- //
    if (isTag2) {
      TagMu = mu2;
      ProbeMu = mu1;
      M = (TagMu.Momentum + ProbeMu.Momentum).M();

      Int_t nHist = (Int_t)vec_Hist.size();
      for(Int_t i=0; i<nHist; i++) {

        TString HistName = vec_Hist[i]->GetName();

        TObjArray *SpName = HistName.Tokenize("~");
        TString Val   = ((TObjString*)(SpName->At(0)))->String();
        TString Range = ((TObjString*)(SpName->At(1)))->String();

        if ( Val.Contains("Pt") ) {
          TObjArray *SpRange = Range.Tokenize("_");
          TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
          TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
          Double_t DouBinLow = BinLow.Atof();
          Double_t DouBinUp  = BinUp.Atof();
          Double_t AbsEtaLow = 0;
          Double_t AbsEtaUp  = 2.4;
          if ( Val.Contains("B") ) {
            AbsEtaLow = 0;
            AbsEtaUp  = 1.2;
          }
          else if ( Val.Contains("E") ) {
            AbsEtaLow = 1.2;
            AbsEtaUp  = 2.4;
          }
          if ( fabs(ProbeMu.eta) > AbsEtaLow && fabs(ProbeMu.eta) < AbsEtaUp) {
            if ( ProbeMu.Pt > DouBinLow && ProbeMu.Pt < DouBinUp ) vec_Hist[i]->Fill(M, weight );
          }
        }
        else if ( Val.Contains("Eta") ) {
          TObjArray *SpRange = Range.Tokenize("_");
          TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
          TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
          TObjArray *SpBinLow = BinLow.Tokenize("p");
          BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
          TObjArray *SpBinUp = BinUp.Tokenize("p");
          BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
          Double_t DouBinLow = BinLow.Atof();
          Double_t DouBinUp  = BinUp.Atof();
          if ( ProbeMu.eta > DouBinLow && ProbeMu.eta < DouBinUp ) vec_Hist[i]->Fill(M, weight );
        }
        else if ( Val.Contains("Phi") ) {
          TObjArray *SpRange = Range.Tokenize("_");
          TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
          TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
          TObjArray *SpBinLow = BinLow.Tokenize("p");
          BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
          TObjArray *SpBinUp = BinUp.Tokenize("p");
          BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
          Double_t DouBinLow = BinLow.Atof();
          Double_t DouBinUp  = BinUp.Atof();
          if ( ProbeMu.phi > DouBinLow && ProbeMu.phi < DouBinUp ) vec_Hist[i]->Fill(M, weight );
        }
      }
    }
  }

  void Fill_Num ( Muon mu1, Bool_t isTag1, Bool_t pass1, Muon mu2, Bool_t isTag2, Bool_t pass2, Double_t weight ) {

    Muon TagMu;
    Muon ProbeMu;
    Double_t M = -1;

    // -- mu1 -> Tag? -- //
    if (isTag1) {
      TagMu = mu1;
      ProbeMu = mu2;
      M = (TagMu.Momentum + ProbeMu.Momentum).M();

      if (pass2) {
        Int_t nHist = (Int_t)vec_Hist.size();
        for(Int_t i=0; i<nHist; i++) {

          TString HistName = vec_Hist[i]->GetName();

          TObjArray *SpName = HistName.Tokenize("~");
          TString Val   = ((TObjString*)(SpName->At(0)))->String();
          TString Range = ((TObjString*)(SpName->At(1)))->String();

          if ( Val.Contains("Pt") ) {
            TObjArray *SpRange = Range.Tokenize("_");
            TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
            TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
            Double_t DouBinLow = BinLow.Atof();
            Double_t DouBinUp  = BinUp.Atof();
            Double_t AbsEtaLow = 0;
            Double_t AbsEtaUp  = 2.4;
            if ( Val.Contains("B") ) {
              AbsEtaLow = 0;
              AbsEtaUp  = 1.2;
            }
            else if ( Val.Contains("E") ) {
              AbsEtaLow = 1.2;
              AbsEtaUp  = 2.4;
            }
            if ( fabs(ProbeMu.eta) > AbsEtaLow && fabs(ProbeMu.eta) < AbsEtaUp) {
              if ( ProbeMu.Pt > DouBinLow && ProbeMu.Pt < DouBinUp ) vec_Hist[i]->Fill(M, weight );
            }
          }
          else if ( Val.Contains("Eta") ) {
            TObjArray *SpRange = Range.Tokenize("_");
            TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
            TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
            TObjArray *SpBinLow = BinLow.Tokenize("p");
            BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
            TObjArray *SpBinUp = BinUp.Tokenize("p");
            BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
            Double_t DouBinLow = BinLow.Atof();
            Double_t DouBinUp  = BinUp.Atof();
            if ( ProbeMu.eta > DouBinLow && ProbeMu.eta < DouBinUp ) vec_Hist[i]->Fill(M, weight );
          }
          else if ( Val.Contains("Phi") ) {
            TObjArray *SpRange = Range.Tokenize("_");
            TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
            TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
            TObjArray *SpBinLow = BinLow.Tokenize("p");
            BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
            TObjArray *SpBinUp = BinUp.Tokenize("p");
            BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
            Double_t DouBinLow = BinLow.Atof();
            Double_t DouBinUp  = BinUp.Atof();
            if ( ProbeMu.phi > DouBinLow && ProbeMu.phi < DouBinUp ) vec_Hist[i]->Fill(M, weight );
          }
        }
      }
    }

    // -- mu2 -> Tag? -- //
    if (isTag2) {
      TagMu = mu2;
      ProbeMu = mu1;
      M = (TagMu.Momentum + ProbeMu.Momentum).M();

      if (pass1) {
        Int_t nHist = (Int_t)vec_Hist.size();
        for(Int_t i=0; i<nHist; i++) {

          TString HistName = vec_Hist[i]->GetName();

          TObjArray *SpName = HistName.Tokenize("~");
          TString Val   = ((TObjString*)(SpName->At(0)))->String();
          TString Range = ((TObjString*)(SpName->At(1)))->String();

          if ( Val.Contains("Pt") ) {
            TObjArray *SpRange = Range.Tokenize("_");
            TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
            TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
            Double_t DouBinLow = BinLow.Atof();
            Double_t DouBinUp  = BinUp.Atof();
            Double_t AbsEtaLow = 0;
            Double_t AbsEtaUp  = 2.4;
            if ( Val.Contains("B") ) {
              AbsEtaLow = 0;
              AbsEtaUp  = 1.2;
            }
            else if ( Val.Contains("E") ) {
              AbsEtaLow = 1.2;
              AbsEtaUp  = 2.4;
            }
            if ( fabs(ProbeMu.eta) > AbsEtaLow && fabs(ProbeMu.eta) < AbsEtaUp) {
              if ( ProbeMu.Pt > DouBinLow && ProbeMu.Pt < DouBinUp ) vec_Hist[i]->Fill(M, weight );
            }
          }
          else if ( Val.Contains("Eta") ) {
            TObjArray *SpRange = Range.Tokenize("_");
            TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
            TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
            TObjArray *SpBinLow = BinLow.Tokenize("p");
            BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
            TObjArray *SpBinUp = BinUp.Tokenize("p");
            BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
            Double_t DouBinLow = BinLow.Atof();
            Double_t DouBinUp  = BinUp.Atof();
            if ( ProbeMu.eta > DouBinLow && ProbeMu.eta < DouBinUp ) vec_Hist[i]->Fill(M, weight );
          }
          else if ( Val.Contains("Phi") ) {
            TObjArray *SpRange = Range.Tokenize("_");
            TString BinLow = ((TObjString*)(SpRange->At(0)))->String();
            TString BinUp  = ((TObjString*)(SpRange->At(1)))->String();
            TObjArray *SpBinLow = BinLow.Tokenize("p");
            BinLow = ((TObjString*)(SpBinLow->At(0)))->String() + "." + ((TObjString*)(SpBinLow->At(1)))->String();
            TObjArray *SpBinUp = BinUp.Tokenize("p");
            BinUp = ((TObjString*)(SpBinUp->At(0)))->String() + "." + ((TObjString*)(SpBinUp->At(1)))->String();
            Double_t DouBinLow = BinLow.Atof();
            Double_t DouBinUp  = BinUp.Atof();
            if ( ProbeMu.phi > DouBinLow && ProbeMu.phi < DouBinUp ) vec_Hist[i]->Fill(M, weight );
          }
        }
      }
    }
  }

  void Save( TFile *f_output )
  {
    f_output->cd();

    TString FileName = f_output->GetName();
    printf( "[Save histograms for %s in %s]\n", Type.Data(), FileName.Data() );

    TString DirName = "BinDist_" + this->Type;
    TDirectory *SubDir = f_output->mkdir(DirName);
    SubDir->cd();

    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++) {
      TString HistName = vec_Hist[i]->GetName();
      vec_Hist[i]->Write();
      printf("\t[%s is saved]\n", HistName.Data() );
    }

    f_output->cd();

    printf("\n");
  }

protected:
};


class HistContainer
{
public:
  TString Type;

  vector< TH1D* > vec_Hist;
  vector< TH2D* > vec_Hist2;

  TH1D* h_Pt;
  TH1D* h_Pt_B;  // |eta| <= 1.2
  TH1D* h_Pt_E;  // |eta| > 1.2

  TH1D* h_Pt_bin;
  TH1D* h_Pt_bin_B;  // |eta| <= 1.2
  TH1D* h_Pt_bin_E;  // |eta| > 1.2

  TH1D* h_Eta;
  TH1D* h_Eta_B;  // |eta| <= 1.2
  TH1D* h_Eta_E;  // |eta| > 1.2

  TH1D* h_Eta_bin;
  TH1D* h_Eta_bin_B;
  TH1D* h_Eta_bin_E;

  TH1D* h_Phi;
  TH1D* h_Phi_B;  // |eta| <= 1.2
  TH1D* h_Phi_E;  // |eta| > 1.2

  TH1D* h_mass;   // mass of muon pair passing selection, fill only if both muon passe
  TH1D* h_mass_Z;
  TH1D* h_ZPt;

  TH2D *h_EtaPhi;
  TH2D *h_EtaPhi_B;
  TH2D *h_EtaPhi_E;
  TH2D *h_EtaPhi_bin;
  TH2D *h_EtaPhi_bin_B;
  TH2D *h_EtaPhi_bin_E;

  // -- N-1 -- //
  TH1D *h_dB;
  TH1D *h_nTrkLayers;
  TH1D *h_nPixelHits;
  TH1D *h_nPixelHits_1p5;
  TH1D *h_nMuonHits;
  TH1D *h_dPtOverPt;
  TH1D *h_ZprimeStation;

  HistContainer( TString _Type )
  {
    Type = _Type;
    this->Set_Histograms();
    TH1::AddDirectory(kFALSE);
  }

  void Set_Histograms()
  {

    const Double_t Pt_bins[9] = {0, 53, 55, 60, 80, 120, 200, 360, 1000};
    const Double_t Eta_bins[16] = {-2.4, -2.1, -1.6, -1.2, -1.04, -0.9, -0.3, -0.2,  0.2, 0.3, 0.9, 1.04, 1.2, 1.6, 2.1, 2.4};

    this->h_Pt = new TH1D("h_Pt_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt );
    this->h_Pt_B = new TH1D("h_Pt_B_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_B );
    this->h_Pt_E = new TH1D("h_Pt_E_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_Pt_E );

    this->h_Pt_bin = new TH1D("h_Pt_bin_"+this->Type, "", 8, Pt_bins); vec_Hist.push_back( this->h_Pt_bin );
    this->h_Pt_bin_B = new TH1D("h_Pt_bin_B_"+this->Type, "", 8, Pt_bins); vec_Hist.push_back( this->h_Pt_bin_B );
    this->h_Pt_bin_E = new TH1D("h_Pt_bin_E_"+this->Type, "", 8, Pt_bins); vec_Hist.push_back( this->h_Pt_bin_E );

    this->h_Eta = new TH1D("h_Eta_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta );
    this->h_Eta_B = new TH1D("h_Eta_B_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta_B );
    this->h_Eta_E = new TH1D("h_Eta_E_"+this->Type, "", 24, -2.4, 2.4); vec_Hist.push_back( this->h_Eta_E );

    this->h_Eta_bin = new TH1D("h_Eta_bin_"+this->Type, "", 15, Eta_bins); vec_Hist.push_back( this->h_Eta_bin );
    this->h_Eta_bin_B = new TH1D("h_Eta_bin_B_"+this->Type, "", 15, Eta_bins); vec_Hist.push_back( this->h_Eta_bin_B );
    this->h_Eta_bin_E = new TH1D("h_Eta_bin_E_"+this->Type, "", 15, Eta_bins); vec_Hist.push_back( this->h_Eta_bin_E );

    this->h_Phi = new TH1D("h_Phi_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi );
    this->h_Phi_B = new TH1D("h_Phi_B_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi_B );
    this->h_Phi_E = new TH1D("h_Phi_E_"+this->Type, "", 40, -3.2, 3.2); vec_Hist.push_back( this->h_Phi_E );

    this->h_mass = new TH1D("h_mass_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_mass );
    this->h_mass_Z = new TH1D("h_mass_Z_"+this->Type, "", 60, 70, 130); vec_Hist.push_back( this->h_mass_Z );
    this->h_ZPt = new TH1D("h_ZPt_"+this->Type, "", 10000, 0, 10000); vec_Hist.push_back( this->h_ZPt );

    this->h_EtaPhi = new TH2D("h_EtaPhi_"+this->Type, "", 24, -2.4, 2.4, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi );
    this->h_EtaPhi_B = new TH2D("h_EtaPhi_B_"+this->Type, "", 24, -2.4, 2.4, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_B );
    this->h_EtaPhi_E = new TH2D("h_EtaPhi_E_"+this->Type, "", 24, -2.4, 2.4, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_E );

    this->h_EtaPhi_bin = new TH2D("h_EtaPhi_bin_"+this->Type, "", 15, Eta_bins, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_bin );
    this->h_EtaPhi_bin_B = new TH2D("h_EtaPhi_bin_B_"+this->Type, "", 15, Eta_bins, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_bin_B );
    this->h_EtaPhi_bin_E = new TH2D("h_EtaPhi_bin_E_"+this->Type, "", 15, Eta_bins, 40, -3.2, 3.2); vec_Hist2.push_back( this->h_EtaPhi_bin_E );

    // N-1
    this->h_dB = new TH1D("h_dB_"+this->Type, "", 2000, -1.0, 1.0); vec_Hist.push_back( this->h_dB );
    this->h_nTrkLayers = new TH1D("h_nTrkLayers_"+this->Type, "", 50, 0, 50); vec_Hist.push_back( this->h_nTrkLayers );
    this->h_nPixelHits = new TH1D("h_nPixelHits_"+this->Type, "", 20, 0, 20); vec_Hist.push_back( this->h_nPixelHits );
    this->h_nPixelHits_1p5 = new TH1D("h_nPixelHits_1p5_"+this->Type, "", 20, 0, 20); vec_Hist.push_back( this->h_nPixelHits_1p5 );
    this->h_nMuonHits = new TH1D("h_nMuonHits_"+this->Type, "", 60, 0, 60); vec_Hist.push_back( this->h_nMuonHits );
    this->h_dPtOverPt = new TH1D("h_dPtOverPt_"+this->Type, "", 2000, 0, 2); vec_Hist.push_back( this->h_dPtOverPt );
    this->h_ZprimeStation = new TH1D("h_ZprimeStation_"+this->Type, "", 10, 0, 10); vec_Hist.push_back( this->h_ZprimeStation );

    Int_t nHist = (Int_t)vec_Hist.size();
    for(Int_t i=0; i<nHist; i++)
      vec_Hist[i]->Sumw2();
    Int_t nHist2 = (Int_t)vec_Hist2.size();
    for(Int_t i2=0; i2<nHist2; i2++)
      vec_Hist2[i2]->Sumw2();
  }

  void Fill_Den ( Muon mu1, Bool_t isTag1, Bool_t pass1, Muon mu2, Bool_t isTag2, Bool_t pass2, Double_t weight )
  {
    /*Verbose*/////cout << "pass1 : " << pass1 << endl;
    /*Verbose*/////cout << "pass2 : " << pass2 << endl;

    Muon TagMu;
    Muon ProbeMu;
    Double_t M = -1;
    Double_t ZPt = -1;

    // -- mu1 -> Tag? -- //
    if (isTag1) {
      TagMu = mu1;
      ProbeMu = mu2;

      M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      // -- Mass -- //
      h_mass->Fill( M, weight );
      if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
      h_ZPt->Fill( ZPt, weight );

      // -- pT, Eta, Phi -- //
      h_Pt     ->Fill( ProbeMu.Pt, weight );
      h_Pt_bin ->Fill( ProbeMu.Pt, weight );
      h_Eta    ->Fill( ProbeMu.eta, weight );
      h_Eta_bin    ->Fill( ProbeMu.eta, weight );
      h_Phi    ->Fill( ProbeMu.phi, weight );
      h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      if (fabs(ProbeMu.eta) <= 1.2) {
        h_Pt_B     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
        h_Eta_B    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
        h_Phi_B    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      else if (fabs(ProbeMu.eta) > 1.2) {
        h_Pt_E     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
        h_Eta_E    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
        h_Phi_E    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      // N-1
      h_dB            ->Fill( ProbeMu.dB, weight );
      h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
      h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
      if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
        h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
      h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
      h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
      h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
    }

    // -- mu2 -> Tag? -- //
    if (isTag2) {
      TagMu = mu2;
      ProbeMu = mu1;

      M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      // -- Mass -- //
      h_mass->Fill( M, weight );
      if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
      h_ZPt->Fill( ZPt, weight );

      // -- pT, Eta, Phi -- //
      h_Pt     ->Fill( ProbeMu.Pt, weight );
      h_Pt_bin ->Fill( ProbeMu.Pt, weight );
      h_Eta    ->Fill( ProbeMu.eta, weight );
      h_Eta_bin    ->Fill( ProbeMu.eta, weight );
      h_Phi    ->Fill( ProbeMu.phi, weight );
      h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      if (fabs(ProbeMu.eta) <= 1.2) {
        h_Pt_B     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
        h_Eta_B    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
        h_Phi_B    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      else if (fabs(ProbeMu.eta) > 1.2) {
        h_Pt_E     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
        h_Eta_E    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
        h_Phi_E    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
      }
      // N-1
      h_dB            ->Fill( ProbeMu.dB, weight );
      h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
      h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
      if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
        h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
      h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
      h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
      h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
    }
  }

  void Fill_Num ( Muon mu1, Bool_t isTag1, Bool_t pass1, Muon mu2, Bool_t isTag2, Bool_t pass2, Double_t weight )
  {
    /*Verbose*/////cout << "pass1 : " << pass1 << endl;
    /*Verbose*/////cout << "pass2 : " << pass2 << endl;

    Muon TagMu;
    Muon ProbeMu;
    Double_t M = -1;
    Double_t ZPt = -1;

    // -- mu1 -> Tag? -- //
    if (isTag1) {
      TagMu = mu1;
      ProbeMu = mu2;

      M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      if (pass2) {
        // -- Mass -- //
        h_mass->Fill( M, weight );
        if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
        h_ZPt->Fill( ZPt, weight );

        // -- pT, Eta, Phi -- //
        h_Pt     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin ->Fill( ProbeMu.Pt, weight );
        h_Eta    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin    ->Fill( ProbeMu.eta, weight );
        h_Phi    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        if (fabs(ProbeMu.eta) <= 1.2) {
          h_Pt_B     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
          h_Eta_B    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
          h_Phi_B    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        else if (fabs(ProbeMu.eta) > 1.2) {
          h_Pt_E     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
          h_Eta_E    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
          h_Phi_E    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        // N-1
        h_dB            ->Fill( ProbeMu.dB, weight );
        h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
        h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
        if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
          h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
        h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
        h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
        h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
      }
    }

    // -- mu2 -> Tag? -- //
    if (isTag2) {
      TagMu = mu2;
      ProbeMu = mu1;

      Double_t M = (TagMu.Momentum + ProbeMu.Momentum).M();
      ZPt = (TagMu.Momentum + ProbeMu.Momentum).Pt();

      if (pass1) {
        // -- Mass -- //
        h_mass->Fill( M, weight );
        if ( M > 70 && M < 130 ) h_mass_Z->Fill( M, weight );
        h_ZPt->Fill( ZPt, weight );

        // -- pT, Eta, Phi -- //
        h_Pt     ->Fill( ProbeMu.Pt, weight );
        h_Pt_bin ->Fill( ProbeMu.Pt, weight );
        h_Eta    ->Fill( ProbeMu.eta, weight );
        h_Eta_bin    ->Fill( ProbeMu.eta, weight );
        h_Phi    ->Fill( ProbeMu.phi, weight );
        h_EtaPhi ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        h_EtaPhi_bin ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        if (fabs(ProbeMu.eta) <= 1.2) {
          h_Pt_B     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_B ->Fill( ProbeMu.Pt, weight );
          h_Eta_B    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_B    ->Fill( ProbeMu.eta, weight );
          h_Phi_B    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_B ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        else if (fabs(ProbeMu.eta) > 1.2) {
          h_Pt_E     ->Fill( ProbeMu.Pt, weight );
          h_Pt_bin_E ->Fill( ProbeMu.Pt, weight );
          h_Eta_E    ->Fill( ProbeMu.eta, weight );
          h_Eta_bin_E    ->Fill( ProbeMu.eta, weight );
          h_Phi_E    ->Fill( ProbeMu.phi, weight );
          h_EtaPhi_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
          h_EtaPhi_bin_E ->Fill( ProbeMu.eta, ProbeMu.phi, weight);
        }
        // N-1
        h_dB            ->Fill( ProbeMu.dB, weight );
        h_nTrkLayers    ->Fill( ProbeMu.trackerLayersGLB, weight );
        h_nPixelHits    ->Fill( ProbeMu.pixelHitsGLB, weight );
        if ( (fabs(ProbeMu.phi) > 1.3) && (fabs(ProbeMu.phi) < 1.7) )
          h_nPixelHits_1p5 ->Fill( ProbeMu.pixelHitsGLB, weight );
        h_nMuonHits     ->Fill( ProbeMu.muonHits, weight );
        h_dPtOverPt     ->Fill(  (ProbeMu.TuneP_pTError / ProbeMu.TuneP_pT ), weight );
        h_ZprimeStation ->Fill( ProbeMu.nMatches, weight );
      }
    }
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

    Int_t nHist2 = (Int_t)vec_Hist2.size();
    for(Int_t i2=0; i2<nHist2; i2++) {
      TString HistName2 = vec_Hist2[i2]->GetName();
      vec_Hist2[i2]->Write();
      printf("\t[%s is saved]\n", HistName2.Data() );

    }

    printf("\n");
  }

protected:
};

class HistogramProducer
{
public:
  Int_t isMC;
  Int_t isZwindow;
  Double_t NormFactor;
  TString FileName_ROOTFileList;
  TString Tag;
  TString Tag2;    //Min

  HistogramProducer( Int_t _isMC, Int_t _isZwindow, TString _FileName_ROOTFileList, Double_t _NormFactor)
  {
    this->isMC = _isMC;
    this->isZwindow = _isZwindow;
    this->FileName_ROOTFileList = _FileName_ROOTFileList;
    this->NormFactor = _NormFactor;
    this->Tag = this->GetTag( this->FileName_ROOTFileList );
    this->Tag2 = this->GetTag( this->FileName_ROOTFileList );    //Min

    if( this->Tag.Contains("DYPowheg") ) this->Tag = "DYPowheg";

    printf("===============[HistogramProducer]===============\n");
    printf("[isMC = %d, isZwin = %d] Tag = %s, NormFactor = %lf\n", this->isMC, this->isZwindow, this->Tag.Data(), this->NormFactor);
    printf("=================================================\n");
  }

  void Producer()
  {
    TStopwatch totaltime;
    totaltime.Start();

    Bool_t MakeBinDist = kFALSE;
    cout << "\t\t|||||  Make Bin distribution : " << MakeBinDist << "|||||\n" << endl;

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

    vector< Nminus1HistContainer* > vec_Nminus1Containers_DEN;
    vector< Nminus1HistContainer* > vec_Nminus1Containers_NUM;

    Nminus1HistContainer *Nminus1_DEN_0 = new Nminus1HistContainer( "DEN", 0 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_0 );
    Nminus1HistContainer *Nminus1_DEN_1 = new Nminus1HistContainer( "DEN", 1 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_1 );
    Nminus1HistContainer *Nminus1_DEN_2 = new Nminus1HistContainer( "DEN", 2 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_2 );
    Nminus1HistContainer *Nminus1_DEN_3 = new Nminus1HistContainer( "DEN", 3 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_3 );
    Nminus1HistContainer *Nminus1_DEN_4 = new Nminus1HistContainer( "DEN", 4 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_4 );
    Nminus1HistContainer *Nminus1_DEN_5 = new Nminus1HistContainer( "DEN", 5 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_5 );
    Nminus1HistContainer *Nminus1_DEN_6 = new Nminus1HistContainer( "DEN", 6 );  vec_Nminus1Containers_DEN.push_back( Nminus1_DEN_6 );

    Nminus1HistContainer *Nminus1_NUM_0 = new Nminus1HistContainer( "NUM", 0 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_0 );
    Nminus1HistContainer *Nminus1_NUM_1 = new Nminus1HistContainer( "NUM", 1 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_1 );
    Nminus1HistContainer *Nminus1_NUM_2 = new Nminus1HistContainer( "NUM", 2 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_2 );
    Nminus1HistContainer *Nminus1_NUM_3 = new Nminus1HistContainer( "NUM", 3 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_3 );
    Nminus1HistContainer *Nminus1_NUM_4 = new Nminus1HistContainer( "NUM", 4 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_4 );
    Nminus1HistContainer *Nminus1_NUM_5 = new Nminus1HistContainer( "NUM", 5 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_5 );
    Nminus1HistContainer *Nminus1_NUM_6 = new Nminus1HistContainer( "NUM", 6 );  vec_Nminus1Containers_NUM.push_back( Nminus1_NUM_6 );

    vector< Nminus1HistContainer* > vec_Nminus1Containers_DEN_RunBtoF;
    vector< Nminus1HistContainer* > vec_Nminus1Containers_NUM_RunBtoF;

    Nminus1HistContainer *Nminus1_DEN_RunBtoF_0 = new Nminus1HistContainer( "DEN_RunBtoF", 0 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_0 );
    Nminus1HistContainer *Nminus1_DEN_RunBtoF_1 = new Nminus1HistContainer( "DEN_RunBtoF", 1 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_1 );
    Nminus1HistContainer *Nminus1_DEN_RunBtoF_2 = new Nminus1HistContainer( "DEN_RunBtoF", 2 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_2 );
    Nminus1HistContainer *Nminus1_DEN_RunBtoF_3 = new Nminus1HistContainer( "DEN_RunBtoF", 3 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_3 );
    Nminus1HistContainer *Nminus1_DEN_RunBtoF_4 = new Nminus1HistContainer( "DEN_RunBtoF", 4 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_4 );
    Nminus1HistContainer *Nminus1_DEN_RunBtoF_5 = new Nminus1HistContainer( "DEN_RunBtoF", 5 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_5 );
    Nminus1HistContainer *Nminus1_DEN_RunBtoF_6 = new Nminus1HistContainer( "DEN_RunBtoF", 6 );  vec_Nminus1Containers_DEN_RunBtoF.push_back( Nminus1_DEN_RunBtoF_6 );

    Nminus1HistContainer *Nminus1_NUM_RunBtoF_0 = new Nminus1HistContainer( "NUM_RunBtoF", 0 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_0 );
    Nminus1HistContainer *Nminus1_NUM_RunBtoF_1 = new Nminus1HistContainer( "NUM_RunBtoF", 1 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_1 );
    Nminus1HistContainer *Nminus1_NUM_RunBtoF_2 = new Nminus1HistContainer( "NUM_RunBtoF", 2 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_2 );
    Nminus1HistContainer *Nminus1_NUM_RunBtoF_3 = new Nminus1HistContainer( "NUM_RunBtoF", 3 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_3 );
    Nminus1HistContainer *Nminus1_NUM_RunBtoF_4 = new Nminus1HistContainer( "NUM_RunBtoF", 4 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_4 );
    Nminus1HistContainer *Nminus1_NUM_RunBtoF_5 = new Nminus1HistContainer( "NUM_RunBtoF", 5 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_5 );
    Nminus1HistContainer *Nminus1_NUM_RunBtoF_6 = new Nminus1HistContainer( "NUM_RunBtoF", 6 );  vec_Nminus1Containers_NUM_RunBtoF.push_back( Nminus1_NUM_RunBtoF_6 );

    vector< Nminus1HistContainer* > vec_Nminus1Containers_DEN_RunGtoH;
    vector< Nminus1HistContainer* > vec_Nminus1Containers_NUM_RunGtoH;

    Nminus1HistContainer *Nminus1_DEN_RunGtoH_0 = new Nminus1HistContainer( "DEN_RunGtoH", 0 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_0 );
    Nminus1HistContainer *Nminus1_DEN_RunGtoH_1 = new Nminus1HistContainer( "DEN_RunGtoH", 1 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_1 );
    Nminus1HistContainer *Nminus1_DEN_RunGtoH_2 = new Nminus1HistContainer( "DEN_RunGtoH", 2 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_2 );
    Nminus1HistContainer *Nminus1_DEN_RunGtoH_3 = new Nminus1HistContainer( "DEN_RunGtoH", 3 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_3 );
    Nminus1HistContainer *Nminus1_DEN_RunGtoH_4 = new Nminus1HistContainer( "DEN_RunGtoH", 4 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_4 );
    Nminus1HistContainer *Nminus1_DEN_RunGtoH_5 = new Nminus1HistContainer( "DEN_RunGtoH", 5 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_5 );
    Nminus1HistContainer *Nminus1_DEN_RunGtoH_6 = new Nminus1HistContainer( "DEN_RunGtoH", 6 );  vec_Nminus1Containers_DEN_RunGtoH.push_back( Nminus1_DEN_RunGtoH_6 );

    Nminus1HistContainer *Nminus1_NUM_RunGtoH_0 = new Nminus1HistContainer( "NUM_RunGtoH", 0 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_0 );
    Nminus1HistContainer *Nminus1_NUM_RunGtoH_1 = new Nminus1HistContainer( "NUM_RunGtoH", 1 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_1 );
    Nminus1HistContainer *Nminus1_NUM_RunGtoH_2 = new Nminus1HistContainer( "NUM_RunGtoH", 2 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_2 );
    Nminus1HistContainer *Nminus1_NUM_RunGtoH_3 = new Nminus1HistContainer( "NUM_RunGtoH", 3 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_3 );
    Nminus1HistContainer *Nminus1_NUM_RunGtoH_4 = new Nminus1HistContainer( "NUM_RunGtoH", 4 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_4 );
    Nminus1HistContainer *Nminus1_NUM_RunGtoH_5 = new Nminus1HistContainer( "NUM_RunGtoH", 5 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_5 );
    Nminus1HistContainer *Nminus1_NUM_RunGtoH_6 = new Nminus1HistContainer( "NUM_RunGtoH", 6 );  vec_Nminus1Containers_NUM_RunGtoH.push_back( Nminus1_NUM_RunGtoH_6 );


    Int_t nTotEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nTotEvent << "]" << endl;

    //nTotEvent = 1000;
    for(Int_t i=0; i<nTotEvent; i++)
    {
      this->loadBar(i+1, nTotEvent, 100, 100);

      ntuple->GetEvent(i);

      // cout << i << "th event, Flag_noBadMuons: " << Flag_noBadMuons << endl;

      //Bring weights for NLO MC events
      Double_t GenWeight;
      ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

      Double_t TotWeight = this->NormFactor*GenWeight;

      // -- GenFlag for DYTauTau -- //
      Bool_t GenFlag = kFALSE;
      //GenFlag = analyzer->SeparateDYLLSample_isHardProcess(this->Tag, ntuple); // -- for DY->tautau process -- //
      if (!this->isMC) {
        GenFlag = kTRUE;
      }
      else if (this->isMC) {
        if( Tag.Contains("DYTauTau") ) {
          vector<GenLepton> GenLeptonCollection;
          Int_t NGenLeptons = ntuple->gnpair;
          for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
          {
            GenLepton genlep;
            genlep.FillFromNtuple(ntuple, i_gen);
            if( abs(genlep.ID) == 15 && genlep.isHardProcess )
              GenLeptonCollection.push_back( genlep );
          }

          if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
          {
            GenFlag = kTRUE;
          }
        }
        else {    //Other Powheg_ZtoMuMu, ttbar, WJets, Diboson...
          GenFlag = kTRUE;
        }
      }
      //cout << "\tGenFlag : " << GenFlag << endl;

      // -- GenFlag for Boosted Z samples -- //   // Min
      //cout << "Tag2 : " << Tag2 << endl;
      Bool_t GenFlag2 = kFALSE;    //Min
      if (!this->isMC) {
        GenFlag2 = kTRUE;
      }
      else if (this->isMC) {
        if( this->Tag2.Contains("Powheg_M50to120") || this->Tag2.Contains("DYLL_Pt") ) {
          vector<GenLepton> GenLeptonCollection;
          Int_t NGenLeptons = ntuple->gnpair;
          for (Int_t i_gen=0; i_gen<NGenLeptons; i_gen++) {
            GenLepton genlep;
            genlep.FillFromNtuple(ntuple, i_gen);

            //cout << "\tGenlep[ " << i_gen << " ]" << endl;
            //cout << "\tgenlep pT = " << genlep.Pt << endl;
            //cout << "\tgenlep isMuon = " << genlep.isMuon() << endl;
            //cout << "\tgenlep isHardProcess = " << genlep.isHardProcess << endl;

            if( genlep.isMuon() && genlep.isHardProcess )
              GenLeptonCollection.push_back( genlep );
          }
          //cout << "\tGenLeptonCollection.size() = " << GenLeptonCollection.size() << endl;
          Double_t gen_M = -999;
          Double_t gen_ZPt = -999;
          if ( GenLeptonCollection.size() == 2 ) {

            TLorentzVector v1 = GenLeptonCollection[0].Momentum;
            TLorentzVector v2 = GenLeptonCollection[1].Momentum;
            gen_M = (v1 + v2).M();
            gen_ZPt = (v1 + v2).Pt();
            //cout << "\t\tgen_M = " << gen_M << endl;
            //cout << "\t\tgen_ZPt = " << gen_ZPt << endl;

            if ( this->Tag2.Contains("Powheg_M50to120") ) {   // Z pT < 50 cut
              if ( gen_ZPt < 50 ) {
                GenFlag2 = kTRUE;
              }
            }

            else if ( this->Tag2.Contains("DYLL_Pt") ) {   // 50 < M < 120 cut
              if ( gen_M > 50 && gen_M < 120 ) {
                GenFlag2 = kTRUE;
              }
            }

            else {   //Other Powheg_ZtoMuMu, ttbar, WJets, Diboson...
              cout << "Warning : Tag2 is wrong...\t" << this->Tag2 << "--> Set GenFlag2 = kTRUE" << endl;
              GenFlag2 = kTRUE;
            }

          }
        }
        else {    //Other Powheg_ZtoMuMu, ttbar, WJets, Diboson...
          GenFlag2 = kTRUE;
        }
      }
      //cout << "\tGenFlag2 : " << GenFlag2 << endl;
      //cout << "\t\tGenFlag * GenFlag2 : " << GenFlag*GenFlag2 << endl;


      if( GenFlag == kTRUE && GenFlag2 == kTRUE)
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
            //    rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayersGLB, qter);

            //  // -- Change Muon pT, eta and phi with updated(corrected) one -- //
            //  mu.Pt = mu.Momentum.Pt();
            //  mu.eta = mu.Momentum.Eta();
            //  mu.phi = mu.Momentum.Phi();
            // }

            MuonCollection.push_back( mu );
          }

          // -- Loop over ID cuts -- //
          for(Int_t iCut=0; iCut < 7; ++iCut) {
            MuPair SelectedPair_Nminus1;
            pair<Bool_t, Bool_t> SelectedPairTag_Nminus1;
            pair<Bool_t, Bool_t> SelectedPairPass_Nminus1;
            Bool_t IsSelected_Nminus1 = this->EventSelection_Zprime_Nminus1( iCut, ntuple, analyzer, MuonCollection, SelectedPair_Nminus1, SelectedPairTag_Nminus1, SelectedPairPass_Nminus1 );

            if( IsSelected_Nminus1 ) {
              Muon mu1 = SelectedPair_Nminus1.First;
              Muon mu2 = SelectedPair_Nminus1.Second;
              Bool_t isTag1 = SelectedPairTag_Nminus1.first;
              Bool_t isTag2 = SelectedPairTag_Nminus1.second;
              Bool_t pass1 = SelectedPairPass_Nminus1.first;
              Bool_t pass2 = SelectedPairPass_Nminus1.second;

              //vec_Nminus1Containers_DEN[iCut]->Fill_Den( mu1, isTag1, pass1, mu2, isTag2, pass2, TotWeight);
              vec_Nminus1Containers_NUM[iCut]->Fill_Num( mu1, isTag1, pass1, mu2, isTag2, pass2, TotWeight);

              if( !isMC ) { // -- data -- //
                if( ntuple->runNum <= 278808 ) {
                  //vec_Nminus1Containers_DEN_RunBtoF[iCut]->Fill_Den( mu1, isTag1, pass1, mu2, isTag2, pass2, TotWeight);
                  vec_Nminus1Containers_NUM_RunBtoF[iCut]->Fill_Num( mu1, isTag1, pass1, mu2, isTag2, pass2, TotWeight);
                }
                else {
                  //vec_Nminus1Containers_DEN_RunGtoH[iCut]->Fill_Den( mu1, isTag1, pass1, mu2, isTag2, pass2, TotWeight);
                  vec_Nminus1Containers_NUM_RunGtoH[iCut]->Fill_Num( mu1, isTag1, pass1, mu2, isTag2, pass2, TotWeight);
                }
              }
            }

          } // end of Loop over ID cuts

        } // -- end of if( ntuple->isTriggered( analyzer->HLT ) ) -- //

      } // -- end of if( GenFlag == kTRUE ) -- //

    } // -- end of event iteration -- //

    TFile *f_output = TFile::Open("ROOTFile_Output_Systematic_Eff_"+this->Tag+"_Nminus1_v1.root", "RECREATE");

    // -- Loop over ID cuts -- //
    for(Int_t iCut=0; iCut < 7; ++iCut) {
      //vec_Nminus1Containers_DEN[iCut]->Save( f_output );
      vec_Nminus1Containers_NUM[iCut]->Save( f_output );

      if( !isMC ) {
        //vec_Nminus1Containers_DEN_RunBtoF[iCut]->Save( f_output );
        //vec_Nminus1Containers_DEN_RunGtoH[iCut]->Save( f_output );
        vec_Nminus1Containers_NUM_RunBtoF[iCut]->Save( f_output );
        vec_Nminus1Containers_NUM_RunGtoH[iCut]->Save( f_output );
      }
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
        && (mu.AbsTrkIso / mu.Inner_pT) < 0.1    //Iso
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
        && mu.trackerLayersGLB > 5    //mu.trackerLayersGLB > 5
        && mu.pixelHitsGLB > 0    //mu.pixelHitsGLB > 0
        && mu.muonHits > 0
        && (  mu.nMatches > 1
          || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
          || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
        && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3
        && (mu.AbsTrkIso / mu.Inner_pT) < 0.1    //Iso
      )
    {
      return 1;
    }
    return 0;
  }

  Bool_t isTag( Muon mu )
  {
    if(    mu.isGLB == 1
        && mu.isTRK == 1
        && mu.TuneP_pT > 53
        && fabs(mu.eta) < 2.4

        && fabs(mu.dB) < 0.2
        && mu.trackerLayersGLB > 5    //mu.trackerLayersGLB > 5
        && mu.pixelHitsGLB > 0    //mu.pixelHitsGLB > 0
        && mu.muonHits > 0
        && (  mu.nMatches > 1
          || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
          || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
        && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3
        && (mu.AbsTrkIso / mu.Inner_pT) < 0.1    //Iso
      )
    {
      return 1;
    }
    return 0;
  }

  Bool_t isMuon_Nminus1_NUM( Muon mu, Int_t theCut)   //Min
  {
    Bool_t isPassExceptTheCut = kFALSE;

    if (theCut == 0 ) {
      if (
               fabs(mu.dB) < 0.2                            // theCut = 1
            && mu.trackerLayersGLB > 5                         // theCut = 2
            && mu.pixelHitsGLB > 0                             // theCut = 3
            && mu.muonHits > 0                              // theCut = 4
            && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            && (  mu.nMatches > 1                           // theCut = 6
              || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
              || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    else if (theCut == 1 ) {
      if (
            //fabs(mu.dB) < 0.2                            // theCut = 1
               mu.trackerLayersGLB > 5                         // theCut = 2
            && mu.pixelHitsGLB > 0                             // theCut = 3
            && mu.muonHits > 0                              // theCut = 4
            && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            && (  mu.nMatches > 1                           // theCut = 6
              || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
              || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    else if (theCut == 2 ) {
      if (
               fabs(mu.dB) < 0.2                            // theCut = 1
            //&& mu.trackerLayersGLB > 5                         // theCut = 2
            && mu.pixelHitsGLB > 0                             // theCut = 3
            && mu.muonHits > 0                              // theCut = 4
            && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            && (  mu.nMatches > 1                           // theCut = 6
              || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
              || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    else if (theCut == 3 ) {
      if (
               fabs(mu.dB) < 0.2                            // theCut = 1
            && mu.trackerLayersGLB > 5                         // theCut = 2
            //&& mu.pixelHitsGLB > 0                             // theCut = 3
            && mu.muonHits > 0                              // theCut = 4
            && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            && (  mu.nMatches > 1                           // theCut = 6
              || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
              || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    else if (theCut == 4 ) {
      if (
               fabs(mu.dB) < 0.2                            // theCut = 1
            && mu.trackerLayersGLB > 5                         // theCut = 2
            && mu.pixelHitsGLB > 0                             // theCut = 3
            //&& mu.muonHits > 0                              // theCut = 4
            && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            && (  mu.nMatches > 1                           // theCut = 6
              || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
              || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    else if (theCut == 5 ) {
      if (
               fabs(mu.dB) < 0.2                            // theCut = 1
            && mu.trackerLayersGLB > 5                         // theCut = 2
            && mu.pixelHitsGLB > 0                             // theCut = 3
            && mu.muonHits > 0                              // theCut = 4
            //&& (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            && (  mu.nMatches > 1                           // theCut = 6
              || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
              || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    else if (theCut == 6 ) {
      if (
               fabs(mu.dB) < 0.2                            // theCut = 1
            && mu.trackerLayersGLB > 5                         // theCut = 2
            && mu.pixelHitsGLB > 0                             // theCut = 3
            && mu.muonHits > 0                              // theCut = 4
            && (mu.TuneP_pTError / mu.TuneP_pT ) < 0.3      // theCut = 5
            //&& (  mu.nMatches > 1                           // theCut = 6
            //  || ( mu.nMatches==1 && !(mu.stationMask==1 || mu.stationMask==16) )
            //  || ( mu.nMatches==1 && (mu.stationMask==1 || mu.stationMask==16) && mu.nMatchesRPCLayers > 2 )  )
          )
      {
        isPassExceptTheCut = kTRUE;
      }
    }

    if(    mu.isGLB == 1
        && mu.isTRK == 1
        && mu.TuneP_pT > 53
        && fabs(mu.eta) < 2.4
        && isPassExceptTheCut
      )
    {
      return 1;
    }
    return 0;
  }

  Bool_t EventSelection_Zprime_Nminus1( Int_t theCut, NtupleHandle* ntuple, DYAnalyzer* analyzer, vector<Muon> &MuonCollection, MuPair &SelectedPair, pair<Bool_t, Bool_t> &SelectedPairTag, pair<Bool_t, Bool_t> &SelectedPairPass )
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

          Bool_t Flag_Zwindow = kTRUE;  // if isZwindow=kFALSE
          if (this->isZwindow) {
            Flag_Zwindow = kFALSE;
            if ( pair_temp.M > 70 && pair_temp.M < 130 ) Flag_Zwindow = kTRUE;
          }

          Bool_t Flag_dRmumu = kFALSE;
          Double_t minDR = 0.6;
          if ( pair_temp.deltaR > minDR ) Flag_dRmumu = kTRUE;

          //Bool_t Flag_B2B = kFALSE;
          //Double_t pi = 3.141592; Double_t dPhim = 0.7*pi; Double_t dPhiM = 1.3*pi;
          //Double_t dPhi = fabs(pair_temp.First.phi - pair_temp.Second.phi);
          //if ( (dPhi < dPhiM) && (dPhi > dPhim) ) Flag_B2B = kTRUE;

          /*Verbose*///cout << "Flag_Mass :" << Flag_Mass << endl;
          /*Verbose*///cout << "Flag_Acc :" << Flag_Acc << endl;
          /*Verbose*///cout << "Flag_3DAngle :" << Flag_3DAngle << endl;
          /*Verbose*///cout << "Flag_Vtx :" << Flag_Vtx << endl;
          /*Verbose*///cout << "Flag_OS :" << Flag_OS << endl;
          /*Verbose*///cout << "Flag_PtRatio :" << Flag_PtRatio << endl;
          /*Verbose*///cout << "Flag_B2B :" << Flag_B2B << endl;

          if( Flag_Mass &&
            //Flag_TrigMatched &&
            Flag_Acc &&
            Flag_3DAngle &&
            Flag_Vtx &&
            Flag_OS &&
            Flag_PtRatio &&
            Flag_Zwindow &&
            Flag_dRmumu
            //Flag_B2B
            )
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
        Bool_t tag1 = false;
        Bool_t tag2 = false;
        if ( isTag(SelectedPair.First) ) tag1 = true;
        if ( isTag(SelectedPair.Second) ) tag2 = true;
        SelectedPairTag = make_pair( tag1, tag2 );

        Bool_t p1 = false;
        Bool_t p2 = false;
        if ( isMuon_Nminus1_NUM(SelectedPair.First, theCut) ) p1 = true;
        if ( isMuon_Nminus1_NUM(SelectedPair.Second, theCut) ) p2 = true;
        SelectedPairPass = make_pair( p1, p2 );
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
void ProdHist_Systematic_Eff_Nminus1_v1(Int_t _isMC, Int_t _isZwindow, TString _FileName_ROOTFileList, Double_t _NormFactor )
{
  HistogramProducer *producer = new HistogramProducer( _isMC, _isZwindow, _FileName_ROOTFileList, _NormFactor );
  producer->Producer();
}

