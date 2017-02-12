root -l -b << EOF

gSystem->Load("/home/kplee/Unfolding/libRooUnfold.so")

.x FSRCorrections_DressedLepton.C++

.q

EOF
