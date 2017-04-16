import os

class MakeConfigTool:
	def __init__(self):
		self.Type = ""
		self.outputFile = file("Def_AllTemplatePDF.py", "w")
		self.outputFile.write("import FWCore.ParameterSet.Config as cms\n\n")

	def Write_AllPDFDef( self, _Type ):
		self.Type = _Type
		self.ROOTFileName = "ROOTFile_MCTemplates_%s.root" % (self.Type)

		self.outputFile.write( "all_pdfs_%s = cms.PSet(\n" % (self.Type) )

		nPtBin = 4
		nEtaBin = 5
		for i_pt in range(0, nPtBin+1):
			for i_eta in range(0, nEtaBin+1):
				self.Write_PDFDef( i_pt, i_eta )

		self.outputFile.write(")\n\n")


	def Write_PDFDef( self, i_pt, i_eta ):
		TemplatePath = "%s/Investigation/TnPSystematic_MCTemplate" % ( os.environ["KP_ANALYZER_PATH"] )
		BinName = "PtBin%d_EtaBin%d" % (i_pt, i_eta)
		self.outputFile.write( 'TmpPlusExpo_%s = cms.vstring(' % (BinName) )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"RooGaussian::signalResPass(mass, meanP[1.0, -5.0, 5.0], sigmaP[0.5, 0.001, 5.000])",' )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"RooGaussian::signalResFail(mass, meanF[1.0, -5.0, 5.0], sigmaF[0.5, 0.001, 5.000])",' )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"ZGeneratorLineShape::signalPhyPass(mass, \\"%s/%s\\", \\"h_Pass_%s\\")",' % (TemplatePath, self.ROOTFileName, BinName) )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"ZGeneratorLineShape::signalPhyFail(mass, \\"%s/%s\\", \\"h_Fail_%s\\")",' % (TemplatePath, self.ROOTFileName, BinName) )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",' )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",' )
		self.outputFile.write( '\n' )

		self.outputFile.write( '"FCONV::signalPass(mass, signalPhyPass, signalResPass)",' )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"FCONV::signalFail(mass, signalPhyFail, signalResFail)",' )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"efficiency[0.9,0,1]",' )
		self.outputFile.write( '\n' )
		self.outputFile.write( '"signalFractionInPassing[0.9]"' )
		self.outputFile.write( '\n' )		
		self.outputFile.write( '),\n\n' )
		
	def Close( self ):
		self.outputFile.close()
		

if __name__ == "__main__":
	tool = MakeConfigTool()
	tool.Write_AllPDFDef( "ID" )
	tool.Write_AllPDFDef( "Iso" )
	tool.Write_AllPDFDef( "HLTv4p2" )
	tool.Write_AllPDFDef( "HLTv4p3" )
	tool.Close()