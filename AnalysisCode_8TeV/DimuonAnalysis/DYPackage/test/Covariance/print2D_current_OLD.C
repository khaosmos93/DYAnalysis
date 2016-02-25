#include <TROOT.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TFile.h>
#include <TString.h>
#include <fstream>
#include <iostream>

void print2D_current(TString caseAna, TString channel, TString fsrFlag, TString accFlag, TString norm_type){

  double sigma_Z = 524.699999999999932;
  if (fsrFlag == "post") sigma_Z = 516.5;

  cout << "Reading " << "covariance_finalResults_2D_mu_"+fsrFlag+"FSR_"+accFlag+"_normalized.root" << endl;
  TFile fin("covariance_finalResults_2D_mu_"+fsrFlag+"FSR_"+accFlag+"_normalized.root","read");
  TMatrixD *Morig=(TMatrixD*)fin.Get("totalCov_TM");
  TMatrixD *Corig=(TMatrixD*)fin.Get("totalCorr_TM");
  fin.Close();

  // copy
  TMatrixD M(*Morig);
  TMatrixD MAlexey(*Morig);
  TMatrixD MTEST(*Morig);
  delete Morig; // no longer need original

  // get errors to apply the correlation cut-off
  int dim=M.GetNcols();
  TVectorD S(dim);

  for (int i=0; i<dim; i++) {
    S[i]=sqrt(M(i,i));
  }

  //
  // Now ready to print the matrix
  //

  // define ranges
  const int nMassBins=6;
  const int nYBins[nMassBins]={ 24, 24, 24, 24, 24, 12 };
  const int massLimits[nMassBins+1] = { 20, 30, 45, 60, 120, 200, 1500 };

  double diagonals_preFSR[132] = {0.00220474, 0.00211757, 0.00208329, 0.00196359, 0.00188362, 0.00178958, 0.00170293, 0.00147753, 0.00132556, 0.00123584, 0.00114805, 0.00112789, 0.00114458, 0.00130523, 0.00135017, 0.00140954, 0.0014109, 0.00147887, 0.00140234, 0.00126359, 0.00112224, 0.000942219, 0.000836654, 0.000407358, 0.00178671, 0.001573, 0.00152372, 0.00145345, 0.00142417, 0.00143499, 0.00144411, 0.00147708, 0.00148128, 0.00142131, 0.00140812, 0.00131649, 0.0012678, 0.0012579, 0.00119449, 0.00116062, 0.0011051, 0.00101312, 0.00100221, 0.000896522, 0.000852432, 0.000762897, 0.000637827, 0.000318889,0.000619175, 0.00063628, 0.000683457, 0.000663991, 0.000833326, 0.000576589, 0.000604494, 0.000665053, 0.000562349, 0.000571806, 0.000574754, 0.000641049, 0.000559305, 0.000510976, 0.000465562, 0.000552576, 0.000426906, 0.000394093, 0.000473651, 0.000490072, 0.000487637, 0.000310961, 0.000382431, 0.000199051, 0.00820416, 0.00817875, 0.00823994, 0.00826059, 0.0083192, 0.00826397, 0.00818159, 0.00799829, 0.0079163, 0.00798274, 0.00788228, 0.00776324, 0.00757453, 0.00723493, 0.00688138, 0.00638258, 0.00615382, 0.00560927, 0.00511678, 0.00477886, 0.00480081, 0.00446441, 0.0035405, 0.00140224, 0.000313394, 0.000313743, 0.000317145, 0.000302152, 0.000274664, 0.000282591, 0.000290322, 0.000263838, 0.000275426, 0.000243859, 0.000232308, 0.000223118, 0.000221724, 0.000214459, 0.000212437, 0.000205239, 0.000199126, 0.000188036, 0.000177693, 0.000186241, 0.000147574, 0.000143643, 0.00010251, 5.09799e-05, 0.000139172, 0.000115512, 0.000103777, 9.132e-05, 7.77451e-05, 7.36202e-05, 6.34563e-05, 5.60115e-05, 5.03875e-05, 4.09827e-05, 3.82219e-05, 1.55491e-05};
  double rdiagonals_preFSR[132] = {0.0339971, 0.0337208, 0.032773, 0.0335877, 0.0340049, 0.0344678, 0.0351017, 0.0345766, 0.0344008, 0.0339871, 0.0334087, 0.0332816, 0.0322232, 0.0322838, 0.0310592, 0.0313493, 0.0294624, 0.0289783, 0.0268042, 0.0240434, 0.0209985, 0.0160163, 0.0104074, 0.00394112, 0.0512196, 0.0488462, 0.049328, 0.0489294, 0.0497668, 0.0516309, 0.0498744, 0.0488812, 0.0501116, 0.0487699, 0.0490278, 0.0486872, 0.0467183, 0.049095, 0.0459175, 0.0444989, 0.0413405, 0.0367446, 0.0315492, 0.0262498, 0.0209891, 0.0150392, 0.00890852, 0.00310112, 0.0213773, 0.0213444, 0.021062, 0.0216232, 0.020339, 0.0222888, 0.0218933, 0.0214533, 0.0215288, 0.0212879, 0.0205312, 0.0196386, 0.0185561, 0.0186806, 0.0170245, 0.0159697, 0.0140271, 0.0122443, 0.0112471, 0.00938589, 0.0077171, 0.00489739, 0.00342707, 0.00116623,0.604588, 0.600796, 0.601957, 0.596068, 0.595779, 0.590047, 0.584797, 0.57086, 0.557382, 0.54488, 0.526063, 0.503772, 0.475897, 0.445807, 0.417606, 0.374982, 0.339285, 0.292885, 0.250475, 0.20262, 0.156481, 0.108768, 0.0637553, 0.0202485,
0.00662107, 0.0068695, 0.00674016, 0.00671329, 0.006447, 0.00671166, 0.00630126, 0.00632929, 0.00613147, 0.00581744, 0.00577516, 0.00531591, 0.00516504, 0.00479951, 0.00464618, 0.00416784, 0.00371484, 0.0030139, 0.00238956, 0.00220813, 0.00138641, 0.000996914, 0.000525727, 0.000204149, 0.00100991, 0.00115943, 0.00124304, 0.000994088, 0.00101695, 0.00100447, 0.000906887, 0.000616144, 0.000469744, 0.000288772, 0.000184214, 4.10414e-05};

  double diagonals_postFSR[132] = {0.00221175, 0.00212597, 0.00208369, 0.00198295, 0.00190802, 0.00182345, 0.00173207, 0.00152478, 0.00137675, 0.00128194, 0.00120738, 0.00120436, 0.00121263, 0.00136807, 0.00139429, 0.00146451, 0.00146562, 0.00152889, 0.00145013, 0.00130909, 0.00114496, 0.000954991, 0.000795812, 0.000393729, 0.00180454, 0.00159939, 0.00153431, 0.0014354, 0.00142042, 0.00147654, 0.00148879, 0.00150211, 0.00153454, 0.00146574, 0.00145038, 0.00138594, 0.00132894, 0.00133732, 0.00126548, 0.00123125, 0.00114978, 0.00107037, 0.00102759, 0.000934409, 0.000851672, 0.000725795, 0.00065131, 0.000296067, 0.000737911, 0.000735332, 0.000724041, 0.000716565, 0.000688831, 0.000705611, 0.000704845, 0.000703817, 0.00069591, 0.000703824, 0.000680863, 0.000665061, 0.000653354, 0.000634177, 0.00058112, 0.000541761, 0.00050425, 0.000485744, 0.000442929, 0.00040406, 0.000364285, 0.000309282, 0.000245493, 0.000136572, 0.0080247, 0.00800476, 0.00806034, 0.00808799, 0.00815336, 0.00810854, 0.00802642, 0.00785401, 0.00778262, 0.00780448, 0.00776131, 0.00766702, 0.00747821, 0.00715004, 0.00681454, 0.00633735, 0.00606506, 0.00554794, 0.00507419, 0.0048055, 0.0047721, 0.00445233, 0.00353591, 0.00137453, 0.00034878, 0.000349607, 0.00034785, 0.00033325, 0.000310629, 0.000321449, 0.000324078, 0.000303162, 0.000315696, 0.000284189, 0.000275219, 0.000264221, 0.000260037, 0.000252548, 0.00025174, 0.000243753, 0.000234663, 0.000214962, 0.000204784, 0.00020642, 0.000165171, 0.000152023, 0.000113424, 6.24019e-05, 0.000150168, 0.000129036, 0.000117808, 0.000102805, 9.06346e-05, 8.79511e-05, 7.67355e-05, 6.53114e-05, 5.9421e-05, 4.99151e-05, 4.55147e-05, 1.97611e-05};

  double rdiagonals_postFSR[132] = {0.0336475, 0.0332778, 0.0326289, 0.0332891, 0.0337836, 0.0342364, 0.034789, 0.0342068, 0.0340713, 0.0336801, 0.0329994, 0.0330048, 0.031868, 0.0319083, 0.0308544, 0.0309242, 0.0290639, 0.0285542, 0.02643, 0.0235605, 0.020521, 0.0155586, 0.0100116, 0.00371247, 0.0515544, 0.0491292, 0.0497977, 0.0493683, 0.0501341, 0.0520321, 0.0501604, 0.0493229, 0.0503617, 0.049072, 0.0493856, 0.0488609, 0.0470033, 0.0491916, 0.0459866, 0.0445097, 0.0411828, 0.0365744, 0.0313967, 0.0260833, 0.0207742, 0.0148265, 0.00878173, 0.00297568,
0.0256592, 0.0257041, 0.0253218, 0.0260294, 0.0248893, 0.0266365, 0.0262303, 0.0256931, 0.0256673, 0.0252969, 0.0246081, 0.0236905, 0.0221783, 0.02201, 0.02022, 0.0187476, 0.0164779, 0.0144995, 0.0130875, 0.0109181, 0.00889321, 0.00575018, 0.00387918, 0.00127005,
0.602668, 0.59906, 0.600088, 0.594422, 0.594111, 0.588837, 0.583597, 0.569714, 0.556393, 0.543969, 0.525012, 0.502787, 0.475247, 0.445218, 0.416989, 0.374532, 0.338999, 0.29251, 0.250117, 0.202428, 0.156312, 0.108611, 0.0636249, 0.0201009,
0.00631477, 0.00651359, 0.00642427, 0.00636077, 0.00614697, 0.0063431, 0.00597265, 0.00601271, 0.00586154, 0.00557037, 0.00550196, 0.00508115, 0.0049044, 0.00452822, 0.00440214, 0.00397127, 0.00353436, 0.0028604, 0.00227513, 0.00209037, 0.00131489, 0.000937039, 0.000501518, 0.000191148,
0.000957856, 0.00111243, 0.00119348, 0.00095393, 0.000971326, 0.000964124, 0.000861841, 0.000586147, 0.000449087, 0.000277411, 0.000177376, 3.9162e-05};


  double sigmaZerrNoLumi = 0.;
  TString accFlag1 = accFlag;
  if (accFlag == "fullAcc") accFlag1= "fullSpace";

  if (fsrFlag == "pre" && accFlag == "inAcc") sigmaZerrNoLumi = pow(0.4,2) +pow(5.1,2) +pow(1.2,2);
  if (fsrFlag == "post" && accFlag == "inAcc")   sigmaZerrNoLumi =pow(0.4,2) +pow(4.9,2) +pow(1.1,2);
  int fi_min=0;
  for (int iM=0; iM<nMassBins; iM++) {
    int fj_min=fi_min;
    for (int jM=iM; jM<nMassBins; ++jM) {
      TString fname=Form("cov_%d_%d_vs_%d_%d_%s_%s_%s.out",
			 massLimits[iM],massLimits[iM+1],
			 massLimits[jM],massLimits[jM+1],string(fsrFlag).c_str(),string(accFlag).c_str(),string(norm_type).c_str());
      std::ofstream fout(fname.Data());
      fout << "Covariances of the Drell-Yan differential cross section in the muon channel as a function of the dilepton mass for mass bins "
	   << Form("(%d,%d) and (%d,%d)\n",
		   massLimits[iM],massLimits[iM+1],
		   massLimits[jM],massLimits[jM+1]);
           if (norm_type == "rshape") {
           string thiss =  "        # "+string(fsrFlag)+"FSR "+string(accFlag1)+" r-shape suppressed\n";
              fout << thiss.c_str();
           } else if (norm_type == "absolute") {
           string thiss =  "        # "+string(fsrFlag)+"FSR "+string(accFlag1)+" absolute suppressed\n";
              fout << thiss.c_str();
           } else {
           string thiss =  "        # "+string(fsrFlag)+"FSR "+string(accFlag1)+" (sigma/sigma_Z) suppressed\n";
              fout << thiss.c_str();
           }
           fout << "M : 20 - 1500 GeV\n";
           if (accFlag != "fullAcc") {
             fout << "ABS(ETA(MU)) <= 2.4\n";
           }
           fout <<  "PT(MU1) : >=9 GEV\n"
           <<  "PT(MU2) : >=14 GEV\n"
           <<  "RE : P P --> DYmumu X\n"
           <<  "SQRT(S) : 7000.0 GEV\n"
	   << "mass-rapidity-bin-index= rapBin + sum_{im=1}^{massBin-1} nYBins(im) \n"
	   << "x: mass-rapidity-bin-index\n"
	   << "y: mass-rapidity-bin-index\n";
           if (norm_type == "rshape") {
             fout << "z: cov(r_x,r_y)\n";
           } else if (norm_type == "absolute") {
             fout << "z: cov(sigma_x,sigma_y) [pb^2]\n";
           } else {
             fout << "z: cov(sigma_x/sigmaZ,sigma_y/sigmaZ)\n";
           }

      // print top row
      fout << "    ";
      int allZeroes=1;
      for (int fj=fj_min; fj<fj_min+nYBins[jM]; fj++) fout << Form("    %6d",fj+1);
      fout << "\n";

     // print the matrix
      for (int fi=fi_min+nYBins[iM]-1; fi>=fi_min; fi--) {
	fout << Form("%4d",fi+1) << "    ";
	for (int fj=fj_min; fj<fj_min+nYBins[jM]; fj++) {
          double corr_ij = MAlexey(fi,fj)/sqrt(MAlexey(fi,fi)*MAlexey(fj,fj));
          double prod = 0;
          if (fsrFlag == "pre" && accFlag == "inAcc") prod = diagonals_preFSR[fi]*diagonals_preFSR[fj];
          if (fsrFlag == "post" && accFlag == "inAcc") prod = diagonals_postFSR[fi]*diagonals_postFSR[fj];
          if (fi!=fj) M(fi,fj) = prod*corr_ij;
          else M(fi,fj) = prod;
          double val = M(fi,fj);
          if (norm_type == "absolute") { 
            if (fsrFlag == "pre" && accFlag == "inAcc")   val = sigma_Z*sigma_Z*val + rdiagonals_preFSR[fi]*rdiagonals_preFSR[fj]*sigmaZerrNoLumi;
            if (fsrFlag == "post" && accFlag == "inAcc")  val = sigma_Z*sigma_Z*val + rdiagonals_postFSR[fi]*rdiagonals_postFSR[fj]*sigmaZerrNoLumi;
          } 
          if (norm_type == "normalized" || norm_type == "absolute") {
              if (jM != nMassBins-1 && iM != nMassBins-1) {val *= 0.1*0.1;}
              else if (((jM == nMassBins-1) && !(iM == nMassBins-1)) || (!(jM == nMassBins-1) && (iM == nMassBins-1))) {
                 val *= 0.1*0.2;
              } else {
                 val *= 0.2*0.2;
              }
          }
          //FIXME correlation cut-off
          //if (fabs(MAlexey(fi,fj)/sqrt(MAlexey(fi,fi)*MAlexey(fj,fj)))<0.01) val=0.;
	  fout << "  " << Form("% 6.1e",val);
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

  for (int fj=0; fj<132; fj++) {
    for (int fi=0; fi < 132; fi++) {

          double corr_ij = MAlexey(fi,fj)/sqrt(MAlexey(fi,fi)*MAlexey(fj,fj));
          double prod = 0;
          if (fsrFlag == "pre") prod = diagonals_preFSR[fi]*diagonals_preFSR[fj];
          if (fsrFlag == "post") prod = diagonals_postFSR[fi]*diagonals_postFSR[fj];
          if (fi!=fj) M(fi,fj) = prod*corr_ij;
          else M(fi,fj) = prod;
          double val = M(fi,fj);
          if (norm_type == "absolute") {
            if (fsrFlag == "pre") val = sigma_Z*sigma_Z*val + rdiagonals_preFSR[fi]*rdiagonals_preFSR[fj]*sigmaZerrNoLumi;
            if (fsrFlag == "post")  val = sigma_Z*sigma_Z*val + rdiagonals_postFSR[fi]*rdiagonals_postFSR[fj]*sigmaZerrNoLumi;
          }
          if (norm_type == "normalized" || norm_type == "absolute") {
              if (fi < 120 && fj < 120) {val *= 0.1*0.1;}
              else if (((fi >=120) && !(fj >= 120)) || (!(fi >= 120) && (fj >= 120))) {
                 val *= 0.1*0.2;
              } else {
                 val *= 0.2*0.2;
              }
          }
          //FIXME correlation cut-off
          if (fabs(MAlexey(fi,fj)/sqrt(MAlexey(fi,fi)*MAlexey(fj,fj)))<0.01) val=0.;
          MTEST(fi,fj) = val;
      } 
   }

  TFile * fFile = new TFile("OUTcovariance_2D_mu_"+fsrFlag+"FSR_"+accFlag+"_"+norm_type+".root","RECREATE");
  MTEST.Write("totalCov_TM");
  Corig->Write("totalCorr_TM");
  fFile->Close();

  //validation
  cout << "Validation " << fsrFlag << " " << accFlag << " " << norm_type << endl;  

  //check positive definiteness by checning the signs of all eigenvalues
  cout << "Test matrix determinant " << MTEST.Determinant() << endl;  


 for (int i = 0; i < 132; i++) {
   for (int j = 0; j < 132; j++) {
     if (fabs(MTEST(i,j)-MTEST(j,i)) > fabs(MTEST(i,j))/10000.) cout << "Warning " << i << " " << j << " " << MTEST(i,j) << " " <<  MTEST(j,i) << endl;
   }
  }

 for (int i = 0; i < 132; i++) {
   for (int j = 0; j < 132; j++) {
     if (i!=j) continue;
     double val = 0;
     if (fsrFlag == "pre") val =diagonals_preFSR[j];
     if (fsrFlag == "post") val = diagonals_postFSR[j];
     if (norm_type == "absolute") {
          if (fsrFlag == "pre") val = sqrt(sigma_Z*sigma_Z*val*val + rdiagonals_preFSR[fi]*rdiagonals_preFSR[fj]*sigmaZerrNoLumi);
          if (fsrFlag == "post")  val = sqrt(sigma_Z*sigma_Z*val*val + rdiagonals_postFSR[fi]*rdiagonals_postFSR[fj]*sigmaZerrNoLumi);
     } 
     if (norm_type == "normalized" || norm_type == "absolute") {
              if (i < 120 && j < 120) {val *= 0.1;}
              else if (((i >=120) && !(j >= 120)) || (!(i >= 120) && (j >= 120))) {
                 val *= sqrt(0.1*0.2);
              } else {
                 val *= 0.2;
              }
     }
     if (fabs(sqrt(MTEST(j,j)) - val) > fabs(sqrt(MTEST(j,j)))/10000.) cout << "Warning " << sqrt(MTEST(j,j)) << " " << val << " " << fabs(sqrt(MTEST(j,j)) - val) << endl;
    }
  } 
}
