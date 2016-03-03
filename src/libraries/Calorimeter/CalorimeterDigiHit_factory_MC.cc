// $Id$
//
//    File: CalorimeterDigiHit_factory_MC.cc
// Created: Tue Mar  1 17:38:32 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterDigiHit_factory_MC.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterDigiHit_factory_MC::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// CalorimeterDigiHit *myCalorimeterDigiHit = new CalorimeterDigiHit;
	// myCalorimeterDigiHit->x = x;
	// myCalorimeterDigiHit->y = y;
	// ...
	// _data.push_back(myCalorimeterDigiHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterDigiHit_factory_MC::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterDigiHit_factory_MC::fini(void)
{
	return NOERROR;
}

