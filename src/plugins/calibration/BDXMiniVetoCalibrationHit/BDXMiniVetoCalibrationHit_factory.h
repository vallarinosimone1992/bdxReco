// $Id$
//
//    File: BDXMiniVetoCalibrationHit_factory.h
// Created: Fri May 17 11:56:57 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _BDXMiniVetoCalibrationHit_factory_
#define _BDXMiniVetoCalibrationHit_factory_

#include <JANA/JFactory.h>
#include "BDXMiniVetoCalibrationHit.h"

class BDXMiniVetoCalibrationHit_factory:public jana::JFactory<BDXMiniVetoCalibrationHit>{
	public:
		BDXMiniVetoCalibrationHit_factory(){};
		~BDXMiniVetoCalibrationHit_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isMC;
		double m_thrIntVetoCaps;
		double m_thrIntVetoL;

};

#endif // _BDXMiniVetoCalibrationHit_factory_

