#!/bin/bash

root -l -b << EOF

TString BasePath = gSystem->Getenv("KP_ROOUNFOLD_PATH")
gSystem->Load(BasePath+"/libRooUnfold.so")

.x FSRCorrections_DressedLepton.C++

.q

EOF

echo "finished"

