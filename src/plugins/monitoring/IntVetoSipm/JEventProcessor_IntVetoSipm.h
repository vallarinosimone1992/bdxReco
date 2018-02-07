// $Id$
//
//    File: JEventProcessor_IntVetoSipm.h
// Created: Tue Feb  6 19:17:53 CET 2018
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_IntVetoSipm_
#define _JEventProcessor_IntVetoSipm_

#include <JANA/JEventProcessor.h>
#include <system/CalibrationHandler.h>

class TH1D;

class JEventProcessor_IntVetoSipm: public jana::JEventProcessor {
public:
	JEventProcessor_IntVetoSipm();
	~JEventProcessor_IntVetoSipm();
	const char* className(void) {
		return "JEventProcessor_IntVetoSipm";
	}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, uint32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.


	int m_sector, m_layer, m_component, m_readout, m_type;
	double Qphe,Qraw;
	double Aphe,Araw;
	double T;

	CalibrationHandler<TranslationTable::INT_VETO_Index_t> *m_sipm_ampl;

	TH1D *m_hSipmQ;
	TH1D *m_hSipmA;
};

#endif // _JEventProcessor_IntVetoSipm_

