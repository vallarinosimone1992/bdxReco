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

	// Code to generate factory data goes here. Add it like:
	//
	// IntVetoDigiHit *myIntVetoDigiHit = new IntVetoDigiHit;
	// myIntVetoDigiHit->x = x;
	// myIntVetoDigiHit->y = y;
	// ...
	// _data.push_back(myIntVetoDigiHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.


	IntVetoDigiHit *m_IntVetoDigiHit=0;
	IntVetoDigiHit::IntVetoSiPMDigiHit digi_hit;


	const IntVetoMCHit *m_IntVetoMCHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoMCHit*> m_IntVetoMCHits;
	vector <const IntVetoMCHit*>::const_iterator it;

	//1b: retrieve IntVetoSiPMHit objects
	loop->Get(m_IntVetoMCHits);

	for (it=m_IntVetoMCHits.begin();it!=m_IntVetoMCHits.end();it++){
		m_IntVetoMCHit = (*it);
		m_IntVetoDigiHit=new IntVetoDigiHit;
		m_IntVetoDigiHit->m_channel.sector=m_IntVetoMCHit->sector;
		m_IntVetoDigiHit->m_channel.layer=0;
		m_IntVetoDigiHit->m_channel.readout=0;  //this is an active-volume object

		switch (m_IntVetoMCHit->channel){
		case(1): //top
				break;
		case(2): //bottom
				break;
		case(3): //upstream
				break;
		case(4): //downstream
				break;
		case(5): //right
				break;
		case(6): //left
				break;
		default:
			break;
		}





/*
		digi_hit.readout=1;  ///TODO: ask Marco
		digi_hit.Q=m_IntVetoMCHit->adc1;
		digi_hit.T=m_IntVetoMCHit->tdc1;
		m_IntVetoDigiHit->m_data.push_back(digi_hit);
*/




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

