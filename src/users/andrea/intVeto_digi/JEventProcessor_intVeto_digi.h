// $Id$
//
//    File: JEventProcessor_intVeto_digi.h
// Created: Tue Feb  9 16:51:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_intVeto_digi_
#define _JEventProcessor_intVeto_digi_

#include <JANA/JEventProcessor.h>
#include <TT/TranslationTable.h>
class TTree;
class JROOTOutput;


class JEventProcessor_intVeto_digi:public jana::JEventProcessor{
public:
	JEventProcessor_intVeto_digi();
	~JEventProcessor_intVeto_digi();
	const char* className(void){return "JEventProcessor_intVeto_digi";}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	TTree *t;

	int eventNumber;
	int m_isFirstCallToBrun;

	int m_sector,m_layer,m_component,m_readout;
	double Q,Q1,Q2,Q3,Q4,T;


	JROOTOutput *m_ROOTOutput;

};

#endif // _JEventProcessor_intVeto_digi_

