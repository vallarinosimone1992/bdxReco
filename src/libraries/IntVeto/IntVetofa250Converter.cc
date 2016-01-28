#include "IntVetofa250Converter.h"

#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>


IntVetoSiPMHit* IntVetofa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const{
	IntVetoSiPMHit *m_IntVetoSiPMHit=new IntVetoSiPMHit;
	m_IntVetoSiPMHit->m_channel=m_channel;
	m_IntVetoSiPMHit->fa250Hit_id=hit->id;

	if (strcmp(hit->className(),"fa250Mode1Hit")==0){
		this->convertMode1Hit(m_IntVetoSiPMHit,(const fa250Mode1Hit*)hit);
	}
	else if (strcmp(hit->className(),"fa250Mode7Hit")==0){
		this->convertMode7Hit(m_IntVetoSiPMHit,(const fa250Mode7Hit*)hit);
	}
	else{
		jerr<<"ExtVetofa250Converter::convertHit unsupported class name: "<<hit->className()<<std::endl;
		return 0;
	}
	return m_IntVetoSiPMHit;
}

jerror_t IntVetofa250Converter::convertMode1Hit(IntVetoSiPMHit* output,const fa250Mode1Hit *input) const{
	int size=input->samples.size();
	double Q=0;
	double ped=0;
	for (int ii=0;ii<=10;ii++){
		ped+=input->samples.at(ii);
	}
	ped/=10;
	for (int ii=0;ii<size;ii++){
		Q+=(input->samples.at(ii)-ped);
	}
	output->Q=Q;
	output->T=0;

	return NOERROR;
}

jerror_t IntVetofa250Converter::convertMode7Hit(IntVetoSiPMHit* output,const fa250Mode7Hit *input) const{


	return NOERROR;
}
