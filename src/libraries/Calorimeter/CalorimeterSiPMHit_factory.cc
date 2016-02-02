// $Id$
//
//    File: CalorimeterSiPMHit_factory.cc
// Created: Wed Jan 27 16:39:49 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterSiPMHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterSiPMHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterSiPMHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterSiPMHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// CalorimeterSiPMHit *myCalorimeterSiPMHit = new CalorimeterSiPMHit;
	// myCalorimeterSiPMHit->x = x;
	// myCalorimeterSiPMHit->y = y;
	// ...
	// _data.push_back(myCalorimeterSiPMHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterSiPMHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterSiPMHit_factory::fini(void)
{
	return NOERROR;
}

