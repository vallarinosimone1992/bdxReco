// $Id$
//
//    File: JEventProcessor_MC_CT_test.h
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _MC_CT_test_
#define _MC_CT_test_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;


class MC_CT_test:public jana::JEventProcessor{
	public:
		MC_CT_test();
		~MC_CT_test();
		const char* className(void){return "MC_CT_test";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;

	  double Ecrys;
      double  multi_iv, multi_ev, multi_iv_coinc, multi_ev_coinc;

      double Ene_ev[99],Ene_ev_MC[99];
      int channel_ev[99],channel_ev_MC[99];


	  int component,eventN, Tw, sector,x,y;


        TTree *t;
        		JROOTOutput *m_ROOTOutput;

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};




#endif // _MC_CT_test_

