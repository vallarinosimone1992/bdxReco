// $Id$
//
//    File: JEventProcessor_test.h
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Paddles_ene_
#define _Paddles_ene_

#include <JANA/JEventProcessor.h>

class TTree;
class JROOTOutput;

class Paddles_ene:public jana::JEventProcessor{
	public:
		Paddles_ene();
		~Paddles_ene();
		const char* className(void){return "Paddles_ene";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;
		TTree *t;
		JROOTOutput *m_ROOTOutput;

		double E;
		double T;
		int id,eventN,tword;
		double Nphe;



};

#endif // _Paddles_Ene_

