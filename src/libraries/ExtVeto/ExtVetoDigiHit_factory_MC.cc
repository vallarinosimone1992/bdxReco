// $Id$
//
//    File: ExtVetoDigiHit_factory_MC.cc
// Created: Tue Mar  8 12:19:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;


#include <MC/ExtVetoMCHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>


#include "ExtVetoDigiHit_factory_MC.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoDigiHit_factory_MC::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	ExtVetoDigiHit *m_ExtVetoDigiHit=0;


	const ExtVetoMCHit *m_ExtVetoMCHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const ExtVetoMCHit*> m_ExtVetoMCHits;
	vector <const ExtVetoMCHit*>::const_iterator it;

	//1b: retrieve ExtVetoSiPMHit objects
	loop->Get(m_ExtVetoMCHits);

	m_map.clear();
	for (it=m_ExtVetoMCHits.begin();it!=m_ExtVetoMCHits.end();it++){
		m_ExtVetoMCHit = (*it);
		m_map_it=m_map.find(std::make_pair(m_ExtVetoMCHit->sector,m_ExtVetoMCHit->channel));
		if (m_map_it==m_map.end()){ //ExtVetoDigiHit was not created yet
			m_ExtVetoDigiHit=new ExtVetoDigiHit;
			m_ExtVetoDigiHit->AddAssociatedObject(m_ExtVetoMCHit);
			m_ExtVetoDigiHit->m_channel.sector=m_ExtVetoMCHit->sector;
			m_ExtVetoDigiHit->m_channel.layer=0;
			m_ExtVetoDigiHit->m_channel.readout=0;  //this is an active-volume object
			if (m_ExtVetoMCHit->system==VetoMCHit::CATANIA_EXTVETO){
				m_ExtVetoDigiHit->m_channel.component=this->getCataniaComponent(m_ExtVetoMCHit->channel);
			}
			else{
				m_ExtVetoDigiHit->m_channel.component=m_ExtVetoMCHit->channel; /*Keep this as in MC. There's no data to compare with!*/
			}


			m_ExtVetoDigiHit->Q=m_ExtVetoMCHit->adc;
			m_ExtVetoDigiHit->T=m_ExtVetoMCHit->tdc/1000.;  //MC is in ps
			m_map[std::make_pair(m_ExtVetoMCHit->sector,m_ExtVetoMCHit->channel)]=m_ExtVetoDigiHit;
		}
		else{ /*object already there*/
			m_ExtVetoDigiHit=m_map_it->second;
			m_ExtVetoDigiHit->Q+=m_ExtVetoMCHit->adc;
			m_ExtVetoDigiHit->AddAssociatedObject(m_ExtVetoMCHit);
			//m_ExtVetoDigiHit->T=m_ExtVetoMCHit->tdc;
		}
	}

	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		m_ExtVetoDigiHit=m_map_it->second;
		_data.push_back(m_ExtVetoDigiHit);
	}



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoDigiHit_factory_MC::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoDigiHit_factory_MC::fini(void)
{
	return NOERROR;
}

int ExtVetoDigiHit_factory_MC::getCataniaComponent(int channel){
	int component;
	switch (channel){
	case(1):
														component=7;
	break;
	case(2):
														component=6;
	break;
	case(3):
														component=8;
	break;
	case(4):
														component=9;
	break;
	case(5):
														component=11;
	break;
	case(6):
														component=10;
	break;
	case(7):
														component=2;
	break;
	case(8):
														component=1;
	break;
	case(9):
														component=0;
	break;
	case(10):
														component=3;
	break;
	case(11):
														component=4;
	break;
	case(12):
														component=5;
	break;
	default:
		break;
	}
	return component;
}
