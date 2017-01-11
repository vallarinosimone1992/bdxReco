// $Id$
//
//    File: ExtVetoDigiHit_factory.h
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoDigiHit_factory_
#define _ExtVetoDigiHit_factory_

#include <JANA/JFactory.h>
#include "ExtVetoDigiHit.h"
class TranslationTable;
class ExtVetoDigiHit_factory:public jana::JFactory<ExtVetoDigiHit>{
public:
	ExtVetoDigiHit_factory(){};
	~ExtVetoDigiHit_factory(){};


private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	const TranslationTable *m_tt;

	std::map<TranslationTable::EXT_VETO_Index_t,ExtVetoDigiHit*> m_map;
	std::map<TranslationTable::EXT_VETO_Index_t,ExtVetoDigiHit*>::iterator m_map_it;
};

#endif // _ExtVetoDigiHit_factory_

