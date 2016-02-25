
void plot()
{

  gStyle->SetOptStat(0);
  gROOT->ProcessLine(".L ../../tools/adamStyles.C");
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");
  gROOT->ForceStyle(true);

  gROOT->LoadMacro("DY.C+");
  //gROOT->ProcessLine(".L DY.C");

  units_with_c    = false;  // set false to remove the c's
  salamanderColors = true;
  GHMStyle = true;  // set false for original colors

  bool drawFig1   = false; // data vs mc before acc correction
//  bool drawFig1_TT = false; //data driven TT vs MC
//  bool drawFig1_QCD = false; //data driven vs MC QCD 
  bool drawFig2   = false; // acc in muon
//  bool drawFig2_vsTP = false; //MC truth vs T&P from MC and Data
  bool drawFig3   = false; // acc in electron
  bool drawFig4   = false; // eff on reco, trig
  bool drawFig5   = false; // eff on iso
  bool drawFig6   = false; // unfolding matrix
  bool drawFig7   = false; // FSR
  bool drawFig8   = true; // final plot
  bool drawFig9   = false;  // data vs mc after acc correction
  bool drawFig10   = false;  // eff in electron
  bool drawFig11   = false; // final plot with histogram type

  if( drawFig9 )
    {
      MassCorr( Fig_9_a )->Print(".pdf");
      MassCorr( Fig_9_b )->Print(".pdf");
    }
  if( drawFig1 )
    {
      Mass( Fig_1_a )->Print(".png");
      Mass( Fig_1_a )->Print(".pdf");

      //Mass( Fig_1_b )->Print(".pdf");
    }
  if( drawFig2 )
    {
      Acceptance( Fig_2 )->Print(".pdf");
      Acceptance( Fig_2 )->Print(".png");
      //Acceptance( Fig_2_a )->Print(".pdf");
      //Acceptance( Fig_2_b )->Print(".pdf");
    }
  if( drawFig3 )
    {
      Acceptance( Fig_3 )->Print(".pdf");
      Acceptance( Fig_3 )->Print(".png");
      //Acceptance( Fig_3_a )->Print(".pdf");
      //Acceptance( Fig_3_b )->Print(".pdf");
    }
  if( drawFig10 )
    {
      Efficiency_Electron( Fig_10_a )->Print(".pdf");
      Efficiency_Electron( Fig_10_b )->Print(".pdf");
      Efficiency_Electron( Fig_10_c )->Print(".pdf");
      Efficiency_Electron( Fig_10_d )->Print(".pdf");
      Efficiency_Electron( Fig_10_e )->Print(".pdf");
      Efficiency_Electron( Fig_10_f )->Print(".pdf");
    }
  if( drawFig4 )
    {
      Efficiency( Fig_4_a )->Print(".pdf");
      Efficiency( Fig_4_b )->Print(".pdf");
    }
  if( drawFig5 )
    {
      Efficiency( Fig_5   )->Print(".pdf");
    }
  if( drawFig6 )
    {
      grayScale = false;
      drawGrid  = true;
      logScaleZ  = true;
      UnfoldingMatrix( Fig_6_a )->Print(".pdf");
      //UnfoldingMatrix( Fig_6_b )->Print(".pdf");
    }
  if( drawFig7 )
    {
      FSR( Fig_7_a )->Print(".pdf");
      FSR( Fig_7_b )->Print(".pdf");
    }
  if( drawFig8 )
    {
      //data_vals2, hardcoded
      Theory(false)->Print(".pdf");
      Theory(false)->Print(".png");
    }
  if( drawFig11 )
    {
      Theory_Hist()->Print(".pdf");
      Theory_Hist()->Print(".png");
    }
}
