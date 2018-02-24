// $Id$
//
//    File: JEventProcessor_Trigger.cc
// Created: Thu Feb  8 18:39:24 CET 2018
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#include "JEventProcessor_Trigger.h"
#include <DAQ/eventData.h>
using namespace jana;

#include <TDirectory.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile2D.h>
#include <TStyle.h>

/*Here goes the histograms*/
TH1D *hTriggerBits = NULL;
TH1D *hTriggerMultiplicity = NULL;
TH1D *hTriggerTimes = NULL;
TH2D *hTriggerTimes2D = NULL;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_Trigger());
}
} // "C"

//------------------
// JEventProcessor_Trigger (Constructor)
//------------------
JEventProcessor_Trigger::JEventProcessor_Trigger() {

}

//------------------
// ~JEventProcessor_Trigger (Destructor)
//------------------
JEventProcessor_Trigger::~JEventProcessor_Trigger() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_Trigger::init(void) {
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	japp->RootWriteLock();
	TDirectory *main = gDirectory;
	gDirectory->mkdir("Trigger")->cd();

	if (hTriggerBits != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}

	hTriggerBits = new TH1D("hTriggerBits", "hTriggerBits", nTriggersMAX, -0.5, nTriggersMAX - 0.5);
	hTriggerMultiplicity = new TH1D("hTriggerMultiplicity", "hTriggerMultiplicity", 10, -0.5, 9.5);
	hTriggerTimes = new TH1D("hTriggerTimes", "hTriggerTimes", 1000, -0.5, 999.5);
	hTriggerTimes2D = new TH2D("hTriggerTimes2D", "hTriggerTimes2D", 1000, -0.5, 999.5, nTriggersMAX, -0.5, nTriggersMAX - 0.5);

	// back to main dir
	main->cd();

	// unlock
	japp->RootUnLock();
	return NOERROR;
}
//------------------
// brun
//------------------
jerror_t JEventProcessor_Trigger::brun(JEventLoop *eventLoop, uint32_t runnumber) {
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_Trigger::evnt(JEventLoop *loop, uint64_t eventnumber) {
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
	int nTriggerSingles[nTriggersMAX] = { 0 };
	vector<int> trgTimes[nTriggersMAX];

	double max = -9999;

	bool triggers[32] = { false };
	int nTriggers = 0;
	int word1, word2;

	try {
		loop->GetSingle(tData);
	} catch (unsigned long e) {
		jout << "JEventProcessor_IntVetoSipm::evnt::evnt no eventData bank this event" << std::endl;
		return OBJECT_NOT_AVAILABLE;
	}

	/*Decode trigger words*/
	for (int ii = 0; ii < tData->triggerWords.size() / 2; ii++) {
		word1 = tData->triggerWords[ii * 2];
		word2 = tData->triggerWords[ii * 2 + 1];
		for (int jj = 0; jj < nTriggersMAX; jj++) {
			if ((word1 >> jj) & 0x1) {
				nTriggers++;
				nTriggerSingles[jj]++;
				trgTimes[jj].push_back(word2);
			}
		}
	}

	/*Fill histograms*/
	japp->RootWriteLock();

	/*Trigger multiplicity*/
	hTriggerMultiplicity->Fill(nTriggers);


	/*Trigger words - trigger times*/
	for (int jj = 0; jj < nTriggersMAX; jj++){
		if (nTriggerSingles[jj]>0) hTriggerBits->Fill(jj,nTriggerSingles[jj]);
		for (int itime=0;itime<trgTimes[jj].size();itime++){
			hTriggerTimes->Fill(trgTimes[jj][itime]);
			hTriggerTimes2D->Fill(trgTimes[jj][itime],jj);
		}
	}



	app->RootUnLock();
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_Trigger::erun(void) {
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_Trigger::fini(void) {
	// Called before program exit after event processing is finished.
	return NOERROR;
}

