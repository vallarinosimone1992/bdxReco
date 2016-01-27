// $Id$
//
//    File: CalorimeterHit_factory.cc
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterHit_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterHit_factory::evnt(JEventLoop *loop, int eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// CalorimeterHit *myCalorimeterHit = new CalorimeterHit;
	// myCalorimeterHit->x = x;
	// myCalorimeterHit->y = y;
	// ...
	// _data.push_back(myCalorimeterHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterHit_factory::fini(void)
{
	return NOERROR;
}

