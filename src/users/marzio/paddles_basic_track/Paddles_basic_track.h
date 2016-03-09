// $Id$
//
//    File: JEventProcessor_test.h
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Paddles_basic_track_
#define _Paddles_basic_track_

#include <JANA/JEventProcessor.h>

class TTree;
class JROOTOutput;

class Paddles_basic_track:public jana::JEventProcessor{
	public:
		Paddles_basic_track();
		~Paddles_basic_track();
		const char* className(void){return "Paddles_basic_track";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;
		TTree *t;
		JROOTOutput *m_ROOTOutput;

		static const int time_window=100;  // Samples

		double E_up,E_down,E6,E9;
		double T_up,T_down,T6,T9;
		int id,eventN,tword,mult,component;

		double amp[time_window];
		int time[time_window];};

#endif // _Paddles_basic_track_

