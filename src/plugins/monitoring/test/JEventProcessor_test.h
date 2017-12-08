// $Id$
//
//    File: JEventProcessor_test.h
// Created: Mon Nov 13 08:49:26 CET 2017
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_test_
#define _JEventProcessor_test_

#include <JANA/JEventProcessor.h>

class JEventProcessor_test:public jana::JEventProcessor{
	public:
		JEventProcessor_test();
		~JEventProcessor_test();
		const char* className(void){return "JEventProcessor_test";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _JEventProcessor_test_

