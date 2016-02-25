#include <TROOT.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TFile.h>
#include <TString.h>
#include <fstream>
#include <iostream>

void print2D_Andrius() {

  double diagonals[132] = {0.00220474, 0.00211757, 0.00208329, 0.00196359, 0.00188362, 0.00178958, 0.00170293, 0.00147753, 0.00132556, 0.00123584, 0.00114805, 0.00112789, 0.00114458, 0.00130523, 0.00135017, 0.00140954, 0.0014109, 0.00147887, 0.00140234, 0.00126359, 0.00112224, 0.000942219, 0.000836654, 0.000407358, 0.00178671, 0.001573, 0.00152372, 0.00145345, 0.00142417, 0.00143499, 0.00144411, 0.00147708, 0.00148128, 0.00142131, 0.00140812, 0.00131649, 0.0012678, 0.0012579, 0.00119449, 0.00116062, 0.0011051, 0.00101312, 0.00100221, 0.000896522, 0.000852432, 0.000762897, 0.000637827, 0.000318889,0.000619175, 0.00063628, 0.000683457, 0.000663991, 0.000833326, 0.000576589, 0.000604494, 0.000665053, 0.000562349, 0.000571806, 0.000574754, 0.000641049, 0.000559305, 0.000510976, 0.000465562, 0.000552576, 0.000426906, 0.000394093, 0.000473651, 0.000490072, 0.000487637, 0.000310961, 0.000382431, 0.000199051, 0.00820416, 0.00817875, 0.00823994, 0.00826059, 0.0083192, 0.00826397, 0.00818159, 0.00799829, 0.0079163, 0.00798274, 0.00788228, 0.00776324, 0.00757453, 0.00723493, 0.00688138, 0.00638258, 0.00615382, 0.00560927, 0.00511678, 0.00477886, 0.00480081, 0.00446441, 0.0035405, 0.00140224, 0.000313394, 0.000313743, 0.000317145, 0.000302152, 0.000274664, 0.000282591, 0.000290322, 0.000263838, 0.000275426, 0.000243859, 0.000232308, 0.000223118, 0.000221724, 0.000214459, 0.000212437, 0.000205239, 0.000199126, 0.000188036, 0.000177693, 0.000186241, 0.000147574, 0.000143643, 0.00010251, 5.09799e-05, 0.000139172, 0.000115512, 0.000103777, 9.132e-05, 7.77451e-05, 7.36202e-05, 6.34563e-05, 5.60115e-05, 5.03875e-05, 4.09827e-05, 3.82219e-05, 1.55491e-05};


  TString norm_type = "abs";//"shapes" "abs","dydy" 
  // get the covariance matrix
  //we will always start from normalized (?) and provide two other normalizations
  //multiplied by dM_i dM_j
  //multiplied by dM_i dM_j and by sigma_Z --> this one we get from unnormalized
  double sigma_Z = 524.699999999999932;
  TFile fin("covariance_finalResults_2D_mu_preFSR_inAcc_normalized.root","read");
  TMatrixD *Morig=(TMatrixD*)fin.Get("totalCov_TM");
  TMatrixD *Corig=(TMatrixD*)fin.Get("totalCorr_TM");
  fin.Close();

  // copy
  TMatrixD M(*Morig);
  TMatrixD MAlexey(*Morig);
  delete Morig; // no longer need original

  // get errors to apply the correlation cut-off
  int dim=M.GetNcols();
  TVectorD S(dim);

  for (int i=0; i<dim; i++) {
    S[i]=sqrt(M(i,i));
  }

  // correlation cut-off
  for (int ir=0; ir<dim; ir++) {
    for (int ic=0; ic<dim; ic++) {
      if (fabs(M(ir,ic)/(S[ir]*S[ic]))<0.01) M(ir,ic)=0.;
    }
  }

  //
  // Now ready to print the matrix
  //

  // define ranges
  const int nMassBins=6;
  const int nYBins[nMassBins]={ 24, 24, 24, 24, 24, 12 };
  const int massLimits[nMassBins+1] = { 20, 30, 45, 60, 120, 200, 1500 };

  int fi_min=0;
  for (int iM=0; iM<nMassBins; iM++) {
    int fj_min=fi_min;
    for (int jM=iM; jM<nMassBins; ++jM) {
      TString fname=Form("cov_%d_%d_vs_%d_%d.out",
			 massLimits[iM],massLimits[iM+1],
			 massLimits[jM],massLimits[jM+1]);
      std::ofstream fout(fname.Data());
      fout << "Covariance matrix in the muon channel for mass bins "
	   << Form("(%d,%d) and (%d,%d)\n",
		   massLimits[iM],massLimits[iM+1],
		   massLimits[jM],massLimits[jM+1])
	   << "mass-rapidity-bin-index= rapBin + sum_{im=1}^{massBin-1} nYBins(im) \n"
	   << "x: mass-rapidity-bin-index\n"
	   << "y: mass-rapidity-bin-index\n"
	   << "z: cov(sigma_x, sigma_y)\n";
      // print top row
      fout << "    ";
      int allZeroes=1;
      for (int fj=fj_min; fj<fj_min+nYBins[jM]; fj++) fout << Form("    %6d",fj+1);
      fout << "\n";

     // print the matrix
      for (int fi=fi_min+nYBins[iM]-1; fi>=fi_min; fi--) {
	// print initial flat-index
	fout << Form("%4d",fi+1) << "    ";
	// print covariances for this row
	for (int fj=fj_min; fj<fj_min+nYBins[jM]; fj++) {
	  fout << "  " << Form("% 6.1e",M(fi,fj));
          double corr_ij = MAlexey(fi,fj)/sqrt(MAlexey(fi,fi)*MAlexey(fj,fj));
          if (fi!=fj) M(fi,fj) = diagonals[fi]*diagonals[fj]*corr_ij:
          else M(fi,fj) = diagonals[fi]*diagonals[fj];
          double val = M(fi,fj);
          //FIXME adjust the value based on the normalization type
          if (norm_type == "dydy" || norm_type == "abs") {
              if (jM != nMassBins-1 && iM != nMassBins-1) {val *= 0.1*0.1;}
              else if (((jM == nMassBins-1) && !(iM == nMassBins-1)) || (!(jM == nMassBins-1) && (iM == nMassBins-1))) {
                 val *= 0.1*0.2;
              } else {
                 val *= 0.2*0.2;
              }
              if (norm_type == "abs") val *= sigma_Z*sigma_Z;
          }
	  if (fabs(M(fi,fj))!=0.) allZeroes=0;
	}
	fout << "\n";
      }
      fj_min+=nYBins[jM];
      fout.close();
      if (allZeroes) {
	std::cout << "all entries are 0 in file <" << fname << ">\n";
      }
    }
    fi_min+=nYBins[iM];
  }

  TFile * fFile = new TFile("covariance_2D_mu_preFSR_inAcc_normalized_Updated.root","RECREATE");
  M.Write("totalCov_TM");
  Corig->Write("totalCorr_TM");
  fFile->Close();
}
