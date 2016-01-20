// $Id$
//
//    File: ExtVetoDigiHit_factory.h
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoDigiHit_factory_
#define _ExtVetoDigiHit_factory_

#include <JANA/JFactory.h>
#include "ExtVetoPMTHit.h"

class TranslationTable;
class ExtVetoPMTHit_factory:public jana::JFactory<ExtVetoPMTHit>{
	public:
		ExtVetoPMTHit_factory():m_tt(0){};
		~ExtVetoPMTHit_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		const TranslationTable *m_tt;
};

#endif // _ExtVetoDigiHit_factory_

