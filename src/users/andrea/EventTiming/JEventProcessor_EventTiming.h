// $Id$
//
//    File: JEventProcessor_EventTiming.h
// Created: Wed Mar 16 18:23:05 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_EventTiming_
#define _JEventProcessor_EventTiming_

#include <JANA/JEventProcessor.h>
class JROOTOutput;
class TTree;
class JEventProcessor_EventTiming:public jana::JEventProcessor{
	public:
		JEventProcessor_EventTiming();
		~JEventProcessor_EventTiming();
		const char* className(void){return "JEventProcessor_EventTiming";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);		///< Called after last event of last event source has been processed.


		int eventNumber;
		int eventTime;
		int m_isFirstCallToBrun;
		JROOTOutput *m_ROOTOutput;
		TTree *t;
};

#endif // _JEventProcessor_EventTiming_

