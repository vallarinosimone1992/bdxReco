#include "CalorimeterHit.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <vector>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <TT/TranslationTable.h>
TCanvas* CalorimeterHit::Draw(int id)const{
	int Nwaves;
	int Nsamples;
	if (m_canvas==0){
		if (id==0){
			m_canvas=new TCanvas();
		}
		else{
			m_canvas=new TCanvas(Form("c%i_%i_%i",m_channel.sector,m_channel.x,m_channel.y),200,200,id);
		}
	}
	m_canvas->Clear();
	m_canvas->cd();
	m_canvas->Divide(4,4);

	vector <const fa250Mode1CalibPedSubHit*> waves;
	this->Get(waves);

	Nwaves=waves.size();
	/*Clear previous histograms and create new ones*/
	for (int ih=0;ih<hWaves.size();ih++){
		if (hWaves[ih]!=0) delete hWaves[ih];
	}
	hWaves.clear();
	for (int ih=0;ih<Nwaves;ih++){
		Nsamples=waves[ih]->samples.size();
		hWaves.push_back(new TH1D(Form("h%i_%i_%i_%i",m_channel.sector,m_channel.x,m_channel.y,ih),Form("h%i_%i_%i_%i",m_channel.sector,m_channel.x,m_channel.y,ih),Nsamples,-0.5,Nsamples-0.5));
		waves[ih]->toHisto(hWaves[ih]);
		m_canvas->cd(ih+1);
		hWaves[ih]->Draw();
	}

	return m_canvas;
}
