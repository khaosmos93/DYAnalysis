void yield( void )
{
   double mass_xbin[41] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
       96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
           200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

  gStyle->SetOptStat(0);
  TFile* f = new TFile("../Inputs/unfold/unfoldData_HLT_Double_POWHEG_TRM.root");
  f->cd();
  TH1D* hafter = (TH1D*)gDirectory->Get("hunfolded");
  TH1D* hbefore = (TH1D*)gDirectory->Get("htrue");

  //put ratio plot
  TH1D* ratio = (TH1D*)hafter->Clone();
  ratio->Divide(hafter,hbefore);

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  c1->cd();
  c1->SetLogy();
  hafter->GetXaxis()->SetTitle("Bin number");
  hafter->GetYaxis()->SetTitle("Number of Events");
  hafter->GetXaxis()->SetMoreLogLabels();
  hafter->SetLineColor(2);
  hafter->SetMarkerStyle(20);
  hafter->SetMarkerColor(2);
  hafter->Draw("ep");
  hbefore->SetMarkerStyle(20);
  hbefore->SetMarkerSize(0.5);
  hbefore->Draw("same");

  TLegend* leg = new TLegend(0.65, 0.65, 0.85, 0.85);
  leg->SetTextFont(72);
  leg->SetTextSize(0.060);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(hafter, "Unfolded", "PL");
  leg->AddEntry(hbefore, "Pre-unfolded", "PL");
  leg->SetFillColor(0);
  leg->Draw("0");
  c1->Print("unfold.png");

  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->cd();
  c2->SetLogy();
  ratio->GetXaxis()->SetTitle("Bin number");
  ratio->GetYaxis()->SetTitle("Pre-unfolded/unfolded");
  ratio->GetXaxis()->SetMoreLogLabels();
  ratio->SetMarkerStyle(20);
  ratio->Draw("ep");
  c2->Print("unfold_ratio.png");

}
