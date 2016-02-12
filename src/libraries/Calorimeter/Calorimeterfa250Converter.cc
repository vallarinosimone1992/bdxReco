#include "Calorimeterfa250Converter.h"
#include "CalorimeterCalibration.h"
#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode7Hit.h>


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
	double Q=0;
	double ped=0;

	vector<double> pedV;
	pedestal->getCalib(output->m_channel.calorimeter,pedV);
	if (pedV.size()!=1){
		jerr<<"Calorimeterfa250Converter::convertMode1Hit error on pedestal db entries. Got: "<<pedV.size()<<std::endl;
		ped=0;
	}
	else{
		ped=pedV.at(0);
	}

	for (int ii=0;ii<size;ii++){
		Q+=(input->samples.at(ii)-ped);
	}
	output->miniped=0;
	for (int ii=0;ii<10;ii++){
			output->miniped+=(input->samples.at(ii));
		}
	output->miniped/=10;


	output->ped=Q/size;
	output->Q=Q;
	output->T=0;

	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{


	return NOERROR;
}
