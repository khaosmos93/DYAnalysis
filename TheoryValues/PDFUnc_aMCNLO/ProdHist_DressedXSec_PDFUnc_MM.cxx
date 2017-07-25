#include "ProdHist_DressedXSec_PDFUnc.cxx"
void ProdHist_DressedXSec_PDFUnc_MM()
{
	HistProducer *producer = new HistProducer( "Muon" );
	producer->Produce();
}