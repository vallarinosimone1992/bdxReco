// $Id$
//
//    File: JEventProcessor_CalorimeterHits.h
// Created: Sat Feb 24 13:11:49 CET 2018
// Creator: celentan (on Darwin celentano-macbook 17.4.0 i386)
//

#ifndef _JEventProcessor_CalorimeterHits_
#define _JEventProcessor_CalorimeterHits_

#include <JANA/JEventProcessor.h>
class TH1D;
class TH2D;
class JEventProcessor_CalorimeterHits: public jana::JEventProcessor {
public:
	JEventProcessor_CalorimeterHits();
	~JEventProcessor_CalorimeterHits();
	const char* className(void) {
		return "JEventProcessor_CalorimeterHits";
	}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, uint32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	static const int rndmTrgBitID = 31;
	int m_sector, m_X, m_Y;
	double E,T;

	TH1D *hEhit_allTrg;
	TH1D *hEhit_Trg0;
	TH1D *hEhit_Trg1;
	TH1D *hEhit_Trg2;
	TH1D *hEhit_Trg3;
	TH1D *hEhit_Trg4;

	TH1D *hEhit_rndmTrg;
};

#endif // _JEventProcessor_CalorimeterHits_

