// $Id$
//
//    File: JEventProcessor_EventTiming.cc
// Created: Wed Mar 16 18:23:05 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_EventTiming.h"
using namespace jana;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

#include <system/BDXEventProcessor.h>
#include <system/JROOTOutput.h>

#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/eventData.h>

#include "TTree.h"



extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_EventTiming());
}
} // "C"


//------------------
// JEventProcessor_EventTiming (Constructor)
//------------------
JEventProcessor_EventTiming::JEventProcessor_EventTiming():
		m_ROOTOutput(0)
{

}

//------------------
// ~JEventProcessor_EventTiming (Destructor)
//------------------
JEventProcessor_EventTiming::~JEventProcessor_EventTiming()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_EventTiming::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	japp->RootWriteLock();
	t=new TTree("EventTiming","EventTiming");
	t->Branch("eventN",&eventNumber);
	t->Branch("eventT",&eventTime);
	japp->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_EventTiming::brun(JEventLoop *eventLoop, int32_t runnumber)
{

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





	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_EventTiming::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	const eventData* tData;
	try{
		loop->GetSingle(tData);
	}
	catch(unsigned long e){
		jout<<"No eventData bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}
	japp->RootWriteLock();
	eventTime=tData->time;
	eventNumber=eventnumber;
	t->Fill();
	japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_EventTiming::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_EventTiming::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

