void stattools() {
}

//  
//Estimator type #1
//
double estimatorPerMassHisto1(TH1D* htp, TH1D* htpd) {
   double estimator = 0;
  
  for (int i = 0; i < htp->GetXaxis()->GetNbins(); i++) {
     estimator += pow(htp->GetBinError(i+1)/htp->GetBinContent(i+1),2) + pow(htpd->GetBinError(i+1)/htpd->GetBinContent(i+1),2);
  }
  return 10.*sqrt(estimator);
} 

//
//Estimator type #2 with MC truth similarity weight
//
double estimatorPerMassHisto2(TH1D* htp, TH1D* htpd, TH1D* hmct) {
   double estimator = 0;
    
  if (htp->GetXaxis()->GetNbins() != hmct->GetXaxis()->GetNbins()) {
    cout << "Incompatible dimensions!!" << endl;
    return -1;
  } else {
   for (int i = 0; i < htp->GetXaxis()->GetNbins(); i++) { 
       estimator += pow(htp->GetBinError(i+1),2) + pow(htpd->GetBinError(i+1),2)  + pow((hmct->GetBinContent(i+1)-htp->GetBinContent(i+1)),2.);
   }
   return 10.*sqrt(estimator)/htp->GetXaxis()->GetNbins();
 }
} 
