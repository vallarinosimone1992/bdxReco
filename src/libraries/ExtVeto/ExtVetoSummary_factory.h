// $Id$
//
//    File: ExtVetoSummary_factory.h
// Created: Fri Mar 11 16:16:37 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoSummary_factory_
#define _ExtVetoSummary_factory_

#include <JANA/JFactory.h>
#include "ExtVetoSummary.h"
#include <system/CalibrationHandler.h>

class ExtVetoSummary_factory:public jana::JFactory<ExtVetoSummary>{
	public:
		ExtVetoSummary_factory(){};
		~ExtVetoSummary_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		std::map<int,ExtVetoSummary*> m_map;
		std::map<int,ExtVetoSummary*>::iterator m_it;
};

#endif // _ExtVetoSummary_factory_

