// $Id$
//
//    File: IntVetoHit_factory.cc
// Created: Wed Jan 13 21:08:09 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;
#include "IntVetoDigiHit.h"
#include "IntVetoHit.h"
#include "IntVetoHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t IntVetoHit_factory::init(void)
{
	gPARMS->GetParameter("MC", isMC);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t IntVetoHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// IntVetoHit *myIntVetoHit = new IntVetoHit;
	// myIntVetoHit->x = x;
	// myIntVetoHit->y = y;
	// ...
	// _data.push_back(myIntVetoHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.



	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoDigiHit*> m_IntVetoDigiHit;
	vector <const IntVetoDigiHit*>::const_iterator it;

	IntVetoHit *m_IntVetoHit=0;

	int readout;
	double Q,T;

	//1b: retrieve IntVetoDigiHit objects

	/*This is very important!! Select - or not - the MC case*/
	if (isMC){
		loop->Get(m_IntVetoDigiHit,"MC");
	}
	else{
		loop->Get(m_IntVetoDigiHit);
	}

	for (it=m_IntVetoDigiHit.begin();it!=m_IntVetoDigiHit.end();it++){
		m_IntVetoHit=new IntVetoHit();
		m_IntVetoHit->m_channel = (*it)->m_channel;

		//Do whatever you need here

		_data.push_back(m_IntVetoHit);
	}


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t IntVetoHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t IntVetoHit_factory::fini(void)
{
	return NOERROR;
}

