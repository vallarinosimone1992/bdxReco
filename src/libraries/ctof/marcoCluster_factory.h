// $Id$
//
//    File: marcoCluster_factory.h
// Created: Thu Dec 17 16:37:25 EST 2015
// Creator: ungaro (on Darwin MauriMP.local 14.5.0 i386)
//

#ifndef _marcoCluster_factory_
#define _marcoCluster_factory_

#include <JANA/JFactory.h>
#include "marcoCluster.h"

class marcoCluster_factory:public jana::JFactory<marcoCluster>{
	public:
		marcoCluster_factory(){};
		~marcoCluster_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _marcoCluster_factory_

