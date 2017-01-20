/*
 * ExtVeto_eff_check.cc
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */
#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVeto_eff_check.h"
#include "system/BDXEventProcessor.h"

#include <DAQ/fa250Mode1Hit.h>

#include <TT/TranslationTable.h>

#include <DAQ/eventData.h>

#include <ExtVeto/ExtVetoHit.h>

#include <IntVeto/IntVetoHit.h>


#include <Calorimeter/CalorimeterHit.h>

#include <Paddles/PaddlesDigiHit.h>


#include <EventBuilder/CataniaEvent.h>



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
	app->AddProcessor(new ExtVeto_eff_check());
}
} // "C"


//------------------
// ExtVeto_SigDisplay (Constructor)
//------------------
ExtVeto_eff_check::ExtVeto_eff_check():m_isFirstCallToBrun(1),m_ROOTOutput(0)
{

}

//------------------
// ~ExtVeto_SigDisplay (Destructor)
//------------------
ExtVeto_eff_check::~ExtVeto_eff_check()
{

}
//------------------
// init
//------------------
jerror_t ExtVeto_eff_check::init(void)
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
	t=new TTree("t","t");

    t->Branch("multi_iv_0",&multi_iv_0);
    t->Branch("multi_iv_1",&multi_iv_1);
    t->Branch("multi_iv_2",&multi_iv_2);
    t->Branch("multi_iv_3",&multi_iv_3);
    t->Branch("multi_iv_4",&multi_iv_4);
    t->Branch("multi_iv_5",&multi_iv_5);
    t->Branch("multi_iv_6",&multi_iv_6);

    t->Branch("multi_ev_0",&multi_ev_0);
    t->Branch("multi_ev_1",&multi_ev_1);
    t->Branch("multi_ev_2",&multi_ev_2);
    t->Branch("multi_ev_3",&multi_ev_3);
    t->Branch("multi_ev_4",&multi_ev_4);
    t->Branch("multi_ev_5",&multi_ev_5);
    t->Branch("multi_ev_6",&multi_ev_6);
    t->Branch("multi_ev_7",&multi_ev_7);
    t->Branch("multi_ev_8",&multi_ev_8);
    t->Branch("multi_ev_9",&multi_ev_9);
    t->Branch("multi_ev_10",&multi_ev_10);
    t->Branch("multi_ev_11",&multi_ev_11);

    t->Branch("T_cal",&T_cal);


    t->Branch("E_cal",&E_cal);



    t->Branch("multi_ev",&multi_ev);
    t->Branch("multi_iv",&multi_iv);

    t->Branch("multi_ev_c",&multi_ev_c);
     t->Branch("multi_iv_c",&multi_iv_c);

    t->Branch("Tw",&Tw);
    t->Branch("good_ped", &good_ped);




	t->Branch("E_up",&E_up);
	t->Branch("E_down",&E_down);
	t->Branch("T_up",&T_up);
	t->Branch("T_downgatta",&T_down);
	t->Branch("eventN",&eventN);



    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVeto_eff_check::brun(JEventLoop *loop, int32_t runnumber)
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
jerror_t ExtVeto_eff_check::evnt(JEventLoop *loop,uint64_t eventnumber)
{
	vector<const CataniaEvent*> data;
	vector<const CataniaEvent*>::const_iterator data_it;

	vector<const IntVetoHit*> ivhit;
	vector<const IntVetoHit*>::const_iterator ivhit_it;

	vector<const ExtVetoHit*> evhit;
	vector<const ExtVetoHit*>::const_iterator evhit_it;

	vector<const PaddlesDigiHit*> pdata;
	vector<const PaddlesDigiHit*>::const_iterator pdata_it;



	const fa250Mode1Hit *fa;
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
	loop->Get(data);
	loop->Get(pdata);
	loop->Get(ivhit);
	loop->Get(evhit);


//	jout<<eventnumber<<" tWord= "<<tWord<<endl;

         E_cal=0;
         T_cal=0;
         multi_iv=0;
         multi_ev=0;
        multi_ev_c=0;
        multi_iv_c=0;


	japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	Tw = tWord;
	for (data_it=data.begin();data_it<data.end();data_it++){
		const CataniaEvent *evhit = *data_it;

          E_cal = evhit->E;
          T_cal = evhit->T;

          multi_iv = evhit->nIntVetoHits;
          multi_ev = evhit->nExtVetoHits;
          multi_iv_c = evhit->nIntVetoHitsCoincidence;
           multi_ev_c = evhit->nExtVetoHitsCoincidence;
            if(evhit->flag_RMS ==1){
            	good_ped = 1;
            }else{
            	good_ped =0;
            }
	}
   multi_iv=0;
  int  multi_iv_temp=0;
	multi_iv_0=0;
	multi_iv_1=0;
	multi_iv_2=0;
	multi_iv_3=0;
	multi_iv_4=0;
	multi_iv_5=0;
	multi_iv_6=0;


		for (ivhit_it=ivhit.begin();ivhit_it<ivhit.end();ivhit_it++){
		const IntVetoHit *iivhit = *ivhit_it;

		   if (iivhit->T >=0){
                multi_iv_temp++;
			   switch (iivhit->m_channel.component){
		     case 0:
		    	 multi_iv_0++;

		    	 break;

		     case 1:
		    	 multi_iv_1++;
		    	  break;

		     case 2:
		    	 multi_iv_2++;;
		    	  break;

		     case 3:
		    	 multi_iv_3++;
		    	  break;

		     case 4:
		    	 multi_iv_4++;
		    	  break;

		     case 5:
		    	 multi_iv_5++;
		    	  break;

		     case 6:
		    	 multi_iv_6++;
		    	  break;


		                                   } // end switch


	                                                          	} //end if

	}  //end for on IV

		multi_iv = multi_iv_temp;


		   multi_ev=0;
		  int  multi_ev_temp=0;
		multi_ev_0=0;
		multi_ev_1=0;
		multi_ev_2=0;
		multi_ev_3=0;
		multi_ev_4=0;
		multi_ev_5=0;
		multi_ev_6=0;
		multi_ev_7=0;
		multi_ev_8=0;
		multi_ev_9=0;
		multi_ev_10=0;
		multi_ev_11=0;

		for (evhit_it=evhit.begin();evhit_it<evhit.end();evhit_it++){
		const ExtVetoHit *eevhit = *evhit_it;

		   if (eevhit->T >=0){
                  multi_ev_temp++;
			   switch (eevhit->m_channel.component){
		     case 0:
		    	 multi_ev_0++;
		    	 break;

		     case 1:
		    	 multi_ev_1++;
		    	  break;

		     case 2:
		    	 multi_ev_2++;;
		    	  break;

		     case 3:
		    	 multi_ev_3++;
		    	  break;

		     case 4:
		    	 multi_ev_4++;
		    	  break;

		     case 5:
		    	 multi_ev_5++;
		    	  break;

		     case 6:
		    	 multi_ev_6++;
		    	  break;

		     case 7:
		    	 multi_ev_7++;
		    	 break;

		     case 8:
		    	 multi_ev_8++;
		    	  break;

		     case 9:
		    	 multi_ev_9++;;
		    	  break;

		     case 10:
		    	 multi_ev_10++;
		    	  break;

		     case 11:
		    	 multi_ev_11++;
		    	  break;

		                                   } // end switch



	                                                          	} //end if





	} //end EV
multi_ev= multi_ev_temp;



		E_up=0;
		E_down=0;
		T_up=0;
		T_down=0;

		double E[2];
		double T[2];

		for(int i=0;i<2;i++){
			E[i]=0;
			T[i]=0;
		}

		for (pdata_it=pdata.begin();pdata_it<pdata.end();pdata_it++){

			const PaddlesDigiHit *phit = *pdata_it;



					if(phit->m_channel.id==0){
						E[0]=phit->Q;
						T[0]=phit->T;
					}
					if(phit->m_channel.id==1){
										E[1]=phit->Q;
										T[1]=phit->T;
									}

		}

		E_down=E[0];
		E_up=E[1];
		T_down=T[0];
		T_up=T[1];









			t->Fill();





	japp->RootUnLock();

//	downthres:


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVeto_eff_check::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVeto_eff_check::fini(void)
{

	return NOERROR;
}





