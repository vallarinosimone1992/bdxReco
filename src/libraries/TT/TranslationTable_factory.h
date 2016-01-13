// $Id$
//
//    File: TranslationTable_factory.h
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TranslationTable_factory_
#define _TranslationTable_factory_

#include <JANA/JFactory.h>
#include "TranslationTable.h"

class TranslationTable_factory:public jana::JFactory<TranslationTable>{
	public:
		TranslationTable_factory(){};
		~TranslationTable_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.


		TranslationTable *tt;
};

#endif // _TranslationTable_factory_

