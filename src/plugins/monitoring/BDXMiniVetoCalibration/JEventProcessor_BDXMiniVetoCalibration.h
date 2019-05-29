// $Id$
//
//    File: JEventProcessor_BDXMiniVetoCalibration.h
// Created: Fri May 17 12:22:59 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _JEventProcessor_BDXMiniVetoCalibration_
#define _JEventProcessor_BDXMiniVetoCalibration_

#include <JANA/JEventProcessor.h>

class JROOTOutput;

class JEventProcessor_BDXMiniVetoCalibration: public jana::JEventProcessor {
public:
	JEventProcessor_BDXMiniVetoCalibration();
	~JEventProcessor_BDXMiniVetoCalibration();
	const char* className(void) {
		return "JEventProcessor_BDXMiniVetoCalibration";
	}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	int m_isFirstCallToBrun;
	JROOTOutput *m_ROOTOutput;
	int m_isMC;

};

#endif // _JEventProcessor_BDXMiniVetoCalibration_

