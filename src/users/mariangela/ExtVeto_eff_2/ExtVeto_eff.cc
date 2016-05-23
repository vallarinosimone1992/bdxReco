/*
 * ExtVeto_eff.cc
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */
#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVeto_eff.h"
#include <system/BDXEventProcessor.h>

#include <DAQ/eventData.h>
#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <TT/TranslationTable.h>

#include <EventBuilder/CataniaEvent.h>

#include <EventBuilder/CataniaEvent.h>
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
	app->AddProcessor(new ExtVeto_eff());
}
} // "C"


//------------------
// ExtVeto_SigDisplay (Constructor)
//------------------
ExtVeto_eff::ExtVeto_eff():m_isFirstCallToBrun(1),m_ROOTOutput(0)
{

}

//------------------
// ~ExtVeto_SigDisplay (Destructor)
//------------------
ExtVeto_eff::~ExtVeto_eff()
{

}
//------------------
// init
//------------------
jerror_t ExtVeto_eff::init(void)
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

    t->Branch("T_cal",&T_cal);
    t->Branch("T_ev",&T_ev);
    t->Branch("T_iv",&T_iv);

    t->Branch("E_cal",&E_cal);
    t->Branch("E_ev", &E_ev);
    t->Branch("E_iv", &E_iv);

    t->Branch("multi_cal",&multi_cal);
    t->Branch("multi_ev",&multi_ev);
    t->Branch("multi_iv",&multi_iv);

    t->Branch("multi_ev_c",&multi_ev_c);
     t->Branch("multi_iv_c",&multi_iv_c);

    t->Branch("Tw",&Tw);
    t->Branch("good_ped", &good_ped);


    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVeto_eff::brun(JEventLoop *loop, int32_t runnumber)
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
jerror_t ExtVeto_eff::evnt(JEventLoop *loop,uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	vector<const CataniaEvent*> data;
	vector<const CataniaEvent*>::const_iterator data_it;
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

    //    jout <<evhit->flag_RMS<< " "<< good_ped << " "<< E_cal << " "<<multi_ev<<" "<<multi_iv<<endl;

	}

			t->Fill();




	japp->RootUnLock();

//	downthres:


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVeto_eff::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVeto_eff::fini(void)
{

	return NOERROR;
}





