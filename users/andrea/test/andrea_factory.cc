// $Id$
//
//    File: andrea_factory.cc
// Created: Mon Dec 28 16:28:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "andrea_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t andrea_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t andrea_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t andrea_factory::evnt(JEventLoop *loop, int eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// andrea *myandrea = new andrea;
	// myandrea->x = x;
	// myandrea->y = y;
	// ...
	// _data.push_back(myandrea);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t andrea_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t andrea_factory::fini(void)
{
	return NOERROR;
}

