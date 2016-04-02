#include "ExtVetofa250Converter.h"

#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>



ExtVetoPMTHit* ExtVetofa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel)const{
//	 jout << "sono qui"<<std::endl;
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
 //   static  double Vpedestal_backup[12]={93.26,91,92,95.6,93,92,92,76.5,98,79,90,108};
	static  vector<double> Vpedestal_backup(100,0);
	double Thr;		//mV

    	Thr = threshold->getCalibSingle(output->m_channel.ext_veto);
    double pedestal_DAQ;


    pedestal_DAQ = m_pedestals->getCalibSingle(input->m_channel);

    //jout << "prima " <<output->m_channel.ext_veto.component << " " << pedestal_DAQ <<" " <<Vpedestal_backup.at(output->m_channel.ext_veto.component) <<std::endl;

    if (Vpedestal_backup.at(output->m_channel.ext_veto.component) == 0){

    	Vpedestal_backup.at(output->m_channel.ext_veto.component) = pedestal_DAQ*0.4884;
    }
  //  jout << "dopo " <<output->m_channel.ext_veto.component << " " << pedestal_DAQ <<" " <<Vpedestal_backup.at(output->m_channel.ext_veto.component) <<std::endl;


    // ************************** Timing calculation ***************************************

    for (int ii=0;ii<size;ii++){
    	if((input->samples.at(ii)>=Thr)){
    		 if(ii==0){
    			 T=0;
    			 break;
    		           }
			    Tinf = ii-1;
           Tsup = ii;
		        Ainf = input->samples.at(ii-1);
		        Asup = input->samples.at(ii);
//      jout << Tinf << " "<< Tsup << " " << Ainf << " " <<Asup <<std::endl;

           break;
      } //endif

		if((ii==size-1)){

    			  T=-10;


    	}
                      }//endfor

               if(Tinf>0)   T=(((Tsup-Tinf)/(Asup-Ainf))*(Thr-Ainf))+Tinf;


   //    jout << "component= " << output->m_channel.ext_veto.component << std::endl;
    //    jout << "T= " << T << std::endl;

        // ********************************** Pedestal & Charge Calculation ******************************
        if (T>=0){
        if(T>35){

          nSamples = T-5;

             int j=0;
            for(int ii=0; ii<nSamples;ii++){

           Q_pedestal+=input->samples.at(ii);
           j +=1;

                       } //endfor

                 pedestal = Q_pedestal/j;
                 Vpedestal_backup[output->m_channel.ext_veto.component] = pedestal;

 //                      jout << "papta " <<output->m_channel.ext_veto.component << " " << pedestal_DAQ <<" " <<Vpedestal_backup.at(output->m_channel.ext_veto.component) <<std::endl;


             for (int ii=0;ii<size;ii++){
                        Q+=(input->samples.at(ii)-pedestal);
                                        } //endfor

           } else                // ******************************* Pedestal & Charge Calculation for T<35 ************************************
                 {

         	  pedestal = Vpedestal_backup[output->m_channel.ext_veto.component];


         	  for (int ii=0;ii<size;ii++){

                  Q+=(input->samples.at(ii)-pedestal);
         	  } //endfor

                                           } //endif-else
        }

              //********************** correction Q<0

          int T_index_max =0;
          int T_index_min=0;

             if(T>=0&&Q<0){

            	 Q=0;
      //  jout << "component= "<<output->m_channel.ext_veto.component<<" "<< "Q= "<<Q<<endl;

             T_index_min = T - 2;
             if(T_index_min<0) T_index_min=0;
             T_index_max = T + 4;
             if(T_index_max>=(size-1)) T_index_max = size-1;

             for(int ii=T_index_min; ii<T_index_max;ii++){

            	 Q+=(input->samples.at(ii)-pedestal_DAQ*0.4884);

             }



         //    jout <<"new Q =  "<< Q <<endl;
             } //end if Q<0









	output->Q = (Q*4*2*1E-3)/50;         /// charge in nWb , 50 ohm, *2 ->splitter
	output->T=T*4;                     //// Time in nS
    output->pedestal=pedestal;












	return NOERROR;
}

jerror_t ExtVetofa250Converter::convertMode7Hit(ExtVetoPMTHit* output,const fa250Mode7Hit *input)const{

	output->Q=0;
	output->T=0;
	return NOERROR;
}
