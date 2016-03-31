// $Id$
//
//    File: JEventProcessor_test.cc
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "Paddles_calib.h"
#include "system/BDXEventProcessor.h"
#include <TT/TranslationTable.h>
#include <DAQ/eventData.h>

#include <Paddles/PaddlesDigiHit.h>

#include <system/JROOTOutput.h>

#include "TTree.h"

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

using namespace jana;

extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new Paddles_calib());
}
} // "C"


//------------------
// Paddles_SigDisplay (Constructor)
//------------------
Paddles_calib::Paddles_calib():m_isFirstCallToBrun(1),m_ROOTOutput(0)
{

}

//------------------
// ~Paddles_SigDisplay (Destructor)
//------------------
Paddles_calib::~Paddles_calib()
{

}
//------------------
// init
//------------------
jerror_t Paddles_calib::init(void)
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

	t=new TTree("t","t");
	t->Branch("id",&id);
	t->Branch("Nphe",&Nphe);
	t->Branch("T",&T);
	t->Branch("eventN",&eventN);
	t->Branch("tword",&tword);

	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t Paddles_calib::brun(JEventLoop *loop, int32_t runnumber)
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
			m_ROOTOutput->AddObject(t);
		}

	}

	return NOERROR;


}

//------------------
// evnt
//------------------
jerror_t Paddles_calib::evnt(JEventLoop *loop,uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	vector<const PaddlesDigiHit*> data;
	vector<const PaddlesDigiHit*>::const_iterator data_it;

	loop->Get(data);

	const eventData* tData;
	try{
		loop->GetSingle(tData);
	}
	catch(unsigned long e){
		jout<<"No trig bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}

	int tWord=tData->triggerWords.at(0);

	japp->RootWriteLock();

	for (data_it=data.begin();data_it<data.end();data_it++){

		const PaddlesDigiHit *evhit = *data_it;
			// Get associated DigiHit object
//			Digi = NULL;
//			evhit->GetSingle(Digi);

//			if(!Digi) continue; // need DigiHit to continue

				eventN=eventnumber;
				tword=tWord;
				id=evhit->m_channel.id;

//				if(tword==2&&eventN==445){
//				jout<<(*data_it)->E<<endl;

				Nphe=(*data_it)->Q;
				T=(*data_it)->T;
			t->Fill();
//			}
	}

	japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t Paddles_calib::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t Paddles_calib::fini(void)
{

	return NOERROR;
}

