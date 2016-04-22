// $Id$
//
//    File: IntVetoDigiHit_factory_MC.h
// Created: Mon Mar  7 12:20:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoDigiHit_factory_MC_
#define _IntVetoDigiHit_factory_MC_

#include <JANA/JFactory.h>
#include "IntVetoDigiHit.h"
#include <map>
#include <utility>
#include <algorithm>

class IntVetoMCHit;

class IntVetoDigiHit_factory_MC:public jana::JFactory<IntVetoDigiHit>{
	public:
		IntVetoDigiHit_factory_MC(){};
		~IntVetoDigiHit_factory_MC(){};
		const char* Tag(void){return "MC";}

		int getComponent(int MCchannel);
	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		/*This code is here because MC could generate more than 1 hit per sector!*/
		/*The key is the way MC is organized: sector - channel (where sector and channel have a different meaning than in the real data!!!)*/
		std::map<std::pair<int,int>,IntVetoDigiHit*> m_map;
		std::map<std::pair<int,int>,IntVetoDigiHit*>::iterator m_map_it;
};

#endif // _IntVetoDigiHit_factory_MC_

