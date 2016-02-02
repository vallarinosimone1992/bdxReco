// $Id$
//
//    File: VetoIntDigiHit_factory.h
// Created: Tue Jan 12 11:52:41 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _VetoIntSIPMHit_factory_
#define _VetoIntSIPMHit_factory_

#include <JANA/JFactory.h>

#include "IntVetoSiPMHit.h"
#include "IntVetoCalibration.h"

class IntVetofa250Converter;




class IntVetoSiPMHit_factory:public jana::JFactory<IntVetoSiPMHit>{
	public:
		IntVetoSiPMHit_factory():m_tt(0){};
		~IntVetoSiPMHit_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		const TranslationTable *m_tt;
		const IntVetofa250Converter *m_intVetofa250Converter;

		IntVetoCalibration m_sipm_gain;

};

#endif // _VetoIntDigiHit_factory_

