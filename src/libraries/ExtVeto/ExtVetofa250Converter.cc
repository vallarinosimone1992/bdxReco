#include "ExtVetofa250Converter.h"

#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>


ExtVetoPMTHit* ExtVetofa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel)const{
	ExtVetoPMTHit *m_ExtVetoPMTHit=new ExtVetoPMTHit;
	m_ExtVetoPMTHit->m_channel=m_channel;

	if (strcmp(hit->className(),"fa250Mode1CalibHit")==0){
		this->convertMode1Hit(m_ExtVetoPMTHit,(const fa250Mode1CalibHit*)hit);
	}
	else if (strcmp(hit->className(),"fa250Mode7Hit")==0){
		this->convertMode7Hit(m_ExtVetoPMTHit,(const fa250Mode7Hit*)hit);
	}
	else{
		jerr<<"ExtVetofa250Converter::convertHit unsupported class name: "<<hit->className()<<std::endl;
		return 0;
	}
	return m_ExtVetoPMTHit;
}

jerror_t ExtVetofa250Converter::convertMode1Hit(ExtVetoPMTHit* output,const fa250Mode1CalibHit *input)const{

	int size=input->samples.size();
	double nSamples=0; // number of the samples from the beginning of the time windows used to calculate the pedestal
	double Q_pedestal = 0;
    double pedestal= 0;
	double Q =0;
	double Tinf=0, Ainf=0;
	double Tsup=0, Asup=0;
	double T=0;
    static  double Vpedestal_backup[12]={93.26,91,92,95.6,93,92,92,76.5,98,79,90,108};
    static double threshold_ch[12]={99,99,99,100,100,100,100,81,105,85,100,115};

    // ************************** Timing calculation ***************************************

          for (int ii=0;ii<size;ii++){

  	    		if((input->samples.at(ii)>=threshold_ch[output->m_channel.ext_veto.component]) &&ii>0){
                        Tinf = ii-1;
                        Tsup = ii;
  	    		        Ainf = input->samples.at(ii-1);
  	    		        Asup = input->samples.at(ii);
                        break;
      	         } //endif
  	    	                       }//endfor

          T=(((Tsup-Tinf)/(Asup-Ainf))*(threshold_ch[output->m_channel.ext_veto.component]-Ainf))+Tinf;		// linear extrapolation

          // ********************************** Pedestal & Charge Calculation ******************************

          if(T>25){

            nSamples = T-5;

              for(int ii=0; ii<nSamples;ii++){

           	    Q_pedestal+=input->samples.at(ii);

               } //endfor

             pedestal = Q_pedestal/nSamples;
             Vpedestal_backup[output->m_channel.ext_veto.component] = pedestal;




               for (int ii=0;ii<size;ii++){

                    Q+=(input->samples.at(ii)-pedestal);

                } //endfor

             } else                // ******************************* Pedestal & Charge Calculation for T<25 ************************************
                   {

           	  pedestal = Vpedestal_backup[output->m_channel.ext_veto.component];


           	  for (int ii=0;ii<size;ii++){

                    Q+=(input->samples.at(ii)-pedestal);
           	  } //endfor

                                             } //endif-else









	output->Q = (Q*4*1E-3)/50;         /// charge in nWb , 50 ohm
	output->T=T*4;                     //// Time in nS











	return NOERROR;
}

jerror_t ExtVetofa250Converter::convertMode7Hit(ExtVetoPMTHit* output,const fa250Mode7Hit *input)const{

	output->Q=0;
	output->T=0;
	return NOERROR;
}
