// $Id$
//
//    File: ExtVetofa250Converter_factory.h
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Paddlesfa250Converter_factory_
#define _Paddlesfa250Converter_factory_

#include <JANA/JFactory.h>
#include "Paddlesfa250Converter.h"

class Paddlesfa250Converter_factory:public jana::JFactory<Paddlesfa250Converter>{
	public:
		Paddlesfa250Converter_factory():m_isFirstCallToBrun(1),m_Paddlesfa250Converter(0){};
		~Paddlesfa250Converter_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;
		Paddlesfa250Converter *m_Paddlesfa250Converter;
};

#endif // _Paddlesfa250Converter_factory_

