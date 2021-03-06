// $Id$
//
//    File: BDXMiniVetoSIPMCalibrationHit_factory.h
// Created: Mon Aug 19 18:11:03 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _BDXMiniVetoSIPMCalibrationHit_factory_
#define _BDXMiniVetoSIPMCalibrationHit_factory_

#include <JANA/JFactory.h>
#include "BDXMiniVetoSIPMCalibrationHit.h"
#include <TT/TranslationTable.h>

class BDXMiniVetoSIPMCalibrationHit_factory: public jana::JFactory<BDXMiniVetoSIPMCalibrationHit> {
public:
	BDXMiniVetoSIPMCalibrationHit_factory() {
	}
	;
	~BDXMiniVetoSIPMCalibrationHit_factory() {
	}
	;

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	int m_isMC;
	double m_Tmin,m_Tmax;


};

#endif // _BDXMiniVetoSIPMCalibrationHit_factory_

