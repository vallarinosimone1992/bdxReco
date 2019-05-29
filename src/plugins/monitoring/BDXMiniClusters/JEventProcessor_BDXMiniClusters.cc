// $Id$
//
//    File: JEventProcessor_BDXMiniClusters.cc
// Created: Tue May 28 11:14:15 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#include "JEventProcessor_BDXMiniClusters.h"
using namespace jana;
using namespace std;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

#include "Calorimeter/CalorimeterCluster.h"
#include "DAQ/eventData.h"

#include <system/JROOTOutput.h>
#include <system/BDXEventProcessor.h>

#include "TROOT.h"
// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

static TH1D *hBDXMiniClusterEtotTOP = 0;
static TH1D *hBDXMiniClusterEtotBOT = 0;

static TH1D *hBDXMiniClusterEseedTOP = 0;
static TH1D *hBDXMiniClusterEseedBOT = 0;

static TH1D *hBDXMiniClusterNTOP = 0;
static TH1D *hBDXMiniClusterNBOT = 0;

extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_BDXMiniClusters());
}
} // "C"

//------------------
// JEventProcessor_BDXMiniClusters (Constructor)
//------------------
JEventProcessor_BDXMiniClusters::JEventProcessor_BDXMiniClusters() :
		m_ROOTOutput(0), m_isMC(0) {
	m_isFirstCallToBrun = 1;
}

//------------------
// ~JEventProcessor_BDXMiniClusters (Destructor)
//------------------
JEventProcessor_BDXMiniClusters::~JEventProcessor_BDXMiniClusters() {

}

//------------------
// init
//------------------
jerror_t JEventProcessor_BDXMiniClusters::init(void) {
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	m_isFirstCallToBrun = 1;
	m_isMC = 0;
	gPARMS->GetParameter("MC", m_isMC);

	japp->RootWriteLock();

	if (hBDXMiniClusterEtotTOP != NULL) {
		japp->RootUnLock();
		return NOERROR;
	}

	gROOT->cd();
	TDirectory *main = gDirectory;
	gDirectory->mkdir("BDXMiniClusters")->cd();

	hBDXMiniClusterEtotTOP = new TH1D("hBDXMiniClusterEtotTOP", "hBDXMiniClusterEtotTOP", 250, 0, 1000.);
	hBDXMiniClusterEtotBOT = new TH1D("hBDXMiniClusterEtotBOT", "hBDXMiniClusterEtotBOT", 250, 0, 1000.);

	hBDXMiniClusterEseedTOP = new TH1D("hBDXMiniClusterEseedTOP", "hBDXMiniClusterEseedTOP", 250, 0, 500.);
	hBDXMiniClusterEseedBOT = new TH1D("hBDXMiniClusterEseedBOT", "hBDXMiniClusterEseedBOT", 250, 0, 500.);

	hBDXMiniClusterNTOP = new TH1D("hBDXMiniClusterNTOP", "hBDXMiniClusterNTOP", 25, -0.5, 24.5);
	hBDXMiniClusterNBOT = new TH1D("hBDXMiniClusterNBOT", "hBDXMiniClusterNBOT", 25, -0.5, 24.5);

	// back to main dir
	main->cd();
	japp->RootUnLock();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_BDXMiniClusters::brun(JEventLoop *eventLoop, int32_t runnumber) {
	jout << m_isFirstCallToBrun << endl;

	if (m_isFirstCallToBrun) {

		jout << "JEventProcessor_BDXMiniClusters::brun searching m_RootOutput" << endl;
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
	}
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_BDXMiniClusters::evnt(JEventLoop *loop, uint64_t eventnumber) {
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

	vector<const CalorimeterCluster*> calo_clusters;
	vector<const CalorimeterCluster*>::const_iterator calo_clusters_it;
	const eventData* tData;
	if (!m_isMC) {
		try {
			loop->GetSingle(tData);
		} catch (unsigned long e) {
			jout << "JEventProcessor_BDXMiniCalorimeterEnergyCalibration::evnt no eventData bank this event" << std::endl;
			return OBJECT_NOT_AVAILABLE;
		}
	}
	loop->Get(calo_clusters);

	const CalorimeterCluster *cluster;
	for (calo_clusters_it = calo_clusters.begin(); calo_clusters_it != calo_clusters.end(); calo_clusters_it++) {
		cluster = *calo_clusters_it;

		if (cluster->m_sector == 0) {
			hBDXMiniClusterEtotTOP->Fill(cluster->E);
			hBDXMiniClusterEseedTOP->Fill(cluster->Eseed);
			hBDXMiniClusterNTOP->Fill(cluster->Nhits);
		} else if (cluster->m_sector == 1) {
			hBDXMiniClusterEtotBOT->Fill(cluster->E);
			hBDXMiniClusterEseedBOT->Fill(cluster->Eseed);
			hBDXMiniClusterNBOT->Fill(cluster->Nhits);
		}
	}
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_BDXMiniClusters::erun(void) {
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	if (m_ROOTOutput != 0) {
		jout << "JEventProcessor_BDXMiniClusters adding histos to m_ROOTOutput" << endl;

		m_ROOTOutput->AddObject(hBDXMiniClusterEtotTOP);
		m_ROOTOutput->AddObject(hBDXMiniClusterEtotBOT);
		m_ROOTOutput->AddObject(hBDXMiniClusterEseedTOP);
		m_ROOTOutput->AddObject(hBDXMiniClusterEseedBOT);
		m_ROOTOutput->AddObject(hBDXMiniClusterNTOP);
		m_ROOTOutput->AddObject(hBDXMiniClusterNBOT);

	}
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_BDXMiniClusters::fini(void) {
	// Called before program exit after event processing is finished.
	return NOERROR;
}

