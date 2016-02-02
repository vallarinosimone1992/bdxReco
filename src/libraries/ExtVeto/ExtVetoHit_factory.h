// $Id$
//
//    File: ExtVetoHit_factory.h
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoHit_factory_
#define _ExtVetoHit_factory_

#include <JANA/JFactory.h>
#include "ExtVetoHit.h"

class ExtVetoHit_factory:public jana::JFactory<ExtVetoHit>{
	public:
		ExtVetoHit_factory(){};
		~ExtVetoHit_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _ExtVetoHit_factory_

