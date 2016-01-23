// $Id$
//
//    File: FadcConverter_factory.cc
// Created: Wed Jan 20 19:27:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "FadcConverter_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t FadcConverter_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t FadcConverter_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t FadcConverter_factory::evnt(JEventLoop *loop, int eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// FadcConverter *myFadcConverter = new FadcConverter;
	// myFadcConverter->x = x;
	// myFadcConverter->y = y;
	// ...
	// _data.push_back(myFadcConverter);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t FadcConverter_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t FadcConverter_factory::fini(void)
{
	return NOERROR;
}

