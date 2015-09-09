// Update
// 2014.04.02: Remove duplicated declaration of gen-variables & Trigger

#define MaxN 50000
#include <TTree.h>
#include <TChain.h>
#include <vector>

class NtupleHandle
{
public:
	TChain *chain;

    //Event Informations
    Int_t nVertices;
    Int_t runNum;

    //Trigger variables
    Int_t HLT_ntrig;
    Int_t HLT_trigFired[MaxN];
    vector<string> *HLT_trigName;
    Double_t HLT_trigEta[MaxN];
    Double_t HLT_trigPhi[MaxN];

    //Generator level information
    Int_t gnpair;
    Double_t genLepton_px[MaxN];
    Double_t genLepton_py[MaxN];
    Double_t genLepton_pz[MaxN];
    Double_t genLepton_mother[MaxN];
    Double_t genLepton_pT[MaxN];
    Double_t genLepton_eta[MaxN];
    Double_t genLepton_phi[MaxN];
    Int_t genLepton_charge[MaxN];
    Int_t genLepton_status[MaxN];
    Int_t genLepton_ID[MaxN];
    Double_t GENEvt_weight;
    Int_t GENLepton_isPrompt[MaxN];
    Int_t GENLepton_isPromptFinalState[MaxN];
    Int_t GENLepton_isTauDecayProduct[MaxN];
    Int_t GENLepton_isPromptTauDecayProduct[MaxN];
    Int_t GENLepton_isDirectPromptTauDecayProductFinalState[MaxN];
    Int_t GENLepton_isHardProcess[MaxN];
    Int_t GENLepton_isLastCopy[MaxN];
    Int_t GENLepton_isLastCopyBeforeFSR[MaxN];
    Int_t GENLepton_isPromptDecayed[MaxN];
    Int_t GENLepton_isDecayedLeptonHadron[MaxN];
    Int_t GENLepton_fromHardProcessBeforeFSR[MaxN];
    Int_t GENLepton_fromHardProcessDecayed[MaxN];
    Int_t GENLepton_fromHardProcessFinalState[MaxN];

//  *****************************************
//  Electron Variables***********************
//  *****************************************
    //Physical Variables
    Double_t Electron_pT[MaxN];
	Double_t Electron_eta[MaxN];
	Double_t Electron_phi[MaxN];
    
    //Cut variables
	Double_t Electron_et[MaxN];
	Double_t Electron_etaSC[MaxN];
	Double_t Electron_sigmaIEtaIEta[MaxN];
	Double_t Electron_dPhiIn[MaxN];
	Double_t Electron_dEtaIn[MaxN];
	Double_t Electron_InvEminusInvP[MaxN];
	Double_t Electron_HoverE[MaxN];
	Double_t Electron_dxyVTX[MaxN];
	Double_t Electron_dzVTX[MaxN];   
	Int_t Nelectrons;
	Int_t Electron_ecalDriven[MaxN];
	Int_t Electron_mHits[MaxN];
    Double_t Electron_phIso03[MaxN];
    Double_t Electron_nhIso03[MaxN];
    Double_t Electron_chIso03[MaxN];
    
    //for invariant mass calculation
    Double_t Electron_E[MaxN];
    Double_t Electron_Px[MaxN];
    Double_t Electron_Py[MaxN];
    Double_t Electron_Pz[MaxN];
    
    //Calibrated Electron variables(e.g. including corrections)
    Double_t Electron_ECalib[MaxN];
    Double_t Electron_PxCalib[MaxN];
    Double_t Electron_PyCalib[MaxN];
    Double_t Electron_PzCalib[MaxN];
    Double_t Electron_etCalib[MaxN];
    Double_t Electron_pTCalib[MaxN];


//  *****************************************
//  Muon Variables***************************
//  *****************************************    
    //Physical Variables
    Double_t Muon_pT[MaxN];
    Double_t Muon_eta[MaxN];
    Double_t Muon_phi[MaxN];
    
    //Cut variables
    Int_t Muon_muonType[MaxN];
    Double_t Muon_chi2dof[MaxN];
    Int_t Muon_muonHits[MaxN];
    Int_t Muon_nSegments[MaxN];
    Int_t Muon_nMatches[MaxN];
    Int_t Muon_trackerLayers[MaxN];
    Int_t Muon_pixelHits[MaxN];
    Double_t Muon_dxyVTX[MaxN];
    Double_t Muon_dzVTX[MaxN];
    Double_t Muon_trkiso[MaxN];
    Int_t isGLBmuon[MaxN];
    Int_t isPFmuon[MaxN];
    Int_t nMuon;
    
    //for invariant mass calculation
    Double_t Muon_Px[MaxN];
    Double_t Muon_Py[MaxN];
    Double_t Muon_Pz[MaxN];
    
    //for the muon momentum corrections
    Int_t Muon_charge[MaxN];
    
    //for High pT Muon ID
    Double_t Muon_cktpT[MaxN];
    Double_t Muon_cktpTError[MaxN];
    Double_t Muon_dxycktVTX[MaxN];
    Double_t Muon_dzcktVTX[MaxN];
    Double_t Muon_cktPx[MaxN];
    Double_t Muon_cktPy[MaxN];
    Double_t Muon_cktPz[MaxN];
    
    //PF information
    Double_t Muon_PfChargedHadronIsoR04[MaxN];
    Double_t Muon_PfNeutralHadronIsoR04[MaxN];
    Double_t Muon_PfGammaIsoR04[MaxN];

    //Dimuon variables
    std::vector<double> *CosAngle;
    std::vector<double> *vtxTrkChi2;
    std::vector<double> *vtxTrkProb;
    std::vector<double> *vtxTrkNdof;
    std::vector<double> *vtxTrkCkt1Pt;
    std::vector<double> *vtxTrkCkt2Pt;
    std::vector<double> *vtxTrkDiEChi2;
    std::vector<double> *vtxTrkDiEProb;
    std::vector<double> *vtxTrkDiENdof;
    std::vector<double> *vtxTrkDiE1Pt;
    std::vector<double> *vtxTrkDiE2Pt;
    std::vector<double> *vtxTrkEMuChi2;
    std::vector<double> *vtxTrkEMuProb;
    std::vector<double> *vtxTrkEMuNdof;
    std::vector<double> *vtxTrkEMu1Pt;
    std::vector<double> *vtxTrkEMu2Pt;

    //Constructor
    NtupleHandle(TChain *chainptr)
    {
        chain = chainptr;
        chain->SetBranchStatus("*", 0);
        
        //Event Information
        chain->SetBranchStatus("nVertices", 1);
        chain->SetBranchStatus("runNum", 1);

        chain->SetBranchAddress("nVertices", &nVertices);
        chain->SetBranchAddress("runNum", &runNum);

        //Trigger Information
        chain->SetBranchStatus("HLT_trigName", 1);
        chain->SetBranchStatus("HLT_ntrig", 1);
        chain->SetBranchStatus("HLT_trigFired", 1);
        chain->SetBranchStatus("HLT_trigEta", 1);
        chain->SetBranchStatus("HLT_trigPhi", 1);

        chain->SetBranchAddress("HLT_trigName", &HLT_trigName);
        chain->SetBranchAddress("HLT_trigFired", HLT_trigFired);
        chain->SetBranchAddress("HLT_ntrig", &HLT_ntrig);
        chain->SetBranchAddress("HLT_trigEta", &HLT_trigEta);
        chain->SetBranchAddress("HLT_trigPhi", &HLT_trigPhi);

        //Generator level information
        chain->SetBranchStatus("GENnPair", 1);
        chain->SetBranchStatus("GENLepton_Px", 1);
        chain->SetBranchStatus("GENLepton_Py", 1);
        chain->SetBranchStatus("GENLepton_Pz", 1);
        chain->SetBranchStatus("GENLepton_mother", 1);
        chain->SetBranchStatus("GENLepton_pT", 1);
        chain->SetBranchStatus("GENLepton_eta", 1);
        chain->SetBranchStatus("GENLepton_phi", 1);
        chain->SetBranchStatus("GENLepton_charge", 1);
        chain->SetBranchStatus("GENLepton_status", 1);
        chain->SetBranchStatus("GENLepton_ID", 1);
        chain->SetBranchStatus("GENEvt_weight", 1);
        chain->SetBranchStatus("GENLepton_isPrompt", 1);
        chain->SetBranchStatus("GENLepton_isPromptFinalState", 1);
        chain->SetBranchStatus("GENLepton_isTauDecayProduct", 1);
        chain->SetBranchStatus("GENLepton_isPromptTauDecayProduct", 1);
        chain->SetBranchStatus("GENLepton_isDirectPromptTauDecayProductFinalState", 1);
        chain->SetBranchStatus("GENLepton_isHardProcess", 1);
        chain->SetBranchStatus("GENLepton_isLastCopy", 1);
        chain->SetBranchStatus("GENLepton_isLastCopyBeforeFSR", 1);
        chain->SetBranchStatus("GENLepton_isPromptDecayed", 1);
        chain->SetBranchStatus("GENLepton_isDecayedLeptonHadron", 1);
        chain->SetBranchStatus("GENLepton_fromHardProcessBeforeFSR", 1);
        chain->SetBranchStatus("GENLepton_fromHardProcessDecayed", 1);
        chain->SetBranchStatus("GENLepton_fromHardProcessFinalState", 1);

        chain->SetBranchAddress("GENnPair", &gnpair);
        chain->SetBranchAddress("GENLepton_Px", genLepton_px);
        chain->SetBranchAddress("GENLepton_Py", genLepton_py);
        chain->SetBranchAddress("GENLepton_Pz", genLepton_pz);
        chain->SetBranchAddress("GENLepton_mother", &genLepton_mother);
        chain->SetBranchAddress("GENLepton_pT", genLepton_pT);
        chain->SetBranchAddress("GENLepton_eta", genLepton_eta);
        chain->SetBranchAddress("GENLepton_phi", genLepton_phi);
        chain->SetBranchAddress("GENLepton_charge", &genLepton_charge);
        chain->SetBranchAddress("GENLepton_status", &genLepton_status);
        chain->SetBranchAddress("GENLepton_ID", &genLepton_ID);
        chain->SetBranchAddress("GENEvt_weight", &GENEvt_weight);
        chain->SetBranchAddress("GENLepton_isPrompt", &GENLepton_isPrompt);
        chain->SetBranchAddress("GENLepton_isPromptFinalState", &GENLepton_isPromptFinalState);
        chain->SetBranchAddress("GENLepton_isTauDecayProduct", &GENLepton_isTauDecayProduct);
        chain->SetBranchAddress("GENLepton_isPromptTauDecayProduct", &GENLepton_isPromptTauDecayProduct);
        chain->SetBranchAddress("GENLepton_isDirectPromptTauDecayProductFinalState", &GENLepton_isDirectPromptTauDecayProductFinalState);
        chain->SetBranchAddress("GENLepton_isHardProcess", &GENLepton_isHardProcess);
        chain->SetBranchAddress("GENLepton_isLastCopy", &GENLepton_isLastCopy);
        chain->SetBranchAddress("GENLepton_isLastCopyBeforeFSR", &GENLepton_isLastCopyBeforeFSR);
        chain->SetBranchAddress("GENLepton_isPromptDecayed", &GENLepton_isPromptDecayed);
        chain->SetBranchAddress("GENLepton_isDecayedLeptonHadron", &GENLepton_isDecayedLeptonHadron);
        chain->SetBranchAddress("GENLepton_fromHardProcessBeforeFSR", &GENLepton_fromHardProcessBeforeFSR);
        chain->SetBranchAddress("GENLepton_fromHardProcessDecayed", &GENLepton_fromHardProcessDecayed);
        chain->SetBranchAddress("GENLepton_fromHardProcessFinalState", &GENLepton_fromHardProcessFinalState);


        //  *****************************************
        //  Electron Variables***********************
        //  *****************************************

        chain->SetBranchStatus("Nelectrons", 1);
        chain->SetBranchStatus("Electron_pT", 1);
        chain->SetBranchStatus("Electron_eta", 1);
        chain->SetBranchStatus("Electron_phi", 1);
        chain->SetBranchStatus("Electron_ecalDriven", 1);
        chain->SetBranchStatus("Electron_et", 1);
        chain->SetBranchStatus("Electron_etaSC", 1);
        chain->SetBranchStatus("Electron_sigmaIEtaIEta", 1);
        chain->SetBranchStatus("Electron_dPhiIn", 1);
        chain->SetBranchStatus("Electron_dEtaIn", 1);
        chain->SetBranchStatus("Electron_InvEminusInvP", 1);
        chain->SetBranchStatus("Electron_HoverE", 1);
        chain->SetBranchStatus("Electron_dxyVTX", 1);
        chain->SetBranchStatus("Electron_dzVTX", 1);
        chain->SetBranchStatus("Electron_mHits", 1);
        chain->SetBranchStatus("Electron_phIso03", 1);
        chain->SetBranchStatus("Electron_nhIso03", 1);
        chain->SetBranchStatus("Electron_chIso03", 1);
        
        chain->SetBranchStatus("Electron_E", 1);
        chain->SetBranchStatus("Electron_Px", 1);
        chain->SetBranchStatus("Electron_Py", 1);
        chain->SetBranchStatus("Electron_Pz", 1);
        
        chain->SetBranchStatus("Electron_ECalib", 1);
        chain->SetBranchStatus("Electron_PxCalib", 1);
        chain->SetBranchStatus("Electron_PyCalib", 1);
        chain->SetBranchStatus("Electron_PzCalib", 1);
        chain->SetBranchStatus("Electron_etCalib", 1);
        chain->SetBranchStatus("Electron_pTCalib", 1);


        chain->SetBranchAddress("Nelectrons", &Nelectrons);
        chain->SetBranchAddress("Electron_pT", Electron_pT);
        chain->SetBranchAddress("Electron_eta", Electron_eta);
        chain->SetBranchAddress("Electron_phi", Electron_phi);
        chain->SetBranchAddress("Electron_ecalDriven", Electron_ecalDriven);
        chain->SetBranchAddress("Electron_et", Electron_et);
        chain->SetBranchAddress("Electron_etaSC", Electron_etaSC);
        chain->SetBranchAddress("Electron_sigmaIEtaIEta", Electron_sigmaIEtaIEta);
        chain->SetBranchAddress("Electron_dPhiIn", Electron_dPhiIn);
        chain->SetBranchAddress("Electron_dEtaIn", Electron_dEtaIn);
        chain->SetBranchAddress("Electron_InvEminusInvP", Electron_InvEminusInvP);
        chain->SetBranchAddress("Electron_HoverE", Electron_HoverE);
        chain->SetBranchAddress("Electron_dxyVTX", Electron_dxyVTX);
        chain->SetBranchAddress("Electron_dzVTX", Electron_dzVTX);
        chain->SetBranchAddress("Electron_mHits", Electron_mHits);
        chain->SetBranchAddress("Electron_phIso03", Electron_phIso03);
        chain->SetBranchAddress("Electron_nhIso03", Electron_nhIso03);
        chain->SetBranchAddress("Electron_chIso03", Electron_chIso03);
                
        chain->SetBranchAddress("Electron_E", Electron_E );
        chain->SetBranchAddress("Electron_Px", Electron_Px );
        chain->SetBranchAddress("Electron_Py", Electron_Py );
        chain->SetBranchAddress("Electron_Pz", Electron_Pz );
        
        chain->SetBranchAddress("Electron_ECalib", Electron_ECalib);
        chain->SetBranchAddress("Electron_PxCalib", Electron_PxCalib);
        chain->SetBranchAddress("Electron_PyCalib", Electron_PyCalib);
        chain->SetBranchAddress("Electron_PzCalib", Electron_PzCalib);
        chain->SetBranchAddress("Electron_etCalib", Electron_etCalib);
        chain->SetBranchAddress("Electron_pTCalib", Electron_pTCalib);

        //  *****************************************
        //  Muon Variables***************************
        //  *****************************************
        chain->SetBranchStatus("nMuon", 1);
        chain->SetBranchStatus("Muon_pT", 1);
        chain->SetBranchStatus("Muon_eta", 1);
        chain->SetBranchStatus("Muon_phi", 1);
        chain->SetBranchStatus("Muon_muonType", 1);
        chain->SetBranchStatus("Muon_chi2dof", 1);
        chain->SetBranchStatus("Muon_muonHits", 1);
        chain->SetBranchStatus("Muon_nSegments", 1);
        chain->SetBranchStatus("Muon_nMatches", 1);
        chain->SetBranchStatus("Muon_trackerLayers", 1);
        chain->SetBranchStatus("Muon_pixelHits", 1);
        chain->SetBranchStatus("Muon_dxyVTX", 1);
        chain->SetBranchStatus("Muon_dzVTX", 1);
        chain->SetBranchStatus("Muon_trkiso", 1);
        chain->SetBranchStatus("isPFmuon", 1);
        chain->SetBranchStatus("isGLBmuon", 1);
        
        chain->SetBranchStatus("Muon_Px", 1);
        chain->SetBranchStatus("Muon_Py", 1);
        chain->SetBranchStatus("Muon_Pz", 1);
        
        chain->SetBranchStatus("Muon_charge", 1);
        
        chain->SetBranchStatus("Muon_cktpT", 1);
        chain->SetBranchStatus("Muon_cktpTError", 1);
        chain->SetBranchStatus("Muon_dxycktVTX", 1);
        chain->SetBranchStatus("Muon_dzcktVTX", 1);
        chain->SetBranchStatus("Muon_cktPx", 1);
        chain->SetBranchStatus("Muon_cktPy", 1);
        chain->SetBranchStatus("Muon_cktPz", 1);
        
        chain->SetBranchStatus("Muon_PfChargedHadronIsoR04", 1);
        chain->SetBranchStatus("Muon_PfNeutralHadronIsoR04" ,1);
        chain->SetBranchStatus("Muon_PfGammaIsoR04", 1);

        chain->SetBranchStatus("CosAngle", 1);
        chain->SetBranchStatus("vtxTrkChi2", 1);
        chain->SetBranchStatus("vtxTrkProb", 1);
        chain->SetBranchStatus("vtxTrkNdof", 1);
        chain->SetBranchStatus("vtxTrkCkt1Pt", 1);
        chain->SetBranchStatus("vtxTrkCkt2Pt", 1);
        chain->SetBranchStatus("vtxTrkDiEChi2", 1);
        chain->SetBranchStatus("vtxTrkDiEProb", 1);
        chain->SetBranchStatus("vtxTrkDiENdof", 1);
        chain->SetBranchStatus("vtxTrkDiE1Pt", 1);
        chain->SetBranchStatus("vtxTrkDiE2Pt", 1);
        chain->SetBranchStatus("vtxTrkEMuChi2", 1);
        chain->SetBranchStatus("vtxTrkEMuProb", 1);
        chain->SetBranchStatus("vtxTrkEMuNdof", 1);
        chain->SetBranchStatus("vtxTrkEMu1Pt", 1);
        chain->SetBranchStatus("vtxTrkEMu2Pt", 1);

        chain->SetBranchAddress("nMuon", &nMuon);
        chain->SetBranchAddress("Muon_pT", Muon_pT);
        chain->SetBranchAddress("Muon_eta", Muon_eta);
        chain->SetBranchAddress("Muon_phi", Muon_phi);
        chain->SetBranchAddress("Muon_muonType", Muon_muonType);
        chain->SetBranchAddress("Muon_chi2dof", Muon_chi2dof);
        chain->SetBranchAddress("Muon_muonHits", Muon_muonHits);
        chain->SetBranchAddress("Muon_nSegments", Muon_nSegments);
        chain->SetBranchAddress("Muon_nMatches", Muon_nMatches);
        chain->SetBranchAddress("Muon_trackerLayers", Muon_trackerLayers);
        chain->SetBranchAddress("Muon_pixelHits", Muon_pixelHits);
        chain->SetBranchAddress("Muon_dxyVTX", Muon_dxyVTX);
        chain->SetBranchAddress("Muon_dzVTX", Muon_dzVTX);
        chain->SetBranchAddress("Muon_trkiso", Muon_trkiso);
        chain->SetBranchAddress("isPFmuon", isPFmuon);
        chain->SetBranchAddress("isGLBmuon", isGLBmuon);
        
        chain->SetBranchAddress("Muon_Px", Muon_Px );
        chain->SetBranchAddress("Muon_Py", Muon_Py );
        chain->SetBranchAddress("Muon_Pz", Muon_Pz );
        
        chain->SetBranchAddress("Muon_charge", Muon_charge);
        
        chain->SetBranchAddress("Muon_cktpT", Muon_cktpT);
        chain->SetBranchAddress("Muon_cktpTError", Muon_cktpTError);
        chain->SetBranchAddress("Muon_dxycktVTX", Muon_dxycktVTX);
        chain->SetBranchAddress("Muon_dzcktVTX", Muon_dzcktVTX);
        chain->SetBranchAddress("Muon_cktPx", Muon_cktPx);
        chain->SetBranchAddress("Muon_cktPy", Muon_cktPy);
        chain->SetBranchAddress("Muon_cktPz", Muon_cktPz);
        
        chain->SetBranchAddress("Muon_PfChargedHadronIsoR04", Muon_PfChargedHadronIsoR04);
        chain->SetBranchAddress("Muon_PfNeutralHadronIsoR04", Muon_PfNeutralHadronIsoR04);
        chain->SetBranchAddress("Muon_PfGammaIsoR04", Muon_PfGammaIsoR04);

        chain->SetBranchAddress("CosAngle", &CosAngle);
        chain->SetBranchAddress("vtxTrkChi2", &vtxTrkChi2);
        chain->SetBranchAddress("vtxTrkProb", &vtxTrkProb);
        chain->SetBranchAddress("vtxTrkNdof", &vtxTrkNdof);
        chain->SetBranchAddress("vtxTrkCkt1Pt", &vtxTrkCkt1Pt);
        chain->SetBranchAddress("vtxTrkCkt2Pt", &vtxTrkCkt2Pt);
        chain->SetBranchAddress("vtxTrkDiEChi2", &vtxTrkDiEChi2);
        chain->SetBranchAddress("vtxTrkDiEProb", &vtxTrkDiEProb);
        chain->SetBranchAddress("vtxTrkDiENdof", &vtxTrkDiENdof);
        chain->SetBranchAddress("vtxTrkDiE1Pt", &vtxTrkDiE1Pt);
        chain->SetBranchAddress("vtxTrkDiE2Pt", &vtxTrkDiE2Pt);
        chain->SetBranchAddress("vtxTrkEMuChi2", &vtxTrkEMuChi2);
        chain->SetBranchAddress("vtxTrkEMuProb", &vtxTrkEMuProb);
        chain->SetBranchAddress("vtxTrkEMuNdof", &vtxTrkEMuNdof);
        chain->SetBranchAddress("vtxTrkEMu1Pt", &vtxTrkEMu1Pt);
        chain->SetBranchAddress("vtxTrkEMu2Pt", &vtxTrkEMu2Pt);
    }

    void GetEvent(Int_t i)
    {
        if(!chain) return;
        
        chain->GetEntry(i);
    }

    //for Zprime Study(Electron channel)
    Bool_t isHLT_DoubleEle33()
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*" )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

    //for Zprime Study(Muon channel)
    Bool_t isHLT_MU40_eta2p1()
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == "HLT_Mu40_eta2p1_v*" )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

    //for DY diff.x-section binning study(Muon channel)
    Bool_t isHLT_Mu17_Mu8()
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == "HLT_Mu17_Mu8_v*" )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

    //for DY diff.x-section binning study(Electron channel)
    Bool_t isHLT_Ele23_Ele12()
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == "HLT_Ele23_Ele12_CaloId_TrackId_Iso_v*" )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

    //for 13TeV Data(Express)
    Bool_t isHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ()
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*" )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

    //for 13TeV Data(SingleMuon)
    Bool_t isHLT_IsoMu27()
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == "HLT_IsoMu27_v*" )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

    Bool_t isTriggered(TString HLT)
    {
        Bool_t isTrigger = false;
        for( Int_t k = 0; k < HLT_ntrig; k++ )
        {
            if( (HLT_trigName->at((unsigned int)k)) == HLT )
            {
                if( HLT_trigFired[k] == 1 )
                {
                    isTrigger = true;
                    break;
                }
            }
        }
        return isTrigger;
    }

};
