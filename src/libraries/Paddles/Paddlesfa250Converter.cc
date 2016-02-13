#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <Paddles/Paddlesfa250Converter.h>


PaddlesPMTHit* Paddlesfa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const{
	PaddlesPMTHit *m_PaddlesPMTHit=new PaddlesPMTHit;
	m_PaddlesPMTHit->m_channel=m_channel;

	if (strcmp(hit->className(),"fa250Mode1CalibHit")==0){
		this->convertMode1Hit(m_PaddlesPMTHit,(const fa250Mode1CalibHit*)hit,m_channel);
	}
	else if (strcmp(hit->className(),"fa250Mode7Hit")==0){
		this->convertMode7Hit(m_PaddlesPMTHit,(const fa250Mode7Hit*)hit);
	}
	else{
		jerr<<"Paddlesfa250Converter::convertHit unsupported class name: "<<hit->className()<<std::endl;
		return 0;
	}
	return m_PaddlesPMTHit;
}

jerror_t Paddlesfa250Converter::convertMode1Hit(PaddlesPMTHit* output,const fa250Mode1CalibHit *input, const TranslationTable::ChannelInfo &m_channel) const{

	double Thr=90; 	// mV for id=0
//	double Ped=0;
//	double Ped_prev=0;
	double Q=0;
	double T=0;

	int size=input->samples.size();

//   *********** Timing ****************
	for (int ii=0;ii<size;ii++){
			if(input->samples.at(ii)>Thr) {
				double Tinf=(ii-1)*4;
				double Tsup=ii*4;
			    T=Tinf+((Tsup-Tinf)/2);
				break;
				}
	}
//   ***********************************



	for (int ii=0;ii<size;ii++){
		Q+=input->samples.at(ii);
	}
	output->Q=Q;
	output->T=0;

	return NOERROR;
}

jerror_t Paddlesfa250Converter::convertMode7Hit(PaddlesPMTHit* output,const fa250Mode7Hit *input) const{

	output->Q=0;
	output->T=0;
	return NOERROR;
}
