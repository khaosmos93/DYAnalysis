#include <TH1D.h>
#include <TLorentzVector.h>
#include <TFile.h>

#include <vector>

// #include <Lepton.h>

class ControlPlots
{
public:
	std::vector<TH1D*> Histo;

	TH1D *h_Pt;
	TH1D *h_eta;
	TH1D *h_phi;
	TH1D *h_mass;
	TH1D *h_diPt;
	TH1D *h_diRap;

	TH1D *h_absdiRap_M20to30;
	TH1D *h_absdiRap_M30to45;
	TH1D *h_absdiRap_M45to60;
	TH1D *h_absdiRap_M60to120;
	TH1D *h_absdiRap_M120to200;
	TH1D *h_absdiRap_M200to1500;

	TH1D *h_lead_Pt;
	TH1D *h_sub_Pt;
	
	TH1D *h_lead_eta;
	TH1D *h_sub_eta;

	TH1D *h_lead_phi;
	TH1D *h_sub_phi;

	TH1D *h_barrel_Pt;
	TH1D *h_barrel_eta;
	TH1D *h_barrel_phi;
	TH1D *h_mass_barrel;

	TH1D *h_mass_OS;
	TH1D *h_mass_SS;

	TH1D *h_Pt_minusCharge;
	TH1D *h_Pt_plusCharge;

	TH1D *h_Pt_M70;
	TH1D *h_Pt_M70to90;
	TH1D *h_Pt_M90to110;
	TH1D *h_Pt_M110toInf;

	TH1D *h_Angle;

	TH1D *h_Pt_TrigLeg;
	TH1D *h_eta_TrigLeg;
	TH1D *h_phi_TrigLeg;

	TH1D *h_Pt_OtherLeg;
	TH1D *h_eta_OtherLeg;
	TH1D *h_phi_OtherLeg;

	TH1D *h_GenMass;

	ControlPlots(TString Type)
	{
		h_Pt = new TH1D("h_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt );
		h_eta = new TH1D("h_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_eta );
		h_phi = new TH1D("h_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_phi );
		h_mass = new TH1D("h_mass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass );
		h_diPt = new TH1D("h_diPt_"+Type, "", 250, 0, 500); Histo.push_back( h_diPt );
		h_diRap = new TH1D("h_diRap_"+Type, "", 60, -3, 3); Histo.push_back( h_diRap );

		h_absdiRap_M20to30 = new TH1D("h_absdiRap_M20to30_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M20to30 );
		h_absdiRap_M30to45 = new TH1D("h_absdiRap_M30to45_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M30to45 );
		h_absdiRap_M45to60 = new TH1D("h_absdiRap_M45to60_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M45to60 );
		h_absdiRap_M60to120 = new TH1D("h_absdiRap_M60to120_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M60to120 );
		h_absdiRap_M120to200 = new TH1D("h_absdiRap_M120to200_"+Type, "", 24, 0, 2.4); Histo.push_back( h_absdiRap_M120to200 );
		h_absdiRap_M200to1500 = new TH1D("h_absdiRap_M200to1500_"+Type, "", 12, 0, 2.4); Histo.push_back( h_absdiRap_M200to1500 );

		h_lead_Pt = new TH1D("h_lead_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_lead_Pt );
		h_sub_Pt = new TH1D("h_sub_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_sub_Pt );
		
		h_lead_eta = new TH1D("h_lead_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_lead_eta );
		h_sub_eta = new TH1D("h_sub_eta_"+Type, "", 60, -3, 3); Histo.push_back( h_sub_eta );

		h_lead_phi = new TH1D("h_lead_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_lead_phi );
		h_sub_phi = new TH1D("h_sub_phi_"+Type, "", 80, -4, 4); Histo.push_back( h_sub_phi );

		h_barrel_Pt = new TH1D("h_barrel_Pt_"+Type, "", 250, 0, 500); Histo.push_back( h_barrel_Pt );
		h_barrel_eta = new TH1D("h_barrel_eta_"+Type, "",  60, -3, 3); Histo.push_back( h_barrel_eta );
		h_barrel_phi = new TH1D("h_barrel_phi_"+Type, "",  80, -4, 4); Histo.push_back( h_barrel_phi );
		h_mass_barrel = new TH1D("h_mass_barrel_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_barrel );

		h_mass_OS = new TH1D("h_mass_OS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_OS );
		h_mass_SS = new TH1D("h_mass_SS_"+Type, "", 10000, 0, 10000); Histo.push_back( h_mass_SS );

		h_Pt_minusCharge = new TH1D("h_Pt_minusCharge_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_minusCharge );
		h_Pt_plusCharge = new TH1D("h_Pt_plusCharge_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_plusCharge );

		h_Pt_M70 = new TH1D("h_Pt_M70_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_M70 );
		h_Pt_M70to90 = new TH1D("h_Pt_M70to90_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_M70to90 );
		h_Pt_M90to110 = new TH1D("h_Pt_M90to110_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_M90to110 );
		h_Pt_M110toInf = new TH1D("h_Pt_M110toInf_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_M110toInf );

		h_Angle = new TH1D("h_Angle_"+Type, "", 80, -4, 4); Histo.push_back( h_Angle );

		h_Pt_TrigLeg = new TH1D("h_Pt_TrigLeg_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_TrigLeg );
		h_eta_TrigLeg = new TH1D("h_eta_TrigLeg_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_TrigLeg );
		h_phi_TrigLeg = new TH1D("h_phi_TrigLeg_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_TrigLeg );

		h_Pt_OtherLeg = new TH1D("h_Pt_OtherLeg_"+Type, "", 250, 0, 500); Histo.push_back( h_Pt_OtherLeg );
		h_eta_OtherLeg = new TH1D("h_eta_OtherLeg_"+Type, "", 60, -3, 3); Histo.push_back( h_eta_OtherLeg );
		h_phi_OtherLeg = new TH1D("h_phi_OtherLeg_"+Type, "", 80, -4, 4); Histo.push_back( h_phi_OtherLeg );

		h_GenMass = new  TH1D("h_GenMass_"+Type, "", 10000, 0, 10000); Histo.push_back( h_GenMass );
	}

	void FillHistograms_GenMu(NtupleHandle *ntuple, GenLepton genlep1, GenLepton genlep2)
	{
		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		TLorentzVector gen_v1 = genlep1.Momentum;
		TLorentzVector gen_v2 = genlep2.Momentum;
		Double_t gen_M = (gen_v1 + gen_v2).M();

		h_GenMass->Fill( gen_M, GenWeight );
	}

	void FillHistograms_SingleMu(NtupleHandle *ntuple, TString HLT, Muon recolep)
	{
		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		//-- Pt ,eta, phi --//
		h_Pt->Fill( recolep.Pt, GenWeight );
		h_eta->Fill( recolep.eta, GenWeight );
		h_phi->Fill( recolep.phi, GenWeight );

		//-- Barrel muons --//
		if( fabs(recolep.eta) < 0.9 )
		{
			h_barrel_Pt->Fill( recolep.Pt, GenWeight );
			h_barrel_eta->Fill( recolep.eta, GenWeight );
			h_barrel_phi->Fill( recolep.phi, GenWeight );
		}

		//-- Pt plots for negative/positive charged muons --//
		if( recolep.charge == -1 )
			h_Pt_minusCharge->Fill( recolep.Pt, GenWeight );
		else if( recolep.charge == 1 )
			h_Pt_plusCharge->Fill( recolep.Pt, GenWeight );

		//-- Trigger Matched objects --//
		if( recolep.isTrigMatched(ntuple, HLT) )
		{
			h_Pt_TrigLeg->Fill( recolep.Pt, GenWeight );
			h_eta_TrigLeg->Fill( recolep.eta, GenWeight );
			h_phi_TrigLeg->Fill( recolep.phi, GenWeight );
		}
		else
		{
			h_Pt_OtherLeg->Fill( recolep.Pt, GenWeight );
			h_eta_OtherLeg->Fill( recolep.eta, GenWeight );
			h_phi_OtherLeg->Fill( recolep.phi, GenWeight );
		}

	}

	void FillHistograms_DoubleMu(NtupleHandle *ntuple, TString HLT, Muon recolep1, Muon recolep2)
	{
		Double_t GenWeight;
		ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;

		this->FillHistograms_SingleMu(ntuple, HLT, recolep1);
		this->FillHistograms_SingleMu(ntuple, HLT, recolep2);

		TLorentzVector reco_v1 = recolep1.Momentum;
		TLorentzVector reco_v2 = recolep2.Momentum;
		Double_t reco_M = (reco_v1 + reco_v2).M();

		//Dimuon Mass/Pt/Rapidity
		Double_t reco_Rap = (reco_v1 + reco_v2).Rapidity();
		Double_t reco_Pt = (reco_v1 + reco_v2).Pt();
		h_mass->Fill( reco_M, GenWeight );
		h_diPt->Fill( reco_Pt, GenWeight );
		h_diRap->Fill( reco_Rap, GenWeight );

		// -- |y(ll)| distributions -- //
		if( reco_M >= 20 && reco_M < 30 )
			h_absdiRap_M20to30->Fill( fabs(reco_Rap), GenWeight);
		else if( reco_M >= 30 && reco_M < 45 )
			h_absdiRap_M30to45->Fill( fabs(reco_Rap), GenWeight);
		else if( reco_M >= 45 && reco_M < 60 )
			h_absdiRap_M45to60->Fill( fabs(reco_Rap), GenWeight);
		else if( reco_M >= 60 && reco_M < 120 )
			h_absdiRap_M60to120->Fill( fabs(reco_Rap), GenWeight);
		else if( reco_M >= 120 && reco_M < 200 )
			h_absdiRap_M120to200->Fill( fabs(reco_Rap), GenWeight);
		else if( reco_M >= 200 && reco_M < 1500 )
			h_absdiRap_M200to1500->Fill( fabs(reco_Rap), GenWeight);

		//leading, sub-leading muon pt,eta,phi distribution
		if( recolep1.Pt > recolep2.Pt )
		{
			h_lead_Pt->Fill( recolep1.Pt, GenWeight );
			h_lead_eta->Fill( recolep1.eta, GenWeight );
			h_lead_phi->Fill( recolep1.phi, GenWeight );

			h_sub_Pt->Fill( recolep2.Pt, GenWeight );
			h_sub_eta->Fill( recolep2.eta, GenWeight );
			h_sub_phi->Fill( recolep2.phi, GenWeight );
		}
		else
		{
			h_lead_Pt->Fill( recolep2.Pt, GenWeight );
			h_lead_eta->Fill( recolep2.eta, GenWeight );
			h_lead_phi->Fill( recolep2.phi, GenWeight );

			h_sub_Pt->Fill( recolep1.Pt, GenWeight );
			h_sub_eta->Fill( recolep1.eta, GenWeight );
			h_sub_phi->Fill( recolep1.phi, GenWeight );
		}

		//-- Mass of barrel Muons --//
		if( fabs(recolep1.eta) < 0.9 && fabs(recolep2.eta) < 0.9 )
			h_mass_barrel->Fill( reco_M, GenWeight );

		//Same-Sign / Opposite Invariant Mass
		if( recolep1.charge != recolep2.charge )
			h_mass_OS->Fill( reco_M, GenWeight );
		else
			h_mass_SS->Fill( reco_M, GenWeight );

		//Pt plots with respect to their pair mass
		if( reco_M < 70 )
		{
			h_Pt_M70->Fill( recolep1.Pt, GenWeight );
			h_Pt_M70->Fill( recolep2.Pt, GenWeight );
		}
		else if( reco_M > 70 && reco_M < 90 )
		{
			h_Pt_M70to90->Fill( recolep1.Pt, GenWeight );
			h_Pt_M70to90->Fill( recolep2.Pt, GenWeight );

		}
		else if( reco_M > 90 && reco_M < 110 )
		{
			h_Pt_M90to110->Fill( recolep1.Pt, GenWeight );
			h_Pt_M90to110->Fill( recolep2.Pt, GenWeight );
		}
		else if( reco_M > 110 )
		{
			h_Pt_M110toInf->Fill( recolep1.Pt, GenWeight );
			h_Pt_M110toInf->Fill( recolep2.Pt, GenWeight );
		}

		//Angle between two muons
		h_Angle->Fill( reco_v1.Angle( reco_v2.Vect() ), GenWeight );
		
	}

	void WriteHistograms(TFile *fout)
	{
		fout->cd();
		for(Int_t i_hist=0; i_hist < (Int_t)Histo.size(); i_hist++)
			Histo[i_hist]->Write();
	}
};






































