#include "Calorimeterfa250Converter.h"


#include "TF1.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TApplication.h"




#include "Math/WrappedTF1.h"
#include "Math/WrappedMultiTF1.h"
#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "HFitInterface.h"
#include "Fit/Fitter.h"
#include "Math/Minimizer.h"


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
	int N,n,idx;
	double min,max,xmin,xmax,prev_xmin,prev_xmax,rms,Tmax;


	output->Qraw=0;
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


	std::pair<int,int> m_thisCrossingTime;

	std::vector<std::pair<int,int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;

	std::vector<int> m_singleCrossingIndexes;
	std::vector<int> m_signalCrossingIndexes;


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


	output->average=0;
	//1: set the pedestal-corrected samples, by creating a faMode1CalibPedSubHit object.
	for (int ii=0;ii<size;ii++){
		m_waveform->samples.push_back(input->samples.at(ii)-output->ped);
		output->average+=input->samples.at(ii);
	}
	output->average/=input->samples.size();


	///TODO!
	int thr;
	if (output->m_channel.calorimeter.readout==1) thr=8;
	if (output->m_channel.calorimeter.readout==2) thr=8;

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
		m_thisCrossingTime.second=size;
		m_crossingTimes.push_back(m_thisCrossingTime);
	}


	//3: deltas
	//3a: if no crossing times are found


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
			m_signalCrossingIndexes.push_back(itime);
		}
		else if((m_crossingTimesDelta.at(itime)>MIN_TOT)||(m_crossingTimes.at(itime).second)==(size)||(m_crossingTimes.at(itime).first)==(0)){
			m_singleCrossingIndexes.push_back(itime);
		}
	}

	output->nSignals=m_signalCrossingIndexes.size();
	output->nSingles=m_singleCrossingIndexes.size();

	/*If there is one pulse only: this is a "single phe" signal*/
	if (((output->nSignals)==0)&&(output->nSingles)==0){
		output->m_type=noise;
		output->T=0;
		output->Qraw=this->sumSamples(m_waveform->samples.size(),&(m_waveform->samples.at(0)));
		output->A=0;
		return NOERROR;
	}
	else if ((output->nSignals==0)&&(output->nSingles==1)){
		idx=m_singleCrossingIndexes.at(0);
		if ((m_crossingTimes.at(idx).first<=30)||(m_crossingTimes.at(idx).second>=(size-1-30))){
			output->m_type=one_phe;
			output->Qraw=this->sumSamples(m_waveform->samples.size(),&(m_waveform->samples.at(0)));
			output->T=0;
			return NOERROR;
		}
		else{


			output->m_type=good_one_phe;
			xmin=m_crossingTimes.at(idx).first-20;
			xmax=m_crossingTimes.at(idx).second+80;

			if (xmin<0) xmin=0;
			if (xmax>=size) xmax=(size-1);
			N=int((xmax-xmin))+1;

			/*Refine the pedestal*/
			output->ped=0;
			output->average=0;
			for (int ii=0;ii<20;ii++) output->ped+=input->samples.at(ii);
			output->ped/=20;

			m_waveform->samples.clear();
			for (int ii=0;ii<size;ii++){
				m_waveform->samples.push_back(input->samples.at(ii)-output->ped);
				output->average+=input->samples.at(ii);
			}
			output->average/=input->samples.size();
			output->A=this->getMaximum(m_crossingTimes.at(idx).first,m_crossingTimes.at(idx).second,&(m_waveform->samples.at(0)),output->T);
			xmin=output->T-10;
			xmax=output->T+20;

			if ((xmin<=0)||(xmax>size)){
				jerr<<xmin<<" "<<xmax<<" "<<output->T<<" "<<m_crossingTimes.at(idx).first<<" "<<m_crossingTimes.at(idx).second<<std::endl;
			}
			output->Qraw=this->sumSamples((int)xmin,(int)xmax,&(m_waveform->samples.at(0)));




		}
	}
	else if (output->nSignals>=1){
		output->m_type=real_signal;
		output->Qraw=this->sumSamples(m_waveform->samples.size(),&(m_waveform->samples.at(0)));
		output->A=this->getMaximum(m_waveform->samples.size(),&(m_waveform->samples.at(0)),output->T);

		idx=m_signalCrossingIndexes.at(0);
		xmin=m_crossingTimes.at(idx).first;  //this is the time of the sample OVER thr
		xmax=m_crossingTimes.at(idx).first+1;
		if (xmin==0) output->T=0;
		else{
			max=m_waveform->samples.at(xmin);
			min=m_waveform->samples.at(xmin-1);
		}
		//y=min+(t-xmin)*(max-min)/(xmax-xmin) , xmin <= t <= xmax
		//y=THR
		//(THR-min)*(xmax-xmin)/(max-min)+xmin=t

		output->T=(thr-min)*(xmax-xmin)/(max-min)+xmin;




	}
	else{
		output->m_type=many_phe;
		output->A=0;
		output->Qraw=0;
		prev_xmin=0;
		for (int iphe=0;iphe<output->nSingles;iphe++){
			idx=m_singleCrossingIndexes.at(iphe);
			xmin=m_crossingTimes.at(idx).first;
			xmax=m_crossingTimes.at(idx).second;
			max=this->getMaximum((int)xmin,(int)xmax,&(m_waveform->samples.at(0)),Tmax);
			if ((output->A) < max) output->A=max;

			xmin=Tmax-10;
			xmax=Tmax+20;

			if (xmin<prev_xmin) xmin=prev_xmin;
			if (xmax>size) xmax=(size-1);

			output->Qraw+=this->sumSamples((int)xmin,(int)xmax,&(m_waveform->samples.at(0)));
		}
		idx=m_singleCrossingIndexes.at(0);
		output->T=m_crossingTimes.at(idx).first;



	}

	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);

	return NOERROR;
}




