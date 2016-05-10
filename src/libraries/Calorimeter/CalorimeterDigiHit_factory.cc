// $Id$
//
//    File: CalorimeterDigiHit_factory.cc
// Created: Wed Jan 27 16:39:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;


//objects we need from the framework
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterSiPMHit.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterDigiHit.h>

#include "CalorimeterDigiHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterDigiHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{



	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterDigiHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	TranslationTable::CALO_Index_t m_channel;
	//	TranslationTable::csc_t		  m_csc;
	CalorimeterDigiHit *m_CalorimeterDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const CalorimeterSiPMHit*> m_CalorimeterSiPMHit;
	vector <const CalorimeterSiPMHit*>::const_iterator it;

	//1b: retrieve CalorimeterSiPMHit objects
	loop->Get(m_CalorimeterSiPMHit);


	/*Do the matching
	 *Proceed in this way:
	 * loop over the hits
	 * get the hit index, but put the readout to 0 (active detector element!)
	 * check if, in the map, a key with this detector index already exists.
	 * if not exist, add it, and create a new CalorimeterDigiHit
	 * if exist, get it, and add the SiPM hit to the list of sipm hits of the CalorimeterDigiHit
	 */
	m_map.clear();
	for (it=m_CalorimeterSiPMHit.begin(); it != m_CalorimeterSiPMHit.end() ; it++){
		m_channel = *((*it)->m_channel.calorimeter);
		m_channel.readout = 0;
		m_map_it=m_map.find(m_channel);
		if (m_map_it == m_map.end()){ //not here. Create a new CalorimeterDigiHit object, and associate the id of this SiPM hit with it
			m_CalorimeterDigiHit=new CalorimeterDigiHit;
			m_CalorimeterDigiHit->timestamp=(*it)->timestamp;
			m_CalorimeterDigiHit->m_channel=m_channel;
			m_CalorimeterDigiHit->AddAssociatedObject((*it));
			m_map.insert(std::make_pair(m_channel,m_CalorimeterDigiHit));
		}
		else{ //element already exists. Get the VetoIntDigiHit and add this hit as id.
			m_CalorimeterDigiHit=m_map[m_channel];
			m_CalorimeterDigiHit->AddAssociatedObject((*it));
		}
	}

	/*Now the map is full of all the hits in different active elements of calorimeter, i.e. with different identifiers, BUT readout, that maps the sipm hits.
	 * Each hit has a reference to the SiPM hits that made it
	 */
	vector <const CalorimeterSiPMHit*> m_CalorimeterSiPMHit_tmp;
	CalorimeterDigiHit* m_CalorimeterDigiHit_tmp;
	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		//create here the hit!
		m_CalorimeterDigiHit_tmp=m_map_it->second;
		m_CalorimeterDigiHit_tmp->Get(m_CalorimeterSiPMHit_tmp,"",0);  //0 means "associated only with this object"
		for (int ihit=0;ihit<m_CalorimeterSiPMHit_tmp.size();ihit++){

			CalorimeterDigiHit::CalorimeterSiPMDigiHit hit;
			hit.Q=m_CalorimeterSiPMHit_tmp[ihit]->Qphe;
			hit.T=m_CalorimeterSiPMHit_tmp[ihit]->T;
			hit.readout=m_CalorimeterSiPMHit_tmp[ihit]->m_channel.calorimeter->readout;
			hit.good_ped_RMS=m_CalorimeterSiPMHit_tmp[ihit]->good_ped_RMS;
			hit.type=m_CalorimeterSiPMHit_tmp[ihit]->m_type;

			m_CalorimeterDigiHit_tmp->m_data.push_back(hit);
		}
		_data.push_back(m_CalorimeterDigiHit_tmp); //publish it
	}

	return NOERROR;
}



//------------------
// erun
//------------------
jerror_t CalorimeterDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterDigiHit_factory::fini(void)
{
	return NOERROR;
}

