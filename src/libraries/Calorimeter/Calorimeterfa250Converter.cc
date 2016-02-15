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

	output->Q=0;
	output->T=0;
	output->A=0;
	/*This part is to read pedestal from dB*/
	vector<double> pedV;
	pedestal->getCalib(output->m_channel.calorimeter,pedV);
	if (pedV.size()==1){
		output->ped=pedV.at(0);
	}
	else{
		jerr<<"Calorimeterfa250Converter::convertMode1Hit error on pedestal db entries. Got: "<<pedV.size()<<std::endl;
		output->ped=0;  //A.C. for now ok, in the future need to revert-back to the non-db case!*/
	}

	/*Charge-amplitude computation*/
	output->A=-9999; //A.C will this always work?
	for (int ii=0;ii<size;ii++){
		output->Q+=(input->samples.at(ii)-output->ped);
		if ((input->samples.at(ii) - output->ped) > output->A){
			output->A = (input->samples.at(ii) - output->ped);
			output->T = ii*4.; //a.c. very dummy for now! also, 4 ns/sample is hard-coded.
		}
	}

	output->miniped=0;
	for (int ii=0;ii<NPED;ii++){
			output->miniped+=(input->samples.at(ii));
		}
	output->miniped/=NPED;



	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{


	return NOERROR;
}
