void chi2Estimator() {


 gROOT->ProcessLine(".L covariantMatrix.C");
 
 double edges[12] = {0,23,24,47,48,71,72,95,96,119,120,131};

 for (int i = 0; i < 11; i++) {
   cout << edges[i] << " " << edges[i+1] << endl;
   estimateChi2(edges[i],edges[i+1]);
   i++;
 } 


}
