// $Id$
//
//    File: CalorimeterDigiHit_factory_MC.cc
// Created: Tue Mar  1 17:38:32 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterDigiHit_factory_MC.h"

#include <MC/CalorimeterMCHit.h>
#include <MC/MCType.h>
#include <Calorimeter/CalorimeterDigiHit.h>

#include <TT/TranslationTable.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterDigiHit_factory_MC::init(void) {
	gPARMS->GetParameter("MC", m_isMC);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber) {

	CalorimeterDigiHit *m_CalorimeterDigiHit = 0;
	const CalorimeterMCHit *m_CalorimeterMCHit = 0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const CalorimeterMCHit*> m_CalorimeterMCHits;
	vector<const CalorimeterMCHit*>::const_iterator it;

	//1b: retrieve CalorimeterMCHit objects
	loop->Get(m_CalorimeterMCHits);
	m_map.clear();
	for (it = m_CalorimeterMCHits.begin(); it != m_CalorimeterMCHits.end(); it++) {
		TranslationTable::CALO_Index_t index;
		m_CalorimeterMCHit = *it;

		this->SetIndex(index, m_CalorimeterMCHit, m_isMC);
		index.readout = 1;
		m_map_it = m_map.find(index);
		if (m_map_it == m_map.end()) {
			m_CalorimeterDigiHit = new CalorimeterDigiHit;
			m_CalorimeterDigiHit->m_channel = index;
			m_CalorimeterDigiHit->m_channel.readout = 1;	///THIS IS CORRECT ---> in MC "right" is the first MPPC, i.e. readout=1
			m_CalorimeterDigiHit->Q = m_CalorimeterMCHit->adcr;
			m_CalorimeterDigiHit->T = m_CalorimeterMCHit->tdcr * 4;
			m_CalorimeterDigiHit->RMSflag = true;
			m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterMCHit);
			m_map[index] = m_CalorimeterDigiHit;
		} else {
			m_CalorimeterDigiHit->Q += m_CalorimeterMCHit->adcr;
		}
		if (m_isMC == MCType::CATANIA_V1) { /*Only Catania_V1 simulations had two readouts in the crystal*/
			index.readout = 2;
			m_map_it = m_map.find(index);
			if (m_map_it == m_map.end()) {
				m_CalorimeterDigiHit = new CalorimeterDigiHit;
				m_CalorimeterDigiHit->m_channel = index;
				m_CalorimeterDigiHit->m_channel.readout = 2;	///THIS IS CORRECT ---> in MC "right" is the first MPPC, i.e. readout=1
				m_CalorimeterDigiHit->Q = m_CalorimeterMCHit->adcl;
				m_CalorimeterDigiHit->T = m_CalorimeterMCHit->tdcl * 4;
				m_CalorimeterDigiHit->RMSflag = true;
				m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterMCHit);
				m_map[index] = m_CalorimeterDigiHit;
			} else {
				m_CalorimeterDigiHit = m_map_it->second;
				m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterMCHit);
				m_CalorimeterDigiHit->Q += m_CalorimeterMCHit->adcl;
			}
		}
	}

	for (m_map_it = m_map.begin(); m_map_it != m_map.end(); m_map_it++) {
		m_CalorimeterDigiHit = m_map_it->second;
		_data.push_back(m_CalorimeterDigiHit);
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterDigiHit_factory_MC::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterDigiHit_factory_MC::fini(void) {
	return NOERROR;
}

void CalorimeterDigiHit_factory_MC::SetIndex(TranslationTable::CALO_Index_t &index, const CalorimeterMCHit *mchit, int MC) {
	if ((MC == MCType::CATANIA_V1) || (MC == MCType::FULL_V1)) {
		index.sector = mchit->sector - 1;
		index.x = mchit->x - 1;
		index.y = mchit->y - 1;
	} else if (MC == MCType::CATANIA_V2) {
		if (mchit->sector == 1) {
			index.sector = 0;
			index.x = mchit->y - 1;
			index.y = 4 - mchit->x;
		} else if (mchit->sector == 100) {
			index.sector = 1;
			index.y = 0;
			index.x = 0;
		}
	} else if (MC == MCType::FULL_V2) {
		//A.C. TO BE DONE
	}

	index.readout = 1;

}

