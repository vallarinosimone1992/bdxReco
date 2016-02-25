#include "IntVetofa250Converter.h"

#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>


IntVetoSiPMHit* IntVetofa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const{
	IntVetoSiPMHit *m_IntVetoSiPMHit=new IntVetoSiPMHit;
	m_IntVetoSiPMHit->m_channel=m_channel;

	if (strcmp(hit->className(),"fa250Mode1CalibPedSubHit")==0){
		this->convertMode1Hit(m_IntVetoSiPMHit,(const fa250Mode1CalibPedSubHit*)hit);
	}
	else if (strcmp(hit->className(),"fa250Mode7Hit")==0){
		this->convertMode7Hit(m_IntVetoSiPMHit,(const fa250Mode7Hit*)hit);
	}
	else{
		jerr<<"IntVetofa250Converter::convertHit unsupported class name: "<<hit->className()<<std::endl;
		return 0;
	}
	return m_IntVetoSiPMHit;
}

jerror_t IntVetofa250Converter::convertMode1Hit(IntVetoSiPMHit* output,const fa250Mode1CalibPedSubHit *input) const{
	int size=input->samples.size();

	int N,n,idx,imax;
	double min,max,xmin,xmax,prev_xmin,prev_xmax,rms,Tmax;

	std::pair<int,int> m_thisCrossingTime;

	std::vector<std::pair<int,int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;

	std::vector<int> m_singleCrossingIndexes;
	std::vector<int> m_signalCrossingIndexes;

	output->AddAssociatedObject(input);
	output->nSingles=0;
	output->average=0;

	//1: compute the average
	for (int ii=0;ii<size;ii++){
		output->average+=input->samples.at(ii);
	}
	output->average/=input->samples.size();


	///TODO NOT HARDCODED!
	int thr;
	if (output->m_channel.int_veto.component==0) thr=8;
	if (output->m_channel.int_veto.component==1) thr=8;
	if (output->m_channel.int_veto.component==2) thr=8;
	if (output->m_channel.int_veto.component==3) thr=8;
	if (output->m_channel.int_veto.component==4) thr=8;
	if (output->m_channel.int_veto.component==5) thr=8;

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
		else if((m_crossingTimesDelta.at(itime)>MIN_TOT)||(m_crossingTimes.at(itime).second)==(size)||(m_crossingTimes.at(itime).first)==(0)){
			m_singleCrossingIndexes.push_back(itime);
		}
	}

	output->nSingles=m_singleCrossingIndexes.size();
	if ((output->nSingles)==0){
		output->m_type=IntVetoSiPMHit::noise;
		output->T=0;
		output->Qraw=this->sumSamples(input->samples.size(),&(input->samples.at(0)));
		output->A=0;
		return NOERROR;
	}
	else {

		output->m_type=IntVetoSiPMHit::real_signal;
		output->A=0;
		/*Loop over the thr crossings and select the higher pulse*/
		for (int iphe=0;iphe<output->nSingles;iphe++){
			idx=m_singleCrossingIndexes.at(iphe);
			xmin=m_crossingTimes.at(idx).first;
			xmax=m_crossingTimes.at(idx).second;
			max=this->getMaximum((int)xmin,(int)xmax,&(input->samples.at(0)),Tmax);
			if ((output->A) < max){
				output->A=max;
				imax=idx;
			}
		}


		xmin=m_crossingTimes.at(imax).first;
		xmax=m_crossingTimes.at(imax).second;
		max=this->getMaximum((int)xmin,(int)xmax,&(input->samples.at(0)),Tmax);
		xmin=Tmax-20;
		if (xmin<0) xmin=0;
		xmax=Tmax+30;
		if (xmax>=size) (xmax=size-1);



		xmin=m_crossingTimes.at(imax).first;  //this is the time of the sample OVER thr
		if (xmin==0) output->T=0;
		else{
			max=input->samples.at(xmin);
			min=input->samples.at(xmin-1);
		}
		//y=min+(t-xmin)*(max-min)/(xmax-xmin) , xmin <= t <= xmax
		//y=THR
		//(THR-min)*(xmax-xmin)/(max-min)+xmin=t

		output->T=(thr-min)*(xmax-xmin)/(max-min)+xmin;
		output->Qraw=this->sumSamples(xmin,xmax,&(input->samples.at(0)));

		return NOERROR;
	}
	//


	return NOERROR;
}

jerror_t IntVetofa250Converter::convertMode7Hit(IntVetoSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);


	return NOERROR;
}
