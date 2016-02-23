// $Id$
//
//    File: CalorimeterSiPMHit_factory.h
// Created: Wed Jan 27 16:39:49 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterSiPMHit_factory_
#define _CalorimeterSiPMHit_factory_

#include <JANA/JFactory.h>
#include "CalorimeterSiPMHit.h"
#include <system/CalibrationHandler.h>
#include <TT/TranslationTable.h>
class Calorimeterfa250Converter;

class CalorimeterSiPMHit_factory:public jana::JFactory<CalorimeterSiPMHit>{
	public:
		CalorimeterSiPMHit_factory(){};
		~CalorimeterSiPMHit_factory(){};


	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		const TranslationTable *m_tt;
		const Calorimeterfa250Converter *m_Calorimeterfa250Converter;

		CalibrationHandler<TranslationTable::CALO_Index_t> m_sipm_gain;
		int VERBOSE;
};

#endif // _CalorimeterSiPMHit_factory_

