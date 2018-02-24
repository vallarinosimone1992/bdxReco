// $Id$
//
//    File: JEventProcessor_CalorimeterHits.cc
// Created: Sat Feb 24 13:11:49 CET 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#include "JEventProcessor_CalorimeterHits.h"
#include <Calorimeter/CalorimeterHit.h>
#include <DAQ/eventData.h>
#include <TT/TranslationTable.h>
using namespace jana;

#include <TDirectory.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile2D.h>
#include <TStyle.h>

/*Here goes the histograms*/
static const int nSectors = 2;
static const int nX = 4;
static const int nY = 4;

static TH1D *hCaloHitE_allTrg[nSectors][nX][nY] = { 0 };
static TH1D *hCaloHitE_rndmTrg[nSectors][nX][nY] = { 0 };

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_CalorimeterHits());
}
} // "C"

//------------------
// JEventProcessor_CalorimeterHits (Constructor)
//------------------
JEventProcessor_CalorimeterHits::JEventProcessor_CalorimeterHits() {

}

//------------------
// ~JEventProcessor_CalorimeterHits (Destructor)
//------------------
JEventProcessor_CalorimeterHits::~JEventProcessor_CalorimeterHits() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_CalorimeterHits::init(void) {
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	japp->RootWriteLock();
	if (hCaloHitE_allTrg[0][0][0] != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}
	TDirectory *main = gDirectory;
	gDirectory->mkdir("CalorimeterHits")->cd();

	/*Create all the histograms*/
	for (int isector = 0; isector < nSectors; isector++) {
		for (int iX = 0; iX < nX; iX++) {
			for (int iY = 0; iY < nY; iY++) {
				hCaloHitE_allTrg[isector][iX][iY] = new TH1D(Form("hCaloHitE_allTrg_s%i_x%i_y%i", isector, iX, iY),
						Form("hCaloHitE_allTrg_s%i_x%i_y%i", isector, iX, iY), 210, -10., 200.);
				hCaloHitE_allTrg[isector][iX][iY]->GetXaxis()->SetTitle("E(MeV)");
				hCaloHitE_rndmTrg[isector][iX][iY] = new TH1D(Form("hCaloHitE_rndmTrg_s%i_x%i_y%i", isector, iX, iY),
						Form("hCaloHitE_rndmTrg_s%i_x%i_y%i", isector, iX, iY), 210, -10., 200.);
				hCaloHitE_rndmTrg[isector][iX][iY]->GetXaxis()->SetTitle("E(MeV)");
			}
		}
	}
	// back to main dir
	main->cd();
	japp->RootUnLock();


	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_CalorimeterHits::brun(JEventLoop *eventLoop, uint32_t runnumber) {
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_CalorimeterHits::evnt(JEventLoop *loop, uint64_t eventnumber) {
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
	int trgWord;
	vector<const CalorimeterHit*> data;
	vector<const CalorimeterHit*>::const_iterator data_it;
	loop->Get(data);

	const eventData* tData;

	try {
		loop->GetSingle(tData);
	} catch (unsigned long e) {
		return OBJECT_NOT_AVAILABLE;
	}

	/*Decode trigger words - if it is random, then it is the first!*/
	japp->RootWriteLock();
	trgWord = tData->triggerWords[0];

	for (data_it = data.begin(); data_it < data.end(); data_it++) {
		m_sector = (*data_it)->m_channel.sector;
		m_X = (*data_it)->m_channel.x;
		m_Y = (*data_it)->m_channel.y;
		E = (*data_it)->E;
		T = (*data_it)->T;
		if ((m_sector<0)||(m_sector>=nSectors)||(m_X<0)||(m_X>=nX)||(m_Y<0)||(m_Y>=nY)){
			continue;
		}

		hEhit_allTrg=hCaloHitE_allTrg[m_sector][m_X][m_Y];
		hEhit_rndmTrg=hCaloHitE_rndmTrg[m_sector][m_X][m_Y];

		hEhit_allTrg->Fill(E);
		if ((trgWord>>rndmTrgBitID)&0x1){
			hEhit_rndmTrg->Fill(E);
		}
	}
	japp->RootUnLock();
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_CalorimeterHits::erun(void) {
// This is called whenever the run number changes, before it is
// changed to give you a chance to clean up before processing
// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_CalorimeterHits::fini(void) {
// Called before program exit after event processing is finished.
	return NOERROR;
}

