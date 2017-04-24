#!/bin/bash

root -l -b << EOF

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x SysUnc_EffCorr.C++

.q

EOF

