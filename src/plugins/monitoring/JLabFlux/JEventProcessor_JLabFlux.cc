// $Id$
//
//    File: JEventProcessor_JLabFlux.cc
// Created: Wed Feb 28 16:11:24 EST 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#include "JEventProcessor_JLabFlux.h"
#include <Calorimeter/CalorimeterHit.h>
#include <DAQ/eventData.h>
#include <IntVeto/IntVetoHit.h>
#include <TT/TranslationTable.h>
using namespace jana;

#include <TDirectory.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile2D.h>
#include <TStyle.h>

/*Here goes the histograms*/
static TH1D* hAsym = 0;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_JLabFlux());
}
} // "C"

//------------------
// JEventProcessor_JLabFlux (Constructor)
//------------------
JEventProcessor_JLabFlux::JEventProcessor_JLabFlux() {

}

//------------------
// ~JEventProcessor_JLabFlux (Destructor)
//------------------
JEventProcessor_JLabFlux::~JEventProcessor_JLabFlux() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_JLabFlux::init(void) {
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	japp->RootWriteLock();
	if (hAsym != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}
	TDirectory *main = gDirectory;
	gDirectory->mkdir("JLabFlux")->cd();

	hAsym = new TH1D("hAsym", "hAsym", 4, -2., 2.);

	japp->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_JLabFlux::brun(JEventLoop *eventLoop, uint32_t runnumber) {
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_JLabFlux::evnt(JEventLoop *loop, uint64_t eventnumber) {
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

	bool isLeft, isRight;
	bool isCrs;
	double tCrs;
	double tL1, tL2, tL3;
	double tR1, tR2, tR3;

	vector<const IntVetoHit*> IntVetoHits;
	vector<const IntVetoHit*>::const_iterator IntVetoHits_it;

	vector<const CalorimeterHit*> CaloHits;
	vector<const CalorimeterHit*>::const_iterator CaloHits_it;

	loop->Get(IntVetoHits);
	loop->Get(CaloHits);

	japp->RootWriteLock();

	tCrs = -1;
	tL1 = -1;
	tL2 = -1;
	tL3 = -1;
	tR1 = -1;
	tR2 = -1;
	tR3 = -1;

	//First check if we have the calo hit
	for (CaloHits_it = CaloHits.begin(); CaloHits_it != CaloHits.end(); CaloHits_it++) {
		if (((*CaloHits_it)->m_channel.x == 0) && ((*CaloHits_it)->m_channel.y == 0)) {
			tCrs = (*CaloHits_it)->T;
		}
	}

	if (tCrs < 0) {
		japp->RootUnLock();
		return OBJECT_NOT_AVAILABLE;
	}

	/*Now check for the coinc.*/
	/*left 6 4 3
	 right 7 5 3
	 200 ns crs
	 */

	for (IntVetoHits_it = IntVetoHits.begin(); IntVetoHits_it != IntVetoHits.end(); IntVetoHits_it++) {
		switch ((*IntVetoHits_it)->m_channel.component) {
		case 3:
			tL1 = (*IntVetoHits_it)->T;
			tR1 = (*IntVetoHits_it)->T;
			break;
		case 4:
			tL2 = (*IntVetoHits_it)->T;
			break;
		case 5:
			tR2 = (*IntVetoHits_it)->T;
			break;
		case 6:
			tL3 = (*IntVetoHits_it)->T;
			break;
		case 7:
			tR3 = (*IntVetoHits_it)->T;
			break;
		default:
			break;
		}
	}

	/*Now check*/
	/*LEFT*/
	if ((tL1 > 0) && (fabs(tL1 - tCrs) < 200) && (tL2 > 0) && (fabs(tL2 - tCrs) < 200) && (tL3 > 0) && (fabs(tL3 - tCrs) < 200)) {
		hAsym->Fill(-1.);
	}
	/*RIGHT*/
	if ((tR1 > 0) && (fabs(tR1 - tCrs) < 200) && (tR2 > 0) && (fabs(tR2 - tCrs) < 200) && (tR3 > 0) && (fabs(tR3 - tCrs) < 200)) {
		hAsym->Fill(1.);
	}

	japp->RootUnLock();

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_JLabFlux::erun(void) {
// This is called whenever the run number changes, before it is
// changed to give you a chance to clean up before processing
// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_JLabFlux::fini(void) {
// Called before program exit after event processing is finished.
	return NOERROR;
}

