// $Id$
//
//    File: Calorimeterfa250Converter_factory.cc
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "Calorimeterfa250Converter_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t Calorimeterfa250Converter_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t Calorimeterfa250Converter_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	if (m_isFirstCallToBrun){
			m_isFirstCallToBrun=0;
			m_calorimeterfa250Converter=new Calorimeterfa250Converter();
			_data.push_back(m_calorimeterfa250Converter);
			SetFactoryFlag(PERSISTANT);
		}
	return NOERROR;
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t Calorimeterfa250Converter_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// Calorimeterfa250Converter *myCalorimeterfa250Converter = new Calorimeterfa250Converter;
	// myCalorimeterfa250Converter->x = x;
	// myCalorimeterfa250Converter->y = y;
	// ...
	// _data.push_back(myCalorimeterfa250Converter);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t Calorimeterfa250Converter_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t Calorimeterfa250Converter_factory::fini(void)
{
	if (m_calorimeterfa250Converter){
		delete m_calorimeterfa250Converter;
	}
	_data.clear();
	return NOERROR;
	return NOERROR;
}

