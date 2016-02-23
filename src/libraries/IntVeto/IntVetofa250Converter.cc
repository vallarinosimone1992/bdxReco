#include "IntVetofa250Converter.h"

#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode7Hit.h>

IntVetoSiPMHit* IntVetofa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const{
	IntVetoSiPMHit *m_IntVetoSiPMHit=new IntVetoSiPMHit;
	m_IntVetoSiPMHit->m_channel=m_channel;

	if (strcmp(hit->className(),"fa250Mode1CalibHit")==0){
		this->convertMode1Hit(m_IntVetoSiPMHit,(const fa250Mode1CalibHit*)hit);
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

jerror_t IntVetofa250Converter::convertMode1Hit(IntVetoSiPMHit* output,const fa250Mode1CalibHit *input) const{
	int size=input->samples.size();

	fa250Mode1CalibPedSubHit *m_waveform=new fa250Mode1CalibPedSubHit;
	output->AddAssociatedObject(m_waveform);
	/*A trick to copy crate-slot-channel*/
	fa250Hit *a = m_waveform;
	const fa250Hit *b = input;
	*a = *b;

	output->ped=0;
	for (int ii=0;ii<20;ii++){
		output->ped+=input->samples.at(ii);
	}
	output->ped/=20.;

	output->average=0;
	//1: set the pedestal-corrected samples, by creating a faMode1CalibPedSubHit object.
	for (int ii=0;ii<size;ii++){
		m_waveform->samples.push_back(input->samples.at(ii)-output->ped);
		output->average+=input->samples.at(ii);
	}
	output->average/=input->samples.size();




	return NOERROR;
}

jerror_t IntVetofa250Converter::convertMode7Hit(IntVetoSiPMHit* output,const fa250Mode7Hit *input) const{
	output->AddAssociatedObject(input);


	return NOERROR;
}
