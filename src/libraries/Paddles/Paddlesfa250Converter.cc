#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>

#include <Paddles/Paddlesfa250Converter.h>
#include <system/CalibrationHandler.h>


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

	double Thr;
	Thr=threshold->getCalibSingle(output->m_channel.paddles);

//	vector<double> m_pedestal_init;
//	m_pedestal_init=pedestal_init->getCalib(output->m_channel.paddles);

	int Nsamples=30;
//	static double Ped_prev_id0=m_pedestal_init.at(0);
//	static double Ped_prev_id1=m_pedestal_init.at(1);

	static double Ped_prev_id0=m_rawpedestal.at(144).at(3);
	static double Ped_prev_id1=m_rawpedestal.at(145).at(3);

	double Ped=0;
	double Q=0;					// nC
	double T=0;					// nsec
	double max=0;

	double Tinf,Tsup,Amp1,Amp2;


	int T_index=-1;
	int max_index=-1;
	int inf_index=-1;
	int sup_index=-1;

	int size=input->samples.size();

//	jout<<"****************"<<std::endl;
//	jout<<"Channel= "<<m_channel.paddles.id<<" Thr= "<<Thr<<" "<<" Ped_prev_id0= "<<Ped_prev_id0<<" Ped_prev_id1= "<<Ped_prev_id1<<std::endl;


//   *********** Timing *******************
	for (int ii=0;ii<size;ii++){

		if(input->samples.at(ii)>Thr&&ii==0){
			T_index=ii;
			T=0;
			break;
		}
			if(input->samples.at(ii)>Thr&&ii>0) {
				Tinf=(ii-1)*4;
				Tsup=ii*4;
				Amp1=input->samples.at(ii-1);
				Amp2=input->samples.at(ii);
				T_index=ii;
//				jout<<"ID= "<<m_channel.paddles.id<<" Thr= "<<Thr<<" Amp= "<<input->samples.at(ii)<<" T= "<<T<<" T_index= "<<T_index<<" "<<std::endl;
//				jout<<"Amp1= "<<Amp1<<" Amp2= "<<Amp2<<std::endl;
			    break;
				}
	}
			if (T_index<0)	// no signal
						{
							output->Q=0;
							output->T=0;
							return NOERROR;
						}

			if(T_index>0)T=(((Tsup-Tinf)/(Amp2-Amp1))*(Thr-Amp1))+Tinf;		// linear interpolation
	//		jout<<"T_interp= "<<T<<std::endl;

	/***************************************/

    /*********** Pedestal ****************/
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

		if(T>0&&Q<0) {					// to recover most of the (few) "strange" signals (small and very fast (2-3 bins))
//			jout<<"Q_before= "<<Q<<std::endl;
			for (int ii=0;ii<size;ii++){
				if(input->samples.at(ii) > max) {
					max = input->samples.at(ii);
					max_index=ii;
					inf_index=ii-3;			// integrate from 12 nsec before ...
					sup_index=ii+3;			// .... to 12 nsec after the maximum
					if (inf_index<0)inf_index=0;
					if (sup_index>size-1)sup_index=size-1;
										}
									}
	//			jout<<"Max= "<<max<<" max_index= "<<max_index<<" inf_index= "<<inf_index<<" sup_index= "<<sup_index<<std::endl;
				Q=0;		// reset Q for new calculation
				for (int ii=inf_index;ii<sup_index+1;ii++)Q+=(input->samples.at(ii)-Ped);
 //    			jout<<"Q_after= "<<Q<<std::endl;
		}

		Q=2*Q*(0.001*4)/50 ;        // from Wb to nCoulomb , 4 [nsec], 50 [Ohm], Q [Volts], 2 splitter

	output->Q=Q;
	output->T=T;

	return NOERROR;
}

jerror_t Paddlesfa250Converter::convertMode7Hit(PaddlesPMTHit* output,const fa250Mode7Hit *input) const{

	output->Q=0;
	output->T=0;
	return NOERROR;
}
