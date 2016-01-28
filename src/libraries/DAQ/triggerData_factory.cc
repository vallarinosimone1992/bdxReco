// $Id$
//
//    File: triggerData_factory.cc
// Created: Thu Jan 28 18:19:51 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "triggerData_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t triggerData_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t triggerData_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t triggerData_factory::evnt(JEventLoop *loop, int eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// triggerData *mytriggerData = new triggerData;
	// mytriggerData->x = x;
	// mytriggerData->y = y;
	// ...
	// _data.push_back(mytriggerData);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t triggerData_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t triggerData_factory::fini(void)
{
	return NOERROR;
}

