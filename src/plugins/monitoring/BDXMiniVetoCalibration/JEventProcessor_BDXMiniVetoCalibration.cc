// $Id$
//
//    File: JEventProcessor_BDXMiniVetoCalibration.cc
// Created: Fri May 17 12:22:59 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#include "JEventProcessor_BDXMiniVetoCalibration.h"
using namespace jana;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

#include "calibration/BDXMiniVetoCalibrationHit/BDXMiniVetoCalibrationHit.h"
#include "DAQ/eventData.h"

#include <system/JROOTOutput.h>
#include <system/BDXEventProcessor.h>

#include "TH1D.h"
#include "TROOT.h"

/*Here goes the histograms*/
static const int nTOT = 10;
//Charge

static TH1D *hBDXMiniIVetoChargeCalibrationTOP[nTOT] = { 0 };
static TH1D *hBDXMiniOVetoChargeCalibrationTOP[nTOT] = { 0 };
static TH1D *hBDXMiniIVetoChargeCalibrationBOT[nTOT] = { 0 };
static TH1D *hBDXMiniOVetoChargeCalibrationBOT[nTOT] = { 0 };

static TH1D *hBDXMiniIVetoChargeCalibrationTOPmatched[nTOT] = { 0 };
static TH1D *hBDXMiniOVetoChargeCalibrationTOPmatched[nTOT] = { 0 };
static TH1D *hBDXMiniIVetoChargeCalibrationBOTmatched[nTOT] = { 0 };
static TH1D *hBDXMiniOVetoChargeCalibrationBOTmatched[nTOT] = { 0 };

extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_BDXMiniVetoCalibration());
}
} // "C"

//------------------
// JEventProcessor_BDXMiniVetoCalibration (Constructor)
//------------------
JEventProcessor_BDXMiniVetoCalibration::JEventProcessor_BDXMiniVetoCalibration() :
		m_ROOTOutput(0), m_isMC(0) {
	m_isFirstCallToBrun = 1;

}

//------------------
// ~JEventProcessor_BDXMiniVetoCalibration (Destructor)
//------------------
JEventProcessor_BDXMiniVetoCalibration::~JEventProcessor_BDXMiniVetoCalibration() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_BDXMiniVetoCalibration::init(void) {

	m_isMC = 0;
	gPARMS->GetParameter("MC", m_isMC);

	japp->RootWriteLock();

	if (hBDXMiniIVetoChargeCalibrationTOP[0] != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}

	gROOT->cd();
	TDirectory *main = gDirectory;
	gDirectory->mkdir("BDXMiniVetoCalib")->cd();

	double Qmin, Qmax;
	int NQ;

	if (m_isMC) {
		Qmin = -10.5;
		Qmax = 200.5;
		NQ = 211;
	} else {
		Qmin = -10.5;
		Qmax = 200.5;
		NQ = 211 * 4;
	}

	for (int id = 0; id < 10; id++) {

		hBDXMiniIVetoChargeCalibrationTOP[id] = new TH1D(Form("hBDXMiniIVetoCalibQTOP_c%i", id + 1), Form("hBDXMiniIVetoCalibQTOP_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniIVetoChargeCalibrationTOP[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniOVetoChargeCalibrationTOP[id] = new TH1D(Form("hBDXMiniOVetoCalibQTOP_c%i", id + 1), Form("hBDXMiniOVetoCalibQTOP_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniOVetoChargeCalibrationTOP[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniIVetoChargeCalibrationBOT[id] = new TH1D(Form("hBDXMiniIVetoCalibQBOT_c%i", id + 1), Form("hBDXMiniIVetoCalibQBOT_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniIVetoChargeCalibrationBOT[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniOVetoChargeCalibrationBOT[id] = new TH1D(Form("hBDXMiniOVetoCalibQBOT_c%i", id + 1), Form("hBDXMiniOVetoCalibQBOT_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniOVetoChargeCalibrationBOT[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniIVetoChargeCalibrationTOPmatched[id] = new TH1D(Form("hBDXMiniIVetoCalibQTOPm_c%i", id + 1), Form("hBDXMiniIVetoCalibQTOPm_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniIVetoChargeCalibrationTOPmatched[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniOVetoChargeCalibrationTOPmatched[id] = new TH1D(Form("hBDXMiniOVetoCalibQTOPm_c%i", id + 1), Form("hBDXMiniOVetoCalibQTOPm_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniOVetoChargeCalibrationTOPmatched[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniIVetoChargeCalibrationBOTmatched[id] = new TH1D(Form("hBDXMiniIVetoCalibQBOTm_c%i", id + 1), Form("hBDXMiniIVetoCalibQBOTm_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniIVetoChargeCalibrationBOTmatched[id]->GetXaxis()->SetTitle("Q");

		hBDXMiniOVetoChargeCalibrationBOTmatched[id] = new TH1D(Form("hBDXMiniOVetoCalibQBOTm_c%i", id + 1), Form("hBDXMiniOVetoCalibQBOTm_c%i", id + 1), NQ, Qmin, Qmax);
		hBDXMiniOVetoChargeCalibrationBOTmatched[id]->GetXaxis()->SetTitle("Q");
	}

	// back to main dir
	main->cd();
	japp->RootUnLock();

	return NOERROR;

}

//------------------
// brun
//------------------
jerror_t JEventProcessor_BDXMiniVetoCalibration::brun(JEventLoop *eventLoop, int32_t runnumber) {
	jout << m_isFirstCallToBrun << endl;

	if (m_isFirstCallToBrun) {

		jout << "JEventProcessor_BDXMiniVetoCalibration::brun searching m_RootOutput" << endl;
		string class_name, this_class_name;
		string joutput_name;
		BDXEventProcessor *m_BDXEventProcessor;
		vector<JEventProcessor*> m_processors = app->GetProcessors();
		vector<JEventProcessor*>::iterator m_processors_it;

		m_isFirstCallToBrun = 0;
		class_name = "BDXEventProcessor";
		joutput_name = "JROOTOutput";
		//Now I need to determine which processor is the one holding the output. Discussing with David, he suggested just to check the class name, since
		//a dynamic cast may not work with plugins
		for (m_processors_it = m_processors.begin(); m_processors_it != m_processors.end(); m_processors_it++) {
			if ((*m_processors_it) != 0) {
				this_class_name = string((*m_processors_it)->className());
				if (this_class_name == class_name) {
					m_BDXEventProcessor = (BDXEventProcessor*) (*m_processors_it);
					if (m_BDXEventProcessor->getOutput() == 0) {
						jerr << "BDXEventProcessor JOutput is null!" << endl;
						break;
					}
					if (string(m_BDXEventProcessor->getOutput()->className()) == joutput_name) {
						m_ROOTOutput = (JROOTOutput*) (m_BDXEventProcessor->getOutput());
						jout << "Got JROOTOutput!" << endl;
					} else {
						jerr << "BDXEventProcessor JOutput is not null BUT class is: " << m_BDXEventProcessor->getOutput()->className() << endl;
					}
				}
			}
		}

		/*For ALL objects you want to add to ROOT file, use the following:*/
		if (m_ROOTOutput) {
		//	m_ROOTOutput->AddObject(h);
		//	m_ROOTOutput->AddObject(t);
		}
	}

	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_BDXMiniVetoCalibration::evnt(JEventLoop *loop, uint64_t eventnumber) {
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

	vector<const BDXMiniVetoCalibrationHit*> veto_hits;
	vector<const BDXMiniVetoCalibrationHit*>::const_iterator veto_hits_it;
	const eventData* tData;
	if (!m_isMC) {
		try {
			loop->GetSingle(tData);
		} catch (unsigned long e) {
			jout << "JEventProcessor_Trigger::evnt no eventData bank this event" << std::endl;
			return OBJECT_NOT_AVAILABLE;
		}
	}
	loop->Get(veto_hits);
	const BDXMiniVetoCalibrationHit *m_VetoHit;
	int sector, layer, component;
	for (veto_hits_it = veto_hits.begin(); veto_hits_it != veto_hits.end(); veto_hits_it++) {
		m_VetoHit = (*veto_hits_it);

		sector = m_VetoHit->m_channel.sector;
		layer = m_VetoHit->m_channel.layer;
		component = m_VetoHit->m_channel.component; //from 1 to 10
		component = component - 1; //from 0 to 9
		japp->RootWriteLock();

		japp->RootUnLock();
	}
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_BDXMiniVetoCalibration::erun(void) {
	if (m_ROOTOutput != 0) {
		jout << "JEventProcessor_BDXMiniVetoCalibration::erun adding histos to m_ROOTOutput" << endl;
		for (int ii = 0; ii < nTOT; ii++) {
			m_ROOTOutput->AddObject(hBDXMiniIVetoChargeCalibrationTOP[ii]);
			m_ROOTOutput->AddObject(hBDXMiniOVetoChargeCalibrationTOP[ii]);
			m_ROOTOutput->AddObject(hBDXMiniIVetoChargeCalibrationTOPmatched[ii]);
			m_ROOTOutput->AddObject(hBDXMiniOVetoChargeCalibrationTOPmatched[ii]);
		}
		for (int ii = 0; ii < nTOT; ii++) {
			m_ROOTOutput->AddObject(hBDXMiniIVetoChargeCalibrationBOT[ii]);
			m_ROOTOutput->AddObject(hBDXMiniOVetoChargeCalibrationBOT[ii]);
			m_ROOTOutput->AddObject(hBDXMiniIVetoChargeCalibrationBOTmatched[ii]);
			m_ROOTOutput->AddObject(hBDXMiniOVetoChargeCalibrationBOTmatched[ii]);
		}

	}

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_BDXMiniVetoCalibration::fini(void) {
	// Called before program exit after event processing is finished.
	return NOERROR;
}

