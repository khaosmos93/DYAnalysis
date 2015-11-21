//2015.03.21: Replace MuonID/ElectronID with NtupleHandle
//2015.04.02: Fix some minor bugs(missing penenthesis)
//2015.04.22: Correct Muon TightID
#include <TLorentzVector.h>

//customized header files
#include <NtupleHandle.h>

// -- Header files for the Rochester Muon Momentum Correction -- //
#include <rochcor2015.h> 
#include <muresolution_run2.h>

#define M_Mu 0.1056583715

class Lepton
{
public:
	Double_t Pt;
	Double_t Et;
	Double_t eta;
	Double_t phi;
	TLorentzVector Momentum;
	
};

class GenLepton : public Lepton
{
public:
    Double_t Px;
    Double_t Py;
    Double_t Pz;
    Double_t Mother;
    Double_t ID;
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

    void FillFromNtuple(NtupleHandle *nh, Int_t index)
    {
        Pt = nh->genLepton_pT[index];
        eta = nh->genLepton_eta[index];
        phi = nh->genLepton_phi[index];
        Px = nh->genLepton_px[index];
        Py = nh->genLepton_py[index];
        Pz = nh->genLepton_pz[index];
        Mother = nh->genLepton_mother[index];
        ID = nh->genLepton_ID[index];
        charge = nh->genLepton_charge[index];
        Status = nh->genLepton_status[index];
        
        if( ID == -11 || ID == 11 )
            Mass = 0.000510998;
        else if( ID == -13 || ID == 13 )
            Mass = 0.1056583;
        Double_t E = sqrt(Px*Px + Py*Py + Pz*Pz + Mass*Mass);
        Momentum.SetPxPyPzE(Px, Py, Pz, E);

        isPrompt = nh->GENLepton_isPrompt[index];
        isPromptFinalState = nh->GENLepton_isPromptFinalState[index];
        isTauDecayProduct = nh->GENLepton_isTauDecayProduct[index];
        isPromptTauDecayProduct = nh->GENLepton_isPromptTauDecayProduct[index];
        isDirectPromptTauDecayProductFinalState = nh->GENLepton_isDirectPromptTauDecayProductFinalState[index];
        isHardProcess = nh->GENLepton_isHardProcess[index];
        isLastCopy = nh->GENLepton_isLastCopy[index];
        isLastCopyBeforeFSR = nh->GENLepton_isLastCopyBeforeFSR[index];
        isPromptDecayed = nh->GENLepton_isPromptDecayed[index];
        isDecayedLeptonHadron = nh->GENLepton_isDecayedLeptonHadron[index];
        fromHardProcessBeforeFSR = nh->GENLepton_fromHardProcessBeforeFSR[index];
        fromHardProcessDecayed = nh->GENLepton_fromHardProcessDecayed[index];
        fromHardProcessFinalState = nh->GENLepton_fromHardProcessFinalState[index];
    }

    Bool_t isElectron()
    {
        if( ID == -11 || ID == 11 )
            return 1;
        return 0;
    }

    Bool_t isMuon()
    {
        if( ID == -13 || ID == 13 )
            return 1;
        return 0;
    }

    Bool_t isMotherZ()
    {
        if( Mother == 23 )
            return 1;
        return 0;
    }
};

class Electron : public Lepton
{
public:
	//Electron ID variables
    Int_t isEcalDriven;
    Double_t etaSC;
    Double_t SigmaIEtaIEta;
    Double_t dEta;
    Double_t dPhi;
    Double_t InvEminusInvP;
    Double_t HoverE;
    Double_t dxyVTX;
    Double_t dzVTX;
    Int_t mHits;

    //Isolations
    Double_t relPFiso;

    void FillFromNtuple(NtupleHandle *nh, Int_t index)
    {
    	Et =nh->Electron_et[index]; 
    	Pt =nh->Electron_pT[index];
    	eta =nh->Electron_eta[index];
    	phi =nh->Electron_phi[index];

	    isEcalDriven =nh->Electron_ecalDriven[index];
	    etaSC =nh->Electron_etaSC[index];
	    SigmaIEtaIEta =nh->Electron_sigmaIEtaIEta[index];
	    dEta =nh->Electron_dEtaIn[index];
	    dPhi =nh->Electron_dPhiIn[index];
	    InvEminusInvP =nh->Electron_InvEminusInvP[index];
	    HoverE =nh->Electron_HoverE[index];
	    dxyVTX =nh->Electron_dxyVTX[index];
	    dzVTX =nh->Electron_dzVTX[index];
	    mHits =nh->Electron_mHits[index];

	    Double_t pfChargedIso =nh->Electron_chIso03[index];
	    Double_t pfNeutralIso =nh->Electron_nhIso03[index];
	    Double_t pfGammaIso =nh->Electron_phIso03[index];
	    relPFiso = (pfChargedIso + pfNeutralIso + pfGammaIso) / nh->Electron_pT[index];

	    Double_t px =nh->Electron_Px[index];
	    Double_t py =nh->Electron_Py[index];
	    Double_t pz =nh->Electron_Pz[index];
	    Double_t E =nh->Electron_E[index];
	    TLorentzVector v;
	    v.SetPxPyPzE(px, py, pz, E);
	    Momentum = v;
    }

    Bool_t isMediumElectron()
    {
    	if( fabs(this->etaSC) < 1.4442 ) //Barrel Region
    	{
    	    if( fabs(this->dEta) < 0.004
    	       && fabs(this->dPhi) < 0.06
    	       && this->SigmaIEtaIEta < 0.01
    	       && this->HoverE < 0.12
    	       && fabs(this->dxyVTX) < 0.02
    	       && fabs(this->dzVTX) < 0.1
    	       && this->InvEminusInvP < 0.05
    	       && this->relPFiso < 0.15
    	       && this->mHits <= 1 )
    	        return 1;
    	}
    	else if( fabs(this->etaSC) < 2.5 && fabs(this->etaSC) > 1.566 ) //Endcap Region
    	{
    	    if( fabs(this->dEta) < 0.007
    	       && fabs(this->dPhi) < 0.03
    	       && this->SigmaIEtaIEta < 0.03
    	       && this->HoverE < 0.10
    	       && fabs(this->dxyVTX) < 0.02
    	       && fabs(this->dzVTX) < 0.1
    	       && this->InvEminusInvP < 0.05
    	       && this->relPFiso < 0.15
    	       && this->mHits <= 1 )
    	        return 1;
    	}
    	return 0;
    }

};

class Muon : public Lepton
{
public:
	//Muon ID variables  
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

	//Isolations
    Double_t trkiso;
    Double_t relPFiso;

	//Cocktail track information
    TLorentzVector cktMomentum;
    Double_t cktpT;
    Double_t cktpTError;
    Double_t dxycktVTX;
    Double_t dzcktVTX;

    void FillFromNtuple(NtupleHandle *nh, Int_t index)
    {
   		Pt =nh->Muon_pT[index];
   		eta =nh->Muon_eta[index];
   		phi =nh->Muon_phi[index];
   		cktpT =nh->Muon_cktpT[index];
   		isGLB =nh->isGLBmuon[index];
   		//isTRK =nh->isTRKmuon[index];
   		//isSTA =nh->isSTAmuon[index];
   		isPF =nh->isPFmuon[index];
      charge = nh->Muon_charge[index];
    	
      chi2dof = nh->Muon_chi2dof[index];
   		muonHits =nh->Muon_muonHits[index];
   		nSegments =nh->Muon_nSegments[index];
      nMatches = nh->Muon_nMatches[index];
   		trackerLayers =nh->Muon_trackerLayers[index];
   		pixelHits =nh->Muon_pixelHits[index];
      dxyVTX = nh->Muon_dxyVTX[index];
      dzVTX = nh->Muon_dzVTX[index];
   		dxycktVTX =nh->Muon_dxycktVTX[index];
   		dzcktVTX =nh->Muon_dzcktVTX[index];
   		trkiso = nh->Muon_trkiso[index] / nh->Muon_pT[index];
   		cktpTError = nh->Muon_cktpTError[index] / nh->Muon_cktpT[index];
    	
    	Double_t pfChargedIso =nh->Muon_PfChargedHadronIsoR04[index];
    	Double_t pfNeutralIso =nh->Muon_PfNeutralHadronIsoR04[index];
    	Double_t pfGammaIso =nh->Muon_PfGammaIsoR04[index];
   		relPFiso = (pfChargedIso + pfNeutralIso + pfGammaIso) / nh->Muon_pT[index];
    	
    	Double_t px =nh->Muon_Px[index];
    	Double_t py =nh->Muon_Py[index];
    	Double_t pz =nh->Muon_Pz[index];
    	Double_t Mu_E = sqrt( px*px + py*py + pz*pz + M_Mu*M_Mu );
    	TLorentzVector v;
    	v.SetPxPyPzE(px, py, pz, Mu_E);
    	
    	Double_t cktpx =nh->Muon_cktPx[index];
    	Double_t cktpy =nh->Muon_cktPy[index];
    	Double_t cktpz =nh->Muon_cktPz[index];
    	Double_t Mu_cktE = sqrt( cktpx*cktpx + cktpy*cktpy + cktpz*cktpz + M_Mu*M_Mu );
    	TLorentzVector cktv;
    	cktv.SetPxPyPzE(cktpx, cktpy, cktpz, Mu_cktE);

   		Momentum = v;
   		cktMomentum = cktv;
    }

    void ApplyRochCorr(rochcor2015 *rmcor, Int_t type)
    {
      float qter = 1.0;

      if (type == 0 ) // -- MC -- //
        rmcor->momcor_mc(Momentum, charge, 0, qter);

      else if (type == 1) // -- Data -- //
        rmcor->momcor_data(Momentum, charge, 0, qter);

      // -- Change Muon pT, eta and phi with updated(corrected) one -- //
      Pt = Momentum.Pt();
      eta = Momentum.Eta();
      phi = Momentum.Phi();
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
           && this->trackerLayers > 5
           // && this->relPFiso < 0.12
           )
        {
            return 1;
        }
        return 0;
    }

    Bool_t isTightMuon_minus_isGLB()
    {
        if(   //this->isGLB == 1
           this->isPF == 1
           && this->chi2dof < 10
           && this->muonHits > 0
           && this->nMatches > 1
           && fabs(this->dxyVTX) < 0.2
           && fabs(this->dzVTX) < 0.5
           && this->pixelHits > 0
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
        {
            return 1;
        }
        return 0;
    }

    Bool_t isTightMuon_minus_isPF()
    {
        if(   this->isGLB == 1
           // this->isPF == 1
           && this->chi2dof < 10
           && this->muonHits > 0
           && this->nMatches > 1
           && fabs(this->dxyVTX) < 0.2
           && fabs(this->dzVTX) < 0.5
           && this->pixelHits > 0
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trackerLayers > 5
           && this->trkiso < 0.10
           )
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
           && this->trkiso < 0.10
           )
        {
            return 1;
        }
        return 0;
    }

    Bool_t isTightMuon_minus_trkiso()
    {
        if(   this->isGLB == 1
           && this->isPF == 1
           && this->chi2dof < 10
           && this->muonHits > 0
           && this->nMatches > 1
           && fabs(this->dxyVTX) < 0.2
           && fabs(this->dzVTX) < 0.5
           && this->pixelHits > 0
           && this->trackerLayers > 5
           // && this->trkiso < 0.10
           )
        {
            return 1;
        }
        return 0;
    }

};
