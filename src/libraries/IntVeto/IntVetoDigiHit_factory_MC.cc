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
#include <IntVeto/IntVetoDigiHit.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t IntVetoDigiHit_factory_MC::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t IntVetoDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	IntVetoDigiHit *m_IntVetoDigiHit=0;
	IntVetoDigiHit::IntVetoSiPMDigiHit digi_hit;


	const IntVetoMCHit *m_IntVetoMCHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoMCHit*> m_IntVetoMCHits;
	vector <const IntVetoMCHit*>::const_iterator it;

	//1b: retrieve IntVetoSiPMHit objects
	loop->Get(m_IntVetoMCHits);

	m_map.clear();
	for (it=m_IntVetoMCHits.begin();it!=m_IntVetoMCHits.end();it++){
		m_IntVetoMCHit = (*it);
		m_map_it=m_map.find(std::make_pair(m_IntVetoMCHit->sector,m_IntVetoMCHit->channel));
		if (m_map_it==m_map.end()){ /*IntVetoDigiHit was not here. Create a new hit*/
			m_IntVetoDigiHit=new IntVetoDigiHit;
			m_IntVetoDigiHit->m_channel.sector=m_IntVetoMCHit->sector;
			m_IntVetoDigiHit->m_channel.layer=0;
			m_IntVetoDigiHit->m_channel.readout=0;  //this is an active-volume object

			m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);

			if (m_IntVetoMCHit->system==VetoMCHit::CATANIA_INTVETO){
				m_IntVetoDigiHit->m_channel.component=this->getCataniaComponent(m_IntVetoMCHit->channel);
			}
			else{
				m_IntVetoDigiHit->m_channel.component=m_IntVetoMCHit->channel; /*Keep this as in MC. There's no data to compare with!*/
			}
			/*create the component sipm-hits*/
			switch (m_IntVetoMCHit->channel){
			case(1):
			case(5):
			case(6):
			case(2): //bottom     ///The inversion 2 <-> 4 is only in the data
			digi_hit.readout=1;
			digi_hit.Q=m_IntVetoMCHit->adc1;
			digi_hit.T=m_IntVetoMCHit->tdc1/1000.;  //MC is in ps
			m_IntVetoDigiHit->m_data.push_back(digi_hit);
			digi_hit.readout=2;
			digi_hit.Q=m_IntVetoMCHit->adc2;
			digi_hit.T=m_IntVetoMCHit->tdc2/1000.;
			m_IntVetoDigiHit->m_data.push_back(digi_hit);
			digi_hit.readout=3;
			digi_hit.Q=m_IntVetoMCHit->adc3;
			digi_hit.T=m_IntVetoMCHit->tdc3/1000.;
			m_IntVetoDigiHit->m_data.push_back(digi_hit);
			digi_hit.readout=4;
			digi_hit.Q=m_IntVetoMCHit->adc4;
			digi_hit.T=m_IntVetoMCHit->tdc4/1000.;
			m_IntVetoDigiHit->m_data.push_back(digi_hit);
			break;

			case(3): //upstream
			case(4): //downstream
			digi_hit.readout=1;
			digi_hit.Q=m_IntVetoMCHit->adc1;
			digi_hit.T=m_IntVetoMCHit->tdc1/1000.;
			m_IntVetoDigiHit->m_data.push_back(digi_hit);
			break;

			}
			m_map[std::make_pair(m_IntVetoMCHit->sector,m_IntVetoMCHit->channel)]=m_IntVetoDigiHit; /*Add to the map*/
		}
		else{ /*There is already the hit. Sum the charges*/
			m_IntVetoDigiHit=m_map_it->second;
			m_IntVetoDigiHit->AddAssociatedObject(m_IntVetoMCHit);
			switch (m_IntVetoMCHit->channel){
			case(1):
			case(5):
			case(6):
			case(2): //bottom     ///The inversion is only in the data
			m_IntVetoDigiHit->m_data[0].Q+=m_IntVetoMCHit->adc1;
			m_IntVetoDigiHit->m_data[1].Q+=m_IntVetoMCHit->adc2;
			m_IntVetoDigiHit->m_data[2].Q+=m_IntVetoMCHit->adc3;
			m_IntVetoDigiHit->m_data[3].Q+=m_IntVetoMCHit->adc4;
			break;

			case(3): //upstream
			case(4): //upstream
			m_IntVetoDigiHit->m_data[0].Q+=m_IntVetoMCHit->adc1;
			break;
			}
		}
	}/*End loop on MC hits*/

	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		m_IntVetoDigiHit=m_map_it->second;
		m_IntVetoDigiHit->Qtot=0;
		for (int ii=0;ii<m_IntVetoDigiHit->m_data.size();ii++){
			m_IntVetoDigiHit->Qtot+=m_IntVetoDigiHit->m_data[ii].Q;
		}
		_data.push_back(m_IntVetoDigiHit);
	}
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t IntVetoDigiHit_factory_MC::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t IntVetoDigiHit_factory_MC::fini(void)
{
	return NOERROR;
}


int IntVetoDigiHit_factory_MC::getCataniaComponent(int MCchannel){
	int component=-1;
	switch (MCchannel){
	case(1): //top
									component=0;break;
	case(2): //bottom
									component=3;break;
	case(3):  //upstream
									component=4;break;
	case(4): //downstream
									component=5;break;
	case(5): //right
									component=2;break;
	case(6): //left
									component=1;break;
	}
	return component;
}

