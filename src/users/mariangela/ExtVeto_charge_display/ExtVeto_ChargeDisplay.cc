/*
 * ExtVeto_ChargeDisplay.cc
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */
#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVeto_ChargeDisplay.h"
#include "system/BDXEventProcessor.h"

#include <DAQ/fa250Mode1CalibHit.h>

#include <TT/TranslationTable.h>

#include <DAQ/eventData.h>

#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>

#include <system/JROOTOutput.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

using namespace jana;



extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new ExtVeto_ChargeDisplay());
}
} // "C"


//------------------
// ExtVeto_SigDisplay (Constructor)
//------------------
ExtVeto_ChargeDisplay::ExtVeto_ChargeDisplay():m_isFirstCallToBrun(1)
{

}

//------------------
// ~ExtVeto_SigDisplay (Destructor)
//------------------
ExtVeto_ChargeDisplay::~ExtVeto_ChargeDisplay()
{

}
//------------------
// init
//------------------
jerror_t ExtVeto_ChargeDisplay::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;
//	h=new TH1D("h","h",100,-0.5,99.5);
	t=new TTree("tout","tout");

	t->Branch("component",&component);
    t->Branch("Q",&Q);
//	t->Branch("pedestal",&pedestal);
	t->Branch("T",&T);
	t->Branch("eventN",&eventN);
	t->Branch("Tw",&Tw);
    t->Branch("A",A, "A[100]/D");
    t->Branch("B",B, "B[100]/D");
    t->Branch("time",time,"time[100]/I");
    t->Branch("T_B",&T_B);
    t->Branch("rise_TB",&rise_TB);
    t->Branch("rise_T",&rise_T);
    t->Branch("Threshold_B",&Threshold_B);

    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVeto_ChargeDisplay::brun(JEventLoop *loop, int32_t runnumber)
{


	// This is called whenever the run number changes
	/* To get access to the ROOT output, use exactly the following code */
	if (m_isFirstCallToBrun){
		string class_name,this_class_name;
		string joutput_name;
		BDXEventProcessor *m_BDXEventProcessor;
		vector<JEventProcessor*> m_processors=app->GetProcessors();
		vector<JEventProcessor*>::iterator m_processors_it;

		m_isFirstCallToBrun=0;
		class_name="BDXEventProcessor";
		joutput_name="JROOTOutput";
		//Now I need to determine which processor is the one holding the output. Discussing with David, he suggested just to check the class name, since
		//a dynamic cast may not work with plugins
		for (m_processors_it=m_processors.begin();m_processors_it!=m_processors.end();m_processors_it++){
			if ((*m_processors_it)!=0){
				this_class_name=string((*m_processors_it)->className());
				if (this_class_name==class_name){
					m_BDXEventProcessor=(BDXEventProcessor*)(*m_processors_it);
					if (m_BDXEventProcessor->getOutput()==0){
						jerr<<"BDXEventProcessor JOutput is null!"<<endl;
						break;
					}
					if (string(m_BDXEventProcessor->getOutput()->className())==joutput_name){
						m_ROOTOutput=(JROOTOutput*)(m_BDXEventProcessor->getOutput());
						jout<<"Got JROOTOutput!"<<endl;
					}
					else{
						jerr<<"BDXEventProcessor JOutput is not null BUT class is: "<<m_BDXEventProcessor->getOutput()->className()<<endl;
					}
				}
			}
		}
		/*For ALL objects you want to add to ROOT file, use the following:*/
		if (m_ROOTOutput){
//			m_ROOTOutput->AddObject(h);
			m_ROOTOutput->AddObject(t);
		}

	}

	return NOERROR;


}

//------------------
// evnt
//------------------
jerror_t ExtVeto_ChargeDisplay::evnt(JEventLoop *loop,uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	vector<const ExtVetoPMTHit*> data;


	vector<const ExtVetoPMTHit*>::const_iterator data_it;
	const fa250Mode1CalibHit *fa;
	loop->Get(data);

	const eventData* tData;
	//has to be in a try-catch block, since if no trigger data is there (prestart - start - end events) trows it!
	try{
		loop->GetSingle(tData);
	}
	catch(unsigned long e){
		jout<<"No trig bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}

	int tWord=tData->triggerWords.at(0);
//	jout<<eventnumber<<" tWord= "<<tWord<<endl;

	int isMPPC=0;
	for (int ii=0;ii<4;ii++){
		if ((((tWord)>>ii)&0x1)&&(ii==2)) isMPPC=1;
	}
//	if (!isMPPC) return OBJECT_NOT_AVAILABLE;

//	jout<<"****************************************************************"<<endl;
//	jout<<"Evt number="<< eventnumber<<" tWord= "<<tWord<<endl;
	japp->RootWriteLock();
	//  ... fill historgrams or trees ...

	for (data_it=data.begin();data_it<data.end();data_it++){



		const ExtVetoPMTHit *evhit = *data_it;
//		cout<<"ExtVeto_component= "<<evhit->m_channel.ext_veto.component<<endl;

//		if ((evhit->m_channel.ext_veto.component==0)){

			// Get associated fa250Mode1CalibHit object
			fa = NULL;
			evhit->GetSingle(fa);


			if(!fa) continue; // need fa250Mode1CalibHit to continue


			eventN=eventnumber;
			component=evhit->m_channel.ext_veto.component;
	   	    Q=(*data_it)->Q;
//			pedestal=(*data_it)->pedestal;
			Tw=tData->triggerWords.at(0);
			T = (*data_it)->T;

			for (int ii=0;ii<fa->samples.size();ii++){
//			h->Fill(ii,fa->samples.at(ii));
                 A[ii]=(fa->samples.at(ii));
                 B[ii]=(fa->samples.at(ii)-pedestal);
                 time[ii] = ii;
             }


			////// TIMING Thershold = A*10% ////////
			double B_max=B[0];
			double T_Bmax=0;

			for(int i=0; i<100; i++ ){

				if(B[i]>B_max){

					T_Bmax = i;
					B_max=B[i];
			                        }
		                            	}




               Threshold_B = B_max * 0.1;
               double Threshold_B90 = B_max*0.9;
               double T_B90=0;

      //         jout <<eventnumber<<" " <<B_max <<" " << Threshold_B <<endl;
               for (int ii=0;ii<fa->samples.size();ii++){

              	    	if (ii!=fa->samples.size()-1) {

              	    		if((B[ii+1]>=Threshold_B) && (B[ii]<Threshold_B)){

              	    		     T_B = ii;

              	    		     break;
                  	    	    	                                                                      } //endif2




              	    		/*
              	    		if(fa->samples.at(ii)>=Threshold_B) {
              	    			T_B =0;
              	    		     break;
              	    		     }
*/
              	    	                  }  else
              	   	                            {
              	   	                     T_B =0;
              	   	                            }	//endif1 - else
              	    	                       }//endfor


               for (int ii=0;ii<fa->samples.size();ii++){

                           	    	if (ii!=fa->samples.size()-1) {

                           	    		if((B[ii+1]>=Threshold_B90) && (B[ii]<Threshold_B90)){

                           	    		     T_B90 = ii;

                           	    		     break;
                               	    	    	                                                                      } //endif2




                           	    		/*
                           	    		if(fa->samples.at(ii)>=Threshold_B) {
                           	    			T_B =0;
                           	    		     break;
                           	    		     }
             */
                           	    	                  }  else
                           	   	                            {
                           	   	                     T_B =0;
                           	   	                            }	//endif1 - else
                           	    	                       }//endfor



                           rise_TB = T_B90 -T_B;
                           rise_T = T_B90 - (*data_it)->T;




			t->Fill();
//		}


}

	japp->RootUnLock();

//	downthres:


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVeto_ChargeDisplay::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVeto_ChargeDisplay::fini(void)
{

	return NOERROR;
}





