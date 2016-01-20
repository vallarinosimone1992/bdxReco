// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoDigiHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoDigiHit_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory::evnt(JEventLoop *loop, int eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// ExtVetoDigiHit *myExtVetoDigiHit = new ExtVetoDigiHit;
	// myExtVetoDigiHit->x = x;
	// myExtVetoDigiHit->y = y;
	// ...
	// _data.push_back(myExtVetoDigiHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoDigiHit_factory::fini(void)
{
	return NOERROR;
}

