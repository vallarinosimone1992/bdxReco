// $Id$
//
//    File: ExtVetoHit_factory.h
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _PaddlesHit_factory_
#define _PaddlesHit_factory_

#include <JANA/JFactory.h>
#include <Paddles/PaddlesHit.h>
#include <system/CalibrationHandler.h>
#include <system/BDXFactory.h>

class PaddlesHit_factory:public BDXFactory<PaddlesHit>{
	public:
		PaddlesHit_factory():m_ENE_gain(0),m_tt(0){isMC=0;};
		~PaddlesHit_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		const TranslationTable *m_tt;
		int isMC;
		CalibrationHandler<TranslationTable::PADDLES_Index_t> *m_ENE_gain;

};

#endif // _PaddlesHit_factory_

