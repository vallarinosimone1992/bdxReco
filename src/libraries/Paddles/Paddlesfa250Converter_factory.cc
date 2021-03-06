// $Id$
//
//    File: ExtVetofa250Converter_factory.cc
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include <Paddles/Paddlesfa250Converter_factory.h>
#include <system/CalibrationHandler.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t Paddlesfa250Converter_factory::init(void)
{
	m_isFirstCallToBrun=1;
	m_Paddlesfa250Converter=new Paddlesfa250Converter();

	m_Paddlesfa250Converter->threshold=new CalibrationHandler<TranslationTable::PADDLES_Index_t>("/Paddles/Threshold");
	this->mapCalibrationHandler(m_Paddlesfa250Converter->threshold);

	m_Paddlesfa250Converter->m_pedestals=new DAQCalibrationHandler("/DAQ/pedestals");
	this->mapCalibrationHandler(m_Paddlesfa250Converter->m_pedestals);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t Paddlesfa250Converter_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{


	this->updateCalibrationHandler(m_Paddlesfa250Converter->threshold,eventLoop);
	this->updateCalibrationHandler(m_Paddlesfa250Converter->m_pedestals,eventLoop);

	if (m_isFirstCallToBrun){
			m_isFirstCallToBrun=0;
			_data.push_back(m_Paddlesfa250Converter);
			SetFactoryFlag(PERSISTANT);
		}





	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t Paddlesfa250Converter_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
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
jerror_t Paddlesfa250Converter_factory::erun(void)
{

	this->clearCalibrationHandler(m_Paddlesfa250Converter->threshold);
	this->clearCalibrationHandler(m_Paddlesfa250Converter->m_pedestals);


	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t Paddlesfa250Converter_factory::fini(void)
{
	_data.clear();
	return NOERROR;
}

