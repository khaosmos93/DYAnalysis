void MomScaleCorrection (
  rochcor* rmcor, TLorentzVector& muMinus, TLorentzVector& muPlus,
  double rpx1, double rpy1, double rpz1, int charge1, 
  double rpx2, double rpy2, double rpz2, int charge2,
  int massOpt = 2, int sysOpt = 0, int runOpt = 0, bool isData = true ) {

  double mu_mass = 0.105658;
  double rp1 = rpx1*rpx1 + rpy1*rpy1 + rpz1*rpz1;
  double rp2 = rpx2*rpx2 + rpy2*rpy2 + rpz2*rpz2;
  double re1 = sqrt(mu_mass*mu_mass + rp1);
  double re2 = sqrt(mu_mass*mu_mass + rp2);

  if( charge1 > 0 ) {
    muPlus.SetPxPyPzE(rpx1, rpy1, rpz1, re1);
    muMinus.SetPxPyPzE(rpx2, rpy2, rpz2, re2);
  }
  else {
    muPlus.SetPxPyPzE(rpx2, rpy2, rpz2, re2);
    muMinus.SetPxPyPzE(rpx1, rpy1, rpz1, re1);
  }

  if( isData ) {
    rmcor->momcor_data(muPlus, 1, sysOpt, runOpt);
    rmcor->momcor_data(muMinus, -1, sysOpt, runOpt);
  }
  else {
    rmcor->momcor_mc(muPlus, 1, sysOpt, runOpt);
    rmcor->momcor_mc(muMinus, -1, sysOpt, runOpt);
  }
  //cout << "Muon = " << muPlus.Pt() << " " << muMinus.Pt() << endl;
}
