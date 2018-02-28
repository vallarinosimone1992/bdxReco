// $Id$
//
//    File: JEventProcessor_JLabFlux.h
// Created: Wed Feb 28 16:11:24 EST 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#ifndef _JEventProcessor_JLabFlux_
#define _JEventProcessor_JLabFlux_

#include <JANA/JEventProcessor.h>

class JEventProcessor_JLabFlux:public jana::JEventProcessor{
	public:
		JEventProcessor_JLabFlux();
		~JEventProcessor_JLabFlux();
		const char* className(void){return "JEventProcessor_JLabFlux";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, uint32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _JEventProcessor_JLabFlux_

