root -l -b << EOF

gSystem->Load("/home/kplee/Unfolding/libRooUnfold.so")

.x MuonResponseMatrix_1D.C++

.q

EOF
