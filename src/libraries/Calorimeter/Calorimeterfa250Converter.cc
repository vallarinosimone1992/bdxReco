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




	//2: find m_THR crossings
	m_thisCrossingTime.first=-1;
	m_thisCrossingTime.second=-1;
	if (input->samples[0]>m_THR) m_thisCrossingTime.first=0;

	for (int ii=1;ii<size;ii++){
		if ((	input->samples[ii]>m_THR)&&	(input->samples[ii-1]<m_THR)) m_thisCrossingTime.first=ii;
		else if ((	input->samples[ii]<m_THR) && (input->samples[ii-1]>m_THR) && (m_thisCrossingTime.first!=-1)) {
			m_thisCrossingTime.second=ii;
			m_crossingTimes.push_back(m_thisCrossingTime);
			m_thisCrossingTime.first=-1;
			m_thisCrossingTime.second=-1;
		}
	}
	//It may happen that the last sample is still over m_THR!
	if (input->samples[size-1]>m_THR){
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
		else if (m_crossingTimesDelta.at(itime)>m_SINGLE_SIGNAL_TOT/4){
			m_signalCrossingIndexes.push_back(itime);
		}
		else if((m_crossingTimesDelta.at(itime)>m_MIN_TOT/4)||(m_crossingTimes.at(itime).second)==(size)||(m_crossingTimes.at(itime).first)==(0)){
			m_singleCrossingIndexes.push_back(itime);
		}
	}

	output->nSignals=m_signalCrossingIndexes.size();
	output->nSingles=m_singleCrossingIndexes.size();


	if (((output->nSignals)==0)&&(output->nSingles)==0){
		output->m_type=CalorimeterSiPMHit::noise;
		output->T=0;
		output->Qraw=this->sumSamples(0,m_NSB+m_NSA,&(input->samples[0])); //to be uniform with the case below
		output->A=0;
		return NOERROR;
	}
	else if ((output->nSignals==0)&&(output->nSingles==1)){
		output->A=this->getMaximum(m_crossingTimes[0].first,m_crossingTimes[0].second,&(input->samples[0]),Tmax);

		if ((Tmax<=m_NSB)||(Tmax>=(size-1-m_NSA))){
			output->m_type=CalorimeterSiPMHit::one_phe;
			output->Qraw=this->sumSamples(0,m_NSB+m_NSA,&(input->samples[0])); //to be uniform with the case below
			output->T=0;
			return NOERROR;
		}
		else{


			output->m_type=CalorimeterSiPMHit::good_one_phe;
			xmin=Tmax-m_NSB;
			xmax=Tmax+m_NSA;

			/*This should never happen*/
			if (xmin<0) xmin=0;
			if (xmax>=size) xmax=(size-1);

			N=int((xmax-xmin))+1;

			output->average/=input->samples.size();
			output->T=Tmax;
			output->Qraw=this->sumSamples((int)xmin,(int)xmax,&(input->samples[0]));
			/*now timing*/
			xmax=m_crossingTimes[0].first; //first sample above m_THR
			xmin=xmax-1; //sample befor m_THR
			max=input->samples[xmax];
			min=input->samples[xmin];

			output->T=(m_THR-min)*(xmax-xmin)/(max-min)+xmin;
			output->T*=4; //in NS!!!

		}
	}
	else if (output->nSignals>=1){
		output->m_type=CalorimeterSiPMHit::real_signal;
		output->Qraw=this->sumSamples(input->samples.size(),&(input->samples[0]));
		output->A=this->getMaximum(input->samples.size(),&(input->samples[0]),Tmax);

		idx=m_signalCrossingIndexes[0];
		xmax=m_crossingTimes[idx].first;  //this is the time of the sample OVER m_THR
		xmin=m_crossingTimes[idx].first-1;
		if (xmin==0) output->T=0;
		else{
			max=input->samples[xmax];
			min=input->samples[xmin];
		}

		output->T=(m_THR-min)*(xmax-xmin)/(max-min)+xmin;
		output->T*=4; //in NS!!!


	}
	else{
		output->m_type=CalorimeterSiPMHit::many_phe;
		output->A=0;
		output->Qraw=0;
		prev_xmin=0;
		for (int iphe=0;iphe<output->nSingles;iphe++){
			idx=m_singleCrossingIndexes[iphe];
			xmin=m_crossingTimes[idx].first;
			xmax=m_crossingTimes[idx].second;
			max=this->getMaximum((int)xmin,(int)xmax,&(input->samples[0]),Tmax);
			if ((output->A) < max) output->A=max;

			xmin=Tmax-m_NSB;
			xmax=Tmax+m_NSA;

			if (xmin<prev_xmin) xmin=prev_xmin;
			if (xmax>size) xmax=(size-1);

			output->Qraw+=this->sumSamples((int)xmin,(int)xmax,&(input->samples[0]));
		}
		idx=m_singleCrossingIndexes[0];
		xmax=m_crossingTimes.at(idx).first;  //this is the time of the sample OVER m_THR
		xmin=m_crossingTimes.at(idx).first-1;
		if (xmin==0) output->T=0;
		else{
			max=input->samples[xmax];
			min=input->samples[xmin];
		}
		output->T=(m_THR-min)*(xmax-xmin)/(max-min)+xmin;
		output->T*=4; //in NS!!!


	}

	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);

	return NOERROR;
}




