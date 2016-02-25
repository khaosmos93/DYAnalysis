#include "DataFormats/Common/interface/Wrapper.h"

#include "../interface/Muon.h"
#include "../interface/Electron.h"
#include "../interface/Dimuon.h"
#include "../interface/Dielectron.h"
#include "../interface/Photon.h"
#include "../interface/Jet.h"
#include "../interface/EventInfo.h"
#include <vector>

namespace {
  struct Purdue_DataFormats {
    purdue::Muon dummyMuon;
    edm::Wrapper<purdue::Muon> dummyMuonWrapper;
    std::vector<purdue::Muon> dummyMuonCollection;
    edm::Wrapper<std::vector<purdue::Muon> > dummyMuonCollectionWrapper;
    edm::Ptr<purdue::Muon> dummyMuonPtr;

    purdue::Electron dummyElectron;
    edm::Wrapper<purdue::Electron> dummyElectronWrapper;
    std::vector<purdue::Electron> dummyElectronCollection;
    edm::Wrapper<std::vector<purdue::Electron> > dummyElectronCollectionWrapper;
    edm::Ptr<purdue::Electron> dummyElectronPtr;

    purdue::Dielectron dummyDielectron;
    edm::Wrapper<purdue::Dielectron> dummyDielectronWrapper;
    std::vector<purdue::Dielectron> dummyDielectronCollection;
    edm::Wrapper<std::vector<purdue::Dielectron> > dummyDielectronCollectionWrapper;
    edm::Ptr<purdue::Dielectron> dummyDielectronPtr;

    purdue::Dimuon dummyDimuon;
    edm::Wrapper<purdue::Dimuon> dummyDimuonWrapper;
    std::vector<purdue::Dimuon> dummyDimuonCollection;
    edm::Wrapper<std::vector<purdue::Dimuon> > dummyDimuonCollectionWrapper;
    edm::Ptr<purdue::Dimuon> dummyDimuonPtr;

    purdue::Photon dummyPhoton;
    edm::Wrapper<purdue::Photon> dummyPhotonWrapper;
    std::vector<purdue::Photon> dummyPhotonCollection;
    edm::Wrapper<std::vector<purdue::Photon> > dummyPhotonCollectionWrapper;
    edm::Ptr<purdue::Photon> dummyPhotonPtr;

    purdue::EventInfo dummyEventInfo;
    edm::Wrapper<purdue::EventInfo> dummyEventInfoWrapper;
    std::vector<purdue::EventInfo> dummyEventInfoCollection;
    edm::Wrapper<std::vector<purdue::EventInfo> > dummyEventInfoCollectionWrapper;
    edm::Ptr<purdue::EventInfo> dummyEventInfoPtr;

    purdue::Jet dummyJet;
    edm::Wrapper<purdue::Jet> dummyJetWrapper;
    std::vector<purdue::Jet> dummyJetCollection;
    edm::Wrapper<std::vector<purdue::Jet> > dummyJetCollectionWrapper;
    edm::Ptr<purdue::Jet> dummyJetPtr;

  };

}
