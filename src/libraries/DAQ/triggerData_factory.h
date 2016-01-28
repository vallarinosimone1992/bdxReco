// $Id$
//
//    File: triggerData_factory.h
// Created: Thu Jan 28 18:19:51 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _triggerData_factory_
#define _triggerData_factory_

#include <JANA/JFactory.h>
#include "triggerData.h"

class triggerData_factory:public jana::JFactory<triggerData>{
	public:
		triggerData_factory(){};
		~triggerData_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _triggerData_factory_

