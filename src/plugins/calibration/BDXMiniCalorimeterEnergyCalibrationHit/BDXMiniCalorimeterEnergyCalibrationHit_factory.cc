// $Id$
//
//    File: BDXMiniCalorimeterEnergyCalibrationHit_factory.cc
// Created: Wed Apr  3 17:08:28 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#include "BDXMiniCalorimeterEnergyCalibrationHit_factory.h"
#include "JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit.h"

#include "Calorimeter/CalorimeterDigiHit.h"
#include "Calorimeter/CalorimeterHit.h"
#include "IntVeto/IntVetoDigiHit.h"
#include <DAQ/eventData.h>

using namespace jana;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddFactoryGenerator(new JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit());
}
} // "C"

//------------------
// init
//------------------
jerror_t BDXMiniCalorimeterEnergyCalibrationHit_factory::init(void) {
	m_isMC = 0;
	gPARMS->GetParameter("MC", m_isMC);

	m_thrIntVetoCaps = 5;
	gPARMS->SetDefaultParameter("BDXMiniCalorimeterEnergyCalibration:THR_CAPS", m_thrIntVetoCaps, "Threshold in phe (charge) for each of the caps");

	m_thrIntVetoL = 10;
	gPARMS->SetDefaultParameter("BDXMiniCalorimeterEnergyCalibration:THR_1L", m_thrIntVetoL, "Threshold in phe (charge) for each of the caps");

	//The hard-coded correspondance
	corrTOP[1].push_back(make_pair(0, -1));
	corrTOP[1].push_back(make_pair(0, 0));

	corrTOP[2].push_back(make_pair(-2, -2));
	corrTOP[2].push_back(make_pair(-2, 0));
	corrTOP[2].push_back(make_pair(-1, -2));
	corrTOP[2].push_back(make_pair(0, -2));
	corrTOP[2].push_back(make_pair(-1, 0));

	corrTOP[3].push_back(make_pair(-2, 1));
	corrTOP[3].push_back(make_pair(-1, 0));

	corrTOP[4].push_back(make_pair(-2, 2));
	corrTOP[4].push_back(make_pair(-1, 2));
	corrTOP[4].push_back(make_pair(-1, 3));

	corrTOP[5].push_back(make_pair(0, 0));
	corrTOP[5].push_back(make_pair(-1, 1));
	corrTOP[5].push_back(make_pair(0, 1));

	corrTOP[6].push_back(make_pair(1, 3));
	corrTOP[6].push_back(make_pair(2, 2));
	corrTOP[6].push_back(make_pair(1, 2));

	corrTOP[7].push_back(make_pair(2, 1));
	corrTOP[7].push_back(make_pair(1, 1));
	corrTOP[7].push_back(make_pair(2, 0));
	corrTOP[7].push_back(make_pair(1, 0));

	corrTOP[8].push_back(make_pair(2, -2));
	corrTOP[8].push_back(make_pair(1, -2));
	corrTOP[8].push_back(make_pair(0, -2));
	corrTOP[8].push_back(make_pair(1, 0));

	corrBOTTOM = corrTOP;

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t BDXMiniCalorimeterEnergyCalibrationHit_factory::brun(jana::JEventLoop *eventLoop, uint32_t runnumber) {
	jout<<"JEventProcessor_BDXMiniCalorimeterEnergyCalibrationHit::brun called"<<endl;
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t BDXMiniCalorimeterEnergyCalibrationHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber) {

	const CalorimeterHit* m_CaloHit;
	vector<const CalorimeterHit*> calo_hits;
	vector<const CalorimeterHit*>::const_iterator calo_hits_it;

	const IntVetoDigiHit* m_IntVetoDigiHit;
	vector<const IntVetoDigiHit*> intveto_hits;
	vector<const IntVetoDigiHit*>::const_iterator intveto_hits_it;

	int nTopCaps = 0;
	int nBottomCaps = 0;
	int iX, iY, sector;
	int maxComponent0L = -1;
	int maxComponent1L = -1;

	double maxComponentQ0L = -999;
	double maxComponentQ1L = -999;

	BDXMiniCalorimeterEnergyCalibrationHit *m_CalorimeterCalibHit = 0;

	/*Get objects from JANA factories*/
	if (m_isMC) {
		loop->Get(intveto_hits, "MC");
	} else {
		loop->Get(intveto_hits);
	}

	loop->Get(calo_hits);
	const eventData* tData;

	if (!m_isMC) {
		try {
			loop->GetSingle(tData);
		} catch (unsigned long e) {
			jout << "JEventProcessor_Trigger::evnt no eventData bank this event" << std::endl;
			return OBJECT_NOT_AVAILABLE;
		}
	}

	/*Here goes the code to create the objects*/

	for (intveto_hits_it = intveto_hits.begin(); intveto_hits_it != intveto_hits.end(); intveto_hits_it++) {
		m_IntVetoDigiHit = (*intveto_hits_it);
		if (m_IntVetoDigiHit->m_channel.component == 10) { //TOP
			if (m_IntVetoDigiHit->Qphe > m_thrIntVetoCaps) {
				nTopCaps++;
			}
		}
		if (m_IntVetoDigiHit->m_channel.component == 9) { //BOTTOM
			if (m_IntVetoDigiHit->Qphe > m_thrIntVetoCaps) {
				nBottomCaps++;
			}
		} else if (m_IntVetoDigiHit->m_channel.component <= 8) {
			if (m_IntVetoDigiHit->m_channel.layer == 0) {
				if ((m_IntVetoDigiHit->Qphe > m_thrIntVetoL) && (m_IntVetoDigiHit->Qphe > maxComponentQ0L)) {
					maxComponent0L = m_IntVetoDigiHit->m_channel.component;
					maxComponentQ0L = m_IntVetoDigiHit->Qphe;
				}
			} else if (m_IntVetoDigiHit->m_channel.layer == 1) {
				if ((m_IntVetoDigiHit->Qphe > m_thrIntVetoL) && (m_IntVetoDigiHit->Qphe > maxComponentQ1L)) {
					maxComponent1L = m_IntVetoDigiHit->m_channel.component;
					maxComponentQ1L = m_IntVetoDigiHit->Qphe;
				}
			}
		}
	}
	if ((nTopCaps < 2) && (nBottomCaps < 2)) {
		return OBJECT_NOT_AVAILABLE;
	}
	if (maxComponent1L == -1) {
		return OBJECT_NOT_AVAILABLE;
	}

	for (calo_hits_it = calo_hits.begin(); calo_hits_it != calo_hits.end(); calo_hits_it++) {
		m_CaloHit = *calo_hits_it;
		iX = m_CaloHit->m_channel.x;
		iY = m_CaloHit->m_channel.y;
		sector = m_CaloHit->m_channel.sector;

		if (!m_isMC) {
			if (m_CaloHit->Eraw < 5) continue;
		} else {
			if (m_CaloHit->Eraw / 1000. < 1) continue;
		}
		if ((sector == 0) && (nTopCaps >= 2) && (std::find(corrTOP[maxComponent1L].begin(), corrTOP[maxComponent1L].end(), std::make_pair(iX, iY)) != corrTOP[maxComponent1L].end())) {
			m_CalorimeterCalibHit = new BDXMiniCalorimeterEnergyCalibrationHit();
			m_CalorimeterCalibHit->m_channel = m_CaloHit->m_channel;
			m_CalorimeterCalibHit->Q = m_CaloHit->Eraw;
			m_CalorimeterCalibHit->E = m_CaloHit->E;

			if (m_isMC) {
				m_CalorimeterCalibHit->Q /= 1E3;
			}
			m_CalorimeterCalibHit->A = m_CaloHit->A;

			_data.push_back(m_CalorimeterCalibHit);
		} else if ((sector == 1) && (nBottomCaps >= 2) && (std::find(corrBOTTOM[maxComponent1L].begin(), corrBOTTOM[maxComponent1L].end(), std::make_pair(iX, iY)) != corrBOTTOM[maxComponent1L].end())) {
			m_CalorimeterCalibHit = new BDXMiniCalorimeterEnergyCalibrationHit();
			m_CalorimeterCalibHit->m_channel = m_CaloHit->m_channel;
			m_CalorimeterCalibHit->Q = m_CaloHit->Eraw;
			m_CalorimeterCalibHit->E = m_CaloHit->E;

			if (m_isMC) {
				m_CalorimeterCalibHit->Q /= 1E3;
			}
			m_CalorimeterCalibHit->A = m_CaloHit->A;

			_data.push_back(m_CalorimeterCalibHit);
		}
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t BDXMiniCalorimeterEnergyCalibrationHit_factory::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t BDXMiniCalorimeterEnergyCalibrationHit_factory::fini(void) {

	return NOERROR;
}

