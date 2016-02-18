#include "Calorimeterfa250Converter.h"
#include "CalorimeterCalibration.h"

#include "TF1.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TApplication.h"


#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "HFitInterface.h"
#include "Fit/Fitter.h"
#include "TVirtualFitter.h"

#include "Math/WrappedMultiTF1.h"
#include "Math/WrappedParamFunction.h"
#include "Math/WrappedTF1.h"



#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>

#include <JANA/JParameterManager.h>

double fSinglePhe2Pole(double *x,double *par){
	double ret=0;
	double t=x[0]-par[0];
	if (t>0){
		ret=par[3]+(par[1]/(2*par[2]*par[2]*par[2]))*t*t*exp(-t/par[2]); // the overall integral is par[1]. Max val is (2*par[1]/par[2])*exp(-2)

	}
	else{
		ret=par[3];
	}
	return ret;
}

double fSinglePhe1Pole(double *x,double *par){
	double ret=0;
	double t=x[0]-par[0];
	if (t>0){
		ret=par[3]+(par[1]/(par[2]*par[2]))*t*exp(-t/par[2]); // the overall integral is par[1]. Max val is (par[1]/par[2])*exp(-1)
	}
	else{
		ret=par[3];
	}
	return ret;
}



CalorimeterSiPMHit* Calorimeterfa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const{
	CalorimeterSiPMHit *m_CalorimeterSiPMHit=new CalorimeterSiPMHit;
	m_CalorimeterSiPMHit->m_channel=m_channel;



	if (strcmp(hit->className(),"fa250Mode1CalibHit")==0){
		this->convertMode1Hit(m_CalorimeterSiPMHit,(const fa250Mode1CalibHit*)hit);
	}
	else if (strcmp(hit->className(),"fa250Mode7Hit")==0){
		this->convertMode7Hit(m_CalorimeterSiPMHit,(const fa250Mode7Hit*)hit);
	}
	else{
		jerr<<"Calorimeterfa250Converter::convertHit unsupported class name: "<<hit->className()<<std::endl;
		return 0;
	}
	return m_CalorimeterSiPMHit;
}

jerror_t Calorimeterfa250Converter::convertMode1Hit(CalorimeterSiPMHit* output,const fa250Mode1CalibHit *input) const{
	int size=input->samples.size();
	int N;

	double tau,max,I,xmin,xmax,r;
	TH1D *hTmp;

	output->Q=0;
	output->T=0;
	output->A=0;
	output->m_type=noise;
	/*This part is to read pedestal from dB*/
	vector<double> pedV;


	fa250Mode1CalibPedSubHit *m_waveform=new fa250Mode1CalibPedSubHit;
	output->AddAssociatedObject(m_waveform);
	/*A trick to copy crate-slot-channel*/
	fa250Hit *a = m_waveform;
	const fa250Hit *b = input;
	*a = *b;



	std::vector<std::pair<int,int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;
	std::pair<int,int> m_thisCrossingTime;

	output->nSingles=0;
	output->nSignals=0;


	pedestal->getCalib(output->m_channel.calorimeter,pedV);
	if (pedV.size()==1){
		output->ped=pedV.at(0);
	}
	else{
		jerr<<"Calorimeterfa250Converter::convertMode1Hit error on pedestal db entries. Got: "<<pedV.size()<<std::endl;
		output->ped=0;  //A.C. for now ok, in the future need to revert-back to the non-db case!*/
	}

	//1: get the pedestal-corrected samples, by creating a faMode1CalibPedSubHit object.
	for (int ii=0;ii<size;ii++){
		m_waveform->samples.push_back(input->samples.at(ii)-output->ped);
	}


	///TODO!
	int thr=5;

	//2: find thr crossings
	m_thisCrossingTime.first=-1;
	m_thisCrossingTime.second=-1;
	if (m_waveform->samples[0]>thr) m_thisCrossingTime.first=0;
	for (int ii=1;ii<size;ii++){
		if ((	m_waveform->samples[ii]>thr)&&(	m_waveform->samples[ii-1]<thr)) m_thisCrossingTime.first=ii;
		else if ((	m_waveform->samples[ii]<thr)&&(	m_waveform->samples[ii-1]>thr) && (m_thisCrossingTime.first!=-1)) {
			m_thisCrossingTime.second=ii;
			m_crossingTimes.push_back(m_thisCrossingTime);
			m_thisCrossingTime.first=-1;
			m_thisCrossingTime.second=-1;
		}
	}
	//It may happen that the last sample is still over thr!
	if (m_waveform->samples[size-1]>thr){
		m_thisCrossingTime.second=size-1;
		m_crossingTimes.push_back(m_thisCrossingTime);
	}


	//3: deltas
	//3a: if no crossing times are found
	if (m_crossingTimes.size()==0){
		output->m_type=noise;
		output->T=0;
		output->Q=this->sumSamples(	m_waveform->samples);
	}

	/*Compute ToT */
	for (int itime=0;itime<m_crossingTimes.size();itime++){
		m_crossingTimesDelta.push_back(m_crossingTimes.at(itime).second-m_crossingTimes.at(itime).first);
	}

	/*Verify the ToT for each pulse*/
	for (int itime=0;itime<m_crossingTimes.size();itime++){
		if (m_crossingTimesDelta.at(itime)<0){
			jerr<<"Calorimeterfa20Converter::convertMode1Hit error, negative ToT?"<<std::endl;
		}
		else if (m_crossingTimesDelta.at(itime)>SINGLE_SIGNAL_TOT){
			output->nSignals++;
		}
		else if(m_crossingTimesDelta.at(itime)>MIN_TOT){
			output->nSingles++;
		}
	}

	/*If there is one pulse only: this is a "single phe" signal*/
	if ((output->nSignals==0)&&(output->nSingles==1)){
		output->m_type=one_phe;

		/*So fit it*/
		xmin=m_crossingTimes.at(0).first-20;
		xmax=m_crossingTimes.at(0).second+80;

		if (xmin<0) xmin=0;
		if (xmax>=size) xmax=(size-1);
		N=int((xmax-xmin))+1;



		hTmp=new TH1D(mName.c_str(),mName.c_str(),N,xmin-0.5,xmax+0.5);

		for (int ii=(int)xmin;ii<=(int)xmax;ii++) hTmp->Fill(ii,m_waveform->samples.at(ii));

		return NOERROR;
		tau=3;
		max=hTmp->GetMaximum();
		//		double I=max*(2*tau*exp(-2));
		I=max*(tau*exp(-1));



		output->m_fitFunction.fSinglePhe=new TF1("fSinglePhe",fSinglePhe1Pole,xmin,xmax,4);


		/*Prepare the fit in thread-safe way*/
		ROOT::Fit::BinData d;
		ROOT::Fit::FillData(d,hTmp,output->m_fitFunction.fSinglePhe);
		// create the function
		ROOT::Math::WrappedMultiTF1 wf(*output->m_fitFunction.fSinglePhe);
		// need to do that to avoid gradient calculation
		ROOT::Math::IParamMultiFunction & f = wf;
		ROOT::Fit::Fitter fitter;
		if (VERBOSE<5) fitter.Config().MinimizerOptions().SetPrintLevel(0);

		//output->m_fitFunction.fSinglePhe->SetParameters(m_crossingTimes.at(0).first,I,tau,0);

		/*Configure parameters*/
		double par[4]={m_crossingTimes.at(0).first,I,tau,0};
		f.SetParameters(par);



		/*Do the fit*/
		fitter.Fit(d, f);

		/*Set parameters to f*/
		output->m_fitFunction.fSinglePhe->SetParameters(fitter.Result().Parameter(0),fitter.Result().Parameter(1),fitter.Result().Parameter(2),fitter.Result().Parameter(3));

		output->Q=output->m_fitFunction.fSinglePhe->GetParameter(1);
		output->T=(output->m_fitFunction.fSinglePhe->GetParameter(0));

		delete hTmp;
	}
	else if ((output->nSingles>=3)||(output->nSignals>=1)){

		if ((output->nSingles==0)&&(output->nSignals==1)){
			output->m_type=good_real_signal;
			output->Q=this->sumSamples(	m_waveform->samples);


			/*Find the first sample not bigger than the previous (after thr)*/
			xmin=0;
			for (int ii=m_crossingTimes.at(0).first;ii<(size-1);ii++){
				if (m_waveform->samples.at(ii+1) < m_waveform->samples.at(ii)){
					xmax=ii;
					max=m_waveform->samples.at(ii);
					break;
				}
			}

			N=(xmax-xmin)+1;
			/*Determine time with a fit*/

			hTmp=new TH1D(mName.c_str(),mName.c_str(),N,xmin-0.5,xmax+0.5);
			for (int ii=(int)xmin;ii<=int(xmax);ii++) hTmp->Fill(ii,m_waveform->samples.at(ii));




			/*Prepare the fit in thread-safe way*/
			output->m_fitFunction.fRiseGoodRealSignal=new TF1("fRiseGoodRealSignal",fSinglePhe2Pole,xmin,xmax,4);
			ROOT::Fit::BinData d;
			ROOT::Fit::FillData(d,hTmp,output->m_fitFunction.fRiseGoodRealSignal);
			// create the function
			ROOT::Math::WrappedMultiTF1 wf(*output->m_fitFunction.fRiseGoodRealSignal);
			// need to do that to avoid gradient calculation
			ROOT::Math::IParamMultiFunction & f = wf;
			ROOT::Fit::Fitter fitter;
			if (VERBOSE<5) fitter.Config().MinimizerOptions().SetPrintLevel(0);

			//output->m_fitFunction.fRiseGoodRealSignal->SetParameters();

			/*Configure parameters*/
			I=max*(tau*tau*exp(-2));
			xmin=0;
			tau=6;
			double par[4]={m_crossingTimes.at(0).first,I,tau,0};
			f.SetParameters(par);


			/*Do the fit*/
			fitter.Fit(d, f);


			/*Set parameters to f*/
			output->m_fitFunction.fRiseGoodRealSignal->SetParameters(fitter.Result().Parameter(0),fitter.Result().Parameter(1),fitter.Result().Parameter(2),fitter.Result().Parameter(3));


			output->T=output->m_fitFunction.fRiseGoodRealSignal->GetParameter(0);

			delete hTmp;
		}
		else{
			output->m_type=real_signal;
			output->Q=this->sumSamples(	m_waveform->samples);
		}
	}
	else{
		output->m_type=single_phes;
		output->Q=this->sumSamples(	m_waveform->samples);
	}

	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);

	return NOERROR;
}




