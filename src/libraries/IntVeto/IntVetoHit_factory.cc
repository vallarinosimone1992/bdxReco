// $Id$
//
//    File: IntVetoHit_factory.cc
// Created: Wed Jan 13 21:08:09 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "IntVetoHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t IntVetoHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t IntVetoHit_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoHit_factory::evnt(JEventLoop *loop, int eventnumber)
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

