void fracEff( void ) 
{
  gStyle->SetOptStat(0);
  gROOT->ProcessLine(".L setTDRStyle.C");
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");
  gROOT->ForceStyle(true);
  tdrStyle->SetTitleX(0.50);
  tdrStyle->SetTitleY(0.92);

  TFile* f3 = new TFile("inputCorrinMC_tmp.root");
  f3->cd();
  h1DFrac0->Sumw2();
  h1DFrac1->Sumw2();
  h1DFrac2->Sumw2();
  hFrac0->Sumw2();
  hFrac1->Sumw2();
  hFrac2->Sumw2();

  TH1D* hfrac1D1 = (TH1D*) h1DFrac0->Clone();
  TH1D* hfrac1D2 = (TH1D*) h1DFrac0->Clone();
  TH1D* hfrac1D3 = (TH1D*) h1DFrac0->Clone();
  TH1D* hfrac1D4 = (TH1D*) h1DFrac0->Clone();

  TH2D* hfrac2D1 = (TH2D*) hFrac0->Clone();
  TH2D* hfrac2D2 = (TH2D*) hFrac0->Clone();
  TH2D* hfrac2D3 = (TH2D*) hFrac0->Clone();
  TH2D* hfrac2D4 = (TH2D*) hFrac0->Clone();

  hfrac1D1->Divide(h1DFrac1, h1DFrac0);
  hfrac1D2->Divide(h1DFrac2, h1DFrac0);

  hfrac2D1->Divide(hFrac1, hFrac0);
  hfrac2D2->Divide(hFrac2, hFrac0);

  TH1D* htmp1D1 = (TH1D*) h1DFrac0->Clone();
  TH1D* htmp1D2 = (TH1D*) h1DFrac0->Clone();
  htmp1D1->Add(h1DFrac1, 1);
  htmp1D2->Add(h1DFrac2, 1);
  hfrac1D3->Divide(htmp1D1, h1DFrac0);
  hfrac1D4->Divide(h1DFrac0, htmp1D2);

  TH2D* htmp2D1 = (TH2D*) hFrac0->Clone();
  TH2D* htmp2D2 = (TH2D*) hFrac0->Clone();
  htmp2D1->Add(hFrac1, 1);
  htmp2D2->Add(hFrac2, 1);
  hfrac2D3->Divide(htmp2D1, hFrac0);
  hfrac2D4->Divide(hFrac0, htmp2D2);

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  c1->cd();
  c1->SetLogx();
  hfrac1D1->SetTitle("");
  hfrac1D1->GetXaxis()->SetTitle("M_{#mu#mu} GeV");
  hfrac1D1->GetYaxis()->SetTitle("Fraction");
  hfrac1D1->GetXaxis()->SetMoreLogLabels();
  hfrac1D1->Draw("hist");
  //c1->Print("./eps/hfrac1D1.png");

  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->cd();
  c2->SetLogx();
  hfrac1D2->SetTitle("");
  hfrac1D2->GetXaxis()->SetTitle("M_{#mu#mu} GeV");
  hfrac1D2->GetYaxis()->SetTitle("Fraction");
  hfrac1D2->GetXaxis()->SetMoreLogLabels();
  hfrac1D2->Draw("hist");
  //c2->Print("./eps/hfrac1D2.png");

  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
  c3->cd();
  c3->SetLogx();
  hfrac2D1->SetTitle("");
  hfrac2D1->GetXaxis()->SetTitle("M_{#mu#mu} GeV");
  hfrac2D1->GetYaxis()->SetTitle("Fraction");
  hfrac2D1->GetXaxis()->SetMoreLogLabels();
  hfrac2D1->Draw("COLZ");
  //c3->Print("./eps/hfrac2D1.png");

  TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
  c4->cd();
  c4->SetLogx();
  hfrac2D2->SetTitle("");
  hfrac2D2->GetXaxis()->SetTitle("M_{#mu#mu} GeV");
  hfrac2D2->GetYaxis()->SetTitle("Fraction");
  hfrac2D2->GetXaxis()->SetMoreLogLabels();
  hfrac2D2->Draw("COLZ");
  //c4->Print("./eps/hfrac2D2.png");

  TH1D* heff2D_1 = new TH1D("heff2D_1", "heff2D_1", 132, 0, 132);
  TH1D* heff2D_2 = new TH1D("heff2D_2", "heff2D_2", 132, 0, 132);
  for( int i = 0; i < 6; i++ ) {
    for( int j = 0; j < 24; j++ ) {
      if( i == 5 && j > 12 ) continue;

      heff2D_1->SetBinContent(24*i+j+1, hfrac2D3->GetBinContent(i+1, j+1));
      heff2D_1->SetBinError(24*i+j+1, hfrac2D3->GetBinError(i+1, j+1));

      heff2D_2->SetBinContent(24*i+j+1, hfrac2D4->GetBinContent(i+1, j+1));
      heff2D_2->SetBinError(24*i+j+1, hfrac2D4->GetBinError(i+1, j+1));
    }
  }

  TFile* _outFile = new TFile("addMoreCorrinMC_tmp.root", "recreate");
  _outFile->cd();
  hfrac2D3->Write("hfrac2D_1");
  hfrac2D4->Write("hfrac2D_2");
  hfrac1D3->Write("heff1D_1");
  hfrac1D4->Write("heff1D_2");
  heff2D_1->Write("heff2D_1");
  heff2D_2->Write("heff2D_2");
  _outFile->Write();
}
