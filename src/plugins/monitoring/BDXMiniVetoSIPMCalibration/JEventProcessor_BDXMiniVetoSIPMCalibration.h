// $Id$
//
//    File: JEventProcessor_BDXMiniVetoSIPMCalibration.h
// Created: Tue Aug 20 15:47:25 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _JEventProcessor_BDXMiniVetoSIPMCalibration_
#define _JEventProcessor_BDXMiniVetoSIPMCalibration_

#include <JANA/JEventProcessor.h>
#include <map>
#include "TH1D.h"

class JROOTOutput;
class JEventProcessor_BDXMiniVetoSIPMCalibration: public jana::JEventProcessor {
public:
	JEventProcessor_BDXMiniVetoSIPMCalibration();
	~JEventProcessor_BDXMiniVetoSIPMCalibration();
	const char* className(void) {
		return "JEventProcessor_BDXMiniVetoSIPMCalibration";
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

#endif // _JEventProcessor_BDXMiniVetoSIPMCalibration_

