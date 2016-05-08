// $Id$
//
//    File: JEventProcessor_MCtest.h
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _MCtest_
#define _MCtest_

#include <JANA/JEventProcessor.h>

class MCtest:public jana::JEventProcessor{
	public:
		MCtest();
		~MCtest();
		const char* className(void){return "MCtest";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;

       double E1,T1, E2,T2;
      // double Ef, Tf, Eb, Tb;

	//   double Q,pedestal, T, phe;

			int component,eventN, Tw, sector,x,y;
	    double A[100];
        int time[100];

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};




#endif // _MCtest_

