///////////////////////////////////////////////////////////////
// -- 2015.03.21: Replace MuonID/ElectronID with NtupleHandle
// -- 2015.04.02: Fix some minor bugs(missing penenthesis)
// -- 2015.04.22: Correct Muon TightID
// -- 2016.01.02: Change the class name as "Objects" and include GenOthers, photon, jet and MET
///////////////////////////////////////////////////////////////

#pragma once

#include <TLorentzVector.h>

//customized header files
#include <Include/NtupleHandle.h>

#define M_Mu 0.1056583715 // -- GeV -- //
#define M_Elec 0.000510998 // -- GeV -- //
#define M_Tau 1.77682 // -- GeV -- //

class Object
{
public:
	Double_t Pt;
	Double_t Et;
	Double_t eta;
	Double_t phi;
	TLorentzVector Momentum;
};

class GenLepton : public Object
{
public:
	Double_t Px;
	Double_t Py;
	Double_t Pz;
	Double_t Mother;
	Int_t ID;
	Double_t charge;
	Double_t Mass;
	Int_t Status;

	//GenFlags(after 7_4_X)
	Int_t isPrompt;
	Int_t isPromptFinalState;
	Int_t isTauDecayProduct;
	Int_t isPromptTauDecayProduct;
	Int_t isDirectPromptTauDecayProductFinalState;
	Int_t isHardProcess;
	Int_t isLastCopy;
	Int_t isLastCopyBeforeFSR;
	Int_t isPromptDecayed;
	Int_t isDecayedLeptonHadron;
	Int_t fromHardProcessBeforeFSR;
	Int_t fromHardProcessDecayed;
	Int_t fromHardProcessFinalState;

	void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
	{
		Pt 		= ntuple->GenLepton_pT[index];
		eta 	= ntuple->GenLepton_eta[index];
		phi 	= ntuple->GenLepton_phi[index];
		Px 		= ntuple->GenLepton_px[index];
		Py 		= ntuple->GenLepton_py[index];
		Pz 		= ntuple->GenLepton_pz[index];
		Mother 	= ntuple->GenLepton_mother[index];
		ID 		= ntuple->GenLepton_ID[index];
		charge 	= ntuple->GenLepton_charge[index];
		Status 	= ntuple->GenLepton_status[index];
		
		if( ID == -11 || ID == 11 )
		    Mass = M_Elec;
		else if( ID == -13 || ID == 13 )
		    Mass = M_Mu;
		else if( ID == -15 || ID == 15 )
			Mass = M_Tau;

		Double_t E = sqrt(Px*Px + Py*Py + Pz*Pz + Mass*Mass);
		Momentum.SetPxPyPzE(Px, Py, Pz, E);

		isPrompt 									= ntuple->GenLepton_isPrompt[index];
		isPromptFinalState 							= ntuple->GenLepton_isPromptFinalState[index];
		isTauDecayProduct 							= ntuple->GenLepton_isTauDecayProduct[index];
		isPromptTauDecayProduct 					= ntuple->GenLepton_isPromptTauDecayProduct[index];
		isDirectPromptTauDecayProductFinalState 	= ntuple->GenLepton_isDirectPromptTauDecayProductFinalState[index];
		isHardProcess 								= ntuple->GenLepton_isHardProcess[index];
		isLastCopy 									= ntuple->GenLepton_isLastCopy[index];
		isLastCopyBeforeFSR 						= ntuple->GenLepton_isLastCopyBeforeFSR[index];
		isPromptDecayed 							= ntuple->GenLepton_isPromptDecayed[index];
		isDecayedLeptonHadron 						= ntuple->GenLepton_isDecayedLeptonHadron[index];
		fromHardProcessBeforeFSR 					= ntuple->GenLepton_fromHardProcessBeforeFSR[index];
		fromHardProcessDecayed 						= ntuple->GenLepton_fromHardProcessDecayed[index];
		fromHardProcessFinalState 					= ntuple->GenLepton_fromHardProcessFinalState[index];
	}

	Bool_t isElectron()
	{
		Bool_t isElec = kFALSE;
		if( abs(ID) == 11 )
			isElec = kTRUE;

		return isElec;
	}

	Bool_t isMuon()
	{
		Bool_t isMu = kFALSE;
		if( abs(ID) == 13 )
			isMu = kTRUE;

		return isMu;
	}

	Bool_t isMotherZ()
	{
		Bool_t isZ = kFALSE;
		if( Mother == 23 )
			isZ = kTRUE;

		return isZ;
	}

};

class GenOthers : public Object
{
public:
	Double_t Px;
	Double_t Py;
	Double_t Pz;
	Double_t Mother;
	Int_t ID;
	Double_t charge;
	Double_t Mass;
	Int_t Status;

	//GenFlags(after 7_4_X)
	Int_t isPrompt;
	Int_t isPromptFinalState;
	Int_t isTauDecayProduct;
	Int_t isPromptTauDecayProduct;
	Int_t isDirectPromptTauDecayProductFinalState;
	Int_t isHardProcess;
	Int_t isLastCopy;
	Int_t isLastCopyBeforeFSR;
	Int_t isPromptDecayed;
	Int_t isDecayedLeptonHadron;
	Int_t fromHardProcessBeforeFSR;
	Int_t fromHardProcessDecayed;
	Int_t fromHardProcessFinalState;

	void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
	{
		Pt      = ntuple->GenOthers_pT[index];
		eta     = ntuple->GenOthers_eta[index];
		phi     = ntuple->GenOthers_phi[index];
		Px      = ntuple->GenOthers_px[index];
		Py      = ntuple->GenOthers_py[index];
		Pz      = ntuple->GenOthers_pz[index];
		Mother  = ntuple->GenOthers_mother[index];
		ID      = ntuple->GenOthers_ID[index];
		charge  = ntuple->GenOthers_charge[index];
		Status  = ntuple->GenOthers_status[index];
		
		Mass = 0;
		if( abs(ID) == 22 ) // -- Photon -- //
		    Mass = 0;

		Double_t E = sqrt(Px*Px + Py*Py + Pz*Pz + Mass*Mass);
		Momentum.SetPxPyPzE(Px, Py, Pz, E);

		isPrompt                                    = ntuple->GenOthers_isPrompt[index];
		isPromptFinalState                          = ntuple->GenOthers_isPromptFinalState[index];
		isTauDecayProduct                           = ntuple->GenOthers_isTauDecayProduct[index];
		isPromptTauDecayProduct                     = ntuple->GenOthers_isPromptTauDecayProduct[index];
		isDirectPromptTauDecayProductFinalState     = ntuple->GenOthers_isDirectPromptTauDecayProductFinalState[index];
		isHardProcess                               = ntuple->GenOthers_isHardProcess[index];
		isLastCopy                                  = ntuple->GenOthers_isLastCopy[index];
		isLastCopyBeforeFSR                         = ntuple->GenOthers_isLastCopyBeforeFSR[index];
		isPromptDecayed                             = ntuple->GenOthers_isPromptDecayed[index];
		isDecayedLeptonHadron                       = ntuple->GenOthers_isDecayedLeptonHadron[index];
		fromHardProcessBeforeFSR                    = ntuple->GenOthers_fromHardProcessBeforeFSR[index];
		fromHardProcessDecayed                      = ntuple->GenOthers_fromHardProcessDecayed[index];
		fromHardProcessFinalState                   = ntuple->GenOthers_fromHardProcessFinalState[index];
	}

};

class Electron : public Object
{
public:
	Double_t Energy;
	Int_t charge;
	Double_t gsfpT;
	Double_t gsfPx;
	Double_t gsfPy;
	Double_t gsfPz;
	Double_t gsfEta;
	Double_t gsfPhi;
	Double_t gsfCharge;
	Double_t etaSC;
	Double_t phiSC;
	Double_t etaWidth;
	Double_t phiWidth;
	Double_t dEtaIn;
	Double_t dPhiIn;
	Double_t sigmaIEtaIEta;
	Double_t HoverE;
	Double_t fbrem;
	Double_t eOverP;
	Double_t InvEminusInvP;
	Double_t dxyVTX;
	Double_t dzVTX;
	Double_t dxy;
	Double_t dz;
	Double_t dxyBS;
	Double_t dzBS;
	Double_t chIso03;
	Double_t nhIso03;
	Double_t phIso03;
	Double_t ChIso03FromPU;
	Int_t mHits;
	Double_t EnergySC;
	Double_t preEnergySC;
	Double_t rawEnergySC;
	Double_t etSC;
	Double_t E15;
	Double_t E25;
	Double_t E55;
	Double_t RelPFIso_dBeta;
	Double_t RelPFIso_Rho;
	Double_t r9;
	Int_t ecalDriven;
	Int_t passConvVeto;

	void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
	{
		Energy = ntuple->Electron_Energy[index];
		Pt = ntuple->Electron_pT[index];
		eta = ntuple->Electron_eta[index];
		phi = ntuple->Electron_phi[index];
		charge = ntuple->Electron_charge[index];
		gsfpT = ntuple->Electron_gsfpT[index];
		gsfPx = ntuple->Electron_gsfPx[index];
		gsfPy = ntuple->Electron_gsfPy[index];
		gsfPz = ntuple->Electron_gsfPz[index];
		gsfEta = ntuple->Electron_gsfEta[index];
		gsfPhi = ntuple->Electron_gsfPhi[index];
		gsfCharge = ntuple->Electron_gsfCharge[index];
		etaSC = ntuple->Electron_etaSC[index];
		phiSC = ntuple->Electron_phiSC[index];
		etaWidth = ntuple->Electron_etaWidth[index];
		phiWidth = ntuple->Electron_phiWidth[index];
		dEtaIn = ntuple->Electron_dEtaIn[index];
		dPhiIn = ntuple->Electron_dPhiIn[index];
		sigmaIEtaIEta = ntuple->Electron_sigmaIEtaIEta[index];
		HoverE = ntuple->Electron_HoverE[index];
		fbrem = ntuple->Electron_fbrem[index];
		eOverP = ntuple->Electron_eOverP[index];
		InvEminusInvP = ntuple->Electron_InvEminusInvP[index];
		dxyVTX = ntuple->Electron_dxyVTX[index];
		dzVTX = ntuple->Electron_dzVTX[index];
		dxy = ntuple->Electron_dxy[index];
		dz = ntuple->Electron_dz[index];
		dxyBS = ntuple->Electron_dxyBS[index];
		dzBS = ntuple->Electron_dzBS[index];
		chIso03 = ntuple->Electron_chIso03[index];
		nhIso03 = ntuple->Electron_nhIso03[index];
		phIso03 = ntuple->Electron_phIso03[index];
		ChIso03FromPU = ntuple->Electron_ChIso03FromPU[index];
		mHits = ntuple->Electron_mHits[index];
		EnergySC = ntuple->Electron_EnergySC[index];
		preEnergySC = ntuple->Electron_preEnergySC[index];
		rawEnergySC = ntuple->Electron_rawEnergySC[index];
		etSC = ntuple->Electron_etSC[index];
		E15 = ntuple->Electron_E15[index];
		E25 = ntuple->Electron_E25[index];
		E55 = ntuple->Electron_E55[index];
		RelPFIso_dBeta = ntuple->Electron_RelPFIso_dBeta[index];
		RelPFIso_Rho = ntuple->Electron_RelPFIso_Rho[index];
		r9 = ntuple->Electron_r9[index];
		ecalDriven = ntuple->Electron_ecalDriven[index];
		passConvVeto = ntuple->Electron_passConvVeto[index];

		TLorentzVector v;
		v.SetPtEtaPhiM(Pt, eta, phi, M_Elec);
		Momentum = v;
	}

	// -- Ref: https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2 -- //
	Bool_t isMediumElectron_Spring25ns()
	{
		Bool_t isPass = kFALSE;

		Bool_t isBarrel = kFALSE;
		Bool_t isEndcap = kFALSE;
		if( fabs(etaSC) <= 1.479 )
			isBarrel = kTRUE;
		else if( fabs(etaSC) > 1.479 && fabs(etaSC) < 2.5 )
			isEndcap = kTRUE;

		if( isBarrel == kTRUE )
		{
			if( sigmaIEtaIEta < 0.0101
				&& fabs(dEtaIn) < 0.0103
				&& fabs(dPhiIn) < 0.0336
				&& HoverE < 0.0876
				&& RelPFIso_Rho < 0.0766
				&& InvEminusInvP < 0.0174
				&& fabs(dxyVTX) < 0.0118
				&& fabs(dzVTX) < 0.373
				&& mHits <= 2 
				// && passConvVeto == 1 
				)
				isPass = kTRUE;
		}
		else if( isEndcap == kTRUE )
		{
			if( sigmaIEtaIEta < 0.0283
				&& fabs(dEtaIn) < 0.00733
				&& fabs(dPhiIn) < 0.114
				&& HoverE < 0.0678
				&& RelPFIso_Rho < 0.0678
				&& InvEminusInvP < 0.0898
				&& fabs(dxyVTX) < 0.0739
				&& fabs(dzVTX) < 0.602
				&& mHits <= 1
				// && passConvVeto == 1 
				)
				isPass = kTRUE;
		}

		return isPass;
	}

	Bool_t isMediumElectron_Spring25ns_minus_PFIso()
	{
		Bool_t isPass = kFALSE;

		Bool_t isBarrel = kFALSE;
		Bool_t isEndcap = kFALSE;
		if( fabs(etaSC) <= 1.479 )
			isBarrel = kTRUE;
		else if( fabs(etaSC) > 1.479 && fabs(etaSC) < 2.5 )
			isEndcap = kTRUE;

		if( isBarrel == kTRUE )
		{
			if( sigmaIEtaIEta < 0.0101
				&& fabs(dEtaIn) < 0.0103
				&& fabs(dPhiIn) < 0.0336
				&& HoverE < 0.0876
				// && RelPFIso_Rho < 0.0766
				&& InvEminusInvP < 0.0174
				&& fabs(dxyVTX) < 0.0118
				&& fabs(dzVTX) < 0.373
				&& mHits <= 2 
				// && passConvVeto == 1 
				)
				isPass = kTRUE;
		}
		else if( isEndcap == kTRUE )
		{
			if( sigmaIEtaIEta < 0.0283
				&& fabs(dEtaIn) < 0.00733
				&& fabs(dPhiIn) < 0.114
				&& HoverE < 0.0678
				// && RelPFIso_Rho < 0.0678
				&& InvEminusInvP < 0.0898
				&& fabs(dxyVTX) < 0.0739
				&& fabs(dzVTX) < 0.602
				&& mHits <= 1
				// && passConvVeto == 1 
				)
				isPass = kTRUE;
		}

		return isPass;
	}

	Bool_t isTrigMatched(NtupleHandle *nh, TString HLT)
	{
		vector<string> *hlt_trigName = nh->HLT_trigName;
		Int_t hlt_ntrig = nh->HLT_ntrig;

		Bool_t isTrigMatch = false;
		for( Int_t k = 0; k < hlt_ntrig; k++ )
		{
			if( (hlt_trigName->at((unsigned int)k)) == HLT )
			{
				Double_t Lepton_eta = this->eta;
				Double_t Lepton_phi = this->phi;
				Double_t Trig_eta = nh->HLT_trigEta[k];
				Double_t Trig_phi = nh->HLT_trigPhi[k];

				Double_t dR = sqrt( (Lepton_eta - Trig_eta)*(Lepton_eta - Trig_eta) + (Lepton_phi - Trig_phi)*(Lepton_phi - Trig_phi) );
				if( dR < 0.3 && fabs( Lepton_eta ) < 2.5 )
				{
					isTrigMatch = true;
					break;
				}
			}
		}
		return isTrigMatch;
	}

};

class Muon : public Object
{
public:
	// -- Muon ID Variables -- //
	Int_t isGLB;
	Int_t isPF;
	Int_t isTRK;
	Int_t isSTA;
	Int_t charge;
	Double_t chi2dof;
	Int_t muonHits;
	Int_t nSegments;
	Int_t nMatches;
	Int_t trackerLayers;
	Int_t pixelHits;
	Double_t dxyVTX;
	Double_t dzVTX;

	Int_t pixelHitsGLB;
	Int_t trackerLayersGLB;
	Double_t dB;

	// -- Isolations -- //
	Double_t AbsTrkIso;
	Double_t trkiso;
	Double_t relPFiso;
	Double_t RelPFIso_dBeta;

	// -- Various Track Information -- //
	Double_t Default_pT;
	// Double_t Default_pTError;
	Double_t Default_Px;
	Double_t Default_Py;
	Double_t Default_Pz;
	Double_t Default_eta;
	Double_t Default_phi;

	Double_t Best_pT;
	Double_t Best_pTError;
	Double_t Best_Px;
	Double_t Best_Py;
	Double_t Best_Pz;
	Double_t Best_eta;
	Double_t Best_phi;

	Double_t Inner_pT;
	Double_t Inner_pTError;
	Double_t Inner_Px;
	Double_t Inner_Py;
	Double_t Inner_Pz;
	Double_t Inner_eta;
	Double_t Inner_phi;
	TLorentzVector Momentum_Inner;

	Double_t Outer_pT;
	Double_t Outer_pTError;
	Double_t Outer_Px;
	Double_t Outer_Py;
	Double_t Outer_Pz;
	Double_t Outer_eta;
	Double_t Outer_phi;

	Double_t GLB_pT;
	Double_t GLB_pTError;
	Double_t GLB_Px;
	Double_t GLB_Py;
	Double_t GLB_Pz;
	Double_t GLB_eta;
	Double_t GLB_phi;

	Double_t TuneP_pT;
	Double_t TuneP_pTError;
	Double_t TuneP_Px;
	Double_t TuneP_Py;
	Double_t TuneP_Pz;
	Double_t TuneP_eta;
	Double_t TuneP_phi;

	// -- Muon station cut study -- //
	Int_t stationMask;
	Int_t nMatchesRPCLayers;

	void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
	{
		Pt = ntuple->Muon_pT[index];
		eta = ntuple->Muon_eta[index];
		phi = ntuple->Muon_phi[index];
		isGLB = ntuple->isGLBmuon[index];
		isPF = ntuple->isPFmuon[index];
		isTRK = ntuple->isTRKmuon[index];
		charge = ntuple->Muon_charge[index];

		chi2dof = ntuple->Muon_chi2dof[index];
		muonHits = ntuple->Muon_muonHits[index];
		nSegments = ntuple->Muon_nSegments[index];
		nMatches = ntuple->Muon_nMatches[index];
		trackerLayers = ntuple->Muon_trackerLayers[index];
		pixelHits = ntuple->Muon_pixelHits[index];
		dxyVTX = ntuple->Muon_dxyVTX[index];
		dzVTX = ntuple->Muon_dzVTX[index];
		AbsTrkIso = ntuple->Muon_trkiso[index];
		trkiso = ntuple->Muon_trkiso[index] / ntuple->Muon_pT[index];

		pixelHitsGLB = ntuple->Muon_pixelHitsGLB[index];
		trackerLayersGLB = ntuple->Muon_trackerLayersGLB[index];
		dB = ntuple->Muon_dB[index];

		Double_t pfChargedIso = ntuple->Muon_PfChargedHadronIsoR04[index];
		Double_t pfNeutralIso = ntuple->Muon_PfNeutralHadronIsoR04[index];
		Double_t pfGammaIso = ntuple->Muon_PfGammaIsoR04[index];
		relPFiso = (pfChargedIso + pfNeutralIso + pfGammaIso) / ntuple->Muon_pT[index];

		Double_t pfSumPUPt = ntuple->Muon_PFSumPUIsoR04[index];
		this->RelPFIso_dBeta = ( pfChargedIso + max(0.0, pfNeutralIso + pfGammaIso - 0.5*pfSumPUPt) ) / this->Pt;

		Double_t px = ntuple->Muon_Px[index];
		Double_t py = ntuple->Muon_Py[index];
		Double_t pz = ntuple->Muon_Pz[index];
		Double_t Mu_E = sqrt( px*px + py*py + pz*pz + M_Mu*M_Mu );
		TLorentzVector v;
		v.SetPxPyPzE(px, py, pz, Mu_E);
		Momentum = v;

		Default_pT = Pt;
		Default_Px = ntuple->Muon_Px[index];
		Default_Py = ntuple->Muon_Py[index];
		Default_Pz = ntuple->Muon_Pz[index];
		Default_eta = eta;
		Default_phi = phi;

		Best_pT = ntuple->Muon_Best_pT[index];
		Best_pTError = ntuple->Muon_Best_pTError[index];
		Best_Px = ntuple->Muon_Best_Px[index];
		Best_Py = ntuple->Muon_Best_Py[index];
		Best_Pz = ntuple->Muon_Best_Pz[index];
		Best_eta = ntuple->Muon_Best_eta[index];
		Best_phi = ntuple->Muon_Best_phi[index];

		Inner_pT = ntuple->Muon_Inner_pT[index];
		Inner_pTError = ntuple->Muon_Inner_pTError[index];
		Inner_Px = ntuple->Muon_Inner_Px[index];
		Inner_Py = ntuple->Muon_Inner_Py[index];
		Inner_Pz = ntuple->Muon_Inner_Pz[index];
		Inner_eta = ntuple->Muon_Inner_eta[index];
		Inner_phi = ntuple->Muon_Inner_phi[index];

		TLorentzVector v_inner;
		Double_t Mu_Inner_E = sqrt( Inner_Px*Inner_Px + Inner_Py*Inner_Py + Inner_Pz*Inner_Pz + M_Mu*M_Mu );
		v_inner.SetPxPyPzE(Inner_Px, Inner_Py, Inner_Pz, Mu_Inner_E);
		Momentum_Inner =  v_inner;

		Outer_pT = ntuple->Muon_Outer_pT[index];
		Outer_pTError = ntuple->Muon_Outer_pTError[index];
		Outer_Px = ntuple->Muon_Outer_Px[index];
		Outer_Py = ntuple->Muon_Outer_Py[index];
		Outer_Pz = ntuple->Muon_Outer_Pz[index];
		Outer_eta = ntuple->Muon_Outer_eta[index];
		Outer_phi = ntuple->Muon_Outer_phi[index];

		GLB_pT = ntuple->Muon_GLB_pT[index];
		GLB_pTError = ntuple->Muon_GLB_pTError[index];
		GLB_Px = ntuple->Muon_GLB_Px[index];
		GLB_Py = ntuple->Muon_GLB_Py[index];
		GLB_Pz = ntuple->Muon_GLB_Pz[index];
		GLB_eta = ntuple->Muon_GLB_eta[index];
		GLB_phi = ntuple->Muon_GLB_phi[index];

		TuneP_pT = ntuple->Muon_TuneP_pT[index];
		TuneP_pTError = ntuple->Muon_TuneP_pTError[index];
		TuneP_Px = ntuple->Muon_TuneP_Px[index];
		TuneP_Py = ntuple->Muon_TuneP_Py[index];
		TuneP_Pz = ntuple->Muon_TuneP_Pz[index];
		TuneP_eta = ntuple->Muon_TuneP_eta[index];
		TuneP_phi = ntuple->Muon_TuneP_phi[index];

		stationMask = ntuple->Muon_stationMask[index];
		nMatchesRPCLayers = ntuple->Muon_nMatchesRPCLayers[index];
	}

	void ConvertMomentum_TuneP()
	{
		this->Pt = this->TuneP_pT;
		this->eta = this->TuneP_eta;
		this->phi = this->TuneP_phi;

		Double_t px = this->TuneP_Px;
		Double_t py = this->TuneP_Py;
		Double_t pz = this->TuneP_Pz;
		Double_t Mu_E = sqrt( px*px + py*py + pz*pz + M_Mu*M_Mu );
		TLorentzVector v;
		v.SetPxPyPzE(px, py, pz, Mu_E);
		this->Momentum = v;
	}

	void ConvertMomentum_GlobalTrack()
	{
		this->Pt = this->GLB_pT;
		this->eta = this->GLB_eta;
		this->phi = this->GLB_phi;

		Double_t px = this->GLB_Px;
		Double_t py = this->GLB_Py;
		Double_t pz = this->GLB_Pz;
		Double_t Mu_E = sqrt( px*px + py*py + pz*pz + M_Mu*M_Mu );
		TLorentzVector v;
		v.SetPxPyPzE(px, py, pz, Mu_E);
		this->Momentum = v;
	}

	Bool_t isTrigMatched(NtupleHandle *nh, TString HLT)
	{
		vector<string> *hlt_trigName = nh->HLT_trigName;
		Int_t hlt_ntrig = nh->HLT_ntrig;

		Bool_t isTrigMatch = false;
		for( Int_t k = 0; k < hlt_ntrig; k++ )
		{
			if( (hlt_trigName->at((unsigned int)k)) == HLT )
			{
				Double_t Lepton_eta = this->eta;
				Double_t Lepton_phi = this->phi;
				Double_t Trig_eta = nh->HLT_trigEta[k];
				Double_t Trig_phi = nh->HLT_trigPhi[k];

				Double_t dR = sqrt( (Lepton_eta - Trig_eta)*(Lepton_eta - Trig_eta) + (Lepton_phi - Trig_phi)*(Lepton_phi - Trig_phi) );
				if( dR < 0.3 && fabs( Lepton_eta ) < 2.4 )
				{
					isTrigMatch = true;
					break;
				}
			}
		}
		return isTrigMatch;
	}

	Bool_t isTrigMatched_dR0p2(NtupleHandle *nh, TString HLT)
	{
		vector<string> *hlt_trigName = nh->HLT_trigName;
		Int_t hlt_ntrig = nh->HLT_ntrig;

		Bool_t isTrigMatch = false;
		for( Int_t k = 0; k < hlt_ntrig; k++ )
		{
			if( (hlt_trigName->at((unsigned int)k)) == HLT )
			{
				// Double_t Lepton_eta = this->eta;
				// Double_t Lepton_phi = this->phi;
				// Double_t Trig_eta = nh->HLT_trigEta[k];
				// Double_t Trig_phi = nh->HLT_trigPhi[k];

				// Double_t dR = sqrt( (Lepton_eta - Trig_eta)*(Lepton_eta - Trig_eta) + (Lepton_phi - Trig_phi)*(Lepton_phi - Trig_phi) );

				TLorentzVector vec_TrigObj;
				Double_t Trig_Pt = nh->HLT_trigPt[k];
				Double_t Trig_eta = nh->HLT_trigEta[k];
				Double_t Trig_phi = nh->HLT_trigPhi[k];
				vec_TrigObj.SetPtEtaPhiM( Trig_Pt, Trig_eta, Trig_phi, M_Mu );

				Double_t dR = this->Momentum.DeltaR( vec_TrigObj );
				// if( dR < 0.2 && fabs( this->eta ) < 2.4 )
				if( dR < 0.2 )
				{
					isTrigMatch = true;
					break;
				}
			}
		}
		return isTrigMatch;
	}

	Bool_t isTightMuon()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isHighPtMuon()
	{
		if(   this->isGLB == 1
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& (this->Best_pTError / this->Best_pT ) < 0.3
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isHighPtMuon_minus_dzVTX()
	{
		if(   this->isGLB == 1
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& (this->Best_pTError / this->Best_pT ) < 0.3
			&& fabs(this->dxyVTX) < 0.2
			// && fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_isGLB()
	{
		if(   // this->isGLB == 1
			this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_isPF()
	{
		if(   this->isGLB == 1
			// && this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_chi2dof()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			// && this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_muonHits()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			// && this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_nMatches()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			// && this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_dxyVTX()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			// && fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_dzVTX()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			// && fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_pixelHits()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			// && this->pixelHits > 0
			&& this->trackerLayers > 5 )
		{
			return 1;
		}

		return 0;
	}

	Bool_t isTightMuon_minus_trackerLayers()
	{
		if(   this->isGLB == 1
			&& this->isPF == 1
			&& this->chi2dof < 10
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& fabs(this->dxyVTX) < 0.2
			&& fabs(this->dzVTX) < 0.5
			&& this->pixelHits > 0
			// && this->trackerLayers > 5 
			)
		{
			return 1;
		}

		return 0;
	}

	// -- https://twiki.cern.ch/twiki/bin/view/CMS/Zprime2muAnalysis -- //
	Bool_t isMuon_ZprimeICHEP2016()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			&& (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_minus_Pt()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			&& (this->AbsTrkIso / this->Inner_pT) < 0.1
			// && this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_minus_MuStationCut()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			// && this->nMatches > 1
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			&& (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_MuStationCutA()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& (  this->nMatches > 1 
				|| ( this->nMatches==1 && !(this->stationMask==1 || this->stationMask==16) )  )
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			&& (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_MuStationCutAorB()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& (  this->nMatches > 1 
				|| ( this->nMatches==1 && !(this->stationMask==1 || this->stationMask==16) )
				|| ( this->nMatches==1 && (this->stationMask==1 || this->stationMask==16) && this->nMatchesRPCLayers > 2 )  )
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			&& (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_minus_TrkIso()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& this->nMatches > 1
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			// && (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_minus_TrkIso_MuStationCut()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			// && this->nMatches > 1
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			// && (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_MuStationCutA_minus_TrkIso()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& (  this->nMatches > 1 
				|| ( this->nMatches==1 && !(this->stationMask==1 || this->stationMask==16) )  )
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			// && (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

	Bool_t isMuon_ZprimeICHEP2016_MuStationCutAorB_minus_TrkIso()
	{
		if( this->isGLB == 1
			&& this->isTRK == 1
			&& fabs(this->dB) < 0.2
			&& this->trackerLayersGLB > 5
			&& this->pixelHitsGLB > 0
			&& this->muonHits > 0
			&& (  this->nMatches > 1 
				|| ( this->nMatches==1 && !(this->stationMask==1 || this->stationMask==16) )
				|| ( this->nMatches==1 && (this->stationMask==1 || this->stationMask==16) && this->nMatchesRPCLayers > 2 )  )
			&& (this->TuneP_pTError / this->TuneP_pT ) < 0.3
			// && (this->AbsTrkIso / this->Inner_pT) < 0.1
			&& this->TuneP_pT > 53
			)
		{
			return 1;
		}
		return 0;
	}

};

class MuPair : public Object
{
public:
	Muon First;
	Muon Second;

	Double_t M;
	Double_t Rapidity;
	Double_t VtxProb;
	Double_t NormVtxChi2;
	Double_t Angle3D;
	Double_t Angle3D_Inner;

	Bool_t isOS;

	// -- default contructor -- //
	MuPair() {};
	MuPair( Muon mu1, Muon mu2 )
	{
		// -- first: leading muon, secound: sub-leading muon -- //
		if( mu1.Pt > mu2.Pt )
		{
			First = mu1;
			Second = mu2;
		}
		else
		{
			First = mu2;
			Second = mu1;
		}

		this->Fill_DimuonVar();
	}

	void Fill_DimuonVar()
	{
		this->Momentum = First.Momentum + Second.Momentum;

		this->M = this->Momentum.M();
		this->Pt = this->Momentum.Pt();
		this->Rapidity = this->Momentum.Rapidity();

		this->Angle3D = First.Momentum.Angle( Second.Momentum.Vect() );
		this->Angle3D_Inner = First.Momentum_Inner.Angle( Second.Momentum_Inner.Vect() );

		// -- initialization -- //
		this->VtxProb = -999; 
		this->NormVtxChi2 = 999;

		this->isOS = First.charge != Second.charge ? kTRUE : kFALSE;

		// // -- actually, these values are almost meaningless ... -- //
		// this->eta = this->Momentum.Eta();
		// this->phi = this->Momentum.Phi();
		// this->Et = this->Momentum.Et();
	}

	void Calc_CommonVertexVariable(NtupleHandle *ntuple)
	{
		vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
		vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
		vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

		Int_t NPt1 = (Int_t)PtCollection1->size();
		Int_t NPt2 = (Int_t)PtCollection2->size();
		Int_t NProb = (Int_t)VtxProbCollection->size();

		if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
			cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

		// -- inner pT values are used -- //
		Double_t Pt1 = this->First.Inner_pT;
		Double_t Pt2 = this->Second.Inner_pT;
		for(Int_t i=0; i<NProb; i++)
		{
			// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
			if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
			{
				this->VtxProb = VtxProbCollection->at(i);
				this->NormVtxChi2 = ntuple->vtxTrkChi2->at(i) / ntuple->vtxTrkNdof->at(i);
				break;
			}
		}
	}

	void Calc_CommonVertexVariable_TuneP( NtupleHandle *ntuple )
	{
		vector<double> *PtCollection1 = ntuple->vtxTrk1Pt_TuneP;
		vector<double> *PtCollection2 = ntuple->vtxTrk2Pt_TuneP;
		vector<double> *VtxProbCollection = ntuple->vtxTrkProb_TuneP;

		Int_t NPt1 = (Int_t)PtCollection1->size();
		Int_t NPt2 = (Int_t)PtCollection2->size();
		Int_t NProb = (Int_t)VtxProbCollection->size();

		if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
			cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

		// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;
		Double_t Pt1 = this->First.TuneP_pT;
		Double_t Pt2 = this->Second.TuneP_pT;

		for(Int_t i=0; i<NProb; i++)
		{
			// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
			if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
			{
				this->VtxProb = VtxProbCollection->at(i);
				this->NormVtxChi2 = ntuple->vtxTrkChi2_TuneP->at(i) / ntuple->vtxTrkNdof_TuneP->at(i);
				break;
			}
		}

		return;
	}

	Bool_t isGoodMuPair( NtupleHandle *ntuple, TString HLT, 
						   Double_t LeadPtCut, Double_t SubPtCut, Double_t LeadEtaCut, Double_t SubEtaCut )
	{
		Bool_t GoodPair = kFALSE;

		// -- Check the existence of at least one muon matched with HLT-object -- //
		Bool_t isHLTMatched = kFALSE;
		Bool_t isMatched_First = this->First.isTrigMatched(ntuple, HLT);
		Bool_t isMatched_Second = this->Second.isTrigMatched(ntuple, HLT);
		if( isMatched_First || isMatched_Second )
			isHLTMatched = kTRUE;

		// -- acceptance -- //
		Bool_t isPassAcc = this->isWithinAcc( LeadPtCut, SubPtCut, LeadEtaCut, SubEtaCut );

		// -- common vertex -- //
		this->Calc_CommonVertexVariable( ntuple );

		if( isPassAcc == kTRUE && // -- acceptance -- //
			this->M > 15 && // -- minimum mass -- //
			this->NormVtxChi2 < 20 &&  // -- vertex chi2 < 20 -- //
			this->Angle3D_Inner < TMath::Pi()-0.005 &&  // -- 3D open angle -- //
			this->isOS == kTRUE // -- opposite sign -- //
			) 
			GoodPair = kTRUE;

		return GoodPair;
	}

	Bool_t isWithinAcc( Double_t LeadPtCut, Double_t SubPtCut, Double_t LeadEtaCut, Double_t SubEtaCut )
	{
		Bool_t Flag = kFALSE;

		// -- first: leading, second: sub-leading -- //
		if( this->First.Pt > LeadPtCut && fabs(this->First.eta) < LeadEtaCut && 
			this->Second.Pt > SubPtCut && fabs(this->Second.eta) < SubEtaCut )
				Flag = kTRUE;

		return Flag;
	}
};

Bool_t ComparePair_VtxChi2( MuPair pair1, MuPair pair2 )
{
	// -- the pair with "smallest" vertex chi2 will be the first element -- //
	return pair1.NormVtxChi2 < pair2.NormVtxChi2; 
}

Bool_t ComparePair_DimuonPt( MuPair pair1, MuPair pair2 )
{
	// -- the pair with "largest" dimuon pT will be the first element -- //
	return pair1.Pt > pair2.Pt; 
}

class Photon : public Object
{
public:
	Int_t hasPixelSeed;
	Double_t etaSC;
	Double_t phiSC;
	Double_t HoverE;
	Double_t Full5x5_SigmaIEtaIEta;
	Double_t ChIso;
	Double_t NhIso;
	Double_t PhIso;
	Double_t ChIsoWithEA;
	Double_t NhIsoWithEA;
	Double_t PhIsoWithEA;

	void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
	{
		Pt = ntuple->Photon_pT[index];
		eta = ntuple->Photon_eta[index];
		phi = ntuple->Photon_phi[index];

		hasPixelSeed = ntuple->Photon_hasPixelSeed[index];
		etaSC = ntuple->Photon_etaSC[index];
		phiSC = ntuple->Photon_phiSC[index];
		HoverE = ntuple->Photon_HoverE[index];
		Full5x5_SigmaIEtaIEta = ntuple->Photon_Full5x5_SigmaIEtaIEta[index];
		ChIso = ntuple->Photon_ChIso[index];
		NhIso = ntuple->Photon_NhIso[index];
		PhIso = ntuple->Photon_PhIso[index];
		ChIsoWithEA = ntuple->Photon_ChIsoWithEA[index];
		NhIsoWithEA = ntuple->Photon_NhIsoWithEA[index];
		PhIsoWithEA = ntuple->Photon_PhIsoWithEA[index];
	}

	Bool_t isMediumPhoton()
	{
		Bool_t isPass = kFALSE;

		Bool_t isBarrel = kFALSE;
		Bool_t isEndcap = kFALSE;
		if( fabs(etaSC) <= 1.479 )
			isBarrel = kTRUE;
		else if( fabs(etaSC) > 1.479 && fabs(etaSC) < 2.5 )
			isEndcap = kTRUE;

		if( isBarrel == kTRUE )
		{
			if( HoverE < 0.05
				&& Full5x5_SigmaIEtaIEta < 0.0102
				&& ChIso < 1.37
				&& NhIsoWithEA < 1.06 + 0.014*Pt + 0.000019*Pt*Pt
				&& PhIsoWithEA < 0.28 + 0.0053*Pt )
				isPass = kTRUE;
		}
		else if( isEndcap == kTRUE )
		{
			if( HoverE < 0.05
				&& Full5x5_SigmaIEtaIEta < 0.0268
				&& ChIso < 1.10
				&& NhIsoWithEA < 2.69 + 0.0139*Pt + 0.000025*Pt*Pt
				&& PhIsoWithEA < 0.39 + 0.0034*Pt  )
				isPass = kTRUE;
		}

		return isPass;		
	}
	
};

class Jet : public Object
{
public:
	Double_t Charge;
	Int_t Flavor;

	Double_t bTag;
	Double_t CHfrac;
	Double_t NHfrac;
	Double_t NHEMfrac;
	Double_t CHEMfrac;
	Int_t CHmulti;
	Int_t NHmulti;
	Int_t NumConst;

	void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
	{
		Pt = ntuple->Jet_pT[index];
		eta = ntuple->Jet_eta[index];
		phi = ntuple->Jet_phi[index];
		Charge = ntuple->Jet_Charge[index];
		Flavor = ntuple->Jet_Flavor[index];
		bTag = ntuple->Jet_bTag[index];
		CHfrac = ntuple->Jet_CHfrac[index];
		NHfrac = ntuple->Jet_NHfrac[index];
		NHEMfrac = ntuple->Jet_NHEMfrac[index];
		CHEMfrac = ntuple->Jet_CHEMfrac[index];
		CHmulti = ntuple->Jet_CHmulti[index];
		NHmulti = ntuple->Jet_NHmulti[index];

		NumConst = CHmulti + NHmulti;
	}

	Bool_t LooseID_80X()
	{
		Bool_t Flag_Pass = kFALSE;

		if( fabs(this->eta) < 2.7 )
		{
			if( this->NHfrac < 0.99 && this->NHEMfrac < 0.99 && NumConst > 1 )
			{
				if( fabs(this->eta) < 2.4 )
				{
					if( CHfrac > 0 && CHmulti > 0 && CHEMfrac < 0.99 )
						Flag_Pass = kTRUE;
				}
				else
					Flag_Pass = kTRUE;
			}
		}
		else if( fabs(this->eta) > 2.7 && fabs(this->eta) < 3.0 )
		{
			if( NHEMfrac > 0.01 && NHfrac < 0.98 && NHmulti > 2 )
				Flag_Pass = kTRUE;
		}
		else
		{
			if( NHEMfrac < 0.9 && NHmulti > 10 )
				Flag_Pass = kTRUE;
		}

		return Flag_Pass;
	}

};



