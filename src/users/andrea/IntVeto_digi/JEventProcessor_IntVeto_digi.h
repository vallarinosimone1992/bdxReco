// $Id$
//
//    File: JEventProcessor_intVeto_digi.h
// Created: Tue Feb  9 16:51:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_IntVeto_digi_
#define _JEventProcessor_IntVeto_digi_

#include <JANA/JEventProcessor.h>
#include <TT/TranslationTable.h>
class TTree;
class JROOTOutput;


class JEventProcessor_IntVeto_digi:public jana::JEventProcessor{
public:
	JEventProcessor_IntVeto_digi();
	~JEventProcessor_IntVeto_digi();
	const char* className(void){return "JEventProcessor_IntVeto_digi";}

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
	double Q0,Q0_1,Q0_2,Q0_3,Q0_4; //up
	double Q1,Q1_1,Q1_2,Q1_3,Q1_4; //left
	double Q2,Q2_1,Q2_2,Q2_3,Q2_4; //right
	double Q3,Q3_1,Q3_2,Q3_3,Q3_4; //down
	double Q4,Q5; //plates

	double Qc1,Qc2,Ep1,Ep2; //crystal and paddles

	int tWord;

	JROOTOutput *m_ROOTOutput;

};

#endif // _JEventProcessor_intVeto_digi_

