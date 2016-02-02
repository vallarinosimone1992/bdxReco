// $Id$
//
//    File: ctofHit_factory.h
// Created: Thu Dec 17 14:09:30 EST 2015
// Creator: ungaro (on Darwin MauriMP.local 14.5.0 i386)
//

#ifndef _ctofHitR_factory_
#define _ctofHitR_factory_

#include <JANA/JFactory.h>
#include "ctofHitR.h"

class ctofHitR_factory:public jana::JFactory<ctofHitR>{
	public:
		ctofHitR_factory(){};
		~ctofHitR_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif

