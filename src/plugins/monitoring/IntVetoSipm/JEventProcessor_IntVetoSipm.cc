// $Id$
//
//    File: JEventProcessor_IntVetoSipm.cc
// Created: Tue Feb  6 19:17:53 CET 2018
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#include "JEventProcessor_IntVetoSipm.h"
#include <IntVeto/IntVetoSiPMHit.h>
#include <TT/TranslationTable.h>
#include <DAQ/eventData.h>
using namespace jana;

#include <TDirectory.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile2D.h>
#include <TStyle.h>

#include "TROOT.h"
/*Here goes the histograms*/
static const int nSectors = 1;
static const int nLayers = 1;
static const int nComponents = 16;
static const int nReadouts = 4;
static TH1D *hSipmCharge[nSectors][nLayers][nComponents][nReadouts + 1] = { 0 };
static TH1D *hSipmAmpl[nSectors][nLayers][nComponents][nReadouts + 1] = { 0 };

static double normQ[nSectors][nLayers][nComponents][nReadouts + 1] = { 0 };
static double normA[nSectors][nLayers][nComponents][nReadouts + 1] = { 0 };

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_IntVetoSipm());
}
} // "C"

//------------------
// JEventProcessor_IntVetoSipm (Constructor)
//------------------
JEventProcessor_IntVetoSipm::JEventProcessor_IntVetoSipm() {

}

//------------------
// ~JEventProcessor_IntVetoSipm (Destructor)
//------------------
JEventProcessor_IntVetoSipm::~JEventProcessor_IntVetoSipm() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_IntVetoSipm::init(void) {
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	japp->RootWriteLock();
	if (hSipmCharge[0][0][0][0] != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}
	gROOT->cd();
	TDirectory *main = gDirectory;
	gDirectory->mkdir("IntVetoSipm")->cd();

	/*Create all the histograms*/
	for (int isector = 0; isector < nSectors; isector++) {
		for (int ilayer = 0; ilayer < nLayers; ilayer++) {
			for (int icomponent = 0; icomponent < nComponents; icomponent++) {
				for (int ireadout = 0; ireadout < nReadouts; ireadout++) {
					hSipmCharge[isector][ilayer][icomponent][ireadout] = new TH1D(Form("hSipmCharge_s%i_l%i_c%i_r%i", isector, ilayer, icomponent, ireadout), Form("hSipmCharge_s%i_l%i_c%i_r%i", isector, ilayer, icomponent, ireadout), 120, -2., 10.);
					hSipmCharge[isector][ilayer][icomponent][ireadout]->GetXaxis()->SetTitle("Q - phe");
					hSipmAmpl[isector][ilayer][icomponent][ireadout] = new TH1D(Form("hSipmAmpl_s%i_l%i_c%i_r%i", isector, ilayer, icomponent, ireadout), Form("hSipmAmpl_s%i_l%i_c%i_r%i", isector, ilayer, icomponent, ireadout), 120, -2., 10.);
					hSipmAmpl[isector][ilayer][icomponent][ireadout]->GetXaxis()->SetTitle("Ampl - phe");
				}
			}
		}
	}
	// back to main dir
	main->cd();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_IntVetoSipm::brun(JEventLoop *eventLoop, uint32_t runnumber) {
	// This is called whenever the run number changes

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_IntVetoSipm::evnt(JEventLoop *loop, uint64_t eventnumber) {
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

	vector<const IntVetoSiPMHit*> data;
	vector<const IntVetoSiPMHit*>::const_iterator data_it;
	loop->Get(data);

	const eventData* tData;

	try {
		loop->GetSingle(tData);
	} catch (unsigned long e) {
		return OBJECT_NOT_AVAILABLE;
	}

	japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	for (data_it = data.begin(); data_it < data.end(); data_it++) {
		m_sector = (*data_it)->m_channel.int_veto->sector;
		m_layer = (*data_it)->m_channel.int_veto->layer;
		m_component = (*data_it)->m_channel.int_veto->component;
		m_readout = (*data_it)->m_channel.int_veto->readout;

		if ((m_sector < 0) || (m_sector >= nSectors) || (m_layer < 0) || (m_layer >= nLayers) || (m_component < 0) || (m_component >= nComponents) || (m_readout < 0) || (m_readout > nReadouts)) continue;

		m_type = (*data_it)->m_type;
		T = (*data_it)->T;
		Aphe = (*data_it)->Aphe;
		Qphe = (*data_it)->Qphe;

		Araw = (*data_it)->Araw;
		Qraw = (*data_it)->Qraw;

		/*The following mimics calibration code for sipms*/
		if ((T < 200) || (T > 1800)) continue;

		m_hSipmQ = hSipmCharge[m_sector][m_layer][m_component][m_readout];
		m_hSipmA = hSipmAmpl[m_sector][m_layer][m_component][m_readout];

		if (normQ[m_sector][m_layer][m_component][m_readout] > 0) m_hSipmQ->Scale(normQ[m_sector][m_layer][m_component][m_readout]);
		if (normA[m_sector][m_layer][m_component][m_readout] > 0) m_hSipmA->Scale(normA[m_sector][m_layer][m_component][m_readout]);

		m_hSipmA->Fill(Aphe);
		m_hSipmQ->Fill(Qphe);

		normQ[m_sector][m_layer][m_component][m_readout] = m_hSipmQ->Integral(m_hSipmQ->FindBin(-2.), m_hSipmQ->FindBin(10.));
		normA[m_sector][m_layer][m_component][m_readout] = m_hSipmA->Integral(m_hSipmA->FindBin(-2.), m_hSipmA->FindBin(10.));

		if (normQ[m_sector][m_layer][m_component][m_readout] > 0) m_hSipmQ->Scale(1 / normQ[m_sector][m_layer][m_component][m_readout]);
		if (normA[m_sector][m_layer][m_component][m_readout] > 0) m_hSipmA->Scale(1 / normA[m_sector][m_layer][m_component][m_readout]);
	}

	japp->RootUnLock();
	return NOERROR;

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_IntVetoSipm::erun(void) {
// This is called whenever the run number changes, before it is
// changed to give you a chance to clean up before processing
// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_IntVetoSipm::fini(void) {
// Called before program exit after event processing is finished.
	return NOERROR;
}

