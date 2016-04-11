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

	for (it=m_CalorimeterMCHits.begin();it!=m_CalorimeterMCHits.end();it++){
		m_CalorimeterMCHit = (*it);
		m_CalorimeterDigiHit=new CalorimeterDigiHit;
		m_CalorimeterDigiHit->m_channel.sector=m_CalorimeterMCHit->sector;
		m_CalorimeterDigiHit->m_channel.x=m_CalorimeterMCHit->x;
		m_CalorimeterDigiHit->m_channel.y=m_CalorimeterMCHit->y;
		m_CalorimeterDigiHit->m_channel.readout=0;  //this is an active-volume object
		m_CalorimeterDigiHit->AddAssociatedObject(m_CalorimeterMCHit);

		digi_hit.readout=1;  ///TODO: ask Marco
		digi_hit.Q=m_CalorimeterMCHit->adcl;
		digi_hit.T=m_CalorimeterMCHit->tdcl;
		digi_hit.good_ped_RMS=true; /*by default!*/
		m_CalorimeterDigiHit->m_data.push_back(digi_hit);

		digi_hit.readout=2;  ///TODO: ask Marco
		digi_hit.Q=m_CalorimeterMCHit->adcr;
		digi_hit.T=m_CalorimeterMCHit->tdcr;
		digi_hit.good_ped_RMS=true;/*by default!*/
		m_CalorimeterDigiHit->m_data.push_back(digi_hit);


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

