// $Id$
//
//    File: CalorimeterDigiHit_factory_MC.h
// Created: Tue Mar  1 17:38:32 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CalorimeterDigiHit_factory_MC_
#define _CalorimeterDigiHit_factory_MC_

#include <JANA/JFactory.h>
#include "CalorimeterDigiHit.h"

class CalorimeterDigiHit_factory_MC:public jana::JFactory<CalorimeterDigiHit>{
	public:
		CalorimeterDigiHit_factory_MC(){};
		~CalorimeterDigiHit_factory_MC(){};
		const char* Tag(void){return "MC";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _CalorimeterDigiHit_factory_MC_

