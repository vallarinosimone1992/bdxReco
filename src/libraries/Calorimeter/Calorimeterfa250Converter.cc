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


#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>

#include <JANA/JParameterManager.h>
#include <DAQ/fa250Mode1CalibHit.h>

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



	if (strcmp(hit->className(),"fa250Mode1CalibPedSubHit")==0){
		this->convertMode1Hit(m_CalorimeterSiPMHit,(const fa250Mode1CalibPedSubHit*)hit);
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

jerror_t Calorimeterfa250Converter::convertMode1Hit(CalorimeterSiPMHit* output,const fa250Mode1CalibPedSubHit *input) const{
	int size=input->samples.size();
	int N,n,idx;
	double min,max,xmin,xmax,prev_xmin,prev_xmax,rms,Tmax;


	output->Qraw=0;
	output->T=0;
	output->A=0;
	output->m_type=CalorimeterSiPMHit::noise;
	/*This part is to read pedestal from dB*/
	double pedV;


	output->AddAssociatedObject(input);



	std::pair<int,int> m_thisCrossingTime;

	std::vector<std::pair<int,int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;

	std::vector<int> m_singleCrossingIndexes;
	std::vector<int> m_signalCrossingIndexes;


	output->nSingles=0;
	output->nSignals=0;

	output->average=0;
	//1: compute the average
	for (int ii=0;ii<size;ii++){
		output->average+=input->samples.at(ii);
	}
	output->average/=input->samples.size();


	///TODO NOT HARDCODED!
	int thr;
	if (output->m_channel.calorimeter.readout==1) thr=8;
	if (output->m_channel.calorimeter.readout==2) thr=8;

	//2: find thr crossings
	m_thisCrossingTime.first=-1;
	m_thisCrossingTime.second=-1;
	if (input->samples[0]>thr) m_thisCrossingTime.first=0;

	for (int ii=1;ii<size;ii++){
		if ((	input->samples[ii]>thr)&&(	input->samples[ii-1]<thr)) m_thisCrossingTime.first=ii;
		else if ((	input->samples[ii]<thr)&&(	input->samples[ii-1]>thr) && (m_thisCrossingTime.first!=-1)) {
			m_thisCrossingTime.second=ii;
			m_crossingTimes.push_back(m_thisCrossingTime);
			m_thisCrossingTime.first=-1;
			m_thisCrossingTime.second=-1;
		}
	}
	//It may happen that the last sample is still over thr!
	if (input->samples[size-1]>thr){
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


	if (((output->nSignals)==0)&&(output->nSingles)==0){
		output->m_type=CalorimeterSiPMHit::noise;
		output->T=0;
		output->Qraw=this->sumSamples(input->samples.size(),&(input->samples.at(0)));
		output->A=0;
		return NOERROR;
	}
	else if ((output->nSignals==0)&&(output->nSingles==1)){
		idx=m_singleCrossingIndexes.at(0);
		if ((m_crossingTimes.at(idx).first<=30)||(m_crossingTimes.at(idx).second>=(size-1-30))){
			output->m_type=CalorimeterSiPMHit::one_phe;
			output->Qraw=this->sumSamples(input->samples.size(),&(input->samples.at(0)));
			output->T=0;
			return NOERROR;
		}
		else{


			output->m_type=CalorimeterSiPMHit::good_one_phe;
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

			vector<double> new_samples;

			for (int ii=0;ii<size;ii++){
				new_samples.push_back(input->samples.at(ii)-output->ped);
				output->average+=new_samples.at(ii);
			}
			output->average/=input->samples.size();
			output->A=this->getMaximum(m_crossingTimes.at(idx).first,m_crossingTimes.at(idx).second,&(input->samples.at(0)),output->T);
			xmin=output->T-10;
			xmax=output->T+20;

			if ((xmin<=0)||(xmax>size)){
				jerr<<xmin<<" "<<xmax<<" "<<output->T<<" "<<m_crossingTimes.at(idx).first<<" "<<m_crossingTimes.at(idx).second<<std::endl;
			}
			output->Qraw=this->sumSamples((int)xmin,(int)xmax,&(input->samples.at(0)));


			output->T*=4; //in NS!!!

		}
	}
	else if (output->nSignals>=1){
		output->m_type=CalorimeterSiPMHit::real_signal;
		output->Qraw=this->sumSamples(input->samples.size(),&(input->samples.at(0)));
		output->A=this->getMaximum(input->samples.size(),&(input->samples.at(0)),output->T);

		idx=m_signalCrossingIndexes.at(0);
		xmin=m_crossingTimes.at(idx).first;  //this is the time of the sample OVER thr
		xmax=m_crossingTimes.at(idx).first+1;
		if (xmin==0) output->T=0;
		else{
			max=input->samples.at(xmin);
			min=input->samples.at(xmin-1);
		}
		//y=min+(t-xmin)*(max-min)/(xmax-xmin) , xmin <= t <= xmax
		//y=THR
		//(THR-min)*(xmax-xmin)/(max-min)+xmin=t

		output->T=(thr-min)*(xmax-xmin)/(max-min)+xmin;

		output->T*=4; //in NS!!!


	}
	else{
		output->m_type=CalorimeterSiPMHit::many_phe;
		output->A=0;
		output->Qraw=0;
		prev_xmin=0;
		for (int iphe=0;iphe<output->nSingles;iphe++){
			idx=m_singleCrossingIndexes.at(iphe);
			xmin=m_crossingTimes.at(idx).first;
			xmax=m_crossingTimes.at(idx).second;
			max=this->getMaximum((int)xmin,(int)xmax,&(input->samples.at(0)),Tmax);
			if ((output->A) < max) output->A=max;

			xmin=Tmax-10;
			xmax=Tmax+20;

			if (xmin<prev_xmin) xmin=prev_xmin;
			if (xmax>size) xmax=(size-1);

			output->Qraw+=this->sumSamples((int)xmin,(int)xmax,&(input->samples.at(0)));
		}
		idx=m_singleCrossingIndexes.at(0);
		xmin=m_crossingTimes.at(idx).first;  //this is the time of the sample OVER thr
		xmax=m_crossingTimes.at(idx).first+1;
		if (xmin==0) output->T=0;
		else{
			max=input->samples.at(xmin);
			min=input->samples.at(xmin-1);
		}
		//y=min+(t-xmin)*(max-min)/(xmax-xmin) , xmin <= t <= xmax
		//y=THR
		//(THR-min)*(xmax-xmin)/(max-min)+xmin=t

		output->T=(thr-min)*(xmax-xmin)/(max-min)+xmin;

		output->T*=4; //in NS!!!


	}

	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);

	return NOERROR;
}




