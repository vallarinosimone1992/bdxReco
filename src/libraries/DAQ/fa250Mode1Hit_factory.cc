// $Id$
//
//    File: fa250Mode1Hit_factory.cc
// Created: Mon Dec 28 11:04:04 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "fa250Mode1Hit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t fa250Mode1Hit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t fa250Mode1Hit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t fa250Mode1Hit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// fa250Mode1Hit *myfa250Mode1Hit = new fa250Mode1Hit;
	// myfa250Mode1Hit->x = x;
	// myfa250Mode1Hit->y = y;
	// ...
	// _data.push_back(myfa250Mode1Hit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t fa250Mode1Hit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t fa250Mode1Hit_factory::fini(void)
{
	return NOERROR;
}

