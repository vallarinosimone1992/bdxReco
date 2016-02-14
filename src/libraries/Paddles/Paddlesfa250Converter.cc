#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <Paddles/Paddlesfa250Converter.h>

#include <math.h>


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


//	jout<<m_channel.paddles.id<<std::endl;

	double Thr=0;
	if(m_channel.paddles.id==0)Thr=90; 		// mV for id=0
	if(m_channel.paddles.id==1)Thr=104; 	// mV for id=1

	int Nsamples=30;
	static double Ped_prev_id0=0;
	static double Ped_prev_id1=0;
	double Ped=0;
	double Q=0;
	double T=0;
	int T_index=-1;

	int size=input->samples.size();

//   *********** Timing *******************
	for (int ii=0;ii<size;ii++){
			if(input->samples.at(ii)>Thr) {
				double Tinf=(ii-1)*4;
				double Tsup=ii*4;
			    T=Tinf+((Tsup-Tinf)/2);
				T_index=ii;
//				jout<<"ID= "<<m_channel.paddles.id<<" Thr= "<<Thr<<" Amp= "<<input->samples.at(ii)<<" T= "<<T<<" T_index= "<<T_index<<" "<<std::endl;
			    break;
				}
	}

			if (T_index<0)	// no signal
						{
							output->Q=0;
							output->T=0;
							return NOERROR;
						}
//   **************************************

//   *********** Pedestall ****************
//	jout<<"Ped= "<<Ped<<" Ped_prev_id0="<<Ped_prev_id0<<" Ped_prev_id1="<<Ped_prev_id1<<std::endl;

	if(T_index<Nsamples&&m_channel.paddles.id==0){Ped=Ped_prev_id0;}
	else if(T_index<Nsamples&&m_channel.paddles.id==1){Ped=Ped_prev_id1;}
	else {
		for (int ii=0;ii<Nsamples;ii++)Ped+=input->samples.at(ii);
							Ped=Ped/30;
							if(m_channel.paddles.id==0)Ped_prev_id0=Ped;
							if(m_channel.paddles.id==1)Ped_prev_id1=Ped;
	}
//	jout<<"Ped= "<<Ped<<" Ped_prev_id0="<<Ped_prev_id0<<" Ped_prev_id1="<<Ped_prev_id1<<std::endl;
//   **************************************

//   *********** Charge *******************
		for (int ii=0;ii<size;ii++)Q+=(input->samples.at(ii)-Ped);
//		Q=(Q*4/50)*(10^(-12));		// from Wb to Coulomb
//   **************************************


	output->Q=Q;
	output->T=T;

	return NOERROR;
}

jerror_t Paddlesfa250Converter::convertMode7Hit(PaddlesPMTHit* output,const fa250Mode7Hit *input) const{

	output->Q=0;
	output->T=0;
	return NOERROR;
}
