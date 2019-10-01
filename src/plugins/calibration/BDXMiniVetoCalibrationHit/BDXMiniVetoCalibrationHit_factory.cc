// $Id$
//
//    File: BDXMiniVetoCalibrationHit_factory.cc
// Created: Fri May 17 11:56:57 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "BDXMiniVetoCalibrationHit_factory.h"
#include "JFactoryGenerator_BDXMiniVetoCalibrationHit.h"

#include "Calorimeter/CalorimeterDigiHit.h"
#include "Calorimeter/CalorimeterHit.h"
#include "IntVeto/IntVetoDigiHit.h"
#include <DAQ/eventData.h>
#include <BDXmini/triggerDataBDXmini.h>

using namespace jana;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
extern "C" {
void InitPlugin(JApplication *app) {
	InitJANAPlugin(app);
	app->AddFactoryGenerator(new JFactoryGenerator_BDXMiniVetoCalibrationHit());
}
} // "C"

//------------------
// init
//------------------
jerror_t BDXMiniVetoCalibrationHit_factory::init(void) {
	m_isMC = 0;
	gPARMS->GetParameter("MC", m_isMC);

	m_thrIntVetoCaps = 5;
	gPARMS->SetDefaultParameter("BDXMiniVetoCalibration:THR_CAPS", m_thrIntVetoCaps, "Threshold in phe (charge) for each of the caps");

	m_thrIntVetoL = 5;
	gPARMS->SetDefaultParameter("BDXMiniVetoCalibration:THR_1L", m_thrIntVetoL, "Threshold in phe (charge) for each of the lateral counters");

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t BDXMiniVetoCalibrationHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t BDXMiniVetoCalibrationHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber) {

	const IntVetoDigiHit* m_IntVetoDigiHit;
	vector<const IntVetoDigiHit*> intveto_hits;
	vector<const IntVetoDigiHit*>::const_iterator intveto_hits_it;

	const eventData* eData;
	const triggerDataBDXmini* tData;

	int nTopCaps = 0;
	int nBottomCaps = 0;

	int maxComponent0L = -1;
	int maxComponent1L = -1;

	double maxComponentQ0L = -999;
	double maxComponentQ1L = -999;

	BDXMiniVetoCalibrationHit *m_VetoCalibHit = 0;

	/*Get objects from JANA factories*/
	if (m_isMC) {
		loop->Get(intveto_hits, "MC");
	} else {
		loop->Get(intveto_hits);
	}

	/*The following happens for EPICS events*/
	if (!m_isMC) {
		try {
			loop->GetSingle(tData);
		} catch (unsigned long e) {
			return OBJECT_NOT_AVAILABLE;
		}

		try {
			loop->GetSingle(eData);
		} catch (unsigned long e) {
			return OBJECT_NOT_AVAILABLE;
		}
	}

	/*Here goes the code to create the objects*/
	for (intveto_hits_it = intveto_hits.begin(); intveto_hits_it != intveto_hits.end(); intveto_hits_it++) {
		m_IntVetoDigiHit = (*intveto_hits_it);

		//A.C. skip SIPM in csc (0,1,2), i.e. Sector0 Layer0 Component3
		if ((m_IntVetoDigiHit->m_channel.layer == 0) && (m_IntVetoDigiHit->m_channel.component == 3)) continue;

		if (m_IntVetoDigiHit->m_channel.component == 10) { //TOP
			if (m_IntVetoDigiHit->Qphe > m_thrIntVetoCaps) {
				nTopCaps++;
			}
		} else if (m_IntVetoDigiHit->m_channel.component == 9) { //BOTTOM
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

	/*Select event with TOP (or BOTTOM) caps, without the BOTTOM (or TOP) caps*/
	/*The idea is to select muons entering from top and exiting from lateral side OR
	 *                      muons entering from lateral and exiting from bottom
	 */
	if ((nTopCaps != 2) && (nBottomCaps != 2)) {
		return OBJECT_NOT_AVAILABLE;
	}
	if ((nTopCaps == 2) && (nBottomCaps != 0)) {
		return OBJECT_NOT_AVAILABLE;
	}
	if ((nBottomCaps == 2) && (nTopCaps != 0)) {
		return OBJECT_NOT_AVAILABLE;
	}

	int sel1, sel2;

	for (intveto_hits_it = intveto_hits.begin(); intveto_hits_it != intveto_hits.end(); intveto_hits_it++) {
		m_IntVetoDigiHit = (*intveto_hits_it);
		if ((m_IntVetoDigiHit->m_channel.layer == 0) && (m_IntVetoDigiHit->m_channel.component == 3)) continue;

		m_VetoCalibHit = new BDXMiniVetoCalibrationHit();
		m_VetoCalibHit->match = 0;
		m_VetoCalibHit->m_channel = m_IntVetoDigiHit->m_channel;
		if (nTopCaps == 2) m_VetoCalibHit->m_channel.sector = 0;
		else
			m_VetoCalibHit->m_channel.sector = 1;

		if ((m_VetoCalibHit->m_channel.layer == 0) && (m_VetoCalibHit->m_channel.component <= 8)) { //OV
			if (maxComponent1L == -1) {
				delete m_VetoCalibHit;
				m_VetoCalibHit = 0;
				continue;
			} else {
				m_VetoCalibHit->match = maxComponent1L;
			}
		} else if ((m_VetoCalibHit->m_channel.layer == 1) && (m_VetoCalibHit->m_channel.component <= 8)) {  //IV
			if (maxComponent0L == -1) {
				delete m_VetoCalibHit;
				m_VetoCalibHit = 0;
				continue;
			} else {
				m_VetoCalibHit->match = maxComponent0L;
			}
		}



		m_VetoCalibHit->Qraw = m_IntVetoDigiHit->Qraw;
		m_VetoCalibHit->Qphe = m_IntVetoDigiHit->Qphe;
		m_VetoCalibHit->T = m_IntVetoDigiHit->T;
		m_VetoCalibHit->A = m_IntVetoDigiHit->A;

		_data.push_back(m_VetoCalibHit);
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t BDXMiniVetoCalibrationHit_factory::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t BDXMiniVetoCalibrationHit_factory::fini(void) {
	return NOERROR;
}

