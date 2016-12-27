// $Id$
//
//    File: TEvent_factory_CataniaProto2.cc
// Created: Thu Dec 22 12:40:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>


#include <EventBuilder/TEvent.h>
#include <EventBuilder/TEventHeader.h>
#include <Calorimeter/CalorimeterHit.h>
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>
#include <DAQ/eventData.h>

#include <JANA/JApplication.h>
#include "TClonesArray.h"


#include "TEvent_factory_CataniaProto2.h"
using namespace std;
using namespace jana;

//------------------
// init
//------------------
jerror_t TEvent_factory_CataniaProto2::init(void)
{

	japp->RootWriteLock();
	m_CaloHits=new TClonesArray("CalorimeterHit");
	m_IntVetoHits=new TClonesArray("IntVetoHit");
	m_ExtVetoHits=new TClonesArray("ExtVetoHit");
	japp->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TEvent_factory_CataniaProto2::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TEvent_factory_CataniaProto2::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	vector <const CalorimeterHit*> chits;
	vector <const IntVetoHit*> ivhits;
	vector <const ExtVetoHit*> evhits;
	const eventData* tData;

	/*Create the TEvent*/
	TEvent* m_event = new TEvent;
	TEventHeader *m_eventHeader=m_event->getEventHeader();
	/*Set the Event header*/
	try{
		loop->GetSingle(tData);
	}
	catch(unsigned long e){
		jout<<"TEvent_factory_CataniaProto2::evnt no eventData bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}
	m_eventHeader->setEventType(CataniaProto1Event);
	m_eventHeader->setRunNumber(tData->runN);
	m_eventHeader->setEventNumber(tData->eventN);
	m_eventHeader->setEventTime(tData->time);
	m_eventHeader->setTriggerWords(tData->triggerWords);


	/*Loop over JANA objects, clear collections and fill them*/

	loop->Get(chits);
	m_CaloHits->Clear("C");
	for (int ii=0;ii<chits.size();ii++){
		((CalorimeterHit*)m_CaloHits->ConstructedAt(ii))->operator=(*(chits[ii]));
	}
	m_event->addCollection(m_CaloHits);


	loop->Get(ivhits);
	m_IntVetoHits->Clear("C");
	for (int ii=0;ii<ivhits.size();ii++){
		((IntVetoHit*)m_IntVetoHits->ConstructedAt(ii))->operator=(*(ivhits[ii]));
	}
	m_event->addCollection(m_IntVetoHits);


	loop->Get(evhits);
	m_ExtVetoHits->Clear("C");
	for (int ii=0;ii<evhits.size();ii++){
		((ExtVetoHit*)m_ExtVetoHits->ConstructedAt(ii))->operator=(*(evhits[ii]));
	}
	m_event->addCollection(m_ExtVetoHits);



	/*publish the event*/
	_data.push_back(m_event);



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TEvent_factory_CataniaProto2::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t TEvent_factory_CataniaProto2::fini(void)
{
	japp->RootWriteLock();
	//	if (m_CaloHits!=0) delete (m_CaloHits);
	//	if (m_IntVetoHits!=0) delete (m_IntVetoHits);
	//	if (m_ExtVetoHits!=0) delete (m_ExtVetoHits);
	japp->RootUnLock();
	return NOERROR;
}

