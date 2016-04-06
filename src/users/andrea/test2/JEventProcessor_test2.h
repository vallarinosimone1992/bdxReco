// $Id$
//
//    File: JEventProcessor_test2.h
// Created: Tue Mar  1 01:15:03 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_test2_
#define _JEventProcessor_test2_

#include <JANA/JEventProcessor.h>

class JEventProcessor_test2:public jana::JEventProcessor{
	public:
		JEventProcessor_test2();
		~JEventProcessor_test2();
		const char* className(void){return "JEventProcessor_test2";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _JEventProcessor_test2_

