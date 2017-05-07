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
#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>

#include <Include/DYAnalyzer.h>
#include <fstream>

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

void Calc_SumWeights()
{
	TStopwatch totaltime;
	totaltime.Start();

	ofstream outFile("log_Calc_SumWeights.txt", ios::out);

	DYAnalyzer *analyzer = new DYAnalyzer( "None" );

	TString NtuplePath = gSystem->Getenv("KP_DATA_PATH");

	vector< TString > vec_NtupleDir; vector< TString > vec_Tag;

	vec_NtupleDir.push_back("DYntuple_v20170504_76X_MINIAODv2_PDFWeights_DYLL_M10to50_25ns"); vec_Tag.push_back("DYMuMu_M10to50");

	const Int_t nNtuple = vec_NtupleDir.size();
	for(Int_t i_ntuple = 0; i_ntuple<nNtuple; i_ntuple++)
	{
		TStopwatch looptime;
		looptime.Start();

		TString NtupleDir = vec_NtupleDir[i_ntuple];
		TString Tag = vec_Tag[i_ntuple];

		outFile << "<" << Tag << ">" << endl;

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(NtuplePath+"/"+NtupleDir+"/ntuple_*.root");

		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_GenLepton();

		Int_t nEvent = chain->GetEntries();
		outFile << "\t[Total Events: " << nEvent << "]" << endl;

		Double_t SumWeight_All = 0;
		Double_t SumWeight_Separated = 0;

		// nEvent = 10000;
		for(Int_t i=0; i<nEvent; i++)
		{
			loadBar(i+1, nEvent, 100, 100);
			
			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

			SumWeight_All += GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag, ntuple);

			if( GenFlag == kTRUE )
				SumWeight_Separated += GenWeight;

		} // -- end of event iteration -- //

		outFile << TString::Format("\t[Sum of weights: (total, separated) = (%.1lf, %.1lf)]\n", SumWeight_All, SumWeight_Separated) << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		outFile << "\tLoop RunTime(" << Tag << "): " << LoopRunTime << " seconds\n" << endl;

	} // -- end of sample iteration -- //

	outFile.close();

	Double_t TotalRunTime = totaltime.CpuTime();
	outFile << "Total RunTime: " << TotalRunTime << " seconds" << endl;
}