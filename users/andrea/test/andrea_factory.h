// $Id$
//
//    File: andrea_factory.h
// Created: Mon Dec 28 16:28:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _andrea_factory_
#define _andrea_factory_

#include <JANA/JFactory.h>
#include "andrea.h"

class andrea_factory:public jana::JFactory<andrea>{
	public:
		andrea_factory(){};
		~andrea_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _andrea_factory_

