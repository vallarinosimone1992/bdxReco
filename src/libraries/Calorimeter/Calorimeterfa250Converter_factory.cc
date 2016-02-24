// $Id$
//
//    File: Calorimeterfa250Converter_factory.cc
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>

#include "TString.h"
using namespace std;

#include "Calorimeterfa250Converter_factory.h"
using namespace jana;


int PThreadIDUniqueInt(pthread_t tid){
	int ret;
	memcpy(&ret, &tid, std::min(sizeof(tid), sizeof(ret)));
	return ret;
}

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
	int threadId= PThreadIDUniqueInt(eventLoop->GetPThreadID());
	m_calorimeterfa250Converter=new Calorimeterfa250Converter();
	m_calorimeterfa250Converter->name()=string(Form("h%i",threadId));

	m_calorimeterfa250Converter->NPED=10; //A.C. for now, hardcoded

	m_calorimeterfa250Converter->SINGLE_SIGNAL_TOT=40; //A.C. for now, hardcoded. These two are higly hardcoded!
	m_calorimeterfa250Converter->MIN_TOT=3; //A.C. for now, hardcoded. These two are higly hardcoded!



	gPARMS->GetParameter("CALORIMETER:VERBOSE",	m_calorimeterfa250Converter->verbose());

	/*Probably not the best way to do so: the calorimeter converter needs to know about the pedestal,
	 * so read cal. constants from DB, create a CalorimeterCalibration object that handles properly the indexing
	 * do so every run, since cal. constants may change.
	 * In erun, consequently, delete the converter
	 */
	m_calorimeterfa250Converter->pedestal=new CalibrationHandler<TranslationTable::CALO_Index_t>;
	vector<vector < double> > m_rawpedestal;
	eventLoop->GetCalib("/Calorimeter/pedestal", m_rawpedestal);
	m_calorimeterfa250Converter->pedestal->fillCalib(m_rawpedestal);


	_data.push_back(m_calorimeterfa250Converter);
	SetFactoryFlag(PERSISTANT);

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
	if (m_calorimeterfa250Converter) delete m_calorimeterfa250Converter;
	_data.clear();
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
}

