#include "ProdHist_DressedXSec_PDFUnc.cxx"
void ProdHist_DressedXSec_PDFUnc_EE()
{
	HistProducer *producer = new HistProducer( "Electron" );
	producer->Produce();
}