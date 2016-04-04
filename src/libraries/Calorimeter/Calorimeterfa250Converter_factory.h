// $Id$
//
//    File: Calorimeterfa250Converter_factory.h
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Calorimeterfa250Converter_factory_
#define _Calorimeterfa250Converter_factory_

#include <JANA/JFactory.h>
#include "Calorimeterfa250Converter.h"

class Calorimeterfa250Converter_factory:public jana::JFactory<Calorimeterfa250Converter>{
	public:
		Calorimeterfa250Converter_factory();
		~Calorimeterfa250Converter_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//int m_isFirstCallToBrun;
		Calorimeterfa250Converter *m_calorimeterfa250Converter;

		double m_SINGLE_SIGNAL_TOT;
		double m_MIN_TOT;
		double m_THR;
		int m_NSB;
		int m_NSA;
		int m_NPED;

};

#endif // _Calorimeterfa250Converter_factory_

