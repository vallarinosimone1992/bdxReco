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
#include <Calorimeter/CalorimeterDigiHit.h>

#include <TT/TranslationTable.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterDigiHit_factory_MC::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	CalorimeterDigiHit *m_CalorimeterDigiHit=0;
	CalorimeterDigiHit::CalorimeterSiPMDigiHit digi_hit;


	const CalorimeterMCHit *m_CalorimeterMCHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const CalorimeterMCHit*> m_CalorimeterMCHits;
	vector <const CalorimeterMCHit*>::const_iterator it;

	//1b: retrieve CalorimeterSiPMHit objects
	loop->Get(m_CalorimeterMCHits);
	m_map.clear();
	for (it=m_CalorimeterMCHits.begin();it!=m_CalorimeterMCHits.end();it++){
		TranslationTable::CALO_Index_t index;
		m_CalorimeterMCHit=*it;
		index.sector=m_CalorimeterMCHit->sector-1;
		index.x=m_CalorimeterMCHit->x-1;
		index.y=m_CalorimeterMCHit->y-1;
		index.readout=0;
		m_map_it=m_map.find(index);
		if (m_map_it==m_map.end()){
			m_CalorimeterDigiHit=new CalorimeterDigiHit;
			m_CalorimeterDigiHit->m_channel=index;
			m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterMCHit);
			digi_hit.readout=1; ///THIS IS CORRECT ---> in MC "right" is the first MPPC, i.e. readout=1
			digi_hit.Q=m_CalorimeterMCHit->adcr;
			digi_hit.T=m_CalorimeterMCHit->tdcr*4;   //MC is in 4*ns
			digi_hit.good_ped_RMS=true;/*by default!*/
			m_CalorimeterDigiHit->m_data.push_back(digi_hit);

			digi_hit.readout=2;  ///THIS IS CORRECT ---> in MC "left" is the second MPPC, i.e. readout=2
			digi_hit.Q=m_CalorimeterMCHit->adcl;
			digi_hit.T=m_CalorimeterMCHit->tdcl*4;
			digi_hit.good_ped_RMS=true; /*by default!*/
			m_CalorimeterDigiHit->m_data.push_back(digi_hit);
			m_map[index]=m_CalorimeterDigiHit;
		}
		else{
			m_CalorimeterDigiHit=m_map_it->second;
			m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterMCHit);
			m_CalorimeterDigiHit->m_data[0].Q+=m_CalorimeterMCHit->adcr;
			m_CalorimeterDigiHit->m_data[1].Q+=m_CalorimeterMCHit->adcl;
		}
	}
	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		m_CalorimeterDigiHit=m_map_it->second;
		_data.push_back(m_CalorimeterDigiHit);
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterDigiHit_factory_MC::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterDigiHit_factory_MC::fini(void)
{
	return NOERROR;
}

