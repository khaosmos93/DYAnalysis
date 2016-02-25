#ifndef EVENTINFO_H
#define EVENTINFO_H

namespace purdue
{
  class EventInfo
  {
    public:
      EventInfo():
      runNum_(0), evtNum_(0), lumiSec_(0), nPU_(0), PUreweightIn_(0), PUreweight_(0), PUreweightminus_(0), PUreweightplus_(0),
      PUweight_(0),
      pvx_(0), pvy_(0), pvz_(0), 
      rhoIso_(0), hasGoodPV_(0)		    
      {}
      ~EventInfo(){}

      unsigned int runNum_; 			             // run number in data
      unsigned int evtNum_; 			             // event number in data
      unsigned int lumiSec_;			             // lumi section
      unsigned int nPU_, PUreweightIn_;
      unsigned int PUreweight_, PUreweightminus_, PUreweightplus_;                // number of reconstructed pile up vertices in event (MC only)       
      unsigned int PUweight_; //other weight
      double pvx_, pvy_, pvz_;		             // best primary vertex
      double rhoIso_;
      bool  hasGoodPV_;                             // event has a good PV?
  };
}
#endif
