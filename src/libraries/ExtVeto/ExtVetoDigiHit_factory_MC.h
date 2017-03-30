// $Id$
//
//    File: ExtVetoDigiHit_factory_MC.h
// Created: Tue Mar  8 12:19:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetoDigiHit_factory_MC_
#define _ExtVetoDigiHit_factory_MC_

#include <JANA/JFactory.h>

#include <map>
#include <utility>

class ExtVetoDigiHit;

class ExtVetoDigiHit_factory_MC:public jana::JFactory<ExtVetoDigiHit>{
public:
	ExtVetoDigiHit_factory_MC(){};
	~ExtVetoDigiHit_factory_MC(){};
	const char* Tag(void){return "MC";}
	int getCataniaV1Component(int MCchannel);
	int getCataniaV2Component(int MCchannel);
	int getFullV1Component(int MCchannel);
	int getFullV2Component(int MCchannel);


private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	/*This code is here because MC could generate more than 1 hit per sector!*/
	std::map<TranslationTable::EXT_VETO_Index_t,vector<ExtVetoDigiHit*>> m_map;
	std::map<TranslationTable::EXT_VETO_Index_t,vector<ExtVetoDigiHit*>>::iterator m_map_it;
	TranslationTable::EXT_VETO_Index_t m_channel;
	int m_isMC;
};

#endif // _ExtVetoDigiHit_factory_MC_

