void histotools(void) {}

TH1D* howerPointer(TH1D* h, TString label) {
   TH1D* hb = (TH1D*)gDirectory->Get(label);
   delete h; 
   TH1D* h = (TH1D*)hb->Clone();
   return h; 
}   

TH1D* rebinner(TH1D* h_in, TString WHICHHIST, bool do_rebin) {

   const int nbin = 28;
   double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 120, 126, 133, 141, 150, 160, 171, 185,
            200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

   const int nbin2 = 1;
   double mass_xbin2[nbin2+1] = {0.,2.4};

   //cout << "Usage inside the macro:" << endl;
   //cout << "TH1D* hnum_mc_init = (TH1D*)gDirectory->Get(WHICHHIST);" << endl;
   //cout << "TH1D* hnum_mc = rebinner(hnum_mc_init,do_rebin);" << endl;

   //const int nbin = 40;
   //double mass_xbin[nbin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
   //         96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
   //         200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

  if (!do_rebin) {
     return h_in;
  } else {
     if (WHICHHIST == "invm") {
        h_in->Rebin(nbin,"rebinned",mass_xbin);
        //TH1D* rebinned_ptr = &rebinned;
        TH1D* h_out = (TH1D*)rebinned->Clone();
        h_out->SetName(h_in->GetName());
        delete rebinned;
        return h_out;
     } else {
        h_in->Rebin(nbin2,"rebinned",mass_xbin2);
        TH1D* h_out = (TH1D*)rebinned->Clone();
        h_out->SetName(h_in->GetName());
        delete rebinned;
        return h_out;
     }
  }
}

TH1F* get2DSlice(TH1F* hist2D, TString hist, bool inputIsEE, bool useSetter) {
    TH1F* retHist;
    if (hist == "invm") {
         return hist2D;
    } else if (hist != "absrap200" && hist != "invm") {
        int last_index = 0;
        if (hist == "absrap3045") last_index = 24;
        if (hist == "absrap4560") last_index = 48;
        if (hist == "absrap60120") last_index = 72;
        if (hist == "absrap120200") last_index = 96;
        //if (inputIsEE) last_index += 1; //23;
        retHist = new TH1F("retHist"+hist,"retHist"+hist,24,0,2.4);
        for (int ibin = 0; ibin < 24; ibin++) {
            if (!useSetter) {
                retHist->Fill((ibin+0.5)*0.1,hist2D->GetBinContent(ibin+1+last_index));
            } else {
                retHist->SetBinContent(ibin+1,hist2D->GetBinContent(ibin+1+last_index));
                retHist->SetBinError(ibin+1,hist2D->GetBinError(ibin+1+last_index));
            }
        }
        return retHist;
    } else {
        cout << "Debug: in last bin " << hist << endl;
        int last_index = 120;
        //if (inputIsEE) last_index += 1;//23;
        retHist = new TH1F("retHist"+hist,"retHist"+hist,12,0,2.4);
        for (int ibin = 0; ibin < 12; ibin++) {
            if (!useSetter) {
               retHist->Fill((ibin+0.5)*0.2,hist2D->GetBinContent(ibin+1+last_index));
            } else {
                retHist->SetBinContent(ibin+1,hist2D->GetBinContent(ibin+1+last_index));
                retHist->SetBinError(ibin+1,hist2D->GetBinError(ibin+1+last_index));
            }
        }
    }
        return retHist;
}

TH1D* get2DSlice(TH1D* hist2D, TString hist, bool inputIsEE, bool useSetter) {
    TH1D* retHist;
    if (hist == "invm") { 
         return hist2D;
    } else if (hist != "absrap200" && hist != "invm") {
        int last_index = 0;
        if (hist == "absrap3045") last_index = 24;
        if (hist == "absrap4560") last_index = 48;
        if (hist == "absrap60120") last_index = 72;
        if (hist == "absrap120200") last_index = 96;
        //if (inputIsEE) last_index += 1; //23;
        retHist = new TH1D("retHist"+hist,"retHist"+hist,24,0,2.4);
        for (int ibin = 0; ibin < 24; ibin++) {
            if (!useSetter) {
                retHist->Fill((ibin+0.5)*0.1,hist2D->GetBinContent(ibin+1+last_index));
            } else {
                retHist->SetBinContent(ibin+1,hist2D->GetBinContent(ibin+1+last_index));
                retHist->SetBinError(ibin+1,hist2D->GetBinError(ibin+1+last_index));
            }
        } 
        return retHist;
    } else {
        cout << "Debug: in last bin " << hist << endl;
        int last_index = 120;
        //if (inputIsEE) last_index += 1;//23;
        retHist = new TH1D("retHist"+hist,"retHist"+hist,12,0,2.4);
        for (int ibin = 0; ibin < 12; ibin++) {
            if (!useSetter) { 
               retHist->Fill((ibin+0.5)*0.2,hist2D->GetBinContent(ibin+1+last_index));
            } else {
                retHist->SetBinContent(ibin+1,hist2D->GetBinContent(ibin+1+last_index));
                retHist->SetBinError(ibin+1,hist2D->GetBinError(ibin+1+last_index));
            }
        }
    } 
        return retHist;
}

TH1F* get2DSliceBambu(TH1F* hist2D, TString hist, bool useSetter) {
    TH1F* retHist;
    if (hist == "invm") {
         return hist2D;
    } else if (hist != "absrap200") { 
        int last_index = 0;
        if (hist == "absrap2030") last_index = 24;
        if (hist == "absrap3045") last_index = 48;
        if (hist == "absrap4560") last_index = 72;
        if (hist == "absrap60120") last_index = 96;
        if (hist == "absrap120200") last_index = 120;
        //if (inputIsEE) last_index += 1; //23;
        retHist = new TH1F("retHist"+hist,"retHist"+hist,24,0,2.4);
        for (int ibin = 0; ibin < 24; ibin++) {
            if (!useSetter) {
                retHist->Fill((ibin+0.5)*0.1,hist2D->GetBinContent(ibin+1+last_index));
            } else {
                retHist->SetBinContent(ibin+1,hist2D->GetBinContent(ibin+1+last_index));
                retHist->SetBinError(ibin+1,hist2D->GetBinError(ibin+1+last_index));
            }
        } 
    } else {
        cout << "Debug: in last bin " << hist << endl;
        int last_index = 144;
        retHist = new TH1F("retHist"+hist,"retHist"+hist,12,0,2.4);
        for (int ibin = 0; ibin < 12; ibin++) {
            if (!useSetter) { 
               retHist->Fill((ibin+0.5)*0.2,hist2D->GetBinContent(ibin+1+last_index));
            } else {
                retHist->SetBinContent(ibin+1,hist2D->GetBinContent(ibin+1+last_index));
                retHist->SetBinError(ibin+1,hist2D->GetBinError(ibin+1+last_index));
            }   
        }   
    }   
        return retHist;
}


TH1D* sliceCombiner(TH1D* h_2030, TH1D* h_3045, TH1D* h_4560, TH1D* h_60120, TH1D* h_120200, TH1D* h_2001500, bool useSetter) {
    TH1D* h = new TH1D("rap","rap",132,0,132);
    for (int i = 0 ; i < h->GetXaxis()->GetNbins();i++) {
         if (i < 24) {
            if (!useSetter) h->Fill(i+0.5,h_2030->GetBinContent(i+1));
            else { 
               h->SetBinContent(i+1,h_2030->GetBinContent(i+1));
               h->SetBinError(i+1,h_2030->GetBinError(i+1));
            }
         }
         if (i >= 24 && i < 48) {
            if (!useSetter) h->Fill(i+0.5,h_3045->GetBinContent(i+1-24));
            else {   
               h->SetBinContent(i+1,h_3045->GetBinContent(i+1-24));
               h->SetBinError(i+1,h_3045->GetBinError(i+1-24));
            }
         }
         if (i >= 48 && i < 72) {
            if (!useSetter) h->Fill(i+0.5,h_4560->GetBinContent(i+1-48));
            else {   
               h->SetBinContent(i+1,h_4560->GetBinContent(i+1-48));
               h->SetBinError(i+1,h_4560->GetBinError(i+1-48));
            }
         }
         if (i >= 72 && i < 96) {
            if (!useSetter) h->Fill(i+0.5,h_60120->GetBinContent(i+1-72));
            else {   
               h->SetBinContent(i+1,h_60120->GetBinContent(i+1-72));
               h->SetBinError(i+1,h_60120->GetBinError(i+1-72));
            }
         }
         if (i >= 96 && i < 120) {
            if (!useSetter) h->Fill(i+0.5,h_120200->GetBinContent(i+1-96));
            else {   
               h->SetBinContent(i+1,h_120200->GetBinContent(i+1-96));
               h->SetBinError(i+1,h_120200->GetBinError(i+1-96));
            }
         }
         if (i >= 120 && i < 132) {
            if (!useSetter) h->Fill(i+0.5,h_2001500->GetBinContent(i+1-120));
            else {   
               h->SetBinContent(i+1,h_2001500->GetBinContent(i+1-120));
               h->SetBinError(i+1,h_2001500->GetBinError(i+1-120));
            }
         }
   }
   return h;
}


TH1F* sliceCombiner(TH1F* h_2030, TH1F* h_3045, TH1F* h_4560, TH1F* h_60120, TH1F* h_120200, TH1F* h_2001500, bool useSetter) {
    TH1F* h = new TH1F("rap","rap",132,0,132);
    for (int i = 0 ; i < h->GetXaxis()->GetNbins();i++) {
         if (i < 24) {
            if (!useSetter) h->Fill(i+0.5,h_2030->GetBinContent(i+1));
            else {
               h->SetBinContent(i+1,h_2030->GetBinContent(i+1));
               h->SetBinError(i+1,h_2030->GetBinError(i+1));
            }
         }
         if (i >= 24 && i < 48) {
            if (!useSetter) h->Fill(i+0.5,h_3045->GetBinContent(i+1-24));
            else {
               h->SetBinContent(i+1,h_3045->GetBinContent(i+1-24));
               h->SetBinError(i+1,h_3045->GetBinError(i+1-24));
            }
         }
         if (i >= 48 && i < 72) {
            if (!useSetter) h->Fill(i+0.5,h_4560->GetBinContent(i+1-48));
            else {
               h->SetBinContent(i+1,h_4560->GetBinContent(i+1-48));
               h->SetBinError(i+1,h_4560->GetBinError(i+1-48));
            }
         }
         if (i >= 72 && i < 96) {
            if (!useSetter) h->Fill(i+0.5,h_60120->GetBinContent(i+1-72));
            else {
               h->SetBinContent(i+1,h_60120->GetBinContent(i+1-72));
               h->SetBinError(i+1,h_60120->GetBinError(i+1-72));
            }
         }
         if (i >= 96 && i < 120) {
            if (!useSetter) h->Fill(i+0.5,h_120200->GetBinContent(i+1-96));
            else {
               h->SetBinContent(i+1,h_120200->GetBinContent(i+1-96));
               h->SetBinError(i+1,h_120200->GetBinError(i+1-96));
            }
         }
         if (i >= 120 && i < 132) {
            if (!useSetter) h->Fill(i+0.5,h_2001500->GetBinContent(i+1-120));
            else {
               h->SetBinContent(i+1,h_2001500->GetBinContent(i+1-120));
               h->SetBinError(i+1,h_2001500->GetBinError(i+1-120));
            }
         }
   }
   return h;
}

TH1F* sliceCombinerBambu(TH1F* h_020, TH1F* h_2030, TH1F* h_3045, TH1F* h_4560, TH1F* h_60120, TH1F* h_120200, TH1F* h_2001500, bool useSetter) {
    TH1F* h = new TH1F("rap","rap",156,0,156);
    for (int i = 0 ; i < h->GetXaxis()->GetNbins();i++) {
         if (i < 24) {
            if (!useSetter) h->Fill(i+0.5,h_020->GetBinContent(i+1));
            else {
               h->SetBinContent(i+1,h_020->GetBinContent(i+1));
               h->SetBinError(i+1,h_020->GetBinError(i+1));
            }
         }
         if (i >= 24 && i < 48) {
            if (!useSetter) h->Fill(i+0.5,h_2030->GetBinContent(i+1-24));
            else {
               h->SetBinContent(i+1,h_2030->GetBinContent(i+1-24));
               h->SetBinError(i+1,h_2030->GetBinError(i+1-24));
            }
         }
         if (i >= 48 && i < 72) {
            if (!useSetter) h->Fill(i+0.5,h_3045->GetBinContent(i+1-48));
            else {
               h->SetBinContent(i+1,h_3045->GetBinContent(i+1-48));
               h->SetBinError(i+1,h_3045->GetBinError(i+1-48));
            }
         }
         if (i >= 72 && i < 96) {
            if (!useSetter) h->Fill(i+0.5,h_4560->GetBinContent(i+1-72));
            else {
               h->SetBinContent(i+1,h_4560->GetBinContent(i+1-72));
               h->SetBinError(i+1,h_4560->GetBinError(i+1-72));
            }
         }
         if (i >= 96 && i < 120) {
            if (!useSetter) h->Fill(i+0.5,h_60120->GetBinContent(i+1-96));
            else {
               h->SetBinContent(i+1,h_60120->GetBinContent(i+1-96));
               h->SetBinError(i+1,h_60120->GetBinError(i+1-96));
            }
         }
         if (i >= 120 && i < 144) {
            if (!useSetter) h->Fill(i+0.5,h_120200->GetBinContent(i+1-120));
            else {
               h->SetBinContent(i+1,h_120200->GetBinContent(i+1-120));
               h->SetBinError(i+1,h_120200->GetBinError(i+1-120));
            }
         } 
         if (i >= 144 && i < 156) {
            if (!useSetter) h->Fill(i+0.5,h_2001500->GetBinContent(i+1-144));
            else {
               h->SetBinContent(i+1,h_2001500->GetBinContent(i+1-144));
               h->SetBinError(i+1,h_2001500->GetBinError(i+1-144));
            }
         } 
   }

   return h;
}

TH1D* histoMultiply3(TH1D* h1, TH1D* h2, TH1D* h3, bool realerror) {

  TH1D* htp = (TH1D*)h1->Clone();
  for (int i = 0; i < htp->GetXaxis()->GetNbins(); i++) {
      htp->SetBinContent(i+1,h1->GetBinContent(i+1)*h2->GetBinContent(i+1)*h3->GetBinContent(i+1));
      if (realerror) {
         htp->SetBinError(i+1,htp->GetBinContent(i+1)*sqrt(pow(h1->GetBinError(i+1)/h1->GetBinContent(i+1),2)+pow(h2->GetBinError(i+1)/h2->GetBinContent(i+1),2)+pow(h3->GetBinError(i+1)/h3->GetBinContent(i+1),2)));
      } else {
         htp->SetBinError(i+1,0.0);
      } 
  }

  return htp;

}

TH1D* histoMultiply3_custom(TH1D* h1, TH1D* h2, TH1D* h3, bool realerror, double factor) {

  TH1D* htp = (TH1D*)h1->Clone();
  for (int i = 0; i < htp->GetXaxis()->GetNbins(); i++) {
      htp->SetBinContent(i+1,h1->GetBinContent(i+1)*h2->GetBinContent(i+1)*h3->GetBinContent(i+1));
      if (realerror) {
         htp->SetBinError(i+1,factor*htp->GetBinContent(i+1)*sqrt(pow(h1->GetBinError(i+1)/h1->GetBinContent(i+1),2)+pow(h2->GetBinError(i+1)/h2->GetBinContent(i+1),2)+pow(h3->GetBinError(i+1)/h3->GetBinContent(i+1),2)));
      } else {
         htp->SetBinError(i+1,0.0);
      }
  }

  return htp;

}

void removeErrors(TH1D* h1, double suppressionFactor) {
  
   for (int i = 0; i < h1->GetXaxis()->GetNbins(); i++) {
      h1->SetBinError(i+1,h1->GetBinError(i+1)/suppressionFactor);
   }
}


TH1D* histoMultiply2(TH1D* h1, TH1D* h2, bool realerror) {

  TH1D* htp = (TH1D*)h1->Clone();
  for (int i = 0; i < htp->GetXaxis()->GetNbins(); i++) {
      htp->SetBinContent(i+1,h1->GetBinContent(i+1)*h2->GetBinContent(i+1));
      if (realerror) {
         htp->SetBinError(i+1,htp->GetBinContent(i+1)*sqrt(pow(h1->GetBinError(i+1)/h1->GetBinContent(i+1),2)+pow(h2->GetBinError(i+1)/h2->GetBinContent(i+1),2)));
      } else {
         htp->SetBinError(i+1,0.0);
      }
  }

  return htp;
}

void scaleErrors(TH1D* initHist, double factor) {

  //TH1D* hout = (TH1D*)initHist->Clone();
  for (int i = 0; i < initHist->GetXaxis()->GetNbins(); i++) {
      initHist->SetBinError(i+1,initHist->GetBinError(i+1)*factor);
  }
  //return hout;
}

//reset a histogram
void histoReset(TH1D* initHist, TH1D* finHist, int fromBin, int toBin, bool errorsOnly) {

  //TH1D* hout = (TH1D*)initHist->Clone();
  for (int i = 0; i < initHist->GetXaxis()->GetNbins(); i++) {
      if (i+1 > fromBin && i+1 < toBin) { 
         if (!errorsOnly) initHist->SetBinContent(i+1,finHist->GetBinContent(i+1));
         initHist->SetBinError(i+1,finHist->GetBinError(i+1)/finHist->GetBinContent(i+1)*initHist->GetBinContent(i+1));
      }
  }
  //return hout;
}

void histoCustomReset(TH1D* valHist, TH1D* errHist) {

  cout << "MIND: sets the contents of the second histo as error to the original (first)!" << endl;
  //TH1D* hout = (TH1D*)initHist->Clone();
  if (valHist->GetXaxis()->GetNbins() != errHist->GetXaxis()->GetNbins()) {
    cout << "Incompatible dimensions!!" << endl;
    return;
  } else {
    for (int i = 0; i < valHist->GetXaxis()->GetNbins(); i++) {
        if (TMath::IsNaN(errHist->GetBinContent(i+1))) {
            valHist->SetBinError(i+1,(errHist->GetBinContent(i)+errHist->GetBinContent(i+2))/2.); 
        } else {
            valHist->SetBinError(i+1,errHist->GetBinContent(i+1));
        }
    }
  }
  //return hout;
}



void FindRatio(TH1D* ratio, TH1D* num, TH1D* denom) {

   cout << "WARNING: avoid using FindRatio for now: errors are not understood!" << endl;
   
   for (int ibin = 0; ibin < num->GetXaxis()->GetNbins()+1;ibin++) {
       if (denom->GetBinContent(ibin+1) != 0) {
                double rat = (double)(num->GetBinContent(ibin+1)/denom->GetBinContent(ibin+1));
                ratio->SetBinContent(ibin+1,rat);
                ratio->SetBinError(ibin+1,sqrt(num->GetBinContent(ibin+1))/denom->GetBinContent(ibin+1) - num->GetBinContent(ibin+1)*sqrt(denom->GetBinContent(ibin+1))/pow(denom->
GetBinContent(ibin+1),2));
        } else {
                ratio->SetBinContent(ibin+1,0.);
       }
  }
}

void normalizeToPeak(TH1D* hmc_init, TH1D* hdata) {
   double this_mc = hmc_init->Integral(73,96);
   double this_data = hdata->Integral(73,96);
   if (hmc_init->GetNbinsX() == 156) {
     this_mc = hmc_init->Integral(73+24,96+24);
     this_data = hdata->Integral(73+24,96+24);
   } 

   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc_init->Scale(this_data/this_mc);
      if (hmc_init->GetNbinsX() == 156) this_mc = hmc_init->Integral(73+24,96+24);
      else this_mc = hmc_init->Integral(73,96); 
   }      
}       

void normalizeToPeak(TH1F* hmc_init, TH1F* hdata) {
   double this_mc = hmc_init->Integral(73,96);
   double this_data = hdata->Integral(73,96);
   if (hmc_init->GetNbinsX() == 156) {
     this_mc = hmc_init->Integral(73+24,96+24);
     this_data = hdata->Integral(73+24,96+24);
   }
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      cout << "Iter " << this_mc/this_data << endl;
      hmc_init->Scale(this_data/this_mc);
      if (hmc_init->GetNbinsX() == 156) this_mc = hmc_init->Integral(73+24,96+24);
      else this_mc = hmc_init->Integral(73,96);
   }
}        

void normalizeToPeak2D(TH2F* hmc_init, TH1F* hdata) {
   double this_mc = hmc_init->Integral(73,96,73,96);
   double this_data = hdata->Integral(73,96);
   if (hmc_init->GetNbinsX() == 156) {
     this_mc = hmc_init->Integral(73+24,96+24,73+24,96+24);
     this_data = hdata->Integral(73+24,96+24);
   }
   while(fabs(this_mc/this_data - 1.) > 0.000001) {
      hmc_init->Scale(this_data/this_mc);
      if (hmc_init->GetNbinsX() == 156) this_mc = hmc_init->Integral(73+24,96+24);
      else this_mc = hmc_init->Integral(73,96); 
   }    
}  
