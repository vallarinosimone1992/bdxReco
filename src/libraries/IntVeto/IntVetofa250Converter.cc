#include "IntVetofa250Converter.h"

#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TApplication.h"
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
	// Copy the fa250Hit part (crate, slot, channel, ...)
	// doing it this way allow one to modify fa250 later and
	// not have to change this code.

	int N,n,idx,imax;
	int istart,istartmin,icheck;
	double min,max,xmin,xmax,prev_xmin,prev_xmax,rms,Tmax;
	double ped,pedRMS,thr,pedRMSmin,pedmin;
	bool found;
	std::pair<int,int> m_thisCrossingTime;

	std::vector<std::pair<int,int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;

	std::vector<int> m_singleCrossingIndexes;
	//std::vector<int> m_signalCrossingIndexs;

	vector<double> m_waveform;
	m_waveform.clear();

	output->AddAssociatedObject(input);
	output->nSingles=0;
	output->average=0;

	//0: refine pedestal, should be ~0 already
	//0a: check if we can use this waveform as pedestal
	found=false;
	pedRMSmin=9999;
	/*Just do few checks!!!*/
	for (icheck=0;icheck<NRMSCHECKS;icheck++){
		//Compute pedestal and rms starting at this point
		istart=icheck*(size/NRMSCHECKS);
		if ((istart+m_NPED)>=size) istart=size-m_NPED-1;
		ped=0;
		pedRMS=0;
		for (int ii=0;ii<m_NPED;ii++){
			ped+=input->samples.at(ii+istart);
			pedRMS+=input->samples.at(ii+istart)*input->samples.at(ii+istart);

		}
		ped/=m_NPED;
		pedRMS/=m_NPED;
		pedRMS=sqrt(pedRMS-ped*ped);
		if (pedRMS <=  input->m_RMS ){//input->m_RMS is read from DB. This is the DAQ-measured RMS, equal for all hits in the same channel and the same run)
			found=true;
			break;
		}
	}
	if (found==false){/*It means we were not able to correct the pedestal here!*/
		ped=0;
	}


	//0b produced the waveform
	for (int ii=0;ii<size;ii++){
		m_waveform.push_back(input->samples[ii]-ped);
	}



	//1: compute the average
	for (int ii=0;ii<size;ii++){
		output->average+=m_waveform.at(ii);
	}
	output->average/=m_waveform.size();






	thr=m_thrCalib->getCalibSingle(*output->m_channel.int_veto); //this is the 1 phe ampl FROM DB

	thr=thr*m_thr;   //put a very low thr at this level


	//2: find thr crossings
	m_thisCrossingTime.first=-1;
	m_thisCrossingTime.second=-1;
	if (m_waveform[0]>thr) m_thisCrossingTime.first=0;
	for (int ii=0;ii<size;ii++){
		if ((	m_waveform[ii]>thr)&&(	m_waveform[ii-1]<thr)) m_thisCrossingTime.first=ii;
		else if ((	m_waveform[ii]<thr)&&(	m_waveform[ii-1]>thr) && (m_thisCrossingTime.first!=-1)) {
			m_thisCrossingTime.second=ii;
			m_crossingTimes.push_back(m_thisCrossingTime);
			m_thisCrossingTime.first=-1;
			m_thisCrossingTime.second=-1;
		}
	}
	//It may happen that the last sample is still over thr!
	if (m_waveform[size-1]>thr){
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
			jerr<<"IntVetofa20Converter::convertMode1Hit error, negative ToT?"<<std::endl;
		}
		else if((m_crossingTimesDelta.at(itime)>(m_minTot/4.))||(m_crossingTimes.at(itime).second)==(size)||(m_crossingTimes.at(itime).first)==(0)){
			m_singleCrossingIndexes.push_back(itime);
		}
	}
	output->nSingles=m_singleCrossingIndexes.size();
	if ((output->nSingles)==0){
		output->m_type=IntVetoSiPMHit::noise;
		output->Qraw=this->sumSamples(m_NSA+m_NSB,&(m_waveform.at(0)));  //for compatibility with case 1
		output->A=this->getMaximum(m_waveform.size(),&(input->samples.at(0)),output->T);
		output->T=-1;
		return NOERROR;
	}

	else {
		output->m_type=IntVetoSiPMHit::real_signal;
		output->A=-9999;
		/*Loop over the thr crossings and select the highest pulse*/
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
		max=this->getMaximum((int)xmin,(int)xmax,&(m_waveform.at(0)),Tmax);

		xmin=Tmax-m_NSA;
		if (xmin<0) xmin=0;
		xmax=Tmax+m_NSB;
		if (xmax>=size) (xmax=size-1);
		output->Qraw=this->sumSamples(xmin,xmax,&(m_waveform.at(0)));


		/*Now do timing*/
		//y=min+(t-xmin)*(max-min)/(xmax-xmin) , xmin <= t <= xmax
		//y=THR
		//(THR-min)*(xmax-xmin)/(max-min)+xmin=t
		xmax=m_crossingTimes.at(imax).first;  //this is the time of the sample OVER thr
		xmin=m_crossingTimes.at(imax).first-1;  //this is the time of the sample UNDER thr
		if (xmax==0) output->T=0;
		else{
			max=m_waveform.at(xmax);
			min=m_waveform.at(xmin);
		}

		output->T=(thr-min)*(xmax-xmin)/(max-min)+xmin;
		output->T*=4; //in NS!!!


		return NOERROR;
	}
	//


	return NOERROR;
}

jerror_t IntVetofa250Converter::convertMode7Hit(IntVetoSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);


	return NOERROR;
}
