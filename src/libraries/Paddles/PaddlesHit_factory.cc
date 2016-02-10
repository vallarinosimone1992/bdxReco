// $Id$
//
//    File: ExtVetoHit_factory.cc
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include <Paddles/PaddlesHit_factory.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t PaddlesHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t PaddlesHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t PaddlesHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// ExtVetoHit *myExtVetoHit = new ExtVetoHit;
	// myExtVetoHit->x = x;
	// myExtVetoHit->y = y;
	// ...
	// _data.push_back(myExtVetoHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t PaddlesHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t PaddlesHit_factory::fini(void)
{
	return NOERROR;
}

