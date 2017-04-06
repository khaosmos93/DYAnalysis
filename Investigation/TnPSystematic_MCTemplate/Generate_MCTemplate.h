#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>
#include <iostream>

#define nPtBin 4
#define nEtaBin 5

class TnPTree
{
public:
	TChain *chain;
	Double_t mass;
	Double_t pt;
	Double_t eta;
	Double_t weight;

	Int_t tag_IsoMu20;
	Int_t tag_pt;
	Int_t tag_eta;

	Int_t IDFlag_HighPtMuon;
	Double_t RelTrkIso;
	Int_t IsoMu20;
	Int_t IsoTkMu20;

	Int_t mcTrue;

	TnPTree()
	{

	}

	TnPTree( TChain *_chain ): TnPTree()
	{
		this->chain = _chain;

		this->chain->SetBranchStatus("*", 0);

		this->chain->SetBranchStatus("mass", 1);
		this->chain->SetBranchAddress("mass", &mass);

		this->chain->SetBranchStatus("pt", 1);
		this->chain->SetBranchAddress("pt", &pt);

		this->chain->SetBranchStatus("eta", 1);
		this->chain->SetBranchAddress("eta", &eta);

		this->chain->SetBranchStatus("weight", 1);
		this->chain->SetBranchAddress("weight", &weight);

		this->chain->SetBranchStatus("tag_IsoMu20", 1);
		this->chain->SetBranchAddress("tag_IsoMu20", &tag_IsoMu20);

		this->chain->SetBranchStatus("tag_pt", 1);
		this->chain->SetBranchAddress("tag_pt", &tag_pt);

		this->chain->SetBranchStatus("tag_eta", 1);
		this->chain->SetBranchAddress("tag_eta", &tag_eta);

		this->chain->SetBranchStatus("IDFlag_HighPtMuon", 1);
		this->chain->SetBranchAddress("IDFlag_HighPtMuon", &IDFlag_HighPtMuon);

		this->chain->SetBranchStatus("RelTrkIso", 1);
		this->chain->SetBranchAddress("RelTrkIso", &RelTrkIso);

		this->chain->SetBranchStatus("IsoMu20", 1);
		this->chain->SetBranchAddress("IsoMu20", &IsoMu20);

		this->chain->SetBranchStatus("IsoTkMu20", 1);
		this->chain->SetBranchAddress("IsoTkMu20", &IsoTkMu20);

		this->chain->SetBranchStatus("mcTrue", 1);
		this->chain->SetBranchAddress("mcTrue", &mcTrue);

		// this->chain->SetBranchStatus("asdf", 1);
		// this->chain->SetBranchAddress("asdf", &asdf);
	}

	void GetPair( Int_t i_event )
	{
		chain->GetEntry( i_event );
	}
};

class MCTemplateTool
{
public:
	TString FileLocation;
	TString Type;
	Double_t PtBinEdges[nPtBin+1] = {10, 22, 40, 70, 250};
	Double_t EtaBinEdges[nEtaBin+1] = {-2.4, -1.2, -0.3, 0.3, 1.2, 2.4};

	TH1D* h_Pass[nPtBin][nEtaBin];
	TH1D* h_Fail[nPtBin][nEtaBin];

	MCTemplateTool()
	{
		cout << "Generate MC templates for TnP signal alternative fit" << endl;
	}

	MCTemplateTool( TString _Type ): MCTemplateTool()
	{
		this->Type = _Type;
		if( this->Type == "ID" || this->Type == "Iso" )
			this->FileLocation = "/home/kplee/data/TagProbe/76X/v20160425_First76XTrees/";
		else if( this->Type == "HLTv4p2" )
			this->FileLocation = "/home/kplee/data/TagProbe/76X/v20160425_First76XTrees/HLTv4p2/";
		else if( this->Type == "HLTv4p3" )
			this->FileLocation = "/home/kplee/data/TagProbe/76X/v20160425_First76XTrees/HLTv4p3/";
		else
			this->FileLocation = "";

		cout << "\t[Type = " << this->Type << "] Ntuple location: " << this->FileLocation << endl;
		this->Init_AllHist();
	}

	~MCTemplateTool()
	{
		for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
		{
			for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
			{
				delete this->h_Pass[i_pt][i_eta];
				delete this->h_Fail[i_pt][i_eta];
			}
		}
	}

	void Produce( Int_t nTestEvent )
	{
		TChain *chain = new TChain("muonEffs/fitter_tree");
		chain->Add(this->FileLocation + "*.root");

		TnPTree *ntuple = new TnPTree( chain );

		Int_t nTotEvent = chain->GetEntries();
		if( nTestEvent > 0 )
			nTotEvent = nTestEvent;
		
		for(Int_t i_ev=0; i_ev<nTotEvent; i_ev++)
		{
			ntuple->GetPair( i_ev );

			if( ntuple->tag_IsoMu20 == 1 &&
				ntuple->tag_pt > 22 &&
				fabs(ntuple->tag_eta) > 2.4 &&
				ntuple->mass > 70 && 
				ntuple->mass < 130 && 
				ntuple->mcTrue == 1 ) // -- common conditions -- //
			{
				if( this->Type == "ID" )
					this->Fill_ID( ntuple );

				else if( this->Type == "Iso" )
					this->Fill_Iso( ntuple );

				else if( this->Type == "HLTv4p2" || this->Type == "HLTv4p3" )
					this->Fill_Trig( ntuple );
			}
		}

		this->Remove_NegativeBins_All();
		cout << "Generation of MC Templates for " << this->Type << " is done" << endl;

		this->Save_All();
	}

protected:
	void Fill_ID( TnPTree* ntuple )
	{
		Bool_t Flag_Pass = kFALSE;
		if( ntuple->IDFlag_HighPtMuon == 1 )
			Flag_Pass = kTRUE;
		else
			Flag_Pass = kFALSE;

		this->FillHist( Flag_Pass, ntuple );
	}

	void Fill_Iso( TnPTree* ntuple )
	{
		if( ntuple->IDFlag_HighPtMuon == 1 ) // -- among probes passing ID -- //
		{
			Bool_t Flag_Pass = kFALSE;
			if( ntuple->RelTrkIso < 0.10 )
				Flag_Pass = kTRUE;
			else
				Flag_Pass = kFALSE;

			this->FillHist( Flag_Pass, ntuple );
		}
	}

	void Fill_Trig( TnPTree* ntuple )
	{
		if( ntuple->IDFlag_HighPtMuon == 1 && 
			ntuple->RelTrkIso < 0.10 ) // -- among probes passing ID & Iso -- //
		{
			Bool_t Flag_Pass = kFALSE;
			if( ntuple->IsoMu20  == 1 || ntuple->IsoTkMu20  == 1 )
				Flag_Pass = kTRUE;
			else
				Flag_Pass = kFALSE;

			this->FillHist( Flag_Pass, ntuple );
		}
	}

	void FillHist( Bool_t Flag_Pass, TnPTree *ntuple )
	{
		Int_t PtBin = this->Find_PtBin( ntuple->pt );
		Int_t EtaBin = this->Find_EtaBin( ntuple->eta );

		// -- if pt or eta is out of range of the binning,
		// -- it returns -1: they should not be included in MC template -- //
		if( PtBin != -1 && EtaBin != -1 )
		{
			if( Flag_Pass )
				this->h_Pass[PtBin][EtaBin]->Fill( ntuple->mass, ntuple->weight );
			else
				this->h_Fail[PtBin][EtaBin]->Fill( ntuple->mass, ntuple->weight );
		}
	}

	Int_t Find_PtBin( Double_t pt )
	{
		Int_t PtBin = -1;
		for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
		{
			if( pt > this->PtBinEdges[i_pt] && pt < this->PtBinEdges[i_pt+1] )
			{
				PtBin = i_pt;
				break;
			}
		}

		return PtBin;
	}

	Int_t Find_EtaBin( Double_t eta )
	{
		Int_t EtaBin = -1;
		for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
		{
			if( eta > this->EtaBinEdges[i_eta] && eta < this->EtaBinEdges[i_eta+1] )
			{
				EtaBin = i_eta;
				break;
			}
		}

		return EtaBin;
	}

	void Remove_NegativeBins_All()
	{
		for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
		{
			for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
			{
				this->Remove_NegativeBins( this->h_Pass[i_pt][i_eta] );
				this->Remove_NegativeBins( this->h_Fail[i_pt][i_eta] );
			}
		}

	}

	void Remove_NegativeBins( TH1D* h )
	{
		Int_t nBin = h->GetNbinsX();
		for(Int_t i=0; i<nBin; i++)
		{
			Int_t i_bin = i+1;
			Double_t entry = h->GetBinContent(i_bin);

			// -- if there's a negative bin in MC template (because of gen-weight), fitting will not work -- //
			if( entry < 0 )
			{
				h->SetBinContent(i_bin, 0);
				h->SetBinError(i_bin, 0);
			}
		}
	}

	void Init_AllHist()
	{
		for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
		{
			for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
			{
				TString BinName = TString::Format("PtBin%d_EtaBin%d", i_pt, i_eta);
				this->h_Pass[i_pt][i_eta] = new TH1D("h_Pass_"+BinName, "", 60, 70, 130);
				this->h_Fail[i_pt][i_eta] = new TH1D("h_Fail_"+BinName, "", 60, 70, 130);
			}
		}

		cout << "Initialization of histograms is completed" << endl;
	}

	void Save_All()
	{
		TString OutputName = TString::Format("ROOTFile_MCTemplates_%s.root", this->Type.Data());

		TFile *f_output = TFile::Open(OutputName, "RECREATE");
		f_output->cd();

		for(Int_t i_pt=0; i_pt<nPtBin; i_pt++)
		{
			for(Int_t i_eta=0; i_eta<nEtaBin; i_eta++)
			{
				this->h_Pass[i_pt][i_eta]->Write();
				this->h_Fail[i_pt][i_eta]->Write();
			}
		}

		f_output->Close();
		cout << "All histograms are saved in " << OutputName << endl;
	}
};