// $Id$
//
//    File: JEventProcessor_test.cc
// Created: Mon Nov 13 08:49:26 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#include "JEventProcessor_test.h"
using namespace jana;
using namespace std;

#include "IntVeto/IntVetoDigiHit.h"

#include <TDirectory.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile2D.h>
#include <TStyle.h>

static TH1D *h1 = NULL;
static TH1D *h2 = NULL;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_test());
}
} // "C"

//------------------
// JEventProcessor_test (Constructor)
//------------------
JEventProcessor_test::JEventProcessor_test() {

}

//------------------
// ~JEventProcessor_test (Destructor)
//------------------
JEventProcessor_test::~JEventProcessor_test() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_test::init(void) {

	// lock all root operations
	japp->RootWriteLock();

	// First thread to get here makes all histograms. If one pointer is
	// already not NULL, assume all histograms are defined and return now
	if (h1 != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}
	TDirectory *main = gDirectory;
	gDirectory->mkdir("test")->cd();
	h1 = new TH1D("h1", "h1", 100, 0, 1000);
	h2 = new TH1D("h2", "h2", 100, 0, 1000);
	// back to main dir
	main->cd();

	// unlock
	japp->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_test::brun(JEventLoop *eventLoop, int32_t runnumber) {
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_test::evnt(JEventLoop *loop, uint64_t eventnumber) {
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

	vector<const IntVetoDigiHit*> m_IntVetoDigiHits;
	vector<const IntVetoDigiHit*>::const_iterator it;

	loop->Get(m_IntVetoDigiHits);

	// Lock ROOT
	japp->RootWriteLock();
	for (it = m_IntVetoDigiHits.begin(); it != m_IntVetoDigiHits.end(); it++) {
		if ((((*it)->m_channel).layer == 0) && (((*it)->m_channel).component == 0)) {
			h1->Fill((*it)->Qraw);
		}
	}

	// Unlock ROOT
	japp->RootUnLock();
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_test::erun(void) {
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_test::fini(void) {
	// Called before program exit after event processing is finished.
	return NOERROR;
}

