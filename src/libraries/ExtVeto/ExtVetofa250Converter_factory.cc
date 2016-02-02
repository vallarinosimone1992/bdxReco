// $Id$
//
//    File: ExtVetofa250Converter_factory.cc
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetofa250Converter_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetofa250Converter_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetofa250Converter_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	if (m_isFirstCallToBrun){
			m_isFirstCallToBrun=0;
			m_extVetofa250Converter=new ExtVetofa250Converter();
			_data.push_back(m_extVetofa250Converter);
			SetFactoryFlag(PERSISTANT);
	}
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetofa250Converter_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// ExtVetofa250Converter *myExtVetofa250Converter = new ExtVetofa250Converter;
	// myExtVetofa250Converter->x = x;
	// myExtVetofa250Converter->y = y;
	// ...
	// _data.push_back(myExtVetofa250Converter);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetofa250Converter_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetofa250Converter_factory::fini(void)
{
	if (m_extVetofa250Converter){
		delete m_extVetofa250Converter;
	}
	_data.clear();
	return NOERROR;
}

