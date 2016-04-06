// $Id$
//
//    File: JEventProcessor_test2.cc
// Created: Tue Mar  1 01:15:03 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#include "JEventProcessor_test2.h"

#include "../../../libraries/MC/GenParticle.h"
using namespace jana;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_test2());
}
} // "C"


//------------------
// JEventProcessor_test2 (Constructor)
//------------------
JEventProcessor_test2::JEventProcessor_test2()
{

}

//------------------
// ~JEventProcessor_test2 (Destructor)
//------------------
JEventProcessor_test2::~JEventProcessor_test2()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_test2::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_test2::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes




	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_test2::evnt(JEventLoop *loop, uint64_t eventnumber)
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

	const GenParticle* particle;
	try{
		loop->GetSingle(particle);
		particle->Print();
	}
	catch (unsigned long a){
		jerr<<"test2: "<<a<<endl;
	}
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_test2::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_test2::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

