#include <Include/PlotTools.h>

class CombineTool
{
public:
	Double_t RelUnc_sgnChange;
	Double_t RelUnc_bkgChange;
	
	Double_t RelUnc_M60to130;
	Double_t RelUnc_M70to120;
	Double_t RelUnc_MassRange;

	Double_t RelUnc_nBin30;
	Double_t RelUnc_nBin50;
	Double_t RelUnc_nBin;

	Double_t RelUnc_TagPt20;
	Double_t RelUnc_TagPt24;
	Double_t RelUnc_TagPt;

	Double_t RelUnc_Tot;

	CombineTool()
	{
		this->Get_Values();
	}

	void Print_All()
	{
		printf("Signal change: %9.5lf\n", this->RelUnc_sgnChange);
		printf("Bkg. change: %9.5lf\n", this->RelUnc_bkgChange);
		printf("Mass range: %9.5lf (60 < M < 130: %9.5lf, 70 < M < 120: %9.5lf)\n", 
			this->RelUnc_MassRange, this->RelUnc_M60to130, this->RelUnc_M70to120);
		printf("# bins: %9.5lf (# bins = 30: %9.5lf, # bins = 50: %9.5lf)\n", 
			this->RelUnc_nBin, this->RelUnc_nBin30, this->RelUnc_nBin50);
		printf("Tag pT: %9.5lf (Tag pT > 20: %9.5lf, Tag pT > 24: %9.5lf)\n", 
			this->RelUnc_TagPt, this->RelUnc_TagPt20, this->RelUnc_TagPt24);
		printf("Total (quad.sum): %9.5lf\n", this->RelUnc_Tot);
	}

	void Save( TFile *f_output )
	{
		SaveAsHist_OneContent( this->RelUnc_sgnChange, "h_RelUnc_sgnChange", f_output );
		SaveAsHist_OneContent( this->RelUnc_bkgChange, "h_RelUnc_bkgChange", f_output );
		
		SaveAsHist_OneContent( this->RelUnc_MassRange, "h_RelUnc_MassRange", f_output );
		SaveAsHist_OneContent( this->RelUnc_M60to130, "h_RelUnc_M60to130", f_output );
		SaveAsHist_OneContent( this->RelUnc_M70to120, "h_RelUnc_M70to120", f_output );

		SaveAsHist_OneContent( this->RelUnc_nBin, "h_RelUnc_nBin", f_output );
		SaveAsHist_OneContent( this->RelUnc_nBin30, "h_RelUnc_nBin30", f_output );
		SaveAsHist_OneContent( this->RelUnc_nBin50, "h_RelUnc_nBin50", f_output );

		SaveAsHist_OneContent( this->RelUnc_TagPt, "h_RelUnc_TagPt", f_output );
		SaveAsHist_OneContent( this->RelUnc_TagPt20, "h_RelUnc_TagPt20", f_output );
		SaveAsHist_OneContent( this->RelUnc_TagPt24, "h_RelUnc_TagPt24", f_output );

		SaveAsHist_OneContent( this->RelUnc_Tot, "h_RelUnc_Tot", f_output );
	}

protected:
	void Get_Values()
	{
		this->RelUnc_sgnChange = this->GetContent_FromHist( "sgnChange" );
		this->RelUnc_bkgChange = this->GetContent_FromHist( "bkgChange" );
		this->RelUnc_M60to130 = this->GetContent_FromHist( "M60to130" );
		this->RelUnc_M70to120 = this->GetContent_FromHist( "M70to120" );
		this->RelUnc_nBin30 = this->GetContent_FromHist( "nBin30" );
		this->RelUnc_nBin50 = this->GetContent_FromHist( "nBin50" );
		this->RelUnc_TagPt20 = this->GetContent_FromHist( "TagPt20" );
		this->RelUnc_TagPt24 = this->GetContent_FromHist( "TagPt24" );

		this->RelUnc_MassRange = this->LargerOne( this->RelUnc_M60to130, this->RelUnc_M70to120 );
		this->RelUnc_nBin = this->LargerOne( this->RelUnc_nBin30, this->RelUnc_nBin50 );
		this->RelUnc_TagPt = this->LargerOne( this->RelUnc_TagPt20, this->RelUnc_TagPt24 );

		this->RelUnc_Tot = sqrt(
			this->RelUnc_sgnChange*this->RelUnc_sgnChange +
			this->RelUnc_bkgChange*this->RelUnc_bkgChange +
			this->RelUnc_MassRange*this->RelUnc_MassRange +
			this->RelUnc_nBin*this->RelUnc_nBin +
			this->RelUnc_TagPt*this->RelUnc_TagPt);
	}

	Double_t GetContent_FromHist( TString SystType )
	{
		TString FileName = SystType+"/Local/ROOTFile_Output_SysUnc_Zpeak_TagProbe_SystSource.root";

		TH1D* h_RelUnc = Get_Hist(FileName, "h_RelUnc");

		return (Double_t)h_RelUnc->GetBinContent(1);
	}

	Double_t LargerOne( Double_t a, Double_t b)
	{
		if( a > b ) return a;
		else return b;
	}

};