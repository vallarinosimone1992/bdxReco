// $Id$
//
//    File: IntVetoDigiHit_factory_MC.cc
// Created: Mon Mar  7 12:20:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "IntVetoDigiHit_factory_MC.h"

#include <MC/IntVetoMCHit.h>
#include <MC/MCType.h>
#include <IntVeto/IntVetoDigiHit.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t IntVetoDigiHit_factory_MC::init(void) {
	gPARMS->GetParameter("MC", m_isMC);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t IntVetoDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber) {
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber) {

	IntVetoDigiHit *m_IntVetoDigiHit = 0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector<const IntVetoMCHit*> m_IntVetoMCHits;
	vector<const IntVetoMCHit*>::const_iterator it;
	const IntVetoMCHit *m_IntVetoMCHit = 0;

	//1b: retrieve IntVetoSiPMHit objects
	loop->Get(m_IntVetoMCHits);

	m_map.clear();
	for (it = m_IntVetoMCHits.begin(); it != m_IntVetoMCHits.end(); it++) {
		m_IntVetoMCHit = (*it);
		m_channel.sector = m_IntVetoMCHit->sector;
		m_channel.layer = 0;
		if (m_isMC == MCType::CATANIA_V1) {
			m_channel.component = this->getCataniaV1Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::CATANIA_V2) {
			m_channel.component = this->getCataniaV2Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::FULL_V1) {
			m_channel.component = this->getFullV1Component(m_IntVetoMCHit->channel);
		} else if (m_isMC == MCType::FULL_V2) {
			m_channel.component = this->getFullV2Component(m_IntVetoMCHit->channel);
		}
		m_channel.readout = 0;
		m_map_it = m_map.find(m_channel);
		if (m_map_it == m_map.end()) { /*IntVetoDigiHit was not here. Create a new hit*/

			/*create the different digi-hits*/
			switch (m_channel.component) {
			case (0): //top
			{
				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 1;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 2;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 3;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 4;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
				break;
			}
			case (1): //left
			case (2): //right
			{
				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 1;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 2;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 3;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 4;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
				break;
			}
			case (3): {
				if (m_isMC == MCType::CATANIA_V1) {
					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 1;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 2;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 3;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 4;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

				} else {
					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 1;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 2;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc2 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 3;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc3 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/

					m_IntVetoDigiHit = new IntVetoDigiHit;
					m_IntVetoDigiHit->m_channel = m_channel;
					m_IntVetoDigiHit->m_channel.readout = 4;
					m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc4 / 1000.; //MC is in ps
					m_IntVetoDigiHit->A = 0;
					m_IntVetoDigiHit->RMSflag = true;
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
				}
				break;
			}
			case (4): //upstream
			case (5): //downstream
			{
				m_IntVetoDigiHit = new IntVetoDigiHit;
				m_IntVetoDigiHit->m_channel = m_channel;
				m_IntVetoDigiHit->m_channel.readout = 1;
				m_IntVetoDigiHit->Qphe = m_IntVetoMCHit->adc1;
				m_IntVetoDigiHit->T = m_IntVetoMCHit->tdc1 / 1000.; //MC is in ps
				m_IntVetoDigiHit->A = 0;
				m_IntVetoDigiHit->RMSflag = true;
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_map[m_IntVetoDigiHit->m_channel].push_back(m_IntVetoDigiHit); /*Add to the map*/
				break;
			}
			}
		} else { /*There is already the hit. Sum the charges*/

			switch (m_channel.component) {
			case (0): //top

			{
				m_IntVetoDigiHit = m_map_it->second[0];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
				m_IntVetoDigiHit = m_map_it->second[1];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
				m_IntVetoDigiHit = m_map_it->second[2];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
				m_IntVetoDigiHit = m_map_it->second[3];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
				break;
			}
			case (1): //left
			case (2): //right
			{
				m_IntVetoDigiHit = m_map_it->second[0];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
				m_IntVetoDigiHit = m_map_it->second[1];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
				m_IntVetoDigiHit = m_map_it->second[2];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
				m_IntVetoDigiHit = m_map_it->second[3];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
				break;
			}
			case (3): //bottom
			{
				if (m_isMC == MCType::CATANIA_V1) {
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit = m_map_it->second[1];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
					m_IntVetoDigiHit = m_map_it->second[2];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit = m_map_it->second[3];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
					break;
				} else {
					m_IntVetoDigiHit = m_map_it->second[0];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
					m_IntVetoDigiHit = m_map_it->second[1];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc2;
					m_IntVetoDigiHit = m_map_it->second[2];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc3;
					m_IntVetoDigiHit = m_map_it->second[3];
					m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
					m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc4;
					break;
				}
			}
			case (4): //upstream
			case (5): //downstream
			{
				m_IntVetoDigiHit = m_map_it->second[0];
				m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
				m_IntVetoDigiHit->Qphe += m_IntVetoMCHit->adc1;
			}
			}
		}
	} //end loop on MC hits
	for (m_map_it = m_map.begin(); m_map_it != m_map.end(); m_map_it++) {
		for (int idigi = 0; idigi < m_map_it->second.size(); idigi++) {
			m_IntVetoDigiHit = m_map_it->second[idigi];
			_data.push_back(m_IntVetoDigiHit);
		}
	}
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t IntVetoDigiHit_factory_MC::erun(void) {
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t IntVetoDigiHit_factory_MC::fini(void) {
	return NOERROR;
}

int IntVetoDigiHit_factory_MC::getCataniaV1Component(int MCchannel) {
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
	case (5): //right
		component = 2;
		break;
	case (6): //left
		component = 1;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getCataniaV2Component(int MCchannel) {
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
		/* A.C. 10/8/2017.
		 * THIS IS CORRECT, in the prototype, looking from the amplifiers's side, the IV plate on the right is the LEFT one, with component n.1.
		 * In the simulations, looking from the amplifiers's side, the IV plate on the right is the RIGHT one, with ch=5. It has to have component n.1
		 * And vice-versa for the other side.
		 */
	case (5): //right
		component = 1;
		break;
	case (6): //left
		component = 2;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getFullV1Component(int MCchannel) {
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
	case (5): //right
		component = 2;
		break;
	case (6): //left
		component = 1;
		break;
	}
	return component;
}

int IntVetoDigiHit_factory_MC::getFullV2Component(int MCchannel) {

	//A.C. to be checked
	int component = -1;
	switch (MCchannel) {
	case (1): //top
		component = 0;
		break;
	case (2): //bottom
		component = 3;
		break;
	case (3):  //upstream
		component = 4;
		break;
	case (4): //downstream
		component = 5;
		break;
	case (5): //right
		component = 2;
		break;
	case (6): //left
		component = 1;
		break;
	}
	return component;
}

