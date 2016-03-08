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

	for (it=m_ExtVetoMCHits.begin();it!=m_ExtVetoMCHits.end();it++){
		m_ExtVetoMCHit = (*it);
		m_ExtVetoDigiHit=new ExtVetoDigiHit;
		m_ExtVetoDigiHit->m_channel.sector=m_ExtVetoMCHit->sector;
		m_ExtVetoDigiHit->m_channel.layer=0;
		m_ExtVetoDigiHit->m_channel.readout=0;  //this is an active-volume object
		/*Here comes the annoying routine that, given the geometry Marco used in the MC, in particular the indexing scheme,
		 * returns it to the scheme I am using in the reconstruction. This was chacked by Andrea and Luca*/

		switch (m_ExtVetoMCHit->channel){
		case(1):
							m_ExtVetoDigiHit->m_channel.component=7;
		break;
		case(2):
							m_ExtVetoDigiHit->m_channel.component=6;
		break;
		case(3):
							m_ExtVetoDigiHit->m_channel.component=8;
		break;
		case(4):
							m_ExtVetoDigiHit->m_channel.component=9;
		break;
		case(5):
							m_ExtVetoDigiHit->m_channel.component=11;
		break;
		case(6):
							m_ExtVetoDigiHit->m_channel.component=10;
		break;
		case(7):
							m_ExtVetoDigiHit->m_channel.component=2;
		break;
		case(8):
							m_ExtVetoDigiHit->m_channel.component=1;
		break;
		case(9):
							m_ExtVetoDigiHit->m_channel.component=0;
		break;
		case(10):
							m_ExtVetoDigiHit->m_channel.component=3;
		break;
		case(11):
							m_ExtVetoDigiHit->m_channel.component=4;
		break;
		case(12):
							m_ExtVetoDigiHit->m_channel.component=5;
		break;
		default:
					break;
		}
		m_ExtVetoDigiHit->Q=m_ExtVetoMCHit->adc;
		m_ExtVetoDigiHit->T=m_ExtVetoMCHit->tdc;
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

