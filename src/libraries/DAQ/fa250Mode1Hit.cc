#include "fa250Mode1Hit.h"

#include "TH1D.h"


void fa250Mode1Hit::toHisto(TH1D *h)const{
	if (h==0){
		jerr<<"fa250Mode1Hit::toHisto, h pointer is null. Do nothing"<<std::endl;
	}
	int N=this->samples.size();
	h->Reset();
	for (int ii=0;ii<N;ii++){
		h->Fill(ii,this->samples[ii]);
	}
	return;
}
