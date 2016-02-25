void plot(void) {

   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(1111);

   //gStyle->SetEndErrorSize(2);
   //gStyle->SetErrorX(0.5);

  TFile* f = new TFile("1.root"); //../../FEWZValidation/rootfiles/DYM_40mass8rap_NEWBIN_AND_TUNE.root");
 
  for (int i = 0; i < 40; i++) {
   ostringstream istr;
   istr << i+1;
   f->cd();
   TH1D* h = (TH1D*)gDirectory->Get("validFEWZ"+TString(istr.str().c_str()));
   cout << h->GetMean(1) << ", ";
  }
 
  for (int i = 0; i < 40; i++) {
   ostringstream istr;   
   istr << i+1;
   TCanvas* c = new TCanvas(TString(istr.str().c_str()));
   c->cd();
   c->SetLogy();
   f->cd();
   TH1D* h = (TH1D*)gDirectory->Get("validFEWZ"+TString(istr.str().c_str()));
   TH1D* h2 = (TH1D*)gDirectory->Get("validFEWZ"+TString(istr.str().c_str()));
   h2->SetMinimum(10);
   h->SetLineColor(kBlue);
   //h->SetFillColor(kBlue);
   h2->GetXaxis()->SetTitle("Weight");
   h2->GetYaxis()->SetTitle("Number of events");  
   h2->Draw("");
   h->Draw("Esame");
   c->SaveAs("Weight"+TString(istr.str().c_str())+".png");
  }


} 
